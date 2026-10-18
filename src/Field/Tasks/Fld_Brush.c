#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Sqrt.h"
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

extern s32  func_ov030_020c01ec(Task* task);
extern void func_ov030_020c01c4(Task* task, s32 mode);

const struct BrushTouchTarget data_ov030_020ecb18[14] = {
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

extern s32  func_ov030_020c0184(TaskPool* pool, s32 owner, s32 type, s32 dataType, s32 size);
extern s32  func_ov030_020bfee4(TaskPool* pool, s32 owner, s32 type);
extern s32  func_ov030_020c04d8(TaskPool* pool, s32 owner);
extern void func_ov030_020c050c(Task* task, s32 mode);
extern void func_ov030_020c0730(TaskPool* pool, s32 owner);
extern void func_ov030_020bff68(Task* task, s32 mode);
extern s32  func_0202599c(s32 flagIndex);
extern void Field_SetTouchEventResult(s32 arg);
extern void Field_SetTouchEventTarget(s32 arg);
extern void func_ov030_020aec1c(s32 arg);
extern void SndMgr_PlaySEWithPan(s32 sound, s32 pan);

s32 Fld_Brush_Destroy(TaskPool* pool, Task* task, void* args);
s32 Fld_Brush_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Brush = {"Tsk_Fld_Brush", Fld_Brush_RunTask, sizeof(Fld_Brush)};

// Nonmatching: the target also copies the touch coordinates to a second stack struct
s32 func_ov030_020c8a58(void) {
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
        s32 dy              = data_ov030_020ecb18[index].y - y;
        s32 distanceSquared = dy * dy;
        s32 dx              = data_ov030_020ecb18[index].x - x;
        distanceSquared += dx * dx;
        if (Sqrti(distanceSquared) < data_ov030_020ecb18[index].radius) {
            return index;
        }
    }
    return 14;
}

s32 func_ov030_020c8ae0(TaskPool* pool, Fld_Brush* data) {
    s16 index;

    for (index = 0; index < 3; index++) {
        if (data->taskIds[index] != -1 && func_ov030_020c01ec(EasyTask_GetTaskById(pool, data->taskIds[index]))) {
            return index;
        }
    }
    return -1;
}

void func_ov030_020c8b38(TaskPool* pool, Fld_Brush* data, s32 selected) {
    s32 index;

    for (index = 0; index < 3; index++) {
        if (data->taskIds[index] != -1) {
            Task* task = EasyTask_GetTaskById(pool, data->taskIds[index]);
            s32   mode = index == selected ? 1 : 2;
            func_ov030_020c01c4(task, mode);
        }
    }
}

// Nonmatching: activeBrush stores use r2 in the target, r0 here
s32 Fld_Brush_Init(TaskPool* pool, Task* task, void* args) {
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

    data->taskIds[0] = func_ov030_020c0184(pool, *ownerArg, 0, 0xE2, 0xA7);
    if (func_0202599c(2)) {
        data->taskIds[1] = func_ov030_020c0184(pool, *ownerArg, 2, 0x23, 0xA0);
    }
    if (func_0202599c(3)) {
        data->taskIds[2] = func_ov030_020c0184(pool, *ownerArg, 3, 0xE2, 0x21);
        func_ov030_020c01c4(EasyTask_GetTaskById(pool, data->taskIds[2]), 1);
        data->taskIds[3]  = func_ov030_020bfee4(pool, *ownerArg, 4);
        data->activeBrush = 2;
        data->taskIds[4]  = func_ov030_020c04d8(pool, *ownerArg);
    } else if (func_0202599c(2)) {
        func_ov030_020c01c4(EasyTask_GetTaskById(pool, data->taskIds[1]), 1);
        data->taskIds[3]  = func_ov030_020bfee4(pool, *ownerArg, 2);
        data->activeBrush = 1;
        data->taskIds[4]  = func_ov030_020c04d8(pool, *ownerArg);
    } else {
        data->taskIds[3]  = -1;
        data->activeBrush = -1;
    }
    data->owner   = (void*)*ownerArg;
    data->frame   = 0;
    data->closing = 0;
    return TRUE;
}

s32 func_ov030_020c8d24(Fld_Brush* data) {
    u32 index;

    for (index = 0; index < 14; index++) {
        if (data->taskIds[index + 5] != -1) {
            return FALSE;
        }
    }
    return TRUE;
}

// Nonmatching: the target does not CSE selected + 5 across the touch checks
s32 Fld_Brush_Update(TaskPool* pool, Task* task, void* args) {
    Fld_Brush* data = task->data;
    s32        selected;

    if (data->closing != 0) {
        return TRUE;
    }
    EasyFade_FadeBothDisplays(0, 0, 0x1000);
    if (TouchInput_WasTouchPressed()) {
        selected = func_ov030_020c8ae0(pool, data);
        if (selected != -1) {
            func_ov030_020c8b38(pool, data, selected);
            if (selected == 0) {
                Field_SetTouchEventResult(3);
                SndMgr_PlaySEWithPan(0xC4, 0xE2);
                data->closing = 1;
            } else if (selected == 1) {
                func_ov030_020bff68(EasyTask_GetTaskById(pool, data->taskIds[3]), 2);
                func_ov030_020c050c(EasyTask_GetTaskById(pool, data->taskIds[4]), 1);
                if (data->activeBrush != 1) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget(0x19);
                    SndMgr_PlaySEWithPan(0xC4, 0x23);
                }
            } else if (selected == 2) {
                func_ov030_020bff68(EasyTask_GetTaskById(pool, data->taskIds[3]), 4);
                func_ov030_020c050c(EasyTask_GetTaskById(pool, data->taskIds[4]), 2);
                if (data->activeBrush != 2) {
                    Field_SetTouchEventResult(4);
                    SndMgr_PlaySEWithPan(0xC4, 0xE2);
                }
            }
            data->activeBrush = selected;
        }
    } else if (TouchInput_IsTouchActive() && data->activeBrush == 2) {
        selected = func_ov030_020c8a58();
        if (selected != 14 && data->frame == 0 && data->taskIds[selected + 5] != -1) {
            EasyTask_DeleteTask(pool, data->taskIds[selected + 5]);
            data->taskIds[selected + 5] = -1;
            func_ov030_020c0730(pool, data->owner);
            data->frame = 5;
        }
        if (func_ov030_020c8d24(data)) {
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

s32 Fld_Brush_Render(TaskPool* pool, Task* task, void* args) {
    return TRUE;
}

s32 Fld_Brush_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_Brush* data = task->data;
    s32        index;

    for (index = 0; index < 19; index++) {
        if (data->taskIds[index] != -1) {
            EasyTask_DeleteTask(pool, data->taskIds[index]);
        }
    }
    return TRUE;
}

s32 Fld_Brush_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
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
