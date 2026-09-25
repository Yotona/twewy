#include "CriSndMgr.h"
#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Interface/Menu/MenuEquip.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/rtc.h>

BgResource* BgResMgr_AllocChar32(BgResMgr* mgr, void* charData, u32 charBase, u32 offset, u32 size);
BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* screenData, u32 screenBase, u32 screenSize);
s32         func_02041680(RTCDate* date);
void        func_02047ec8(void* head, u32 num, u32 width, s32 (*compare)(u16*, u16*), void* buffer);

MenuEquipItemEntry data_ov043_020cc2d8 = {
    .itemId      = 0xFFFF,
    .subCategory = 0xF,
    .brand       = 0xE,
};

// NitroSDK MATH_CountLeadingZeros / MATH_ILog2 / MATH_QSortStackSize
static inline u32 MATH_CountLeadingZeros(register u32 x) {
    asm { clz x, x }
    return x;
}

static inline s32 MATH_ILog2(u32 x) {
    return (s32)(31 - MATH_CountLeadingZeros(x));
}

static inline u32 MATH_QSortStackSize(u32 num) {
    s32 tmp = MATH_ILog2(num);

    if (tmp <= 0) {
        return sizeof(s32);
    } else {
        return (u32)((MATH_ILog2(num) + 1) * sizeof(s32) * 2);
    }
}

s32 MenuEquip_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void MenuEquip_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

void MenuEquip_SetSpriteFrameFromPack(Sprite* sprite, s16 frame, s32 animIndex, s32 frameDataIndex) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, animIndex);
    void* frames = Data_GetPackEntryData(sprite->resourceData, frameDataIndex);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

void MenuEquip_SwapItemEntries(MenuEquipItemEntry* a, MenuEquipItemEntry* b) {
    MenuEquipItemEntry temp;

    temp = *a;
    *a   = *b;
    *b   = temp;
}

void MenuEquip_LoadItemData(RawItemData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x1A40, &data_ov043_020c9758[12]));
}

void MenuEquip_LoadFoodData(RawFoodData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x348, &data_ov043_020c9758[13]));
}

void MenuEquip_LoadTreasureData(RawTreasureData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x4B0, &data_ov043_020c9758[14]));
}

u16 MenuEquip_GetFreePaletteSlot(void) {
    u16 i;
    PalSlot(*slots)[16] = g_PaletteManagers[DISPLAY_EXTENDED]->slots;

    for (i = 0; i < 16; i++) {
        if (slots[6][i].flags == 0) {
            return 6;
        }
    }
    return 5;
}

s32 MenuEquip_IsCdItem(u32 itemId) {
    if ((itemId >= 0x285) && (itemId <= 0x2A6)) {
        return 1;
    }
    return 0;
}

void MenuEquip_PlayCdTrack(u16 itemId) {
    s32 adxTable[34] = {
        1,    2,    4,    5,    6,    7,    3,    8,    9,    0xA,  0xB,  0xC,  0xD,  0xE, 0xF,  0x10, 0x11,
        0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0,   0x1F, 0x20, 0x21,
    };
    u16 adx = adxTable[itemId - 0x285];

    CriSndMgr_PlayFile(adx);
    gSaveData.unk_2430 = adx;
}

u32 MenuEquip_CalcEquippedBravery(MenuEquipObject* owner, u16 character) {
    u32 sum = 0;

    for (u16 i = 0; i < 4; i++) {
        sum = (u16)(sum + owner->equipped[character][i].braveryReq);
    }
    if (sum > 999) {
        sum = 999;
    }
    return sum;
}

s32 MenuEquip_CalcMaxBravery(u16 bravery, u16 level) {
    u16 total, result;

    total  = (bravery + level) - 1;
    result = total;
    if (total > 999) {
        result = 999;
    }
    return result;
}

void MenuEquip_AddSwagBonus(MenuEquipObject* owner, s32 itemId, u8 count) {
    switch (itemId) {
        case 0x2B2:
            owner->swagAttackShiki += count;
            return;
        case 0x2B3:
            owner->swagDefenseShiki += count;
            return;
        case 0x2BA:
            owner->swagAttackJoshua += count;
            return;
        case 0x2BB:
            owner->swagDefenseJoshua += count;
            return;
        case 0x2C2:
            owner->swagAttackBeat += count;
            return;
        case 0x2C3:
            owner->swagDefenseBeat += count;
            return;
    }
}

s32 MenuEquip_GetAbilityAttackBonus(s32 kind, u16 character) {
    s32 bonus = 0;

    switch (kind) {
        case 0xA5:
            if (character == 0) {
                bonus = 1;
            }
            break;
        case 0xA6:
            if (character == 0) {
                bonus = 3;
            }
            break;
        case 0xA7:
            if (character == 0) {
                bonus = 5;
            }
            break;
        case 0xA8:
            if (character == 1) {
                bonus = 1;
            }
            break;
        case 0xA9:
            if (character == 1) {
                bonus = 2;
            }
            break;
        case 0xAA:
            if (character == 2) {
                bonus = 2;
            }
            break;
        case 0xAB:
            if (character == 2) {
                bonus = 3;
            }
            break;
        case 0xAC:
            if (character == 3) {
                bonus = 3;
            }
            break;
        case 0xAD:
            if (character == 3) {
                bonus = 4;
            }
            break;
        case 0xAE:
            bonus = 5;
            break;
        case 0xAF:
            bonus = 1;
            break;
        case 0xB0:
            bonus = 2;
            break;
        case 0xB1:
            bonus = 3;
            break;
        case 0xB2:
            bonus = 10;
            break;
        case 0xCD:
            bonus = 2;
            break;
        case 0xCE:
            if (character == 0) {
                bonus = 3;
            }
            break;
    }
    return bonus;
}

