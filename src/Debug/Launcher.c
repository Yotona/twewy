#include "Debug/Launcher.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Text.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <NitroSDK/fx.h>
#include <NitroSDK/mi/cpumem.h>

SpriteFrameInfo* func_ov046_02083698(Sprite* sprite, s32 arg1, s32 arg2);
static s32       LauncherIcon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const char* Sequence = "Seq_Launcher(void *)";

const BinIdentifier data_ov046_02083a4c = {46, "Apl_Usr/Grp_Launcher.bin"};

const Vec data_ov046_02083a34 = {0, 0x1000, 0};
const Vec data_ov046_02083a40 = {0, 0, 0xFFFFF000};

SpriteAnimation data_ov046_02084804 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0,
    .unk_06     = 0,
    .unk_08     = func_ov046_02083698,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov046_02083a4c,
    .unk_18     = 2,
    .packIndex  = 2,
    .unk_1C     = 0,
    .unk_1E     = 0,
    .unk_20     = 10,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 0,
    .unk_28     = 0,
    .unk_2A     = 1,
};

const TaskHandle Tsk_LauncherIcon = {"LauncherIcon", LauncherIcon_RunTask, sizeof(LauncherIcon)};

// Nonmatching: Major differences in REG_* assignments and G3i_* calls
void Launcher_ResetDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
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
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x2;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x8;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x10;
    G3X_SetClearColor(0, 0, 0x7fff, 0x3f, 0);
    GFX_FIFO_VIEWPORT     = 0xbfff0000;
    GFX_FIFO_SWAP_BUFFERS = 0;

    G3i_OrthoW(0, 0xc0000, 0, 0x100000, -0x400000, 0x400000, 0x400000, 1, 0);
    Vec unk_14 = {0, 0, 0};
    Vec unk_20 = data_ov046_02083a34;
    Vec unk_2C = data_ov046_02083a40;
    G3i_LookAt(&unk_14, &unk_20, &unk_2C, 1, 0);
    GFX_FIFO_MATRIX_STORE = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_64K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;

    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_InitEngine(0, DISPLAY_SUB);
    MI_CpuFill(0, (void*)0x6800000, 0xa4000);
    OamMgr_ResetAndCommit();
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, TRUE);
    Color_CopyRange(&data_02066eec, &data_0205a128, 0, 0x200, TRUE);
}

