#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
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

// The battle-actor sprite-animation state machine and the attack-queue helpers it shares with the
// psych effect units: 0x70-byte sprite slots, the actor list, and the unit box/segment tests used
// to pick hit targets.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 16-bit status word at 0x196 of a battle unit.

/// A battle unit: the shared `CombatActor` prefix, its list node at 0x178 and the status word.

/// One of the 0x70-byte sprite slots an effect owns: a `CombatSprite` plus its spin/fade timers.

/// One attack-info record (see `ov003_02087b7c.c`); bit 12 of `flags` selects the hit side and
/// bits 19..23 hold the per-call amount.

/// The per-hit arguments of an attack (attack x/y/amount pairs).

/// One row of the encounter-id â†’ kind table at 0x020dc568.
typedef struct Ov003EncounterEntry {
    /* 0x0 */ u32 encounterId;
    /* 0x4 */ u16 unk_04;
    /* 0x6 */ u16 pad;
} Ov003EncounterEntry; // Size: 0x8

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern s32 func_020243d4(u8);

extern BOOL Sprite_HasAnimation(Sprite*);

extern s32 func_ov003_0208442c(s32, s32);
extern s32 func_ov003_02084460(s32, s32, s32);

extern s32  func_ov003_0208a128(const Ov003AttackInfo*, void*, void*, s32);
extern s32  func_ov003_0208a164(const Ov003AttackInfo*, Ov003AttackArgs*, s32, s32, s32);
extern s32  func_ov003_0208a1a4(const Ov003AttackInfo*, void*, s32, s32);
extern void func_ov003_0208a234(u16, Ov003AttackArgs*, void*);
extern s32  func_ov003_0208a25c(void*);

extern void func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

// Defined in this unit.

