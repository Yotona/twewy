#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEffects.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

// The hit effect (`Tsk_BtlEff_Hit`): the small spark sprite the battle shows where an attack
// landed, plus the SE it plays. Every record of the effect table carries the screen it belongs to,
// the sprite animation it plays and the SE index, so the whole effect is data driven.

/// The parameters `func_ov003_0209ed70` hands to the new task.
typedef struct BtlEff_HitArgs {
    /* 0x00 */ u8  id; // index into the hit-effect table
    /* 0x01 */ u8  unk_01[3];
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s32 flip;
} BtlEff_HitArgs; // Size: 0x14

/// Named battle sprite-page identifiers.
extern const BinIdentifier data_ov003_020d77f8;

extern s32 func_ov003_020843b0(s32 engine, s32 x);
extern s32 func_ov003_020843ec(s32 engine, s32 y, s32 z);

// Defined in this unit.

char data_ov003_020e3d90[0x10] = "Tsk_BtlEff_Hit";

const TaskHandle data_ov003_020d8410 = {data_ov003_020e3d90, func_ov003_0209ed08, sizeof(CombatHitEffect)};

const TaskStages data_ov003_020d841c = {
    func_ov003_0209ea94,
    func_ov003_0209ebd0,
    func_ov003_0209ebfc,
    func_ov003_0209ec9c,
};

// One real array owns all 26 records, including the retail interior symbol
// data_ov003_020d8434. Keep indexed accesses within this declared object.
const CombatHitParams data_ov003_020d842c[26] = {
    {  {8, 10, 9, 0}, 1, 4, 64,  1, 113},
    {{11, 13, 12, 0}, 1, 4, 80,  1, 114},
    {{20, 22, 21, 0}, 1, 4, 64,  1, 115},
    {{23, 25, 24, 0}, 1, 4, 72,  1, 116},
    {{32, 34, 33, 0}, 1, 4, 32,  1, 117},
    {{35, 37, 36, 0}, 1, 4, 64,  1, 118},
    {{44, 46, 45, 0}, 1, 4, 16,  1, 119},
    {{47, 49, 48, 0}, 1, 4, 80,  1, 120},
    {   {2, 4, 3, 0}, 2, 4, 64,  1, 113},
    {   {5, 7, 6, 0}, 2, 4, 80,  1, 114},
    {{14, 16, 15, 0}, 2, 4, 64,  1, 115},
    {{17, 19, 18, 0}, 2, 4, 72,  1, 116},
    {{26, 28, 27, 0}, 2, 4, 32,  1, 117},
    {{29, 31, 30, 0}, 2, 4, 64,  1, 118},
    {{38, 40, 39, 0}, 2, 4, 16,  1, 119},
    {{41, 43, 42, 0}, 2, 4, 80,  1, 120},
    {{59, 61, 60, 0}, 2, 4, 32, 62, 121},
    {{63, 65, 64, 0}, 2, 4, 64, 66, 122},
    {{67, 69, 68, 0}, 2, 4, 32, 70, 123},
    {{71, 73, 72, 0}, 2, 4, 40, 74, 124},
    {{75, 77, 76, 0}, 2, 4, 36, 78, 125},
    {{79, 81, 80, 0}, 2, 4, 32, 82, 126},
    {{83, 85, 84, 0}, 2, 4, 32, 86, 127},
    {{87, 89, 88, 0}, 2, 4, 36, 90, 128},
    {{91, 93, 92, 0}, 2, 4, 40, 94, 129},
    {{95, 97, 96, 0}, 2, 4, 36, 98, 130},
};

