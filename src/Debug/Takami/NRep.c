#include "Debug/Takami/NRep.h"
#include "DMA.h"
#include "Display.h"
#include "EasyFade.h"
#include "Input.h"
#include "Interrupts.h"
#include "OverlayDispatcher.h"
#include "SpriteMgr.h"
#include "System.h"
#include "TouchInput.h"
#include "common_data.h"
#include <NitroSDK/fs/overlay.h>
#include <NitroSDK/os/cache.h>
#include <registers.h>
#include <types.h>

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
} NRepHitboxData;

extern void func_ov043_02084040(void*);

void func_ov043_020c6304(void);
void func_ov043_020c632c(void);
void func_ov043_020c6330(void);
void func_ov043_020c6358(NRepState* state);
void func_ov043_020c64a4(NRepState* state);
void func_ov043_020c65d4(NRepState* state);
void func_ov043_020c6a58(void);
void func_ov043_020c6b40(void);
void func_ov043_020c6b5c(void);
void func_ov043_020c6bf8(NRepContext* ctx);
void func_ov043_020c6bfc(NRepContext* ctx);
void func_ov043_020c6f9c(NRepContext* ctx);
void func_ov043_020c707c(NRepContext* ctx);
void func_ov043_020c7080(NRepContext* ctx);

static s32 NRepMenu_CreateTask(TaskPool* pool, s32 arg1, void* arg2);
static s32 NRepText_CreateTask(TaskPool* pool, s32 arg1, void* arg2);
static s32 NRepCursor_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, NRepContext* context);

NRepState* data_ov043_020cd294;

const BinIdentifier data_ov043_020cb470[5] = {
    {43,     "Apl_Tak/Grp_Menu_BGD.bin"},
    {43,   "Apl_Tak/Grp_Save_BGD00.bin"},
    {43, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    {43,   "Apl_Tak/Grp_Save_OBD00.bin"},
    {43,   "Apl_Tak/Grp_Save_BGU00.bin"},
};

const NRepHitboxData data_ov043_020cb4b0 = {23, 19, 205, 2, 230, 2};

void func_ov043_020c61e4(NRepState* state) {
    NRepContext* ctx = &state->context;

    state->unk_21680 = 0;
    state->unk_21684 = 0;
    state->unk_21686 = 0;
    state->unk_21688 = 0;

    ctx->unk_00 = 0;
    ctx->unk_02 = 0;
    ctx->unk_03 = 0;
    ctx->unk_04 = 0;
    ctx->unk_05 = 0;
    func_ov043_020c6bf8(ctx);
}

void func_ov043_020c6238(NRepState* state) {
    NRepContext* context = &state->context;

    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -16, 0x1000);
    state->taskId_Menu = NRepMenu_CreateTask(&state->taskPool, state->unk_11588, context);
    state->taskId_Text = NRepText_CreateTask(&state->taskPool, state->unk_11588, context);

    for (u16 index = 0; index < ARRAY_COUNT(state->taskIds_Cursor); index++) {
        state->taskIds_Cursor[index] = NRepCursor_CreateTask(&state->taskPool, state->unk_11588, index, context);
    }
}

