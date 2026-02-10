#include "BgResMgr.h"
#include "DMA.h"
#include "DatMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "EasyTask.h"
#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "OverlayManager.h"
#include "System.h"
#include "TouchInput.h"
#include "common_data.h"
#include <NitroSDK/gx.h>
#include <types.h>

// Forward declarations of external functions
extern void func_0200434c(void);
extern void GX_Init(void);
extern void func_0200270c(s32, s32);
extern void func_0203b2d0(s32, void*, s32);
extern void func_0200283c(void*, s32, s32);
extern void DC_PurgeRange(void*, u32);
extern void func_02001c34(void*, void*, s32, s32, s32);
extern void func_0200cef0(void*);
extern void func_02003440(void*);
extern void func_020034b0(void*);
extern void func_0200adf8(void*, void*, s32, s32, s32);
extern void func_0200bf60(void*, s32);
extern void func_0200d120(void*);
extern void func_02026b20(s32);
extern void func_ov003_02082724(void*, s32, s32);
extern void func_ov003_02082940(u16*, s32, BinIdentifier*);
extern void func_ov003_02082998(void*, u16*);
extern void func_ov003_02082b0c(void*);
extern void func_ov003_02082b64(void*);
extern void func_ov003_02082cc4(void*);

extern InputState InputStatus;
extern void*      func_ov003_0208ec74;
extern MemPool    gDebugHeap;

// Local structures
typedef struct {
    u8  unk0;
    u8  unk1;
    u8  unk2;
    u8  unk3;
    u16 unk4;
    u16 unk6;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    u8  padB;
    u16 unkC;
    u16 unkE;
} TutorialConfig;

typedef struct {
    u8    pad0[0x800];
    u8    pad800[0xC];
    u8    pad80C[0x80];
    u8    pad88C[0x11580];
    Data* unk11E0C;
    Data* unk11E10;
    u8    pad11E14[0x60];
    s16   unk11E74;
    s16   unk11E76;
} Ov026State;

// Static data
static const char* data_ov026_020e8060   = "Seq_Tutorial( )";
static const char  data_ov026_020e8074[] = "Apl_Fur/Grp_Tutorial.bin";

static const BinIdentifier data_ov026_020e7fd8 = {0x1A, (void*)data_ov026_020e8074};

