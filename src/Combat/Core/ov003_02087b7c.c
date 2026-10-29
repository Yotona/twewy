#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/Color.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Save.h"
#include "common_data.h"

#include <nitro/fx.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>

// Battle hit resolution: the attack records' entry points, the damage/knockback maths and the
// hit tests. The attack bookkeeping structures below are file-local because `Combat.h` does not
// model them yet.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 8-byte payload an attack entry point allocates for its on-fire handler.
typedef struct Ov003AttackData {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ CombatActor*      target;
} Ov003AttackData; // Size: 0x8

/// An attack entry point: the handler the battle calls when the attack lands, plus its payload.
typedef struct Ov003AttackCmd {
    /* 0x00 */ void (*fire)(Ov003AttackData*);
    /* 0x04 */ s32              unk_04;
    /* 0x08 */ Ov003AttackData* data;
} Ov003AttackCmd; // Size: 0xC

/// One attack-info record: the base power, the reaction/stun ids, the half-extents the hit test
/// uses and the bit-packed flags (attack-parameter id in bits 19..23). `func_ov003_0208a114`
/// indexes the command table with these.

/// The per-hit arguments of an attack: damage scale percentages and the hit point.

/// An incoming attack: the info record, its arguments and the target point.
typedef struct Ov003Attack {
    /* 0x00 */ const Ov003AttackInfo* info;
    /* 0x04 */ const Ov003AttackArgs* args;
    /* 0x08 */ s32                    flags;
    /* 0x0C */ s32                    unk_0C;
    /* 0x10 */ s32                    x;
    /* 0x14 */ s32                    y;
    /* 0x18 */ s32                    z;
} Ov003Attack; // Size: 0x1C

/// A battle unit (task data): the shared `CombatActor` prefix plus the attack bookkeeping.

/// A queued hit being resolved against one unit, also used as the spawn request that queues it.
typedef struct Ov003Hit {
    /* 0x00 */ const Ov003AttackInfo* info;
    /* 0x04 */ void*                  payload;
    /* 0x08 */ u16                    kind;
    /* 0x0A */ u16                    pad;
    /* 0x0C */ Ov003EnemyActor*       unit;
    /* 0x10 */ s32                    x;
    /* 0x14 */ s32                    y;
    /* 0x18 */ s32                    z;
} Ov003Hit; // Size: 0x1C

/// One record of the per-attack parameter table at 0x020d76d0.
typedef struct Ov003AttackParams {
    /* 0x00 */ u16 unk_00; // variant
    /* 0x02 */ s16 unk_02; // damage scale
    /* 0x04 */ s16 unk_04; // hit chance percentage
    /* 0x06 */ s16 unk_06; // knockback percentage
} Ov003AttackParams;       // Size: 0x8

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern s32  func_ov003_020843b0(s32, s32);
extern s32  func_ov003_020843ec(s32, s32, s32);
extern s32  func_ov003_0208442c(s32, s32);
extern s32  func_ov003_02084460(s32, s32, s32);
extern s32  func_ov003_02085dc8(s32);
extern void func_ov003_02087f28(SndMgrSeIdx, s32);
extern void func_ov003_02087f64(SndMgrSeIdx, s32);
extern s32  func_ov003_0208a128(const Ov003AttackInfo*, void*, void*, s32);
extern s32  func_ov003_0208a164(const Ov003AttackInfo*, Ov003AttackArgs*, s32, s32, s32);
extern void func_ov003_0208b3cc(s32, s16);
extern s32  func_ov003_0208b518(s32, Ov003EnemyActor*, s16);
extern BOOL func_ov003_0208ca04(s32, u32);
extern s16  func_ov003_0208ca24(s32, u32);
extern void func_ov003_0208cb6c(s32, s32);
extern void func_ov003_0209b564(s32, s32, CombatActor*, s32);
extern void func_ov003_0209b61c(s32, s32, Ov003EnemyActor*);
extern void func_ov003_0209c568(s32, u32, s32);
extern void func_ov003_0209c984(s32, s32, s32, s32, s32);
extern void func_ov003_0209ed70(u8, s32, s32, s32, s32);

/// The special attack effect helpers of the other overlays, called through the attack handlers.
extern void SndMgr_StartPlayingSE(s32);
extern void SndMgr_PlaySEWithPan(s32, s32);

/// Attack-info table (indexed by `func_ov003_0208a114`).
extern const Ov003AttackInfo data_ov003_020d422c[];
/// Battle-actor pack used by the effect actors.
extern const BinIdentifier data_ov003_020d7800;

/// Set while the battle is muted (bit 5 of the battle flags).
extern u8 data_ov003_020e71d8;
extern u8 data_ov003_020e71d9;

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

/// Overall damage scaling percentage.
const s16 data_ov003_020d7620[1] = {200};
/// Combo-rank damage bonuses (one per combo rank).
const s16 data_ov003_020d7622[7] = {50, 100, 100, 61, 200, 40, 200};
/// Damage per chained battle, plus the flat cap for later battles.
const s16 data_ov003_020d7630[16] = {100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 300, 300, 300, 300, 300};
/// Knockback strength per attack variant (the second word of each pair lives in `...654`).
const s32 data_ov003_020d7650[1] = {0};
/// Hit-flash/knockback parameters, interleaved with `data_ov003_020d7650`.
const s32 data_ov003_020d7654[31] = {
    0x00000000, 0x00002333, 0x00000000, 0x00004800, 0x00000000, 0x00001000, 0x00000000, 0x00000000,
    0xFFFFD000, 0x00001000, 0x00000000, 0x00001000, 0xFFFFC800, 0x00001800, 0xFFFFB000, 0x00000000,
    0xFFFF9000, 0x00000000, 0xFFFFC000, 0x0000A000, 0x00000000, 0x00000800, 0xFFFFE000, 0x00001000,
    0xFFFFB000, 0x00001000, 0x00000800, 0x00002000, 0xFFFF8000, 0x00001000, 0xFFFFC800,
};
/// Per-attack parameters, indexed by the attack id (`arg2` in `func_ov003_02088b5c`).
const Ov003AttackParams data_ov003_020d76d0[32] = {
    {2, 100,    0,   0},
    {2,  50, 1800, 100},
    {2,  50, 1800,  40},
    {2,  50,  900, 100},
    {2,  50,  900,  40},
    {3, 150,  600, 100},
    {3, 150,  600,  40},
    {3, 150,  300, 100},
    {3, 150,  300,  40},
    {1,   5, 1800,  50},
    {1,   5, 1800,  10},
    {1,   5,  900,  50},
    {1,   5,  900,  10},
    {4,   0,  360,  60},
    {4,   0,  360,  15},
    {4,   0,  180,  60},
    {4,   0,  180,  15},
    {2,  20, 1000, 100},
    {2,  20, 1000,  70},
    {2,  20, 1000,  50},
    {2,  20, 1000,  20},
    {3, 120,  500, 100},
    {3, 120,  500,  70},
    {3, 120,  500,  50},
    {3, 120,  500,  20},
    {1,   5, 1000, 100},
    {1,   5, 1000,  80},
    {1,   5, 1000,  50},
    {1,   5, 1000,  20},
    {4,   0,  360, 100},
    {4,   0,  360,  70},
    {4,   0,  360,  20},
};

