#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/Events.h"
#include "Field/FieldTasks.h"
#include "Save.h"
#include <nitro/types.h>

extern void func_ov030_020ab554(void*);
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
    if (mgr->unk_21630 == 4 || mgr->unk_21630 == 7) {
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
    if (func_ov030_020b0454() != 0) {
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
        mgr->unk_21AAE = 0x1E;
        mgr->unk_21AB0 = 0x10000;
        if (mgr->unk_21ACC != 0 && gSaveData.unk_2458 != 0x25) {
            mgr->unk_216D0 = LocaTitle_CreateTask(&mgr->taskPool, mgr->unk_21614, func_ov030_02082fc0());
        } else {
            mgr->unk_216D0 = -1;
        }
        func_ov030_020b3034(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D4));
        func_ov030_020c2d50(gSaveData.unk_2548[gSaveData.unk_2458]);
    }
}

void func_ov030_020826a4(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (mgr->unk_21AAE != 0) {
        func_02026590(&mgr->unk_21AB0, 0x10000, mgr->unk_21AAE);
        mgr->unk_21AAE--;
    }

    s32 temp = F2I(mgr->unk_21AB0);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    if (temp >= 0x10) {
        func_ov030_020ad3a8(mgr);
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
        mgr->unk_21AAE = 0x1E;
        DebugOvlDisp_Pop();
    }
}

void func_ov030_020827e4(FieldEventManager* mgr) {
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
    if (gSaveData.unk_2458 == 23) {
        func_ov030_020aeaa0(23, 1);
    }
    func_ov030_020b88e8(mgr->unk_21A58);
    if (mgr->unk_21A58->destRegion == 102) {
        gSaveData.fieldPos.x = mgr->unk_21B08;
        gSaveData.fieldPos.y = mgr->unk_21B0C;
    }
    gSaveData.unk_2454 = func_ov030_020aeca4(gSaveData.fieldPos.y);
    func_ov030_020c1960();
    func_ov030_020ad278(mgr);
    func_ov030_020bd298(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D8));
    func_ov030_02083024(mgr);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020825d8, mgr, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 24;
    mgr->unk_21A5C = 10;
    func_ov030_020846bc(mgr, 1);
    func_ov030_020af10c();
    func_ov030_020c2f74();
}

void func_ov030_020828fc(FieldEventManager* mgr) {
    if (mgr->unk_21AAE != 0) {
        func_02026590(&mgr->unk_21AB0, 0, mgr->unk_21AAE);
        mgr->unk_21AAE--;
    }

    s32 temp = F2I(mgr->unk_21AB0);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    mgr->unk_21AC8 = 1;
    if (temp == 0 && func_ov030_02082fe4(mgr)) {
        DebugOvlDisp_Pop();
        mgr->unk_21AC8 = 0;
        DebugOvlDisp_Push((OverlayCB)func_ov030_020ab554, mgr, 0);
    }
}

void func_ov030_0208299c(FieldEventManager* mgr) {
    if (func_ov030_020b0454() != 0) {
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
    func_ov030_020b0454();
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
    if (gSaveData.unk_2458 == 5) {
        func_ov030_0208434c(mgr, 3, 5);
        mgr->unk_21A5C = 2;
    }
}

void func_ov030_02082b8c(FieldEventManager* mgr) {
    func_ov030_020ad4f4(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 2) & ~1;
    if (mgr->unk_216C8 == -1 && mgr->unk_21CF4 != 0) {
        mgr->unk_216C8 = Fld_TalkEventTitle_CreateTask(&mgr->taskPool, mgr->unk_21614);
    }
    DebugOvlDisp_Pop();
}

void func_ov030_02082bf0(FieldEventManager* mgr) {
    func_ov030_020ad958(mgr, 0);
    DebugOvlDisp_Pop();
}

void func_ov030_02082c04(FieldEventManager* mgr) {
    REG_POWER_CNT &= ~0x8000;
    // Bug: Can never execute
    if (mgr->unk_21D22 == 1 && mgr->unk_21D22 == 2) {
        func_ov030_020846bc(mgr, 7);
    }
    func_ov030_020ad730(mgr);
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
        func_ov030_020ad3a8(mgr);
    } else {
        func_ov030_020ad48c(mgr);
    }
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 1;
}

void func_ov030_02082cec(FieldEventManager* mgr) {
    func_ov030_020ad278(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers & ~2) | 1;
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 0x18;
}

void func_ov030_02082d3c(FieldEventManager* mgr) {
    func_ov030_020adaf8(mgr);
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
    if (mgr->unk_21D22 != 3) {
        func_ov030_020ad48c(mgr);
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
    mgr->unk_21D22 = 0;
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 2;
}

void func_ov030_02082e70(FieldEventManager* mgr) {
    if (mgr->unk_21AAE != 0) {
        if (func_ov030_020b0454() != 0) {
            mgr->unk_21AAE = 1;
        }
        func_02026590(&mgr->unk_21AB0, 0xA000, mgr->unk_21AAE);
        func_02026590(&mgr->unk_21AB4, 0x8000, mgr->unk_21AAE);
        mgr->unk_21AAE--;

        s32 coeff0 = mgr->unk_21AB0;
        s32 coeff1 = mgr->unk_21AB4;

        g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = coeff0 >> 0xC;
        g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = coeff1 >> 0xC;

    } else {
        DebugOvlDisp_Pop();
    }
}

void func_ov030_02082f10(FieldEventManager* mgr) {
    if (mgr->unk_21AAE != 0) {
        if (func_ov030_020b0454() != 0) {
            mgr->unk_21AAE = 1;
        }
        func_02026590(&mgr->unk_21AB0, 0, mgr->unk_21AAE);
        func_02026590(&mgr->unk_21AB4, 0x10000, mgr->unk_21AAE);
        mgr->unk_21AAE--;

        s32 coeff0 = mgr->unk_21AB0;
        s32 coeff1 = mgr->unk_21AB4;

        g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = coeff0 >> 0xC;
        g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = coeff1 >> 0xC;
    } else {
        g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
        DebugOvlDisp_Pop();
    }
}

s32 func_ov030_02082fc0(void) {
    if ((u32)gSaveData.unk_2458 >= 44) {
        return 7;
    }
    return data_ov030_020da4b8[gSaveData.unk_2458];
}
