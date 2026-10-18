#include "Field/Tasks/Fld_NpcRG.h"
#include "Engine/EasyTask.h"
#include "Field/Scene/FieldPaths.h"
#include "SpriteMgr.h"

// The task data is a Sprite extended with a sentinel.
typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    npc;
} NpcRGSprite;

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ s32 npc; // NpcRGEntry index, -1 = none
} NpcRGArgs;

// Animation parameter records.
typedef struct {
    /* 0x00 */ s16 charIndex;
    /* 0x02 */ s16 animDataIndex;
    /* 0x04 */ s16 cellIndex;
    /* 0x06 */ s16 paletteIndex;
    /* 0x08 */ s16 animIndex;
    /* 0x0A */ s16 packIndex;
} NpcRGEntry; // Size: 0xC

static SpriteFrameInfo* Fld_NpcRG_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_NpcRG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_NpcRG = {"Tsk_Fld_NpcRG", Fld_NpcRG_RunTask, sizeof(NpcRGSprite)};

static const SpriteAnimation Fld_NpcRG_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0000,
    .posX              = 128,
    .posY              = 78,
    .frameInfoCallback = Fld_NpcRG_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_NpcCeparateRG,
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

static const NpcRGEntry Fld_NpcRG_Npcs[207] = {
    {4, 6, 5, 1, 1,  1},
    {4, 6, 5, 2, 1,  1},
    {4, 6, 5, 3, 1,  1},
    {4, 6, 5, 1, 2,  1},
    {4, 6, 5, 2, 2,  1},
    {4, 6, 5, 3, 2,  1},
    {4, 6, 5, 1, 1,  2},
    {4, 6, 5, 2, 1,  2},
    {4, 6, 5, 3, 1,  2},
    {4, 6, 5, 1, 2,  2},
    {4, 6, 5, 2, 2,  2},
    {4, 6, 5, 3, 2,  2},
    {4, 6, 5, 1, 1,  3},
    {4, 6, 5, 2, 1,  3},
    {4, 6, 5, 3, 1,  3},
    {4, 6, 5, 1, 2,  3},
    {4, 6, 5, 2, 2,  3},
    {4, 6, 5, 3, 2,  3},
    {4, 6, 5, 1, 1,  4},
    {4, 6, 5, 2, 1,  4},
    {4, 6, 5, 3, 1,  4},
    {4, 6, 5, 1, 2,  4},
    {4, 6, 5, 2, 2,  4},
    {4, 6, 5, 3, 2,  4},
    {4, 6, 5, 1, 1,  5},
    {4, 6, 5, 2, 1,  5},
    {4, 6, 5, 3, 1,  5},
    {4, 6, 5, 1, 2,  5},
    {4, 6, 5, 2, 2,  5},
    {4, 6, 5, 3, 2,  5},
    {4, 6, 5, 1, 1,  6},
    {4, 6, 5, 2, 1,  6},
    {4, 6, 5, 3, 1,  6},
    {4, 6, 5, 1, 2,  6},
    {4, 6, 5, 2, 2,  6},
    {4, 6, 5, 3, 2,  6},
    {4, 6, 5, 1, 1,  7},
    {4, 6, 5, 2, 1,  7},
    {4, 6, 5, 3, 1,  7},
    {4, 6, 5, 1, 2,  7},
    {4, 6, 5, 2, 2,  7},
    {4, 6, 5, 3, 2,  7},
    {4, 6, 5, 1, 1,  8},
    {4, 6, 5, 2, 1,  8},
    {4, 6, 5, 3, 1,  8},
    {4, 6, 5, 1, 2,  8},
    {4, 6, 5, 2, 2,  8},
    {4, 6, 5, 3, 2,  8},
    {4, 6, 5, 1, 1,  9},
    {4, 6, 5, 2, 1,  9},
    {4, 6, 5, 3, 1,  9},
    {4, 6, 5, 1, 2,  9},
    {4, 6, 5, 2, 2,  9},
    {4, 6, 5, 3, 2,  9},
    {4, 6, 5, 1, 1, 10},
    {4, 6, 5, 2, 1, 10},
    {4, 6, 5, 3, 1, 10},
    {4, 6, 5, 1, 2, 10},
    {4, 6, 5, 2, 2, 10},
    {4, 6, 5, 3, 2, 10},
    {4, 6, 5, 1, 1, 11},
    {4, 6, 5, 2, 1, 11},
    {4, 6, 5, 3, 1, 11},
    {4, 6, 5, 1, 2, 11},
    {4, 6, 5, 2, 2, 11},
    {4, 6, 5, 3, 2, 11},
    {4, 6, 5, 1, 1, 12},
    {4, 6, 5, 2, 1, 12},
    {4, 6, 5, 3, 1, 12},
    {4, 6, 5, 1, 2, 12},
    {4, 6, 5, 2, 2, 12},
    {4, 6, 5, 3, 2, 12},
    {4, 6, 5, 1, 1, 13},
    {4, 6, 5, 2, 1, 13},
    {4, 6, 5, 3, 1, 13},
    {4, 6, 5, 1, 2, 13},
    {4, 6, 5, 2, 2, 13},
    {4, 6, 5, 3, 2, 13},
    {4, 6, 5, 1, 1, 14},
    {4, 6, 5, 2, 1, 14},
    {4, 6, 5, 3, 1, 14},
    {4, 6, 5, 1, 2, 14},
    {4, 6, 5, 2, 2, 14},
    {4, 6, 5, 3, 2, 14},
    {4, 6, 5, 1, 1, 15},
    {4, 6, 5, 2, 1, 15},
    {4, 6, 5, 3, 1, 15},
    {4, 6, 5, 1, 2, 15},
    {4, 6, 5, 2, 2, 15},
    {4, 6, 5, 3, 2, 15},
    {4, 6, 5, 1, 1, 16},
    {4, 6, 5, 2, 1, 16},
    {4, 6, 5, 3, 1, 16},
    {4, 6, 5, 1, 2, 16},
    {4, 6, 5, 2, 2, 16},
    {4, 6, 5, 3, 2, 16},
    {4, 6, 5, 1, 1, 17},
    {4, 6, 5, 2, 1, 17},
    {4, 6, 5, 3, 1, 17},
    {4, 6, 5, 1, 2, 17},
    {4, 6, 5, 2, 2, 17},
    {4, 6, 5, 3, 2, 17},
    {4, 6, 5, 1, 1, 18},
    {4, 6, 5, 2, 1, 18},
    {4, 6, 5, 3, 1, 18},
    {4, 6, 5, 1, 2, 18},
    {4, 6, 5, 2, 2, 18},
    {4, 6, 5, 3, 2, 18},
    {4, 6, 5, 1, 1, 19},
    {4, 6, 5, 2, 1, 19},
    {4, 6, 5, 3, 1, 19},
    {4, 6, 5, 1, 2, 19},
    {4, 6, 5, 2, 2, 19},
    {4, 6, 5, 3, 2, 19},
    {4, 6, 5, 1, 1, 20},
    {4, 6, 5, 2, 1, 20},
    {4, 6, 5, 3, 1, 20},
    {4, 6, 5, 1, 2, 20},
    {4, 6, 5, 2, 2, 20},
    {4, 6, 5, 3, 2, 20},
    {4, 6, 5, 1, 1, 21},
    {4, 6, 5, 2, 1, 21},
    {4, 6, 5, 3, 1, 21},
    {4, 6, 5, 1, 2, 21},
    {4, 6, 5, 2, 2, 21},
    {4, 6, 5, 3, 2, 21},
    {4, 6, 5, 1, 1, 22},
    {4, 6, 5, 2, 1, 22},
    {4, 6, 5, 3, 1, 22},
    {4, 6, 5, 1, 2, 22},
    {4, 6, 5, 2, 2, 22},
    {4, 6, 5, 3, 2, 22},
    {4, 6, 5, 1, 1, 23},
    {4, 6, 5, 2, 1, 23},
    {4, 6, 5, 3, 1, 23},
    {4, 6, 5, 1, 2, 23},
    {4, 6, 5, 2, 2, 23},
    {4, 6, 5, 3, 2, 23},
    {4, 6, 5, 1, 1, 24},
    {4, 6, 5, 2, 1, 24},
    {4, 6, 5, 3, 1, 24},
    {4, 6, 5, 1, 2, 24},
    {4, 6, 5, 2, 2, 24},
    {4, 6, 5, 3, 2, 24},
    {4, 6, 5, 1, 1, 25},
    {4, 6, 5, 2, 1, 25},
    {4, 6, 5, 3, 1, 25},
    {4, 6, 5, 1, 2, 25},
    {4, 6, 5, 2, 2, 25},
    {4, 6, 5, 3, 2, 25},
    {4, 6, 5, 1, 1, 26},
    {4, 6, 5, 2, 1, 26},
    {4, 6, 5, 3, 1, 26},
    {4, 6, 5, 1, 2, 26},
    {4, 6, 5, 2, 2, 26},
    {4, 6, 5, 3, 2, 26},
    {4, 6, 5, 1, 1, 27},
    {4, 6, 5, 2, 1, 27},
    {4, 6, 5, 3, 1, 27},
    {4, 6, 5, 1, 2, 27},
    {4, 6, 5, 2, 2, 27},
    {4, 6, 5, 3, 2, 27},
    {5, 7, 6, 1, 1, 28},
    {5, 7, 6, 1, 1, 29},
    {5, 7, 6, 1, 1, 30},
    {5, 7, 6, 1, 1, 31},
    {5, 7, 6, 1, 1, 32},
    {5, 7, 6, 1, 1, 33},
    {5, 7, 6, 1, 1, 34},
    {5, 7, 6, 1, 1, 35},
    {5, 7, 6, 1, 1, 36},
    {5, 7, 6, 1, 1, 37},
    {5, 7, 6, 1, 1, 38},
    {5, 7, 6, 1, 1, 39},
    {5, 7, 6, 1, 1, 40},
    {5, 7, 6, 1, 1, 41},
    {5, 7, 6, 1, 1, 42},
    {5, 7, 6, 1, 1, 43},
    {5, 7, 6, 1, 1, 44},
    {5, 7, 6, 1, 1, 45},
    {5, 7, 6, 1, 1, 46},
    {5, 7, 6, 1, 1, 47},
    {5, 7, 6, 1, 1, 48},
    {5, 7, 6, 1, 1, 49},
    {5, 7, 6, 1, 1, 50},
    {5, 7, 6, 1, 1, 51},
    {5, 7, 6, 1, 1, 52},
    {5, 7, 6, 1, 1, 53},
    {5, 7, 6, 1, 1, 54},
    {5, 7, 6, 1, 1, 55},
    {5, 7, 6, 1, 1, 56},
    {5, 7, 6, 1, 1, 57},
    {5, 7, 6, 1, 1, 58},
    {5, 7, 6, 1, 1, 59},
    {5, 7, 6, 1, 1, 60},
    {5, 7, 6, 1, 1, 61},
    {5, 7, 6, 1, 1, 62},
    {5, 7, 6, 1, 1, 63},
    {5, 7, 6, 1, 1, 64},
    {5, 7, 6, 1, 1, 65},
    {5, 7, 6, 1, 1, 66},
    {5, 7, 6, 1, 1, 67},
    {5, 7, 6, 1, 1, 68},
    {5, 7, 6, 1, 1, 69},
    {5, 7, 6, 1, 1, 70},
    {5, 7, 6, 1, 1, 72},
    {5, 7, 6, 1, 1, 73},
};