static const TutorialConfig data_ov026_020e7fec[6] = {
    {  2, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {  6, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {  7, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {0xB, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {0xC, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {0xD, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
};

// Forward declarations of local functions
static void func_ov026_020e7620(void);
static void func_ov026_020e7c2c(void* arg0);
static void func_ov026_020e7c9c(void* arg0);
static void func_ov026_020e7cec(void* arg0);
static void func_ov026_020e7d3c(void* arg0);
static void func_ov026_020e7e7c(void* arg0);
static void func_ov026_020e7f30(void* arg0);

static const OverlayCB data_ov026_020e7fe0[3] = {func_ov026_020e7d3c, func_ov026_020e7e7c, func_ov026_020e7f30};

// Function implementations

static void func_ov026_020e7360(void) {
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForBg(GX_VRAM_A);
    GX_SetBankForObj(GX_VRAM_B);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_D);
    g_DisplaySettings.controls[0].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[1].objTileMode = GX_OBJTILEMODE_1D_128K;
    func_0200270c(0, 0);
    func_0200270c(0, 1);
    func_0203b2d0(0, (void*)0x06800000, 0xA4000);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_02001c34(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

static void func_ov026_020e749c(void) {
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;
    *(u16*)0x04000304 &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);
    g_DisplaySettings.engineState[0].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[0].bgSettings[1].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[0].bgSettings[1].screenBase     = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].extPlttSlot    = 0;
    ((vu16*)0x0400000A)[0]                                        = (((vu16*)0x0400000A)[0] & 0x43) | 0x84;
    g_DisplaySettings.engineState[0].bgSettings[2].charBase       = 3;
    g_DisplaySettings.engineState[0].bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[0].bgSettings[2].screenBase     = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[0].bgSettings[2].extPlttSlot    = 1;
    ((vu16*)0x0400000A)[1]                                        = (((vu16*)0x0400000A)[1] & 0x43) | 0x28C;
    g_DisplaySettings.engineState[0].bgSettings[1].priority       = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority       = 2;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic         = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic         = 0;
    g_DisplaySettings.controls[0].layers                          = 0x16;
    g_DisplaySettings.controls[1].bgMode                          = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    g_DisplaySettings.engineState[1].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[1].bgSettings[1].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[1].bgSettings[1].screenBase     = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].extPlttSlot    = 0;
    ((vu16*)0x0400100A)[0]                                        = (((vu16*)0x0400100A)[0] & 0x43) | 0x84;
    g_DisplaySettings.engineState[1].bgSettings[2].charBase       = 3;
    g_DisplaySettings.engineState[1].bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[1].bgSettings[2].screenBase     = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[1].bgSettings[2].extPlttSlot    = 1;
    ((vu16*)0x0400100A)[1]                                        = (((vu16*)0x0400100A)[1] & 0x43) | 0x28C;
    g_DisplaySettings.engineState[1].bgSettings[1].priority       = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority       = 2;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic         = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic         = 0;
    g_DisplaySettings.controls[1].layers                          = 6;
}

static void func_ov026_020e7620(void) {
    if (SystemStatusFlags.vblank == 0) {
        return;
    }
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

static void func_ov026_020e76f0(void) {
    func_ov026_020e7360();
    func_ov026_020e749c();
    Interrupts_RegisterVBlankCallback(func_ov026_020e7620, 1);
}

static void func_ov026_020e7710(void) {
    func_ov026_020e7360();
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

static void func_ov026_020e7728(void* state, s32 arg1) {
    u16 sp0;
    s16 sp18;
    s16 sp1A;
    s16 sp1C;
    s16 sp20;
    s16 sp26;
    s16 sp28;
    s16 sp2A;
    s32 sp2C;

    func_ov003_02082940(&sp0, 0, (BinIdentifier*)&data_ov026_020e7fd8);
    sp18 = 2;
    sp1A = 1;
    sp0 &= ~0xC00;
    sp1C = 3;
    sp26 = 4;
    sp28 = 5;
    sp20 = 6;
    sp2A = arg1 + 1;
    sp2C = 0;
    func_ov003_02082998(state, &sp0);
    func_ov003_02082724(state, 0x80, 0x60);
}

static s32 func_ov026_020e77bc(void) {
    s32 value = data_02074d10.unk_410;

    switch (value) {
        case 2:
            return 0;
        case 4:
            return 1;
        case 5:
            return 2;
        case 9:
            return 3;
        case 0x12:
            return 4;
        case 0x21:
            return 5;
        default:
            return 0;
    }
}

static void* GetPackEntryData(Data* data, u8 index) {
    if (data == NULL || index <= 0) {
        return NULL;
    }
    void* base = (u8*)data->buffer + 0x20;
    return (u8*)base + ((u32*)base)[index];
}

static void func_ov026_020e7838(Ov026State* state) {
    s32                   index  = func_ov026_020e77bc();
    const TutorialConfig* config = &data_ov026_020e7fec[index];

    state->unk11E0C = DatMgr_LoadPackEntry(1, NULL, 0, (BinIdentifier*)&data_ov026_020e7fd8, config->unk0, 0);
    state->unk11E10 = DatMgr_LoadPackEntry(1, NULL, 0, (BinIdentifier*)&data_ov026_020e7fd8, 1, 0);

    void* char2Data   = GetPackEntryData(state->unk11E0C, config->unk2);
    void* screen1Data = NULL;
    if (state->unk11E10 != NULL) {
        void* base  = (u8*)state->unk11E10->buffer + 0x20;
        screen1Data = (u8*)base + ((u32*)base)[1];
    }

    BgResMgr_AllocCharExtended(g_BgResourceManagers[1], char2Data, g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0,
                               config->unk4);
    BgResMgr_AllocScreen(g_BgResourceManagers[1], screen1Data, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);

    void* char3Data   = GetPackEntryData(state->unk11E0C, config->unk3);
    void* screen2Data = NULL;
    if (state->unk11E10 != NULL) {
        void* base  = (u8*)state->unk11E10->buffer + 0x20;
        screen2Data = (u8*)base + ((u32*)base)[2];
    }

    BgResMgr_AllocCharExtended(g_BgResourceManagers[1], char3Data, g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0,
                               config->unk6);
    BgResMgr_AllocScreen(g_BgResourceManagers[1], screen2Data, g_DisplaySettings.engineState[1].bgSettings[2].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText);

    void* char1Data = GetPackEntryData(state->unk11E0C, config->unk1);
    func_0200adf8(data_0206b3cc[1], char1Data, 0, 0, 0x10);

    void* char9Data   = GetPackEntryData(state->unk11E0C, config->unk9);
    void* screen3Data = NULL;
    if (state->unk11E10 != NULL) {
        void* base  = (u8*)state->unk11E10->buffer + 0x20;
        screen3Data = (u8*)base + ((u32*)base)[1];
    }

    BgResMgr_AllocCharExtended(g_BgResourceManagers[0], char9Data, g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0,
                               config->unkC);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], screen3Data, g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);

    void* charAData   = GetPackEntryData(state->unk11E0C, config->unkA);
    void* screen4Data = NULL;
    if (state->unk11E10 != NULL) {
        void* base  = (u8*)state->unk11E10->buffer + 0x20;
        screen4Data = (u8*)base + ((u32*)base)[2];
    }

    BgResMgr_AllocCharExtended(g_BgResourceManagers[0], charAData, g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0,
                               config->unkE);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], screen4Data, g_DisplaySettings.engineState[0].bgSettings[2].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText);

    void* char8Data = GetPackEntryData(state->unk11E0C, config->unk8);
    func_0200adf8(data_0206b3cc[0], char8Data, 0, 0, 0x10);
}

static void func_ov026_020e7ba0(Ov026State* state) {
    func_ov026_020e7838(state);
    func_ov026_020e7728((u8*)state + 0x11E14, 0);
}

static void func_ov026_020e7bc0(Ov026State* state) {
    func_ov003_02082b0c((u8*)state + 0x11E14);
}

static void func_ov026_020e7bd4(Ov026State* state) {
    func_ov003_02082b64((u8*)state + 0x11E14);
}

static void func_ov026_020e7be8(Ov026State* state) {
    func_ov003_02082cc4((u8*)state + 0x11E14);
}

static void func_ov026_020e7bfc(void) {
    OverlayTag tag;
    MainOvlDisp_ReplaceTop(&tag, 3, (OverlayCB)&func_ov003_0208ec74, NULL, 0);
}

static void func_ov026_020e7c2c(void* arg0) {
    Ov026State* state = (Ov026State*)arg0;

    if (state->unk11E74 == 0) {
        EasyFade_FadeBothDisplays(0, 0, 0x1000);
        state->unk11E74++;
    }
    if (EasyFade_IsFading()) {
        return;
    }
    state->unk11E74 = 0;
    state->unk11E76 = 0;
    DebugOvlDisp_ReplaceTop(func_ov026_020e7cec, state, 0);
}

static void func_ov026_020e7c9c(void* arg0) {
    Ov026State* state = (Ov026State*)arg0;

    if (state->unk11E74 == 0) {
        EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
        state->unk11E74++;
    }
    if (EasyFade_IsFading()) {
        return;
    }
    DebugOvlDisp_Pop();
}

static void func_ov026_020e7cec(void* arg0) {
    Ov026State* state = (Ov026State*)arg0;

    if (!(InputStatus.buttonState.pressedButtons & 4)) {
        return;
    }
    func_02026b20(0);
    state->unk11E74 = 0;
    state->unk11E76 = 0;
    DebugOvlDisp_ReplaceTop(func_ov026_020e7c9c, state, 0);
}

static void func_ov026_020e7d3c(void* arg0) {
    Ov026State* state = (Ov026State*)arg0;

    if (state == NULL) {
        state = (Ov026State*)Mem_AllocHeapTail(&gDebugHeap, 0x11E78);
        Mem_SetSequence(&gDebugHeap, state, data_ov026_020e8060);
        MainOvlDisp_SetCbArg(state);
    }

    func_0203b2d0(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    OvlMgr_LoadOverlay(3, 3);
    Mem_InitializeHeap((MemPool*)((u8*)state + 0x800), state, 0x800);
    EasyTask_InitializePool((TaskPool*)((u8*)state + 0x80C), (MemPool*)((u8*)state + 0x800), 8, NULL, NULL);
    func_ov026_020e76f0();
    func_0200cef0((u8*)state + 0x88C);
    data_02066aec = 0;
    data_02066eec = 0;
    Input_Init(&InputStatus, 0xC, 2, 1);
    TouchInput_Init();
    EasyTask_CreateTask((TaskPool*)((u8*)state + 0x80C), &Task_EasyFade, NULL, 0, NULL, NULL);
    func_ov026_020e7ba0(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push(func_ov026_020e7c2c, state, 0);
    MainOvlDisp_IncrementRepeatCount();
    MainOvlDisp_Run();
}

static void func_ov026_020e7e7c(void* arg0) {
    Ov026State* state = (Ov026State*)arg0;

    TouchInput_Update();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov026_020e7bc0(state);
    func_ov026_020e7bd4(state);
    DebugOvlDisp_Run();

    if (DebugOvlDisp_IsStackAtBase()) {
        func_ov026_020e7bfc();
        return;
    }

    EasyTask_UpdatePool((TaskPool*)((u8*)state + 0x80C));
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

static void func_ov026_020e7f30(void* arg0) {
    Ov026State* state = (Ov026State*)arg0;

    if (state == NULL) {
        return;
    }

    func_ov026_020e7be8(state);
    func_0200d120((u8*)state + 0x88C);
    DatMgr_ReleaseData(state->unk11E0C);
    DatMgr_ReleaseData(state->unk11E10);
    func_ov026_020e7710();
    EasyTask_DestroyPool((TaskPool*)((u8*)state + 0x80C));
    Mem_Free(&gDebugHeap, state);
    MainOvlDisp_SetCbArg(NULL);
    OvlMgr_UnloadOverlay(3);
}

void func_ov026_020e7f98(void* arg0) {
    s32 repeatCount = MainOvlDisp_GetRepeatCount();

    if (repeatCount == 0x7FFFFFFF) {
        func_ov026_020e7f30(arg0);
        return;
    }

    data_ov026_020e7fe0[repeatCount](arg0);
}
