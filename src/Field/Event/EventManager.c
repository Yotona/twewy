#include "Field/FieldRegion.h"

#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Event/EventFlags.h"
#include "Field/Event/EventManager.h"
#include "Field/Event/EventTables.h"
#include "Field/Event/Events.h"
#include "Field/Scene/FieldAmbientSound.h"
#include "Field/Scene/FieldBg.h"
#include "Field/Scene/FieldMain.h"
#include "Field/Scene/FieldMusic.h"
#include "Field/Scene/FieldPaths.h"
#include "Field/Tasks/FldMesWindow.h"
#include "Field/Tasks/FldSpriteFrame.h"
#include "Field/Tasks/Fld_Badge.h"
#include "Field/Tasks/Fld_BustShot.h"
#include "Field/Tasks/Fld_Choice.h"
#include "Field/Tasks/Fld_GetBase.h"
#include "Field/Tasks/Fld_GetItem.h"
#include "Field/Tasks/Fld_GetMess.h"
#include "Field/Tasks/Fld_GetPlate.h"
#include "Field/Tasks/Fld_Player.h"
#include "Field/Tasks/Fld_TalkEventDecide.h"
#include "Field/Tasks/Fld_TalkEventKeywordMenu.h"
#include "Field/Tasks/Fld_TalkEventStringPlate.h"
#include "Field/Tasks/Fld_TalkEventTitle.h"
#include "Field/Tasks/Fld_TalkEventTouch.h"
#include "Field/Tasks/Fld_TouchNext.h"
#include "Field/Tasks/Fld_UGFade.h"
#include "Field/Tasks/Fld_WhichArrow.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "Util/SysFont.h"
#include "nitro/reg.h"
#include <nitro/types.h>
#include <stdarg.h>
extern s32 data_ov030_020da264;

extern s32  func_02023be8(u16 itemID, s32 arg1);
extern void func_02026590(void*, s32, s32);
extern s32  func_ov030_020aa4bc(FieldEventManager*);

// Nonmatching
void FldEvent_SetCurrent(FieldEvent event) {
    s32 var_lr                  = (event >= 2207) ? event - 2207 : event;
    gSaveData.unk_249C          = 0;
    gSaveData.currentStoryEvent = event;
    gSaveData.unk_24A0          = EventScripts[var_lr];
}

s32 func_ov030_020a66e8(FieldEventManager* object) {
    if (gSaveData.unk_2488 == 0) {
        if ((gSaveData.currentStoryEvent >= 5) && (gSaveData.currentStoryEvent <= 39)) {
            return 1;
        }
    }
    if (object->touchEventKind != 0) {
        return 1;
    }
    return 0;
}

// Nonmatching
void FldEvent_AdvanceScript(FieldEventManager* eventMgr) {
    gSaveData.unk_249C += data_ov030_020dbdd8[gSaveData.unk_24A0[gSaveData.unk_249C]];
    if (eventMgr->unk_21ADC != 0 && func_ov030_020a66e8(eventMgr) == 0 && eventMgr->unk_21AE0 != 0) {
        while (TRUE) {
            switch (gSaveData.unk_24A0[gSaveData.unk_249C]) {
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
                    gSaveData.unk_249C += data_ov030_020dbdd8[gSaveData.unk_249C];
            }
        }
    }
}

s32 func_ov030_020a68a8(FieldEventManager* eventMgr, s32 arg1) {
    if (arg1 == -1) {
        s32 prevVal = eventMgr->unk_22E2C;
        arg1        = eventMgr->unk_22E2C % 280;
        eventMgr->unk_22E2C++;
        eventMgr->unk_22E2C = eventMgr->unk_22E2C & 0xFFFFFFFFFFFFFFFF;
    }
    return arg1;
}

// Not yet implemented
s32 func_ov030_020a68f4(s32 progress) {}

u32 func_ov030_020a6960(FieldEventManager* eventMgr, s32 arg1) {
    if (arg1 == 0x118) {
        if (eventMgr->unk_21CD0 == 0) {
            eventMgr->unk_21CD0 = eventMgr->unk_21BBC;
        }
    } else {
        eventMgr->unk_21CD0 = &eventMgr->unk_21BBC[func_ov030_020a68f4(arg1)];
    }
}

s32 func_ov030_020a69ac(TaskPool* taskPool, u32* arg1, s16 arg2) {
    if (*arg1 != -1) {
        Fld_BustShot_SlideOut(EasyTask_GetTaskById(taskPool, *arg1), arg2, 0);
        if (Fld_BustShot_IsSlideOutDone(EasyTask_GetTaskById(taskPool, *arg1)) == 0) {
            return 0;
        }
        EasyTask_DeleteTask(taskPool, *arg1);
        *arg1 = -1;
        return 1;
    }
    return 1;
}

void func_ov030_020a6a1c(FieldEventManager* eventMgr) {
    for (s32 i = 0; i < 32; i++) {
        if (eventMgr->unk_21C48[i] != -1) {
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21C48[i]);
            eventMgr->unk_21C48[i] = -1;
        }
    }
}

void func_ov030_020a6a68(FieldEventManager* eventMgr) {
    for (s32 i = 16; i < 32; i++) {
        if (eventMgr->unk_21C48[i] != -1) {
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21C48[i]);
            eventMgr->unk_21C48[i] = -1;
        }
    }
}

BOOL func_ov030_020a6ab4(FieldEventManager* eventMgr) {
    s32  i;
    BOOL found = FALSE;

    for (i = 0; i < 32; i++) {
        if ((eventMgr->unk_21C48[i] != -1) && (EasyTask_ValidateTaskId(&eventMgr->taskPool, &eventMgr->unk_21C48[i]))) {
            if (FieldEvent_IsDebugSkipHeld() != 0) {
                if (eventMgr->unk_21CD4 == i) {
                    FldMesWindow_SkipAnim(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[i]));
                } else {
                    EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21C48[i]);
                    eventMgr->unk_21C48[i] = -1;
                }
            }
            found = TRUE;
        }
    }
    return found;
}

void func_ov030_020a6b6c(FieldEventManager* eventMgr, s16 arg1, s32 arg2, s32 arg3) {
    for (s32 i = arg2; i < arg3; i++) {
        if ((eventMgr->unk_21C48[i] != -1) && (eventMgr->unk_21CD4 != i)) {
            if (FieldEvent_IsDebugSkipHeld() != 0) {
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21C48[i]);
                eventMgr->unk_21C48[i] = -1;
            } else {
                FldMesWindow_ScrollUp(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[i]), arg1);
                FldMesWindow_SetPriority(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[i]), 2);
            }
        }
    }
}

void func_ov030_020a6c24(FieldEventManager* eventMgr, s16 arg1) {
    func_ov030_020a6b6c(eventMgr, arg1, 0, 0x10);
}

void func_ov030_020a6c38(FieldEventManager* eventMgr, s16 arg1) {
    func_ov030_020a6b6c(eventMgr, arg1, 0x10, 0x20);
}

void func_ov030_020a6c4c(FieldEventManager* eventMgr, s16 arg1, s32 arg2, s32 arg3) {
    for (s32 i = arg2; i < arg3; i++) {
        if (eventMgr->unk_21C48[i] != -1) {
            if (FieldEvent_IsDebugSkipHeld() != 0) {
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21C48[i]);
                eventMgr->unk_21C48[i] = -1;
            } else {
                FldMesWindow_ScrollUp(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[i]), arg1);
                FldMesWindow_SetPriority(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[i]), 2);
            }
        }
    }
}

void func_ov030_020a6cf8(FieldEventManager* eventMgr, s16 arg1) {
    func_ov030_020a6c4c(eventMgr, arg1, 0, 0x20);
}

void func_ov030_020a6d0c(FieldEventManager* eventMgr, s16 arg1) {
    func_ov030_020a6c4c(eventMgr, arg1, 0x10, 0x20);
}

void func_ov030_020a6d20(FieldEventManager* eventMgr, s16 arg1) {
    func_ov030_020a6c4c(eventMgr, arg1, 0, 0x10);
}

