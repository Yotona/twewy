#include "Otosu/OtosuMenu.h"
static const OtosuMenuCursor    OtosuMenu_CommError_BackCursor  = {2, 0x02, 0x9D};
static const OtosuMenuRectList2 OtosuMenu_CommError_BackButtons = {
    {
     {0x0000, 0x0008, 0x00A0, 0x0058, 0x00B8},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};
static const OtosuMenuRectList2 OtosuMenu_CommError_ConnectionLostTexts = {
    {
     {0x23EE, 0x0000, 0x0000, 0x0100, 0x00C0},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList3 OtosuMenu_CommError_WirelessUnavailableTexts = {
    {
     {0x23EF, 8, 8, 0xF8, 0x40},
     {0x23EB, 0x18, 0xA6, 0x58, 0xB6},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList3 OtosuMenu_CommError_JoinFailedTexts = {
    {
     {0x23EC, 8, 8, 0xF8, 0x40},
     {0x23EB, 0x18, 0xA6, 0x58, 0xB6},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList3 OtosuMenu_CommError_JoinFailedPopTexts = {
    {
     {0x23EC, 8, 8, 0xF8, 0x40},
     {0x23EB, 0x18, 0xA6, 0x58, 0xB6},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};

static void          OtosuMenu_CommError_Load(void);
static void          OtosuMenu_CommError_Destroy(PrcCtx* ctx, void* arg1);
static void          OtosuMenu_CommError_Update(void);
static void          OtosuMenu_CommError_Render(void);
static PrcStepResult OtosuMenu_CommError_Step_DestroyObjects(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_ClearWindow(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_CommError_Step_ShutdownWireless(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_CommError_Step_SetupJoinFailedScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_SetupConnectionLostScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_SetupJoinFailedPopScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_SetupWirelessUnavailableScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_WaitBack(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_WaitA(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_CommError_Step_ReturnToGame(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_ExitToMenu(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_ReturnToRoleSelect(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_CommError_Step_ReleaseScreens(PrcCtx* ctx, void* object);

PrcFrameDesc OtosuMenu_CommError_JoinFailedFrameDesc = {
    .enter     = OtosuMenu_CommError_Load,
    .stepTable = OtosuMenu_CommError_JoinFailedStepTable,
    .update    = OtosuMenu_CommError_Update,
    .render    = OtosuMenu_CommError_Render,
    .exit      = OtosuMenu_CommError_Destroy,
};

PrcStepFn OtosuMenu_CommError_ConnectionLostStepTable[] = {
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_DestroyObjects,
    OtosuMenu_CommError_Step_ClearWindow,
    OtosuMenu_CommError_Step_ShutdownWireless,
    OtosuMenu_CommError_Step_SetupConnectionLostScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_CommError_Step_WaitA,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_ReturnToGame,
};

PrcStepFn OtosuMenu_CommError_JoinFailedPopStepTable[] = {
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_DestroyObjects,
    OtosuMenu_CommError_Step_ClearWindow,
    OtosuMenu_CommError_Step_ShutdownWireless,
    OtosuMenu_CommError_Step_SetupJoinFailedPopScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_CommError_Step_WaitBack,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_ReleaseScreens,
    PrcStep_PopFrame,
};

PrcFrameDesc OtosuMenu_CommError_JoinFailedPopFrameDesc = {
    .enter     = OtosuMenu_CommError_Load,
    .stepTable = OtosuMenu_CommError_JoinFailedPopStepTable,
    .update    = OtosuMenu_CommError_Update,
    .render    = OtosuMenu_CommError_Render,
    .exit      = OtosuMenu_CommError_Destroy,
};

PrcFrameDesc OtosuMenu_CommError_WirelessUnavailableFrameDesc = {
    .enter     = OtosuMenu_CommError_Load,
    .stepTable = OtosuMenu_CommError_WirelessUnavailableStepTable,
    .update    = OtosuMenu_CommError_Update,
    .render    = OtosuMenu_CommError_Render,
    .exit      = OtosuMenu_CommError_Destroy,
};

PrcStepFn OtosuMenu_CommError_WirelessUnavailableStepTable[] = {
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_DestroyObjects,
    OtosuMenu_CommError_Step_ClearWindow,
    OtosuMenu_CommError_Step_SetupWirelessUnavailableScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_CommError_Step_WaitBack,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_ExitToMenu,
    PrcStep_PopFrame,
};

PrcFrameDesc OtosuMenu_CommError_ConnectionLostFrameDesc = {
    .enter     = OtosuMenu_CommError_Load,
    .stepTable = OtosuMenu_CommError_ConnectionLostStepTable,
    .update    = OtosuMenu_CommError_Update,
    .render    = OtosuMenu_CommError_Render,
    .exit      = OtosuMenu_CommError_Destroy,
};

PrcStepFn OtosuMenu_CommError_JoinFailedStepTable[] = {
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_DestroyObjects,
    OtosuMenu_CommError_Step_ClearWindow,
    OtosuMenu_CommError_Step_ShutdownWireless,
    OtosuMenu_CommError_Step_SetupJoinFailedScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_CommError_Step_WaitBack,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_CommError_Step_ReturnToRoleSelect,
    PrcStep_PopFrame,
};

static void OtosuMenu_CommError_Load(void) {}

static void OtosuMenu_CommError_Destroy(PrcCtx* ctx, void* menuObj) {
    OtosuMenu_ReleaseScreens(menuObj);
}

static void OtosuMenu_CommError_Update(void) {}

static void OtosuMenu_CommError_Render(void) {}

static PrcStepResult OtosuMenu_CommError_Step_DestroyObjects(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[2]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[3]);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

static PrcStepResult OtosuMenu_CommError_Step_ClearWindow(PrcCtx* ctx, void* unused) {
    g_DisplaySettings.engineState[0].window0              = 31;
    g_DisplaySettings.engineState[0].window0Effects       = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    Display_Commit();
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_ShutdownWireless(PrcCtx* ctx, void* unused) {
    s32 state;

    state = (s32)func_ov040_0209cb78();
    switch (state) { /* irregular */
        case 3:
            break;
        case 10:
        case 0:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        default:
            func_ov040_0209d540();
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_SetupJoinFailedScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList3 texts   = OtosuMenu_CommError_JoinFailedTexts;

    SystemStatusFlags;
    SystemStatusFlags.unk_06 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    OtosuMenu_SetupScreens(menuObj, 0x18, 0x1B, texts.rects);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_SetupConnectionLostScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList2 texts = OtosuMenu_CommError_ConnectionLostTexts;

    CriSndMgr_Pause(0x19, 1);
    SystemStatusFlags;
    SystemStatusFlags.unk_06 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    OtosuMenu_ReleaseScreens(menuObj);
    OtosuMenu_InitDisplay();
    OtosuMenu_SetupConnectionLostScreens(menuObj, texts.rects);
    menuObj->unk_474C8 = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP does not reset the link-level icon position
static PrcStepResult OtosuMenu_CommError_Step_SetupJoinFailedPopScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList3 texts   = OtosuMenu_CommError_JoinFailedPopTexts;

    SystemStatusFlags;
    SystemStatusFlags.unk_06 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    OtosuMenu_SetupScreens(menuObj, 0x18, 0x1B, texts.rects);
    menuObj->cursor.posX        = 0;
    menuObj->cursor.posY        = 0xC8;
    menuObj->linkLevelIcon.posX = 0;
    menuObj->linkLevelIcon.posY = 0xC8;
    menuObj->unk_474C8          = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_SetupWirelessUnavailableScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj = (OtosuMenuObj*)object;
    OtosuMenuRectList3 texts   = OtosuMenu_CommError_WirelessUnavailableTexts;

    SystemStatusFlags;
    SystemStatusFlags.unk_06 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    menuObj->unk_474C8       = 0xFFFF;
    OtosuMenu_SetupScreens(menuObj, 0x18, 0x1B, texts.rects);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP leaves the cursor in place on confirm
static PrcStepResult OtosuMenu_CommError_Step_WaitBack(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj    = (OtosuMenuObj*)object;
    OtosuMenuRectList2 buttons    = OtosuMenu_CommError_BackButtons;
    OtosuMenuCursor    cursors[1] = {OtosuMenu_CommError_BackCursor};
    u16                selected   = OtosuMenu_GetTouchedButton(buttons.rects);

    if (selected == 0xFFFF) {
        return PRC_STEP_CONTINUE;
    }
    if (selected == 0xFFFE) {
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0;
        menuObj->cursor.posY = 210;
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->unk_474C8 == selected) {
        if (selected == 0) {
            menuObj->cursor.posX = 0;
            menuObj->cursor.posY = 210;
            SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
            PrcCtx_AdvanceStep(ctx);
            return PRC_STEP_CONTINUE;
        }
    } else {
        menuObj->unk_474C8 = selected;
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
        if (selected == 0) {
            menuObj->cursor.posX = cursors[selected].x;
            menuObj->cursor.posY = cursors[selected].y;
            Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, cursors[selected].anim,
                                   menuObj->cursor.cellTable);
            return PRC_STEP_CONTINUE;
        }
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_WaitA(PrcCtx* ctx, void* unused) {
    if (SysControl.buttonState.pressedButtons & 1) {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CANCEL);
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_ReturnToGame(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->nextScene = 3;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_ExitToMenu(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->nextScene = 5;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_ReturnToRoleSelect(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenu_ReleaseScreens(menuObj);
    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_RoleSelect_FrameDesc, NULL);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_CommError_Step_ReleaseScreens(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenu_ReleaseScreens(menuObj);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}
