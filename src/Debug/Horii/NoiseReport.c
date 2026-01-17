#include "Debug/NoiseReport.h"
#include "DatMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "OverlayManager.h"
#include "System.h"
#include "TouchInput.h"
#include "common_data.h"

s32 func_ov028_020eb9dc(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32 func_ov028_020ec4a8(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32 func_ov028_020ecd08(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32 func_ov028_020eb760(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32 func_ov028_020ea6ac(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32 func_ov028_020ea21c(TaskPool* pool, Task* task, void* arg2, s32 arg3);
s32 func_ov028_020e8d50(TaskPool* pool, Task* task, void* arg2, s32 arg3);

NoiseReport* data_ov028_020ed3e0 = NULL;

s16 data_ov028_020ed3a4[4] = {21, 95, 96, 0};

s32 data_ov028_020ed3ac;

extern void func_ov043_02084040(void* state);
extern void func_ov030_020ae92c();

void func_ov028_020e8878(NoiseReportState*);
void func_ov028_020e89d0(NoiseReportState*);
void func_ov028_020e8b68(NoiseReportState*);

const NoiseReportFunc data_ov028_020ecf24[3] = {
    func_ov028_020e8878,
    func_ov028_020e89d0,
    func_ov028_020e8b68,
};

s16 func_ov028_020e7360(s16) {
    // Not yet implemented
}

func_ov028_020e73a4() {
    // Not yet implemented
}

void func_ov028_020e776c(int* param_1, s32* param_2) {
    func_ov028_020e7798(param_1, param_2);
    func_ov028_020e7888(param_1, param_2);
    func_ov028_020e7970(param_1, param_2);
}

func_ov028_020e7798() {
    // Not yet implemented
}

func_ov028_020e7888() {
    // Not yet implemented
}

func_ov028_020e7970() {
    // Not yet implemented
}

func_ov028_020e7a50() {
    // Not yet implemented
}

func_ov028_020e7be8(void*) {
    // Not yet implemented
}

s32 func_ov028_020e7c80(s32 param_1) {
    switch (param_1) {
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            return 4;
        case 3:
            return 8;
        default:
            return 0;
    }
}

s32 func_ov028_020e7cc4(s32 param_1) {
    switch (param_1) {
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            return 4;
        case 3:
            return 8;
        default:
            return 0;
    }
}

func_ov028_020e7d08(TaskPool* pool, NoiseReport* arg1) {
    // Not yet implemented
}

func_ov028_020e7dbc(TaskPool* pool, NoiseReport* arg1) {
    // Not yet implemented
}

void func_ov028_020e7f00(TaskPool* pool, NoiseReport* arg1) {
    func_02027388(0);
    DatMgr_ClearSlot(arg1->unk_00);
    OvlMgr_UnloadOverlay(4);
    OvlMgr_UnloadOverlay(3);
}

func_ov028_020e7f2c() {
    // Not yet implemented
}

func_ov028_020e7fc8(u16*, s32, s32, s32, s32, s32, s32, s32) {
    // Not yet implemented
}

func_ov028_020e80a0(UnkStruct_0200e998*, s32, s32, s32, s32) {
    // Not yet implemented
}

BOOL func_ov028_020e8158(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s16 param_5, s16 param_6) {
    if ((((param_1 >= param_3) && (param_1 <= param_3 + param_5)) && (param_2 >= param_4)) && (param_2 <= param_4 + param_6)) {
        return TRUE;
    }
    return FALSE;
}

func_ov028_020e8194() {
    // Not yet implemented
}

func_ov028_020e81fc(UnkStruct_0200e998*, s16) {
    // Not yet implemented
}

void func_ov028_020e8220(void) {
    func_02023d1c(2);
}

func_ov028_020e8230(s16) {
    // Not yet implemented
}

func_ov028_020e8240(s16) {
    // Not yet implemented
}

func_ov028_020e8278() {
    // Not yet implemented
}

void func_ov028_020e82d0(NoiseReportState* state) {
    s32 idx = MainOvlDisp_GetRepeatCount();
    if (idx == 0x7fffffff) {
        func_ov028_020e8b68(state);
        return;
    }
    data_ov028_020ecf24[idx](state);
}

void func_ov028_020e8310(void) {
    if (SystemStatusFlags.vblank != FALSE) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 1024);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 1024);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 1024);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 1024);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
        func_02001b44(2, NULL, &data_020672ec, 0x400);
    }
}

