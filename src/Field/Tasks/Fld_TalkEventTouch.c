#include "Field/Tasks/Fld_TalkEventTouch.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprites[2];
} Ov030TalkTouch; // Size: 0x80

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ s32 style;
} Ov030TalkParams;

static SpriteFrameInfo* Fld_TalkEventTouch_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TalkEventTouch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TalkEventTouch = {"Tsk_Fld_TalkEventTouch", Fld_TalkEventTouch_RunTask,
                                                  sizeof(Ov030TalkTouch)};

static const SpriteAnimation Fld_TalkEventTouch_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0000,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_TalkEventTouch_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 7,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 6,
};

static SpriteFrameInfo* Fld_TalkEventTouch_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0x12)
}

static void Fld_TalkEventTouch_LoadPart(Sprite* sprite, Ov030TalkParams* params, s16 part) {
    SpriteAnimation anim = Fld_TalkEventTouch_Anim;

    anim.dataType = params->dataType;
    anim.posX     = params->x;
    anim.posY     = params->y;
    if (params->style == 0) {
        if (part == 0 && params->x >= 0x80) {
            anim.animIndex = 0xA;
        } else {
            anim.animIndex += part;
        }
    } else {
        anim.unk_02.raw &= ~0xC00;
        if (part == 0) {
            if (params->x >= 0x80) {
                anim.animIndex = 0xD;
            } else {
                anim.animIndex = 0xC;
            }
        } else {
            anim.animIndex = 0xB;
        }
    }
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TalkEventTouch_Init(TaskPool* pool, Task* task, void* args) {
    Ov030TalkParams* params = args;
    Ov030TalkTouch*  touch  = task->data;
    s16              i;

    for (i = 1; i >= 0; i--) {
        Fld_TalkEventTouch_LoadPart(&touch->sprites[i], params, i);
    }
    return 1;
}

static s32 Fld_TalkEventTouch_Update(TaskPool* pool, Task* task, void* args) {
    Ov030TalkTouch* touch = task->data;
    s16             i;

    for (i = 1; i >= 0; i--) {
        Sprite_Update(&touch->sprites[i]);
    }
    if (touch->sprites[0].isPlaying == 1) {
        SndMgr_PlaySEWithPan(0x124, touch->sprites[0].posX);
    }
    return 1;
}

static s32 Fld_TalkEventTouch_Render(TaskPool* pool, Task* task, void* args) {
    Ov030TalkTouch* touch = task->data;
    s16             i;

    for (i = 1; i >= 0; i--) {
        Sprite_RenderFrame(&touch->sprites[i]);
    }
    return 1;
}

static s32 Fld_TalkEventTouch_Destroy(TaskPool* pool, Task* task, void* args) {
    Ov030TalkTouch* touch = task->data;
    s16             i;

    for (i = 1; i >= 0; i--) {
        Sprite_Release(&touch->sprites[i]);
    }
    return 1;
}

static s32 Fld_TalkEventTouch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventTouch_Init,
        .update     = Fld_TalkEventTouch_Update,
        .render     = Fld_TalkEventTouch_Render,
        .cleanup    = Fld_TalkEventTouch_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventTouch_CreateTask(TaskPool* pool, s32 dataType, s16 x, s16 y) {
    Ov030TalkParams params;

    params.x        = x;
    params.dataType = dataType;
    params.y        = y;
    params.style    = 0;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventTouch, NULL, 0, NULL, &params);
}

s32 Fld_TalkEventTouch_CreateKeywordTask(TaskPool* pool, s32 dataType, s16 x, s16 y) {
    Ov030TalkParams params;

    params.dataType = dataType;
    params.x        = x;
    params.style    = 1;
    params.y        = y;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventTouch, NULL, 0, NULL, &params);
}
