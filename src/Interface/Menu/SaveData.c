#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/Save.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save.h"
#include <nitro/mi/cpumem.h>
#include <nitro/os/os_owner.h>
#include <nitro/rtc.h>

void func_0203a96c(u8* macAddress);
s32  func_020417e0(RTCDate* date, RTCTime* time);
u16  Inventory_GetNoiseReportCount(void);
u16  Inventory_GetMasteredPinCount(void);
u16  Inventory_GetCollectedItemCount(void);

s32 Save_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void Save_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

void Save_LoadPinData(RawPinData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x3DC0, &Save_BinIdentifiers[13]));
}

void Save_LoadItemData(RawItemData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x1A40, &Save_BinIdentifiers[14]));
}

void Save_LoadFoodData(RawFoodData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x348, &Save_BinIdentifiers[15]));
}

void Save_LoadTreasureData(RawTreasureData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x4B0, &Save_BinIdentifiers[16]));
}

u8 Save_GetBadgeSlotCount(void) {
    u8 count = Inventory_GetOwnedCount(ITEM_STICKER_EXTRA_SLOT) + 2;

    if (count > 6) {
        count = 6;
    }
    return count;
}

u16 Save_SumPinPP(u16 battlePP, u16 minglePP, u16 shutdownPP) {
    return shutdownPP + (battlePP + minglePP);
}

u16 Save_GetPinLevelPP(u8 level, s32 ppCurve) {
    return func_02023480(level, ppCurve);
}

u16 Save_GetPinNextLevelPP(u8 level, u8 maxLevel, s32 ppCurve) {
    if (level == maxLevel) {
        return 0xFFFF;
    }
    return func_02023480((u8)(level + 1), ppCurve);
}

