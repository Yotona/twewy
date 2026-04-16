#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

/// MARK: BtlFeat02

typedef struct BtlFeat02 {
    /* 0x000 */ s32 (**stateCallbacks)(struct BtlFeat02*);
    /* 0x004 */ char unk_004[4];

    /* 0x008 */ CombatSprite phaseSpriteMainLeft;
    /* 0x068 */ s32          phaseSpriteMainLeftPosX;
    /* 0x06C */ s32          phaseSpriteMainLeftPosY;

    /* 0x070 */ CombatSprite phaseSpriteMainRight;
    /* 0x0D0 */ s32          phaseSpriteMainRightPosX;
    /* 0x0D4 */ s32          phaseSpriteMainRightPosY;

    /* 0x0D8 */ CombatSprite phaseSpriteSubLeft;
    /* 0x138 */ s32          phaseSpriteSubLeftPosX;
    /* 0x13C */ s32          phaseSpriteSubLeftPosY;

    /* 0x140 */ CombatSprite phaseSpriteSubRight;
    /* 0x1A0 */ s32          phaseSpriteSubRightPosX;
    /* 0x1A4 */ s32          phaseSpriteSubRightPosY;

    /* 0x1A8 */ CombatSprite companionSpriteA;
    /* 0x208 */ s32          unk_208;
    /* 0x20C */ s32          unk_20C;

    /* 0x210 */ CombatSprite companionSpriteB;
    /* 0x270 */ s32          unk_270;
    /* 0x274 */ s32          unk_274;

    /* 0x278 */ s16         companionLayoutMode;
    /* 0x27A */ char        unk_27A[2];
    /* 0x27C */ s32         pairedSpriteSeparationY; // +/- Y separation for symmetric paired sprites
    /* 0x280 */ s32         pairedSpriteBaseY;    // Base Y position for symmetric paired sprites, modified by vertical motion
    /* 0x284 */ s32         shakeScreenOffset;    // Oscillates sprite offset between +/- shakeAmpLimit
    /* 0x288 */ s32         shakeStep;            // Shake amplitude delta when shaking
    /* 0x28C */ s32         shakeDirectionToggle; // Direction to shake
    /* 0x290 */ s32         shakeAmpLimit;        // Shake amplitude limit
    /* 0x294 */ Data*       phasePackData;
    /* 0x298 */ char        unk_298[0x29C - 0x298];
    /* 0x29C */ Data*       rapidFrameMainPackData;
    /* 0x2A0 */ Data*       rapidFrameSubPackData;
    /* 0x2A4 */ BgResource* mainBgCharResLayerC;
    /* 0x2A8 */ BgResource* subBgCharResLayerC;
    /* 0x2AC */ BgResource* mainBgCharResLayerD;
    /* 0x2B0 */ BgResource* subBgCharResLayerD;
    /* 0x2B4 */ s32         unk_2B4;
    /* 0x2B8 */ s32         unk_2B8;
    /* 0x2BC */ char        unk_2BC[0x2DC - 0x2BC];
    /* 0x2DC */ s32         unk_2DC;
    /* 0x2E0 */ s32         unk_2E0;
    /* 0x2E4 */ char        unk_2E4[0x304 - 0x2E4];
    /* 0x304 */ s32         unk_304;
    /* 0x308 */ s32         unk_308;
    /* 0x30C */ char        unk_30C[0x32C - 0x30C];
    /* 0x32C */ s32         unk_32C;
    /* 0x330 */ s32         unk_330;
    /* 0x334 */ char        unk_334[0x354 - 0x334];
    /* 0x354 */ s32         unk_354;
    /* 0x358 */ s32         unk_358;
    /* 0x35C */ s32         unk_35C;
    /* 0x360 */ s32         unk_360;
    /* 0x364 */ s32         unk_364;
    /* 0x368 */ char        unk_368[0x36C - 0x368];
    /* 0x36C */ s32         unk_36C;
    /* 0x370 */ s32         unk_370;
    /* 0x374 */ s32         unk_374;
    /* 0x378 */ s32         unk_378;
    /* 0x37C */ char        unk_37C[0x384 - 0x37C];
    /* 0x384 */ PaletteResource* mainPalette;
    /* 0x388 */ PaletteResource* subPalette;
    /* 0x38C */ char             unk_38C[0x39E - 0x38C];
    /* 0x39E */ u16              phaseCountdown;
    /* 0x3A0 */ s16              stateIndex;
    /* 0x3A2 */ char             unk_3A2[2];
    /* 0x3A4 */ s32              unk_3A4;
    /* 0x3A8 */ s32              unk_3A8;
    /* 0x3AC */ s32              unk_3AC;
    /* 0x3B0 */ s32              bgLayerAOffsetH; // Horizontal offset for BG layer A
    /* 0x3B4 */ s32              bgLayerAOffsetV; // Vertical offset for BG layer A
    /* 0x3B8 */ s32              bgLayerBOffsetH; // Horizontal offset for BG layer B
    /* 0x3BC */ s32              bgLayerBOffsetV; // Vertical offset for BG layer B
    /* 0x3C0 */ s32              bgLayerCOffsetH; // Horizontal offset for BG layer C
    /* 0x3C4 */ s32              bgLayerCOffsetV; // Vertical offset for BG layer C
    /* 0x3C8 */ s32              bgLayerDOffsetH; // Horizontal offset for BG layer D
    /* 0x3CC */ s32              bgLayerDOffsetV; // Vertical offset for BG layer D
    /* 0x3D0 */ u16              stateTimer;
    /* 0x3D2 */ u16              frameStepCountdown;
    /* 0x3D4 */ u16              subStateInterval;
    /* 0x3D6 */ u16              shakeCallbackCooldown;
    /* 0x3D8 */ char             unk_3D8[0x3E0 - 0x3D8];
    /* 0x3E0 */ s32              verticalVelocity;
    /* 0x3E4 */ s32              verticalAcceleration;
    /* 0x3E8 */ char             unk_3E8[0x3F0 - 0x3E8];
    /* 0x3F0 */ s16              unk_3F0;
    /* 0x3F2 */ s16              unk_3F2;
    /* 0x3F4 */ s16              unk_3F4;
} BtlFeat02; // Size: 0x3F8

