#include "Display.h"
#include "Engine/Color.h"
#include "Engine/Core/DMA.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Field/EventFlags.h"
#include "Field/Events.h"
#include "Player/Inventory.h"
#include "Save.h"
#include <nitro/types.h>

extern BgResource*   BgResMgr_AllocScreen(BgResMgr* mgr, void* source, s32 screenBase, s32 screenSize);
extern BgResource*   BgResMgr_AllocChar32(BgResMgr* mgr, void* source, u32 charBase, u32 offset, u32 size);
extern BgResource*   BgResMgr_AllocCharExtended(BgResMgr* mgr, void* source, s32 charBase, u32 arg3, u32 size);
extern void          func_02001b44(s32, s32, void*, s32);
extern s32           func_0202599c(s32 flagIndex);
extern void          func_ov030_020aec38(s32 arg0);
extern void          func_ov030_020ad428(FieldEventManager* mgr);
extern void          func_ov030_020ae96c(s32 arg0);
extern void          func_ov030_020b3014(Task* task, s32 arg1);
extern void          func_ov030_020bd310(Task* task, s32 arg1);
extern s32           func_ov030_020b4500(Task* task);
extern s32           func_ov030_020b4514(Task* task);
extern s32           func_ov030_020b5ce4(Task* task);
extern s32           func_ov030_020b7c34(u32 arg0);
extern s32           func_ov030_020b7dd8(s32 arg0);
extern FieldExit*    func_ov030_020b85a8(s32 arg0, s32* arg1);
extern void          func_ov030_020c26bc(s32 state);
extern void          func_ov030_020c2fc0(s32 arg0);
extern s32           func_ov030_020c5374(TaskPool* pool, s32 arg1, s32 arg2);
extern s32           func_ov030_020c5404(s16 arg0);
extern s32           func_ov030_020c5460(s16 arg0);
extern s32           data_02059d24;
extern s32           data_0205a124;
extern BinIdentifier data_ov030_020d9594;
extern BinIdentifier data_ov030_020d95ac;
extern BinIdentifier data_ov030_020d95bc;
extern BinIdentifier data_ov030_020d9944;
extern BinIdentifier data_ov030_020d994c;
extern s32           data_ov030_020daa58[];
extern s32           data_ov030_020fde84[];

// Raw 12-byte entries loaded from the data_ov030_020d95bc table (see
// func_ov030_0208416c). Only offsets 0x2 and 0x4 are read here.
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

s32  func_ov030_02082fe4(FieldEventManager* mgr);
void func_ov030_02083024(FieldEventManager* mgr);
void func_ov030_02083088(FieldEventManager* mgr);
void func_ov030_020830ac(FieldEventManager* mgr);
void func_ov030_020831f4(FieldEventManager* mgr);
s32  func_ov030_0208331c(s32 arg0);
void func_ov030_02083350(FieldEventManager* mgr);
void func_ov030_020833fc(FieldEventManager* mgr);
void func_ov030_020834c4(FieldEventManager* mgr);
void func_ov030_02083528(FieldEventManager* mgr);
void func_ov030_02083664(FieldEventManager* mgr);
void func_ov030_020836c8(FieldEventManager* mgr);
void func_ov030_02083804(FieldEventManager* mgr);
void func_ov030_02083940(FieldEventManager* mgr, s32 arg1);
void func_ov030_02083ab4(FieldEventManager* mgr, void* charData, void* screenData, void* palData, Data* pack, s32 palCount);
void func_ov030_02083bcc(FieldEventManager* mgr, void* charData, void* screenData, void* palData, Data* pack);
s32  func_ov030_02083bf0(u8 arg0);
void func_ov030_02083c04(FieldEventManager* mgr, s32 arg1);
void func_ov030_02083cac(FieldEventManager* mgr, s32 arg1);
void func_ov030_02083d54(FieldEventManager* mgr, s32 arg1);
void func_ov030_02083dfc(FieldEventManager* mgr);
void func_ov030_02083f58(FieldEventManager* mgr);
void func_ov030_0208403c(FieldEventManager* mgr);
void func_ov030_0208416c(void* buffer, s32 index);
void func_ov030_02084198(FieldEventManager* mgr);
void func_ov030_020842b4(FieldEventManager* mgr, s32 arg1);
s32  func_ov030_0208430c(void);
void func_ov030_0208434c(FieldEventManager* mgr, s32 arg1, s32 arg2);
void func_ov030_020846bc(FieldEventManager* mgr, s32 arg1);
s32  func_ov030_02084788(FieldEventManager* mgr);
s32  func_ov030_020847c8(FieldEventManager* mgr);
s32  func_ov030_020847f4(FieldEventManager* mgr);
s32  func_ov030_02084860(s32 arg0, s32 arg1, s32 arg2);
s32  func_ov030_020848a4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_ov030_020848e4(FieldEventManager* mgr, s32 arg1);
void func_ov030_02084944(s32 arg0, s32 arg1);
void func_ov030_0208496c(void);
s32  func_ov030_02084970(FieldEventManager* mgr);
s32  func_ov030_02084978(FieldEventManager* mgr);
void func_ov030_02084980(void);

