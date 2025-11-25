#include "Debug/Launcher.h"

void func_ov046_020824a0(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    G3X_Init();
    G3X_InitMtxStack();
    DMA_Init(0x100);
    func_02005250();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1ff);
    GX_SetBankForTex(1);
    GX_SetBankForBg(2);
    GX_SetBankForObj(0x10);
    GX_SetBankForTexPltt(0x20);
    GX_SetBankForBgExtPltt(0x40);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    GX_SetBankForSubBgExtPltt(0x80);
    GX_SetBankForSubObjExtPltt(0x100);
    REG_DISP3DCNT = REG_DISP3DCNT & 0xffffcffd;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x8;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x10;
    G3X_SetClearColor(0, 0, 0x7fff, 0x3f, 0);
    GFX_FIFO_VIEWPORT     = 0xbfff0000;
    GFX_FIFO_SWAP_BUFFERS = 0;
    G3i_OrthoW(0, 0xc0000, 0, 0x100000, 0xffc00000, 0x400000, 0x400000, 1, 0);

    s32                           unk_14[3] = {0, 0, 0};
    UnkStruct_DebugLauncher_tuple unk_20    = data_ov046_02083a34;
    UnkStruct_DebugLauncher_tuple unk_2C    = data_ov046_02083a40;
    G3i_LookAt(&unk_14, &unk_20, &unk_2C, 1, 0);
    GFX_FIFO_MATRIX_STORE = 0;

    data_0206aa80.unk_10 = 0x200010;
    data_0206aa80.unk_14 = 0x40;
    data_0206aa80.unk_44 = 0x40;
    data_0206aa80.unk_40 = 0x100010;

    func_0200270c(0, 0);
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

void func_ov046_02082720(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
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
}

void func_ov046_020827f0(void) {
    func_ov046_020824a0();
    Interrupts_RegisterVBlankCallback(func_ov046_02082720, TRUE);
}