s32 BtlFeat02_Step_FadeInFast(BtlFeat02* feat);
s32 BtlFeat02_Step_FadeInSlow(BtlFeat02* feat);
s32 BtlFeat02_Step_EndTask(BtlFeat02* feat);
s32 BtlFeat02_Step_RestoreDisplay(BtlFeat02* feat);
s32 BtlFeat02_Step_SetupDisplay(BtlFeat02* feat);
s32 BtlFeat02_Step_LoadPhase1Resources(BtlFeat02* feat);
s32 BtlFeat02_Step_AdvancePhase1RiseMotion(BtlFeat02* feat);
s32 BtlFeat02_Step_DampenMotionAndStartCountdown(BtlFeat02* feat);
s32 BtlFeat02_Step_RunCountdownAndFadeOutMidpoint(BtlFeat02* feat);
s32 BtlFeat02_Step_LoadPhase2Resources(BtlFeat02* feat);
s32 BtlFeat02_Step_AccelerateDive(BtlFeat02* feat);
s32 BtlFeat02_Step_BeginRapidFrameSequence(BtlFeat02* feat);
s32 BtlFeat02_Step_UpdateRapidFrameSequence(BtlFeat02* feat);
s32 BtlFeat02_Step_LoadFinalPhaseResources(BtlFeat02* feat);
s32 BtlFeat02_Step_UpdateFinalPhaseShakeAndTimeout(BtlFeat02* feat);
s32 BtlFeat02_Step_ReleasePhase(BtlFeat02* feat);
s32 BtlFeat02_Step_ReleaseCompanionSprite(BtlFeat02* feat);
s32 BtlFeat02_Step_SetShortDelay(BtlFeat02* feat);
s32 BtlFeat02_Step_SetLongDelay(BtlFeat02* feat);
s32 BtlFeat02_Step_UpdateDelay(BtlFeat02* feat);
s32 BtlFeat02_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern BinIdentifier data_ov003_020dd78c;
extern BinIdentifier data_ov003_020dd7c4;

static u16 data_ov004_020f05a0[];

static u16 data_ov004_020f051c[];

s32 (*BtlFeat02_StepTable[25])(BtlFeat02*) = {
    BtlFeat02_Step_SetupDisplay,
    BtlFeat02_Step_LoadPhase1Resources,
    BtlFeat02_Step_FadeInFast,
    BtlFeat02_Step_AdvancePhase1RiseMotion,
    BtlFeat02_Step_DampenMotionAndStartCountdown,
    BtlFeat02_Step_RunCountdownAndFadeOutMidpoint,
    BtlFeat02_Step_ReleasePhase,
    BtlFeat02_Step_LoadPhase2Resources,
    BtlFeat02_Step_FadeInSlow,
    BtlFeat02_Step_AccelerateDive,
    BtlFeat02_Step_SetLongDelay,
    BtlFeat02_Step_UpdateDelay,
    BtlFeat02_Step_BeginRapidFrameSequence,
    BtlFeat02_Step_FadeInFast,
    BtlFeat02_Step_UpdateRapidFrameSequence,
    BtlFeat02_Step_SetShortDelay,
    BtlFeat02_Step_UpdateDelay,
    BtlFeat02_Step_ReleasePhase,
    BtlFeat02_Step_LoadFinalPhaseResources,
    BtlFeat02_Step_FadeInFast,
    BtlFeat02_Step_UpdateFinalPhaseShakeAndTimeout,
    BtlFeat02_Step_ReleasePhase,
    BtlFeat02_Step_ReleaseCompanionSprite,
    BtlFeat02_Step_RestoreDisplay,
    BtlFeat02_Step_EndTask,
};

const SpriteAnimEntry data_ov004_020f0540[2] = {
    { 0xE, 0x10,  0xF, 0},
    {0x16, 0x18, 0x17, 0}
};

const SpriteAnimEntry data_ov004_020f0550[4] = {
    {0x12, 0x14, 0x13, 0},
    {0x1A, 0x1C, 0x1B, 0},
    {0x12, 0x14, 0x13, 1},
    {0x1A, 0x1C, 0x1B, 1},
};

typedef struct {
    u16 unk_0;
    u16 unk_2;
    u16 unk_4;
    s16 unk_6;
} Quad;

Quad data_ov004_020f0570[6] = {
    {30, 32, 31, 0},
    {30, 35, 34, 0},
    {37, 39, 38, 0},
    {37, 42, 41, 0},
    {44, 46, 45, 0},
    {44, 49, 48, 0}
};

const TaskHandle Tsk_BtlFeat02 = {"Tsk_BtlFeat02", BtlFeat02_RunTask, 0x3F8};

void BtlFeat02_NextState(BtlFeat02* feat) {
    feat->stateIndex++;
    feat->unk_3F0 = 0;
    feat->unk_3F2 = 0;
    feat->unk_3F4 = 0;
}

void BtlFeat02_SetStateTable(BtlFeat02* feat, s32 (**cbTable)(BtlFeat02*)) {
    feat->stateCallbacks = cbTable;
}

void BtlFeat02_ReleasePhaseResources(BtlFeat02* feat) {
    func_0200d954(0, 2);
    func_0200d954(0, 3);
    func_0200d954(1, 2);
    func_0200d954(1, 3);

    if (feat->mainPalette != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], feat->mainPalette);
        feat->mainPalette = NULL;
    }
    if (feat->mainBgCharResLayerC != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], feat->mainBgCharResLayerC);
        feat->mainBgCharResLayerC = NULL;
    }
    if (feat->mainBgCharResLayerD != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], feat->mainBgCharResLayerD);
        feat->mainBgCharResLayerD = NULL;
    }
    if (feat->subPalette != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], feat->subPalette);
        feat->subPalette = NULL;
    }
    if (feat->subBgCharResLayerC != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], feat->subBgCharResLayerC);
        feat->subBgCharResLayerC = NULL;
    }
    if (feat->subBgCharResLayerD != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], feat->subBgCharResLayerD);
        feat->subBgCharResLayerD = NULL;
    }
    if (feat->phasePackData != NULL) {
        DatMgr_ReleaseData(feat->phasePackData);
        feat->phasePackData = NULL;
    }
    if (feat->rapidFrameMainPackData != NULL) {
        DatMgr_ReleaseData(feat->rapidFrameMainPackData);
        feat->rapidFrameMainPackData = NULL;
    }
    if (feat->rapidFrameSubPackData != NULL) {
        DatMgr_ReleaseData(feat->rapidFrameSubPackData);
        feat->rapidFrameSubPackData = NULL;
    }

    CombatSprite_Release(&feat->phaseSpriteMainLeft);
    CombatSprite_Release(&feat->phaseSpriteMainRight);
    CombatSprite_Release(&feat->phaseSpriteSubLeft);
    CombatSprite_Release(&feat->phaseSpriteSubRight);
}

