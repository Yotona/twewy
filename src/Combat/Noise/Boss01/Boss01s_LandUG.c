#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"

/// MARK: Boss01s_LandUG

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          unk_60;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ char         unk_6C[0x78 - 0x6C];
} Boss01s_LandUG; // Size: 0x78

s32 Boss01s_LandUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle      Tsk_Boss01s_LandUG = {"Tsk_Boss01s_LandUG", Boss01s_LandUG_RunTask, 0x78};
static const SpriteAnimEntry LandUG_Anim        = {1, 3, 2, 2};

extern const BinIdentifier data_ov016_02128f98;

s32 Boss01s_LandUG_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandUG* landUG = task->data;

    MI_CpuSet(landUG, 0, sizeof(Boss01s_LandUG));
    CombatSprite_LoadFromTable(0, &landUG->sprite, &data_ov016_02128f98, &LandUG_Anim, 0, 4, 0xC);
    landUG->unk_60 = data_ov003_020e71b8->unk3D7CC / 2;
    landUG->unk_64 = -0x1E000;
    landUG->unk_68 = 0;
    return 1;
}

s32 Boss01s_LandUG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandUG* landUG = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    CombatSprite_Update(&landUG->sprite);
    return 1;
}

s32 Boss01s_LandUG_Render(TaskPool* pool, Task* task, void* args) {
    s16             spA, sp8;
    Boss01s_LandUG* landUG = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(landUG), &spA, &sp8, landUG->unk_60, landUG->unk_64, landUG->unk_68);
    CombatSprite_SetPosition(&landUG->sprite, spA, sp8);
    func_ov003_02082730(&landUG->sprite, 0x7FFFFFFF);
    CombatSprite_Render(&landUG->sprite);
    return 1;
}

s32 Boss01s_LandUG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandUG* landUG = task->data;
    CombatSprite_Release(&landUG->sprite);
    return 1;
}

s32 Boss01s_LandUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_0212908c = {
        .initialize = Boss01s_LandUG_Init,
        .update     = Boss01s_LandUG_Update,
        .render     = Boss01s_LandUG_Render,
        .cleanup    = Boss01s_LandUG_Destroy,
    };
    return data_ov016_0212908c.iter[stage](pool, task, args);
}

void Boss01s_LandUG_CreateTask(void) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_Boss01s_LandUG, NULL, 0, NULL, NULL);
}
