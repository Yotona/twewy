#include "Field/FieldRegion.h"

#include "Display.h"
#include "Engine/Color.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Str.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Text.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Flash/FO3DViewer.h"
#include "Field/Scene/FieldBg.h"
#include "Field/Scene/FieldSceneLoad.h"
#include "Field/Scene/FieldSceneRender.h"
#include "Interface/Debug/Field/FieldDebug.h"
#include "Save.h"
#include "common_data.h"

#include <nitro/fx/fx_matrix.h>
#include <nitro/g3.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>

extern s32  func_0202599c(s32 flagIndex);
extern void func_020267e0(s32 arg0);
extern void func_02027b0c(s32 arg0, s32 arg1);
extern void func_02028a44(void);
extern void func_02028ad0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_02028b0c(s32 arg0, s32 arg1);

extern void func_02010b50(TextObject* textObj);
extern void func_02010b84(TextObject* textObj, s32 x, s32 y, s32 width, s32 height);
extern void func_02025b68(void* textObj, UnkStruct_usedby_02025b68* params);
extern void func_02025e30(void* textResource);
extern void GX_LoadBgPltt(void* ptr, u32 offset, u32 size);
extern void GX_LoadObjPltt(void* ptr, u32 offset, u32 size);
extern void GXs_LoadBgPltt(void* ptr, u32 offset, u32 size);
extern u32  GXs_LoadObjPltt(void* ptr, u32 offset, u32 size);
extern void G3i_OrthoW(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void G3i_LookAt(Vec* eye, Vec* up, Vec* target, s32 arg3, s32 arg4);

extern s32 (*data_02065fd4)(s32, s32, s32);

extern const char* data_ov030_02106a3c;
extern const char  data_ov030_02106a54[];
extern const char  data_ov030_02106a64[];
extern const char  data_ov030_02106a6c[];
extern void*       data_ov030_02106a70[1];
extern void*       data_ov030_02106a74[1];
extern void*       data_ov030_02106a78[2];
extern void*       data_ov030_02106a80[2];
extern void*       data_ov030_02106a88[3];
extern void*       data_ov030_02106a94[11];
extern void*       data_ov030_02106ac0[13];
extern u8          data_ov030_0210722c[];
extern u8          data_ov030_02107238[];
extern u8          data_ov030_02107244[];
typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} FldDataPair;
extern FldDataPair data_ov030_02109d68;

typedef void (*Ov030Proc)(void*);
extern Ov030Proc data_ov030_020e0d18[];

typedef struct {
    /* 0x0 */ const void* p[4];
} Ov030Ptr4;
typedef struct {
    /* 0x0 */ const void* p[13];
} Ov030Ptr13;
typedef struct {
    /* 0x0 */ const void* p[14];
} Ov030Ptr14;

extern const Ov030Ptr4  data_ov030_020e0d3c;
extern const Ov030Ptr13 data_ov030_020e0d4c;
extern const Ov030Ptr13 data_ov030_020e0d80;
extern const Ov030Ptr14 data_ov030_020e0db4;
extern const Ov030Ptr14 data_ov030_020e0dec;

const Vec data_ov030_020e0d24 = {0, 0x1000, 0};
const Vec data_ov030_020e0d30 = {0, 0, -0x1000};

void func_ov030_020b69f4(EventSelectObject* arg0) {
    UnkStruct_usedby_02025b68 params;

    params.unk_00 = 1;
    params.unk_04 = 0;
    params.unk_08 = 1;
    params.unk_0C = &data_0205c9b0;
    params.unk_10 = 0;
    params.unk_14 = NULL;
    params.unk_18 = 0;
    params.unk_1A = 0;
    params.unk_1C = 0x20;
    params.unk_1E = 0x18;
    func_02025b68(&arg0->unk_21594, &params);
    Text_RenderToScreen(&arg0->text, 0, 0, data_ov030_02106a54);
}

void func_ov030_020b6a74(EventSelectObject* arg0) {
    func_02010b84(&arg0->text, 0, 0x10, 0x100, 0x58);
    func_02010b50(&arg0->text);
    Text_RenderToScreen(&arg0->text, 8, 0x10, Str_SPrintf(data_ov030_02106a64, gSaveData.unk_26E0, func_ov030_020b5fe8()));
    Text_RenderToScreen(&arg0->text, 0, (arg0->selectedRow * 8) + 0x10, data_ov030_02106a6c);
}

void func_ov030_020b6b14(EventSelectObject* arg0) {
    func_02025e30(&arg0->unk_21594);
}

void func_ov030_020b6b28(EventSelectObject* arg0) {
    func_ov030_020b69f4(arg0);
}

