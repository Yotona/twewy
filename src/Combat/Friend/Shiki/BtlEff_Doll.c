#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020d77e0;

typedef struct {
    /* 0x00 */ char unk_00[0x24];
    /* 0x24 */ u32  unk_24;
    /* 0x28 */ u32  unk_28;
    /* 0x2C */ u32  unk_2C;
    /* 0x30 */ u32  unk_30;
    /* 0x34 */ char unk_34[0x20];
    /* 0x54 */ u32  unk_54;
} DollUnk;

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ DollUnk*     unk_60;
} BtlEff_Doll; // Size: 0x64

typedef struct {
    /* 0x0 */ u16 unk_0;
    /* 0x2 */ u16 unk_2;
    /* 0x4 */ u32 unk_4;
} BtlEff_Doll_Args;

s32 BtlEff_Doll_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov004_020f0374[5] = {
    {0x2D, 0x2F, 0x2E, 0},
    {0x2D, 0x2F, 0x2E, 1},
    {0x2D, 0x2F, 0x2E, 2},
    {0x33, 0x35, 0x34, 0},
    {0x39, 0x3B, 0x3A, 0},
};

static const TaskHandle Tsk_BtlEff_Doll = {"Tsk_BtlEff_Doll", BtlEff_Doll_RunTask, 0x64};

s32 BtlEff_Doll_Init(TaskPool* pool, Task* task, void* args) {
    BtlEff_Doll*      doll     = task->data;
    BtlEff_Doll_Args* dollArgs = args;

    MI_CpuSet(doll, 0, sizeof(BtlEff_Doll));
    doll->unk_60 = (s32)dollArgs->unk_4;
    CombatSprite_LoadFromTable(1, &doll->sprite, &data_ov003_020d77e0, data_ov004_020f0374, dollArgs->unk_0, 0x3F, 0x40);
    CombatSprite_SetAnimFromTable(&doll->sprite, dollArgs->unk_0, 1);
    return 1;
}

s32 BtlEff_Doll_Update(TaskPool* pool, Task* task, void* args) {
    BtlEff_Doll* doll = task->data;

    if (doll->unk_60->unk_54 & 2) {
        return 0;
    }
    if (SpriteMgr_IsAnimationFinished(&doll->sprite.sprite) != 0) {
        return 0;
    }
    CombatSprite_Update(&doll->sprite);
    return 1;
}

s32 BtlEff_Doll_Render(TaskPool* pool, Task* task, void* args) {
    BtlEff_Doll* doll    = task->data;
    DollUnk*     temp_r6 = doll->unk_60;

    if (temp_r6->unk_24 == 0) {
        CombatSprite_SetFlip(&doll->sprite, 0);
    } else {
        CombatSprite_SetFlip(&doll->sprite, 1);
    }

    CombatSprite_SetPosition(&doll->sprite, func_ov003_020843b0(1, temp_r6->unk_28),
                             func_ov003_020843ec(1, temp_r6->unk_2C, temp_r6->unk_30));
    func_ov003_02082730(&doll->sprite, 0x7FFFFFFF - (temp_r6->unk_2C + 0x1000));
    CombatSprite_Render(&doll->sprite);
    return 1;
}

s32 BtlEff_Doll_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlEff_Doll* doll = task->data;

    CombatSprite_Release(&doll->sprite);
    return 1;
}

s32 BtlEff_Doll_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlEff_Doll_Init,
        .update     = BtlEff_Doll_Update,
        .render     = BtlEff_Doll_Render,
        .cleanup    = BtlEff_Doll_Destroy,
    };
    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

void BtlEff_Doll_CreateTask(u16 arg0, void* arg1) {
    BtlEff_Doll_Args args;
    args.unk_0 = arg0;
    args.unk_4 = arg1;
    EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlEff_Doll, NULL, 0, NULL, &args);
}
