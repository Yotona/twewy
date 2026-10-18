#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Field/FO3D.h"
#include "common_data.h"
#include <nitro/gx.h>

extern FieldPtrList* gFOPtrList;

void* FOPtrList_Create(void) {
    gFOPtrList           = FOList_Alloc(sizeof(*gFOPtrList));
    gFOPtrList->items    = NULL;
    gFOPtrList->capacity = 0;
    return gFOPtrList;
}

void FOPtrList_Destroy(void) {
    FOList_Free(gFOPtrList->items);
    FOList_Free(gFOPtrList);
    gFOPtrList = NULL;
}

void FOPtrList_SetCurrent(FieldPtrList* list) {
    gFOPtrList = list;
}

FieldPtrList* FOPtrList_GetCurrent(void) {
    return gFOPtrList;
}

void FOPtrList_Init(s32 capacity) {
    FOPtrList_Create();
    gFOPtrList->items    = FOList_Alloc(capacity * sizeof(*gFOPtrList->items));
    gFOPtrList->capacity = capacity;
    FOPtrList_Clear();
}

void FOPtrList_Clear(void) {
    s32 i;

    if (gFOPtrList == NULL) {
        return;
    }
    i = gFOPtrList->capacity;
    while (i > 0) {
        gFOPtrList->items[--i] = NULL;
    }
}

s32 FOPtrList_Add(void* item) {
    s32           result = -1;
    s32           i      = 0;
    FieldPtrList* list   = gFOPtrList;

    if (list->capacity > 0) {
        do {
            if (list->items[i] == NULL) {
                result         = i;
                list->items[i] = item;
                break;
            }
            i++;
        } while (i < list->capacity);
    }
    return result;
}

// Nonmatching
void FO3D_InitGraphics(void) {
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForTex(3);
    GX_SetBankForTexPltt(0x40);
    GX_SetBankForBg(0x10);
    GX_SetBankForObj(0x20);
    GX_SetBankForSubObj(8);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObjExtPltt(0x100);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    Display_CommitSynced();

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(0, 0, 0, 16, 0, 0, 0x1000);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 1;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[0].objBmpMode  = GX_OBJBMPMODE_1D_128K;

    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);

    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(0, 0, 0, 0, 1, 0, 4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic   = 0;
    g_DisplaySettings.controls[1].objTileMode               = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode                = GX_OBJBMPMODE_1D_128K;
    Display_SetSubLayers(17);
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

void FO3D_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(g_OamMgr[DISPLAY_MAIN].oam, 0x400);
        GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
        DC_PurgeRange(g_OamMgr[DISPLAY_SUB].oam, 0x400);
        GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void FO3D_InitDisplay(void) {
    FO3D_InitGraphics();
    Interrupts_RegisterVBlankCallback(FO3D_VBlank, 1);
}

void FO3D_ExitDisplay(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

void FO3D_FadeInStep(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void FO3D_FadeOutStep(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, -0x20, 0x800);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}