char data_ov003_020e391c[0x18] = "Apl_Fur/Grp_BtlTch.bin";
char data_ov003_020e3934[0x10] = "Sword_Pierce_T";
char data_ov003_020e3944[0x14] = "ARG_Sword_Syoryu_S";
char data_ov003_020e3958[0x14] = "ARG_Sword_Kabuto_S";
char data_ov003_020e396c[0x10] = "ARG_Teleport_T";

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

extern Ov003EnemyActor* func_ov003_0208495c(CombatList*);
extern Ov003EnemyActor* func_ov003_02084984(void*);
extern void             func_ov003_02089f90(s32 mode, Ov003EnemyActor* unit, u16 variant, s16 arg3, s16 arg4);
extern s32              func_ov003_0208a3ec(Ov003Hit* spawn);

/// TRUE when the two screen-space positions overlap on the axis `arg0` selects. The four
/// halfwords at `Ov003Global + 0x3D378` are the current and previous touch/focus positions.
// Nonmatching: register allocation of the four cached halfword positions
s32 func_ov003_02087b7c(s32 arg0) {
    s16 curX   = data_ov003_020e71b8->unk3D378;
    s16 curY   = data_ov003_020e71b8->unk3D37A;
    s16 prevX  = data_ov003_020e71b8->unk3D37C;
    s16 prevY  = data_ov003_020e71b8->unk3D37E;
    s32 result = 0;

    switch (arg0) {
        case 0: {
            s32 dx = curY - prevY;
            s32 dy = curX - prevX;

            if (dx < 0) {
                dx = -dx;
            }
            if (dy < 0) {
                dy = -dy;
            }
            if (dy < dx && curY > prevY) {
                result = 1;
            }
            break;
        }
        case 1: {
            s32 dx = curY - prevY;
            s32 dy = curX - prevX;

            if (dx < 0) {
                dx = -dx;
            }
            if (dy < 0) {
                dy = -dy;
            }
            if (dy < dx && curY < prevY) {
                result = 1;
            }
            break;
        }
    }
    return result;
}

void func_ov003_02087c10(Ov003AttackData* data) {
    func_ov003_02087250(data->effect, data->target, 0);
}

/// Registers the "pierce" attack: the handler is `func_ov003_02087c10`.
s32 func_ov003_02087c28(Ov003AttackCmd* cmd, Ov003EffectState* effect) {
    Ov003AttackData* data;
    CombatActor*     state = func_ov003_02085dc8(1);

    if (state == 0) {
        return 0;
    }
    data         = (Ov003AttackData*)func_ov003_020a5d2c(8, data_ov003_020e3934);
    data->effect = effect;
    data->target = state;
    cmd->fire    = func_ov003_02087c10;
    cmd->data    = data;
    return 1;
}

void func_ov003_02087c78(Ov003AttackData* data) {
    func_ov003_02087250(data->effect, data->target, 0);
    func_ov003_020a5a94(data->effect);
}

/// Registers the "slash" attack: the handler is `func_ov003_02087c78`.
s32 func_ov003_02087c98(Ov003AttackCmd* cmd, Ov003EffectState* effect) {
    Ov003AttackData* data;
    CombatActor*     state;

    if (func_ov003_02087b7c(0) == 0) {
        return 0;
    }
    state = func_ov003_02085ec0(1);
    if (state == 0) {
        return 0;
    }
    data         = (Ov003AttackData*)func_ov003_020a5d2c(8, data_ov003_020e3944);
    data->effect = effect;
    data->target = state;
    cmd->fire    = func_ov003_02087c78;
    cmd->data    = data;
    return 1;
}

void func_ov003_02087cfc(Ov003AttackData* data) {
    func_ov003_02087250(data->effect, data->target, 0);
    func_ov003_020a5a94(data->effect);
}

/// Registers the "kabuto" attack: the handler is `func_ov003_02087cfc`.
s32 func_ov003_02087d1c(Ov003AttackCmd* cmd, Ov003EffectState* effect) {
    Ov003AttackData* data;
    CombatActor*     state;

    if (data_ov003_020e71b8->unk3D894 < 0x64) {
        return 0;
    }
    if (func_ov003_02087b7c(1) == 0) {
        return 0;
    }
    state = func_ov003_02085ec0(1);
    if (state == 0) {
        return 0;
    }
    data         = (Ov003AttackData*)func_ov003_020a5d2c(8, data_ov003_020e3958);
    data->effect = effect;
    data->target = state;
    cmd->fire    = func_ov003_02087cfc;
    cmd->data    = data;
    return 1;
}

void func_ov003_02087da0(Ov003AttackData* data) {
    s32 x;
    s32 y;

    func_ov003_02087f28(0x3B, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x);
    x = func_ov003_0208442c(0, data_ov003_020e71b8->unk3D3AA);
    y = func_ov003_02084460(0, data_ov003_020e71b8->unk3D3AC, 0);
    func_ov003_02083ca0(&x, &y, 0x8000, 0x8000);
    func_ov003_02087f28(0x3C, x);
    func_ov003_02087b00(0, x, y, 0);
    func_ov003_020a5aa4(data->effect);
    func_ov003_020a5a94(data->effect);
}

/// Registers the "teleport" attack: the handler is `func_ov003_02087da0`.
s32 func_ov003_02087e5c(Ov003AttackCmd* cmd, Ov003EffectState* effect) {
    Ov003EffectState** data;

    if (data_ov003_020e71b8->unk3D894 < 0x64) {
        return 0;
    }
    if (func_ov003_02085cfc(2) == 0) {
        return 0;
    }
    if (func_ov003_02085d3c(0) == 0) {
        return 0;
    }
    data      = (Ov003EffectState**)func_ov003_020a5d2c(4, data_ov003_020e396c);
    *data     = effect;
    cmd->fire = (void (*)(Ov003AttackData*))func_ov003_02087da0;
    cmd->data = (Ov003AttackData*)data;
    return 1;
}

/// Plays an SE unless the battle is muted.
void func_ov003_02087ed8(SndMgrSeIdx seIdx) {
    if (data_ov003_020e71b8->unk3D878 & 0x20) {
        return;
    }
    SndMgr_StartPlayingSE(seIdx);
}

/// Plays an SE at a pan position unless the battle is muted.
void func_ov003_02087f00(SndMgrSeIdx seIdx, s32 sePan) {
    if (data_ov003_020e71b8->unk3D878 & 0x20) {
        return;
    }
    SndMgr_PlaySEWithPan(seIdx, sePan);
}

void func_ov003_02087f28(SndMgrSeIdx seIdx, s32 pan) {
    if (data_ov003_020e71b8->unk3D878 & 0x20) {
        return;
    }
    SndMgr_PlaySEWithPan(seIdx, func_ov003_020843b0(0, pan));
}

void func_ov003_02087f64(SndMgrSeIdx seIdx, s32 pan) {
    if (data_ov003_020e71b8->unk3D878 & 0x20) {
        return;
    }
    SndMgr_PlaySEWithPan(seIdx, func_ov003_020843b0(1, pan));
}

/// Maps a pixel distance to one of the six effect sizes.
s32 func_ov003_02087fa0(s32 arg0) {
    if (arg0 < 8) {
        return 0;
    }
    if (arg0 < 0xC) {
        return 1;
    }
    if (arg0 < 0x18) {
        return 2;
    }
    if (arg0 < 0x20) {
        return 3;
    }
    if (arg0 < 0x40) {
        return 4;
    }
    return 5;
}

