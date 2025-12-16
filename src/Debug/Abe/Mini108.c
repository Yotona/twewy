#include "Debug/Mini108.h"
#include "Display.h"
#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "System.h"
#include "common_data.h"
#include "registers.h"
#include <NitroSDK/gx.h>

void func_ov000_020824a0(void);
void func_ov000_0208257c(void);
void func_ov000_0208259c(void);
void func_ov000_02082854(Mini108State* state);

typedef void (*Mini108Func)(Mini108State*);

BinIdentifier data_ov000_020831c0 = {0, "Apl_Abe/Grp_Mini108.bin"};

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_Mini108_tuple;

const UnkStruct_Mini108_tuple data_ov000_020831a8 = {0, 0x1000, 0};
const UnkStruct_Mini108_tuple data_ov000_020831b4 = {0, 0, 0xFFFFF000};

void func_ov000_020830bc() {
    s32 val1 = g_DisplaySettings.mainControl.brightness;

    if (val1 > 0) {
        val1 = val1 - 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.mainControl.brightness = val1;
    } else if (val1 < 0) {
        val1 = val1 + 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.mainControl.brightness = val1;
    }

    if (g_DisplaySettings.subControl.brightness > 0) {
        val1 = g_DisplaySettings.subControl.brightness - 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.subControl.brightness = val1;
    } else if (g_DisplaySettings.subControl.brightness < 0) {
        val1 = g_DisplaySettings.subControl.brightness + 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.subControl.brightness = val1;
    } else {
        DebugOvlDisp_Pop();
    }
}

void func_ov000_020830b8() {
    return;
}