s32  func_ov003_020a62b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_ov003_020a632c(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_ov003_020a652c(Ov003AttackInfo* info, s32 force, u16 amount);
s32  func_ov003_020a66f4(Ov003AttackStreak* streak, s32 attackId, s32 x, s32 y, s32 force, u16 amount);
s32  func_ov003_020a6c7c(s32 arg0);
s32  func_ov003_020a6cc4(s32 arg0);

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

/// Animation ids handed to `func_ov003_020a3498`, indexed by the caller's set number.
const u32 data_ov003_020dc560[2] = {0xCF, 0xA3};

/// Encounter id â‡’ per-encounter hit kind.
const Ov003EncounterEntry data_ov003_020dc568[11] = {
    {0xA7,  0x56, 0},
    {0xA8,  0x6B, 0},
    {0xA9,  0x63, 0},
    {0xAA,  0x8B, 0},
    {0xAB,  0xBA, 0},
    {0xAC,  0x54, 0},
    {0xAD,  0xC8, 0},
    {0xAE,  0x9D, 0},
    {0xAF, 0x109, 0},
    {0xB0,  0x05, 0},
    {0xB1,  0x3D, 0},
};

// ---------------------------------------------------------------------------------------------
// Sprite-slot state machine
// ---------------------------------------------------------------------------------------------

/// Advances the spin/fade timers of `count` sprite slots and restarts their animations on demand.
// Nonmatching: loop-end/anim-id registers are swapped and the animation id load is hoisted; pure regalloc.
void func_ov003_020a5db8(Ov003AnimSprite* sprites, s32 count, s32 animSet, s32 arg3, s32 arg4, s32 arg5) {
    s32              animId = data_ov003_020dc560[animSet];
    Ov003AnimSprite* sprite;
    Ov003AnimSprite* end = sprites + count;

    for (sprite = sprites; sprite != end; sprite++) {
        if (sprite->timer > 0) {
            sprite->timer--;
            if (sprite->timer > 0) {
                continue;
            }
        }
        if (Sprite_HasAnimation(&sprite->sprite.sprite) == 0) {
            func_ov003_020a3498(&sprite->sprite, 2, 1, animId, 1);
            sprite->animTimer = arg3;
            sprite->angle     = RNG_Next(0x10000);
            sprite->decay     = arg4;
        } else if (sprite->animTimer == 0) {
            sprite->animTimer = arg3;
            sprite->angle     = RNG_Next(0x10000);
            sprite->decay     = arg4;
            CombatSprite_Restart(&sprite->sprite);
        } else {
            sprite->animTimer -= sprite->decay;
            if (sprite->animTimer < 0) {
                sprite->animTimer = 0;
            }
            sprite->decay += arg5;
        }
    }
    for (sprite = sprites; sprite != end; sprite++) {
        if (Sprite_HasAnimation(&sprite->sprite.sprite) != 0) {
            CombatSprite_Update(&sprite->sprite);
        }
    }
}

/// Spins the live sprite slots around (x, y) and renders them.
// Nonmatching: the index/angle/speed locals are scheduled into different registers around the affine call.
void func_ov003_020a5ed4(Ov003AnimSprite* sprites, s32 count, s32 x, s32 y) {
    Ov003AnimSprite* sprite;
    Ov003AnimSprite* end = sprites + count;

    for (sprite = sprites; sprite != end; sprite++) {
        if (Sprite_HasAnimation(&sprite->sprite.sprite) != 0) {
            u16 angle = sprite->angle;
            s32 speed = sprite->animTimer;
            s32 idx   = (angle >> 4) * 2;

            CombatSprite_SetAffineTransform(&sprite->sprite, (u16)(angle + 0x4000), 0x1000, 0x1000, 1);
            CombatSprite_SetPosition(&sprite->sprite, (s16)(x + (FX_Mul(FX_SinCosTable_[idx + 1], speed) >> 12)),
                                     (s16)(y + (FX_Mul(FX_SinCosTable_[idx], speed) >> 12)));
            func_ov003_02082730(&sprite->sprite, 0x385000);
            CombatSprite_Render(&sprite->sprite);
        }
    }
}

/// Releases the live sprite slots.
void func_ov003_020a5fc0(Ov003AnimSprite* sprites, s32 count) {
    Ov003AnimSprite* end = sprites + count;
    Ov003AnimSprite* sprite;

    for (sprite = sprites; sprite != end; sprite++) {
        if (Sprite_HasAnimation(&sprite->sprite.sprite) != 0) {
            CombatSprite_Release(&sprite->sprite);
        }
    }
}

// ---------------------------------------------------------------------------------------------
// Task pool helpers
// ---------------------------------------------------------------------------------------------

s32 func_ov003_020a6000(const TaskHandle* handle, void* arg) {
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_201A4, handle, NULL, 0, NULL, arg);
}

BOOL func_ov003_020a603c(u32* id) {
    return EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_201A4, id);
}

void func_ov003_020a6060(u32 id) {
    EasyTask_DeleteTask(&data_ov003_020e71b8->unk_201A4, id);
}

void* func_ov003_020a6084(u32 id) {
    return EasyTask_GetTaskData(&data_ov003_020e71b8->unk_201A4, id);
}

// ---------------------------------------------------------------------------------------------
// Unit command state
// ---------------------------------------------------------------------------------------------

/// Whether `unit` can take the pending command tagged `arg1`.
s32 func_ov003_020a60a8(Ov003EnemyActor* unit, s32 arg1) {
    s32 flags = unit->actor.flags;

    if ((flags & 2) || (flags & 0x10) || (flags & 4)) {
        return 0;
    }
    if (!(flags & 0x200)) {
        return 0;
    }
    if ((u16)unit->actor.unk_58 == arg1) {
        return 1;
    }
    return 0;
}