/// Loads a battle effect actor's box sprite.
///
/// The retail call site passes `variant` through without narrowing it to the `u16` parameter of
/// `CombatSprite_LoadDirect`'s prototype, so call it through a plain-integer signature.
#define CombatSprite_LoadDirectNoProto                                                                     \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, s32, s32, s32, s32, s32))CombatSprite_LoadDirect)

void func_ov003_02087fe0(s32 engine, CombatSprite* sprite, s32 variant) {
    if (engine == 0) {
        CombatSprite_LoadDirectNoProto(0, sprite, &data_ov003_020d7800, 0xD, 0xF, 0xE, 0x3D, variant);
    } else {
        CombatSprite_LoadDirectNoProto(1, sprite, &data_ov003_020d7800, 0xD, 0xF, 0xE, 0x3F, variant);
    }
    func_ov003_02082730(sprite, 0x7FFFFFFF);
    sprite->flags46 |= 4;
}

/// Positions and starts an effect actor for a unit that was just hit.
void func_ov003_02088064(CombatSprite* visual, s32 x, s32 y) {
    s32 pan;
    s32 volume;

    if (visual->sprite.bits_0_1 == 0) {
        pan    = func_ov003_020843b0(0, x);
        volume = func_ov003_020843ec(0, y, 0);
    } else {
        pan    = func_ov003_020843b0(1, x);
        volume = func_ov003_020843ec(1, y, 0);
    }
    CombatSprite_SetPosition(visual, pan, volume);
    if (func_ov003_02088130() == 0) {
        return;
    }
    CombatSprite_Render(visual);
}

/// Spawns a hit effect for `unit` at the effect size its hit box implies.
void func_ov003_020880e4(s32 engine, CombatSprite* sprite, Ov003EnemyActor* unit) {
    func_ov003_02087fe0(engine, sprite, func_ov003_02087fa0(unit->actor.unk_70));
}

/// Spawns the hit effect of an incoming attack on `unit`.
void func_ov003_0208810c(CombatSprite* visual, CombatActor* actor) {
    if (actor->flags & 0x80000000) {
        return;
    }
    func_ov003_02088064(visual, actor->position.x, actor->position.y);
}

/// TRUE unless the battle's noise/attack phase suppresses effect spawning.
s32 func_ov003_02088130(void) {
    s32 result = 1;

    if (data_ov003_020e71b8->unk3D878 & 2) {
        switch (data_ov003_020e71d9) {
            default:
                break;
            case 0:
                if ((u32)(u8)(data_ov003_020e71d8 + 0xFF) <= 1U) {
                    result = 0;
                }
                break;
            case 1:
                result = 0;
                break;
            case 2:
                if (data_ov003_020e71d8 == 2) {
                    result = 0;
                }
                break;
        }
    }
    return result;
}

/// Spawns the attack's own visual effect in front of the attacking unit.
// Nonmatching: register allocation of the info/hit pointers
u32 func_ov003_020881b4(Ov003Hit* hit) {
    const Ov003AttackInfo* info = hit->info;
    u32                    id   = (u32)(info->flags << 0x17) >> 0x1F;
    u8                     halfW;
    u8                     halfH;

    if (id != 0) {
        return id;
    }
    halfW = info->unk_05;
    halfH = info->unk_06;
    if (hit->kind == 3) {
        return func_ov003_020a2a04(2, 0, hit->x - (halfW << 12), hit->y - (halfH << 12), (s16)(halfW * 2), (s16)(halfH * 2));
    }
    return func_ov003_020a2a04(1, 0, hit->x - (halfW << 12), hit->y - (halfH << 12), (s16)(halfW * 2), (s16)(halfH * 2));
}

/// Lowers `*damage` to zero when the combo rank is high enough.
void func_ov003_02088264(s32 mode, s16* damage) {
    s16 rank;
    s16 maxRank;
    s16 chance;

    if (mode != 1) {
        rank = data_ov003_020e71b8->unk3D8D6;
    } else {
        rank = data_ov003_020e71b8->unk3D8D4;
    }
    maxRank = data_ov003_020e71b8->unk3D8D2;
    if (rank < maxRank / 2) {
        return;
    }
    if (func_ov003_0208ca04(mode, 0x38) == 0) {
        return;
    }
    chance = func_ov003_0208ca24(mode, 0x38);
    if ((s32)RNG_Next(0x64) < chance) {
        *damage = 0;
    }
}

/// Scales `*damage` up by the combo-rank bonus.
void func_ov003_020882f0(s32 mode, s16* damage) {
    s16 rank;
    s16 maxRank;
    s16 bonus;

    if (mode != 1) {
        rank = data_ov003_020e71b8->unk3D8D6;
    } else {
        rank = data_ov003_020e71b8->unk3D8D4;
    }
    maxRank = data_ov003_020e71b8->unk3D8D2;
    if (rank < maxRank / 2) {
        return;
    }
    if (func_ov003_0208ca04(mode, 0x3D) == 0) {
        return;
    }
    bonus = func_ov003_0208ca24(mode, 0x3D);
    *damage += (s16)(*damage * bonus / 100);
}

/// Scales `*damage` down by the combo-rank penalty.
void func_ov003_0208838c(s32 mode, s16* damage) {
    s16 rank;
    s16 maxRank;
    s16 bonus;

    if (mode != 1) {
        rank = data_ov003_020e71b8->unk3D8D6;
    } else {
        rank = data_ov003_020e71b8->unk3D8D4;
    }
    maxRank = data_ov003_020e71b8->unk3D8D2;
    if (rank < maxRank / 2) {
        return;
    }
    if (func_ov003_0208ca04(mode, 0x3E) == 0) {
        return;
    }
    bonus = func_ov003_0208ca24(mode, 0x3E);
    *damage -= (s16)(*damage * bonus / 100);
}

#define OV003_ATTACK_PERCENT(value, pct) ((s16)((value) * (pct) / 100))

/// Applies the attack-variant damage modifiers when the attack lands on a unit.
// Nonmatching: jump-table layout of the two variant switches
void func_ov003_02088428(s32 mode, s16* attack, s16* damage, u32 variant, s32 friendly) {
    if (friendly != 0) {
        switch (variant) {
            default:
                return;
            case 1:
                if (func_ov003_0208ca04(mode, 0x32) == 0) {
                    return;
                }
                *damage -= func_ov003_0208ca24(mode, 0x32);
                return;
            case 2:
                if (func_ov003_0208ca04(mode, 0x33) == 0) {
                    return;
                }
                *damage -= func_ov003_0208ca24(mode, 0x33);
                return;
            case 3:
                if (func_ov003_0208ca04(mode, 0x34) == 0) {
                    return;
                }
                *damage -= func_ov003_0208ca24(mode, 0x34);
                return;
            case 4:
                if (func_ov003_0208ca04(mode, 0x35) == 0) {
                    return;
                }
                *damage -= func_ov003_0208ca24(mode, 0x35);
                return;
        }
    }
    switch (variant) {
        default:
            return;
        case 1:
            if (func_ov003_0208ca04(mode, 0x14) != 0) {
                *attack *= func_ov003_0208ca24(mode, 0x14);
            }
            if (func_ov003_0208ca04(mode, 0x18) == 0) {
                return;
            }
            *damage += func_ov003_0208ca24(mode, 0x18);
            return;
        case 2:
            if (func_ov003_0208ca04(mode, 0x15) != 0) {
                *attack *= func_ov003_0208ca24(mode, 0x15);
            }
            if (func_ov003_0208ca04(mode, 0x19) == 0) {
                return;
            }
            *damage += func_ov003_0208ca24(mode, 0x19);
            return;
        case 3:
            if (func_ov003_0208ca04(mode, 0x16) != 0) {
                *attack *= func_ov003_0208ca24(mode, 0x16);
            }
            if (func_ov003_0208ca04(mode, 0x1A) == 0) {
                return;
            }
            *damage += func_ov003_0208ca24(mode, 0x1A);
            return;
        case 4:
            if (func_ov003_0208ca04(mode, 0x17) != 0) {
                *attack *= func_ov003_0208ca24(mode, 0x17);
            }
            if (func_ov003_0208ca04(mode, 0x1B) == 0) {
                return;
            }
            *damage += func_ov003_0208ca24(mode, 0x1B);
            return;
    }
}

