#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/TusinSet.h"

typedef struct {
    /* 0x00 */ Sprite          sprite;
    /* 0x40 */ BOOL            visible;
    /* 0x44 */ TusinSetObject* tusinSet;
} TusinSet_pointer; // Size: 0x48

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_pointer_Args;

static SpriteFrameInfo* TusinSet_pointer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_pointer = {"Tsk_TusinSet_pointer", TusinSet_pointer_RunTask, sizeof(TusinSet_pointer)};

static const SpriteAnimation TusinSet_pointer_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_pointer_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 7,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* TusinSet_pointer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_pointer_Load(Sprite* sprite, TusinSet_pointer_Args* args) {
    SpriteAnimation anim = TusinSet_pointer_Anim;

    anim.dataType  = args->dataType;
    anim.animIndex = 31;
    _Sprite_Load(sprite, &anim);
}

static s32 TusinSet_pointer_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_pointer*      pointer     = task->data;
    TusinSet_pointer_Args* pointerArgs = args;

    pointer->tusinSet = pointerArgs->tusinSet;
    pointer->visible  = FALSE;
    TusinSet_pointer_Load(&pointer->sprite, pointerArgs);
    return 1;
}

static s32 TusinSet_pointer_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_pointer* pointer = task->data;
    TouchCoord        coord;

    if (TouchInput_IsTouchActive() == FALSE) {
        pointer->visible = FALSE;
    } else {
        TouchInput_GetCoord(&coord);
        pointer->sprite.posX = coord.x;
        pointer->sprite.posY = coord.y;
        pointer->visible     = TRUE;
    }

    Sprite_Update(&pointer->sprite);
    return 1;
}

static s32 TusinSet_pointer_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_pointer* pointer = task->data;

    if (pointer->visible == TRUE) {
        Sprite_RenderFrame(&pointer->sprite);
    }
    return 1;
}

static s32 TusinSet_pointer_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_pointer* pointer = task->data;

    Sprite_Release(&pointer->sprite);
    return 1;
}

static s32 TusinSet_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_pointer_Init,
        .update     = TusinSet_pointer_Update,
        .render     = TusinSet_pointer_Render,
        .cleanup    = TusinSet_pointer_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_pointer_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_pointer_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_pointer, NULL, 0, NULL, &args);
}