void Launcher_VBlank(void) {
    if (SystemStatusFlags.vblank != FALSE) {
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

void Launcher_RegisterVBlank(void) {
    Launcher_ResetDisplay();
    Interrupts_RegisterVBlankCallback(Launcher_VBlank, TRUE);
}

void Launcher_UnregisterVBlank(void) {
    Launcher_ResetDisplay();
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov046_02082824(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void func_ov046_0208284c(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, -32, 0x800);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
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
    s32 stackframe[6];

    TouchCoord touch;

    TouchInput_GetCoord(&touch);

    if (TouchInput_WasTouchPressed() == TRUE) {
        if (touch.y < 64) {
            u32 selectedCell = (((touch.y - 64) / 32) * 8) + (touch.x / 32);
            if (selectedCell == 0xF) { // Why 0xF?
                if (state->selectedCategoryIndex >= 0 && state->selectedOptionIndex >= 0) {
                    DebugOvlDisp_Pop();
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
        }
    }

    if (TouchInput_IsTouchActive() != TRUE) {
        return;
    }

    if (touch.y < 64) {
        u32 selectedCell = (((touch.y - 64) / 32) * 8) + (touch.x / 32);
        if (selectedCell < state->unk_1C) {
            if (selectedCell != state->selectedCategoryIndex) {
                func_ov046_02083670(EasyTask_GetTaskData(&state->unk_11650, state->unk_24[state->selectedCategoryIndex]),
                                    FALSE);
            }
            func_ov046_02083670(EasyTask_GetTaskData(&state->unk_11650, state->unk_24[selectedCell]), TRUE);
            for (s32 i = 0; i < 24; i++) {
                if (state->unk_6C[i] != 0xFFFFFFFF) {
                    func_ov046_0208368c(EasyTask_GetTaskData(&state->unk_11650, state->unk_6C[i]));
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
                                state->unk_6C[i] = EasyTask_CreateTask(&state->unk_11650, &Tsk_LauncherIcon, stackframe[0],
                                                                       stackframe[0], stackframe[0], &stackframe[5]);
                                unkownIndex++;
                                catIndex++;
                            } while (((&Categories[catIndex])->options)->unk_00 > 0);
                        }
                        state->unk_64 = 0;
                    }
                    func_02010b84(&state->unk_156E8, 0x0, 0x80, 0x100, 0x0);
                    Text_RenderToScreen(&state->unk_156E8, 0x8, 0x80, Categories[selectedCell].creator);
                    Text_RenderToScreen(&state->unk_156E8, 0x8, 0x88, Categories[selectedCell].description);
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
        func_ov046_02083670(EasyTask_GetTaskData(&state->unk_11650, state->unk_6C[state->selectedOptionIndex]), FALSE);
    }
    func_ov046_02083670(EasyTask_GetTaskData(&state->unk_11650, state->unk_6C[selectedCell]), TRUE);
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
                ResourceMgr_SetActiveManagers(list->data);
                return;
            case 1:
                DatMgr_ClearSlot(list->data);
                return;
            case 2:
                DatMgr_ReleaseData(list->data);
                return;
            case 3:
                EasyTask_DestroyPool(list->data);
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
    Launcher_RegisterVBlank();
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = -16;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = -16;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x2004);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 4, 1, 0x2110);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].layers = LAYER_BG0 | LAYER_BG1 | LAYER_OBJ;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 4, 0, 0x110);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].layers = LAYER_BG0 | LAYER_BG1 | LAYER_BG2;

    TouchInput_Init();
    EasyList_Init(&state->unk_list_15F14, NULL, 32, func_ov046_02082c0c);
    state->unk_1164C = ResourceMgr_ReinitManagers(&state->unk_CC);
    EasyList_InsertSorted(&state->unk_list_15F14, 0, ResourceMgr_ReinitManagers(&state->unk_CC));
    state->unk_14 = DatMgr_AllocateSlot();
    // stackframe[0] = 0x1;
    // stackframe[1] = 0x0;
    state->unk_18 = DatMgr_LoadPackEntry(state->unk_14, 0, 0, &data_ov046_02083a4c, 0, 0);
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

/*Not implemented: Unclear stack frame */
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
    EasyTask_CreateTask(&state->unk_11650, &Task_EasyFade, 0, 0, 0, 0);
    // index = 0;
    // stack->0xC = Categories[index](.unk_00?);
    // stack->0x10 = (index << 0x1D) >> 0x18; // what? is this just "index % 8"?
    // stack->0x14 = (index / 8) * 32;
    // EasyTask_CreateTask(&state->unk_11650, &Tsk_LauncherIcon, index, index, index, &stack->0x8);
    // index++
    // state->unk_24[index] = result of last func-call
    //  Loop back if(Categories[index](.unk_00?) > 0)
    //  Else state->unk_1C = index;

    // stack->0x14 = 0x60
    // stack->0xC = 0x26
    // stack->0x10 = 0xe0
    // state->unk_60 = EasyTask_CreateTask(&state->unk_11650, &Tsk_LauncherIcon, 0, 0, 0, &stack->0x8);
}

/*Nonmatching: The start of the loop is different in the assembly than in the code.*/
void Launcher_Destroy(DebugLauncherState* state) {
    BOOL finishedCleaning = FALSE;
    if (state != NULL) {
        while (finishedCleaning == FALSE) {
            if (state->unk_list_15F14.tailSentinel.prev != NULL) {
                EasyList_RemoveNode(&state->unk_list_15F14, state->unk_list_15F14.tailSentinel.prev);
            }
            finishedCleaning = FALSE;
            if (state->unk_list_15F14.headSentinel.next == NULL) {
                finishedCleaning = TRUE;
            }
        }
        Launcher_UnregisterVBlank();
        Mem_Free(&gDebugHeap, state);
        MainOvlDisp_SetCbArg(NULL);
    }
}

void Launcher_Update(DebugLauncherState* state) {
    state->buttonState = InputStatus.buttonState;

    TouchInput_Update();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    DebugOvlDisp_Run();

    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->active = TRUE;
    }

    if (state->active == TRUE) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, state->overlay, state->overlayCB, NULL, 0);
    } else {
        EasyTask_UpdatePool(&state->unk_11650);
        OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
        OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
    }
}

void Launcher_Init(DebugLauncherState* state) {
    if (state == NULL) {
        const char* seq = Sequence;
        state           = Mem_AllocHeapTail(&gDebugHeap, sizeof(DebugLauncherState));
        Mem_SetSequence(&gDebugHeap, state, seq);
        MainOvlDisp_SetCbArg(state);
    }

    MI_CpuFill(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    func_ov046_02082c78(state);
    func_ov046_02083368(state);
    MainOvlDisp_NextProcessStage();
}

static const OverlayProcess OvlProc_DebugLauncher = {
    .init = Launcher_Init,
    .main = Launcher_Update,
    .exit = Launcher_Destroy,
};

// Overlay Init function
void func_ov046_02083630(DebugLauncherState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Launcher_Destroy(state);
    } else {
        OvlProc_DebugLauncher.funcs[stage](state);
    }
}