// Nonmatching
s32 func_ov030_020a6d34(FieldEventManager* eventMgr, s32 arg1, s32 arg2) {
    for (s32 i = arg1; i < arg2; i++) {
        if (eventMgr->unk_21C48[i] == -1) {
            eventMgr->unk_21CD4 = i;
            return eventMgr->unk_21C48[i];
        }
    }
    return 0;
}

s32 func_ov030_020a6d80(FieldEventManager* eventMgr) {
    return func_ov030_020a6d34(eventMgr, 0, 0x10);
}

s32 func_ov030_020a6d94(FieldEventManager* eventMgr) {
    return func_ov030_020a6d34(eventMgr, 0x10, 0x20);
}

void FldEvent_InitState(FieldEventManager* eventMgr) {
    for (s32 i = 0; i < 35; i++) {
        eventMgr->unk_21BBC[i] = -1;
    }

    eventMgr->unk_21CD0 = 0;
    for (s32 i = 0; i < 32; i++) {
        eventMgr->unk_21C48[i] = -1;
    }

    eventMgr->unk_21CEA      = 0;
    eventMgr->unk_21CD4      = -1;
    eventMgr->unk_21D00      = 280;
    eventMgr->unk_21CEC      = 0;
    eventMgr->unk_21CF0      = 1;
    eventMgr->unk_21AD8      = 0;
    eventMgr->unk_21ADC      = 1;
    eventMgr->unk_21AE0      = 0;
    eventMgr->unk_21CF8      = 0;
    eventMgr->unk_21CFC      = 0;
    eventMgr->unk_21758      = 0;
    eventMgr->unk_2176A      = 0;
    eventMgr->touchEventKind = 0;
}

s32 func_ov030_020a6e50(void) {
    return g_DisplaySettings.controls[DISPLAY_SUB].layers & 8;
}

// Nonmatching
void func_ov030_020a6e64(FieldEventManager* eventMgr, u32 arg1, Task* arg2) {
    if (func_ov030_020a6e50() != 0 && func_ov030_020a6960(eventMgr, eventMgr->unk_21D00) == arg1) {
        Fld_BustShot_SetPriority(arg2, 0);
    }
}

void func_ov030_020a6eac(FieldEventManager* eventMgr, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    s32  resourceId = func_ov030_020a68a8(eventMgr, arg1);
    u32* taskIdPtr  = (u32*)func_ov030_020a6960(eventMgr, resourceId);
    u32  taskId     = *taskIdPtr;

    if (taskId != -1) {
        Task* task = EasyTask_GetTaskById(&eventMgr->taskPool, taskId);
        Fld_BustShot_ChangePose(&eventMgr->taskPool, task, arg1, 1);
        Fld_BustShot_ClearPoseChanged(task);
        Fld_BustShot_MoveTo(task, arg5, arg6, arg2);
        func_ov030_020a6e64(eventMgr, *taskIdPtr, task);
    } else {
        *taskIdPtr =
            Fld_BustShot_CreateTask(&eventMgr->taskPool, eventMgr->dataType, resourceId, arg2, arg3, arg4, arg5, arg6);
    }
}

void func_ov030_020a6f90(FieldEventManager* eventMgr) {
    u32*  temp_r4 = (u32*)func_ov030_020a6d94(eventMgr);
    u32*  temp_r5 = (u32*)func_ov030_020a6960(eventMgr, eventMgr->unk_21D00);
    Task* task    = EasyTask_GetTaskById(&eventMgr->taskPool, *temp_r5);
    s32   val     = Fld_BustShot_GetScreenX(task);
    *temp_r4      = FldMesWindow_Create(&eventMgr->taskPool, eventMgr->dataType, val, 0xD0, val, 0xD0, 0, 0x16, 8, 0, 0, 0);
}

s32 func_ov030_020a701c(FieldEventManager* eventMgr) {
    s32 temp_r0 = Fld_TalkEventKeywordMenu_GetTouchedKeyword(
        &eventMgr->taskPool, EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_KeywordMenu));

    if (temp_r0 >= 0) {
        if (temp_r0 != eventMgr->unk_21D2A) {
            eventMgr->unk_21D2A = (s16)temp_r0;
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);

            Task* task = EasyTask_GetTaskById(&eventMgr->taskPool, func_ov030_020a6960(eventMgr, eventMgr->unk_21D00));
            Fld_BustShot_SetPriority(task, 1);

            s16 temp_r4        = Fld_BustShot_GetScreenX(task);
            gSaveData.unk_24C4 = eventMgr->unk_21D2A;
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_KeywordMenu);
            eventMgr->taskId_KeywordMenu = -1;
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = 0;
            eventMgr->unk_216C0 = Fld_TalkEventDecide_CreateTask(&eventMgr->taskPool, eventMgr->dataType, temp_r4, 0x78);
            eventMgr->unk_216C4 = Fld_Badge_CreateTask(&eventMgr->taskPool, eventMgr->dataType, temp_r4, 0x78, 0, 1);
            Fld_Badge_SetAnimation(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_216C4), 2);
            func_ov030_020a6f90(eventMgr);
            func_ov030_020a6d0c(eventMgr, 0x44);
        }
        return 0;
    }
    if (temp_r0 != -2) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020a71a8(FieldEventManager* eventMgr) {
    if (eventMgr->unk_216C0 != -1) {
        if (EasyTask_ValidateTaskId(&eventMgr->taskPool, &eventMgr->unk_216C0) == 0) {
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_216C4);
            eventMgr->unk_216C4 = -1;
            eventMgr->unk_21D08(eventMgr);
        }
        return 0;
    }
    if (eventMgr->taskId_KeywordMenu == -1) {
        return 1;
    }
    return func_ov030_020a701c(eventMgr);
}

s32 func_ov030_020a722c(FieldEventManager* eventMgr) {
    if (func_ov030_020a71a8(eventMgr) == 0) {
        return 0;
    }
    if ((TouchInput_WasTouchPressed() != 0) ||
        (SysControl.buttonState.pressedButtons &
         (INPUT_BUTTON_X | INPUT_BUTTON_Y | INPUT_BUTTON_UP | INPUT_BUTTON_DOWN | INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT |
          INPUT_BUTTON_START | INPUT_BUTTON_B | INPUT_BUTTON_A)))
    {
        return 1;
    }
    if (FieldEvent_IsDebugSkipHeld() != 0) {
        return 1;
    }
    return 0;
}

void func_ov030_020a7284(FieldEventManager* eventMgr, s16 arg1, s32 arg2, s32 arg3) {
    Task* task;
    u32*  temp_r0;

    temp_r0 = (u32*)func_ov030_020a6960(eventMgr, arg1);
    task    = EasyTask_GetTaskById(&eventMgr->taskPool, *temp_r0);
    if (Fld_BustShot_IsCrossFadeDone(task) != 0) {
        FldEvent_AdvanceScript(eventMgr);
    } else {
        Fld_BustShot_CrossFadeTo(&eventMgr->taskPool, task, arg1, arg2, arg3);
    }
    func_ov030_020a6e64(eventMgr, *temp_r0, task);
}

void func_ov030_020a72fc(FieldEventManager* eventMgr, s16 arg1, s16 arg2) {
    Task* temp_r0_2;
    u32*  temp_r0;

    temp_r0   = (u32*)func_ov030_020a6960(eventMgr, arg1);
    temp_r0_2 = EasyTask_GetTaskById(&eventMgr->taskPool, *temp_r0);
    if (temp_r0_2 == NULL) {
        FldEvent_AdvanceScript(eventMgr);
        return;
    }
    if (Fld_BustShot_ConsumePoseChanged(temp_r0_2) != 0) {
        FldEvent_AdvanceScript(eventMgr);
    } else {
        Fld_BustShot_ChangePose(&eventMgr->taskPool, temp_r0_2, arg1, arg2);
    }
    func_ov030_020a6e64(eventMgr, *temp_r0, temp_r0_2);
}

