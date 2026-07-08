#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[10];
    /* 0x280 */ s32              visibleFlags[10];
    /* 0x2A8 */ MenuBadgeObject* unk_2A8;
    /* 0x2AC */ u8               unk_2AC;
} MenuBadge_numMoney; // Size: 0x2B0

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* unk_4;
    /* 0x8 */ u32              money;
} MenuBadge_numMoney_Args;

static SpriteFrameInfo* MenuBadge_numMoney_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_numMoney_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_numMoney = {"Tsk_MenuBadge_numMoney", MenuBadge_numMoney_RunTask,
                                                  sizeof(MenuBadge_numMoney)};

static const SpriteAnimation MenuBadge_numMoney_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_numMoney_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static void func_ov043_020957b8(MenuBadge_numMoney* numMoney, u32 money, u32* digits) {
    digits[0] = money / 1000000;
    u32 value = money % 1000000;

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

    for (s32 i = 0; i < 10; i++) {
        numMoney->visibleFlags[i] = TRUE;
    }

    numMoney->unk_2AC = 7;
    for (s32 i = 0; i < 6; i++) {
        if (digits[i] != 0) {
            break;
        }
        numMoney->visibleFlags[i + 1] = FALSE;
        numMoney->unk_2AC--;
    }

    if (money < 1000000) {
        numMoney->visibleFlags[8] = FALSE;
        if (money < 1000) {
            numMoney->visibleFlags[9] = FALSE;
        }
    }
}

// Nonmatching: regswap
static void func_ov043_02095940(MenuBadge_numMoney* numMoney) {
    MenuBadgeObject* owner = numMoney->unk_2A8;

    s16 labelFrame;
    s32 digitBase;
    s16 suffixFrame;

    u32 digits[7];
    func_ov043_020957b8(numMoney, owner->unk_AF0C, digits);

    if (func_ov043_0208fdf0(owner, owner->unk_AF05) == 0) {
        labelFrame  = 0x49;
        digitBase   = 0x3F;
        suffixFrame = 0x5E;
    } else {
        labelFrame  = 0x54;
        digitBase   = 0x4A;
        suffixFrame = 0x5F;
    }

    func_ov043_0208fa60(&numMoney->sprites[0], labelFrame, 3, 2);

    for (u16 i = 0; i < 7; i++) {
        func_ov043_0208fa60(&numMoney->sprites[i + 1], digitBase + digits[i], 3, 2);
    }

    func_ov043_0208fa60(&numMoney->sprites[8], suffixFrame, 3, 2);
    func_ov043_0208fa60(&numMoney->sprites[9], suffixFrame, 3, 2);
}

static SpriteFrameInfo* MenuBadge_numMoney_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

// Nonmatching: regswap
static void MenuBadge_numMoney_Load(MenuBadge_numMoney* numMoney, Sprite* sprites, MenuBadge_numMoney_Args* args) {
    MenuBadgeObject* menuBadge = numMoney->unk_2A8;
    SpriteAnimation  anim      = MenuBadge_numMoney_Anim;

    MenuBadgePoint positions[5] = {
        {-10,  0},
        { 10, 16},
        { 22, 32},
        { 38, 44},
        {  5, 27},
    };

    s16 labelFrame;
    s32 digitBase;
    s16 suffixFrame;

    anim.dataType = args->dataType;

    u32 digits[7];
    func_ov043_020957b8(numMoney, args->money, digits);

    if (func_ov043_0208fdf0(menuBadge, menuBadge->unk_AF05) == 0) {
        labelFrame  = 0x49;
        digitBase   = 0x3F;
        suffixFrame = 0x5E;
    } else {
        labelFrame  = 0x54;
        digitBase   = 0x4A;
        suffixFrame = 0x5F;
    }

    anim.unk_2A = labelFrame;
    anim.unk_04 = positions[0].x + 0x25;
    anim.unk_06 = 0xB9;
    _Sprite_Load(&sprites[0], &anim);

    for (u16 i = 0; i < 7; i++) {
        anim.unk_2A = digitBase + digits[i];
        anim.unk_04 = positions[i].y + 0x25;
        anim.unk_06 = 0xB9;
        _Sprite_Load(&sprites[i + 1], &anim);
    }

    anim.unk_2A = suffixFrame;
    anim.unk_04 = positions[4].x + 0x25;
    anim.unk_06 = 0xB9;
    _Sprite_Load(&sprites[8], &anim);

    anim.unk_2A = suffixFrame;
    anim.unk_04 = positions[4].y + 0x25;
    anim.unk_06 = 0xB9;
    _Sprite_Load(&sprites[9], &anim);
}

static s32 MenuBadge_numMoney_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numMoney*      numMoney = task->data;
    MenuBadge_numMoney_Args* initArgs = args;

    numMoney->unk_2A8 = initArgs->unk_4;
    MenuBadge_numMoney_Load(numMoney, numMoney->sprites, initArgs);
    return 1;
}

static s32 MenuBadge_numMoney_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numMoney* numMoney = task->data;

    func_ov043_02095940(numMoney);
    for (s32 i = 0; i < 10; i++) {
        Sprite_Update(&numMoney->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_numMoney_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numMoney* numMoney = task->data;

    for (s32 i = 0; i < 10; i++) {
        if (numMoney->visibleFlags[i] != 0) {
            Sprite_RenderFrame(&numMoney->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_numMoney_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numMoney* numMoney = task->data;

    for (s32 i = 0; i < 10; i++) {
        Sprite_Release(&numMoney->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_numMoney_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_numMoney_Init,
        .update     = MenuBadge_numMoney_Update,
        .render     = MenuBadge_numMoney_Render,
        .cleanup    = MenuBadge_numMoney_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_numMoney_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* owner) {
    MenuBadge_numMoney_Args args;

    args.dataType = dataType;
    args.unk_4    = owner;
    args.money    = owner->unk_AF0C;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_numMoney, NULL, 0, NULL, &args);
}
