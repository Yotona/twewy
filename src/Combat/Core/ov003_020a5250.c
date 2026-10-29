#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
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

// The touch-action "effect" runtime: the state block at 0xCC of every touch actor, the per-effect
// behaviour table driven through it, and the battle-actor physics integrator that pushes the touch
// actors around. The actors themselves are created by `ov003_020a3b9c.c` and wired up here.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// One entry of the per-effect behaviour table at 0x020dc254; `animId` is the sprite animation
/// `func_ov003_020a5660` returns to its caller.
typedef union Ov003EffectFlags {
    s32 raw;
    struct {
        s32 flag : 1;
        s32 rest : 31;
    } bits;
} Ov003EffectFlags;

typedef struct Ov003EffectEntry {
    /* 0x0 */ s32 (*fn)(void*, void*);
    /* 0x4 */ u16              animId;
    /* 0x6 */ u16              pad;
    /* 0x8 */ Ov003EffectFlags flags;
} Ov003EffectEntry; // Size: 0xC

/// The task/lifetime hooks of an effect state.

/// The 0x3C-byte effect block at 0xCC of a touch actor, driven by `ov003_020a5250.c`.

/// The touch-slot origin/destination block at `Ov003Global + 0x3D358`.

/// One of the 0x70-byte sprite slots an effect owns: a `CombatSprite` plus its spin/fade timers.

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern u32  func_0200f93c(void*);
extern void func_020274e4(void*, s32, s32, const BinIdentifier*);

// Defined in this unit.
void              func_ov003_020a5540(Ov003EffectState* effect, Ov003TouchActor* actor);
void              func_ov003_020a563c(Ov003EffectState* effect);
void              func_ov003_020a58e4(Ov003EffectState* effect, u16 arg1, u8 arg2);
Ov003EffectEntry* func_ov003_020a5d74(s32 state);

extern s32 func_ov003_020a62b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/// The effect entry points referenced by `data_ov003_020dc254`; only their addresses are used.
extern s32 func_ov003_020af590();
extern s32 func_ov003_020af280();
extern s32 func_ov003_020af41c();
extern s32 func_ov003_020b87a0();
extern s32 func_ov003_020b8c1c();
extern s32 func_ov003_020b7bf4();
extern s32 func_ov003_020b7f9c();
extern s32 func_ov003_020b8398();
extern s32 func_ov003_020b2710();
extern s32 func_ov003_020b1fd4();
extern s32 func_ov003_020b94b8();
extern s32 func_ov003_020b9838();
extern s32 func_ov003_020afdf0();
extern s32 func_ov003_020b0058();
extern s32 func_ov003_020ade84();
extern s32 func_ov003_020ad720();
extern s32 func_ov003_020b15dc();
extern s32 func_ov003_020b0cd4();
extern s32 func_ov003_020b10d8();
extern s32 func_ov003_020a9a58();
extern s32 func_ov003_020a9830();
extern s32 func_ov003_020ab1c4();
extern s32 func_ov003_020ab35c();
extern s32 func_ov003_020aaa84();
extern s32 func_ov003_020aaa9c();
extern s32 func_ov003_020aaab8();
extern s32 func_ov003_020b05b8();
extern s32 func_ov003_020b0b14();
extern s32 func_ov003_02094cf8();
extern s32 func_ov003_02094d4c();
extern s32 func_ov003_020950ac();
extern s32 func_ov003_020ac4f4();
extern s32 func_ov003_020ac1ec();
extern s32 func_ov003_020ac36c();
extern s32 func_ov003_02087e5c();
extern s32 func_ov003_020ba714();
extern s32 func_ov003_02095350();
extern s32 func_ov003_0209588c();
extern s32 func_ov003_02087c28();
extern s32 func_ov003_02095ac8();
extern s32 func_ov003_02095cac();
extern s32 func_ov003_02096048();
extern s32 func_ov003_02095e64();
extern s32 func_ov003_02087c98();
extern s32 func_ov003_02087d1c();
extern s32 func_ov003_020b5f74();
extern s32 func_ov003_020b5d54();
extern s32 func_ov003_020b537c();
extern s32 func_ov003_020b47fc();
extern s32 func_ov003_020b4b60();
extern s32 func_ov003_020b3da8();
extern s32 func_ov003_020b37c8();
extern s32 func_ov003_020b39c0();
extern s32 func_ov003_020baa58();
extern s32 func_ov003_020ba2e8();
extern s32 func_ov003_020b6644();
extern s32 func_ov003_020b66b4();
extern s32 func_ov003_020b292c();
extern s32 func_ov003_020ac634();
extern s32 func_ov003_020ac6bc();
extern s32 func_ov003_020a9ec0();
extern s32 func_ov003_020b6368();
extern s32 func_ov003_020b641c();
extern s32 func_ov003_020b6498();
extern s32 func_ov003_020ba6c8();

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

