#include "Display.h"
#include "Field/FieldTasks.h"

extern void func_02026590(s32*, s32, u16);
extern void func_ov030_020af7f0(void);

typedef struct {
    /* 0x00 */ s32  blendCoeff0;
    /* 0x04 */ s32  blendCoeff1;
    /* 0x08 */ s32  unk_08;
    /* 0x0C */ s32  unk_0C;
    /* 0x10 */ u16  unk_10;
    /* 0x12 */ u16  unk_12;
    /* 0x14 */ s32* unk_14;
} Fld_UGFade; // Size: 0x18

typedef struct {
    u16  unk_0;
    s32  unk_4;
    s32* unk_8;
} Fld_UGFade_Args;

s32 Fld_UGFade_RunTask(TaskPool*, Task*, void*, s32);

static const TaskHandle Tsk_Fld_UGFade = {"Tsk_Fld_UGFade", Fld_UGFade_RunTask, sizeof(Fld_UGFade)};

s32 Fld_UGFade_Init(TaskPool* pool, Task* task, void* args) {
    Fld_UGFade*      fade     = task->data;
    Fld_UGFade_Args* fadeArgs = args;

    func_ov030_020af7f0();

    u32 coeff0 = g_DisplaySettings.engineState[0].blendCoeff0;
    u32 coeff1 = g_DisplaySettings.engineState[0].blendCoeff1;

    fade->blendCoeff0 = I2F(coeff0);
    fade->blendCoeff1 = I2F(coeff1);

    if (fadeArgs->unk_4 == 0) {
        fade->unk_08 = 0xA000;
        fade->unk_0C = 0x8000;
    } else if (fadeArgs->unk_4 == 1) {
        fade->unk_08 = 0x10000;
        fade->unk_0C = 0;
    } else {
        fade->unk_08 = 0;
        fade->unk_0C = 0x10000;
    }
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 2;
    g_DisplaySettings.engineState[0].blendLayer1 = 51;

    fade->unk_10 = fadeArgs->unk_0;
    fade->unk_14 = fadeArgs->unk_8;
    return 1;
}

s32 Fld_UGFade_Update(TaskPool* pool, Task* task, void* args) {
    Fld_UGFade* fade = task->data;

    if (fade->unk_10 != 0) {
        func_02026590(&fade->blendCoeff0, fade->unk_08, fade->unk_10);
        func_02026590(&fade->blendCoeff1, fade->unk_0C, fade->unk_10);

        fade->unk_10--;

        g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = F2I(fade->blendCoeff0);
        g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = F2I(fade->blendCoeff1);
        return 1;
    }
    *fade->unk_14 = -1;
    return 0;
}

s32 Fld_UGFade_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Fld_UGFade_Destroy(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Fld_UGFade_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_UGFade_Init,
        .update     = Fld_UGFade_Update,
        .render     = Fld_UGFade_Render,
        .cleanup    = Fld_UGFade_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_UGFade_CreateTask(TaskPool* pool, u16 arg1, s32 arg2, s32* arg3) {
    Fld_UGFade_Args args;
    args.unk_4 = arg2;
    args.unk_8 = arg3;
    args.unk_0 = arg1;
    return EasyTask_CreateTask(pool, &Tsk_Fld_UGFade, NULL, 0, NULL, &args);
}
