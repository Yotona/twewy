#include "Field/FieldRegion.h"

#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Event/EventTables.h"
#include "Field/Tasks/Fld_Chain.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

extern void func_020265d4(s32* value, s32 target, u16 scale);

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

static SpriteFrameInfo* Fld_Chain_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_Chain_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Chain = {"Tsk_Fld_Chain", Fld_Chain_RunTask, sizeof(FldChainData)};

static const SpriteAnimation Fld_Chain_Anim = {
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
    .frameInfoCallback = Fld_Chain_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldScan,
    .unk_18            = 2,
    .packIndex         = 6,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_Chain_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Fills one glyph of the combo counter.
static void Fld_Chain_LoadGlyph(Sprite* sprite, s32 dataType, s16 animIndex, s16 posX, s16 posY) {
    SpriteAnimation anim = Fld_Chain_Anim;

    anim.dataType  = dataType;
    anim.animIndex = animIndex;
    anim.posX      = posX;
    anim.posY      = posY;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_Chain_Init(TaskPool* pool, Task* task, void* args) {
    FldChainArgs* params = args;
    FldChainData* data   = task->data;
    s16           posX;
    s16           posY;
    s32           units;
    s32           tens;

    units = params->value % 10;
    tens  = params->value / 10;
    Field_GetPlayerScreenPos(&posX, &posY);
    posX -= 4;
    Fld_Chain_LoadGlyph(&data->sprites[0], params->dataType, 0xB, posX, posY);
    posX += 0x17;
    data->count = 3;
    if (tens != 0) {
        Fld_Chain_LoadGlyph(&data->sprites[3], params->dataType, (s16)(tens + 1), posX, posY);
        posX += 7;
        data->count = data->count + 1;
    }
    Fld_Chain_LoadGlyph(&data->sprites[1], params->dataType, (s16)(units + 1), posX, posY);
    posX += 8;
    Fld_Chain_LoadGlyph(&data->sprites[2], params->dataType, 0xC, posX, posY);
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

static s32 Fld_Chain_Update(TaskPool* pool, Task* task, void* args) {
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

static s32 Fld_Chain_Render(TaskPool* pool, Task* task, void* args) {
    FldChainData* data = task->data;
    s32           i;

    for (i = 0; i < data->count; i++) {
        Sprite_RenderFrame(&data->sprites[i]);
    }
    return 1;
}

static s32 Fld_Chain_Destroy(TaskPool* pool, Task* task, void* args) {
    FldChainData* data = task->data;
    s32           i;

    for (i = 0; i < data->count; i++) {
        Sprite_Release(&data->sprites[i]);
    }
    return 1;
}

static s32 Fld_Chain_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Chain_Init,
        .update     = Fld_Chain_Update,
        .render     = Fld_Chain_Render,
        .cleanup    = Fld_Chain_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Chain_CreateTask(TaskPool* pool, s32 dataType, s32 value) {
    FldChainArgs params;

    params.dataType = dataType;
    params.value    = value;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Chain, NULL, 0, NULL, &params);
}
