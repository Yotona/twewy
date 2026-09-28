#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/MenuBadge.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/math.h>

void func_02047ec8(void* head, u32 num, u32 width, s32 (*compare)(void*, void*), void* buffer);

// A pin's Tin Pin Slammer stats
typedef struct {
    /* 0x00 */ u8   whammyStinger;
    /* 0x01 */ u8   whammyBomber;
    /* 0x02 */ u8   whammyHammer;
    /* 0x03 */ u8   whammyHand;
    /* 0x04 */ u8   weight;   // 0-based
    /* 0x05 */ char unk_05[0x06 - 0x05];
    /* 0x06 */ s16  spin;     // shown divided by 409
    /* 0x08 */ char unk_08[0x18 - 0x08];
    /* 0x18 */ u16  koLength; // frames
    /* 0x1A */ char unk_1A[0x1C - 0x1A];
} MenuBadgeParm;              // Size: 0x1C

s32 MenuBadge_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void MenuBadge_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

void MenuBadge_SetSpriteFrameFromPack(Sprite* sprite, s16 frame, s32 animIndex, s32 frameDataIndex) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, animIndex);
    void* frames = Data_GetPackEntryData(sprite->resourceData, frameDataIndex);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

s32 MenuBadge_IsSlotMastered(MenuBadgeObject* owner, u16 index) {
    MenuBadgeEntry* entry = owner->slots[index];

    if (entry->level == entry->maxLevel) {
        return 1;
    }
    return 0;
}

u16 MenuBadge_CalcTotalPP(u16 battlePP, u16 minglePP, u16 shutdownPP) {
    return battlePP + minglePP + shutdownPP;
}

u16 MenuBadge_GetLevelPP(u8 level, u8 ppCurve) {
    return func_02023480(level, ppCurve);
}

u16 MenuBadge_GetNextLevelPP(u8 level, u8 maxLevel, u8 ppCurve) {
    if (level == maxLevel) {
        return 0xFFFF;
    }
    return func_02023480((u8)(level + 1), ppCurve);
}

void MenuBadge_LoadPinData(RawPinData* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x3DC0, &MenuBadge_BinIdentifiers[13]));
}

void MenuBadge_LoadPinParmData(MenuBadgeParm* buffer) {
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, buffer, 0x2140, &MenuBadge_BinIdentifiers[14]));
}

u16 MenuBadge_GetFreePaletteSlot(void) {
    u16 i;
    PalSlot(*slots)[16] = g_PaletteManagers[DISPLAY_EXTENDED]->slots;

    for (i = 0; i < 16; i++) {
        if (slots[6][i].flags == 0) {
            return 6;
        }
    }
    return 5;
}

s32 MenuBadge_CompareByPinId(MenuBadgeEntry* a, MenuBadgeEntry* b) {
    if (a->pinId == b->pinId) {
        if (a->level == b->level) {
            return b->totalPP - a->totalPP;
        }
        return b->level - a->level;
    }
    return a->pinId - b->pinId;
}

s32 MenuBadge_CompareByPsych(MenuBadgeEntry* a, MenuBadgeEntry* b) {
    static const u16 MenuBadge_PsychSortOrder[65] = {
        0x1A, 0x18, 0x19, 0xE,  0xD,  0x10, 0xF,  0x11, 2,    3,    0x35, 0x2D, 0x1E, 0x1F, 0x2C, 0x2E, 0x2A,
        0x29, 0x2B, 0,    1,    0x20, 0x21, 0x22, 0x23, 0x24, 0x1B, 0x30, 0x25, 0x26, 0x3F, 0x31, 0x28, 0x32,
        0x38, 0x39, 4,    8,    7,    9,    0xA,  0xC,  0xB,  5,    6,    0x14, 0x12, 0x13, 0x1C, 0x1D, 0x15,
        0x16, 0x17, 0x2F, 0x27, 0x3A, 0x3B, 0x37, 0x33, 0x34, 0x36, 0x3C, 0x3D, 0x3E, 0x40,
    };
    static const u16 MenuBadge_AbilitySortOrder[223] = {
        0xE,    0x10,   0x11,   0x12,   0x13,   0x14,   0x15,   0x16,   0x17,   0x18,   0x19,   0x1A,   0x1B,   0xF,    0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0x29,   0x2A,   0x2B,   0x2C,   0x2D,   0x2E,
        0x24,   0x25,   0x26,   0x27,   0xFD00, 0x1C,   0x1D,   0x1E,   0x1F,   0x20,   0x21,   0x22,   0x23,   0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0x31,
        0x32,   0x33,   0x34,   0x35,   0x36,   0x37,   0x38,   0x28,   0x39,   0x3A,   0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0x2F,   0x30,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00,
        0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0xFD00, 0,      0x3C,   0x3B,
        1,      2,      3,      4,      5,      6,      7,      8,      9,      0xA,    0xB,    0xC,    0xD,
    };
    MenuBadgeEntry* entries[2];
    s32             key[2];
    u16             i;

    entries[0] = a;
    entries[1] = b;
    for (i = 0; i < 2; i++) {
        MenuBadgeEntry* entry = entries[i];
        if (entry->pinId == 0xFFFF) {
            key[i] = 0xFFFF;
        } else if ((entry->pinId >= 0xF5) && (entry->pinId <= 0x104)) {
            key[i] = entry->pinId;
        } else if (entry->psychId != 0xFFFF) {
            key[i] = MenuBadge_PsychSortOrder[entry->psychId] + 0x1FF;
        } else {
            key[i] = MenuBadge_AbilitySortOrder[entry->abilityId] + 0x2FF;
        }
    }
    if (key[0] == key[1]) {
        if (entries[0]->pinId == entries[1]->pinId) {
            if (entries[0]->level != entries[1]->level) {
                return entries[1]->level - entries[0]->level;
            }
            return entries[1]->totalPP - entries[0]->totalPP;
        }
        return entries[0]->pinId - entries[1]->pinId;
    }
    return key[0] - key[1];
}

