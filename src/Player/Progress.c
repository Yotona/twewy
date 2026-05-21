#include "Player/Progress.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/FieldTasks.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "registers.h"
#include "types.h"

extern s16 data_ov030_020d9586[];
extern s16 data_ov030_020d9588[];
extern s16 data_ov030_020d958a[];
extern s32 data_ov030_020da264;

extern u8    data_ov030_020dbdd8[88];
extern void* EventScripts[2207];

extern s32  func_02023be8(u16 itemID, s32 arg1);
extern void func_02026590(void*, s32, s32);

extern void func_ov030_020824a0(void*);
extern void func_ov030_02082534(void*);
extern void func_ov030_02082578(void*);
extern void func_ov030_0208299c(void*);
extern void func_ov030_02082a04(void*);
extern void func_ov030_02082a84(void*);
extern void func_ov030_02082b4c(void*);
extern void func_ov030_02082b8c(void*);
extern void func_ov030_02082bf0(void*);
extern void func_ov030_02082c04(void*);
extern void func_ov030_02082c68(void*);
extern void func_ov030_02082d64(void*);
extern void func_ov030_02082e70(void*);
extern void func_ov030_02082f10(void*);
extern void func_ov030_020830ac(ProgressObject*);
extern void func_ov030_02083940(ProgressObject*, s32);
extern void func_ov030_0208434c(ProgressObject*, s32, s32);
extern void func_ov030_020846bc(ProgressObject*, s32);
extern s32  func_ov030_020aa4bc(ProgressObject*);
extern void func_ov030_020aac84(ProgressObject*);
extern BOOL func_ov030_020ab3ec(u32);
extern void func_ov030_020ac95c(ProgressObject*);
extern void func_ov030_020ad044(ProgressObject*);
extern void func_ov030_020ad1ac(ProgressObject*);
extern void func_ov030_020ad428(ProgressObject*);
extern void func_ov030_020af364(s32);
extern BOOL func_ov030_020b0454(void);
extern void func_ov030_020b3034(Task*);
extern s32  func_ov030_020b3774(TaskPool*, s32, s32, s32, s32, s16, s16, s16);
extern void func_ov030_020b37cc(Task*, s32, s32);
extern BOOL func_ov030_020b38a4(Task*);
extern BOOL func_ov030_020b38d4(Task*);
extern void func_ov030_020b38f8(Task*, s32);
extern void func_ov030_020b3934(Task*, s32);
extern void func_ov030_020b3948(Task*);
extern void func_ov030_020b39e0(TaskPool*, Task*, s32, s32, s32);
extern s32  func_ov030_020b3b10(Task*);
extern void func_ov030_020b3b3c(Task*, s32);
extern void func_ov030_020b3b64(Task*, s32);
extern void func_ov030_020b3b70(Task*, s16, s16, s16);
extern void func_ov030_020b3bdc(TaskPool*, Task*, s32, s32);
extern BOOL func_ov030_020b3c84(Task*);
extern void func_ov030_020b3ca0(Task*);
extern s16  func_ov030_020b3cc8(Task*);
extern void func_ov030_020b3cd4(Task*, s32);
extern void func_ov030_020b3cf0(Task*);
extern s32  func_ov030_020b3d88(TaskPool*, s32, s32, s32, s32, s32, s16, s16, s16, s16, s16, s16);
extern u32  func_ov030_020b3fbc(TaskPool*, s32*, s32, s32, s16, s16, u32, s8, s16, s16);
extern void func_ov030_020b400c(Task*, s32);
extern void func_ov030_020b4038(Task*);
extern void func_ov030_020b4060(Task*, s32);
extern void func_ov030_020b40ac(Task*, s32, s32);
extern BOOL func_ov030_020b40e8(Task*);
extern void func_ov030_020b411c(Task*);
extern s32  func_ov030_020b44b8(TaskPool*, s32, s32, s32, s32, s32);
extern BOOL func_ov030_020b4514(Task*);
extern s32  func_ov030_020b4570(Task*, s32);
extern void func_ov030_020b458c(Task*, s32);
extern void func_ov030_020b4598(Task*, s32);
extern void func_ov030_020c2d50(s32);
extern void func_ov030_020c2de8(void);
extern void func_ov030_020c2e28(void);
extern void func_ov030_020c2fc0(s32);
extern s32  func_ov030_020c3654(TaskPool*, s32, s32, s16);
extern s32  func_ov030_020c420c(TaskPool*, Task*);
extern s32  func_ov030_020c47d0(TaskPool*, s32, s32, s32);
extern s32  func_ov030_020c4a48(TaskPool*, s32);
extern void func_ov030_020c6150(s32*);
extern void func_ov030_020c6300(void);
extern s32  func_ov030_020c6e5c(TaskPool*, s32, s32);
extern s32  func_ov030_020c717c(TaskPool*, s32, s32);
extern s32  func_ov030_020c83e8(TaskPool*, Task*);

extern s32 func_ov031_0210c060(u16);

// Nonmatching
void Progress_SetCurrentEvent(ProgressEvent event) {
    s32 var_lr                          = (event >= 2207) ? event - 2207 : event;
    gSaveState.unk_20.unk_249C          = 0;
    gSaveState.unk_20.currentStoryEvent = event;
    gSaveState.unk_20.unk_24A0          = EventScripts[var_lr];
}

s32 func_ov030_020a66e8(ProgressObject* object) {
    if (gSaveState.unk_20.unk_2488 == 0) {
        if ((gSaveState.unk_20.currentStoryEvent >= 5) && (gSaveState.unk_20.currentStoryEvent <= 39)) {
            return 1;
        }
    }
    if (object->unk_21D22 != 0) {
        return 1;
    }
    return 0;
}

// Nonmatching
void Progress_AdvanceEventScript(ProgressObject* progress) {
    gSaveState.unk_20.unk_249C += data_ov030_020dbdd8[gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C]];
    if (progress->unk_21ADC != 0 && func_ov030_020a66e8(progress) == 0 && progress->unk_21AE0 != 0) {
        while (TRUE) {
            switch (gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C]) {
                case 0x00:
                case 0x01:
                case 0x02:
                case 0x03:
                case 0x04:
                case 0x05:
                case 0x06:
                case 0x07:
                case 0x08:
                case 0x12:
                case 0x27:
                case 0x2D:
                case 0x2E:
                case 0x30:
                case 0x3C:
                case 0x3D:
                case 0x3E:
                case 0x3F:
                case 0x55:
                case 0x56:
                    return;
                default:
                    gSaveState.unk_20.unk_249C += data_ov030_020dbdd8[gSaveState.unk_20.unk_249C];
            }
        }
    }
}

// Nonmatching
s32 func_ov030_020a68a8(ProgressObject* progress, s32 arg1) {
    if (arg1 == -1) {
        arg1 = progress->unk_22E2C % 280;
        progress->unk_22E2C++;
    }
    return arg1;
}

// Not yet implemented
s32 func_ov030_020a68f4(s32 progress) {
    // s32 *var_lr;
    // s32 *var_r4;
    // s32 temp_r0;
    // s32 temp_r1;
    // s32 temp_r2;
    // s32 temp_r3;
    // s32 var_ip;
    // u32 var_r0;

    // var_r4 = &data_ov030_020da264;
    // var_lr = &subroutine_arg0;
    // var_ip = 8;
    // do {
    //     temp_r0 = var_r4->unk0;
    //     temp_r1 = var_r4->unk4;
    //     temp_r2 = var_r4->unk8;
    //     temp_r3 = var_r4->unkC;
    //     var_r4 += 0x10;
    //     var_lr->unk0 = temp_r0;
    //     var_lr->unk4 = temp_r1;
    //     var_lr->unk8 = temp_r2;
    //     var_lr->unkC = temp_r3;
    //     var_lr += 0x10;
    //     var_ip -= 1;
    // } while (var_ip != 0);
    // var_lr->unk0 = (s32)var_r4->unk0;
    // var_lr->unk4 = (s32)var_r4->unk4;
    // var_lr->unk8 = (s32)var_r4->unk8;
    // var_r0       = 0;
    // if (0U >= 35) {
    //     return;
    // }
    // loop_5:
    // if ( progress < (s32)(&subroutine_arg0)[var_r0]) {
    //     return;
    // }
    // var_r0 += 1;
    // if (var_r0 >= 35) {
    //     return;
    // }
    // goto loop_5;
}

u32 func_ov030_020a6960(ProgressObject* progress, s32 arg1) {
    if (arg1 == 0x118) {
        if (progress->unk_21CD0 == 0) {
            progress->unk_21CD0 = progress->unk_21BBC;
        }
    } else {
        progress->unk_21CD0 = &progress->unk_21BBC[func_ov030_020a68f4(arg1)];
    }
}

