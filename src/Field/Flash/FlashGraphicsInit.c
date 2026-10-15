#include "Display.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Field/FlashPlayer.h"
#include "common_data.h"
#include <nitro/g3.h>
#include <nitro/mi/cpumem.h>
#include <nitro/os/cpustat.h>

extern void Interrupts_ForceVBlank(void);
extern void func_0202b878(void);
extern void GX_LoadBgPltt(const void* src, u32 offset, u32 size);
extern void GX_LoadObjPltt(const void* src, u32 offset, u32 size);
extern void GXs_LoadBgPltt(const void* src, u32 offset, u32 size);
extern void GXs_LoadObjPltt(const void* src, u32 offset, u32 size);
extern void G3i_OrthoW(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 near, fx32 far, fx32 scaleW, BOOL draw, void* mtx);

void func_ov030_020d53b0(void);
void func_ov030_020d56f8(void);
void func_ov030_020d57e4(void);
void func_ov030_020d58a4(void);
void func_ov030_020d5938(void);
void func_ov030_020d5a00(void);

BOOL      data_ov030_02109c00 = TRUE;
BOOL      data_ov030_02109df0;
GXOamAttr data_ov030_02109df4[128];

// Nonmatching: not yet compared
void func_ov030_020d53b0(void) {
    Interrupts_Init();
    HBlank_Init();
    while (REG_VCOUNT < (s16)0xC0) {
    }
    GX_Init();
    func_0202b878();
    DMA_Init(256);
    Display_Init();
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = -16;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = -16;
    Display_Commit();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForTex(3);
    GX_SetBankForTexPltt(0x40);
    GX_SetBankForBg(0x20);
    GX_SetBankForObj(0x10);
    GX_SetBankForSubObj(8);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObjExtPltt(0x100);
    GX_SetBankForSubBgExtPltt(0x80);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    Display_CommitSynced();

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0x10, 0, 0, 0x1000);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[0].mosaic   = FALSE;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic   = FALSE;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic   = FALSE;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic   = FALSE;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic   = FALSE;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = FALSE;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = FALSE;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic   = FALSE;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    Display_SetSubLayers(LAYER_BG0 | LAYER_OBJ);
    REG_POWER_CNT &= ~0x8000;

    OamMgr_Init3DSpritePipeline();
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 63;
    g_DisplaySettings.engineState[0].blendCoeff0 = 10;
    g_DisplaySettings.engineState[0].blendCoeff1 = 6;
    OamMgr_InitExtended();
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
}

void func_ov030_020d56f8(void) {
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

void func_ov030_020d57c8(void) {
    func_ov030_020d53b0();
    Interrupts_RegisterVBlankCallback(func_ov030_020d56f8, TRUE);
}

void func_ov030_020d57e4(void) {
    REG_POWER_CNT |= 0x8000;
    GX_ResetBankForSubObj();
    GX_SetBankForSubBg(4);
    GX_SetBankForLcdc(8);
    REG_DISPCAPCNT = 0x81330010;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    REG_DISPCNT = (REG_DISPCNT & ~0x1F00) | 0x100;
    REG_BG0CNT &= ~3;
    GXs_SetGraphicsMode(GX_BGMODE_5);
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x1F00) | 0x400;
    REG_BG2CNT_SUB  = (REG_BG2CNT_SUB & 0x43) | 0x84 | 0x4000;
    REG_BG2CNT_SUB &= ~3;
    REG_BG2CNT_SUB &= ~0x40;
}

void func_ov030_020d58a4(void) {
    REG_POWER_CNT &= ~0x8000;
    GX_ResetBankForSubBg();
    GX_SetBankForSubObj(8);
    GX_SetBankForLcdc(4);
    REG_DISPCAPCNT = 0x81320010;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    REG_DISPCNT = (REG_DISPCNT & ~0x1F00) | 0x100;
    REG_BG0CNT &= ~3;
    GXs_SetGraphicsMode(GX_BGMODE_5);
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x1F00) | 0x1000;
}

void func_ov030_020d5938(void) {
    s32 i;
    s32 x;
    s32 y;
    s32 index;

    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x60) | 0x20;
    index           = 0;
    for (i = 0; i < 128; i++) {
        data_ov030_02109df4[i].attr01 = 0;
        data_ov030_02109df4[i].attr23 = 0;
    }
    for (y = 0; y < 192; y += 64) {
        for (x = 0; x < 256; x += 64) {
            data_ov030_02109df4[index].attr01 = (y & 0xFF) | 0xC0000C00 | (((u32)x << 23) >> 7);
            data_ov030_02109df4[index].attr2  = (x / 8 + (y / 8) * 32) | 0xF000;
            index++;
        }
    }
    DC_PurgeRange(data_ov030_02109df4, 0x400);
    GXs_LoadOam(data_ov030_02109df4, 0, 0x400);
}

void func_ov030_020d5a00(void) {
    u32 irq = OS_DisableIRQ();

    REG_GFX_FIFO_SWAP_BUFFERS = 1;
    data_ov030_02109df0       = TRUE;
    OS_RestoreIRQ(irq);
}

void func_ov030_020d5a2c(void) {
    data_ov030_02109c00 = TRUE;
    data_ov030_02109df0 = FALSE;
    G3X_Init();
    G3X_InitTable();
    G3X_InitMtxStack();
    GX_SetBankForTex(3);
    GX_SetBankForTexPltt(0x10);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);
    func_ov030_020d5938();
    REG_GFX_FIFO_VIEWPORT        = 0xBFFF0000;
    REG_GFX_FIFO_MATRIX_MODE     = 0;
    REG_GFX_FIFO_MATRIX_IDENTITY = 0;
    G3i_OrthoW(0x600, -0x600, -0x800, 0x800, 0x1000, 0x100000, 0x1000, TRUE, NULL);
    REG_GFX_FIFO_MATRIX_STORE = 0;
    Interrupts_ForceVBlank();
    GX_DispOn();
    REG_DISPCNT_SUB |= 0x10000;
    G3X_Reset();
    func_ov030_020d5a00();
}
