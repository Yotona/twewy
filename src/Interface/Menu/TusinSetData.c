#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/TusinSet.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save.h"
#include <nitro/math.h>

void func_02047ec8(void* head, u32 num, u32 width, s32 (*compare)(u16*, u16*), void* buffer);
u16  func_02024080(void);
u16  func_020241b0(void);
u16  func_02024244(void);

s32 TusinSet_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void TusinSet_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

void TusinSet_LoadPinData(RawPinData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x3DC0, &TusinSet_BinIdentifiers[13]));
}

void TusinSet_LoadItemData(RawItemData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x1A40, &TusinSet_BinIdentifiers[14]));
}

void TusinSet_LoadFoodData(RawFoodData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x348, &TusinSet_BinIdentifiers[15]));
}

void TusinSet_LoadTreasureData(RawTreasureData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x4B0, &TusinSet_BinIdentifiers[16]));
}

u8 TusinSet_GetBadgeSlotCount(void) {
    u8 count = Inventory_GetOwnedCount(ITEM_STICKER_EXTRA_SLOT) + 2;

    if (count > 6) {
        count = 6;
    }
    return count;
}

u16 TusinSet_SumPinPP(u16 battlePP, u16 minglePP, u16 shutdownPP) {
    return shutdownPP + (battlePP + minglePP);
}

u16 TusinSet_GetPinLevelPP(u8 level, s32 ppCurve) {
    return func_02023480(level, ppCurve);
}

u16 TusinSet_GetPinNextLevelPP(u8 level, u8 maxLevel, s32 ppCurve) {
    if (level == maxLevel) {
        return 0xFFFF;
    }
    return func_02023480((u8)(level + 1), ppCurve);
}