s32 func_ov030_020a69ac(TaskPool* taskPool, u32* arg1, s16 arg2) {
    if (*arg1 != -1) {
        func_ov030_020b37cc(EasyTask_GetTaskById(taskPool, *arg1), arg2, 0);
        if (func_ov030_020b38a4(EasyTask_GetTaskById(taskPool, *arg1)) == 0) {
            return 0;
        }
        EasyTask_DeleteTask(taskPool, *arg1);
        *arg1 = -1;
        return 1;
    }
    return 1;
}

void func_ov030_020a6a1c(ProgressObject* progress) {
    for (s32 i = 0; i < 32; i++) {
        if (progress->unk_21C48[i] != -1) {
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_21C48[i]);
            progress->unk_21C48[i] = -1;
        }
    }
}

void func_ov030_020a6a68(ProgressObject* progress) {
    for (s32 i = 16; i < 32; i++) {
        if (progress->unk_21C48[i] != -1) {
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_21C48[i]);
            progress->unk_21C48[i] = -1;
        }
    }
}

BOOL func_ov030_020a6ab4(ProgressObject* progress) {
    s32  i;
    BOOL found = FALSE;

    for (i = 0; i < 32; i++) {
        if ((progress->unk_21C48[i] != -1) && (EasyTask_ValidateTaskId(&progress->taskPool, &progress->unk_21C48[i]))) {
            if (func_ov030_020b0454() != 0) {
                if (progress->unk_21CD4 == i) {
                    func_ov030_020b4038(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[i]));
                } else {
                    EasyTask_DeleteTask(&progress->taskPool, progress->unk_21C48[i]);
                    progress->unk_21C48[i] = -1;
                }
            }
            found = TRUE;
        }
    }
    return found;
}

void func_ov030_020a6b6c(ProgressObject* progress, s32 arg1, s32 arg2, s32 arg3) {
    for (s32 i = arg2; i < arg3; i++) {
        if ((progress->unk_21C48[i] != -1) && (progress->unk_21CD4 != i)) {
            if (func_ov030_020b0454() != 0) {
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_21C48[i]);
                progress->unk_21C48[i] = -1;
            } else {
                func_ov030_020b4060(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[i]), arg1);
                func_ov030_020b400c(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[i]), 2);
            }
        }
    }
}

void func_ov030_020a6c24(ProgressObject* progress, s32 arg1) {
    func_ov030_020a6b6c(progress, arg1, 0, 0x10);
}

void func_ov030_020a6c38(ProgressObject* progress, s32 arg1) {
    func_ov030_020a6b6c(progress, arg1, 0x10, 0x20);
}

void func_ov030_020a6c4c(ProgressObject* progress, s32 arg1, s32 arg2, s32 arg3) {
    for (s32 i = arg2; i < arg3; i++) {
        if (progress->unk_21C48[i] != -1) {
            if (func_ov030_020b0454() != 0) {
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_21C48[i]);
                progress->unk_21C48[i] = -1;
            } else {
                func_ov030_020b4060(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[i]), arg1);
                func_ov030_020b400c(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[i]), 2);
            }
        }
    }
}

void func_ov030_020a6cf8(ProgressObject* progress, s32 arg1) {
    func_ov030_020a6c4c(progress, arg1, 0, 0x20);
}

void func_ov030_020a6d0c(ProgressObject* progress, s32 arg1) {
    func_ov030_020a6c4c(progress, arg1, 0x10, 0x20);
}

void func_ov030_020a6d20(ProgressObject* progress, s32 arg1) {
    func_ov030_020a6c4c(progress, arg1, 0, 0x10);
}

// Nonmatching
s32 func_ov030_020a6d34(ProgressObject* progress, s32 arg1, s32 arg2) {
    for (s32 i = arg1; i < arg2; i++) {
        if (progress->unk_21C48[i] == -1) {
            progress->unk_21CD4 = i;
            return progress->unk_21C48[i];
        }
    }
    return 0;
}

s32 func_ov030_020a6d80(ProgressObject* progress) {
    return func_ov030_020a6d34(progress, 0, 0x10);
}

s32 func_ov030_020a6d94(ProgressObject* progress) {
    return func_ov030_020a6d34(progress, 0x10, 0x20);
}

void Progress_InitState(ProgressObject* progress) {
    for (s32 i = 0; i < 35; i++) {
        progress->unk_21BBC[i] = -1;
    }

    progress->unk_21CD0 = 0;
    for (s32 i = 0; i < 32; i++) {
        progress->unk_21C48[i] = -1;
    }

    progress->unk_21CEA = 0;
    progress->unk_21CD4 = -1;
    progress->unk_21D00 = 280;
    progress->unk_21CEC = 0;
    progress->unk_21CF0 = 1;
    progress->unk_21AD8 = 0;
    progress->unk_21ADC = 1;
    progress->unk_21AE0 = 0;
    progress->unk_21CF8 = 0;
    progress->unk_21CFC = 0;
    progress->unk_21758 = 0;
    progress->unk_2176A = 0;
    progress->unk_21D22 = 0;
}

s32 func_ov030_020a6e50(void) {
    return g_DisplaySettings.controls[DISPLAY_SUB].layers & 8;
}

// Nonmatching
void func_ov030_020a6e64(ProgressObject* progress, u32 arg1, Task* arg2) {
    if (func_ov030_020a6e50() != 0 && func_ov030_020a6960(progress, progress->unk_21D00) == arg1) {
        func_ov030_020b3cd4(arg2, 0);
    }
}

void func_ov030_020a6eac(ProgressObject* progress, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6) {
    s32  resourceId = func_ov030_020a68a8(progress, arg1);
    u32* taskIdPtr  = (u32*)func_ov030_020a6960(progress, resourceId);
    u32  taskId     = *taskIdPtr;

    if (taskId != -1) {
        Task* task = EasyTask_GetTaskById(&progress->taskPool, taskId);
        func_ov030_020b3bdc(&progress->taskPool, task, arg1, 1);
        func_ov030_020b3cf0(task);
        func_ov030_020b3b70(task, arg5, arg6, arg2);
        func_ov030_020a6e64(progress, *taskIdPtr, task);
    } else {
        *taskIdPtr = func_ov030_020b3774(&progress->taskPool, progress->unk_21614, resourceId, arg2, arg3, arg4, arg5, arg6);
    }
}

void func_ov030_020a6f90(ProgressObject* progress) {
    u32*  temp_r4 = (u32*)func_ov030_020a6d94(progress);
    u32*  temp_r5 = (u32*)func_ov030_020a6960(progress, progress->unk_21D00);
    Task* task    = EasyTask_GetTaskById(&progress->taskPool, *temp_r5);
    s32   val     = func_ov030_020b3cc8(task);
    *temp_r4      = func_ov030_020b3d88(&progress->taskPool, progress->unk_21614, val, 0xD0, val, 0xD0, 0, 0x16, 8, 0, 0, 0);
}

s32 func_ov030_020a701c(ProgressObject* progress) {
    s32 temp_r0 =
        func_ov030_020c420c(&progress->taskPool, EasyTask_GetTaskById(&progress->taskPool, progress->taskId_KeywordMenu));

    if (temp_r0 >= 0) {
        if (temp_r0 != progress->unk_21D2A) {
            progress->unk_21D2A = (s16)temp_r0;
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);

            Task* task = EasyTask_GetTaskById(&progress->taskPool, func_ov030_020a6960(progress, progress->unk_21D00));
            func_ov030_020b3cd4(task, 1);

            s32 temp_r4                = func_ov030_020b3cc8(task);
            gSaveState.unk_20.unk_24C4 = progress->unk_21D2A;
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_KeywordMenu);
            progress->taskId_KeywordMenu = -1;
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = 0;
            progress->unk_216C0 = func_ov030_020c47d0(&progress->taskPool, progress->unk_21614, temp_r4, 0x78);
            progress->unk_216C4 = func_ov030_020b44b8(&progress->taskPool, progress->unk_21614, temp_r4, 0x78, 0, 1);
            func_ov030_020b4570(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216C4), 2);
            func_ov030_020a6f90(progress);
            func_ov030_020a6d0c(progress, 0x44);
        }
        return 0;
    }
    if (temp_r0 != -2) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020a71a8(ProgressObject* progress) {
    if (progress->unk_216C0 != -1) {
        if (EasyTask_ValidateTaskId(&progress->taskPool, &progress->unk_216C0) == 0) {
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_216C4);
            progress->unk_216C4 = -1;
            progress->unk_21D08(progress);
        }
        return 0;
    }
    if (progress->taskId_KeywordMenu == -1) {
        return 1;
    }
    return func_ov030_020a701c(progress);
}

