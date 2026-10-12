#include "Otosu/OtosuMenu.h"

static void          OtosuMenu_RoleSelect_Load(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_RoleSelect_Destroy(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_RoleSelect_Update(void);
static void          OtosuMenu_RoleSelect_Render(void);
static PrcStepResult OtosuMenu_RoleSelect_Step_Setup(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_RoleSelect_Step_Select(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_RoleSelect_Step_Decide(PrcCtx* ctx, void* object);

static PrcStepFn OtosuMenu_RoleSelect_StepTable[] = {
    OtosuMenu_RoleSelect_Step_Setup,        OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,          OtosuMenu_RoleSelect_Step_Select,
    OtosuPrcStep_FadeStart_BrightImmediate, OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_RoleSelect_Step_Decide,       PrcStep_PopFrame,
};

PrcFrameDesc OtosuMenu_RoleSelect_FrameDesc = {
    .enter     = OtosuMenu_RoleSelect_Load,
    .stepTable = OtosuMenu_RoleSelect_StepTable,
    .update    = OtosuMenu_RoleSelect_Update,
    .render    = OtosuMenu_RoleSelect_Render,
    .exit      = OtosuMenu_RoleSelect_Destroy,
};

static void OtosuMenu_RoleSelect_Load(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    for (u16 i = 0; i < 4; i++) {
        gSaveData.otosuScores[i] = 0;
    }
    gSaveData.mabsBasePP     = 0;
    gSaveData.unk_1D84       = 0;
    menuObj->commErrorScreen = 0;
    OtosuMenu_LoadCursorAndLinkLevel(menuObj);
}

static void OtosuMenu_RoleSelect_Destroy(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    OtosuMenu_ReleaseScreens(menuObj);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
}

static void OtosuMenu_RoleSelect_Update(void) {}

static void OtosuMenu_RoleSelect_Render(void) {}

static PrcStepResult OtosuMenu_RoleSelect_Step_Setup(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;
    u16           iconY;
    OtosuMenuRect texts[6] = {
        {0x23F0,   0x08,   0x08,   0xF8, 0x40},
        {0x23EB,   0x18,   0xA6,   0x58, 0xB6},
        {0x23F1,   0x58,   0x56,   0xB8, 0x66},
        {0x23F2,   0x58,   0x7E,   0xB8, 0x8E},
        {0x23F3,   0xC5,   0xA6,   0xF5, 0xB6},
        {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,    0},
    };

    SysFont_SetSpacing(&menuObj->font, TRUE, 0);
    OtosuMenu_SetupScreens(menuObj, 0x15, 0x14, texts);
    CriSndMgr_PlayFile(ADX_B11);
    menuObj->unk_474C8 = 0xFFFF;
    // Each icon object is given a whole OtosuMenuObj of work memory, though only a Sprite is used.
    iconY = 0x60;
    PrcCtx_Init(&menuObj->objCtx[0], "OtosuMenu_Icon", sizeof(OtosuMenuObj));
    PrcCtx_ReplaceFrame(&menuObj->objCtx[0], &OtosuMenu_Icon_FrameDesc, &iconY);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    iconY = 0x88;
    PrcCtx_Init(&menuObj->objCtx[1], "OtosuMenu_Icon", sizeof(OtosuMenuObj));
    PrcCtx_ReplaceFrame(&menuObj->objCtx[1], &OtosuMenu_Icon_FrameDesc, &iconY);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
    SystemStatusFlags;
    SystemStatusFlags.unk_06 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP does not move the link-level icon
static PrcStepResult OtosuMenu_RoleSelect_Step_Select(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj    = object;
    OtosuMenuRect buttons[5] = {
        {     0,   0x08,   0xA8,   0x58,   0xB8},
        {     1,   0x4C,   0x58,   0xB8,   0x68},
        {     2,   0x4C,   0x80,   0xB8,   0x90},
        {     3,   0xA5,   0xA0,   0xF5,   0xB8},
        {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    };
    OtosuMenuCursor cursors[4] = {
        {2, 0x02, 0x9D},
        {1, 0x3F, 0x4D},
        {1, 0x3F, 0x75},
        {2, 0xA4, 0x9D},
    };
    u16 selected = OtosuMenu_GetTouchedButton(buttons);

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
        switch (selected) {
            case 0:
                SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CANCEL);
                PrcCtx_AdvanceStep(ctx);
                return PRC_STEP_CONTINUE;
            case 1:
            case 2:
            case 3:
                menuObj->linkLevelIcon.posX = 0x80;
                menuObj->linkLevelIcon.posY = 0x60;
                SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
                PrcCtx_AdvanceStep(ctx);
                return PRC_STEP_CONTINUE;
        }
    } else {
        menuObj->unk_474C8 = selected;
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
        switch (selected) {
            case 0:
            case 1:
            case 2:
            case 3:
                menuObj->cursor.posX = cursors[selected].x;
                menuObj->cursor.posY = cursors[selected].y;
                Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, cursors[selected].anim,
                                       menuObj->cursor.cellTable);
                return PRC_STEP_CONTINUE;
        }
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_RoleSelect_Step_Decide(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = object;
    u32           wasUnk06;
    s32           enterArg; // passed uninitialised; the lobby frames don't read it

    switch (menuObj->unk_474C8) {
        case 0:
            menuObj->nextScene = 5;
            return PRC_STEP_CONTINUE;
        case 1:
            wasUnk06                 = SystemStatusFlags.unk_06;
            SystemStatusFlags.unk_06 = 0;
            menuObj->unk_46070       = wasUnk06;
            SystemStatusFlags;
            SystemStatusFlags.unk_07 = 0;
            PrcCtx_ReplaceFrame(ctx, &OtosuMenu_Lobby_HostFrameDesc, &enterArg);
            break;
        case 2:
            wasUnk06                 = SystemStatusFlags.unk_06;
            SystemStatusFlags.unk_06 = 0;
            menuObj->unk_46070       = wasUnk06;
            SystemStatusFlags;
            SystemStatusFlags.unk_07 = 0;
            PrcCtx_ReplaceFrame(ctx, &OtosuMenu_Lobby_JoinFrameDesc, &enterArg);
            break;
        case 3:
            gSaveData.unk_341C         = 0x1F;
            gSaveData.otosuPlayerCount = 3;
            menuObj->nextScene         = 1;
            break;
    }
    return PRC_STEP_CONTINUE;
}