void BtlFeat02_ReleaseAllResources(BtlFeat02* feat) {
    func_0200d954(0, 2);
    func_0200d954(0, 3);
    func_0200d954(1, 2);
    func_0200d954(1, 3);

    if (feat->mainPalette != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], feat->mainPalette);
        feat->mainPalette = NULL;
    }

    if (feat->mainBgCharResLayerC != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], feat->mainBgCharResLayerC);
        feat->mainBgCharResLayerC = NULL;
    }

    if (feat->mainBgCharResLayerD != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], feat->mainBgCharResLayerD);
        feat->mainBgCharResLayerD = NULL;
    }

    if (feat->subPalette != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], feat->subPalette);
        feat->subPalette = NULL;
    }

    if (feat->subBgCharResLayerC != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], feat->subBgCharResLayerC);
        feat->subBgCharResLayerC = NULL;
    }

    if (feat->subBgCharResLayerD != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], feat->subBgCharResLayerD);
        feat->subBgCharResLayerD = NULL;
    }

    if (feat->phasePackData != NULL) {
        DatMgr_ReleaseData(feat->phasePackData);
        feat->phasePackData = NULL;
    }

    if (feat->rapidFrameMainPackData != NULL) {
        DatMgr_ReleaseData(feat->rapidFrameMainPackData);
        feat->rapidFrameMainPackData = NULL;
    }

    if (feat->rapidFrameSubPackData != NULL) {
        DatMgr_ReleaseData(feat->rapidFrameSubPackData);
        feat->rapidFrameSubPackData = NULL;
    }

    CombatSprite_Release(&feat->phaseSpriteMainLeft);
    CombatSprite_Release(&feat->phaseSpriteMainRight);
    CombatSprite_Release(&feat->phaseSpriteSubLeft);
    CombatSprite_Release(&feat->phaseSpriteSubRight);
    CombatSprite_Release(&feat->companionSpriteA);
    CombatSprite_Release(&feat->companionSpriteB);
}

void BtlFeat02_LoadPhasePairedSprites(BtlFeat02* feat, s32 arg1) {
    u16 sp10[3];
    sp10[0] = data_ov004_020f051c[0];
    sp10[1] = data_ov004_020f051c[1];
    sp10[2] = data_ov004_020f051c[2];

    if (arg1 > 0) {
        CombatSprite_Release(&feat->phaseSpriteMainLeft);
        CombatSprite_Release(&feat->phaseSpriteMainRight);
        CombatSprite_Release(&feat->phaseSpriteSubLeft);
        CombatSprite_Release(&feat->phaseSpriteSubRight);
    }

    s32 temp_r4 = arg1 * 2;
    u32 temp_r7 = (u32)(temp_r4 << 0x10) >> 0xD;
    u16 temp_r6 = data_ov004_020f0570[temp_r7].unk_6;
    CombatSprite_LoadDirect(1, &feat->phaseSpriteMainLeft, &data_ov003_020dd78c, (s32)data_ov004_020f0570[temp_r7].unk_0,
                            (s32)data_ov004_020f0570[temp_r7].unk_2, data_ov004_020f0570[temp_r7].unk_4, (s32)sp10[arg1],
                            (s32)data_ov004_020f0570[temp_r7].unk_6);
    CombatSprite_SetAnim(&feat->phaseSpriteMainLeft, temp_r6, 0);
    func_ov003_02082738(&feat->phaseSpriteMainLeft, 2);
    func_ov003_02082730(&feat->phaseSpriteMainLeft, 0x7FFFFFFF);
    CombatSprite_SetFlip(&feat->phaseSpriteMainLeft, 0);

    u32 temp_r7_2 = temp_r4 + 1;
    u32 temp_r3   = (u32)(temp_r7_2 << 0x10) >> 0xD;
    u16 temp_r6_2 = data_ov004_020f0570[temp_r3].unk_6;
    CombatSprite_LoadDirect(1, &feat->phaseSpriteMainRight, &data_ov003_020dd78c, (s32)data_ov004_020f0570[temp_r3].unk_0,
                            (u16)(s32)data_ov004_020f0570[temp_r3].unk_2, (u16)(s32)data_ov004_020f0570[temp_r3].unk_4,
                            (u16)(s32)sp10[arg1], (u16)(s32)temp_r6_2);
    CombatSprite_SetAnim(&feat->phaseSpriteMainRight, (s16)temp_r6_2, 0);
    CombatSprite_SetFlip(&feat->phaseSpriteMainRight, 0);
    func_ov003_02082738(&feat->phaseSpriteMainRight, 2);
    func_ov003_02082730(&feat->phaseSpriteMainRight, 0x7FFFFFFF);

    u32 temp_r3_2 = (u32)(temp_r4 << 0x10) >> 0xD;
    s16 temp_r4_2 = data_ov004_020f0570[temp_r3_2].unk_6;
    CombatSprite_LoadDirect(0, &feat->phaseSpriteSubLeft, &data_ov003_020dd78c, (s32)data_ov004_020f0570[temp_r3_2].unk_0,
                            (u16)(s32)data_ov004_020f0570[temp_r3_2].unk_2, (u16)(s32)data_ov004_020f0570[temp_r3_2].unk_4,
                            (u16)(s32)sp10[arg1], (u16)(s32)temp_r4_2);
    CombatSprite_SetAnim(&feat->phaseSpriteSubLeft, temp_r4_2, 0);
    CombatSprite_SetFlip(&feat->phaseSpriteSubLeft, 0);
    func_ov003_02082738(&feat->phaseSpriteSubLeft, 2);
    func_ov003_02082730(&feat->phaseSpriteSubLeft, 0x7FFFFFFF);

    u32 temp_r6_3 = (u32)(temp_r7_2 << 0x10) >> 0xD;
    s16 temp_r4_3 = data_ov004_020f0570[temp_r6_3].unk_6;
    CombatSprite_LoadDirect(0, &feat->phaseSpriteSubRight, &data_ov003_020dd78c, (s32)data_ov004_020f0570[temp_r6_3].unk_0,
                            (u16)(s32)data_ov004_020f0570[temp_r6_3].unk_2, (u16)(s32)data_ov004_020f0570[temp_r6_3].unk_4,
                            (u16)(s32)sp10[arg1], (u16)(s32)temp_r4_3);
    CombatSprite_SetAnim(&feat->phaseSpriteSubRight, temp_r4_3, 0);
    CombatSprite_SetFlip(&feat->phaseSpriteSubRight, 0);
    func_ov003_02082738(&feat->phaseSpriteSubRight, 2);
    func_ov003_02082730(&feat->phaseSpriteSubRight, 0x7FFFFFFF);
}

