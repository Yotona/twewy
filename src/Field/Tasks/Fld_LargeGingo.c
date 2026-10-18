// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020bf084-0x020bf360; 8 functions.
// Research draft: docs/decomp/ov030/Fld_LargeGingo.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Display.h"
#include "Engine/EasyTask.h"
#include "Field/TaskHelpers.h"

typedef struct {
    /* 0x00 */ u16 timer;
    /* 0x02 */ u16 phase;  // 0: scroll in, 1: cross-fade
    /* 0x04 */ s32 scroll; // fx32 sub BG0/BG1 vertical offset
    /* 0x08 */ s32 fade;   // fx32 blend weight
} Ov030LargeGingo;         // Size: 0xC

typedef TaskStages Ov030LargeGingoFns;

extern Ov030LargeGingoFns data_ov030_020e6d84;
extern TaskHandle         data_ov030_020e6d78;

s32 func_ov030_020bf084(TaskPool* pool, Task* task) {
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
void func_ov030_020bf130(Ov030LargeGingo* data) {
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

void func_ov030_020bf23c(Ov030LargeGingo* data) {
    s32 v;

    if (data->timer == 0) {
        return;
    }
    func_02026590(&data->fade, 0x10000, data->timer);
    data->timer = (u16)(data->timer - 1);
    v           = data->fade >> 0xC;
    Display_SetBlendCoeffs(DISPLAY_SUB, 0x10 - v, v);
}

s32 func_ov030_020bf288(TaskPool* pool, Task* task) {
    Ov030LargeGingo* data = task->data;

    if (data->phase == 0) {
        func_ov030_020bf130(data);
    } else {
        func_ov030_020bf23c(data);
    }
    return 1;
}

s32 func_ov030_020bf2b0(TaskPool* pool, Task* task) {
    return 1;
}

s32 func_ov030_020bf2b8(TaskPool* pool, Task* task) {
    g_DisplaySettings.controls[DISPLAY_SUB].layers         = g_DisplaySettings.controls[DISPLAY_SUB].layers & ~1;
    g_DisplaySettings.controls[DISPLAY_SUB].layers         = g_DisplaySettings.controls[DISPLAY_SUB].layers & ~2;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 0x34;
    return 1;
}

s32 func_ov030_020bf2e8(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030LargeGingoFns funcs = data_ov030_020e6d84;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020bf330(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &data_ov030_020e6d78, NULL, 0, NULL, &param);
}