/// Whether `unit` can be given `arg1`; the busy/idle and status bits gate it.
s32 func_ov003_020a60ec(Ov003EnemyActor* unit, s32 arg1) {
    s32 flags = unit->actor.flags;

    if ((flags & 2) || (flags & 0x10) || (flags & 4)) {
        return 0;
    }
    if (unit->actor.unk_00 == 1 && (unit->flags196.bits.b0 || unit->flags196.bits.b8)) {
        return 0;
    }
    if (!(flags & 0x200)) {
        return 1;
    }
    if ((u16)unit->actor.unk_58 < arg1) {
        return 1;
    }
    return 0;
}

/// Applies pending command 4 to `unit` and records `arg1`.
s32 func_ov003_020a6164(Ov003EnemyActor* unit, u16 arg1) {
    if (func_ov003_020a60ec(unit, arg1) == 0) {
        return 0;
    }
    CombatActor_SetPendingCommand(&unit->actor, 4);
    unit->actor.unk_58 = arg1;
    return 1;
}

/// Applies pending command 5 to `unit` and clears its velocity.
void func_ov003_020a6198(Ov003EnemyActor* unit, s32 arg1) {
    if (func_ov003_020a60a8(unit, arg1) == 0) {
        return;
    }
    CombatActor_SetPendingCommand(&unit->actor, 5);
    unit->actor.unk_58 = 0;
    func_ov003_02082fe0(&unit->actor);
    unit->actor.xVelocity = 0;
    unit->actor.yVelocity = 0;
    unit->actor.zVelocity = 0;
    unit->actor.zGravity  = 0x400;
}

/// Applies pending command 5 and launches `unit` at `speed` in direction `angle`.
void func_ov003_020a61e4(Ov003EnemyActor* unit, u16 arg1, s32 attackId, Ov003AttackArgs* args, s32 angle, s32 speed,
                         s32 zVelocity) {
    s32 idx;

    if (func_ov003_020a60ec(unit, arg1) == 0) {
        return;
    }
    CombatActor_SetPendingCommand(&unit->actor, 5);
    unit->actor.unk_58 = 0;
    func_ov003_0208a234((u16)attackId, args, unit);
    idx                   = (angle >> 4) * 2;
    unit->actor.xVelocity = FX_Mul(FX_SinCosTable_[idx + 1], speed);
    unit->actor.yVelocity = FX_Mul(FX_SinCosTable_[idx], speed);
    unit->actor.zVelocity = 0;
    unit->actor.zGravity  = 0x400;
    unit->actor.zVelocity = zVelocity;
}

// ---------------------------------------------------------------------------------------------
// Geometry helpers
// ---------------------------------------------------------------------------------------------

BtlTouchWork* func_ov003_020a62a0(void) {
    return &data_ov003_020e71b8->touchWork;
}

s32 func_ov003_020a62b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return FX_Atan2Idx(arg3 - arg1, arg2 - arg0);
}

s32 func_ov003_020a62d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return FX_Atan2Idx((arg3 - arg1) << 12, (arg2 - arg0) << 12);
}

s32 func_ov003_020a62ec(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 dx = arg2 - arg0;
    s32 dy = arg3 - arg1;

    return FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
}

s32 func_ov003_020a632c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 dx = (arg2 - arg0) << 12;
    s32 dy = (arg3 - arg1) << 12;

    return FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
}

/// Whether a (0..0x100, 0..0xC0) point lies inside a square of half-size `arg2`.
s32 func_ov003_020a6374(s32 arg0, s32 arg1, s32 arg2) {
    s32 lo = -arg2;

    if (arg0 >= lo && arg1 >= lo) {
        if (arg0 <= arg2 + 0x100 && arg1 <= arg2 + 0xC0) {
            goto ret_true;
        }
    }
    return 0;
ret_true:
    return 1;
}

/// Whether the current battle lets the effect run.
s32 func_ov003_020a63a8(Ov003EffectState* effect) {
    func_ov003_020a6438();
    if (func_ov003_020a3edc(func_ov003_020a5a34(effect)) == 0 || func_ov003_020876c0() != 0) {
        return 1;
    }
    return 0;
}

