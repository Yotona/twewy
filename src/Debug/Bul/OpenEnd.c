#include "Debug/OpenEnd.h"
#include "Display.h"
#include "EasyFade.h"
#include "TouchInput.h"
#include "common_data.h"

/*Nonmatching: Regswaps, Opcode reorder, likely inlines not defined yet*/
void func_ov037_020824a0(void) {
    Interrupts_Init();
    func_0200434c();
    DMA_Init(0x100);
    Interrupts_ForceVBlank();
    GX_Init();
    data_02066aec = 0;
    data_02066eec = 0;
    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT, 0 - 0x10);
    GXx_SetMasterBrightness(&REG_DISP_B_MASTER_BRIGHT, ~0xf);
    GX_DispOn();
    REG_DISPCNT_SUB |= 0x10000;
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(0x1);
    GX_SetBankForObj(0x2);
    GX_SetBankForBgExtPltt(0x10);
    GX_SetBankForSubBg(0x4);
    GX_SetBankForSubObj(0x8);
    GX_SetBankForSubBgExtPltt(0x80);
    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    DisplayBGSettings* mainBg0 = Display_GetBG0Settings(DISPLAY_MAIN);
    mainBg0->bgMode            = 0;
    mainBg0->screenSizeText    = 0;
    mainBg0->screenBase        = 0;
    mainBg0->extPlttSlot       = 0;
    mainBg0->colorMode         = 1;
    mainBg0->charBase          = 2;

    if (g_DisplaySettings.controls[DISPLAY_MAIN].dimension == GX2D3D_MODE_2D) {
        REG_BG0CNT = (REG_BG0CNT & 0x43) | 0x88;
    }

    DisplayBGSettings* mainBg1 = Display_GetBG1Settings(DISPLAY_MAIN);
    mainBg1->colorMode         = 1;
    mainBg1->screenBase        = 2;
    mainBg1->bgMode            = 0;
    mainBg1->screenSizeText    = 0;
    mainBg1->extPlttSlot       = 0;
    mainBg1->charBase          = 4;
    REG_BG1CNT                 = (REG_BG1CNT & 0x43) | 0x290;

    g_DisplaySettings.controls[DISPLAY_MAIN].layers = LAYER_BG0 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode  = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    DisplayBGSettings* subBg0 = Display_GetBG0Settings(DISPLAY_SUB);
    subBg0->bgMode            = 0;
    subBg0->screenSizeText    = 0;
    subBg0->colorMode         = 1;
    subBg0->screenBase        = 0;
    subBg0->charBase          = 2;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = (REG_BG0CNT_SUB & 0x43) | 0x88;

    DisplayBGSettings* subBg1 = Display_GetBG1Settings(DISPLAY_SUB);
    subBg1->charBase          = 4;
    subBg1->bgMode            = 0;
    subBg1->screenSizeText    = 0;
    subBg1->colorMode         = 1;
    subBg1->screenBase        = 2;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = (REG_BG1CNT_SUB & 0x43) | 0x290;

    g_DisplaySettings.controls[DISPLAY_SUB].layers       = 17;
    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;

    func_0200270c(0, 0);
    func_0200270c(1, 0);
    func_0203b2d0(0x0, 0x6800000, 0xa4000);
    func_0203b2d0(0x0, 0x6000000, 0x80000);
    func_0203b2d0(0x0, 0x6200000, 0x20000);
    func_0203b2d0(0x0, 0x6400000, 0x40000);
    func_0203b2d0(0x0, 0x6600000, 0x20000);
    func_0200270c(0, 0);
    func_0200283c(0, &data_020676ec, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(0, &data_02068778, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_02001c34(&data_02066aec, &data_0205a128, 0x0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0x0, 0x200, 1);
}

void func_ov037_0208280c(void) {
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

void func_ov037_020828dc(void) {
    func_ov037_020824a0();
    Interrupts_RegisterVBlankCallback(func_ov037_0208280c, TRUE);
}

void func_ov037_020828f8(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

// Load/Use .nbfs file
void func_ov037_0208290c(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize) {
    if (r0 == 0) {
        if (r1 == 0) {
            func_020373c0(buffer, r3, dataSize);
        }
        if (r1 == 1) {
            func_02037480(buffer, r3, dataSize);
        }
        if (r1 == 2) {
            func_02037540(buffer, r3, dataSize);
        }
        if (r1 == 3) {
            func_02037600(buffer, r3, dataSize);
        }
    }
    if (r0 != 1) {
        return;
    }
    if (r1 == 0) {
        func_02037420(buffer, r3, dataSize);
    }
    if (r1 == 1) {
        func_020374e0(buffer, r3, dataSize);
    }
    if (r1 == 2) {
        func_020375a0(buffer, r3, dataSize);
    }
    if (r1 != 3) {
        return;
    }
    func_02037660(buffer, r3, dataSize);
}

// Load/Use .nbfc file
void func_ov037_020829f4(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize) {
    if (r0 == 0) {
        if (r1 == 0) {
            func_020376c0(buffer, r3, dataSize);
        }
        if (r1 == 1) {
            func_02037780(buffer, r3, dataSize);
        }
        if (r1 == 2) {
            func_02037840(buffer, r3, dataSize);
        }
        if (r1 == 3) {
            func_02037900(buffer, r3, dataSize);
        }
    }
    if (r0 != 1) {
        return;
    }
    if (r1 == 0) {
        func_02037720(buffer, r3, dataSize);
    }
    if (r1 == 1) {
        func_020377e0(buffer, r3, dataSize);
    }
    if (r1 == 2) {
        func_020378a0(buffer, r3, dataSize);
    }
    if (r1 != 3) {
        return;
    }
    func_02037960(buffer, r3, dataSize);
}

// Load/Use .nbfp file
void func_ov037_02082adc(s32 r0, void* buffer, s32 r2, s32 dataSize) {
    if (r0 == 0) {
        GX_BeginLoadBgExtPltt();
        GX_LoadBgExtPltt(buffer, r2, dataSize);
        GX_EndLoadBgExtPltt();
    }
    if (r0 != 1) {
        return;
    }
    func_02037c10();
    func_02037c28(buffer, r2, dataSize);
    func_02037c8c();
    return;
}

void func_ov037_02082b30(OpenEndState* r0, s32 r1, s32 r2, s32 r3) {
    for (s32 idx = 0; idx < 3; idx++) {
        r0->dataList[idx] = DatMgr_LoadRawData(r0->dataType, 0, 0, &OpenEnd_FileList[(r3 * 3) + 1 + idx]);
    }
    DC_PurgeAll();
    func_ov037_0208290c(r1, r2, r0->dataList[2]->buffer, 0, r0->dataList[2]->dataSize);
    func_ov037_020829f4(r1, r2, r0->dataList[0]->buffer, 0, r0->dataList[0]->dataSize);
    func_ov037_02082adc(r1, r0->dataList[1]->buffer, r2 << 0xd, r0->dataList[1]->dataSize);
    for (s32 idx = 0; idx < 3; idx++) {
        DatMgr_ReleaseData(r0->dataList[idx]);
    }
}

void func_ov037_02082c00(OpenEndState* r0) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, r0->fadeRate);
    if (EasyFade_IsFading() == 0) {
        DebugOvlDisp_Pop();
    }
}

void func_ov037_02082c2c(OpenEndState* r0) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, r0->fadeBrightness, r0->fadeRate);
    if (EasyFade_IsFading() == 0) {
        DebugOvlDisp_Pop();
    }
}