/// PsyData.bin pack identifier.
extern char         data_ov003_020e48cc[0x14];
const BinIdentifier data_ov003_020dc234 = {3, data_ov003_020e48cc};

/// Effect priority per psych kind, indexed by `actor->params.unk_30`; lower wins.
const u8 data_ov003_020dc23c[0x18] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

/// Per-effect behaviour table; `func_ov003_020a5d74` indexes it with the effect state.
const Ov003EffectEntry data_ov003_020dc254[65] = {
    {func_ov003_020af590,  0x2, 0x0, 0x1},
    {func_ov003_020af280,  0x2, 0x0, 0x1},
    {func_ov003_020af41c,  0x2, 0x0, 0x1},
    {func_ov003_020b87a0,  0x4, 0x0, 0x1},
    {func_ov003_020b8c1c,  0x4, 0x0, 0x1},
    {func_ov003_020b7bf4,  0x4, 0x0, 0x1},
    {func_ov003_020b7f9c,  0x4, 0x0, 0x1},
    {func_ov003_020b8398,  0x4, 0x0, 0x1},
    {func_ov003_020b2710,  0x4, 0x0, 0x0},
    {func_ov003_020b1fd4,  0x4, 0x0, 0x0},
    {func_ov003_020b94b8,  0x4, 0x0, 0x0},
    {func_ov003_020b9838,  0x4, 0x0, 0x1},
    {func_ov003_020afdf0,  0x2, 0x0, 0x0},
    {func_ov003_020b0058,  0x2, 0x0, 0x0},
    {func_ov003_020ade84,  0x2, 0x0, 0x0},
    {func_ov003_020ad720,  0x2, 0x0, 0x0},
    {func_ov003_020b15dc,  0x2, 0x0, 0x0},
    {func_ov003_020b0cd4,  0x2, 0x0, 0x0},
    {func_ov003_020b10d8,  0x2, 0x0, 0x0},
    {func_ov003_020a9a58,  0x2, 0x0, 0x0},
    {func_ov003_020a9830,  0x2, 0x0, 0x0},
    {func_ov003_020ab1c4,  0x2, 0x0, 0x0},
    {func_ov003_020ab35c,  0x2, 0x0, 0x0},
    {func_ov003_020aaa84,  0x2, 0x0, 0x1},
    {func_ov003_020aaa9c,  0x2, 0x0, 0x1},
    {func_ov003_020aaab8,  0x2, 0x0, 0x1},
    {func_ov003_020b05b8,  0x5, 0x0, 0x0},
    {func_ov003_020b0b14,  0x5, 0x0, 0x0},
    {func_ov003_02094cf8,  0xE, 0x0, 0x0},
    {func_ov003_02094d4c,  0xE, 0x0, 0x0},
    {func_ov003_020950ac,  0xF, 0x0, 0x0},
    {func_ov003_020ac4f4,  0x2, 0x0, 0x0},
    {func_ov003_020ac1ec,  0x2, 0x0, 0x0},
    {func_ov003_020ac36c,  0x2, 0x0, 0x0},
    {func_ov003_02087e5c, 0x10, 0x0, 0x0},
    {func_ov003_020ba714,  0x2, 0x0, 0x0},
    {func_ov003_02095350,  0x1, 0x0, 0x0},
    {func_ov003_0209588c,  0xC, 0x0, 0x0},
    {func_ov003_02087c28,  0x6, 0x0, 0x0},
    {func_ov003_02095ac8,  0x7, 0x0, 0x0},
    {func_ov003_02095cac,  0xD, 0x0, 0x0},
    {func_ov003_02096048,  0x8, 0x0, 0x0},
    {func_ov003_02095e64,  0x9, 0x0, 0x0},
    {func_ov003_02087c98,  0xA, 0x0, 0x0},
    {func_ov003_02087d1c,  0xB, 0x0, 0x0},
    {func_ov003_020b5f74,  0x2, 0x0, 0x0},
    {func_ov003_020b5d54,  0x2, 0x0, 0x0},
    {func_ov003_020b537c,  0x2, 0x0, 0x0},
    {func_ov003_020b47fc,  0x5, 0x0, 0x0},
    {func_ov003_020b4b60,  0x5, 0x0, 0x0},
    {func_ov003_020b3da8,  0x2, 0x0, 0x0},
    {func_ov003_020b37c8,  0x2, 0x0, 0x0},
    {func_ov003_020b39c0,  0x2, 0x0, 0x0},
    {func_ov003_020baa58,  0x4, 0x0, 0x0},
    {func_ov003_020ba2e8,  0x5, 0x0, 0x0},
    {func_ov003_020b6644,  0x3, 0x0, 0x0},
    {func_ov003_020b66b4,  0x3, 0x0, 0x0},
    {func_ov003_020b292c,  0x4, 0x0, 0x0},
    {func_ov003_020ac634,  0x4, 0x0, 0x1},
    {func_ov003_020ac6bc,  0x4, 0x0, 0x1},
    {func_ov003_020a9ec0,  0x0, 0x0, 0x0},
    {func_ov003_020b6368,  0x0, 0x0, 0x0},
    {func_ov003_020b641c,  0x0, 0x0, 0x0},
    {func_ov003_020b6498,  0x0, 0x0, 0x0},
    {func_ov003_020ba6c8,  0x4, 0x0, 0x0},
};