s32 MenuEquip_GetAbilityDefenseBonus(u8 kind, u16 character) {
    s32 bonus = 0;

    switch (kind) {
        case 0xB3:
            if (character == 0) {
                bonus = 1;
            }
            break;
        case 0xB4:
            if (character == 0) {
                bonus = 3;
            }
            break;
        case 0xB5:
            if (character == 0) {
                bonus = 5;
            }
            break;
        case 0xB6:
            if (character == 1) {
                bonus = 1;
            }
            break;
        case 0xB7:
            if (character == 1) {
                bonus = 2;
            }
            break;
        case 0xB8:
            if (character == 2) {
                bonus = 2;
            }
            break;
        case 0xB9:
            if (character == 2) {
                bonus = 3;
            }
            break;
        case 0xBA:
            if (character == 3) {
                bonus = 3;
            }
            break;
        case 0xBB:
            if (character == 3) {
                bonus = 4;
            }
            break;
        case 0xBC:
            bonus = 5;
            break;
        case 0xBD:
            bonus = 1;
            break;
        case 0xBE:
            bonus = 2;
            break;
        case 0xBF:
            bonus = 3;
            break;
        case 0xCD:
            bonus = 2;
            break;
        case 0xCE:
            if (character == 0) {
                bonus = 3;
            }
            break;
    }
    return bonus;
}

s32 MenuEquip_GetAbilityHealthBonus(u8 kind, u32 character) {
    s32 bonus = 0;

    switch (kind) {
        case 0xC0:
            if (character == 0) {
                bonus = 20;
            }
            break;
        case 0xC1:
            if (character == 0) {
                bonus = 40;
            }
            break;
        case 0xC2:
            if (character == 0) {
                bonus = 70;
            }
            break;
        case 0xC3:
            if (character == 1) {
                bonus = 10;
            }
            break;
        case 0xC4:
            if (character == 1) {
                bonus = 20;
            }
            break;
        case 0xC5:
            if (character == 2) {
                bonus = 20;
            }
            break;
        case 0xC6:
            if (character == 2) {
                bonus = 40;
            }
            break;
        case 0xC7:
            if (character == 3) {
                bonus = 30;
            }
            break;
        case 0xC8:
            if (character == 3) {
                bonus = 70;
            }
            break;
        case 0xC9:
            bonus = 100;
            break;
        case 0xCA:
            bonus = 5;
            break;
        case 0xCB:
            bonus = 15;
            break;
        case 0xCC:
            bonus = 35;
            break;
        case 0xCD:
            bonus = 85;
            break;
        case 0xCE:
            if (character == 0) {
                bonus = 50;
            }
            break;
    }
    return bonus;
}

s32 MenuEquip_CalcAttackBonus(MenuEquipObject* owner, u16 character) {
    u16                 i;
    s16                 sum;
    MenuEquipItemEntry* item;

    sum = 0;
    for (i = 0; i < 4; i++) {
        item = &owner->equipped[character][i];
        if (item->itemId != 0xFFFF) {
            sum += item->attackBonus;
            if (item->abilityUnlocked == 1) {
                sum += MenuEquip_GetAbilityAttackBonus(item->abilityId, character);
            }
        }
    }
    if (character == 1) {
        sum += owner->swagAttackShiki;
    } else if (character == 2) {
        sum += owner->swagAttackJoshua;
    } else if (character == 3) {
        sum += owner->swagAttackBeat;
    }
    if (sum > 999) {
        sum = 999;
    }
    return sum;
}

s32 MenuEquip_CalcDefenseBonus(MenuEquipObject* owner, u16 character) {
    u16                 i;
    s16                 sum;
    MenuEquipItemEntry* item;

    sum = 0;
    for (i = 0; i < 4; i++) {
        item = &owner->equipped[character][i];
        if (item->itemId != 0xFFFF) {
            sum += item->defenseBonus;
            if (item->abilityUnlocked == 1) {
                sum += MenuEquip_GetAbilityDefenseBonus(item->abilityId, character);
            }
        }
    }
    if (character == 1) {
        sum += owner->swagDefenseShiki;
    } else if (character == 2) {
        sum += owner->swagDefenseJoshua;
    } else if (character == 3) {
        sum += owner->swagDefenseBeat;
    }
    if (sum > 999) {
        sum = 999;
    }
    return sum;
}

s32 MenuEquip_CalcHealthBonus(MenuEquipObject* owner, u32 character) {
    u16                 i;
    s16                 sum;
    MenuEquipItemEntry* item;

    sum = 0;
    for (i = 0; i < 4; i++) {
        item = &owner->equipped[0][i];
        if (item->itemId != 0xFFFF) {
            sum += item->healthBonus;
            if (item->abilityUnlocked == 1) {
                sum += MenuEquip_GetAbilityHealthBonus(item->abilityId, 0);
            }
        }
    }
    if (character <= 3) {
        for (i = 0; i < 4; i++) {
            item = &owner->equipped[character][i];
            if (item->itemId != 0xFFFF) {
                sum += item->healthBonus;
                if (item->abilityUnlocked == 1) {
                    sum += MenuEquip_GetAbilityHealthBonus(item->abilityId, character);
                }
            }
        }
    }
    if (sum > 9999) {
        sum = 9999;
    }
    return sum;
}

void MenuEquip_ApplySyncGain(MenuEquipObject* owner, u8 character) {
    s16 partner  = owner->activePartner;
    s8  taste    = owner->cursorItem.taste[character];
    u16 syncGain = owner->cursorItem.syncGain;
    s32 rate;
    u16 gain;

    if (partner == 0xFF) {
        return;
    }
    switch (taste) {
        case 0:
            rate = 50;
            break;
        case 1:
            rate = 75;
            break;
        case 2:
            rate = 100;
            break;
        case 3:
            rate = 150;
            break;
        default:
            rate = 200;
            break;
    }
    gain = (syncGain * rate) / 10;
    owner->stats[partner + 1].sync += gain;
    if (owner->stats[partner + 1].sync > 1000) {
        owner->stats[partner + 1].sync = 1000;
    }
}