/// Advances the "tension" gauge while the unit is falling off screen.
void func_ov003_02088694(Ov003EnemyActor* unit) {
    s16 tension;

    if (Inventory_GetOwnedCount(0x2AA) == 0) {
        return;
    }
    if (unit->actor.position.z >= -0x10000) {
        return;
    }
    tension = data_ov003_020e71b8->unk3D8F8;
    if (tension < 0xA) {
        return;
    }
    if (tension < 0x14) {
        data_ov003_020e71b8->unk3D8FA++;
        return;
    }
    if (tension < 0x1E) {
        data_ov003_020e71b8->unk3D8FA += 2;
        return;
    }
    data_ov003_020e71b8->unk3D8FA += 3;
}

/// TRUE when `unit` cannot be hit by this attack.
s32 func_ov003_02088714(Ov003EnemyActor* unit, const Ov003AttackInfo* info) {
    s32 flags = unit->actor.flags;

    if ((flags & 0x8000) || (flags & 4) || (flags & 0x10)) {
        return 1;
    }
    if (unit->actor.unk_5A > 0 && ((u32)(info->flags << 0x12) >> 0x1F) != 1) {
        return 1;
    }
    return 0;
}

/// Cancels the unit's current move if it is interruptible.
s32 func_ov003_02088764(Ov003EnemyActor* unit) {
    if (!(unit->actor.flags & 0x40)) {
        return 0;
    }
    CombatActor_SetPendingCommand(&unit->actor, 7);
    return 1;
}

/// Box overlap test between the unit's hit volume and the attack's.
// Nonmatching: register allocation of the hit-volume bounds
s32 func_ov003_02088788(Ov003EnemyActor* unit, s32 x, s32 y, s32 z, s32 halfW, s32 halfH, s32 halfD) {
    s16 radius  = unit->actor.unk_70;
    s16 zRadius = unit->actor.unk_72;
    s32 posX    = unit->actor.position.x;
    s32 posY    = unit->actor.position.y;
    s32 posZ    = unit->actor.position.z;

    if (posX - (radius << 12) > x + halfW) {
        return 0;
    }
    if (posX + (radius << 12) < x - halfW) {
        return 0;
    }
    if (posY - ((radius / 2) << 12) > y + halfH) {
        return 0;
    }
    if (posY + ((radius / 2) << 12) < y - halfH) {
        return 0;
    }
    if (posZ < z - halfD) {
        return 0;
    }
    if (posZ - (zRadius << 12) <= z + halfD) {
        return 1;
    }
    return 0;
}

/// Box overlap test in screen space, used for the attack's hit point.
// Nonmatching: register allocation of the info/hit pointers
s32 func_ov003_0208882c(Ov003EnemyActor* unit, Ov003Hit* hit) {
    const Ov003AttackInfo* info  = hit->info;
    s32                    halfW = hit->x * 0x10;
    s16                    posY;
    s16                    posZ = hit->y >> 12;

    halfW >>= 16;
    if ((s32)(unit->actor.screenX - unit->actor.unk_70) > (s32)(info->unk_05 + halfW)) {
        return 0;
    }
    if ((s32)(unit->actor.screenX + unit->actor.unk_70) < (s32)(halfW - info->unk_05)) {
        return 0;
    }
    posY = unit->actor.screenY;
    if ((s32)(posY - unit->actor.unk_72) > (s32)(info->unk_06 + (s16)posZ)) {
        return 0;
    }
    if ((s32)posY >= (s32)((s16)posZ - info->unk_06)) {
        return 1;
    }
    return 0;
}

/// Decides the unit's reaction to a hit and applies it.
// Nonmatching: register allocation of the flags word
void func_ov003_020888b0(Ov003EnemyActor* unit, const Ov003AttackInfo* info, s32 arg2) {
    if (unit->actor.position.z < -0x10000 || unit->actor.pendingCommand == 2 || (unit->actor.flags & 0x4000) ||
        ((u32)(info->flags << 0xD) >> 0x1C) >= 6U)
    {
        if (unit->actor.periodicEffectMode == 4) {
            CombatActor_ClearPeriodicEffect(&unit->actor);
        }
        CombatActor_SetPendingCommand(&unit->actor, 2);
    } else if (arg2 == 0 && ((u32)(info->flags << 0x11) >> 0x1F) != 1 && !(unit->actor.flags & 0x40000000)) {
        CombatActor_SetPendingCommand(&unit->actor, 1);
    }
    unit->actor.unk_10 = info->unk_03;
    unit->actor.unk_12 = (u16)((unit->actor.unk_12 & ~1) | ((u16)((u32)(info->flags << 0x13) >> 0x1F) & 1));
    unit->actor.flags |= 0x1000;
}

/// Marks the battle as failed/cleared according to the attack flags.
// Nonmatching: register allocation of the flags word
void func_ov003_02088988(const Ov003AttackInfo* info) {
    if (((u32)(info->flags << 0x13) >> 0x1F) == 0) {
        return;
    }
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        if (data_ov003_020e71b8->unk3D8ED != 0 && data_ov003_020e71b8->unk3D8F4 == 0) {
            return;
        }
        data_ov003_020e71b8->unk3D8EC = 1;
        data_ov003_020e71b8->unk3D878 |= 0x80;
        return;
    }
    if (data_ov003_020e71b8->unk3D8ED != 0 && data_ov003_020e71b8->unk3D8F0 == 0) {
        return;
    }
    data_ov003_020e71b8->unk3D8EC = 2;
    data_ov003_020e71b8->unk3D878 |= 0x80;
}

/// Turns the unit towards the attack's origin.
// Nonmatching: register allocation of the flags word
void func_ov003_02088a48(Ov003EnemyActor* unit, const Ov003AttackInfo* info, s32 x) {
    s32 turn = 0;
    u16 bit;

    if (((u32)(info->flags << 0x17) >> 0x1F) == 1 && ((u32)(info->flags << 0x18) >> 0x1F) == 1) {
        if (unit->actor.isFlipped == 0) {
            if (unit->actor.position.x < x) {
                turn = 1;
            }
        } else if (unit->actor.position.x > x) {
            turn = 1;
        }
    }
    bit                = (turn != 0) ? 1 : 0;
    unit->actor.unk_12 = (u16)((unit->actor.unk_12 & ~0x40) | ((u32)(bit << 0x1F) >> 0x19));
}