// Nonmatching
void func_ov028_020e83f8(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    func_0202b878();
    DMA_Init(256);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1ff);
    GX_SetBankForTex(1);
    GX_SetBankForTexPltt(0x40);
    GX_SetBankForBg(2);
    GX_SetBankForObj(0x10);
    GX_SetBankForBgExtPltt(0);
    GX_SetBankForObjExtPltt(0);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    GX_SetBankForSubBgExtPltt(0);
    GX_SetBankForSubObjExtPltt(0);
    func_0203b2d0(0, 0x06800000, 0xa4000);
    func_0203b2d0(0, 0x6000000, 0x80000);
    func_0203b2d0(0, 0x06200000, 0x20000);
    func_0203b2d0(0, 0x06400000, 0x40000);
    func_0203b2d0(0, 0x06600000, 0x20000);
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    DisplayBGSettings* mainBg1 = &g_DisplaySettings.engineState[0].bgSettings[1];
    mainBg1->bgMode            = 0;
    mainBg1->screenSizeText    = 0;
    mainBg1->colorMode         = 0;
    mainBg1->screenBase        = 1;
    mainBg1->charBase          = 1;
    mainBg1->extPlttSlot       = 0;
    REG_BG1CNT                 = REG_BG1CNT & 0x43 | 4;

    DisplayBGSettings* mainBg2 = &g_DisplaySettings.engineState[0].bgSettings[2];
    mainBg2->bgMode            = 0;
    mainBg2->screenSizeText    = 0;
    mainBg2->colorMode         = 0;
    mainBg2->screenBase        = 1;
    mainBg2->charBase          = 3;
    mainBg2->extPlttSlot       = 1;
    REG_BG2CNT                 = REG_BG2CNT & 0x43 | 0x10c;

    DisplayBGSettings* mainBg3 = &g_DisplaySettings.engineState[0].bgSettings[3];
    mainBg3->bgMode            = 0;
    mainBg3->screenSizeText    = 0;
    mainBg3->colorMode         = 0;
    mainBg3->screenBase        = 2;
    mainBg3->charBase          = 5;
    mainBg3->extPlttSlot       = 1;
    REG_BG3CNT                 = REG_BG3CNT & 0x43 | 0x214;

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;

    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[DISPLAY_MAIN].layers     = LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 16;
    g_DisplaySettings.engineState[0].blendMode          = 1;
    g_DisplaySettings.engineState[0].blendLayer0        = 1;
    g_DisplaySettings.engineState[0].blendLayer1        = 62;
    g_DisplaySettings.engineState[0].blendCoeff0        = 6;
    g_DisplaySettings.engineState[0].blendCoeff1        = 10;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode      = GX_BGMODE_0;
    GXs_SetGraphicsMode(0);

    DisplayBGSettings* subBg1 = &g_DisplaySettings.engineState[1].bgSettings[1];
    subBg1->bgMode            = 0;
    subBg1->screenSizeText    = 0;
    subBg1->colorMode         = 0;
    subBg1->screenBase        = 1;
    subBg1->charBase          = 1;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = REG_BG1CNT_SUB & 0x43 | 0x104;

    DisplayBGSettings* subBg2 = &g_DisplaySettings.engineState[1].bgSettings[2];
    subBg2->bgMode            = 0;
    subBg2->screenSizeText    = 0;
    subBg2->colorMode         = 0;
    subBg2->screenBase        = 2;
    subBg2->charBase          = 3;
    subBg2->extPlttSlot       = 1;
    REG_BG2CNT_SUB            = REG_BG2CNT_SUB & 0x43 | 0x20c;

    DisplayBGSettings* subBg3 = &g_DisplaySettings.engineState[1].bgSettings[3];
    subBg3->bgMode            = 0;
    subBg3->screenSizeText    = 0;
    subBg3->colorMode         = 0;
    subBg3->screenBase        = 3;
    subBg3->charBase          = 5;
    subBg3->extPlttSlot       = 1;
    REG_BG3CNT_SUB            = REG_BG3CNT_SUB & 0x43 | 0x314;

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic   = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].layers      = LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 16;

    func_02003ad0();
    func_02003c68();
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 1024);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 1024);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_0200270c(0, 2);
    func_02002890(&data_02069804, 0);
    Interrupts_RegisterVBlankCallback(func_ov028_020e8310, TRUE);
    return;
}