s32 MenuEquip_CanEquipSubCategory(MenuEquipObject* owner, u16 character, u8 subCategory, u8 replacedSubCategory) {
    u16 i;

    if (subCategory == replacedSubCategory) {
        return 1;
    }
    for (i = 0; i < 4; i++) {
        if ((owner->equipped[character][i].itemId != 0xFFFF) && (subCategory == owner->equipped[character][i].subCategory)) {
            return 0;
        }
    }
    return 1;
}

void MenuEquip_InitStats(MenuEquipObject* owner) {
    u16 i;

    owner->stats[0].health      = gSaveData.playerStats.baseHealth + (s16)(((s16)gSaveData.playerStats.unk_04 - 1) * 50 + 200);
    owner->stats[0].healthBonus = MenuEquip_CalcHealthBonus(owner, (u16)(gSaveData.playerStats.activePartner + 1));
    owner->stats[0].sync        = 1;
    owner->stats[0].braveryBonus     = 0;
    owner->stats[0].bravery          = MenuEquip_CalcMaxBravery(gSaveData.playerStats.bravery, gSaveData.playerStats.level);
    owner->stats[0].attack           = gSaveData.playerStats.attack;
    owner->stats[0].attackBonus      = MenuEquip_CalcAttackBonus(owner, 0);
    owner->stats[0].defense          = gSaveData.playerStats.defense;
    owner->stats[0].defenseBonus     = MenuEquip_CalcDefenseBonus(owner, 0);
    owner->stats[0].foodCapacityUsed = gSaveData.playerStats.foodBytesLeft;
    owner->stats[0].foodCapacityLeft = gSaveData.playerStats.foodCapacityLeft;
    for (i = 0; i < 3; i++) {
        owner->stats[i + 1].health       = 0;
        owner->stats[i + 1].healthBonus  = 0;
        owner->stats[i + 1].sync         = gSaveData.friendStats[i].sync;
        owner->stats[i + 1].braveryBonus = 0;
        owner->stats[i + 1].bravery = MenuEquip_CalcMaxBravery(gSaveData.friendStats[i].bravery, gSaveData.playerStats.level);
        owner->stats[i + 1].attack  = gSaveData.friendStats[i].attack;
        owner->stats[i + 1].attackBonus      = MenuEquip_CalcAttackBonus(owner, i + 1);
        owner->stats[i + 1].defense          = gSaveData.friendStats[i].defense;
        owner->stats[i + 1].defenseBonus     = MenuEquip_CalcDefenseBonus(owner, i + 1);
        owner->stats[i + 1].foodCapacityUsed = gSaveData.friendStats[i].foodBytesLeft;
        owner->stats[i + 1].foodCapacityLeft = gSaveData.friendStats[i].foodCapacityLeft;
    }
}

void MenuEquip_UpdateStatBonuses(MenuEquipObject* owner) {
    u16 i;

    owner->stats[0].healthBonus  = MenuEquip_CalcHealthBonus(owner, (u16)(owner->activePartner + 1));
    owner->stats[0].braveryBonus = MenuEquip_CalcEquippedBravery(owner, 0);
    owner->stats[0].attackBonus  = MenuEquip_CalcAttackBonus(owner, 0);
    owner->stats[0].defenseBonus = MenuEquip_CalcDefenseBonus(owner, 0);
    for (i = 0; i < 3; i++) {
        owner->stats[i + 1].braveryBonus = MenuEquip_CalcEquippedBravery(owner, i + 1);
        owner->stats[i + 1].attackBonus  = MenuEquip_CalcAttackBonus(owner, i + 1);
        owner->stats[i + 1].defenseBonus = MenuEquip_CalcDefenseBonus(owner, i + 1);
    }
}

