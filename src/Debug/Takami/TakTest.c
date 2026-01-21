#include "Debug/Takami/TakTest.h"
#include "BgResMgr.h"
#include "DatMgr.h"
#include "Display.h"
#include "Interrupts.h"
#include "OverlayDispatcher.h"
#include "SpriteMgr.h"
#include "System.h"
#include "common_data.h"

void func_ov043_02082af4(void);
s32  func_ov043_02082db4(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32  func_ov043_02082fa0(TaskPool* pool, s32 arg1);

void func_ov043_020824a0(TakTestState* state) {
    return;
}

void func_ov043_020824a4(TakTestState* state) {
    EasyTask_UpdatePool(&state->taskPool);
}

void func_ov043_020824b8(TakTestState* state) {
    EasyTask_DestroyPool(&state->taskPool);
}

const char*      data_ov043_020cb580 = "Seq_TakTest()";
const TaskHandle data_ov043_020c78a4 = {"Tsk_TakTest_BG", func_ov043_02082db4, 0x90};

void func_ov043_020824cc(TakTestState* state) {
    if (state == NULL) {
        const char* sequence = data_ov043_020cb580;
        state                = Mem_AllocHeapTail(&gDebugHeap, 0x21A38);
        Mem_SetSequence(&gDebugHeap, state, sequence);
        MainOvlDisp_SetCbArg(state);
    }
    state->unk_11584 = DatMgr_AllocateSlot();
    state->unk_11588 = DatMgr_AllocateSlot();
    func_ov043_02082af4();
    state->unk_11580 = func_0200cef0(state);
    Mem_InitializeHeap(&state->memPool, state->unk_11598, 0x10000);
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 0x80, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    func_ov043_020824a0(state);

    EasyTask_CreateTask(&state->taskPool, &data_ov043_020c78a4, 0, 0, 0, NULL);

    state->unk_21618 = func_ov043_02082fa0(&state->taskPool, state->unk_11588);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov043_020825ec(TakTestState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov043_020824a4(state);
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

void func_ov043_0208266c(TakTestState* state) {
    func_ov043_020824b8(state);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    DatMgr_ClearSlot(state->unk_11588);
    Mem_Free(&gDebugHeap, state);
}

typedef void (*TakTestCb)(TakTestState*);
static const TakTestCb data_ov043_020c7870[3] = {
    func_ov043_020824cc,
    func_ov043_020825ec,
    func_ov043_0208266c,
};

void func_ov043_020826ac(TakTestState* state) {
    s32 step = MainOvlDisp_GetRepeatCount();
    if (step == 0x7FFFFFFF) {
        func_ov043_0208266c(state);
    } else {
        data_ov043_020c7870[step](state);
    }
}

// Nonmatching: Some data access differences
void func_ov043_020826ec(void) {
    Interrupts_Init();
    func_0200434c();
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
    func_0203b2d0(0, 0x06800000, 0xA4000);
    func_0203b2d0(0, 0x06000000, 0x80000);
    func_0203b2d0(0, 0x06200000, 0x20000);
    func_0203b2d0(0, 0x06400000, 0x40000);
    func_0203b2d0(0, 0x06600000, 0x20000);
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    DisplayBGSettings* mainBg0 = Display_GetBG1Settings(DISPLAY_MAIN);
    mainBg0->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg0->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg0->colorMode         = GX_BG_COLORS_16;
    mainBg0->screenBase        = 0;
    mainBg0->charBase          = 1;
    mainBg0->extPlttSlot       = 0;
    REG_BG1CNT                 = (REG_BG1CNT & 0x43) | 4;

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

    DisplayBGSettings* subBg0 = Display_GetBG0Settings(DISPLAY_SUB);
    subBg0->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg0->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg0->colorMode         = GX_BG_COLORS_16;
    subBg0->screenBase        = 0;
    subBg0->charBase          = 1;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = (REG_BG0CNT_SUB & 0x43) | 4;

    DisplayBGSettings* subBg1 = Display_GetBG1Settings(DISPLAY_SUB);
    subBg1->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg1->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg1->colorMode         = GX_BG_COLORS_16;
    subBg1->screenBase        = 0;
    subBg1->charBase          = 1;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = (REG_BG1CNT_SUB & 0x43) | 4;

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
    func_02003ad0();
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_0200270c(0, 2);
    func_02002890(&data_02069804, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
}

void func_ov043_02082a24(void) {
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

void func_ov043_02082af4(void) {
    func_ov043_020826ec();
    Interrupts_RegisterVBlankCallback(func_ov043_02082a24, 1);
}

const BinIdentifier data_ov043_020c787c = {43, "Apl_Tak/Grp_MenuBadge_BGD00.bin"};

s32 func_ov043_02082b10(TaskPool* pool, Task* task, s32 arg2) {
    // Not yet implemented
}

s32 func_ov043_02082cb8(TaskPool* pool, Task* task, s32 arg2) {
    // Not yet implemented
}

s32 func_ov043_02082d54(TaskPool* pool, Task* task, s32 arg2) {
    return 1;
}

s32 func_ov043_02082d5c(TaskPool* pool, Task* task, s32 arg2) {
    TakTestUnknown* unk = task->data;
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], unk->resChar);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], unk->resScreen);
    func_0200afec(data_0206b3cc.unk_04, unk->unk_08);
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    return 1;
}

s32 func_ov043_02082db4(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    struct TakTestBG {
        s32 (*funcs[4])(TaskPool* pool, Task* task, s32 arg2);
    } takTestBg = {func_ov043_02082b10, func_ov043_02082cb8, func_ov043_02082d54, func_ov043_02082d5c};

    return takTestBg.funcs[arg3](pool, task, arg2);
}

void* func_ov043_02082dfc(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_02082e98(s32 arg0, s32* arg1) {
    // Not yet implemented
}

s32 func_ov043_02082f04(TaskPool* pool, Task* task, void* arg2) {
    Sprite* sprite = task->data;

    func_ov043_02082e98(sprite, arg2);
    return 1;
}

s32 func_ov043_02082f1c(TaskPool* pool, Task* task, void* arg2) {
    Sprite* sprite = task->data;

    Sprite_Update(sprite);
    return 1;
}

s32 func_ov043_02082f30(TaskPool* pool, Task* task, void* arg2) {
    Sprite* sprite = task->data;

    Sprite_RenderFrame(sprite);
    return 1;
}

s32 func_ov043_02082f44(TaskPool* pool, Task* task, void* arg2) {
    Sprite* sprite = task->data;

    Sprite_Release(sprite);
    return 1;
}

s32 func_ov043_02082f58(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    struct TakTestBG {
        s32 (*funcs[4])(TaskPool* pool, Task* task, void* arg2);
    } takTestObj = {func_ov043_02082f04, func_ov043_02082f1c, func_ov043_02082f30, func_ov043_02082f44};

    return takTestObj.funcs[arg3](pool, task, arg2);
}

const TaskHandle data_ov043_020c78b0 = {"Tsk_TakTest_OBJ", func_ov043_02082f58, 0x90};

s32 func_ov043_02082fa0(TaskPool* pool, s32 arg1) {
    s32 sp8 = arg1;
    EasyTask_CreateTask(pool, &data_ov043_020c78b0, 0, 0, 0, &sp8);
}
