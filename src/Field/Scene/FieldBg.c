#include "Field/FieldRegion.h"

#include "Display.h"
#include "Engine/Color.h"
#include "Engine/Core/DMA.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Event/EventFlags.h"
#include "Field/Event/EventManager.h"
#include "Field/Event/EventTables.h"
#include "Field/Scene/FieldArea.h"
#include "Field/Scene/FieldBg.h"
#include "Field/Scene/FieldMusic.h"
#include "Field/Scene/FieldSceneRender.h"
#include "Field/Scene/FieldSeq.h"
#include "Field/Scene/FieldShopName.h"
#include "Field/Tasks/Fld_Badge.h"
#include "Field/Tasks/Fld_MapName.h"
#include "Field/Tasks/Fld_Partner.h"
#include "Field/Tasks/Fld_Player.h"
#include "Field/Tasks/Fld_Wall.h"
#include "Player/Inventory.h"
#include "Save.h"
#include <nitro/types.h>

extern void* BgResMgr_AllocCharExtended(BgResMgr* arg0, void* arg1, s32 arg2, u32 arg3, u32 arg4);
extern void  func_02001b44(s32, s32, void*, s32);
extern s32   func_0202599c(s32 flagIndex);
extern s32   data_02059d24;
extern s32   data_0205a124;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} FldRawEntry;

// Pack entry 4 of the 0x2A sub-screen background: 0x80-byte palettes after a header word.
typedef struct {
    /* 0x0 */ u32 unk_00;
    /* 0x4 */ u16 palettes[][0x40];
} FldBgPaletteSet;

s32 FieldBg_IsAreaTitleDone(FieldEventManager* mgr) {
    if (mgr->unk_216D0 == -1) {
        return 1;
    }
    return EasyTask_ValidateTaskId(&mgr->taskPool, &mgr->unk_216D0) == 0;
}

void FieldBg_UpdateWalkSpeed(FieldEventManager* mgr) {
    s32 id;

    if (gSaveData.fieldArea == 1) {
        id = 0x2666;
    } else {
        id = 0x2CCD;
    }
    Fld_Player_SetSpeed(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Player), id);
    Fld_Partner_SetSpeed(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Partner), id);
}

void FieldBg_ResetScanSeen(FieldEventManager* mgr) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        mgr->scanSeenIds[i] = -1;
    }
}

void FieldBg_LoadScanPicture(FieldEventManager* mgr) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 2, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 4);
    u32   charSize;

    Field_ReleaseEventPicture(mgr);
    mgr->eventPicScreen = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                               g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                               g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    charSize            = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->eventPicChar    = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                                g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, charSize);
    mgr->eventPicPalette = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->eventPicPalette);
    DatMgr_ReleaseData(pack);
}

void FieldBg_ShowEventPicture(FieldEventManager* mgr) {
    Data* pack       = mgr->unk_21620;
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 4);
    u32   charSize;

    Field_ReleaseEventPicture(mgr);
    mgr->eventPicScreen = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                               g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                               g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    charSize            = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->eventPicChar    = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                                g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, charSize);
    mgr->eventPicPalette = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->eventPicPalette);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= 2;
}

static s32 FieldBg_GetScanLevel(s32 arg0) {
    if (arg0 >= 0x1F) {
        return 4;
    }
    if (arg0 >= 0x17) {
        return 1;
    }
    if (arg0 >= 9) {
        return 3;
    }
    if (arg0 == 8) {
        return 2;
    }
    return 0;
}

void FieldBg_UpdateScanPicturePalette(FieldEventManager* mgr) {
    Data* pack    = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 2, FALSE);
    void* palData = Data_GetPackEntryData(pack, FieldBg_GetScanLevel(mgr->scanBgLevel) + 5);

    PaletteMgr_SetSource(g_PaletteManagers[DISPLAY_MAIN], mgr->eventPicPalette, palData);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->eventPicPalette);
    DatMgr_ReleaseData(pack);
}

void FieldBg_UpdateTopBgPalette(FieldEventManager* mgr) {
    Data* pack    = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 5, FALSE);
    void* palData = Data_GetPackEntryData(pack, FieldBg_GetScanLevel(mgr->scanBgLevel) + 0xC);

    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A68);
    mgr->unk_21A68 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 0, 0, 0xE);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A68);
}

