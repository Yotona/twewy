#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          unk_60;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ u8           pad_6C[0x4];
} Boss01s_LandRG; // Size: 0x70

s32 Boss01s_LandRG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov016_02129054 = {1, 3, 2, 1};

extern const BinIdentifier data_ov016_02128f98;
static const TaskHandle    Tsk_Boss01s_LandRG = {"Tsk_Boss01s_LandRG", Boss01s_LandRG_RunTask, 0x70};

s32 Boss01s_LandRG_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandRG* rg = task->data;

    MI_CpuSet(rg, 0, sizeof(Boss01s_LandRG));
    CombatSprite_LoadFromTable(1, &rg->sprite, &data_ov016_02128f98, &data_ov016_02129054, 0, 4, 0x3E);
    rg->unk_60 = data_ov003_020e71b8->unk3D824 / 2;
    rg->unk_64 = 0;
    rg->unk_68 = -0x4B000;
    return 1;
}

s32 Boss01s_LandRG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandRG* rg = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    CombatSprite_Update(&rg->sprite);
    return 1;
}

s32 Boss01s_LandRG_Render(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandRG* rg = task->data;
    s16             spA, sp8;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(rg), &spA, &sp8, rg->unk_60, rg->unk_64, rg->unk_68);
    CombatSprite_SetPosition(&rg->sprite, spA, sp8);
    func_ov003_02082730(&rg->sprite, 0x7FFFFFFF);
    CombatSprite_Render(&rg->sprite);
    return 1;
}

s32 Boss01s_LandRG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_LandRG* rg = task->data;

    CombatSprite_Release(&rg->sprite);
    return 1;
}

s32 Boss01s_LandRG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129068 = {
        .initialize = Boss01s_LandRG_Init,
        .update     = Boss01s_LandRG_Update,
        .render     = Boss01s_LandRG_Render,
        .cleanup    = Boss01s_LandRG_Destroy,
    };
    return data_ov016_02129068.iter[stage](pool, task, args);
}

void Boss01s_LandRG_CreateTask(void) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_Boss01s_LandRG, 0, 0, 0, NULL);
}