/// Gives the unit a knockback velocity away from the attack point.
// Nonmatching: register allocation of the flags/variant temporaries
void func_ov003_02088abc(Ov003EnemyActor* unit, const Ov003AttackInfo* info, s32 x) {
    s32 targetX;
    u32 variant;
    s32 speed;
    s32 zSpeed;

    if (unit->actor.pendingCommand != 1 && unit->actor.pendingCommand != 2) {
        return;
    }
    variant = (u32)(info->flags << 0xD) >> 0x1C;
    speed   = *(const s32*)((const u8*)data_ov003_020d7650 + variant * 8);
    zSpeed  = *(const s32*)((const u8*)data_ov003_020d7654 + variant * 8);
    targetX = x;
    if (((u32)(info->flags << 0x14) >> 0x1F) == 1) {
        if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
            targetX = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x;
        } else {
            targetX = ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x;
        }
    }
    if (unit->actor.position.x < targetX) {
        speed = -speed;
    }
    unit->actor.xVelocity = speed;
    unit->actor.zVelocity = zSpeed;
    unit->actor.unk_0C    = targetX;
}

/// Rolls the attack's hit chance and damage and applies them to `unit`.
void func_ov003_02088b5c(s32 mode, Ov003EnemyActor* unit, u16 attackId) {
    const Ov003AttackParams* params;
    s16                      damage;
    s16                      attack;

    if (unit->actor.pendingCommand != 1) {
        return;
    }
    if (attackId == 0) {
        return;
    }
    params = &data_ov003_020d76d0[attackId];
    if (unit->actor.unk_00 == 1) {
        s32 hit = 0;

        switch (params->unk_00) {
            default:
                break;
            case 1:
                if (((u32)(unit->flags196.raw << 0x1D) >> 0x1F) == 1) {
                    hit = 1;
                }
                break;
            case 2:
                if (((u32)(unit->flags196.raw << 0x1B) >> 0x1F) == 1) {
                    hit = 1;
                }
                break;
            case 3:
                if (((u32)(unit->flags196.raw << 0x1A) >> 0x1F) == 1) {
                    hit = 1;
                }
                break;
            case 4:
                if (((u32)(unit->flags196.raw << 0x1C) >> 0x1F) == 1) {
                    hit = 1;
                }
                break;
        }
        if (hit != 0) {
            return;
        }
    }
    attack = params->unk_04;
    damage = params->unk_06;
    func_ov003_02088428(mode, &attack, &damage, params->unk_00, unit->actor.unk_00 == 0);
    if ((s32)RNG_Next(0x2710) >= (s32)(damage * 0x64)) {
        return;
    }
    func_ov003_02089f90(mode, unit, params->unk_00, params->unk_02, attack);
}

// Nonmatching: register allocation of the flags word and branch delay slot
void func_ov003_02088cc0(Ov003EnemyActor* unit, const Ov003AttackInfo* info) {
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        func_ov003_02088b5c(0, unit, (u16)((u32)(info->flags << 8) >> 0x1B));
        return;
    }
    func_ov003_02088b5c(1, unit, (u16)((u32)(info->flags << 8) >> 0x1B));
}

/// Spawns the attack's damage number and effect at the hit point.
// Nonmatching: callee-saved register rotation of the position temporaries
void func_ov003_02088d0c(Ov003EnemyActor* unit, Ov003Hit* hit) {
    s32 x;
    s32 y;
    s32 z;
    s32 targetX;
    s32 jitterX;
    s32 jitterZ;
    s32 flipped;
    s32 radius;
    s32 halfRadius;

    if (unit->actor.flags & 0x20000000) {
        return;
    }
    if (((u32)(hit->info->flags << 0x1B) >> 0x1B) == 0x1A) {
        return;
    }
    switch (hit->kind) {
        case 3:
            if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
                x = func_ov003_0208442c(0, (hit->x * 0x10) >> 0x10);
                y = func_ov003_02084460(0, (hit->y * 0x10) >> 0x10, unit->actor.position.z);
            } else {
                x = func_ov003_0208442c(1, (hit->x * 0x10) >> 0x10);
                y = func_ov003_02084460(1, (hit->y * 0x10) >> 0x10, unit->actor.position.z);
            }
            z       = unit->actor.position.z;
            targetX = x;
            break;
        case 1:
            x       = unit->actor.position.x;
            z       = unit->actor.position.z - (unit->actor.unk_76 << 12);
            y       = unit->actor.position.y;
            targetX = hit->x;
            break;
        case 2:
            x       = hit->x;
            y       = hit->y;
            z       = hit->z;
            targetX = x;
            break;
        default:
            x       = 0;
            y       = 0;
            z       = 0;
            targetX = 0;
            break;
    }
    switch ((u32)(hit->info->flags << 0x19) >> 0x1E) {
        case 1:
            break;
        case 2:
            radius = unit->actor.unk_70;
            if (unit->actor.position.x < x) {
                x = ((unit->actor.position.x + (radius << 12)) + (x - (hit->info->unk_05 << 12))) / 2;
            } else {
                x = ((unit->actor.position.x - (radius << 12)) + (x + (hit->info->unk_05 << 12))) / 2;
            }
            halfRadius = radius / 2;
            if (unit->actor.position.y < y) {
                y = ((unit->actor.position.y + (halfRadius << 12)) + (y - (hit->info->unk_06 << 12))) / 2;
            } else {
                y = ((unit->actor.position.y - (halfRadius << 12)) + (y + (hit->info->unk_06 << 12))) / 2;
            }
            z = (z + (unit->actor.position.z - (unit->actor.unk_76 << 12))) / 2;
            break;
        default:
        case 0:
            x = unit->actor.position.x;
            y = unit->actor.position.y;
            z = unit->actor.position.z - (unit->actor.unk_76 << 12);
            break;
    }
    if (((u32)(hit->info->flags << 0x14) >> 0x1F) == 1) {
        if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
            targetX = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x;
        } else {
            targetX = ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x;
        }
    }
    jitterX = x + ((RNG_Next(0x11) - 8) << 12);
    jitterZ = z + ((RNG_Next(0x11) - 8) << 12);
    flipped = (unit->actor.position.x >= targetX) ? 1 : 0;
    func_ov003_0209ed70((u8)((u32)(hit->info->flags << 0x1B) >> 0x1B), jitterX, y, jitterZ, flipped);
}

/// Starts the unit's "hit" reaction; returns 1 when the attack was applied.
s32 func_ov003_02088f5c(Ov003EnemyActor* unit, const Ov003AttackInfo* info, s32 x) {
    if (!(unit->actor.flags & 8)) {
        return 0;
    }
    CombatActor_SetPendingCommand(&unit->actor, 9);
    if (!(data_ov003_020e71b8->unk3D878 & 0x40000000)) {
        unit->actor.isFlipped = (unit->actor.position.x < x) ? 1 : 0;
    }
    unit->actor.unk_5A = info->unk_04;
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        func_ov003_0209e9f8(0, 0, unit->actor.position.x, unit->actor.position.y,
                            unit->actor.position.z - (unit->actor.unk_76 << 12), (unit->actor.isFlipped != 0) ? 1 : 0);
        func_ov003_0209c984(0, 0, unit->actor.position.x, unit->actor.position.y,
                            unit->actor.position.z - ((unit->actor.unk_72 / 2) << 12));
    } else {
        func_ov003_0209e9f8(1, 0, unit->actor.position.x, unit->actor.position.y,
                            unit->actor.position.z - (unit->actor.unk_76 << 12), (unit->actor.isFlipped != 0) ? 1 : 0);
        func_ov003_0209c984(1, 0, unit->actor.position.x, unit->actor.position.y,
                            unit->actor.position.z - ((unit->actor.unk_72 / 2) << 12));
    }
    func_ov003_02088abc(unit, info, x);
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        data_ov003_020e71b8->unk3D906 = info->unk_02;
    } else {
        data_ov003_020e71b8->unk3D90A = info->unk_02;
    }
    return 1;
}