void FieldBg_ReleaseSubBg0(FieldEventManager* mgr) {
    if (mgr->unk_21A6C == 0) {
        return;
    }
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A6C);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A70);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A74);
    mgr->unk_21A6C = 0;
}

void FieldBg_LoadScanSubBg0(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    FieldBg_ReleaseSubBg0(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 3, FALSE);
    charData       = Data_GetPackEntryData(pack, 1);
    screenData     = Data_GetPackEntryData(pack, 2);
    palData        = Data_GetPackEntryData(pack, 3);
    mgr->unk_21A6C = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[0].screenBase, 0);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A70 = BgResMgr_AllocCharExtended(g_BgResourceManagers[DISPLAY_SUB], charData,
                                                g_DisplaySettings.engineState[1].bgSettings[0].charBase, 0, charSize);
    mgr->unk_21A74 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 1, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A74);
    DatMgr_ReleaseData(pack);
}

void FieldBg_ReleaseSubBg1(FieldEventManager* mgr) {
    if (mgr->unk_21A78 == 0) {
        return;
    }
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A78);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A7C);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A80);
    mgr->unk_21A78 = 0;
}

void FieldBg_LoadScanSubBg1(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    FieldBg_ReleaseSubBg1(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 4, FALSE);
    charData       = Data_GetPackEntryData(pack, 1);
    screenData     = Data_GetPackEntryData(pack, 2);
    palData        = Data_GetPackEntryData(pack, 5);
    mgr->unk_21A78 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[1].screenBase, 0);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A7C = BgResMgr_AllocCharExtended(g_BgResourceManagers[DISPLAY_SUB], charData,
                                                g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, charSize);
    mgr->unk_21A80 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 2, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A80);
    DatMgr_ReleaseData(pack);
}

void FieldBg_LoadScanFrameSubBg0(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    FieldBg_ReleaseSubBg0(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 4, FALSE);
    charData       = Data_GetPackEntryData(pack, 3);
    screenData     = Data_GetPackEntryData(pack, 4);
    palData        = Data_GetPackEntryData(pack, 5);
    mgr->unk_21A6C = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[0].screenBase, 0);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A70 = BgResMgr_AllocCharExtended(g_BgResourceManagers[DISPLAY_SUB], charData,
                                                g_DisplaySettings.engineState[1].bgSettings[0].charBase, 0, charSize);
    mgr->unk_21A74 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 1, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A74);
    DatMgr_ReleaseData(pack);
}

void FieldBg_LoadDownScreenSubBg(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldDownScreen, 6, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData;

    if (arg1 != 0) {
        palData = Data_GetPackEntryData(pack, 3);
    } else {
        palData = Data_GetPackEntryData(pack, 4);
    }
    FieldBg_ReleaseSubBg1(mgr);
    mgr->unk_21A78 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[3].screenBase, 0);
    {
        u32 charSize = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
        if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
            charSize -= 4;
        }
        mgr->unk_21A7C = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], charData,
                                              g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, charSize);
    }
    mgr->unk_21A80 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 0, 0xF, 1);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A80);
    DatMgr_ReleaseData(pack);
}

static void FieldBg_SetTopBg(FieldEventManager* mgr, void* charData, void* screenData, void* palData, Data* pack,
                             s32 palCount) {
    u32 charSize;

    if (mgr->unk_21A5C != 0) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A60);
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A64);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A68);
    }
    mgr->unk_21A60 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[2].screenBase, 0);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A64 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], charData,
                                          g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, charSize);
    mgr->unk_21A68 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 0, 0, palCount);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A68);
    if (pack == NULL) {
        return;
    }
    DatMgr_ReleaseData(pack);
}

static void FieldBg_SetTopBgDefault(FieldEventManager* mgr, void* charData, void* screenData, void* palData, Data* pack) {
    FieldBg_SetTopBg(mgr, charData, screenData, palData, pack, 0xE);
}

static s32 FieldBg_GetBrandBgIndex(u8 arg0) {
    return Field_BrandBgIndices[arg0] - 1;
}

static void FieldBg_LoadUpBg(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldUpBg, 1, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 3);

    FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = arg1;
}

