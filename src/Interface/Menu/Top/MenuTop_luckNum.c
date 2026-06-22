#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x000 */ Sprite             sprites[5];
    /* 0x140 */ s32                visible[5];
    /* 0x154 */ UnkStruct_TopMenu* unk_154;
    /* 0x158 */ s32                unk_158;
    /* 0x15C */ u16                unk_15C;
} MenuTop_luckNum; // Size: 0x160

typedef struct {
    /* 0x0 */ s32                unk_0;
    /* 0x4 */ UnkStruct_TopMenu* unk_4;
    /* 0x8 */ s32                unk_8;
} MenuTop_luckNum_Args;

extern BinIdentifier data_ov043_020c79c8;
extern void          func_ov043_02084620(void* sprite, s16 frame);

static SpriteFrameInfo* MenuTop_luckNum_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_luckNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_luckNum = {"Tsk_MenuTop_luckNum", MenuTop_luckNum_RunTask, sizeof(MenuTop_luckNum)};

static const SpriteAnimation data_ov043_020c7bd0 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 230,
    .unk_06     = 180,
    .unk_08     = MenuTop_luckNum_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79c8,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_luckNum_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info = temp;
        } break;
    }

    return info;
}

// Nonmatching
static void func_ov043_02086544(MenuTop_luckNum* luckNum) {
    s32 luckVal = luckNum->unk_154->unk_1C;
    s32 digits[5];

    if (luckVal > 999)
        luckVal = 999;

    digits[0] = luckVal / 10000;
    luckVal %= 10000;
    digits[1] = luckVal / 1000;
    luckVal %= 1000;
    digits[2] = luckVal / 100;
    luckVal %= 100;
    digits[3] = luckVal / 10;
    digits[4] = luckVal % 10;

    for (u16 i = 0; i < 5; i++) {
        luckNum->visible[i] = 1;
    }

    for (u16 i = 0; i < 4 && digits[i] == 0; i++) {
        luckNum->visible[i] = 0;
    }

    for (u16 i = 0; i < 5; i++) {
        func_ov043_02084620(&luckNum->sprites[i], (s16)(digits[i] + 1));
    }
}

// Nonmatching
void MenuTop_luckNum_Load(MenuTop_luckNum* luckNum, Sprite* sprites, MenuTop_luckNum_Args* args) {
    SpriteAnimation anim    = data_ov043_020c7bd0;
    s32             luckVal = luckNum->unk_154->unk_1C;
    s32             digits[5];

    anim.dataType = args->unk_0;

    if (luckVal > 999)
        luckVal = 999;

    digits[0] = luckVal / 10000;
    luckVal %= 10000;
    digits[1] = luckVal / 1000;
    luckVal %= 1000;
    digits[2] = luckVal / 100;
    luckVal %= 100;
    digits[3] = luckVal / 10;
    digits[4] = luckVal % 10;

    for (u16 i = 0; i < 5; i++) {
        luckNum->visible[i] = 1;
    }

    for (u16 i = 0; i < 4 && digits[i] == 0; i++) {
        luckNum->visible[i] = 0;
    }

    for (u16 i = 0; i < 5; i++) {
        anim.unk_2A = (s16)(digits[i] + 1);
        anim.unk_04 = (s16)(i * 8 + 214);
        anim.unk_06 = 182;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_luckNum_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckNum*      luckNum  = task->data;
    MenuTop_luckNum_Args* initArgs = args;

    luckNum->unk_154 = initArgs->unk_4;
    luckNum->unk_158 = initArgs->unk_8;
    luckNum->unk_15C = luckNum->unk_154->unk_06;
    MenuTop_luckNum_Load(luckNum, luckNum->sprites, initArgs);
    return 1;
}

static s32 MenuTop_luckNum_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckNum*   luckNum    = task->data;
    UnkStruct_TopMenu* owner      = luckNum->unk_154;
    u16                ownerUnk6  = owner->unk_06;
    u16                cachedUnk6 = luckNum->unk_15C;

    if (cachedUnk6 != ownerUnk6) {
        owner->unk_1C -= (s32)(ownerUnk6 - cachedUnk6);
        func_ov043_02086544(luckNum);
        luckNum->unk_15C = owner->unk_06;
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&luckNum->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_luckNum_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckNum* luckNum = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (luckNum->visible[i] != 0) {
            Sprite_RenderFrame(&luckNum->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_luckNum_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckNum* luckNum = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&luckNum->sprites[i]);
    }
    return 1;
}

s32 MenuTop_luckNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_luckNum_Init,
        .update     = MenuTop_luckNum_Update,
        .render     = MenuTop_luckNum_Render,
        .cleanup    = MenuTop_luckNum_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_luckNum_CreateTask(TaskPool* pool, s32 arg1, UnkStruct_TopMenu* arg2) {
    MenuTop_luckNum_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    args.unk_8 = arg2->unk_1C;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_luckNum, NULL, 0, NULL, &args);
}