void func_ov030_020a737c(FieldEventManager* eventMgr) {
    if (eventMgr->unk_21A5C != 2) {
        if (eventMgr->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness) {
            EasyFade_FadeBothDisplays(FADER_INSTANT, eventMgr->targetBrightness, (s32)eventMgr->unk_21D12);
            return;
        }
        REG_POWER_CNT |= 0x8000;
        FieldBg_SetTopBgMode(eventMgr, 2);
        FieldBg_LoadScanPicture(eventMgr);
        g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1;
        Field_DestroyHud(eventMgr);
        Fld_Badge_SetVisible(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_Badge), 0);
        FldEvent_AdvanceScript(eventMgr);
        EasyFade_FadeBothDisplays(FADER_INSTANT, 0, eventMgr->unk_21D12);
        eventMgr->unk_216C8 = Fld_TalkEventTitle_CreateTask(&eventMgr->taskPool, eventMgr->dataType);
        return;
    }
    FldEvent_AdvanceScript(eventMgr);
}

void func_ov030_020a7468(FieldEventManager* eventMgr) {
    EasyFade_FadeMainDisplay(FADER_INSTANT, 0, eventMgr->unk_21D12);
    if (eventMgr->taskId_UGFade == -1) {
        g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 0;
        g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 0x10;
        eventMgr->taskId_UGFade =
            Fld_UGFade_CreateTask(&eventMgr->taskPool, (u16)eventMgr->unk_21D12, 0, &eventMgr->taskId_UGFade);
    }
    if (eventMgr->unk_21A5C != 2) {
        if (eventMgr->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness) {
            EasyFade_FadeSubDisplay(FADER_INSTANT, eventMgr->targetBrightness, eventMgr->unk_21D12);
            return;
        }
        FieldBg_SetTopBgMode(eventMgr, 2);
        FldEvent_AdvanceScript(eventMgr);
        EasyFade_FadeSubDisplay(FADER_INSTANT, 0, eventMgr->unk_21D12);
        return;
    }
    FldEvent_AdvanceScript(eventMgr);
}

s32 func_ov030_020a7550(FieldEventManager* eventMgr) {
    s32  var_r7 = 0;
    u32* var_r8 = eventMgr->unk_21BBC;
    do {
        func_ov030_020a69ac(&eventMgr->taskPool, var_r8, 1);
        var_r7 += 1;
        var_r8 += 1;
    } while (var_r7 < 35);

    for (s32 i = 0; i < 35; i++) {
        if (eventMgr->unk_21BBC[i] != -1) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020a75c0(FieldEventManager* eventMgr) {
    func_ov030_020a6a1c(eventMgr);
    func_ov030_020a7550(eventMgr);
}

s32 func_ov030_020a75d8(FieldEventManager* eventMgr, s16 arg1) {
    if (eventMgr->unk_21A5C != 3) {
        if (eventMgr->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness) {
            EasyFade_FadeSubDisplay(FADER_INSTANT, eventMgr->targetBrightness, eventMgr->unk_21D12);
            if (eventMgr->unk_21AC4 != 0) {
                EasyFade_FadeMainDisplay(FADER_INSTANT, eventMgr->targetBrightness, eventMgr->unk_21D12);
            }
        } else {
            func_ov030_020a6a1c(eventMgr);
            if (func_ov030_020a7550(eventMgr) != 0) {
                return 1;
            }
            if (arg1 == 0x49) {
                arg1 = gSaveData.fieldArea;
            }
            FieldBg_LoadAreaBg(eventMgr, 3, arg1);
            if (eventMgr->unk_21AC4 != 0) {
                EasyFade_FadeMainDisplay(FADER_INSTANT, 0, eventMgr->unk_21D12);
                if (eventMgr->taskId_UGFade == -1) {
                    if (arg1 == gSaveData.fieldArea) {
                        eventMgr->taskId_UGFade =
                            Fld_UGFade_CreateTask(&eventMgr->taskPool, (u16)eventMgr->unk_21D12, 0, &eventMgr->taskId_UGFade);
                    } else {
                        eventMgr->taskId_UGFade =
                            Fld_UGFade_CreateTask(&eventMgr->taskPool, (u16)eventMgr->unk_21D12, 1, &eventMgr->taskId_UGFade);
                    }
                }
            }
            EasyFade_FadeSubDisplay(FADER_INSTANT, 0, eventMgr->unk_21D12);
        }
        return 1;
    }

    eventMgr->unk_21A5C = (arg1 == gSaveData.fieldArea) ? 2 : 9;
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

void func_ov030_020a7810(FieldEventManager* eventMgr) {
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= 8;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[DISPLAY_BG3].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode                        = 1;

    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0 = 8;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1 = 20;

    eventMgr->blendEva = 0;
    eventMgr->blendEvb = 0x10000;

    Display_SetBlendCoeffs(DISPLAY_SUB, F2I(eventMgr->blendEva), F2I(eventMgr->blendEvb));

    eventMgr->blendTimer = 30;
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepBlendInSub, eventMgr, PROCESS_STAGE_INIT);
}

void func_ov030_020a7890(FieldEventManager* eventMgr) {
    eventMgr->blendEva   = 0xA000;
    eventMgr->blendEvb   = 0x8000;
    eventMgr->blendTimer = 10;
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepBlendOutSub, eventMgr, PROCESS_STAGE_INIT);
}

void func_ov030_020a78d0(FieldEventManager* eventMgr) {
    if (eventMgr->unk_21CF0 != 0) {
        SndMgr_StartPlayingSeqWithCustomSeqArc(1, 2, 7);
        eventMgr->unk_21CF0 = 0;
    }
    if (eventMgr->talkEventActive != 0) {
        func_ov030_020a737c(eventMgr);
        return;
    }
    func_ov030_020a7468(eventMgr);
}

void func_ov030_020a7928(FieldEventManager* eventMgr, s32 arg1, s16 arg2, u16 arg3) {
    eventMgr->unk_21CE4 = arg1;
    eventMgr->unk_21CE8 = arg3;
    if (eventMgr->unk_21CE8 == 0) {
        eventMgr->unk_21CE8 = 1;
    }
    eventMgr->unk_21CDC = I2F(g_DisplaySettings.controls[DISPLAY_MAIN].brightness);
    eventMgr->unk_21CE0 = I2F(g_DisplaySettings.controls[DISPLAY_SUB].brightness);
    if (arg2 == 0) {
        eventMgr->unk_21CD8 = 0x10;
        return;
    }
    if (arg2 == 1) {
        eventMgr->unk_21CD8 = -0x10;
        return;
    }
    eventMgr->unk_21CD8 = 0;
}

s32 func_ov030_020a7994(FieldEventManager* eventMgr) {
    if (eventMgr->unk_21C48[eventMgr->unk_21CD4] != -1) {
        if (FldMesWindow_IsAnimating(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[eventMgr->unk_21CD4])) != 0)
        {
            return 0;
        }
    }

    Task* task = EasyTask_GetTaskById(&eventMgr->taskPool, *(u32*)func_ov030_020a6960(eventMgr, 0x118));
    if ((task != NULL) && Fld_BustShot_IsMoveDone(task) == 0) {
        return 0;
    }
    if (eventMgr->unk_21A24 == -1) {
        s32 var_r2 = 0;
        if (eventMgr->talkEventActive != 0) {
            var_r2 = 1;
        }
        eventMgr->unk_21A24 = Fld_TouchNext_CreateTask(&eventMgr->taskPool, eventMgr->dataType, var_r2);
    }
    if (func_ov030_020a722c(eventMgr) != 0) {
        SndMgr_StartPlayingSE(SEIDX_SE_DIALOG_NEXT);
        if (eventMgr->unk_21A24 != -1) {
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21A24);
            eventMgr->unk_21A24 = -1;
        }
        return 1;
    }
    return 0;
}

void func_ov030_020a7aa0(u32 arg0, FieldEventManager* eventMgr, u32 arg2) {
    if (arg0 != -1) {
        if ((eventMgr->unk_21CEA + 1) == SysFont_LoadMsgCount(arg2)) {
            eventMgr->unk_21CEA = 0;
            FldEvent_AdvanceScript(eventMgr);
        } else {
            gSaveData.unk_24AC = gSaveData.unk_249C;
            gSaveData.unk_24B0 = gSaveData.unk_24A0;
            gSaveData.unk_24A0 = data_ov030_020d958a;
            gSaveData.unk_249C = 0;
        }
    } else {
        func_ov030_020a6cf8(eventMgr, 0x120);
        gSaveData.unk_24AC = gSaveData.unk_249C;
        gSaveData.unk_24B0 = gSaveData.unk_24A0;
        gSaveData.unk_24A0 = data_ov030_020d9588;
        gSaveData.unk_249C = 0;
    }
}

