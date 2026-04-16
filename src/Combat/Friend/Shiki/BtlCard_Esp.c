#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern s32 func_ov003_0208b690(u16);

typedef struct {
    /* 0x00 */ u16          unk_00;
    /* 0x02 */ char         unk_02[2];
    /* 0x04 */ CombatSprite unk_04;
    /* 0x64 */ CombatSprite unk_64;
    /* 0xC4 */ s16          unk_C4;
    /* 0xC6 */ s16          unk_C6;
    /* 0xC8 */ s16          unk_C8;
    /* 0xCA */ u16          unk_CA;
    /* 0xCC */ s16          unk_CC;
    /* 0xCE */ s16          unk_CE;
    /* 0xD0 */ s16          unk_D0;
    /* 0xD4 */ s32          unk_D4;
    /* 0xD8 */ s16          unk_D8;
    /* 0xDA */ char         unk_DA[0x2];
    /* 0xDC */ char         unk_DC[12];
} CardUnkEntry; // Size: 0xE8

typedef struct {
    /* 0x000 */ s16          unk_000;
    /* 0x002 */ char         unk_002[2];
    /* 0x004 */ char         unk_004[0x8];
    /* 0x00C */ CardUnkEntry cards[3];
} CardUnk; // Size: 0x2C4

typedef struct {
    /* 0x000 */ CombatSprite sprite;
    /* 0x060 */ char         unk_060[0x8];
    /* 0x068 */ s32          unk_068;
    /* 0x06C */ char         unk_06C[0x6E - 0x6C];
    /* 0x06E */ u16          unk_06E;
    /* 0x070 */ char         unk_070[0x28];
    /* 0x098 */ CardUnk      unk_098;
} BtlCard_Esp; // Size: 0x35C

s32 BtlCard_Esp_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern BinIdentifier data_ov003_020d7808;

static const SpriteAnimEntry data_ov004_020f0290[25] = {
    {1, 3, 2,  0},
    {1, 3, 2, 24},
    {1, 3, 2,  2},
    {1, 3, 2,  3},
    {1, 3, 2,  4},
    {1, 3, 2,  5},
    {1, 3, 2,  6},
    {1, 3, 2,  7},
    {1, 3, 2,  8},
    {1, 3, 2,  9},
    {1, 3, 2, 10},
    {1, 3, 2, 11},
    {1, 3, 2, 14},
    {1, 3, 2, 15},
    {1, 3, 2, 16},
    {1, 3, 2, 17},
    {1, 3, 2, 18},
    {1, 3, 2, 19},
    {1, 3, 2, 20},
    {1, 3, 2, 21},
    {1, 3, 2, 22},
    {1, 3, 2, 23},
    {1, 3, 2,  1},
    {1, 3, 2, 12},
    {1, 3, 2, 13},
};

const TaskHandle Tsk_BtlCard_Esp = {"Tsk_BtlCard_Esp", BtlCard_Esp_RunTask, 0x35C};

void func_ov004_020eac00(CombatSprite* arg0, s32 arg1, s32 arg2) {
    SpriteAnimationEx anim;

    CombatSprite_InitAnim(&anim.anim, 1, &data_ov003_020d7808);
    anim.anim.bits_10_11 = 1;
    anim.anim.unk_18     = 2;
    anim.anim.packIndex  = 2;
    anim.anim.unk_1C     = data_ov004_020f0290[arg1].charDataIndex;
    anim.anim.unk_1E     = arg2 << 5;
    anim.anim.unk_26     = data_ov004_020f0290[arg1].frameDataIndex;
    anim.anim.unk_28     = data_ov004_020f0290[arg1].paletteDataIndex;
    anim.anim.unk_20     = 7;
    anim.anim.unk_2A     = data_ov004_020f0290[arg1].animDataIndex + 1;
    anim.unk_2C          = &data_ov004_020f0290;
    CombatSprite_Load(arg0, &anim);
    func_ov003_02082738(arg0, 0);
    func_ov003_02082730(arg0, 2);
}

