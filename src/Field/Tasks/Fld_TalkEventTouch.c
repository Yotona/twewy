// USA TU; original assembly remains linked (incomplete).
// Range: 0x020c3380-0x020c36d0; 9 functions.
// Research draft: docs/decomp/ov030/Fld_TalkEventTouch.c
#include "Engine/EasyTask.h"
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

extern TaskHandle      data_ov030_020e7124;
extern TaskStages      data_ov030_020e7130;
extern SpriteAnimation data_ov030_020e7140;

SpriteFrameInfo* func_ov030_020c3380(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0x12)
}

void func_ov030_020c3424(Sprite* sprite, Ov030TalkParams* params, s16 part) {
    SpriteAnimation anim = data_ov030_020e7140;

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

s32 func_ov030_020c3510(TaskPool* pool, Task* task, Ov030TalkParams* params) {
    Ov030TalkTouch* touch = task->data;
    s16             i;

    for (i = 1; i >= 0; i--) {
        func_ov030_020c3424(&touch->sprites[i], params, i);
    }
    return 1;
}

s32 func_ov030_020c3550(TaskPool* pool, Task* task) {
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

s32 func_ov030_020c35a4(TaskPool* pool, Task* task) {
    Ov030TalkTouch* touch = task->data;
    s16             i;

    for (i = 1; i >= 0; i--) {
        Sprite_RenderFrame(&touch->sprites[i]);
    }
    return 1;
}

s32 func_ov030_020c35d8(TaskPool* pool, Task* task) {
    Ov030TalkTouch* touch = task->data;
    s16             i;

    for (i = 1; i >= 0; i--) {
        Sprite_Release(&touch->sprites[i]);
    }
    return 1;
}

s32 func_ov030_020c360c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov030_020e7130;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020c3654(TaskPool* pool, s32 dataType, s16 x, s16 y) {
    Ov030TalkParams params;

    params.x        = x;
    params.dataType = dataType;
    params.y        = y;
    params.style    = 0;
    return EasyTask_CreateTask(pool, &data_ov030_020e7124, NULL, 0, NULL, &params);
}

s32 func_ov030_020c3690(TaskPool* pool, s32 dataType, s16 x, s16 y) {
    Ov030TalkParams params;

    params.dataType = dataType;
    params.x        = x;
    params.style    = 1;
    params.y        = y;
    return EasyTask_CreateTask(pool, &data_ov030_020e7124, NULL, 0, NULL, &params);
}