void func_ov030_020a7b50(FieldEventManager* eventMgr) {
    func_ov030_020a6ab4(eventMgr);
    switch (gSaveData.unk_24A0[gSaveData.unk_249C]) {
        case 0x0:
        case 0x2:
            eventMgr->targetBrightness = -0x10;
            eventMgr->unk_21D12        = 0x10;
            func_ov030_020a78d0(eventMgr);
            return;
        case 0x4:
            eventMgr->targetBrightness = -0x10;
            eventMgr->unk_21D12        = gSaveData.unk_24A0[gSaveData.unk_249C];
            func_ov030_020a78d0(eventMgr);
            return;
        case 0x1:
            eventMgr->targetBrightness = 0x10;
            eventMgr->unk_21D12        = 0x10;
            func_ov030_020a78d0(eventMgr);
            return;
        case 0x3:
            eventMgr->targetBrightness = 0x10;
            eventMgr->unk_21D12        = gSaveData.unk_24A0[gSaveData.unk_249C];
            func_ov030_020a78d0(eventMgr);
            return;
        case 0x9:
            eventMgr->targetBrightness = 0x10;
            eventMgr->unk_21D12        = gSaveData.unk_24A0[gSaveData.unk_249C];
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0xA:
            eventMgr->targetBrightness = -0x10;
            eventMgr->unk_21D12        = gSaveData.unk_24A0[gSaveData.unk_249C];
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0xB:
            func_ov030_020a6eac(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)), 0x14, -0x3F3, -0x3F1, -0x3F6,
                                -0x3F1);
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020aac84(eventMgr);
            return;
        case 0xC:
            func_ov030_020a6eac(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)), 0x14, -0x3FD, -0x3F1, -0x3FA,
                                -0x3F1);
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020aac84(eventMgr);
            return;
        case 0xD:
            func_ov030_020a6eac(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)), -0x3F3, -0x3F1, -0x3F6, -0x3F1);
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020aac84(eventMgr);
            return;
        case 0xE:
            func_ov030_020a6eac(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)), -0x3FD, -0x3F1, -0x3FA, -0x3F1);
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020aac84(eventMgr);
            return;
        case 0xF:
            func_ov030_020a6eac(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)),
                                (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 3) * 2)),
                                (s16)(s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 4) * 2)),
                                (s16)(s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 5) * 2)),
                                (s16)(s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 6) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020aac84(eventMgr);
            return;
        case 0x10:
            if (func_ov030_020a7994(eventMgr) == 0) {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x11:
            if (eventMgr->unk_21C48[eventMgr->unk_21CD4] != -1) {
                FldMesWindow_Move(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[eventMgr->unk_21CD4]),
                                  *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                  *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x12:
            FldGate_Set(*(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;

        case 0x1A: {
            u32* temp_r6 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r6     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, (s16)(gSaveData.unk_24A0[gSaveData.unk_249C + 1] - 0x40),
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0x40, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 8, 8, 1, (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r6, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x1B: {
            u32* temp_r7 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, (s16)(gSaveData.unk_24A0[gSaveData.unk_249C + 1] - 0x40),
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0x40, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 8, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 4], 1,
                (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7, eventMgr, gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x1C:
            func_ov030_020a6a1c(eventMgr);
            u32* temp_r6_2 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r6_2     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0x80, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 2, 0, 0, eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r6_2, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
            return;

        case 0x1D: {
            u32* temp_r7_2 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7_2     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0xFFFFFC02, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 0xC, 8, 1, (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_2, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x1E: {
            u32* temp_r7_3 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7_3     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0xFFFFFC02, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 0xC, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 4], 1,
                (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_3, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x1F: {
            u32* temp_r7_4 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7_4     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0xFFFFFC02, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 0xE, 8, 1, (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_4, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x20: {
            u32* temp_r7_5 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7_5     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0xFFFFFC02, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 0xA, 8, 1, (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_5, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x21: {
            u32* temp_r7_6 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7_6     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0xFFFFFC02, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], 0xA, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 4], 1,
                (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_6, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x22: {
            u32* temp_r8_3 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r8_3     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 4], (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 5],
                (s32)(u8)gSaveData.unk_24A0[gSaveData.unk_249C + 6], (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 7],
                (s32)(u8)gSaveData.unk_24A0[gSaveData.unk_249C + 8], (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r8_3, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 5]);
        } break;

        case 0x23: {
            u32* temp_r7_7 = (u32*)func_ov030_020a6d94(eventMgr);
            u8   var_r6    = (u8)gSaveData.unk_24A0[gSaveData.unk_249C + 6];
            s16  var_r0    = gSaveData.unk_24A0[gSaveData.unk_249C + 1];
            s16  var_r2    = gSaveData.unk_24A0[gSaveData.unk_249C + 3];
            if (var_r6 != 2) {
                var_r6 = 0x5D;
                var_r0 = Fld_BustShot_GetScreenX(
                    EasyTask_GetTaskById(&eventMgr->taskPool, *(u32*)func_ov030_020a6960(eventMgr, eventMgr->unk_21D00)));
                var_r2 = var_r0;
            }
            *temp_r7_7 = FldMesWindow_Create(&eventMgr->taskPool, eventMgr->dataType, var_r2, 0xD0, var_r0, 0xD0,
                                             gSaveData.unk_24A0[gSaveData.unk_249C + 5], var_r6,
                                             gSaveData.unk_24A0[gSaveData.unk_249C + 7], 0, 0, eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_7, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 5]);
        } break;

        case 0x24: {
            u32* temp_r7_8 = (u32*)func_ov030_020a6d80(eventMgr);
            *temp_r7_8     = FldMesWindow_CreateMessage(
                &eventMgr->taskPool, eventMgr->dataType, gSaveData.unk_24A0[gSaveData.unk_249C + 1],
                gSaveData.unk_24A0[gSaveData.unk_249C + 2], 0xFFFFFC02, (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 2],
                (s32)gSaveData.unk_24A0[gSaveData.unk_249C + 3], (s32)(u8)gSaveData.unk_24A0[gSaveData.unk_249C + 4], 8, 1,
                (s32)eventMgr->unk_21CEA);
            func_ov030_020a7aa0(*temp_r7_8, eventMgr, (u16)gSaveData.unk_24A0[gSaveData.unk_249C + 3]);
        } break;

        case 0x25:
            if (func_ov030_020a6ab4(eventMgr) != 0) {
                return;
            }
            gSaveData.unk_249C = gSaveData.unk_24AC;
            gSaveData.unk_24A0 = gSaveData.unk_24B0;
            return;
        case 0x26:
            if (func_ov030_020a7994(eventMgr) == 0) {
                return;
            }
            eventMgr->unk_21CEA = (u16)(eventMgr->unk_21CEA + 1);
            gSaveData.unk_249C  = gSaveData.unk_24AC;
            gSaveData.unk_24A0  = gSaveData.unk_24B0;
            return;
        case 0x13:
            func_ov030_020a6c24(eventMgr, 0x44);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x14:
            func_ov030_020a6cf8(eventMgr, 0x44);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x15:
            func_ov030_020a6c38(eventMgr, 0x44);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x16:
            func_ov030_020a6d0c(eventMgr, 0x44);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x17:
            func_ov030_020a6cf8(eventMgr, 0x120);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x18:
            func_ov030_020a6d0c(eventMgr, 0x120);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x19:
            func_ov030_020a6a1c(eventMgr);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x27:
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x2D:
            FieldBattle_SetupScripted(*(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x2E:
            eventMgr->exitRequest = 3;
            DebugOvlDisp_Pop();
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x2F:
            eventMgr->unk_21CEC = 1;
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x30:
            gSaveData.unk_24C4 = (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x31:
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0x10, 0);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x32:
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0, 0);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x33:
            func_ov030_020a7928(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)),
                                (u16) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 3) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x34:
            func_ov030_020a7928(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)), 0x14U);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x35:
            if (FieldEvent_IsDebugSkipHeld() != 0) {
                eventMgr->unk_21CE8 = 1U;
            }
            if (eventMgr->unk_21CE4 == 0) {
                func_02026590(&eventMgr->unk_21CE0, eventMgr->unk_21CD8 << 0xC, eventMgr->unk_21CE8);
                EasyFade_FadeSubDisplay(FADER_SMOOTH, (s32)eventMgr->unk_21CE0 >> 0xC, 0);
            } else {
                if (eventMgr->unk_21CE4 == 1) {
                    func_02026590(&eventMgr->unk_21CDC, eventMgr->unk_21CD8 << 0xC, eventMgr->unk_21CE8);
                    EasyFade_FadeMainDisplay(FADER_SMOOTH, (s32)eventMgr->unk_21CDC >> 0xC, 0);
                } else {
                    func_02026590(&eventMgr->unk_21CDC, eventMgr->unk_21CD8 << 0xC, eventMgr->unk_21CE8);
                    EasyFade_FadeMainDisplay(FADER_SMOOTH, (s32)eventMgr->unk_21CDC >> 0xC, 0);
                    func_02026590(&eventMgr->unk_21CE0, eventMgr->unk_21CD8 << 0xC, eventMgr->unk_21CE8);
                    EasyFade_FadeSubDisplay(FADER_SMOOTH, (s32)eventMgr->unk_21CE0 >> 0xC, 0);
                }
            }
            eventMgr->unk_21CE8 = (u16)(eventMgr->unk_21CE8 - 1);
            if (eventMgr->unk_21CE8 != 0) {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            return;

        case 0x36: {
            u32 temp_r1_14 =
                *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)));
            if (temp_r1_14 != -1) {
                Fld_BustShot_SetPriority(EasyTask_GetTaskById(&eventMgr->taskPool, temp_r1_14), 0);
            }
            FieldBg_LoadDownScreenSubBg(eventMgr, 0);
            func_ov030_020a7810(eventMgr);
            FldEvent_AdvanceScript(eventMgr);
        } break;

        case 0x37:
            func_ov030_020a7890(eventMgr);
            gSaveData.unk_24AC = gSaveData.unk_249C;
            gSaveData.unk_24B0 = gSaveData.unk_24A0;
            gSaveData.unk_24A0 = data_ov030_020d9586;
            gSaveData.unk_249C = 0;
            return;
        case 0x38:
            gSaveData.unk_249C = gSaveData.unk_24AC;
            gSaveData.unk_24A0 = gSaveData.unk_24B0;
            u32 temp_r1_16     = *(u32*)func_ov030_020a6960(eventMgr, gSaveData.unk_24A0[gSaveData.unk_24AC]);
            if (temp_r1_16 != -1) {
                Fld_BustShot_SetPriority(EasyTask_GetTaskById(&eventMgr->taskPool, temp_r1_16), 1);
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x39:
            if (eventMgr->unk_21C48[eventMgr->unk_21CD4] != -1) {
                FldMesWindow_Close(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[eventMgr->unk_21CD4]));
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x3A:
            SndMgr_StartPlayingSE((enum SndMgrSeIdx) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x3B:
            SndMgr_PlaySEWithPan(*(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                 *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x3C:
            Fld_Badge_SetAnimation(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_Badge),
                                   *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x3D:
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x3E:
            func_ov030_020c2fc0(*(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x3F:
            CriSndMgr_Stop(ADX_TITLE);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x40:
            gSaveData.unk_24A8 = 0;
            FldEvent_AdvanceScript(eventMgr);
            /* fallthrough */
        case 0x41:
            gSaveData.unk_24A8 = gSaveData.unk_24A8 + 1;
            if (gSaveData.unk_24A8 < (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2))) {
                if (eventMgr->unk_21ADC == 0) {
                    return;
                }
                if (FieldEvent_IsDebugSkipHeld() == 0) {
                    return;
                }
                FldEvent_AdvanceScript(eventMgr);
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x42:
            if (func_ov030_020a6ab4(eventMgr) != 0) {
                return;
            }
            if (func_ov030_020a7550(eventMgr) != 0) {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x43:
            eventMgr->unk_21758 = (u16)(eventMgr->unk_21758 - 1);
            if (eventMgr->unk_21748[eventMgr->unk_21758] == 0) {
                eventMgr->unk_21704 = Fld_GetPlate_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 24014);
            } else if (eventMgr->unk_21748[eventMgr->unk_21758] == 1) {
                eventMgr->unk_21704 = Fld_GetPlate_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 24015);
            } else {
                eventMgr->unk_21704 = Fld_GetPlate_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 24016);
            }

            eventMgr->taskId_Mess =
                Fld_GetMess_CreateTask(&eventMgr->taskPool, eventMgr->dataType, eventMgr->unk_21728[eventMgr->unk_21758],
                                       eventMgr->unk_21738[eventMgr->unk_21758]);
            eventMgr->taskId_Plate = Fld_TalkEventStringPlate_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x88, 0x60,
                                                                         eventMgr->unk_21748[eventMgr->unk_21758],
                                                                         eventMgr->unk_21718[eventMgr->unk_21758], 0);

            if (eventMgr->unk_21748[eventMgr->unk_21758] == 0) {
                eventMgr->taskId_Item  = Fld_WhichArrow_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x60, 4, 1);
                eventMgr->taskId_Arrow = Fld_WhichArrow_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x60, 5, 1);
            } else if (eventMgr->unk_21748[eventMgr->unk_21758] == 1) {
                eventMgr->taskId_Item  = Fld_WhichArrow_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x60, 7, 1);
                eventMgr->taskId_Arrow = Fld_WhichArrow_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x60, 8, 1);
            } else {
                eventMgr->taskId_Item  = Fld_WhichArrow_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x60, 0xA, 1);
                eventMgr->taskId_Arrow = Fld_WhichArrow_CreateTask(&eventMgr->taskPool, eventMgr->dataType, 0x60, 0xB, 1);
            }
            SndMgr_StartPlayingSE(SEIDX_SE_GET_KEYWORD);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x44:
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21704);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Mess);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Plate);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Item);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Arrow);
            FldEvent_AdvanceScript(eventMgr);
            return;

        case 0x45: {
            u16 sp22;
            u16 sp20;

            eventMgr->unk_2176A--;

            s32 temp_r4_2          = func_ov030_020a775c(eventMgr->unk_2175A[eventMgr->unk_2176A], &sp22, &sp20);
            eventMgr->unk_21704    = Fld_GetPlate_CreateTask(&eventMgr->taskPool, eventMgr->dataType, sp22);
            eventMgr->taskId_Mess  = Fld_GetMess_CreateTask(&eventMgr->taskPool, eventMgr->dataType, sp20, 0xFFFF);
            eventMgr->taskId_Plate = Fld_GetBase_CreateTask(&eventMgr->taskPool, eventMgr->dataType, temp_r4_2);
            eventMgr->taskId_Item =
                Fld_GetItem_CreateTask(&eventMgr->taskPool, eventMgr->dataType, eventMgr->unk_2175A[eventMgr->unk_2176A]);
            SndMgr_StartPlayingSE(SEIDX_SE_GET_ITEM);
            FldEvent_AdvanceScript(eventMgr);
        } break;

        case 0x46:
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21704);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Mess);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Plate);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Item);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x47:

            if (Fld_BustShot_IsMoveDone(EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2))))) == 0)
            {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x48:
            Fld_BustShot_ToggleFlip(EasyTask_GetTaskById(
                &eventMgr->taskPool,
                *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x49:
            Fld_BustShot_Nudge(
                EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x4A:
            Fld_BustShot_MoveToCenter(
                EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x4B:
            Fld_BustShot_SetSortKey(
                EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x4C:
            Fld_BustShot_MoveTo(
                EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)), *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 3) * 2)),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 4) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x4F:
            func_ov030_020a7284(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)), 1,
                                (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            return;
        case 0x4D:
            func_ov030_020a72fc(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)), 1);
            return;
        case 0x4E:
            func_ov030_020a72fc(eventMgr, *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)),
                                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            return;
        case 0x50:
            Fld_BustShot_StartShake(EasyTask_GetTaskById(
                &eventMgr->taskPool,
                *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x28:
            if (func_ov030_020a69ac(
                    &eventMgr->taskPool,
                    (u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2))),
                    1) == 0)
            {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020a7b50(eventMgr);
            return;
        case 0x29:
            if (func_ov030_020a69ac(&eventMgr->taskPool, (u32*)func_ov030_020a6960(eventMgr, 0x118), 1) == 0) {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020a7b50(eventMgr);
            return;
        case 0x2A:
            if (func_ov030_020a69ac(
                    &eventMgr->taskPool,
                    (u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2))),
                    0) == 0)
            {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            func_ov030_020a7b50(eventMgr);
            return;
        case 0x2B:
            Fld_BustShot_SlideOut(
                EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)), 1);
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x2C:
            Fld_BustShot_SwapSide(
                EasyTask_GetTaskById(
                    &eventMgr->taskPool,
                    *(u32*)func_ov030_020a6960(eventMgr, (s32) * (gSaveData.unk_24A0 + ((gSaveData.unk_249C + 1) * 2)))),
                *(gSaveData.unk_24A0 + ((gSaveData.unk_249C + 2) * 2)));
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x51:
            eventMgr->eventPictureId = gSaveData.unk_24A0[gSaveData.unk_249C];
            if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
            {
                DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeInMain, eventMgr, PROCESS_STAGE_INIT);
            }
            DebugOvlDisp_Push((OverlayCB)FieldMain_StepOpenEventPicture, eventMgr, PROCESS_STAGE_INIT);
            DebugOvlDisp_Push((OverlayCB)FieldMain_StepSetup2D, eventMgr, PROCESS_STAGE_INIT);
            DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseEventPicture, eventMgr, PROCESS_STAGE_INIT);
            if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
            {
                DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeMain, eventMgr, PROCESS_STAGE_INIT);
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x52:
            if (eventMgr->unk_21AC4 == 0) {
                if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                    (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
                {
                    DebugOvlDisp_Push((OverlayCB)FieldMain_StepStartUGFade, eventMgr, PROCESS_STAGE_INIT);
                }
                DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseTouchEvent, eventMgr, PROCESS_STAGE_INIT);
                if ((GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != 0x10) &&
                    (GXx_GetMasterBrightness(&REG_MASTER_BRIGHT) != -0x10))
                {
                    DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeMain, eventMgr, PROCESS_STAGE_INIT);
                }
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x53:
            if (func_ov030_020a75d8(eventMgr, gSaveData.unk_24A0[gSaveData.unk_249C]) != 0) {
                return;
            }
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x54:
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x5:
            eventMgr->targetBrightness = 0x10;
            eventMgr->unk_21D12        = 0x10;
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x7:
            eventMgr->targetBrightness = 0x10;
            eventMgr->unk_21D12        = gSaveData.unk_24A0[gSaveData.unk_249C];
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x6:
            eventMgr->targetBrightness = -0x10;
            eventMgr->unk_21D12        = 0x10;
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x8:
            eventMgr->targetBrightness = -0x10;
            eventMgr->unk_21D12        = gSaveData.unk_24A0[gSaveData.unk_249C];
            FldEvent_AdvanceScript(eventMgr);
            return;
        case 0x55:
            eventMgr->unk_21AE0 = 0;
            SndMgr_StartPlayingSE(SEIDX_EV_SE_LOOP_STOP);
            if (func_ov030_020aa4bc(eventMgr) != 0) {
                return;
            }
            if (eventMgr->unk_21758 != 0) {
                eventMgr->unk_21ADC = 0;
                FldEvent_SetCurrent(1);
                return;
            }
            if (eventMgr->unk_2176A != 0) {
                eventMgr->unk_21ADC = 0;
                FldEvent_SetCurrent(2);
                return;
            }
            if (gSaveData.unk_24B8 != 0) {
                return;
            }
            FldEvent_SetCurrent(3);
            return;

        case 0x56: {
            for (s32 i = 0; i < 35; i++) {
                func_ov030_020a69ac(&eventMgr->taskPool, &eventMgr->unk_21BBC[i], 1);
            }

            if ((eventMgr->targetBrightness != g_DisplaySettings.controls[DISPLAY_MAIN].brightness) ||
                (eventMgr->targetBrightness != g_DisplaySettings.controls[DISPLAY_SUB].brightness))
            {
                EasyFade_FadeBothDisplays(FADER_INSTANT, eventMgr->targetBrightness, (s32)eventMgr->unk_21D12);
                return;
            }

            if (func_ov030_020a75c0(eventMgr) != 0) {
                return;
            }
            if (eventMgr->unk_216C8 != -1) {
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_216C8);
                eventMgr->unk_216C8 = -1;
            }
            if (eventMgr->unk_21A24 != -1) {
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21A24);
                eventMgr->unk_21A24 = -1;
            }
            if (eventMgr->unk_21AC4 == 0) {
                DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseTouchEvent, eventMgr, PROCESS_STAGE_INIT);
                return;
            }
            if (eventMgr->unk_21A5C != 1) {
                FieldBg_SetTopBgMode(eventMgr, 1);
            }
            if (eventMgr->talkEventActive != 0) {
                REG_POWER_CNT &= ~0x8000;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~2;
                Field_CreateHud(eventMgr);
                Fld_Badge_SetVisible(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_Badge), 1);
                eventMgr->talkEventActive = 0;
            }
            if (eventMgr->unk_21CF8 != 0) {
                if (eventMgr->unk_21CFC != 0) {
                    eventMgr->unk_21CF8 = 0;
                } else {
                    if (eventMgr->humanCount != 0) {
                        s32 var_r7_2 = 0;
                        if (eventMgr->humanCount > 0) {
                            do {
                                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Humans[var_r7_2]);
                                var_r7_2 += 1;
                            } while (var_r7_2 < (s32)eventMgr->humanCount);
                        }
                        func_ov030_020c6300();
                        eventMgr->humanCount = 0;
                        return;
                    }
                    func_ov030_020c6150(&eventMgr->unk_21D30);
                    Field_SpawnHumans(eventMgr);
                    eventMgr->unk_21CFC = 1;
                    return;
                }
            }
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
            g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~2;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 1;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x3F;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 0xA;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 6;
            Fld_Badge_SetPriority(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_Badge), 0);
            gSaveData.unk_2498 = 0;
            SndMgr_StartPlayingSE(SEIDX_EV_SE_LOOP_STOP);
            func_ov030_020c2de8();
            func_ov030_020c2d50(gSaveData.areaPopulation[gSaveData.fieldArea]);
            Field_ReleaseEventPicture(eventMgr);
            if (eventMgr->unk_21D0C != 0) {
                func_ov030_020c2e28();
            }
            EasyFade_FadeBothDisplays(FADER_INSTANT, 0, (s32)eventMgr->unk_21D12);
        } break;
    }
}

