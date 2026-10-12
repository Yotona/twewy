#include "Otosu/OtosuMenu.h"

static void          OtosuMenu_FontList_Load(void);
static void          OtosuMenu_FontList_Destroy(PrcCtx* ctx, void* menuObj);
static void          OtosuMenu_FontList_Update(void);
static void          OtosuMenu_FontList_Render(void);
static PrcStepResult OtosuMenu_FontList_Step_Setup(PrcCtx* ctx, void* object);
static PrcStepResult OtosuPrcStep_CheckButtonInput(PrcCtx* ctx, void* object);

static PrcStepFn OtosuMenu_FontList_StepTable[] = {
    OtosuMenu_FontList_Step_Setup,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuPrcStep_CheckButtonInput,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    PrcStep_PopFrame,
};

PrcFrameDesc OtosuMenu_FontList_FrameDesc = {
    .enter     = OtosuMenu_FontList_Load,
    .stepTable = OtosuMenu_FontList_StepTable,
    .update    = OtosuMenu_FontList_Update,
    .render    = OtosuMenu_FontList_Render,
    .exit      = OtosuMenu_FontList_Destroy,
};

#ifdef REGION_USA
static const u16 OtosuMenu_FontList_SampleMsgs[14] = {
    SYSMSG_FONT_CHECK_TEXT + 16, SYSMSG_FONT_CHECK_TEXT + 5, SYSMSG_FONT_CHECK_TEXT + 6,  SYSMSG_FONT_CHECK_TEXT + 7,
    SYSMSG_FONT_CHECK_TEXT + 8,  SYSMSG_FONT_CHECK_TEXT + 9, SYSMSG_FONT_CHECK_TEXT + 10, SYSMSG_FONT_CHECK_TEXT + 11,
    SYSMSG_FONT_CHECK_TEXT + 12, SYSMSG_FONT_CHECK_TEXT + 0, SYSMSG_FONT_CHECK_TEXT + 1,  SYSMSG_FONT_CHECK_TEXT + 2,
    SYSMSG_FONT_CHECK_TEXT + 3,  SYSMSG_FONT_CHECK_TEXT + 4,
};
#endif

static void OtosuMenu_FontList_Load(void) {
    return;
}

static void OtosuMenu_FontList_Destroy(PrcCtx* ctx, void* menuObj) {
    OtosuMenu_ReleaseScreens(menuObj);
}

static void OtosuMenu_FontList_Update(void) {
    return;
}

static void OtosuMenu_FontList_Render(void) {
    return;
}

static void OtosuMenu_FontList_DrawSample(OtosuMenuObj* menuObj, void* chars, void* screen) {
    SysFont* font = menuObj->fontListFont;
#ifdef REGION_USA
    u16 text[0xC8] = {0xFFFF};

    SysFont_SetMsg(font, OtosuMenu_FontList_SampleMsgs[menuObj->unk_474CA]);
#else
    // JP keeps its sample messages in a local table
    u16 sampleMsgs[9] = {
        SYSMSG_FONT_CHECK_TEXT + 9, SYSMSG_FONT_CHECK_TEXT + 1, SYSMSG_FONT_CHECK_TEXT + 2,
        SYSMSG_FONT_CHECK_TEXT + 3, SYSMSG_FONT_CHECK_TEXT + 4, SYSMSG_FONT_CHECK_TEXT + 5,
        SYSMSG_FONT_CHECK_TEXT + 6, SYSMSG_FONT_CHECK_TEXT + 7, SYSMSG_FONT_CHECK_TEXT + 8,
    };
    u16 text[0xC8] = {0xFFFF};

    SysFont_SetMsg(font, sampleMsgs[menuObj->unk_474CA]);
#endif
    SysFont_SetPos(font, 0, 0);
    SysFont_SetHAlign(font, 0, 256);
    SysFont_SetVAlign(font, 0, 192);
    SysFont_DrawCurrentToScreen(font, screen, chars, 0);
}

