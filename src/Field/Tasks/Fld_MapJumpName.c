#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Save.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"
#include <nitro/types.h>

typedef struct {
    /* 0x00 */ u32    unk_00[9];
    /* 0x24 */ Sprite sprite;
    /* 0x64 */ s32    childTaskId;
    /* 0x68 */ s32    unk_68;
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

extern s32                 func_ov030_020c4d80(TaskPool*, Task*, void*);
extern s32                 func_ov030_020c4ee4(TaskPool*, Task*, void*);
extern const TaskHandle    Tsk_Task_Mess;
extern const BinIdentifier data_ov030_020d9944;

s32              Fld_MapJumpName_Update(TaskPool* pool, Task* task, void* args);
s32              Fld_MapJumpName_Destroy(TaskPool* pool, Task* task, void* args);
s32              Fld_MapJumpName_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo* Fld_MapJumpName_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

char                  data_ov030_02107620[20] = "Fld_Mess_GetStrCat";
const TaskHandle      Tsk_Fld_MapJumpName     = {"Tsk_Fld_MapJumpName", Fld_MapJumpName_RunTask, sizeof(Fld_MapJumpName)};
const SpriteAnimation Fld_MapJumpName_Anim    = {
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
       .binIden           = &data_ov030_020d9944,
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

s32 func_ov030_020c4a78(TaskPool* pool, Sprite* sprite, SysCode* text, u8* displayed, s32 arg4) {
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

SysCode* func_ov030_020c4ae8(SysFont* font, u16 msgIndex1, u16 msgIndex2) {
    SysCode* msg1      = SysFont_GetMsgBuf(font, msgIndex1);
    SysCode* msg2      = SysFont_GetMsgBuf(font, msgIndex2);
    s32      length    = SysFont_GetStrBufLen(font, (u16*)msg1) + SysFont_GetStrBufLen(font, (u16*)msg2) + 2;
    u16      codeCount = length;
    SysCode* result    = Mem_AllocHeapTail(&gDebugHeap, codeCount << 1);

    Mem_SetSequence(&gDebugHeap, result, data_ov030_02107620);
    SysFont_Format(result, msg1, msg2);
    Mem_Free(&gDebugHeap, msg2);
    Mem_Free(&gDebugHeap, msg1);
    return result;
}

void func_ov030_020c4b8c(SysFont* font, SysCode* text, s32 align, Sprite* sprite) {
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

SpriteFrameInfo* Fld_MapJumpName_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

s32 func_ov030_020c4cbc(s32 mapId) {
    if (mapId == 0x2d40 && gSaveData.fieldArea == 3) {
        return -15;
    }
    return 0;
}

s32 Fld_MapJumpName_Update(TaskPool* pool, Task* task, void* args) {
    return TRUE;
}

s32 Fld_MapJumpName_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_MapJumpName* data = task->data;

    Sprite_Destroy(&data->sprite);
    if (EasyTask_ValidateTaskId(pool, &data->childTaskId) != FALSE) {
        EasyTask_DeleteTask(pool, data->childTaskId);
    }
    return TRUE;
}

s32 Fld_MapJumpName_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = func_ov030_020c4d80,
        .update     = Fld_MapJumpName_Update,
        .render     = func_ov030_020c4ee4,
        .cleanup    = Fld_MapJumpName_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
