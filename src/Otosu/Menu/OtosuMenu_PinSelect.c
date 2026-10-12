#include "Otosu/OtosuMenu.h"

typedef struct {
    u16 data[18][2];
} Ov002_U16_18x2;

typedef struct {
    u16 data[27];
} Ov002_U16_27;

/** @brief Per-player palette entries for the stage-select screen, and the file holding the board maps. */
typedef struct {
    /* 0x0 */ u16           palettes[4]; // pack entry for each player's palette
    /* 0x8 */ BinIdentifier mapFile;
} OtosuMenuBoardFiles;

static PrcStepResult OtosuMenu_StageSelect_Step_DrawPlayerNames(PrcCtx* ctx, void* object);
static void          OtosuMenu_StageSelect_Load(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_StageSelect_Destroy(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_StageSelect_Update(PrcCtx* ctx, OtosuMenuObj* menuObj);
static void          OtosuMenu_StageSelect_Render(void);
static PrcStepResult OtosuMenu_StageSelect_Step_InitDisplay(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_StageSelect_Step_StartGame(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_InitHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_StartHost(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_SetupHostWaitScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_HostWaitGuests(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_SetupHostScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_HostSelect(PrcCtx* ctx, void* object);
static s32           OtosuMenu_StageSelect_Step_SetupGuestWaitScreen(void* arg0, OtosuMenuObj* menuObj);
static s32           OtosuMenu_StageSelect_Step_ConnectToHost(void* arg0, OtosuMenuObj* menuObj);
static s32           OtosuMenu_StageSelect_Step_InitGuestWait(PrcCtx* ctx, OtosuMenuObj* menuObj);
static s32           OtosuMenu_StageSelect_Step_GuestWait(void* arg0, OtosuMenuObj* menuObj);
static PrcStepResult OtosuMenu_StageSelect_Step_SetupGuestScreen(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_FollowHostSelection(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_LeaveSession(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_StageSelect_Step_InitSendStart(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_SendStart(PrcCtx* ctx, void* object);
static PrcStepResult OtosuMenu_StageSelect_Step_StopHost(PrcCtx* ctx, void* object);

static PrcStepFn OtosuMenu_StageSelect_GuestStepTable[] = {
    OtosuMenu_StageSelect_Step_InitDisplay,
    OtosuPrcStep_LoadPlayers,
    (PrcStepFn)OtosuMenu_StageSelect_Step_SetupGuestWaitScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    (PrcStepFn)OtosuMenu_StageSelect_Step_ConnectToHost,
    (PrcStepFn)OtosuMenu_StageSelect_Step_InitGuestWait,
    OtosuMenu_StageSelect_Step_DrawPlayerNames,
    (PrcStepFn)OtosuMenu_StageSelect_Step_GuestWait,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_StageSelect_Step_SetupGuestScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_StageSelect_Step_FollowHostSelection,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_StageSelect_Step_LeaveSession,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_StageSelect_Step_StartGame,
    PrcStep_PopFrame,
};

static const OtosuMenuRectList10 OtosuMenu_StageSelect_Buttons[2] = {
    {{
        {0, 16, 88, 80, 112},
        {1, 16, 120, 80, 144},
        {2, 16, 152, 80, 176},
        {3, 96, 88, 160, 112},
        {4, 96, 120, 160, 144},
        {5, 96, 152, 160, 176},
        {6, 176, 88, 240, 112},
        {7, 176, 120, 240, 144},
        {8, 176, 152, 240, 176},
        {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    }},
    {{
        {0, 16, 88, 80, 112},
        {1, 16, 120, 80, 144},
        {2, 16, 152, 80, 176},
        {3, 96, 88, 160, 112},
        {4, 96, 120, 160, 144},
        {5, 96, 152, 160, 176},
        {6, 176, 88, 240, 112},
        {7, 176, 120, 240, 144},
        {8, 176, 152, 240, 176},
        {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    }},
};

static PrcStepFn OtosuMenu_StageSelect_HostStepTable[] = {
    OtosuMenu_StageSelect_Step_InitDisplay,
    OtosuPrcStep_LoadPlayers,
    OtosuMenu_StageSelect_Step_InitHost,
    OtosuMenu_StageSelect_Step_StartHost,
    OtosuMenu_StageSelect_Step_SetupHostWaitScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuMenu_StageSelect_Step_DrawPlayerNames,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_StageSelect_Step_HostWaitGuests,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    PrcStep_StartDelay,
    PrcStep_TickDelay,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_StageSelect_Step_SetupHostScreen,
    OtosuPrcStep_FadeStart_Neutral,
    OtosuPrcStep_FadeWait_Neutral,
    OtosuMenu_StageSelect_Step_HostSelect,
    OtosuPrcStep_FadeStart_BrightImmediate,
    OtosuPrcStep_FadeWait_Immediate,
    OtosuMenu_StageSelect_Step_InitSendStart,
    OtosuMenu_StageSelect_Step_SendStart,
    OtosuMenu_StageSelect_Step_StopHost,
    OtosuPrcStep_SavePlayers,
    OtosuMenu_StageSelect_Step_StartGame,
    PrcStep_PopFrame,
};

static const s32 OtosuMenu_MapExtra2Sizes[0xA] = {0, 0, 0, 0, 0x38, 0, 0, 0, 0, 0};                            /* const */

static const s32 OtosuMenu_MapHeaderSizes[0xA] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}; /* const */

PrcFrameDesc OtosuMenu_StageSelect_HostFrameDesc = {
    .enter     = OtosuMenu_StageSelect_Load,
    .stepTable = OtosuMenu_StageSelect_HostStepTable,
    .update    = OtosuMenu_StageSelect_Update,
    .render    = OtosuMenu_StageSelect_Render,
    .exit      = OtosuMenu_StageSelect_Destroy,
};
static const s32 OtosuMenu_MapExtra1Offsets[0xA] = {
    0x1398, 0x2730, 0x3AC8, 0x4E60, 0x6210, 0x75F0, 0x8998, 0x9D30, 0xB0C8, 0xC460,
}; /* const */
OtosuMenuGuestPacket OtosuMenu_GuestPacket;
static const s32     OtosuMenu_MapCellsOffsets[0xA] = {
    0x10, 0x13A8, 0x2740, 0x3AD8, 0x4E88, 0x6268, 0x7610, 0x89A8, 0x9D40, 0xB0D8,
}; /* const */
static const s32                 OtosuMenu_MapExtra1Sizes[0xA] = {0, 0, 0, 0x18, 0x10, 0x10, 0, 0, 0, 0}; /* const */
static const OtosuMenuBoardFiles OtosuMenu_BoardFiles          = {
    {1, 2, 3, 4},
    {0x27, "data/BeBadge_MapData.bin"}
};
PrcFrameDesc OtosuMenu_StageSelect_GuestFrameDesc = {
    .enter     = OtosuMenu_StageSelect_Load,
    .stepTable = OtosuMenu_StageSelect_GuestStepTable,
    .update    = OtosuMenu_StageSelect_Update,
    .render    = OtosuMenu_StageSelect_Render,
    .exit      = OtosuMenu_StageSelect_Destroy,
};
static const Ov002_U16_27 OtosuMenu_StageSelect_HostCursors = {
    3,    7, 0x55, 3,    7, 0x75, 3,    7, 0x95, 3,    0x57, 0x55, 3,    0x57,
    0x75, 3, 0x57, 0x95, 3, 0xA7, 0x55, 3, 0xA7, 0x75, 3,    0xA7, 0x95,
}; /* const */
static const OtosuMenuRectList2 OtosuMenu_StageSelect_GuestTexts = {
    {
     {0x23FA, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 OtosuMenu_StageSelect_HostWaitTexts = {
    {
     {0x23F8, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 OtosuMenu_StageSelect_GuestWaitTexts = {
    {
     {0x23F8, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const OtosuMenuRectList2 OtosuMenu_StageSelect_HostTexts = {
    {
     {0x23F9, 0x0008, 0x0008, 0x00F8, 0x0040},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000},
     }
};
static const Ov002_U16_27 OtosuMenu_StageSelect_GuestCursors = {
    3,    7, 0x55, 3,    7, 0x75, 3,    7, 0x95, 3,    0x57, 0x55, 3,    0x57,
    0x75, 3, 0x57, 0x95, 3, 0xA7, 0x55, 3, 0xA7, 0x75, 3,    0xA7, 0x95,
}; /* const */
static const s32 OtosuMenu_MapCellsSizes[0xA] = {
    0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388,
}; /* const */
static const s32 OtosuMenu_MapExtra2Offsets[0xA] = {
    0x1398, 0x2730, 0x3AC8, 0x4E78, 0x6220, 0x7600, 0x8998, 0x9D30, 0xB0C8, 0xC460,
}; /* const */
static const s32            OtosuMenu_MapHeaderOffsets[0xA] = {0,      0x1398, 0x2730, 0x3AC8, 0x4E78,
                                                               0x6258, 0x7600, 0x8998, 0x9D30, 0xB0C8}; /* const */
OtosuMenuHostPacket         OtosuMenu_HostPacket;
static const Ov002_U16_18x2 OtosuMenu_StageSelect_PaletteAnim = {
    {
     {0x0000, 0x0032},
     {0x0001, 0x0002},
     {0x0002, 0x0004},
     {0x0003, 0x0004},
     {0x0004, 0x0006},
     {0x0005, 0x0006},
     {0x0006, 0x0008},
     {0x0007, 0x0008},
     {0x0008, 0x0008},
     {0x0009, 0x0006},
     {0x0008, 0x0006},
     {0x0007, 0x0005},
     {0x0006, 0x0004},
     {0x0005, 0x0004},
     {0x0004, 0x0003},
     {0x0003, 0x0002},
     {0x0002, 0x0002},
     {0x0001, 0x0001},
     }
}; /* const */

static PrcStepResult OtosuMenu_StageSelect_Step_DrawPlayerNames(PrcCtx* ctx, void* object) {
    OtosuMenu_DrawPlayerNames(object);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_StageSelect_Load(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    menuObj->commErrorScreen   = 1;
    menuObj->paletteAnimActive = 0;
    menuObj->cursor.posX       = 0;
    menuObj->cursor.posY       = 0xC8;
}

static void OtosuMenu_StageSelect_Destroy(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    menuObj->paletteAnimActive = 0;
    OtosuMenu_ReleaseScreens(menuObj);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
}

// Nonmatching: control flow differs
static void OtosuMenu_StageSelect_AnimatePalette(OtosuMenuObj* menuObj) {
    Ov002_U16_18x2 frames = OtosuMenu_StageSelect_PaletteAnim;
    u16            palettes[4];
    u16            paletteEntry;
    void*          pack;
    void*          palette;
    void*          animColors;

    if (menuObj->paletteAnimTimer == 0) {
        menuObj->paletteAnimFrame++;
        menuObj->paletteAnimFrame %= 0x12U;
        menuObj->paletteAnimTimer = frames.data[menuObj->paletteAnimFrame][1];
    } else {
        menuObj->paletteAnimTimer--;
        return;
    }
    if (menuObj->paletteAnimActive == 0) {
        return;
    }
    pack         = menuObj->packs[1];
    palettes[0]  = OtosuMenu_BoardFiles.palettes[0];
    palettes[1]  = OtosuMenu_BoardFiles.palettes[1];
    palettes[2]  = OtosuMenu_BoardFiles.palettes[2];
    palettes[3]  = OtosuMenu_BoardFiles.palettes[3];
    paletteEntry = palettes[gSaveData.otosuGameKey];
    if ((pack == NULL) || ((s32)paletteEntry <= 0)) {
        palette = NULL;
    } else {
        palette = Data_GetPackEntryData((Data*)pack, paletteEntry);
    }
    if (pack == NULL) {
        animColors = NULL;
    } else {
        animColors = Data_GetPackEntryData((Data*)pack, 5);
    }
    MI_CpuCopyU16((s32)(animColors + (frames.data[menuObj->paletteAnimFrame][0] << 5)), palette + 0x20, 0x20);
    if (menuObj->subPalette != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[1], menuObj->subPalette);
    }
    menuObj->subPalette = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[1], menuObj->subPalette);
}

static void OtosuMenu_StageSelect_Update(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    if (menuObj->paletteAnimActive == 0) {
        return;
    }
    OtosuMenu_StageSelect_AnimatePalette(menuObj);
}

static void OtosuMenu_StageSelect_Render(void) {}

// Nonmatching: instruction selection differs
static void OtosuMenu_LoadBoardMap(OtosuMenuObj* menuObj, s32 board) {
    BinIdentifier binId;
    s32           path;
    s32           binIdValue;
    s32           cellsSize;
    s32           cellsData;
    s32           extra1Size;
    s32           extra2Size;
    Data*         data;

    binIdValue = OtosuMenu_BoardFiles.mapFile.id;
    path       = OtosuMenu_BoardFiles.mapFile.path;
    binId.id   = (u32)binIdValue;
    binId.path = (char*)path;
    data       = DatMgr_LoadRawDataWithOffset(1, &menuObj->mapHeader, OtosuMenu_MapHeaderSizes[board], &binId,
                                              OtosuMenu_MapHeaderOffsets[board]);
    DatMgr_ReleaseData(data);
    if (OtosuMenu_MapCellsSizes[board] != 0) {
        if (menuObj->mapCells != NULL) {
            Mem_Free(&menuObj->heap, menuObj->mapCells);
        }
        cellsSize         = OtosuMenu_MapCellsSizes[board];
        menuObj->mapCells = Mem_AllocHeapTail(&menuObj->heap, cellsSize);
        cellsData = DatMgr_LoadRawDataWithOffset(1, menuObj->mapCells, cellsSize, &binId, OtosuMenu_MapCellsOffsets[board]);
        if (menuObj->mapCells == NULL) {
            OS_WaitForever();
        }
        DatMgr_ReleaseData(cellsData);
    } else {
        menuObj->mapCells = NULL;
    }
    if (OtosuMenu_MapExtra1Sizes[board] != 0) {
        if (menuObj->mapExtra1 != NULL) {
            Mem_Free(&menuObj->heap, menuObj->mapExtra1);
        }
        extra1Size         = OtosuMenu_MapExtra1Sizes[board];
        menuObj->mapExtra1 = Mem_AllocHeapTail(&menuObj->heap, extra1Size);
        data = DatMgr_LoadRawDataWithOffset(1, menuObj->mapExtra1, extra1Size, &binId, OtosuMenu_MapExtra1Offsets[board]);
        DatMgr_ReleaseData(data);
    } else {
        menuObj->mapExtra1 = NULL;
    }
    if (OtosuMenu_MapExtra2Sizes[board] != 0) {
        if (menuObj->mapExtra2 != NULL) {
            Mem_Free(&menuObj->heap, menuObj->mapExtra2);
        }
        extra2Size         = OtosuMenu_MapExtra2Sizes[board];
        menuObj->mapExtra2 = Mem_AllocHeapTail(&menuObj->heap, extra2Size);
        data = DatMgr_LoadRawDataWithOffset(1, menuObj->mapExtra2, extra2Size, &binId, OtosuMenu_MapExtra2Offsets[board]);
        DatMgr_ReleaseData(data);
        return;
    }
    menuObj->mapExtra2 = NULL;
}

// Nonmatching: instruction selection differs
static void OtosuMenu_DrawBoardPreview(void* menuObj, s32 screen) {
    typedef struct {
        u8  unk0;
        u8  unk1;
        u8  width;
        u8  height;
        u8  pad_4[0x10 - 4];
        u8* cells;
    } BoardMapView;

    s32           rowStart;
    s32           top;
    s32           row;
    s32           rows;
    s32           left;
    s16           tile;
    s32           rowCount;
    s32           cols;
    s32           nextRow;
    s32           col;
    s32           x;
    s32           y;
    s32           cellY0;
    s32           cellY1;
    s32           cellX0;
    s32           cellX1;
    u16           bit;
    u32           padY;
    u32           innerHeight;
    u32           padX;
    u32           innerWidth;
    u8            cell;
    u8            height;
    u8            width;
    BoardMapView* map;
    u8*           tilePtr;

    map         = (BoardMapView*)((u8*)menuObj + 0x46304);
    width       = map->width;
    height      = map->height;
    row         = 0;
    padX        = 0x28 - (width - 0xA);
    padY        = 0x28 - (height - 0xA);
    innerHeight = height - 9;
    rowCount    = (s32)(innerHeight + (innerHeight >> 0x1F)) >> 1;
    innerWidth  = width - 9;
    rows        = rowCount;
    left        = ((s32)(padX + (padX >> 0x1F)) >> 1) + 8;
    top         = ((s32)(padY + (padY >> 0x1F)) >> 1) + 2;
    cols        = (s32)(innerWidth + (innerWidth >> 0x1F)) >> 1;
    if (rowCount <= 0) {
        return;
    }
    cellY0 = 5;
    cellY1 = 7;
    do {
        col = 0;
        if (cols > 0) {
            cellX0   = 5;
            cellX1   = 7;
            rowStart = screen + (left * 2) + (((top + row) << 5) * 2);
            do {
                tile = 0x2000;
                bit  = 1;
                y    = cellY0;
                if (cellY0 < cellY1) {
                    do {
                        x = cellX0;
                        if (cellX0 < cellX1) {
                            do {
                                cell = *(map->cells + (y * map->width * 2) + (x * 2));
                                x += 1;
                                if (cell != 0) {
                                    tile |= bit;
                                }
                                bit *= 2;
                            } while (x < cellX1);
                        }
                        y += 1;
                    } while (y < cellY1);
                }
                cellX0 += 2;
                tilePtr = (u8*)(rowStart + (col * 2));
                col += 1;
                *(u16*)(tilePtr + 4) = tile;
                cellX1 += 2;
            } while (col < cols);
        }
        cellY0 += 2;
        nextRow = row + 1;
        row     = nextRow;
        cellY1 += 2;
    } while (nextRow < rows);
}

// Nonmatching: scheduling and register allocation
static void OtosuMenu_StageSelect_ShowBoardPreview(OtosuMenuObj* menuObj) {
    u16 index = menuObj->board;
    if (index == 0xFFFF) {
        func_0203aafc(3, G2S_GetBG0ScrPtr(), 0, 0x1000);
        return;
    }
    OtosuMenu_LoadBoardMap(menuObj, index);
    if (menuObj->mapCells == NULL) {
        OS_WaitForever();
    }
    menuObj->subScreens[0][0] = menuObj->tilemap;
    OtosuMenu_DrawBoardPreview(menuObj, menuObj->subScreens[0][0]);
    DC_PurgeAll();
    func_0203ab7c(3, menuObj->tilemap, G2S_GetBG0ScrPtr(), 0x1000);
}

// Nonmatching: scheduling and register allocation
static PrcStepResult OtosuMenu_StageSelect_Step_InitDisplay(PrcCtx* ctx, void* unused) {
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 12;
    g_DisplaySettings.engineState[0].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[0].windowOutside        = 31;
    g_DisplaySettings.engineState[0].windowOutsideEffects = TRUE;
    g_DisplaySettings.engineState[0].blendMode            = 1;
    g_DisplaySettings.engineState[0].blendLayer0          = 4;
    g_DisplaySettings.engineState[0].blendLayer1          = 8;
    g_DisplaySettings.engineState[0].blendCoeff0          = 12;
    g_DisplaySettings.engineState[0].blendCoeff1          = 4;
    g_DisplaySettings.engineState[1].blendMode            = 1;
    g_DisplaySettings.engineState[1].windowObj            = 12;
    g_DisplaySettings.engineState[1].windowObjEffects     = TRUE;
    g_DisplaySettings.engineState[1].blendLayer0          = 4;
    g_DisplaySettings.engineState[1].blendLayer1          = 8;
    g_DisplaySettings.engineState[1].windowOutside        = 31;
    g_DisplaySettings.engineState[1].windowOutsideEffects = FALSE;
    g_DisplaySettings.engineState[1].blendCoeff0          = 12;
    g_DisplaySettings.engineState[1].blendCoeff1          = 4;

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 5, 0, 0x4014);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 0, 0x4204);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 4, 1, 1, 0x4404);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 6, 3, 1, 0x460c);

    Display_Commit();
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_StageSelect_Step_StartGame(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->nextScene      = 2;
    menuObj->wirelessLoaded = 0;
    return PRC_STEP_CONTINUE;
}

static void OtosuMenu_StageSelect_OnSendDone(void* arg, OtosuMenuObj* menuObj) {
    menuObj->sendBusy = 0;
}

static s32 OtosuMenu_StageSelect_OnGuestConnected(OtosuMenuChildInfo* guest, OtosuMenuObj* menuObj) {
    if (guest->aid >= 4) {
        return FALSE;
    }
    MI_CpuCopyU8(guest->name, menuObj->players[guest->player].name, 0x16);
    MI_CpuCopyU8(guest->bssid, menuObj->players[guest->player].bssid, 6);
    menuObj->playerMask |= 1 << guest->aid;
    menuObj->playerDirtyMask |= 1 << guest->aid;
    menuObj->guestAckMask[guest->aid] = 0;
    return TRUE;
}

static void OtosuMenu_StageSelect_OnGuestDisconnected(OtosuMenuChildInfo* guest, OtosuMenuObj* menuObj) {
    if (guest->aid < 4) {
        menuObj->playerMask &= ~(1 << guest->aid);
        menuObj->playerDirtyMask |= 1 << guest->aid;
    }
}

static void OtosuMenu_StageSelect_OnBeaconUpdated(void* arg, OtosuMenuObj* menuObj) {
    menuObj->beaconUpdateBusy = 0;
}

static void OtosuMenu_StageSelect_OnGuestPacket(s32 guest, void* data, void* size, OtosuMenuObj* menuObj) {
    OtosuMenuGuestPacket* packet = data;

    if (data == NULL) {
        return;
    }
    if (size != (void*)0xA) {
        return;
    }
    if (menuObj == NULL) {
        return;
    }
    menuObj->guestAckMask[guest]    = packet->playerMask;
    menuObj->guestAckVersion[guest] = packet->rosterVersion;
}

static void OtosuMenu_StageSelect_RefreshRoster(OtosuMenuObj* menuObj) {
    menuObj->beaconUpdatePending = 1;
    menuObj->rosterChanged       = 1;
    u32 irqState                 = OS_DisableIRQ();
    menuObj->playerDirtyMask     = 0;
    OS_RestoreIRQ(irqState);
}

static void OtosuMenu_StageSelect_UpdateBeacon(OtosuMenuObj* menuObj) {
    s32 acceptNew;

    menuObj->beaconUpdatePending = 0;
    menuObj->beacon.playerCount  = func_02047e84(menuObj->playerMask);
    acceptNew                    = 1;
    menuObj->beaconUpdateBusy    = 1;
    if (menuObj->acceptingGuests != 0) {
        if ((u32)menuObj->beacon.playerCount >= 4U) {
            acceptNew = 0;
        }
    } else {
        acceptNew = 0;
    }
    if (func_ov040_0209d788(OtosuMenu_StageSelect_OnBeaconUpdated, menuObj, &menuObj->beacon, 0x1E, acceptNew) == 0) {
        menuObj->beaconUpdateBusy = 0;
    }
    OtosuMenu_DrawPlayerNames(menuObj);
}

static PrcStepResult OtosuMenu_StageSelect_Step_InitHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->playerMask          = 0;
    menuObj->playerDirtyMask     = 0;
    menuObj->beaconUpdateBusy    = 0;
    menuObj->beaconUpdatePending = 0;
    menuObj->acceptingGuests     = 1;
    menuObj->sendBusy            = 0;
    menuObj->rosterChanged       = 0;
    menuObj->rosterVersion       = 0;
    MI_CpuSet(menuObj->guestAckMask, 0, 8);
    MI_CpuSet(menuObj->guestAckVersion, 0, 8);
    menuObj->syncState          = 0;
    menuObj->beacon.playerCount = 1;
    menuObj->playerMask         = 1;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: instruction scheduling
static PrcStepResult OtosuMenu_StageSelect_Step_StartHost(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    s32 temp_r4;
    s32 temp_r5;
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 0:
        case 2:
        case 3:
        case 5:
        case 6:
            break;
        default:
            break;
        case 1:
            func_ov040_0209cb9c();
            break;
        case 7:
            temp_r5 = func_ov040_0209cde4();
            temp_r4 = func_020442f8();
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(1);
            func_ov040_0209cb08(&menuObj->beacon, 0x1E);
            func_ov040_0209d40c(OtosuMenu_StageSelect_OnGuestConnected, menuObj);
            func_ov040_0209d420(OtosuMenu_StageSelect_OnGuestDisconnected, menuObj);
            func_ov040_0209d434(OtosuMenu_StageSelect_OnGuestPacket, menuObj);
            func_ov040_0209d0a8(0, temp_r4, temp_r5);
            break;
        case 4:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_StageSelect_Step_SetupHostWaitScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList2 texts = OtosuMenu_StageSelect_HostWaitTexts;

    PrcCtx_Init(&menuObj->objCtx[4], "OtosuMenu_Icon2", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[4], &OtosuMenu_Icon2_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    OtosuMenu_SetupScreens(menuObj, 0xFFFF, 0x16, texts.rects);
    menuObj->unk_474C8 = 600;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_StageSelect_Step_HostWaitGuests(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    s32 irq;
    s32 sendPacket;
    s32 i;
    s32 j;
    u16 mask;
    u32 n;

    sendPacket = 0;
    func_ov002_020824a0();

    if (menuObj->playerDirtyMask != 0) {
        OtosuMenu_StageSelect_RefreshRoster(menuObj);
    }
    if ((menuObj->beaconUpdatePending != 0) && (menuObj->beaconUpdateBusy == 0)) {
        OtosuMenu_StageSelect_UpdateBeacon(menuObj);
    }
    if ((menuObj->playerMask != 1) && (menuObj->sendBusy == 0)) {
        if (menuObj->rosterChanged != 0) {
            menuObj->rosterChanged = 0;
            sendPacket             = 1;
            menuObj->rosterVersion++;
        } else {
            i = 1;
        loop_16:
            if (i < 4) {
                mask = menuObj->playerMask;
                if (mask & (1 << i)) {
                    if (mask != menuObj->guestAckMask[i]) {
                        sendPacket = 1;
                    } else if (menuObj->rosterVersion != menuObj->guestAckVersion[i]) {
                        sendPacket = 1;
                    } else {
                        goto block_15;
                    }
                } else {
                block_15:
                    i += 1;
                    goto loop_16;
                }
            }
            if ((sendPacket == 0) && (menuObj->beacon.playerCount == gSaveData.otosuPlayerCount)) {
                menuObj->syncState = 4U;
                sendPacket         = 1;
            }
        }
    }
    if (sendPacket != 0) {
        irq                            = OS_DisableIRQ();
        OtosuMenu_HostPacket.syncState = menuObj->syncState;
        if (menuObj->syncState != 0) {

        } else {
            j                                  = 0;
            OtosuMenu_HostPacket.playerMask    = menuObj->playerMask;
            OtosuMenu_HostPacket.rosterVersion = menuObj->rosterVersion;
        loop_31:
            if (j < 4) {
                if (menuObj->playerMask & (1 << j)) {
                    if (j == 0) {
                        n = 0;
                    loop_28:
                        if (n < 0xBU) {
                            n = (u32)(u16)(n + 1);
                            goto loop_28;
                        }
                    }
                    MI_CpuCopyU8(menuObj->players[j].name, OtosuMenu_HostPacket.names[j], 0x16);
                }
                j += 1;
                goto loop_31;
            }
        }
        OS_RestoreIRQ(irq);
        menuObj->sendBusy = 1;
        if (func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_StageSelect_OnSendDone, menuObj) == 0) {
            menuObj->sendBusy = 0;
        }
    }
    menuObj->unk_474C8--;
    if (menuObj->unk_474C8 == 0) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
        return 0;
    }
    if (menuObj->syncState == 4) {
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_StageSelect_Step_SetupHostScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList2 texts = OtosuMenu_StageSelect_HostTexts;

    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    OtosuMenu_SetupStageSelectScreens(menuObj, gSaveData.otosuGameKey, texts.rects);
    menuObj->paletteAnimActive = 1;
    PrcCtx_Init(&menuObj->objCtx[0], "OtosuMenu_DaizaObj", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[0], &OtosuMenu_Daiza_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->board     = 0xFFFF;
    menuObj->unk_474C8 = 0xFFFF;
    menuObj->unk_474CA = menuObj->playerMask;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: control flow differs
static void OtosuMenu_StageSelect_SendSelection(void* ctx, OtosuMenuObj* menuObj) {
    s32 irq;
    u16 state;

    irq                            = OS_DisableIRQ();
    OtosuMenu_HostPacket.syncState = menuObj->syncState;
    state                          = menuObj->syncState;
    switch (state) { /* irregular */
        case 5:
            OtosuMenu_HostPacket.board = menuObj->board;
            break;
        case 6:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    OS_RestoreIRQ(irq);
    func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_StageSelect_OnSendDone, menuObj);
}

// Nonmatching: control flow differs
static PrcStepResult OtosuMenu_StageSelect_Step_HostSelect(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    u16          buttons[0x32];
    Ov002_U16_27 cursors = OtosuMenu_StageSelect_HostCursors;
    u16          touched;
    u16          i;

    for (i = 0; i < 0x19 * 2; i++) {
        buttons[i] = ((const u16*)&OtosuMenu_StageSelect_Buttons[1])[i];
    }

    touched = OtosuMenu_GetTouchedButton((const OtosuMenuRect*)buttons);
    if (menuObj->unk_474CA != func_ov040_0209cb68()) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
        return 0;
    }
    if (touched == 0xFFFF) {
        return 0;
    }
    if (touched == 0xFFFE) {
        menuObj->unk_474C8   = 0xFFFF;
        menuObj->cursor.posX = 0U;
        menuObj->cursor.posY = 0xD2U;
        menuObj->syncState   = 5;
        menuObj->board       = 0xFFFFU;
        OtosuMenu_StageSelect_ShowBoardPreview(menuObj);
    } else if (menuObj->board == touched) {
        menuObj->syncState = 6;
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
    } else {
        u16 cursor = (u16)(touched * 3);

        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
        menuObj->board       = touched;
        menuObj->cursor.posX = cursors.data[cursor + 1];
        menuObj->cursor.posY = cursors.data[cursor + 2];
        Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)cursors.data[cursor],
                               menuObj->cursor.cellTable);
        menuObj->syncState = 5;
        OtosuMenu_StageSelect_ShowBoardPreview(menuObj);
    }
    OtosuMenu_StageSelect_SendSelection(ctx, menuObj);
    return PRC_STEP_CONTINUE;
}

// Nonmatching (JP): JP's host packet carries a single bssid (0x66 bytes)
static void OtosuMenu_StageSelect_OnHostPacket(s32 sender, void* data, void* size, OtosuMenuObj* menuObj) {
    OtosuMenuHostPacket* packet = data;
    s32                  i;

    if (data == NULL) {
        return;
    }
    if (size != (void*)0x78) {
        return;
    }
    if (menuObj == NULL) {
        return;
    }
    menuObj->syncState = packet->syncState;
    switch (packet->syncState) {
        case 3:
        case 4:
            return;
        case 0:
            for (i = 0; i < 4; i++) {
                if (packet->playerMask & (1 << i)) {
                    MI_CpuCopyU8(packet->names[i], menuObj->players[i].name, 0x16);
                }
            }
            menuObj->playerMask      = packet->playerMask;
            menuObj->playerDirtyMask = 0xF;
            menuObj->rosterVersion   = packet->rosterVersion;
            return;
    }
}

static s32 OtosuMenu_StageSelect_Step_SetupGuestWaitScreen(void* ctx, OtosuMenuObj* menuObj) {
    OtosuMenuRectList2 texts = OtosuMenu_StageSelect_GuestWaitTexts;

    PrcCtx_Init(&menuObj->objCtx[4], "OtosuMenu_Icon2", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[4], &OtosuMenu_Icon2_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    menuObj->cursor.posX = 0;
    menuObj->cursor.posY = 0xC8;
    OtosuMenu_SetupScreens(menuObj, 0xFFFF, 0x16, texts.rects);
    menuObj->unk_474C8 = 600;
    PrcCtx_AdvanceStep(ctx);
    return 0;
}

// Nonmatching: instruction scheduling
static s32 OtosuMenu_StageSelect_Step_ConnectToHost(void* ctx, OtosuMenuObj* menuObj) {
    u32 state = func_ov040_0209cb78();
    switch (state) {
        case 0:
        case 2:
        case 3:
            break;
        default:
            break;
        case 1:
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(1);
            func_ov040_0209cabc(menuObj->ownName, 0x18);
            func_ov040_0209b8b8(1, menuObj->players[0].bssid, 0);
            func_ov040_0209d434(OtosuMenu_StageSelect_OnHostPacket, menuObj);
            break;
        case 4:
            PrcCtx_AdvanceStep(ctx);
            break;
    }
    menuObj->unk_474C8--;
    if (menuObj->unk_474C8 != 0) {
        return 0;
    }
    PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
    return 0;
}

static void OtosuMenu_StageSelect_OnGuestSendDone(void* arg, OtosuMenuObj* menuObj) {
    menuObj->sendBusy = 0;
}

static s32 OtosuMenu_StageSelect_Step_InitGuestWait(PrcCtx* ctx, OtosuMenuObj* menuObj) {
    menuObj->unk_474C8 = 0xFFFF;
    menuObj->unk_474CA = 0;
    menuObj->unk_474CC = 0;
    PrcCtx_AdvanceStep(ctx);
    return 0;
}

// Nonmatching: control flow differs
static s32 OtosuMenu_StageSelect_Step_GuestWait(void* ctx, OtosuMenuObj* menuObj) {
    if (menuObj->playerDirtyMask != 0) {
        OtosuMenu_DrawPlayerNames(menuObj);
        menuObj->unk_474CC = 1U;
    }
    if ((menuObj->sendBusy == 0) && (menuObj->unk_474CC != 0)) {
        u32 irqState1                       = OS_DisableIRQ();
        OtosuMenu_GuestPacket.playerMask    = menuObj->playerMask;
        OtosuMenu_GuestPacket.rosterVersion = menuObj->rosterVersion;
        OS_RestoreIRQ(irqState1);
        menuObj->unk_474CC = 0U;
        menuObj->sendBusy  = 1;
        if (func_ov040_0209d48c(&OtosuMenu_GuestPacket, 0xA, OtosuMenu_StageSelect_OnGuestSendDone, menuObj) == 0) {
            menuObj->sendBusy = 0;
        }
    }
    if (menuObj->syncState == 4) {
        PrcCtx_AdvanceStep(ctx);
    }
    return 0;
}

// Nonmatching (JP): JP's host packet carries a single bssid (0x66 bytes)
static void OtosuMenu_StageSelect_OnHostSelection(s32 sender, void* data, void* size, OtosuMenuObj* menuObj) {
    OtosuMenuHostPacket* packet = data;

    if (data == NULL) {
        return;
    }
    if (size != (void*)0x78) {
        return;
    }
    if (menuObj == NULL) {
        return;
    }
    menuObj->syncState = packet->syncState;
    menuObj->board     = packet->board;
}

static PrcStepResult OtosuMenu_StageSelect_Step_SetupGuestScreen(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    OtosuMenuRectList2 texts = OtosuMenu_StageSelect_GuestTexts;

    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
    PrcCtx_Init(&menuObj->objCtx[0], "OtosuMenu_DaizaObj", 0x40);
    PrcCtx_ReplaceFrame(&menuObj->objCtx[0], &OtosuMenu_Daiza_FrameDesc, NULL);
    PrcMaster_RegisterContext(&menuObj->prcMaster, &menuObj->objCtx[0]);
    menuObj->paletteAnimActive = 1;
    menuObj->unk_474C8         = 0xFFFF;
    OtosuMenu_SetupStageSelectScreens(menuObj, gSaveData.otosuGameKey, texts.rects);
    menuObj->unk_474CA = menuObj->board;
    func_ov040_0209d434(OtosuMenu_StageSelect_OnHostSelection, menuObj);
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

// Nonmatching: instruction selection differs
static PrcStepResult OtosuMenu_StageSelect_Step_FollowHostSelection(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    Ov002_U16_27 cursors = OtosuMenu_StageSelect_GuestCursors;
    u16          cursor;
    u16          board;

    if (menuObj->syncState == 5) {
        board = menuObj->board;
        if (board == 0xFFFF) {
            menuObj->unk_474CA   = board;
            menuObj->cursor.posX = 0U;
            menuObj->cursor.posY = 0xD2U;
            menuObj->syncState   = 5U;
            OtosuMenu_StageSelect_ShowBoardPreview(menuObj);
        } else if (menuObj->unk_474CA != board) {
            SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_CURSOR);
            menuObj->unk_474CA   = menuObj->board;
            cursor               = (u16)(menuObj->board * 3);
            menuObj->cursor.posX = cursors.data[cursor + 1];
            menuObj->cursor.posY = cursors.data[cursor + 2];
            Sprite_ChangeAnimation(&menuObj->cursor, menuObj->cursor.animData, (s16)cursors.data[cursor],
                                   menuObj->cursor.cellTable);
            OtosuMenu_StageSelect_ShowBoardPreview(menuObj);
        }
    }
    if (menuObj->syncState == 6) {
        SndMgr_StartPlayingSE(SEIDX_SE_BAYBADGE_MENU_EXECUTE);
        PrcCtx_AdvanceStep(ctx);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_StageSelect_Step_LeaveSession(PrcCtx* ctx, void* unused) {
    switch (func_ov040_0209cb78()) {
        default:
            break;
        case 0:
            OS_WaitForever();
            break;
        case 1:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_StageSelect_Step_InitSendStart(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    menuObj->unk_474C8 = 0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

// Nonmatching: scheduling and register allocation
static PrcStepResult OtosuMenu_StageSelect_Step_SendStart(PrcCtx* ctx, void* object) {
    OtosuMenuObj* menuObj = (OtosuMenuObj*)object;

    if (func_ov040_0209cb68() == 1) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    u32 irqState2                  = OS_DisableIRQ();
    OtosuMenu_HostPacket.syncState = 6;
    OS_RestoreIRQ(irqState2);
    if (func_ov040_0209d48c(&OtosuMenu_HostPacket, 0x78, OtosuMenu_StageSelect_OnSendDone, menuObj) == 0) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
        return PRC_STEP_CONTINUE;
    }
    menuObj->unk_474C8++;
    if (menuObj->unk_474C8 == 180) {
        PrcCtx_ReplaceFrame(&menuObj->mainCtx, &OtosuMenu_CommError_ConnectionLostFrameDesc, NULL);
    }
    return PRC_STEP_CONTINUE;
}

static PrcStepResult OtosuMenu_StageSelect_Step_StopHost(PrcCtx* ctx, void* object) {
    u32 state;

    state = func_ov040_0209cb78();
    switch (state) {
        case 0:
            OS_WaitForever();
            break;
        case 1:
            PrcCtx_AdvanceStep(ctx);
            break;
        case 3:
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
        case 2:
        default:
            func_ov040_0209d588();
            break;
    }
    return PRC_STEP_CONTINUE;
}
