#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern s32           data_ov003_020e71e0;
extern BinIdentifier data_ov003_020d77e0;

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          unk_60;
    /* 0x64 */ s16          posX;
    /* 0x66 */ s16          posY;
} Feat00_EffDoll; // Size: 0x68

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s16 animTableIndex;
    /* 0x8 */ s32 unk_8;
    /* 0xC */ s16 posX;
    /* 0xE */ s16 posY;
} Feat00_EffDoll_Args;

s32 Feat00_EffDoll_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov004_020f0480[3] = {
    {0x2D, 0x2F, 0x2E, 0},
    {0x2D, 0x2F, 0x2E, 1},
    {0x2D, 0x2F, 0x2E, 2}
};

static const TaskHandle Tsk_Feat00_EffDoll = {"Tsk_Feat00_EffDoll", Feat00_EffDoll_RunTask, sizeof(Feat00_EffDoll)};

s32 Feat00_EffDoll_Init(TaskPool* pool, Task* task, void* args) {
    Feat00_EffDoll*      doll     = task->data;
    Feat00_EffDoll_Args* dollArgs = args;

    MI_CpuSet(doll, 0, sizeof(Feat00_EffDoll));
    CombatSprite_LoadFromTable(dollArgs->unk_0, &doll->sprite, &data_ov003_020d77e0, data_ov004_020f0480,
                               dollArgs->animTableIndex, 63, 68);
    CombatSprite_SetAnimFromTable(&doll->sprite, dollArgs->animTableIndex, 1);

    doll->posX = dollArgs->posX;
    doll->posY = dollArgs->posY;

    doll->unk_60 = (dollArgs->unk_8 == 0);
    return 1;
}

s32 Feat00_EffDoll_Update(TaskPool* pool, Task* task, void* args) {
    Feat00_EffDoll* doll = task->data;

    if (SpriteMgr_IsAnimationFinished(&doll->sprite.sprite)) {
        return 0;
    }

    CombatSprite_Update(&doll->sprite);

    if (data_ov003_020e71e0 != 1) {
        return 1;
    }
    return 0;
}

s32 Feat00_EffDoll_Render(TaskPool* pool, Task* task, void* args) {
    Feat00_EffDoll* doll = task->data;

    if (doll->unk_60 != 0) {
        CombatSprite_SetFlip(&doll->sprite, FALSE);
    } else {
        CombatSprite_SetFlip(&doll->sprite, TRUE);
    }
    CombatSprite_SetPosition(&doll->sprite, doll->posX, doll->posY);
    func_ov003_02082730(&doll->sprite, 0x7FFFFFFF - doll->posY);
    CombatSprite_Render(&doll->sprite);
    return 1;
}

s32 Feat00_EffDoll_Destroy(TaskPool* pool, Task* task, void* args) {
    Feat00_EffDoll* doll = task->data;

    CombatSprite_Release(&doll->sprite);
    return 1;
}

s32 Feat00_EffDoll_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Feat00_EffDoll_Init,
        .update     = Feat00_EffDoll_Update,
        .render     = Feat00_EffDoll_Render,
        .cleanup    = Feat00_EffDoll_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void Feat00_EffDoll_CreateTask(s32 arg0, s16 animIndex, s16 posX, s16 posY, s32 arg4, s32 arg5) {
    Feat00_EffDoll_Args args;
    args.unk_0          = arg0;
    args.animTableIndex = animIndex;
    args.posX           = posX;
    args.posY           = posY;
    args.unk_8          = arg5;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &Tsk_Feat00_EffDoll, NULL, 0, NULL, &args);
}