void func_ov030_020b6b34(EventSelectObject* arg0) {
    func_ov030_020b6b14(arg0);
}

void func_ov030_020b6b40(void) {
    if (SysControl.buttonState.holdButtons & 0x10) {
        gSaveData.unk_26E0 += 1;
    } else if (SysControl.buttonState.holdButtons & 0x20) {
        gSaveData.unk_26E0 -= 1;
    }
    if (gSaveData.unk_26E0 < 0) {
        gSaveData.unk_26E0 = func_ov030_020b600c() - 1;
        return;
    }
    if (gSaveData.unk_26E0 >= func_ov030_020b600c()) {
        gSaveData.unk_26E0 = 0;
    }
}

void func_ov030_020b6bc8(EventSelectObject* arg0) {
    OverlayTag tagReplace;
    OverlayTag tagPop;
    s32        sel;

    if (SysControl.buttonState.pressedButtons & 1) {
        MainOvlDisp_ReplaceTop(&tagReplace, (s32)&OVERLAY_30_ID, &func_ov030_020b69b4, NULL, PROCESS_STAGE_INIT);
    } else if (SysControl.buttonState.pressedButtons & 4) {
        MainOvlDisp_Pop(&tagPop);
    } else if (SysControl.buttonState.holdButtons == 0x40) {
        arg0->selectedRow = arg0->selectedRow - 1;
    } else if (SysControl.buttonState.holdButtons == 0x80) {
        arg0->selectedRow = arg0->selectedRow + 1;
    }
    sel = arg0->selectedRow;
    if (sel < 0) {
        arg0->selectedRow = 0;
    } else if (sel >= 1) {
        arg0->selectedRow = 0;
    }
    if (arg0->selectedRow == 0) {
        func_ov030_020b6b40();
    }
    func_ov030_020b6a74(arg0);
}

// Nonmatching
void func_ov030_020b6ca0(void) {
    EventSelectObject* obj;
    const char*        seq;

    seq = data_ov030_02106a3c;
    obj = Mem_AllocHeapTail(&gDebugHeap, sizeof(EventSelectObject));
    Mem_SetSequence(&gDebugHeap, obj, seq);
    MainOvlDisp_SetCbArg(obj);
    func_ov030_020b20c0();
    g_DisplaySettings.controls[0].layers     = 0;
    obj->prevResources                       = ResourceMgr_ReinitManagers(&obj->resources);
    data_02066aec                            = 0;
    data_02066eec                            = 0;
    g_DisplaySettings.controls[0].brightness = 0;
    g_DisplaySettings.controls[1].brightness = 0;
    func_ov030_020b6b28(obj);
    MainOvlDisp_NextProcessStage();
}

void func_ov030_020b6d3c(void* arg0) {
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    OamMgr_ResetCommandQueues(&g_OamMgr[1]);
    func_ov030_020b6bc8(arg0);
    OamMgr_FlushCommands(&g_OamMgr[0]);
    OamMgr_FlushCommands(&g_OamMgr[1]);
    PaletteMgr_Flush(g_PaletteManagers[0], NULL);
    PaletteMgr_Flush(g_PaletteManagers[1], NULL);
}

void func_ov030_020b6dbc(void* arg0) {
    func_ov030_020b6b34(arg0);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(1);
    Mem_Free(&gDebugHeap, arg0);
}

void func_ov030_020b6dec(void* arg0) {
    s32 stage = MainOvlDisp_GetProcessStage();

    if (stage == 0x7FFFFFFF) {
        func_ov030_020b6dbc(arg0);
    } else {
        data_ov030_020e0d18[stage](arg0);
    }
}