static void FieldBg_LoadScanTopBg(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 2, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 12);
    void* screenData = Data_GetPackEntryData(pack, 13);
    void* palData    = Data_GetPackEntryData(pack, 14);

    FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = arg1;
}

static void FieldBg_LoadScanTopBgAlt(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 2, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 10);
    void* screenData = Data_GetPackEntryData(pack, 11);
    void* palData    = Data_GetPackEntryData(pack, 3);

    FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = arg1;
}

void FieldBg_LoadScanLevelSubBg1(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;

    FieldBg_ReleaseSubBg1(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 5, FALSE);
    charData       = Data_GetPackEntryData(pack, 17);
    screenData     = Data_GetPackEntryData(pack, 18);
    palData        = Data_GetPackEntryData(pack, FieldBg_GetScanLevel(mgr->scanBgLevel) + 3);
    mgr->unk_21A78 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[1].screenBase, 0);
    {
        u32 charSize = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
        if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
            charSize -= 4;
        }
        mgr->unk_21A7C = BgResMgr_AllocCharExtended(g_BgResourceManagers[DISPLAY_SUB], charData,
                                                    g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, charSize);
    }
    mgr->unk_21A80 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 2, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A80);
    DatMgr_ReleaseData(pack);
}

void FieldBg_LoadScanSubBg3(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    u32   charSize;

    FieldBg_ReleaseSubBg0(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 5, FALSE);
    charData       = Data_GetPackEntryData(pack, 10);
    screenData     = Data_GetPackEntryData(pack, 11);
    mgr->unk_21A6C = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], screenData,
                                          g_DisplaySettings.engineState[1].bgSettings[3].screenBase, 0);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A70 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], charData,
                                          g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, charSize);
    DatMgr_ReleaseData(pack);
}

void FieldBg_LoadScanLevelTopBg(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;

    if (mgr->unk_21A5C == 8) {
        return;
    }
    pack = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldScan, 5, FALSE);
    if (FieldBg_GetScanLevel(mgr->scanBgLevel) == 2) {
        charData   = Data_GetPackEntryData(pack, 8);
        screenData = Data_GetPackEntryData(pack, 9);
    } else {
        charData   = Data_GetPackEntryData(pack, 1);
        screenData = Data_GetPackEntryData(pack, 2);
    }
    palData = Data_GetPackEntryData(pack, FieldBg_GetScanLevel(mgr->scanBgLevel) + 0xC);
    FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = 8;
}

static void FieldBg_LoadShopData(void* buffer, s32 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, buffer, 0xC, &Iden_ShopData, index * 0xC));
}

static void FieldBg_LoadShopBg(FieldEventManager* mgr) {
    FldRawEntry entry;
    s16         sel = 0x46;
    s32         index;

    if (mgr->activeExit != NULL) {
        sel = mgr->activeExit->destArea;
    }
    FieldBg_LoadShopData(&entry, func_ov030_020c5404(sel));
    if (entry.unk_04 >= 3U) {
        entry.unk_04 = 0;
    }
    func_ov030_020c2fc0(entry.unk_04 + 0x1F);
    if (func_ov030_020c5460(sel) != 0) {
        index = entry.unk_02 + 1;
    } else {
        index = entry.unk_02 + 0x3F;
    }
    if (index == 0x4C && func_0202599c(5) != 0) {
        index = 0x4E;
    }
    {
        Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldBG, index, FALSE);
        void* charData   = Data_GetPackEntryData(pack, 1);
        void* screenData = Data_GetPackEntryData(pack, 2);
        void* palData    = Data_GetPackEntryData(pack, 3);

        FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
    }
    mgr->unk_21A5C = 2;
}

static void FieldBg_LoadEventPictureTopBg(FieldEventManager* mgr, s32 arg1) {
    void* charData = Data_GetPackEntryData(mgr->unk_21620, 1);
    void* palData;

    if (arg1 != 0) {
        palData = &data_02059d24;
    } else {
        palData = &data_0205a128;
    }
    FieldBg_SetTopBgDefault(mgr, charData, &data_0205a124, palData, NULL);
}

s32 FieldBg_AreTrendsVisible(void) {
    if (Inventory_HasRequiredQuantity(0x2AE, 1, 0) != 0) {
        return 1;
    }
    return gSaveData.chapter > 2;
}

