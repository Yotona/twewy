#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Save.h"
#include "SndMgrSeIdx.h"
#include "common_data.h"
#include <NitroSDK/gx.h>
#include <types.h>

#include "Debug/Furukawa/Tutorial.h"

// Forward declarations of external functions
extern void HBlank_Init(void);
extern void GX_Init(void);
extern void MI_CpuFill(s32, void*, s32);
extern void DC_PurgeRange(void*, u32);
extern void Color_CopyRange(void*, void*, s32, s32, s32);
extern void SndMgr_StartPlayingSE(s32);
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

// Static data
static const char* data_ov026_020e8060 = "Seq_Tutorial( )";

static const BinIdentifier data_ov026_020e7fd8 = {26, "Apl_Fur/Grp_Tutorial.bin"};

static const TutorialConfig data_ov026_020e7fec[6] = {
    { 2, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    { 6, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    { 7, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {11, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {12, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
    {13, 3, 1, 2, 0x6400, 0x6400, 6, 4, 5, 0, 0x6400, 0x6400},
};

// Forward declarations of local functions
static void func_ov026_020e7cec(TutorialObject* object);

// Function implementations

static void Tutorial_InitHardware(void) {
    Interrupts_Init();
    HBlank_Init();
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
    OamMgr_Init(0, 0);
    OamMgr_Init(0, 1);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    Color_CopyRange(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

static void Tutorial_InitDisplay(void) {
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_256, 0, 1, 0, 0x84);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_256, 2, 3, 1, 0x28C);

    g_DisplaySettings.engineState[0].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.controls[0].layers                    = 0x16;
    g_DisplaySettings.controls[1].bgMode                    = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_256, 0, 1, 0, 0x84);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_256, 2, 3, 1, 0x28C);

    g_DisplaySettings.engineState[1].bgSettings[1].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.controls[1].layers                    = 6;
}

static void Tutorial_VBlank(void) {
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

static void Tutorial_RegisterVBlank(void) {
    Tutorial_InitHardware();
    Tutorial_InitDisplay();
    Interrupts_RegisterVBlankCallback(Tutorial_VBlank, 1);
}

static void Tutorial_UnregisterVBlank(void) {
    Tutorial_InitHardware();
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

    func_ov003_02082940(&sp0, 0, &data_ov026_020e7fd8);
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
    s32 val = 0;
    switch (data_02071cf0.unk_20.unk_3134) {
        case 2:
            return 0;
        case 4:
            return 1;
        case 5:
            return 2;
        case 9:
            return 3;
        case 18:
            return 4;
        case 33:
            val = 5;
    }
    return val;
}

static void Tutorial_LoadResources(TutorialObject* object) {
    s32                   index  = func_ov026_020e77bc();
    const TutorialConfig* config = &data_ov026_020e7fec[index];

    object->unk_11E0C = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov026_020e7fd8, config->unk0, 0);
    object->unk_11E10 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov026_020e7fd8, 1, 0);

    void* char2Data   = Data_GetPackEntryData(object->unk_11E0C, config->unk2);
    void* screen1Data = Data_GetPackEntryData(object->unk_11E10, 1);

    BgResMgr_AllocCharExtended(g_BgResourceManagers[1], char2Data, g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0,
                               config->unk4);

    BgResMgr_AllocScreen(g_BgResourceManagers[1], screen1Data, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);

    void* char3Data   = Data_GetPackEntryData(object->unk_11E0C, config->unk3);
    void* screen2Data = Data_GetPackEntryData(object->unk_11E10, 2);

    BgResMgr_AllocCharExtended(g_BgResourceManagers[1], char3Data, g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0,
                               config->unk6);
    BgResMgr_AllocScreen(g_BgResourceManagers[1], screen2Data, g_DisplaySettings.engineState[1].bgSettings[2].screenBase,
                         g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText);

    void* char1Data = Data_GetPackEntryData(object->unk_11E0C, config->unk1);
    PaletteMgr_AllocPalette(g_PaletteManagers[1], char1Data, 0, 0, 0x10);

    void* char9Data   = Data_GetPackEntryData(object->unk_11E0C, config->unk9);
    void* screen3Data = Data_GetPackEntryData(object->unk_11E10, 1);

    BgResMgr_AllocCharExtended(g_BgResourceManagers[0], char9Data, g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0,
                               config->unkC);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], screen3Data, g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);

    void* charAData   = Data_GetPackEntryData(object->unk_11E0C, config->unkA);
    void* screen4Data = Data_GetPackEntryData(object->unk_11E10, 2);

    BgResMgr_AllocCharExtended(g_BgResourceManagers[0], charAData, g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0,
                               config->unkE);
    BgResMgr_AllocScreen(g_BgResourceManagers[0], screen4Data, g_DisplaySettings.engineState[0].bgSettings[2].screenBase,
                         g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText);

    void* char8Data = Data_GetPackEntryData(object->unk_11E0C, config->unk8);
    PaletteMgr_AllocPalette(g_PaletteManagers[0], char8Data, 0, 0, 0x10);
}