// Nonmatching: register rotation around the ability-flag table base, which the target reloads for the partner
// loop instead of keeping it live (same as TusinSet_LoadFromSave)
void Save_LoadFromSave(SaveMenuObject* save) {
    RawPinData*      pinData;
    RawItemData*     itemData;
    RawFoodData*     foodData;
    RawTreasureData* treasureData;
    ItemCategory     category;
    u16              itemId, index, i, j;

    pinData = Mem_AllocHeapTail(&gDebugHeap, 0x3DC0);
    Mem_SetSequence(&gDebugHeap, pinData, "badge_data");
    itemData = Mem_AllocHeapTail(&gDebugHeap, 0x1A40);
    Mem_SetSequence(&gDebugHeap, itemData, "item_data");
    foodData = Mem_AllocHeapTail(&gDebugHeap, 0x348);
    Mem_SetSequence(&gDebugHeap, foodData, "food_data");
    treasureData = Mem_AllocHeapTail(&gDebugHeap, 0x4B0);
    Mem_SetSequence(&gDebugHeap, treasureData, "treasure_data");

    Save_LoadPinData(pinData);
    Save_LoadItemData(itemData);
    Save_LoadFoodData(foodData);
    Save_LoadTreasureData(treasureData);

    save->partner = gSaveData.lastPlayerStats.activePartner;
    for (i = 0; i < 4; i++) {
        itemId                  = gSaveData.lastPlayerStats.equippedThreads[i];
        save->threads[i].itemId = itemId;
        index                   = Inventory_GetCategorizedIndex(itemId);
#ifdef REGION_USA
        if (index == 0xFFFF) {
            save->threads[i].abilityUnlocked = 0;
        } else {
            save->threads[i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
        }
#else
        save->threads[i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
#endif
    }

    if (save->partner == 0xFF) {
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 4; i++) {
                save->threads[4 + j * 4 + i].itemId          = 0xFFFF;
                save->threads[4 + j * 4 + i].abilityUnlocked = 0;
            }
        }
    } else {
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 4; i++) {
                itemId                              = gSaveData.lastFriendStats[j].equippedThreads[i];
                save->threads[4 + j * 4 + i].itemId = itemId;
                index                               = Inventory_GetCategorizedIndex(itemId);
#ifdef REGION_USA
                if (index == 0xFFFF) {
                    save->threads[4 + j * 4 + i].abilityUnlocked = 0;
                } else {
                    save->threads[4 + j * 4 + i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
                }
#else
                save->threads[4 + j * 4 + i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
#endif
            }
        }
    }

    save->gift.itemId          = gSaveData.lastGiftItemId;
    save->gift.abilityUnlocked = 0;
    for (i = 0; i < 17; i++) {
        index    = Inventory_GetCategorizedIndex(save->threads[i].itemId);
        category = Inventory_GetCategory(save->threads[i].itemId);
        if (index == 0xFFFF) {
            save->threads[i].graphicIndex = 0xFFFF;
        } else {
            switch (category) {
                case ITEM_CATEGORY_THREAD:
                    save->threads[i].graphicIndex = itemData[index].unk_00;
                    break;
                case ITEM_CATEGORY_FOOD:
                    save->threads[i].graphicIndex = foodData[index].unk_00;
                    break;
                case ITEM_CATEGORY_SWAG:
                    save->threads[i].graphicIndex = treasureData[index].unk_00;
                    break;
                default:
                    OS_WaitForever();
                    break;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        save->slots[i] = &save->threads[i];
    }
    if (save->partner == 0xFF) {
        for (i = 0; i < 4; i++) {
            save->slots[i + 4] = &save->threads[i + 4];
        }
    } else {
        for (i = 0; i < 4; i++) {
            save->slots[i + 4] = &save->threads[i + (save->partner + 1) * 4];
        }
    }
    save->slots[8] = &save->gift;

    for (i = 0; i < 6; i++) {
        u16 pinId = gSaveData.lastEquippedPins[i].pinID;

        if (pinId == 0xFFFF) {
            save->pins[i].pinId       = 0xFFFF;
            save->pins[i].iconIndex   = 0xFFFF;
            save->pins[i].unk_4       = 0;
            save->pins[i].level       = 1;
            save->pins[i].maxLevel    = 100;
            save->pins[i].levelPP     = 0;
            save->pins[i].totalPP     = 0;
            save->pins[i].nextLevelPP = 0;
        } else {
            save->pins[i].pinId     = pinId;
            save->pins[i].iconIndex = pinData[pinId].iconIndex;
            save->pins[i].unk_4     = gSaveData.lastEquippedPins[i].unk_08;
            save->pins[i].level     = gSaveData.lastEquippedPins[i].flags.bits.level;
            save->pins[i].maxLevel  = pinData[pinId].maxLevel;
            save->pins[i].levelPP = Save_GetPinLevelPP(gSaveData.lastEquippedPins[i].flags.bits.level, pinData[pinId].ppCurve);
            save->pins[i].totalPP =
                Save_SumPinPP(gSaveData.lastEquippedPins[i].battlePP, gSaveData.lastEquippedPins[i].minglePP,
                              gSaveData.lastEquippedPins[i].shutdownPP);
            save->pins[i].nextLevelPP = Save_GetPinNextLevelPP(gSaveData.lastEquippedPins[i].flags.bits.level,
                                                               pinData[pinId].maxLevel, pinData[pinId].ppCurve);
        }
    }

    save->saveYear         = gSaveData.lastSaveTime.year;
    save->saveMonth        = gSaveData.lastSaveTime.month;
    save->saveDay          = gSaveData.lastSaveTime.day;
    save->saveHour         = gSaveData.lastSaveTime.hour;
    save->saveMinute       = gSaveData.lastSaveTime.minute;
    save->esperPoints      = gSaveData.lastExperience.current;
    save->noiseReportCount = gSaveData.lastExperience.unk_0_0;
    save->pinsMastered     = gSaveData.lastExperience.pinCount;
    save->itemsCollected   = gSaveData.lastExperience.itemCount;
    save->timeAttackFrames = gSaveData.lastExperience.unk_8;
    save->chapter          = gSaveData.lastChapter;
    save->area             = gSaveData.lastArea;
    save->badgeSlots       = gSaveData.lastBadgeSlots;

    Mem_Free(&gDebugHeap, pinData);
    Mem_Free(&gDebugHeap, itemData);
    Mem_Free(&gDebugHeap, foodData);
    Mem_Free(&gDebugHeap, treasureData);
}

void Save_LoadCard(SaveMenuObject* save) {
    Save_LoadFromSave(save);
#ifdef REGION_USA
    save->prevChapter = gSaveData.chapter;
    if ((gSaveData.unk_1AB4 & 0x80) && (gSaveData.unk_1AB4 & 2)) {
        gSaveData.chapter = 21;
    } else if (gSaveData.unk_1AB4 & 2) {
        gSaveData.chapter++;
    }
#endif
    save->message = 0;
}

#ifdef REGION_USA
void Save_WriteSnapshot(SaveMenuObject* save, u8 chapter) {
#else
void Save_WriteSnapshot(SaveMenuObject* save) {
#endif
    OSOwnerInfo ownerInfo;
    RTCDate     date;
    RTCTime     time;
    u16         i;

    func_0203a96c(gSaveData.lastMacAddress);
    OS_GetOwnerInfo(&ownerInfo);
    MI_CpuCopyU8(ownerInfo.nickName, gSaveData.lastNickName, sizeof(gSaveData.lastNickName));
    gSaveData.experience.unk_0_0   = Inventory_GetNoiseReportCount();
    gSaveData.experience.pinCount  = Inventory_GetMasteredPinCount();
    gSaveData.experience.itemCount = Inventory_GetCollectedItemCount();
    gSaveData.lastExperience       = gSaveData.experience;

    func_020417e0(&date, &time);
    gSaveData.lastSaveTime.year   = date.year;
    gSaveData.lastSaveTime.month  = date.month;
    gSaveData.lastSaveTime.day    = date.day;
    gSaveData.lastSaveTime.hour   = time.hour;
    gSaveData.lastSaveTime.minute = time.minute;
    gSaveData.lastSaveTime.second = time.second;
#ifdef REGION_USA
    gSaveData.lastChapter = chapter;
#else
    gSaveData.lastChapter = gSaveData.chapter;
#endif
    gSaveData.lastArea        = gSaveData.currentArea;
    gSaveData.lastPlayerStats = gSaveData.playerStats;

    for (i = 0; i < 3; i++) {
        gSaveData.lastFriendStats[i] = gSaveData.friendStats[i];
    }
    for (i = 0; i < 6; i++) {
        gSaveData.lastEquippedPins[i] = gSaveData.equippedPins[i];
    }

    gSaveData.lastBadgeSlots = Save_GetBadgeSlotCount();
    gSaveData.lastGiftItemId = gSaveData.mingleShop.giftItemId;
}

s16 Save_GetIconAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {205, 2},
        {230, 2},
    };
    s16 size[2] = {23, 19};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (Save_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 Save_IsPointOnSaveBtn(s16 x, s16 y) {
    s16 pos[2]  = {209, 159};
    s16 size[2] = {46, 32};

    if (Save_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s16 Save_GetPartnerAtPoint(s16 x, s16 y) {
    s16 posX[3] = {125, 143, 161};
    s16 size[2] = {16, 13};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (Save_IsPointInRect(x, y, posX[i], 5, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 Save_GetHelpBtnAtPoint(s16 x, s16 y) {
    s16 pos[3][2] = {
        { 32,  56},
        {192,  56},
        { 80, 115},
    };
    s16 size[3][2] = {
        {32, 64},
        {32, 64},
        {96, 20},
    };
    s16 i;

    for (i = 0; i < 3; i++) {
        if (Save_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

void Save_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &Save_BinIdentifiers[binIndex]);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, 2);
    res->paletteData = Data_GetPackEntryData(res->data, 3);
    if (engine == DISPLAY_MAIN) {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void Save_LoadBgResourceIndexed(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                s32 screenIndex, s32 palIndex) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &Save_BinIdentifiers[binIndex]);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, screenIndex + 2);
    res->paletteData = Data_GetPackEntryData(res->data, palIndex + 3);
    if (engine == DISPLAY_MAIN) {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void Save_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
    res->screenMap = Data_GetPackEntryData(data, screenIndex);
    if (engine == DISPLAY_MAIN) {
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
    } else {
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
    }
}

void Save_ReleaseBgResource(MenuBgResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], res->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], res->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
    DatMgr_ReleaseData(res->data);
}

void Save_ReleaseBgScreen(MenuBgResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
    } else {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
    }
}

void Save_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    Save_ReleaseBgResource(res, engine);
    Save_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void Save_ClearBgResource(MenuBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void Save_LoadBackgrounds(SaveMenuObject* save) {
    s32             i;
    MenuBgResource* subRes  = &save->resources[0];
    MenuBgResource* mainRes = &save->resources[4];
    s32             screenIndex;

    for (i = 0; i < 4; i++) {
        Save_ClearBgResource(subRes);
        Save_ClearBgResource(mainRes);
        subRes += 1;
        mainRes += 1;
    }

    Save_LoadBgResource(&save->resources[5], DISPLAY_MAIN, 1, 2, 15, 1);
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) == 0 || save->partner == 0xFF) {
        Save_LoadBgResourceIndexed(&save->resources[6], DISPLAY_MAIN, 2, 1, 1, 2, 0, 0);
    } else {
        Save_LoadBgResourceIndexed(&save->resources[6], DISPLAY_MAIN, 2, 1, 1, 2, 2, 0);
    }
    Save_LoadBgResource(&save->resources[7], DISPLAY_MAIN, 3, 0, 0, 1);
    Save_LoadBgScreen(&save->resources[4], save->resources[6].data, DISPLAY_MAIN, 0, 5);

    Save_LoadBgResource(&save->resources[0], DISPLAY_SUB, 0, 6, 15, 1);
    if (save->partner == 0xFF) {
        Save_LoadBgResourceIndexed(&save->resources[3], DISPLAY_SUB, 3, 5, 1, 13, 8, 0);
    } else {
        if (save->partner == 0) {
            screenIndex = 0;
        } else if (save->partner == 1) {
            screenIndex = 4;
        } else {
            screenIndex = 5;
        }
        Save_LoadBgResourceIndexed(&save->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex, save->partner);
    }
    Save_LoadBgScreen(&save->resources[1], save->resources[3].data, DISPLAY_SUB, 1, 8);
    Save_LoadBgScreen(&save->resources[2], save->resources[3].data, DISPLAY_SUB, 2, 9);
}

void Save_UpdateBackgrounds(SaveMenuObject* save) {}

void Save_ReleaseBackgrounds(SaveMenuObject* save) {
    Save_ReleaseBgScreen(&save->resources[4], DISPLAY_MAIN);
    Save_ReleaseBgResource(&save->resources[5], DISPLAY_MAIN);
    Save_ReleaseBgResource(&save->resources[6], DISPLAY_MAIN);
    Save_ReleaseBgResource(&save->resources[7], DISPLAY_MAIN);
    Save_ReleaseBgScreen(&save->resources[1], DISPLAY_SUB);
    Save_ReleaseBgScreen(&save->resources[2], DISPLAY_SUB);
    Save_ReleaseBgResource(&save->resources[0], DISPLAY_SUB);
    Save_ReleaseBgResource(&save->resources[3], DISPLAY_SUB);
}