void func_ov046_0208280c(void) {
    func_ov046_020824a0();
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov046_02082824(void) {
    func_02026180(0, 0, 0x800);
    if (func_0202623c() == FALSE) {
        func_02007328();
    }
}

void func_ov046_0208284c(void) {
    func_02026180(0, -0x20, 0x800);
    if (func_0202623c() == FALSE) {
        func_02007328();
    }
}

/*
 Nonmatching: Stack frame not fully figured out, requires decompiling functions called in here first.
*/
void func_ov046_02082874(DebugLauncherState* state) {
    /*
    Stack frame (size 0x20):
    0x00: u32 //unknown
    0x04: u16 //?
    0x06: u16 //?
    0x08: u32 //touchX?
    0x0C: u32 //touchY?
    0x10: u32 //
    0x14: u32 //
    0x18: u32 //gridX?
    0x1C: u32 //gridY?
    */
    s32 stackframe[8];

    func_02006df0(stackframe[2]);  // populate touchscreen info?

    if (func_02006d4c() == TRUE) { // if OK button pressed?
        if (stackframe[3] < 64) {
            u32 selectedCell = (((stackframe[3] - 64) / 32) * 8) + (stackframe[2] / 32);
            if (selectedCell == 0xF) { // Why 0xF?
                if (state->selectedCategoryIndex >= 0 && state->selectedOptionIndex >= 0) {
                    func_02007328();
                    u32                          categoryIndex = state->selectedCategoryIndex;
                    u32                          optionIndex   = state->selectedOptionIndex;
                    const DebugLauncherCategory* category      = &Categories[categoryIndex];
                    const DebugLauncherOption*   option        = &category->options[optionIndex];
                    // state->overlay = option->overlayId;
                    // state->overlayCB = option->overlayCB;
                    OverlayCB overlayCB = option->overlayCB;
                    u32       overlayId = option->overlayId;
                    state->overlay      = overlayId;
                    state->overlayCB    = overlayCB;
                    return;
                }
            }
            return;
        }
    }

    if (func_02006d30() != TRUE) // if Category button pressed, render option buttons?
        return;
    if (stackframe[3] < 64) {
        u32 selectedCell = (((stackframe[3] - 64) / 32) * 8) + (stackframe[2] / 32);
        if (selectedCell < state->unk_1C) {
            if (selectedCell != state->selectedCategoryIndex) {
                func_ov046_02083670(func_0200f850(&state->unk_11650, state->unk_24[state->selectedCategoryIndex]), FALSE);
            }
            func_ov046_02083670(func_0200f850(&state->unk_11650, state->unk_24[selectedCell]), TRUE);
            for (s32 i = 0; i < 0x18; i++) {
                if (state->unk_6C[i] != 0xFFFFFFFF) {
                    func_ov046_0208368c(func_0200f850(&state->unk_11650)); // unk_11650 is a DebugLauncherState pointer?
                    state->unk_6C[i] = 0xFFFFFFFF;
                    if (&Categories[selectedCell].options != NULL) {
                        if (((&Categories[selectedCell])->options)->unk_00 > 0) {
                            const DebugLauncherOption* option = (&Categories[selectedCell])->options;
                            u32                        unkownIndex, catIndex = 0;
                            do {
                                stackframe[4] = state->unk_14;
                                stackframe[6] = ((unkownIndex << 0x1D) >>
                                                 0x18); // Touch screen X position calculation, what's with the bit shifts?
                                stackframe[7] =
                                    ((unkownIndex * 8) / 32) + 64; // Touch screen Y position calculation, might be signed?
                                stackframe[5] = option;            // huh
                                stackframe[0] = 0;                 // NULL?
                                // render option button?
                                state->unk_6C[i] = func_0200f68c(&state->unk_11650, &data_ov046_02083f70, stackframe[0],
                                                                 stackframe[0], stackframe[0], &stackframe[5]);
                                unkownIndex++;
                                catIndex++;
                            } while (((&Categories[catIndex])->options)->unk_00 > 0);
                        }
                        state->unk_64 = 0;
                    }
                    func_02010b84(&state->unk_156E8, 0x0, 0x80, 0x100, 0x0);
                    Text_RenderToScreen(&state->unk_156E8, 0x8, 0x80, &Categories[selectedCell].creator);
                    Text_RenderToScreen(&state->unk_156E8, 0x8, 0x80, &Categories[selectedCell].description);
                    state->selectedCategoryIndex = selectedCell;
                    state->selectedOptionIndex   = 0xFFFFFFFF;
                    func_02010b84(&state->unk_15B04, 0x0, 0x60, 0x100, 0x20);
                    return;
                }
            }
            return;
        }
        return;
    }

    // Option selected, render option info
    u32 selectedCell = (((stackframe[3] - 64) / 32) * 8) + (stackframe[2] / 32);
    if (selectedCell >= state->unk_64 || selectedCell == state->selectedOptionIndex)
        return;
    if (state->selectedOptionIndex >= 0) {
        func_ov046_02083670(func_0200f850(&state->unk_11650, state->unk_6C[state->selectedOptionIndex]), FALSE);
    }
    func_ov046_02083670(func_0200f850(&state->unk_11650, state->unk_6C[selectedCell]), TRUE);
    func_02010b84(&state->unk_156E8, 0x0, 0x98, 0x100, 0x20);
    Text_RenderToScreen(&state->unk_156E8, 0x8, 0x98,
                        (&Categories[state->selectedCategoryIndex])->options[selectedCell].function);
    Text_RenderToScreen(&state->unk_156E8, 0x8, 0xA0,
                        (&Categories[state->selectedCategoryIndex])->options[selectedCell].description);
    state->selectedOptionIndex = selectedCell;
}

void func_ov046_02082c0c(EasyListNode* list, BOOL unk_r1) {
    if (unk_r1 == TRUE) {
        switch (list->priority) {
            case 0:
                func_0200cfe8(list->data);
                return;
            case 1:
                func_02008ebc(list->data);
                return;
            case 2:
                func_02008dbc(list->data);
                return;
            case 3:
                func_0200f4f8(list->data);
                return;
            case 4:
                func_02025e30(list->data);
                return;
        }
    }
}

/*
 Not fully implemented, function was too big to implement, likely an inline.
*/
void func_ov046_02082c78(DebugLauncherState* state) {
    state->active                = FALSE;
    state->unk_1C                = 0;
    state->selectedCategoryIndex = -1;
    state->unk_64                = 0;
    state->selectedOptionIndex   = -1;
    for (u32 i = 0; i < 0x18; i++) {
        state->unk_6C[i] = 0xFFFFFFFF;
    }
    func_ov046_020827f0();
    data_0206aa80.unk_30 = ~0xf;
    data_0206aa80.unk_60 = ~0xf;
    func_02006390();
    data_0206aa80.unk_00 = 0;
    REG_POWER_CNT        = REG_POWER_CNT & 0x7fff;
    data_0206aa80.unk_04 = 1;
    data_0206aa80.unk_08 = 0;
    data_0206aa80.unk_0C = 0;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);
    data_0206aae4.unk_00 = 0;
    data_0206aae4.unk_04 = 0;
    data_0206aae4.unk_1C = 0;
    data_0206aae4.unk_20 = 0;
    data_0206aae4.unk_28 = 0;
    data_0206aae4.unk_2C = 0;
    if (data_0206aa80.unk_0C == 0) {
        REG_BG0CNT = REG_BG0CNT & 0x43 | 0x2004;
    }
    data_0206ab20.unk_00  = 0;
    data_0206ab20.unk_04  = 0;
    data_0206ab20.unk_1C  = 0;
    data_0206ab20.unk_20  = 1;
    data_0206ab20.unk_28  = 4;
    data_0206ab20.unk_2C  = 1;
    REG_BG1CNT            = REG_BG1CNT & 0x43 | 0x2110;
    data_0206ab5c.unk_00  = 0;
    data_0206ab5c.unk_04  = 0;
    data_0206ab5c.unk_1C  = 0;
    data_0206ab5c.unk_20  = 0;
    data_0206ab5c.unk_28  = 1;
    data_0206ab5c.unk_2C  = 1;
    REG_BG2CNT            = REG_BG2CNT & 0x43 | 4;
    data_0206ab98.unk_00  = 0;
    data_0206ab98.unk_04  = 0;
    data_0206ab98.unk_1C  = 0;
    data_0206ab98.unk_20  = 0;
    data_0206ab98.unk_28  = 1;
    data_0206ab98.unk_2C  = 1;
    REG_BG3CNT            = REG_BG3CNT & 0x43 | 4;
    data_0206aa80.unk_98  = 0;
    data_0206aa80.unk_D4  = 1;
    data_0206aa80.unk_110 = 2;
    data_0206aa80.unk_14C = 3;
    data_0206aa80.unk_9C  = 0;
    data_0206aa80.unk_D8  = 0;
    data_0206aa80.unk_114 = 0;
    data_0206aa80.unk_150 = 0;
    data_0206aa80.unk_1C  = 0 | 0x13;
    data_0206aa80.unk_38  = 0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    data_0206ad04.unk_00  = 0;
    data_0206ad04.unk_04  = 0;
    data_0206ad04.unk_1C  = 0;
    data_0206ad04.unk_20  = 0;
    data_0206ad04.unk_28  = 1;
    data_0206ad04.unk_2C  = 0;
    REG_BG0CNT_SUB        = REG_BG0CNT_SUB & 0x43 | 4;
    data_0206ad40.unk_28  = 4;
    data_0206ad40.unk_00  = 0;
    data_0206ad40.unk_04  = 0;
    data_0206ad40.unk_1C  = 0;
    data_0206ad40.unk_20  = 1;
    data_0206ad40.unk_2C  = 0;
    REG_BG1CNT_SUB        = REG_BG1CNT_SUB & 0x43 | 0x110;
    data_0206ad7c.unk_00  = 0;
    data_0206ad7c.unk_04  = 0;
    data_0206ad7c.unk_1C  = 0;
    data_0206ad7c.unk_20  = 0;
    data_0206ad7c.unk_28  = 1;
    data_0206ad7c.unk_2C  = 1;
    REG_BG2CNT_SUB        = REG_BG2CNT_SUB & 0x43 | 4;
    data_0206adb8.unk_00  = 0;
    data_0206adb8.unk_04  = 0;
    data_0206adb8.unk_1C  = 0;
    data_0206adb8.unk_20  = 0;
    data_0206adb8.unk_28  = 1;
    data_0206adb8.unk_2C  = 1;
    REG_BG3CNT_SUB        = REG_BG3CNT_SUB & 0x43 | 4;
    data_0206aa80.unk_2B8 = 0;
    data_0206aa80.unk_2F4 = 1;
    data_0206aa80.unk_330 = 2;
    data_0206aa80.unk_36C = 3;
    data_0206aa80.unk_2BC = 0;
    data_0206aa80.unk_2F8 = 0;
    data_0206aa80.unk_334 = 0;
    data_0206aa80.unk_370 = 0;
    data_0206aa80.unk_4C  = 0x0 | 0x3;
    data_0206aa80.unk_4C  = 0x0 | 0x3 | 0x4; // what
    func_02006ad8();
    EasyList_Init(&state->unk_list_15F14, NULL, 32, func_ov046_02082c0c);
    state->unk_1164C = func_0200cef0(state->unk_CC);
    EasyList_InsertSorted(&state->unk_list_15F14, NULL, func_0200cef0(state->unk_CC)); // is it actually NULL?
    state->unk_14 = func_02008e80();
    // stackframe[0] = 0x1;
    // stackframe[1] = 0x0;
    state->unk_18 = func_02008650(state->unk_14, 0, 0, &data_ov046_02083a4c);
    EasyList_InsertSorted(&state->unk_list_15F14, 1, state->unk_14);
    EasyList_InsertSorted(&state->unk_list_15F14, 2, state->unk_18);
    data_02066aec = 0x1F;
    // s32 r5 = state->unk_18 == NULL ? 0 : (state->unk_18)->unk_08 + 0x20 + *(s32*)((((state->unk_18)->unk_08) + 0x20) +
    // 0x08); s32 r6 = state->unk_18 == NULL ? 0 : (state->unk_18)->unk_08 + 0x20 + *(s32*)((((state->unk_18)->unk_08) + 0x20)
    // + 0x10); s32 r7 = state->unk_18 == NULL ? 0 : (state->unk_18)->unk_08 + 0x20 + *(s32*)((((state->unk_18)->unk_08) +
    // 0x20) + 0x20);

    /*
    Not fully implemented, function was too big to implement, likely an inline.
    */
}

