#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAura.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

// The aura on the enemy screen (`Tsk_BtlEnmAuraSub`): the same sixteen-flame record as the player's
// aura, but tinted with the enemy palette, driven by the enemy's aura state and fading out through
// the shared enemy sort palette.

/// The enemy's screen-space aura state at `data_ov003_020e7324`, shared with `BtlEnmAura`.
// External data.

/// The enemy aura sprite sheet every flame is loaded from.
extern const BinIdentifier data_ov003_020e0b90;

// External functions.

extern void func_020265d4(void* dst, s32 step, u16 count);
extern void func_ov003_02082a04(s32 engine, Sprite* sprite, const BinIdentifier* iden, const SpriteAnimEntry* table, u16 entry,
                                s32 palette, s32 paletteCount);
extern void func_ov003_02082a90(s32 engine, Sprite* sprite, const BinIdentifier* iden, u16 entry, u16 arg4, u16 arg5,
                                s32 palette, s32 anim);
extern void func_ov003_0208b2fc(s32* x, s32* y, s32 height, s32 speed);
extern s32  func_ov003_020c37f8(CombatSprite* sprite);  // whether the enemy's aura is charged
extern s32  func_ov003_020c3c28();                      // whether the enemy is mid-attack
extern s16  func_ov003_020c42ec(Ov003EnemyActor* unit); // the unit's attack value

s32 func_ov003_020ca9e4(TaskPool*, Task*, void*);
s32 func_ov003_020caacc(TaskPool*, Task*, void*);
s32 func_ov003_020cab4c(TaskPool*, Task*, void*);
s32 func_ov003_020cabd0(TaskPool*, Task*, void*);

s32 func_ov003_020cabf0(TaskPool*, Task*, void*, s32);

// Defined in this unit.

/// `TaskHandle.taskName` of `data_ov003_020e0bd4`.
char data_ov003_020e6ba0[0x14] = "Tsk_BtlEnmAuraSub";

/// The enemy aura sheet, named for the task-handle table.
char data_ov003_020e6bb4[0x1C] = "Apl_Mor/Grp_BtlEnmAura.bin";

/// The animation entry `func_ov003_020ca9e4` loads the burst sprite from.
const SpriteAnimEntry data_ov003_020e0bc4[1] = {
    {0x01, 0x03, 0x02, 0x01},
};
const BinIdentifier data_ov003_020e0bcc = {3, data_ov003_020e6bb4};

/// The enemy aura sprite sheet the flames are loaded from.

/// `Tsk_BtlEnmAuraSub`.
/// The four stage functions `func_ov003_020cabf0` dispatches on.
const TaskStages data_ov003_020e0be0 = {
    func_ov003_020ca9e4,
    func_ov003_020caacc,
    func_ov003_020cab4c,
    func_ov003_020cabd0,
};

const TaskHandle data_ov003_020e0bd4 = {data_ov003_020e6ba0, func_ov003_020cabf0, 0x774};

/// Sets the enemy aura state to `mode` unless the aura is already engaged this frame.
void func_ov003_020ca6a8(Ov003CutinActor* unit) {
    if (data_ov003_020e7324.unk_16 > 0) {
        return;
    }
    if (func_ov003_020c37f8(&unit->sprite) != 0) {
        if (data_ov003_020e7324.ready != 0 && data_ov003_020e7324.activeMain == 0) {
            return;
        }
        data_ov003_020e7324.mode = 2;
        return;
    }
    if (data_ov003_020e7324.ready != 0 && data_ov003_020e7324.activeSub == 0) {
        return;
    }
    data_ov003_020e7324.mode = 1;
}

/// The unit's attack value, scaled by the aura's per-cent scale while the aura is engaged.
s16 func_ov003_020ca728(Ov003CutinActor* unit) {
    s16 value;

    if ((func_ov003_020c37f8(&unit->sprite) != 0 && data_ov003_020e7324.activeMain != 0) ||
        (func_ov003_020c37f8(&unit->sprite) == 0 && data_ov003_020e7324.activeSub != 0))
    {
        value = func_ov003_020c42ec(&unit->base);
        return (s16)(data_ov003_020e7324.scale * value / 100);
    }
    return func_ov003_020c42ec(&unit->base);
}

/// Gives every flame the same start position, a random angle and spread around the aura, a random
/// drift speed and a random first animation.
void func_ov003_020ca7b0(s32 engine, BtlAuraSub* aura) {
    s32           i;
    s32           palette;
    u32           spread;
    s16           anim;
    BtlAuraFlame* flame;

    i      = 0;
    spread = RNG_Next(0x100) << 8;
    if (engine == 0) {
        palette = 7;
    } else {
        palette = 8;
    }
    flame = aura->flames;
    do {
        flame->x      = aura->x;
        flame->y      = aura->y;
        flame->height = spread + (RNG_Next(0x100) << 8) / 32;
        func_ov003_0208b2fc(&flame->x, &flame->y, flame->height, (RNG_Next(9) + 0x18) << 12);
        if (i & 1) {
            flame->riseSpeed = 0;
            anim             = RNG_Next(3) + 2;
        } else {
            flame->riseSpeed = (RNG_Next(3) + 1) << 12;
            anim             = RNG_Next(2);
        }
        func_ov003_02082a90(engine, &flame->sprite.sprite, &data_ov003_020e0b90, 4, 6, 5, palette, anim);
        CombatSprite_SetAnim((CombatSprite*)flame, (u16)anim, 1);
        i++;
        spread += 0x1000;
        flame++;
    } while (i < 16);
}