/// Per-actor approach speeds (20.12); ramped up while an effect is close to its origin.
s32 data_ov003_020e48bc[4] = {0x333, 0x333, 0x333, 0x333};

/// PsyData.bin path (referenced by `data_ov003_020dc234`).
char data_ov003_020e48cc[0x14] = "Data/PsyData.bin";

// ---------------------------------------------------------------------------------------------
// Physics integrator
// ---------------------------------------------------------------------------------------------

/// Integrates one touch actor's velocity, bouncing it off the battle bounds and pushing it away
/// from overlapping actors; also ramps the effect speed of actors near the effect origin.
void func_ov003_020a5250(Ov003TouchActor* actor) {
    Ov003TouchActor* other;
    BtlTouchWork*    origin;
    s32              angle;

    if (func_ov003_02085cfc(2) != 0) {
        origin = func_ov003_020a62a0();
        if (func_ov003_020a62ec(origin->x << 12, origin->y << 12, actor->x, actor->y) < 0xF000) {
            angle            = (func_ov003_020a62b8(origin->x << 12, origin->y << 12, actor->x, actor->y) >> 4) * 2;
            actor->velocityX = FX_Mul(FX_SinCosTable_[angle + 1], 0x1000);
            actor->velocityY = FX_Mul(FX_SinCosTable_[angle], 0x1000);
            data_ov003_020e48bc[actor->effect.index] += 0x19A;
            if (data_ov003_020e48bc[actor->effect.index] > 0xA000) {
                data_ov003_020e48bc[actor->effect.index] = 0xA000;
            }
        }
    }

    actor->x += FX_Mul(data_ov003_020e48bc[actor->effect.index], actor->velocityX);
    actor->y += FX_Mul(data_ov003_020e48bc[actor->effect.index], actor->velocityY);
    data_ov003_020e48bc[actor->effect.index] -= 4;
    if (data_ov003_020e48bc[actor->effect.index] < 0x333) {
        data_ov003_020e48bc[actor->effect.index] = 0x333;
    }

    if (actor->x < (actor->radius << 12)) {
        actor->x         = actor->radius << 12;
        actor->velocityX = -actor->velocityX;
    } else if (actor->x > ((0x100 - actor->radius) << 12)) {
        actor->x         = (0x100 - actor->radius) << 12;
        actor->velocityX = -actor->velocityX;
    }

    if (actor->y < (actor->radius << 12)) {
        actor->y         = actor->radius << 12;
        actor->velocityY = -actor->velocityY;
    } else if (actor->y > ((0xC0 - actor->radius) << 12)) {
        actor->y         = (0xC0 - actor->radius) << 12;
        actor->velocityY = -actor->velocityY;
    }

    for (other = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC); other != NULL; other = func_ov003_02084984(&other->node))
    {
        if (other != actor &&
            func_ov003_020a62ec(other->x, other->y, actor->x, actor->y) < ((other->radius + actor->radius) << 12))
        {
            angle            = (func_ov003_020a62b8(other->x, other->y, actor->x, actor->y) >> 4) * 2;
            actor->velocityX = FX_Mul(FX_SinCosTable_[angle + 1], 0x1000);
            actor->velocityY = FX_Mul(FX_SinCosTable_[angle], 0x1000);
        }
    }
}