// Nonmatching
void FldEvent_BeginScript(FieldEventManager* eventMgr, FieldEvent event, s32 arg2) {
    if (gSaveData.unk_2498 != FALSE) {
        return;
    }

    FldEvent_InitState(eventMgr);
    gSaveData.unk_2498 = TRUE;

    if (event >= 0x8000) {
        event -= 0x8000;
    }

    gSaveData.unk_249C        = 0;
    gSaveData.unk_24A4        = arg2;
    eventMgr->unk_21D0C       = FALSE;
    eventMgr->talkEventActive = FALSE;

    if (event >= 0x4000) {
        eventMgr->unk_21D0C = TRUE;
        event -= 0x4000;
    } else if (event >= 0x2000) {
        eventMgr->talkEventActive = TRUE;
        event -= 0x2000;
    }

    FldEvent_SetCurrent(event);
    Fld_Player_StopWalking(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_Player));
}

s32 func_ov030_020a9d00(FieldEventManager* eventMgr, FieldEvent event) {
    s32  var_r8 = 0;
    u32* var_r9 = eventMgr->unk_21BBC;
    do {
        func_ov030_020a69ac(&eventMgr->taskPool, var_r9, 1);
        var_r8 += 1;
        var_r9 += 1;
    } while (var_r8 < 0x23);

    EasyFade_FadeBothDisplays(FADER_INSTANT, eventMgr->targetBrightness, eventMgr->unk_21D12);
    if (EasyFade_IsFading() != 0) {
        return 0;
    }

    for (s32 var_r2 = 0; var_r2 < 35; var_r2++) {
        if (eventMgr->unk_21BBC[var_r2] != -1) {
            return 0;
        }
    }

    func_ov030_020a6a1c(eventMgr);
    FldEvent_SetCurrent(event);
    FldEvent_AdvanceScript(eventMgr);
    return 1;
}

