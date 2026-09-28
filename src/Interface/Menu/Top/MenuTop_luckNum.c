#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x000 */ Sprite         sprites[5];
    /* 0x140 */ BOOL           visible[5];
    /* 0x154 */ MenuTopObject* topMenu;
    /* 0x158 */ s32            initialDropRate;
    /* 0x15C */ u16            prevLevel;
} MenuTop_luckNum; // Size: 0x160

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ s32            dropRate;
} MenuTop_luckNum_Args;

static SpriteFrameInfo* MenuTop_luckNum_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_luckNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_luckNum = {"Tsk_MenuTop_luckNum", MenuTop_luckNum_RunTask, sizeof(MenuTop_luckNum)};

static const SpriteAnimation MenuTop_luckNum_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 230,
    .posY              = 180,
    .frameInfoCallback = MenuTop_luckNum_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[11],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

void MenuTop_luckNum_Refresh(MenuTop_luckNum* luckNum) {
    u32 luckVal = luckNum->topMenu->dropRate;
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
        luckNum->visible[i] = TRUE;
    }

    for (u16 i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        luckNum->visible[i] = FALSE;
    }

    for (u16 i = 0; i < 5; i++) {
        MenuTop_SetSpriteFrame(&luckNum->sprites[i], (s16)(digits[i] + 1));
    }
}

static SpriteFrameInfo* MenuTop_luckNum_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void MenuTop_luckNum_Load(MenuTop_luckNum* luckNum, Sprite* sprites, MenuTop_luckNum_Args* args) {
    MenuTopObject*  topMenu = luckNum->topMenu;
    SpriteAnimation anim    = MenuTop_luckNum_Anim;
    u32             luckVal;
    s32             digits[5];

    anim.dataType = args->dataType;

    luckVal = topMenu->dropRate;

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
        luckNum->visible[i] = TRUE;
    }

    for (u16 i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        luckNum->visible[i] = FALSE;
    }

    for (u16 i = 0; i < 5; i++) {
        anim.animIndex = (s16)(digits[i] + 1);
        anim.posX      = (s16)(i * 8 + 214);
        anim.posY      = 182;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_luckNum_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckNum*      luckNum  = task->data;
    MenuTop_luckNum_Args* initArgs = args;

    luckNum->topMenu         = initArgs->topMenu;
    luckNum->initialDropRate = initArgs->dropRate;
    luckNum->prevLevel       = luckNum->topMenu->currentLevel;
    MenuTop_luckNum_Load(luckNum, luckNum->sprites, initArgs);
    return 1;
}

static s32 MenuTop_luckNum_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_luckNum* luckNum   = task->data;
    MenuTopObject*   topMenu   = luckNum->topMenu;
    u16              level     = topMenu->currentLevel;
    u16              prevLevel = luckNum->prevLevel;

    if (prevLevel != level) {
        topMenu->dropRate -= (s32)(level - prevLevel);
        MenuTop_luckNum_Refresh(luckNum);
        luckNum->prevLevel = topMenu->currentLevel;
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

s32 MenuTop_luckNum_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_luckNum_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    args.dropRate = topMenu->dropRate;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_luckNum, NULL, 0, NULL, &args);
}