/// Whether the battle is in the post-boss state that unlocks the extra effects.
s32 func_ov003_020a63e4(void) {
    if (func_ov003_02085cfc(6) == 0) {
        return 0;
    }
    if (func_ov003_02085d3c(1) != 0) {
        return 0;
    }
    if (data_ov003_020e71b8->unk3D3C4 > 0xB) {
        return 1;
    }
    return 0;
}

/// The player's battle actor.
CombatActor* func_ov003_020a6438(void) {
    return (CombatActor*)data_ov003_020e71b8->unk3D898;
}

void func_ov003_020a6450(void) {
    func_ov003_020a6438()->unk_5A = 0xA;
}

void func_ov003_020a6464(s32 arg0) {
    data_ov003_020e71b8->unk3D8C8 = arg0;
}

void func_ov003_020a647c(s32 arg0) {
    data_ov003_020e71b8->unk3D8CC = arg0;
}

void func_ov003_020a6494(s16 arg0) {
    func_ov003_020a647c(func_ov003_0208442c(0, arg0));
}

void func_ov003_020a64ac(void) {
    data_ov003_020e71b8->unk3D8CC = func_ov003_020a6438()->position.x;
}

/// Initialises the effect's sprite anchor from the owning touch actor.
void func_ov003_020a64d0(Ov003EffectState* effect, s16 arg1) {
    CombatActor*     player = func_ov003_020a6438();
    Ov003TouchActor* actor  = func_ov003_020a5a34(effect);

    effect->unk_0C = (s16)(player->unk_66 + func_ov003_020a4778(&actor->params, actor->unk_00C));
    effect->unk_10 = arg1;
    effect->unk_12 = player->unk_0A;
    effect->unk_0E = (s16)actor->params.unk_2F;
    func_ov003_020a6bb8(&effect->unk_0C, (Ov003EnemyActor*)actor);
}

/// Overrides the info record's side bit and amount field.
void func_ov003_020a652c(Ov003AttackInfo* info, s32 force, u16 amount) {
    if (force != 0) {
        info->flags = (info->flags & ~0x1000) | (((u32)force << 31) >> 19);
    }
    if (amount != 0) {
        info->flags = (info->flags & ~0xF80000) | (((u32)amount << 27) >> 8);
    }
}

/// Queues a sweep attack against `unit`, resetting the streak when the info record says so.
s32 func_ov003_020a6568(Ov003AttackStreak* streak, s32 attackId, Ov003EnemyActor* unit, s32 x, s32 force, u16 amount) {
    Ov003AttackInfo info = *func_ov003_0208a114(attackId);
    s32             result;

    if (streak->maxHits != 0xFF && streak->hits >= (s32)streak->maxHits) {
        force = 1;
    }
    func_ov003_020a652c(&info, force, amount);
    result = func_ov003_0208a128(&info, &streak->args, unit, x);
    if (result == 1) {
        streak->hits++;
        if (((u32)info.flags << 19) >> 31) {
            streak->hits = 0;
            func_ov003_0209fc88(2, unit->actor.position.x, unit->actor.position.y, unit->actor.position.z, 0x10, 0);
        }
    }
    return result;
}

/// Queues a point attack at (x, y, z) and resets the streak when the info record says so.
s32 func_ov003_020a6628(Ov003AttackStreak* streak, s32 attackId, s32 x, s32 y, s32 z, s32 force, u16 amount) {
    Ov003AttackInfo info = *func_ov003_0208a114(attackId);
    s32             result;

    if (streak->maxHits != 0xFF && streak->hits >= (s32)streak->maxHits) {
        force = 1;
    }
    func_ov003_020a652c(&info, force, amount);
    result = func_ov003_0208a164(&info, &streak->args, x, y, z);
    if (result == 1) {
        streak->hits++;
        if (((u32)info.flags << 19) >> 31) {
            streak->hits = 0;
            func_ov003_0209fc88(2, x, y, z, 0x10, 0);
        }
    }
    return result;
}