void func_ov030_020a9db8(FieldEventManager* eventMgr, FieldEvent event) {
    if (func_ov030_020a9d00(eventMgr, event) == 0) {
        return;
    }
    if (eventMgr->touchEventKind == 3) {
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeInBoth, eventMgr, PROCESS_STAGE_INIT);
    } else if (eventMgr->touchEventResult != 3) {
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepStartUGFadeWithBg, eventMgr, PROCESS_STAGE_INIT);
    }
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseTouchEvent, eventMgr, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeBoth, eventMgr, PROCESS_STAGE_INIT);
}

void func_ov030_020a9e48(FieldEventManager* eventMgr) {
    Task* task    = EasyTask_GetTaskById(&eventMgr->taskPool, func_ov030_020a6960(eventMgr, eventMgr->unk_21D00));
    s16   temp_r0 = Fld_BustShot_GetScreenX(task);

    eventMgr->unk_216BC = Fld_Badge_CreateTask(&eventMgr->taskPool, eventMgr->dataType, temp_r0, 0x78, 0, 1);
    SndMgr_PlaySEWithPan(0x122, temp_r0);
    Fld_Badge_SetAnimation(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_216BC), 2);
    eventMgr->unk_216C4 = Fld_TalkEventTouch_CreateTask(&eventMgr->taskPool, eventMgr->dataType, temp_r0, 0x78);
    eventMgr->unk_21D2A = -1;
    func_ov030_020a6d20(eventMgr, 0x44);
    if (eventMgr->taskId_KeywordMenu != -1) {
        EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_KeywordMenu);
        eventMgr->taskId_KeywordMenu = -1;
    }
    func_ov030_020a7890(eventMgr);
}

