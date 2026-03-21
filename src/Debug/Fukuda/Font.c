#include "Display.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <types.h>

typedef struct {
    /* 0x00000 */ ResourceManager  unk_00000;
    /* 0x11580 */ ResourceManager* unk_11580;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ char             unk_11588[0x10428];
    /* 0x219B0 */ UnkOv31Struct    unk_219B0;
    /* 0x21A2C */ s32              unk_21A2C;
    /* 0x21A30 */ char             unk_21A30[0x78];
    /* 0x21AA8 */ void*            unk_21AA8;
    /* 0x21AAC */ s32              unk_21AAC;
    /* 0x21AB0 */ Sprite           unk_21AB0;
    /* 0x21AF0 */ Sprite           unk_21AF0;
    /* 0x21B30 */ char             unk_21B30[0xC];
    /* 0x21B3C */ s32              unk_21B3C;
    /* 0x21B40 */ s32              unk_21B40;
} FontState; // Size: 0x21B44

const BinIdentifier data_ov001_02082f98 = {1, "Apl_Fuk/Grp_FldMessWin.bin"};
const BinIdentifier data_ov001_02082f90 = {1, "Apl_Fuk/Grp_ForTest.bin"};

void FontTest_RegisterVBlank(void);

void func_ov001_020824a0(FontState* arg0) {
    data_02066eec = 0;
    func_ov031_0210aaac(&arg0->unk_219B0, 1, 1);
    func_ov031_0210ab28(&arg0->unk_219B0, 0, 0);
    arg0->unk_219B0.unk_5C = 1;
    func_ov031_0210ab28(&arg0->unk_219B0, 0, 0);
    func_ov031_0210b630(&arg0->unk_219B0, 8334);
    func_ov031_0210be18(&arg0->unk_219B0, arg0->unk_21B3C + 4, arg0->unk_21B40 + 4, 0);
    func_ov031_0210aabc(&arg0->unk_219B0);
}

void func_ov001_0208254c(FontState* arg0) {
    // Not yet implemented
}

void func_ov001_020825f4(FontState* arg0) {
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], arg0->unk_21AAC);
    Mem_Free(&gDebugHeap, arg0->unk_21AA8);
}

void func_ov001_0208262c(FontState* arg0) {
    // Not yet implemented
}

func_ov001_020826e4() {}

func_ov001_02082780(FontState* arg0) {}

void func_ov001_0208288c(FontState* arg0) {
    FS_LoadOverlay(0, &OVERLAY_31_ID);
    func_ov001_0208262c(arg0);
    func_ov001_02082780(arg0);
    Sprite_Update(&arg0->unk_21AB0);
    Sprite_RenderFrame(&arg0->unk_21AB0);
    Sprite_Update(&arg0->unk_21AF0);
    Sprite_RenderAltPalette(&arg0->unk_21AF0, 0, 0, 2);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_ov001_020824a0(arg0);
    DC_PurgeAll();

    s32 val = arg0->unk_21B3C + 4;
    func_0203ab7c(3, val, G2S_GetBG0ScrPtr(), 0x800);

    val = arg0->unk_21B40 + 4;
    func_0203ab7c(3, val, G2S_GetBG0CharPtr(), 0x8000);

    PaletteMgr_Flush(g_PaletteManagers[0], 0);
    PaletteMgr_Flush(g_PaletteManagers[1], 0);
}

func_ov001_02082984(FontState* arg0) {
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    Sprite_Release(&arg0->unk_21AB0);
    Sprite_Release(&arg0->unk_21AF0);
    func_ov001_020825f4(arg0);
}

static const char* data_ov001_02083020 = "Seq_Text()";

void FontTest_Init(FontState* arg0) {
    char* sequence = data_ov001_02083020;
    arg0           = Mem_AllocHeapTail(&gDebugHeap, sizeof(FontState));
    Mem_SetSequence(&gDebugHeap, arg0, sequence);
    MainOvlDisp_SetCbArg(arg0);

    arg0->unk_11584 = DatMgr_AllocateSlot();
    FontTest_RegisterVBlank();
    arg0->unk_11580 = ResourceMgr_ReinitManagers(&arg0->unk_00000);

    data_02066aec                                       = 0;
    data_02066eec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;

    func_ov001_0208288c(arg0);
    MainOvlDisp_NextProcessStage();
}

void FontTest_Update(FontState* arg0) {
    TouchInput_Update();
    Input_PollState(&InputStatus);
    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) {
        func_ov001_0208254c(arg0);
    }

    if (func_02041458() == FALSE) {
        SysControl.buttonState.pressedButtons = 0;
        SysControl.buttonState.currButtons    = 0;
        TouchInput_Init();
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_R) {
        func_02004a68(&gDebugHeap);
        func_02004a68(&gMainHeap);
    }
}

void FontTest_Destroy(FontState* arg0) {
    func_ov001_02082984(arg0);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(arg0->unk_11584);
    Mem_Free(&gDebugHeap, arg0);
}

static const OverlayProcess OvlProc_FontTest = {
    .init = FontTest_Init,
    .main = FontTest_Update,
    .exit = FontTest_Destroy,
};

void ProcessOverlay_FontTest(FontState* arg0) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        FontTest_Destroy(arg0);
    } else {
        OvlProc_FontTest.funcs[stage](arg0);
    }
}

void func_ov001_02082b74(void) {
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    func_0202b878();
    DMA_Init(256);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1ff);
    GX_SetBankForTex(3);
    GX_SetBankForTexPltt(0x40);
    GX_SetBankForBg(0x20);
    GX_SetBankForObj(0x10);
    GX_SetBankForBgExtPltt(0);
    GX_SetBankForObjExtPltt(0);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    GX_SetBankForSubBgExtPltt(0x80);
    GX_SetBankForSubObjExtPltt(0x100);
    MI_CpuFill(0, 0x6800000, 0xa4000);
    MI_CpuFill(0, 0x6000000, 0x80000);
    MI_CpuFill(0, 0x6200000, 0x20000);
    MI_CpuFill(0, 0x6400000, 0x40000);
    MI_CpuFill(0, 0x6600000, 0x20000);
    MI_CpuFill(0, 0x6800000, 0xa4000);
    Display_CommitSynced();

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 16, 0, 0, 0x1000);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 1;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers      = 17;
    data_0206aa78                                        = 0x300010;
    data_0206aa7c                                        = 0x400040;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode       = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].layers      = 17;
    REG_POWER_CNT &= ~0x8000;
    OamMgr_Init3DSpritePipeline();
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 63;
    g_DisplaySettings.engineState[0].blendCoeff0 = 10;
    g_DisplaySettings.engineState[0].blendCoeff1 = 6;
    OamMgr_Init(0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    DC_PurgeRange(&data_0206770c, 1024);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    OamMgr_Init(0, 1);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    DC_PurgeRange(&data_02068798, 1024);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    OamMgr_Init(0, 2);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
}

void FontTest_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void FontTest_RegisterVBlank(void) {
    func_ov001_02082b74();
    Interrupts_RegisterVBlankCallback(FontTest_VBlank, TRUE);
}