/// Damage multiplier for the attack variant: 100% plus the per-variant bonus, and the hit
/// direction marker (low/high/normal) in the unit's status bits.
s32 func_ov003_020890f0(Ov003EnemyActor* unit, const Ov003AttackInfo* info) {
    s16 multiplier = 0x64;

    unit->actor.unk_12 =
        (u16)((unit->actor.unk_12 & ~0x30) | ((u32)((u16)((u32)(info->flags << 0x15) >> 0x1E) << 0x1E) >> 0x1A));
    if (((u32)(info->flags << 0x15) >> 0x1E) == 1) {
        multiplier = unit->unk_192;
    }
    if (((u32)(info->flags << 0x15) >> 0x1E) == 2) {
        multiplier = unit->unk_194;
    }
    if (multiplier < 0) {
        unit->actor.unk_12 = (u16)((unit->actor.unk_12 & ~0xC) | 4);
    } else if (multiplier < 0x64) {
        unit->actor.unk_12 = (u16)((unit->actor.unk_12 & ~0xC) | 8);
    } else if (multiplier > 0x64) {
        unit->actor.unk_12 = (u16)((unit->actor.unk_12 & ~0xC) | 0xC);
    }
    return multiplier;
}

/// Computes the damage a melee attack deals.
// Nonmatching: register allocation around the sync multiplier
s16 func_ov003_02089194(Ov003EnemyActor* unit, const Ov003AttackInfo* info, const Ov003AttackArgs* args) {
    s16 damage = OV003_ATTACK_PERCENT((s16)args->unk_00, info->unk_00);
    s32 bonus;

    damage = OV003_ATTACK_PERCENT(damage, (s16)args->unk_04);
    damage = OV003_ATTACK_PERCENT(damage, unit->actor.unk_62);
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        if (data_ov003_020e71b8->unk3D8F4 == 0 && ((u32)(info->flags << 0x12) >> 0x1F) != 1 &&
            ((u32)(unit->flags196.raw << 0x16) >> 0x1F) == 1)
        {
            damage = (s16)(damage / 4);
        }
        s16 multiplier = func_ov003_020890f0(unit, info);

        damage = OV003_ATTACK_PERCENT(damage, multiplier);
        if (((u32)(info->flags << 0x13) >> 0x1F) == 1 && data_ov003_020e71b8->unk3D8F4 != 0) {
            s16 sync = data_ov003_020e71b8->unk3D8EE;

            damage = OV003_ATTACK_PERCENT(damage, sync);
            func_ov003_0209c568(0, 0, damage);
            unit->actor.unk_12 |= 2;
        }
        func_ov003_020882f0(0, &damage);
    } else {
        if (data_ov003_020e71b8->unk3D8F0 == 0 && ((u32)(info->flags << 0x12) >> 0x1F) != 1 &&
            ((u32)(unit->flags196.raw << 0x16) >> 0x1F) == 1)
        {
            damage = (s16)(damage / 4);
        }
        if (((u32)(info->flags << 0x13) >> 0x1F) == 1 && data_ov003_020e71b8->unk3D8F0 != 0) {
            s16 sync = data_ov003_020e71b8->unk3D8EE;

            damage = OV003_ATTACK_PERCENT(damage, sync);
            func_ov003_0209c568(1, 0, damage);
            unit->actor.unk_12 |= 2;
        }
        func_ov003_020882f0(1, &damage);
    }
    damage = OV003_ATTACK_PERCENT(damage, ((const s16*)data_ov003_020d7620)[gSaveData.battleDifficulty * 2]);
    damage = OV003_ATTACK_PERCENT(damage, (s16)args->unk_06);
    damage = OV003_ATTACK_PERCENT(damage, unit->actor.unk_64);
    return damage;
}

/// Computes the damage a ranged attack deals.
s16 func_ov003_0208943c(Ov003EnemyActor* unit, const Ov003AttackInfo* info, const Ov003AttackArgs* args) {
    s16 damage = (s16)args->unk_00 + info->unk_00;

    damage = OV003_ATTACK_PERCENT(damage, (s16)args->unk_04);
    damage = OV003_ATTACK_PERCENT(damage, ((const s16*)data_ov003_020d7622)[gSaveData.battleDifficulty * 2]);
    damage = OV003_ATTACK_PERCENT(damage, (s16)args->unk_06);
    damage = OV003_ATTACK_PERCENT(damage, unit->actor.unk_64);
    damage = OV003_ATTACK_PERCENT(damage, data_ov003_020d7630[data_ov003_020e71b8->unk3D88C]);
    damage -= unit->actor.unk_68;
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        func_ov003_0208838c(0, &damage);
    } else {
        func_ov003_0208838c(1, &damage);
        if (((u32)(unit->actor.unk_12 << 0x19) >> 0x1F) == 1) {
            damage = OV003_ATTACK_PERCENT(damage, 0x96);
        }
    }
    if (damage < 0) {
        damage = 0;
    }
    return damage;
}

/// Applies the computed damage and the hit reaction to the unit.
void func_ov003_020895ac(Ov003EnemyActor* unit, const Ov003AttackInfo* info, s32 damage, s16 arg3) {
    if (damage < 0) {
        unit->actor.unk_12 = (u16)((unit->actor.unk_12 & ~0xC) | 4);
    }
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        func_ov003_0208b518(0, unit, -damage);
        func_ov003_0209b61c(0, damage, unit);
        if (damage > 0) {
            func_ov003_0208cb6c(0, damage);
            func_ov003_0209c150(arg3);
            func_ov003_02088694(unit);
        }
    } else {
        func_ov003_0208b518(1, unit, -damage);
        func_ov003_0209b61c(1, damage, unit);
        if (damage > 0) {
            func_ov003_0208cb6c(1, damage);
        }
    }
    unit->actor.unk_5A = info->unk_04;
    if (unit->actor.currentHp <= 0) {
        unit->actor.flags |= 0x2000;
        CombatActor_ForceDefeat(&unit->actor);
        CombatActor_ForceDefeat((CombatActor*)unit->actor.unk_78);
        return;
    }
    if (damage <= 0) {
        return;
    }
    func_ov003_020888b0(unit, info, ((u32)(unit->flags196.raw << 0x1E) >> 0x1F) == 1 ? 1 : 0);
}

/// Applies the "gauge" damage bank used by the special attacks.
void func_ov003_020896d0(Ov003EnemyActor* unit, const Ov003AttackInfo* info, s16 gauge) {
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        func_ov003_02088264(0, &gauge);
        func_ov003_0208b3cc(0, -gauge);
        func_ov003_0209b564(0, gauge, &unit->actor, 3);
    } else {
        func_ov003_02088264(1, &gauge);
        func_ov003_0208b3cc(1, -gauge);
        func_ov003_0209b564(1, gauge, &unit->actor, 3);
    }
    unit->actor.unk_5A = info->unk_04;
    if (unit->actor.currentHp <= 0) {
        unit->actor.flags |= 0x2000;
        CombatActor_ForceDefeat((CombatActor*)data_ov003_020e71b8->unk3D898);
        CombatActor_ForceDefeat((CombatActor*)data_ov003_020e71b8->unk3D89C);
        data_ov003_020e71b8->unk3D878 |= 0x30000;
        return;
    }
    if (gauge <= 0) {
        return;
    }
    func_ov003_020888b0(unit, info, 0);
    func_ov003_0208cbac();
}

