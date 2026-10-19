#include "Field/FieldRegion.h"

#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Event/EventFlags.h"
#include "Field/Event/EventManager.h"
#include "Field/Event/EventTables.h"
#include "Field/Scene/FieldAmbientSound.h"
#include "Field/Scene/FieldArea.h"
#include "Field/Scene/FieldBg.h"
#include "Field/Scene/FieldMain.h"
#include "Field/Scene/FieldMusic.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Scene/LocaTitle.h"
#include "Field/Tasks/Fld_Luck.h"
#include "Field/Tasks/Fld_Partner.h"
#include "Field/Tasks/Fld_Player.h"
#include "Field/Tasks/Fld_TalkEventTitle.h"
#include "Field/Tasks/Fld_TouchShine.h"
#include "Field/Tasks/Fld_UGFade.h"
#include "Save.h"
#include <nitro/types.h>

extern void func_02026590(s32* value, s32 target, u16 frames);

static s32  FieldMain_GetAreaTitleKind(void);
static void FieldMain_StepLoadArea(FieldEventManager*);
static void FieldMain_StepAreaFadeIn(FieldEventManager*);

void FieldMain_StepFadeIn(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepFadeOutExit(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (mgr->exitRequest == 4 || mgr->exitRequest == 7) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, -0x10, 0x1000);
    } else {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
    }
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepFadeBoth(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_INSTANT, mgr->targetBrightness, mgr->unk_21D12);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepFadeMain(FieldEventManager* mgr) {
    if (FieldEvent_IsDebugSkipHeld() != 0) {
        EasyFade_FadeMainDisplay(FADER_INSTANT, mgr->targetBrightness, 1);
    } else {
        EasyFade_FadeMainDisplay(FADER_INSTANT, mgr->targetBrightness, mgr->unk_21D12);
    }
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepEnterArea(FieldEventManager* mgr) {
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepAreaFadeIn, mgr, 0);
        mgr->blendTimer = 0x1E;
        mgr->blendEva   = 0x10000;
        if (mgr->unk_21ACC != 0 && gSaveData.fieldArea != 0x25) {
            mgr->unk_216D0 = LocaTitle_CreateTask(&mgr->taskPool, mgr->dataType, FieldMain_GetAreaTitleKind());
        } else {
            mgr->unk_216D0 = -1;
        }
        Fld_Player_StopWalking(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Player));
        func_ov030_020c2d50(gSaveData.areaPopulation[gSaveData.fieldArea]);
    }
}

void FieldMain_StepLeaveArea(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (mgr->blendTimer != 0) {
        func_02026590(&mgr->blendEva, 0x10000, mgr->blendTimer);
        mgr->blendTimer--;
    }

    s32 temp = F2I(mgr->blendEva);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    if (temp >= 0x10) {
        Field_DestroyObjects(mgr);
        if (mgr->unk_21700 != -1) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21700);
            mgr->unk_21700 = -1;
        }
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepLoadArea, mgr, 0);
        mgr->unk_21AAC = 2;
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepWait, mgr, 0);
        func_ov030_020c3360();
    }
}

void FieldMain_StepWait(FieldEventManager* mgr) {
    if (mgr->unk_21AAC != 0) {
        mgr->unk_21AAC--;
    } else {
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepWaitThenBlend(FieldEventManager* mgr) {
    if (mgr->unk_21AAC != 0) {
        mgr->unk_21AAC--;
    } else {
        mgr->blendTimer = 0x1E;
        DebugOvlDisp_Pop();
    }
}

static void FieldMain_StepLoadArea(FieldEventManager* mgr) {
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
    if (gSaveData.fieldArea == 23) {
        func_ov030_020aeaa0(23, 1);
    }
    func_ov030_020b88e8(mgr->activeExit);
    if (mgr->activeExit->destRegion == 102) {
        gSaveData.fieldPos.x = mgr->unk_21B08;
        gSaveData.fieldPos.y = mgr->unk_21B0C;
    }
    gSaveData.unk_2454 = func_ov030_020aeca4(gSaveData.fieldPos.y);
    Fld_Luck_GenerateSymbols();
    Field_CreateObjects(mgr);
    Fld_Partner_WarpToPlayer(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Partner));
    FieldBg_UpdateWalkSpeed(mgr);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepEnterArea, mgr, 0);
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepWait, mgr, 0);
    mgr->unk_21AAC = 24;
    mgr->unk_21A5C = 10;
    FieldBg_SetTopBgMode(mgr, 1);
    FieldDebug_DrawAreaName();
    func_ov030_020c2f74();
}

static void FieldMain_StepAreaFadeIn(FieldEventManager* mgr) {
    if (mgr->blendTimer != 0) {
        func_02026590(&mgr->blendEva, 0, mgr->blendTimer);
        mgr->blendTimer--;
    }

    s32 temp = F2I(mgr->blendEva);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    mgr->unk_21AC8 = 1;
    if (temp == 0 && FieldBg_IsAreaTitleDone(mgr)) {
        DebugOvlDisp_Pop();
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Push((OverlayCB)Field_MainUpdate, mgr, 0);
    }
}

