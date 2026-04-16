#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020d77d8;
extern s32           data_ov003_020e71e0;

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          unk_60;
    /* 0x64 */ s16          unk_64;
    /* 0x66 */ s16          unk_66;
} Feat00_Sword; // Size: 0x68

typedef struct {
    /* 0x0 */ s32  unk_0;
    /* 0x4 */ u16  unk_4;
    /* 0x6 */ char unk_6[2];
    /* 0x8 */ s32  unk_8;
    /* 0xC */ s16  unk_C;
    /* 0xE */ s16  unk_E;
} Feat00_Sword_Args;

s32 Feat00_Sword_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov004_020f044c[3] = {
    {0x3B, 0x3D, 0x3C, 0},
    {0x3E, 0x40, 0x3F, 0},
    {0x41, 0x43, 0x42, 0}
};

const TaskHandle Tsk_Feat00_Sword = {"Tsk_Feat00_Sword", Feat00_Sword_RunTask, 0x68};

s32 Feat00_Sword_Init(TaskPool* pool, Task* task, void* args) {
    Feat00_Sword*      sword     = task->data;
    Feat00_Sword_Args* swordArgs = args;

    MI_CpuSet(sword, 0, sizeof(Feat00_Sword));
    CombatSprite_LoadFromTable(swordArgs->unk_0, &sword->sprite, &data_ov003_020d77d8, data_ov004_020f044c,
                               (u16)(s32)swordArgs->unk_4, 58, 68);
    CombatSprite_SetAnimFromTable(&sword->sprite, swordArgs->unk_4, 1);

    sword->unk_64 = swordArgs->unk_C;
    sword->unk_66 = swordArgs->unk_E;
    sword->unk_60 = (swordArgs->unk_8 == 0);
    return 1;
}

s32 Feat00_Sword_Update(TaskPool* pool, Task* task, void* args) {
    Feat00_Sword* sword = task->data;

    if (SpriteMgr_IsAnimationFinished(&sword->sprite.sprite)) {
        return 0;
    }
    CombatSprite_Update(&sword->sprite);
    if (data_ov003_020e71e0 != 1) {
        return 1;
    }
    return 0;
}

s32 Feat00_Sword_Render(TaskPool* pool, Task* task, void* args) {
    Feat00_Sword* sword = task->data;

    if (sword->unk_60 != 0) {
        CombatSprite_SetFlip(&sword->sprite, 0);
    } else {
        CombatSprite_SetFlip(&sword->sprite, 1);
    }
    CombatSprite_SetPosition(&sword->sprite, sword->unk_64, sword->unk_66);
    func_ov003_02082730(&sword->sprite, 0x7FFFFFFF - sword->unk_66);
    CombatSprite_Render(&sword->sprite);
    return 1;
}

s32 Feat00_Sword_Destroy(TaskPool* pool, Task* task, void* args) {
    Feat00_Sword* sword = task->data;

    CombatSprite_Release(&sword->sprite);
    return 1;
}

s32 Feat00_Sword_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Feat00_Sword_Init,
        .update     = Feat00_Sword_Update,
        .render     = Feat00_Sword_Render,
        .cleanup    = Feat00_Sword_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void Feat00_Sword_CreateTask(u32 arg0, u16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5) {
    Feat00_Sword_Args args;
    args.unk_4 = arg1;
    args.unk_C = arg2;
    args.unk_0 = arg0;
    args.unk_E = arg3;
    args.unk_8 = arg5;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &Tsk_Feat00_Sword, NULL, 0, NULL, &args);
}
