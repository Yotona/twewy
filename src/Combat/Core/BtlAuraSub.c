#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAura.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

// The aura on the sub screen (`Tsk_BtlAuraSub`): sixteen small sprites that drift up out of the
// ground around the owner's world position, fade with a shared sort palette and disappear once the
// aura's timer runs out. The record is `CombatSprite`-shaped at the front because the `CombatSprite`
// helpers drive the whole task data; the flames themselves live in the 0x60..0x760 range.

// External data.

/// The battle sprite sheet every flame is loaded from.
extern const BinIdentifier data_ov003_020d7800;

// External functions.

extern void func_020265d4(s32* dst, s32 target, u16 weight);
extern void func_ov003_02082a04(s32 engine, Sprite* sprite, const BinIdentifier* iden, const SpriteAnimEntry* table, u16 entry,
                                s32 palette, s32 paletteCount);
extern void func_ov003_02082a90(s32 engine, Sprite* sprite, const BinIdentifier* iden, u16 entry, u16 arg4, u16 arg5,
                                s32 palette, s32 anim);
extern void func_ov003_0208b2fc(s32* x, s32* y, s32 angle, s32 scale);

extern void func_ov003_020e3c38(); // aura delegate that lives in a sibling unit

s32 func_ov003_0209727c(TaskPool* pool, Task* task, void* args);
s32 func_ov003_02097364(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020973e4(TaskPool* pool, Task* task, void* args);
s32 func_ov003_02097470(TaskPool* pool, Task* task, void* args);

s32 func_ov003_02097490(TaskPool* pool, Task* task, void* args, s32 stage);

// Defined in this unit.

/// `TaskHandle.taskName` of `data_ov003_020d7ed0`.
char data_ov003_020e3c44[0x10] = "Tsk_BtlAuraSub";

/// The animation entry `func_ov003_0209727c` loads the burst sprite from.
const SpriteAnimEntry data_ov003_020d7ec8[1] = {
    {0x10, 0x12, 0x11, 0x01},
};

/// `Tsk_BtlAuraSub`.
/// The four stage functions `func_ov003_02097490` dispatches on.
const TaskStages data_ov003_020d7edc = {
    func_ov003_0209727c,
    func_ov003_02097364,
    func_ov003_020973e4,
    func_ov003_02097470,
};

const TaskHandle data_ov003_020d7ed0 = {data_ov003_020e3c44, func_ov003_02097490, 0x774};

/// Gives every flame the same start position, a random angle and spread around the aura, a random
/// drift speed and a random first animation.
void func_ov003_0209703c(s32 engine, BtlAuraSub* aura) {
    s32           i;
    s32           palette;
    u32           spread;
    s16           anim;
    BtlAuraFlame* flame;

    i      = 0;
    spread = RNG_Next(0x100) << 8;
    if (engine == 0) {
        palette = 0x3E;
    } else {
        palette = 0x40;
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
        func_ov003_02082a90(engine, &flame->sprite.sprite, &data_ov003_020d7800, 0x13, 0x15, 0x14, palette, anim);
        CombatSprite_SetAnim((CombatSprite*)flame, (u16)anim, 1);
        i++;
        spread += 0x1000;
        flame++;
    } while (i < 16);
}

/// Advances the still-running flames and records whether any of them is left.
void func_ov003_02097158(BtlAuraSub* aura) {
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

/// Draws every still-running flame, depth-sorted against the shared aura palette.
void func_ov003_020971c0(BtlAuraSub* aura) {
    s32              i;
    s32              sortKey;
    Ov003CutinActor* owner;
    BtlAuraFlame*    flame;

    i     = 0;
    flame = aura->flames;
    do {
        if (flame->sprite.sprite.bits_0_1 == 0) {
            owner = data_ov003_020e71b8->unk3D898;
        } else {
            owner = data_ov003_020e71b8->unk3D89C;
        }
        sortKey = 0x80000000 - owner->base.actor.position.y;
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
void func_ov003_02097254(BtlAuraSub* aura) {
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
s32 func_ov003_0209727c(TaskPool* pool, Task* task, void* args) {
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
        palette = 0x40;
    } else {
        engine = 0;
        aura->y -= 0xC0000;
        palette = 0x3E;
    }
    func_ov003_02082a04(engine, &aura->sprite.sprite, &data_ov003_020d7800, data_ov003_020d7ec8, 1, palette, 0x90);
    CombatSprite_SetPosition(&aura->sprite, (s32)(aura->x * 0x10) >> 16, (s32)(aura->y * 0x10) >> 16);
    func_ov003_0209703c(engine, aura);
    func_ov003_02087f00(SEIDX_SE_GREEN_LIGHT, (s32)(aura->x * 0x10) >> 16);
    return 1;
}

/// Task stage 1: runs the descent timer, updates the flames and reports whether the aura is done.
s32 func_ov003_02097364(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub* aura = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x40) {
        return 1;
    }
    if ((s32)aura->timer <= 0) {
        aura->sortKey = 0;
    } else {
        func_020265d4(&aura->sortKey, 0x1000, (u16)aura->timer);
        aura->timer = (s16)(aura->timer - 1);
    }
    CombatSprite_Update(&aura->sprite);
    func_ov003_02097158(aura);
    return aura->active;
}

/// Task stage 2: draws the burst that follows the aura down while its timer runs, then the flames.
s32 func_ov003_020973e4(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub*      aura = task->data;
    Ov003CutinActor* owner;

    if (aura->sortKey > 0) {
        if (aura->sprite.sprite.bits_0_1 == 0) {
            owner = data_ov003_020e71b8->unk3D898;
        } else {
            owner = data_ov003_020e71b8->unk3D89C;
        }
        CombatSprite_SetAffineTransform(&aura->sprite, 0, aura->sortKey, aura->sortKey, 1);
        func_ov003_02082730(&aura->sprite, 0x80000000 - owner->base.actor.position.y);
        CombatSprite_Render(&aura->sprite);
    }
    func_ov003_020971c0(aura);
    return 1;
}

/// Task stage 3: releases the burst sprite and the flames.
s32 func_ov003_02097470(TaskPool* pool, Task* task, void* args) {
    BtlAuraSub* aura = task->data;

    CombatSprite_Release(&aura->sprite);
    func_ov003_02097254(aura);
    return 1;
}

/// Task dispatcher: runs `stage` of the aura's stage table unless the encounter cutscene is running.
s32 func_ov003_02097490(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7edc;

    if (func_ov003_0208b690((u16)stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns the sub-screen aura at world position (`x`, `y`).
void func_ov003_020974f8(s32 x, s32 y) {
    BtlAuraArgs args;

    args.x = x;
    args.y = y;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7ed0, NULL, 0, NULL, &args);
}