void func_ov028_020e87f4(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov028_020e8808(void* unused) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void func_ov028_020e8830(void* state) {
    if (((NoiseReportState*)state)->unk_2164C != 0) {
        DebugOvlDisp_Pop();
    }
}

void func_ov028_020e8850(void* unused) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

const char* data_ov028_020ed3e4 = "Seq_Nrep()";

void func_ov028_020e8878(NoiseReportState* state) {
    if (state == NULL) {
        const char* sequence = data_ov028_020ed3e4;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(NoiseReportState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        MainOvlDisp_SetCbArg(state);
    }

    func_ov028_020e83f8();

    state->unk_11580 = func_0200cef0(state);
    TouchInput_Init();
    Mem_InitializeHeap(&state->memPool, &state->memBlock, 0x10000);
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 512, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, 0);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, 16, 0x1000);
    func_ov028_020e7d08(&state->taskPool, &state->unk_21610);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push(func_ov028_020e8850, state, 0);
    DebugOvlDisp_Push(func_ov028_020e8830, state, 0);
    DebugOvlDisp_Push(func_ov028_020e8808, state, 0);
    MainOvlDisp_IncrementRepeatCount();

    state->unk_2164C = 0;
}

void func_ov028_020e89d0(NoiseReportState* state) {
    TouchInput_Update();
    func_02003c4c();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02002890(&data_02069804, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    DebugOvlDisp_Run();

    if (state->unk_2164C == 0) {
        state->unk_2164C = func_ov028_020e7dbc(&state->taskPool, &state->unk_21610);
    }

    EasyTask_ProcessPendingTasks(&state->taskPool);
    EasyTask_UpdateActiveTasks(&state->taskPool);

    BOOL isAtBase = DebugOvlDisp_IsStackAtBase();

    func_02003c68();
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
    func_0200bf60(data_0206b3cc.unk_08, 0);

    if (isAtBase) {
        switch (state->unk_2164C) {
            case 1: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_43_ID, func_ov043_02084040, NULL, 0);
            } break;

            case 2: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_30_ID, func_ov030_020ae92c, NULL, 0);
            } break;

            default: {
                OverlayTag tag;
                MainOvlDisp_ReplaceTop(&tag, &OVERLAY_43_ID, func_ov043_02084040, NULL, 0);
            } break;
        }
    }
}

void func_ov028_020e8b68(NoiseReportState* state) {
    func_ov028_020e7f00(&state->taskPool, &state->unk_21610);
    EasyTask_DestroyPool(&state->taskPool);
    func_0200cef0(NULL);
    func_ov028_020e87f4();
    Mem_Free(&gDebugHeap, state);
}

void func_ov028_020e8bb0(void) {
    static const TaskHandle data_ov028_020ecf38 = {"Tsk_NrepBG", func_ov028_020e8d50, 208};

    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &data_ov028_020ecf38, NULL, 0, NULL, 0);
}

func_ov028_020e8bec(void*, s32) {
    // Not yet implemented
}

func_ov028_020e8c94(void*) {
    // Not yet implemented
}

func_ov028_020e8cfc(void*) {
    // Not yet implemented
}

s32 func_ov028_020e8d24(void* arg0) {
    for (s32 i = 0; i < 4; i++) {
        func_ov028_020e7be8(arg0);
        arg0 += 0x34;
    }
    return 1;
}