void MenuEquip_BuildItemEntries(MenuEquipObject* owner) {
    u16                 character;
    MenuEquipItemEntry* entry;
    u16                 itemId, index, i;

    RawItemData* itemData = Mem_AllocHeapTail(&gDebugHeap, 0x1A40);
    Mem_SetSequence(&gDebugHeap, itemData, "item_data");

    RawFoodData* foodData = Mem_AllocHeapTail(&gDebugHeap, 0x348);
    Mem_SetSequence(&gDebugHeap, foodData, "food_data");

    RawTreasureData* treasureData = Mem_AllocHeapTail(&gDebugHeap, 0x4B0);
    Mem_SetSequence(&gDebugHeap, treasureData, "treasure_data");

    MenuEquip_LoadItemData(itemData);
    MenuEquip_LoadFoodData(foodData);
    MenuEquip_LoadTreasureData(treasureData);

    for (i = 0; i < 4; i++) {
        itemId = gSaveData.playerStats.equippedThreads[i];
        if (itemId == 0xFFFF) {
            owner->equipped[0][i] = data_ov043_020cc2d8;
        } else {
            index = Inventory_GetCategorizedIndex(itemId);
            entry = &owner->equipped[0][i];

            entry->itemId          = itemId;
            entry->graphicIndex    = itemData[index].unk_00;
            entry->category        = ITEM_CATEGORY_THREAD;
            entry->subCategory     = itemData[index].unk_03;
            entry->capacityCost    = 0;
            entry->braveryReq      = itemData[index].unk_08;
            entry->attackBonus     = itemData[index].attack;
            entry->defenseBonus    = itemData[index].defense;
            entry->healthBonus     = itemData[index].health;
            entry->count           = 1;
            entry->unk_11          = 0;
            entry->abilityUnlocked = (u32)(gSaveData.unk_1EB2[index] << 0x1F) >> 0x1F;
            entry->brand           = itemData[index].brand;
            entry->unk_14          = itemData[index].unk_04;
            entry->abilityId       = itemData[index].unk_10;
            entry->unk_1A          = itemData[index].unk_12;
            entry->unk_1C          = itemData[index].unk_14;
            entry->foodEffectMsg   = 0;
            entry->foodEffectValue = 0;
            entry->syncGain        = 0;
            entry->taste[0]        = 0;
            entry->taste[1]        = 0;
            entry->taste[2]        = 0;
            entry->taste[3]        = 0;
        }
    }

    for (character = 0; character < 3; character++) {
        for (i = 0; i < 4; i++) {
            itemId = gSaveData.friendStats[character].equippedThreads[i];
            index  = Inventory_GetCategorizedIndex(itemId);
            if (itemId == 0xFFFF) {
                owner->equipped[character + 1][i] = data_ov043_020cc2d8;
            } else {
                entry                  = &owner->equipped[character + 1][i];
                entry->itemId          = itemId;
                entry->graphicIndex    = itemData[index].unk_00;
                entry->category        = ITEM_CATEGORY_THREAD;
                entry->subCategory     = itemData[index].unk_03;
                entry->capacityCost    = 0;
                entry->braveryReq      = itemData[index].unk_08;
                entry->attackBonus     = itemData[index].attack;
                entry->defenseBonus    = itemData[index].defense;
                entry->healthBonus     = itemData[index].health;
                entry->count           = 1;
                entry->unk_11          = 0;
                entry->abilityUnlocked = (u32)(gSaveData.unk_1EB2[index] << 0x1F) >> 0x1F;
                entry->brand           = itemData[index].brand;
                entry->unk_14          = itemData[index].unk_04;
                entry->abilityId       = itemData[index].unk_10;
                entry->unk_1A          = itemData[index].unk_12;
                entry->unk_1C          = itemData[index].unk_14;
                entry->foodEffectMsg   = 0;
                entry->foodEffectValue = 0;
                entry->syncGain        = 0;
                entry->taste[0]        = 0;
                entry->taste[1]        = 0;
                entry->taste[2]        = 0;
                entry->taste[3]        = 0;
            }
        }
    }

    itemId = gSaveData.playerStats.foodItem;
    index  = Inventory_GetCategorizedIndex(itemId);
    if (itemId == 0xFFFF) {
        owner->equipped[0][4] = data_ov043_020cc2d8;
    } else {
        entry                  = &owner->equipped[0][4];
        entry->itemId          = itemId;
        entry->graphicIndex    = foodData[index].unk_00;
        entry->category        = ITEM_CATEGORY_FOOD;
        entry->subCategory     = 0;
        entry->capacityCost    = gSaveData.playerStats.foodBytesLeft;
        entry->braveryReq      = 0;
        entry->attackBonus     = 0;
        entry->defenseBonus    = 0;
        entry->healthBonus     = 0;
        entry->count           = 1;
        entry->unk_11          = 0;
        entry->abilityUnlocked = 0;
        entry->brand           = 0xE;
        entry->unk_14          = foodData[index].unk_04;
        entry->abilityId       = 0;
        entry->unk_1A          = 0;
        entry->unk_1C          = 0;
        entry->foodEffectMsg   = foodData[index].stat;
        entry->foodEffectValue = foodData[index].statBonus;
        entry->syncGain        = foodData[index].unk_0C;
        entry->taste[0]        = foodData[index].unk_10[0];
        entry->taste[1]        = foodData[index].unk_10[1];
        entry->taste[2]        = foodData[index].unk_10[2];
        entry->taste[3]        = foodData[index].unk_10[3];
    }

    for (i = 0; i < 3; i++) {
        itemId = gSaveData.friendStats[i].foodItem;
        index  = Inventory_GetCategorizedIndex(itemId);
        if (itemId == 0xFFFF) {
            owner->equipped[i + 1][4] = data_ov043_020cc2d8;
        } else {
            entry                  = &owner->equipped[i + 1][4];
            entry->itemId          = itemId;
            entry->graphicIndex    = foodData[index].unk_00;
            entry->category        = ITEM_CATEGORY_FOOD;
            entry->subCategory     = 0;
            entry->capacityCost    = gSaveData.friendStats[i].foodBytesLeft;
            entry->braveryReq      = 0;
            entry->attackBonus     = 0;
            entry->defenseBonus    = 0;
            entry->healthBonus     = 0;
            entry->count           = 1;
            entry->unk_11          = 0;
            entry->abilityUnlocked = 0;
            entry->brand           = 0xE;
            entry->unk_14          = foodData[index].unk_04;
            entry->abilityId       = 0;
            entry->unk_1A          = 0;
            entry->unk_1C          = 0;
            entry->foodEffectMsg   = foodData[index].stat;
            entry->foodEffectValue = foodData[index].statBonus;
            entry->syncGain        = foodData[index].unk_0C;
            entry->taste[0]        = foodData[index].unk_10[0];
            entry->taste[1]        = foodData[index].unk_10[1];
            entry->taste[2]        = foodData[index].unk_10[2];
            entry->taste[3]        = foodData[index].unk_10[3];
        }
    }

    for (i = 0; i < 472; i++) {
        itemId = gSaveData.inventoryItems[i].itemID;
        if (itemId == 0xFFFF) {
            owner->inventory[i] = data_ov043_020cc2d8;
        } else {
            index = Inventory_GetCategorizedIndex(itemId);
            switch (Inventory_GetCategory(itemId)) {
                case ITEM_CATEGORY_THREAD:
                    entry                  = &owner->inventory[i];
                    entry->itemId          = itemId;
                    entry->graphicIndex    = itemData[index].unk_00;
                    entry->category        = ITEM_CATEGORY_THREAD;
                    entry->subCategory     = itemData[index].unk_03;
                    entry->capacityCost    = 0;
                    entry->braveryReq      = itemData[index].unk_08;
                    entry->attackBonus     = itemData[index].attack;
                    entry->defenseBonus    = itemData[index].defense;
                    entry->healthBonus     = itemData[index].health;
                    entry->count           = (u32)(gSaveData.inventoryItems[i].flags << 0x1C) >> 0x1C;
                    entry->unk_11          = (u32)(gSaveData.inventoryItems[i].flags << 0x1B) >> 0x1F;
                    entry->abilityUnlocked = (u32)(gSaveData.unk_1EB2[index] << 0x1F) >> 0x1F;
                    entry->brand           = itemData[index].brand;
                    entry->unk_14          = itemData[index].unk_04;
                    entry->abilityId       = itemData[index].unk_10;
                    entry->unk_1A          = itemData[index].unk_12;
                    entry->unk_1C          = itemData[index].unk_14;
                    entry->foodEffectMsg   = 0;
                    entry->foodEffectValue = 0;
                    entry->syncGain        = 0;
                    entry->taste[0]        = 0;
                    entry->taste[1]        = 0;
                    entry->taste[2]        = 0;
                    entry->taste[3]        = 0;
                    break;
                case ITEM_CATEGORY_FOOD:
                    entry                  = &owner->inventory[i];
                    entry->itemId          = itemId;
                    entry->graphicIndex    = foodData[index].unk_00;
                    entry->category        = ITEM_CATEGORY_FOOD;
                    entry->subCategory     = 0;
                    entry->capacityCost    = foodData[index].unk_02;
                    entry->braveryReq      = 0;
                    entry->attackBonus     = 0;
                    entry->defenseBonus    = 0;
                    entry->healthBonus     = 0;
                    entry->count           = (u32)(gSaveData.inventoryItems[i].flags << 0x1C) >> 0x1C;
                    entry->unk_11          = (u32)(gSaveData.inventoryItems[i].flags << 0x1B) >> 0x1F;
                    entry->abilityUnlocked = 0;
                    entry->brand           = 0xE;
                    entry->unk_14          = foodData[index].unk_04;
                    entry->abilityId       = 0;
                    entry->unk_1A          = 0;
                    entry->unk_1C          = 0;
                    entry->foodEffectMsg   = foodData[index].stat;
                    entry->foodEffectValue = foodData[index].statBonus;
                    entry->syncGain        = foodData[index].unk_0C;
                    entry->taste[0]        = foodData[index].unk_10[0];
                    entry->taste[1]        = foodData[index].unk_10[1];
                    entry->taste[2]        = foodData[index].unk_10[2];
                    entry->taste[3]        = foodData[index].unk_10[3];
                    break;
                case ITEM_CATEGORY_SWAG:
                    entry                  = &owner->inventory[i];
                    entry->itemId          = itemId;
                    entry->graphicIndex    = treasureData[index].unk_00;
                    entry->category        = ITEM_CATEGORY_SWAG;
                    entry->subCategory     = 0;
                    entry->capacityCost    = 0;
                    entry->braveryReq      = 0;
                    entry->attackBonus     = 0;
                    entry->defenseBonus    = 0;
                    entry->healthBonus     = 0;
                    entry->count           = (u32)(gSaveData.inventoryItems[i].flags << 0x1C) >> 0x1C;
                    entry->unk_11          = 0;
                    entry->abilityUnlocked = 0;
                    entry->brand           = 0xE;
                    entry->unk_14          = treasureData[index].unk_04;
                    entry->abilityId       = 0;
                    entry->unk_1A          = 0;
                    entry->unk_1C          = 0;
                    entry->foodEffectMsg   = 0;
                    entry->foodEffectValue = 0;
                    entry->syncGain        = 0;
                    entry->taste[0]        = 0;
                    entry->taste[1]        = 0;
                    entry->taste[2]        = 0;
                    entry->taste[3]        = 0;
                    MenuEquip_AddSwagBonus(owner, entry->itemId, entry->count);
                    break;
            }
        }
    }
    MenuEquip_InitStats(owner);
    Mem_Free(&gDebugHeap, itemData);
    Mem_Free(&gDebugHeap, foodData);
    Mem_Free(&gDebugHeap, treasureData);
}

