#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/FriendList.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save.h"
#include <nitro/mi/cpumem.h>

s32 FriendList_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void FriendList_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

void FriendList_LoadPinData(RawPinData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x3DC0, &FriendList_BinIdentifiers[14]));
}

void FriendList_LoadItemData(RawItemData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x1A40, &FriendList_BinIdentifiers[15]));
}

void FriendList_LoadFoodData(RawFoodData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x348, &FriendList_BinIdentifiers[16]));
}

void FriendList_LoadTreasureData(RawTreasureData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x4B0, &FriendList_BinIdentifiers[17]));
}

BOOL FriendList_IsMacAddressSet(u8* macAddress) {
    for (u16 i = 0; i < 6; i++) {
        if (macAddress[i] != 0xFF) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FriendList_CheckSellable(FriendListObject* friendList, u16 index, RawPinData* pinData, RawItemData* itemData) {
    u16 i;

    for (i = 0; i < 6; i++) {
        u16 pinId = friendList->friends[index].shop.unk_04[i];

        if (pinId != 0xFFFF) {
            u32 price = pinData[pinId].price;

            if (price != 0 && price != 10000000) {
                return TRUE;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        u16 itemId    = friendList->friends[index].shop.unk_10[i];
        u16 itemIndex = Inventory_GetCategorizedIndex(itemId);

        if (itemId != 0xFFFF) {
            u32 price = itemData[itemIndex].unk_04;

            if (price != 0 && price != 10000000) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

s8 FriendList_GetShopIndex(s8 shopId) {
    for (s8 i = 0; i < 35; i++) {
        if (shopId == FriendList_ShopIds[i]) {
            return i;
        }
    }
    return 0;
}

void FriendList_LoadFromSave(FriendListObject* friendList) {
    RawPinData*      pinData;
    RawItemData*     itemData;
    RawFoodData*     foodData;
    RawTreasureData* treasureData;
    ItemCategory     category;
    u16              index, i, j;

    pinData = Mem_AllocHeapTail(&gDebugHeap, 0x3DC0);
    Mem_SetSequence(&gDebugHeap, pinData, "badge_data");
    itemData = Mem_AllocHeapTail(&gDebugHeap, 0x1A40);
    Mem_SetSequence(&gDebugHeap, itemData, "item_data");
    foodData = Mem_AllocHeapTail(&gDebugHeap, 0x348);
    Mem_SetSequence(&gDebugHeap, foodData, "food_data");
    treasureData = Mem_AllocHeapTail(&gDebugHeap, 0x4B0);
    Mem_SetSequence(&gDebugHeap, treasureData, "treasure_data");

    FriendList_LoadPinData(pinData);
    FriendList_LoadItemData(itemData);
    FriendList_LoadFoodData(foodData);
    FriendList_LoadTreasureData(treasureData);

    for (i = 0; i < 50; i++) {
        MI_CpuCopyU8(friendList->friendData->unk_0000[i].unk_00, friendList->friends[i].macAddress, 6);
        MI_CpuCopyU8(friendList->friendData->unk_0000[i].nickName, friendList->friends[i].nickName, 20);
        MI_CpuCopyU8(friendList->friendData->unk_0000[i].message, friendList->friends[i].message, 52);
        friendList->friends[i].timesMet   = friendList->friendData->unk_0000[i].timesMet;
        friendList->friends[i].lastMet    = friendList->friendData->unk_0000[i].lastMet;
        friendList->friends[i].experience = friendList->friendData->unk_0000[i].experience;
        friendList->friends[i].shop       = friendList->friendData->unk_0000[i].shop;

        for (j = 0; j < 6; j++) {
            friendList->friends[i].pinIcons[j] = friendList->friends[i].shop.unk_04[j];
        }

        for (j = 0; j < 16; j++) {
            index    = Inventory_GetCategorizedIndex(friendList->friends[i].shop.unk_10[j]);
            category = Inventory_GetCategory(friendList->friends[i].shop.unk_10[j]);
            if (index == 0xFFFF) {
                friendList->friends[i].itemIcons[j] = 0xFFFF;
            } else {
                switch (category) {
                    case ITEM_CATEGORY_THREAD:
                        friendList->friends[i].itemIcons[j] = itemData[index].unk_00;
                        break;
                    case ITEM_CATEGORY_FOOD:
                        friendList->friends[i].itemIcons[j] = foodData[index].unk_00;
                        break;
                    case ITEM_CATEGORY_SWAG:
                        friendList->friends[i].itemIcons[j] = treasureData[index].unk_00;
                        break;
                    default:
                        OS_WaitForever();
                        break;
                }
            }
        }

        index    = Inventory_GetCategorizedIndex(friendList->friends[i].shop.giftItemId);
        category = Inventory_GetCategory(friendList->friends[i].shop.giftItemId);
        if (index == 0xFFFF) {
            friendList->friends[i].giftIcon = 0xFFFF;
        } else {
            switch (category) {
                case ITEM_CATEGORY_THREAD:
                    friendList->friends[i].giftIcon = itemData[index].unk_00;
                    break;
                case ITEM_CATEGORY_FOOD:
                    friendList->friends[i].giftIcon = foodData[index].unk_00;
                    break;
                case ITEM_CATEGORY_SWAG:
                    friendList->friends[i].giftIcon = treasureData[index].unk_00;
                    break;
                default:
                    OS_WaitForever();
                    break;
            }
        }

        friendList->friends[i].hasSellable = FriendList_CheckSellable(friendList, i, pinData, itemData);
    }

#ifdef REGION_USA
    friendList->lastScroll = friendList->scroll = gSaveData.unk_2432;

    if (FriendList_IsMacAddressSet(friendList->friends[friendList->cursor = gSaveData.unk_2434].macAddress) == FALSE) {
        for (i = 0; i < 16; i++) {
            friendList->threads[i].itemId       = 0xFFFF;
            friendList->threads[i].graphicIndex = 0xFFFF;
        }
        friendList->gift.itemId       = 0xFFFF;
        friendList->gift.graphicIndex = 0xFFFF;
        for (i = 0; i < 6; i++) {
            friendList->pins[i].itemId       = 0xFFFF;
            friendList->pins[i].graphicIndex = 0xFFFF;
        }
        friendList->esperPoints      = 0;
        friendList->noiseReportCount = 0;
        friendList->pinsMastered     = 0;
        friendList->itemsCollected   = 0;
        friendList->timeAttackFrames = 0;
    } else {
        for (i = 0; i < 4; i++) {
            friendList->threads[i].itemId       = friendList->friends[friendList->cursor].shop.unk_10[i];
            friendList->threads[i].graphicIndex = friendList->friends[friendList->cursor].itemIcons[i];
        }
        for (i = 4; i < 16; i++) {
            friendList->threads[i].itemId       = friendList->friends[friendList->cursor].shop.unk_10[i];
            friendList->threads[i].graphicIndex = friendList->friends[friendList->cursor].itemIcons[i];
        }
        friendList->gift.itemId       = friendList->friends[friendList->cursor].shop.giftItemId;
        friendList->gift.graphicIndex = friendList->friends[friendList->cursor].giftIcon;
        for (i = 0; i < 6; i++) {
            friendList->pins[i].itemId       = friendList->friends[friendList->cursor].shop.unk_04[i];
            friendList->pins[i].graphicIndex = friendList->friends[friendList->cursor].pinIcons[i];
        }
        friendList->esperPoints      = friendList->friends[friendList->cursor].experience.current;
        friendList->noiseReportCount = friendList->friends[friendList->cursor].experience.unk_0_0;
        friendList->pinsMastered     = friendList->friends[friendList->cursor].experience.pinCount;
        friendList->itemsCollected   = friendList->friends[friendList->cursor].experience.itemCount;
        friendList->timeAttackFrames = friendList->friends[friendList->cursor].experience.unk_8;
    }
#else
    if (FriendList_IsMacAddressSet(friendList->friends[0].macAddress) == FALSE) {
        for (i = 0; i < 16; i++) {
            friendList->threads[i].itemId       = 0xFFFF;
            friendList->threads[i].graphicIndex = 0xFFFF;
        }
        friendList->gift.itemId       = 0xFFFF;
        friendList->gift.graphicIndex = 0xFFFF;
        for (i = 0; i < 6; i++) {
            friendList->pins[i].itemId       = 0xFFFF;
            friendList->pins[i].graphicIndex = 0xFFFF;
        }
        friendList->esperPoints      = 0;
        friendList->noiseReportCount = 0;
        friendList->pinsMastered     = 0;
        friendList->itemsCollected   = 0;
        friendList->timeAttackFrames = 0;
    } else {
        for (i = 0; i < 4; i++) {
            friendList->threads[i].itemId       = friendList->friends[0].shop.unk_10[i];
            friendList->threads[i].graphicIndex = friendList->friends[0].itemIcons[i];
        }
        for (i = 4; i < 16; i++) {
            friendList->threads[i].itemId       = friendList->friends[0].shop.unk_10[i];
            friendList->threads[i].graphicIndex = friendList->friends[0].itemIcons[i];
        }
        friendList->gift.itemId       = friendList->friends[0].shop.giftItemId;
        friendList->gift.graphicIndex = friendList->friends[0].giftIcon;
        for (i = 0; i < 6; i++) {
            friendList->pins[i].itemId       = friendList->friends[0].shop.unk_04[i];
            friendList->pins[i].graphicIndex = friendList->friends[0].pinIcons[i];
        }
        friendList->esperPoints      = friendList->friends[0].experience.current;
        friendList->noiseReportCount = friendList->friends[0].experience.unk_0_0;
        friendList->pinsMastered     = friendList->friends[0].experience.pinCount;
        friendList->itemsCollected   = friendList->friends[0].experience.itemCount;
        friendList->timeAttackFrames = friendList->friends[0].experience.unk_8;
    }

    for (i = 0; i < 3; i++) {
        friendList->rowFriends[i] = &friendList->friends[i];
    }
#endif

    for (i = 0; i < 4; i++) {
        friendList->slots[i] = &friendList->threads[i];
    }
    for (i = 0; i < 4; i++) {
        friendList->slots[i + 4] = &friendList->threads[i + 4];
    }
    friendList->slots[8] = &friendList->gift;
    for (i = 0; i < 6; i++) {
        friendList->pinSlots[i] = &friendList->pins[i];
    }

    friendList->badgeSlots = 6;
    friendList->partner    = 0;
    friendList->shopId     = gSaveData.mingleShop.shopId;
    friendList->shopIndex  = FriendList_GetShopIndex(friendList->shopId);
    friendList->clerkId    = gSaveData.mingleShop.clerkId;
    friendList->musicId    = gSaveData.mingleShop.musicId;

    Mem_Free(&gDebugHeap, pinData);
    Mem_Free(&gDebugHeap, itemData);
    Mem_Free(&gDebugHeap, foodData);
    Mem_Free(&gDebugHeap, treasureData);
}

void FriendList_WriteToSave(FriendListObject* friendList) {
    gSaveData.mingleShop.shopId  = friendList->shopId;
    gSaveData.mingleShop.clerkId = friendList->clerkId;
    gSaveData.mingleShop.musicId = friendList->musicId;
#ifdef REGION_USA
    gSaveData.unk_2432 = 0;
    gSaveData.unk_2434 = 0;
#endif
}

void FriendList_WriteToSaveForShop(FriendListObject* friendList) {
    gSaveData.mingleShop.shopId  = friendList->shopId;
    gSaveData.mingleShop.clerkId = friendList->clerkId;
    gSaveData.mingleShop.musicId = friendList->musicId;
    gSaveData.unk_1AF2           = friendList->friends[friendList->cursor].shop;
    gSaveData.unk_1AB9           = friendList->cursor;
#ifdef REGION_USA
    gSaveData.unk_2432 = friendList->scroll;
    gSaveData.unk_2434 = friendList->cursor;
#endif
}

s16 FriendList_GetIconAtPoint(s16 x, s16 y) {
    s16 pos[3][2] = {
        {180, 2},
        {205, 2},
        {230, 2},
    };
    s16 size[2] = {23, 19};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (FriendList_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 FriendList_GetPartnerAtPoint(s16 x, s16 y) {
    s16 posX[3] = {125, 143, 161};
    s16 size[2] = {16, 13};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (FriendList_IsPointInRect(x, y, posX[i], 5, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 FriendList_GetRowAtPoint(s16 x, s16 y) {
    s16 posY[3] = {26, 74, 122};
    s16 size[2] = {236, 46};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (FriendList_IsPointInRect(x, y, 5, posY[i], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 FriendList_GetRowBtnAtPoint(s16 x, s16 y) {
    s16 posY[3] = {33, 81, 129};
    s16 size[2] = {52, 12};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (FriendList_IsPointInRect(x, y, 188, posY[i], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 FriendList_IsSellable(FriendListObject* friendList, u16 index) {
    if (friendList->friends[index].hasSellable == 1) {
        return TRUE;
    }
    return FALSE;
}

s32 FriendList_IsPointOnBtn(s16 x, s16 y) {
    s16 pos[2]  = {32, 155};
    s16 size[2] = {62, 16};

    if (FriendList_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s16 FriendList_GetArrowAtPoint(s16 x, s16 y) {
    Point positions[6] = {
        {210,  39},
        {210,  79},
        {210, 119},
        {210,  66},
        {210, 106},
        {210, 146},
    };
    s16 size[2] = {13, 15};
    s16 i;

    for (i = 0; i < 6; i++) {
        if (FriendList_IsPointInRect(x, y, positions[i].x, positions[i].y, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 FriendList_IsPointOnSbar(s16 x, s16 y) {
#ifdef REGION_USA
    s16 pos[2]  = {241, 36};
    s16 size[2] = {15, 122};
#else
    s16 pos[2]  = {244, 36};
    s16 size[2] = {12, 122};
#endif

    if (FriendList_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 FriendList_IsPointOnSbarKnob(s16 x, s16 y, s16 knobX, s16 knobY) {
#ifdef REGION_USA
    s16 size[2] = {13, 38};
#else
    s16 size[2] = {10, 38};
#endif

#ifdef REGION_USA
    if (FriendList_IsPointInRect(x, y, (s16)(knobX - 6), (s16)(knobY - 38), size[0], size[1]) == 1) {
#else
    if (FriendList_IsPointInRect(x, y, (s16)(knobX - 3), (s16)(knobY - 38), size[0], size[1]) == 1) {
#endif
        return 1;
    }
    return 0;
}

s16 FriendList_GetSbarArrowAtPoint(s16 x, s16 y) {
#ifdef REGION_USA
    Point pos[2] = {
        {241,  23},
        {241, 161},
    };
    s16 size[2] = {15, 9};
#else
    Point pos[2] = {
        {244,  23},
        {244, 161},
    };
    s16 size[2] = {12, 9};
#endif
    s16 i;

    for (i = 0; i < 2; i++) {
        if (FriendList_IsPointInRect(x, y, pos[i].x, pos[i].y, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 FriendList_GetHelpBtnAtPoint(s16 x, s16 y) {
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
        if (FriendList_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

void FriendList_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &FriendList_BinIdentifiers[binIndex]);
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

void FriendList_LoadBgResourceIndexed(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                      s32 screenIndex, s32 palIndex) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &FriendList_BinIdentifiers[binIndex]);
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

void FriendList_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
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

void FriendList_ReleaseBgResource(MenuBgResource* res, s32 engine) {
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

void FriendList_ReleaseBgScreen(MenuBgResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
    } else {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
    }
}

void FriendList_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    FriendList_ReleaseBgResource(res, engine);
    FriendList_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void FriendList_ClearBgResource(MenuBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void FriendList_LoadBackgrounds(FriendListObject* friendList) {
    s32             i;
    MenuBgResource* subRes  = &friendList->resources[0];
    MenuBgResource* mainRes = &friendList->resources[4];
    s32             screenIndex;

    for (i = 0; i < 4; i++) {
        FriendList_ClearBgResource(subRes);
        FriendList_ClearBgResource(mainRes);
        subRes += 1;
        mainRes += 1;
    }

    FriendList_LoadBgResource(&friendList->resources[5], DISPLAY_MAIN, 1, 1, 15, 1);
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) == 0 || friendList->partner == 0xFF) {
        FriendList_LoadBgResource(&friendList->resources[7], DISPLAY_MAIN, 3, 0, 0, 4);
    } else {
        FriendList_LoadBgResourceIndexed(&friendList->resources[7], DISPLAY_MAIN, 3, 0, 0, 4, 2, 0);
    }
    FriendList_LoadBgScreen(&friendList->resources[6], friendList->resources[7].data, DISPLAY_MAIN, 2, 5);

    FriendList_LoadBgResource(&friendList->resources[0], DISPLAY_SUB, 0, 6, 15, 1);
    if (friendList->partner == 0xFF) {
        FriendList_LoadBgResourceIndexed(&friendList->resources[3], DISPLAY_SUB, 3, 5, 1, 13, 8, 0);
    } else {
        if (friendList->partner == 0) {
            screenIndex = 0;
        } else if (friendList->partner == 1) {
            screenIndex = 4;
        } else {
            screenIndex = 5;
        }
        FriendList_LoadBgResourceIndexed(&friendList->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex,
                                         friendList->partner);
    }
    FriendList_LoadBgScreen(&friendList->resources[1], friendList->resources[3].data, DISPLAY_SUB, 1, 8);
    FriendList_LoadBgScreen(&friendList->resources[2], friendList->resources[3].data, DISPLAY_SUB, 2, 9);
}

void FriendList_UpdateBackgrounds(FriendListObject* friendList) {}

void FriendList_ReleaseBackgrounds(FriendListObject* friendList) {
    FriendList_ReleaseBgResource(&friendList->resources[5], DISPLAY_MAIN);
    FriendList_ReleaseBgScreen(&friendList->resources[6], DISPLAY_MAIN);
    FriendList_ReleaseBgResource(&friendList->resources[7], DISPLAY_MAIN);
    FriendList_ReleaseBgResource(&friendList->resources[0], DISPLAY_SUB);
    FriendList_ReleaseBgScreen(&friendList->resources[1], DISPLAY_SUB);
    FriendList_ReleaseBgScreen(&friendList->resources[2], DISPLAY_SUB);
    FriendList_ReleaseBgResource(&friendList->resources[3], DISPLAY_SUB);
}
