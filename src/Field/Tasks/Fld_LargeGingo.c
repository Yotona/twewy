#include "Field/FieldRegion.h"

#include "Display.h"
#include "Engine/EasyTask.h"
#include "Field/Tasks/Fld_LargeGingo.h"

extern void func_02026590(s32* value, s32 target, u16 frames);
extern void func_020265d4(s32* value, s32 target, u16 scale);

typedef struct {
    /* 0x00 */ u16 timer;
    /* 0x02 */ u16 phase;  // 0: scroll in, 1: cross-fade
    /* 0x04 */ s32 scroll; // fx32 sub BG0/BG1 vertical offset
    /* 0x08 */ s32 fade;   // fx32 blend weight
} Ov030LargeGingo;         // Size: 0xC
static s32 Fld_LargeGingo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_LargeGingo = {"Tsk_Fld_LargeGingo", Fld_LargeGingo_RunTask, sizeof(Ov030LargeGingo)};

static s32 Fld_LargeGingo_Init(TaskPool* pool, Task* task, void* args) {
    Ov030LargeGingo* data = task->data;

    data->timer  = 0x32;
    data->scroll = 0x40000;
    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, data->scroll);
    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, data->scroll);
    data->phase = 0;
    return 1;
}

// Eases the sub screen BG0/BG1 scroll back to 0, then switches to the
// cross-fade phase once the timer runs out.
static void Fld_LargeGingo_UpdateScroll(Ov030LargeGingo* data) {
    if (data->timer == 0) {
        return;
    }
    func_020265d4(&data->scroll, 0, data->timer);
    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, data->scroll);
    Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, data->scroll);
    data->timer = (u16)(data->timer - 1);
    if (data->timer != 0) {
        return;
    }
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode   = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 3;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 0x34;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = 0x10;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = 0;
    data->timer                                            = 0x32;
    data->fade                                             = 0;
    data->phase                                            = (u16)(data->phase + 1);
}

static void Fld_LargeGingo_UpdateFade(Ov030LargeGingo* data) {
    s32 v;

    if (data->timer == 0) {
        return;
    }
    func_02026590(&data->fade, 0x10000, data->timer);
    data->timer = (u16)(data->timer - 1);
    v           = data->fade >> 0xC;
    Display_SetBlendCoeffs(DISPLAY_SUB, 0x10 - v, v);
}

static s32 Fld_LargeGingo_Update(TaskPool* pool, Task* task, void* args) {
    Ov030LargeGingo* data = task->data;

    if (data->phase == 0) {
        Fld_LargeGingo_UpdateScroll(data);
    } else {
        Fld_LargeGingo_UpdateFade(data);
    }
    return 1;
}

static s32 Fld_LargeGingo_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_LargeGingo_Destroy(TaskPool* pool, Task* task, void* args) {
    g_DisplaySettings.controls[DISPLAY_SUB].layers         = g_DisplaySettings.controls[DISPLAY_SUB].layers & ~1;
    g_DisplaySettings.controls[DISPLAY_SUB].layers         = g_DisplaySettings.controls[DISPLAY_SUB].layers & ~2;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 0x34;
    return 1;
}

static s32 Fld_LargeGingo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_LargeGingo_Init,
        .update     = Fld_LargeGingo_Update,
        .render     = Fld_LargeGingo_Render,
        .cleanup    = Fld_LargeGingo_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_LargeGingo_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &Tsk_Fld_LargeGingo, NULL, 0, NULL, &param);
}