/*Not implemented: Unclear stack frame, need to research func_0200f68c*/
void func_ov046_02083368(DebugLauncherState* state) {
    /*
    Stack frame:
    0x0 -> used for function calls
    0x4 -> used for function calls
    0x8 -> used as parameter for function calls? passed by reference?
    0xC -> Categories[index]? Categories[index]->unk_00?
    0x10 -> index modulus something?
    0x14 -> uhhhh TouchGridX?
    */
    func_0200f68c(&state->unk_11650, &data_0205cb10, 0, 0, 0, 0);
    // index = 0;
    // stack->0xC = Categories[index](.unk_00?);
    // stack->0x10 = (index << 0x1D) >> 0x18; // what? is this just "index % 8"?
    // stack->0x14 = (index / 8) * 32;
    // func_0200f68c(&state->unk_11650, &data_ov046_02083f70, index, index, index, &stack->0x8);
    // index++
    // state->unk_24[index] = result of last func-call
    //  Loop back if(Categories[index](.unk_00?) > 0)
    //  Else state->unk_1C = index;

    // stack->0x14 = 0x60
    // stack->0xC = 0x26
    // stack->0x10 = 0xe0
    // state->unk_60 = func_0200f68c(&state->unk_11650, &data_ov046_02083f70, 0, 0, 0, &stack->0x8);
}