void func_ov037_02082c58(OpenEndState* r0) {
    if (r0->unk_11A3C != 0)
        return;
    s32 temp      = func_02024aa4();
    r0->unk_11A38 = temp;
    r0->unk_11A3C = 1;
    r0->unk_11A40 = 0;
    if (temp == 0) {
        if (((data_02073710[0x5B]) & 1) != 0) {
            r0->unk_11A44 = 1;
        }
        return;
    } else if (temp != 1) {
        r0->unk_11A48 = 1;
        return;
    }
    func_02024d04();
    r0->unk_11A40 = 1;
}

void func_ov037_02082cd4(OpenEndState* r0) {
    func_ov037_02082c58(r0);
    func_ov037_02082b30(r0, 1, 0, 0);
    func_ov037_02082b30(r0, 0, 0, 1);
    func_ov037_02082b30(r0, 1, 1, 2);
    func_ov037_02082b30(r0, 0, 1, 3);
    r0->unk_11A1C      = 0;
    r0->fadeBrightness = data_ov037_02083a7c[data_02074d10.unk_410];
    r0->fadeRate       = data_ov037_02083a74[data_02074d10.unk_410];
    DebugOvlDisp_Pop();
}

void func_ov037_02082d7c(OpenEndState* r0) {
    switch (r0->unk_11A20) {
        case 0:
            if (r0->unk_11A1C > 0x78) {
                r0->unk_11A1C = 0;
                r0->unk_11A20++;
                break;
            }
            r0->unk_11A1C++;
            break;
        case 1:
            EasyFade_FadeMainDisplay(FADER_LINEAR, -16, 0x333);
            EasyFade_FadeSubDisplay(FADER_LINEAR, -16, 0x333);
            if (EasyFade_IsFading() != 0)
                break;
            r0->unk_11A1C = 0;
            r0->unk_11A20++;
            break;
        case 2:
            if (r0->unk_11A1C == 0) {
                g_DisplaySettings.controls[DISPLAY_MAIN].layers = g_DisplaySettings.controls[DISPLAY_MAIN].layers & ~0x1 | 0x2;
                g_DisplaySettings.controls[DISPLAY_SUB].layers  = g_DisplaySettings.controls[DISPLAY_SUB].layers & ~0x1 | 0x2;
            }
            EasyFade_FadeMainDisplay(FADER_LINEAR, 0, 0x333);
            EasyFade_FadeSubDisplay(FADER_LINEAR, 0, 0x333);
            if (EasyFade_IsFading() == 0) {
                r0->unk_11A1C = 0;
                r0->unk_11A20++;
                break;
            }
            r0->unk_11A1C++;
            break;
        case 3:
            if (r0->unk_11A1C > 0x78) {
                DebugOvlDisp_Pop();
                break;
            }
            r0->unk_11A1C++;
            break;
    }
}