// Nonmatching: promoted-int load before a halfword store and loop scheduling
// (USA 96.56%; JP unvalidated).
s32 func_ov030_020a9f54(FieldEventManager* eventMgr, s32 (*callback)(FieldEventManager*), s32 arg2, u16 count, ...) {
    if (eventMgr->taskId_Choice == -1) {
        func_ov030_020a6a1c(eventMgr);

        va_list choices;
        if (count > 8) {
            count = 8;
        }
        va_start(choices, count);
        for (s32 index = 0; index < count; index++) {
            eventMgr->unk_2176C[index] = (u16)va_arg(choices, int);
        }
        va_end(choices);

        eventMgr->unk_21784 =
            FldMesWindow_CreateMessage(&eventMgr->taskPool, eventMgr->dataType, 0x80, 0x60, 0x80, 0x60, arg2, 2, 0, 0, 0);
        eventMgr->taskId_Choice =
            Fld_Choice_CreateTask(&eventMgr->taskPool, eventMgr->dataType, (s16)count, eventMgr->unk_2176C);
    } else {
        eventMgr->unk_2177C =
            Fld_Choice_GetResult(&eventMgr->taskPool, EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->taskId_Choice));
        if (eventMgr->unk_2177C >= 0) {
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->taskId_Choice);
            EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_21784);
            eventMgr->taskId_Choice = -1;
            callback(eventMgr);
        }
    }
    return 1;
}

s32 func_ov030_020aa0c8(FieldEventManager* eventMgr, s32 arg1, s16 arg2, s16 arg3, s32 (*arg4)(FieldEventManager*)) {
    s16 temp_r4;

    eventMgr->unk_21D00 = arg1;
    eventMgr->unk_21D04 = arg2;
    eventMgr->unk_21D06 = arg3;
    eventMgr->unk_21D08 = arg4;
    if ((eventMgr->unk_21AD8 == 0) && (eventMgr->unk_216CC == -1)) {
        Task* task = EasyTask_GetTaskById(&eventMgr->taskPool, *(u32*)func_ov030_020a6960(eventMgr, eventMgr->unk_21D00));
        eventMgr->unk_216CC = Fld_TalkEventDecide_CreateSpeakerTask(&eventMgr->taskPool, eventMgr->dataType,
                                                                    Fld_BustShot_GetScreenX(task), 0x78);
    } else if ((eventMgr->unk_21AD8 == 0) && (EasyTask_ValidateTaskId(&eventMgr->taskPool, &eventMgr->unk_216CC) == 0)) {
        eventMgr->unk_21AD8 = 1;
        eventMgr->unk_216CC = -1;
        func_ov030_020a9e48(eventMgr);
    } else {
        if ((eventMgr->unk_21AD8 != 0) && (eventMgr->unk_216BC == -1U)) {
            func_ov030_020a9e48(eventMgr);
        } else if ((eventMgr->unk_21AD8 != 0) && (func_ov030_020a722c(eventMgr) != 0)) {
            Fld_BustShot_SetPriority(
                EasyTask_GetTaskById(&eventMgr->taskPool, *(u32*)func_ov030_020a6960(eventMgr, eventMgr->unk_21D00)), 1);

            if ((TouchInput_WasTouchPressed() != 0) &&
                (Fld_Badge_IsTouched(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_216BC)) != 0))
            {
                SndMgr_StartPlayingSE(SEIDX_SE_SCANS_OPEN);
                Task* temp_r0 =
                    EasyTask_GetTaskById(&eventMgr->taskPool, *(u32*)func_ov030_020a6960(eventMgr, eventMgr->unk_21D00));
                Fld_BustShot_SetPriority(temp_r0, 0);
                temp_r4 = Fld_BustShot_GetScreenX(temp_r0);
                FldMesWindow_SetPriority(EasyTask_GetTaskById(&eventMgr->taskPool, eventMgr->unk_21C48[eventMgr->unk_21CD4]),
                                         2);
                if ((eventMgr->unk_21D08 != 0) && (FieldTalk_GetKeywordCount() != 0)) {
                    eventMgr->taskId_KeywordMenu =
                        Fld_TalkEventKeywordMenu_CreateTask(&eventMgr->taskPool, eventMgr->dataType, temp_r4);
                }
                FieldBg_LoadDownScreenSubBg(eventMgr, 1);
                func_ov030_020a7810(eventMgr);
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_216BC);
                eventMgr->unk_216BC = -1;
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_216C4);
                FldEvent_SetCurrent(eventMgr->unk_21D04);
                FldEvent_AdvanceScript(eventMgr);
            } else {
                SndMgr_StartPlayingSE(SEIDX_SE_DIALOG_NEXT);
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_216BC);
                eventMgr->unk_216BC = -1;
                EasyTask_DeleteTask(&eventMgr->taskPool, eventMgr->unk_216C4);
                eventMgr->unk_21AD8 = 0;
                FldEvent_SetCurrent(eventMgr->unk_21D06);
                FldEvent_AdvanceScript(eventMgr);
            }
        }
    }
    return 1;
}