void func_ov004_020eaccc(CardUnk* arg0, s16 arg1, s16 arg2, s16 arg3) {
    CardUnkEntry* temp_r4 = &arg0->cards[arg1];

    temp_r4->unk_00 = 0xFFFF;
    temp_r4->unk_CE = arg2;
    temp_r4->unk_D0 = arg3;
    temp_r4->unk_CC = arg1;
    temp_r4->unk_C4 = 1;
    temp_r4->unk_C6 = 0;
    temp_r4->unk_C8 = 0;
    temp_r4->unk_D4 = 0;
    temp_r4->unk_D8 = temp_r4->unk_CC * 0x10;
    func_ov004_020eac00(&temp_r4->unk_04, 0, 0x10);
    func_ov004_020eac00(&temp_r4->unk_64, 0x18, 0x14);
    CombatSprite_SetAnimFromTable(&temp_r4->unk_04, 0, 0);
    CombatSprite_SetPosition(&temp_r4->unk_04, arg2, arg3);
    CombatSprite_SetAnimFromTable(&temp_r4->unk_64, 0x18, 0);
    CombatSprite_SetPosition(&temp_r4->unk_64, (s16)(arg2 + 7), (s16)(arg3 + 7));
    func_ov003_020849bc(&arg0->unk_004, temp_r4->unk_DC, temp_r4);
}

void func_ov004_020eada8(CardUnkEntry* arg0) {
    switch (arg0->unk_C4) {
        case 0: {
            if (arg0->unk_C8 == 0) {
                CombatSprite_SetAnimFromTable(&arg0->unk_04, 1, 1);
                arg0->unk_C8++;
            }
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_04.sprite)) {
                arg0->unk_C4 = 1;
                arg0->unk_C8 = 0;
            }
        } break;

        case 1: {
            if (arg0->unk_C8 == 0) {
                CombatSprite_SetAnimFromTable(&arg0->unk_04, 0, 0);
                arg0->unk_C8++;
            }
            if (arg0->unk_CA & 1) {
                CombatSprite_SetAnimFromTable(&arg0->unk_04, arg0->unk_00 + 7, 1);
                arg0->unk_C4 = 2;
                arg0->unk_C8 = 0;
            }
            if (arg0->unk_CA & 0x10) {
                CombatSprite_SetAnimFromTable(&arg0->unk_04, arg0->unk_00 + 0xC, 1);
                arg0->unk_CA &= ~0x10;
                arg0->unk_C4 = 5;
                arg0->unk_C8 = 0;
            }
        } break;

        case 2: {
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_04.sprite)) {
                CombatSprite_SetAnimFromTable(&arg0->unk_04, arg0->unk_00 + 2, 1);
                arg0->unk_CA |= 2;
                arg0->unk_C4 = 3;
            }
        } break;

        case 5: {
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_04.sprite)) {
                arg0->unk_C4 = 1;
                arg0->unk_C8 = 0;
            }
        } break;
    }
}

void func_ov004_020eaf34(CardUnkEntry* arg0) {
    switch (arg0->unk_C6) {
        case 0: {
            if (arg0->unk_CA & 8) {
                CombatSprite_SetAnimFromTable(&arg0->unk_64, 23, 1);
                arg0->unk_C6++;
            }
        } break;

        case 1: {
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_64.sprite)) {
                CombatSprite_SetAnimFromTable(&arg0->unk_64, 24, 0);
                arg0->unk_C6++;
            }
        } break;
    }
}

void func_ov004_020eafb0(CardUnk* arg0) {
    CardUnkEntry* var_r4 = func_ov003_0208495c(&arg0->unk_004);

    while (var_r4 != NULL) {
        func_ov004_020eada8(var_r4);
        func_ov004_020eaf34(var_r4);
        CombatSprite_Update(&var_r4->unk_04);
        CombatSprite_Update(&var_r4->unk_64);
        func_ov003_02086c78(&var_r4->unk_D4, &var_r4->unk_D8);
        var_r4 = func_ov003_02084984(var_r4->unk_DC);
    }
}