// Nonmatching: instruction selection differs
static void OtosuMenu_FontList_DrawPageInfo(OtosuMenuObj* menuObj, void* chars, void* screen) {
    u16      text[0xC8]       = {0xFFFF};
    u16      fontNameMsgs[3]  = {SYSMSG_FONTLIST_FONT_NAME_START + 1, SYSMSG_FONTLIST_FONT_NAME_START,
                                 SYSMSG_FONTLIST_FONT_NAME_START + 2};
    u16      colorNameMsgs[3] = {SYSMSG_FONTLIST_COLOR_NAME_START, SYSMSG_FONTLIST_COLOR_NAME_START + 1,
                                 SYSMSG_FONTLIST_COLOR_NAME_START + 2};
    SysCode* lowerText;
    SysCode* upperText;
    SysCode* fmt;
    void*    font;

    font      = menuObj->fontListFont;
    fmt       = SysFont_GetMsgBuf(font, SYSMSG_FONT_PAGE_FMT);
    upperText = SysFont_GetMsgBuf(font, fontNameMsgs[menuObj->unk_474C8]);
    lowerText = SysFont_GetMsgBuf(font, colorNameMsgs[menuObj->unk_474CC]);
    SysFont_Format(text, fmt, upperText, menuObj->unk_474CA + 1, 14, lowerText);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, upperText);
    Mem_Free(&gDebugHeap, lowerText);
    SysFont_SetMsgPtr(font, text);
    SysFont_SetPos(font, 0, 0);
    SysFont_SetHAlign(font, 0, 256);
    SysFont_SetVAlign(font, 0, 192);
    SysFont_DrawCurrentToScreen(font, screen, chars, 0);
}

// Nonmatching: control flow differs
static void OtosuMenu_FontList_SetupScreens(OtosuMenuObj* menuObj) {
    u8*   fontPalette;
    u8*   fontPalette2;
    s32   offset;
    s32   offset2;
    u16   i;
    u16   j;
    void* pack;
    void* color;
    void* pack2;
    void* color2;
    void* subScreen;
    void* mainScreen;
    void* subPalette;
    void* subChars;
    void* mainPalette;
    void* mainChars;

    OtosuMenu_ReleaseScreens(menuObj);
    menuObj->packs[0] = DatMgr_LoadPackEntry(1, 0, 0, &OtosuMenu_BinId, 1, 0);
    Display_SetMainLayers(LAYER_BG0);
    Display_SetSubLayers(LAYER_BG3);
    fontPalette = SysFont_GetAllocPal(0);
    subPalette  = Data_GetPackEntryData(menuObj->packs[0], 4);
    i           = 0;
    do {
        offset                      = i * 2;
        color                       = subPalette + (i * 2);
        *(u16*)((u8*)color + 0x120) = *(u16*)(fontPalette + offset);
        i += 1;
    } while ((u32)i < 0x10U);
    Mem_Free(&gDebugHeap, fontPalette);
    DC_PurgeAll();
    menuObj->subPalette = PaletteMgr_AllocPalette(g_PaletteManagers[1], subPalette, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[1], menuObj->subPalette);
    subChars                  = Data_GetPackEntryData(menuObj->packs[0], 5);
    menuObj->subChars[3]      = BgResMgr_AllocChar32(g_BgResourceManagers[1], subChars,
                                                     g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, 0x6000);
    pack                      = menuObj->packs[0];
    subScreen                 = Data_GetPackEntryData(pack, 6);
    menuObj->subScreens[3][0] = (void*)(subScreen + 4);
    OtosuMenu_FontList_DrawSample(menuObj, subChars + 4, menuObj->subScreens[3]);
    func_0200d1d8(&menuObj->subMaps[3], 1, 3, 0, menuObj->subScreens[3], 1, 1);
    pack2        = menuObj->packs[0];
    fontPalette2 = SysFont_GetAllocPal(0);
    mainPalette  = Data_GetPackEntryData(pack2, 3);
    j            = 0;
    do {
        offset2 = j * 2;
        color2  = mainPalette + (j * 2);
        j += 1;
        *(u16*)((u8*)color2 + 0x120) = *(u16*)(fontPalette2 + offset2);
    } while ((u32)j < 0x10U);
    Mem_Free(&gDebugHeap, fontPalette2);
    DC_PurgeAll();
    menuObj->mainPalette = PaletteMgr_AllocPalette(g_PaletteManagers[0], mainPalette, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[0], menuObj->mainPalette);
    mainChars = Data_GetPackEntryData(menuObj->packs[0], 7);
    if (menuObj->packs[0] == NULL) {
        mainScreen = NULL;
    } else {
        mainScreen = Data_GetPackEntryData(menuObj->packs[0], 8);
    }
    menuObj->mainScreens[0][0] = (void*)(mainScreen + 4);
    menuObj->mainChars[0]      = BgResMgr_AllocChar32(g_BgResourceManagers[0], mainChars,
                                                      g_DisplaySettings.engineState[0].bgSettings[0].charBase, 0, 0x6000);
    OtosuMenu_FontList_DrawPageInfo(menuObj, mainChars + 4, menuObj->mainScreens[0]);
    func_0200d1d8(&menuObj->mainMaps[0], 0, 0, 0, menuObj->mainScreens[0], 1, 1);
    Display_Commit();
}