void func_ov043_020c6304(void) {
    EasyFade_FadeBothDisplays(0, 0, 0x1000);
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov043_020c632c(void) {}

void func_ov043_020c6330(void) {
    EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
    if (EasyFade_IsFading() != 0) {
        return;
    }
    DebugOvlDisp_Pop();
}

const char* data_ov043_020cd164 = "Seq_NRep()";

void func_ov043_020c6358(NRepState* state) {
    if (state == NULL) {
        const char* sequence = data_ov043_020cd164;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(NRepState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        data_ov043_020cd294 = state;
        MainOvlDisp_SetCbArg(state);
    }

    state->unk_11584 = DatMgr_AllocateSlot();
    state->unk_11588 = DatMgr_AllocateSlot();
    func_ov043_020c6b40();
    state->unk_11580 = func_0200cef0(state);
    TouchInput_Init();
    Mem_InitializeHeap(&state->memPool, state->memPoolBuffer, sizeof(state->memPoolBuffer));
    FS_LoadOverlay(0, &OVERLAY_31_ID);
    EasyTask_InitializePool(&state->taskPool, &state->memPool, 0x200, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    func_ov043_020c61e4(state);
    func_ov043_020c6f9c(&state->context);
    func_ov043_020c6238(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov043_020c6330, state, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov043_020c632c, state, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov043_020c6304, state, 0);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov043_020c64a4(NRepState* state) {
    NRepContext* ctx = &state->context;

    TouchInput_Update();
    func_02003c4c();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02002890(&data_02069804, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov043_020c707c(ctx);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->taskPool);
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->unk_21680 = 1;
    }
    func_02003c68();
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
    func_0200bf60(data_0206b3cc[2], 0);

    if (state->unk_21680 != 0) {
        if (ctx->unk_03 == 0) {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, &OVERLAY_43_ID, func_ov043_02084040, NULL, 0);
        } else {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        }
    }
}

void func_ov043_020c65d4(NRepState* state) {
    NRepContext* ctx = &state->context;

    func_ov043_020c6bfc(ctx);
    func_ov043_020c7080(ctx);
    EasyTask_DestroyPool(&state->taskPool);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    DatMgr_ClearSlot(state->unk_11588);
    func_ov043_020c6b5c();
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
}

typedef void (*NRepFunc)(NRepState*);
const NRepFunc data_ov043_020cb464[3] = {func_ov043_020c6358, func_ov043_020c64a4, func_ov043_020c65d4};

void func_ov043_020c6644(NRepState* state) {
    s32 step = MainOvlDisp_GetRepeatCount();

    if (step == 0x7FFFFFFF) {
        func_ov043_020c65d4(state);
    } else {
        data_ov043_020cb464[step](state);
    }
}

void func_ov043_020c6684(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
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
    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    DisplayBGSettings* mainBg1 = Display_GetBG1Settings(DISPLAY_MAIN);
    mainBg1->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg1->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg1->colorMode         = GX_BG_COLORS_16;
    mainBg1->screenBase        = 0;
    mainBg1->charBase          = 1;
    mainBg1->extPlttSlot       = 0;
    REG_BG1CNT                 = (REG_BG1CNT & 0x43) | 4;

    DisplayBGSettings* mainBg2 = Display_GetBG2Settings(DISPLAY_MAIN);
    mainBg2->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg2->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg2->colorMode         = GX_BG_COLORS_16;
    mainBg2->screenBase        = 1;
    mainBg2->charBase          = 3;
    mainBg2->extPlttSlot       = 1;
    REG_BG2CNT                 = (REG_BG2CNT & 0x43) | 0x10C;

    DisplayBGSettings* mainBg3 = Display_GetBG3Settings(DISPLAY_MAIN);
    mainBg3->bgMode            = DISPLAY_BGMODE_TEXT;
    mainBg3->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    mainBg3->colorMode         = GX_BG_COLORS_16;
    mainBg3->screenBase        = 2;
    mainBg3->charBase          = 5;
    mainBg3->extPlttSlot       = 1;
    REG_BG3CNT                 = (REG_BG3CNT & 0x43) | 0x214;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers      = LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness  = 16;

    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(0);

    DisplayBGSettings* subBg1 = Display_GetBG1Settings(DISPLAY_SUB);
    subBg1->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg1->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg1->colorMode         = GX_BG_COLORS_16;
    subBg1->screenBase        = 1;
    subBg1->charBase          = 1;
    subBg1->extPlttSlot       = 0;
    REG_BG1CNT_SUB            = (REG_BG1CNT_SUB & 0x43) | 0x104;

    DisplayBGSettings* subBg2 = Display_GetBG2Settings(DISPLAY_SUB);
    subBg2->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg2->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg2->colorMode         = GX_BG_COLORS_16;
    subBg2->screenBase        = 2;
    subBg2->charBase          = 3;
    subBg2->extPlttSlot       = 1;
    REG_BG2CNT_SUB            = (REG_BG2CNT_SUB & 0x43) | 0x20C;

    DisplayBGSettings* subBg3 = Display_GetBG3Settings(DISPLAY_SUB);
    subBg3->bgMode            = DISPLAY_BGMODE_TEXT;
    subBg3->screenSizeText    = GX_BG_SIZE_TEXT_256x256;
    subBg3->colorMode         = GX_BG_COLORS_16;
    subBg3->screenBase        = 3;
    subBg3->charBase          = 5;
    subBg3->extPlttSlot       = 1;
    REG_BG3CNT_SUB            = (REG_BG3CNT_SUB & 0x43) | 0x314;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].layers      = LAYER_BG0 | LAYER_BG1 | LAYER_OBJ;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 16;

    func_02003ad0();
    func_02003c68();
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
}

void func_ov043_020c6a58(void) {
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
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
}

void func_ov043_020c6b40(void) {
    func_ov043_020c6684();
    Interrupts_RegisterVBlankCallback((void (*)(void))func_ov043_020c6a58, 1);
}

void func_ov043_020c6b5c(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

s32 func_ov043_020c6b70(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5) {
    if ((arg0 >= arg2) && (arg0 <= (s32)(arg2 + arg4)) && (arg1 >= arg3) && (arg1 <= (s32)(arg3 + arg5))) {
        return 1;
    }
    return 0;
}

void func_ov043_020c6bac(Sprite* sprite, s16 arg1) {
    // Not yet implemented
}

void func_ov043_020c6bf8(NRepContext* ctx) {
    return;
}

void func_ov043_020c6bfc(NRepContext* ctx) {
    return;
}

s16 func_ov043_020c6c00(s16 arg0, s16 arg1) {
    // Not yet implemented
}

void func_ov043_020c6ca8(NRepBgRes* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    // Not yet implemented
}

void func_ov043_020c6ea8(NRepBgRes* arg0, s32 arg1) {
    if (arg1 == 0) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[0], arg0->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[0], arg0->screenResource);
        func_0200afec(data_0206b3cc[0], arg0->unk_0C);
    } else {
        BgResMgr_ReleaseChar(g_BgResourceManagers[1], arg0->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[1], arg0->screenResource);
        func_0200afec(data_0206b3cc[1], arg0->unk_0C);
    }
    DatMgr_ReleaseData(arg0->data);
}

