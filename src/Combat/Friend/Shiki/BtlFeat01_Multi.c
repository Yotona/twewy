#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020dd78c;
extern s32           data_ov003_020e71e0;

typedef struct BtlFeat01_Multi {
    /* 0x000 */ s32 (**unk_000)(struct BtlFeat01_Multi*);
    /* 0x004 */ s32          unk_004;
    /* 0x008 */ CombatSprite unk_008;
    /* 0x068 */ char         unk_068[0xC8 - 0x68];
    /* 0x0C8 */ s32          unk_0C8;
    /* 0x0CC */ s32          unk_0CC;
    /* 0x0D0 */ s32          unk_0D0;
    /* 0x0D4 */ s32          unk_0D4;
    /* 0x0D8 */ s32          unk_0D8;
    /* 0x0DC */ s32          unk_0DC;
    /* 0x0E0 */ s16          unk_0E0;
    /* 0x0E2 */ s16          unk_0E2;
    /* 0x0E4 */ u16          unk_0E4;
    /* 0x0E6 */ s16          unk_0E6;
    /* 0x0E8 */ char         unk_0E8[0xF0 - 0xE8];
    /* 0x0F0 */ s32          unk_0F0;
    /* 0x0F4 */ s32          unk_0F4;
    /* 0x0F8 */ s32          unk_0F8;
    /* 0x0FC */ s16          unk_0FC;
    /* 0x0FE */ s16          unk_0FE;
    /* 0x100 */ s16          unk_100;
} BtlFeat01_Multi; // Size: 0x104

typedef struct {
    /* 0x00 */ s32  unk_00;
    /* 0x04 */ char unk_04[0xC0];
    /* 0xC4 */ s32  unk_C4;
    /* 0xC8 */ s32  unk_C8;
    /* 0xCC */ char unk_CC[0xC];
    /* 0xD8 */ s32  unk_D8;
    /* 0xDC */ u16  unk_DC;
} BtlFeat01_Multi_Args;

s32 BtlFeat01_Multi_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

SpriteAnimEntry data_ov004_020f04d0[];

static s32 (*data_ov004_020f07c8[])(BtlFeat01_Multi*);
static s32 (*data_ov004_020f07dc[])(BtlFeat01_Multi*);

static const TaskHandle Tsk_BtlFeat01_Multi = {"Tsk_BtlFeat01_Multi", BtlFeat01_Multi_RunTask, 0x104};

void func_ov004_020ed1f8(BtlFeat01_Multi* multi) {
    multi->unk_0E6++;
    multi->unk_0FC = 0;
    multi->unk_0FE = 0;
    multi->unk_100 = 0;
}

void func_ov004_020ed21c(BtlFeat01_Multi* multi, s32 (**cbTable)(BtlFeat01_Multi*)) {
    multi->unk_000 = cbTable;
}

s32 func_ov004_020ed224(void) {
    return 1;
}

s32 func_ov004_020ed22c(BtlFeat01_Multi* multi) {
    multi->unk_0E4 = 0x23;
    multi->unk_0D8 = 0x10000;
    multi->unk_0F0 = 0;
    multi->unk_0F4 = 0x59A;
    func_ov004_020ed1f8(multi);
    return 2;
}

s32 func_ov004_020ed260(BtlFeat01_Multi* multi) {
    if (multi->unk_0DC != 0) {
        multi->unk_0C8 += multi->unk_0F0;
    } else {
        multi->unk_0C8 -= multi->unk_0F0;
    }
    multi->unk_0F0 += multi->unk_0F4;
    if (multi->unk_0E4 == 0) {
        func_ov004_020ed1f8(multi);
    }
    multi->unk_0E4--;
    return 0;
}

s32 func_ov004_020ed2c0(BtlFeat01_Multi* multi) {
    func_ov004_020ed1f8(multi);
    return 2;
}

s32 func_ov004_020ed2d0(BtlFeat01_Multi* multi) {
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r3;

    temp_r3 = multi->unk_0C8;
    temp_r2 = multi->unk_0F0;
    if (multi->unk_0DC != 0) {
        temp_r2_2      = temp_r3 + temp_r2;
        multi->unk_0C8 = temp_r2_2;
        if (temp_r2_2 > 0x11E000) {
            func_ov004_020ed1f8(multi);
        }
    } else {
        temp_r2_3      = temp_r3 - temp_r2;
        multi->unk_0C8 = temp_r2_3;
        if (temp_r2_3 < -0x1E000) {
            func_ov004_020ed1f8(multi);
        }
    }
    return 0;
}

