#include "Field/Tasks/Fld_MapJumpName.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/FieldTypes.h"
#include "Field/Tasks/Task_Mess.h"
#include "Save.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"
#include <nitro/types.h>

typedef struct {
    /* 0x00 */ FieldPos* origin;
    /* 0x04 */ FieldPos  pos;
    /* 0x0C */ s32       dataType;
    /* 0x10 */ s32       code; // message id of the destination name
    /* 0x14 */ s32       animIndex;
    /* 0x18 */ s32       useBig;
} Fld_MapJumpName_Args; // Size: 0x1C

typedef struct {
    /* 0x00 */ Fld_MapJumpName_Args args;
    /* 0x1C */ s32                  x; // fx32 map position of the label
    /* 0x20 */ s32                  y;
    /* 0x24 */ Sprite               sprite;
    /* 0x64 */ s32                  childTaskId;
    /* 0x68 */ s32                  unk_68;
} Fld_MapJumpName; // Size: 0x6C

typedef struct {
    Sprite*  sprite;
    SysCode* text;
    u8*      displayed;
    u8       unk_0C;
    s16      unk_0E;
    u32      unk_10;
    s32      unk_14;
    u32      unk_18;
    u32      unk_1C;
    s32      unk_20;
} Fld_MapJumpName_MessageArgs;

s32                     Fld_MapJumpName_Init(TaskPool*, Task*, void*);
s32                     Fld_MapJumpName_Render(TaskPool*, Task*, void*);
static s32              Fld_MapJumpName_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* Fld_MapJumpName_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static char                  Fld_Mess_GetStrCat_SeqName[] = "Fld_Mess_GetStrCat";
static const TaskHandle      Tsk_Fld_MapJumpName  = {"Tsk_Fld_MapJumpName", Fld_MapJumpName_RunTask, sizeof(Fld_MapJumpName)};
static const SpriteAnimation Fld_MapJumpName_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x0,
    .posY              = 0x0,
    .frameInfoCallback = Fld_MapJumpName_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 3,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 5,
};

s32 Fld_Mess_StartText(TaskPool* pool, Sprite* sprite, SysCode* text, u8* displayed, s32 arg4) {
    Fld_MapJumpName_MessageArgs args;

    Sprite_Update(sprite);
    *displayed     = 0;
    args.sprite    = sprite;
    args.text      = text;
    args.displayed = displayed;
    args.unk_0C    = 0;
    args.unk_0E    = 0;
    args.unk_14    = arg4;
    args.unk_20    = 1;
    return EasyTask_CreateTask(pool, &Tsk_Task_Mess, NULL, 0, NULL, &args);
}

SysCode* Fld_Mess_GetStrCat(SysFont* font, u16 msgIndex1, u16 msgIndex2) {
    SysCode* msg1      = SysFont_GetMsgBuf(font, msgIndex1);
    SysCode* msg2      = SysFont_GetMsgBuf(font, msgIndex2);
    s32      length    = SysFont_GetStrBufLen(font, (u16*)msg1) + SysFont_GetStrBufLen(font, (u16*)msg2) + 2;
    u16      codeCount = length;
    SysCode* result    = Mem_AllocHeapTail(&gDebugHeap, codeCount << 1);

    Mem_SetSequence(&gDebugHeap, result, Fld_Mess_GetStrCat_SeqName);
    SysFont_Format(result, msg1, msg2);
    Mem_Free(&gDebugHeap, msg2);
    Mem_Free(&gDebugHeap, msg1);
    return result;
}

void Fld_Mess_CenterText(SysFont* font, SysCode* text, s32 align, Sprite* sprite) {
    s32 minX;
    s32 minY;
    s32 width;
    s32 height;

    SysFont_GetCellBoundsMin(sprite, &minX, &minY);
    width  = SysFont_MeasureWidth(font, text);
    height = SysFont_MeasureHeight(font, text);
    SysFont_SetHAlign(font, align, width);
    SysFont_SetPos(font, (u16)(-width / 2 - minX), (u16)(-height / 2 - minY));
}

static SpriteFrameInfo* Fld_MapJumpName_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static s32 Fld_MapJumpName_GetOffsetX(s32 mapId) {
    if (mapId == 0x2d40 && gSaveData.fieldArea == 3) {
        return -15;
    }
    return 0;
}

void Fld_MapJumpName_Load(Sprite* sprite, Fld_MapJumpName* data) {
    SpriteAnimation anim = Fld_MapJumpName_Anim;
    s32             dx;

    anim.dataType = (u16)data->args.dataType;
    dx            = data->x - data->args.origin->x;
    anim.posX     = Fld_MapJumpName_GetOffsetX(data->args.code) + ((dx * 0x10) >> 0x10);
    anim.posY     = (data->y - data->args.origin->y) >> 12;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_MapJumpName_Update(TaskPool* pool, Task* task, void* args) {
    return TRUE;
}

static s32 Fld_MapJumpName_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_MapJumpName* data = task->data;

    Sprite_Destroy(&data->sprite);
    if (EasyTask_ValidateTaskId(pool, &data->childTaskId) != FALSE) {
        EasyTask_DeleteTask(pool, data->childTaskId);
    }
    return TRUE;
}

static s32 Fld_MapJumpName_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_MapJumpName_Init,
        .update     = Fld_MapJumpName_Update,
        .render     = Fld_MapJumpName_Render,
        .cleanup    = Fld_MapJumpName_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_MapJumpName_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, s32 code, s32 animIndex,
                               s32 useBig) {
    Fld_MapJumpName_Args args;

    args.origin    = origin;
    args.pos       = pos;
    args.dataType  = dataType;
    args.code      = code;
    args.animIndex = animIndex;
    args.useBig    = useBig;
    return EasyTask_CreateTask(pool, &Tsk_Fld_MapJumpName, NULL, 0, NULL, &args);
}