/// Runs one step of an effect's start/update callbacks through its behaviour table.
void func_ov003_020a5540(Ov003EffectState* effect, Ov003TouchActor* actor) {
    Ov003EffectEntry* entry;

    if (actor->state != 3 || effect->state == 0xFFFF) {
        return;
    }
    effect->result           = 0;
    effect->context.callback = NULL;
    effect->context.handle   = NULL;
    effect->context.post     = NULL;
    if (effect->context.arg != NULL) {
        func_ov003_020a5d50(effect->context.arg);
        effect->context.arg = NULL;
    }
    if (func_0200f93c(&data_ov003_020e71b8->unk_201A4) > 0x80U) {
        return;
    }
    entry = func_ov003_020a5d74(effect->state);
    if (entry->flags.bits.flag && (func_ov003_020a6438()->position.z < 0)) {
        return;
    }
    effect->result = entry->fn(&effect->context, effect);
    if (effect->result != 0) {
        actor->unk_052 = 0xF;
    }
    if (effect->context.post != NULL) {
        effect->result = effect->context.post(effect->context.arg);
    }
    if (effect->state == 0x3C && actor->state == 3) {
        func_ov003_020a5250(actor);
    }
}

/// Releases the memory an effect allocated when it started.
void func_ov003_020a563c(Ov003EffectState* effect) {
    if (effect->context.arg == NULL) {
        return;
    }
    func_ov003_020a5d50(effect->context.arg);
    effect->context.arg = NULL;
}

/// Starts effect `effect`: resets its per-frame counters and spawns its task/callbacks.
u16 func_ov003_020a5660(Ov003TouchActor* actor) {
    Ov003EffectEntry* entry = func_ov003_020a5d74(actor->effect.state);

    actor->effect.unk_08 = 0;
    actor->effect.flag   = (u8)actor->params.unk_2E;
    actor->unk_054       = 1;
    func_ov003_020a64d0(&actor->effect, 0x64);
    if (actor->effect.context.callback != NULL) {
        actor->effect.context.callback(actor->effect.context.arg);
    }
    if (actor->effect.context.handle != NULL) {
        func_ov003_020a6000(actor->effect.context.handle, actor->effect.context.arg);
    }
    return entry->animId;
}

/// Whether `actor` should be considered by the touch-effect selection.
s32 func_ov003_020a56c8(Ov003TouchActor* actor) {
    if (func_ov003_020a3edc(actor) == 0) {
        return 0;
    }
    if (actor->params.unk_12 != 0 && actor->unk_044 <= 0) {
        return 0;
    }
    if (actor->state != 3) {
        return 0;
    }
    if (actor->effect.state == 0xFFFF) {
        return 0;
    }
    if (actor->effect.result != 0) {
        return 1;
    }
    return 0;
}

