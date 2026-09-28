#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x000 */ Sprite         sprites[5];
    /* 0x140 */ BOOL           visible[5];
    /* 0x154 */ MenuTopObject* topMenu;
} MenuTop_numExpNext; // Size: 0x158

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u32            expNext;
} MenuTop_numExpNext_Args;

static SpriteFrameInfo* MenuTop_numExpNext_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_numExpNext_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_numExpNext = {"Tsk_MenuTop_numExpNext", MenuTop_numExpNext_RunTask,
                                                  sizeof(MenuTop_numExpNext)};

static const SpriteAnimation MenuTop_numExpNext_Anim = {
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
    .frameInfoCallback = MenuTop_numExpNext_GetFrameInfo,
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

static SpriteFrameInfo* MenuTop_numExpNext_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_numExpNext_Load(MenuTop_numExpNext* taskData, Sprite* sprites, MenuTop_numExpNext_Args* args) {
    SpriteAnimation anim = MenuTop_numExpNext_Anim;
    u32             value;
    u32             digits[5];
    u16             i;

    anim.dataType = args->dataType;

    value     = args->expNext;
    digits[0] = value / 10000;
    value %= 10000;
    digits[1] = value / 1000;
    value %= 1000;
    digits[2] = value / 100;
    value %= 100;
    digits[3] = value / 10;
    digits[4] = value % 10;

    for (i = 0; i < 5; i++) {
        taskData->visible[i] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        taskData->visible[i] = FALSE;
    }

    for (i = 0; i < 5; i++) {
        anim.animIndex = digits[i] + 10;
#ifdef REGION_USA
        anim.posX = i * 6 + 135;
#else
        anim.posX = i * 8 + 133;
#endif
        anim.posY = 139;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuTop_numExpNext_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext*      taskData = task->data;
    MenuTop_numExpNext_Args* initArgs = args;

    MenuTop_numExpNext_Load(taskData, taskData->sprites, initArgs);
    taskData->topMenu = initArgs->topMenu;
    return 1;
}

static s32 MenuTop_numExpNext_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_numExpNext_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (taskData->visible[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuTop_numExpNext_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_numExpNext* taskData = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_numExpNext_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_numExpNext_Init,
        .update     = MenuTop_numExpNext_Update,
        .render     = MenuTop_numExpNext_Render,
        .cleanup    = MenuTop_numExpNext_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_numExpNext_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_numExpNext_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;
    args.expNext  = topMenu->expToNextLevel;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_numExpNext, NULL, 0, NULL, &args);
}