void BtlFeat02_LoadCompanionSpritesDefault(BtlFeat02* feat) {
    CombatSprite_LoadFromTable(1, &feat->companionSpriteA, &data_ov003_020dd78c, data_ov004_020f0540, 0, 17, 0x50);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteA, 0, 0);
    CombatSprite_SetFlip(&feat->companionSpriteA, 0);

    CombatSprite_LoadFromTable(1, &feat->companionSpriteB, &data_ov003_020dd78c, data_ov004_020f0540, 1, 25, 0x50);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteB, 1, 0);
    CombatSprite_SetFlip(&feat->companionSpriteB, 0);
}

s32 BtlFeat02_Step_FadeInSlow(BtlFeat02* feat) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x2000);
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_FadeInFast(BtlFeat02* feat) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    BtlFeat02_NextState(feat);
    return 0;
}

void BtlFeat02_FadeOutSlow(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x2000);
}

void BtlFeat02_FadeOutFast(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x800);
}

s32 BtlFeat02_Step_EndTask(BtlFeat02* feat) {
    return 1;
}

s32 BtlFeat02_Step_RestoreDisplay(BtlFeat02* feat) {
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;

    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 1, 0, 0x4004);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 1, 3, 0x420C);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 1, 1, 0x404);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;
    g_DisplaySettings.controls[0].layers                  = 0x1F;

    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_256, 6, 5, 0, 0x4694);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_256, 0, 1, 0, 0x4084);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_256, 2, 3, 1, 0x428C);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_256, 4, 5, 1, 0x494);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;
    g_DisplaySettings.controls[1].layers                  = 0x1E;

    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[0].blendCoeff0 = 6;
    g_DisplaySettings.engineState[0].blendCoeff1 = 0xA;

    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 1;
    g_DisplaySettings.engineState[1].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[1].blendCoeff0 = 6;
    g_DisplaySettings.engineState[1].blendCoeff1 = 0xA;

    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 8;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x17;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;

    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowObj            = 8;
    g_DisplaySettings.engineState[1].windowObjEffects     = 0;
    g_DisplaySettings.engineState[1].windowOutside        = 0x17;
    g_DisplaySettings.engineState[1].windowOutsideEffects = 0;
    Display_Commit();
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_SetupDisplay(BtlFeat02* feat) {
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 0;
    g_DisplaySettings.engineState[0].blendLayer1 = 47;
    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 0;
    g_DisplaySettings.engineState[1].blendLayer1 = 47;

    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 3, 0, 0xC);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x420C);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 5, 1, 0x414);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic   = 0;
    g_DisplaySettings.controls[1].layers                    = 28;

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 3, 0, 0xC);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x420C);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 1, 1, 0x404);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[0].layers = 29;

    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 8;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x1F;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;

    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowObj            = 8;
    g_DisplaySettings.engineState[1].windowObjEffects     = 0;
    g_DisplaySettings.engineState[1].windowOutside        = 0x1F;
    g_DisplaySettings.engineState[1].windowOutsideEffects = 0;
    Display_Commit();
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_LoadPhase1Resources(BtlFeat02* feat) {
    feat->phasePackData = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, 5, 0);

    void* var_r1      = Data_GetPackEntryData(feat->phasePackData, 2);
    void* var_r4      = Data_GetPackEntryData(feat->phasePackData, 5);
    void* subPalette  = Data_GetPackEntryData(feat->phasePackData, 1);
    void* var_r6      = Data_GetPackEntryData(feat->phasePackData, 8);
    void* mainPalette = Data_GetPackEntryData(feat->phasePackData, 7);

    feat->subBgCharResLayerC = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1,
                                                    g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x1260);
    feat->subBgCharResLayerD = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r4,
                                                    g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, 0x68C0);

    feat->subPalette = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], subPalette, 0, 0, 0xC);

    feat->mainBgCharResLayerC = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r6,
                                                     g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, 0x5E0);

    feat->mainPalette = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], mainPalette, 0, 0, 0xC);

    void* var_r0  = Data_GetPackEntryData(feat->phasePackData, 3);
    feat->unk_354 = (void*)(var_r0 + 4);

    void* var_r0_2 = Data_GetPackEntryData(feat->phasePackData, 4);
    feat->unk_358  = (void*)(var_r0_2 + 4);

    void* var_r0_3 = Data_GetPackEntryData(feat->phasePackData, 6);
    feat->unk_360  = (void*)(var_r0_3 + 4);

    void* var_r0_4 = Data_GetPackEntryData(feat->phasePackData, 9);
    feat->unk_36C  = (void*)(var_r0_4 + 4);

    void* var_r0_5 = Data_GetPackEntryData(feat->phasePackData, 10);
    feat->unk_370  = (void*)(var_r0_5 + 4);

    func_0200d1d8(&feat->unk_2DC, 1, 2, 0, &feat->unk_354, 1, 2);
    func_0200d1d8(&feat->unk_32C, 1, 3, 0, &feat->unk_360, 1, 1);
    func_0200d1d8(&feat->unk_2B4, 0, 2, 0, &feat->unk_36C, 1, 2);

    func_0200d954(0, 3);

    func_0200d858(&feat->unk_2DC, 1, 0, 0);
    func_0200d858(&feat->unk_32C, 1, 0, 0);
    func_0200d858(&feat->unk_2B4, 1, 0, 0);

    PaletteMgr_Flush(g_PaletteManagers[1], feat->subPalette);
    PaletteMgr_Flush(g_PaletteManagers[0], feat->mainPalette);

    feat->bgLayerAOffsetH         = 0;
    feat->bgLayerAOffsetV         = 0;
    feat->bgLayerCOffsetH         = 0;
    feat->bgLayerCOffsetV         = 0;
    feat->bgLayerBOffsetH         = 0;
    feat->bgLayerBOffsetV         = 0x20000;
    feat->bgLayerDOffsetH         = 0;
    feat->bgLayerDOffsetV         = 0x20000;
    feat->pairedSpriteSeparationY = 0x50000;
    feat->pairedSpriteBaseY       = 0x1B6000;
    feat->verticalVelocity        = 0x1000;
    feat->verticalAcceleration    = 0xFFFFECCD;
    feat->subStateInterval        = 0;
    feat->shakeScreenOffset       = 0;
    feat->shakeAmpLimit           = 0x4000;
    feat->shakeDirectionToggle    = 0;
    feat->companionLayoutMode     = 0;
    feat->shakeStep               = 0x4000;
    SndMgr_StartPlayingSE(SEIDX_SE_GIRL_SP_LV3_01);
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_AdvancePhase1RiseMotion(BtlFeat02* feat) {
    feat->bgLayerAOffsetV += feat->verticalVelocity;
    feat->bgLayerCOffsetV += feat->verticalVelocity;
    feat->bgLayerBOffsetV -= 0x800;
    feat->bgLayerDOffsetV -= 0x800;
    if (feat->bgLayerDOffsetV <= 0) {
        feat->bgLayerBOffsetV = 0;
        feat->bgLayerDOffsetV = 0;
    }
    feat->verticalAcceleration += 61;
    feat->pairedSpriteBaseY -= feat->verticalVelocity;
    feat->pairedSpriteSeparationY += feat->verticalAcceleration;
    if (feat->bgLayerAOffsetV > 0x40000) {
        BtlFeat02_NextState(feat);
    }
    return 0;
}