void MenuBadge_SortList(MenuBadgeObject* menuBadge, u8 listMode, s32 sortMode) {
    MenuBadgeEntry* list;
    u32             count;
    s32 (*compare)(void*, void*);
    void* buffer;

    if (listMode == 0) {
        list  = menuBadge->stockpile;
        count = 256;
    } else {
        list  = menuBadge->mastered;
        count = 304;
    }
    if (sortMode == 0) {
        compare = (s32(*)(void*, void*))MenuBadge_CompareByPinId;
    } else {
        compare = (s32(*)(void*, void*))MenuBadge_CompareByPsych;
    }
    buffer = Mem_AllocHeapTail(&gDebugHeap, MATH_QSortStackSize(count));
    Mem_SetSequence(&gDebugHeap, buffer, "BadgeData_sortBuf");
    func_02047ec8(list, count, sizeof(MenuBadgeEntry), compare, buffer);
    Mem_Free(&gDebugHeap, buffer);
}

u32 MenuBadge_CalcSellPrice(u32 basePrice, s16 levelPrice, u8 level) {
    if (basePrice == 10000000) {
        return 0;
    }
    return ((levelPrice * (level - 1)) + basePrice) >> 1;
}

s32 MenuBadge_ClampMoney(MenuBadgeObject* owner, u8 capLevel) {
    if (capLevel == 3) {
        if (owner->money >= 9999999) {
            owner->money = 9999999;
            return 1;
        }
    } else if (capLevel == 2) {
        if (owner->money >= 999999) {
            owner->money = 999999;
            return 1;
        }
    } else if (capLevel == 1) {
        if (owner->money >= 99999) {
            owner->money = 99999;
            return 1;
        }
    } else {
        if (owner->money >= 99999) {
            owner->money = 99999;
            return 1;
        }
    }
    return 0;
}

s32 MenuBadge_GetMoneyCapLevel(void) {
    if (func_02023010(0x284) != 0) {
        return 3;
    }
    if (func_02023010(0x283) != 0) {
        return 2;
    }
    if (func_02023010(0x282) != 0) {
        return 1;
    }
    return 0;
}

s32 func_ov043_0208fed8(void) {
    if (func_02023010(0x2CB) != 0) {
        return 1;
    }
    return 0;
}

static inline s32 IsEqual(u16 a, u16 b) {
    return a == b;
}

s32 MenuBadge_CanEquipToDeck(MenuBadgeObject* menuBadge, u16 src, u16 dst) {
    u16 count;
    u8  i;

    count = 0;

    switch (menuBadge->slots[src]->pinClass) {
        case PIN_CLASS_ANGEL:
            for (i = 0; i < menuBadge->deckSlotCount; i++) {
                if ((i != dst) && (menuBadge->slots[i]->pinClass == PIN_CLASS_ANGEL)) {
                    return 0;
                }
            }
            break;
        case PIN_CLASS_REAPER:
            for (i = 0; i < menuBadge->deckSlotCount; i++) {
                if ((i != dst) && (menuBadge->slots[i]->pinClass == PIN_CLASS_REAPER)) {
                    return 0;
                }
            }
            break;
        case PIN_CLASS_A:
            for (i = 0; i < menuBadge->deckSlotCount; i++) {
                MenuBadgeEntry* other = menuBadge->slots[i];
                if ((other->pinClass != PIN_CLASS_ANGEL) && (other->pinClass != PIN_CLASS_REAPER) && (i != dst)) {
                    if (menuBadge->slots[src]->psychId != 0xFFFF) {
                        if (IsEqual(menuBadge->slots[src]->psychId, other->psychId)) {
                            return 0;
                        }
                    } else if (IsEqual(menuBadge->slots[src]->abilityId, other->abilityId)) {
                        return 0;
                    }
                }
            }
            break;
        case PIN_CLASS_B:
            for (i = 0; i < menuBadge->deckSlotCount; i++) {
                MenuBadgeEntry* other = menuBadge->slots[i];
                if ((other->pinClass != PIN_CLASS_ANGEL) && (other->pinClass != PIN_CLASS_REAPER) && (i != dst)) {
                    if (menuBadge->slots[src]->psychId != 0xFFFF) {
                        if (IsEqual(menuBadge->slots[src]->psychId, other->psychId)) {
                            count++;
                        }
                    } else if (IsEqual(menuBadge->slots[src]->abilityId, other->abilityId)) {
                        count++;
                    }
                }
            }
            if (count >= 2) {
                return 0;
            }
            break;
        case PIN_CLASS_C:
            for (i = 0; i < menuBadge->deckSlotCount; i++) {
                MenuBadgeEntry* other = menuBadge->slots[i];
                if ((other->pinClass != PIN_CLASS_ANGEL) && (other->pinClass != PIN_CLASS_REAPER) && (i != dst)) {
                    if (menuBadge->slots[src]->psychId != 0xFFFF) {
                        if (IsEqual(menuBadge->slots[src]->psychId, other->psychId)) {
                            count++;
                        }
                    } else if (IsEqual(menuBadge->slots[src]->abilityId, other->abilityId)) {
                        count++;
                    }
                }
            }
            if (count >= 3) {
                return 0;
            }
            break;
    }
    return 1;
}

u8 MenuBadge_GetDeckSlotCount(void) {
    u8 slotCount = func_02023010(0x2A8) + 2;

    if (slotCount > 6) {
        slotCount = 6;
    }
    return slotCount;
}