/// Resolves one attack against a unit: computes the damage and applies it.
void func_ov003_02089804(Ov003EnemyActor* unit, const Ov003AttackInfo* info, const Ov003AttackArgs* args) {
    unit->actor.unk_12 &= ~2;
    unit->actor.unk_12 &= ~0xC;
    unit->actor.unk_12 &= ~0x30;
    if (((u32)(info->flags << 0x18) >> 0x1F) == 0) {
        func_ov003_020895ac(unit, info, func_ov003_02089194(unit, info, args), (s16)args->unk_02);
        func_ov003_02088988(info);
        return;
    }
    func_ov003_020896d0(unit, info, func_ov003_0208943c(unit, info, args));
}

/// Resolves a hit whose target is already known.
s32 func_ov003_02089884(Ov003Hit* hit) {
    if (hit->unit == NULL) {
        return 0;
    }
    if (func_ov003_02088714(hit->unit, hit->info) != 0) {
        return 0;
    }
    if (func_ov003_02088764(hit->unit) != 0) {
        return 0;
    }
    if (func_ov003_02088f5c(hit->unit, hit->info, hit->x) != 0) {
        return 2;
    }
    func_ov003_02088a48(hit->unit, hit->info, hit->x);
    func_ov003_02089804(hit->unit, hit->info, (Ov003AttackArgs*)hit->payload);
    func_ov003_02088d0c(hit->unit, hit);
    func_ov003_02088abc(hit->unit, hit->info, hit->x);
    func_ov003_02088cc0(hit->unit, hit->info);
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        data_ov003_020e71b8->unk3D906 = hit->info->unk_02;
    } else {
        data_ov003_020e71b8->unk3D90A = hit->info->unk_02;
    }
    return 1;
}

/// Resolves an area hit against every unit the attack overlaps.
s32 func_ov003_02089974(Ov003Hit* hit) {
    s32              result = 0;
    Ov003EnemyActor* unit;
    s32              halfW;
    s32              halfH;
    s32              halfD;

    func_ov003_020881b4(hit);
    halfW = hit->info->unk_05 << 12;
    halfH = hit->info->unk_06 << 12;
    halfD = hit->info->unk_07 << 12;
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    } else {
        unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    }
    while (unit != NULL) {
        if (func_ov003_02088714(unit, hit->info) == 0 &&
            func_ov003_02088788(unit, hit->x, hit->y, hit->z, halfW, halfH, halfD) != 0 && func_ov003_02088764(unit) == 0)
        {
            if (func_ov003_02088f5c(unit, hit->info, hit->x) != 0) {
                if (result == 0) {
                    result = 2;
                }
            } else {
                func_ov003_02089804(unit, hit->info, (Ov003AttackArgs*)hit->payload);
                func_ov003_02088d0c(unit, hit);
                func_ov003_02088abc(unit, hit->info, hit->x);
                func_ov003_02088cc0(unit, hit->info);
                result = 1;
            }
        }
        unit = func_ov003_02084984(&unit->node.prev);
    }
    if (result == 1) {
        if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
            data_ov003_020e71b8->unk3D906 = hit->info->unk_02;
        } else {
            data_ov003_020e71b8->unk3D90A = hit->info->unk_02;
        }
    }
    return result;
}

/// Resolves a line hit against the first unit in front of the attack.
s32 func_ov003_02089b04(Ov003Hit* hit) {
    Ov003EnemyActor* unit;
    s32              halfW;
    s32              halfH;
    s32              halfD;

    func_ov003_020881b4(hit);
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        unit = data_ov003_020e71b8->unk3D898;
    } else {
        unit = data_ov003_020e71b8->unk3D89C;
    }
    hit->unit = unit;
    halfW     = hit->info->unk_05 << 12;
    halfH     = hit->info->unk_06 << 12;
    halfD     = hit->info->unk_07 << 12;
    if (func_ov003_02088714(unit, hit->info) != 0 ||
        func_ov003_02088788(hit->unit, hit->x, hit->y, hit->z, halfW, halfH, halfD) == 0)
    {
        return 0;
    }
    if (func_ov003_02088764(hit->unit) != 0) {
        return 0;
    }
    if (func_ov003_02088f5c(hit->unit, hit->info, hit->x) != 0) {
        return 2;
    }
    func_ov003_02088a48(hit->unit, hit->info, hit->x);
    func_ov003_02089804(hit->unit, hit->info, (Ov003AttackArgs*)hit->payload);
    func_ov003_02088d0c(hit->unit, hit);
    func_ov003_02088abc(hit->unit, hit->info, hit->x);
    func_ov003_02088cc0(hit->unit, hit->info);
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        data_ov003_020e71b8->unk3D906 = hit->info->unk_02;
    } else {
        data_ov003_020e71b8->unk3D90A = hit->info->unk_02;
    }
    return 1;
}

/// Resolves a screen-space (projected) hit against every overlapping unit.
s32 func_ov003_02089c74(Ov003Hit* hit) {
    Ov003EnemyActor* unit;
    s32              x;
    s32              result = 0;

    func_ov003_020881b4(hit);
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        x = func_ov003_0208442c(0, (hit->x * 0x10) >> 0x10);
    } else {
        x = func_ov003_0208442c(1, (hit->x * 0x10) >> 0x10);
    }
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    } else {
        unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    }
    while (unit != NULL) {
        if (func_ov003_02088714(unit, hit->info) == 0 && func_ov003_0208882c(unit, hit) != 0 && func_ov003_02088764(unit) == 0)
        {
            if (func_ov003_02088f5c(unit, hit->info, x) != 0) {
                if (result == 0) {
                    result = 2;
                }
            } else {
                func_ov003_02089804(unit, hit->info, (Ov003AttackArgs*)hit->payload);
                func_ov003_02088d0c(unit, hit);
                func_ov003_02088abc(unit, hit->info, x);
                func_ov003_02088cc0(unit, hit->info);
                result = 1;
            }
        }
        unit = func_ov003_02084984(&unit->node.prev);
    }
    if (result == 1) {
        if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
            data_ov003_020e71b8->unk3D906 = hit->info->unk_02;
        } else {
            data_ov003_020e71b8->unk3D90A = hit->info->unk_02;
        }
    }
    return result;
}

/// Resolves a screen-space hit against the main display actor.
s32 func_ov003_02089e18(Ov003Hit* hit) {
    Ov003EnemyActor* unit;
    s32              x;

    func_ov003_020881b4(hit);
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        x = func_ov003_0208442c(0, (hit->x * 0x10) >> 0x10);
    } else {
        x = func_ov003_0208442c(1, (hit->x * 0x10) >> 0x10);
    }
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        unit = data_ov003_020e71b8->unk3D898;
    } else {
        unit = data_ov003_020e71b8->unk3D89C;
    }
    hit->unit = unit;
    if (func_ov003_02088714(unit, hit->info) != 0 || func_ov003_0208882c(hit->unit, hit) == 0) {
        return 0;
    }
    if (func_ov003_02088764(hit->unit) != 0) {
        return 0;
    }
    if (func_ov003_02088f5c(hit->unit, hit->info, x) != 0) {
        return 2;
    }
    func_ov003_02088a48(hit->unit, hit->info, x);
    func_ov003_02089804(hit->unit, hit->info, (Ov003AttackArgs*)hit->payload);
    func_ov003_02088d0c(hit->unit, hit);
    func_ov003_02088abc(hit->unit, hit->info, x);
    func_ov003_02088cc0(hit->unit, hit->info);
    if (((u32)(hit->info->flags << 0x17) >> 0x1F) == 0) {
        data_ov003_020e71b8->unk3D906 = hit->info->unk_02;
    } else {
        data_ov003_020e71b8->unk3D90A = hit->info->unk_02;
    }
    return 1;
}