s32 func_ov030_020a722c(ProgressObject* progress) {
    if (func_ov030_020a71a8(progress) == 0) {
        return 0;
    }
    if ((TouchInput_WasTouchPressed() != 0) ||
        (SysControl.buttonState.pressedButtons &
         (INPUT_BUTTON_X | INPUT_BUTTON_Y | INPUT_BUTTON_UP | INPUT_BUTTON_DOWN | INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT |
          INPUT_BUTTON_START | INPUT_BUTTON_B | INPUT_BUTTON_A)))
    {
        return 1;
    }
    if (func_ov030_020b0454() != 0) {
        return 1;
    }
    return 0;
}

void func_ov030_020a7284(ProgressObject* progress, s16 arg1, s32 arg2, s32 arg3) {
    Task* task;
    u32*  temp_r0;

    temp_r0 = (u32*)func_ov030_020a6960(progress, arg1);
    task    = EasyTask_GetTaskById(&progress->taskPool, *temp_r0);
    if (func_ov030_020b3b10(task) != 0) {
        Progress_AdvanceEventScript(progress);
    } else {
        func_ov030_020b39e0(&progress->taskPool, task, arg1, arg2, arg3);
    }
    func_ov030_020a6e64(progress, *temp_r0, task);
}

void func_ov030_020a72fc(ProgressObject* progress, s16 arg1, s16 arg2) {
    Task* temp_r0_2;
    u32*  temp_r0;

    temp_r0   = (u32*)func_ov030_020a6960(progress, arg1);
    temp_r0_2 = EasyTask_GetTaskById(&progress->taskPool, *temp_r0);
    if (temp_r0_2 == NULL) {
        Progress_AdvanceEventScript(progress);
        return;
    }
    if (func_ov030_020b3c84(temp_r0_2) != 0) {
        Progress_AdvanceEventScript(progress);
    } else {
        func_ov030_020b3bdc(&progress->taskPool, temp_r0_2, arg1, arg2);
    }
    func_ov030_020a6e64(progress, *temp_r0, temp_r0_2);
}

void func_ov030_020a737c(ProgressObject* progress) {
    if (progress->unk_21A5C != 2) {
        if (progress->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness) {
            EasyFade_FadeBothDisplays(FADER_INSTANT, progress->targetBrightness, (s32)progress->unk_21D12);
            return;
        }
        REG_POWER_CNT |= 0x8000;
        func_ov030_020846bc(progress, 2);
        func_ov030_020830ac(progress);
        g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1;
        func_ov030_020ad1ac(progress);
        func_ov030_020b458c(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216B0), 0);
        Progress_AdvanceEventScript(progress);
        EasyFade_FadeBothDisplays(FADER_INSTANT, 0, progress->unk_21D12);
        progress->unk_216C8 = func_ov030_020c4a48(&progress->taskPool, progress->unk_21614);
        return;
    }
    Progress_AdvanceEventScript(progress);
}

void func_ov030_020a7468(ProgressObject* progress) {
    EasyFade_FadeMainDisplay(FADER_INSTANT, 0, progress->unk_21D12);
    if (progress->taskId_UGFade == -1) {
        g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 0;
        g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 0x10;
        progress->taskId_UGFade =
            Fld_UGFade_CreateTask(&progress->taskPool, (u16)progress->unk_21D12, 0, &progress->taskId_UGFade);
    }
    if (progress->unk_21A5C != 2) {
        if (progress->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness) {
            EasyFade_FadeSubDisplay(FADER_INSTANT, progress->targetBrightness, progress->unk_21D12);
            return;
        }
        func_ov030_020846bc(progress, 2);
        Progress_AdvanceEventScript(progress);
        EasyFade_FadeSubDisplay(FADER_INSTANT, 0, progress->unk_21D12);
        return;
    }
    Progress_AdvanceEventScript(progress);
}

s32 func_ov030_020a7550(ProgressObject* progress) {
    s32  var_r7 = 0;
    u32* var_r8 = progress->unk_21BBC;
    do {
        func_ov030_020a69ac(&progress->taskPool, var_r8, 1);
        var_r7 += 1;
        var_r8 += 1;
    } while (var_r7 < 35);

    for (s32 i = 0; i < 35; i++) {
        if (progress->unk_21BBC[i] != -1) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020a75c0(ProgressObject* progress) {
    func_ov030_020a6a1c(progress);
    func_ov030_020a7550(progress);
}

s32 func_ov030_020a75d8(ProgressObject* progress, s16 arg1) {
    if (progress->unk_21A5C != 3) {
        if (progress->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness) {
            EasyFade_FadeSubDisplay(FADER_INSTANT, progress->targetBrightness, progress->unk_21D12);
            if (progress->unk_21AC4 != 0) {
                EasyFade_FadeMainDisplay(FADER_INSTANT, progress->targetBrightness, progress->unk_21D12);
            }
        } else {
            func_ov030_020a6a1c(progress);
            if (func_ov030_020a7550(progress) != 0) {
                return 1;
            }
            if (arg1 == 0x49) {
                arg1 = gSaveState.unk_20.unk_2458;
            }
            func_ov030_0208434c(progress, 3, arg1);
            if (progress->unk_21AC4 != 0) {
                EasyFade_FadeMainDisplay(FADER_INSTANT, 0, progress->unk_21D12);
                if (progress->taskId_UGFade == -1) {
                    if (arg1 == gSaveState.unk_20.unk_2458) {
                        progress->taskId_UGFade =
                            Fld_UGFade_CreateTask(&progress->taskPool, (u16)progress->unk_21D12, 0, &progress->taskId_UGFade);
                    } else {
                        progress->taskId_UGFade =
                            Fld_UGFade_CreateTask(&progress->taskPool, (u16)progress->unk_21D12, 1, &progress->taskId_UGFade);
                    }
                }
            }
            EasyFade_FadeSubDisplay(FADER_INSTANT, 0, progress->unk_21D12);
        }
        return 1;
    }

    progress->unk_21A5C = (arg1 == gSaveState.unk_20.unk_2458) ? 2 : 9;
    return 0;
}

s32 func_ov030_020a775c(u16 itemID, u16* arg1, u16* arg2) {
    s32          val      = 0;
    ItemCategory category = Inventory_GetCategory(itemID);
    u16          index    = Inventory_GetCategorizedIndex(itemID);

    if (category == ITEM_CATEGORY_THREAD) {
        *arg1 = index + 0x2C14;
        *arg2 = index + 0x2AFC;
    } else if (category == ITEM_CATEGORY_FOOD) {
        *arg1 = index + 0x2A6D;
        *arg2 = index + 0x2A43;
    } else if (category == ITEM_CATEGORY_SWAG) {
        *arg1 = index + 0x34BE;
        *arg2 = index + 0x3428;
    } else {
        *arg1 = index + 0x27C1;
        *arg2 = index + 0x2430;
        val   = 1;
    }
    return val;
}

// Nonmatching
void func_ov030_020a7810(ProgressObject* progress) {
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= 8;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[DISPLAY_BG3].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode                        = 1;

    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 8;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 20;

    progress->unk_21AB0 = 0;
    progress->unk_21AB4 = 0x10000;

    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = F2I(progress->unk_21AB0);
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = F2I(0x10000);

    progress->unk_21AAE = 30;
    DebugOvlDisp_Push(func_ov030_02082e70, progress, PROCESS_STAGE_INIT);
}

void func_ov030_020a7890(ProgressObject* progress) {
    progress->unk_21AB0 = 0xA000;
    progress->unk_21AB4 = 0x8000;
    progress->unk_21AAE = 10;
    DebugOvlDisp_Push(func_ov030_02082f10, progress, PROCESS_STAGE_INIT);
}

void func_ov030_020a78d0(ProgressObject* progress) {
    if (progress->unk_21CF0 != 0) {
        SndMgr_StartPlayingSeqWithCustomSeqArc(1, 2, 7);
        progress->unk_21CF0 = 0;
    }
    if (progress->unk_21CF4 != 0) {
        func_ov030_020a737c(progress);
        return;
    }
    func_ov030_020a7468(progress);
}

void func_ov030_020a7928(ProgressObject* progress, s32 arg1, s16 arg2, u16 arg3) {
    progress->unk_21CE4 = arg1;
    progress->unk_21CE8 = arg3;
    if (progress->unk_21CE8 == 0) {
        progress->unk_21CE8 = 1;
    }
    progress->unk_21CDC = I2F(g_DisplaySettings.controls[DISPLAY_MAIN].brightness);
    progress->unk_21CE0 = I2F(g_DisplaySettings.controls[DISPLAY_SUB].brightness);
    if (arg2 == 0) {
        progress->unk_21CD8 = 0x10;
        return;
    }
    if (arg2 == 1) {
        progress->unk_21CD8 = -0x10;
        return;
    }
    progress->unk_21CD8 = 0;
}

s32 func_ov030_020a7994(ProgressObject* progress) {
    if (progress->unk_21C48[progress->unk_21CD4] != -1) {
        if (func_ov030_020b40e8(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[progress->unk_21CD4])) != 0) {
            return 0;
        }
    }

    Task* task = EasyTask_GetTaskById(&progress->taskPool, *(u32*)func_ov030_020a6960(progress, 0x118));
    if ((task != NULL) && func_ov030_020b38d4(task) == 0) {
        return 0;
    }
    if (progress->unk_21A24 == -1) {
        s32 var_r2 = 0;
        if (progress->unk_21CF4 != 0) {
            var_r2 = 1;
        }
        progress->unk_21A24 = func_ov030_020c6e5c(&progress->taskPool, progress->unk_21614, var_r2);
    }
    if (func_ov030_020a722c(progress) != 0) {
        SndMgr_StartPlayingSE(SEIDX_SE_DIALOG_NEXT);
        if (progress->unk_21A24 != -1) {
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_21A24);
            progress->unk_21A24 = -1;
        }
        return 1;
    }
    return 0;
}