s32 func_ov043_02090194(u16 unk_02, u8 unk_24, u8 unk_30) {
    if ((unk_30 == 0) || (unk_30 == 0xFF)) {
        return 1;
    }
    return 0;
}

u32 MenuBadge_CountUnmasteredBadges(MenuBadgeObject* menuBadge) {
    u16 deck;
    u16 i;
    u16 total;
    u16 j;

    total = 0;
    for (deck = 0; deck < 4; deck++) {
        for (i = 0; i < 6; i++) {
            MenuBadgeEntry* entry = &menuBadge->decks[deck][i];
            if ((entry->pinId != 0xFFFF) && (entry->level != entry->maxLevel)) {
                total++;
            }
        }
    }
    for (j = 0; j < 256; j++) {
        if (menuBadge->stockpile[j].pinId != 0xFFFF) {
            total++;
        }
    }
    return total;
}

s16 MenuBadge_CountMasteredBadges(MenuBadgeObject* menuBadge, u16 pinId) {
    u16 i;
    u16 deck;
    u16 count;
    u16 j;

    count = 0;
    for (deck = 0; deck < 4; deck++) {
        for (i = 0; i < 6; i++) {
            MenuBadgeEntry* entry = &menuBadge->decks[deck][i];
            if ((pinId == entry->pinId) && (entry->level == entry->maxLevel)) {
                count++;
            }
        }
    }
    for (j = 0; j < 304; j++) {
        if (pinId == menuBadge->mastered[j].pinId) {
            return (u16)(count + menuBadge->mastered[j].count);
        }
    }
    return count;
}

void MenuBadge_InitMasteredCounts(MenuBadgeObject* menuBadge) {
    for (u16 i = 0; i < 304; i++) {
        menuBadge->masteredCounts[i] = MenuBadge_CountMasteredBadges(menuBadge, i);
    }
}

void MenuBadge_SubtractMasteredCount(MenuBadgeObject* menuBadge, u16 pinId, u16 count) {
    menuBadge->masteredCounts[pinId] -= count;
    if (menuBadge->masteredCounts[pinId] < 0) {
        menuBadge->masteredCounts[pinId] = 0;
    }
}

