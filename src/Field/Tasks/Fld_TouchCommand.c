#include "Field/FieldRegion.h"

#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/Tasks/Fld_MapName.h"
#include "Field/Tasks/Fld_TouchCommand.h"
#include "SpriteMgr.h"

extern void func_02026590(s32* value, s32 target, u16 frames);

typedef struct {
    /* 0x00 */ s32 dataType;
    /* 0x04 */ u16 labelMsg;
    /* 0x06 */ u16 pad_06;
    /* 0x08 */ s32 onMainEngine;
} FldTouchCommandArgs;

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ u16    labelMsg;
    /* 0x42 */ u16    nextLabelMsg;
    /* 0x44 */ s32    slideX;
    /* 0x48 */ s32    slideTargetX;
    /* 0x4C */ u16    slideTimer;
    /* 0x4E */ u16    pad_4E;
    /* 0x50 */ s32    onMainEngine;
    /* 0x54 */ s32    dataType;
    /* 0x58 */ s32    labelTaskId;
} FldTouchCommand;

static SpriteFrameInfo* Fld_TouchCommand_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_TouchCommand_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TouchCommand = {"Tsk_Fld_TouchCommand", Fld_TouchCommand_RunTask, sizeof(FldTouchCommand)};

static const SpriteAnimation Fld_TouchCommand_Anim = {
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
    .frameInfoCallback = Fld_TouchCommand_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldTouch,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

static SpriteFrameInfo* Fld_TouchCommand_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_TouchCommand_Load(Sprite* sprite, FldTouchCommandArgs* arg1) {
    SpriteAnimation anim = Fld_TouchCommand_Anim;
    u16             dataType;

    dataType       = (u16)arg1->dataType;
    anim.dataType  = dataType;
    anim.animIndex = 1;
    if (arg1->onMainEngine == 0) {
        anim.bits_0_1 = 1;
    }
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_TouchCommand_Init(TaskPool* pool, Task* task, void* args) {
    FldTouchCommandArgs* arg2   = args;
    FldTouchCommand*     sprite = task->data;

    Fld_TouchCommand_Load(&sprite->sprite, arg2);
    sprite->labelMsg     = arg2->labelMsg;
    sprite->nextLabelMsg = arg2->labelMsg;
    sprite->slideTargetX = 0;
    sprite->slideX       = 0;
    sprite->slideTimer   = 0;
    sprite->dataType     = arg2->dataType;
    sprite->onMainEngine = arg2->onMainEngine;
    if (sprite->onMainEngine != 0) {
        sprite->labelTaskId = Fld_MapName_CreateTaskOnMain(pool, sprite->dataType, sprite->labelMsg);
    } else {
        sprite->labelTaskId = Fld_MapName_CreateTask(pool, sprite->dataType, sprite->labelMsg);
    }
    Fld_MapName_SetPosition(EasyTask_GetTaskById(pool, sprite->labelTaskId), (s16)((sprite->slideX >> 0xC) + 0x10), 8);
    return 1;
}

static s32 Fld_TouchCommand_Update(TaskPool* pool, Task* task, void* args) {
    FldTouchCommand* sprite = task->data;

    if (sprite->slideTimer != 0) {
        func_02026590(&sprite->slideX, sprite->slideTargetX, sprite->slideTimer);
        sprite->slideTimer = sprite->slideTimer - 1;
        if (sprite->slideTimer == 0 && sprite->slideTargetX != 0) {
            EasyTask_DeleteTask(pool, sprite->labelTaskId);
            sprite->labelMsg = sprite->nextLabelMsg;
            if (sprite->onMainEngine != 0) {
                sprite->labelTaskId = Fld_MapName_CreateTaskOnMain(pool, sprite->dataType, sprite->labelMsg);
            } else {
                sprite->labelTaskId = Fld_MapName_CreateTask(pool, sprite->dataType, sprite->labelMsg);
            }
            sprite->slideTargetX = 0;
            sprite->slideTimer   = 0x14;
        }
        sprite->sprite.posX = (s16)(sprite->slideX >> 0xC);
        Fld_MapName_SetPosition(EasyTask_GetTaskById(pool, sprite->labelTaskId), (s16)((sprite->slideX >> 0xC) + 0x10), 8);
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

static s32 Fld_TouchCommand_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_TouchCommand_Destroy(TaskPool* pool, Task* task, void* args) {
    FldTouchCommand* sprite = task->data;

    Sprite_Release(&sprite->sprite);
    EasyTask_DeleteTask(pool, sprite->labelTaskId);
    return 1;
}

static s32 Fld_TouchCommand_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TouchCommand_Init,
        .update     = Fld_TouchCommand_Update,
        .render     = Fld_TouchCommand_Render,
        .cleanup    = Fld_TouchCommand_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

static u16 Fld_TouchCommand_GetLabelMsg(s32 arg0) {
    if (arg0 == 1) {
        return 0x5DD2;
    }
    if (arg0 == 4) {
        return 0x5DD5;
    }
    if (arg0 == 3) {
        return 0x5DD4;
    }
    return 0x5DD3;
}

s32 Fld_TouchCommand_CreateTask(TaskPool* pool, s32 arg1, s32 arg2) {
    FldTouchCommandArgs params;

    params.dataType     = arg1;
    params.onMainEngine = 1;
    params.labelMsg     = Fld_TouchCommand_GetLabelMsg(arg2);
    return EasyTask_CreateTask(pool, &Tsk_Fld_TouchCommand, NULL, 0, NULL, &params);
}

s32 Fld_TouchCommand_CreateTaskWithMsg(TaskPool* pool, s32 arg1, u16 arg2, s32 arg3) {
    FldTouchCommandArgs params;

    params.dataType     = arg1;
    params.labelMsg     = arg2;
    params.onMainEngine = arg3;
    return EasyTask_CreateTask(pool, &Tsk_Fld_TouchCommand, NULL, 0, NULL, &params);
}

void Fld_TouchCommand_SetCommand(Task* task, s32 arg1) {
    FldTouchCommand* sprite = task->data;
    u16              val;

    val = Fld_TouchCommand_GetLabelMsg(arg1);
    if (sprite->nextLabelMsg == val) {
        return;
    }
    sprite->nextLabelMsg = val;
    if (sprite->slideTargetX != 0) {
        return;
    }
    sprite->slideTargetX = -0x50000;
    sprite->slideTimer   = 0x14;
}