void func_ov043_020c6f30(NRepBgRes* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_ov043_020c6ea8(arg0, arg1);
    func_ov043_020c6ca8(arg0, arg1, arg2, arg3, arg4, arg5);
}

void func_ov043_020c6f78(NRepBgRes* arg0) {
    arg0->data           = NULL;
    arg0->screenResource = NULL;
    arg0->charResource   = NULL;
    arg0->unk_0C         = NULL;
    arg0->unk_14         = NULL;
    arg0->unk_10         = NULL;
    arg0->unk_18         = NULL;
}

void func_ov043_020c6f9c(NRepContext* ctx) {
    for (s32 i = 0; i < 4; i++) {
        func_ov043_020c6f78(&ctx->mainRes[i]);
        func_ov043_020c6f78(&ctx->subRes[i]);
    }
    func_ov043_020c6ca8(&ctx->subRes[1], 0, 1, 2, 15, 1);
    func_ov043_020c6ca8(&ctx->subRes[2], 0, 2, 1, 1, 1);
    func_ov043_020c6ca8(&ctx->subRes[3], 0, 3, 0, 0, 1);
    func_ov043_020c6ca8(&ctx->mainRes[1], 1, 1, 5, 15, 1);
    func_ov043_020c6ca8(&ctx->mainRes[3], 1, 3, 4, 1, 13);
}