// Nonmatching: Data references, register allocation, and bitmagic operations that don't match (see below)
void func_ov000_02082b1c(Mini108State* state) {
    state->unk_00 = 0;
    state->unk_10 = 0;
    func_ov000_0208259c();

    Display_SetMainBrightness(-16);
    Display_SetSubBrightness(-16);

    g_DisplaySettings.unk_000 = 1;
    REG_POWER_CNT             = REG_POWER_CNT | 0x8000;

    g_DisplaySettings.mainControl.dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.mainControl.bgMode    = GX_BGMODE_0;
    g_DisplaySettings.mainControl.dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    DisplayBGSettings* mainBg0 = Display_GetBG0Settings(DISPLAY_ENGINE_MAIN);
    mainBg0->bgMode            = 0;
    mainBg0->screenSizeText    = 0;
    mainBg0->colorMode         = 0;
    mainBg0->screenBase        = 0;
    mainBg0->charBase          = 2;
    mainBg0->extPlttSlot       = 1;

    if (g_DisplaySettings.mainControl.dimension == GX2D3D_MODE_2D) {
        REG_BG0CNT = REG_BG0CNT & 0x43 | 0x2008;
    }

    DisplayBGSettings* mainBg1 = Display_GetBG1Settings(DISPLAY_ENGINE_MAIN);
    mainBg1->bgMode            = 0;
    mainBg1->screenSizeText    = 0;
    mainBg1->colorMode         = 0;
    mainBg1->screenBase        = 0;
    mainBg1->charBase          = 1;
    mainBg1->extPlttSlot       = 1;
    REG_BG1CNT                 = REG_BG1CNT & 0x43 | 0x2004;

    DisplayBGSettings* mainBg2 = Display_GetBG2Settings(DISPLAY_ENGINE_MAIN);
    mainBg2->bgMode            = 0;
    mainBg2->screenSizeText    = 0;
    mainBg2->colorMode         = 0;
    mainBg2->screenBase        = 1;
    mainBg2->charBase          = 1;
    mainBg2->extPlttSlot       = 1;
    REG_BG2CNT                 = REG_BG2CNT & 0x43 | 0x104;

    DisplayBGSettings* mainBg3 = Display_GetBG3Settings(DISPLAY_ENGINE_MAIN);
    mainBg3->bgMode            = 0;
    mainBg3->screenSizeText    = 0;
    mainBg3->colorMode         = 0;
    mainBg3->screenBase        = 0;
    mainBg3->charBase          = 1;
    mainBg3->extPlttSlot       = 1;
    REG_BG3CNT                 = REG_BG3CNT & 0x43 | 4;

    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.mainControl.layers = LAYER_BG0 | LAYER_BG2 | LAYER_OBJ;
    g_DisplaySettings.subControl.bgMode  = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    DisplayBGSettings* subBg0 = Display_GetBG0Settings(DISPLAY_ENGINE_SUB);
    subBg0->bgMode            = 0;
    subBg0->screenSizeText    = 0;
    subBg0->colorMode         = 0;
    subBg0->screenBase        = 0;
    subBg0->charBase          = 2;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = (REG_BG0CNT_SUB & 0x43) | 8;

    DisplayBGSettings* subBg1 = Display_GetBG1Settings(DISPLAY_ENGINE_SUB);
    subBg1->bgMode            = 0;
    subBg1->screenSizeText    = 0;
    subBg1->colorMode         = 0;
    subBg1->screenBase        = 0;
    subBg1->charBase          = 1;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = (REG_BG1CNT_SUB & 0x43) | 4;

    DisplayBGSettings* subBg2 = Display_GetBG2Settings(DISPLAY_ENGINE_SUB);
    subBg2->bgMode            = 0;
    subBg2->screenSizeText    = 0;
    subBg2->colorMode         = 0;
    subBg2->screenBase        = 1;
    subBg2->charBase          = 1;
    subBg2->extPlttSlot       = 1;
    REG_BG2CNT_SUB            = REG_BG2CNT_SUB & 0x43 | 0x104;

    DisplayBGSettings* subBg3 = Display_GetBG3Settings(DISPLAY_ENGINE_SUB);
    subBg3->bgMode            = 0;
    subBg3->screenSizeText    = 0;
    subBg3->colorMode         = 0;
    subBg3->screenBase        = 0;
    subBg3->charBase          = 1;
    subBg3->extPlttSlot       = 1;
    REG_BG3CNT_SUB            = REG_BG3CNT_SUB & 0x43 | 4;

    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_ENGINE_SUB].bgSettings[3].mosaic = 0;

    g_DisplaySettings.subControl.layers = LAYER_BG0 | LAYER_BG2 | LAYER_OBJ;

    func_02006ad8();
    s32 temp         = func_0200cef0(&state->unk_48D0);
    state->unk_15e50 = temp;
    Data* temp1      = DatMgr_LoadRawData(2, 0, 0, &data_ov000_020831c0);
    state->unk_84C   = temp1;
    data_02066aec    = 0x1f;

    Data* temp2 = state->unk_84C;
    u32   temp_r1, temp_r6, temp_r5;
    if (temp2 == NULL) {
        temp_r1 = 0;
    } else {
        s32* temp_ptr = (s32*)(temp2->buffer) + 0x08;
        temp_r1       = (s32)temp_ptr + *(temp_ptr + 0x02);
    }
    if (temp2 == NULL) {
        temp_r6 = 0;
    } else {
        s32* temp_ptr = (s32*)(temp2->buffer) + 0x08;
        temp_r6       = (s32)temp_ptr + *(temp_ptr + 0x06);
    }
    if (temp2 == NULL) {
        temp_r5 = 0;
    } else {
        s32* temp_ptr = (s32*)(temp2->buffer) + 0x08;
        temp_r5       = (s32)temp_ptr + *(temp_ptr + 0x08);
    }

    u32 r2_val = (*(u32*)temp_r1 & ~0xFF) >> 8;
    if (((*(u8*)temp_r1) & 0xF0) == 0) {
        r2_val -= 4;
    }
    func_02009a74(data_0206b3c4.unk_00, temp_r1, g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, r2_val);
    func_02009f18(data_0206b3c4.unk_00, temp_r6, g_DisplaySettings.engineState[0].bgSettings[2].screenBase,
                  g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText);
    func_0200adf8(data_0206b3cc.unk_00, temp_r5, 0, 0, 2);
    data_02066eec = 0x1F;
    temp2         = state->unk_84C;
    if (temp2 == 0) {
        temp_r1 = 0;
    } else {
        s32* temp_ptr = (s32*)(*(s32*)(temp2 + 0x08)) + 0x08;
        temp_r1       = (s32)temp_ptr + *(temp_ptr + 0x02);
    }
    if (temp2 == 0) {
        temp_r6 = 0;
    } else {
        s32* temp_ptr = (s32*)(*(s32*)(temp2 + 0x08)) + 0x08;
        temp_r6       = (s32)temp_ptr + *(temp_ptr + 0x04);
    }
    if (temp2 == 0) {
        temp_r5 = 0;
    } else {
        s32* temp_ptr = (s32*)(*(s32*)(temp2 + 0x08)) + 0x08;
        temp_r5       = (s32)temp_ptr + *(temp_ptr + 0x08);
    }

    r2_val = (*(u32*)temp_r1 & ~0xFF) >> 8;
    if (((*(u8*)temp_r1) & 0xF0) == 0) {
        r2_val -= 4;
    }
    func_02009a74(data_0206b3c4.unk_04, temp_r1, g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, r2_val);
    func_02009f18(data_0206b3c4.unk_04, temp_r6, g_DisplaySettings.engineState[1].bgSettings[2].screenBase,
                  g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText);
    func_0200adf8(data_0206b3c4.unk_04, temp_r5, 0, 0, 2);

    // func_0200eed4(&state->unk_84C->slotIndex, &state->unk_8D0, 0x4000, 0x100); is what this is. But it throws "IllegaL
    // operand" error.
    func_0200eed4(&state->unk_84C, &state->unk_8D0, 0x4000,
                  0x100); // TODO: is it really using slotIndex or is it using another flag?
    UnkStruct_ov000_02082b1c s_04;
    s_04.unk_00 = 0;
    s_04.unk_04 = 0;
    s_04.unk_08 = 2;
    s_04.unk_0C = (s32)&data_0205c9b0;
    s_04.unk_10 = 5;
    s_04.unk_14 = 0;
    s_04.unk_18 = 0;
    s_04.unk_1A = 0;
    s_04.unk_1C = 0x20;
    s_04.unk_1E = 0x18;
    func_02025b68(&state->unk_14, &s_04);
    s_04.unk_00 = 1;
    func_02025b68(&state->unk_430, &s_04);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push(func_ov000_020830b8, state, 0);
    DebugOvlDisp_Push(func_ov000_020830bc, state, 0);
    state->unk_0C = 0;
    if ((InputStatus.currButtons & INPUT_BUTTON_L) != 0) {
        state->unk_0C = 1;
    }
}