#ifdef REGION_USA
u16 TusinSet_BuildListBySubCategory(TusinSetObject* tusinSet, s32 tab, s32 category, s32 subCategory) {
#else
void TusinSet_BuildListBySubCategory(TusinSetObject* tusinSet, s32 tab, s32 category, s32 subCategory) {
#endif
    u16 i, count;

    count = 0;
    for (i = 0; i < 472; i++) {
        if ((category == tusinSet->inventory[i].category) && (subCategory == tusinSet->inventory[i].subCategory)) {
            tusinSet->lists[tab][count] = &tusinSet->inventory[i];
            count++;
        }
    }
#ifdef REGION_USA
    return count;
#endif
}

#ifdef REGION_USA
u16 TusinSet_BuildListByCategory(TusinSetObject* tusinSet, s32 tab, s32 category) {
#else
void TusinSet_BuildListByCategory(TusinSetObject* tusinSet, s32 tab, s32 category) {
#endif
    u16 i, count;

    count = 0;
    for (i = 0; i < 472; i++) {
        if (category == tusinSet->inventory[i].category) {
            tusinSet->lists[tab][count] = &tusinSet->inventory[i];
            count++;
        }
    }
#ifdef REGION_USA
    return count;
#endif
}

#ifdef REGION_USA
// Rounds a list length up to whole pages of 8 rows, with a minimum of two pages.
u16 TusinSet_RoundUpToPages(u16 count) {
    if (count < 16) {
        return 16;
    }
    if (count % 8 == 0) {
        return count;
    }
    return ((count / 8) + 1) * 8;
}
#endif

// JP's lists always span a fixed 57 rows, so it keeps no per-tab sizes.
void TusinSet_BuildLists(TusinSetObject* tusinSet) {
#ifdef REGION_USA
    u16 counts[9];
    u16 count;
    u16 tabIndex;
    u32 sum;
#endif

    for (u16 tab = 0; tab < 9; tab++) {
        for (u16 i = 0; i < 472; i++) {
            tusinSet->lists[tab][i] = &TusinSet_EmptyItem;
        }
    }
#ifdef REGION_USA
    counts[0] = TusinSet_BuildListBySubCategory(tusinSet, 0, ITEM_CATEGORY_THREAD, 0);
    counts[1] = TusinSet_BuildListBySubCategory(tusinSet, 1, ITEM_CATEGORY_THREAD, 1);
    counts[2] = TusinSet_BuildListBySubCategory(tusinSet, 2, ITEM_CATEGORY_THREAD, 2);
    counts[3] = TusinSet_BuildListBySubCategory(tusinSet, 3, ITEM_CATEGORY_THREAD, 3);
    counts[4] = TusinSet_BuildListBySubCategory(tusinSet, 4, ITEM_CATEGORY_THREAD, 4);
    counts[5] = TusinSet_BuildListBySubCategory(tusinSet, 5, ITEM_CATEGORY_THREAD, 5);
    counts[6] = TusinSet_BuildListByCategory(tusinSet, 6, ITEM_CATEGORY_FOOD);
    counts[7] = TusinSet_BuildListByCategory(tusinSet, 7, ITEM_CATEGORY_SWAG);
#else
    TusinSet_BuildListBySubCategory(tusinSet, 0, ITEM_CATEGORY_THREAD, 0);
    TusinSet_BuildListBySubCategory(tusinSet, 1, ITEM_CATEGORY_THREAD, 1);
    TusinSet_BuildListBySubCategory(tusinSet, 2, ITEM_CATEGORY_THREAD, 2);
    TusinSet_BuildListBySubCategory(tusinSet, 3, ITEM_CATEGORY_THREAD, 3);
    TusinSet_BuildListBySubCategory(tusinSet, 4, ITEM_CATEGORY_THREAD, 4);
    TusinSet_BuildListBySubCategory(tusinSet, 5, ITEM_CATEGORY_THREAD, 5);
    TusinSet_BuildListByCategory(tusinSet, 6, ITEM_CATEGORY_FOOD);
    TusinSet_BuildListByCategory(tusinSet, 7, ITEM_CATEGORY_SWAG);
#endif
    for (u16 i = 0; i < 472; i++) {
        tusinSet->lists[8][i] = &tusinSet->inventory[i];
    }
#ifdef REGION_USA
    sum = 0;
    for (u16 i = 0; i < 8; i++) {
        sum = (u16)(sum + counts[i]);
    }
    counts[8] = sum;
    for (tabIndex = 0; tabIndex < 9; tabIndex++) {
        count = counts[tabIndex];

        tusinSet->listSizes[tabIndex]      = TusinSet_RoundUpToPages(count);
        tusinSet->maxScrollRow[tabIndex]   = tusinSet->listSizes[tabIndex] / 8 - 2;
        tusinSet->scrollBarRange[tabIndex] = tusinSet->maxScrollRow[tabIndex];
        if (count <= 16) {
            tusinSet->fitsOnePage[tabIndex] = 1;
        } else {
            tusinSet->fitsOnePage[tabIndex] = 0;
        }
    }
#endif
}

s32 TusinSet_CompareItemIds(u16* a, u16* b) {
    return *a - *b;
}

void TusinSet_SortInventory(TusinSetObject* tusinSet) {
    void* buffer = Mem_AllocHeapTail(&gDebugHeap, MATH_QSortStackSize(472));

    Mem_SetSequence(&gDebugHeap, buffer, "ItemID_sortBuf");
    func_02047ec8(tusinSet->inventory, 472, sizeof(TusinSetItem), TusinSet_CompareItemIds, buffer);
    Mem_Free(&gDebugHeap, buffer);
}

// Nonmatching: register rotation around the hoisted ability-flag table base (the (u8*) cast is what makes mwcc
// load gSaveData+0x1EB2 as its own constant, as the target does)
void TusinSet_LoadFromSave(TusinSetObject* tusinSet) {
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

    TusinSet_LoadPinData(pinData);
    TusinSet_LoadItemData(itemData);
    TusinSet_LoadFoodData(foodData);
    TusinSet_LoadTreasureData(treasureData);

    tusinSet->partner = gSaveData.playerStats.activePartner;
#ifndef REGION_USA
    tusinSet->tab = 8;
#endif
    for (i = 0; i < 4; i++) {
        itemId                      = gSaveData.playerStats.equippedThreads[i];
        tusinSet->threads[i].itemId = itemId;
        index                       = Inventory_GetCategorizedIndex(itemId);
#ifdef REGION_USA
        if (index == 0xFFFF) {
            tusinSet->threads[i].abilityUnlocked = 0;
        } else {
            tusinSet->threads[i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
        }
#else
        tusinSet->threads[i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
#endif
    }

    if (tusinSet->partner == 0xFF) {
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 4; i++) {
                tusinSet->threads[4 + j * 4 + i].itemId          = 0xFFFF;
                tusinSet->threads[4 + j * 4 + i].abilityUnlocked = 0;
            }
        }
    } else {
        for (j = 0; j < 3; j++) {
            for (i = 0; i < 4; i++) {
                itemId                                  = gSaveData.friendStats[j].equippedThreads[i];
                tusinSet->threads[4 + j * 4 + i].itemId = itemId;
                index                                   = Inventory_GetCategorizedIndex(itemId);
#ifdef REGION_USA
                if (index == 0xFFFF) {
                    tusinSet->threads[4 + j * 4 + i].abilityUnlocked = 0;
                } else {
                    tusinSet->threads[4 + j * 4 + i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
                }
#else
                tusinSet->threads[4 + j * 4 + i].abilityUnlocked = (u32)(((u8*)gSaveData.unk_1EB2)[index] << 0x1F) >> 0x1F;
#endif
            }
        }
    }

    tusinSet->gift.itemId          = gSaveData.mingleShop.giftItemId;
    tusinSet->gift.abilityUnlocked = 0;
    for (i = 0; i < 17; i++) {
        index    = Inventory_GetCategorizedIndex(tusinSet->threads[i].itemId);
        category = Inventory_GetCategory(tusinSet->threads[i].itemId);
        if (index == 0xFFFF) {
            tusinSet->threads[i].graphicIndex = 0xFFFF;
        } else {
            switch (category) {
                case ITEM_CATEGORY_THREAD:
                    tusinSet->threads[i].graphicIndex = itemData[index].unk_00;
                    break;
                case ITEM_CATEGORY_FOOD:
                    tusinSet->threads[i].graphicIndex = foodData[index].unk_00;
                    break;
                case ITEM_CATEGORY_SWAG:
                    tusinSet->threads[i].graphicIndex = treasureData[index].unk_00;
                    break;
                default:
                    OS_WaitForever();
                    break;
            }
        }
        tusinSet->threads[i].category    = 0;
        tusinSet->threads[i].subCategory = 0;
    }
#ifndef REGION_USA
    // JP sorts before the inventory below has been read.
    TusinSet_SortInventory(tusinSet);
#endif

    for (i = 0; i < 4; i++) {
        tusinSet->slots[i] = &tusinSet->threads[i];
    }
    if (tusinSet->partner == 0xFF) {
        for (i = 0; i < 4; i++) {
            tusinSet->slots[i + 4] = &tusinSet->threads[i + 4];
        }
    } else {
        for (i = 0; i < 4; i++) {
            tusinSet->slots[i + 4] = &tusinSet->threads[i + (tusinSet->partner + 1) * 4];
        }
    }
    tusinSet->slots[8] = &tusinSet->gift;

    for (i = 0; i < 6; i++) {
        u16 pinId = gSaveData.equippedPins[i].pinID;

        if (pinId == 0xFFFF) {
            tusinSet->pins[i].pinId       = 0xFFFF;
            tusinSet->pins[i].iconIndex   = 0xFFFF;
            tusinSet->pins[i].unk_4       = 0;
            tusinSet->pins[i].level       = 1;
            tusinSet->pins[i].maxLevel    = 100;
            tusinSet->pins[i].levelPP     = 0;
            tusinSet->pins[i].totalPP     = 0;
            tusinSet->pins[i].nextLevelPP = 0;
        } else {
            tusinSet->pins[i].pinId     = pinId;
            tusinSet->pins[i].iconIndex = pinData[pinId].iconIndex;
            tusinSet->pins[i].unk_4     = gSaveData.equippedPins[i].unk_08;
            tusinSet->pins[i].level     = gSaveData.equippedPins[i].flags.bits.level;
            tusinSet->pins[i].maxLevel  = pinData[pinId].maxLevel;
            tusinSet->pins[i].levelPP   = TusinSet_GetPinLevelPP(tusinSet->pins[i].level, pinData[pinId].ppCurve);
            tusinSet->pins[i].totalPP   = TusinSet_SumPinPP(
                gSaveData.equippedPins[i].battlePP, gSaveData.equippedPins[i].minglePP, gSaveData.equippedPins[i].shutdownPP);
            tusinSet->pins[i].nextLevelPP =
                TusinSet_GetPinNextLevelPP(tusinSet->pins[i].level, tusinSet->pins[i].maxLevel, pinData[pinId].ppCurve);
        }
    }

    gSaveData.experience.unk_0_0   = func_02024080();
    gSaveData.experience.pinCount  = func_020241b0();
    gSaveData.experience.itemCount = func_02024244();
    tusinSet->unk_61DA             = (u32)(gSaveData.experience.unk_2 << 0x14) >> 0x1D;
    tusinSet->esperPoints          = gSaveData.experience.current;
    tusinSet->noiseReportCount     = gSaveData.experience.unk_0_0;
    tusinSet->pinsMastered         = gSaveData.experience.pinCount;
    tusinSet->itemsCollected       = gSaveData.experience.itemCount;
    tusinSet->timeAttackFrames     = gSaveData.experience.unk_8;

    for (i = 0; i < 472; i++) {
        tusinSet->inventory[i].itemId = gSaveData.inventoryItems[i].itemID;
        index                         = Inventory_GetCategorizedIndex(tusinSet->inventory[i].itemId);
        category                      = Inventory_GetCategory(tusinSet->inventory[i].itemId);
        if (index == 0xFFFF) {
            tusinSet->inventory[i].graphicIndex = 0xFFFF;
            tusinSet->inventory[i].category     = 0;
            tusinSet->inventory[i].subCategory  = 0;
        } else {
            switch (category) {
                case ITEM_CATEGORY_THREAD:
                    tusinSet->inventory[i].graphicIndex = itemData[index].unk_00;
                    tusinSet->inventory[i].category     = ITEM_CATEGORY_THREAD;
                    tusinSet->inventory[i].subCategory  = itemData[index].unk_03;
                    break;
                case ITEM_CATEGORY_FOOD:
                    tusinSet->inventory[i].graphicIndex = foodData[index].unk_00;
                    tusinSet->inventory[i].category     = ITEM_CATEGORY_FOOD;
                    tusinSet->inventory[i].subCategory  = 0;
                    break;
                case ITEM_CATEGORY_SWAG:
                    tusinSet->inventory[i].graphicIndex = treasureData[index].unk_00;
                    tusinSet->inventory[i].category     = ITEM_CATEGORY_SWAG;
                    tusinSet->inventory[i].subCategory  = 0;
                    break;
                default:
                    OS_WaitForever();
                    break;
            }
        }
    }

#ifdef REGION_USA
    TusinSet_SortInventory(tusinSet);
#endif
    TusinSet_BuildLists(tusinSet);

#ifdef REGION_USA
    tusinSet->scroll   = gSaveData.unk_2436;
    tusinSet->unk_61D6 = tusinSet->scroll;
    tusinSet->cursor   = gSaveData.unk_2438;
    if (gSaveData.unk_243E == 0) {
        tusinSet->tab = 8;
    } else {
        tusinSet->tab = gSaveData.unk_243E - 1;
    }
    if (tusinSet->tab < 8) {
        tusinSet->tabSelected[tusinSet->tab] = 1;
    }
    for (i = 0; i < 16; i++) {
        tusinSet->visible[i] = tusinSet->lists[tusinSet->tab][tusinSet->scroll + i];
    }
    if (tusinSet->inventory[tusinSet->cursor].itemId != 0xFFFF) {
        tusinSet->selected = *tusinSet->lists[tusinSet->tab][tusinSet->cursor];
    } else {
        tusinSet->selected = TusinSet_EmptyItem;
    }
#else
    for (i = 0; i < 16; i++) {
        tusinSet->visible[i] = tusinSet->lists[tusinSet->tab][i];
    }
    if (tusinSet->visible[0]->itemId != 0xFFFF) {
        tusinSet->selected = *tusinSet->lists[tusinSet->tab][0];
    } else {
        tusinSet->selected = TusinSet_EmptyItem;
    }
#endif
    tusinSet->badgeSlots = TusinSet_GetBadgeSlotCount();

    Mem_Free(&gDebugHeap, pinData);
    Mem_Free(&gDebugHeap, itemData);
    Mem_Free(&gDebugHeap, foodData);
    Mem_Free(&gDebugHeap, treasureData);
}

void TusinSet_WriteToSave(TusinSetObject* tusinSet) {
    u16 i;

    for (i = 0; i < 16; i++) {
        gSaveData.mingleShop.unk_10[i] = tusinSet->threads[i].itemId;
    }
    for (i = 0; i < 6; i++) {
        gSaveData.mingleShop.unk_04[i] = tusinSet->pins[i].pinId;
    }
    gSaveData.mingleShop.giftItemId = tusinSet->slots[8]->itemId;
#ifdef REGION_USA
    gSaveData.unk_2436 = tusinSet->scroll;
    gSaveData.unk_2438 = tusinSet->cursor;
    if (tusinSet->tab == 8) {
        gSaveData.unk_243E = 0;
    } else {
        gSaveData.unk_243E = tusinSet->tab + 1;
    }
#endif
}

s16 TusinSet_GetIconAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {205, 2},
        {230, 2},
    };
    s16 size[2] = {23, 19};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (TusinSet_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 TusinSet_IsPointOnBtn(s16 x, s16 y) {
    s16 pos[2]  = {209, 159};
    s16 size[2] = {46, 32};

    if (TusinSet_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s16 TusinSet_GetTabAtPoint(s16 x, s16 y) {
    s16 posX[8] = {132, 146, 160, 174, 188, 202, 216, 230};
    s16 size[2] = {12, 12};
    s16 i;

    for (i = 0; i < 8; i++) {
        if (TusinSet_IsPointInRect(x, y, posX[i], 83, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 TusinSet_GetPartnerAtPoint(s16 x, s16 y) {
    s16 posX[3] = {125, 143, 161};
    s16 size[2] = {16, 13};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (TusinSet_IsPointInRect(x, y, posX[i], 5, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 TusinSet_GetSlotAtPoint(s16 x, s16 y) {
    s16 offset[2] = {-13, -13};
    s16 size[2]   = {26, 26};
    s16 i;

    for (i = 0; i < 16; i++) {
#ifdef REGION_USA
        s32 shrink = (TusinSet_SlotPositions[i].x == 229) ? 2 : 0;

        if (TusinSet_IsPointInRect(x, y, (s16)(offset[0] + TusinSet_SlotPositions[i].x),
                                   (s16)(offset[1] + TusinSet_SlotPositions[i].y), (s16)(size[0] - shrink), size[1]) == 1)
        {
#else
        if (TusinSet_IsPointInRect(x, y, (s16)(offset[0] + TusinSet_SlotPositions[i].x),
                                   (s16)(offset[1] + TusinSet_SlotPositions[i].y), size[0], size[1]) == 1)
        {
#endif
            return i;
        }
    }
    return -1;
}

s32 TusinSet_IsPointOnSbar(s16 x, s16 y) {
#ifdef REGION_USA
    s16 pos[2]  = {241, 94};
    s16 size[2] = {15, 51};
#else
    s16 pos[2]  = {244, 94};
    s16 size[2] = {12, 51};
#endif

    if (TusinSet_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 TusinSet_IsPointOnSbarKnob(s16 x, s16 y, s16 knobX, s16 knobY) {
#ifdef REGION_USA
    s16 size[2] = {13, 13};
#else
    s16 size[2] = {10, 13};
#endif

#ifdef REGION_USA
    if (TusinSet_IsPointInRect(x, y, (s16)(knobX - 6), (s16)(knobY - 13), size[0], size[1]) == 1) {
#else
    if (TusinSet_IsPointInRect(x, y, (s16)(knobX - 3), (s16)(knobY - 13), size[0], size[1]) == 1) {
#endif
        return 1;
    }
    return 0;
}

s16 TusinSet_GetSbarArrowAtPoint(s16 x, s16 y) {
#ifdef REGION_USA
    s16 pos[2][2] = {
        {241,  80},
        {244, 146},
    };
    s16 size[2] = {15, 9};
#else
    s16 pos[2][2] = {
        {244,  80},
        {244, 146},
    };
    s16 size[2] = {12, 9};
#endif
    s16 i;

    for (i = 0; i < 2; i++) {
        if (TusinSet_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 TusinSet_GetHelpBtnAtPoint(s16 x, s16 y) {
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
        if (TusinSet_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

void TusinSet_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &TusinSet_BinIdentifiers[binIndex]);
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

void TusinSet_LoadBgResourceIndexed(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                    s32 screenIndex, s32 palIndex) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &TusinSet_BinIdentifiers[binIndex]);
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

void TusinSet_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
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

void TusinSet_ReleaseBgResource(MenuBgResource* res, s32 engine) {
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

void TusinSet_ReleaseBgScreen(MenuBgResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
    } else {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
    }
}

void TusinSet_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    TusinSet_ReleaseBgResource(res, engine);
    TusinSet_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void TusinSet_ClearBgResource(MenuBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void TusinSet_LoadBackgrounds(TusinSetObject* tusinSet) {
    s32             i;
    MenuBgResource* subRes  = &tusinSet->resources[0];
    MenuBgResource* mainRes = &tusinSet->resources[4];
    s32             screenIndex;

    for (i = 0; i < 4; i++) {
        TusinSet_ClearBgResource(subRes);
        TusinSet_ClearBgResource(mainRes);
        subRes += 1;
        mainRes += 1;
    }

    TusinSet_LoadBgResource(&tusinSet->resources[6], DISPLAY_MAIN, 2, 1, 15, 1);
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) == 0 || tusinSet->partner == 0xFF) {
        TusinSet_LoadBgResource(&tusinSet->resources[7], DISPLAY_MAIN, 3, 0, 0, 4);
    } else {
        TusinSet_LoadBgResourceIndexed(&tusinSet->resources[7], DISPLAY_MAIN, 3, 0, 0, 4, 2, 0);
    }
    TusinSet_LoadBgScreen(&tusinSet->resources[5], tusinSet->resources[7].data, DISPLAY_MAIN, 1, 5);

    TusinSet_LoadBgResource(&tusinSet->resources[0], DISPLAY_SUB, 0, 6, 15, 1);
    if (tusinSet->partner == 0xFF) {
        TusinSet_LoadBgResourceIndexed(&tusinSet->resources[3], DISPLAY_SUB, 3, 5, 1, 13, 8, 0);
    } else {
        if (tusinSet->partner == 0) {
            screenIndex = 0;
        } else if (tusinSet->partner == 1) {
            screenIndex = 4;
        } else {
            screenIndex = 5;
        }
        TusinSet_LoadBgResourceIndexed(&tusinSet->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex, tusinSet->partner);
    }
    TusinSet_LoadBgScreen(&tusinSet->resources[1], tusinSet->resources[3].data, DISPLAY_SUB, 1, 8);
    TusinSet_LoadBgScreen(&tusinSet->resources[2], tusinSet->resources[3].data, DISPLAY_SUB, 2, 9);
}

void TusinSet_UpdateBackgrounds(TusinSetObject* tusinSet) {}

void TusinSet_ReleaseBackgrounds(TusinSetObject* tusinSet) {
    TusinSet_ReleaseBgScreen(&tusinSet->resources[5], DISPLAY_MAIN);
    TusinSet_ReleaseBgResource(&tusinSet->resources[6], DISPLAY_MAIN);
    TusinSet_ReleaseBgResource(&tusinSet->resources[7], DISPLAY_MAIN);
    TusinSet_ReleaseBgResource(&tusinSet->resources[0], DISPLAY_SUB);
    TusinSet_ReleaseBgScreen(&tusinSet->resources[1], DISPLAY_SUB);
    TusinSet_ReleaseBgScreen(&tusinSet->resources[2], DISPLAY_SUB);
    TusinSet_ReleaseBgResource(&tusinSet->resources[3], DISPLAY_SUB);
}