void func_ov030_020a7aa0(u32 arg0, ProgressObject* progress, u32 arg2) {
    if (arg0 != -1) {
        if ((progress->unk_21CEA + 1) == func_ov031_0210c060(arg2)) {
            progress->unk_21CEA = 0;
            Progress_AdvanceEventScript(progress);
        } else {
            gSaveState.unk_20.unk_24AC = gSaveState.unk_20.unk_249C;
            gSaveState.unk_20.unk_24B0 = gSaveState.unk_20.unk_24A0;
            gSaveState.unk_20.unk_24A0 = data_ov030_020d958a;
            gSaveState.unk_20.unk_249C = 0;
        }
    } else {
        func_ov030_020a6cf8(progress, 0x120);
        gSaveState.unk_20.unk_24AC = gSaveState.unk_20.unk_249C;
        gSaveState.unk_20.unk_24B0 = gSaveState.unk_20.unk_24A0;
        gSaveState.unk_20.unk_24A0 = data_ov030_020d9588;
        gSaveState.unk_20.unk_249C = 0;
    }
}

void func_ov030_020a7b50(ProgressObject* progress) {
    func_ov030_020a6ab4(progress);
    switch (gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C]) {
        case 0x0:
        case 0x2:
            progress->targetBrightness = -0x10;
            progress->unk_21D12        = 0x10;
            func_ov030_020a78d0(progress);
            return;
        case 0x4:
            progress->targetBrightness = -0x10;
            progress->unk_21D12        = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            func_ov030_020a78d0(progress);
            return;
        case 0x1:
            progress->targetBrightness = 0x10;
            progress->unk_21D12        = 0x10;
            func_ov030_020a78d0(progress);
            return;
        case 0x3:
            progress->targetBrightness = 0x10;
            progress->unk_21D12        = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            func_ov030_020a78d0(progress);
            return;
        case 0x9:
            progress->targetBrightness = 0x10;
            progress->unk_21D12        = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            Progress_AdvanceEventScript(progress);
            return;
        case 0xA:
            progress->targetBrightness = -0x10;
            progress->unk_21D12        = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            Progress_AdvanceEventScript(progress);
            return;
        case 0xB:
            func_ov030_020a6eac(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0x14, -0x3F3,
                                -0x3F1, -0x3F6, -0x3F1);
            Progress_AdvanceEventScript(progress);
            func_ov030_020aac84(progress);
            return;
        case 0xC:
            func_ov030_020a6eac(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0x14, -0x3FD,
                                -0x3F1, -0x3FA, -0x3F1);
            Progress_AdvanceEventScript(progress);
            func_ov030_020aac84(progress);
            return;
        case 0xD:
            func_ov030_020a6eac(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)), -0x3F3, -0x3F1, -0x3F6,
                                -0x3F1);
            Progress_AdvanceEventScript(progress);
            func_ov030_020aac84(progress);
            return;
        case 0xE:
            func_ov030_020a6eac(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)), -0x3FD, -0x3F1, -0x3FA,
                                -0x3F1);
            Progress_AdvanceEventScript(progress);
            func_ov030_020aac84(progress);
            return;
        case 0xF:
            func_ov030_020a6eac(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)),
                                (s16)(s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)),
                                (s16)(s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 5) * 2)),
                                (s16)(s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 6) * 2)));
            Progress_AdvanceEventScript(progress);
            func_ov030_020aac84(progress);
            return;
        case 0x10:
            if (func_ov030_020a7994(progress) == 0) {
                return;
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x11:
            if (progress->unk_21C48[progress->unk_21CD4] != -1) {
                func_ov030_020b40ac(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[progress->unk_21CD4]),
                                    *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                    *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x12:
            Progress_SetGate(*(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)));
            Progress_AdvanceEventScript(progress);
            return;

        case 0x1A: {
            u32* temp_r6 = (u32*)func_ov030_020a6d80(progress);
            *temp_r6 =
                func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                    (s16)(*(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)) - 0x40), 0x40,
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 8, 8, 1,
                                    (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r6, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x1B: {
            u32* temp_r7 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7     = func_ov030_020b3fbc(
                &progress->taskPool, &progress->unk_21614,
                (s16)(*(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)) - 0x40), 0x40,
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 8,
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)), 1, (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7, progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x1C:
            func_ov030_020a6a1c(progress);
            u32* temp_r6_2 = (u32*)func_ov030_020a6d80(progress);
            *temp_r6_2     = func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                                 *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0x80,
                                                 (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                                 (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 2,
                                                 0, 0, progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r6_2, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
            return;

        case 0x1D: {
            u32* temp_r7_2 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7_2 =
                func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                    *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0xFFFFFC02,
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 0xC, 8, 1,
                                    (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_2, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x1E: {
            u32* temp_r7_3 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7_3     = func_ov030_020b3fbc(
                &progress->taskPool, &progress->unk_21614,
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0xFFFFFC02,
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 0xC,
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)), 1, (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_3, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x1F: {
            u32* temp_r7_4 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7_4 =
                func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                    *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0xFFFFFC02,
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 0xE, 8, 1,
                                    (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_4, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x20: {
            u32* temp_r7_5 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7_5 =
                func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                    *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0xFFFFFC02,
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 0xA, 8, 1,
                                    (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_5, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x21: {
            u32* temp_r7_6 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7_6     = func_ov030_020b3fbc(
                &progress->taskPool, &progress->unk_21614,
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0xFFFFFC02,
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)), 0xA,
                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)), 1, (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_6, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x22: {
            u32* temp_r8_3 = (u32*)func_ov030_020a6d80(progress);
            *temp_r8_3     = func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                                 *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                                 (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)),
                                                 (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)),
                                                 (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 5) * 2)),
                                                 (s32)(u8) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 6) * 2)),
                                                 (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 7) * 2)),
                                                 (s32)(u8) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 8) * 2)),
                                                 (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r8_3, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 5) * 2)));
        } break;

        case 0x23: {
            u32* temp_r7_7 = (u32*)func_ov030_020a6d94(progress);
            u8   var_r6    = (u8) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 6) * 2));
            s16  var_r0    = *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2));
            s16  var_r2    = *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2));
            if (var_r6 != 2) {
                var_r6 = 0x5D;
                var_r0 = func_ov030_020b3cc8(
                    EasyTask_GetTaskById(&progress->taskPool, *(u32*)func_ov030_020a6960(progress, progress->unk_21D00)));
                var_r2 = var_r0;
            }
            *temp_r7_7 =
                func_ov030_020b3d88(&progress->taskPool, progress->unk_21614, var_r2, 0xD0,
                                    (s16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 5) * 2)), (s32)var_r6,
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 7) * 2)), 0, 0,
                                    (s32)progress->unk_21CEA, 0, 0);
            func_ov030_020a7aa0(*temp_r7_7, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 5) * 2)));
        } break;

        case 0x24: {
            u32* temp_r7_8 = (u32*)func_ov030_020a6d80(progress);
            *temp_r7_8 =
                func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614,
                                    *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 0xFFFFFC02,
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                    (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)),
                                    (s32)(u8) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)), 8, 1,
                                    (s32)progress->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_8, progress,
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
        } break;

        case 0x25:
            if (func_ov030_020a6ab4(progress) != 0) {
                return;
            }
            gSaveState.unk_20.unk_249C = gSaveState.unk_20.unk_24AC;
            gSaveState.unk_20.unk_24A0 = gSaveState.unk_20.unk_24B0;
            return;
        case 0x26:
            if (func_ov030_020a7994(progress) == 0) {
                return;
            }
            progress->unk_21CEA        = (u16)(progress->unk_21CEA + 1);
            gSaveState.unk_20.unk_249C = gSaveState.unk_20.unk_24AC;
            gSaveState.unk_20.unk_24A0 = gSaveState.unk_20.unk_24B0;
            return;
        case 0x13:
            func_ov030_020a6c24(progress, 0x44);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x14:
            func_ov030_020a6cf8(progress, 0x44);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x15:
            func_ov030_020a6c38(progress, 0x44);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x16:
            func_ov030_020a6d0c(progress, 0x44);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x17:
            func_ov030_020a6cf8(progress, 0x120);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x18:
            func_ov030_020a6d0c(progress, 0x120);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x19:
            func_ov030_020a6a1c(progress);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x27:
            Progress_AdvanceEventScript(progress);
            return;
        case 0x2D:
            func_ov030_020af364(*(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x2E:
            progress->unk_21630 = 3;
            DebugOvlDisp_Pop();
            Progress_AdvanceEventScript(progress);
            return;
        case 0x2F:
            progress->unk_21CEC = 1;
            Progress_AdvanceEventScript(progress);
            return;
        case 0x30:
            gSaveState.unk_20.unk_24C4 = (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x31:
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0x10, 0);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x32:
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0, 0);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x33:
            func_ov030_020a7928(progress, (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                                (u16) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x34:
            func_ov030_020a7928(progress, (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)), 0x14U);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x35:
            if (func_ov030_020b0454() != 0) {
                progress->unk_21CE8 = 1U;
            }
            if (progress->unk_21CE4 == 0) {
                func_02026590(&progress->unk_21CE0, progress->unk_21CD8 << 0xC, progress->unk_21CE8);
                EasyFade_FadeSubDisplay(FADER_SMOOTH, (s32)progress->unk_21CE0 >> 0xC, 0);
            } else {
                if (progress->unk_21CE4 == 1) {
                    func_02026590(&progress->unk_21CDC, progress->unk_21CD8 << 0xC, progress->unk_21CE8);
                    EasyFade_FadeMainDisplay(FADER_SMOOTH, (s32)progress->unk_21CDC >> 0xC, 0);
                } else {
                    func_02026590(&progress->unk_21CDC, progress->unk_21CD8 << 0xC, progress->unk_21CE8);
                    EasyFade_FadeMainDisplay(FADER_SMOOTH, (s32)progress->unk_21CDC >> 0xC, 0);
                    func_02026590(&progress->unk_21CE0, progress->unk_21CD8 << 0xC, progress->unk_21CE8);
                    EasyFade_FadeSubDisplay(FADER_SMOOTH, (s32)progress->unk_21CE0 >> 0xC, 0);
                }
            }
            progress->unk_21CE8 = (u16)(progress->unk_21CE8 - 1);
            if (progress->unk_21CE8 != 0) {
                return;
            }
            Progress_AdvanceEventScript(progress);
            return;

        case 0x36: {
            u32 temp_r1_14 = *(u32*)func_ov030_020a6960(
                progress, (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)));
            if (temp_r1_14 != -1) {
                func_ov030_020b3cd4(EasyTask_GetTaskById(&progress->taskPool, temp_r1_14), 0);
            }
            func_ov030_02083940(progress, 0);
            func_ov030_020a7810(progress);
            Progress_AdvanceEventScript(progress);
        } break;

        case 0x37:
            func_ov030_020a7890(progress);
            gSaveState.unk_20.unk_24AC = gSaveState.unk_20.unk_249C;
            gSaveState.unk_20.unk_24B0 = gSaveState.unk_20.unk_24A0;
            gSaveState.unk_20.unk_24A0 = data_ov030_020d9586;
            gSaveState.unk_20.unk_249C = 0;
            return;
        case 0x38:
            gSaveState.unk_20.unk_249C = gSaveState.unk_20.unk_24AC;
            gSaveState.unk_20.unk_24A0 = gSaveState.unk_20.unk_24B0;
            u32 temp_r1_16 = *(u32*)func_ov030_020a6960(progress, gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_24AC]);
            if (temp_r1_16 != -1) {
                func_ov030_020b3cd4(EasyTask_GetTaskById(&progress->taskPool, temp_r1_16), 1);
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x39:
            if (progress->unk_21C48[progress->unk_21CD4] != -1) {
                func_ov030_020b411c(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[progress->unk_21CD4]));
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x3A:
            SndMgr_StartPlayingSE((enum SndMgrSeIdx) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x3B:
            SndMgr_PlaySEWithPan(*(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                 *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x3C:
            func_ov030_020b4570(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216B0),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x3D:
            Progress_AdvanceEventScript(progress);
            return;
        case 0x3E:
            func_ov030_020c2fc0(*(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x3F:
            CriSndMgr_Stop(ADX_TITLE);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x40:
            gSaveState.unk_20.unk_24A8 = 0;
            Progress_AdvanceEventScript(progress);
            /* fallthrough */
        case 0x41:
            gSaveState.unk_20.unk_24A8 = gSaveState.unk_20.unk_24A8 + 1;
            if (gSaveState.unk_20.unk_24A8 < (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2))) {
                if (progress->unk_21ADC == 0) {
                    return;
                }
                if (func_ov030_020b0454() == 0) {
                    return;
                }
                Progress_AdvanceEventScript(progress);
                return;
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x42:
            if (func_ov030_020a6ab4(progress) != 0) {
                return;
            }
            if (func_ov030_020a7550(progress) != 0) {
                return;
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x43:
            progress->unk_21758 = (u16)(progress->unk_21758 - 1);
            if (progress->unk_21748[progress->unk_21758] == 0) {
                progress->unk_21704 = func_ov030_020c717c(&progress->taskPool, progress->unk_21614, 24014);
            } else if (progress->unk_21748[progress->unk_21758] == 1) {
                progress->unk_21704 = func_ov030_020c717c(&progress->taskPool, progress->unk_21614, 24015);
            } else {
                progress->unk_21704 = func_ov030_020c717c(&progress->taskPool, progress->unk_21614, 24016);
            }

            progress->taskId_Mess =
                Fld_GetMess_CreateTask(&progress->taskPool, progress->unk_21614, progress->unk_21728[progress->unk_21758],
                                       progress->unk_21738[progress->unk_21758]);
            progress->taskId_Plate = Fld_TalkEventStringPlate_CreateTask(&progress->taskPool, progress->unk_21614, 0x88, 0x60,
                                                                         progress->unk_21748[progress->unk_21758],
                                                                         progress->unk_21718[progress->unk_21758], 0);

            if (progress->unk_21748[progress->unk_21758] == 0) {
                progress->taskId_Item  = Fld_WhichArrow_CreateTask(&progress->taskPool, progress->unk_21614, 0x60, 4, 1);
                progress->taskId_Arrow = Fld_WhichArrow_CreateTask(&progress->taskPool, progress->unk_21614, 0x60, 5, 1);
            } else if (progress->unk_21748[progress->unk_21758] == 1) {
                progress->taskId_Item  = Fld_WhichArrow_CreateTask(&progress->taskPool, progress->unk_21614, 0x60, 7, 1);
                progress->taskId_Arrow = Fld_WhichArrow_CreateTask(&progress->taskPool, progress->unk_21614, 0x60, 8, 1);
            } else {
                progress->taskId_Item  = Fld_WhichArrow_CreateTask(&progress->taskPool, progress->unk_21614, 0x60, 0xA, 1);
                progress->taskId_Arrow = Fld_WhichArrow_CreateTask(&progress->taskPool, progress->unk_21614, 0x60, 0xB, 1);
            }
            SndMgr_StartPlayingSE(SEIDX_SE_GET_KEYWORD);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x44:
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_21704);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Mess);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Plate);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Item);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Arrow);
            Progress_AdvanceEventScript(progress);
            return;

        case 0x45: {
            u16 sp22;
            u16 sp20;

            progress->unk_2176A--;

            s32 temp_r4_2          = func_ov030_020a775c(progress->unk_2175A[progress->unk_2176A], &sp22, &sp20);
            progress->unk_21704    = func_ov030_020c717c(&progress->taskPool, progress->unk_21614, sp22);
            progress->taskId_Mess  = Fld_GetMess_CreateTask(&progress->taskPool, progress->unk_21614, sp20, 0xFFFF);
            progress->taskId_Plate = Fld_GetBase_CreateTask(&progress->taskPool, progress->unk_21614, temp_r4_2);
            progress->taskId_Item =
                Fld_GetItem_CreateTask(&progress->taskPool, progress->unk_21614, progress->unk_2175A[progress->unk_2176A]);
            SndMgr_StartPlayingSE(SEIDX_SE_GET_ITEM);
            Progress_AdvanceEventScript(progress);
        } break;

        case 0x46:
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_21704);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Mess);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Plate);
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Item);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x47:

            if (func_ov030_020b38d4(EasyTask_GetTaskById(
                    &progress->taskPool,
                    *(u32*)func_ov030_020a6960(
                        progress, (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2))))) == 0)
            {
                return;
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x48:
            func_ov030_020b3948(EasyTask_GetTaskById(
                &progress->taskPool, *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x49:
            func_ov030_020b38f8(
                EasyTask_GetTaskById(&progress->taskPool,
                                     *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x4A:
            func_ov030_020b3934(
                EasyTask_GetTaskById(&progress->taskPool,
                                     *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x4B:
            func_ov030_020b3b64(
                EasyTask_GetTaskById(&progress->taskPool,
                                     *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x4C:
            func_ov030_020b3b70(
                EasyTask_GetTaskById(&progress->taskPool,
                                     *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 3) * 2)),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 4) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x4F:
            func_ov030_020a7284(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 1,
                                (s32) * (gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            return;
        case 0x4D:
            func_ov030_020a72fc(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)), 1);
            return;
        case 0x4E:
            func_ov030_020a72fc(progress, *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 1) * 2)),
                                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            return;
        case 0x50:
            func_ov030_020b3ca0(EasyTask_GetTaskById(
                &progress->taskPool, *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x28:
            if (func_ov030_020a69ac(&progress->taskPool,
                                    (u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                 ((gSaveState.unk_20.unk_249C + 1) * 2))),
                                    1) == 0)
            {
                return;
            }
            Progress_AdvanceEventScript(progress);
            func_ov030_020a7b50(progress);
            return;
        case 0x29:
            if (func_ov030_020a69ac(&progress->taskPool, (u32*)func_ov030_020a6960(progress, 0x118), 1) == 0) {
                return;
            }
            Progress_AdvanceEventScript(progress);
            func_ov030_020a7b50(progress);
            return;
        case 0x2A:
            if (func_ov030_020a69ac(&progress->taskPool,
                                    (u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                 ((gSaveState.unk_20.unk_249C + 1) * 2))),
                                    0) == 0)
            {
                return;
            }
            Progress_AdvanceEventScript(progress);
            func_ov030_020a7b50(progress);
            return;
        case 0x2B:
            func_ov030_020b37cc(
                EasyTask_GetTaskById(&progress->taskPool,
                                     *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)), 1);
            Progress_AdvanceEventScript(progress);
            return;
        case 0x2C:
            func_ov030_020b3b3c(
                EasyTask_GetTaskById(&progress->taskPool,
                                     *(u32*)func_ov030_020a6960(progress, (s32) * (gSaveState.unk_20.unk_24A0 +
                                                                                   ((gSaveState.unk_20.unk_249C + 1) * 2)))),
                *(gSaveState.unk_20.unk_24A0 + ((gSaveState.unk_20.unk_249C + 2) * 2)));
            Progress_AdvanceEventScript(progress);
            return;
        case 0x51:
            progress->unk_21D14 = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
            {
                DebugOvlDisp_Push(func_ov030_0208299c, progress, PROCESS_STAGE_INIT);
            }
            DebugOvlDisp_Push(func_ov030_02082b8c, progress, PROCESS_STAGE_INIT);
            DebugOvlDisp_Push(func_ov030_02082bf0, progress, PROCESS_STAGE_INIT);
            DebugOvlDisp_Push(func_ov030_02082c68, progress, PROCESS_STAGE_INIT);
            if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
            {
                DebugOvlDisp_Push(func_ov030_02082578, progress, PROCESS_STAGE_INIT);
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x52:
            if (progress->unk_21AC4 == 0) {
                if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                    (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
                {
                    DebugOvlDisp_Push(func_ov030_02082a84, progress, PROCESS_STAGE_INIT);
                }
                DebugOvlDisp_Push(func_ov030_02082d64, progress, PROCESS_STAGE_INIT);
                if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                    (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
                {
                    DebugOvlDisp_Push(func_ov030_02082578, progress, PROCESS_STAGE_INIT);
                }
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x53:
            if (func_ov030_020a75d8(progress, gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C]) != 0) {
                return;
            }
            Progress_AdvanceEventScript(progress);
            return;
        case 0x54:
            Progress_AdvanceEventScript(progress);
            return;
        case 0x5:
            progress->targetBrightness = 0x10;
            progress->unk_21D12        = 0x10;
            Progress_AdvanceEventScript(progress);
            return;
        case 0x7:
            progress->targetBrightness = 0x10;
            progress->unk_21D12        = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            Progress_AdvanceEventScript(progress);
            return;
        case 0x6:
            progress->targetBrightness = -0x10;
            progress->unk_21D12        = 0x10;
            Progress_AdvanceEventScript(progress);
            return;
        case 0x8:
            progress->targetBrightness = -0x10;
            progress->unk_21D12        = gSaveState.unk_20.unk_24A0[gSaveState.unk_20.unk_249C];
            Progress_AdvanceEventScript(progress);
            return;
        case 0x55:
            progress->unk_21AE0 = 0;
            SndMgr_StartPlayingSE(SEIDX_EV_SE_LOOP_STOP);
            if (func_ov030_020aa4bc(progress) != 0) {
                return;
            }
            if (progress->unk_21758 != 0) {
                progress->unk_21ADC = 0;
                Progress_SetCurrentEvent(1);
                return;
            }
            if (progress->unk_2176A != 0) {
                progress->unk_21ADC = 0;
                Progress_SetCurrentEvent(2);
                return;
            }
            if (gSaveState.unk_20.unk_24B8 != 0) {
                return;
            }
            Progress_SetCurrentEvent(3);
            return;

        case 0x56: {
            for (s32 i = 0; i < 35; i++) {
                func_ov030_020a69ac(&progress->taskPool, &progress->unk_21BBC[i], 1);
            }

            if ((progress->targetBrightness != g_DisplaySettings.controls[DISPLAY_MAIN].brightness) ||
                (progress->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness))
            {
                EasyFade_FadeBothDisplays(FADER_INSTANT, progress->targetBrightness, (s32)progress->unk_21D12);
                return;
            }

            if (func_ov030_020a75c0(progress) != 0) {
                return;
            }
            if (progress->unk_216C8 != -1) {
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_216C8);
                progress->unk_216C8 = -1;
            }
            if (progress->unk_21A24 != -1) {
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_21A24);
                progress->unk_21A24 = -1;
            }
            if (progress->unk_21AC4 == 0) {
                DebugOvlDisp_Push(func_ov030_02082d64, progress, PROCESS_STAGE_INIT);
                return;
            }
            if (progress->unk_21A5C != 1) {
                func_ov030_020846bc(progress, 1);
            }
            if (progress->unk_21CF4 != 0) {
                REG_POWER_CNT &= ~0x8000;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~2;
                func_ov030_020ad044(progress);
                func_ov030_020b458c(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216B0), 1);
                progress->unk_21CF4 = 0;
            }
            if (progress->unk_21CF8 != 0) {
                if (progress->unk_21CFC != 0) {
                    progress->unk_21CF8 = 0;
                } else {
                    if (progress->unk_21918 != 0) {
                        s32 var_r7_2 = 0;
                        if (progress->unk_21918 > 0) {
                            do {
                                EasyTask_DeleteTask(&progress->taskPool, progress->unk_21788[var_r7_2]);
                                var_r7_2 += 1;
                            } while (var_r7_2 < (s32)progress->unk_21918);
                        }
                        func_ov030_020c6300();
                        progress->unk_21918 = 0;
                        return;
                    }
                    func_ov030_020c6150(&progress->unk_21D30);
                    func_ov030_020ac95c(progress);
                    progress->unk_21CFC = 1;
                    return;
                }
            }
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
            g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~2;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 1;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x3F;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 0xA;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 6;
            func_ov030_020b4598(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216B0), 0);
            gSaveState.unk_20.unk_2498 = 0;
            SndMgr_StartPlayingSE(SEIDX_EV_SE_LOOP_STOP);
            func_ov030_020c2de8();
            func_ov030_020c2d50(gSaveState.unk_20.unk_2548[gSaveState.unk_20.unk_2458]);
            func_ov030_020ad428(progress);
            if (progress->unk_21D0C != 0) {
                func_ov030_020c2e28();
            }
            EasyFade_FadeBothDisplays(FADER_INSTANT, 0, (s32)progress->unk_21D12);
        } break;
    }
}

// Nonmatching
void Progress_BeginScriptedEvent(ProgressObject* progress, ProgressEvent event, s32 arg2) {
    if (gSaveState.unk_20.unk_2498 != FALSE) {
        return;
    }

    Progress_InitState(progress);
    gSaveState.unk_20.unk_2498 = TRUE;

    if (event >= 0x8000) {
        event -= 0x8000;
    }

    gSaveState.unk_20.unk_249C = 0;
    gSaveState.unk_20.unk_24A4 = arg2;
    progress->unk_21D0C        = FALSE;
    progress->unk_21CF4        = FALSE;

    if (event >= 0x4000) {
        progress->unk_21D0C = TRUE;
        event -= 0x4000;
    } else if (event >= 0x2000) {
        progress->unk_21CF4 = TRUE;
        event -= 0x2000;
    }

    Progress_SetCurrentEvent(event);
    func_ov030_020b3034(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216D4));
}