/*Nonmatching: The start of the loop is different in the assembly than in the code.*/
void func_ov046_destructor_02083454(DebugLauncherState* state) {
    BOOL finishedCleaning;
    if (state == NULL) {
        return;
    }
    do {
        if (state->unk_list_15F14.tailSentinel.prev != NULL) {
            EasyList_RemoveNode(&(state->unk_list_15F14), state->unk_list_15F14.tailSentinel.prev);
        }
        finishedCleaning = FALSE; // the assembly starts the loop from here
        if (state->unk_list_15F14.headSentinel.next == NULL) {
            finishedCleaning = TRUE;
        }
    } while (finishedCleaning == FALSE);
    func_ov046_0208280c();
    Mem_Free(&gDebugHeap, state);
    func_02007260(NULL);
}

/*Nonmatching: Opcode reordering in the first block of the function*/
void func_ov046_main_020834c0(DebugLauncherState* state) {
    u16 uVar1             = InputStatus.pressedButtons;
    state->currButtons    = InputStatus.currButtons;
    state->pressedButtons = uVar1;
    uVar1                 = InputStatus.prevButtons;
    state->holdButtons    = InputStatus.holdButtons;
    state->prevButtons    = uVar1;

    func_02006ba0();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_02007390();

    if (func_0200737c() == TRUE) {
        state->active = TRUE;
    }

    if (state->active == TRUE) {
        OverlayTag tag;
        func_020071f4(&tag, state->overlay, state->overlayCB, NULL, 0);
    } else {
        func_0200f514(&state->unk_11650);
        func_020034b0(&data_020676ec);
        func_020034b0(&data_02068778);
        func_0200bf60(data_0206b3cc.unk_00, 0);
        func_0200bf60(data_0206b3cc.unk_04, 0);
    }
}