u16 MenuEquip_BuildTabBySubCategory(MenuEquipObject* owner, s32 tab, s32 category, s32 subCategory) {
    u16 i, count;

    count = 0;
    for (i = 0; i < 472; i++) {
        if ((category == owner->inventory[i].category) && (subCategory == owner->inventory[i].subCategory)) {
            owner->tabItems[tab][count] = &owner->inventory[i];
            count++;
        }
    }
    return count;
}

u16 MenuEquip_BuildTabByCategory(MenuEquipObject* owner, s32 tab, s32 category) {
    u16 i, count;

    count = 0;
    for (i = 0; i < 472; i++) {
        if (category == owner->inventory[i].category) {
            owner->tabItems[tab][count] = &owner->inventory[i];
            count++;
        }
    }
    return count;
}

// Rounds a list length up to whole pages of 8 rows, with a minimum of two pages.
u16 MenuEquip_RoundUpToPages(u16 count) {
    if (count < 16) {
        return 16;
    }
    if (count % 8 == 0) {
        return count;
    }
    return ((count / 8) + 1) * 8;
}

s32 MenuEquip_CompareItemIds(u16* a, u16* b) {
    return *a - *b;
}

void MenuEquip_SortInventory(MenuEquipObject* owner) {
    void* buffer = Mem_AllocHeapTail(&gDebugHeap, MATH_QSortStackSize(472));

    Mem_SetSequence(&gDebugHeap, buffer, "ItemID_sortBuf");
    func_02047ec8(owner->inventory, 472, sizeof(MenuEquipItemEntry), MenuEquip_CompareItemIds, buffer);
    Mem_Free(&gDebugHeap, buffer);
}