/// Collects the live touch actors into `slots`; `mode != 0` keeps only the two chain-eligible
/// effect kinds. Returns the number collected.
// Nonmatching: the two "store the actor" arms are if-converted into conditional stores here;
// the retail build keeps a separate store block per arm.
s32 func_ov003_020a5738(Ov003TouchActor** slots, s32 count, s32 mode) {
    Ov003TouchActor* actor;
    s32              n = 0;
    s32              i;

    for (i = 0; i < count; i++) {
        slots[i] = NULL;
    }
    for (actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC); actor != NULL; actor = func_ov003_02084984(&actor->node))
    {
        if (actor->kind == 0xFFFE) {
            continue;
        }
        if (func_ov003_020a56c8(actor) == 0) {
            continue;
        }
        if (mode != 0) {
            if (actor->effect.state == 0x3F) {
                slots[n] = actor;
                n++;
            } else if (actor->effect.state != 0x38) {
                continue;
            } else if (actor->effect.index == 0 || actor->effect.index == 3) {
                slots[n] = actor;
                n++;
            }
        } else {
            slots[n] = actor;
            n++;
        }
    }
    return n;
}

/// Picks the highest-priority live touch actor and starts its effect; returns the effect's
/// animation id (0 if nothing was picked).
u16 func_ov003_020a5804(s32 mode) {
    Ov003TouchActor* slots[6];
    s32              count = func_ov003_020a5738(slots, 6, mode);

    if (count > 0) {
        s32 i;
        s32 best;
        s32 bestPriority = 0xFF;

        best = 0;
        for (i = 0; i < count; i++) {
            u8 priority = data_ov003_020dc23c[slots[i]->params.unk_30];
            if (bestPriority > (s32)priority) {
                best         = i;
                bestPriority = priority;
            }
        }
        if (slots[best] != NULL) {
            if (func_ov003_020a59f8() != 0 && slots[best]->params.unk_12 != 0) {
                func_ov003_020a5974(&slots[best]->effect);
            } else {
                return func_ov003_020a5660(slots[best]);
            }
        }
    }
    return 0;
}

void func_ov003_020a58c4(void) {
    func_ov003_020a5804(1);
}

void func_ov003_020a58d4(void) {
    func_ov003_020a5804(0);
}

/// Initialises an effect state from its kind and per-actor index.
void func_ov003_020a58e4(Ov003EffectState* effect, u16 arg1, u8 arg2) {
    effect->state  = arg1;
    effect->index  = arg2;
    effect->unk_08 = 0;
    effect->unk_0A = 0;
    if (effect->state != 0xFFFF) {
        func_020274e4(effect->data, effect->state, effect->index, &data_ov003_020dc234);
    }
    effect->result           = 0;
    effect->context.callback = NULL;
    effect->context.handle   = NULL;
    effect->context.arg      = NULL;
}

/// Records the touch-slot origins used by the effect selection.
void func_ov003_020a593c(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Ov003EffectSlots* slots = &data_ov003_020e71b8->effectSlots;

    slots->unk_00   = arg0;
    slots->unk_04   = arg1;
    slots->unk_08   = arg2;
    slots->unk_0C   = arg3;
    slots->unk_10   = arg4;
    slots->cooldown = 0;
    slots->unk_15   = 0;
}

/// Fires the shared effect for all five touch slots and arms its cooldown.
void func_ov003_020a5974(Ov003EffectState* effect) {
    Ov003EffectSlots* state = &data_ov003_020e71b8->effectSlots;
    s32               i;

    if (state->cooldown != 0) {
        return;
    }
    RNG_Next(3);
    for (i = 0; i <= 4; i++) {
        func_ov003_020a912c(state, effect);
    }
    state->cooldown = 0xA;
    func_ov003_020a5a94(effect);
    func_ov003_020a5aa4(effect);
}

void func_ov003_020a59dc(void) {
    data_ov003_020e71b8->effectSlots.unk_0C = 0;
}

s32 func_ov003_020a59f8(void) {
    if (data_ov003_020e71b8->effectSlots.unk_0C != 0) {
        return 1;
    }
    return 0;
}

u8 func_ov003_020a5a1c(void) {
    return data_ov003_020e71b8->effectSlots.unk_15;
}

/// Finds the touch actor that owns the effect state at `effect`.
Ov003TouchActor* func_ov003_020a5a34(Ov003EffectState* effect) {
    Ov003TouchActor* actor;

    for (actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC); actor != NULL; actor = func_ov003_02084984(&actor->node))
    {
        if (actor->kind != 0xFFFE && &actor->effect == effect) {
            return actor;
        }
    }
    return NULL;
}