s32 func_ov030_020a9d00(ProgressObject* progress, ProgressEvent event) {
    s32  var_r8 = 0;
    u32* var_r9 = progress->unk_21BBC;
    do {
        func_ov030_020a69ac(&progress->taskPool, var_r9, 1);
        var_r8 += 1;
        var_r9 += 1;
    } while (var_r8 < 0x23);

    EasyFade_FadeBothDisplays(FADER_INSTANT, progress->targetBrightness, progress->unk_21D12);
    if (EasyFade_IsFading() != 0) {
        return 0;
    }

    for (s32 var_r2 = 0; var_r2 < 35; var_r2++) {
        if (progress->unk_21BBC[var_r2] != -1) {
            return 0;
        }
    }

    func_ov030_020a6a1c(progress);
    Progress_SetCurrentEvent(event);
    Progress_AdvanceEventScript(progress);
    return 1;
}

void func_ov030_020a9db8(ProgressObject* progress, ProgressEvent event) {
    if (func_ov030_020a9d00(progress, event) == 0) {
        return;
    }
    if (progress->unk_21D22 == 3) {
        DebugOvlDisp_Push(func_ov030_02082a04, progress, PROCESS_STAGE_INIT);
    } else if (progress->unk_21D18 != 3) {
        DebugOvlDisp_Push(func_ov030_02082b4c, progress, PROCESS_STAGE_INIT);
    }
    DebugOvlDisp_Push(func_ov030_02082d64, progress, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push(func_ov030_02082534, progress, PROCESS_STAGE_INIT);
}

void func_ov030_020a9e48(ProgressObject* progress) {
    Task* task    = EasyTask_GetTaskById(&progress->taskPool, func_ov030_020a6960(progress, progress->unk_21D00));
    s32   temp_r0 = func_ov030_020b3cc8(task);

    progress->unk_216BC = func_ov030_020b44b8(&progress->taskPool, progress->unk_21614, temp_r0, 0x78, 0, 1);
    SndMgr_PlaySEWithPan(0x122, temp_r0);
    func_ov030_020b4570(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216BC), 2);
    progress->unk_216C4 = func_ov030_020c3654(&progress->taskPool, progress->unk_21614, temp_r0, 0x78);
    progress->unk_21D2A = -1;
    func_ov030_020a6d20(progress, 0x44);
    if (progress->taskId_KeywordMenu != -1) {
        EasyTask_DeleteTask(&progress->taskPool, progress->taskId_KeywordMenu);
        progress->taskId_KeywordMenu = -1;
    }
    func_ov030_020a7890(progress);
}

