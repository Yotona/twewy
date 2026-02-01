#include "Display.h"
#include <NitroSDK/fx.h>
#include <registers.h>

extern vu32 data_02066a58;

s32             data_0206aa78     = 0;
s32             data_0206aa7c     = 0;
DisplaySettings g_DisplaySettings = {0};

static void func_020063b8(DisplayEngine engine, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 hOffset, s32 vOffset);
static void func_020064cc(DisplayEngine engine, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 hOffset, s32 vOffset);

static void Display_InitControlDefaults(DisplayControlSettings* dispcnt) {
    dispcnt->dispMode    = GX_DISPMODE_GRAPHICS;
    dispcnt->bgMode      = GX_BGMODE_0;
    dispcnt->dimension   = GX2D3D_MODE_2D;
    dispcnt->objTileMode = GX_OBJTILEMODE_1D_32K;
    dispcnt->objBmpMode  = GX_OBJBMPMODE_1D_128K;
    dispcnt->displaying  = TRUE;
    dispcnt->layers      = LAYER_BG1 | LAYER_OBJ;
    dispcnt->windows     = 0;
    dispcnt->hBlank      = TRUE;
    dispcnt->screenBase  = 0;
    dispcnt->charBase    = 0;
    dispcnt->brightness  = 16;
}

static void Display_InitControls(void) {
    Display_InitControlDefaults(&g_DisplaySettings.controls[DISPLAY_MAIN]);
    Display_InitControlDefaults(&g_DisplaySettings.controls[DISPLAY_SUB]);
}

