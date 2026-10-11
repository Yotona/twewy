#include "Engine/EasyTask.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

// USA 0x020baa64-0x020bad80: the field talk-icon sprite task, a Sprite that
// follows a screen position (raised by scale * height). A SpriteAnimation
// template at data_ov030_020e5eb4 is copied and patched per spawned icon.

typedef struct {
    /* 0x00 */ Sprite    sprite;
    /* 0x40 */ FieldPos* screen;
    /* 0x44 */ s32*      scale;
    /* 0x48 */ s32       height;
} TalkIconSprite; // Size: 0x4C

typedef struct {
    /* 0x00 */ s32       dataType;
    /* 0x04 */ FieldPos* screen;
    /* 0x08 */ s32*      scale;
    /* 0x0C */ s32       height;
    /* 0x10 */ s32       animIndex;
} TalkIconArgs; // Size: 0x14

typedef TaskStages TalkIconFns;

extern SpriteAnimation data_ov030_020e5eb4;
extern TalkIconFns     data_ov030_020e5ea4;
extern TaskHandle      data_ov030_020e5e98;

SpriteFrameInfo* func_ov030_020baa64(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0xC0 - sprite->posY)
}

void func_ov030_020bab0c(Sprite* sprite, TalkIconArgs* args) {
    SpriteAnimation anim = data_ov030_020e5eb4;

    anim.dataType  = args->dataType;
    anim.animIndex = args->animIndex;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020bab80(TaskPool* pool, Task* task, TalkIconArgs* args) {
    TalkIconSprite* sprite = task->data;

    sprite->screen = args->screen;
    sprite->scale  = args->scale;
    sprite->height = args->height;
    func_ov030_020bab0c(&sprite->sprite, args);
    if (SndMgr_IsSEPlaying((SndMgrSeIdx)0xC3) == 0 && gSaveData.unk_2498 == 0) {
        SndMgr_StartPlayingSE((SndMgrSeIdx)0xC3);
    }
    return 1;
}

s32 func_ov030_020babd8(TaskPool* pool, Task* task) {
    Sprite_UpdateAndCheck(task->data);
    return 1;
}

s32 func_ov030_020babec(TaskPool* pool, Task* task) {
    TalkIconSprite* sprite = task->data;
    s32             offset = (s32)(((s64)sprite->height * *sprite->scale + 0x800) >> 0xC);

    sprite->sprite.posX = sprite->screen->x >> 0xC;
    sprite->sprite.posY = (sprite->screen->y - offset) >> 0xC;
    if (func_ov030_020aefe0() == 0) {
        Sprite_Render(&sprite->sprite);
    }
    return 1;
}

s32 func_ov030_020bac54(TaskPool* pool, Task* task) {
    Sprite_Destroy(task->data);
    return 1;
}

s32 func_ov030_020bac68(TaskPool* pool, Task* task, void* args, s32 stage) {
    TalkIconFns funcs = data_ov030_020e5ea4;

    return funcs.iter[stage](pool, task, args);
}

s32 func_ov030_020bacb0(TaskPool* pool, s32 dataType, FieldPos* screen, s32* scale, s32 height, s32 animIndex) {
    TalkIconArgs params;

    params.dataType  = dataType;
    params.screen    = screen;
    params.scale     = scale;
    params.height    = height;
    params.animIndex = animIndex;
    return EasyTask_CreateTask(pool, &data_ov030_020e5e98, NULL, 0, NULL, &params);
}

void func_ov030_020bacf8(Task* task) {
    TalkIconSprite* sprite = task->data;

    if (sprite->sprite.animIndex == 1) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 2, sprite->sprite.cellTable);
    } else if (sprite->sprite.animIndex == 3) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 4, sprite->sprite.cellTable);
    }
}

void func_ov030_020bad3c(Task* task) {
    TalkIconSprite* sprite = task->data;

    if (sprite->sprite.animIndex == 2) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 1, sprite->sprite.cellTable);
    } else if (sprite->sprite.animIndex == 4) {
        Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, 3, sprite->sprite.cellTable);
    }
}
