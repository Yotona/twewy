#include "Engine/EasyTask.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 posX;
    /* 0x6 */ s16 posY;
    /* 0x8 */ s16 animIndex;
} Fld_TalkEventDecide_Args;

extern const BinIdentifier data_ov030_020d9944;

SpriteFrameInfo* Fld_TalkEventDecide_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
void             Fld_TalkEventDecide_Load(Sprite* sprite, Fld_TalkEventDecide_Args* args);
s32              Fld_TalkEventDecide_Init(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventDecide_Update(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventDecide_Render(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventDecide_Destroy(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventDecide_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TalkEventDecide = {"Tsk_Fld_TalkEventDecide", Fld_TalkEventDecide_RunTask, sizeof(Sprite)};
static const SpriteAnimation Fld_TalkEventDecide_Anim = {
    .bits_0_1          = 1,
    .bits_7_9          = 5,
    .frameInfoCallback = Fld_TalkEventDecide_GetFrameInfo,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 7,
    .unk_1C            = 1,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 8,
};

SpriteFrameInfo* Fld_TalkEventDecide_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void Fld_TalkEventDecide_Load(Sprite* sprite, Fld_TalkEventDecide_Args* args) {
    SpriteAnimation anim = Fld_TalkEventDecide_Anim;

    anim.dataType  = args->dataType;
    anim.posX      = args->posX;
    anim.posY      = args->posY;
    anim.animIndex = args->animIndex;
    _Sprite_Load(sprite, &anim);
}

s32 Fld_TalkEventDecide_Init(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite = task->data;

    Fld_TalkEventDecide_Load(sprite, args);
    SndMgr_PlaySEWithPan(0x121, sprite->posX);
    return TRUE;
}

s32 Fld_TalkEventDecide_Update(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite = task->data;

    Sprite_Update(sprite);
    return Sprite_HasAnimation(sprite) != FALSE;
}

s32 Fld_TalkEventDecide_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return TRUE;
}

s32 Fld_TalkEventDecide_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return TRUE;
}

s32 Fld_TalkEventDecide_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventDecide_Init,
        .update     = Fld_TalkEventDecide_Update,
        .render     = Fld_TalkEventDecide_Render,
        .cleanup    = Fld_TalkEventDecide_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventDecide_CreateTask(TaskPool* pool, s32 dataType, s16 posX, s16 posY) {
    Fld_TalkEventDecide_Args args;

    args.dataType  = dataType;
    args.posX      = posX;
    args.posY      = posY;
    args.animIndex = 8;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventDecide, NULL, 0, NULL, &args);
}

s32 func_ov030_020c4810(TaskPool* pool, s32 dataType, s16 posX, s16 posY) {
    Fld_TalkEventDecide_Args args;

    args.dataType  = dataType;
    args.posX      = posX;
    args.posY      = posY;
    args.animIndex = 0xE;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventDecide, NULL, 0, NULL, &args);
}
