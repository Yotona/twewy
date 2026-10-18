#include "Field/Tasks/Fld_Brush.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Sqrt.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Event/EventFlags.h"
#include "Field/Tasks/Fld_BrushHati.h"
#include "Field/Tasks/Fld_TouchCommand.h"
#include "Field/Tasks/Fld_TouchCursor.h"
#include "Field/Tasks/Fld_TouchSeal.h"
#include "Field/Tasks/Fld_TouchShine.h"
#include "SndMgr.h"
#include <nitro/types.h>

struct BrushTouchTarget {
    s16 x;
    s16 y;
    s32 radius;
};

typedef struct {
    /* 0x0 */ s32 owner;
} Fld_Brush_Args;

typedef struct {
    /* 0x00 */ s32 owner;
    /* 0x04 */ s16 frame;
    /* 0x06 */ s16 selected;
    /* 0x08 */ s32 taskIds[19];
    /* 0x54 */ s16 activeBrush;
    /* 0x56 */ s16 unk_56;
    /* 0x58 */ s32 closing;
} Fld_Brush; // Size: 0x5C

static const struct BrushTouchTarget Fld_Brush_Targets[14] = {
    {101,  35, 7},
    {119,  32, 7},
    {107,  56, 7},
    {127,  50, 7},
    {132,  70, 9},
    {108,  81, 9},
    {138,  89, 7},
    {102, 106, 7},
    {128, 106, 7},
    {147, 105, 7},
    { 95, 134, 7},
    {125, 132, 9},
    {150, 126, 9},
    {167, 138, 7},
};
extern s32 func_0202599c(s32 flagIndex);
static s32 Fld_Brush_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Brush = {"Tsk_Fld_Brush", Fld_Brush_RunTask, sizeof(Fld_Brush)};

// Nonmatching: the target also copies the touch coordinates to a second stack struct
static s32 Fld_Brush_GetTouchedBrush(void) {
    TouchCoord coord;
    s32        x;
    s32        y;
    s16        index;
    TouchCoord pos;

    TouchInput_GetCoord(&coord);
    pos = coord;
    x   = coord.x;
    y   = coord.y;
    for (index = 0; index < 14u; index++) {
        s32 dy              = Fld_Brush_Targets[index].y - y;
        s32 distanceSquared = dy * dy;
        s32 dx              = Fld_Brush_Targets[index].x - x;
        distanceSquared += dx * dx;
        if (Sqrti(distanceSquared) < Fld_Brush_Targets[index].radius) {
            return index;
        }
    }
    return 14;
}

static s32 Fld_Brush_FindTouchedSeal(TaskPool* pool, Fld_Brush* data) {
    s16 index;

    for (index = 0; index < 3; index++) {
        if (data->taskIds[index] != -1 && Fld_TouchSeal_IsTouched(EasyTask_GetTaskById(pool, data->taskIds[index]))) {
            return index;
        }
    }
    return -1;
}

static void Fld_Brush_SelectSeal(TaskPool* pool, Fld_Brush* data, s32 selected) {
    s32 index;

    for (index = 0; index < 3; index++) {
        if (data->taskIds[index] != -1) {
            Task* task = EasyTask_GetTaskById(pool, data->taskIds[index]);
            s32   mode = index == selected ? 1 : 2;
            ((void (*)())Fld_TouchSeal_SetAnimation)(task, mode);
        }
    }
}