void MenuBadge_LoadFromSave(MenuBadgeObject* menuBadge) {
    RawPinData*    pinData;
    MenuBadgeParm* parmData;
    s32            i;
    s32            deck;

    menuBadge->money       = gSaveData.playerStats.money;
    menuBadge->currentDeck = gSaveData.playerStats.pinDeck;
    if (menuBadge->currentDeck >= 4) {
        OS_WaitForever();
    }
    if (gSaveData.playerStats.pinQuickSell == 1) {
        menuBadge->flags |= MENUBADGE_FLAG_QUICK_SELL;
    } else {
        menuBadge->flags = 0;
    }

    pinData = Mem_AllocHeapTail(&gDebugHeap, 0x3DC0);
    Mem_SetSequence(&gDebugHeap, pinData, "badge_data");
    parmData = Mem_AllocHeapTail(&gDebugHeap, 0x2140);
    Mem_SetSequence(&gDebugHeap, parmData, "beBadge_data");
    MenuBadge_LoadPinData(pinData);
    MenuBadge_LoadPinParmData(parmData);

    for (i = 0; i < 6; i++) {
        gSaveData.pinLayouts[menuBadge->currentDeck][i].pinID             = gSaveData.equippedPins[i].pinID;
        gSaveData.pinLayouts[menuBadge->currentDeck][i].flags.bits.level  = gSaveData.equippedPins[i].flags.bits.level;
        gSaveData.pinLayouts[menuBadge->currentDeck][i].battlePP          = gSaveData.equippedPins[i].battlePP;
        gSaveData.pinLayouts[menuBadge->currentDeck][i].minglePP          = gSaveData.equippedPins[i].minglePP;
        gSaveData.pinLayouts[menuBadge->currentDeck][i].shutdownPP        = gSaveData.equippedPins[i].shutdownPP;
        gSaveData.pinLayouts[menuBadge->currentDeck][i].unk_08            = gSaveData.equippedPins[i].unk_08;
        gSaveData.pinLayouts[menuBadge->currentDeck][i].flags.bits.unk_07 = gSaveData.equippedPins[i].flags.bits.unk_07;
    }

    for (deck = 0; deck < 4; deck++) {
        for (s32 slot = 0; slot < 6; slot++) {
            u16 pinId = gSaveData.pinLayouts[deck][slot].pinID;

            if (pinId == 0xFFFF) {
                menuBadge->decks[deck][slot] = MenuBadge_EmptyEntry;
            } else {
                RawPinData*    raw;
                MenuBadgeParm* parm;

                menuBadge->decks[deck][slot].pinId      = pinId;
                menuBadge->decks[deck][slot].battlePP   = gSaveData.pinLayouts[deck][slot].battlePP;
                menuBadge->decks[deck][slot].minglePP   = gSaveData.pinLayouts[deck][slot].minglePP;
                raw                                     = &pinData[pinId];
                menuBadge->decks[deck][slot].shutdownPP = gSaveData.pinLayouts[deck][slot].shutdownPP;
                menuBadge->decks[deck][slot].level      = gSaveData.pinLayouts[deck][slot].flags.bits.level;
                menuBadge->decks[deck][slot].levelPP = MenuBadge_GetLevelPP(menuBadge->decks[deck][slot].level, raw->ppCurve);
                menuBadge->decks[deck][slot].totalPP =
                    MenuBadge_CalcTotalPP(menuBadge->decks[deck][slot].battlePP, menuBadge->decks[deck][slot].minglePP,
                                          menuBadge->decks[deck][slot].shutdownPP);
                menuBadge->decks[deck][slot].nextLevelPP =
                    MenuBadge_GetNextLevelPP(menuBadge->decks[deck][slot].level, raw->maxLevel, raw->ppCurve);
                parm                                          = &parmData[pinId];
                menuBadge->decks[deck][slot].deckSlot         = slot;
                menuBadge->decks[deck][slot].count            = 1;
                menuBadge->decks[deck][slot].unk_18           = gSaveData.pinLayouts[deck][slot].flags.bits.unk_07;
                menuBadge->decks[deck][slot].iconIndex        = pinData[pinId].iconIndex;
                menuBadge->decks[deck][slot].psychId          = raw->psychId;
                menuBadge->decks[deck][slot].unk_1E           = raw->unk_04;
                menuBadge->decks[deck][slot].brand            = raw->brand;
                menuBadge->decks[deck][slot].ppCurve          = raw->ppCurve;
                menuBadge->decks[deck][slot].pinClass         = raw->pinClass;
                menuBadge->decks[deck][slot].price            = raw->price;
                menuBadge->decks[deck][slot].priceGrowth      = raw->priceGrowth;
                menuBadge->decks[deck][slot].attack           = raw->attack;
                menuBadge->decks[deck][slot].attackGrowth     = raw->attackGrowth;
                menuBadge->decks[deck][slot].durationType     = raw->durationType;
                menuBadge->decks[deck][slot].duration         = raw->duration;
                menuBadge->decks[deck][slot].durationGrowth   = raw->durationGrowth;
                menuBadge->decks[deck][slot].bootType         = raw->bootType;
                menuBadge->decks[deck][slot].bootTime         = raw->bootTime;
                menuBadge->decks[deck][slot].bootTimeGrowth   = raw->bootTimeGrowth;
                menuBadge->decks[deck][slot].rebootType       = raw->rebootType;
                menuBadge->decks[deck][slot].rebootTime       = raw->rebootTime;
                menuBadge->decks[deck][slot].rebootTimeGrowth = raw->rebootTimeGrowth;
                menuBadge->decks[deck][slot].abilityId        = raw->abilityId;
                menuBadge->decks[deck][slot].maxLevel         = raw->maxLevel;
                menuBadge->decks[deck][slot].inputType        = raw->inputType;
                menuBadge->decks[deck][slot].tinPinWeight     = parm->weight + 1;
                menuBadge->decks[deck][slot].tinPinSpin       = parm->spin / 409;
                menuBadge->decks[deck][slot].tinPinKOLength   = parm->koLength / 60;
                menuBadge->decks[deck][slot].whammyStinger    = parm->whammyStinger;
                menuBadge->decks[deck][slot].whammyBomber     = parm->whammyBomber;
                menuBadge->decks[deck][slot].whammyHammer     = parm->whammyHammer;
                menuBadge->decks[deck][slot].whammyHand       = parm->whammyHand;
                menuBadge->decks[deck][slot].maxPP =
                    func_02023480(menuBadge->decks[deck][slot].maxLevel, menuBadge->decks[deck][slot].ppCurve);
                if (func_ov043_02090194(raw->psychId, raw->abilityId, raw->inputType) == 1) {
                    menuBadge->decks[deck][slot].unk_16 = 2;
                } else {
                    menuBadge->decks[deck][slot].unk_16 = gSaveData.pinLayouts[deck][slot].unk_08;
                }
            }
        }
    }

    for (i = 0; i < 256; i++) {
        u16 pinId = gSaveData.stockpilePins[i].pinID;

        if (pinId == 0xFFFF) {
            menuBadge->stockpile[i] = MenuBadge_EmptyEntry;
        } else {
            RawPinData*    raw = &pinData[pinId];
            MenuBadgeParm* parm;

            menuBadge->stockpile[i].pinId      = pinId;
            menuBadge->stockpile[i].battlePP   = gSaveData.stockpilePins[i].battlePP;
            menuBadge->stockpile[i].minglePP   = gSaveData.stockpilePins[i].minglePP;
            menuBadge->stockpile[i].shutdownPP = gSaveData.stockpilePins[i].shutdownPP;
            menuBadge->stockpile[i].level      = gSaveData.stockpilePins[i].flags.bits.level;
            menuBadge->stockpile[i].levelPP    = MenuBadge_GetLevelPP(menuBadge->stockpile[i].level, raw->ppCurve);
            menuBadge->stockpile[i].totalPP    = MenuBadge_CalcTotalPP(
                menuBadge->stockpile[i].battlePP, menuBadge->stockpile[i].minglePP, menuBadge->stockpile[i].shutdownPP);
            menuBadge->stockpile[i].nextLevelPP =
                MenuBadge_GetNextLevelPP(menuBadge->stockpile[i].level, raw->maxLevel, raw->ppCurve);
            menuBadge->stockpile[i].deckSlot         = 0;
            menuBadge->stockpile[i].count            = 1;
            parm                                     = &parmData[pinId];
            menuBadge->stockpile[i].unk_18           = gSaveData.stockpilePins[i].flags.bits.unk_07;
            menuBadge->stockpile[i].iconIndex        = pinData[pinId].iconIndex;
            menuBadge->stockpile[i].psychId          = raw->psychId;
            menuBadge->stockpile[i].unk_1E           = raw->unk_04;
            menuBadge->stockpile[i].brand            = raw->brand;
            menuBadge->stockpile[i].ppCurve          = raw->ppCurve;
            menuBadge->stockpile[i].pinClass         = raw->pinClass;
            menuBadge->stockpile[i].price            = raw->price;
            menuBadge->stockpile[i].priceGrowth      = raw->priceGrowth;
            menuBadge->stockpile[i].attack           = raw->attack;
            menuBadge->stockpile[i].attackGrowth     = raw->attackGrowth;
            menuBadge->stockpile[i].durationType     = raw->durationType;
            menuBadge->stockpile[i].duration         = raw->duration;
            menuBadge->stockpile[i].durationGrowth   = raw->durationGrowth;
            menuBadge->stockpile[i].bootType         = raw->bootType;
            menuBadge->stockpile[i].bootTime         = raw->bootTime;
            menuBadge->stockpile[i].bootTimeGrowth   = raw->bootTimeGrowth;
            menuBadge->stockpile[i].rebootType       = raw->rebootType;
            menuBadge->stockpile[i].rebootTime       = raw->rebootTime;
            menuBadge->stockpile[i].rebootTimeGrowth = raw->rebootTimeGrowth;
            menuBadge->stockpile[i].abilityId        = raw->abilityId;
            menuBadge->stockpile[i].maxLevel         = raw->maxLevel;
            menuBadge->stockpile[i].inputType        = raw->inputType;
            menuBadge->stockpile[i].tinPinWeight     = parm->weight + 1;
            menuBadge->stockpile[i].tinPinSpin       = parm->spin / 409;
            menuBadge->stockpile[i].tinPinKOLength   = parm->koLength / 60;
            menuBadge->stockpile[i].whammyStinger    = parm->whammyStinger;
            menuBadge->stockpile[i].whammyBomber     = parm->whammyBomber;
            menuBadge->stockpile[i].whammyHammer     = parm->whammyHammer;
            menuBadge->stockpile[i].whammyHand       = parm->whammyHand;
            menuBadge->stockpile[i].maxPP = func_02023480(menuBadge->stockpile[i].maxLevel, menuBadge->stockpile[i].ppCurve);
            if (func_ov043_02090194(raw->psychId, raw->abilityId, raw->inputType) == 1) {
                menuBadge->stockpile[i].unk_16 = 2;
            } else {
                menuBadge->stockpile[i].unk_16 = 0;
            }
        }
    }

    for (i = 0; i < 304; i++) {
        u16 pinId = gSaveData.masteredPins[i].pinID;

        if (pinId == 0xFFFF) {
            menuBadge->mastered[i] = MenuBadge_EmptyEntry;
        } else {
            RawPinData*    raw = &pinData[pinId];
            MenuBadgeParm* parm;

            menuBadge->mastered[i].pinId            = pinId;
            menuBadge->mastered[i].battlePP         = 80;
            menuBadge->mastered[i].minglePP         = 80;
            menuBadge->mastered[i].shutdownPP       = 80;
            menuBadge->mastered[i].level            = raw->maxLevel;
            menuBadge->mastered[i].levelPP          = 0xFFFF;
            menuBadge->mastered[i].totalPP          = 0xFFFF;
            menuBadge->mastered[i].nextLevelPP      = 0xFFFF;
            menuBadge->mastered[i].deckSlot         = 0;
            parm                                    = &parmData[pinId];
            menuBadge->mastered[i].count            = gSaveData.masteredPins[i].count;
            menuBadge->mastered[i].unk_18           = gSaveData.masteredPins[i].flags.bits.unk_07;
            menuBadge->mastered[i].iconIndex        = pinData[pinId].iconIndex;
            menuBadge->mastered[i].psychId          = raw->psychId;
            menuBadge->mastered[i].unk_1E           = raw->unk_04;
            menuBadge->mastered[i].brand            = raw->brand;
            menuBadge->mastered[i].ppCurve          = raw->ppCurve;
            menuBadge->mastered[i].pinClass         = raw->pinClass;
            menuBadge->mastered[i].price            = raw->price;
            menuBadge->mastered[i].priceGrowth      = raw->priceGrowth;
            menuBadge->mastered[i].attack           = raw->attack;
            menuBadge->mastered[i].attackGrowth     = raw->attackGrowth;
            menuBadge->mastered[i].durationType     = raw->durationType;
            menuBadge->mastered[i].duration         = raw->duration;
            menuBadge->mastered[i].durationGrowth   = raw->durationGrowth;
            menuBadge->mastered[i].bootType         = raw->bootType;
            menuBadge->mastered[i].bootTime         = raw->bootTime;
            menuBadge->mastered[i].bootTimeGrowth   = raw->bootTimeGrowth;
            menuBadge->mastered[i].rebootType       = raw->rebootType;
            menuBadge->mastered[i].rebootTime       = raw->rebootTime;
            menuBadge->mastered[i].rebootTimeGrowth = raw->rebootTimeGrowth;
            menuBadge->mastered[i].abilityId        = raw->abilityId;
            menuBadge->mastered[i].maxLevel         = raw->maxLevel;
            menuBadge->mastered[i].inputType        = raw->inputType;
            menuBadge->mastered[i].tinPinWeight     = parm->weight + 1;
            menuBadge->mastered[i].tinPinSpin       = parm->spin / 409;
            menuBadge->mastered[i].tinPinKOLength   = parm->koLength / 60;
            menuBadge->mastered[i].whammyStinger    = parm->whammyStinger;
            menuBadge->mastered[i].whammyBomber     = parm->whammyBomber;
            menuBadge->mastered[i].whammyHammer     = parm->whammyHammer;
            menuBadge->mastered[i].whammyHand       = parm->whammyHand;
            menuBadge->mastered[i].maxPP = func_02023480(menuBadge->mastered[i].maxLevel, menuBadge->mastered[i].ppCurve);
            if (func_ov043_02090194(raw->psychId, raw->abilityId, raw->inputType) == 1) {
                menuBadge->mastered[i].unk_16 = 2;
            } else {
                menuBadge->mastered[i].unk_16 = 0;
            }
        }
    }
    Mem_Free(&gDebugHeap, pinData);
    Mem_Free(&gDebugHeap, parmData);
}

