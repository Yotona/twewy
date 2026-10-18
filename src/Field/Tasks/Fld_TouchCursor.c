#include "Field/Tasks/Fld_TouchCursor.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk40;
    /* 0x44 */ s16    unk44;
    /* 0x46 */ s16    unk46;
} Ov030TouchCursorSprite;

static SpriteFrameInfo* Fld_TouchCursor_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TouchCursor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TouchCursor = {"Tsk_Fld_TouchCursor", Fld_TouchCursor_RunTask, sizeof(Ov030TouchCursorSprite)};

static const SpriteAnimation Fld_TouchCursor_Anim = {
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
    .frameInfoCallback = Fld_TouchCursor_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldTouch,
    .unk_18            = 2,
    .packIndex         = 2,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 2,
};

static SpriteFrameInfo* Fld_TouchCursor_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_TouchCursor_Load(Sprite* sprite, FldSpriteAnimationParams* arg1) {
    SpriteAnimation anim = Fld_TouchCursor_Anim;

    anim.dataType  = arg1->dataType;
    anim.animIndex = arg1->animIndex;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TouchCursor_Init(TaskPool* pool, Task* task, void* args) {
    FldSpriteAnimationParams* arg2   = args;
    Ov030TouchCursorSprite*   sprite = task->data;

    Fld_TouchCursor_Load(&sprite->sprite, arg2);
    sprite->unk40 = 0;
    sprite->unk44 = 0;
    sprite->unk46 = arg2->animIndex;
    return 1;
}

static s32 Fld_TouchCursor_Update(TaskPool* pool, Task* task, void* args) {
    Ov030TouchCursorSprite* sprite = task->data;
    TouchCoord              coord;
    s16                     temp;

    if (TouchInput_WasTouchPressed() != 0) {
        Sprite_SetAnimation(&sprite->sprite, sprite->sprite.animData, sprite->unk46, sprite->sprite.cellTable);
        sprite->unk44 = 8;
        if (sprite->unk46 == 1) {
            sprite->unk44 = 0xC;
        }
    }
    if (TouchInput_IsTouchActive() != 0) {
        sprite->unk40 = 1;
        TouchInput_GetCoord(&coord);
        sprite->sprite.posX = coord.x;
        sprite->sprite.posY = coord.y;
    } else if (sprite->unk44 != 0) {
        sprite->unk40 = 1;
    } else {
        sprite->unk40 = 0;
    }
    temp = sprite->unk44;
    if (temp != 0) {
        sprite->unk44 = temp - 1;
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

static s32 Fld_TouchCursor_Render(TaskPool* pool, Task* task, void* args) {
    Ov030TouchCursorSprite* sprite = task->data;

    if (sprite->unk40 != 0) {
        Sprite_RenderFrame(&sprite->sprite);
    }
    return 1;
}

static s32 Fld_TouchCursor_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_TouchCursor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TouchCursor_Init,
        .update     = Fld_TouchCursor_Update,
        .render     = Fld_TouchCursor_Render,
        .cleanup    = Fld_TouchCursor_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_TouchCursor_CreateTask(TaskPool* pool, s32 arg1, s16 arg2) {
    FldSpriteAnimationParams params;

    params.dataType  = arg1;
    params.animIndex = arg2;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TouchCursor, NULL, 0, NULL, &params);
}

void Fld_TouchCursor_SetAnimation(Task* task, s16 arg1) {
    Ov030TouchCursorSprite* sprite = task->data;

    Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, arg1, sprite->sprite.cellTable);
    sprite->unk46 = arg1;
}
