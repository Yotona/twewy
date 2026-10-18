#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/Events.h"
#include "Field/FieldTasks.h"
#include "Save.h"
#include <nitro/types.h>

extern void Field_MainUpdate(void*);
extern s32  data_ov030_020da4b8[];

void func_ov030_02082788(FieldEventManager*);
void func_ov030_020827e4(FieldEventManager*);
void func_ov030_020828fc(FieldEventManager*);

void func_ov030_020824a0(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void func_ov030_020824e4(FieldEventManager* mgr) {
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

void func_ov030_02082534(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_INSTANT, mgr->targetBrightness, mgr->unk_21D12);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void func_ov030_02082578(FieldEventManager* mgr) {
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

void func_ov030_020825d8(FieldEventManager* mgr) {
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)func_ov030_020828fc, mgr, 0);
        mgr->blendTimer = 0x1E;
        mgr->blendEva   = 0x10000;
        if (mgr->unk_21ACC != 0 && gSaveData.fieldArea != 0x25) {
            mgr->unk_216D0 = LocaTitle_CreateTask(&mgr->taskPool, mgr->dataType, func_ov030_02082fc0());
        } else {
            mgr->unk_216D0 = -1;
        }
        func_ov030_020b3034(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Player));
        func_ov030_020c2d50(gSaveData.areaPopulation[gSaveData.fieldArea]);
    }
}

void func_ov030_020826a4(FieldEventManager* mgr) {
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
        DebugOvlDisp_Push((OverlayCB)func_ov030_020827e4, mgr, 0);
        mgr->unk_21AAC = 2;
        DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
        func_ov030_020c3360();
    }
}

void func_ov030_02082788(FieldEventManager* mgr) {
    if (mgr->unk_21AAC != 0) {
        mgr->unk_21AAC--;
    } else {
        DebugOvlDisp_Pop();
    }
}

void func_ov030_020827b0(FieldEventManager* mgr) {
    if (mgr->unk_21AAC != 0) {
        mgr->unk_21AAC--;
    } else {
        mgr->blendTimer = 0x1E;
        DebugOvlDisp_Pop();
    }
}

void func_ov030_020827e4(FieldEventManager* mgr) {
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
    func_ov030_020c1960();
    Field_CreateObjects(mgr);
    func_ov030_020bd298(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Partner));
    func_ov030_02083024(mgr);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020825d8, mgr, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 24;
    mgr->unk_21A5C = 10;
    func_ov030_020846bc(mgr, 1);
    FieldDebug_DrawAreaName();
    func_ov030_020c2f74();
}

void func_ov030_020828fc(FieldEventManager* mgr) {
    if (mgr->blendTimer != 0) {
        func_02026590(&mgr->blendEva, 0, mgr->blendTimer);
        mgr->blendTimer--;
    }

    s32 temp = F2I(mgr->blendEva);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    mgr->unk_21AC8 = 1;
    if (temp == 0 && func_ov030_02082fe4(mgr)) {
        DebugOvlDisp_Pop();
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Push((OverlayCB)Field_MainUpdate, mgr, 0);
    }
}

void func_ov030_0208299c(FieldEventManager* mgr) {
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

void func_ov030_02082a04(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_INSTANT, 0, mgr->unk_21D12);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void func_ov030_02082a50(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() == FALSE) {
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Pop();
    }
}

void func_ov030_02082a84(FieldEventManager* mgr) {
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
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082a50, mgr, 0);
}

void func_ov030_02082b4c(FieldEventManager* mgr) {
    func_ov030_02082a84(mgr);
    if (gSaveData.fieldArea == 5) {
        func_ov030_0208434c(mgr, 3, 5);
        mgr->unk_21A5C = 2;
    }
}

void func_ov030_02082b8c(FieldEventManager* mgr) {
    Field_LoadEventPicture(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 2) & ~1;
    if (mgr->unk_216C8 == -1 && mgr->talkEventActive != 0) {
        mgr->unk_216C8 = Fld_TalkEventTitle_CreateTask(&mgr->taskPool, mgr->dataType);
    }
    DebugOvlDisp_Pop();
}

void func_ov030_02082bf0(FieldEventManager* mgr) {
    Field_SetupDisplay2D(mgr, 0);
    DebugOvlDisp_Pop();
}

void func_ov030_02082c04(FieldEventManager* mgr) {
    REG_POWER_CNT &= ~0x8000;
    // Bug: Can never execute
    if (mgr->touchEventKind == 1 && mgr->touchEventKind == 2) {
        func_ov030_020846bc(mgr, 7);
    }
    Field_LoadTouchEventBg(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 2) & ~1;
    DebugOvlDisp_Pop();
}

void func_ov030_02082c68(FieldEventManager* mgr) {
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
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 1;
}

void func_ov030_02082cec(FieldEventManager* mgr) {
    Field_CreateObjects(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers & ~2) | 1;
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 0x18;
}

void func_ov030_02082d3c(FieldEventManager* mgr) {
    Field_SetupDisplay3D(mgr);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082cec, mgr, 0);
}

void func_ov030_02082d64(FieldEventManager* mgr) {
    if (func_ov030_020c0770() != 0) {
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
        DebugOvlDisp_Push((OverlayCB)func_ov030_02082d3c, mgr, 0);
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
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 2;
}

void func_ov030_02082e70(FieldEventManager* mgr) {
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

void func_ov030_02082f10(FieldEventManager* mgr) {
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

s32 func_ov030_02082fc0(void) {
    if ((u32)gSaveData.fieldArea >= 44) {
        return 7;
    }
    return data_ov030_020da4b8[gSaveData.fieldArea];
}
