#include "Interface/Debug/TakTest.h"
#include "Display.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/mi/cpumem.h>

void TakTest_RegisterVBlank(void);
s32  TakTest_BG_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32  TakTest_OBJ_CreateTask(TaskPool* pool, s32 arg1);

const char* data_ov043_020cb580 = "Seq_TakTest()";

void func_ov043_020824a0(TakTestState* state) {
    return;
}

void TakTest_UpdateTaskPool(TakTestState* state) {
    EasyTask_UpdatePool(&state->taskPool);
}

void TakTest_DestroyTaskPool(TakTestState* state) {
    EasyTask_DestroyPool(&state->taskPool);
}

void TakTest_Init(TakTestState* state) {
    if (state == NULL) {
        const char* sequence = data_ov043_020cb580;
        state                = Mem_AllocHeapTail(&gDebugHeap, 0x21A38);
        Mem_SetSequence(&gDebugHeap, state, sequence);
        MainOvlDisp_SetCbArg(state);
    }
    state->unk_11584 = DatMgr_AllocateSlot();
    state->unk_11588 = DatMgr_AllocateSlot();
    TakTest_RegisterVBlank();
    state->unk_11580 = ResourceMgr_ReinitManagers(&state->unk_00000);
    Mem_InitializeHeap(&state->heap, state->heapBuffer, sizeof(state->heapBuffer));
    EasyTask_InitializePool(&state->taskPool, &state->heap, 0x80, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    func_ov043_020824a0(state);

    EasyTask_CreateTask(&state->taskPool, &Tsk_TakTest_BG, NULL, 0, NULL, NULL);

    state->unk_21618 = TakTest_OBJ_CreateTask(&state->taskPool, state->unk_11588);
    MainOvlDisp_NextProcessStage();
}

void TakTest_Update(TakTestState* state) {
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    TakTest_UpdateTaskPool(state);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void TakTest_Destroy(TakTestState* state) {
    TakTest_DestroyTaskPool(state);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    DatMgr_ClearSlot(state->unk_11588);
    Mem_Free(&gDebugHeap, state);
}

static const OverlayProcess OvlProc_TakTest = {
    .init = TakTest_Init,
    .main = TakTest_Update,
    .exit = TakTest_Destroy,
};

void ProcessOverlay_TakTest(TakTestState* state) {
    s32 step = MainOvlDisp_GetProcessStage();
    if (step == PROCESS_STAGE_EXIT) {
        TakTest_Destroy(state);
    } else {
        OvlProc_TakTest.funcs[step](state);
    }
}

// Nonmatching: Some data access differences
void TakTest_InitSystems(void) {
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_AB);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_F);
    GX_SetBankForObj(GX_VRAM_E);
    GX_SetBankForBgExtPltt(GX_VRAM_NONE);
    GX_SetBankForObjExtPltt(GX_VRAM_NONE);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_D);
    GX_SetBankForSubBgExtPltt(GX_VRAM_H);
    GX_SetBankForSubObjExtPltt(GX_VRAM_I);
    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[0].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_OBJ);
    g_DisplaySettings.controls[0].brightness = 16;

    data_0206aa78                        = 0x300010;
    data_0206aa7c                        = 0x400040;
    g_DisplaySettings.controls[1].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    Display_SetSubLayers(LAYER_BG0 | LAYER_BG1 | LAYER_OBJ);
    g_DisplaySettings.controls[1].brightness = 16;
    OamMgr_Init3DSpritePipeline();
    OamMgr_InitExtended();
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
}

void TakTest_VBlank(void) {
    if (SystemStatusFlags.vblank) {
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

void TakTest_RegisterVBlank(void) {
    TakTest_InitSystems();
    Interrupts_RegisterVBlankCallback(TakTest_VBlank, 1);
}