s32 func_ov030_02082fe4(FieldEventManager* mgr) {
    if (mgr->unk_216D0 == -1) {
        return 1;
    }
    return EasyTask_ValidateTaskId(&mgr->taskPool, &mgr->unk_216D0) == 0;
}

void func_ov030_02083024(FieldEventManager* mgr) {
    s32 id;

    if (gSaveData.unk_2458 == 1) {
        id = 0x2666;
    } else {
        id = 0x2CCD;
    }
    func_ov030_020b3014(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D4), id);
    func_ov030_020bd310(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D8), id);
}

void func_ov030_02083088(FieldEventManager* mgr) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        mgr->unk_21B3C[i] = -1;
    }
}

void func_ov030_020830ac(FieldEventManager* mgr) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 2, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 4);
    u32   charSize;

    func_ov030_020ad428(mgr);
    mgr->unk_21A84 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A88 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, charSize);
    mgr->unk_21A8C = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->unk_21A8C);
    DatMgr_ReleaseData(pack);
}

void func_ov030_020831f4(FieldEventManager* mgr) {
    Data* pack       = mgr->unk_21620;
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 4);
    u32   charSize;

    func_ov030_020ad428(mgr);
    mgr->unk_21A84 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A88 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, charSize);
    mgr->unk_21A8C = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->unk_21A8C);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= 2;
}

s32 func_ov030_0208331c(s32 arg0) {
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

void func_ov030_02083350(FieldEventManager* mgr) {
    Data* pack    = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 2, FALSE);
    void* palData = Data_GetPackEntryData(pack, func_ov030_0208331c(mgr->unk_21A98) + 5);

    PaletteMgr_SetSource(g_PaletteManagers[DISPLAY_MAIN], mgr->unk_21A8C, palData);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->unk_21A8C);
    DatMgr_ReleaseData(pack);
}

void func_ov030_020833fc(FieldEventManager* mgr) {
    Data* pack    = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 5, FALSE);
    void* palData = Data_GetPackEntryData(pack, func_ov030_0208331c(mgr->unk_21A98) + 0xC);

    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A68);
    mgr->unk_21A68 = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], palData, 0, 0, 0xE);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A68);
}

void func_ov030_020834c4(FieldEventManager* mgr) {
    if (mgr->unk_21A6C == 0) {
        return;
    }
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A6C);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A70);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A74);
    mgr->unk_21A6C = 0;
}

void func_ov030_02083528(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    func_ov030_020834c4(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 3, FALSE);
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

void func_ov030_02083664(FieldEventManager* mgr) {
    if (mgr->unk_21A78 == 0) {
        return;
    }
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A78);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], mgr->unk_21A7C);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], mgr->unk_21A80);
    mgr->unk_21A78 = 0;
}