s32 func_ov028_020e8d50(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    void* data = task->data;
    switch (arg3) {
        case 0:
            return func_ov028_020e8bec(data, arg2);
        case 1:
            return func_ov028_020e8c94(data);
        case 2:
            return func_ov028_020e8cfc(data);
        case 3:
            return func_ov028_020e8d24(data);
        default:
            return 0;
    }
}

void func_ov028_020e8da0(void) {
    static const TaskHandle data_ov028_020ed41c = {"Tsk_Font", func_ov028_020ea21c, 3356};

    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &data_ov028_020ed41c, NULL, 0, NULL, 0);
}

func_ov028_020e8ddc() {
    // Not yet implemented
}

func_ov028_020e8e18() {
    // Not yet implemented
}

func_ov028_020e8f0c() {
    // Not yet implemented
}

func_ov028_020e8fe0() {
    // Not yet implemented
}

func_ov028_020e90b0() {
    // Not yet implemented
}

func_ov028_020e9160() {
    // Not yet implemented
}

func_ov028_020e91cc() {
    // Not yet implemented
}

func_ov028_020e9464() {
    // Not yet implemented
}

func_ov028_020e957c() {
    // Not yet implemented
}

func_ov028_020e9650() {
    // Not yet implemented
}

func_ov028_020e9704() {
    // Not yet implemented
}

func_ov028_020e9808() {
    // Not yet implemented
}

func_ov028_020e99a4() {
    // Not yet implemented
}

func_ov028_020e9a1c() {
    // Not yet implemented
}

func_ov028_020e9ac0() {
    // Not yet implemented
}

func_ov028_020e9b38(void*) {
    // Not yet implemented
}

func_ov028_020e9b64(void*) {
    // Not yet implemented
}

func_ov028_020e9bcc() {
    // Not yet implemented
}

func_ov028_020e9cd8(void*) {
    // Not yet implemented
}

func_ov028_020e9f44(void*) {
    // Not yet implemented
}

func_ov028_020ea058(void*, s32) {
    // Not yet implemented
}

s32 func_ov028_020ea15c(void* arg0) {

    if (func_ov028_020e9b38(arg0)) {
        func_ov028_020e9cd8(arg0);
        func_ov028_020e9f44(arg0);
    } else {
        if (func_ov028_020e9b64(arg0) != 0) {
            func_ov028_020e9cd8(arg0);
        }
    }
    return 1;
}

s32 func_ov028_020ea1a0(void*) {
    return 1;
}

func_ov028_020ea1a8(void*) {
    // Not yet implemented
}

s32 func_ov028_020ea21c(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    void* data = task->data;
    switch (arg3) {
        case 0:
            return func_ov028_020ea058(data, arg2);
        case 1:
            return func_ov028_020ea15c(data);
        case 2:
            return func_ov028_020ea1a0(data);
        case 3:
            return func_ov028_020ea1a8(data);
        default:
            return 0;
    }
}

void func_ov028_020ea26c(void) {
    static const TaskHandle data_ov028_020ed204 = {"Tsk_NrepIcon", func_ov028_020ea6ac, 132};

    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &data_ov028_020ed204, NULL, 0, NULL, 0);
}

func_ov028_020ea2a8() {
    // Not yet implemented
}

func_ov028_020ea350() {
    // Not yet implemented
}

func_ov028_020ea3ec() {
    // Not yet implemented
}

func_ov028_020ea49c() {
    // Not yet implemented
}

func_ov028_020ea4ec() {
    // Not yet implemented
}

func_ov028_020ea654() {
    // Not yet implemented
}

func_ov028_020ea680() {
    // Not yet implemented
}

s32 func_ov028_020ea6ac(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov028_020ea6f4(void) {
    static const TaskHandle data_ov028_020ed254 = {"Tsk_NrepInterD", func_ov028_020eb760, 856};

    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &data_ov028_020ed254, NULL, 0, NULL, 0);
}

func_ov028_020ea730() {
    // Not yet implemented
}

func_ov028_020ea888() {
    // Not yet implemented
}