/// Advances the still-running flames and records whether any of them is left.
void func_ov003_020ca8cc(BtlAuraSub* aura) {
    BtlAuraFlame* flame;
    s32           i;
    s32           active;

    active = 0;
    i      = 0;
    flame  = aura->flames;
    do {
        if (SpriteMgr_IsAnimationFinished(&flame->sprite.sprite) == 0) {
            if (flame->riseSpeed != 0) {
                func_ov003_0208b2fc(&flame->x, &flame->y, flame->height, flame->riseSpeed);
            }
            CombatSprite_Update((CombatSprite*)flame);
            active = 1;
        }
        i++;
        flame++;
    } while (i < 16);
    aura->active = active;
}

/// Draws every still-running flame, depth-sorted against the shared enemy sort palette.
void func_ov003_020ca934(BtlAuraSub* aura) {
    s32              i;
    s32              sortKey;
    Ov003CutinActor* owner;
    BtlAuraFlame*    flame;

    i     = 0;
    flame = aura->flames;
    do {
        if (flame->sprite.sprite.bits_0_1 == 0) {
            owner = data_ov003_020e7324.ownerMain;
        } else {
            owner = data_ov003_020e7324.ownerSub;
        }
        sortKey = 0x80000000 - owner->actor.position.y;
        if (SpriteMgr_IsAnimationFinished(&flame->sprite.sprite) == 0) {
            CombatSprite_SetPosition(&flame->sprite, (s32)(flame->x * 0x10) >> 16, (s32)(flame->y * 0x10) >> 16);
            func_ov003_02082730((CombatSprite*)flame, sortKey);
            CombatSprite_Render((CombatSprite*)flame);
        }
        i++;
        flame++;
    } while (i < 16);
}

/// Releases the sixteen flame sprites.
void func_ov003_020ca9bc(BtlAuraSub* aura) {
    s32           i;
    BtlAuraFlame* flame;

    i     = 0;
    flame = aura->flames;
    do {
        CombatSprite_Release((CombatSprite*)flame);
        i++;
        flame++;
    } while (i < 16);
}

/// Task stage 0: seeds the record, picks the screen-side palette and spawns the flames.
s32 func_ov003_020ca9e4(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub*  aura = task->data;
    BtlAuraArgs* p    = args;
    s32          engine;
    s32          palette;

    MI_CpuSet(aura, NULL, 0x774);
    engine        = 1;
    aura->active  = 1;
    aura->x       = p->x;
    aura->y       = p->y;
    aura->timer   = 0x14;
    aura->sortKey = 0;
    if (aura->y < 0xC0000) {
        palette = 8;
    } else {
        engine = 0;
        aura->y -= 0xC0000;
        palette = 7;
    }
    func_ov003_02082a04(engine, &aura->sprite.sprite, &data_ov003_020e0bcc, data_ov003_020e0bc4, 0, palette, 0x90);
    CombatSprite_SetPosition(&aura->sprite, (s32)(aura->x * 0x10) >> 16, (s32)(aura->y * 0x10) >> 16);
    func_ov003_020ca7b0(engine, aura);
    func_ov003_02087f00(SEIDX_SE_GREEN_LIGHT, (s32)(aura->x * 0x10) >> 16);
    return 1;
}

/// Task stage 1: runs the descent timer, updates the flames and reports whether the aura is done.
s32 func_ov003_020caacc(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub* aura = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    if ((s32)aura->timer <= 0) {
        aura->sortKey = 0;
    } else {
        func_020265d4(&aura->sortKey, 0x1000, (u16)aura->timer);
        aura->timer = (s16)(aura->timer - 1);
    }
    CombatSprite_Update(&aura->sprite);
    func_ov003_020ca8cc(aura);
    return aura->active;
}

/// Task stage 2: draws the burst that follows the aura down while its timer runs, then the flames.
s32 func_ov003_020cab4c(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub*      aura = task->data;
    Ov003CutinActor* owner;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    if (aura->sortKey > 0) {
        if (aura->sprite.sprite.bits_0_1 == 0) {
            owner = data_ov003_020e7324.ownerMain;
        } else {
            owner = data_ov003_020e7324.ownerSub;
        }
        CombatSprite_SetAffineTransform(&aura->sprite, 0, aura->sortKey, aura->sortKey, 1);
        func_ov003_02082730(&aura->sprite, 0x80000000 - owner->actor.position.y);
        CombatSprite_Render(&aura->sprite);
    }
    func_ov003_020ca934(aura);
    return 1;
}

/// Task stage 3: releases the burst sprite and the flames.
s32 func_ov003_020cabd0(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub* aura = task->data;

    CombatSprite_Release(&aura->sprite);
    func_ov003_020ca9bc(aura);
    return 1;
}

/// Task dispatcher: runs `stage` of the enemy aura's stage table.
s32 func_ov003_020cabf0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020e0be0;

    return stages.iter[stage](pool, task, args);
}

/// Spawns the enemy aura at world position (`x`, `y`).
void func_ov003_020cac38(s32 x, s32 y) {
    BtlAuraArgs args;

    args.x = x;
    args.y = y;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020e0bd4, NULL, 0, NULL, &args);
}