// Nonmatching
s32 func_ov030_020a9f54(ProgressObject* progress, s32 (*callback)(void*), s32 arg2) {
    if (progress->taskId_Choice == -1) {
        func_ov030_020a6a1c(progress);

        u16 count;
        if (count > 8) {
            count = 8;
        }

        void* dataPtr = ((s32)&count & ~3) + 4;
        for (s32 index = 0; index < count; index++) {
            dataPtr += 4;
            ProgressObject* itemPtr   = progress + (index * 2);
            itemPtr->unk_2176C[index] = *(u16*)dataPtr - 4;
        }

        progress->unk_21784 = func_ov030_020b3fbc(&progress->taskPool, &progress->unk_21614, 0x80, 0x60, arg2, 2, 0, 0, 0, 0);
        progress->taskId_Choice =
            Fld_Choice_CreateTask(&progress->taskPool, progress->unk_21614, (s16)count, progress->unk_2176C);
    } else {
        progress->unk_2177C =
            func_ov030_020c83e8(&progress->taskPool, EasyTask_GetTaskById(&progress->taskPool, progress->taskId_Choice));
        if (progress->unk_2177C >= 0) {
            EasyTask_DeleteTask(&progress->taskPool, progress->taskId_Choice);
            EasyTask_DeleteTask(&progress->taskPool, progress->unk_21784);
            progress->taskId_Choice = -1;
            callback(progress);
        }
    }
    return 1;
}