static void Display_InitEngineDefaults(DisplayEngineState* engineState) {
    engineState->bgSettings[0].bgMode         = DISPLAY_BGMODE_TEXT;
    engineState->bgSettings[0].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    engineState->bgSettings[0].colorMode      = GX_BG_COLORS_16;
    engineState->bgSettings[0].screenBase     = 0;
    engineState->bgSettings[0].charBase       = 0;
    engineState->bgSettings[0].extPlttSlot    = 1;
    engineState->bgSettings[0].priority       = 0;
    engineState->bgSettings[0].mosaic         = FALSE;

    engineState->bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    engineState->bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    engineState->bgSettings[1].colorMode      = GX_BG_COLORS_16;
    engineState->bgSettings[1].screenBase     = 0;
    engineState->bgSettings[1].charBase       = 0;
    engineState->bgSettings[1].extPlttSlot    = 1;
    engineState->bgSettings[1].priority       = 0;
    engineState->bgSettings[1].mosaic         = FALSE;

    engineState->bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    engineState->bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    engineState->bgSettings[2].colorMode      = GX_BG_COLORS_16;
    engineState->bgSettings[2].screenBase     = 0;
    engineState->bgSettings[2].charBase       = 0;
    engineState->bgSettings[2].extPlttSlot    = 1;
    engineState->bgSettings[2].priority       = 0;
    engineState->bgSettings[2].mosaic         = FALSE;

    engineState->bgSettings[3].bgMode         = DISPLAY_BGMODE_TEXT;
    engineState->bgSettings[3].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    engineState->bgSettings[3].colorMode      = GX_BG_COLORS_16;
    engineState->bgSettings[3].screenBase     = 0;
    engineState->bgSettings[3].charBase       = 0;
    engineState->bgSettings[3].extPlttSlot    = 1;
    engineState->bgSettings[3].priority       = 0;
    engineState->bgSettings[3].mosaic         = FALSE;

    engineState->bgOffsets[DISPLAY_BG0].hOffset = 0;
    engineState->bgOffsets[DISPLAY_BG0].vOffset = 0;

    engineState->bgOffsets[DISPLAY_BG1].hOffset = 0;
    engineState->bgOffsets[DISPLAY_BG1].vOffset = 0;

    engineState->bgOffsets[DISPLAY_BG2].hOffset = 0;
    engineState->bgOffsets[DISPLAY_BG2].vOffset = 0;

    engineState->bgAffines[DISPLAY_BG2].dx     = 0x100;
    engineState->bgAffines[DISPLAY_BG2].dmx    = 0;
    engineState->bgAffines[DISPLAY_BG2].dy     = 0;
    engineState->bgAffines[DISPLAY_BG2].dmy    = 0x100;
    engineState->bgAffines[DISPLAY_BG2].posX   = 0;
    engineState->bgAffines[DISPLAY_BG2].posY   = 0;
    engineState->bgAffines[DISPLAY_BG2].unk_14 = 1;
    engineState->bgAffines[DISPLAY_BG2].unk_1C = 0x1000;
    engineState->bgAffines[DISPLAY_BG2].unk_20 = 0x1000;
    engineState->bgAffines[DISPLAY_BG2].unk_24 = 0;
    engineState->bgAffines[DISPLAY_BG2].unk_28 = 0;

    engineState->bgOffsets[DISPLAY_BG3].hOffset = 0;
    engineState->bgOffsets[DISPLAY_BG3].vOffset = 0;

    engineState->bgAffines[DISPLAY_BG3].dx     = 0x100;
    engineState->bgAffines[DISPLAY_BG3].dmx    = 0;
    engineState->bgAffines[DISPLAY_BG3].dy     = 0;
    engineState->bgAffines[DISPLAY_BG3].dmy    = 0x100;
    engineState->bgAffines[DISPLAY_BG3].posX   = 0;
    engineState->bgAffines[DISPLAY_BG3].posY   = 0;
    engineState->bgAffines[DISPLAY_BG3].unk_14 = 1;
    engineState->bgAffines[DISPLAY_BG3].unk_1C = 0x1000;
    engineState->bgAffines[DISPLAY_BG3].unk_20 = 0x1000;
    engineState->bgAffines[DISPLAY_BG3].unk_24 = 0;
    engineState->bgAffines[DISPLAY_BG3].unk_28 = 0;

    engineState->window0        = 0;
    engineState->window0Effects = FALSE;

    engineState->windowOutside        = 0;
    engineState->windowOutsideEffects = FALSE;

    engineState->window1        = 0;
    engineState->window1Effects = FALSE;

    engineState->windowObj        = 0;
    engineState->windowObjEffects = FALSE;

    engineState->window0Top    = 0;
    engineState->window0Bottom = 0;
    engineState->window0Left   = 0;
    engineState->window0Right  = 0;

    engineState->window1Top    = 0;
    engineState->window1Bottom = 0;
    engineState->window1Left   = 0;
    engineState->window1Right  = 0;

    engineState->mosaicBGWidth  = 0;
    engineState->mosaicBGHeight = 0;

    engineState->mosaicOBJWidth  = 0;
    engineState->mosaicOBJHeight = 0;

    engineState->blendMode       = 0;
    engineState->blendLayer0     = 0;
    engineState->blendLayer1     = 0;
    engineState->blendCoeff1     = 0;
    engineState->blendBrightness = 0;
}

static void Display_InitEngines(void) {
    Display_InitEngineDefaults(&g_DisplaySettings.engineState[DISPLAY_MAIN]);
    Display_InitEngineDefaults(&g_DisplaySettings.engineState[DISPLAY_SUB]);
}

void Display_Init(void) {
    Display_InitControls();
    Display_InitEngines();
}