/// Queues a pixel-space attack at (x, y) and resets the streak when the info record says so.
s32 func_ov003_020a66f4(Ov003AttackStreak* streak, s32 attackId, s32 x, s32 y, s32 force, u16 amount) {
    Ov003AttackInfo info = *func_ov003_0208a114(attackId);
    s32             result;

    if (streak->maxHits != 0xFF && streak->hits >= (s32)streak->maxHits) {
        force = 1;
    }
    func_ov003_020a652c(&info, force, amount);
    result = func_ov003_0208a1a4(&info, &streak->args, x, y);
    if (result == 1) {
        streak->hits++;
        if (((u32)info.flags << 19) >> 31) {
            streak->hits = 0;
            s32 posY     = func_ov003_02084460(0, y, 0);
            s32 posX     = func_ov003_0208442c(0, x);

            func_ov003_0209fc88(2, posX, posY, 0, 0x10, 0);
        }
    }
    return result;
}

/// Segment-segment intersection test; both segments are axis-aligned bounding boxes first.
// Nonmatching: the d < 0 acceptance arm is emitted with inverted branch polarity.
s32 func_ov003_020a67d4(s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 x4, s32 y4) {
    s32 lo;
    s32 hi;
    s32 d;
    s32 t1;
    s32 t2;

    if (x3 < x4) {
        lo = x3;
    } else {
        lo = x4;
    }
    if (x1 > x2) {
        hi = x1;
    } else {
        hi = x2;
    }
    if (hi < lo) {
        return 0;
    }
    if (x3 > x4) {
        lo = x3;
    } else {
        lo = x4;
    }
    if (x1 < x2) {
        hi = x1;
    } else {
        hi = x2;
    }
    if (hi > lo) {
        return 0;
    }
    if (y3 < y4) {
        lo = y3;
    } else {
        lo = y4;
    }
    if (y1 > y2) {
        hi = y1;
    } else {
        hi = y2;
    }
    if (hi < lo) {
        return 0;
    }
    if (y3 > y4) {
        lo = y3;
    } else {
        lo = y4;
    }
    if (y1 < y2) {
        hi = y1;
    } else {
        hi = y2;
    }
    if (hi > lo) {
        return 0;
    }
    d = (x2 - x1) * (y4 - y3) - (y2 - y1) * (x4 - x3);
    if (d == 0) {
        return 0;
    }
    t1 = (y1 - y3) * (x4 - x3) - (x1 - x3) * (y4 - y3);
    t2 = (y1 - y3) * (x2 - x1) - (x1 - x3) * (y2 - y1);
    if (d <= 0 || t1 < 0 || t1 > d) {
        if (d < 0) {
            if (d <= t1 && t1 <= 0) {
                goto block_t2;
            }
        }
        return 0;
    }
block_t2:
    if (d <= 0 || t2 < 0 || t2 > d) {
        if (d < 0) {
            if (d <= t2 && t2 <= 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

/// Whether `unit`'s screen-space bounding box overlaps (x1, y1)-(x2, y2).
s32 func_ov003_020a6924(Ov003EnemyActor* unit, s32 x1, s32 y1, s32 x2, s32 y2) {
    s16 halfW   = unit->actor.unk_70;
    s16 halfH   = unit->actor.unk_72;
    s16 centerX = unit->actor.screenX;
    s16 centerY = unit->actor.screenY;
    s32 left    = centerX - halfW;
    s32 right   = centerX + halfW;
    s32 top     = centerY - halfH;

    if (left > x2 && left > x1) {
        return 0;
    }
    if (right < x2 && right < x1) {
        return 0;
    }
    if (top > y2 && top > y1) {
        return 0;
    }
    if (centerY < y2 && centerY < y1) {
        return 0;
    }
    if (left < (x1 < x2 ? x1 : x2)) {
        if ((x1 > x2 ? x1 : x2) < right) {
            if (top < (y1 < y2 ? y1 : y2)) {
                if ((y1 > y2 ? y1 : y2) < centerY) {
                    return 1;
                }
            }
        }
    }
    if (func_ov003_020a67d4(x1, y1, x2, y2, left, top, right, top) != 0) {
        return 1;
    }
    if (func_ov003_020a67d4(x1, y1, x2, y2, left, centerY, right, centerY) != 0) {
        return 1;
    }
    if (func_ov003_020a67d4(x1, y1, x2, y2, left, top, left, centerY) != 0) {
        return 1;
    }
    if (func_ov003_020a67d4(x1, y1, x2, y2, right, top, right, centerY) != 0) {
        return 1;
    }
    return 0;
}

/// Queues a sweep attack against every unit whose box overlaps (x1, y1)-(x2, y2).
s32 func_ov003_020a6acc(Ov003AttackStreak* streak, s32 attackId, s32 x1, s32 y1, s32 x2, s32 y2, s32 x) {
    Ov003EnemyActor* unit;
    s32              hit = 0;

    for (unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4); unit != NULL; unit = func_ov003_02084984(&unit->node)) {
        if (func_ov003_020a6924(unit, x1, y1, x2, y2) != 0 && func_ov003_020a6568(streak, attackId, unit, x, 0, 0) == 1) {
            hit = 1;
        }
    }
    return hit;
}

/// Plays the streak's current attack from `streak->args` and restores the unit's X velocity.
s32 func_ov003_020a6b84(Ov003AttackStreak* streak, u16 arg1, Ov003EnemyActor* unit) {
    s32 velocity = unit->actor.xVelocity;

    unit->actor.xVelocity = 0x1000;
    func_ov003_0208a234(arg1, &streak->args, unit);
    s32 result            = func_ov003_0208a25c(unit);
    unit->actor.xVelocity = velocity;
    return result;
}

/// Clears `*out` when the current encounter uses the same hit kind as `unit`.
void func_ov003_020a6bb8(s16* out, Ov003EnemyActor* unit) {
    u32 i;

    for (i = 0; i < 0xB; i++) {
        if (data_ov003_020e71b8->unk3D88E == data_ov003_020dc568[i].encounterId &&
            (u16)unit->actor.unk_0A != data_ov003_020dc568[i].unk_04)
        {
            *out = 0;
            return;
        }
    }
}

// ---------------------------------------------------------------------------------------------
// Story progress checks
// ---------------------------------------------------------------------------------------------

/// Story-progress gate for the pin effects: 0 = none, 1 = early, 2 = late.
// Nonmatching: the first early-return is if-converted into moveq/ldmeq instead of a branch.
s32 func_ov003_020a6c14(void) {
    s32 mode = gSaveData.unk_3138;
    s32 area;

    if (mode == 0x28 || mode == 0x2A) {
        return 0;
    }
    if (Inventory_GetOwnedCount(0x2AC) != 0) {
        area = gSaveData.currentArea;
        if (area < 0x15) {
            return 1;
        }
        if (area >= 0x16 && area <= 0x22) {
            return 2;
        }
    }
    return 0;
}

/// The brand-ranking slot of `arg0` in the current area (12 when absent).
// Nonmatching: mwcc strength-reduces `ranking[i]` into a pointer increment; the retail build indexes.
s32 func_ov003_020a6c7c(s32 arg0) {
    BrandTrend* trends = gSaveData.brandTrends;
    s32         i;

    for (i = 0; i <= 12; i++) {
        if (arg0 == trends[gSaveData.currentArea].ranking[i]) {
            return i;
        }
    }
    return 12;
}

/// Whether pin brand `arg0` may be used in the current area at this story step.
s32 func_ov003_020a6cc4(s32 arg0) {
    if (arg0 >= 0xD) {
        return 0;
    }
    switch (func_ov003_020a6c14()) {
        default:
        case 0:
            return 0;
        case 1:
            return 0;
        case 2:
            return arg0 != func_020243d4(gSaveData.currentArea);
    }
    return 0;
}
