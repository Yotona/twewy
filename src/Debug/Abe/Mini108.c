#include "Debug/Mini108.h"
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
    s32 val1 = data_0206aa80.unk_30;

    if (val1 > 0) {
        val1 = val1 - 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        data_0206aa80.unk_30 = val1;
    } else if (val1 < 0) {
        val1 = val1 + 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        data_0206aa80.unk_30 = val1;
    }

    if (data_0206aa80.unk_60 > 0) {
        val1 = data_0206aa80.unk_60 - 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        data_0206aa80.unk_60 = val1;
    } else if (data_0206aa80.unk_60 < 0) {
        val1 = data_0206aa80.unk_60 + 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        data_0206aa80.unk_60 = val1;
    } else {
        func_02007328();
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
    data_0206aa80.unk_30 = ~0xF;
    data_0206aa80.unk_60 = ~0xF;
    data_0206aa80.unk_00 = 1;
    REG_POWER_CNT        = REG_POWER_CNT | 0x8000;
    data_0206aa80.unk_04 = 1;
    data_0206aa80.unk_08 = 0;
    data_0206aa80.unk_0C = 0;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);
    data_0206aae4.unk_00 = 0;
    data_0206aae4.unk_04 = 0;
    data_0206aae4.unk_1C = 0;
    data_0206aae4.unk_20 = 0;
    data_0206aae4.unk_28 = 2;
    data_0206aae4.unk_2C = 1;
    if (data_0206aa80.unk_0C == 0) {
        REG_BG0CNT = REG_BG0CNT & 0x43 | 0x2008;
    }
    data_0206ab20.unk_00  = 0;
    data_0206ab20.unk_04  = 0;
    data_0206ab20.unk_1C  = 0;
    data_0206ab20.unk_20  = 0;
    data_0206ab20.unk_28  = 1;
    data_0206ab20.unk_2C  = 1;
    REG_BG1CNT            = REG_BG1CNT & 0x43 | 0x2004;
    data_0206ab5c.unk_00  = 0;
    data_0206ab5c.unk_04  = 0;
    data_0206ab5c.unk_1C  = 0;
    data_0206ab5c.unk_20  = 1;
    data_0206ab5c.unk_28  = 1;
    data_0206ab5c.unk_2C  = 1;
    REG_BG2CNT            = REG_BG2CNT & 0x43 | 0x104;
    data_0206ab98.unk_00  = 0;
    data_0206ab98.unk_04  = 0;
    data_0206ab98.unk_1C  = 0;
    data_0206ab98.unk_20  = 0;
    data_0206ab98.unk_28  = 1;
    data_0206ab98.unk_2C  = 1;
    REG_BG3CNT            = REG_BG3CNT & 0x43 | 4;
    u32 bitmagic          = 0 | 4;
    u32 bitmagic1         = bitmagic | 0x10;
    data_0206aa80.unk_1C  = 0;
    data_0206aa80.unk_1C  = bitmagic;
    data_0206aa80.unk_1C  = bitmagic; // | 0x10;
    data_0206aa80.unk_98  = 2;
    data_0206aa80.unk_D4  = 1;
    data_0206aa80.unk_110 = 2;
    data_0206aa80.unk_14C = 3;
    data_0206aa80.unk_9C  = 0;
    data_0206aa80.unk_D8  = 0;
    data_0206aa80.unk_114 = 0;
    data_0206aa80.unk_150 = 0;
    bitmagic              = bitmagic | 1;
    data_0206aa80.unk_1C  = bitmagic;
    data_0206aa80.unk_38  = 0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    data_0206ad04.unk_00 = 0;
    data_0206ad04.unk_04 = 0;
    data_0206ad04.unk_1C = 0;
    data_0206ad04.unk_20 = 0;
    data_0206ad04.unk_28 = 2;
    data_0206ad04.unk_2C = 0;
    REG_BG0CNT_SUB       = REG_BG0CNT_SUB & 0x43 | 8;
    data_0206ad40.unk_00 = 0;
    data_0206ad40.unk_04 = 0;
    data_0206ad40.unk_1C = 0;
    data_0206ad40.unk_20 = 0;
    data_0206ad40.unk_28 = 1;
    data_0206ad40.unk_2C = 0;
    REG_BG1CNT_SUB       = REG_BG1CNT_SUB & 0x43 | 4;
    data_0206ad7c.unk_00 = 0;
    data_0206ad7c.unk_04 = 0;
    data_0206ad7c.unk_1C = 0;
    data_0206ad7c.unk_20 = 1;
    data_0206ad7c.unk_28 = 1;
    data_0206ad7c.unk_2C = 1;
    REG_BG2CNT_SUB       = REG_BG2CNT_SUB & 0x43 | 0x104;
    data_0206adb8.unk_00 = 0;
    data_0206adb8.unk_04 = 0;
    data_0206adb8.unk_1C = 0;
    data_0206adb8.unk_20 = 0;
    data_0206adb8.unk_28 = 1;
    data_0206adb8.unk_2C = 1;
    REG_BG3CNT_SUB       = REG_BG3CNT_SUB & 0x43 | 4;
    data_0206aa80.unk_4C = 0;
    data_0206aa80.unk_4C = 0 | 0x4;
    data_0206aa80.unk_4C = (0 | 0x4) | 0x10;

    data_0206aa80.unk_2B8 = 2;
    data_0206aa80.unk_2F4 = 1;
    data_0206aa80.unk_330 = 2;
    data_0206aa80.unk_36C = 3;
    data_0206aa80.unk_2BC = 0;
    data_0206aa80.unk_2F8 = 0;
    data_0206aa80.unk_334 = 0;
    data_0206aa80.unk_370 = 0;
    data_0206aa80.unk_4C  = ((0 | 0x4) | 0x10) | 1;
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
    func_02009a74(data_0206b3c4.unk_00, temp_r1, data_0206aa80.unk_104, 0, r2_val);
    func_02009f18(data_0206b3c4.unk_00, temp_r6, data_0206aa80.unk_FC, data_0206aa80.unk_E0);
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
    func_02009a74(data_0206b3c4.unk_04, temp_r1, data_0206aa80.unk_324, 0, r2_val);
    func_02009f18(data_0206b3c4.unk_04, temp_r6, data_0206aa80.unk_31C[0], data_0206aa80.unk_300[0]);
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
    func_020072ec();
    func_02007300(func_ov000_020830b8, state, 0);
    func_02007300(func_ov000_020830bc, state, 0);
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
    func_02007260(0);
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
    func_02007390();

    if ((stateptr->pressedButtons & INPUT_BUTTON_START) != 0) {
        u32 unk = func_02011f44("Seq_Mini108(void *) reset");
        if (unk == 1) {
            data_02066a58 = data_02066a58 | 2;
        }
        func_020071f4(s_04, (s32)&OVERLAY_0_ID, &func_ov000_02082854, NULL, 0);
        return;
    } else if ((stateptr->pressedButtons & INPUT_BUTTON_SELECT) != 0) {
        stateptr->unk_00 = 1;
    }
    if (stateptr->unk_00 == 1) {
        u32 var3 = func_02011f44("Seq_Mini108(void *) leave");
        if (var3 == 1) {
            data_02066a58 = data_02066a58 | 2;
        }
        func_02007174(s_0C);
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
        func_02007260(stateptr);
    }
    func_0203b2d0(0, stateptr, Mem_GetBlockSize(&gDebugHeap, stateptr));
    func_ov000_02082b1c(stateptr);
    func_ov000_02082b18(stateptr);
    func_020072a4();
    func_020072b8();
}

void func_ov000_02082854(Mini108State* state) {
    static const Mini108Func funcPtrs[3] = {
        func_ov000_02082894,
        func_ov000_02082944,
        func_ov000_02082ac0,
    };

    int var1 = func_02007278();
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
    func_02005250();
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
    data_0206aa80.unk_10  = 0x200010;
    data_0206aa80.unk_14  = 0x40;
    data_0206aa80.unk_44  = 0x40;
    data_0206aa80.unk_40  = 0x100010;
    func_0200270c(0, 0); // Might also use &data_0206aa80 as third parameter?
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
    func_02006380();
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
