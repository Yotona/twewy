#include "Otosu/OtosuMenu.h"

static PrcStepResult    OtosuMenu_SaveData_Step_ShowSaveFailure(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_HangSaveFailure(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowLoadFailure(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_HangLoadFailure(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowCorrupted(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowSaveIcon(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_WriteSave(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowCorruptDeleted(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_WaitDone(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_Finish(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowInitializing(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowInitialized(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowDeleteConfirm1(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_DeleteConfirmChoice(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowDeleteConfirm2(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowDeleteConfirm3(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ResetSavePipeline(PrcCtx* ctx, void*);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowDeleting(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_ShowDeleted(PrcCtx* ctx, void* object);
static PrcStepResult    OtosuMenu_SaveData_Step_WaitDeleteDone(PrcCtx* ctx, void* object);
static SpriteFrameInfo* OtosuMenu_SaveData_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static void             OtosuMenu_SaveData_Load(s32 ctx, OtosuMenuObj* menuObj);
static void             OtosuMenu_SaveData_Update(void);
static void             OtosuMenu_SaveData_Render(void);
static void             OtosuMenu_SaveData_Destroy(s32 ctx, OtosuMenuObj* menuObj);

static PrcStepFn OtosuMenu_SaveData_SaveFailureStepTable[] = {OtosuMenu_SaveData_Step_ShowSaveFailure,
                                                              OtosuPrcStep_FadeStart_Neutral2, OtosuPrcStep_FadeWait_Neutral2,
                                                              OtosuMenu_SaveData_Step_HangSaveFailure};

static PrcStepFn                OtosuMenu_SaveData_LoadFailureStepTable[] = {OtosuMenu_SaveData_Step_ShowLoadFailure,
                                                                             OtosuPrcStep_FadeStart_Neutral2, OtosuPrcStep_FadeWait_Neutral2,
                                                                             OtosuMenu_SaveData_Step_HangLoadFailure};
static const OtosuMenuRectList1 sNoText5                                  = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText3 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText2 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText7 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText0 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText6 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText1 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList1 sNoText4 = {
    {
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList2 sDeletingText = {
    {
     {0x2A37, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 sDeletedText = {
    {
     {0x2A38, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};

static PrcStepFn OtosuMenu_SaveData_CorruptedStepTable[] = {
    OtosuMenu_SaveData_Step_ShowCorrupted,
    OtosuPrcStep_FadeStart_Neutral2,
    OtosuPrcStep_FadeWait_Neutral2,
    OtosuMenu_SaveData_Step_ShowSaveIcon,
    OtosuMenu_SaveData_Step_WriteSave,
    OtosuPrcStep_FadeStart_DarkImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_SaveData_Step_ShowCorruptDeleted,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_SaveData_Step_WaitDone,
    OtosuPrcStep_FadeStart_Dark,
    OtosuPrcStep_FadeWait,
    OtosuMenu_SaveData_Step_Finish,
};

static PrcStepFn OtosuMenu_SaveData_InitializeStepTable[] = {
    OtosuMenu_SaveData_Step_ShowInitializing,
    OtosuPrcStep_FadeStart_Neutral2,
    OtosuPrcStep_FadeWait_Neutral2,
    OtosuMenu_SaveData_Step_ShowSaveIcon,
    OtosuMenu_SaveData_Step_WriteSave,
    OtosuPrcStep_FadeStart_DarkImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_SaveData_Step_ShowInitialized,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_SaveData_Step_WaitDone,
    OtosuPrcStep_FadeStart_Dark,
    OtosuPrcStep_FadeWait,
    OtosuMenu_SaveData_Step_Finish,
};

static PrcStepFn OtosuMenu_SaveData_DeleteStepTable[] = {
    OtosuMenu_SaveData_Step_ShowDeleteConfirm1,
    OtosuPrcStep_FadeStart_Neutral2,
    OtosuPrcStep_FadeWait_Neutral2,
    OtosuMenu_SaveData_Step_DeleteConfirmChoice,
    PrcStep_Advance,
    OtosuPrcStep_FadeStart_DarkImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_SaveData_Step_ShowDeleteConfirm2,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_SaveData_Step_DeleteConfirmChoice,
    PrcStep_Advance,
    OtosuPrcStep_FadeStart_DarkImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_SaveData_Step_ShowDeleteConfirm3,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_SaveData_Step_DeleteConfirmChoice,
    OtosuMenu_SaveData_Step_ResetSavePipeline,
    OtosuPrcStep_FadeStart_DarkImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_SaveData_Step_ShowSaveIcon,
    OtosuMenu_SaveData_Step_ShowDeleting,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_SaveData_Step_WriteSave,
    OtosuPrcStep_FadeStart_DarkImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_SaveData_Step_ShowDeleted,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_SaveData_Step_WaitDeleteDone,
    OtosuPrcStep_FadeStart_Dark,
    OtosuPrcStep_FadeWait,
    OtosuMenu_SaveData_Step_Finish,
    PrcStep_PopFrame,
};

static const SpriteAnimation OtosuMenu_SaveData_CursorAnim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 2,
    .bits_14_15        = 0,
    .unk_02            = 0x0000,
    .posX              = 0x0000,
    .posY              = 0x00C8,
    .frameInfoCallback = OtosuMenu_SaveData_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = (BinIdentifier*)&OtosuMenu_FldDownScreenBinId,
    .unk_18            = 0x0002,
    .packIndex         = 0x000C,
    .unk_1C            = 0x0002,
    .unk_1E            = 0x0000,
    .unk_20            = 0x0001,
    .unk_22            = 0x0002,
    .unk_24            = 0x0000,
    .unk_26            = 0x0003,
    .unk_28            = 0x0004,
    .animIndex         = 0x0003,
};

static PrcStepFn OtosuMenu_SaveData_CancelStepTable[] = {OtosuMenu_SaveData_Step_Finish, PrcStep_PopFrame};

PrcFrameDesc OtosuMenu_SaveData_LoadFailureFrameDesc = {
    .enter     = OtosuMenu_SaveData_Load,
    .stepTable = OtosuMenu_SaveData_LoadFailureStepTable,
    .update    = OtosuMenu_SaveData_Update,
    .render    = OtosuMenu_SaveData_Render,
    .exit      = OtosuMenu_SaveData_Destroy,
};

PrcFrameDesc OtosuMenu_SaveData_DeleteFrameDesc = {
    .enter     = OtosuMenu_SaveData_Load,
    .stepTable = OtosuMenu_SaveData_DeleteStepTable,
    .update    = OtosuMenu_SaveData_Update,
    .render    = OtosuMenu_SaveData_Render,
    .exit      = OtosuMenu_SaveData_Destroy,
};
static const OtosuMenuRectList3 sDeleteConfirm1Buttons = {
    {
     {0x2A39, 0x38, 0x51, 0xC8, 0x5B},
     {0x2A3A, 0x38, 0x65, 0xC8, 0x6F},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
PrcFrameDesc OtosuMenu_SaveData_SaveFailureFrameDesc = {
    .enter     = OtosuMenu_SaveData_Load,
    .stepTable = OtosuMenu_SaveData_SaveFailureStepTable,
    .update    = OtosuMenu_SaveData_Update,
    .render    = OtosuMenu_SaveData_Render,
    .exit      = OtosuMenu_SaveData_Destroy,
};
static const OtosuMenuRectList2 sDeleteConfirm3Text = {
    {
     {0x2A36, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 sDeleteConfirm2Text = {
    {
     {0x2A35, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 sLoadFailureText = {
    {
     {0x2A41, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 sSaveFailureText = {
    {
     {0x2A42, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 sCorruptedText = {
    {
     {0x2A3F, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 sCorruptDeletedText = {
    {
     {0x2A40, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};

static void OtosuMenu_SaveData_Load(s32 ctx, OtosuMenuObj* menuObj);
static void OtosuMenu_SaveData_Destroy(s32 ctx, OtosuMenuObj* menuObj);
static void OtosuMenu_SaveData_Update(void);
static void OtosuMenu_SaveData_Render(void);

PrcFrameDesc OtosuMenu_SaveData_CorruptedFrameDesc = {
    .enter     = OtosuMenu_SaveData_Load,
    .stepTable = OtosuMenu_SaveData_CorruptedStepTable,
    .update    = OtosuMenu_SaveData_Update,
    .render    = OtosuMenu_SaveData_Render,
    .exit      = OtosuMenu_SaveData_Destroy,
};
static const OtosuMenuRectList2 sInitializedText = {
    {
     {0x3384, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
PrcFrameDesc OtosuMenu_SaveData_InitializeFrameDesc = {
    .enter     = OtosuMenu_SaveData_Load,
    .stepTable = OtosuMenu_SaveData_InitializeStepTable,
    .update    = OtosuMenu_SaveData_Update,
    .render    = OtosuMenu_SaveData_Render,
    .exit      = OtosuMenu_SaveData_Destroy,
};
static const OtosuMenuRectList2 sInitializingText = {
    {
     {0x3383, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList3 sDeleteConfirm3Buttons = {
    {
     {0x2A3D, 0x38, 0x51, 0xC8, 0x5B},
     {0x2A3E, 0x38, 0x65, 0xC8, 0x6F},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};
static const OtosuMenuRectList2 sDeleteConfirm1Text = {
    {
     {0x2A34, 0x0010, 0x0050, 0x00F0, 0x0098},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList3 sDeleteConfirm2Buttons = {
    {
     {0x2A3B, 0x38, 0x51, 0xC8, 0x5B},
     {0x2A3C, 0x38, 0x65, 0xC8, 0x6F},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0},
     }
};

static SpriteFrameInfo* OtosuMenu_SaveData_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const OtosuMenuRectList3 sDeleteChoiceRects = {
    {
     {0, 0x38, 0x4C, 0xC8, 0x60},
     {1, 0x38, 0x61, 0xC8, 0x74},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
     }
};

static void OtosuMenu_SaveData_LoadCursor(OtosuMenuObj* menuObj) {
    SpriteAnimation anim = OtosuMenu_SaveData_CursorAnim;

    _Sprite_Load(&menuObj->cursor, &anim);
    Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, 3, menuObj->cursor.cellTable);
}

static void OtosuMenu_SaveData_Load(s32 ctx, OtosuMenuObj* menuObj) {
    PrcCtx_Init(&menuObj->objCtx[0], "DelData_WinObj", 0x100);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[0], &OtosuMenu_DelDataWin_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    Sprite_Destroy(&menuObj->cursor);
    OtosuMenu_SaveData_LoadCursor(menuObj);
}

static void OtosuMenu_SaveData_Destroy(s32 ctx, OtosuMenuObj* menuObj) {
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
}

static void OtosuMenu_SaveData_Update(void) {
    return; // do nothing
}

static void OtosuMenu_SaveData_Render(void) {
    return; // do nothing
}

static void OtosuMenu_SaveData_ReloadTextLayer(OtosuMenuObj* menuObj) {
    if (menuObj->mainChars[0] != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[0], menuObj->mainChars[0]);
        menuObj->mainChars[0] = NULL;
    }
    if (menuObj->packs[0] != NULL) {
        DatMgr_ReleaseData(menuObj->packs[0]);
        menuObj->packs[0] = NULL;
    }
    menuObj->packs[0] = DatMgr_LoadPackEntry(1, 0, 0, &OtosuMenu_BinId, 1, 0);
    void* textChars   = Data_GetPackEntryData(menuObj->packs[0], 7);

    u16 col;
    u16 row;
    u16 tile = 0;
    for (row = 0; row < 32; row++) {
        for (col = 0; col < 64; col++) {
            menuObj->tilemap[row][col] = tile | 0x1000;
            tile++;
        }
    }

    DC_PurgeAll();
    menuObj->mainScreens[0][0] = menuObj->tilemap;
    menuObj->mainChars[0]      = BgResMgr_AllocChar32(g_BgResourceManagers[0], textChars,
                                                      g_DisplaySettings.engineState[0].bgSettings[0].charBase, 0, 0x6000);
    func_0200d1d8(&menuObj->mainMaps[0], 0, 0, 0, menuObj->mainScreens[0], 1, 1);
    Display_Commit();
}

static void OtosuMenu_SaveData_SetupScreens(OtosuMenuObj* menuObj, const OtosuMenuRect* subTexts,
                                            const OtosuMenuRect* mainTexts) {
    OtosuMenu_ReleaseScreens(menuObj);
    menuObj->packs[0] = DatMgr_LoadPackEntry(1, 0, 0, &OtosuMenu_BinId, 1, 0);
    menuObj->packs[1] = DatMgr_LoadPackEntry(1, 0, 0, &OtosuMenu_DelMenuBinId, 1, 0);
    menuObj->packs[2] = DatMgr_LoadRawData(1, 0, 0, &OtosuMenu_MenuBgUpperBinId);
    menuObj->packs[3] = DatMgr_LoadRawData(1, 0, 0, &OtosuMenu_MenuBgLowerBinId);

    Display_SetMainLayers(LAYER_BG0 | LAYER_BG3 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);

    menuObj->subPalette =
        PaletteMgr_AllocPalette(g_PaletteManagers[1], Data_GetPackEntryData(menuObj->packs[1], 1), 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[1], menuObj->subPalette);

    void* chars               = Data_GetPackEntryData(menuObj->packs[2], 1);
    void* screen              = Data_GetPackEntryData(menuObj->packs[2], 2);
    menuObj->subScreens[3][0] = screen + 4;
    menuObj->subChars[3]      = BgResMgr_AllocChar32(g_BgResourceManagers[1], chars,
                                                     g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, 0x6000);
    func_0200d1d8(&menuObj->subMaps[3], 1, 3, 0, menuObj->subScreens[3], 1, 1);

    chars                     = Data_GetPackEntryData(menuObj->packs[1], 2);
    screen                    = Data_GetPackEntryData(menuObj->packs[1], 3);
    menuObj->subScreens[2][0] = screen + 4;
    OtosuMenu_DrawTexts(&menuObj->font, subTexts, chars + 4, menuObj->subScreens[2][0]);
    menuObj->subChars[2] = BgResMgr_AllocChar32(g_BgResourceManagers[1], chars,
                                                g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x4380);
    func_0200d1d8(&menuObj->subMaps[2], 1, 2, 0, menuObj->subScreens[2], 1, 1);

    menuObj->mainPalette =
        PaletteMgr_AllocPalette(g_PaletteManagers[0], Data_GetPackEntryData(menuObj->packs[1], 1), 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[0], menuObj->mainPalette);

    chars                      = Data_GetPackEntryData(menuObj->packs[3], 1);
    screen                     = Data_GetPackEntryData(menuObj->packs[3], 2);
    menuObj->mainScreens[3][0] = screen + 4;
    menuObj->mainChars[3]      = BgResMgr_AllocChar32(g_BgResourceManagers[0], chars,
                                                      g_DisplaySettings.engineState[0].bgSettings[3].charBase, 0, 0x6000);
    func_0200d1d8(&menuObj->mainMaps[3], 0, 3, 0, menuObj->mainScreens[3], 1, 1);

    void* textChars = Data_GetPackEntryData(menuObj->packs[0], 7);

    u16 col;
    u16 row;
    u16 tile = 0;
    for (row = 0; row < 32; row++) {
        for (col = 0; col < 64; col++) {
            menuObj->tilemap[row][col] = tile | 0x1000;
            tile++;
        }
    }
    DC_PurgeAll();
    menuObj->mainScreens[0][0] = menuObj->tilemap;
    menuObj->mainChars[0]      = BgResMgr_AllocChar32(g_BgResourceManagers[0], textChars,
                                                      g_DisplaySettings.engineState[0].bgSettings[0].charBase, 0, 0x6000);
    OtosuMenu_DrawTexts(&menuObj->font, mainTexts, textChars + 4, menuObj->mainScreens[0][0]);
    func_0200d1d8(&menuObj->mainMaps[0], 0, 0, 0, menuObj->mainScreens[0], 1, 1);
    Display_Commit();
}

// Nonmatching (JP): JP has no text-layer reload or cursor reset
static PrcStepResult OtosuMenu_SaveData_Step_DeleteConfirmChoice(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj    = object;
    OtosuMenuRectList3 buttons    = sDeleteChoiceRects;
    OtosuMenuCursor    cursors[2] = {
        {3, 0x80, 0x56},
        {3, 0x80, 0x6A},
    };
    u16 selected = OtosuMenu_GetTouchedButton(buttons.rects);

    if (selected == 0xFFFF) {
        return PRC_STEP_CONTINUE;
    }
    if (selected == 0xFFFE) {
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0;
        menuObj->cursor.posY = 210;
        return PRC_STEP_CONTINUE;
    }
    if (menuObj->unk_474C8 == selected) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        switch (selected) {
            case 0:
                PrcCtx_AdvanceStep(ctx);
                return PRC_STEP_CONTINUE;
            case 1:
                PrcCtx_ReplaceStepTable(ctx, OtosuMenu_SaveData_CancelStepTable);
                OtosuMenu_SaveData_ReloadTextLayer(menuObj);
                menuObj->cursor.posX = 0;
                menuObj->cursor.posY = 210;
                PrcCtx_PushStepTable(ctx, PrcSteps_FadeDark);
                return PRC_STEP_CONTINUE;
        }
    } else {
        menuObj->unk_474C8 = selected;
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        switch (selected) {
            case 0:
            case 1:
                menuObj->cursor.posX = cursors[selected].x;
                menuObj->cursor.posY = cursors[selected].y;
                Sprite_SetAnimation(&menuObj->cursor, menuObj->cursor.animData, cursors[selected].anim,
                                    menuObj->cursor.cellTable);
                return PRC_STEP_CONTINUE;
        }
    }
    return PRC_STEP_REPEAT;
}

static PrcStepResult OtosuMenu_SaveData_Step_ResetSavePipeline(PrcCtx* ctx, void*) {
    PrcCtx_AdvanceStep(ctx);
    Savefile_ResetIOPipeline();
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_ShowDeleteConfirm1(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj    = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_sp1E = sDeleteConfirm1Text;
    OtosuMenuRectList3 table_sub  = sDeleteConfirm1Buttons;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_sp1E.rects, table_sub.rects);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_ShowDeleteConfirm2(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj    = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_sp1E = sDeleteConfirm2Text;
    OtosuMenuRectList3 table_sub  = sDeleteConfirm2Buttons;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_sp1E.rects, table_sub.rects);
    menuObj->unk_474C8   = 0xFFFF;
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_ShowDeleteConfirm3(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj    = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_sp1E = sDeleteConfirm3Text;
    OtosuMenuRectList3 table_sub  = sDeleteConfirm3Buttons;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_sp1E.rects, table_sub.rects);
    menuObj->unk_474C8   = 0xFFFF;
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's text layout templates sit at other offsets
static PrcStepResult OtosuMenu_SaveData_Step_ShowDeleting(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sDeletingText;
    OtosuMenuRectList1 table_sub = sNoText0;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_ShowSaveIcon(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    PrcCtx_Init(&menuObj->objCtx[1], "OtosuMenu_Icon2", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[1], &OtosuMenu_Icon2_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_WriteSave(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    if (Savefile_RunSavePipelineStep() == 1) {
        u16 errorFlags = Savefile_GetWriteErrorFlags();
        PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[1]);
        if (errorFlags == 0) {
            SndMgr_StartPlayingSE(SEIDX_MENU_SAVE);
            PrcCtx_AdvanceStep(ctx);
        } else if (errorFlags & 2) {
            PrcCtx_ReplaceFrame(ctx, &OtosuMenu_SaveData_LoadFailureFrameDesc, 0);
        } else {
            PrcCtx_ReplaceFrame(ctx, &OtosuMenu_SaveData_SaveFailureFrameDesc, 0);
        }
    }
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's text layout templates sit at other offsets
static PrcStepResult OtosuMenu_SaveData_Step_ShowDeleted(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sDeletedText;
    OtosuMenuRectList1 table_sub = sNoText1;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xF0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_WaitDeleteDone(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    if (TouchInput_WasTouchPressed()) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    }
    if ((TouchInput_WasTouchPressed()) || (menuObj->unk_474C8 == 0)) {
        PrcCtx_AdvanceStep(ctx);
    }
    menuObj->unk_474C8--;
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's text layout templates sit at other offsets
static PrcStepResult OtosuMenu_SaveData_Step_ShowCorrupted(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sCorruptedText;
    OtosuMenuRectList1 table_sub = sNoText2;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_ShowInitializing(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sInitializingText;
    OtosuMenuRectList1 table_sub = sNoText3;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's text layout templates sit at other offsets
static PrcStepResult OtosuMenu_SaveData_Step_ShowCorruptDeleted(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sCorruptDeletedText;
    OtosuMenuRectList1 table_sub = sNoText4;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xF0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_ShowInitialized(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sInitializedText;
    OtosuMenuRectList1 table_sub = sNoText5;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xD2;
    menuObj->unk_474C8   = 0xF0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_WaitDone(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    if (TouchInput_WasTouchPressed()) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    }
    if ((TouchInput_WasTouchPressed()) || (menuObj->unk_474C8 == 0)) {
        PrcCtx_AdvanceStep(ctx);
    }
    menuObj->unk_474C8--;
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's text layout templates sit at other offsets
static PrcStepResult OtosuMenu_SaveData_Step_ShowSaveFailure(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sSaveFailureText;
    OtosuMenuRectList1 table_sub = sNoText6;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 210;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's text layout templates sit at other offsets
static PrcStepResult OtosuMenu_SaveData_Step_ShowLoadFailure(PrcCtx* ctx, void* object) {
    OtosuMenuObj*      menuObj   = (OtosuMenuObj*)object;
    OtosuMenuRectList2 table_spA = sLoadFailureText;
    OtosuMenuRectList1 table_sub = sNoText7;

    Display_SetMainLayers(LAYER_BG0 | LAYER_OBJ);
    Display_SetSubLayers(LAYER_BG2 | LAYER_OBJ);
    Display_Commit();
    OtosuMenu_SaveData_SetupScreens(menuObj, table_spA.rects, table_sub.rects);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 210;
    menuObj->unk_474C8   = 0xFFFF;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_HangLoadFailure(PrcCtx* ctx, void* object) {
    OS_WaitForever();
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_HangSaveFailure(PrcCtx* ctx, void* object) {
    OS_WaitForever();
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_SaveData_Step_Finish(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    OtosuMenu_ReleaseScreens(menuObj);
    menuObj->nextScene = 6;
    return PRC_STEP_CONTINUE;
}