s32 BtlFeat02_Step_DampenMotionAndStartCountdown(BtlFeat02* feat) {
    feat->verticalVelocity     = (s32)((((u64)feat->verticalVelocity * 0x2C00) + 0x800) >> 0xC);
    feat->verticalAcceleration = (s32)((((u64)feat->verticalAcceleration * 0x2C00) + 0x800) >> 0xC);
    feat->stateTimer           = 0;
    feat->shakeScreenOffset    = 0;
    feat->phaseCountdown       = 15;
    BtlFeat02_NextState(feat);
    return 2;
}

s32 BtlFeat02_Step_RunCountdownAndFadeOutMidpoint(BtlFeat02* feat) {
    feat->verticalAcceleration += 0xA9;
    feat->bgLayerAOffsetV += feat->verticalVelocity;
    feat->bgLayerCOffsetV += feat->verticalVelocity;
    if (feat->bgLayerAOffsetV > 0x5E000) {
        feat->bgLayerAOffsetV = 0x5E000;
        feat->bgLayerCOffsetV = 0x5E000;
    }
    feat->pairedSpriteBaseY -= feat->verticalVelocity;
    feat->pairedSpriteSeparationY += feat->verticalAcceleration;

    feat->phaseCountdown--;
    if (feat->phaseCountdown == 8) {
        feat->stateTimer = 1;
        BtlFeat02_FadeOutSlow();
    }
    if (feat->phaseCountdown == 0) {
        BtlFeat02_NextState(feat);
    }
    return 0;
}

s32 func_ov004_020eed18(BtlFeat02* feat, u16 arg1) {
    Data* temp_r0_3;
    s32   temp_r4;
    s32   var_r2;
    u16   temp_r0;
    u16   temp_r1;
    u16   temp_r1_2;
    u16*  var_r3;

    u16* var_r4 = data_ov004_020f05a0;
    var_r2      = 0x48;
    do {
        temp_r1 = var_r4[0];
        temp_r0 = var_r4[1];
        var_r4 += 4;
        var_r3[0] = temp_r1;
        var_r3[1] = temp_r0;
        var_r3 += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);

    if (feat->rapidFrameMainPackData != NULL) {
        DatMgr_ReleaseData(feat->rapidFrameMainPackData);
    }

    if (feat->rapidFrameSubPackData != NULL) {
        DatMgr_ReleaseData(feat->rapidFrameSubPackData);
    }

    temp_r4   = arg1 * 3;
    temp_r1_2 = (&var_r3[0])[temp_r4 + 2];
    if (temp_r1_2 != 0) {
        if (temp_r1_2 == 0xFFFF) {
            SndMgr_StartPlayingSE(SEIDX_SE_GIRL_SP_LV3_03);
        } else {
            SndMgr_PlaySEWithPan(SEIDX_SE_GIRL_SP_LV3_02);
        }
    }
    feat->rapidFrameMainPackData =
        DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, (s32) * (&var_r3[0] + (arg1 * 6)), 0);
    feat->rapidFrameSubPackData = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, (s32)(&var_r3[0])[temp_r4 + 1], 0);
    feat->unk_354               = (void*)(feat->rapidFrameMainPackData->buffer + 4);
    feat->unk_36C               = (void*)(feat->rapidFrameSubPackData->buffer + 4);
    func_0200d1d8(&feat->unk_2DC, 1, 2, 0, feat + 0x354, 1, 1);
    func_0200d1d8(&feat->unk_2B4, 0, 2, 0, feat + 0x36C, 1, 1);
    func_0200d954(0, 3);
    func_0200d858(&feat->unk_2DC, 1, 0, 0);
    func_0200d858(&feat->unk_32C, 1, 0, 0);
    func_0200d858(&feat->unk_2B4, 1, 0, 0);
    return 0x30;
}