/// Starts the unit's periodic hit effect and, for a fresh effect, its sprite animation.
void func_ov003_02089f90(s32 mode, Ov003EnemyActor* unit, u16 variant, s16 arg3, s16 arg4) {
    u16 previous = unit->actor.periodicEffectMode;

    CombatActor_ClearPeriodicEffect(&unit->actor);
    unit->actor.periodicEffectMode  = variant;
    unit->actor.periodicEffectTimer = arg4;
    switch (unit->actor.periodicEffectMode) {
        case 2:
            unit->actor.unk_0A = arg3;
            break;
        case 3:
            unit->actor.unk_64 = arg3;
            break;
        case 1:
            unit->actor.periodicEffectAmount = (s16)(unit->actor.maxHp * arg3 / 100);
            break;
        case 0:
        case 4:
            break;
    }
    if (previous != 0) {
        return;
    }
    func_ov003_0209ccc0(mode, unit);
}

/// Applies a plain hit to a unit (used by the projectile effects).
s32 func_ov003_0208a028(s32 mode, Ov003EnemyActor* unit, u16 attackId) {
    s32 flags = unit->actor.flags;

    if ((flags & 4) || (flags & 0x10) || (flags & 0x4000) || unit->actor.pendingCommand == 2) {
        return 0;
    }
    CombatActor_SetPendingCommand(&unit->actor, 1);
    func_ov003_02088b5c(mode, unit, attackId);
    return 1;
}

/// Applies a bare "stagger" hit to a unit.
s32 func_ov003_0208a08c(s32 mode, Ov003EnemyActor* unit, s32 arg2) {
    s32 flags = unit->actor.flags;

    if ((flags & 4) || (flags & 0x10) || (flags & 0x4000) || unit->actor.pendingCommand == 2) {
        return 0;
    }
    unit->actor.flags = flags & ~8;
    CombatActor_SetPendingCommand(&unit->actor, 1);
    if (arg2 > 0) {
        ((void (*)(s32, Ov003EnemyActor*, u16, s32, s32))func_ov003_02089f90)(mode, unit, 4, 0, arg2);
    }
    return 1;
}

/// The attack-info table entry for `id`.
const Ov003AttackInfo* func_ov003_0208a114(u16 id) {
    return &data_ov003_020d422c[id];
}

/// Queues one of the "sweep" attack animations.
s32 func_ov003_0208a128(const Ov003AttackInfo* info, void* payload, void* unit, s32 arg3) {
    Ov003Hit spawn;

    spawn.info    = info;
    spawn.payload = payload;
    spawn.kind    = 1;
    spawn.unit    = unit;
    spawn.x       = arg3;
    spawn.y       = 0;
    spawn.z       = 0;
    return func_ov003_0208a3ec(&spawn);
}

/// Queues a point attack animation.
s32 func_ov003_0208a164(const Ov003AttackInfo* info, Ov003AttackArgs* args, s32 x, s32 y, s32 z) {
    Ov003Hit spawn;

    spawn.info    = info;
    spawn.payload = args;
    spawn.kind    = 2;
    spawn.unit    = NULL;
    spawn.x       = x;
    spawn.y       = y;
    spawn.z       = z;
    return func_ov003_0208a3ec(&spawn);
}

/// Queues a pixel-space attack animation.
s32 func_ov003_0208a1a4(const Ov003AttackInfo* info, void* target, s32 x, s32 y) {
    Ov003Hit spawn;

    spawn.info    = info;
    spawn.payload = target;
    spawn.kind    = 3;
    spawn.unit    = NULL;
    spawn.x       = x << 12;
    spawn.y       = y << 12;
    spawn.z       = 0;
    return func_ov003_0208a3ec(&spawn);
}

s32 func_ov003_0208a1e8(const Ov003AttackInfo* info, void* payload, s32 arg2) {
    if (((u32)(info->flags << 0x17) >> 0x1F) == 0) {
        return func_ov003_0208a128(info, payload, data_ov003_020e71b8->unk3D898, arg2);
    }
    return func_ov003_0208a128(info, payload, data_ov003_020e71b8->unk3D89C, arg2);
}

/// Copies a spawn's attack arguments into the record the hit resolvers read.
void func_ov003_0208a234(s16 arg0, const Ov003AttackArgs* args, Ov003EnemyActor* unit) {
    unit->actor.unk_14     = arg0;
    unit->actor.attackArgs = *args;
}

/// Spawns the attack animation and the effect it carries.
// Nonmatching: instruction schedule of the local attack copy
s32 func_ov003_0208a25c(CombatActor* actor) {
    const Ov003AttackInfo* info;
    Ov003AttackInfo        attack;
    Ov003AttackArgs        args;
    s32                    result;

    if (actor->xVelocity == 0 && actor->yVelocity == 0) {
        if (actor->zVelocity < 0x5000) {
            return 0;
        }
    }
    info   = func_ov003_0208a114(actor->unk_14);
    attack = *info;
    args   = actor->attackArgs;
    if (((u32)(attack.flags << 0x18) >> 0x1F) == 0) {
        args.unk_00 = OV003_ATTACK_PERCENT(args.unk_00, actor->unk_6A);
    } else {
        args.unk_00   = OV003_ATTACK_PERCENT(actor->unk_6A, (s16)args.unk_00 + attack.unk_00);
        attack.unk_00 = 0;
    }
    attack.unk_07 = actor->unk_72 / 2;
    attack.unk_06 = attack.unk_05 = actor->unk_70;
    actor->flags |= 0x8000;
    result = func_ov003_0208a164(&attack, &args, actor->position.x, actor->position.y,
                                 actor->position.z - ((actor->unk_72 / 2) << 12));
    actor->flags &= ~0x8000;
    if (actor->unk_00 == 2) {
        s16 hp = actor->currentHp;

        if (hp > 0 && result == 1) {
            actor->currentHp = hp - 1;
            if (actor->currentHp <= 0) {
                CombatActor_SetPendingCommand(actor, 1);
                actor->flags |= 4;
            }
        }
    }
    return result;
}

s32 func_ov003_0208a3ec(Ov003Hit* spawn) {
    u16 kind;

    if (data_ov003_020e71b8->unk3D874 != 2) {
        return 0;
    }
    kind = spawn->kind;
    switch (kind) {
        case 1:
            return func_ov003_02089884((Ov003Hit*)spawn);
        case 2:
            if (((u32)(spawn->info->flags << 0x18) >> 0x1F) == 0) {
                return func_ov003_02089974((Ov003Hit*)spawn);
            }
            return func_ov003_02089b04((Ov003Hit*)spawn);
        case 3:
            if (((u32)(spawn->info->flags << 0x18) >> 0x1F) == 0) {
                return func_ov003_02089c74((Ov003Hit*)spawn);
            }
            return func_ov003_02089e18((Ov003Hit*)spawn);
        default:
            return 0;
    }
}