static SpriteFrameInfo* Fld_NpcRG_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

static void Fld_NpcRG_Load(Sprite* sprite, NpcRGArgs* args) {
    SpriteAnimation anim = Fld_NpcRG_Anim;
    NpcRGEntry*     entry;
    s32             idx;

    anim.dataType = args->dataType;
    idx           = args->npc;
    if (idx < 0 || (u32)idx >= 0xCFU) {
        idx = 0;
    }
    entry          = &Fld_NpcRG_Npcs[idx];
    anim.unk_1C    = entry->charIndex;
    anim.unk_20    = entry->paletteIndex;
    anim.unk_26    = entry->cellIndex;
    anim.unk_28    = entry->animDataIndex;
    anim.animIndex = entry->animIndex;
    anim.packIndex = entry->packIndex;
    _Sprite_Load(sprite, &anim);
    Sprite_Update(sprite);
}

static s32 Fld_NpcRG_Init(TaskPool* pool, Task* task, void* args) {
    NpcRGArgs*   params = args;
    NpcRGSprite* sprite = task->data;

    sprite->npc = params->npc;
    if (sprite->npc != -1) {
        Fld_NpcRG_Load(&sprite->sprite, params);
    }
    return 1;
}

static s32 Fld_NpcRG_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_NpcRG_Render(TaskPool* pool, Task* task, void* args) {
    NpcRGSprite* sprite = task->data;

    if (sprite->npc != -1) {
        Sprite_RenderFrame(&sprite->sprite);
    }
    return 1;
}

static s32 Fld_NpcRG_Destroy(TaskPool* pool, Task* task, void* args) {
    NpcRGSprite* sprite = task->data;

    if (sprite->npc != -1) {
        Sprite_Release(&sprite->sprite);
    }
    return 1;
}

static s32 Fld_NpcRG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_NpcRG_Init,
        .update     = Fld_NpcRG_Update,
        .render     = Fld_NpcRG_Render,
        .cleanup    = Fld_NpcRG_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_NpcRG_CreateTask(TaskPool* pool, s32 dataType, s32 npc) {
    NpcRGArgs params;

    params.dataType = dataType;
    params.npc      = npc;
    return EasyTask_CreateTask(pool, &Tsk_Fld_NpcRG, NULL, 0, NULL, &params);
}
