#include "Debug/Mori/GrpCheck.h"
#include "Display.h"
#include "Engine/Color.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "common_data.h"

// Nonmatching: instruction scheduling
void GrpCheck_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    DMA_Init(0x100);
    GX_Init();
    G3X_Init();
    G3X_InitMtxStack();
    GX_DispOn();
    REG_DISPCNT_SUB |= 0x10000;
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForBgExtPltt(0x20);
    GX_SetBankForObjExtPltt(0x40);
    GX_SetBankForSubBg(4);
    if (GrpCheck_SpriteMode == 2) {
        GX_SetBankForTex(8);
        GX_SetBankForTexPltt(0x10);
    } else {
        GX_SetBankForSubObj(8);
        GX_SetBankForSubObjExtPltt(0x100);
    }
    OamMgr_Init3DSpritePipeline();
    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 1, 0, 0x4004);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 3, 1, 0x420C);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 1;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    Display_SetSubLayers(LAYER_BG0 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;

    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    OamMgr_InitExtended();
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    Color_CopyRange(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

void GrpCheck_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        OamMgr_Commit();
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void GrpCheck_RegisterVBlank(void) {
    GrpCheck_InitDisplay();
    Interrupts_RegisterVBlankCallback(GrpCheck_VBlank, 1);
}

void GrpCheck_ClearVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