void func_ov030_020836c8(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    func_ov030_02083664(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 4, FALSE);
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

void func_ov030_02083804(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    func_ov030_020834c4(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 4, FALSE);
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

void func_ov030_02083940(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d9944, 6, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData;

    if (arg1 != 0) {
        palData = Data_GetPackEntryData(pack, 3);
    } else {
        palData = Data_GetPackEntryData(pack, 4);
    }
    func_ov030_02083664(mgr);
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

void func_ov030_02083ab4(FieldEventManager* mgr, void* charData, void* screenData, void* palData, Data* pack, s32 palCount) {
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

void func_ov030_02083bcc(FieldEventManager* mgr, void* charData, void* screenData, void* palData, Data* pack) {
    func_ov030_02083ab4(mgr, charData, screenData, palData, pack, 0xE);
}

s32 func_ov030_02083bf0(u8 arg0) {
    return data_ov030_020fde84[arg0] - 1;
}

void func_ov030_02083c04(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d994c, 1, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 1);
    void* screenData = Data_GetPackEntryData(pack, 2);
    void* palData    = Data_GetPackEntryData(pack, 3);

    func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = arg1;
}

void func_ov030_02083cac(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 2, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 12);
    void* screenData = Data_GetPackEntryData(pack, 13);
    void* palData    = Data_GetPackEntryData(pack, 14);

    func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = arg1;
}

void func_ov030_02083d54(FieldEventManager* mgr, s32 arg1) {
    Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 2, FALSE);
    void* charData   = Data_GetPackEntryData(pack, 10);
    void* screenData = Data_GetPackEntryData(pack, 11);
    void* palData    = Data_GetPackEntryData(pack, 3);

    func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = arg1;
}

void func_ov030_02083dfc(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;

    func_ov030_02083664(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 5, FALSE);
    charData       = Data_GetPackEntryData(pack, 17);
    screenData     = Data_GetPackEntryData(pack, 18);
    palData        = Data_GetPackEntryData(pack, func_ov030_0208331c(mgr->unk_21A98) + 3);
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

void func_ov030_02083f58(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    u32   charSize;

    func_ov030_020834c4(mgr);
    pack           = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 5, FALSE);
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

void func_ov030_0208403c(FieldEventManager* mgr) {
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;

    if (mgr->unk_21A5C == 8) {
        return;
    }
    pack = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95ac, 5, FALSE);
    if (func_ov030_0208331c(mgr->unk_21A98) == 2) {
        charData   = Data_GetPackEntryData(pack, 8);
        screenData = Data_GetPackEntryData(pack, 9);
    } else {
        charData   = Data_GetPackEntryData(pack, 1);
        screenData = Data_GetPackEntryData(pack, 2);
    }
    palData = Data_GetPackEntryData(pack, func_ov030_0208331c(mgr->unk_21A98) + 0xC);
    func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
    mgr->unk_21A5C = 8;
}

void func_ov030_0208416c(void* buffer, s32 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, buffer, 0xC, &data_ov030_020d95bc, index * 0xC));
}

void func_ov030_02084198(FieldEventManager* mgr) {
    FldRawEntry entry;
    s16         sel = 0x46;
    s32         index;

    if (mgr->unk_21A58 != NULL) {
        sel = mgr->unk_21A58->destArea;
    }
    func_ov030_0208416c(&entry, func_ov030_020c5404(sel));
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
        Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d9594, index, FALSE);
        void* charData   = Data_GetPackEntryData(pack, 1);
        void* screenData = Data_GetPackEntryData(pack, 2);
        void* palData    = Data_GetPackEntryData(pack, 3);

        func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
    }
    mgr->unk_21A5C = 2;
}

void func_ov030_020842b4(FieldEventManager* mgr, s32 arg1) {
    void* charData = Data_GetPackEntryData(mgr->unk_21620, 1);
    void* palData;

    if (arg1 != 0) {
        palData = &data_02059d24;
    } else {
        palData = &data_0205a128;
    }
    func_ov030_02083bcc(mgr, charData, &data_0205a124, palData, NULL);
}

s32 func_ov030_0208430c(void) {
    if (Inventory_HasRequiredQuantity(0x2AE, 1, 0) != 0) {
        return 1;
    }
    return gSaveData.chapter > 2;
}