func_ov028_020ea960() {
    // Not yet implemented
}

func_ov028_020eaa00() {
    // Not yet implemented
}

func_ov028_020eaa90() {
    // Not yet implemented
}

func_ov028_020eab04() {
    // Not yet implemented
}

func_ov028_020eaba4() {
    // Not yet implemented
}

func_ov028_020eacf8() {
    // Not yet implemented
}

func_ov028_020eb054() {
    // Not yet implemented
}

func_ov028_020eb0d4() {
    // Not yet implemented
}

func_ov028_020eb174(u32* param_1, s32 param_2, s32 param_3, s32 param_4) {
    // Not yet implemented
}

func_ov028_020eb3e8(u32* param_1) {
    // Not yet implemented
}

func_ov028_020eb654(u32* param_1) {
    // Not yet implemented
}

func_ov028_020eb718(u32* param_1) {
    // Not yet implemented
}

s32 func_ov028_020eb760(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    u32* puVar1 = task->data;
    switch (arg3) {
        case 0:
            return func_ov028_020eb174(puVar1, arg2, arg2, arg3);
        case 1:
            return func_ov028_020eb3e8(puVar1);
        case 2:
            return func_ov028_020eb654(puVar1);
        case 3:
            return func_ov028_020eb718(puVar1);
        default:
            return 0;
    }
}

void func_ov028_020eb7b0(void) {
    static const TaskHandle Tsk_NrepInterU = {"Tsk_NrepInterU", func_ov028_020eb9dc, 964};

    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &Tsk_NrepInterU, NULL, 0, NULL, 0);
}

func_ov028_020eb7ec(void* param_1, s32 param_2, s32 param_3, s32 param_4) {
    // Not yet implemented
}

func_ov028_020eb8ec(UnkStruct_0200e998* arg0) {
    for (s32 i = 0; i < 15; i++) {
        func_0200dd60(arg0);
        arg0++;
    }
    return 1;
}

func_ov028_020eb918(void* param_1) {
    // Not yet implemented
}

s32 func_ov028_020eb9b0(UnkStruct_0200e998* arg0) {
    for (s32 i = 0; i < 15; i++) {
        func_0200e998(arg0);
        arg0++;
    }
    return 1;
}

s32 func_ov028_020eb9dc(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    void* puVar1 = (void*)task->data;
    switch (arg3) {
        case 0:
            return func_ov028_020eb7ec(puVar1, arg2, arg2, arg3);
        case 1:
            return func_ov028_020eb8ec(puVar1);
        case 2:
            return func_ov028_020eb918(puVar1);
        case 3:
            return func_ov028_020eb9b0(puVar1);
        default:
            return 0;
    }
}

void func_ov028_020eba2c(s32 arg0) {
    s32                     stack[1];
    static const TaskHandle Tsk_NrepPreview = {"Tsk_NrepPreview", func_ov028_020ec4a8, 1992};

    func_0203b3c0(&stack, 0, 4);
    stack[0] = arg0;
    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &Tsk_NrepPreview, NULL, 0, NULL, &stack);
}

func_ov028_020eba84(NrepPreview* arg0, UnkStruct_0200e998* arg1, s32* arg2) {
    // Not yet implemented
}

