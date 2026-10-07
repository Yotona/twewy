#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/Events.h"
#include "Field/FieldTasks.h"
#include "Save.h"
#include <nitro/types.h>

extern void func_02026590(void*, s32, s32);
extern BOOL func_ov030_020b0454(void);
extern void func_ov030_020b3034(Task*);
extern void func_ov030_020b88e8(void*);
extern void func_ov030_020bd298(Task*);
extern void func_ov030_020ab554(void*);
extern s32  func_ov030_02082fe4(FieldEventManager*);
extern void func_ov030_02083024(FieldEventManager*);
extern void func_ov030_020a6a68(FieldEventManager*);
extern void func_ov030_020ad278(FieldEventManager*);
extern void func_ov030_020ad3a8(FieldEventManager*);
extern void func_ov030_020ad48c(FieldEventManager*);
extern void func_ov030_020ad4f4(FieldEventManager*);
extern void func_ov030_020ad730(FieldEventManager*);
extern void func_ov030_020ad958(FieldEventManager*, s32);
extern void func_ov030_020adaf8(FieldEventManager*);
extern void func_ov030_020aeaa0(s32, s32);
extern s32  func_ov030_020c0770(FieldEventManager*);
extern void func_ov030_020c1960(void);
extern void func_ov030_020c2d50(s32);
extern void func_ov030_020c2f74(void);
extern void func_ov030_020c3360(void);
extern s32  func_ov030_020c4a48(TaskPool*, s32);
extern void func_ov030_020af10c(void);
extern s32  func_ov030_020d311c(void*, s32, s32);
extern s32  data_ov030_020da4b8[];

// Object referenced through FieldEventManager.unk_21A58. Only offset 0x8 is
// read here; the rest of its layout belongs to a later TU.
typedef struct {
    char unk_00[8];
    s32  unk_08;
} Unk21A58;

void func_ov030_020824a0(FieldEventManager*);
void func_ov030_020824e4(FieldEventManager*);
void func_ov030_02082534(FieldEventManager*);
void func_ov030_02082578(FieldEventManager*);
void func_ov030_020825d8(FieldEventManager*);
void func_ov030_020826a4(FieldEventManager*);
void func_ov030_02082788(FieldEventManager*);
void func_ov030_020827b0(FieldEventManager*);
void func_ov030_020827e4(FieldEventManager*);
void func_ov030_020828fc(FieldEventManager*);
void func_ov030_0208299c(FieldEventManager*);
void func_ov030_02082a04(FieldEventManager*);
void func_ov030_02082a50(FieldEventManager*);
void func_ov030_02082a84(FieldEventManager*);
void func_ov030_02082b4c(FieldEventManager*);
void func_ov030_02082b8c(FieldEventManager*);
void func_ov030_02082bf0(FieldEventManager*);
void func_ov030_02082c04(FieldEventManager*);
void func_ov030_02082c68(FieldEventManager*);
void func_ov030_02082cec(FieldEventManager*);
void func_ov030_02082d3c(FieldEventManager*);
void func_ov030_02082d64(FieldEventManager*);
void func_ov030_02082e70(FieldEventManager*);
void func_ov030_02082f10(FieldEventManager*);
s32  func_ov030_02082fc0(void);

void func_ov030_020824a0(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() != 0) {
        return;
    }
    mgr->unk_21AC8 = 0;
    DebugOvlDisp_Pop();
}

void func_ov030_020824e4(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (mgr->unk_21630 == 4 || mgr->unk_21630 == 7) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, -0x10, 0x1000);
    } else {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
    }
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov030_02082534(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_INSTANT, mgr->targetBrightness, mgr->unk_21D12);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov030_02082578(FieldEventManager* mgr) {
    if (func_ov030_020b0454() != 0) {
        EasyFade_FadeMainDisplay(FADER_INSTANT, mgr->targetBrightness, 1);
    } else {
        EasyFade_FadeMainDisplay(FADER_INSTANT, mgr->targetBrightness, mgr->unk_21D12);
    }
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov030_020825d8(FieldEventManager* mgr) {
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020828fc, mgr, 0);
    mgr->unk_21AAE = 0x1E;
    mgr->unk_21AB0 = 0x10000;
    if (mgr->unk_21ACC != 0 && gSaveData.unk_2458 != 0x25) {
        mgr->unk_216D0 = func_ov030_020d311c(&mgr->taskPool, mgr->unk_21614, func_ov030_02082fc0());
    } else {
        mgr->unk_216D0 = -1;
    }
    func_ov030_020b3034(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D4));
    func_ov030_020c2d50(gSaveData.unk_2548[gSaveData.unk_2458]);
}