void func_ov043_020c707c(NRepContext* ctx) {
    return;
}

void func_ov043_020c7080(NRepContext* ctx) {
    func_ov043_020c6ea8(&ctx->subRes[1], 0);
    func_ov043_020c6ea8(&ctx->subRes[2], 0);
    func_ov043_020c6ea8(&ctx->subRes[3], 0);
    func_ov043_020c6ea8(&ctx->mainRes[1], 1);
    func_ov043_020c6ea8(&ctx->mainRes[3], 1);
}

/// MARK: NRepMenu

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u8  unk2;
    /* 0x03 */ u8  unk3;
    /* 0x04 */ u8  optionStates[2];
} NRepMenuState;

typedef struct {
    /* 0x00 */ s32            unk_0;
    /* 0x04 */ NRepMenuState* unk_4;
} NRepMenuTaskArgs;

typedef struct {
    /* 0x00 */ Sprite         sprites[2];
    /* 0x80 */ void*          unk_80;
    /* 0x84 */ NRepMenuState* unk_84;
} NRepMenu;

static SpriteFrameInfo* func_ov043_020c70c8(Sprite* sprite, s32 arg1, s32 arg2);
static s32              NRepMenu_Init(TaskPool* pool, Task* task, void* args);
static s32              NRepMenu_Update(TaskPool* pool, Task* task, void* args);
static s32              NRepMenu_Render(TaskPool* pool, Task* task, void* args);
static s32              NRepMenu_Release(TaskPool* pool, Task* task, void* args);
static s32              NRepMenu_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const BinIdentifier data_ov043_020cb4a0 = {43, "Apl_Tak/Grp_MenuIcon.bin"};

const SpriteAnimation data_ov043_020cb4d8 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0x400,
    .unk_04     = 80,
    .unk_06     = 80,
    .unk_08     = func_ov043_020c70c8,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020cb4a0,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

SpriteFrameInfo* func_ov043_020c70c8(Sprite* sprite, s32 arg1, s32 arg2) {
    // Not yet implemented
}

void NrepMenu_Load(NRepMenu* arg0, NRepMenuTaskArgs* args) {
    SpriteAnimation anim = data_ov043_020cb4d8;

    anim.dataType = args->unk_0;
    anim.unk_2A   = 3;
    anim.unk_04   = 218;
    anim.unk_06   = 12;
    _Sprite_Load(&arg0->sprites[0], &anim);

    anim.unk_2A = 5;
    anim.unk_04 = 243;
    anim.unk_06 = 12;
    _Sprite_Load(&arg0->sprites[1], &anim);
}

s32 NRepMenu_Init(TaskPool* pool, Task* task, void* args) {
    NRepMenu*         menu     = task->data;
    NRepMenuTaskArgs* menuArgs = args;

    menu->unk_84 = menuArgs->unk_4;
    NrepMenu_Load(menu, menuArgs);
    return 1;
}

s32 NRepMenu_Update(TaskPool* pool, Task* task, void* args) {
    NRepMenu*      menu  = task->data;
    NRepMenuState* state = menu->unk_84;

    if (state->unk0 & 0x1100) {
        return 1;
    }

    if (TouchInput_WasTouchPressed()) {
        TouchCoord touchCoord;
        TouchInput_GetCoord(&touchCoord);

        s16 hitboxIdx = func_ov043_020c6c00(touchCoord.x, touchCoord.y);

        if (hitboxIdx != -1) {
            if (state->optionStates[hitboxIdx] == 1) {
                func_ov043_020c6bac(&menu->sprites[hitboxIdx], (hitboxIdx * 2) + 3);
                state->optionStates[hitboxIdx] = 0;
            } else {
                func_ov043_020c6bac(&menu->sprites[hitboxIdx], (hitboxIdx * 2) + 4);
                state->optionStates[hitboxIdx] = 1;
            }

            if (hitboxIdx == 1) {
                state->unk0 |= 0x1000;
                func_02026b20(0x11B);
                func_02026b20(0x117);
                state->unk3 = 0;
                DebugOvlDisp_Pop();
            } else {
                func_02026b20(0x11A);
            }
        }
    }

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&menu->sprites[i]);
    }

    return 1;
}