static s32 Fld_Brush_Init(TaskPool* pool, Task* task, void* args) {
    Fld_Brush* data     = task->data;
    s32*       ownerArg = args;
    s32        index;
    u32        brushIndex;

    for (index = 0; index < 19; index++) {
        data->taskIds[index] = -1;
    }
    for (brushIndex = 0; brushIndex < 14; brushIndex++) {
        data->taskIds[brushIndex + 5] = Fld_BrushHati_CreateTask(pool, *ownerArg, (s16)brushIndex);
    }

    data->taskIds[0] = Fld_TouchSeal_CreateTask(pool, *ownerArg, 0, 0xE2, 0xA7);
    if (func_0202599c(2)) {
        data->taskIds[1] = Fld_TouchSeal_CreateTask(pool, *ownerArg, 2, 0x23, 0xA0);
    }
    if (func_0202599c(3)) {
        data->taskIds[2] = Fld_TouchSeal_CreateTask(pool, *ownerArg, 3, 0xE2, 0x21);
        Fld_TouchSeal_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[2]), 1);
        data->taskIds[3]  = Fld_TouchCommand_CreateTask(pool, *ownerArg, 4);
        data->activeBrush = 2;
        data->taskIds[4]  = Fld_TouchCursor_CreateTask(pool, *ownerArg, 2);
    } else if (func_0202599c(2)) {
        Fld_TouchSeal_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[1]), 1);
        data->taskIds[3]  = Fld_TouchCommand_CreateTask(pool, *ownerArg, 2);
        data->activeBrush = 1;
        data->taskIds[4]  = Fld_TouchCursor_CreateTask(pool, *ownerArg, 1);
    } else {
        data->taskIds[3]  = -1;
        data->activeBrush = -1;
    }
    data->owner   = (void*)*ownerArg;
    data->frame   = 0;
    data->closing = 0;
    return TRUE;
}

static s32 Fld_Brush_AreAllBrushesErased(Fld_Brush* data) {
    u32 index;

    for (index = 0; index < 14; index++) {
        if (data->taskIds[index + 5] != -1) {
            return FALSE;
        }
    }
    return TRUE;
}

// Nonmatching: the target does not CSE selected + 5 across the touch checks
static s32 Fld_Brush_Update(TaskPool* pool, Task* task, void* args) {
    Fld_Brush* data = task->data;
    s32        selected;

    if (data->closing != 0) {
        return TRUE;
    }
    EasyFade_FadeBothDisplays(0, 0, 0x1000);
    if (TouchInput_WasTouchPressed()) {
        selected = Fld_Brush_FindTouchedSeal(pool, data);
        if (selected != -1) {
            Fld_Brush_SelectSeal(pool, data, selected);
            if (selected == 0) {
                Field_SetTouchEventResult(3);
                SndMgr_PlaySEWithPan(0xC4, 0xE2);
                data->closing = 1;
            } else if (selected == 1) {
                Fld_TouchCommand_SetCommand(EasyTask_GetTaskById(pool, data->taskIds[3]), 2);
                Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[4]), 1);
                if (data->activeBrush != 1) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget(0x19);
                    SndMgr_PlaySEWithPan(0xC4, 0x23);
                }
            } else if (selected == 2) {
                Fld_TouchCommand_SetCommand(EasyTask_GetTaskById(pool, data->taskIds[3]), 4);
                Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[4]), 2);
                if (data->activeBrush != 2) {
                    Field_SetTouchEventResult(4);
                    SndMgr_PlaySEWithPan(0xC4, 0xE2);
                }
            }
            data->activeBrush = selected;
        }
    } else if (TouchInput_IsTouchActive() && data->activeBrush == 2) {
        selected = Fld_Brush_GetTouchedBrush();
        if (selected != 14 && data->frame == 0 && data->taskIds[selected + 5] != -1) {
            EasyTask_DeleteTask(pool, data->taskIds[selected + 5]);
            data->taskIds[selected + 5] = -1;
            Fld_TouchShine_CreateTask(pool, data->owner);
            data->frame = 5;
        }
        if (Fld_Brush_AreAllBrushesErased(data)) {
            Field_SetTouchEventResult(1);
            data->closing = 1;
            func_ov030_020aec1c(8);
        }
    }
    if (data->frame != 0) {
        data->frame--;
    }
    return TRUE;
}

static s32 Fld_Brush_Render(TaskPool* pool, Task* task, void* args) {
    return TRUE;
}

static s32 Fld_Brush_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_Brush* data = task->data;
    s32        index;

    for (index = 0; index < 19; index++) {
        if (data->taskIds[index] != -1) {
            EasyTask_DeleteTask(pool, data->taskIds[index]);
        }
    }
    return TRUE;
}

static s32 Fld_Brush_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Brush_Init,
        .update     = Fld_Brush_Update,
        .render     = Fld_Brush_Render,
        .cleanup    = Fld_Brush_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_Brush_CreateTask(TaskPool* pool, s32 owner) {
    Fld_Brush_Args args;

    args.owner = owner;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Brush, NULL, 0, NULL, &args);
}