void func_ov028_020ebe9c(NrepPreview* arg0) {
    UnkStruct_0200e998* puVar5 = &arg0->unk_084;
    s16                 iVar6  = data_ov028_020ed3e0->unk_16;
    s32                 iVar4  = 29;

    if ((iVar6 >= 0x60) || (arg0->unk_000 == 0)) {
        func_ov028_020e80a0(&arg0->unk_084, 13, 0, 5, 0);
        func_ov028_020e80a0(&arg0->unk_1C4, 13, 0, 4, 0);
        func_ov028_020e80a0(&arg0->unk_2C4, 13, 0, 3, 0);
        func_ov028_020e80a0(&arg0->unk_3C4, 13, 0, 3, 0);
        func_ov028_020e80a0(&arg0->unk_4C4, 13, 0, 3, 0);
        func_ov028_020e80a0(&arg0->unk_584, 13, 0, 5, 0);
        func_ov028_020e81fc(&arg0->unk_6C4, 0);
        func_ov028_020e81fc(&arg0->unk_704, 0);
        func_ov028_020e81fc(&arg0->unk_744, 0);
        func_ov028_020e81fc(&arg0->unk_784, 0);
    } else {
        s32 iVar1 = func_ov028_020e8230(iVar6);
        iVar6     = func_ov028_020e7360(iVar6);

        s32 iVar2 = 200 - *(s16*)(iVar6 + 0x10);
        s32 iVar7 = 200 - *(s16*)(iVar6 + 0x12);

        if (iVar2 > 200) {
            iVar2 = 200;
        } else if (iVar2 < 0) {
            iVar2 = 0;
        }

        if (iVar7 > 200) {
            iVar7 = 200;
        } else if (iVar7 < 0) {
            iVar7 = 0;
        }

        func_ov028_020e80a0(&arg0->unk_084, 13, (int)*(s16*)(iVar6 + 10), 5, 0);
        func_ov028_020e80a0(&arg0->unk_1C4, 13, (int)*(s16*)(iVar6 + 0xc), 4, 0);
        func_ov028_020e80a0(&arg0->unk_2C4, 13, iVar2, 3, 0);
        func_ov028_020e80a0(&arg0->unk_3C4, 13, (s16)iVar7, 3, 0);
        func_ov028_020e80a0(&arg0->unk_4C4, 13, (u32) * (u8*)(iVar6 + 7), 3, 0);
        func_ov028_020e80a0(&arg0->unk_584, 13, (u32) * (u16*)(iVar6 + 8), 5, 0);

        s16 sVar3 = ((*(s8*)(iVar1 + 1) & 8) != 0) ? 0x18 : 0;
        func_ov028_020e81fc(&arg0->unk_6C4, sVar3);

        sVar3 = ((*(s8*)(iVar1 + 1) & 4) != 0) ? 0x18 : 0;
        func_ov028_020e81fc(&arg0->unk_704, sVar3);

        sVar3 = ((*(s8*)(iVar1 + 1) & 2) != 0) ? 0x18 : 0;
        func_ov028_020e81fc(&arg0->unk_744, sVar3);

        sVar3 = ((*(s8*)(iVar1 + 1) & 1) != 0) ? 0x18 : 0;
        func_ov028_020e81fc(&arg0->unk_784, sVar3);
    }

    do {
        func_0200dd60(puVar5);
        iVar4--;
        puVar5++;
    } while (0 < iVar4);
}

void func_ov028_020ec16c(NrepPreview* arg0) {
    int                 iVar1;
    UnkStruct_0200e998* puVar2;

    puVar2 = &arg0->unk_084;
    iVar1  = 29;
    do {
        func_0200e2c4(puVar2);
        iVar1--;
        puVar2++;
    } while (0 < iVar1);
}

void func_ov028_020ec194(NrepPreview* arg0) {
    int                 iVar1;
    UnkStruct_0200e998* puVar2;

    puVar2 = &arg0->unk_084;
    iVar1  = 29;
    do {
        func_0200e998(puVar2);
        iVar1--;
        puVar2++;
    } while (0 < iVar1);
}

s32 func_ov028_020ec1bc(void) {
    s32 iVar1 = 0;
    while (TRUE) {
        if (data_ov028_020ed3a4[iVar1] == 96) {
            return 0;
        }
        if (data_ov028_020ed3a4[iVar1] == data_ov028_020ed3e0->unk_16) {
            return 1;
        }

        iVar1++;
    }
    return 1;
}