void func_ov000_02082b18(Mini108State* unk_r0) {
    return;
}

void func_ov000_02082ac0(Mini108State* state) {
    if (state == NULL) {
        return;
    }
    DatMgr_ClearSlot(0x2);
    func_0200ef80(&state->unk_84C); // should have ->slotIndex(?) but throws "Illegal operand" error if it has that.
    func_02025e30(&state->unk_14);
    func_02025e30(&state->unk_430);
    func_ov000_0208257c();
    func_0200cfe8(state->unk_15e50);
    Mem_Free(&gDebugHeap, state);
    MainOvlDisp_SetState(0);
}

// Nonmatching: Opcode order swap, Stack difference
void func_ov000_02082944(Mini108State* stateptr) {
    u16 a, b;

    b                        = InputStatus.currButtons;
    a                        = InputStatus.pressedButtons;
    stateptr->currButtons    = b;
    stateptr->pressedButtons = a;

    b                     = InputStatus.holdButtons;
    a                     = InputStatus.prevButtons;
    stateptr->prevButtons = a;
    stateptr->holdButtons = b;
    OverlayTag* s_04;
    OverlayTag* s_0C;
    func_02006ba0();

    Text_RenderToScreen(stateptr->unk_80, 0, 0xa, func_02006930("Score : %6d", stateptr->unk_40));
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    DebugOvlDisp_Run();

    if ((stateptr->pressedButtons & INPUT_BUTTON_START) != 0) {
        u32 unk = func_02011f44("Seq_Mini108(void *) reset");
        if (unk == 1) {
            data_02066a58 = data_02066a58 | 2;
        }
        MainOvlDisp_ReplaceTop(s_04, (s32)&OVERLAY_0_ID, &func_ov000_02082854, NULL, 0);
        return;
    } else if ((stateptr->pressedButtons & INPUT_BUTTON_SELECT) != 0) {
        stateptr->unk_00 = 1;
    }
    if (stateptr->unk_00 == 1) {
        u32 var3 = func_02011f44("Seq_Mini108(void *) leave");
        if (var3 == 1) {
            data_02066a58 = data_02066a58 | 2;
        }
        MainOvlDisp_Pop(s_0C);
        return;
    }
    func_0200efdc(&stateptr->unk_84C); // should have ->slotIndex(?) but throws "Illegal operand" error if it has that.

    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

const char* data_ov000_020831e0 = "Seq_Mini108(void *)";

void func_ov000_02082894(Mini108State* stateptr) {
    if (func_02011f44("Seq_Mini108(void *) enter") == 1) {
        data_02066a58 = data_02066a58 | 2;
    }
    if (stateptr == NULL) {
        const char* temp = data_ov000_020831e0;
        stateptr         = Mem_AllocHeapTail(&gDebugHeap, 0x15e54);
        Mem_SetSequence(&gDebugHeap, stateptr, temp);
        MainOvlDisp_SetState(stateptr);
    }
    func_0203b2d0(0, stateptr, Mem_GetBlockSize(&gDebugHeap, stateptr));
    func_ov000_02082b1c(stateptr);
    func_ov000_02082b18(stateptr);
    MainOvlDisp_IncrementRepeatCount();
    MainOvlDisp_Run();
}

void func_ov000_02082854(Mini108State* state) {
    static const Mini108Func funcPtrs[3] = {
        func_ov000_02082894,
        func_ov000_02082944,
        func_ov000_02082ac0,
    };

    int var1 = MainOvlDisp_GetRepeatCount();
    if (var1 == 0x7FFFFFFF) {
        func_ov000_02082ac0(state);
    } else {
        Mini108Func target_function = funcPtrs[var1];
        target_function(state);
    }
}

void func_ov000_020825c0(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    G3X_Init();
    G3X_InitMtxStack();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_A);
    GX_SetBankForBg(GX_VRAM_B);
    GX_SetBankForObj(GX_VRAM_E);
    GX_SetBankForTexPltt(GX_VRAM_F);
    GX_SetBankForBgExtPltt(GX_VRAM_G);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_D);
    GX_SetBankForSubBgExtPltt(GX_VRAM_H);
    GX_SetBankForSubObjExtPltt(GX_VRAM_I);
    REG_DISP3DCNT = REG_DISP3DCNT & 0xffffcffd;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x08;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x10;
    G3X_SetClearColor(0, 0, 0x7fff, 0x3f, 0);
    GFX_FIFO_VIEWPORT     = 0xbfff0000;
    GFX_FIFO_SWAP_BUFFERS = 0;
    G3i_OrthoW(0, 0xc0000, 0, 0x100000, 0 - 0x400000, 0x400000, 0x400000, 1, 0);

    UnkStruct_Mini108_tuple unk_14 = {0, 0, 0};
    UnkStruct_Mini108_tuple unk_20 = data_ov000_020831a8;
    UnkStruct_Mini108_tuple unk_2C = data_ov000_020831b4;
    G3i_LookAt(&unk_14, &unk_20, &unk_2C, 1, 0);

    GFX_FIFO_MATRIX_STORE = 0;

    g_DisplaySettings.mainControl.objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.mainControl.objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.subControl.objBmpMode   = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.subControl.objTileMode  = GX_OBJTILEMODE_1D_64K;

    func_0200270c(0, 0); // Might also use &g_DisplaySettings as third parameter?
    func_0200270c(0, 1);
    func_0203b2d0(0, 0x6800000, 0xa4000);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);

    func_02001c34(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

void func_ov000_0208259c(void) {
    func_ov000_020825c0();
    Interrupts_RegisterVBlankCallback(func_ov000_020824a0, TRUE);
}

void func_ov000_0208257c(void) {
    func_ov000_020825c0();
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov000_020824a0(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0) == 0)
        return;
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