void func_ov003_020a5a94(Ov003EffectState* effect) {
    func_ov003_020a40b0(func_ov003_020a5a34(effect));
}

s32 func_ov003_020a5aa4(Ov003EffectState* effect) {
    return func_ov003_020a40bc(func_ov003_020a5a34(effect));
}

s32 func_ov003_020a5ab4(Ov003EffectState* effect) {
    return func_ov003_020a3edc(func_ov003_020a5a34(effect));
}

/// Counts the actors the effect selection collected and compares against the effect's slot count.
s32 func_ov003_020a5ac4(Ov003TouchActor** slots, s32 count, Ov003EffectState* effect, s32 requireReady) {
    s32 i;
    s32 n = 0;

    func_ov003_020a5bac(slots, count, effect, requireReady);
    for (i = 0; i < count; i++) {
        if (slots[i] != NULL) {
            n++;
        }
    }
    return n == func_ov003_020a5b80(effect->state);
}

/// Sums the per-actor amount of every actor running effect `state`.
s32 func_ov003_020a5b18(s32 state) {
    Ov003TouchActor* actor;
    s32              total = 0;

    for (actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC); actor != NULL; actor = func_ov003_02084984(&actor->node))
    {
        if (actor->kind != 0xFFFE && actor->effect.state == state) {
            total += actor->effect.unk_0A;
        }
    }
    return total;
}

/// Number of slots effect `state` occupies.
s32 func_ov003_020a5b80(s32 state) {
    switch (state) {
        case 0x3B:
            return 2;
        case 0x3C:
            return 4;
        default:
            return 0;
    }
}

/// Fills `slots` with the actors participating in `effect`, one per effect slot.
void func_ov003_020a5bac(Ov003TouchActor** slots, s32 count, Ov003EffectState* effect, s32 arg3) {
    Ov003TouchActor* actor;
    s32              i;
    s32              slot;

    for (i = 0; i < count; i++) {
        slots[i] = NULL;
    }
    actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC);
    while (actor != NULL) {
        if (actor->kind == 0xFFFE) {
            actor = func_ov003_02084984(&actor->node);
        } else if (actor->effect.state != effect->state) {
            actor = func_ov003_02084984(&actor->node);
        } else if (actor->effect.index == effect->index && &actor->effect != effect) {
            actor = func_ov003_02084984(&actor->node);
        } else if (func_ov003_020a3edc(actor) == 0) {
            actor = func_ov003_02084984(&actor->node);
        } else if (arg3 != 0) {
            if (actor->params.unk_12 != 0 && actor->unk_044 <= 0) {
                actor = func_ov003_02084984(&actor->node);
            } else if (actor->state != 3) {
                actor = func_ov003_02084984(&actor->node);
            } else {
                goto add;
            }
        } else {
        add:
            slot = actor->effect.index;
            if (slot >= count) {
                actor = func_ov003_02084984(&actor->node);
            } else if (slots[slot] != NULL) {
                actor = func_ov003_02084984(&actor->node);
            } else {
                slots[slot] = actor;
                actor       = func_ov003_02084984(&actor->node);
            }
        }
    }
}

void* func_ov003_020a5d2c(s32 size, const char* name) {
    return Mem_AllocHeapTail(&data_ov003_020e71b8->heap_20224, size);
}

void func_ov003_020a5d50(void* ptr) {
    Mem_Free(&data_ov003_020e71b8->heap_20224, ptr);
}

/// The behaviour table entry for effect `state`.
Ov003EffectEntry* func_ov003_020a5d74(s32 state) {
    return &data_ov003_020dc254[state];
}

/// Staggers the countdown timers of `count` sprite slots.
// Nonmatching: mwcc strength-reduces i * step to an addition; the retail build keeps the mul.
void func_ov003_020a5d88(Ov003AnimSprite* sprites, s32 count, s32 step) {
    Ov003AnimSprite* end = sprites + count;
    s32              i   = 0;

    while (sprites != end) {
        sprites->timer = i * step;
        i++;
        sprites++;
    }
}