void func_ov028_020ec204(NrepPreview* arg0) {
    u16   auStack_38[22];
    short sStack_1e;

    if (arg0->unk_7C4 == data_ov028_020ed3e0->unk_16) {
        return;
    }

    arg0->unk_7C4 = data_ov028_020ed3e0->unk_16;
    if (arg0->unk_004.unk_28 != 0) {
        func_0200e998(&arg0->unk_004);
    }
    if (arg0->unk_044.unk_28 != 0) {
        func_0200e998(&arg0->unk_044);
    }
    func_ov028_020e7fc8(auStack_38, 1, *(int*)data_ov028_020ed3e0, &data_ov028_020ed3ac, 1, 2, 3, 4);
    sStack_1e = data_ov028_020ed3e0->unk_16 + 1;
    if (sStack_1e < 0x61) {
        func_0200ecdc(&arg0->unk_004, auStack_38);
        func_ov028_020e81fc(&arg0->unk_004, 1);
        arg0->unk_004.unk_0C = 0xbe;
        arg0->unk_004.unk_0E = 0x58;
        arg0->unk_004.unk_0A = arg0->unk_004.unk_0A & ~0xC00 | 0x800;
        if (func_ov028_020ec1bc()) {
            func_0200ecdc(&arg0->unk_044, auStack_38);
            func_ov028_020e81fc(&arg0->unk_044, 2);
            arg0->unk_044.unk_0C = 0xbe;
            arg0->unk_044.unk_0E = 0x58;
            arg0->unk_044.unk_0A = arg0->unk_044.unk_0A & ~0xC00 | 0x800;
        }
    }
}

s32 func_ov028_020ec358(NrepPreview* arg0, s32* arg1) {
    func_0203b3c0(arg0, 0, sizeof(NrepPreview));
    arg0->unk_7C4 = -1;
    func_ov028_020eba84(arg0, &arg0->unk_084, arg1);
    func_ov028_020ec204(arg0);
    return 1;
}

s32 func_ov028_020ec3a0(NrepPreview* arg0) {
    s16 uVar1 = data_ov028_020ed3e0->unk_16;
    func_ov028_020e8230(uVar1);
    arg0->unk_000 = func_ov028_020e8240(uVar1) == 0;
    func_ov028_020ec204(arg0);
    func_ov028_020ebe9c(arg0);
    if (arg0->unk_000 != 0) {
        if (arg0->unk_004.unk_28 != 0) {
            func_0200dd60(&arg0->unk_004);
        }
        if (arg0->unk_044.unk_28 != 0) {
            func_0200dd60(&arg0->unk_044);
        }
    }
    return 1;
}

s32 func_ov028_020ec424(NrepPreview* arg0) {
    func_ov028_020ec16c(arg0);
    if (arg0->unk_000 != 0) {
        if (arg0->unk_004.unk_28 != 0) {
            func_0200e2c4(&arg0->unk_004);
        }
        if (arg0->unk_044.unk_28 != 0) {
            func_0200e2c4(&arg0->unk_044);
        }
    }
    return 1;
}

s32 func_ov028_020ec46c(NrepPreview* arg0) {
    func_ov028_020ec194(arg0);
    if (arg0->unk_004.unk_28 != 0) {
        func_0200e998(&arg0->unk_004);
    }
    if (arg0->unk_044.unk_28 != 0) {
        func_0200e998(&arg0->unk_044);
    }
    return 1;
}

s32 func_ov028_020ec4a8(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    NrepPreview* puVar1 = (NrepPreview*)task->data;
    switch (arg3) {
        case 0:
            return func_ov028_020ec358(puVar1, arg2);
        case 1:
            return func_ov028_020ec3a0(puVar1);
        case 2:
            return func_ov028_020ec424(puVar1);
        case 3:
            return func_ov028_020ec46c(puVar1);
        default:
            return 0;
    }
}

void func_ov028_020ec4f8(s32 arg0) {
    s32                     stack[1];
    static const TaskHandle Tsk_NrepSeal = {"Tsk_NrepSeal", func_ov028_020ecd08, 0x3664};

    func_0203b3c0(&stack, 0, 4);
    stack[0] = arg0;
    EasyTask_CreateTask(data_ov028_020ed3e0->pool, &Tsk_NrepSeal, NULL, 0, NULL, &stack);
}

func_ov028_020ec550() {
    // Not yet implemented
}

func_ov028_020ec590(NrepSeal* arg0) {
    // Not yet implemented
}