s32 BtlFeat02_Step_LoadPhase2Resources(BtlFeat02* feat) {
    feat->phasePackData = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, 6, 0);
    func_ov004_020eed18(feat, 0);

    void* var_r1 = Data_GetPackEntryData(feat->phasePackData, 2);
    void* var_r4 = Data_GetPackEntryData(feat->phasePackData, 3);
    void* var_r5 = Data_GetPackEntryData(feat->phasePackData, 1);
    void* var_r6 = Data_GetPackEntryData(feat->phasePackData, 6);
    void* var_r7 = Data_GetPackEntryData(feat->phasePackData, 5);

    feat->subBgCharResLayerC  = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1,
                                                     g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x5200);
    feat->subBgCharResLayerD  = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r4,
                                                     g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, 0x6020);
    feat->subPalette          = PaletteMgr_AllocPalette(g_PaletteManagers[1], var_r5, 0U, 0, 0xF);
    feat->mainBgCharResLayerC = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r6,
                                                     g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, 0x6DA0);
    feat->mainPalette         = PaletteMgr_AllocPalette(g_PaletteManagers[0], var_r7, 0U, 0, 0xD);

    void* var_r0  = Data_GetPackEntryData(feat->phasePackData, 4);
    feat->unk_360 = (void*)(var_r0 + 4);

    func_0200d1d8(&feat->unk_32C, 1, 3, 0, &feat->unk_360, 1, 1);
    PaletteMgr_Flush(g_PaletteManagers[1], feat->subPalette);
    PaletteMgr_Flush(g_PaletteManagers[0], feat->mainPalette);
    feat->bgLayerAOffsetH = 0;
    feat->bgLayerAOffsetV = 0;
    feat->bgLayerCOffsetH = 0;
    feat->bgLayerCOffsetV = 0;
    feat->bgLayerBOffsetH = 0;
    feat->bgLayerBOffsetV = 0;
    feat->bgLayerDOffsetH = 0;
    feat->bgLayerDOffsetV = 0;
    BtlFeat02_LoadPhasePairedSprites(feat, 1);
    feat->pairedSpriteBaseY    = 0xC0000;
    feat->verticalAcceleration = 4506;
    feat->subStateInterval     = 0;
    feat->companionLayoutMode  = 1;

    CombatSprite_Release(&feat->companionSpriteA);
    CombatSprite_LoadFromTable(1, &feat->companionSpriteA, &data_ov003_020dd78c, data_ov004_020f0550, 0, 0x15, 0x50);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteA, 0, 0);
    CombatSprite_SetFlip(&feat->companionSpriteA, 0);

    CombatSprite_Release(&feat->companionSpriteB);
    CombatSprite_LoadFromTable(1, &feat->companionSpriteB, &data_ov003_020dd78c, data_ov004_020f0550, 1, 0x1D, 0x50);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteB, 1, 0);
    CombatSprite_SetFlip(&feat->companionSpriteB, 0);

    SndMgr_StartPlayingSE(SEIDX_SE_GIRL_SP_LV3_04);
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_AccelerateDive(BtlFeat02* feat) {
    feat->shakeAmpLimit -= 0x800;
    if (feat->shakeAmpLimit < 0) {
        feat->shakeAmpLimit = 0;
    }
    feat->verticalVelocity += 0x8CD;
    feat->pairedSpriteSeparationY += feat->verticalAcceleration;
    feat->verticalAcceleration += 0x11F;
    feat->pairedSpriteSeparationY += feat->verticalAcceleration;
    feat->pairedSpriteBaseY -= feat->verticalVelocity;
    if (feat->pairedSpriteBaseY < 0x32000) {
        feat->pairedSpriteBaseY = 0x32000;
        BtlFeat02_NextState(feat);
    }
    return 0;
}

s32 BtlFeat02_Step_BeginRapidFrameSequence(BtlFeat02* feat) {
    feat->subStateInterval   = 4;
    feat->stateTimer         = 0;
    feat->frameStepCountdown = feat->subStateInterval;
    feat->shakeAmpLimit      = 0;
    feat->shakeScreenOffset  = 0;
    CombatSprite_SetAnimFromTable(&feat->companionSpriteA, 2, 0);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteB, 3, 0);
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_UpdateRapidFrameSequence(BtlFeat02* feat) {
    if (feat->frameStepCountdown != 0) {
        feat->frameStepCountdown = feat->frameStepCountdown - 1;
    } else {
        feat->stateTimer++;
        feat->frameStepCountdown = feat->subStateInterval;
    }

    s32 temp_r0 = func_ov004_020eed18(feat, feat->stateTimer);
    if ((feat->stateTimer == (temp_r0 - 6)) && (feat->frameStepCountdown == feat->subStateInterval)) {
        BtlFeat02_FadeOutFast();
    }
    if (feat->stateTimer != (temp_r0 - 1)) {
        return 0;
    }
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_LoadFinalPhaseResources(BtlFeat02* feat) {
    feat->phasePackData = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7c4, 7, 0);
    BtlFeat02_LoadPhasePairedSprites(feat, 2);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    void* var_r1 = Data_GetPackEntryData(feat->phasePackData, 2);
    void* var_r4 = Data_GetPackEntryData(feat->phasePackData, 4);
    void* var_r5 = Data_GetPackEntryData(feat->phasePackData, 1);
    void* var_r6 = Data_GetPackEntryData(feat->phasePackData, 8);
    void* var_r7 = Data_GetPackEntryData(feat->phasePackData, 7);

    feat->subBgCharResLayerC  = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1,
                                                     g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x8000);
    feat->subBgCharResLayerD  = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r4,
                                                     g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, 0x4CA0);
    feat->subPalette          = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], var_r5, 0, 0, 0xC);
    feat->mainBgCharResLayerC = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r6,
                                                     g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, 0x8000);
    feat->mainPalette         = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], var_r7, 0, 0, 0xC);

    void* var_r0  = Data_GetPackEntryData(feat->phasePackData, 3);
    feat->unk_354 = (void*)(var_r0 + 4);

    void* var_r0_2 = Data_GetPackEntryData(feat->phasePackData, 5);
    feat->unk_360  = (void*)(var_r0_2 + 4);

    void* var_r0_3 = Data_GetPackEntryData(feat->phasePackData, 6);
    feat->unk_364  = (void*)(var_r0_3 + 4);

    void* var_r0_4 = Data_GetPackEntryData(feat->phasePackData, 9);
    feat->unk_36C  = (void*)(var_r0_4 + 4);

    func_0200d1d8(&feat->unk_2DC, 1, 2, 0, &feat->unk_354, 1, 1);
    func_0200d1d8(&feat->unk_32C, 1, 3, 0, &feat->unk_360, 1, 2);
    func_0200d1d8(&feat->unk_2B4, 0, 2, 0, &feat->unk_36C, 1, 1);
    func_0200d954(0, 3);
    func_0200d858(&feat->unk_2DC, 1, 0, 0);
    func_0200d858(&feat->unk_32C, 1, 1, 0);
    func_0200d858(&feat->unk_2B4, 1, 0, 0);
    PaletteMgr_Flush(g_PaletteManagers[1], feat->subPalette);
    PaletteMgr_Flush(g_PaletteManagers[0], feat->mainPalette);
    feat->shakeAmpLimit        = 0x4000;
    feat->shakeStep            = 0x4000;
    feat->verticalVelocity     = 0;
    feat->verticalAcceleration = feat->pairedSpriteBaseY;
    feat->stateTimer           = 180;
    feat->subStateInterval     = 0;
    func_ov003_020bc690(4, 35, 351);

    g_DisplaySettings.controls[0].layers |= 2;
    g_DisplaySettings.controls[1].layers |= 2;

    DisplayEngineState* engineMain = &g_DisplaySettings.engineState[0];
    switch (engineMain->bgSettings[1].bgMode) {
        case DISPLAY_BGMODE_AFFINE:
        case DISPLAY_BGMODE_PLTT:
        case DISPLAY_BGMODE_BMP256:
        case DISPLAY_BGMODE_BMPDIRECT:
        case DISPLAY_BGMODE_BMPLARGE:
            engineMain->bgAffines[1].unk_14 = 1;
            break;
    }
    engineMain->bgOffsets[1].hOffset = 0;
    engineMain->bgOffsets[1].vOffset = 0x8000;

    DisplayEngineState* engineSub = &g_DisplaySettings.engineState[1];
    switch (engineSub->bgSettings[1].bgMode) {
        case DISPLAY_BGMODE_AFFINE:
        case DISPLAY_BGMODE_PLTT:
        case DISPLAY_BGMODE_BMP256:
        case DISPLAY_BGMODE_BMPDIRECT:
        case DISPLAY_BGMODE_BMPLARGE:
            engineSub->bgAffines[1].unk_14 = 1;
            break;
    }
    engineSub->bgOffsets[1].hOffset = 0;
    engineSub->bgOffsets[1].vOffset = 0x8000;

    Display_Commit();
    func_ov003_020bce18(0, 0, 2, 180);
    func_ov003_020bce18(1, 0, 2, 180);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteA, 0, 0);
    CombatSprite_SetAnimFromTable(&feat->companionSpriteB, 1, 0);
    SndMgr_StartPlayingSE(SEIDX_SE_GIRL_SP_LV3_06);
    feat->shakeCallbackCooldown = 0;
    BtlFeat02_NextState(feat);
    return 0;
}

