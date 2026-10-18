#include "Field/Tasks/Fld_OnePicturCell.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

// The task data is a Sprite extended with a selector.
typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk40;
} OnePicturSprite;

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ s32 picture; // OnePicturEntry index
} OnePicturArgs;

// Animation parameter records.
typedef struct {
    /* 0x00 */ s16 binIndex;
    /* 0x02 */ s16 packIndex;
    /* 0x04 */ s16 paletteIndex;
    /* 0x06 */ s16 charIndex;
    /* 0x08 */ s16 cellIndex;
    /* 0x0A */ s16 animDataIndex;
} OnePicturEntry; // Size: 0xC

static SpriteFrameInfo* Fld_OnePicturCell_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_OnePicturCell_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_OnePicturCell = {"Tsk_Fld_OnePicturCell", Fld_OnePicturCell_RunTask, sizeof(OnePicturSprite)};

static const OnePicturEntry Fld_OnePicturCell_Pictures[2] = {
    {0, 1, 1, 2, 3, 4},
    {0, 2, 1, 2, 3, 4},
};

static const SpriteAnimation Fld_OnePicturCell_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 1,
    .bits_12_13        = 2,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 128,
    .posY              = 96,
    .frameInfoCallback = Fld_OnePicturCell_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = NULL,
    .unk_18            = 2,
    .packIndex         = 0,
    .unk_1C            = 0,
    .unk_1E            = 0,
    .unk_20            = 0,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 0,
    .unk_28            = 0,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_OnePicturCell_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

// Nonmatching: mwcc rotates the six live register roles (ours: binIden=r5,
// anim bits=r12, packIndex=r4, dataType<<0x1c=r3; retail: r12/r5/r3/r4), so our
// packIndex load and its store get sunk after the dataType bic/orr merge
// instead of sitting between the BinIdentifier address add and that merge
// (USA 87.57%; JP unvalidated).
static void Fld_OnePicturCell_Load(Sprite* sprite, OnePicturArgs* args) {
    SpriteAnimation anim      = Fld_OnePicturCell_Anim;
    s32             idx       = args->picture;
    OnePicturEntry* entry     = &Fld_OnePicturCell_Pictures[idx];
    s16             packIndex = entry->packIndex;
    s16             binIden   = entry->binIndex;
    u16             dataType  = (u16)args->dataType;

    anim.packIndex = packIndex;
    anim.binIden   = &Iden_FldEventPacks[binIden];
    anim.dataType  = dataType;
    anim.unk_1C    = entry->charIndex;
    anim.unk_20    = entry->paletteIndex;
    anim.unk_26    = entry->cellIndex;
    anim.unk_28    = entry->animDataIndex;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_OnePicturCell_Init(TaskPool* pool, Task* task, void* args) {
    OnePicturArgs*   params = args;
    OnePicturSprite* sprite = task->data;

    sprite->unk40 = params->picture;
    Fld_OnePicturCell_Load(&sprite->sprite, params);
    return 1;
}

static s32 Fld_OnePicturCell_Update(TaskPool* pool, Task* task, void* args) {
    Sprite* sprite   = task->data;
    s32     finished = 0;

    if (SpriteMgr_IsFrameFinished(sprite) != 0) {
        finished = 1;
    }
    Sprite_UpdateAndCheck(sprite);
    if (finished != 0 && SpriteMgr_IsAnimationFinished(sprite) == 0) {
        SndMgr_StartPlayingSE((SndMgrSeIdx)0xD3);
    }
    return 1;
}

static s32 Fld_OnePicturCell_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_Render(task->data);
    return 1;
}

static s32 Fld_OnePicturCell_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Destroy(task->data);
    return 1;
}

static s32 Fld_OnePicturCell_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_OnePicturCell_Init,
        .update     = Fld_OnePicturCell_Update,
        .render     = Fld_OnePicturCell_Render,
        .cleanup    = Fld_OnePicturCell_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_OnePicturCell_CreateTask(TaskPool* pool, s32 dataType, s32 picture) {
    OnePicturArgs params;

    params.dataType = dataType;
    params.picture  = picture;
    return EasyTask_CreateTask(pool, &Tsk_Fld_OnePicturCell, NULL, 0, NULL, &params);
}