func_ov028_020ec73c(NrepSeal* arg0, s32) {
    // Not yet implemented
}

s32 func_ov028_020eca54(NrepSeal* arg0) {
    func_ov028_020ec590(arg0);

    for (s32 i = 0; i < 24; i++) {
        s16   iVar3  = (data_ov028_020ed3e0->unk_14 * 8 + (s16)i);
        char* pcVar2 = (char*)func_ov028_020e8230(iVar3);

        if (func_ov028_020e8240(iVar3) != 0) {
            arg0->unk_000 = 0;
        }

        func_ov028_020e81fc(&arg0->unk_044, ((pcVar2[1] & 8) != 0) ? 0xC : 0);
        func_ov028_020e81fc(&arg0->unk_084, ((pcVar2[1] & 4) != 0) ? 0xC : 0);
        func_ov028_020e81fc(&arg0->unk_0C4, ((pcVar2[1] & 2) != 0) ? 0xC : 0);
        func_ov028_020e81fc(&arg0->unk_104, ((pcVar2[1] & 1) != 0) ? 0xC : 0);

        if (*pcVar2 == 0) {
            func_ov028_020e81fc(&arg0->unk_144, 0);
            func_ov028_020e81fc(&arg0->unk_184, 0);
            func_ov028_020e81fc(&arg0->unk_1C4, 0);
            func_ov028_020e81fc(&arg0->unk_204, 0);
        } else {
            func_ov028_020e81fc(&arg0->unk_144, 1);
            func_ov028_020e80a0(&arg0->unk_184, 2, *pcVar2, 3, 0);
        }

        func_0200dd60(&arg0->unk_004);
        func_0200dd60(&arg0->unk_044);
        func_0200dd60(&arg0->unk_084);
        func_0200dd60(&arg0->unk_0C4);
        func_0200dd60(&arg0->unk_104);
        func_0200dd60(&arg0->unk_144);
        func_0200dd60(&arg0->unk_184);
        func_0200dd60(&arg0->unk_1C4);
        func_0200dd60(&arg0->unk_204);
        arg0++;
    }

    return 1;
}

s32 func_ov028_020ecbf0(NrepSeal* arg0) {
    for (s32 i = 0; i < 24; i++) {
        if (arg0->unk_000 != 0) {
            data_0206a890.unk_0C = (data_ov028_020ed3e0->unk_0C != 0) ? 0xF : 0x1F;
            data_0206a890.unk_0C &= 0x1F;
            func_0200e2c4(&arg0->unk_004);
            func_0200e2c4(&arg0->unk_044);
            func_0200e2c4(&arg0->unk_084);
            func_0200e2c4(&arg0->unk_0C4);
            func_0200e2c4(&arg0->unk_104);
            func_0200e2c4(&arg0->unk_144);
            func_0200e2c4(&arg0->unk_184);
            func_0200e2c4(&arg0->unk_1C4);
            func_0200e2c4(&arg0->unk_204);
        }
        arg0++;
    }
    return 1;
}

s32 func_ov028_020ecc9c(NrepSeal* arg0) {
    for (s32 i = 0; i < 24; i++) {
        func_0200e998(&arg0->unk_004);
        func_0200e998(&arg0->unk_044);
        func_0200e998(&arg0->unk_084);
        func_0200e998(&arg0->unk_0C4);
        func_0200e998(&arg0->unk_104);
        func_0200e998(&arg0->unk_144);
        func_0200e998(&arg0->unk_184);
        func_0200e998(&arg0->unk_1C4);
        func_0200e998(&arg0->unk_204);
        arg0++;
    }
    return 1;
}

s32 func_ov028_020ecd08(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    NrepSeal* seal = (NrepSeal*)task->data;

    switch (arg3) {
        case 0:
            return func_ov028_020ec73c(seal, arg2);
        case 1:
            return func_ov028_020eca54(seal);
        case 2:
            return func_ov028_020ecbf0(seal);
        case 3:
            return func_ov028_020ecc9c(seal);
        default:
            return 0;
    }
}