s32 func_ov030_020aa0c8(ProgressObject* progress, s32 arg1, s16 arg2, s16 arg3, void (*arg4)(void*)) {
    s32 temp_r4;

    progress->unk_21D00 = arg1;
    progress->unk_21D04 = arg2;
    progress->unk_21D06 = arg3;
    progress->unk_21D08 = arg4;
    if ((progress->unk_21AD8 == 0) && (progress->unk_216CC == -1)) {
        Task* task = EasyTask_GetTaskById(&progress->taskPool, *(u32*)func_ov030_020a6960(progress, progress->unk_21D00));
        progress->unk_216CC = func_ov030_020c4810(&progress->taskPool, progress->unk_21614, func_ov030_020b3cc8(task), 0x78);
    } else if ((progress->unk_21AD8 == 0) && (EasyTask_ValidateTaskId(&progress->taskPool, &progress->unk_216CC) == 0)) {
        progress->unk_21AD8 = 1;
        progress->unk_216CC = -1;
        func_ov030_020a9e48(progress);
    } else {
        if ((progress->unk_21AD8 != 0) && (progress->unk_216BC == -1U)) {
            func_ov030_020a9e48(progress);
        } else if ((progress->unk_21AD8 != 0) && (func_ov030_020a722c(progress) != 0)) {
            func_ov030_020b3cd4(
                EasyTask_GetTaskById(&progress->taskPool, *(u32*)func_ov030_020a6960(progress, progress->unk_21D00)), 1);

            if ((TouchInput_WasTouchPressed() != 0) &&
                (func_ov030_020b4514(EasyTask_GetTaskById(&progress->taskPool, progress->unk_216BC)) != 0))
            {
                SndMgr_StartPlayingSE(SEIDX_SE_SCANS_OPEN);
                Task* temp_r0 =
                    EasyTask_GetTaskById(&progress->taskPool, *(u32*)func_ov030_020a6960(progress, progress->unk_21D00));
                func_ov030_020b3cd4(temp_r0, 0);
                temp_r4 = func_ov030_020b3cc8(temp_r0);
                func_ov030_020b400c(EasyTask_GetTaskById(&progress->taskPool, progress->unk_21C48[progress->unk_21CD4]), 2);
                if ((progress->unk_21D08 != 0) && (func_ov030_020b0418() != 0)) {
                    progress->taskId_KeywordMenu =
                        Fld_TalkEventKeywordMenu_CreateTask(&progress->taskPool, progress->unk_21614, temp_r4);
                }
                func_ov030_02083940(progress, 1);
                func_ov030_020a7810(progress);
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_216BC);
                progress->unk_216BC = -1;
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_216C4);
                Progress_SetCurrentEvent(progress->unk_21D04);
                Progress_AdvanceEventScript(progress);
            } else {
                SndMgr_StartPlayingSE(SEIDX_SE_DIALOG_NEXT);
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_216BC);
                progress->unk_216BC = -1;
                EasyTask_DeleteTask(&progress->taskPool, progress->unk_216C4);
                progress->unk_21AD8 = 0;
                Progress_SetCurrentEvent(progress->unk_21D06);
                Progress_AdvanceEventScript(progress);
            }
        }
    }
    return 1;
}