s32 BtlFeat01_Multi_Init(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_Multi*      multi     = task->data;
    BtlFeat01_Multi_Args* multiArgs = args;

    MI_CpuSet(multi, 0, sizeof(BtlFeat01_Multi));

    multi->unk_004 = multiArgs->unk_00;
    multi->unk_0E0 = multiArgs->unk_DC;
    multi->unk_0DC = multiArgs->unk_D8;
    multi->unk_0C8 = multiArgs->unk_C4;
    multi->unk_0CC = multiArgs->unk_C8;
    multi->unk_0D0 = 0x1000;
    multi->unk_0D4 = 0x1000;

    if (multi->unk_004 == 1) {
        SpriteAnimEntry* animEntry = &data_ov004_020f04d0[multiArgs->unk_DC];

        CombatSprite_LoadDirect(multiArgs->unk_00, &multi->unk_008, &data_ov003_020dd78c, animEntry->charDataIndex,
                                animEntry->paletteDataIndex, animEntry->frameDataIndex, 3, animEntry->animDataIndex);
        CombatSprite_SetAnim(&multi->unk_008, animEntry->animDataIndex, 0);
    } else {
        SpriteAnimEntry* animEntry = &data_ov004_020f04d0[multiArgs->unk_DC];

        CombatSprite_LoadDirect(multiArgs->unk_00, &multi->unk_008, &data_ov003_020dd78c, animEntry->charDataIndex,
                                animEntry->paletteDataIndex, animEntry->frameDataIndex, 4, animEntry->animDataIndex);
        CombatSprite_SetAnim(&multi->unk_008, animEntry->animDataIndex, 0);
    }
    CombatSprite_SetAnimFromTable(&multi->unk_008, multiArgs->unk_DC, 1);
    CombatSprite_SetFlip(&multi->unk_008, multiArgs->unk_D8);
    if (multi->unk_004 == 0) {
        func_ov004_020ed21c(multi, data_ov004_020f07c8);
    } else {
        func_ov004_020ed21c(multi, data_ov004_020f07dc);
    }
    return 1;
}

s32 BtlFeat01_Multi_Update(TaskPool* pool, Task* task, void* args) {
    s32              temp_r0;
    BtlFeat01_Multi* multi = task->data;

    if (multi->unk_000[multi->unk_0E6] == NULL) {
        return 0;
    }
    do {
        temp_r0 = multi->unk_000[multi->unk_0E6](multi);
    } while (temp_r0 == 2);
    if (temp_r0 == 1) {
        return 0;
    }
    if (data_ov003_020e71e0 == 1) {
        return 0;
    }
    CombatSprite_Update(&multi->unk_008);
    return 1;
}

s32 BtlFeat01_Multi_Render(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_Multi* multi = task->data;

    CombatSprite_SetPosition(&multi->unk_008, F2Ix(multi->unk_0C8), F2Ix(multi->unk_0CC));
    CombatSprite_SetAffineTransform(&multi->unk_008, 0, multi->unk_0D0, multi->unk_0D4, 1);
    func_ov003_02082730(&multi->unk_008, 0x7FFFFFFF - multi->unk_0CC);
    CombatSprite_Render(&multi->unk_008);
    return 1;
}

s32 BtlFeat01_Multi_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_Multi* multi = task->data;

    CombatSprite_Release(&multi->unk_008);
    return 0;
}

s32 BtlFeat01_Multi_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlFeat01_Multi_Init,
        .update     = BtlFeat01_Multi_Update,
        .render     = BtlFeat01_Multi_Render,
        .cleanup    = BtlFeat01_Multi_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void BtlFeat01_Multi_CreateTask(s32 arg0, s32 arg1, u16 arg2) {
    BtlFeat01_Multi_Args args;
    args.unk_00 = arg0;
    if (arg0 == 0) {
        args.unk_C4 = -0x5000;
        args.unk_D8 = 1;
    } else {
        args.unk_C4 = 0x105000;
        args.unk_D8 = 0;
    }
    args.unk_C8 = arg1;
    args.unk_DC = arg2;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &Tsk_BtlFeat01_Multi, NULL, 0, NULL, &args);
}