void MenuBadge_InitBadgeData(MenuBadgeObject* menuBadge) {
    s32 i;

    MenuBadge_LoadFromSave(menuBadge);
    for (i = 0; i < 4; i++) {
        menuBadge->arrangeButtonPressed[i] = 0;
    }
    if (gSaveData.playerStats.pinAutoArrange == 1) {
        if (gSaveData.playerStats.pinAutoArrangeBy == 0) {
            menuBadge->autoArrangeBy[0] = 1;
            menuBadge->autoArrangeBy[1] = 0;
        } else {
            menuBadge->autoArrangeBy[0] = 0;
            menuBadge->autoArrangeBy[1] = 1;
        }
        menuBadge->autoArrange = 1;
    } else {
        menuBadge->autoArrangeBy[0] = 0;
        menuBadge->autoArrangeBy[1] = 0;
        menuBadge->autoArrange      = 0;
    }
    if (menuBadge->autoArrange == 1) {
        if (menuBadge->autoArrangeBy[0] == 1) {
            MenuBadge_SortList(menuBadge, 0, 0);
            MenuBadge_SortList(menuBadge, 1, 0);
        } else {
            MenuBadge_SortList(menuBadge, 0, 1);
            MenuBadge_SortList(menuBadge, 1, 1);
        }
    }
    for (i = 0; i < 6; i++) {
        menuBadge->slots[i] = &menuBadge->decks[menuBadge->currentDeck][i];
    }
    for (i = 0; i < 24; i++) {
        menuBadge->slots[6 + i] = &menuBadge->stockpile[i];
    }
    if (menuBadge->slots[0]->pinId != 0xFFFF) {
        menuBadge->cursorBadge = *menuBadge->slots[0];
    } else {
        menuBadge->cursorBadge = MenuBadge_EmptyEntry;
    }
    menuBadge->dragBadge     = MenuBadge_EmptyEntry;
    menuBadge->deckSlotCount = MenuBadge_GetDeckSlotCount();
    menuBadge->moneyCapLevel = MenuBadge_GetMoneyCapLevel();
    menuBadge->unk_AF08      = func_ov043_0208fed8();
    MenuBadge_InitMasteredCounts(menuBadge);
}