extern void func_ov002_02086acc(void* r0);
void        func_ov037_02082f04(OpenEndState* r0) {
    OverlayTag tag;
    data_02074d10.unk_410 = 1;
    if (r0->unk_11A40 != 0) {
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, func_ov002_02086acc, NULL, 0);
    } else {
        func_ov037_020833a8(r0);
    }
}

/*Nonmatching: regswap */
extern void func_ov002_02086a8c(void* state);
extern void func_ov002_02086b0c(void* state);
void        func_ov037_02082f60(OpenEndState* r0) {
    OverlayTag tag, tag2;
    func_ov037_02082c58(r0);
    if (r0->unk_11A48 != 0) {
        if (r0->unk_11A38 == 2) {
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, func_ov002_02086b0c, 0, 0);
        } else {
            MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_2_ID, func_ov002_02086a8c, 0, 0);
        }
        DebugOvlDisp_Init();
        return;
    }
    g_DisplaySettings.controls[DISPLAY_MAIN].layers = (g_DisplaySettings.controls[DISPLAY_MAIN].layers | 0x1) & ~0x2;
    g_DisplaySettings.controls[DISPLAY_SUB].layers  = (g_DisplaySettings.controls[DISPLAY_SUB].layers | 0x1) & ~0x2;
    func_ov037_02082b30(r0, 0, 0, 5);
    func_ov037_02082b30(r0, 1, 0, 4);
    OpenEnd_CreateBadgeTask(0);
    if (r0->unk_11A44 != 0) {
        OpenEnd_CreateBadgeTask(1);
    }
    r0->unk_11A20      = 0;
    r0->unk_11A24      = 0;
    r0->fadeBrightness = data_ov037_02083a7c[data_02074d10.unk_410]; // Regswap here
    r0->fadeRate       = data_ov037_02083a74[data_02074d10.unk_410]; // and here
    r0->unk_11A2C      = 0;
    func_0202733c();
    DebugOvlDisp_Pop();
}

