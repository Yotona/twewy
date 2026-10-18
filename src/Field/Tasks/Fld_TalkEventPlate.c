#include "Field/FieldRegion.h"

#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/Tasks/Fld_TalkEventPlate.h"
#include "SpriteMgr.h"

extern void func_02026590(s32* value, s32 target, u16 frames);

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    x;       // fx32, slides towards targetX
    /* 0x44 */ s32    targetX; // fx32
    /* 0x48 */ u16    timer;
    /* 0x4A */ u16    pad4A;
} Ov030PlateSprite; // Size: 0x4C

// Also the parameter block created by Fld_TalkEventPlate_CreateTask.
typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s32 animIndex;
} Ov030PlateConfig; // Size: 0xC

static SpriteFrameInfo* Fld_TalkEventPlate_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TalkEventPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TalkEventPlate = {"Tsk_Fld_TalkEventPlate", Fld_TalkEventPlate_RunTask,
                                                  sizeof(Ov030PlateSprite)};

static const SpriteAnimation Fld_TalkEventPlate_Anim = {
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
    .frameInfoCallback = Fld_TalkEventPlate_GetFrameInfo,
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
    .animIndex         = 2,
};

static SpriteFrameInfo* Fld_TalkEventPlate_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0x10)
}

static void Fld_TalkEventPlate_Load(Sprite* sprite, Ov030PlateConfig* arg1, s32 arg2, s32 arg3) {
    SpriteAnimation anim = Fld_TalkEventPlate_Anim;

    anim.dataType  = arg1->dataType;
    anim.posX      = arg2;
    anim.posY      = arg3;
    anim.animIndex = arg1->animIndex;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TalkEventPlate_Init(TaskPool* pool, Task* task, void* args) {
    Ov030PlateConfig* arg2   = args;
    Ov030PlateSprite* sprite = task->data;

    sprite->targetX = arg2->x << 12;
    if (arg2->x < 0x80) {
        sprite->x = (arg2->x - 0xA8) << 12;
    } else {
        sprite->x = (arg2->x + 0xA8) << 12;
    }
    Fld_TalkEventPlate_Load(&sprite->sprite, arg2, (sprite->x * 0x10) >> 0x10, arg2->y);
    sprite->timer = 0x14;
    return 1;
}

static s32 Fld_TalkEventPlate_Update(TaskPool* pool, Task* task, void* args) {
    Ov030PlateSprite* sprite = task->data;
    u16               timer  = sprite->timer;

    if (timer != 0) {
        func_02026590(&sprite->x, sprite->targetX, sprite->timer);
        sprite->timer       = sprite->timer - 1;
        sprite->sprite.posX = (s16)(sprite->x >> 12);
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

static s32 Fld_TalkEventPlate_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_TalkEventPlate_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_TalkEventPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventPlate_Init,
        .update     = Fld_TalkEventPlate_Update,
        .render     = Fld_TalkEventPlate_Render,
        .cleanup    = Fld_TalkEventPlate_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventPlate_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4) {
    Ov030PlateConfig params;

    params.dataType  = arg1;
    params.x         = arg2;
    params.animIndex = arg4;
    params.y         = arg3;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TalkEventPlate, NULL, 0, NULL, &params);
}