/// Loads the hit sprite from record `args->id`, positions it, plays its SE and accounts for it in
/// the per-screen live effect count. Returns 0 when the sprite could not be loaded.
// Nonmatching: select the named effect resource object instead of cross-object indexing.
s32 func_ov003_0209ea94(TaskPool* pool, Task* task, void* args) {
    CombatHitEffect*       eff = task->data;
    BtlEff_HitArgs*        p   = args;
    const CombatHitParams* def;

    MI_CpuSet(eff, 0, sizeof(CombatHitEffect));

    eff->effect.active     = 1;
    eff->kind              = p->id;
    eff->effect.position.x = p->x;
    eff->effect.position.y = p->y;
    eff->effect.position.z = p->z;
    def                    = &data_ov003_020d842c[p->id];

    CombatSprite_LoadFromTable(def->engine, &eff->effect.sprite, &data_ov003_020d77f8, &def->anim, 0, def->paletteStart,
                               def->paletteCount);
    if (eff->effect.sprite.flags46 & 0x8000) {
        return 0;
    }

    CombatSprite_SetAnimFromTable(&eff->effect.sprite, 0, 1);
    CombatSprite_SetFlip(&eff->effect.sprite, p->flip);

    if (eff->effect.sprite.sprite.bits_0_1 == 1) {
        data_ov003_020e71b8->unk3D8FF = (s8)data_ov003_020e71b8->unk3D8FF + 1;
        func_ov003_02087f64(def->sound, eff->effect.position.x);
    } else {
        data_ov003_020e71b8->unk3D8FE = (s8)data_ov003_020e71b8->unk3D8FE + 1;
        func_ov003_02087f28(def->sound, eff->effect.position.x);
    }
    return 1;
}

/// Advances the hit sprite and stops it once its animation has played out.
s32 func_ov003_0209ebd0(TaskPool* pool, Task* task, void* args) {
    CombatHitEffect* eff = task->data;

    if (SpriteMgr_IsAnimationFinished(&eff->effect.sprite.sprite) != 0) {
        eff->effect.active = 0;
    }
    CombatSprite_Update(&eff->effect.sprite);
    return eff->effect.active;
}

/// Draws the hit sprite at its world position, depth-sorted by Y. The main-screen half is drawn a
/// little lower than its projected Y.
s32 func_ov003_0209ebfc(TaskPool* pool, Task* task, void* args) {
    CombatHitEffect* eff = task->data;
    s32              posX;
    s32              posY;
    s32              sortKey;

    if (eff->effect.sprite.sprite.bits_0_1 == 1) {
        posX    = func_ov003_020843b0(1, eff->effect.position.x);
        posY    = func_ov003_020843ec(1, eff->effect.position.y, eff->effect.position.z);
        sortKey = 0x7FFFFFFF - (eff->effect.position.y + 0x8000);
    } else {
        posX    = func_ov003_020843b0(0, eff->effect.position.x);
        posY    = func_ov003_020843ec(0, eff->effect.position.y, eff->effect.position.z);
        sortKey = posY + 8;
    }
    CombatSprite_SetPosition(&eff->effect.sprite, posX, posY);
    func_ov003_02082730(&eff->effect.sprite, sortKey);
    CombatSprite_Render(&eff->effect.sprite);
    return 1;
}

/// Releases the hit sprite and drops it from the per-screen live effect count.
s32 func_ov003_0209ec9c(TaskPool* pool, Task* task, void* args) {
    CombatHitEffect* eff = task->data;

    if (eff->effect.sprite.sprite.bits_0_1 == 1) {
        data_ov003_020e71b8->unk3D8FF = (s8)data_ov003_020e71b8->unk3D8FF - 1;
    } else {
        data_ov003_020e71b8->unk3D8FE = (s8)data_ov003_020e71b8->unk3D8FE - 1;
    }
    CombatSprite_Release(&eff->effect.sprite);
    return 1;
}

/// Task dispatcher: runs `stage` of the hit effect's stage table unless the encounter cutscene is
/// running, which freezes every effect task.
s32 func_ov003_0209ed08(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d841c;

    if (func_ov003_0208b690((u16)stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns hit effect `id` at world position (`x`, `y`, `z`), flipped to match `flip`. Ignored once
/// ten effects are already live on the screen the record belongs to.
// Nonmatching: the real array owns the interior symbol formerly named data_ov003_020d8434.
void func_ov003_0209ed70(u8 id, s32 x, s32 y, s32 z, s32 flip) {
    BtlEff_HitArgs args;
    s32            live;

    if (data_ov003_020d842c[id].engine == 1) {
        live = (s8)data_ov003_020e71b8->unk3D8FF;
    } else {
        live = (s8)data_ov003_020e71b8->unk3D8FE;
    }
    // The retail code folds the room test back into the same variable.
    live = live < 10;
    if (live == 0) {
        return;
    }

    args.id   = id;
    args.x    = x;
    args.y    = y;
    args.z    = z;
    args.flip = flip;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d8410, NULL, 0, NULL, &args);
}
