#include "Field/Tasks/Fld_TouchSeal.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Event/EventTables.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
} Ov030TouchSealEntry;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
} Ov030TouchSealConfig;

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk40;
} Ov030TouchSealSprite;

static SpriteFrameInfo* Fld_TouchSeal_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TouchSeal_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TouchSeal = {"Tsk_Fld_TouchSeal", Fld_TouchSeal_RunTask, sizeof(Ov030TouchSealSprite)};

static const Ov030TouchSealEntry Fld_TouchSeal_Seals[5] = {
    {3, 16, 24},
    {4, 24, 24},
    {6, 16, 32},
    {5, 16, 16},
    {7, 32, 24},
};

static const SpriteAnimation Fld_TouchSeal_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_TouchSeal_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldTouch,
    .unk_18            = 2,
    .packIndex         = 3,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 2,
};

static SpriteFrameInfo* Fld_TouchSeal_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching: register allocation + load scheduling in the SpriteAnimation build
// (retail loads the config dataType before the table entry and keeps &anim in r1
// while using r2 for the index and lr for the base; ours hoists the anim-word load
// of the dataType read-modify-write and sinks the dataType load to the insert)
// (USA 70.61%; JP unvalidated).
static void Fld_TouchSeal_Load(Sprite* sprite, Ov030TouchSealConfig* arg1) {
    SpriteAnimation anim = Fld_TouchSeal_Anim;

    anim.posX      = arg1->unk8;
    anim.packIndex = Fld_TouchSeal_Seals[arg1->unk4].unk0;
    anim.dataType  = arg1->unk0;
    anim.posY      = arg1->unkA;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TouchSeal_Init(TaskPool* pool, Task* task, void* args) {
    Ov030TouchSealConfig* arg2   = args;
    Ov030TouchSealSprite* sprite = task->data;

    sprite->unk40 = arg2->unk4;
    Fld_TouchSeal_Load(&sprite->sprite, arg2);
    return 1;
}

static s32 Fld_TouchSeal_Update(TaskPool* pool, Task* task, void* args) {
    Sprite_Update(task->data);
    return 1;
}

static s32 Fld_TouchSeal_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_TouchSeal_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_TouchSeal_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TouchSeal_Init,
        .update     = Fld_TouchSeal_Update,
        .render     = Fld_TouchSeal_Render,
        .cleanup    = Fld_TouchSeal_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_TouchSeal_CreateTask(TaskPool* pool, s32 arg1, s32 arg2, s16 arg3, s16 arg4) {
    Ov030TouchSealConfig params;

    params.unk0 = arg1;
    params.unk4 = arg2;
    params.unk8 = arg3;
    params.unkA = arg4;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TouchSeal, NULL, 0, NULL, &params);
}

void Fld_TouchSeal_SetAnimation(Task* task, s16 arg1) {
    Ov030TouchSealSprite* sprite = task->data;

    Sprite_ChangeAnimation(&sprite->sprite, sprite->sprite.animData, arg1, sprite->sprite.cellTable);
    Sprite_Update(&sprite->sprite);
}

s32 Fld_TouchSeal_IsTouched(Task* task) {
    Ov030TouchSealSprite* sprite = task->data;
    Ov030TouchSealEntry*  entry  = &Fld_TouchSeal_Seals[sprite->unk40];
    s16                   x;
    s16                   y;
    TouchCoord            coord;

    x = sprite->sprite.posX;
    y = sprite->sprite.posY;
    TouchInput_GetCoord(&coord);
    if (x - entry->unk4 < coord.x && coord.x < x + entry->unk4) {
        if (y - entry->unk2 < coord.y && coord.y < y + entry->unk2) {
            return 1;
        }
    }
    return 0;
}
