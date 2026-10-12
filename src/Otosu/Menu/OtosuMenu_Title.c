#include "Otosu/OtosuMenu.h"

static void          OtosuMenu_Title_Load(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_Title_Destroy(void);
static void          OtosuMenu_Title_Update(void);
static void          OtosuMenu_Title_Render(void);
static PrcStepResult OtosuMenu_Title_Step_Setup(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_StartDelay(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_Delay(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_WaitEntrance(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_StartScroll(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_Scroll(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_ToIdle(PrcCtx* ctx, void* unused);
static void          OtosuMenu_Title_IdleLoad(void);
static void          OtosuMenu_Title_IdleDestroy(PrcCtx* ctx, OtosuMenuObj* menuObj);
static PrcStepResult OtosuMenu_Title_Step_IdleSetup(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_IdleWait(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_Title_Step_ToRoleSelect(PrcCtx* ctx, void* unused);

static PrcStepFn OtosuMenu_Title_StepTable[] = {
    OtosuMenu_Title_Step_Setup,       OtosuPrcStep_FadeStart_Neutral2,
    OtosuPrcStep_FadeWait_Neutral2,   OtosuMenu_Title_Step_StartDelay,
    OtosuMenu_Title_Step_Delay,       OtosuMenu_Title_Step_WaitEntrance,
    OtosuMenu_Title_Step_StartScroll, OtosuMenu_Title_Step_Scroll,
    OtosuMenu_Title_Step_ToIdle,      PrcStep_PopFrame,
};

static PrcStepFn OtosuMenu_Title_IdleStepTable[] = {
    OtosuPrcStep_FadeStart_BrightImmediate, OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Title_Step_IdleSetup,         OtosuPrcStep_FadeStart_NeutralSlow,
    OtosuPrcStep_FadeWait_NeutralSlow,      OtosuMenu_Title_Step_IdleWait,
    OtosuPrcStep_FadeStart_BrightImmediate, OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_Title_Step_ToRoleSelect,      PrcStep_PopFrame,
};

PrcFrameDesc OtosuMenu_Title_FrameDesc = {
    .enter     = OtosuMenu_Title_Load,
    .stepTable = OtosuMenu_Title_StepTable,
    .update    = OtosuMenu_Title_Update,
    .render    = OtosuMenu_Title_Render,
    .exit      = OtosuMenu_Title_Destroy,
};

static PrcFrameDesc OtosuMenu_Title_IdleFrameDesc = {
    .enter     = OtosuMenu_Title_IdleLoad,
    .stepTable = OtosuMenu_Title_IdleStepTable,
    .update    = OtosuMenu_Title_Update,
    .render    = OtosuMenu_Title_Render,
    .exit      = OtosuMenu_Title_IdleDestroy,
};

static void OtosuMenu_Title_Load(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    OtosuMenu_TitleSkipped = FALSE;
    OtosuMenu_ReleaseScreens(menuObj);
}

static void OtosuMenu_Title_IdleLoad(void) {
    return; // do nothing
}

static void OtosuMenu_Title_Destroy(void) {
    SndMgr_StopPlayingSE(SEIDX_MC_01);
}

static void OtosuMenu_Title_IdleDestroy(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    SndMgr_StopPlayingSE(SEIDX_MC_01);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[2]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[3]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    OtosuMenu_InitDisplay();
    OtosuMenu_ReleaseScreens(menuObj);
}

static void OtosuMenu_Title_Update(void) {
    return; // do nothing
}

static void OtosuMenu_Title_Render(void) {
    return; // do nothing
}

static PrcStepResult OtosuMenu_Title_Step_StartDelay(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    menuObj->unk_474CC = 30;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

static PrcStepResult OtosuMenu_Title_Step_Delay(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    if (TouchInput_WasTouchPressed()) {
        PrcCtx_ReplaceFrame(ctx, &OtosuMenu_Title_IdleFrameDesc, NULL);
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->unk_474CC == 0) {
        PrcCtx_AdvanceStep(ctx);
    }
    menuObj->unk_474CC--;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_Setup(PrcCtx* ctx, void* object) {
    OtosuMenuObj*          menuObj    = object;
    PrcCtx*                enemies[4] = {&menuObj->objCtx[0], &menuObj->objCtx[1], &menuObj->objCtx[2], &menuObj->objCtx[3]};
    OtosuMenu_TitleBossArg bossArg;
    OtosuMenu_TitleEnmArg  enemyArg;

    OtosuMenu_SetupTitleScreens(menuObj);
    for (u16 i = 0; i < 4; i++) {
        enemyArg.index    = 3 - i;
        enemyArg.duration = 120;
        PrcCtx_Init(enemies[i], "OtosuMenu_TitleEnmObj", sizeof(OtosuMenu_TitleEnmObj));
        PrcCtx_ReplaceFrame(enemies[i], &OtosuMenu_TitleEnemy_FrameDesc, &enemyArg);
        PrcMaster_RegisterContext(&menuObj->prcMaster, enemies[i]);
    }
    bossArg.upper    = &menuObj->subMaps[2];
    bossArg.lower    = &menuObj->mainMaps[2];
    bossArg.duration = 120;
    PrcCtx_Init(&menuObj->objCtx[4], "OtosuMenu_TitleBossObj", sizeof(OtosuMenu_TitleBossObj));
    PrcCtx_ReplaceFrame(&menuObj->objCtx[4], &OtosuMenu_TitleBoss_FrameDesc, &bossArg);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    menuObj->unk_474C8  = 120;
    menuObj->bg0ScrollX = -0x200000;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_WaitEntrance(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    if (TouchInput_WasTouchPressed()) {
        PrcCtx_ReplaceFrame(ctx, &OtosuMenu_Title_IdleFrameDesc, NULL);
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->unk_474C8 == 0) {
        PrcCtx_AdvanceStep(ctx);
    }
    menuObj->unk_474C8--;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_StartScroll(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    menuObj->unk_474C8  = 20;
    menuObj->bg0ScrollX = 0x100000;
    menuObj->bg1ScrollX = -0x100000;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_Scroll(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    if ((TouchInput_WasTouchPressed()) || (menuObj->unk_474C8 == 0)) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    func_020265d4(&menuObj->bg0ScrollX, 0, menuObj->unk_474C8);
    func_020265d4(&menuObj->bg1ScrollX, 0, menuObj->unk_474C8);

    Display_SetBGOffset(menuObj->subMaps[0].engineId, menuObj->subMaps[0].bgLayer, menuObj->bg0ScrollX, 0);
    Display_SetBGOffset(menuObj->mainMaps[0].engineId, menuObj->mainMaps[0].bgLayer, menuObj->bg0ScrollX, 0);
    Display_SetBGOffset(menuObj->mainMaps[1].engineId, menuObj->mainMaps[1].bgLayer, menuObj->bg1ScrollX, 0);

    menuObj->unk_474C8--;
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_ToIdle(PrcCtx* ctx, void* unused) {
    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_Title_IdleFrameDesc, NULL);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_IdleSetup(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    Display_SetBGOffset(menuObj->subMaps[2].engineId, menuObj->subMaps[2].bgLayer, 0, 0);
    Display_SetBGOffset(menuObj->mainMaps[2].engineId, menuObj->mainMaps[2].bgLayer, 0, 0x100000);
    Display_SetBGOffset(menuObj->subMaps[0].engineId, menuObj->subMaps[0].bgLayer, 0, 0);
    Display_SetBGOffset(menuObj->mainMaps[0].engineId, menuObj->mainMaps[0].bgLayer, 0, 0);
    Display_SetBGOffset(menuObj->mainMaps[1].engineId, menuObj->mainMaps[1].bgLayer, 0, 0);
    menuObj->subMaps[2].flags |= 2;
    menuObj->mainMaps[2].flags |= 2;
    menuObj->subMaps[0].flags |= 2;
    menuObj->mainMaps[0].flags |= 2;
    menuObj->mainMaps[1].flags |= 2;
    Display_SetBGOffset(menuObj->subMaps[1].engineId, menuObj->subMaps[1].bgLayer, 0, 0);
    menuObj->subMaps[1].flags |= 2;
    OtosuMenu_TitleSkipped = TRUE;
    Display_SetBGOffset(menuObj->subMaps[2].engineId, menuObj->subMaps[2].bgLayer, 0, 0);
    Display_SetBGOffset(menuObj->mainMaps[2].engineId, menuObj->mainMaps[2].bgLayer, 0, 0x100000);
    menuObj->unk_474C8 = 240;
    SndMgr_StartPlayingSE(SEIDX_MC_01);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_IdleWait(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;

    menuObj->unk_474C8--;
    if (TouchInput_WasTouchPressed()) {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
    }
    if ((TouchInput_WasTouchPressed()) || (menuObj->unk_474C8 == 0)) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_Title_Step_ToRoleSelect(PrcCtx* ctx, void* unused) {
    PrcCtx_ReplaceFrame(ctx, &OtosuMenu_RoleSelect_FrameDesc, NULL);
    return PRC_STEP_CONTINUE;
}
