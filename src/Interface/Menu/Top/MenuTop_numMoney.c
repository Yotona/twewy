#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite              sprites[10];
    /* 0x280 */ s32                visibleFlags[10];
    /* 0x2A8 */ UnkStruct_TopMenu* unk_2A8;
} MenuTop_numMoney; // Size: 0x2AC

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* unk_4;
    /* 0x8 */ u32   money;
} MenuTop_numMoney_Args;

extern s32 func_ov043_0208466c(void* owner, u8 displayMode);

static SpriteFrameInfo* MenuTop_numMoney_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuTop_numMoney_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_numMoney = {"Tsk_MenuTop_numMoney", MenuTop_numMoney_RunTask, sizeof(MenuTop_numMoney)};

static const Point data_ov043_020c7e7c[5] = {
    {-10,  0},
    { 10, 16},
    { 22, 32},
    { 38, 44},
    {  5, 27},
};

static const SpriteAnimation data_ov043_020c7e90 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_numMoney_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c7988,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_numMoney_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

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
static void MenuTop_numMoney_Load(MenuTop_numMoney* numMoney, void* spritesPtr, MenuTop_numMoney_Args* args) {
    Sprite*            sprites = spritesPtr;
    UnkStruct_TopMenu* owner   = numMoney->unk_2A8;
    SpriteAnimation    anim    = data_ov043_020c7e90;
    u32                value;
    u32                digits[7];
    s32                frameBase;
    s32                digitBase;
    s32                suffixFrame;

    anim.dataType = args->dataType;

    value     = args->money;
    digits[0] = value / 1000000;

    value %= 1000000;
    digits[1] = value / 100000;

    value %= 100000;
    digits[2] = value / 10000;

    value %= 10000;
    digits[3] = value / 1000;

    value %= 1000;
    digits[4] = value / 100;

    value %= 100;
    digits[5] = value / 10;
    digits[6] = value % 10;

    for (u16 i = 0; i < 10; i++) {
        numMoney->visibleFlags[i] = 1;
    }

    for (u16 i = 0; i < 6 && digits[i] == 0; i++) {
        numMoney->visibleFlags[i + 1] = 0;
    }

    if (func_ov043_0208466c(owner, owner->unk_5C) == 0) {
        frameBase   = 0x20;
        digitBase   = 0x16;
        suffixFrame = 0x49;
    } else {
        frameBase   = 0x47;
        digitBase   = 0x3D;
        suffixFrame = 0x4A;
    }

    anim.unk_2A = frameBase;
    anim.unk_04 = data_ov043_020c7e7c[0].x + 0x47;
    anim.unk_06 = 0xB7;
    _Sprite_Load(&sprites[0], &anim);

    for (u16 i = 0; i < 7; i++) {
        anim.unk_2A = digitBase + digits[i];
        anim.unk_04 = data_ov043_020c7e7c[i].y + 0x47;
        anim.unk_06 = 0xB7;
        _Sprite_Load(&sprites[i + 1], &anim);
    }

    if (args->money < 1000000) {
        numMoney->visibleFlags[8] = 0;
        if (args->money < 1000) {
            numMoney->visibleFlags[9] = 0;
        }
    }

    anim.unk_2A = suffixFrame;
    anim.unk_04 = data_ov043_020c7e7c[3].x + 0x47;
    anim.unk_06 = 0xB7;
    _Sprite_Load(&sprites[8], &anim);

    anim.unk_2A = suffixFrame;
    anim.unk_04 = data_ov043_020c7e7c[4].x + 0x47;
    anim.unk_06 = 0xB7;
    _Sprite_Load(&sprites[9], &anim);
}

static s32 MenuTop_numMoney_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_numMoney*      taskData = task->data;
    MenuTop_numMoney_Args* initArgs = args;

    taskData->unk_2A8 = initArgs->unk_4;
    MenuTop_numMoney_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_numMoney_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_numMoney* taskData = task->data;

    for (s32 i = 0; i < 10; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_numMoney_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_numMoney* taskData = task->data;

    for (s32 i = 0; i < 10; i++) {
        if (taskData->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_numMoney_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_numMoney* taskData = task->data;

    for (s32 i = 0; i < 10; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_numMoney_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_numMoney_Init,
        .update     = MenuTop_numMoney_Update,
        .render     = MenuTop_numMoney_Render,
        .cleanup    = MenuTop_numMoney_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_numMoney_CreateTask(TaskPool* pool, s32 dataType, UnkStruct_TopMenu* arg2) {
    MenuTop_numMoney_Args args;

    args.dataType = dataType;
    args.unk_4    = arg2;
    args.money    = arg2->unk_18;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_numMoney, NULL, 0, NULL, &args);
}
