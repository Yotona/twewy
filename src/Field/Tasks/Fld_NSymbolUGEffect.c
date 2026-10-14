#include "Field/TaskHelpers.h"
// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020bbe08-0x020bc068; 8 functions.
// Research draft: docs/decomp/ov030/Fld_NSymbolUGEffect.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ u16*   pos; // followed sprite's posX/posY pair
} Ov030UGEffectSprite;

typedef struct {
    /* 0x00 */ s32  dataType;
    /* 0x04 */ u16* pos;
    /* 0x08 */ s32  mode;
    /* 0x0C */ s32  effect;
} Ov030UGEffectConfig;

typedef TaskStages Ov030UGEffectFns;

extern SpriteAnimation  data_ov030_020e6034;
extern Ov030UGEffectFns data_ov030_020e6024;
extern TaskHandle       data_ov030_020e6018;

// Written out instead of `Sprite_FrameInfoCallbackSorted` (which hoists
// `info_ = &g_SpriteFrameInfo` above the switch): the original keeps `info_`
// NULL until the render case, so the zero constant is shared with the
// pieceCount/cellPieces/affine stores and the single exit `mov r0, r3`.
SpriteFrameInfo* func_ov030_020bbe08(Sprite* sprite, s32 arg, s32 mode) {
    SpriteFrameInfo* info_ = NULL;

    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER: {
            SpriteFrameInfo* temp_ = &g_SpriteFrameInfo;
            Sprite_FillFrameInfo(temp_, sprite);
            temp_->sortKey = 0x181 - sprite->posY * 2;
            info_          = temp_;
        } break;
    }
    return info_;
}

// posY is assigned before posX on purpose: mwcc then loads pos[0] first and
// lets pos[1]'s load reuse the base register, which is the target's pairing
// (`ldrh r1,[r1,#0]` / `ldrh r0,[r1,#2]`). Written in the natural order both
// the loads and the stores come out with the two values' registers swapped.
void func_ov030_020bbeb8(Sprite* sprite, s32 dataType, Ov030UGEffectConfig* config) {
    SpriteAnimation anim = data_ov030_020e6034;
    u16*            pos;
    s16             posY;
    s16             posX;

    anim.dataType = config->dataType;
    pos           = config->pos;
    posY          = pos[1];
    posX          = pos[0];
    anim.posY     = posY;
    anim.posX     = posX;
    if (config->mode == 1) {
        anim.unk_20 = 0x2F;
    }
    if (config->effect == 1) {
        anim.animIndex = 2;
    } else if (config->effect == 2) {
        anim.animIndex = 3;
    }
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020bbf64(TaskPool* pool, Task* task, Ov030UGEffectConfig* config) {
    Ov030UGEffectSprite* sprite = task->data;

    func_ov030_020bbeb8(&sprite->sprite, config->dataType, config);
    sprite->pos = config->pos;
    return 1;
}

s32 func_ov030_020bbf8c(TaskPool* pool, Task* task) {
    Ov030UGEffectSprite* sprite = task->data;
    u16*                 pos;
    s16                  x;
    s16                  v;

    Sprite_UpdateAndCheck(&sprite->sprite);
    pos                 = sprite->pos;
    v                   = pos[0];
    x                   = v;
    v                   = pos[1];
    sprite->sprite.posX = x;
    sprite->sprite.posY = v;
    return 1;
}

s32 func_ov030_020bbfb8(TaskPool* pool, Task* task) {
    Sprite_Render(&((Ov030UGEffectSprite*)task->data)->sprite);
    return 1;
}

s32 func_ov030_020bbfcc(TaskPool* pool, Task* task) {
    Sprite_Destroy(&((Ov030UGEffectSprite*)task->data)->sprite);
    return 1;
}

s32 func_ov030_020bbfe0(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov030UGEffectFns funcs = data_ov030_020e6024;

    return funcs.iter[stage](pool, task, args);
}

s32 func_ov030_020bc028(TaskPool* pool, s32 dataType, u16* pos, s32 mode, s32 effect) {
    Ov030UGEffectConfig params;

    params.dataType = dataType;
    params.pos      = pos;
    params.mode     = mode;
    params.effect   = effect;
    return EasyTask_CreateTask(pool, &data_ov030_020e6018, NULL, 0, NULL, &params);
}