void func_ov046_02083670(u16* unkptr, BOOL param_2) {
    *(unkptr + 0x0D) = (param_2 == TRUE) ? ((*(unkptr + 0x0D) & ~0xf000) | 0x1000) : (*(unkptr + 0x0D) & ~0xf000);
}

void func_ov046_0208368c(DebugLauncherState* state) {
    state->active = TRUE;
}

// Non-matching: Significant differences
SpriteFrameInfo* func_ov046_02083698(Sprite* sprite, s32 arg1, s32 mode) {
    void* temp = sprite->unk24;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
        } break;

        case 2: {
            if (*(s32*)(temp + 0x8) == 0x1000 && *(s32*)(temp + 0xC) != 0x1000) {
                sprite->unk_0A.unk_00 = 1;
                sprite->unk_0A.unk_01 = 0;
                sprite->unk_0A.unk_05 =
                    OamMgr_AllocAffineGroup(&g_OamMgr[sprite->bits_0_1], 0, *(u32*)(temp + 0x8), *(u32*)(temp + 0xC), 0);
            } else {
                sprite->unk_0A.unk_00 = 0;
                sprite->unk_0A.unk_01 = 0;
                sprite->unk_0A.unk_05 = 0;
            }
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;
            data_0206b408.unk_0C = 0;
            data_0206b408.unk_10 = -1;
            if (sprite->animData != NULL && sprite->frameDataTable != 0 && sprite->unk16 >= 0) {
                data_0206b408.unk_04 = *(u16*)(sprite->frameDataTable + (((sprite->unk16 * 2) + 1) * 2));
                data_0206b408.unk_08 = sprite->frameDataTable + (*(u16*)(sprite->frameDataTable + (sprite->unk16 * 8)) * 2);
            }
            if (*(s32*)(temp + 0x8) != 0 && *(s32*)(temp + 0xC) == 0) {
                data_0206b408.unk_08 = 0;
            }
        } break;
    }
    return &data_0206b408;
}

s32 func_ov046_020837f8(LauncherIcon* icon, LauncherArgs* args) {
    icon->unk_00                  = 0;
    icon->unk_04                  = 0;
    icon->unk_08                  = 0;
    icon->unk_0C                  = 0;
    data_ov046_02084804.dataType  = args->dataType;
    data_ov046_02084804.unk_04    = args->unk_8 + 16;
    data_ov046_02084804.unk_06    = args->unk_C + 16;
    data_ov046_02084804.unk_10    = icon;
    data_ov046_02084804.packIndex = 2;
    if (args->unk_4 < 10) {
        data_ov046_02084804.unk_2A = args->unk_4 + 1;
        data_ov046_02084804.unk_1C = 4;
        data_ov046_02084804.unk_26 = 5;
        data_ov046_02084804.unk_28 = 6;
    } else if (args->unk_4 < 37) {
        data_ov046_02084804.unk_2A = args->unk_4 - 10;
        data_ov046_02084804.unk_1C = 1;
        data_ov046_02084804.unk_26 = 2;
        data_ov046_02084804.unk_28 = 3;
    } else {
        data_ov046_02084804.unk_2A = args->unk_4 - 37;
        data_ov046_02084804.unk_1C = 7;
        data_ov046_02084804.unk_26 = 8;
        data_ov046_02084804.unk_28 = 9;
    }
    _Sprite_Load(&icon->sprite, &data_ov046_02084804);
    return 1;
}

BOOL func_ov046_020838ec(LauncherIcon* icon) {
    BOOL result = TRUE;
    switch (icon->unk_00) {
        case 0:
            if (icon->unk_04 < 8) {
                icon->unk_04 += 1;
            } else {
                if (icon->unk_08 < 0x1000) {
                    icon->unk_08 += 0x200;
                    icon->unk_0C += 0x200;
                }
            }
            break;

        case 1:
            if (icon->unk_08 <= 0) {
                result = FALSE;
            } else {
                icon->unk_08 -= 0x200;
                icon->unk_0C -= 0x200;
            }
            break;
    }

    if (result == TRUE) {
        Sprite_UpdateAndCheck(&icon->sprite);
    }
    return result;
}

void func_ov046_02083980(LauncherIcon* icon) {
    Sprite_Render(&icon->sprite);
}

void func_ov046_02083990(LauncherIcon* icon) {
    Sprite_Destroy(&icon->sprite);
}

s32 LauncherIcon_Init(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    return func_ov046_020837f8(icon, args);
}

s32 LauncherIcon_Update(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    return func_ov046_020838ec(icon);
}

s32 LauncherIcon_Render(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    func_ov046_02083980(icon);
    return 1;
}

s32 LauncherIcon_Destroy(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    func_ov046_02083990(icon);
    return 1;
}

s32 LauncherIcon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = LauncherIcon_Init,
        .update     = LauncherIcon_Update,
        .render     = LauncherIcon_Render,
        .cleanup    = LauncherIcon_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
