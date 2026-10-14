// USA TU; original assembly remains linked (incomplete).
// Range: 0x020bd5cc-0x020bd890; 8 functions.
// Research draft: docs/decomp/ov030/Fld_CockPit.c
#include "Engine/EasyTask.h"
#include "Field/TaskHelpers.h"
#include "SpriteMgr.h"

s32 func_020236a0(void);

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s16    unk40;
} Ov030CockPitSprite;

extern TaskHandle      data_ov030_020e6234;
extern TaskStages      data_ov030_020e6240;
extern SpriteAnimation data_ov030_020e6250;

SpriteFrameInfo* func_ov030_020bd5cc(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

void func_ov030_020bd668(Sprite* sprite, FldSpriteAnimationParams* params) {
    SpriteAnimation anim = data_ov030_020e6250;

    anim.dataType  = params->dataType;
    anim.animIndex = params->animIndex;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020bd6dc(TaskPool* pool, Task* task, FldSpriteAnimationParams* params) {
    Ov030CockPitSprite* sprite = task->data;

    sprite->unk40 = params->animIndex;
    func_ov030_020bd668(&sprite->sprite, params);
    return 1;
}

s32 func_ov030_020bd6fc(TaskPool* pool, Task* task) {
    Ov030CockPitSprite* sprite = task->data;
    s16                 state;

    state = sprite->unk40;
    if (state == 3) {
        if (func_0202599c(0xA) == 0) {
            Sprite_SetAnimation(&sprite->sprite, sprite->sprite.animData, 4, sprite->sprite.cellTable);
            sprite->unk40 = 4;
        }
    } else if (state == 4 && func_0202599c(0xA) != 0) {
        Sprite_SetAnimation(&sprite->sprite, sprite->sprite.animData, 3, sprite->sprite.cellTable);
        sprite->unk40 = 3;
    }
    state = sprite->unk40;
    if (state == 1) {
        if (func_020236a0() == 0) {
            Sprite_SetAnimation(&sprite->sprite, sprite->sprite.animData, 2, sprite->sprite.cellTable);
            sprite->unk40 = 2;
        }
    } else if (state == 2 && func_020236a0() != 0) {
        Sprite_SetAnimation(&sprite->sprite, sprite->sprite.animData, 1, sprite->sprite.cellTable);
        sprite->unk40 = 1;
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

s32 func_ov030_020bd7ec(TaskPool* pool, Task* task) {
    Sprite_RenderFrame(task->data);
    return 1;
}

s32 func_ov030_020bd800(TaskPool* pool, Task* task) {
    Sprite_Release(task->data);
    return 1;
}

s32 func_ov030_020bd814(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov030_020e6240;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020bd85c(TaskPool* pool, s32 dataType, s16 animIndex) {
    FldSpriteAnimationParams params;

    params.dataType  = dataType;
    params.animIndex = animIndex;
    return EasyTask_CreateTask(pool, &data_ov030_020e6234, NULL, 0, NULL, &params);
}