// Nonmatching: pure register allocation (var_r5 in r4 vs target r5); all
// branches, offsets and immediates verified identical.
void FieldBg_LoadAreaBg(FieldEventManager* mgr, s32 arg1, s32 arg2) {
    if (arg2 == 0x47) {
        FieldBg_LoadEventPictureTopBg(mgr, 0);
        mgr->unk_21A5C = arg1;
        return;
    }
    if (arg2 == 0x48) {
        FieldBg_LoadEventPictureTopBg(mgr, 1);
        mgr->unk_21A5C = arg1;
        return;
    }
    s32 var_r5;

    if ((u32)arg2 < 0x31U) {
        var_r5 = Field_AreaBgIndices[arg2];
    } else {
        var_r5 = 0xC;
    }

    if (arg2 == 5) {
        if (func_0202599c(0) != 0) {
            var_r5 = 8;
        } else if (func_0202599c(8) != 0) {
            var_r5 = 0xA;
        } else {
            var_r5 = 6;
        }
    }
    if ((u32)(arg1 - 2) <= 1U) {
        var_r5 += 1;
        if (arg2 == 0x27) {
            if (func_0202599c(6) != 0) {
                var_r5 = 0x35;
            } else {
                var_r5 = 0x34;
            }
        } else if (arg2 == 0x28) {
            var_r5 = 0x36;
        } else if (arg2 == 0x25) {
            var_r5 = 0x97;
        } else if (arg2 == 0x2D) {
            var_r5 = 0x50;
        } else if (arg2 == 0x2E) {
            var_r5 = 0x4F;
        } else if (arg2 == 0x2C) {
            var_r5 = 0x3D;
        } else if (arg2 == 0x45) {
            var_r5 = 0x3E;
        } else if (arg2 == 0x2F) {
            var_r5 = 0x4D;
        } else if (arg2 == 0x30) {
            var_r5 = 0x92;
        } else if (arg2 == 0x31) {
            var_r5 = 0x93;
        } else if (arg2 == 0x32) {
            var_r5 = 0x94;
        } else if (arg2 == 0x33) {
            var_r5 = 0x95;
        } else if (arg2 == 0x34) {
            var_r5 = 0x96;
        } else if (arg2 >= 0x35 && arg2 <= 0x42) {
            var_r5 = arg2 + 0xA;
        } else if (arg2 == 0x43) {
            var_r5 = 0x4E;
        }
    } else if (arg2 != 0x25) {
        mgr->unk_21700 = Fld_MapName_CreateTask(&mgr->taskPool, mgr->dataType, func_ov030_020b7c34(arg2));
    }
    if (var_r5 == 3) {
        if (FieldBg_AreTrendsVisible() != 0) {
            var_r5 = FieldBg_GetBrandBgIndex(gSaveData.brandTrends[1].ranking[0]) + 0x6B;
        }
    } else if (var_r5 == 4) {
        if (FieldBg_AreTrendsVisible() != 0) {
            var_r5 = FieldBg_GetBrandBgIndex(gSaveData.brandTrends[1].ranking[0]) + 0x78;
        }
    } else if (var_r5 == 1) {
        if (FieldBg_AreTrendsVisible() != 0) {
            var_r5 = FieldBg_GetBrandBgIndex(gSaveData.brandTrends[1].ranking[0]) + 0x51;
        }
    } else if (var_r5 == 2) {
        var_r5 = FieldBg_GetBrandBgIndex(gSaveData.brandTrends[0].ranking[0]) + 0x5E;
    } else if (arg2 == 4 && arg1 == 2) {
        var_r5 = FieldBg_GetBrandBgIndex(gSaveData.brandTrends[0].ranking[0]) + 0x85;
    }
    {
        Data* pack       = DatMgr_LoadPackEntry(mgr->dataType, NULL, 0, &Iden_FldBG, var_r5, FALSE);
        void* charData   = Data_GetPackEntryData(pack, 1);
        void* screenData = Data_GetPackEntryData(pack, 2);
        void* palData    = Data_GetPackEntryData(pack, 3);

        if (var_r5 == 0x2A) {
            FldBgPaletteSet* extraData = Data_GetPackEntryData(pack, 4);
            func_02001b44(7, (g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].charBase << 0xE) + 0x06205000,
                          extraData->palettes[arg2 - 0x18], 0x80);
            DMA_Flush();
            FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
        } else {
            FieldBg_SetTopBgDefault(mgr, charData, screenData, palData, pack);
        }
    }
    mgr->unk_21A5C = arg1;
}