void func_ov030_020826a4(FieldEventManager* mgr) {
    s32 temp;

    mgr->unk_21AC8 = 1;
    if (mgr->unk_21AAE != 0) {
        func_02026590(&mgr->unk_21AB0, 0x10000, mgr->unk_21AAE);
        mgr->unk_21AAE--;
    }
    temp = mgr->unk_21AB0 >> 0xC;
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    if (temp < 0x10) {
        return;
    }
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

void func_ov030_02082788(FieldEventManager* mgr) {
    if (mgr->unk_21AAC != 0) {
        mgr->unk_21AAC--;
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov030_020827b0(FieldEventManager* mgr) {
    if (mgr->unk_21AAC != 0) {
        mgr->unk_21AAC--;
        return;
    }
    mgr->unk_21AAE = 0x1E;
    DebugOvlDisp_Pop();
}

void func_ov030_020827e4(FieldEventManager* mgr) {
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
    if (gSaveData.unk_2458 == 0x17) {
        func_ov030_020aeaa0(0x17, 1);
    }
    func_ov030_020b88e8(mgr->unk_21A58);
    if (((Unk21A58*)mgr->unk_21A58)->unk_08 == 0x66) {
        gSaveData.unk_244C = mgr->unk_21B08;
        gSaveData.unk_2450 = mgr->unk_21B0C;
    }
    gSaveData.unk_2454 = func_ov030_020aeca4(gSaveData.unk_2450);
    func_ov030_020c1960();
    func_ov030_020ad278(mgr);
    func_ov030_020bd298(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D8));
    func_ov030_02083024(mgr);
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020825d8, mgr, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
    mgr->unk_21AAC = 0x18;
    mgr->unk_21A5C = 0xA;
    func_ov030_020846bc(mgr, 1);
    func_ov030_020af10c();
    func_ov030_020c2f74();
}

void func_ov030_020828fc(FieldEventManager* mgr) {
    s32 temp;

    if (mgr->unk_21AAE != 0) {
        func_02026590(&mgr->unk_21AB0, 0, mgr->unk_21AAE);
        mgr->unk_21AAE--;
    }
    temp = mgr->unk_21AB0 >> 0xC;
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -temp, 0);
    mgr->unk_21AC8 = 1;
    if (temp != 0) {
        return;
    }
    if (func_ov030_02082fe4(mgr) == 0) {
        return;
    }
    DebugOvlDisp_Pop();
    mgr->unk_21AC8 = 0;
    DebugOvlDisp_Push((OverlayCB)func_ov030_020ab554, mgr, 0);
}

void func_ov030_0208299c(FieldEventManager* mgr) {
    if (func_ov030_020b0454() != 0) {
        EasyFade_FadeMainDisplay(FADER_INSTANT, 0, 1);
    } else {
        EasyFade_FadeMainDisplay(FADER_INSTANT, 0, mgr->unk_21D12);
    }
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() != 0) {
        return;
    }
    mgr->unk_21AC8 = 0;
    DebugOvlDisp_Pop();
}

void func_ov030_02082a04(FieldEventManager* mgr) {
    EasyFade_FadeBothDisplays(FADER_INSTANT, 0, mgr->unk_21D12);
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() != 0) {
        return;
    }
    mgr->unk_21AC8 = 0;
    DebugOvlDisp_Pop();
}

void func_ov030_02082a50(FieldEventManager* mgr) {
    mgr->unk_21AC8 = 1;
    if (EasyFade_IsFading() != 0) {
        return;
    }
    mgr->unk_21AC8 = 0;
    DebugOvlDisp_Pop();
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
    if (gSaveData.unk_2458 != 5) {
        return;
    }
    func_ov030_0208434c(mgr, 3, 5);
    mgr->unk_21A5C = 2;
}

void func_ov030_02082b8c(FieldEventManager* mgr) {
    func_ov030_020ad4f4(mgr);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 2) & ~1;
    if (mgr->unk_216C8 == -1 && mgr->unk_21CF4 != 0) {
        mgr->unk_216C8 = func_ov030_020c4a48(&mgr->taskPool, mgr->unk_21614);
    }
    DebugOvlDisp_Pop();
}

void func_ov030_02082bf0(FieldEventManager* mgr) {
    func_ov030_020ad958(mgr, 0);
    DebugOvlDisp_Pop();
}

void func_ov030_02082c04(FieldEventManager* mgr) {
    *(u16*)0x04000304 &= ~0x8000;
    // NOTE: the target preserves this provably-false check as cmp;cmpeq, the
    // same shape mwcc emits for a nested if or && chain (verified with the
    // repo toolchain); the call never executes.
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
    if (func_ov030_020c0770(mgr) != 0) {
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
        {
            s32 coeff0                                             = mgr->unk_21AB0;
            s32 coeff1                                             = mgr->unk_21AB4;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = coeff0 >> 0xC;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = coeff1 >> 0xC;
        }
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov030_02082f10(FieldEventManager* mgr) {
    if (mgr->unk_21AAE != 0) {
        if (func_ov030_020b0454() != 0) {
            mgr->unk_21AAE = 1;
        }
        func_02026590(&mgr->unk_21AB0, 0, mgr->unk_21AAE);
        func_02026590(&mgr->unk_21AB4, 0x10000, mgr->unk_21AAE);
        mgr->unk_21AAE--;
        {
            s32 coeff0                                             = mgr->unk_21AB0;
            s32 coeff1                                             = mgr->unk_21AB4;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = coeff0 >> 0xC;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = coeff1 >> 0xC;
        }
        return;
    }
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
    DebugOvlDisp_Pop();
}

s32 func_ov030_02082fc0(void) {
    s16 sel = gSaveData.unk_2458;

    if ((u32)sel >= 0x2C) {
        return 7;
    }
    return data_ov030_020da4b8[sel];
}
