#include "Debug/Takami/TakTest.h"
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
#include <NitroSDK/mi/cpumem.h>

void TakTest_RegisterVBlank(void);
s32  TakTest_BG_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32  TakTest_OBJ_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
s32  TakTest_OBJ_CreateTask(TaskPool* pool, s32 arg1);

const char*           data_ov043_020cb580 = "Seq_TakTest()";
const TaskHandle      Tsk_TakTest_BG      = {"Tsk_TakTest_BG", TakTest_BG_RunTask, 0x90};
const TaskHandle      Tsk_TakTest_OBJ     = {"Tsk_TakTest_OBJ", TakTest_OBJ_RunTask, 0x90};
const BinIdentifier   data_ov043_020c787c = {43, "Apl_Tak/Grp_MenuBadge_BGD00.bin"};
const SpriteAnimation data_ov043_020c78dc = {
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
    .unk_08     = NULL,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c787c,
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
    Mem_InitializeHeap(&state->memPool, state->memPoolBuffer, sizeof(state->memPoolBuffer));
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 0x80, NULL, NULL);
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
    g_DisplaySettings.controls[0].layers      = LAYER_BG0 | LAYER_BG1 | LAYER_OBJ;
    g_DisplaySettings.controls[0].brightness  = 16;

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
    g_DisplaySettings.controls[1].layers      = LAYER_BG0 | LAYER_BG1 | LAYER_OBJ;
    g_DisplaySettings.controls[1].brightness  = 16;
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

s32 TakTest_BG_Init(TaskPool* pool, Task* task, s32 arg2) {
    // Not yet implemented
}

s32 TakTest_BG_Update(TaskPool* pool, Task* task, s32 arg2) {
    // Not yet implemented
}

s32 TakTest_BG_Render(TaskPool* pool, Task* task, s32 arg2) {
    return 1;
}

s32 TakTest_BG_Destroy(TaskPool* pool, Task* task, s32 arg2) {
    TakTest_BG* bg = task->data;
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], bg->resChar);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], bg->resScreen);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], bg->resPal);
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    return 1;
}

s32 TakTest_BG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = TakTest_BG_Init,
        .update     = TakTest_BG_Update,
        .render     = TakTest_BG_Render,
        .cleanup    = TakTest_BG_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void* TakTest_OBJ_GetFrameInfo(void* arg0, s32 arg2) {
    // Not yet implemented
}

void TakTest_OBJ_Load(TakTest_OBJ* obj, TakTest_OBJ_Args* args) {
    SpriteAnimation anim = data_ov043_020c78dc;
    anim.dataType        = args->dataType;
    _Sprite_Load(&obj->sprite, &anim);
}

s32 TakTest_OBJ_Init(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ*      obj     = task->data;
    TakTest_OBJ_Args* objArgs = args;

    TakTest_OBJ_Load(obj, objArgs);
    return 1;
}

s32 TakTest_OBJ_Update(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ* obj = task->data;

    Sprite_Update(&obj->sprite);
    return 1;
}

s32 TakTest_OBJ_Render(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ* obj = task->data;

    Sprite_RenderFrame(&obj->sprite);
    return 1;
}

s32 TakTest_OBJ_Destroy(TaskPool* pool, Task* task, void* args) {
    TakTest_OBJ* obj = task->data;

    Sprite_Release(&obj->sprite);
    return 1;
}

s32 TakTest_OBJ_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = TakTest_OBJ_Init,
        .update     = TakTest_OBJ_Update,
        .render     = TakTest_OBJ_Render,
        .cleanup    = TakTest_OBJ_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TakTest_OBJ_CreateTask(TaskPool* pool, s32 arg1) {
    s32 sp8 = arg1;
    EasyTask_CreateTask(pool, &Tsk_TakTest_OBJ, 0, 0, 0, &sp8);
}