void MenuEquip_BuildTabs(MenuEquipObject* owner) {
    u16 counts[9];
    u16 count;    // count before tabIndex: mwcc assigns registers in declaration order, and the
    u16 tabIndex; // target keeps the entry value in r7 and the loop counter in r8.
    u32 sum;

    for (u16 tab = 0; tab < 9; tab++) {
        for (u16 i = 0; i < 472; i++) {
            owner->tabItems[tab][i] = &data_ov043_020cc2d8;
        }
    }
    counts[0] = MenuEquip_BuildTabBySubCategory(owner, 0, ITEM_CATEGORY_THREAD, 0);
    counts[1] = MenuEquip_BuildTabBySubCategory(owner, 1, ITEM_CATEGORY_THREAD, 1);
    counts[2] = MenuEquip_BuildTabBySubCategory(owner, 2, ITEM_CATEGORY_THREAD, 2);
    counts[3] = MenuEquip_BuildTabBySubCategory(owner, 3, ITEM_CATEGORY_THREAD, 3);
    counts[4] = MenuEquip_BuildTabBySubCategory(owner, 4, ITEM_CATEGORY_THREAD, 4);
    counts[5] = MenuEquip_BuildTabBySubCategory(owner, 5, ITEM_CATEGORY_THREAD, 5);
    counts[6] = MenuEquip_BuildTabByCategory(owner, 6, ITEM_CATEGORY_FOOD);
    counts[7] = MenuEquip_BuildTabByCategory(owner, 7, ITEM_CATEGORY_SWAG);
    for (u16 i = 0; i < 472; i++) {
        owner->tabItems[8][i] = &owner->inventory[i];
    }
    sum = 0;
    for (u16 i = 0; i < 8; i++) {
        sum = (u16)(sum + counts[i]);
    }
    counts[8] = sum;
    for (tabIndex = 0; tabIndex < 9; tabIndex++) {
        count = counts[tabIndex];

        owner->pagedCount[tabIndex]     = MenuEquip_RoundUpToPages(count);
        owner->maxScrollRow[tabIndex]   = owner->pagedCount[tabIndex] / 8 - 2;
        owner->scrollBarRange[tabIndex] = owner->maxScrollRow[tabIndex];
        if (count <= 16) {
            owner->fitsOnePage[tabIndex] = 1;
        } else {
            owner->fitsOnePage[tabIndex] = 0;
        }
    }
}

u32 MenuEquip_GetCurrentDate(void) {
    RTCDate date;

    func_02041680(&date);
    return date.day + ((date.year * 10000) + (date.month * 100));
}

void MenuEquip_LoadFromSave(MenuEquipObject* owner) {
    MenuEquipItemEntry* item;
    u32                 date;
    s32                 i;

    owner->activePartner     = gSaveData.playerStats.activePartner;
    owner->currentTab        = 8;
    owner->swagAttackShiki   = 0;
    owner->swagDefenseShiki  = 0;
    owner->swagAttackJoshua  = 0;
    owner->swagDefenseJoshua = 0;
    owner->swagAttackBeat    = 0;
    owner->swagDefenseBeat   = 0;
    MenuEquip_BuildItemEntries(owner);
    MenuEquip_SortInventory(owner);
    for (i = 0; i < 5; i++) {
        owner->slots[i] = &owner->equipped[0][i];
    }
    if (owner->activePartner == 0xFF) {
        for (i = 0; i < 5; i++) {
            owner->slots[5 + i] = &data_ov043_020cc2d8;
        }
    } else {
        for (i = 0; i < 5; i++) {
            owner->slots[5 + i] = &owner->equipped[owner->activePartner + 1][i];
        }
    }
    MenuEquip_BuildTabs(owner);
    for (i = 0; i < 16; i++) {
        owner->slots[10 + i] = owner->tabItems[owner->currentTab][i];
    }
    item = owner->slots[0];
    if (item->itemId == 0xFFFF) {
        item = &data_ov043_020cc2d8;
    }
    owner->cursorItem = *item;
    date              = MenuEquip_GetCurrentDate();
    if (date != gSaveData.unk_242C) {
        for (i = 0; i < 4; i++) {
            owner->stats[i].foodCapacityLeft = 24;
        }
    }
    gSaveData.unk_242C = date;
    if (func_02023010(0x2CB) == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        owner->stats[i].foodCapacityLeft = 24;
    }
}

void MenuEquip_WriteBackToSave(MenuEquipObject* owner) {
    s32 i, j;

    for (i = 0; i < 4; i++) {
        gSaveData.playerStats.equippedThreads[i] = owner->equipped[0][i].itemId;
        gSaveData.playerStats.unk_24[i]          = owner->equipped[0][i].abilityUnlocked;
    }
    gSaveData.playerStats.foodItem         = owner->equipped[0][4].itemId;
    gSaveData.playerStats.foodBytesLeft    = owner->equipped[0][4].capacityCost;
    gSaveData.playerStats.foodCapacityLeft = owner->stats[0].foodCapacityLeft;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            gSaveData.friendStats[i].equippedThreads[j] = owner->equipped[i + 1][j].itemId;
            gSaveData.friendStats[i].unk_10[j]          = owner->equipped[i + 1][j].abilityUnlocked;
        }
        gSaveData.friendStats[i].foodItem         = owner->equipped[i + 1][4].itemId;
        gSaveData.friendStats[i].foodBytesLeft    = owner->equipped[i + 1][4].capacityCost;
        gSaveData.friendStats[i].foodCapacityLeft = owner->stats[i + 1].foodCapacityLeft;
    }
    for (i = 0; i < 472; i++) {
        gSaveData.inventoryItems[i].itemID = owner->inventory[i].itemId;
        gSaveData.inventoryItems[i].flags  = (gSaveData.inventoryItems[i].flags & ~0xF) | (owner->inventory[i].count & 0xF);
        gSaveData.inventoryItems[i].flags =
            (gSaveData.inventoryItems[i].flags & ~0x10) | ((u32)(owner->inventory[i].unk_11 << 0x1F) >> 0x1B);
        gSaveData.inventoryItems[i].flags =
            (gSaveData.inventoryItems[i].flags & ~0x20) | ((u32)(owner->inventory[i].abilityUnlocked << 0x1F) >> 0x1A);
    }
    if (owner->activePartner != 0xFF) {
        gSaveData.friendStats[0].sync = owner->stats[1].sync;
        gSaveData.friendStats[1].sync = owner->stats[2].sync;
        gSaveData.friendStats[2].sync = owner->stats[3].sync;
    }
    gSaveData.playerStats.activePartner = owner->activePartner;
}