void func_ov046_constructor_020835b4(DebugLauncherState* state) {
    if (state == NULL) {
        const char* seq = Sequence;
        state           = Mem_AllocHeapTail(&gDebugHeap, sizeof(DebugLauncherState));
        Mem_SetSequence(&gDebugHeap, state, seq);
        func_02007260(state);
    }

    func_0203b2d0(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    func_ov046_02082c78(state);
    func_ov046_02083368(state);
    func_020072a4();
}

// Overlay Init function
void func_ov046_02083630(DebugLauncherState* state) {

    s32 sVar1 = func_02007278();
    if (sVar1 == 0x7fffffff) {
        func_ov046_destructor_02083454(state);
    } else {
        funcs[sVar1](state);
    }
}

void func_ov046_02083670(u16* unkptr, BOOL param_2) {
    *(unkptr + 0x0D) = (param_2 == TRUE) ? ((*(unkptr + 0x0D) & ~0xf000) | 0x1000) : (*(unkptr + 0x0D) & ~0xf000);
}

void func_ov046_0208368c(DebugLauncherState* state) {
    state->active = TRUE;
}

/*Non-matching: Not even going to match this. Why is r1 unused? What *is* unkptr exactly? Why is this function only called
 * through a pointer?*/
void func_ov046_02083698(void* unkptr, s32 unused_r1, s32 r2) {
    void* temp = *(void**)(unkptr + 0x24);
    if (r2 == 1) {
        data_0206b408.unk_00 = 1;
        return;
    } else if (r2 != 2) {
        return;
    }
    if (*(s32*)(temp + 0x8) == 0x1000 && *(s32*)(temp + 0xC) == 0x1000) {
        *(u16*)(unkptr + 0xa) = *(u16*)(unkptr + 0xa) & ~0x1 | 1;
        *(u16*)(unkptr + 0xa) = *(u16*)(unkptr + 0xa) & ~0x2;
        u8 thing = (*(u8*)(*(u16*)(temp + 0x0))) * 0x108 + data_020676ec; // Nonmatching: 0x108 is a global constant?
        func_02002bc4(thing, 0, *(u32*)(temp + 0x8), *(u32*)(temp + 0xC), 0);
        *(u16*)(unkptr + 0xa) = (*(u16*)(unkptr + 0xa) & ~0x3E0) | ((u16)(thing << 0x1B) >> 0x16);
    } else {
        *(u16*)(unkptr + 0xa) = *(u16*)(unkptr + 0xa) & ~0x1;
        *(u16*)(unkptr + 0xa) = *(u16*)(unkptr + 0xa) & ~0x2;
        *(u16*)(unkptr + 0xa) = *(u16*)(unkptr + 0xa) & ~0x3E0;
    }
    data_0206b408.unk_04 = 0;
    data_0206b408.unk_08 = 0;
    data_0206b408.unk_0C = 0;
    data_0206b408.unk_10 = -1;
    if (*(s32*)(unkptr + 0x18) != 0 && *(s32*)(unkptr + 0x1C) != 0 && *(s16*)(unkptr + 0x16) >= 0) {
        data_0206b408.unk_04 = *(u16*)(*(s32*)(unkptr + 0x1C) + (((*(s16*)(unkptr + 0x16) * 2) + 1) * 2));
        data_0206b408.unk_08 = *(s32*)(unkptr + 0x1C) + (*(u16*)(*(s32*)(unkptr + 0x1C) + (*(s16*)(unkptr + 0x16) * 8)) * 2);
    }
    if (*(s32*)(temp + 0x8) != 0 && *(s32*)(temp + 0xC) == 0) { // Nonmatching: compiler reverses the condition
        data_0206b408.unk_08 = 0;                               // nonmatching: data_0206b408 is referenced from r0?
    }
}

s32 func_ov046_020837f8(s32* r0, s32* r1) {
    r0[0x0]                              = 0;
    r0[0x1]                              = 0;
    r0[0x2]                              = 0;
    r0[0x3]                              = 0;
    data_ov046_02084804                  = data_ov046_02084804 & ~0x3c | ((u32)((u16)(r1[0]) << 0x1c) >> 0x1a);
    *(u16*)(&data_ov046_02084804 + 0x2)  = r1[2] + 0x10;
    *(u16*)(&data_ov046_02084804 + 0x3)  = r1[3] + 0x10;
    *(u32*)(&data_ov046_02084804 + 0x08) = r0;
    *(u16*)(&data_ov046_02084804 + 0x0D) = 2;
    if (r1[1] < 0x0A) {
        *(u16*)(&data_ov046_02084804 + 0x15) = r1[1] + 1;
        *(u16*)(&data_ov046_02084804 + 0x0E) = 4;
        *(u16*)(&data_ov046_02084804 + 0x13) = 5;
        *(u16*)(&data_ov046_02084804 + 0x14) = 6;
    } else if (r1[1] < 0x25) {
        *(u16*)(&data_ov046_02084804 + 0x15) = r1[1] - 0xA;
        *(u16*)(&data_ov046_02084804 + 0x0E) = 1;
        *(u16*)(&data_ov046_02084804 + 0x13) = 2;
        *(u16*)(&data_ov046_02084804 + 0x14) = 3;
    } else {
        *(u16*)(&data_ov046_02084804 + 0x15) = r1[1] - 0x25;
        *(u16*)(&data_ov046_02084804 + 0x0E) = 7;
        *(u16*)(&data_ov046_02084804 + 0x13) = 8;
        *(u16*)(&data_ov046_02084804 + 0x14) = 9;
    }
    func_0200ecdc(r0 + 0x04, &data_ov046_02084804);
    return 1;
}

BOOL func_ov046_020838ec(void* unkptr) {
    BOOL result = TRUE;
    switch (*(BOOL*)unkptr) {
        case FALSE:
            if (*(s32*)(unkptr + 0x4) < 8) {
                *(s32*)(unkptr + 0x4) += 1;
            } else {
                if (*(s32*)(unkptr + 0x8) < 0x1000) {
                    *(s32*)(unkptr + 0x8) += 0x200;
                    *(s32*)(unkptr + 0xc) += 0x200;
                }
            }
            break;

        case TRUE:
            if (*(s32*)(unkptr + 0x8) <= 0) {
                result = FALSE;
            } else {
                *(s32*)(unkptr + 0x8) -= 0x200;
                *(s32*)(unkptr + 0xc) -= 0x200;
            }
            break;

        default:
            break;
    }
    if (result == TRUE) {
        func_0200ece8(unkptr + 0x10);
    }
    return result;
}

void func_ov046_02083980(s32 r0) {
    func_0200ed10(r0 + 0x10);
}
void func_ov046_02083990(s32 r0) {
    func_0200ed24(r0 + 0x10);
}

int func_ov046_020839a0(s32 r0, DebugLauncherState* state, s32* r2) {
    return func_ov046_020837f8(state->unk_18, r2);
}

int func_ov046_020839b4(s32 r0, DebugLauncherState* state, s32* r2) {
    return func_ov046_020838ec(state->unk_18);
}

int func_ov046_020839c4(s32 r0, DebugLauncherState* state, s32* r2) {
    func_ov046_02083980(state->unk_18);
    return 1;
}

int func_ov046_020839d8(s32 r0, DebugLauncherState* state, s32* r2) {
    func_ov046_02083990(state->unk_18);
    return 1;
}

int func_ov046_020839ec(s32 r0, DebugLauncherState* state, s32* r2, s32 functionPointerIndex) {
    DebugLauncherFuncStruct exportedFunctionTable = data_ov046_02083f7c;
    return exportedFunctionTable.entries[functionPointerIndex](r0, state, r2);
}