/* Nonmatching: Opcode reordering*/
void func_ov037_020830a8(OpenEndState* r0) {
    TouchCoord coords;

    TouchInput_GetCoord(&coords);
    switch (r0->unk_11A20) {
        case 0:
            if (r0->unk_11A24 <= 0x9e340) {
                r0->unk_11A24++;
            } else {
                func_02027388(0);
                r0->unk_11A24 = 0;
                r0->unk_11A20++;
            }
            break;
        case 1:
            if (r0->unk_11A24 <= 0xa) {
                r0->unk_11A24++;
            } else {
                func_0202733c(0);
                r0->unk_11A20 = 0;
                r0->unk_11A24 = 0;
            }
            break;
    }
    if (r0->unk_11A2C == 0)
        return;
    r0->fadeBrightness = -16;
    r0->fadeRate       = 0x800;
    DebugOvlDisp_Pop();
}

extern void func_ov002_02086a4c(void* r0);
extern void func_ov030_020b0fe8(void* r0);
void        func_ov037_02083188(OpenEndState* r0) {
    OverlayTag tag, tag2;
    if (r0->unk_11A44 != 0) {
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, func_ov002_02086a4c, NULL, 0);
    } else {
        MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, NULL, 0);
    }
}

/*Nonmatching: Compiler optimizes 0x100 out of index access for data_02073710*/
extern void func_ov044_02084a88();
extern void func_ov030_020ae92c();
void        func_ov037_020831ec(OpenEndState* r0) {
    OverlayTag tag, tag2, tag3, tag4;
    if (func_020256bc() == 0) {
        if (func_02023010(0x2AB) != 0) {
            data_02073710[0xB4] |= 0x10;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_44_ID, func_ov044_02084a88, 0, 0);
            return;
        }
        if ((data_02073710[0xB4] & 0x2) != 0) {
            data_02073710[0xB4] &= ~0x2;
            MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, 0, 0);
            return;
        }
        MainOvlDisp_ReplaceTop(&tag3, &OVERLAY_30_ID, func_ov030_020ae92c, 0, 0);
        return;
    }
    MainOvlDisp_ReplaceTop(&tag4, &OVERLAY_2_ID, func_ov002_02086a8c, 0, 0);
}

void func_ov037_020832dc(OpenEndState* r0) {
    OverlayTag tag, tag2;
    r0->unk_11A3C = 0;
    func_ov037_02082c58(r0);
    if (r0->unk_11A48 != 0) {
        if (r0->unk_11A38 == 2) {
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_2_ID, func_ov002_02086b0c, 0, 0);
        } else {
            MainOvlDisp_ReplaceTop(&tag2, &OVERLAY_2_ID, func_ov002_02086a8c, 0, 0);
        }
        DebugOvlDisp_Init();
        return;
    }
    func_02027388(0);
    switch (r0->unk_11A28) {
        case 0:
            func_ov037_02083188(r0);
            break;
        case 1:
            func_ov037_020831ec(r0);
            break;
    }
    DebugOvlDisp_Pop();
}

void func_ov037_020833a8(OpenEndState* r0) {
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)data_ov037_02083a90[data_02074d10.unk_410][2], r0, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov037_02082c2c, r0, 0);
    DebugOvlDisp_Push((OverlayCB)data_ov037_02083a90[data_02074d10.unk_410][1], r0, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov037_02082c00, r0, 0);
    DebugOvlDisp_Push((OverlayCB)data_ov037_02083a90[data_02074d10.unk_410][0], r0, 0);
}