s16 MenuEquip_GetSlotAtPoint(s16 x, s16 y) {
    s16 offset[2] = {-13, -13};
    s16 size[2]   = {26, 26};
    s16 i;
    s32 shrink;
    s16 slotX;

    for (i = 0; i < 26; i++) {
        slotX = data_ov043_020c96f0[i][0];
        if (slotX == 0xE5) {
            shrink = 2;
        } else {
            shrink = 0;
        }
        if (MenuEquip_IsPointInRect(x, y, (s16)(offset[0] + slotX), (s16)(offset[1] + data_ov043_020c96f0[i][1]),
                                    size[0] - shrink, size[1]) == 1)
        {
            return i;
        }
    }
    return -1;
}

s16 MenuEquip_GetDropTargetAtPoint(s16 x, s16 y) {
    s16 slotOffset[2] = {-13, -13};
    s16 slotSize[2]   = {26, 26};
    s16 listPos[2]    = {3, 0x80};
    s16 listSize[2]   = {240, 60};
    s16 foodPos[2]    = {3, 26};
    s16 foodSize[2]   = {60, 80};
    s16 i;
    s32 foodX = foodPos[0] + 3;

    for (i = 0; i < 2; i++) {
        if (MenuEquip_IsPointInRect(x, y, foodX, foodPos[1], foodSize[0], foodSize[1]) == 1) {
            return (i * 5) + 4;
        }
        foodX += 0x80;
    }

    for (i = 0; i < 10; i++) {
        if ((i != 4) && (i != 9)) {
            if (MenuEquip_IsPointInRect(x, y, (s16)(slotOffset[0] + data_ov043_020c96f0[i][0]),
                                        (s16)(slotOffset[1] + data_ov043_020c96f0[i][1]), slotSize[0], slotSize[1]) == 1)
            {
                return i;
            }
        }
    }
    if (MenuEquip_IsPointInRect(x, y, listPos[0], listPos[1], listSize[0], listSize[1]) == 1) {
        return 30;
    }
    return -1;
}