static void OtosuMenu_FontList_Redraw(void* menuObj) {
    OtosuMenu_FontList_SetupScreens(menuObj);
}

static void OtosuMenu_FontList_ApplyColor(OtosuMenuObj* menuObj) {
    u8 colors[3] = {0x0E, 0x0C, 0x0A};

    SysFont_SetColor(&menuObj->font, colors[menuObj->unk_474CC]);
}

static void OtosuMenu_FontList_ApplyFont(OtosuMenuObj* menuObj) {
    void* fonts[3];

    fonts[0]              = &menuObj->font;
    menuObj->fontListFont = fonts[menuObj->unk_474C8];
}

static PrcStepResult OtosuMenu_FontList_Step_Setup(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->fontListFont = &menuObj->font;
    SysFont_SetColor(&menuObj->font, 14);

    Display_SetMainLayers(LAYER_BG0);
    Display_SetSubLayers(LAYER_BG2);
    Display_Commit();
    OtosuMenu_FontList_ApplyFont(menuObj);
    OtosuMenu_FontList_SetupScreens(menuObj);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP cycles through 9 sample pages instead of 14
static PrcStepResult OtosuPrcStep_CheckButtonInput(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    BOOL buttonPressed = FALSE;

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) {
        buttonPressed = TRUE;
        menuObj->unk_474CA++;
        menuObj->unk_474CA %= (u16)14;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_B) {
        if (menuObj->unk_474CA == 0) {
            menuObj->unk_474CA = 13;
        } else {
            menuObj->unk_474CA--;
        }
        buttonPressed = TRUE;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_LEFT) {
        if (menuObj->unk_474C8 == 0) {
            menuObj->unk_474C8 = 0;
        } else {
            menuObj->unk_474C8--;
        }
        buttonPressed = TRUE;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_RIGHT) {
        if (menuObj->unk_474C8 == 0) {
            menuObj->unk_474C8 = 0;
        } else {
            menuObj->unk_474C8++;
        }
        buttonPressed = TRUE;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_L) {
        if (menuObj->unk_474CC == 0) {
            menuObj->unk_474CC = 2;
        } else {
            menuObj->unk_474CC--;
        }
        buttonPressed = TRUE;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_R) {
        if (menuObj->unk_474CC == 2) {
            menuObj->unk_474CC = 0;
        } else {
            menuObj->unk_474CC++;
        }
        buttonPressed = TRUE;
    }

    if (buttonPressed) {
        OtosuMenu_FontList_ApplyColor(menuObj);
        OtosuMenu_FontList_ApplyFont(menuObj);
        OtosuMenu_FontList_Redraw(menuObj);
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_START) {
        PrcCtx_AdvanceStep(ctx);
    }

    return PRC_STEP_CONTINUE;
}
