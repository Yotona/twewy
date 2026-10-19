#include "Otosu/OtosuMenu.h"

static void OtosuMenu_Entry_Load(void) {
    return; // do nothing
}

static void OtosuMenu_Entry_Destroy(PrcCtx* ctx, void* object) {
    OtosuMenu_ReleaseScreens(object);
}

static void OtosuMenu_Entry_Update(void) {
    return; // do nothing
}

static void OtosuMenu_Entry_Render(void) {
    return; // do nothing
}

static PrcStepResult OtosuMenu_Entry_Step_Setup(PrcCtx* ctx, void* object) {
    OtosuMenu_SetupEntryScreens(object);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Entry_Step_WaitInput(PrcCtx* ctx, void* object) {
    if ((TouchInput_WasTouchPressed()) || (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_B) || (SysControl.buttonState.pressedButtons & INPUT_BUTTON_X) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_Y) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_UP) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_DOWN) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_LEFT) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_RIGHT))
    {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Entry_Step_Finish(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->nextScene = 1;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Entry_Step_WaitInputExit(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    if ((TouchInput_WasTouchPressed()) || (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_B) || (SysControl.buttonState.pressedButtons & INPUT_BUTTON_X) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_Y) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_UP) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_DOWN) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_LEFT) ||
        (SysControl.buttonState.pressedButtons & INPUT_BUTTON_RIGHT))
    {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_Title_FrameDesc, NULL);
        PrcCtx_PushStepTable(ctx, PrcSteps_FadeBright);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepFn OtosuMenu_Entry_MultiplayerStepTable[] = {
    OtosuMenu_Entry_Step_Setup,
    OtosuPrcStep_FadeStart_Neutral2,
    OtosuPrcStep_FadeWait_Neutral2,
    OtosuMenu_Entry_Step_WaitInputExit,
    OtosuPrcStep_FadeStart_Bright,
    OtosuPrcStep_FadeWait,
    PrcStep_PopFrame,
};

static PrcStepFn OtosuMenu_Entry_SinglePlayerStepTable[] = {
    OtosuMenu_Entry_Step_Setup,     OtosuPrcStep_FadeStart_Neutral2,
    OtosuPrcStep_FadeWait_Neutral2, OtosuMenu_Entry_Step_WaitInput,
    OtosuPrcStep_FadeStart_Bright,  OtosuPrcStep_FadeWait,
    OtosuMenu_Entry_Step_Finish,    PrcStep_PopFrame,
};

PrcFrameDesc OtosuMenu_Entry_MultiplayerFrameDesc = {
    .enter     = OtosuMenu_Entry_Load,
    .stepTable = OtosuMenu_Entry_MultiplayerStepTable,
    .update    = OtosuMenu_Entry_Update,
    .render    = OtosuMenu_Entry_Render,
    .exit      = OtosuMenu_Entry_Destroy,
};

PrcFrameDesc OtosuMenu_Entry_SinglePlayerFrameDesc = {
    .enter     = OtosuMenu_Entry_Load,
    .stepTable = OtosuMenu_Entry_SinglePlayerStepTable,
    .update    = OtosuMenu_Entry_Update,
    .render    = OtosuMenu_Entry_Render,
    .exit      = OtosuMenu_Entry_Destroy,
};