s16 MenuEquip_GetButtonAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0xCD, 2},
        {0xE6, 2},
    };
    s16 size[2] = {0x17, 0x13};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuEquip_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuEquip_GetTabAtPoint(s16 x, s16 y) {
    s16 tabX[8] = {0x84, 0x92, 0xA0, 0xAE, 0xBC, 0xCA, 0xD8, 0xE6};
    s16 size[2] = {12, 12};
    s16 i;

    for (i = 0; i < 8; i++) {
        if (MenuEquip_IsPointInRect(x, y, tabX[i], 0x73, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuEquip_GetPartnerTabAtPoint(s16 x, s16 y) {
    s16 posX[3] = {0x7D, 0x8F, 0xA1};
    s16 size[2] = {0x10, 0xD};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (MenuEquip_IsPointInRect(x, y, posX[i], 5, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuEquip_IsPointOnScrollBar(s16 x, s16 y) {
    s16 pos[2]  = {0xF1, 0x7D};
    s16 size[2] = {0xF, 0x33};

    if (MenuEquip_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuEquip_IsPointOnScrollKnob(s16 x, s16 y, s16 targetX, s16 targetY) {
    s16 size[2] = {0xD, 0xD};

    if (MenuEquip_IsPointInRect(x, y, (s16)(targetX - 6), (s16)(targetY - 13), size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuEquip_GetScrollArrowAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0xF1, 0x70},
        {0xF1, 0xB3},
    };
    s16 size[2] = {0xF, 9};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuEquip_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuEquip_CanDragSlot(MenuEquipObject* owner, s32 slot) {
    if (slot == -1) {
        return 0;
    }
    if (owner->slots[slot]->itemId == 0xFFFF) {
        return 0;
    }
    if ((owner->activePartner == 0xFF) && (slot >= 5) && (slot <= 8)) {
        return 0;
    }
    return 1;
}

s32 MenuEquip_CanDropOnSlot(MenuEquipObject* owner, s32 src, s32 dst) {
    if (dst == -1) {
        return 0;
    }
    if (dst == src) {
        return 0;
    }
    if ((dst >= 10) && (src >= 10)) {
        return 0;
    }
    if ((owner->activePartner == 0xFF) && (((dst >= 5) && (dst <= 8)) || (dst == 9))) {
        return 0;
    }
    return 1;
}

s32 MenuEquip_StackIntoInventory(MenuEquipObject* owner, u16 itemId) {
    u16 i;

    for (i = 0; i < 472; i++) {
        if (itemId == owner->inventory[i].itemId) {
            owner->inventory[i].count++;
            if (owner->inventory[i].count > 9) {
                owner->inventory[i].count = 9;
            }
            return 1;
        }
    }
    return 0;
}

s32 MenuEquip_InsertIntoInventory(MenuEquipObject* owner, MenuEquipItemEntry* item) {
    u16 emptyIndex, minIndex;
    u32 minItemId;
    u16 insertIndex, itemId, i;
    s32 last;

    minItemId = 0xFFFF;
    for (i = 0; i < 472; i++) {
        itemId = owner->inventory[i].itemId;
        if (itemId < minItemId) {
            minItemId = itemId;
            minIndex  = i;
        }
        if (itemId == 0xFFFF) {
            emptyIndex = i;
            break;
        }
    }
    if (minIndex == 0) {
        insertIndex = emptyIndex;
        for (i = 0; i < 472; i++) {
            if (item->itemId < owner->inventory[i].itemId) {
                insertIndex = i;
                break;
            }
        }
    } else {
        itemId = item->itemId;
        if (itemId > owner->inventory[0].itemId) {
            insertIndex = minIndex;
            for (i = 0; i < minIndex; i++) {
                if (itemId < owner->inventory[i].itemId) {
                    insertIndex = i;
                    break;
                }
            }
        } else {
            insertIndex = emptyIndex;
            for (i = minIndex; i < 472; i++) {
                if (itemId < owner->inventory[i].itemId) {
                    insertIndex = i;
                    break;
                }
            }
            if ((insertIndex == emptyIndex) && (item->subCategory == owner->inventory[0].subCategory)) {
                insertIndex = 0;
            }
        }
    }
    last = 471 - insertIndex;
    for (i = 1; last >= i; i++) {
        owner->inventory[472 - i] = owner->inventory[471 - i];
    }
    owner->inventory[insertIndex] = *item;
    return 1;
}

void MenuEquip_PlayTasteVoice(u8 character, u32 taste) {
    s32 seTable[4][5] = {
        {0x40C, 0x40D, 0x40E, 0x40F, 0x410},
        {0x411, 0x412, 0x413, 0x414, 0x415},
        {0x416, 0x417, 0x418, 0x419, 0x41A},
        {0x41B, 0x41C, 0x41D, 0x41E, 0x41F},
    };

    if (taste > 4) {
        return;
    }
    SndMgr_StartPlayingSE(seTable[character][(u8)(4 - taste)]);
}

s16 MenuEquip_GetHelpButtonAtPoint(s16 x, s16 y) {
    s16 pos[3][2] = {
        {0x20, 0x38},
        {0xC0, 0x38},
        {0x50, 0x73},
    };
    s16 size[3][2] = {
        {0x20, 0x40},
        {0x20, 0x40},
        {0x60, 0x14},
    };
    s16 i;

    for (i = 0; i < 3; i++) {
        if (MenuEquip_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

void MenuEquip_LoadBgResource(MenuEquipResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &data_ov043_020c9758[binIndex]);
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
            PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
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
            PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void MenuEquip_LoadBgResourceIndexed(MenuEquipResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, s32 palCount,
                                     s32 screenIndex, s32 palIndex) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &data_ov043_020c9758[binIndex]);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, screenIndex + 2);
    res->paletteData = Data_GetPackEntryData(res->data, palIndex + 8);
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
            PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
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
            PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void MenuEquip_LoadBgScreen(MenuEquipResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
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

void MenuEquip_ReleaseBgResource(MenuEquipResource* res, s32 engine) {
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

void MenuEquip_ReleaseBgScreen(MenuEquipResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
    } else {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
    }
}

void MenuEquip_ReloadBgResource(MenuEquipResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, s32 palCount) {
    MenuEquip_ReleaseBgResource(res, engine);
    MenuEquip_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void MenuEquip_ClearBgResource(MenuEquipResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void MenuEquip_LoadBackgrounds(MenuEquipObject* menuEquip) {
    s16 partner;
    s32 i; // Declared here rather than in the for: mwcc assigns registers in
           // declaration order, and hoisting i into the loop flips r5/r6/r7.
    MenuEquipResource* mainRes = &menuEquip->resources[0];
    MenuEquipResource* subRes  = &menuEquip->resources[4];

    for (i = 0; i < 4; i++) {
        MenuEquip_ClearBgResource(mainRes);
        MenuEquip_ClearBgResource(subRes);
        mainRes += 1;
        subRes += 1;
    }

    if (func_02023010(0x2AE) == 0) {
        menuEquip->partnerArtSet = 0;
    } else {
        menuEquip->partnerArtSet = 1;
    }

    partner = menuEquip->activePartner;
    if (partner == 0xFF) {
        MenuEquip_LoadBgResourceIndexed(&menuEquip->resources[7], 0, 3, 0, 0, 0xE, 0, 0);
    } else {
        MenuEquip_LoadBgResourceIndexed(&menuEquip->resources[7], 0, 3, 0, 0, 0xE, partner + (menuEquip->partnerArtSet * 3),
                                        partner);
    }
    MenuEquip_LoadBgResource(&menuEquip->resources[5], 0, 1, 11, 15, 1);
    MenuEquip_LoadBgScreen(&menuEquip->resources[6], menuEquip->resources[7].data, 0, 2, 11);
    MenuEquip_LoadBgResource(&menuEquip->resources[0], 1, 0, 10, 15, 1);
    MenuEquip_LoadBgResource(&menuEquip->resources[3], 1, 3, 3, 1, 1);
    MenuEquip_LoadBgScreen(&menuEquip->resources[1], menuEquip->resources[3].data, 1, 1, 4);
    MenuEquip_LoadBgScreen(&menuEquip->resources[2], menuEquip->resources[3].data, 1, 2, 5);
}

void MenuEquip_UpdateBackgrounds(MenuEquipObject* menuEquip) {
    return;
}

void MenuEquip_ReleaseBackgrounds(MenuEquipObject* menuEquip) {
    MenuEquip_ReleaseBgResource(&menuEquip->resources[5], 0);
    MenuEquip_ReleaseBgScreen(&menuEquip->resources[6], 0);
    MenuEquip_ReleaseBgResource(&menuEquip->resources[7], 0);
    MenuEquip_ReleaseBgResource(&menuEquip->resources[0], 1);
    MenuEquip_ReleaseBgScreen(&menuEquip->resources[1], 1);
    MenuEquip_ReleaseBgScreen(&menuEquip->resources[2], 1);
    MenuEquip_ReleaseBgResource(&menuEquip->resources[3], 1);
}