extern vu32        data_02066a58;
static const char* seq_OpenEnd = "Seq_OpenEnd(void *)";
/*Nomatching: regswaps*/
void func_ov037_0208345c(OpenEndState* r0) {
    if (r0 == NULL) {
        char* SeqName = seq_OpenEnd;
        void* unk     = Mem_AllocHeapTail(&gDebugHeap, 0x11A4C);
        Mem_SetSequence(0x11A4C, unk, SeqName);
        data_ov037_02083e00 = unk;
        MainOvlDisp_SetCbArg(unk);
    }
    func_0203b2d0(0, r0, Mem_GetBlockSize(&gDebugHeap, r0));
    func_ov037_020828dc();
    u32 temp = data_ov037_02083a7c[data_02074d10.unk_410];
    if (data_ov037_02083a7c[data_02074d10.unk_410] > 0x10) {
        temp = 0x10;
    } else if (data_ov037_02083a7c[data_02074d10.unk_410] < ~0xf) {
        temp = ~0xF;
    }
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = temp;
    if (data_ov037_02083a7c[data_02074d10.unk_410] > 0x10) {
        temp = 0x10;
    } else if (data_ov037_02083a7c[data_02074d10.unk_410] < ~0xf) {
        temp = ~0xF;
    }
    g_DisplaySettings.controls[DISPLAY_SUB].brightness = temp;
    func_0200cef0(&(r0->unk_10));
    Input_Init(&InputStatus, 8, 1, 2);
    TouchInput_Init();
    TouchInput_Update();
    func_02025b1c();
    data_02066a58 &= ~0x8;
    r0->dataType = &data_02066a58;
    DatMgr_AllocateSlot();
    r0->unk_11A38 = 0;
    Mem_InitializeHeap(&r0->unk_11610, &r0->unk_1161C, 0x400);
    EasyTask_InitializePool(&r0->unk_11590, &r0->unk_11610, 0x10, 0, 0);
    data_ov037_02083e08 = &r0->unk_11590;
    EasyTask_CreateTask(&r0->unk_11590, &Task_EasyFade, 0, 0, 0, 0);
    func_ov037_020833a8(r0);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov037_02083604(OpenEndState* r0) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    TouchInput_Update();
    if (TouchInput_WasTouchPressed() != 0) {
        data_ov037_02083e04 = 0x1;
    }
    EasyTask_UpdatePool(&r0->unk_11590);
    DebugOvlDisp_Run();
    if (DebugOvlDisp_IsStackAtBase() != 0)
        return;
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

void func_ov037_020836b4(OpenEndState* r0) {
    if (r0 == NULL) {
        return;
    }
    data_02074d10.unk_410 = 1;
    func_ov037_020828f8();
    DatMgr_ClearSlot(r0->dataType);
    EasyTask_DestroyPool(&r0->unk_11590);
    Mem_Free(&gDebugHeap, r0);
    MainOvlDisp_SetCbArg(0);
    func_02027388(0);
}

void func_ov037_0208370c(OpenEndState* r0) {
    u32 index = MainOvlDisp_GetRepeatCount();
    if (index == ~0x80000000) {
        func_ov037_020836b4(r0);
        return;
    }
    data_ov037_02083a84[index](r0);
}

void func_ov037_0208374c(u32 r0) {

    if (data_ov037_02083e00->unk_11A2C != 0)
        return;
    data_ov037_02083e00->unk_11A28 = r0;
    data_ov037_02083e00->unk_11A2C = 1;

    func_02026b20(2);
}

BOOL OpenEnd_IsInCircle(s32* r0, s32 x, s32 y) {
    s32 xx   = (x - r0[0]) * (x - r0[0]);
    s32 yy   = (y - r0[1]) * (y - r0[1]);
    s32 dSqr = xx + yy;
    s32 maxD = r0[2] * r0[2];
    return dSqr <= maxD;
}

void func_ov037_020837b8(SpriteAnimation* anim, s16 r1, s16 r2, s16 r3, s16 s1) {
    *anim         = data_ov037_02083b84;
    anim->binIden = OpenEnd_FileList;
    anim->unk_1C  = r1;
    anim->unk_26  = r2;
    anim->unk_28  = r3;
    anim->unk_2A  = s1;
}

typedef struct {
    /* 0x00 */ Sprite unk_00;
    /* 0x40 */ Sprite unk_40;
    /* 0x80 */ u32    unk_80;
} UnkTaskData;

int func_ov037_02083814(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    SpriteAnimation anim;

    // r1, r2 structs
    // UnkStruct2* r1 = (UnkStruct2*)_r1;
    u32* r2 = (u32*)taskParam;

    UnkTaskData* ptr = r1->data;
    func_0203b3c0(ptr, 0, 0x84);

    u32 tmp     = *r2;
    ptr->unk_80 = tmp;
    func_ov037_020837b8(&anim, (tmp * 3) + 2, (tmp * 3) + 3, (tmp * 3) + 4, 1);

    Sprite_Load(&ptr->unk_40, &anim);

    anim.unk_2A = 2;
    Sprite_Load(&ptr->unk_00, &anim);

    return 1;
}

const u32 OpenEnd_TitleScreen_ButtonInfo[][5] = {
    {0x22, 0x1F, 0x19, 0x00, 0x00},
    {0xD7, 0x1F, 0x19, 0x01, 0x01}
};

// Nonmatching
int func_ov037_020838a4(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    TouchCoord   coords, coords2;
    UnkTaskData* unk = r1->data;
    TouchInput_GetCoord(&coords2);

    coords = coords2;

    if (data_ov037_02083e04 != 0 && TouchInput_IsTouchActive() != FALSE &&
        OpenEnd_IsInCircle(&OpenEnd_TitleScreen_ButtonInfo[unk->unk_80][0], coords.x, coords.y) != FALSE)
    {
        unk->unk_00.posX = 130;
        unk->unk_00.posY = 98;
    } else {
        unk->unk_00.posX = 128;
        unk->unk_00.posY = 96;
    }

    if (data_ov037_02083e04 != 0 && TouchInput_WasTouchReleased() != FALSE) {
        if (OpenEnd_IsInCircle(&OpenEnd_TitleScreen_ButtonInfo[unk->unk_80][0], coords.x, coords.y) != FALSE) {
            func_ov037_0208374c(OpenEnd_TitleScreen_ButtonInfo[unk->unk_80][1]);
        }
    }
    Sprite_Update(&unk->unk_00);
    Sprite_Update(&unk->unk_40);
    return 1;
}

int func_ov037_020839ac(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    UnkTaskData* unk = r1->data;
    Sprite_RenderFrame(&unk->unk_40);
    Sprite_RenderFrame(&unk->unk_00);
    return 1;
}

int func_ov037_020839cc(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    UnkTaskData* unk = r1->data;
    Sprite_Release(&unk->unk_00);
    Sprite_Release(&unk->unk_40);
    return 1;
}

s32 func_ov037_020839ec(struct TaskPool* pool, struct Task* task, void* taskParam, s32 index) {
    OpenEndExportFuncTable functable = data_ov037_02083b4c;
    functable.funcs[index](pool, task, taskParam);
}

s32 OpenEnd_CreateBadgeTask(u32 r0) {
    static const TaskHandle TaskHandle_OpenEnd_Badge = {"Tsk_OpenEnd_Badge", func_ov037_020839ec, 0x84};
    u32                     sp8                      = r0;
    return EasyTask_CreateTask(data_ov037_02083e08, &TaskHandle_OpenEnd_Badge, NULL, 0, 0, &sp8);
}