// Nonmatching
void func_ov030_020b6e2c(void) {
    Vec v2C;
    Vec v20;
    Vec v14;

    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    G3X_Init();
    G3X_InitMtxStack();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForTex(0xF);
    GX_SetBankForTexPltt(0x40);
    GX_SetBankForBg(0x10);
    GX_SetBankForObj(0x20);
    GX_SetBankForSubObj(0x100);
    GX_SetBankForSubBg(0x80);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    DC_PurgeRange(g_OamMgr[0].oam, 0x400);
    GX_LoadOam(g_OamMgr[0].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_SUB);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    DC_PurgeRange(g_OamMgr[1].oam, 0x400);
    GXs_LoadOam(g_OamMgr[1].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_EXTENDED);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    Color_CopyRange(&data_02066eec, &data_0205a128, 0, 0x200, 1);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    Display_CommitSynced();
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0x10, 0, 0, 0x1000);
    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic   = 0;
    g_DisplaySettings.controls[0].objTileMode               = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[0].objBmpMode                = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode               = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode                = GX_OBJBMPMODE_1D_128K;
    Display_SetMainLayers(0x11);
    data_0206aa78                        = 0x300010;
    data_0206aa7c                        = 0x400040;
    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 4);
    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic   = 0;
    g_DisplaySettings.controls[1].objTileMode               = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode                = GX_OBJBMPMODE_1D_128K;
    Display_SetSubLayers(0x11);
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
    REG_POWER_CNT &= ~0x8000;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3002;
    REG_DISP3DCNT = REG_DISP3DCNT & (u16)~0x3004;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x08;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x10;
    REG_DISP3DCNT = REG_DISP3DCNT & (u16)~0x3020;
    G3X_SetClearColor(0, 0, 0x7FFF, 0, 0);
    REG_GFX_FIFO_VIEWPORT     = 0xBFFF0000;
    REG_GFX_FIFO_SWAP_BUFFERS = 0;
    G3i_OrthoW(0, 0xC0000, 0, 0x100000, -0x400000, 0x400000, 0x400000, 1, 0);
    v2C.x = 0;
    v2C.y = 0;
    v2C.z = 0;
    v20   = data_ov030_020e0d24;
    v14   = data_ov030_020e0d30;
    G3i_LookAt(&v2C, &v20, &v14, 1, 0);
    REG_GFX_FIFO_MATRIX_STORE = 0;
    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    DC_PurgeRange(g_OamMgr[0].oam, 0x400);
    GX_LoadOam(g_OamMgr[0].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_SUB);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    DC_PurgeRange(g_OamMgr[1].oam, 0x400);
    GXs_LoadOam(g_OamMgr[1].oam, 0, 0x400);
    OamMgr_Init3DSpritePipeline();
    OamMgr_InitEngine(0, DISPLAY_EXTENDED);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    OamMgr_ResetCommandQueues(&g_OamMgr[1]);
}

u32 func_ov030_020b7330(void) {
    u32 v = SystemStatusFlags.vblank;

    if (v == 0) {
        return v;
    }
    Display_Commit();
    DMA_Flush();
    DC_PurgeRange(g_OamMgr[0].oam, 0x400);
    GX_LoadOam(g_OamMgr[0].oam, 0, 0x400);
    DC_PurgeRange(g_OamMgr[1].oam, 0x400);
    GXs_LoadOam(g_OamMgr[1].oam, 0, 0x400);
    DC_PurgeRange(&data_02066aec, 0x400);
    GX_LoadBgPltt(&data_02066aec, 0, 0x200);
    GX_LoadObjPltt(&data_02066cec, 0, 0x200);
    DC_PurgeRange(&data_02066eec, 0x400);
    GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
    return GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
}

void func_ov030_020b7400(void) {
    func_ov030_020b6e2c();
    Interrupts_RegisterVBlankCallback((IRQCallback)func_ov030_020b7330, 1);
}

void func_ov030_020b741c(void) {
    u32 i = 0;

    do {
        FO3DViewer_Hide(data_ov030_02106a94[i]);
        i += 1;
    } while (i < 0xB);
}

void func_ov030_020b7444(void) {
    u32 i    = 0;
    s32 skip = gSaveData.fieldArea - 0x18;

    do {
        if (i != skip) {
            FO3DViewer_Hide(data_ov030_02106ac0[i]);
        }
        i += 1;
    } while (i < 0xD);
}

void func_ov030_020b7484(void) {
    s32 i;

    for (i = 0; i < ARRAY_LEN_U(data_ov030_02106a74); i++) {
        FO3DViewer_Show(data_ov030_02106a74[i]);
    }
    for (i = 0; i < ARRAY_LEN_U(data_ov030_02106a80); i++) {
        FO3DViewer_Show(data_ov030_02106a80[i]);
    }
}

void func_ov030_020b74cc(void) {
    s32 i;

    for (i = 0; i < ARRAY_LEN_U(data_ov030_02106a74); i++) {
        FO3DViewer_Show(data_ov030_02106a74[i]);
    }
    for (i = 0; i < ARRAY_LEN_U(data_ov030_02106a78); i++) {
        FO3DViewer_Show(data_ov030_02106a78[i]);
    }
}

void func_ov030_020b7514(void) {
    s32 i;

    for (i = 0; i < ARRAY_LEN_U(data_ov030_02106a70); i++) {
        FO3DViewer_Show(data_ov030_02106a70[i]);
    }
    for (i = 0; i < ARRAY_LEN_U(data_ov030_02106a88); i++) {
        FO3DViewer_Show(data_ov030_02106a88[i]);
    }
}