void MenuBadge_WriteBackToSave(MenuBadgeObject* menuBadge) {
    s32 i;
    s32 deck;

    gSaveData.playerStats.money   = menuBadge->money;
    gSaveData.playerStats.pinDeck = menuBadge->currentDeck;
    if (menuBadge->flags & MENUBADGE_FLAG_QUICK_SELL) {
        gSaveData.playerStats.pinQuickSell = 1;
    } else {
        gSaveData.playerStats.pinQuickSell = 0;
    }

    for (deck = 0; deck < 4; deck++) {
        for (i = 0; i < 6; i++) {
            if (menuBadge->decks[deck][i].pinId == 0xFFFF) {
                gSaveData.pinLayouts[deck][i].pinID             = 0xFFFF;
                gSaveData.pinLayouts[deck][i].flags.bits.level  = 1;
                gSaveData.pinLayouts[deck][i].battlePP          = 0;
                gSaveData.pinLayouts[deck][i].minglePP          = 0;
                gSaveData.pinLayouts[deck][i].shutdownPP        = 0;
                gSaveData.pinLayouts[deck][i].unk_08            = 0;
                gSaveData.pinLayouts[deck][i].flags.bits.unk_07 = 0;
            } else {
                gSaveData.pinLayouts[deck][i].pinID             = menuBadge->decks[deck][i].pinId;
                gSaveData.pinLayouts[deck][i].flags.bits.level  = menuBadge->decks[deck][i].level;
                gSaveData.pinLayouts[deck][i].battlePP          = menuBadge->decks[deck][i].battlePP;
                gSaveData.pinLayouts[deck][i].minglePP          = menuBadge->decks[deck][i].minglePP;
                gSaveData.pinLayouts[deck][i].shutdownPP        = menuBadge->decks[deck][i].shutdownPP;
                gSaveData.pinLayouts[deck][i].unk_08            = menuBadge->decks[deck][i].unk_16;
                gSaveData.pinLayouts[deck][i].flags.bits.unk_07 = menuBadge->decks[deck][i].unk_18;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        gSaveData.equippedPins[i].pinID             = gSaveData.pinLayouts[menuBadge->currentDeck][i].pinID;
        gSaveData.equippedPins[i].flags.bits.level  = gSaveData.pinLayouts[menuBadge->currentDeck][i].flags.bits.level;
        gSaveData.equippedPins[i].battlePP          = gSaveData.pinLayouts[menuBadge->currentDeck][i].battlePP;
        gSaveData.equippedPins[i].minglePP          = gSaveData.pinLayouts[menuBadge->currentDeck][i].minglePP;
        gSaveData.equippedPins[i].shutdownPP        = gSaveData.pinLayouts[menuBadge->currentDeck][i].shutdownPP;
        gSaveData.equippedPins[i].unk_08            = gSaveData.pinLayouts[menuBadge->currentDeck][i].unk_08;
        gSaveData.equippedPins[i].flags.bits.unk_07 = gSaveData.pinLayouts[menuBadge->currentDeck][i].flags.bits.unk_07;
    }

    for (i = 0; i < 256; i++) {
        if (menuBadge->stockpile[i].pinId == 0xFFFF) {
            gSaveData.stockpilePins[i].pinID             = 0xFFFF;
            gSaveData.stockpilePins[i].flags.bits.level  = 1;
            gSaveData.stockpilePins[i].battlePP          = 0;
            gSaveData.stockpilePins[i].minglePP          = 0;
            gSaveData.stockpilePins[i].shutdownPP        = 0;
            gSaveData.stockpilePins[i].flags.bits.unk_07 = 0;
        } else {
            gSaveData.stockpilePins[i].pinID             = menuBadge->stockpile[i].pinId;
            gSaveData.stockpilePins[i].flags.bits.level  = menuBadge->stockpile[i].level;
            gSaveData.stockpilePins[i].battlePP          = menuBadge->stockpile[i].battlePP;
            gSaveData.stockpilePins[i].minglePP          = menuBadge->stockpile[i].minglePP;
            gSaveData.stockpilePins[i].shutdownPP        = menuBadge->stockpile[i].shutdownPP;
            gSaveData.stockpilePins[i].flags.bits.unk_07 = menuBadge->stockpile[i].unk_18;
        }
    }

    for (i = 0; i < 304; i++) {
        if (menuBadge->mastered[i].pinId == 0xFFFF) {
            gSaveData.masteredPins[i].pinID             = 0xFFFF;
            gSaveData.masteredPins[i].count             = 0;
            gSaveData.masteredPins[i].flags.bits.level  = 100;
            gSaveData.masteredPins[i].flags.bits.unk_07 = 0;
        } else {
            gSaveData.masteredPins[i].pinID             = menuBadge->mastered[i].pinId;
            gSaveData.masteredPins[i].count             = menuBadge->mastered[i].count;
            gSaveData.masteredPins[i].flags.bits.level  = menuBadge->mastered[i].maxLevel;
            gSaveData.masteredPins[i].flags.bits.unk_07 = menuBadge->mastered[i].unk_18;
        }
    }

    if (menuBadge->autoArrange == 1) {
        gSaveData.playerStats.pinAutoArrange = 1;
        if (menuBadge->autoArrangeBy[0] == 1) {
            gSaveData.playerStats.pinAutoArrangeBy = 0;
        } else {
            gSaveData.playerStats.pinAutoArrangeBy = 1;
        }
    } else {
        gSaveData.playerStats.pinAutoArrange   = 0;
        gSaveData.playerStats.pinAutoArrangeBy = 2;
    }
}

s16 MenuBadge_GetSlotAtPoint(s16 x, s16 y, u8 slotCount) {
    s16 unused[2] = {0xF0, 0x1B}; // Never read, but its template is still emitted
    s16 i;

    for (i = 0; i < slotCount; i++) {
        if (MenuBadge_IsPointInRect(x, y, (s16)(MenuBadge_SlotPositions[i].x - 15), (s16)(MenuBadge_SlotPositions[i].y - 14),
                                    30, 37) == 1)
        {
            return i;
        }
    }
    for (i = 6; i < 30; i++) {
        s32 shrink;
        if (MenuBadge_SlotPositions[i].x == 0xE5) {
            shrink = 3;
        } else {
            shrink = 0;
        }
        if (MenuBadge_IsPointInRect(x, y, (s16)(MenuBadge_SlotPositions[i].x - 15), (s16)(MenuBadge_SlotPositions[i].y - 14),
                                    30 - shrink, 37) == 1)
        {
            return i;
        }
    }
    return -1;
}

s16 MenuBadge_GetDropTargetAtPoint(s16 x, s16 y, u16 src, u8 slotCount) {
    s16 pos[2]  = {3, 0x41};
    s16 size[2] = {0xFA, 0x6F};
    s16 i;

    for (i = 0; i < slotCount; i++) {
        if (MenuBadge_IsPointInRect(x, y, (s16)(MenuBadge_SlotPositions[i].x - 15), (s16)(MenuBadge_SlotPositions[i].y - 14),
                                    30, 37) == 1)
        {
            return i;
        }
    }
    if (src < 6) {
        if (MenuBadge_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
            return 40;
        }
    } else {
        for (i = 6; i < 30; i++) {
            if (MenuBadge_IsPointInRect(x, y, (s16)(MenuBadge_SlotPositions[i].x - 15),
                                        (s16)(MenuBadge_SlotPositions[i].y - 14), 30, 37) == 1)
            {
                return i;
            }
        }
    }
    return -1;
}

s16 MenuBadge_GetButtonAtPoint(s16 x, s16 y) {
    s16 pos[3][2] = {
        {0xB4, 2},
        {0xCD, 2},
        {0xE6, 2},
    };
    s16 size[2] = {0x17, 0x13};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuBadge_GetListTabAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0x64, 0xB2},
        {0xAF, 0xB2},
    };
    s16 size[2] = {0x48, 0x48};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuBadge_IsPointOnScrollBar(s16 x, s16 y) {
    s16 pos[2]  = {0xF1, 0x4F};
    s16 size[2] = {0xF, 0x54};

    if (MenuBadge_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuBadge_IsPointOnScrollKnob(s16 x, s16 y, s16 targetX, s16 targetY) {
    s16 size[2] = {0xD, 0x15};

    if (MenuBadge_IsPointInRect(x, y, (s16)(targetX - 6), (s16)(targetY - 21), size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuBadge_GetScrollArrowAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0xF1, 0x42},
        {0xF1, 0xA6},
    };
    s16 size[2] = {0xF, 9};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuBadge_IsPointOnSellBox(s16 x, s16 y) {
    s16 pos[2]    = {0xD1, 0x18};
    s16 unused[2] = {0x0B, 0x0A}; // Never read, but its template is still emitted
    s16 size[2]   = {0x2B, 0x28};

    if (MenuBadge_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

MenuBadgeEntry MenuBadge_EmptyEntry = {
    .pinId     = 0xFFFF,
    .count     = 1,
    .psychId   = 0xFFFF,
    .pinClass  = PIN_CLASS_NONE,
    .abilityId = 0xFF,
    .maxLevel  = 100,
};

u32 MenuBadge_GetTabAtPoint(s16 x, s16 y) {
    s16 pos[3][2] = {
        {0x4E, 0},
        {0x6F, 0},
        {0x90, 0},
    };
    s16 size[2] = {0x20, 0xC};
    s16 i;

    for (i = 0; i < 3; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuBadge_CanDragSlot(MenuBadgeObject* menuBadge, s16 slot) {
    MenuBadgeEntry* entry;

    if (slot == -1) {
        return 0;
    }
    entry = menuBadge->slots[slot];
    if (entry->pinId == 0xFFFF) {
        return 0;
    }
    if (slot < 6) {
        if (entry->pinId == 0xFFFF) {
            return 0;
        }
    } else if (entry->count == 0) {
        return 0;
    }
    return 1;
}

s32 MenuBadge_CanDropOnSlot(MenuBadgeObject* menuBadge, s16 src, s16 dst) {
    if (dst == -1) {
        return 0;
    }
    if (dst != src) {
        return 1;
    }
    return 0;
}

s16 MenuBadge_GetSellButtonAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0x2B, 0x6C},
        {0x94, 0x6C},
    };
    s16 size[2] = {0x40, 0x13};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuBadge_GetSortButtonAtPoint(s16 x, s16 y) {
    s16 pos[4][2] = {
        {0x1A, 0x3E},
        {0x1A, 0x55},
        {0x1A, 0x72},
        {0xB8, 0x85},
    };
    s16 size[4][2] = {
        {0x40, 0x13},
        {0x40, 0x13},
        {0x40, 0x13},
        {0x30, 0x13},
    };
    s16 i;

    for (i = 0; i < 4; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuBadge_GetSellCountArrowAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0xD9, 0x3D},
        {0xD9, 0x58},
    };
    s16 size[2] = {0x10, 0xF};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuBadge_GetHelpButtonAtPoint(s16 x, s16 y) {
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
        if (MenuBadge_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuBadge_GetDeckTabAtPoint(s16 x, s16 y) {
    s16 posX[4] = {0x80, 0x8B, 0x96, 0xA1};
    s16 size[2] = {0xA, 0xA};
    s16 i;

    for (i = 0; i < 4; i++) {
        if (MenuBadge_IsPointInRect(x, y, posX[i], 0xF, size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

void MenuBadge_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &MenuBadge_BinIdentifiers[binIndex]);
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

void MenuBadge_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
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

void MenuBadge_ReleaseBgResource(MenuBgResource* res, s32 engine) {
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

void MenuBadge_ReleaseBgScreen(MenuBgResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
    } else {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
    }
}

void MenuBadge_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    MenuBadge_ReleaseBgResource(res, engine);
    MenuBadge_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void MenuBadge_ReloadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
    MenuBadge_ReleaseBgScreen(res, engine);
    MenuBadge_LoadBgScreen(res, data, engine, layer, screenIndex);
}

void MenuBadge_ClearBgResource(MenuBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void MenuBadge_LoadBackgrounds(MenuBadgeObject* menuBadge) {
    s32             i;
    MenuBgResource* mainRes = &menuBadge->resources[0];
    MenuBgResource* subRes  = &menuBadge->resources[4];

    for (i = 0; i < 4; i++) {
        MenuBadge_ClearBgResource(mainRes);
        MenuBadge_ClearBgResource(subRes);
        mainRes += 1;
        subRes += 1;
    }

    MenuBadge_LoadBgResource(&menuBadge->resources[5], DISPLAY_MAIN, 1, 6, 15, 1);
    MenuBadge_LoadBgResource(&menuBadge->resources[7], DISPLAY_MAIN, 3, 0, 0, 10);
    MenuBadge_LoadBgScreen(&menuBadge->resources[6], menuBadge->resources[7].data, DISPLAY_MAIN, 2, 5);
    MenuBadge_LoadBgResource(&menuBadge->resources[0], DISPLAY_SUB, 0, 7, 15, 1);
    MenuBadge_LoadBgResource(&menuBadge->resources[3], DISPLAY_SUB, 3, 3, 1, 1);
    MenuBadge_LoadBgScreen(&menuBadge->resources[1], menuBadge->resources[3].data, DISPLAY_SUB, 1, 6);
    MenuBadge_LoadBgScreen(&menuBadge->resources[2], menuBadge->resources[3].data, DISPLAY_SUB, 2, 7);
}

void MenuBadge_UpdateBackgrounds(MenuBadgeObject* menuBadge) {
    return;
}

void MenuBadge_ReleaseBackgrounds(MenuBadgeObject* menuBadge) {
    MenuBadge_ReleaseBgResource(&menuBadge->resources[5], DISPLAY_MAIN);
    MenuBadge_ReleaseBgResource(&menuBadge->resources[7], DISPLAY_MAIN);
    MenuBadge_ReleaseBgScreen(&menuBadge->resources[6], DISPLAY_MAIN);
    MenuBadge_ReleaseBgResource(&menuBadge->resources[0], DISPLAY_SUB);
    MenuBadge_ReleaseBgResource(&menuBadge->resources[3], DISPLAY_SUB);
    MenuBadge_ReleaseBgScreen(&menuBadge->resources[1], DISPLAY_SUB);
    MenuBadge_ReleaseBgScreen(&menuBadge->resources[2], DISPLAY_SUB);
}