s32 NRepMenu_Render(TaskPool* pool, Task* task, void* args) {
    NRepMenu* menu = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&menu->sprites[i]);
    }
    return 1;
}

s32 NRepMenu_Release(TaskPool* pool, Task* task, void* args) {
    NRepMenu* menu = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&menu->sprites[i]);
    }
    return 1;
}

s32 NRepMenu_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cb4c8 = {
        .initialize = NRepMenu_Init,
        .update     = NRepMenu_Update,
        .render     = NRepMenu_Render,
        .cleanup    = NRepMenu_Release,
    };
    return data_ov043_020cb4c8.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020cb4bc = {"Tsk_NRep_Menu", NRepMenu_RunTask, sizeof(NRepMenu)};

s32 NRepMenu_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    NRepMenuTaskArgs args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020cb4bc, NULL, 0, NULL, &args);
}

/// MARK: NRepText

typedef struct {
    /* 0x00 */ u8  pad_00[0xA4];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
} NRepTextRes;

typedef struct {
    /* 0x00 */ NRepTextRes*  unk_00;
    /* 0x04 */ UnkOv31Struct unk_04;
    /* 0x80 */ u16           unk_80;
    /* 0x82 */ u8            pad_82[0x2];
} NRepText;

typedef struct {
    /* 0x00 */ s32          unk_0;
    /* 0x04 */ NRepTextRes* unk_4;
} NRepTextTaskArgs;

static s32 NRepText_Init(TaskPool* pool, Task* task, void* args);
static s32 NRepText_Update(TaskPool* pool, Task* task, void* args);
static s32 NRepText_Render(TaskPool* pool, Task* task, void* args);
static s32 NRepText_Release(TaskPool* pool, Task* task, void* args);
static s32 NRepText_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

void func_ov043_020c7420(NRepText* text) {
    func_ov031_0210aa94(&text->unk_04);
    func_ov031_0210ab34(&text->unk_04, 14);
}

void func_ov043_020c7440(NRepText* arg0) {
    (void)arg0;
}

s32 NRepText_Init(TaskPool* pool, Task* task, void* args) {
    NRepText*         text     = task->data;
    NRepTextTaskArgs* taskArgs = args;

    text->unk_00 = taskArgs->unk_4;
    text->unk_80 = 0x3388;
    func_ov043_020c7420(text);
    func_ov043_020c7440(text);
    return 1;
}

s32 NRepText_Update(TaskPool* pool, Task* task, void* args) {
    NRepText*    text    = task->data;
    NRepTextRes* textRes = text->unk_00;

    if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_A) {
        text->unk_80++;
        func_ov043_020c6f30((NRepBgRes*)((u8*)textRes + 0x94), 0, 2, 1, 0xF, 1);
        func_ov043_020c7440(text);
    }
    return 1;
}

s32 NRepText_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 NRepText_Release(TaskPool* pool, Task* task, void* args) {
    NRepText* text = task->data;

    func_ov031_0210aabc(&text->unk_04);
    return 1;
}

s32 NRepText_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cb514 = {
        .initialize = NRepText_Init,
        .update     = NRepText_Update,
        .render     = NRepText_Render,
        .cleanup    = NRepText_Release,
    };
    return data_ov043_020cb514.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020cb508 = {"Tsk_NRep_Text", NRepText_RunTask, sizeof(NRepText)};

s32 NRepText_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    NRepTextTaskArgs args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020cb508, NULL, 0, NULL, &args);
}

/// MARK: NRepCursor

