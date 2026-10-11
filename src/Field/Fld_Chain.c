#include "Engine/EasyTask.h"
#include "Field/TaskHelpers.h"
#include "SpriteMgr.h"

// USA 0x020c2088-0x020c248c: the combo/chain number display task. The task data
// is four Sprites (tens, units, two static glyphs) followed by the fixed-point
// vertical offset/step, the shrink timer and the number of active glyphs.

typedef struct {
    /* 0x000 */ Sprite sprites[4];
    /* 0x100 */ s32    posY;    // fx32, eases towards targetY
    /* 0x104 */ s32    targetY; // fx32
    /* 0x108 */ s16    timer;
    /* 0x10A */ s16    unk10A;
    /* 0x10C */ s32    count; // glyphs in use
} FldChainData;               // Size: 0x110

typedef struct {
    /* 0x0 */ u32 dataType;
    /* 0x4 */ s32 value; // drawn as tens/units
} FldChainArgs;

typedef TaskStages FldChainFns;

extern TaskHandle      data_ov030_020e7004;
extern FldChainFns     data_ov030_020e7010;
extern SpriteAnimation data_ov030_020e7020;

extern void func_ov030_020af644(s16* out0, s16* out1);

SpriteFrameInfo* func_ov030_020c2088(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Fills one glyph of the combo counter: the shared animation template plus the
// dataType, the glyph frame and its screen position.
void func_ov030_020c2124(Sprite* sprite, s32 dataType, s16 animIndex, s16 posX, s16 posY) {
    SpriteAnimation anim = data_ov030_020e7020;

    anim.dataType  = dataType;
    anim.animIndex = animIndex;
    anim.posX      = posX;
    anim.posY      = posY;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020c21a4(TaskPool* pool, Task* task, FldChainArgs* args) {
    FldChainData* data = task->data;
    s16           posX;
    s16           posY;
    s32           units;
    s32           tens;

    units = args->value % 10;
    tens  = args->value / 10;
    func_ov030_020af644(&posX, &posY);
    posX -= 4;
    func_ov030_020c2124(&data->sprites[0], args->dataType, 0xB, posX, posY);
    posX += 0x17;
    data->count = 3;
    if (tens != 0) {
        func_ov030_020c2124(&data->sprites[3], args->dataType, (s16)(tens + 1), posX, posY);
        posX += 7;
        data->count = data->count + 1;
    }
    func_ov030_020c2124(&data->sprites[1], args->dataType, (s16)(units + 1), posX, posY);
    posX += 8;
    func_ov030_020c2124(&data->sprites[2], args->dataType, 0xC, posX, posY);
    data->posY = posY << 0xC;
    posY -= 0x20;
    if (posY < 8) {
        posY = 8;
    }
    data->targetY = posY << 0xC;
    data->timer   = 0x20;
    SndMgr_PlaySEWithPan(0x114, posX);
    return 1;
}

s32 func_ov030_020c2310(TaskPool* pool, Task* task) {
    FldChainData* data = task->data;
    s32           i;

    if (data->timer != 0) {
        func_020265d4(&data->posY, data->targetY, (u16)data->timer);
        data->timer = data->timer - 1;
    }
    for (i = 0; i < data->count; i++) {
        Sprite_Update(&data->sprites[i]);
        data->sprites[i].posY = data->posY >> 0xC;
    }
    return 1;
}

s32 func_ov030_020c2390(TaskPool* pool, Task* task) {
    FldChainData* data = task->data;
    s32           i;

    for (i = 0; i < data->count; i++) {
        Sprite_RenderFrame(&data->sprites[i]);
    }
    return 1;
}

s32 func_ov030_020c23d0(TaskPool* pool, Task* task) {
    FldChainData* data = task->data;
    s32           i;

    for (i = 0; i < data->count; i++) {
        Sprite_Release(&data->sprites[i]);
    }
    return 1;
}

s32 func_ov030_020c2410(TaskPool* pool, Task* task, void* args, s32 stage) {
    FldChainFns funcs = data_ov030_020e7010;

    return funcs.iter[stage](pool, task, args);
}

s32 func_ov030_020c2458(TaskPool* pool, s32 dataType, s32 value) {
    FldChainArgs params;

    params.dataType = dataType;
    params.value    = value;
    return EasyTask_CreateTask(pool, &data_ov030_020e7004, NULL, 0, NULL, &params);
}
