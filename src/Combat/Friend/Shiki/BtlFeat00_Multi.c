#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern s32           data_ov003_020e71e0;
extern BinIdentifier data_ov003_020d77d8;
extern BinIdentifier data_ov003_020d77e0;

extern void func_02026590(s32, s32, u16);

typedef struct BtlFeat00_Multi {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          unk_60;
    /* 0x62 */ s16          unk_62;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ s32          unk_6C;
    /* 0x70 */ u16          unk_70;
    /* 0x74 */ void (*unk_74)(struct BtlFeat00_Multi*);
    /* 0x78 */ s16 unk_78;
    /* 0x7A */ s16 unk_7A;
    /* 0x7C */ s32 unk_7C;
    /* 0x80 */ s32 unk_80;
} BtlFeat00_Multi; // Size: 0x84

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ u16 unk_4;
    /* 0x6 */ s16 unk_6;
    /* 0x8 */ s16 unk_8;
    /* 0xC */ s32 unk_C;
} BtlFeat00_Multi_Args;

void func_ov004_020ebe28(BtlFeat00_Multi* multi);
void func_ov004_020ebe64(BtlFeat00_Multi* multi);
s32  BtlFeat00_Multi_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov004_020f0400[6] = {
    {0x19, 0x1B, 0x1A, 1},
    {0x1C, 0x1E, 0x1D, 1},
    {0x1F, 0x21, 0x20, 1},
    {0x2A, 0x2C, 0x2B, 5},
    {0x2A, 0x2C, 0x2B, 6},
    {0x2A, 0x2C, 0x2B, 7},
};

static const TaskHandle Tsk_BtlFeat00_Multi = {"Tsk_BtlFeat00_Multi", BtlFeat00_Multi_RunTask, sizeof(BtlFeat00_Multi)};

void func_ov004_020ebc2c(BtlFeat00_Multi* multi, void (*callback)(BtlFeat00_Multi*)) {
    multi->unk_74 = callback;
    multi->unk_78 = 0;
    multi->unk_7A = 0;
}

void func_ov004_020ebc40(BtlFeat00_Multi* multi) {
    s16 temp_r5;

    if (multi->sprite.flags46 & 1) {
        temp_r5 = multi->unk_60 + 0x24;
    } else {
        temp_r5 = multi->unk_60 - 0x24;
    }

    if (multi->sprite.sprite.bits_0_1 != 0) {
        func_ov003_0209fc88(1, func_ov003_0208442c(1, temp_r5), func_ov003_02084460(1, multi->unk_62, 0), 0, 4, 1);
    } else {
        func_ov003_0209fc88(2, func_ov003_0208442c(0, temp_r5), func_ov003_02084460(0, multi->unk_62, 0), 0, 4, 1);
    }
}

void func_ov004_020ebd04(BtlFeat00_Multi* multi) {
    s32 var_r4;
    s16 var_r0;

    if (multi->sprite.flags46 & 1) {
        var_r4 = 1;
        var_r0 = multi->unk_60 + 0x24;
    } else {
        var_r4 = 0;
        var_r0 = multi->unk_60 - 0x24;
    }
    func_ov004_020ebc40(multi);

    if (multi->unk_70 < 3) {
        Feat00_Sword_CreateTask(multi->sprite.sprite.bits_0_1, multi->unk_70, multi->unk_60, multi->unk_62, -0x10000, var_r4);
    } else {
        Feat00_EffDoll_CreateTask(multi->sprite.sprite.bits_0_1, multi->unk_70 - 3, var_r0, multi->unk_62, -0x10000, var_r4);
    }
}

void func_ov004_020ebda4(BtlFeat00_Multi* multi) {
    if (multi->unk_7A == 0) {
        func_ov004_020ebd04(multi);
        multi->unk_7A = 5;
        multi->unk_6C = 0x10000;
    }
    func_02026590(&multi->unk_6C, 0, multi->unk_7A);
    func_02026590(&multi->unk_64, 0x1000, multi->unk_7A);
    func_02026590(&multi->unk_68, 0x1000, multi->unk_7A);
    multi->unk_7A--;
    if (multi->unk_7A <= 0) {
        func_ov004_020ebc2c(multi, func_ov004_020ebe28);
    }
}