void BtlFeat02_ApplyRandomScreenShakeCallback(void* arg0) {
    // arg1->unk0->unk28 = (s32)(arg1->unk2C + ((RNG_Next(0xBU) - 5) << 0xC));
    // arg1->unk0->unk2C = (s32)(arg1->unk30 + ((RNG_Next(0xBU) - 5) << 0xC));
}

s32 BtlFeat02_Step_UpdateFinalPhaseShakeAndTimeout(BtlFeat02* feat) {
    feat->bgLayerBOffsetV += 0xB000;

    if (feat->shakeCallbackCooldown == 0) {
        func_ov003_020bd150(0, BtlFeat02_ApplyRandomScreenShakeCallback, feat);
        feat->shakeCallbackCooldown = 2;
    } else {
        feat->shakeCallbackCooldown--;
    }

    if (feat->subStateInterval != 0) {
        feat->bgLayerAOffsetV += (RNG_Next(4) << 0xC);
        feat->bgLayerCOffsetV = feat->bgLayerAOffsetV;
    } else {
        feat->bgLayerAOffsetV += ((RNG_Next(4) - 3) << 0xC);
        feat->bgLayerCOffsetV += feat->bgLayerAOffsetV;
    }

    if (feat->bgLayerAOffsetV < -0x2000) {
        feat->bgLayerAOffsetV = -0x2000;
        feat->bgLayerCOffsetV = -0x2000;
        feat->subStateInterval++;
        feat->subStateInterval &= 1;
    } else if (feat->bgLayerAOffsetV > 0x2000) {
        feat->bgLayerAOffsetV = 0x2000;
        feat->bgLayerCOffsetV = 0x2000;
        feat->subStateInterval++;
        feat->subStateInterval &= 1;
    }

    feat->pairedSpriteBaseY = feat->verticalAcceleration - feat->bgLayerCOffsetV;
    if (feat->stateTimer == 10) {
        BtlFeat02_FadeOutSlow();
    }

    if (feat->stateTimer != 0) {
        feat->stateTimer--;
        return 0;
    }
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_ReleasePhase(BtlFeat02* feat) {
    BtlFeat02_ReleasePhaseResources(feat);
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_ReleaseCompanionSprite(BtlFeat02* feat) {
    CombatSprite_Release(&feat->companionSpriteA);
    CombatSprite_Release(&feat->companionSpriteB);
    BtlFeat02_NextState(feat);
    return 0;
}

s32 BtlFeat02_Step_SetShortDelay(BtlFeat02* feat) {
    feat->stateTimer = 10;
    BtlFeat02_NextState(feat);
    return 2;
}

s32 BtlFeat02_Step_SetLongDelay(BtlFeat02* feat) {
    feat->stateTimer = 30;
    BtlFeat02_NextState(feat);
    return 2;
}

s32 BtlFeat02_Step_UpdateDelay(BtlFeat02* feat) {
    if (feat->stateTimer != 0) {
        feat->stateTimer--;
    } else {
        BtlFeat02_NextState(feat);
    }
    return 0;
}

void BtlFeat02_Reset(BtlFeat02* feat) {
    MI_CpuSet(feat, 0, sizeof(BtlFeat02));
    feat->unk_3A4 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    feat->unk_3A8 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    BtlFeat02_LoadPhasePairedSprites(feat, 0);
    BtlFeat02_LoadCompanionSpritesDefault(feat);
    BtlFeat02_SetStateTable(feat, BtlFeat02_StepTable);
}

s32 BtlFeat02_Init(TaskPool* pool, Task* task, void* args) {
    BtlFeat02* feat = task->data;

    BtlFeat02_Reset(feat);
    return 1;
}

void func_ov004_020efab8(void** arg1) {
    // (*arg1)->unk28 = 0x128000;
    // (*arg1)->unk2C = 0xDE000;
}

s32 BtlFeat02_Update(TaskPool* pool, Task* task, void* args) {
    BtlFeat02* feat = task->data;

    if (feat->stateCallbacks[feat->stateIndex] == NULL) {
        return 0;
    }

    s32 temp_r0;
    do {
        temp_r0 = feat->stateCallbacks[feat->stateIndex](feat);
    } while (temp_r0 == 2);
    if (temp_r0 == 1) {
        return 0;
    }

    func_ov003_020bd150(1, func_ov004_020efab8, feat);
    CombatSprite_Update(&feat->phaseSpriteMainLeft);
    CombatSprite_Update(&feat->phaseSpriteMainRight);
    if (SpriteMgr_IsAnimationFinished(&feat->phaseSpriteSubLeft.sprite) == 0) {
        CombatSprite_Update(&feat->phaseSpriteSubLeft);
        CombatSprite_Update(&feat->phaseSpriteSubRight);
    }
    CombatSprite_Update(&feat->companionSpriteA);
    CombatSprite_Update(&feat->companionSpriteB);

    if (feat->shakeDirectionToggle != 0) {
        feat->shakeScreenOffset += feat->shakeStep;
        if (feat->shakeScreenOffset > feat->shakeAmpLimit) {
            feat->shakeScreenOffset    = feat->shakeAmpLimit;
            feat->shakeDirectionToggle = ((feat->shakeDirectionToggle + 1) & 1);
        }
    } else {
        feat->shakeScreenOffset -= feat->shakeStep;
        if (feat->shakeScreenOffset < -feat->shakeAmpLimit) {
            feat->shakeScreenOffset    = -feat->shakeAmpLimit;
            feat->shakeDirectionToggle = ((feat->shakeDirectionToggle + 1) & 1);
        }
    }
    feat->bgLayerAOffsetH = feat->shakeScreenOffset;
    feat->bgLayerCOffsetH = feat->shakeScreenOffset;

    if (feat->unk_354 != 0) {
        s32                 layer   = feat->unk_2E0;
        DisplayEngineState* temp_ip = &g_DisplaySettings.engineState[feat->unk_2DC];

        switch (temp_ip->bgSettings[layer].bgMode) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                temp_ip->bgAffines[layer].unk_14 = 1;
                break;
        }
        temp_ip->bgOffsets[layer].hOffset = feat->bgLayerAOffsetH;
        temp_ip->bgOffsets[layer].vOffset = (feat->bgLayerAOffsetV + 0x8000);
    }

    if (feat->unk_360 != 0) {
        s32                 layer   = feat->unk_330;
        DisplayEngineState* temp_ip = &g_DisplaySettings.engineState[feat->unk_32C];

        switch (temp_ip->bgSettings[layer].bgMode) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                temp_ip->bgAffines[layer].unk_14 = 1;
                break;
        }
        temp_ip->bgOffsets[layer].hOffset = feat->bgLayerBOffsetH;
        temp_ip->bgOffsets[layer].vOffset = (feat->bgLayerBOffsetV + 0x8000);
    }

    if (feat->unk_36C != 0) {
        s32                 layer   = feat->unk_2B8;
        DisplayEngineState* temp_ip = &g_DisplaySettings.engineState[feat->unk_2B4];

        switch (temp_ip->bgSettings[layer].bgMode) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                temp_ip->bgAffines[layer].unk_14 = 1;
                break;
        }
        temp_ip->bgOffsets[layer].hOffset = feat->bgLayerCOffsetH;
        temp_ip->bgOffsets[layer].vOffset = (feat->bgLayerCOffsetV + 0x8000);
    }

    if (feat->unk_378 != 0) {
        s32                 layer   = feat->unk_308;
        DisplayEngineState* temp_ip = &g_DisplaySettings.engineState[feat->unk_304];

        switch (temp_ip->bgSettings[layer].bgMode) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                temp_ip->bgAffines[layer].unk_14 = 1;
                break;
        }
        temp_ip->bgOffsets[layer].hOffset = feat->bgLayerDOffsetH;
        temp_ip->bgOffsets[layer].vOffset = (feat->bgLayerDOffsetV + 0x8000);
    }

    return 1;
}

