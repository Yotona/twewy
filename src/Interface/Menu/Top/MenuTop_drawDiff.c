#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ BOOL           visible;
    /* 0x44 */ MenuTopObject* topMenu;
    /* 0x48 */ u16            initialDifficulty;
} MenuTop_drawDiff; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            difficulty;
} MenuTop_drawDiff_Args;

static SpriteFrameInfo* MenuTop_drawDiff_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_drawDiff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_drawDiff = {"Tsk_MenuTop_drawDiff", MenuTop_drawDiff_RunTask, sizeof(MenuTop_drawDiff)};

static const SpriteAnimation MenuTop_drawDiff_Anim = {
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
    .frameInfoCallback = MenuTop_drawDiff_GetFrameInfo,
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

static s16 MenuTop_drawDiff_GetFrame(MenuTop_drawDiff* taskData) {
    s16 frames[4] = {0x33, 0x32, 0x31, 0x30};

    return frames[taskData->topMenu->difficulty];
}

static SpriteFrameInfo* MenuTop_drawDiff_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_drawDiff_Load(MenuTop_drawDiff* taskData, Sprite* sprite, MenuTop_drawDiff_Args* args) {
    SpriteAnimation anim = MenuTop_drawDiff_Anim;

    s32 val = MenuTop_drawDiff_GetFrame(taskData);

    anim.dataType     = args->dataType;
    taskData->visible = TRUE;
    anim.animIndex    = val;
    anim.posX         = 0x62;
    anim.posY         = 0xA4;

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_drawDiff_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawDiff*      taskData = task->data;
    MenuTop_drawDiff_Args* initArgs = args;

    taskData->topMenu           = initArgs->topMenu;
    taskData->initialDifficulty = initArgs->difficulty;
    MenuTop_drawDiff_Load(taskData, &taskData->sprite, initArgs);
    return 1;
}

static s32 MenuTop_drawDiff_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawDiff* taskData = task->data;

    MenuTop_SetSpriteFrame(&taskData->sprite, MenuTop_drawDiff_GetFrame(taskData));
    Sprite_Update(&taskData->sprite);
    return 1;
}

static s32 MenuTop_drawDiff_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawDiff* taskData = task->data;

    if (taskData->visible != 0) {
        Sprite_RenderFrame(&taskData->sprite);
    }
    return 1;
}

static s32 MenuTop_drawDiff_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_drawDiff* taskData = task->data;

    Sprite_Release(&taskData->sprite);
    return 1;
}

static s32 MenuTop_drawDiff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_drawDiff_Init,
        .update     = MenuTop_drawDiff_Update,
        .render     = MenuTop_drawDiff_Render,
        .cleanup    = MenuTop_drawDiff_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_drawDiff_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_drawDiff_Args args;

    args.dataType   = dataType;
    args.topMenu    = topMenu;
    args.difficulty = topMenu->difficulty;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_drawDiff, NULL, 0, NULL, &args);
}