typedef struct {
    /* 0x00 */ Sprite       sprite;
    /* 0x40 */ s32          unk_40;
    /* 0x44 */ NRepContext* context;
} NRepCursor;

typedef struct {
    /* 0x00 */ s32          unk_0;
    /* 0x04 */ NRepContext* context;
    /* 0x08 */ u16          unk_8;
} NRepCursorTaskArgs;

static SpriteFrameInfo* NRepCursor_GetAnimInfo(Sprite* sprite, s32 arg1, s32 arg2);
static void             NRepCursor_Load(NRepCursor* arg0, Sprite* arg1, NRepCursorTaskArgs* args);
static s32              NRepCursor_Init(TaskPool* pool, Task* task, void* args);
static s32              NRepCursor_Update(TaskPool* pool, Task* task, void* args);
static s32              NRepCursor_Render(TaskPool* pool, Task* task, void* args);
static s32              NRepCursor_Release(TaskPool* pool, Task* task, void* args);
static s32              NrepCursor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const BinIdentifier data_ov043_020cb4a8 = {43, "Apl_Tak/Grp_Badge.bin"};

const SpriteAnimation data_ov043_020cb540 = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 80,
    .unk_06     = 80,
    .unk_08     = NRepCursor_GetAnimInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020cb4a8,
    .unk_18     = 2,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

SpriteFrameInfo* NRepCursor_GetAnimInfo(Sprite* sprite, s32 arg1, s32 arg2) {
    (void)sprite;
    (void)arg2;
    return NULL;
}

void NRepCursor_Load(NRepCursor* cursor, Sprite* sprite, NRepCursorTaskArgs* args) {
    SpriteAnimation anim = data_ov043_020cb540;

    anim.dataType = args->unk_0;
    anim.unk_04   = ((args->unk_8 % 8) * 26) + 40;
    anim.unk_06   = ((args->unk_8 / 8) * 26) + 40;

    if (args->unk_8 < 16) {
        anim.bits_7_9 = 5;
    } else {
        anim.bits_7_9 = 6;
    }

    anim.packIndex = args->unk_8 + 1;
    cursor->unk_40 = 1;
    _Sprite_Load(sprite, &anim);
}

s32 NRepCursor_Init(TaskPool* pool, Task* task, void* args) {
    NRepCursor*         cursor   = task->data;
    NRepCursorTaskArgs* taskArgs = args;

    cursor->context = taskArgs->context;
    NRepCursor_Load(cursor, &cursor->sprite, taskArgs);
    return 1;
}

s32 NRepCursor_Update(TaskPool* pool, Task* task, void* args) {
    NRepCursor* cursor = task->data;

    Sprite_Update(&cursor->sprite);
    return 1;
}

s32 NRepCursor_Render(TaskPool* pool, Task* task, void* args) {
    NRepCursor* cursor = task->data;

    if (cursor->unk_40 != 0) {
        Sprite_RenderFrame(&cursor->sprite);
    }
    return 1;
}

s32 NRepCursor_Release(TaskPool* pool, Task* task, void* args) {
    NRepCursor* cursor = task->data;

    Sprite_Release(&cursor->sprite);
    return 1;
}

s32 NrepCursor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cb530 = {
        .initialize = NRepCursor_Init,
        .update     = NRepCursor_Update,
        .render     = NRepCursor_Render,
        .cleanup    = NRepCursor_Release,
    };
    return data_ov043_020cb530.iter[stage](pool, task, args);
}

const TaskHandle data_ov043_020cb524 = {"Tsk_NRep_Cursor", NrepCursor_RunTask, sizeof(NRepCursor)};

s32 NRepCursor_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, NRepContext* context) {
    NRepCursorTaskArgs args;
    args.unk_0   = arg1;
    args.unk_8   = arg2;
    args.context = context;
    return EasyTask_CreateTask(pool, &data_ov043_020cb524, NULL, 0, NULL, &args);
}