s32 BtlFeat02_Render(TaskPool* pool, Task* task, void* args) {
    BtlFeat02* feat = task->data;

    feat->phaseSpriteMainLeftPosX  = ((0x7C000 - feat->pairedSpriteSeparationY) + feat->shakeScreenOffset);
    feat->phaseSpriteMainRightPosX = (feat->pairedSpriteSeparationY + 0x7C000 + feat->shakeScreenOffset);
    feat->phaseSpriteSubLeftPosX   = ((0x7C000 - feat->pairedSpriteSeparationY) + feat->shakeScreenOffset);
    feat->phaseSpriteSubRightPosX  = (feat->pairedSpriteSeparationY + 0x7C000 + feat->shakeScreenOffset);
    feat->phaseSpriteMainLeftPosY  = feat->pairedSpriteBaseY;
    feat->phaseSpriteMainRightPosY = feat->pairedSpriteBaseY;
    feat->phaseSpriteSubLeftPosY   = (feat->pairedSpriteBaseY - 0x120000);
    feat->phaseSpriteSubRightPosY  = (feat->pairedSpriteBaseY - 0x120000);

    CombatSprite_SetPosition(&feat->phaseSpriteMainLeft, feat->phaseSpriteMainLeftPosX, feat->phaseSpriteMainLeftPosY);
    CombatSprite_Render(&feat->phaseSpriteMainLeft);

    CombatSprite_SetPosition(&feat->phaseSpriteMainRight, feat->phaseSpriteMainRightPosX, feat->phaseSpriteMainRightPosY);
    CombatSprite_Render(&feat->phaseSpriteMainRight);

    if (SpriteMgr_IsAnimationFinished(&feat->phaseSpriteSubLeft.sprite) == 0) {
        CombatSprite_SetPosition(&feat->phaseSpriteSubLeft, feat->phaseSpriteSubLeftPosX, feat->phaseSpriteSubLeftPosY);
        CombatSprite_Render(&feat->phaseSpriteSubLeft);
        CombatSprite_SetPosition(&feat->phaseSpriteSubRight, feat->phaseSpriteSubRightPosX, feat->phaseSpriteSubRightPosY);
        CombatSprite_Render(&feat->phaseSpriteSubRight);
    }

    s16 var_r0 =
        (feat->companionLayoutMode == 0) ? (-feat->bgLayerAOffsetV >> 0xC) + 198 : (-feat->bgLayerAOffsetV >> 0xC) + 96;
    CombatSprite_SetPosition(&feat->companionSpriteA, ((-feat->bgLayerAOffsetH >> 0xC) + 128), var_r0);
    func_ov003_02082738(&feat->companionSpriteA, 3);
    func_ov003_02082730(&feat->companionSpriteA, 0x7FFFFFFF - feat->unk_20C);
    CombatSprite_Render(&feat->companionSpriteA);

    s16 var_r0_2 =
        (feat->companionLayoutMode == 0) ? (-feat->bgLayerAOffsetV >> 0xC) + 0x60 : (feat->bgLayerAOffsetV >> 0xC) + 0xC6;
    CombatSprite_SetPosition(&feat->companionSpriteB, ((-feat->bgLayerAOffsetH >> 0xC) + 0x80), var_r0_2);
    func_ov003_02082738(&feat->companionSpriteB, 3);
    func_ov003_02082730(&feat->companionSpriteB, 0x7FFFFFFF - feat->unk_274);
    CombatSprite_Render(&feat->companionSpriteB);
    return 1;
}

s32 BtlFeat02_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlFeat02* feat = task->data;

    BtlFeat02_ReleaseAllResources(feat);
    return 0;
}

s32 BtlFeat02_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlFeat02_Init,
        .update     = BtlFeat02_Update,
        .render     = BtlFeat02_Render,
        .cleanup    = BtlFeat02_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