s32 func_ov030_020aa390(ProgressObject* progress) {
    switch (progress->unk_21D1C) {
        case 0:
            Progress_SetCurrentEvent(6);
            return 1;
        case 1:
            Progress_SetCurrentEvent(7);
            return 1;
        case 2:
            Progress_SetCurrentEvent(8);
            return 1;
        case 3:
            Progress_SetCurrentEvent(9);
            return 1;
        case 4:
            Progress_SetCurrentEvent(10);
            return 1;
        case 5:
            Progress_SetCurrentEvent(11);
            return 1;
        case 6:
            Progress_SetCurrentEvent(12);
            return 1;
        case 7:
            Progress_SetCurrentEvent(13);
            return 1;
        case 8:
            Progress_SetCurrentEvent(14);
            return 1;
        case 9:
            func_ov030_020a9db8(progress, 0x32);
            return 0;
        case 10:
            func_ov030_020a9db8(progress, 0x33);
            return 0;
        case 11:
            func_ov030_020a9db8(progress, 0x34);
            return 0;
        case 13:
            func_ov030_020a9db8(progress, 0x29);
            return 0;
        default:
        case 12:
            return 0;
    }
}

s32 func_ov030_020aa4bc(ProgressObject* progress) {
    if (gSaveState.unk_20.unk_24B8 != 0) {
        return 0;
    }
    if (gSaveState.unk_20.unk_1AB0 >= 44) {
        gSaveState.unk_20.unk_1AB0 = 0;
    }
    if (data_ov030_020da618[gSaveState.unk_20.unk_1AB0](progress) != 0) {
        return 1;
    }
    if (func_ov030_020ab3b8(gSaveState.unk_20.currentStoryEvent) != 0) {
        s16 temp_r0_2                       = func_ov030_020ab420(gSaveState.unk_20.currentStoryEvent);
        gSaveState.unk_20.unk_24B4          = 0;
        gSaveState.unk_20.unk_24B8          = 1;
        gSaveState.unk_20.currentStoryEvent = temp_r0_2;
        func_ov030_020af364(-2);
        return 1;
    }
    if (func_ov030_020ab3ec(gSaveState.unk_20.currentStoryEvent) != 0) {
        ProgressReward_GrantPin(progress, 0xFBU);
        return 0;
    }

    if (gSaveState.unk_20.currentStoryEvent == 0x30A) {
        gSaveState.unk_20.unk_3124 = 0x30;
        gSaveState.unk_20.unk_3128 = 0;
        progress->unk_21630        = 4;
        DebugOvlDisp_Pop();
        return 0;
    } else if (gSaveState.unk_20.currentStoryEvent == 0x36) {
        if (func_ov030_020a9d00(progress, 5) == 0) {
            return 1;
        }
        progress->unk_21D22 = 3;
        progress->unk_21D24 = func_ov030_020aa390;
        progress->unk_21D28 = 0x5DCC;
        progress->unk_21D18 = 0;
        DebugOvlDisp_Push(func_ov030_020824a0, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082c04, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082bf0, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082c68, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082534, progress, PROCESS_STAGE_INIT);
        return 1;
    } else if (gSaveState.unk_20.currentStoryEvent == 0x34) {
        if (func_ov030_020a9d00(progress, 5) == 0) {
            return 1;
        }
        progress->unk_21D1E = 0x83;
        progress->unk_21D20 = 0x83;
        progress->unk_21D22 = 1;
        progress->unk_21D24 = NULL;

        if (progress->unk_21D22 == 1) {
            func_ov030_020aec1c(3);
            func_ov030_020aec1c(2);
        } else if (progress->unk_21D22 == 2) {
            func_ov030_020aec38(3);
            func_ov030_020aec1c(2);
        } else if (progress->unk_21D22 == 4) {
            func_ov030_020aec1c(3);
            func_ov030_020aec38(2);
        } else if (progress->unk_21D22 == 5) {
            func_ov030_020aec38(3);
            func_ov030_020aec38(2);
        }

        progress->unk_21D18 = 0;
        DebugOvlDisp_Push(func_ov030_020824a0, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082c04, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082bf0, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082c68, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082534, progress, PROCESS_STAGE_INIT);
        return 1;
    } else if (gSaveState.unk_20.currentStoryEvent == 0x35) {
        if (func_ov030_020a9d00(progress, 5) == 0) {
            return 1;
        }
        progress->unk_21D1E = 0x83;
        progress->unk_21D20 = 0x83;
        progress->unk_21D22 = 5;
        progress->unk_21D24 = NULL;

        if (progress->unk_21D22 == 1) {
            func_ov030_020aec1c(3);
            func_ov030_020aec1c(2);
        } else if (progress->unk_21D22 == 2) {
            func_ov030_020aec38(3);
            func_ov030_020aec1c(2);
        } else if (progress->unk_21D22 == 4) {
            func_ov030_020aec1c(3);
            func_ov030_020aec38(2);
        } else if (progress->unk_21D22 == 5) {
            func_ov030_020aec38(3);
            func_ov030_020aec38(2);
        }

        progress->unk_21D18 = 0;
        DebugOvlDisp_Push(func_ov030_020824a0, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082c04, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082bf0, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082c68, progress, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push(func_ov030_02082534, progress, PROCESS_STAGE_INIT);
        return 1;
    } else {
        if (func_ov030_020a66e8(progress) != 0) {
            if (progress->unk_21D18 == 0) {
                return 1;
            } else if (progress->unk_21D18 == 1) {
                func_ov030_020a9db8(progress, progress->unk_21D1E);
                return 1;
            } else if (progress->unk_21D18 == 2) {
                func_ov030_020a9db8(progress, progress->unk_21D20);
                return 1;
            } else if (progress->unk_21D18 == 4) {
                Progress_SetCurrentEvent((s32)progress->unk_21D1C);
                Progress_AdvanceEventScript(progress);
                progress->unk_21D18 = 0;
                return 1;
            } else if (progress->unk_21D18 == 5) {
                if (progress->unk_21D24 != NULL) {
                    if (progress->unk_21D24(progress) != 0) {
                        Progress_AdvanceEventScript(progress);
                        progress->unk_21D18 = 0;
                    }
                    return 1;
                }
                func_ov030_020a9db8(progress, 4);
                return 1;
            } else if (progress->unk_21D18 == 3) {
                func_ov030_020a9db8(progress, 4);
                return 1;
            }
        }
        return 0;
    }
}

// Nonmatching
void func_ov030_020aa9f4(ProgressObject* progress, s16 arg1, s16 arg2, s16 arg3) {
    for (s32 i = 0; i < 0x10; i++) {
        if (arg1 == gSaveState.unk_20.unk_24C8[i]) {
            return;
        }
        if (gSaveState.unk_20.unk_24C8[i] == -1) {
            gSaveState.unk_20.unk_24C8[i]        = arg1;
            gSaveState.unk_20.unk_24C8[i + 0x10] = arg3;
            if (progress->unk_21758 >= 8) {
                return;
            }
            progress->unk_21718[progress->unk_21758] = arg1;
            progress->unk_21728[progress->unk_21758] = arg2;
            if (arg3 == 0) {
                progress->unk_21738[progress->unk_21758] = arg1;
            } else {
                progress->unk_21738[progress->unk_21758] = -2;
            }
            progress->unk_21748[progress->unk_21758] = arg3;
            progress->unk_21758++;
            return;
        }
    }
}

u8 func_ov030_020aaad8(void) {
    return gSaveState.unk_20.unk_16EA[func_ov030_020af350() * 0x28];
}

s32 func_ov030_020aaaf8(u16 itemID, s32 arg1) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) == FALSE) {
        return 1;
    }

    s32 temp_r0 = func_02023be8(itemID, arg1);
    if (temp_r0 == 0) {
        return 0;
    }
    if (temp_r0 != 10000000) {
        return 1;
    }
    if (Inventory_HasRequiredQuantity(itemID, 1, 0) == FALSE) {
        return 1;
    }
    return 0;
}

void ProgressReward_GrantItem(ProgressObject* progress, u16 itemID) {
    if (func_ov030_020aaaf8(itemID, 0) && Inventory_AddItem(itemID, 0)) {
        progress->unk_2175A[progress->unk_2176A] = itemID;
        progress->unk_2176A++;
    }
}

void ProgressReward_GrantPin(ProgressObject* progress, u16 itemID) {
    if (func_ov030_020aaaf8(itemID, 2) && Inventory_AddItem(itemID, 2)) {
        progress->unk_2175A[progress->unk_2176A] = itemID;
        progress->unk_2176A++;
    }
}

void func_ov030_020aac28(ProgressObject* progress, u16 itemID) {
    if (func_ov030_020aaaf8(itemID, 1) && Inventory_AddItem(itemID, 1)) {
        progress->unk_2175A[progress->unk_2176A] = itemID;
        progress->unk_2176A++;
    }
}
