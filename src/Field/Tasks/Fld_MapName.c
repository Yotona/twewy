#include "Field/Tasks/Fld_MapName.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Tasks/Fld_MapJumpName.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"
#include "Util/SysFontMsgs.h"
#include <nitro/types.h>

typedef struct {
    Sprite   sprite;
    u32      unk_40;
    u8       unk_44;
    u8       unk_45;
    u16      width;
    SysCode* msg;
    SysFont  font;
} Fld_MapName;

typedef struct {
    s32 dataType;
    s32 messageId;
    s32 onMainEngine;
} Fld_MapName_Args;
s32              Fld_MapName_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
SpriteFrameInfo* Fld_MapName_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_Fld_MapName = {"Tsk_Fld_MapName", Fld_MapName_RunTask, sizeof(Fld_MapName)};

static const SpriteAnimation Fld_MapName_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0,
    .posX              = 0x3c,
    .posY              = 0xb,
    .frameInfoCallback = Fld_MapName_GetFrameInfo,
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

SpriteFrameInfo* Fld_MapName_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void Fld_MapName_Load(Sprite* sprite, Fld_MapName_Args* args, s16 width) {
    SpriteAnimation anim = Fld_MapName_Anim;

    anim.dataType = args->dataType;
    anim.posX     = width;
    if (args->onMainEngine != 0) {
        anim.bits_0_1 = 0;
    }
    _Sprite_Load(sprite, &anim);
}

s32 Fld_MapName_Init(TaskPool* pool, Task* task, void* args) {
    Fld_MapName*      mapName     = task->data;
    Fld_MapName_Args* mapNameArgs = args;
    u16               messageId;
    u32               width;

    SysFont_InitWithFont(&mapName->font, 1, TRUE);
    SysFont_SetSpacing(&mapName->font, TRUE, 0);
    messageId = mapNameArgs->messageId;
    if (messageId == SYSMSG_AREA_NAMES_START + 21 && func_ov030_020aeadc(0x17, 1) == 0) {
        messageId = SYSMSG_AREA_NAME_UNKNOWN;
    }
    mapName->msg   = SysFont_GetMsgBuf(&mapName->font, messageId);
    width          = SysFont_MeasureWidth(&mapName->font, mapName->msg);
    mapName->width = width;
    Fld_MapName_Load(&mapName->sprite, mapNameArgs, width / 2 + 4);
    Sprite_Update(&mapName->sprite);
    mapName->unk_44 = 2;
    SysFont_SetSpacing(&mapName->font, TRUE, 0);
    Fld_Mess_CenterText(&mapName->font, mapName->msg, 1, &mapName->sprite);
    return 1;
}

s32 Fld_MapName_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Fld_MapName_Render(TaskPool* pool, Task* task, void* args) {
    Fld_MapName* mapName = task->data;

    Sprite_RenderFrame(&mapName->sprite);
    if (mapName->unk_44 != 0) {
        mapName->unk_44--;
        if (mapName->unk_44 == 0) {
            SysFont_DrawToSprite(&mapName->font, mapName->msg, &mapName->sprite, TRUE);
            Mem_Free(&gDebugHeap, mapName->msg);
            SysFont_Destroy(&mapName->font);
            mapName->msg = NULL;
        }
    }
    return 1;
}

s32 Fld_MapName_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_MapName* mapName = task->data;

    Sprite_Destroy(&mapName->sprite);
    if (mapName->msg != NULL) {
        Mem_Free(&gDebugHeap, mapName->msg);
        SysFont_Destroy(&mapName->font);
    }
    return 1;
}

s32 Fld_MapName_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_MapName_Init,
        .update     = Fld_MapName_Update,
        .render     = Fld_MapName_Render,
        .cleanup    = Fld_MapName_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_MapName_CreateTask(TaskPool* pool, s32 dataType, s32 messageId) {
    Fld_MapName_Args args;
    args.dataType     = dataType;
    args.messageId    = messageId;
    args.onMainEngine = 0;
    return EasyTask_CreateTask(pool, &Tsk_Fld_MapName, NULL, 0, NULL, &args);
}

s32 Fld_MapName_CreateTaskOnMain(TaskPool* pool, s32 dataType, s32 messageId) {
    Fld_MapName_Args args;
    args.dataType     = dataType;
    args.messageId    = messageId;
    args.onMainEngine = 1;
    return EasyTask_CreateTask(pool, &Tsk_Fld_MapName, NULL, 0, NULL, &args);
}

void Fld_MapName_SetPosition(Task* task, s16 x, s16 y) {
    Fld_MapName* mapName = task->data;

    mapName->sprite.posY = y + 4;
    mapName->sprite.posX = x + mapName->width / 2;
}
