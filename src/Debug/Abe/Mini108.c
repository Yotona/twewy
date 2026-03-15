#include "Debug/Mini108.h"
#include "Display.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "common_data.h"
#include "registers.h"
#include <NitroSDK/gx.h>

void Mini108_VBlank(void);
void Mini108_UnregisterVBlank(void);
void Mini108_RegisterVBlank(void);
void func_ov000_02082854(Mini108State* state);

BinIdentifier data_ov000_020831c0 = {0, "Apl_Abe/Grp_Mini108.bin"};

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_Mini108_tuple;

const UnkStruct_Mini108_tuple data_ov000_020831a8 = {0, 0x1000, 0};
const UnkStruct_Mini108_tuple data_ov000_020831b4 = {0, 0, -0x1000};

void func_ov000_020830bc() {
    s32 val1 = g_DisplaySettings.controls[DISPLAY_MAIN].brightness;

    if (val1 > 0) {
        val1 = val1 - 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.controls[DISPLAY_MAIN].brightness = val1;
    } else if (val1 < 0) {
        val1 = val1 + 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.controls[DISPLAY_MAIN].brightness = val1;
    }

    if (g_DisplaySettings.controls[DISPLAY_SUB].brightness > 0) {
        val1 = g_DisplaySettings.controls[DISPLAY_SUB].brightness - 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.controls[DISPLAY_SUB].brightness = val1;
    } else if (g_DisplaySettings.controls[DISPLAY_SUB].brightness < 0) {
        val1 = g_DisplaySettings.controls[DISPLAY_SUB].brightness + 1;
        if (val1 > 16)
            val1 = 16;
        else if (val1 < -16)
            val1 = -16;
        g_DisplaySettings.controls[DISPLAY_SUB].brightness = val1;
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
    Mini108_RegisterVBlank();

    Display_SetMainBrightness(-16);
    Display_SetSubBrightness(-16);

    g_DisplaySettings.unk_000 = 1;
    REG_POWER_CNT |= 0x8000;

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 2, 1, 0x2008);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x2004);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 1, 1, 0x104);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].layers = LAYER_BG0 | LAYER_BG2 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode  = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 2, 0, 0x8);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 1, 1, 0x104);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 1, 0x4);

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].layers = LAYER_BG0 | LAYER_BG2 | LAYER_OBJ;

    TouchInput_Init();

    state->unk_15e50 = ResourceMgr_ReinitManagers(&state->unk_48D0);
    state->unk_84C   = DatMgr_LoadRawData(2, 0, 0, &data_ov000_020831c0);
    data_02066aec    = 0x1f;

    u8* temp_r1 = Data_GetPackEntryData(state->unk_84C, 1);
    u8* temp_r6 = Data_GetPackEntryData(state->unk_84C, 3);
    u8* temp_r5 = Data_GetPackEntryData(state->unk_84C, 4);

    u32 r2_val = (*(u32*)temp_r1 & ~0xFF) >> 8;
    if (((*(u8*)temp_r1) & 0xF0) == 0) {
        r2_val -= 4;
    }
    BgResMgr_AllocChar32(g_BgResourceManagers[0], temp_r1, g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, r2_val);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], temp_r6, g_DisplaySettings.engineState[0].bgSettings[2].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText);
    PaletteMgr_AllocPalette(g_PaletteManagers[0], temp_r5, 0, 0, 2);
    data_02066eec = 0x1F;

    temp_r1 = Data_GetPackEntryData(state->unk_84C, 1);
    temp_r6 = Data_GetPackEntryData(state->unk_84C, 2);
    temp_r5 = Data_GetPackEntryData(state->unk_84C, 4);

    r2_val = (*(u32*)temp_r1 & ~0xFF) >> 8;
    if (((*(u8*)temp_r1) & 0xF0) == 0) {
        r2_val -= 4;
    }
    BgResMgr_AllocChar32(g_BgResourceManagers[1], temp_r1, g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, r2_val);
    BgResMgr_AllocScreen(g_BgResourceManagers[1], temp_r6, g_DisplaySettings.engineState[1].bgSettings[2].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText);
    PaletteMgr_AllocPalette(g_PaletteManagers[1], temp_r5, 0, 0, 2);

    func_0200eed4(&state->unk_850, &state->unk_8D0, 0x4000, 0x100);
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
    if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_L) != 0) {
        state->unk_0C = 1;
    }
}

void func_ov000_02082b18(Mini108State* unk_r0) {
    return;
}

