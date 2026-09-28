#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "SndMgr.h"

// Single-digit numbers shift left to stay centred
#ifdef REGION_USA
    #define MENUTOP_NUMLV_ONE_DIGIT_SHIFT -3
#else
    #define MENUTOP_NUMLV_ONE_DIGIT_SHIFT -4
#endif

typedef struct {
    /* 0x000 */ Sprite         sprites[5];
    /* 0x140 */ BOOL           visible[5];
    /* 0x154 */ MenuTopObject* topMenu;
    /* 0x158 */ u16            previousLevel;
} MenuTop_numLV; // Size: 0x15C

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            level;
    /* 0xA */ u16            maxLevel;
} MenuTop_numLV_Args;

static SpriteFrameInfo* MenuTop_numLV_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_numLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_numLV = {"Tsk_MenuTop_numLV", MenuTop_numLV_RunTask, sizeof(MenuTop_numLV)};

static const SpriteAnimation MenuTop_numLV_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_numLV_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[3],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

void MenuTop_numLV_Refresh(MenuTop_numLV* taskData) {
    u16 level = taskData->topMenu->currentLevel;
    s16 tensFrame;
    s16 onesXOffset;
    s16 onesFrame;

    if (level >= 100) {
        taskData->visible[0] = FALSE;
        onesFrame            = 21;
        onesXOffset          = MENUTOP_NUMLV_ONE_DIGIT_SHIFT;
        tensFrame            = 1;
    } else {
        tensFrame = (u16)(level / 10) + 10;
        onesFrame = (u16)(level % 10) + 10;

        if (level < 10) {
            taskData->visible[0] = FALSE;
            taskData->visible[1] = TRUE;
            onesXOffset          = MENUTOP_NUMLV_ONE_DIGIT_SHIFT;
        } else {
            taskData->visible[0] = TRUE;
            taskData->visible[1] = TRUE;
            onesXOffset          = 0;
        }
    }

    MenuTop_SetSpriteFrame(&taskData->sprites[0], tensFrame);
    MenuTop_SetSpriteFrame(&taskData->sprites[1], onesFrame);
#ifdef REGION_USA
    taskData->sprites[0].posX = 50;
#else
    taskData->sprites[0].posX = 48;
#endif
    taskData->sprites[1].posX = onesXOffset + 56;
}

static SpriteFrameInfo* MenuTop_numLV_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void MenuTop_numLV_Load(MenuTop_numLV* taskData, Sprite* sprites, MenuTop_numLV_Args* args) {
    SpriteAnimation anim = MenuTop_numLV_Anim;
#ifdef REGION_USA
    s16 digitX[5] = {50, 56, 64, 72, 78};
#else
    s16 digitX[5] = {48, 56, 63, 71, 79};
#endif
    s16 xOffsets[5] = {0};
    s16 digitFrames[5];
    u16 i;

    anim.dataType = args->dataType;

    for (i = 0; i < 5; i++) {
        taskData->visible[i] = TRUE;
    }

    if (args->level >= 100) {
        taskData->visible[0] = FALSE;
        digitFrames[0]       = 1;
        digitFrames[1]       = 21;
        xOffsets[1]          = MENUTOP_NUMLV_ONE_DIGIT_SHIFT;
    } else {
        digitFrames[0] = (u16)(args->level / 10) + 10;
        digitFrames[1] = (u16)(args->level % 10) + 10;
        if (args->level < 10) {
            taskData->visible[0] = FALSE;
            xOffsets[1]          = MENUTOP_NUMLV_ONE_DIGIT_SHIFT;
        } else {
            xOffsets[1] = 0;
        }
    }

    digitFrames[2] = 20;

    if (args->maxLevel >= 100) {
        taskData->visible[3] = FALSE;
        digitFrames[3]       = 1;
        digitFrames[4]       = 21;
        xOffsets[4]          = MENUTOP_NUMLV_ONE_DIGIT_SHIFT;
    } else {
        digitFrames[3] = (u16)(args->maxLevel / 10) + 10;
        digitFrames[4] = (u16)(args->maxLevel % 10) + 10;
        if (args->maxLevel < 10) {
            taskData->visible[3] = FALSE;
            xOffsets[4]          = MENUTOP_NUMLV_ONE_DIGIT_SHIFT;
        } else {
            xOffsets[4] = 0;
        }
    }

    for (i = 0; i < 5; i++) {
        anim.animIndex = digitFrames[i];
        anim.posX      = digitX[i] + xOffsets[i];
        anim.posY      = 139;
        _Sprite_Load(&sprites[i], &anim);
    }

    taskData->previousLevel = args->level;
}

static s32 MenuTop_numLV_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;

    MenuTop_numLV_Load(taskData, taskData->sprites, args);
    taskData->topMenu = ((MenuTop_numLV_Args*)args)->topMenu;
    return 1;
}

static s32 MenuTop_numLV_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;
    MenuTopObject* topMenu  = taskData->topMenu;

    MenuTop_numLV_Refresh(taskData);
    if (taskData->previousLevel != topMenu->currentLevel) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
        taskData->previousLevel = topMenu->currentLevel;
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }

    return 1;
}

static s32 MenuTop_numLV_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visible[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }

    return 1;
}

static s32 MenuTop_numLV_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_numLV* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }

    return 1;
}

s32 MenuTop_numLV_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_numLV_Init,
        .update     = MenuTop_numLV_Update,
        .render     = MenuTop_numLV_Render,
        .cleanup    = MenuTop_numLV_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_numLV_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_numLV_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;
    args.level    = topMenu->currentLevel;
    args.maxLevel = topMenu->maxLevel;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_numLV, NULL, 0, NULL, &args);
}
