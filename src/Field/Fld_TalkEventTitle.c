#include "Engine/EasyTask.h"
#include "SpriteMgr.h"
#include <nitro/types.h>

extern void                func_020265d4(s32* value, s32 target, s32 duration);
extern const BinIdentifier data_ov030_020d9944;

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    posX; // 20.12 fixed point
    /* 0x44 */ s32    targetPosX;
    /* 0x48 */ u16    slideFrames;
} Fld_TalkEventTitle; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32 dataType;
} Fld_TalkEventTitle_Args;

s32              Fld_TalkEventTitle_Init(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventTitle_Update(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventTitle_Render(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventTitle_Destroy(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventTitle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo* Fld_TalkEventTitle_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_Fld_TalkEventTitle = {"Tsk_Fld_TalkEventTitle", Fld_TalkEventTitle_RunTask,
                                                  sizeof(Fld_TalkEventTitle)};

static const SpriteAnimation Fld_TalkEventTitle_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0xFF88,
    .posY              = 0,
    .frameInfoCallback = Fld_TalkEventTitle_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 7,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

SpriteFrameInfo* Fld_TalkEventTitle_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void Fld_TalkEventTitle_Load(Sprite* sprite, Fld_TalkEventTitle_Args* args) {
    SpriteAnimation anim = Fld_TalkEventTitle_Anim;

    anim.dataType = args->dataType;
    _Sprite_Load(sprite, &anim);
}

s32 Fld_TalkEventTitle_Init(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventTitle* title = task->data;
    Fld_TalkEventTitle_Load(&title->sprite, args);
    title->slideFrames = 0x14;
    title->posX        = -0x78000;
    title->targetPosX  = 0;
    return 1;
}

s32 Fld_TalkEventTitle_Update(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventTitle* title = task->data;

    if (title->slideFrames != 0) {
        func_020265d4(&title->posX, title->targetPosX, title->slideFrames);
        title->slideFrames--;
        title->sprite.posX = title->posX >> 12;
    }
    Sprite_Update(&title->sprite);
    return 1;
}

s32 Fld_TalkEventTitle_Render(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventTitle* title = task->data;

    Sprite_RenderFrame(&title->sprite);
    return 1;
}

s32 Fld_TalkEventTitle_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventTitle* title = task->data;

    Sprite_Release(&title->sprite);
    return 1;
}

s32 Fld_TalkEventTitle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventTitle_Init,
        .update     = Fld_TalkEventTitle_Update,
        .render     = Fld_TalkEventTitle_Render,
        .cleanup    = Fld_TalkEventTitle_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventTitle_CreateTask(TaskPool* pool, s32 dataType) {
    Fld_TalkEventTitle_Args args;
    args.dataType = dataType;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventTitle, NULL, 0, NULL, &args);
}