void FieldMain_StepFadeInMain(FieldEventManager* mgr) {
    if (FieldEvent_IsDebugSkipHeld() != 0) {
        EasyFade_FadeMainDisplay(FADER_INSTANT, 0, 1);
    } else {
        EasyFade_FadeMainDisplay(FADER_INSTANT, 0, mgr->unk_21D12);
    }
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepFadeInBoth(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_INSTANT, 0, mgr->unk_21D12);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepWaitFade(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepStartUGFade(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    FieldEvent_IsDebugSkipHeld();
    EasyFade_FadeMainDisplay(FADER_INSTANT, 0, mgr->unk_21D12);
    if (mgr->taskId_UGFade == -1) {
        if (mgr->unk_21A5C != 9) {
            mgr->taskId_UGFade = Fld_UGFade_CreateTask(&mgr->taskPool, 0x10, 0, &mgr->taskId_UGFade);
        } else {
            mgr->taskId_UGFade = Fld_UGFade_CreateTask(&mgr->taskPool, 1, 1, &mgr->taskId_UGFade);
        }
    }
    EasyFade_FadeSubDisplay(FADER_INSTANT, 0, mgr->unk_21D12);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepWaitFade, mgr, 0);
}

void FieldMain_StepStartUGFadeWithBg(FieldEventManager* mgr) {
    FieldMain_StepStartUGFade(mgr);
    if (gSaveData.fieldArea == 5) {
        FieldBg_LoadAreaBg(mgr, 3, 5);
        mgr->unk_21A5C = 2;
    }
}

void FieldMain_StepOpenEventPicture(FieldEventManager* mgr) {
    Field_LoadEventPicture(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 2) & ~1;
    if (mgr->unk_216C8 == -1 && mgr->talkEventActive != 0) {
        mgr->unk_216C8 = Fld_TalkEventTitle_CreateTask(&mgr->taskPool, mgr->dataType);
    }
    DebugOvlDisp_Pop();
}

void FieldMain_StepSetup2D(FieldEventManager* mgr) {
    Field_SetupDisplay2D(mgr, 0);
    DebugOvlDisp_Pop();
}

void FieldMain_StepOpenTouchEvent(FieldEventManager* mgr) {
    REG_POWER_CNT &= ~0x8000;
    // Bug: Can never execute
    if (mgr->touchEventKind == 1 && mgr->touchEventKind == 2) {
        FieldBg_SetTopBgMode(mgr, 7);
    }
    Field_LoadTouchEventBg(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 2) & ~1;
    DebugOvlDisp_Pop();
}

void FieldMain_StepCloseEventPicture(FieldEventManager* mgr) {
    func_ov030_020a6a68(mgr);
    if (mgr->unk_216C8 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216C8);
        mgr->unk_216C8 = -1;
    }
    if (mgr->unk_21AC4 != 0) {
        Field_DestroyObjects(mgr);
    } else {
        Field_CloseEventPicture(mgr);
    }
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepWait, mgr, 0);
    mgr->unk_21AAC = 1;
}

static void FieldMain_StepRestoreObjects(FieldEventManager* mgr) {
    Field_CreateObjects(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers & ~2) | 1;
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepWait, mgr, 0);
    mgr->unk_21AAC = 0x18;
}

static void FieldMain_StepSetup3D(FieldEventManager* mgr) {
    Field_SetupDisplay3D(mgr);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepRestoreObjects, mgr, 0);
}

void FieldMain_StepCloseTouchEvent(FieldEventManager* mgr) {
    if (Fld_TouchShine_GetActiveCount() != 0) {
        return;
    }
    func_ov030_020a6a68(mgr);
    if (mgr->unk_216C8 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216C8);
        mgr->unk_216C8 = -1;
    }
    if (mgr->touchEventKind != 3) {
        Field_CloseEventPicture(mgr);
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)FieldMain_StepSetup3D, mgr, 0);
    } else {
        if (mgr->unk_21A90 != -1) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21A90);
            mgr->unk_21A90 = -1;
        }
        if (mgr->unk_21A94 != -1) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21A94);
            mgr->unk_21A94 = -1;
        }
        DebugOvlDisp_Pop();
    }
    mgr->touchEventKind = 0;
    DebugOvlDisp_Push((OverlayCB)FieldMain_StepWait, mgr, 0);
    mgr->unk_21AAC = 2;
}

void FieldMain_StepBlendInSub(FieldEventManager* mgr) {
    if (mgr->blendTimer != 0) {
        if (FieldEvent_IsDebugSkipHeld() != 0) {
            mgr->blendTimer = 1;
        }
        func_02026590(&mgr->blendEva, 0xA000, mgr->blendTimer);
        func_02026590(&mgr->blendEvb, 0x8000, mgr->blendTimer);
        mgr->blendTimer--;
        Display_SetBlendCoeffs(DISPLAY_SUB, F2I(mgr->blendEva), F2I(mgr->blendEvb));
    } else {
        DebugOvlDisp_Pop();
    }
}

void FieldMain_StepBlendOutSub(FieldEventManager* mgr) {
    if (mgr->blendTimer != 0) {
        if (FieldEvent_IsDebugSkipHeld() != 0) {
            mgr->blendTimer = 1;
        }
        func_02026590(&mgr->blendEva, 0, mgr->blendTimer);
        func_02026590(&mgr->blendEvb, 0x10000, mgr->blendTimer);
        mgr->blendTimer--;
        Display_SetBlendCoeffs(DISPLAY_SUB, F2I(mgr->blendEva), F2I(mgr->blendEvb));
    } else {
        g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
        DebugOvlDisp_Pop();
    }
}

static s32 FieldMain_GetAreaTitleKind(void) {
    if ((u32)gSaveData.fieldArea >= 44) {
        return 7;
    }
    return Field_AreaTitleKinds[gSaveData.fieldArea];
}
