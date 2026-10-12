#include "Otosu/OtosuMenu.h"

static void OtosuMenu_TitleBoss_Load(PrcCtx* ctx, OtosuMenu_TitleBossObj* boss, OtosuMenu_TitleBossArg* arg) {
    boss->upper    = arg->upper;
    boss->lower    = arg->lower;
    boss->duration = arg->duration;
    boss->timer    = arg->duration;
    boss->scrollY  = -0x200000;
}

static void OtosuMenu_TitleBoss_Destroy(void) {
    return; // do nothing
}

static void OtosuMenu_TitleBoss_Update(PrcCtx* ctx, OtosuMenu_TitleBossObj* boss) {
    Display_SetBGOffset(boss->upper->engineId, boss->upper->bgLayer, 0, boss->scrollY);
    Display_SetBGOffset(boss->lower->engineId, boss->lower->bgLayer, 0, boss->scrollY + 0x100000);
}

static void OtosuMenu_TitleBoss_Render(void) {
    return; // do nothing
}

static PrcStepResult OtosuMenu_TitleBoss_Step_Begin(PrcCtx* ctx, void* unused) {
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

// Nonmatching (JP): JP does not refresh the scrolled maps
static PrcStepResult OtosuMenu_TitleBoss_Step_Scroll(PrcCtx* ctx, void* work) {
    OtosuMenu_TitleBossObj* boss = work;

    func_020265d4(&boss->scrollY, 0, boss->timer);
    if (boss->timer == 0) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    if (OtosuMenu_TitleSkipped == 1) {
        boss->scrollY = 0;
        Display_SetBGOffset(boss->upper->engineId, boss->upper->bgLayer, 0, 0);
        Display_SetBGOffset(boss->lower->engineId, boss->lower->bgLayer, 0, 0x100000);
        boss->upper->flags |= 2;
        boss->lower->flags |= 2;
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    boss->timer--;
    return PRC_STEP_CONTINUE;
}

static PrcStepFn OtosuMenu_TitleBoss_StepTable[] = {OtosuMenu_TitleBoss_Step_Begin, OtosuMenu_TitleBoss_Step_Scroll,
                                                    PrcStep_Continue};

PrcFrameDesc OtosuMenu_TitleBoss_FrameDesc = {
    .enter     = OtosuMenu_TitleBoss_Load,
    .stepTable = OtosuMenu_TitleBoss_StepTable,
    .update    = OtosuMenu_TitleBoss_Update,
    .render    = OtosuMenu_TitleBoss_Render,
    .exit      = OtosuMenu_TitleBoss_Destroy,
};
