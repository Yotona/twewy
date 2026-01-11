#include "DatMgr.h"
#include "Display.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "OverlayManager.h"
#include "System.h"
#include "TouchInput.h"
#include "common_data.h"
#include <types.h>

typedef struct {
    /* 0x00000 */ s32   unk_00000;
    /* 0x00004 */ char  unk_00004[0x1157C];
    /* 0x11580 */ s32   unk_11580;
    /* 0x11584 */ s32   unk_11584;
    /* 0x11588 */ char  unk_11588[0x10428];
    /* 0x219B0 */ u32   unk_219B0;
    /* 0x219B4 */ char  unk_219B4[0x58];
    /* 0x21A0C */ s16   unk_21A0C;
    /* 0x21A0E */ char  unk_21A0E[0x1E];
    /* 0x21A2C */ s32   unk_21A2C;
    /* 0x21A30 */ char  unk_21A30[0x78];
    /* 0x21AA8 */ void* unk_21AA8;
    /* 0x21AAC */ s32   unk_21AAC;
    /* 0x21AB0 */ u32   unk_21AB0;
    /* 0x21AB4 */ char  unk_21AB4[0x3C];
    /* 0x21AF0 */ u32   unk_21AF0;
    /* 0x21AF4 */ char  unk_21AF4[0x48];
    /* 0x21B3C */ s32   unk_21B3C;
    /* 0x21B40 */ s32   unk_21B40;
} FontState; // Size: 0x21B44

typedef void (*FuncPtr)(FontState*);

static const char* data_ov001_02083020 = "Seq_Text()";

const BinIdentifier data_ov001_02082f98 = {1, "Apl_Fuk/Grp_FldMessWin.bin"};
const BinIdentifier data_ov001_02082f90 = {1, "Apl_Fuk/Grp_ForTest.bin"};

void func_ov001_02082f6c(void);

void func_ov001_020824a0(FontState* arg0) {
    data_02066eec = 0;
    func_ov031_0210aaac(&arg0->unk_219B0, 1, 1);
    func_ov031_0210ab28(&arg0->unk_219B0, 0, 0);
    arg0->unk_21A0C = 1;
    func_ov031_0210ab28(&arg0->unk_219B0, 0, 0);
    func_ov031_0210b630(&arg0->unk_219B0, 8334);
    func_ov031_0210be18(&arg0->unk_219B0, arg0->unk_21B3C + 4, arg0->unk_21B40 + 4, 0);
    func_ov031_0210aabc(&arg0->unk_219B0);
}

void func_ov001_0208254c(FontState* arg0) {
    // Not yet implemented
}

void func_ov001_020825f4(FontState* arg0) {
    func_0200afec(data_0206b3cc.unk_04, arg0->unk_21AAC);
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
    func_0200dd60(&arg0->unk_21AB0);
    func_0200e2c4(&arg0->unk_21AB0);
    func_0200dd60(&arg0->unk_21AF0);
    func_0200ec7c(&arg0->unk_21AF0, 0, 0, 2);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_ov001_020824a0(arg0);
    DC_PurgeAll();

    s32 val = arg0->unk_21B3C + 4;
    func_0203ab7c(3, val, G2S_GetBG0ScrPtr(), 0x800);

    val = arg0->unk_21B40 + 4;
    func_0203ab7c(3, val, G2S_GetBG0CharPtr(), 0x8000);

    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

func_ov001_02082984(FontState* arg0) {
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    func_0200e998(&arg0->unk_21AB0);
    func_0200e998(&arg0->unk_21AF0);
    func_ov001_020825f4(arg0);
}

void func_ov001_020829c0(FontState* arg0) {
    char* sequence = data_ov001_02083020;
    void* data     = Mem_AllocHeapTail(&gDebugHeap, sizeof(FontState));
    Mem_SetSequence(&gDebugHeap, data, sequence);
    void* prevCbArg = MainOvlDisp_SetCbArg(data);
    DatMgr_AllocateSlot();
    arg0->unk_11584 = prevCbArg;
    func_ov001_02082f6c();
    arg0->unk_11580                          = func_0200cef0(NULL);
    data_02066aec                            = 0;
    g_DisplaySettings.mainControl.brightness = 0;
    data_02066eec                            = 0;
    g_DisplaySettings.subControl.brightness  = 0;
    func_ov001_0208288c(arg0);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov001_02082a5c(FontState* arg0) {
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

void func_ov001_02082b00(FontState* arg0) {
    func_ov001_02082984(arg0);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(arg0->unk_11584);
    Mem_Free(&gDebugHeap, arg0);
}

void func_ov001_02082b34(FontState* arg0) {
    static const FuncPtr data_ov001_02082fa0[3] = {
        func_ov001_020829c0,
        func_ov001_02082a5c,
        func_ov001_02082b00,
    };

    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov001_02082b00(arg0);
    } else {
        data_ov001_02082fa0[count](arg0);
    }
}

void func_ov001_02082b74(void) {
    Interrupts_Init();
    func_0200434c();
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
    func_0203b2d0(0, 0x6800000, 0xa4000);
    func_0203b2d0(0, 0x6000000, 0x80000);
    func_0203b2d0(0, 0x6200000, 0x20000);
    func_0203b2d0(0, 0x6400000, 0x40000);
    func_0203b2d0(0, 0x6600000, 0x20000);
    func_0203b2d0(0, 0x6800000, 0xa4000);
    Display_CommitSynced();

    g_DisplaySettings.mainControl.dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.mainControl.bgMode    = GX_BGMODE_0;
    g_DisplaySettings.mainControl.dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    DisplayBGSettings* mainBg1 = &g_DisplaySettings.engineState[0].bgSettings[1];
    mainBg1->bgMode            = 0;
    mainBg1->screenSizeText    = 0;
    mainBg1->colorMode         = 0;
    mainBg1->screenBase        = 16;
    mainBg1->charBase          = 0;
    mainBg1->extPlttSlot       = 0;
    REG_BG1CNT                 = REG_BG1CNT & 0x43 | 0x1000;

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 1;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.mainControl.objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.mainControl.objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.subControl.objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.subControl.objBmpMode   = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.mainControl.layers      = 17;
    data_0206aa78                             = 0x300010;
    data_0206aa7c                             = 0x400040;
    g_DisplaySettings.subControl.bgMode       = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    DisplayBGSettings* subBg0 = &g_DisplaySettings.engineState[1].bgSettings[0];
    subBg0->bgMode            = 0;
    subBg0->screenSizeText    = 0;
    subBg0->colorMode         = 0;
    subBg0->screenBase        = 0;
    subBg0->charBase          = 1;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = REG_BG0CNT_SUB & 0x43 | 4;

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic   = 0;
    g_DisplaySettings.subControl.objTileMode                = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.subControl.objBmpMode                 = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.subControl.layers                     = 17;
    REG_POWER_CNT &= ~0x8000;
    func_02003ad0();
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 63;
    g_DisplaySettings.engineState[0].blendCoeff0 = 10;
    g_DisplaySettings.engineState[0].blendCoeff1 = 6;
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 1024);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 1024);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_0200270c(0, 2);
    func_02002890(&data_02069804, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    return;
}

void func_ov001_02082e9c(void) {
    if (SystemStatusFlags.vblank != FALSE) {
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

void func_ov001_02082f6c(void) {
    func_ov001_02082b74();
    Interrupts_RegisterVBlankCallback(func_ov001_02082e9c, TRUE);
}