void FieldBg_SetTopBgMode(FieldEventManager* mgr, s32 arg1) {
    if (mgr->unk_21A5C == arg1) {
        return;
    }
    if (mgr->unk_21700 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21700);
        mgr->unk_21700 = -1;
    }
    if (arg1 == 4) {
        FieldBg_LoadScanTopBg(mgr, arg1);
        return;
    }
    if (arg1 == 5) {
        FieldBg_LoadScanTopBgAlt(mgr, arg1);
        return;
    }
    if (arg1 == 7) {
        FieldBg_LoadUpBg(mgr, arg1);
        return;
    }
    if (arg1 == 2 && mgr->unk_21D0C != 0) {
        FieldBg_LoadShopBg(mgr);
        return;
    }
    FieldBg_LoadAreaBg(mgr, arg1, gSaveData.fieldArea);
}

s32 FieldBg_IsBadgeTapped(FieldEventManager* mgr) {
    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    return Fld_Badge_IsTouched(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Badge)) != 0;
}

s32 FieldBg_IsBadgeHeld(FieldEventManager* mgr) {
    return Fld_Badge_IsHeld(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Badge)) != 0;
}

s32 FieldBg_GetTouchedHuman(FieldEventManager* mgr) {
    s32 i;

    if (TouchInput_WasTouchPressed() == 0) {
        return -1;
    }
    for (i = 0; i < mgr->humanCount; i++) {
        if (func_ov030_020b5ce4(EasyTask_GetTaskById(&mgr->taskPool, mgr->taskId_Humans[i])) != 0) {
            return i;
        }
    }
    return -1;
}

s32 FieldBg_IsPlayerNear(s32 arg0, s32 arg1, s32 arg2) {
    s32 dx = arg0 - (gSaveData.fieldPos.x >> 0xC);
    s32 dy;

    if (dx < 0) {
        dx = -dx;
    }
    dy = arg1 - (gSaveData.fieldPos.y >> 0xC);
    if (dx < arg2) {
        if (dy < 0) {
            dy = -dy;
        }
        if (dy < arg2) {
            return 1;
        }
    }
    return 0;
}

s32 FieldBg_IsPlayerInRect(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 cx = gSaveData.fieldPos.x >> 0xC;
    s32 cy;

    if (arg0 < cx && cx < arg2) {
        cy = gSaveData.fieldPos.y >> 0xC;
        if (arg1 < cy && cy < arg3) {
            return 1;
        }
    }
    return 0;
}

s32 FieldBg_CheckExit(FieldEventManager* mgr, s32 arg1) {
    s32 context;

    mgr->activeExit = func_ov030_020b85a8(arg1, &context);
    if (mgr->activeExit != NULL) {
        if (mgr->unk_21AC0 != 0) {
            return 1;
        }
        if (func_ov030_020b7dd8(context) != 0) {
            return 1;
        }
    }
    return 0;
}

void FieldBg_SetAreaPopulation(s32 arg0, s32 arg1) {
    // Bug: the clamp is inverted, so the stored population is always 0 or 100.
    if (arg1 >= 0) {
        arg1 = 0;
    } else if (arg1 < 0x64) {
        arg1 = 0x64;
    }
    gSaveData.areaPopulation[arg0] = arg1;
}

void func_ov030_0208496c(void) {}

s32 func_ov030_02084970(FieldEventManager* mgr) {
    (void)mgr;
    return 0;
}

s32 func_ov030_02084978(FieldEventManager* mgr) {
    (void)mgr;
    return 0;
}

void func_ov030_02084980(void) {
    if (Inventory_HasRequiredQuantity(0x2AE, 1, 0) != 0) {
        FldGate_Set(0);
    }
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = 0x8056;
    Fld_Wall_SetAllWallStates(0);
    gSaveData.unk_2648      = 3;
    gSaveData.unk_264A      = 3;
    gSaveData.unk_264C      = 3;
    gSaveData.unk_264E      = 3;
    gSaveData.unk_2656      = 3;
    gSaveData.flashSequence = 2;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activePartner = 0xFF;
}