s32 func_ov030_020aa390(FieldEventManager* eventMgr) {
    switch (eventMgr->touchEventTarget) {
        case 0:
            FldEvent_SetCurrent(6);
            return 1;
        case 1:
            FldEvent_SetCurrent(7);
            return 1;
        case 2:
            FldEvent_SetCurrent(8);
            return 1;
        case 3:
            FldEvent_SetCurrent(9);
            return 1;
        case 4:
            FldEvent_SetCurrent(10);
            return 1;
        case 5:
            FldEvent_SetCurrent(11);
            return 1;
        case 6:
            FldEvent_SetCurrent(12);
            return 1;
        case 7:
            FldEvent_SetCurrent(13);
            return 1;
        case 8:
            FldEvent_SetCurrent(14);
            return 1;
        case 9:
            func_ov030_020a9db8(eventMgr, 0x32);
            return 0;
        case 10:
            func_ov030_020a9db8(eventMgr, 0x33);
            return 0;
        case 11:
            func_ov030_020a9db8(eventMgr, 0x34);
            return 0;
        case 13:
            func_ov030_020a9db8(eventMgr, 0x29);
            return 0;
        default:
        case 12:
            return 0;
    }
}

s32 func_ov030_020aa4bc(FieldEventManager* eventMgr) {
    if (gSaveData.unk_24B8 != 0) {
        return 0;
    }
    if (gSaveData.chapter >= 44) {
        gSaveData.chapter = 0;
    }
    if (data_ov030_020da618[gSaveData.chapter](eventMgr) != 0) {
        return 1;
    }
    if (func_ov030_020ab3b8(gSaveData.currentStoryEvent) != 0) {
        s16 temp_r0_2               = func_ov030_020ab420(gSaveData.currentStoryEvent);
        gSaveData.unk_24B4          = 0;
        gSaveData.unk_24B8          = 1;
        gSaveData.currentStoryEvent = temp_r0_2;
        FieldBattle_SetupScripted(-2);
        return 1;
    }
    if (func_ov030_020ab3ec(gSaveData.currentStoryEvent) != 0) {
        FldEvent_GrantPin(eventMgr, 0xFBU);
        return 0;
    }

    if (gSaveData.currentStoryEvent == 0x30A) {
        gSaveData.flashSequence = 0x30;
        gSaveData.flashExitMode = 0;
        eventMgr->exitRequest   = 4;
        DebugOvlDisp_Pop();
        return 0;
    } else if (gSaveData.currentStoryEvent == 0x36) {
        if (func_ov030_020a9d00(eventMgr, 5) == 0) {
            return 1;
        }
        eventMgr->touchEventKind   = 3;
        eventMgr->unk_21D24        = func_ov030_020aa390;
        eventMgr->unk_21D28        = 0x5DCC;
        eventMgr->touchEventResult = 0;
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeIn, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepOpenTouchEvent, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepSetup2D, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseEventPicture, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeBoth, eventMgr, PROCESS_STAGE_INIT);
        return 1;
    } else if (gSaveData.currentStoryEvent == 0x34) {
        if (func_ov030_020a9d00(eventMgr, 5) == 0) {
            return 1;
        }
        eventMgr->unk_21D1E      = 0x83;
        eventMgr->unk_21D20      = 0x83;
        eventMgr->touchEventKind = 1;
        eventMgr->unk_21D24      = NULL;

        if (eventMgr->touchEventKind == 1) {
            func_ov030_020aec1c(3);
            func_ov030_020aec1c(2);
        } else if (eventMgr->touchEventKind == 2) {
            func_ov030_020aec38(3);
            func_ov030_020aec1c(2);
        } else if (eventMgr->touchEventKind == 4) {
            func_ov030_020aec1c(3);
            func_ov030_020aec38(2);
        } else if (eventMgr->touchEventKind == 5) {
            func_ov030_020aec38(3);
            func_ov030_020aec38(2);
        }

        eventMgr->touchEventResult = 0;
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeIn, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepOpenTouchEvent, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepSetup2D, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseEventPicture, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeBoth, eventMgr, PROCESS_STAGE_INIT);
        return 1;
    } else if (gSaveData.currentStoryEvent == 0x35) {
        if (func_ov030_020a9d00(eventMgr, 5) == 0) {
            return 1;
        }
        eventMgr->unk_21D1E      = 0x83;
        eventMgr->unk_21D20      = 0x83;
        eventMgr->touchEventKind = 5;
        eventMgr->unk_21D24      = NULL;

        if (eventMgr->touchEventKind == 1) {
            func_ov030_020aec1c(3);
            func_ov030_020aec1c(2);
        } else if (eventMgr->touchEventKind == 2) {
            func_ov030_020aec38(3);
            func_ov030_020aec1c(2);
        } else if (eventMgr->touchEventKind == 4) {
            func_ov030_020aec1c(3);
            func_ov030_020aec38(2);
        } else if (eventMgr->touchEventKind == 5) {
            func_ov030_020aec38(3);
            func_ov030_020aec38(2);
        }

        eventMgr->touchEventResult = 0;
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeIn, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepOpenTouchEvent, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepSetup2D, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepCloseEventPicture, eventMgr, PROCESS_STAGE_INIT);
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepFadeBoth, eventMgr, PROCESS_STAGE_INIT);
        return 1;
    } else {
        if (func_ov030_020a66e8(eventMgr) != 0) {
            if (eventMgr->touchEventResult == 0) {
                return 1;
            } else if (eventMgr->touchEventResult == 1) {
                func_ov030_020a9db8(eventMgr, eventMgr->unk_21D1E);
                return 1;
            } else if (eventMgr->touchEventResult == 2) {
                func_ov030_020a9db8(eventMgr, eventMgr->unk_21D20);
                return 1;
            } else if (eventMgr->touchEventResult == 4) {
                FldEvent_SetCurrent((s32)eventMgr->touchEventTarget);
                FldEvent_AdvanceScript(eventMgr);
                eventMgr->touchEventResult = 0;
                return 1;
            } else if (eventMgr->touchEventResult == 5) {
                if (eventMgr->unk_21D24 != NULL) {
                    if (eventMgr->unk_21D24(eventMgr) != 0) {
                        FldEvent_AdvanceScript(eventMgr);
                        eventMgr->touchEventResult = 0;
                    }
                    return 1;
                }
                func_ov030_020a9db8(eventMgr, 4);
                return 1;
            } else if (eventMgr->touchEventResult == 3) {
                func_ov030_020a9db8(eventMgr, 4);
                return 1;
            }
        }
        return 0;
    }
}

// Nonmatching
void func_ov030_020aa9f4(FieldEventManager* eventMgr, s16 arg1, s16 arg2, s16 arg3) {
    for (s32 i = 0; i < 0x10; i++) {
        if (arg1 == gSaveData.unk_24C8[i]) {
            return;
        }
        if (gSaveData.unk_24C8[i] == -1) {
            gSaveData.unk_24C8[i] = arg1;
            gSaveData.unk_2508[i] = arg3;
            if (eventMgr->unk_21758 >= 8) {
                return;
            }
            eventMgr->unk_21718[eventMgr->unk_21758] = arg1;
            eventMgr->unk_21728[eventMgr->unk_21758] = arg2;
            if (arg3 == 0) {
                eventMgr->unk_21738[eventMgr->unk_21758] = arg1;
            } else {
                eventMgr->unk_21738[eventMgr->unk_21758] = -2;
            }
            eventMgr->unk_21748[eventMgr->unk_21758] = arg3;
            eventMgr->unk_21758++;
            return;
        }
    }
}

u8 func_ov030_020aaad8(void) {
    return gSaveData.brandTrends[Field_GetCurrentTrendZone()].ranking[0];
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

void FldEvent_GrantItem(FieldEventManager* eventMgr, u16 itemID) {
    if (func_ov030_020aaaf8(itemID, 0) && Inventory_AddItem(itemID, 0)) {
        eventMgr->unk_2175A[eventMgr->unk_2176A] = itemID;
        eventMgr->unk_2176A++;
    }
}

void FldEvent_GrantPin(FieldEventManager* eventMgr, u16 itemID) {
    if (func_ov030_020aaaf8(itemID, 2) && Inventory_AddItem(itemID, 2)) {
        eventMgr->unk_2175A[eventMgr->unk_2176A] = itemID;
        eventMgr->unk_2176A++;
    }
}

void func_ov030_020aac28(FieldEventManager* eventMgr, u16 itemID) {
    if (func_ov030_020aaaf8(itemID, 1) && Inventory_AddItem(itemID, 1)) {
        eventMgr->unk_2175A[eventMgr->unk_2176A] = itemID;
        eventMgr->unk_2176A++;
    }
}
