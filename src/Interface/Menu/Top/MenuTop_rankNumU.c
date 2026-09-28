#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite         sprites[2];
    /* 0x80 */ BOOL           visible[2];
    /* 0x88 */ MenuTopObject* topMenu;
} MenuTop_rankNumU; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32            dataType;
    /* 0x04 */ MenuTopObject* topMenu;
    /* 0x08 */ u16            positionIndex;
    /* 0x0A */ u16            value;
} MenuTop_rankNumU_Args;

static SpriteFrameInfo* MenuTop_rankNumU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_rankNumU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_rankNumU = {"Tsk_MenuTop_rankNumU", MenuTop_rankNumU_RunTask, sizeof(MenuTop_rankNumU)};

static const SpriteAnimation MenuTop_rankNumU_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuTop_rankNumU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[7],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_rankNumU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_rankNumU_Load(MenuTop_rankNumU* taskData, Sprite* sprites, MenuTop_rankNumU_Args* args) {
    SpriteAnimation anim         = MenuTop_rankNumU_Anim;
    Point           positions[4] = {
        {13, 105},
        {13, 130},
        {13, 155},
        {13, 180}
    };
    u16 tens = args->value / 10;
    s16 xOffset;
    u16 ones = args->value % 10;

    anim.dataType = args->dataType;

    if (args->value < 10) {
        taskData->visible[0] = FALSE;
        taskData->visible[1] = TRUE;
        xOffset              = -3;
    } else {
        taskData->visible[0] = TRUE;
        taskData->visible[1] = TRUE;
        xOffset              = 0;
    }

    anim.animIndex = tens + 3;
    anim.posX      = xOffset + positions[args->positionIndex].x;
    anim.posY      = positions[args->positionIndex].y;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = ones + 3;
    anim.posX      = xOffset + positions[args->positionIndex].x + 7;
    anim.posY      = positions[args->positionIndex].y;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuTop_rankNumU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU*      taskData = task->data;
    MenuTop_rankNumU_Args* initArgs = args;

    taskData->topMenu = initArgs->topMenu;
    MenuTop_rankNumU_Load(taskData, taskData->sprites, initArgs);
    return 1;
}

static s32 MenuTop_rankNumU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU* taskData = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Update(&taskData->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_rankNumU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU* taskData = task->data;

    for (s16 i = 0; i < 2; i++) {
        if (taskData->visible[i] != 0) {
            Sprite_RenderFrame(&taskData->sprites[i]);
        }
    }

    return 1;
}

static s32 MenuTop_rankNumU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_rankNumU* taskData = task->data;

    for (s16 i = 0; i < 2; i++) {
        Sprite_Release(&taskData->sprites[i]);
    }
    return 1;
}

s32 MenuTop_rankNumU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_rankNumU_Init,
        .update     = MenuTop_rankNumU_Update,
        .render     = MenuTop_rankNumU_Render,
        .cleanup    = MenuTop_rankNumU_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_rankNumU_CreateTask(TaskPool* pool, s32 dataType, s16 positionIndex, s16 value, MenuTopObject* topMenu) {
    MenuTop_rankNumU_Args args;

    args.dataType      = dataType;
    args.topMenu       = topMenu;
    args.positionIndex = positionIndex;
    args.value         = value;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_rankNumU, NULL, 0, NULL, &args);
}