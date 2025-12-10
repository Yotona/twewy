#include "Display.h"
#include <NitroSDK/fx.h>
#include <registers.h>

s32             data_0206aa78 = 0;
s32             data_0206aa7c = 0;
DisplaySettings data_0206aa80 = {0};

static void func_020063b8(s32 arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
static void func_020064cc(s32 arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

extern vu32 data_02066a58;

static void func_0200504c(DisplayControlSettings* dispcnt) {
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

static void func_02005094(void) {
    func_0200504c(&data_0206aa80.mainControl);
    func_0200504c(&data_0206aa80.subControl);
}

static void func_020050b4(UnkStruct_usedby_20050B4* arg0) {
    arg0->bgSettings[0].unk_00      = 0;
    arg0->bgSettings[0].screenSize  = 0;
    arg0->bgSettings[0].colorMode   = 0;
    arg0->bgSettings[0].screenBase  = 0;
    arg0->bgSettings[0].charBase    = 0;
    arg0->bgSettings[0].extPlttSlot = 1;
    arg0->bgSettings[0].unk_34      = 0;
    arg0->bgSettings[0].unk_38      = 0;

    arg0->bgSettings[1].unk_00      = 0;
    arg0->bgSettings[1].screenSize  = 0;
    arg0->bgSettings[1].colorMode   = 0;
    arg0->bgSettings[1].screenBase  = 0;
    arg0->bgSettings[1].charBase    = 0;
    arg0->bgSettings[1].extPlttSlot = 1;
    arg0->bgSettings[1].unk_34      = 0;
    arg0->bgSettings[1].unk_38      = 0;

    arg0->bgSettings[2].unk_00      = 0;
    arg0->bgSettings[2].screenSize  = 0;
    arg0->bgSettings[2].colorMode   = 0;
    arg0->bgSettings[2].screenBase  = 0;
    arg0->bgSettings[2].charBase    = 0;
    arg0->bgSettings[2].extPlttSlot = 1;
    arg0->bgSettings[2].unk_34      = 0;
    arg0->bgSettings[2].unk_38      = 0;

    arg0->bgSettings[3].unk_00      = 0;
    arg0->bgSettings[3].screenSize  = 0;
    arg0->bgSettings[3].colorMode   = 0;
    arg0->bgSettings[3].screenBase  = 0;
    arg0->bgSettings[3].charBase    = 0;
    arg0->bgSettings[3].extPlttSlot = 1;
    arg0->bgSettings[3].unk_34      = 0;
    arg0->bgSettings[3].unk_38      = 0;

    arg0->unk_0F0 = 0;
    arg0->unk_0F4 = 0;
    arg0->unk_0F8 = 0;
    arg0->unk_0FC = 0;
    arg0->unk_100 = 0;
    arg0->unk_104 = 0;
    arg0->unk_168 = 0x100;
    arg0->unk_16A = 0;
    arg0->unk_16C = 0;
    arg0->unk_16E = 0x100;
    arg0->unk_170 = 0;
    arg0->unk_174 = 0;
    arg0->unk_17C = 1;
    arg0->unk_184 = 0x1000;
    arg0->unk_188 = 0x1000;
    arg0->unk_18C = 0;
    arg0->unk_190 = 0;
    arg0->unk_108 = 0;
    arg0->unk_10C = 0;
    arg0->unk_194 = 0x100;
    arg0->unk_196 = 0;
    arg0->unk_198 = 0;
    arg0->unk_19A = 0x100;
    arg0->unk_19C = 0;
    arg0->unk_1A0 = 0;
    arg0->unk_1A8 = 1;
    arg0->unk_1B0 = 0x1000;
    arg0->unk_1B4 = 0x1000;
    arg0->unk_1B8 = 0;
    arg0->unk_1BC = 0;

    arg0->window0        = 0;
    arg0->window0Effects = FALSE;

    arg0->windowOutside        = 0;
    arg0->windowOutsideEffects = FALSE;

    arg0->window1        = 0;
    arg0->window1Effects = FALSE;

    arg0->windowObj        = 0;
    arg0->windowObjEffects = FALSE;

    arg0->unk_1D0 = 0;
    arg0->unk_1D4 = 0;
    arg0->unk_1DC = 0;
    arg0->unk_1D8 = 0;
    arg0->unk_1F0 = 0;
    arg0->unk_1F4 = 0;
    arg0->unk_1FC = 0;
    arg0->unk_1F8 = 0;
    arg0->unk_200 = 0;
    arg0->unk_202 = 0;
    arg0->unk_204 = 0;
    arg0->unk_206 = 0;
    arg0->unk_208 = 0;
    arg0->unk_20C = 0;
    arg0->unk_210 = 0;
    arg0->unk_218 = 0;
    arg0->unk_21C = 0;
}

static void func_02005230(void) {
    func_020050b4(&data_0206aa80.unk_064[0]);
    func_020050b4(&data_0206aa80.unk_064[1]);
}

void func_02005250(void) {
    func_02005094();
    func_02005230();
}

// Nonmatching: Register swap
// Scratch: l0nuV
static void func_02005260(void) {
    DisplayControlSettings* mainSettings = Display_GetMainControl();

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

    DisplayControlSettings* subSettings = Display_GetSubControl();

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

// Nonmatching: Several register swaps, some extra instructions
// Scratch: wgpMk
void func_02005444(DisplayEngine engine, DisplayBGLayer layer) {
    DisplayBGSettings* bgSettings = Display_GetBGSettings(engine, layer);

    switch (layer) {
        case DISPLAY_BG0: {
            DisplayBGSettings* bg0         = Display_GetBG0Settings(engine);
            s32                extPlttSlot = bgSettings->extPlttSlot;
            s32                charBase    = bgSettings->charBase;
            s32                screenBase  = bgSettings->screenBase;
            s32                colorMode   = bgSettings->colorMode;
            s32                screenSize  = bgSettings->screenSize;

            bg0->unk_00      = 0;
            bg0->screenSize  = screenSize;
            bg0->colorMode   = colorMode;
            bg0->screenBase  = screenBase;
            bg0->charBase    = charBase;
            bg0->extPlttSlot = extPlttSlot;

            switch (engine) {
                case DISPLAY_ENGINE_MAIN:
                    if (data_0206aa80.mainControl.dimension == GX2D3D_MODE_2D) {
                        GX_SetBG0Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    }
                    return;
                case DISPLAY_ENGINE_SUB:
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
            s32                screenSize  = bgSettings->screenSize;

            bg1->unk_00      = 0;
            bg1->screenSize  = screenSize;
            bg1->colorMode   = colorMode;
            bg1->screenBase  = screenBase;
            bg1->charBase    = charBase;
            bg1->extPlttSlot = extPlttSlot;

            switch (engine) {
                case 0:
                    GX_SetBG1Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    return;
                case 1:
                    GXs_SetBG1Control(screenSize, colorMode, screenBase, charBase, extPlttSlot);
                    return;
            }
        } break;

        case DISPLAY_BG2: {

            switch (bgSettings->unk_00) {
                case 0: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    s32                charBase   = bgSettings->charBase;
                    s32                screenBase = bgSettings->screenBase;
                    s32                colorMode  = bgSettings->colorMode;
                    s32                screenSize = bgSettings->screenSize;

                    bg2->unk_00      = 0;
                    bg2->screenSize  = screenSize;
                    bg2->colorMode   = colorMode;
                    bg2->screenBase  = screenBase;
                    bg2->charBase    = charBase;
                    bg2->extPlttSlot = 1;

                    switch (engine) {
                        case DISPLAY_ENGINE_MAIN:
                            GX_SetBG2ControlText(screenSize, colorMode, screenBase, charBase);
                            return;
                        case DISPLAY_ENGINE_SUB:
                            GXs_SetBG2ControlText(screenSize, colorMode, screenBase, charBase);
                            return;
                    }
                } break;

                case 1: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    s32                charBase   = bgSettings->charBase;
                    s32                screenBase = bgSettings->screenBase;
                    s32                unk30      = bgSettings->unk_30;
                    s32                unk08      = bgSettings->unk_08;

                    bg2->unk_00     = 1;
                    bg2->unk_08     = unk08;
                    bg2->unk_30     = unk30;
                    bg2->screenBase = screenBase;
                    bg2->charBase   = charBase;

                    switch (engine) {
                        case DISPLAY_ENGINE_MAIN:
                            GX_SetBG2ControlAffine(unk08, unk30, screenBase, charBase);
                            return;
                        case DISPLAY_ENGINE_SUB:
                            GXs_SetBG2ControlAffine(unk08, unk30, screenBase, charBase);
                            return;
                    }
                } break;

                case 2: {
                    DisplayBGSettings* bg2        = Display_GetBG2Settings(engine);
                    s32                charBase   = bgSettings->charBase;
                    s32                screenBase = bgSettings->screenBase;
                    s32                unk30      = bgSettings->unk_30;
                    s32                unk0C      = bgSettings->unk_0C;

                    bg2->unk_00      = 2;
                    bg2->unk_0C      = unk0C;
                    bg2->unk_30      = unk30;
                    bg2->screenBase  = screenBase;
                    bg2->charBase    = charBase;
                    bg2->extPlttSlot = 1;

                    switch (engine) {
                        case DISPLAY_ENGINE_MAIN:
                            GX_SetBG2ControlPltt(unk0C, unk30, screenBase, charBase);
                            return;
                        case DISPLAY_ENGINE_SUB:
                            GXs_SetBG2ControlPltt(unk0C, unk30, screenBase, charBase);
                            return;
                    }
                } break;

                case 3: {
                    DisplayBGSettings* bg2   = Display_GetBG2Settings(engine);
                    s32                unk24 = bgSettings->unk_24;
                    s32                unk30 = bgSettings->unk_30;
                    s32                unk10 = bgSettings->unk_10;

                    bg2->unk_00 = 3;
                    bg2->unk_10 = unk10;
                    bg2->unk_30 = unk30;
                    bg2->unk_24 = unk24;

                    switch (engine) {
                        case DISPLAY_ENGINE_MAIN:
                            REG_BG2CNT = (u16)((REG_BG2CNT & 0x43) | (unk10 << 0xE) | 0x80 | (unk24 << 8) | (unk30 << 0xD));
                            return;
                        case DISPLAY_ENGINE_SUB:
                            REG_BG2CNT_SUB =
                                (u16)((REG_BG2CNT_SUB & 0x43) | (unk10 << 0xE) | 0x80 | (unk24 << 8) | (unk30 << 0xD));
                            return;
                    }
                } break;

                case 4: {
                    DisplayBGSettings* bg2   = Display_GetBG2Settings(engine);
                    s32                unk24 = bgSettings->unk_24;
                    s32                unk30 = bgSettings->unk_30;
                    s32                unk14 = bgSettings->unk_14;

                    bg2->unk_00 = 4;
                    bg2->unk_14 = unk14;
                    bg2->unk_30 = unk30;
                    bg2->unk_24 = unk24;

                    switch (engine) {
                        case DISPLAY_ENGINE_MAIN:
                            REG_BG2CNT = (u16)((REG_BG2CNT & 0x43) | (unk14 << 0xE) | 0x88 | (unk24 << 8) | (unk30 << 0xD));
                            return;
                        case DISPLAY_ENGINE_SUB:
                            REG_BG2CNT_SUB =
                                (u16)((REG_BG2CNT_SUB & 0x43) | (unk14 << 0xE) | 0x88 | (unk24 << 8) | (unk30 << 0xD));
                            return;
                    }
                } break;

                case 5: {
                    if (engine == DISPLAY_ENGINE_MAIN) {
                        DisplayBGSettings* bg2 = Display_GetBG2Settings(DISPLAY_ENGINE_MAIN);

                        s32 unk30 = bgSettings->unk_30;
                        s32 unk18 = bgSettings->unk_18;

                        bg2->unk_00 = 5;
                        bg2->unk_30 = unk30;
                        bg2->unk_18 = unk18;

                        REG_BG2CNT = (REG_BG2CNT & 0x43) | (unk18 << 0xE) | (unk30 << 0xD);
                        return;
                    }
                } break;

                default:
                    return;
            }
        } break;

        case DISPLAY_BG3:
            return;
    }
}

// Nonmatching: Work in progress, many stack and inlined bitshifting differences
void func_02005908(void) {
    DisplayControlSettings*   mainControl = Display_GetMainControl();
    DisplayControlSettings*   subControl  = Display_GetSubControl();
    UnkStruct_usedby_20050B4* mainState   = &data_0206aa80.unk_064[DISPLAY_ENGINE_MAIN];
    UnkStruct_usedby_20050B4* subState    = &data_0206aa80.unk_064[DISPLAY_ENGINE_SUB];

    REG_BG0CNT = (u16)((REG_BG0CNT & ~3) | mainState->bgSettings[DISPLAY_BG0].unk_34);
    REG_BG1CNT = (u16)((REG_BG1CNT & ~3) | mainState->bgSettings[DISPLAY_BG1].unk_34);
    REG_BG2CNT = (u16)((REG_BG2CNT & ~3) | mainState->bgSettings[DISPLAY_BG2].unk_34);
    REG_BG3CNT = (u16)((REG_BG3CNT & ~3) | mainState->bgSettings[DISPLAY_BG3].unk_34);

    func_02005444(DISPLAY_ENGINE_MAIN, DISPLAY_BG0);
    func_02005444(DISPLAY_ENGINE_MAIN, DISPLAY_BG1);
    func_02005444(DISPLAY_ENGINE_MAIN, DISPLAY_BG2);
    func_02005444(DISPLAY_ENGINE_MAIN, DISPLAY_BG3);

    if (Display_GetBG0Settings(DISPLAY_ENGINE_MAIN)->unk_38 != 0) {
        REG_BG0CNT = (u16)(REG_BG0CNT | 0x40);
    } else {
        REG_BG0CNT = (u16)(REG_BG0CNT & ~0x40);
    }

    if (Display_GetBG1Settings(DISPLAY_ENGINE_MAIN)->unk_38 != 0) {
        REG_BG1CNT |= 0x40;
    } else {
        REG_BG1CNT &= ~0x40;
    }

    if (Display_GetBG2Settings(DISPLAY_ENGINE_MAIN)->unk_38 != 0) {
        REG_BG2CNT |= 0x40;
    } else {
        REG_BG2CNT &= ~0x40;
    }

    if (Display_GetBG3Settings(DISPLAY_ENGINE_MAIN)->unk_38 != 0) {
        REG_BG3CNT |= 0x40;
    } else {
        REG_BG3CNT &= ~0x40;
    }

    if (mainControl->layers & 1) {
        if (mainControl->dimension == GX2D3D_MODE_2D) {
            REG_BG0HOFS =
                (0x1FF & ((s32)mainState->unk_0F0 >> 0xC)) | (0x01FF0000 & (((s32)mainState->unk_0F4 >> 0xC) << 0x10));
        } else {
            func_02036f78((s32)mainState->unk_0F0 >> 0xC);
        }
    }
    if (mainControl->layers & 2) {
        REG_BG1HOFS = (0x1FF & ((s32)mainState->unk_0F8 >> 0xC)) | (0x01FF0000 & (((s32)mainState->unk_0FC >> 0xC) << 0x10));
    }
    if ((mainControl->layers & 4) && (mainState->bgSettings[DISPLAY_BG2].unk_00 == 0)) {
        REG_BG2HOFS = (0x1FF & ((s32)mainState->unk_100 >> 0xC)) | (0x01FF0000 & (((s32)mainState->unk_104 >> 0xC) << 0x10));
    }
    if ((mainControl->layers & 8) && (mainState->bgSettings[DISPLAY_BG3].unk_00 == 0)) {
        REG_BG3HOFS = (0x1FF & ((s32)mainState->unk_108 >> 0xC)) | (0x01FF0000 & (((s32)mainState->unk_10C >> 0xC) << 0x10));
    }

    GX_SetWindow0Inside(mainState->window0, mainState->window0Effects);
    GX_SetWindow1Inside(mainState->window1, mainState->window1Effects);
    GX_SetWindowOutside(mainState->windowOutside, mainState->windowOutsideEffects);
    GX_SetWindowObjInside(mainState->windowObj, mainState->windowObjEffects);

    GX_SetWindow0Position(mainState->unk_1DC, mainState->unk_1D8, mainState->unk_1D0, mainState->unk_1D4);

    GX_SetWindow1Position(mainState->unk_1FC, mainState->unk_1F8, mainState->unk_1F0, mainState->unk_1F4);

    REG_MOSAIC_BG  = (s8)(mainState->unk_200 | (mainState->unk_202 * 0x10));
    REG_MOSAIC_OBJ = (s8)(mainState->unk_204 | (mainState->unk_206 * 0x10));

    switch (mainState->unk_208) { /* switch 1 */
        default:                  /* switch 1 */
            break;
        case 0:                   /* switch 1 */
        case 1:                   /* switch 1 */
            G2x_SetBlendAlpha(REG_BLDCNT, mainState->unk_20C, mainState->unk_210, mainState->unk_214, mainState->unk_218);
            break;
        case 2: /* switch 1 */
            func_02036a70(REG_BLDCNT, mainState->unk_20C, mainState->unk_210, mainState->unk_214, mainState->unk_218,
                          mainState->unk_21C);
            break;
        case 3: /* switch 1 */
            func_02036a70(REG_BLDCNT, mainState->unk_20C, mainState->unk_210, mainState->unk_214, mainState->unk_218,
                          0 - mainState->unk_21C);
            break;
    }

    if (mainState->unk_17C == 1) {
        mainState->unk_17C = 0;
        func_020063b8(0, mainState->unk_180, mainState->unk_184, mainState->unk_188, mainState->unk_18C, mainState->unk_190,
                      mainState->unk_100, mainState->unk_104);
    }

    REG_BG2PA = (u16)mainState->unk_168;
    REG_BG2PB = (u16)mainState->unk_16A;
    REG_BG2PC = (u16)mainState->unk_16C;
    REG_BG2PD = (u16)mainState->unk_16E;
    REG_BG2X  = (s32)mainState->unk_170;
    REG_BG2Y  = (s32)mainState->unk_174;

    if (mainState->unk_1A8 == 1) {
        mainState->unk_1A8 = 0;
        func_020064cc(0, mainState->unk_1AC, mainState->unk_1B0, mainState->unk_1B4, mainState->unk_1B8, mainState->unk_1BC,
                      mainState->unk_108, mainState->unk_10C);
    }
    REG_BG3PA = (u16)mainState->unk_194;
    REG_BG3PB = (u16)mainState->unk_196;
    REG_BG3PC = (u16)mainState->unk_198;
    REG_BG3PD = (u16)mainState->unk_19A;
    REG_BG3X  = (s32)mainState->unk_19C;
    REG_BG3Y  = (s32)mainState->unk_1A0;

    REG_BG0CNT_SUB = (u16)((REG_BG0CNT_SUB & ~3) | subState->bgSettings[DISPLAY_BG0].unk_34);
    REG_BG1CNT_SUB = (u16)((REG_BG1CNT_SUB & ~3) | subState->bgSettings[DISPLAY_BG1].unk_34);
    REG_BG2CNT_SUB = (u16)((REG_BG2CNT_SUB & ~3) | subState->bgSettings[DISPLAY_BG2].unk_34);
    REG_BG3CNT_SUB = (u16)((REG_BG3CNT_SUB & ~3) | subState->bgSettings[DISPLAY_BG3].unk_34);

    func_02005444(DISPLAY_ENGINE_SUB, DISPLAY_BG0);
    func_02005444(DISPLAY_ENGINE_SUB, DISPLAY_BG1);
    func_02005444(DISPLAY_ENGINE_SUB, DISPLAY_BG2);
    func_02005444(DISPLAY_ENGINE_SUB, DISPLAY_BG3);

    if (subState->bgSettings[DISPLAY_BG0].unk_38 != 0) {
        REG_BG0CNT_SUB = (u16)(REG_BG0CNT_SUB | 0x40);
    } else {
        REG_BG0CNT_SUB = (u16)(REG_BG0CNT_SUB & ~0x40);
    }

    if (subState->bgSettings[DISPLAY_BG1].unk_38 != 0) {
        REG_BG1CNT_SUB = (u16)(REG_BG1CNT_SUB | 0x40);
    } else {
        REG_BG1CNT_SUB = (u16)(REG_BG1CNT_SUB & ~0x40);
    }

    if (subState->bgSettings[DISPLAY_BG2].unk_38 != 0) {
        REG_BG2CNT_SUB = (u16)(REG_BG2CNT_SUB | 0x40);
    } else {
        REG_BG2CNT_SUB = (u16)(REG_BG2CNT_SUB & ~0x40);
    }

    if (subState->bgSettings[DISPLAY_BG3].unk_38 != 0) {
        REG_BG3CNT_SUB = (u16)(REG_BG3CNT_SUB | 0x40);
    } else {
        REG_BG3CNT_SUB = (u16)(REG_BG3CNT_SUB & ~0x40);
    }

    if (subControl->layers & 1) {
        REG_BG0HOFS_SUB = (0x1FF & ((s32)subState->unk_0F0 >> 0xC)) | (0x01FF0000 & (((s32)subState->unk_0F4 >> 0xC) << 0x10));
    }
    if (subControl->layers & 2) {
        REG_BG1HOFS_SUB = (0x1FF & ((s32)subState->unk_0F8 >> 0xC)) | (0x01FF0000 & (((s32)subState->unk_0FC >> 0xC) << 0x10));
    }
    if ((subControl->layers & 4) && (subState->bgSettings[2].unk_00 == 0)) {
        REG_BG2HOFS_SUB = (0x1FF & ((s32)subState->unk_100 >> 0xC)) | (0x01FF0000 & (((s32)subState->unk_104 >> 0xC) << 0x10));
    }
    if ((subControl->layers & 8) && (subState->bgSettings[3].unk_00 == 0)) {
        REG_BG3HOFS_SUB = (0x1FF & ((s32)subState->unk_108 >> 0xC)) | (0x01FF0000 & (((s32)subState->unk_10C >> 0xC) << 0x10));
    }

    GXs_SetWindow0Inside(subState->window0, subState->window0Effects);
    GXs_SetWindow1Inside(subState->window1, subState->window1Effects);
    GXs_SetWindowOutside(subState->windowOutside, subState->windowOutsideEffects);
    GXs_SetWindowObjInside(subState->windowObj, subState->windowObjEffects);

    GXs_SetWindow0Position(subState->unk_1DC, subState->unk_1D8, subState->unk_1D0, subState->unk_1D4);
    GXs_SetWindow1Position(subState->unk_1FC, subState->unk_1F8, subState->unk_1F0, subState->unk_1F4);

    REG_MOSAIC_BG_SUB  = (s8)(subState->unk_200 | (subState->unk_202 * 0x10));
    REG_MOSAIC_OBJ_SUB = (s8)(subState->unk_204 | (subState->unk_206 * 0x10));

    switch (subState->unk_208) { /* switch 2 */
        default:                 /* switch 2 */
            break;
        case 0:                  /* switch 2 */
        case 1:                  /* switch 2 */
            G2x_SetBlendAlpha(&REG_BLDCNT_SUB, subState->unk_20C, subState->unk_210, subState->unk_214, subState->unk_218);
            break;
        case 2: /* switch 2 */
            func_02036a70(&REG_BLDCNT_SUB, subState->unk_20C, subState->unk_210, subState->unk_214, subState->unk_218,
                          subState->unk_21C);
            break;
        case 3: /* switch 2 */
            func_02036a70(&REG_BLDCNT_SUB, subState->unk_20C, subState->unk_210, subState->unk_214, subState->unk_218,
                          0 - subState->unk_21C);
            break;
    }
    if (subState->unk_17C == 1) {
        subState->unk_17C = 0;
        func_020063b8(1, subState->unk_180, subState->unk_184, subState->unk_188, subState->unk_18C, subState->unk_190,
                      subState->unk_100, subState->unk_104);
    }

    REG_BG2PA_SUB = (u16)subState->unk_168;
    REG_BG2PB_SUB = (u16)subState->unk_16A;
    REG_BG2PC_SUB = (u16)subState->unk_16C;
    REG_BG2PD_SUB = (u16)subState->unk_16E;
    REG_BG2X_SUB  = (s32)subState->unk_170;
    REG_BG2Y_SUB  = (s32)subState->unk_174;

    if (subState->unk_1A8 == 1) {
        subState->unk_1A8 = 0;
        func_020064cc(1, subState->unk_1AC, subState->unk_1B0, subState->unk_1B4, subState->unk_1B8, subState->unk_1BC,
                      subState->unk_108, subState->unk_10C);
    }

    REG_BG3PA_SUB = (u16)subState->unk_194;
    REG_BG3PB_SUB = (u16)subState->unk_196;
    REG_BG3PC_SUB = (u16)subState->unk_198;
    REG_BG3PD_SUB = (u16)subState->unk_19A;
    REG_BG3X_SUB  = (s32)subState->unk_19C;
    REG_BG3Y_SUB  = (s32)subState->unk_1A0;
}

void func_02006380(void) {
    func_02005260();
    func_02005908();
}

void func_02006390(void) {
    do {
    } while (REG_VCOUNT < 0xC0);
    func_02006380();
}

void func_020063b8(s32 arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {}

void func_020064cc(s32 arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {}