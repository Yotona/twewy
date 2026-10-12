#include "Engine/EasyTask.h"
#include "Field/FieldTasks.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprites[2];
} Fld_TalkEventCursor; // Size: 0x80

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 posX;
    /* 0x6 */ s16 posY;
} Fld_TalkEventCursor_Args;

extern const BinIdentifier data_ov030_020d9944;

SpriteFrameInfo* Fld_TalkEventCursor_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
s32              Fld_TalkEventCursor_Init(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventCursor_Update(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventCursor_Render(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventCursor_Destroy(TaskPool* pool, Task* task, void* args);
s32              Fld_TalkEventCursor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle      Tsk_Fld_TalkEventCursor  = {"Tsk_Fld_TalkEventCursor", Fld_TalkEventCursor_RunTask,
                                                         sizeof(Fld_TalkEventCursor)};
static const SpriteAnimation Fld_TalkEventCursor_Anim = {
    .bits_0_1          = 1,
    .bits_7_9          = 5,
    .bits_12_13        = 1,
    .frameInfoCallback = Fld_TalkEventCursor_GetFrameInfo,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 7,
    .unk_1C            = 1,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 4,
};

SpriteFrameInfo* Fld_TalkEventCursor_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0xC);
}

s32 Fld_TalkEventCursor_Load(Sprite* sprite, Fld_TalkEventCursor_Args* args, s32 index) {
    SpriteAnimation anim = Fld_TalkEventCursor_Anim;
    Unk_Bitfield    attr;

    anim.dataType = args->dataType;
    anim.posX     = args->posX;
    anim.posY     = args->posY;
    anim.animIndex += index;
    if (index == 0 && args->posX < 0x80) {
        anim.posX -= 0x11;
        attr = anim.unk_02;
        attr.unk_05 |= 0x8;
        anim.unk_02 = attr;
    }
    if (index == 1) {
        anim.bits_12_13 = 2;
    }
    return _Sprite_Load(sprite, &anim);
}

s32 Fld_TalkEventCursor_Init(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventCursor* cursor = task->data;
    s16                  i;

    for (i = 0; i < 2; i++) {
        Fld_TalkEventCursor_Load(&cursor->sprites[i], args, i);
    }
    SndMgr_StartPlayingSE(0x11a);
    return TRUE;
}

s32 Fld_TalkEventCursor_Update(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventCursor* cursor = task->data;
    s16                  i;

    for (i = 0; i < 2; i++) {
        Sprite_Update(&cursor->sprites[i]);
    }
    return TRUE;
}

s32 Fld_TalkEventCursor_Render(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventCursor* cursor = task->data;
    s16                  i;

    for (i = 0; i < 2; i++) {
        Sprite_RenderFrame(&cursor->sprites[i]);
    }
    return TRUE;
}

s32 Fld_TalkEventCursor_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventCursor* cursor = task->data;
    s16                  i;

    for (i = 0; i < 2; i++) {
        Sprite_Release(&cursor->sprites[i]);
    }
    return TRUE;
}

s32 Fld_TalkEventCursor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventCursor_Init,
        .update     = Fld_TalkEventCursor_Update,
        .render     = Fld_TalkEventCursor_Render,
        .cleanup    = Fld_TalkEventCursor_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventCursor_CreateTask(TaskPool* pool, s32 dataType, s16 posX, s16 posY) {
    Fld_TalkEventCursor_Args args;

    args.dataType = dataType;
    args.posX     = posX;
    args.posY     = posY;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventCursor, NULL, 0, NULL, &args);
}