void func_ov004_020eb004(CardUnk* arg0) {
    s16 var_r5;

    CardUnkEntry* var_r4 = func_ov003_0208495c(&arg0->unk_004);

    while (var_r4 != NULL) {
        if (var_r4->unk_CA & 1) {
            var_r5 = var_r4->unk_D0;
        } else {
            var_r5 = var_r4->unk_D0 + ((s32)var_r4->unk_D4 >> 0xC);
        }
        CombatSprite_SetPosition(&var_r4->unk_04, var_r4->unk_CE, var_r5);
        CombatSprite_Render(&var_r4->unk_04);
        if (var_r4->unk_CA & 8) {
            CombatSprite_SetPosition(&var_r4->unk_64, (s16)(var_r4->unk_CE + 7), (s16)(var_r5 + 7));
            CombatSprite_Render(&var_r4->unk_64);
        }
        var_r4 = func_ov003_02084984(&var_r4->unk_DC);
    }
}

void func_ov004_020eb0a0(CardUnk* arg0) {
    CardUnkEntry* var_r4 = func_ov003_0208495c(&arg0->unk_004);

    while (var_r4 != NULL) {
        CombatSprite_Release(&var_r4->unk_04);
        CombatSprite_Release(&var_r4->unk_64);
        var_r4 = func_ov003_02084984(&var_r4->unk_DC);
    }
}

void func_ov004_020eb0d8(BtlCard_Esp* esp) {
    CardUnkEntry* temp_r0 = func_ov004_020e742c(&esp->unk_098, esp->unk_098.unk_000);

    if (temp_r0 != NULL) {
        CombatSprite_SetPosition(&esp->sprite, temp_r0->unk_CE, (s16)(temp_r0->unk_D0 + (temp_r0->unk_D4 >> 0xC)));
        CombatSprite_Render(&esp->sprite);
    }
}

s32 BtlCard_Esp_Init(TaskPool* pool, Task* task, void* args) {
    BtlCard_Esp* esp = task->data;

    MI_CpuSet(esp, 0, sizeof(BtlCard_Esp));
    func_ov003_02087094(&esp->unk_068, 0, &esp->unk_098);
    if (func_ov004_020e7360() == 0) {
        func_ov003_0208722c();
    }
    func_ov003_020849ac(&esp->unk_098.unk_004);
    func_ov004_020eaccc(&esp->unk_098, 0, 0x98, 0x14);
    func_ov004_020eaccc(&esp->unk_098, 1, 0x7F, 0x14);
    func_ov004_020eaccc(&esp->unk_098, 2, 0x66, 0x14);
    func_ov003_020871cc(0);
    func_ov004_020eac00(&esp->sprite, 0x16, 4);
    CombatSprite_SetAnimFromTable(&esp->sprite, 0x16, 0);
    return 1;
}

s32 BtlCard_Esp_Update(TaskPool* pool, Task* task, void* args) {
    BtlCard_Esp* esp = task->data;

    if ((esp->unk_06E & 1) == 0) {
        func_ov004_020eafb0(&esp->unk_098);
        CombatSprite_Update(&esp->sprite);
    }
    return 1;
}

s32 BtlCard_Esp_Render(TaskPool* pool, Task* task, void* args) {
    BtlCard_Esp* esp = task->data;

    if ((esp->unk_06E & 1) == 0) {
        func_ov004_020eb004(&esp->unk_098);
        func_ov004_020eb0d8(esp);
    }
    return 1;
}

s32 BtlCard_Esp_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlCard_Esp* esp = task->data;

    func_ov004_020eb0a0(&esp->unk_098);
    CombatSprite_Release(&esp->sprite);
    func_ov003_020871b0(&esp->unk_068);
    return 1;
}

s32 BtlCard_Esp_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlCard_Esp_Init,
        .update     = BtlCard_Esp_Update,
        .render     = BtlCard_Esp_Render,
        .cleanup    = BtlCard_Esp_Destroy,
    };
    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
