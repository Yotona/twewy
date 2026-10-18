#include "Field/Tasks/Fld_CockPit.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Player/Inventory.h"
#include "SpriteMgr.h"

extern s32 func_0202599c(s32 flagIndex);

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s16    unk40;
} Ov030CockPitSprite;

static SpriteFrameInfo* Fld_CockPit_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_CockPit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_CockPit = {"Tsk_Fld_CockPit", Fld_CockPit_RunTask, sizeof(Ov030CockPitSprite)};

static const SpriteAnimation Fld_CockPit_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 192,
    .posY              = 0,
    .frameInfoCallback = Fld_CockPit_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 4,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 11,
};

static SpriteFrameInfo* Fld_CockPit_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

static void Fld_CockPit_Load(Sprite* sprite, FldSpriteAnimationParams* params) {
    SpriteAnimation anim = Fld_CockPit_Anim;

    anim.dataType  = params->dataType;
    anim.animIndex = params->animIndex;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_CockPit_Init(TaskPool* pool, Task* task, void* args) {
    FldSpriteAnimationParams* params = args;
    Ov030CockPitSprite*       sprite = task->data;

    sprite->unk40 = params->animIndex;
    Fld_CockPit_Load(&sprite->sprite, params);
    return 1;
}

static s32 Fld_CockPit_Update(TaskPool* pool, Task* task, void* args) {
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

static s32 Fld_CockPit_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_CockPit_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_CockPit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_CockPit_Init,
        .update     = Fld_CockPit_Update,
        .render     = Fld_CockPit_Render,
        .cleanup    = Fld_CockPit_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_CockPit_CreateTask(TaskPool* pool, s32 dataType, s16 animIndex) {
    FldSpriteAnimationParams params;

    params.dataType  = dataType;
    params.animIndex = animIndex;
    return EasyTask_CreateTask(pool, &Tsk_Fld_CockPit, NULL, 0, NULL, &params);
}
