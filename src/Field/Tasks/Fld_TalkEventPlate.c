// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020c36d0-0x020c3948; 8 functions.
// Research draft: docs/decomp/ov030/Fld_TalkEventPlate.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Field/TaskHelpers.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;  // 0x40
    /* 0x40 */ s32    x;       // fx32, slides towards targetX
    /* 0x44 */ s32    targetX; // fx32
    /* 0x48 */ u16    timer;
    /* 0x4A */ u16    pad4A;
} Ov030PlateSprite; // 0x4C

// Also the parameter block created by func_ov030_020c3908.
typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s32 animIndex;
} Ov030PlateConfig; // 0xC

typedef TaskStages Ov030PlateFns;

extern Ov030PlateFns   data_ov030_020e7178;
extern TaskHandle      data_ov030_020e716c;
extern SpriteAnimation data_ov030_020e7188;

SpriteFrameInfo* func_ov030_020c36d0(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0x10)
}

void func_ov030_020c3774(Sprite* arg0, Ov030PlateConfig* arg1, s32 arg2, s32 arg3) {
    SpriteAnimation anim = data_ov030_020e7188;

    anim.dataType  = arg1->dataType;
    anim.posX      = arg2;
    anim.posY      = arg3;
    anim.animIndex = arg1->animIndex;
    _Sprite_Load(arg0, &anim);
}

s32 func_ov030_020c37f8(TaskPool* arg0, Task* arg1, Ov030PlateConfig* arg2) {
    Ov030PlateSprite* sprite = arg1->data;

    sprite->targetX = arg2->x << 12;
    if (arg2->x < 0x80) {
        sprite->x = (arg2->x - 0xA8) << 12;
    } else {
        sprite->x = (arg2->x + 0xA8) << 12;
    }
    func_ov030_020c3774(&sprite->sprite, arg2, (sprite->x * 0x10) >> 0x10, arg2->y);
    sprite->timer = 0x14;
    return 1;
}

s32 func_ov030_020c3850(TaskPool* arg0, Task* arg1) {
    Ov030PlateSprite* sprite = arg1->data;
    u16               timer  = sprite->timer;

    if (timer != 0) {
        func_02026590(&sprite->x, sprite->targetX, sprite->timer);
        sprite->timer       = sprite->timer - 1;
        sprite->sprite.posX = (s16)(sprite->x >> 12);
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

s32 func_ov030_020c3898(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_RenderFrame(arg1->data);
    return 1;
}

s32 func_ov030_020c38ac(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_Release(arg1->data);
    return 1;
}

s32 func_ov030_020c38c0(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030PlateFns funcs = data_ov030_020e7178;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020c3908(TaskPool* arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4) {
    Ov030PlateConfig params;

    params.dataType  = arg1;
    params.x         = arg2;
    params.animIndex = arg4;
    params.y         = arg3;
    return EasyTask_CreateTask(arg0, &data_ov030_020e716c, NULL, 0, NULL, &params);
}
