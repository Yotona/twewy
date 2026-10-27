#include "Otosu/OtosuRegion.h"

#include "Display.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Otosu/OtosuGame.h"
#include "common_data.h"
#include <nitro/gx.h>

static void OtuScene_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&g_OamMgr[DISPLAY_MAIN].oam, 0x400);
        GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
        DC_PurgeRange(&g_OamMgr[DISPLAY_SUB].oam, 0x400);
        GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
        DC_PurgeRange(g_PaletteBuffers[DISPLAY_MAIN].bg, 0x400);
        GX_LoadBgPltt(g_PaletteBuffers[DISPLAY_MAIN].bg, 0, 0x200);
        GX_LoadObjPltt(g_PaletteBuffers[DISPLAY_MAIN].obj, 0, 0x200);
        DC_PurgeRange(g_PaletteBuffers[DISPLAY_SUB].bg, 0x400);
        GXs_LoadBgPltt(g_PaletteBuffers[DISPLAY_SUB].bg, 0, 0x200);
        GXs_LoadObjPltt(g_PaletteBuffers[DISPLAY_SUB].obj, 0, 0x200);
        func_02001b44(2, 0, &g_PaletteBuffers[DISPLAY_EXTENDED], 0x400);
    }
}

static void OtuScene_VBlankWireless(void) {
    if (SystemStatusFlags.vblank) {
        OtuScene_ExchangeInputs();
        OtuScene_UpdateLocalInput();
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&g_OamMgr[DISPLAY_MAIN].oam, 0x400);
        GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
        DC_PurgeRange(&g_OamMgr[DISPLAY_SUB].oam, 0x400);
        GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
        DC_PurgeRange(g_PaletteBuffers[DISPLAY_MAIN].bg, 0x400);
        GX_LoadBgPltt(g_PaletteBuffers[DISPLAY_MAIN].bg, 0, 0x200);
        GX_LoadObjPltt(g_PaletteBuffers[DISPLAY_MAIN].obj, 0, 0x200);
        DC_PurgeRange(g_PaletteBuffers[DISPLAY_SUB].bg, 0x400);
        GXs_LoadBgPltt(g_PaletteBuffers[DISPLAY_SUB].bg, 0, 0x200);
        GXs_LoadObjPltt(g_PaletteBuffers[DISPLAY_SUB].obj, 0, 0x200);
        func_02001b44(2, 0, &g_PaletteBuffers[DISPLAY_EXTENDED], 0x400);
    }
}

void OtuScene_InitDisplay(void) {
    OtuScene_InitGraphics();
    Interrupts_RegisterVBlankCallback(OtuScene_VBlank, TRUE);
}

void OtuScene_InitDisplayWireless(void) {
    OtuScene_InitGraphics();
    Interrupts_RegisterVBlankCallback(OtuScene_VBlankWireless, TRUE);
}

void OtuScene_ClearVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}