static void Display_ApplyControls(void) {
    DisplayControlSettings* mainSettings = Display_GetMainControls();

    if (mainSettings->displaying == TRUE) {
        GX_DispOn();
    } else {
        GX_DispOff();
    }

    u32 layers = mainSettings->layers;
    if (((data_02066a58 << 0x1C) >> 0x1F) != 0) {
        layers |= LAYER_OBJ;
    }

    GX_SetGraphicsMode(mainSettings->dispMode, mainSettings->bgMode, mainSettings->dimension);
    GX_SetOBJTileMode(mainSettings->objTileMode);
    GX_SetOBJBmpMode(mainSettings->objBmpMode);
    GX_SetVisibleLayers(layers);
    GX_SetVisibleWindows(mainSettings->windows);

    if (mainSettings->hBlank != 0) {
        REG_DISPCNT_GET()->hblankProcess = TRUE;
    } else {
        REG_DISPCNT_GET()->hblankProcess = FALSE;
    }

    GX_SetScreenBase(mainSettings->screenBase);
    GX_SetCharBase(mainSettings->charBase);

    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT, mainSettings->brightness);

    DisplayControlSettings* subSettings = Display_GetSubControls();

    if (subSettings->displaying == TRUE) {
        REG_DISPCNT_SUB_GET()->dispMode = GX_DISPMODE_GRAPHICS;
    } else {
        REG_DISPCNT_SUB_GET()->dispMode = GX_DISPMODE_OFF;
    }

    layers = subSettings->layers;
    if (((u32)(data_02066a58 << 0x1C) >> 0x1F) != 0) {
        layers |= LAYER_OBJ;
    }

    GXs_SetGraphicsMode(subSettings->bgMode);
    GXs_SetOBJTileMode(subSettings->objTileMode);
    GXs_SetOBJBmpMode(subSettings->objBmpMode);
    GXs_SetVisibleLayers(layers);
    GXs_SetVisibleWindows(subSettings->windows);

    if (subSettings->hBlank != 0) {
        REG_DISPCNT_SUB_GET()->hblankProcess = TRUE;
    } else {
        REG_DISPCNT_SUB_GET()->hblankProcess = FALSE;
    }

    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT_SUB, subSettings->brightness);
}