void Mini108_Exit(Mini108State* state) {
    if (state != NULL) {
        DatMgr_ClearSlot(2);
        func_0200ef80(&state->unk_850);
        func_02025e30(&state->unk_14);
        func_02025e30(&state->unk_430);
        Mini108_UnregisterVBlank();
        ResourceMgr_SetActiveManagers(state->unk_15e50);
        Mem_Free(&gDebugHeap, state);
        MainOvlDisp_SetCbArg(0);
    }
}

// Nonmatching: Opcode order swap, Stack difference
void Mini108_Update(Mini108State* stateptr) {
    stateptr->buttonState = InputStatus.buttonState;

    TouchInput_Update();

    Text_RenderToScreen(stateptr->unk_80, 0, 0xa, func_02006930("Score : %6d", stateptr->unk_40));
    OamMgr_Reset(&data_020676ec, 0, 0);
    OamMgr_Reset(&data_02068778, 0, 0);
    OamMgr_ResetCommandQueues(&data_020676ec);
    OamMgr_ResetCommandQueues(&data_02068778);
    DebugOvlDisp_Run();

    if ((stateptr->buttonState.pressedButtons & INPUT_BUTTON_START) != 0) {
        if (func_02011f44("Seq_Mini108(void *) reset") == 1) {
            data_02066a58 |= 2;
        }
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_0_ID, &func_ov000_02082854, NULL, 0);
        return;
    } else if ((stateptr->buttonState.pressedButtons & INPUT_BUTTON_SELECT) != 0) {
        stateptr->unk_00 = 1;
    }

    if (stateptr->unk_00 == 1) {
        if (func_02011f44("Seq_Mini108(void *) leave") == 1) {
            data_02066a58 |= 2;
        }
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
        return;
    }
    func_0200efdc(&stateptr->unk_84C); // should have ->slotIndex(?) but throws "Illegal operand" error if it has that.

    OamMgr_FlushCommands(&data_020676ec);
    OamMgr_FlushCommands(&data_02068778);
    PaletteMgr_Flush(g_PaletteManagers[0], 0);
    PaletteMgr_Flush(g_PaletteManagers[1], 0);
}

const char* data_ov000_020831e0 = "Seq_Mini108(void *)";

void Mini108_Init(Mini108State* stateptr) {
    if (func_02011f44("Seq_Mini108(void *) enter") == 1) {
        data_02066a58 |= 2;
    }
    if (stateptr == NULL) {
        const char* sequence = data_ov000_020831e0;
        stateptr             = Mem_AllocHeapTail(&gDebugHeap, 0x15e54);
        Mem_SetSequence(&gDebugHeap, stateptr, sequence);
        MainOvlDisp_SetCbArg(stateptr);
    }
    MI_CpuFill(0, stateptr, Mem_GetBlockSize(&gDebugHeap, stateptr));
    func_ov000_02082b1c(stateptr);
    func_ov000_02082b18(stateptr);
    MainOvlDisp_NextProcessStage();
    MainOvlDisp_Run();
}

static const OverlayProcess OvlProc_Mini108 = {
    .init = Mini108_Init,
    .main = Mini108_Update,
    .exit = Mini108_Exit,
};

void func_ov000_02082854(Mini108State* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Mini108_Exit(state);
    } else {
        OvlProc_Mini108.funcs[stage](state);
    }
}

void func_ov000_020825c0(void) {
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

    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3002;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x08;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x10;
    G3X_SetClearColor(0, 0, 0x7fff, 0x3f, 0);
    GFX_FIFO_VIEWPORT     = 0xbfff0000;
    GFX_FIFO_SWAP_BUFFERS = 0;
    G3i_OrthoW(0, 0xc0000, 0, 0x100000, -0x400000, 0x400000, 0x400000, 1, 0);

    UnkStruct_Mini108_tuple unk_14 = {0, 0, 0};
    UnkStruct_Mini108_tuple unk_20 = data_ov000_020831a8;
    UnkStruct_Mini108_tuple unk_2C = data_ov000_020831b4;
    G3i_LookAt(&unk_14, &unk_20, &unk_2C, 1, 0);

    GFX_FIFO_MATRIX_STORE = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_64K;

    OamMgr_Init(0, 0); // Might also use &g_DisplaySettings as third parameter?
    OamMgr_Init(0, 1);
    MI_CpuFill(0, 0x6800000, 0xa4000);
    OamMgr_Reset(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    OamMgr_Reset(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);

    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    Color_CopyRange(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

void Mini108_RegisterVBlank(void) {
    func_ov000_020825c0();
    Interrupts_RegisterVBlankCallback(Mini108_VBlank, TRUE);
}

void Mini108_UnregisterVBlank(void) {
    func_ov000_020825c0();
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void Mini108_VBlank(void) {
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