void func_ov004_020ebe28(BtlFeat00_Multi* multi) {
    if (multi->unk_7A == 0) {
        multi->unk_7A = 10;
    }
    multi->unk_7A--;
    if (multi->unk_7A <= 0) {
        func_ov004_020ebc2c(multi, func_ov004_020ebe64);
    }
}

void func_ov004_020ebe64(BtlFeat00_Multi* multi) {
    if (multi->unk_7A == 0) {
        multi->unk_7A = 5;
    }
    func_02026590(&multi->unk_64, 0, multi->unk_7A);
    func_02026590(&multi->unk_68, 0x2000, multi->unk_7A);
    multi->unk_7A--;
    if (multi->unk_7A == 5) {
        CombatSprite_Release(&multi->sprite);
    }
    if (multi->unk_7A <= 0) {
        multi->unk_80 = 0;
    }
}

s32 BtlFeat00_Multi_Init(TaskPool* pool, Task* task, void* args) {
    BtlFeat00_Multi*      multi     = task->data;
    BtlFeat00_Multi_Args* multiArgs = args;

    MI_CpuSet(multi, 0U, sizeof(BtlFeat00_Multi));
    multi->unk_80 = 1;
    multi->unk_70 = multiArgs->unk_4;
    multi->unk_60 = multiArgs->unk_6;
    multi->unk_62 = multiArgs->unk_8;
    multi->unk_7C = multiArgs->unk_C;
    multi->unk_64 = 0x1000;
    multi->unk_68 = 0x2000;
    if (multiArgs->unk_4 < 3) {
        CombatSprite_LoadFromTable(multiArgs->unk_0, &multi->sprite, &data_ov003_020d77d8, data_ov004_020f0400,
                                   multiArgs->unk_4, 58, 80);
    } else {
        CombatSprite_LoadFromTable(multiArgs->unk_0, &multi->sprite, &data_ov003_020d77e0, data_ov004_020f0400,
                                   multiArgs->unk_4, 63, 20);
    }
    CombatSprite_SetAnimFromTable(&multi->sprite, multiArgs->unk_4, 1);
    CombatSprite_SetFlip(&multi->sprite, multiArgs->unk_C);
    func_ov004_020ebc2c(multi, func_ov004_020ebda4);
    return 1;
}

s32 BtlFeat00_Multi_Update(TaskPool* pool, Task* task, void* args) {
    BtlFeat00_Multi* multi = task->data;

    if (multi->unk_74 != NULL) {
        multi->unk_74(multi);
    }
    if (data_ov003_020e71e0 == 1) {
        return 0;
    }
    CombatSprite_Update(&multi->sprite);
    return multi->unk_80;
}

s32 BtlFeat00_Multi_Render(TaskPool* pool, Task* task, void* args) {
    BtlFeat00_Multi* multi = task->data;

    CombatSprite_SetAffineTransform(&multi->sprite, 0, multi->unk_64, multi->unk_68, 1);
    CombatSprite_SetPosition(&multi->sprite, multi->unk_60, multi->unk_62);
    func_ov003_02082730(&multi->sprite, 0x7FFFFFFF - multi->unk_62);
    CombatSprite_Render(&multi->sprite);
    return 1;
}

s32 BtlFeat00_Multi_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlFeat00_Multi* multi = task->data;

    if (Sprite_HasAnimation(&multi->sprite.sprite) != 0) {
        CombatSprite_Release(&multi->sprite);
    }
    return 1;
}

s32 BtlFeat00_Multi_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlFeat00_Multi_Init,
        .update     = BtlFeat00_Multi_Update,
        .render     = BtlFeat00_Multi_Render,
        .cleanup    = BtlFeat00_Multi_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void BtlFeat00_Multi_CreateTask(s32 arg0, u16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5) {
    BtlFeat00_Multi_Args args;
    args.unk_0 = arg0;
    args.unk_4 = arg1;
    args.unk_6 = arg2;
    args.unk_8 = arg3;
    args.unk_C = arg4;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &Tsk_BtlFeat00_Multi, NULL, 0, NULL, &args);
}