// Nonmatching: Several register swaps
// Scratch: wgpMk
static void Display_ApplyBGLayer(DisplayEngine engine, DisplayBGLayer layer) {
    DisplayBGSettings* bgSettings = Display_GetBGSettings(engine, layer);

    switch (layer) {
        case DISPLAY_BG0: {
            DisplayBGSettings* bg0         = Display_GetBG0Settings(engine);
            s32                extPlttSlot = bgSettings->extPlttSlot;
            s32                charBase    = bgSettings->charBase;
            s32                screenBase  = bgSettings->screenBase;
            s32                colorMode   = bgSettings->colorMode;
            s32                screenSize  = bgSettings->screenSizeText;

            bg0->bgMode         = DISPLAY_BGMODE_TEXT;
            bg0->screenSizeText = screenSize;
            bg0->colorMode      = colorMode;
            bg0->screenBase     = screenBase;
            bg0->charBase       = charBase;
            bg0->extPlttSlot    = extPlttSlot;

            switch (engine) {
                case DISPLAY_MAIN:
                    if (g_DisplaySettings.controls[DISPLAY_MAIN].dimension == GX2D3D_MODE_2D) {
                        GX_SetBG0Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    }
                    return;
                case DISPLAY_SUB:
                    GXs_SetBG0Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    return;
            }
        } break;

        case DISPLAY_BG1: {
            DisplayBGSettings* bg1         = Display_GetBG1Settings(engine);
            s32                extPlttSlot = bgSettings->extPlttSlot;
            s32                charBase    = bgSettings->charBase;
            s32                screenBase  = bgSettings->screenBase;
            s32                colorMode   = bgSettings->colorMode;
            s32                screenSize  = bgSettings->screenSizeText;

            bg1->bgMode         = DISPLAY_BGMODE_TEXT;
            bg1->screenSizeText = screenSize;
            bg1->colorMode      = colorMode;
            bg1->screenBase     = screenBase;
            bg1->charBase       = charBase;
            bg1->extPlttSlot    = extPlttSlot;

            switch (engine) {
                case DISPLAY_MAIN:
                    GX_SetBG1Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    return;
                case DISPLAY_SUB:
                    GXs_SetBG1Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    return;
            }
        } break;

        case DISPLAY_BG2: {
            switch (bgSettings->bgMode) {
                case DISPLAY_BGMODE_TEXT: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    s32                charBase   = bgSettings->charBase;
                    s32                screenBase = bgSettings->screenBase;
                    s32                colorMode  = bgSettings->colorMode;
                    s32                screenSize = bgSettings->screenSizeText;

                    bg2->bgMode         = DISPLAY_BGMODE_TEXT;
                    bg2->screenSizeText = screenSize;
                    bg2->colorMode      = colorMode;
                    bg2->screenBase     = screenBase;
                    bg2->charBase       = charBase;
                    bg2->extPlttSlot    = 1;

                    switch (engine) {
                        case DISPLAY_MAIN:
                            GX_SetBG2ControlText(screenSize, colorMode, screenBase, charBase);
                            return;
                        case DISPLAY_SUB:
                            GXs_SetBG2ControlText(screenSize, colorMode, screenBase, charBase);
                            return;
                    }
                } break;

                case DISPLAY_BGMODE_AFFINE: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    s32                charBase   = bgSettings->charBase;
                    s32                screenBase = bgSettings->screenBase;
                    GXBGOverflow       overflow   = bgSettings->overflow;
                    s32                screenSize = bgSettings->screenSizeAffine;

                    bg2->bgMode           = DISPLAY_BGMODE_AFFINE;
                    bg2->screenSizeAffine = screenSize;
                    bg2->overflow         = overflow;
                    bg2->screenBase       = screenBase;
                    bg2->charBase         = charBase;

                    switch (engine) {
                        case DISPLAY_MAIN:
                            GX_SetBG2ControlAffine(screenSize, overflow, screenBase, charBase);
                            return;
                        case DISPLAY_SUB:
                            GXs_SetBG2ControlAffine(screenSize, overflow, screenBase, charBase);
                            return;
                    }
                } break;

                case DISPLAY_BGMODE_PLTT: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    s32                charBase   = bgSettings->charBase;
                    s32                screenBase = bgSettings->screenBase;
                    GXBGOverflow       overflow   = bgSettings->overflow;
                    GXBGScreenSizePltt screenSize = bgSettings->screenSizePltt;

                    bg2->bgMode         = DISPLAY_BGMODE_PLTT;
                    bg2->screenSizePltt = screenSize;
                    bg2->overflow       = overflow;
                    bg2->screenBase     = screenBase;
                    bg2->charBase       = charBase;
                    bg2->extPlttSlot    = 1;

                    switch (engine) {
                        case DISPLAY_MAIN:
                            GX_SetBG2ControlPltt(screenSize, overflow, screenBase, charBase);
                            return;
                        case DISPLAY_SUB:
                            GXs_SetBG2ControlPltt(screenSize, overflow, screenBase, charBase);
                            return;
                    }
                } break;

                case DISPLAY_BGMODE_BMP256: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    u32                screenBase = bgSettings->screenBaseBmp;
                    GXBGOverflow       overflow   = bgSettings->overflow;
                    GXBGScreenSizeBmp  screenSize = bgSettings->screenSizeBmp256;

                    bg2->bgMode           = DISPLAY_BGMODE_BMP256;
                    bg2->screenSizeBmp256 = screenSize;
                    bg2->overflow         = overflow;
                    bg2->screenBaseBmp    = screenBase;

                    switch (engine) {
                        case DISPLAY_MAIN:
                            GX_SetBG2ControlBmp256(screenSize, overflow, screenBase);
                            return;
                        case DISPLAY_SUB:
                            GXs_SetBG2ControlBmp256(screenSize, overflow, screenBase);
                            return;
                    }
                } break;

                case DISPLAY_BGMODE_BMPDIRECT: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    u32                screenBase = bgSettings->screenBaseBmp;
                    GXBGOverflow       overflow   = bgSettings->overflow;
                    GXBGScreenSizeBmp  screenSize = bgSettings->screenSizeBmpDirect;

                    bg2->bgMode              = DISPLAY_BGMODE_BMPDIRECT;
                    bg2->screenSizeBmpDirect = screenSize;
                    bg2->overflow            = overflow;
                    bg2->screenBaseBmp       = screenBase;

                    switch (engine) {
                        case DISPLAY_MAIN:
                            GX_SetBG2ControlBmpDirect(screenSize, overflow, screenBase);
                            return;
                        case DISPLAY_SUB:
                            GXs_SetBG2ControlBmpDirect(screenSize, overflow, screenBase);
                            return;
                    }
                } break;

                case DISPLAY_BGMODE_BMPLARGE: {
                    if (engine == DISPLAY_MAIN) {
                        DisplayBGSettings* bg2 = Display_GetBG2Settings(DISPLAY_MAIN);

                        GXBGOverflow           overflow   = bgSettings->overflow;
                        GXBGScreenSizeBmpLarge screenSize = bgSettings->screenSizeBmpLarge;

                        bg2->bgMode             = DISPLAY_BGMODE_BMPLARGE;
                        bg2->overflow           = overflow;
                        bg2->screenSizeBmpLarge = screenSize;
                        GX_SetBGControlBmpLarge(screenSize, overflow);
                        return;
                    }
                } break;
            }
        } break;

        case DISPLAY_BG3:
            return;
    }
}

