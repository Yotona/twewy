#include "Field/Tasks/Fld_MapJumpArrow.h"
#include "Engine/EasyTask.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Field/Scene/FieldArea.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Tasks/Fld_MapJumpName.h"
#include "Save.h"
#include "SpriteMgr.h"

// Map-jump code remap entry.
typedef struct {
    /* 0x00 */ s16 key;
    /* 0x02 */ s16 val;
} FldMapEntry;

// Task args, copied verbatim into the task data.
typedef struct {
    /* 0x00 */ FieldPos*        origin;
    /* 0x04 */ FieldPos         pos;
    /* 0x0C */ s32              dataType;
    /* 0x10 */ FldSpawnTemplate tmpl;
    /* 0x2C */ s32              destArea;
} MapJumpArgs; // Size: 0x30

typedef struct {
    /* 0x00 */ MapJumpArgs hdr;
    /* 0x30 */ FieldPos    pos;
    /* 0x38 */ s32         timer;
    /* 0x3C */ Sprite      sprite;
    /* 0x7C */ s32         unk_7C;
} MapJumpData; // Size: 0x80

static SpriteFrameInfo* Fld_MapJumpArrow_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_MapJumpArrow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_MapJumpArrow = {"Tsk_Fld_MapJumpArrow", Fld_MapJumpArrow_RunTask, sizeof(MapJumpData)};

static const FldMapEntry Fld_MapJumpArrow_MessageRemap[13] = {
    {0x2D42, 0x2D57},
    {0x2D43, 0x2D58},
    {0x2D44, 0x2D59},
    {0x2D45, 0x2D5A},
    {0x2D46, 0x2D5B},
    {0x2D47, 0x2D5C},
    {0x2D48, 0x2D5D},
    {0x2D49, 0x2D5E},
    {0x2D4A, 0x2D5F},
    {0x2D4B, 0x2D60},
    {0x2D4C, 0x2D61},
    {0x2D4D, 0x2D62},
    {0x2D4E, 0x2D63},
};

static const SpriteAnimation Fld_MapJumpArrow_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_MapJumpArrow_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 3,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 6,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_MapJumpArrow_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_MapJumpArrow_Load(Sprite* sprite, MapJumpData* data) {
    SpriteAnimation anim = Fld_MapJumpArrow_Anim;

    anim.dataType = data->hdr.dataType;
    anim.posX     = (data->pos.x - data->hdr.origin->x) >> 12;
    anim.posY     = (data->pos.y - data->hdr.origin->y) >> 12;
    if (data->hdr.tmpl.unk_0A == 2) {
        anim.animIndex = 2;
    } else if (data->hdr.tmpl.unk_0A == 6) {
        anim.animIndex = 3;
    } else if (data->hdr.tmpl.unk_0A == 7) {
        anim.animIndex = 4;
    }
    if (data->hdr.destArea >= 0x46) {
        anim.unk_20 = 5;
    }
    _Sprite_Load(sprite, &anim);
}

// Remaps a map-jump message code through Fld_MapJumpArrow_MessageRemap.
static s16 Fld_MapJumpArrow_RemapMessage(s16 arg0) {
    u32 i;

    if (arg0 == 0x2D42 && gSaveData.fieldArea == 0x13) {
        return 0x2D55;
    }
    for (i = 0; i < 0xD; i++) {
        if (arg0 == Fld_MapJumpArrow_MessageRemap[i].key) {
            return Fld_MapJumpArrow_MessageRemap[i].val;
        }
    }
    return arg0;
}

static s32 Fld_MapJumpArrow_Init(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;
    s32          useBig;
    s16          code;

    data->hdr   = *(MapJumpArgs*)args;
    data->pos.x = data->hdr.pos.x;
    data->pos.y = data->hdr.pos.y;
    Fld_MapJumpArrow_Load(&data->sprite, data);
    useBig      = 0;
    data->timer = 0;
    if (data->hdr.destArea >= 0x46) {
        useBig = 1;
    }
    if (data->hdr.destArea >= 0x46) {
        code = Fld_MapJumpArrow_RemapMessage(func_ov030_020b7c34(data->hdr.destArea));
    } else if (func_ov030_020aeadc((s16)data->hdr.destArea, 1) == 0) {
        code = 0x2D56;
    } else {
        code = Fld_MapJumpArrow_RemapMessage(func_ov030_020b7c34(data->hdr.destArea));
    }
    data->unk_7C = Fld_MapJumpName_CreateTask(pool, data->hdr.origin, data->pos, data->hdr.dataType, (s32)code,
                                              (s32)data->hdr.tmpl.unk_0A, useBig);
    return 1;
}

static s32 Fld_MapJumpArrow_Update(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;

    if ((data->timer & 3) == 0 && gSaveData.unk_2498 == 0) {
        Sprite_Update(&data->sprite);
    }
    data->timer = data->timer + 1;
    return 1;
}

static s32 Fld_MapJumpArrow_Render(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;

    if (FieldScan_IsOpen() != 0 || Field_IsTalkEventActive() != 0) {
        return 1;
    }
    data->sprite.posX = (data->pos.x - data->hdr.origin->x) >> 12;
    data->sprite.posY = (data->pos.y - data->hdr.origin->y) >> 12;
    Sprite_RenderFrame(&data->sprite);
    return 1;
}

static s32 Fld_MapJumpArrow_Destroy(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;

    Sprite_Destroy(&data->sprite);
    EasyTask_DeleteTask(pool, data->unk_7C);
    return 1;
}

static s32 Fld_MapJumpArrow_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_MapJumpArrow_Init,
        .update     = Fld_MapJumpArrow_Update,
        .render     = Fld_MapJumpArrow_Render,
        .cleanup    = Fld_MapJumpArrow_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_MapJumpArrow_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, FldSpawnTemplate tmpl,
                                s32 destArea) {
    MapJumpArgs param;

    param.origin   = origin;
    param.pos.x    = pos.x;
    param.pos.y    = pos.y;
    param.dataType = dataType;
    param.tmpl     = tmpl;
    param.destArea = destArea;
    return EasyTask_CreateTask(pool, &Tsk_Fld_MapJumpArrow, NULL, 0, NULL, &param);
}