void func_ov030_020b755c(s32 arg0) {
    Ov030Ptr4 tbl = data_ov030_020e0d3c;
    u32       i   = 0;

    do {
        if (i != arg0) {
            FO3DViewer_Hide(tbl.p[i]);
        }
        i += 1;
    } while (i < 4U);
}

void func_ov030_020b75a4(void) {
    Ov030Ptr13 tblA = data_ov030_020e0d80;
    u8         v    = gSaveData.brandTrends[0].ranking[0];
    Ov030Ptr14 tblB = data_ov030_020e0dec;
    u8         v2;

    if (v >= 0xD) {
        v = 0;
    }
    FO3DViewer_LoadExtra(0, tblA.p[v]);
    v2 = gSaveData.brandTrends[1].ranking[0];
    if (FieldBg_AreTrendsVisible() == 0) {
        v2 = 0xD;
    }
    if (v2 >= 0xE) {
        v2 = 0;
    }
    FO3DViewer_LoadExtra(0, tblB.p[v2]);
}

void func_ov030_020b765c(void) {
    Ov030Ptr14 tbl = data_ov030_020e0db4;
    u8         v;

    v = gSaveData.brandTrends[Field_GetCurrentTrendZone()].ranking[0];
    if (FieldBg_AreTrendsVisible() == 0) {
        v = 0xD;
    }
    if (v >= 0xE) {
        v = 0;
    }
    FO3DViewer_LoadExtra(gSaveData.fieldArea, tbl.p[v]);
}

void func_ov030_020b76dc(void) {
    Ov030Ptr13 tbl = data_ov030_020e0d4c;
    u8         v   = gSaveData.brandTrends[0].ranking[0];

    if (v >= 0xD) {
        v = 0;
    }
    FO3DViewer_LoadExtra(gSaveData.fieldArea, tbl.p[v]);
}

void func_ov030_020b7744(s32* arg0, s32 arg1) {
    s32 area;

    data_ov030_02109d68.unk_00 = 0x1000;
    data_ov030_02109d68.unk_04 = 0x800000;
    func_02028ad0(0, 0, 0, -0x1000);
    func_02028b0c(0, 0x7FFF);
    Mtx_OrthoW(0xC0000, 0, 0, 0x100000, -0x400000, 0x400000, 0x400000, &data_02076a4c);
    data_02076a44.unk_FC = data_02076a44.unk_FC & ~0x50;
    func_02028a44();
    func_020267e0(0x80);
    func_02027b0c(0x4000, 1);
    *arg0 = data_02065fd4(0x800, 0, 1);
    area  = gSaveData.fieldArea;
    if (area == 0) {
        FO3DViewer_Init(2);
    } else if (area == 1) {
        FO3DViewer_Init(0);
    } else if (area == 2) {
        FO3DViewer_Init(1);
    } else if (area == 5) {
        FO3DViewer_Init(5);
        if (func_0202599c(0) != 0) {
            func_ov030_020b755c(2);
        } else if (func_0202599c(8) != 0) {
            func_ov030_020b755c(1);
        } else {
            func_ov030_020b755c(0);
        }
    } else if (area == 0x18) {
        FO3DViewer_Init(0x18);
        func_ov030_020b741c();
        func_ov030_020b7484();
        func_ov030_020b7444();
    } else if (area == 0x19) {
        FO3DViewer_Init(0x18);
        func_ov030_020b741c();
        func_ov030_020b74cc();
        func_ov030_020b7444();
    } else if (area >= 0x1A && area <= 0x24) {
        FO3DViewer_Init(0x18);
        func_ov030_020b741c();
        func_ov030_020b7514();
        func_ov030_020b7444();
    } else if (area == 0x13) {
        FO3DViewer_Init(0x13);
        if (func_0202599c(4) == 0) {
            FO3DViewer_Hide(data_ov030_0210722c);
        } else {
            FO3DViewer_Hide(data_ov030_02107238);
        }
    } else if (area == 0x27) {
        if (func_0202599c(6) != 0) {
            FO3DViewer_Init(0x2D);
        } else {
            FO3DViewer_Init(0x27);
        }
    } else if (area >= 0x2C) {
        FO3DViewer_Init(0x25);
    } else {
        FO3DViewer_Init(area);
    }
    if (gSaveData.fieldArea == 1) {
        func_ov030_020b75a4();
    }
    if (gSaveData.fieldArea == 0xE) {
        FO3DViewer_LoadExtra(gSaveData.fieldArea, data_ov030_02107244);
    }
    if (gSaveData.fieldArea == 3) {
        func_ov030_020b765c();
    }
    if (gSaveData.fieldArea == 4) {
        func_ov030_020b76dc();
    }
    FO3DViewer_LoadToVram();
}