static void func_ov026_020e7ba0(TutorialObject* object) {
    Tutorial_LoadResources(object);
    func_ov026_020e7728((u8*)object + 0x11E14, 0);
}

static void func_ov026_020e7bc0(TutorialObject* object) {
    func_ov003_02082b0c((u8*)object + 0x11E14);
}

static void func_ov026_020e7bd4(TutorialObject* object) {
    func_ov003_02082b64((u8*)object + 0x11E14);
}

static void func_ov026_020e7be8(TutorialObject* object) {
    func_ov003_02082cc4((u8*)object + 0x11E14);
}

static void func_ov026_020e7bfc(TutorialObject* object) {
    OverlayTag tag;
    MainOvlDisp_ReplaceTop(&tag, &OVERLAY_3_ID, (OverlayCB)&func_ov003_0208ec74, NULL, 0);
}

static void func_ov026_020e7c2c(TutorialObject* object) {
    if (object->unk_11E74 == 0) {
        EasyFade_FadeBothDisplays(0, 0, 0x1000);
        object->unk_11E74++;
    }
    if (EasyFade_IsFading() == FALSE) {
        object->unk_11E74 = 0;
        object->unk_11E76 = 0;
        DebugOvlDisp_ReplaceTop((OverlayCB)func_ov026_020e7cec, object, 0);
    }
}

static void func_ov026_020e7c9c(TutorialObject* object) {
    if (object->unk_11E74 == 0) {
        EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
        object->unk_11E74++;
    }
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

static void func_ov026_020e7cec(TutorialObject* object) {
    if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_IN);
        object->unk_11E74 = 0;
        object->unk_11E76 = 0;
        DebugOvlDisp_ReplaceTop((OverlayCB)func_ov026_020e7c9c, object, 0);
    }
}

static void Tutorial_Init(TutorialObject* object) {
    if (object == NULL) {
        const char* sequence = data_ov026_020e8060;
        object               = Mem_AllocHeapTail(&gDebugHeap, 0x11E78);
        Mem_SetSequence(&gDebugHeap, object, sequence);
        MainOvlDisp_SetCbArg(object);
    }

    MI_CpuFill(0, object, Mem_GetBlockSize(&gDebugHeap, object));
    OvlMgr_LoadOverlay(3, &OVERLAY_3_ID);
    Mem_InitializeHeap(&object->memPool, object->memBuffer, sizeof(object->memBuffer));
    EasyTask_InitializePool(&object->taskPool, &object->memPool, 8, NULL, NULL);
    Tutorial_RegisterVBlank();
    ResourceMgr_ReinitManagers(&object->resMgr);
    data_02066aec = 0;
    data_02066eec = 0;
    Input_Init(&InputStatus, 12, 2, 1);
    TouchInput_Init();
    EasyTask_CreateTask(&object->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    func_ov026_020e7ba0(object);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov026_020e7c2c, object, 0);
    MainOvlDisp_NextProcessStage();
    MainOvlDisp_Run();
}

static void Tutorial_Update(TutorialObject* object) {
    TouchInput_Update();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    func_ov026_020e7bc0(object);
    func_ov026_020e7bd4(object);
    DebugOvlDisp_Run();

    if (DebugOvlDisp_IsStackAtBase()) {
        func_ov026_020e7bfc(object);
        return;
    }

    EasyTask_UpdatePool(&object->taskPool);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

static void Tutorial_Destroy(TutorialObject* object) {
    if (object != NULL) {
        func_ov026_020e7be8(object);
        ResourceMgr_ReleaseResource((Resource*)&object->resMgr); // BUG: Attempt to release ResourceManager instead of Resource
        DatMgr_ReleaseData(object->unk_11E0C);
        DatMgr_ReleaseData(object->unk_11E10);
        Tutorial_UnregisterVBlank();
        EasyTask_DestroyPool(&object->taskPool);
        Mem_Free(&gDebugHeap, object);
        MainOvlDisp_SetCbArg(NULL);
        OvlMgr_UnloadOverlay(3);
    }
}

static const OverlayProcess OvlProc_Tutorial = {
    .init = Tutorial_Init,
    .main = Tutorial_Update,
    .exit = Tutorial_Destroy,
};

void ProcessOverlay_Tutorial(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == 0x7FFFFFFF) {
        Tutorial_Destroy(object);
    } else {
        OvlProc_Tutorial.funcs[stage](object);
    }
}
