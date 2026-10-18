#include "Field/Tasks/Fld_NSymbolUGEffect.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
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

static SpriteFrameInfo* Fld_NSymbolUGEffect_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_NSymbolUGEffect_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_NSymbolUGEffect = {"Tsk_Fld_NSymbolUGEffect", Fld_NSymbolUGEffect_RunTask,
                                                   sizeof(Ov030UGEffectSprite)};

static const SpriteAnimation Fld_NSymbolUGEffect_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0000,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_NSymbolUGEffect_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldEncount,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 43,
    .unk_1E            = 0,
    .unk_20            = 46,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 44,
    .unk_28            = 45,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_NSymbolUGEffect_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
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

static void Fld_NSymbolUGEffect_Load(Sprite* sprite, s32 dataType, Ov030UGEffectConfig* config) {
    SpriteAnimation anim = Fld_NSymbolUGEffect_Anim;
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

static s32 Fld_NSymbolUGEffect_Init(TaskPool* pool, Task* task, void* args) {
    Ov030UGEffectConfig* config = args;
    Ov030UGEffectSprite* sprite = task->data;

    Fld_NSymbolUGEffect_Load(&sprite->sprite, config->dataType, config);
    sprite->pos = config->pos;
    return 1;
}

static s32 Fld_NSymbolUGEffect_Update(TaskPool* pool, Task* task, void* args) {
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

static s32 Fld_NSymbolUGEffect_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_Render(&((Ov030UGEffectSprite*)task->data)->sprite);
    return 1;
}

static s32 Fld_NSymbolUGEffect_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(&((Ov030UGEffectSprite*)task->data)->sprite);
    return 1;
}

static s32 Fld_NSymbolUGEffect_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_NSymbolUGEffect_Init,
        .update     = Fld_NSymbolUGEffect_Update,
        .render     = Fld_NSymbolUGEffect_Render,
        .cleanup    = Fld_NSymbolUGEffect_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_NSymbolUGEffect_CreateTask(TaskPool* pool, s32 dataType, u16* pos, s32 mode, s32 effect) {
    Ov030UGEffectConfig params;

    params.dataType = dataType;
    params.pos      = pos;
    params.mode     = mode;
    params.effect   = effect;
    return EasyTask_CreateTask(pool, &Tsk_Fld_NSymbolUGEffect, NULL, 0, NULL, &params);
}
