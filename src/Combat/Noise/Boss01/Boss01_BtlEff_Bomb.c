#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/Boss01.h"
#include <NitroSDK/fx.h>

typedef struct {
    /* 0x00 */ Vec   unk_00;
    /* 0x0C */ Quad* unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ s16   unk_14;
} BtlEff_Bomb_Args;

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          unk_60;
    /* 0x6C */ Quad         unk_6C;
    /* 0x74 */ s32          unk_74;
    /* 0x78 */ s32          unk_78;
    /* 0x7C */ s16          unk_7C;
    /* 0x7E */ s16          unk_7E;
    /* 0x80 */ s16          unk_80;
} BtlEff_Bomb; // Size: 0x84

s32 BtlEff_Bomb_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov016_021290a4 = {4, 6, 5, 0};
static const TaskHandle      Task_BtlEff_Bomb    = {"Task_BtlEff_Bomb", BtlEff_Bomb_RunTask, sizeof(BtlEff_Bomb)};

extern s32 func_ov003_020c5bc4(u16, Quad*, s32, s32, s32);

s32 BtlEff_Bomb_Init(TaskPool* pool, Task* task, void* args) {
    BtlEff_Bomb_Args* bombArgs = (BtlEff_Bomb_Args*)args;
    BtlEff_Bomb*      eff      = task->data;

    MI_CpuSet(eff, 0, sizeof(BtlEff_Bomb));

    eff->unk_60 = bombArgs->unk_00;
    eff->unk_74 = bombArgs->unk_10;
    eff->unk_7C = bombArgs->unk_14;
    eff->unk_6C = *bombArgs->unk_0C;

    CombatSprite_LoadFromTable(func_ov003_020c3aac(eff->unk_74), &eff->sprite, &data_ov016_0212909c, &data_ov016_021290a4, 0,
                               7, 0);
    func_ov003_020c38e8(eff);
    return 1;
}

s32 BtlEff_Bomb_Update(TaskPool* pool, Task* task, void* args) {
    BtlEff_Bomb* eff    = task->data;
    s32          var_r5 = 1;

    switch (eff->unk_80) {
        case 0:
            if (eff->unk_7E < eff->unk_7C) {
                eff->unk_7E++;
            } else {
                eff->unk_7E = 0;
                eff->unk_80++;
            }
            break;

        case 1:
            if (eff->unk_7E == 0) {
                CombatSprite_SetAnimFromTable(&eff->sprite, 0, 1);
                func_ov003_02087f28(501, eff->unk_60.x);
            }
            if (eff->unk_78 == 0) {
                eff->unk_78 = func_ov003_020c5bc4(eff->unk_74, &eff->unk_6C, eff->unk_60.x, eff->unk_60.y, eff->unk_60.z);
            }
            if (SpriteMgr_IsAnimationFinished(&eff->sprite.sprite) != 0) {
                var_r5 = 0;
            } else {
                eff->unk_7E++;
            }
    }

    CombatSprite_Update(&eff->sprite);
    return var_r5;
}

s32 BtlEff_Bomb_Render(TaskPool* pool, Task* task, void* args) {
    s16          spA;
    s16          sp8;
    BtlEff_Bomb* temp_r4 = task->data;

    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk_60.x, temp_r4->unk_60.y, temp_r4->unk_60.z);
    CombatSprite_SetPosition((CombatSprite*)temp_r4, spA, sp8);
    if (func_ov003_020c37f8(temp_r4) != 0) {
        func_ov003_02082730((CombatSprite*)temp_r4, 0x7FDFFFFF);
    } else {
        func_ov003_02082730((CombatSprite*)temp_r4, 0x7FFFDFFF - temp_r4->unk_60.y);
    }
    CombatSprite_Render((CombatSprite*)temp_r4);
    return 1;
}

s32 BtlEff_Bomb_Destroy(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

s32 BtlEff_Bomb_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_021290b8 = {
        .initialize = BtlEff_Bomb_Init,
        .update     = BtlEff_Bomb_Update,
        .render     = BtlEff_Bomb_Render,
        .cleanup    = BtlEff_Bomb_Destroy,
    };
    return data_ov016_021290b8.iter[stage](pool, task, args);
}

void BtlEff_Bomb_CreateTaskDefault(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4) {
    BtlEff_Bomb_CreateTask(arg0, arg1, arg2, arg3, arg4, 0);
}

void BtlEff_Bomb_CreateTask(s32 arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    BtlEff_Bomb_Args args;
    args.unk_00.x = arg2;
    args.unk_00.y = arg3;
    args.unk_00.z = arg4;
    args.unk_0C   = arg1;
    args.unk_10   = arg0;
    args.unk_14   = arg5;

    s32 val = func_ov003_020c3aac(arg0);
    EasyTask_CreateTask(func_ov003_020c3a10(val), &Task_BtlEff_Bomb, NULL, 0, NULL, &args);
}