// Nonmatching: pure register allocation (var_r5 in r4 vs target r5); all
// branches, offsets and immediates verified identical.
void func_ov030_0208434c(FieldEventManager* mgr, s32 arg1, s32 arg2) {
    if (arg2 == 0x47) {
        func_ov030_020842b4(mgr, 0);
        mgr->unk_21A5C = arg1;
        return;
    }
    if (arg2 == 0x48) {
        func_ov030_020842b4(mgr, 1);
        mgr->unk_21A5C = arg1;
        return;
    }
    s32 var_r5;

    if ((u32)arg2 < 0x31U) {
        var_r5 = data_ov030_020daa58[arg2];
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
        mgr->unk_21700 = func_ov030_020c5374(&mgr->taskPool, mgr->unk_21614, func_ov030_020b7c34(arg2));
    }
    if (var_r5 == 3) {
        if (func_ov030_0208430c() != 0) {
            var_r5 = func_ov030_02083bf0(gSaveData.brandTrends[1].ranking[0]) + 0x6B;
        }
    } else if (var_r5 == 4) {
        if (func_ov030_0208430c() != 0) {
            var_r5 = func_ov030_02083bf0(gSaveData.brandTrends[1].ranking[0]) + 0x78;
        }
    } else if (var_r5 == 1) {
        if (func_ov030_0208430c() != 0) {
            var_r5 = func_ov030_02083bf0(gSaveData.brandTrends[1].ranking[0]) + 0x51;
        }
    } else if (var_r5 == 2) {
        var_r5 = func_ov030_02083bf0(gSaveData.brandTrends[0].ranking[0]) + 0x5E;
    } else if (arg2 == 4 && arg1 == 2) {
        var_r5 = func_ov030_02083bf0(gSaveData.brandTrends[0].ranking[0]) + 0x85;
    }
    {
        Data* pack       = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d9594, var_r5, FALSE);
        void* charData   = Data_GetPackEntryData(pack, 1);
        void* screenData = Data_GetPackEntryData(pack, 2);
        void* palData    = Data_GetPackEntryData(pack, 3);

        if (var_r5 == 0x2A) {
            FldBgPaletteSet* extraData = Data_GetPackEntryData(pack, 4);
            func_02001b44(7, (g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].charBase << 0xE) + 0x06205000,
                          extraData->palettes[arg2 - 0x18], 0x80);
            DMA_Flush();
            func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
        } else {
            func_ov030_02083bcc(mgr, charData, screenData, palData, pack);
        }
    }
    mgr->unk_21A5C = arg1;
}

void func_ov030_020846bc(FieldEventManager* mgr, s32 arg1) {
    if (mgr->unk_21A5C == arg1) {
        return;
    }
    if (mgr->unk_21700 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21700);
        mgr->unk_21700 = -1;
    }
    if (arg1 == 4) {
        func_ov030_02083cac(mgr, arg1);
        return;
    }
    if (arg1 == 5) {
        func_ov030_02083d54(mgr, arg1);
        return;
    }
    if (arg1 == 7) {
        func_ov030_02083c04(mgr, arg1);
        return;
    }
    if (arg1 == 2 && mgr->unk_21D0C != 0) {
        func_ov030_02084198(mgr);
        return;
    }
    func_ov030_0208434c(mgr, arg1, gSaveData.unk_2458);
}

s32 func_ov030_02084788(FieldEventManager* mgr) {
    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    return func_ov030_020b4514(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B0)) != 0;
}

s32 func_ov030_020847c8(FieldEventManager* mgr) {
    return func_ov030_020b4500(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B0)) != 0;
}

s32 func_ov030_020847f4(FieldEventManager* mgr) {
    s32 i;

    if (TouchInput_WasTouchPressed() == 0) {
        return -1;
    }
    for (i = 0; i < mgr->unk_21918; i++) {
        if (func_ov030_020b5ce4(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_21788[i])) != 0) {
            return i;
        }
    }
    return -1;
}

s32 func_ov030_02084860(s32 arg0, s32 arg1, s32 arg2) {
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

s32 func_ov030_020848a4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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

s32 func_ov030_020848e4(FieldEventManager* mgr, s32 arg1) {
    s32 context;

    mgr->unk_21A58 = func_ov030_020b85a8(arg1, &context);
    if (mgr->unk_21A58 != NULL) {
        if (mgr->unk_21AC0 != 0) {
            return 1;
        }
        if (func_ov030_020b7dd8(context) != 0) {
            return 1;
        }
    }
    return 0;
}

void func_ov030_02084944(s32 arg0, s32 arg1) {
    if (arg1 >= 0) {
        arg1 = 0;
    } else if (arg1 < 0x64) {
        arg1 = 0x64;
    }
    gSaveData.unk_2548[arg0] = arg1;
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
    func_ov030_020c26bc(0);
    gSaveData.unk_2648 = 3;
    gSaveData.unk_264A = 3;
    gSaveData.unk_264C = 3;
    gSaveData.unk_264E = 3;
    gSaveData.unk_2656 = 3;
    gSaveData.unk_3124 = 2;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activePartner = 0xFF;
}
