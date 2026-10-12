#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite left;
    /* 0x40 */ Sprite right;
} Fld_TouchNext;

typedef struct {
    s32 dataType;
    u16 flags;
} Fld_TouchNext_Args;

SpriteFrameInfo* Fld_TouchNext_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
s32              Fld_TouchNext_Init(TaskPool* pool, Task* task, void* args);
s32              Fld_TouchNext_Update(TaskPool* pool, Task* task, void* args);
s32              Fld_TouchNext_Render(TaskPool* pool, Task* task, void* args);
s32              Fld_TouchNext_Destroy(TaskPool* pool, Task* task, void* args);
s32              Fld_TouchNext_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern BinIdentifier data_ov030_020d9944;

static const TaskHandle      Tsk_Fld_TouchNext  = {"Tsk_Fld_TouchNext", Fld_TouchNext_RunTask, sizeof(Fld_TouchNext)};
static const SpriteAnimation Fld_TouchNext_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0,
    .posX              = 0x80,
    .posY              = 0xB8,
    .frameInfoCallback = Fld_TouchNext_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 10,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

SpriteFrameInfo* Fld_TouchNext_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void Fld_TouchNext_Load(Sprite* sprite, Fld_TouchNext_Args* args, s16 animIndex) {
    SpriteAnimation animation = Fld_TouchNext_Anim;
    animation.dataType        = args->dataType;
    animation.bits_0_1        = args->flags;
    animation.animIndex       = animIndex;
    _Sprite_Load(sprite, &animation);
}

s32 Fld_TouchNext_Init(TaskPool* pool, Task* task, void* args) {
    Fld_TouchNext* touchNext = task->data;

    Fld_TouchNext_Load(&touchNext->left, args, 1);
    Fld_TouchNext_Load(&touchNext->right, args, 2);
    return 1;
}

s32 Fld_TouchNext_Update(TaskPool* pool, Task* task, void* args) {
    Fld_TouchNext* touchNext = task->data;

    Sprite_Update(&touchNext->left);
    Sprite_Update(&touchNext->right);
    return 1;
}

s32 Fld_TouchNext_Render(TaskPool* pool, Task* task, void* args) {
    Fld_TouchNext* touchNext = task->data;

    Sprite_RenderFrame(&touchNext->left);
    Sprite_RenderFrame(&touchNext->right);
    return 1;
}

s32 Fld_TouchNext_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_TouchNext* touchNext = task->data;

    Sprite_Release(&touchNext->left);
    Sprite_Release(&touchNext->right);
    return 1;
}

s32 Fld_TouchNext_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TouchNext_Init,
        .update     = Fld_TouchNext_Update,
        .render     = Fld_TouchNext_Render,
        .cleanup    = Fld_TouchNext_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_TouchNext_CreateTask(TaskPool* pool, s32 dataType, u16 flags) {
    Fld_TouchNext_Args args;
    args.dataType = dataType;
    args.flags    = flags;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TouchNext, NULL, 0, NULL, &args);
}
