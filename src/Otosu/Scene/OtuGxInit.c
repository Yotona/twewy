#include "Display.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Otosu/OtosuGame.h"
#include "common_data.h"
#include "nitro/reg.h"
#include <nitro/gx.h>

// Nonmatching: register allocation in the g_DisplaySettings block
void OtuScene_InitGraphics(void) {
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();

    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_A);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_B);
    GX_SetBankForObj(GX_VRAM_E);
    GX_SetBankForSubObj(GX_VRAM_D);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObjExtPltt(GX_VRAM_I);
    GX_SetBankForSubBgExtPltt(GX_VRAM_H);

    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    *(vu16*)0x04000304 &= ~0x8000;
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    Display_CommitSynced();

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;

    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 0, 2, 0, 0x4008);

    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 2, 4, 1, 0x4210);

    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_512x256, GX_BG_COLORS_16, 6, 6, 1, 0x4618);

    Display_GetBG0Settings(DISPLAY_MAIN)->priority = 2;
    Display_GetBG1Settings(DISPLAY_MAIN)->priority = 3;
    Display_GetBG2Settings(DISPLAY_MAIN)->priority = 0;
    Display_GetBG3Settings(DISPLAY_MAIN)->priority = 1;
    Display_GetBG0Settings(DISPLAY_MAIN)->mosaic   = 0;
    Display_GetBG1Settings(DISPLAY_MAIN)->mosaic   = 0;
    Display_GetBG2Settings(DISPLAY_MAIN)->mosaic   = 0;
    Display_GetBG3Settings(DISPLAY_MAIN)->mosaic   = 0;
    Display_SetMainLayers(0x13);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= 0xC;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 2, 0, 0x8);

    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 4, 0, 0x110);

    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 4, 1, 0x210);

    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 3, 6, 1, 0x318);

    Display_GetBG0Settings(DISPLAY_SUB)->priority = 0;
    Display_GetBG1Settings(DISPLAY_SUB)->priority = 1;
    Display_GetBG2Settings(DISPLAY_SUB)->priority = 2;
    Display_GetBG3Settings(DISPLAY_SUB)->priority = 3;
    Display_GetBG0Settings(DISPLAY_SUB)->mosaic   = 0;
    Display_GetBG1Settings(DISPLAY_SUB)->mosaic   = 0;
    Display_GetBG2Settings(DISPLAY_SUB)->mosaic   = 0;
    Display_GetBG3Settings(DISPLAY_SUB)->mosaic   = 0;
    Display_SetSubLayers(0x11);

    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3002) | 2;
    REG_DISP3DCNT &= 0xCFDF;
    OamMgr_Init3DSpritePipeline();
    G3i_OrthoW(0, 0xC0000, 0, 0x100000, 0xF0000000, 0x400000, 0x400000, 1, 0);

    g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode   = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 8;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x23;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 5;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 0xA;

    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    DC_PurgeRange(&g_OamMgr[DISPLAY_MAIN].oam, 0x400);
    GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);

    OamMgr_InitEngine(0, DISPLAY_SUB);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    DC_PurgeRange(&g_OamMgr[DISPLAY_SUB].oam, 0x400);
    GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);

    OamMgr_InitEngine(0, DISPLAY_EXTENDED);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
}
