#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEffects.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

// The shared battle-effect task (`Tsk_BtlEff`): the sprite-plus-world-position record every ov003
// effect task owns, its four-stage runner, and the hit-effect spawn wrapper. The per-effect units
// (`BtlEff_Hit`, `BtlEff_Sword`, `BtlEff_Dead`, `BtlEff_Warp`) reuse the same record shape and the
// same stage-table convention.

/// The parameters `func_ov003_0209e9f8` hands to the new task.
typedef struct BtlEffArgs {
    /* 0x00 */ s32 mode; // display engine: 1 = sub screen, 0 = main screen
    /* 0x04 */ u8  id;   // index into `data_ov003_020d8208`
    /* 0x05 */ u8  unk_05[3];
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 z;
    /* 0x14 */ s32 flip;
} BtlEffArgs; // Size: 0x18

/// Named battle sprite-page identifiers.
extern const BinIdentifier data_ov003_020d77d8, data_ov003_020d77f8;

extern s32 func_ov003_020843b0(s32 engine, s32 x);
extern s32 func_ov003_020843ec(s32 engine, s32 y, s32 z);

// Defined in this unit.

char data_ov003_020e3d84[0xC] = "Tsk_BtlEff";

const TaskHandle data_ov003_020d81ec = {data_ov003_020e3d84, func_ov003_0209e990, sizeof(CombatEffect)};

const TaskStages data_ov003_020d81f8 = {
    func_ov003_0209e754,
    func_ov003_0209e864,
    func_ov003_0209e890,
    func_ov003_0209e928,
};

const CombatEffectParams data_ov003_020d8208[26] = {
    {{0x32, 0x34, 0x33, 0}, 4, 0x40, 1, 0, 0},
    {{0x38, 0x3A, 0x39, 0}, 4, 0x24, 1, 0, 0},
};

/// Loads the effect sprite from the record `args->id` selects, positions it and accounts for it in
/// the per-screen live effect count. Returns 0 when the sprite could not be loaded.
// Nonmatching: select named resource objects instead of indexing across separate objects.
s32 func_ov003_0209e754(TaskPool* pool, Task* task, void* args) {
    CombatEffect*             eff = task->data;
    BtlEffArgs*               p   = args;
    const CombatEffectParams* def;

    MI_CpuSet(eff, 0, sizeof(CombatEffect));

    def             = &data_ov003_020d8208[p->id];
    eff->position.x = p->x;
    eff->position.y = p->y;
    eff->position.z = p->z;
    eff->active     = 1;

    CombatSprite_LoadFromTable(p->mode, &eff->sprite, (def->pack == 4 ? &data_ov003_020d77f8 : &data_ov003_020d77d8),
                               &def->anim, 0, def->paletteStart, def->paletteCount);
    if (eff->sprite.flags46 & 0x8000) {
        return 0;
    }

    CombatSprite_SetAnimFromTable(&eff->sprite, 0, 1);
    CombatSprite_SetFlip(&eff->sprite, p->flip);

    if (p->mode != 1) {
        data_ov003_020e71b8->unk3D8FE = (s8)data_ov003_020e71b8->unk3D8FE + 1;
    } else {
        data_ov003_020e71b8->unk3D8FF = (s8)data_ov003_020e71b8->unk3D8FF + 1;
    }
    return 1;
}

/// Advances the effect and stops it once its animation has played out.
s32 func_ov003_0209e864(TaskPool* pool, Task* task, void* args) {
    CombatEffect* eff = task->data;

    if (SpriteMgr_IsAnimationFinished(&eff->sprite.sprite) != 0) {
        eff->active = 0;
    }
    CombatSprite_Update(&eff->sprite);
    return eff->active;
}

/// Draws the effect at its world position, depth-sorted by Y.
// Nonmatching: the sort key stays in r5 instead of r6 (mwcc schedules the subtraction after the
// SetPosition argument copy); the instruction sequence is otherwise identical.
s32 func_ov003_0209e890(TaskPool* pool, Task* task, void* args) {
    CombatEffect* eff = task->data;
    s32           posX;
    s32           posY;
    s32           sortKey;

    if (eff->sprite.sprite.bits_0_1 != 1) {
        posX = func_ov003_020843b0(0, eff->position.x);
        posY = func_ov003_020843ec(0, eff->position.y, eff->position.z);
    } else {
        posX = func_ov003_020843b0(1, eff->position.x);
        posY = func_ov003_020843ec(1, eff->position.y, eff->position.z);
    }
    sortKey = 0x7FFFFFFF - (eff->position.y + 0x8000);
    CombatSprite_SetPosition(&eff->sprite, posX, posY);
    func_ov003_02082730(&eff->sprite, sortKey);
    CombatSprite_Render(&eff->sprite);
    return 1;
}

/// Releases the effect sprite and drops it from the per-screen live effect count.
s32 func_ov003_0209e928(TaskPool* pool, Task* task, void* args) {
    CombatEffect* eff = task->data;

    if (eff->sprite.sprite.bits_0_1 != 1) {
        data_ov003_020e71b8->unk3D8FE = (s8)data_ov003_020e71b8->unk3D8FE - 1;
    } else {
        data_ov003_020e71b8->unk3D8FF = (s8)data_ov003_020e71b8->unk3D8FF - 1;
    }
    CombatSprite_Release(&eff->sprite);
    return 1;
}

/// Task dispatcher: runs `stage` of the effect's stage table unless the encounter cutscene is
/// running, which freezes every effect task.
s32 func_ov003_0209e990(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d81f8;

    if (func_ov003_0208b690((u16)stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns a hit effect of id `id` on the display engine `mode` at world position (`x`, `y`, `z`),
/// flipped to match `flip`. Ignored once ten effects are already live on that screen.
void func_ov003_0209e9f8(s32 mode, s32 id, s32 x, s32 y, s32 z, s32 flip) {
    BtlEffArgs args;
    s32        live;

    if (mode != 1) {
        live = (s8)data_ov003_020e71b8->unk3D8FE;
    } else {
        live = (s8)data_ov003_020e71b8->unk3D8FF;
    }
    // The retail code folds the room test back into the same variable.
    live = live < 10;
    if (live == 0) {
        return;
    }

    args.mode = mode;
    args.id   = id;
    args.x    = x;
    args.y    = y;
    args.z    = z;
    args.flip = flip;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d81ec, NULL, 0, NULL, &args);
}