// Nonmatching: Work in progress, many stack and inlined bitshifting differences
static void Display_ApplyRenderState(void) {
    DisplayControlSettings* controls  = Display_GetMainControls();
    DisplayEngineState*     mainState = &g_DisplaySettings.engineState[DISPLAY_MAIN];

    GX_SetBG0Priority(mainState->bgSettings[DISPLAY_BG0].priority);
    GX_SetBG1Priority(mainState->bgSettings[DISPLAY_BG1].priority);
    GX_SetBG2Priority(mainState->bgSettings[DISPLAY_BG2].priority);
    GX_SetBG3Priority(mainState->bgSettings[DISPLAY_BG3].priority);

    Display_ApplyBGLayer(DISPLAY_MAIN, DISPLAY_BG0);
    Display_ApplyBGLayer(DISPLAY_MAIN, DISPLAY_BG1);
    Display_ApplyBGLayer(DISPLAY_MAIN, DISPLAY_BG2);
    Display_ApplyBGLayer(DISPLAY_MAIN, DISPLAY_BG3);

    GX_SetBG0Mosaic(mainState->bgSettings[DISPLAY_BG0].mosaic);
    GX_SetBG1Mosaic(mainState->bgSettings[DISPLAY_BG1].mosaic);
    GX_SetBG2Mosaic(mainState->bgSettings[DISPLAY_BG2].mosaic);
    GX_SetBG3Mosaic(mainState->bgSettings[DISPLAY_BG3].mosaic);

    if (controls->layers & LAYER_BG0) {

        // TODO: This if statement compiles as though being accessed via 0x0206aa78
        if (Display_GetMainControls()->dimension == GX2D3D_MODE_2D) {
            GX_SetBG0Offset(mainState->bgOffsets[DISPLAY_BG0].hOffset >> 0xC,
                            mainState->bgOffsets[DISPLAY_BG0].vOffset >> 0xC);
        } else {
            func_02036f78(mainState->bgOffsets[DISPLAY_BG0].hOffset >> 0xC);
        }
    }

    if (controls->layers & LAYER_BG1) {
        GX_SetBG1Offset(mainState->bgOffsets[DISPLAY_BG1].hOffset >> 0xC, mainState->bgOffsets[DISPLAY_BG1].vOffset >> 0xC);
    }

    if ((controls->layers & LAYER_BG2) && (mainState->bgSettings[DISPLAY_BG2].bgMode == DISPLAY_BGMODE_TEXT)) {
        GX_SetBG2Offset(mainState->bgOffsets[DISPLAY_BG2].hOffset >> 0xC, mainState->bgOffsets[DISPLAY_BG2].vOffset >> 0xC);
    }

    if ((controls->layers & LAYER_BG3) && (mainState->bgSettings[DISPLAY_BG3].bgMode == DISPLAY_BGMODE_TEXT)) {
        GX_SetBG3Offset(mainState->bgOffsets[DISPLAY_BG3].hOffset >> 0xC, mainState->bgOffsets[DISPLAY_BG3].vOffset >> 0xC);
    }

    GX_SetWindow0Inside(mainState->window0, mainState->window0Effects);
    GX_SetWindow1Inside(mainState->window1, mainState->window1Effects);
    GX_SetWindowOutside(mainState->windowOutside, mainState->windowOutsideEffects);
    GX_SetWindowObjInside(mainState->windowObj, mainState->windowObjEffects);

    GX_SetWindow0Position(mainState->window0Left >> 12, mainState->window0Top >> 12, mainState->window0Right >> 12,
                          mainState->window0Bottom >> 12);
    GX_SetWindow1Position(mainState->window1Left >> 12, mainState->window1Top >> 12, mainState->window1Right >> 12,
                          mainState->window1Bottom >> 12);

    GX_SetMosaicSizeBG(mainState->mosaicBGWidth, mainState->mosaicBGHeight);
    GX_SetMosaicSizeOBJ(mainState->mosaicOBJWidth, mainState->mosaicOBJHeight);

    switch (mainState->blendMode) {
        case 0:
        case 1:
            G2x_SetBlendAlpha(&REG_BLDCNT, mainState->blendLayer0, mainState->blendLayer1, mainState->blendCoeff0,
                              mainState->blendCoeff1);
            break;
        case 2:
            func_02036a70(&REG_BLDCNT, mainState->blendLayer0, mainState->blendLayer1, mainState->blendCoeff0,
                          mainState->blendCoeff1, mainState->blendBrightness);
            break;
        case 3:
            func_02036a70(&REG_BLDCNT, mainState->blendLayer0, mainState->blendLayer1, mainState->blendCoeff0,
                          mainState->blendCoeff1, -mainState->blendBrightness);
            break;
    }

    if (mainState->bgAffines[DISPLAY_BG2].unk_14 == 1) {
        mainState->bgAffines[DISPLAY_BG2].unk_14 = 0;
        func_020063b8(DISPLAY_MAIN, mainState->bgAffines[DISPLAY_BG2].unk_18, mainState->bgAffines[DISPLAY_BG2].unk_1C,
                      mainState->bgAffines[DISPLAY_BG2].unk_20, mainState->bgAffines[DISPLAY_BG2].unk_24,
                      mainState->bgAffines[DISPLAY_BG2].unk_28, mainState->bgOffsets[DISPLAY_BG2].hOffset,
                      mainState->bgOffsets[DISPLAY_BG2].vOffset);
    }

    REG_BG2PA = mainState->bgAffines[DISPLAY_BG2].dx;
    REG_BG2PB = mainState->bgAffines[DISPLAY_BG2].dmx;
    REG_BG2PC = mainState->bgAffines[DISPLAY_BG2].dy;
    REG_BG2PD = mainState->bgAffines[DISPLAY_BG2].dmy;

    REG_BG2X = mainState->bgAffines[DISPLAY_BG2].posX;
    REG_BG2Y = mainState->bgAffines[DISPLAY_BG2].posY;

    if (mainState->bgAffines[DISPLAY_BG3].unk_14 == 1) {
        mainState->bgAffines[DISPLAY_BG3].unk_14 = 0;
        func_020064cc(DISPLAY_MAIN, mainState->bgAffines[DISPLAY_BG3].unk_18, mainState->bgAffines[DISPLAY_BG3].unk_1C,
                      mainState->bgAffines[DISPLAY_BG3].unk_20, mainState->bgAffines[DISPLAY_BG3].unk_24,
                      mainState->bgAffines[DISPLAY_BG3].unk_28, mainState->bgOffsets[DISPLAY_BG3].hOffset,
                      mainState->bgOffsets[DISPLAY_BG3].vOffset);
    }
    REG_BG3PA = mainState->bgAffines[DISPLAY_BG3].dx;
    REG_BG3PB = mainState->bgAffines[DISPLAY_BG3].dmx;
    REG_BG3PC = mainState->bgAffines[DISPLAY_BG3].dy;
    REG_BG3PD = mainState->bgAffines[DISPLAY_BG3].dmy;

    REG_BG3X = mainState->bgAffines[DISPLAY_BG3].posX;
    REG_BG3Y = mainState->bgAffines[DISPLAY_BG3].posY;

    controls                     = Display_GetSubControls();
    DisplayEngineState* subState = &g_DisplaySettings.engineState[DISPLAY_SUB];

    GXs_SetBG0Priority(subState->bgSettings[DISPLAY_BG0].priority);
    GXs_SetBG1Priority(subState->bgSettings[DISPLAY_BG1].priority);
    GXs_SetBG2Priority(subState->bgSettings[DISPLAY_BG2].priority);
    GXs_SetBG3Priority(subState->bgSettings[DISPLAY_BG3].priority);

    Display_ApplyBGLayer(DISPLAY_SUB, DISPLAY_BG0);
    Display_ApplyBGLayer(DISPLAY_SUB, DISPLAY_BG1);
    Display_ApplyBGLayer(DISPLAY_SUB, DISPLAY_BG2);
    Display_ApplyBGLayer(DISPLAY_SUB, DISPLAY_BG3);

    GXs_SetBG0Mosaic(subState->bgSettings[DISPLAY_BG0].mosaic);
    GXs_SetBG1Mosaic(subState->bgSettings[DISPLAY_BG1].mosaic);
    GXs_SetBG2Mosaic(subState->bgSettings[DISPLAY_BG2].mosaic);
    GXs_SetBG3Mosaic(subState->bgSettings[DISPLAY_BG3].mosaic);

    if (controls->layers & 1) {
        GXs_SetBG0Offset(subState->bgOffsets[DISPLAY_BG0].hOffset >> 0xC, subState->bgOffsets[DISPLAY_BG0].vOffset >> 0xC);
    }

    if (controls->layers & 2) {
        GXs_SetBG1Offset(subState->bgOffsets[DISPLAY_BG1].hOffset >> 0xC, subState->bgOffsets[DISPLAY_BG1].vOffset >> 0xC);
    }

    if ((controls->layers & 4) && (subState->bgSettings[2].bgMode == 0)) {
        GXs_SetBG2Offset(subState->bgOffsets[DISPLAY_BG2].hOffset >> 0xC, subState->bgOffsets[DISPLAY_BG2].vOffset >> 0xC);
    }

    if ((controls->layers & 8) && (subState->bgSettings[3].bgMode == 0)) {
        GXs_SetBG3Offset(subState->bgOffsets[DISPLAY_BG3].hOffset >> 0xC, subState->bgOffsets[DISPLAY_BG3].vOffset >> 0xC);
    }

    GXs_SetWindow0Inside(subState->window0, subState->window0Effects);
    GXs_SetWindow1Inside(subState->window1, subState->window1Effects);
    GXs_SetWindowOutside(subState->windowOutside, subState->windowOutsideEffects);
    GXs_SetWindowObjInside(subState->windowObj, subState->windowObjEffects);

    GXs_SetWindow0Position(subState->window0Left >> 12, subState->window0Top >> 12, subState->window0Right >> 12,
                           subState->window0Bottom >> 12);
    GXs_SetWindow1Position(subState->window1Left >> 12, subState->window1Top >> 12, subState->window1Right >> 12,
                           subState->window1Bottom >> 12);

    GXs_SetMosaicSizeBG(subState->mosaicBGWidth, subState->mosaicBGHeight);
    GXs_SetMosaicSizeOBJ(subState->mosaicOBJWidth, subState->mosaicOBJHeight);

    switch (subState->blendMode) {
        default:
            break;
        case 0:
        case 1:
            G2x_SetBlendAlpha(&REG_BLDCNT_SUB, subState->blendLayer0, subState->blendLayer1, subState->blendCoeff0,
                              subState->blendCoeff1);
            break;
        case 2:
            func_02036a70(&REG_BLDCNT_SUB, subState->blendLayer0, subState->blendLayer1, subState->blendCoeff0,
                          subState->blendCoeff1, subState->blendBrightness);
            break;
        case 3:
            func_02036a70(&REG_BLDCNT_SUB, subState->blendLayer0, subState->blendLayer1, subState->blendCoeff0,
                          subState->blendCoeff1, 0 - subState->blendBrightness);
            break;
    }
    if (subState->bgAffines[DISPLAY_BG2].unk_14 == 1) {
        subState->bgAffines[DISPLAY_BG2].unk_14 = 0;
        func_020063b8(DISPLAY_SUB, subState->bgAffines[DISPLAY_BG2].unk_18, subState->bgAffines[DISPLAY_BG2].unk_1C,
                      subState->bgAffines[DISPLAY_BG2].unk_20, subState->bgAffines[DISPLAY_BG2].unk_24,
                      subState->bgAffines[DISPLAY_BG2].unk_28, subState->bgOffsets[DISPLAY_BG2].hOffset,
                      subState->bgOffsets[DISPLAY_BG2].vOffset);
    }

    REG_BG2PA_SUB = subState->bgAffines[DISPLAY_BG2].dx;
    REG_BG2PB_SUB = subState->bgAffines[DISPLAY_BG2].dmx;
    REG_BG2PC_SUB = subState->bgAffines[DISPLAY_BG2].dy;
    REG_BG2PD_SUB = subState->bgAffines[DISPLAY_BG2].dmy;
    REG_BG2X_SUB  = subState->bgAffines[DISPLAY_BG2].posX;
    REG_BG2Y_SUB  = subState->bgAffines[DISPLAY_BG2].posY;

    if (subState->bgAffines[DISPLAY_BG3].unk_14 == 1) {
        subState->bgAffines[DISPLAY_BG3].unk_14 = 0;
        func_020064cc(DISPLAY_SUB, subState->bgAffines[DISPLAY_BG3].unk_18, subState->bgAffines[DISPLAY_BG3].unk_1C,
                      subState->bgAffines[DISPLAY_BG3].unk_20, subState->bgAffines[DISPLAY_BG3].unk_24,
                      subState->bgAffines[DISPLAY_BG3].unk_28, subState->bgOffsets[DISPLAY_BG3].hOffset,
                      subState->bgOffsets[DISPLAY_BG3].vOffset);
    }

    REG_BG3PA_SUB = subState->bgAffines[DISPLAY_BG3].dx;
    REG_BG3PB_SUB = subState->bgAffines[DISPLAY_BG3].dmx;
    REG_BG3PC_SUB = subState->bgAffines[DISPLAY_BG3].dy;
    REG_BG3PD_SUB = subState->bgAffines[DISPLAY_BG3].dmy;
    REG_BG3X_SUB  = subState->bgAffines[DISPLAY_BG3].posX;
    REG_BG3Y_SUB  = subState->bgAffines[DISPLAY_BG3].posY;
}

void Display_Commit(void) {
    Display_ApplyControls();
    Display_ApplyRenderState();
}

void Display_CommitSynced(void) {
    do {
    } while (REG_VCOUNT < 0xC0);
    Display_Commit();
}

void func_020063b8(DisplayEngine engine, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 hOffset, s32 vOffset) {}

void func_020064cc(DisplayEngine engine, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 hOffset, s32 vOffset) {}
