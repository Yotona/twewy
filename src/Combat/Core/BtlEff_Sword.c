#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEffects.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"

#include <nitro/mi/cpumem.h>

// The sword-slash battle effect: a single one-shot sprite spawned at the hit position by
// `ov003_02087b7c` and played back to completion. Its task data is a bare `CombatSprite`, so the
// spawn only carries the animation (slash direction) to play.

/// The sword effect's spawn parameter, handed to the init stage by `EasyTask_CreateTask`.
typedef struct BtlEffSwordSpawn {
    /* 0x00 */ s16 animIndex;
} BtlEffSwordSpawn;

extern s32 func_ov003_020843b0(s32 engine, s32 x);
extern s32 func_ov003_020843ec(s32 engine, s32 y, s32 z);

extern const BinIdentifier data_ov003_020d77d8;

extern const TaskHandle      data_ov003_020d8634;
extern const TaskStages      data_ov003_020d8640;
extern const SpriteAnimEntry data_ov003_020d8650[];

/// Task init: clears the effect sprite and loads the slash animation named by the spawn parameter.
s32 func_ov003_0209ee18(TaskPool* pool, Task* task, void* args) {
    CombatSprite*     sprite = task->data;
    BtlEffSwordSpawn* spawn  = args;

    MI_CpuSet(sprite, 0, sizeof(CombatSprite));
    CombatSprite_LoadFromTable(0, sprite, &data_ov003_020d77d8, data_ov003_020d8650, spawn->animIndex, 0x3A, 0x68);
    CombatSprite_SetAnimFromTable(sprite, spawn->animIndex, 1);
    return (sprite->flags46 & 0x8000) ? 0 : 1;
}

/// Task update: retires the effect once the battle leaves the effect-visible state, the hit-stun
/// is over, or the slash animation has finished.
s32 func_ov003_0209ee94(TaskPool* pool, Task* task, void* args) {
    CombatSprite* sprite = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x40) {
        return 1;
    }
    if (func_ov003_020876c0() != 0) {
        return 0;
    }
    if (SpriteMgr_IsAnimationFinished(&sprite->sprite) != 0) {
        return 0;
    }
    CombatSprite_Update(sprite);
    return 1;
}

/// Task render: pins the slash to the main display's target actor and sorts it in front of it.
s32 func_ov003_0209eef0(TaskPool* pool, Task* task, void* args) {
    CombatSprite* sprite = task->data;
    CombatActor*  actor  = data_ov003_020e71b8->unk3D898;
    s32           posX;
    s32           posY;

    if (actor->isFlipped == 0) {
        CombatSprite_SetFlip(sprite, 0);
    } else {
        CombatSprite_SetFlip(sprite, 1);
    }
    posX = func_ov003_020843b0(0, actor->position.x);
    posY = func_ov003_020843ec(0, actor->position.y, actor->position.z);
    CombatSprite_SetPosition(sprite, posX, posY);
    func_ov003_02082730(sprite, 0x7FFFFFFF - actor->position.y);
    CombatSprite_Render(sprite);
    return 1;
}

/// Task cleanup: releases the slash sprite.
s32 func_ov003_0209ef84(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

/// Four-stage task dispatcher.
s32 func_ov003_0209ef98(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8640;

    if (func_ov003_0208b690((u16)stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns the sword-slash effect playing `animIndex`.
void func_ov003_0209f000(s16 animIndex) {
    // The spawn parameter is handed over by address, so keep a local copy of it rather than
    // taking the address of an incoming register argument.
    s16 anim = animIndex;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &data_ov003_020d8634, NULL, 0, NULL, &anim);
}

char data_ov003_020e3da0[0x14] = "Tsk_BtlEff_Sword";

const TaskHandle data_ov003_020d8634 = {data_ov003_020e3da0, func_ov003_0209ef98, sizeof(CombatSprite)};

const TaskStages data_ov003_020d8640 = {
    func_ov003_0209ee18,
    func_ov003_0209ee94,
    func_ov003_0209eef0,
    func_ov003_0209ef84,
};

const SpriteAnimEntry data_ov003_020d8650[10] = {
    {0x3B, 0x3D, 0x3C, 0x0},
    {0x3E, 0x40, 0x3F, 0x0},
    {0x41, 0x43, 0x42, 0x0},
    {0x44, 0x46, 0x45, 0x0},
    {0x44, 0x46, 0x45, 0x4},
    {0x47, 0x49, 0x48, 0x1},
    {0x4A, 0x4C, 0x4B, 0x0},
    {0x4D, 0x4F, 0x4E, 0x0},
    {0x50, 0x52, 0x51, 0x2},
    {0x53, 0x55, 0x54, 0x0},
};
