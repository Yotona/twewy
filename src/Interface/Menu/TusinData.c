#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Math/Random.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/Tusin.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Pins.h"
#include "Player/Stats.h"
#include "Save.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"
#include <nitro/mi/cpumem.h>
#include <nitro/os/os_owner.h>
#include <nitro/rtc.h>

void func_0203a96c(u8* macAddress);
s32  func_020417e0(RTCDate* date, RTCTime* time);
s64  func_02041f14(RTCDate* date, RTCTime* time);
void Stats_AddExperience(u32 exp);
u16  Inventory_GetNoiseReportCount(void);
u16  Inventory_GetMasteredPinCount(void);
u16  Inventory_GetCollectedItemCount(void);

#ifdef REGION_USA
extern EquippedPin* Tusin_SurePins[6];

TusinBadge Tusin_EmptyBadge = {
    .pinID         = 0xFFFF,
    .brand         = 14,
    .levelPP       = 1,
    .nextLevelPP   = 100,
    .ppToNextLevel = 100,
    .slot          = 1,
    .evolveChoice  = 2,
    .evolveLevel   = {5, 5},
};
#endif

s32 Tusin_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void Tusin_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

#ifdef REGION_USA
void Tusin_LoadPinData(RawPinData* pinData, u16 index) {
    DatMgr_ReleaseData(
        DatMgr_LoadRawDataWithOffset(1, pinData, sizeof(RawPinData), &Tusin_BinIdentifiers[11], index * sizeof(RawPinData)));
}
#endif

void Tusin_LoadItemData(RawItemData* itemData, u16 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, itemData, sizeof(RawItemData), &Tusin_BinIdentifiers[12],
                                                    index * sizeof(RawItemData)));
}

void Tusin_LoadFoodData(RawFoodData* foodData, u16 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, foodData, sizeof(RawFoodData), &Tusin_BinIdentifiers[13],
                                                    index * sizeof(RawFoodData)));
}

void Tusin_LoadTreasureData(RawTreasureData* treasureData, u16 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, treasureData, sizeof(RawTreasureData), &Tusin_BinIdentifiers[14],
                                                    index * sizeof(RawTreasureData)));
}

u16 Tusin_GetRandomCivvyGift(void) {
    u16 items[39] = {
        0x133, 0x137, 0x145, 0x147, 0x149, 0x155, 0x158, 0x15B, 0x15D, 0x171, 0x180, 0x182, 0x184,
        0x186, 0x18F, 0x191, 0x194, 0x196, 0x198, 0x19A, 0x1B1, 0x1B3, 0x1B5, 0x1B7, 0x1B9, 0x1BB,
        0x1CC, 0x1CE, 0x1D0, 0x1D2, 0x1EB, 0x1ED, 0x1EF, 0x1F1, 0x203, 0x205, 0x206, 0x208, 0x20A,
    };
    u16 index = RNG_Next(0xFFFF) % 39;

    return items[index];
}

u16 Tusin_GetAlienGift(u16 index) {
    u16 items[30] = {
        0x237, 0x23E, 0x230, 0x231, 0x17F, 0x232, 0x284, 0x1C7, 0x246, 0x249, 0x235, 0x22D, 0x22F, 0x1E5, 0x228,
        0x1E0, 0x243, 0x21A, 0x17B, 0x23F, 0x1C1, 0x245, 0x274, 0x232, 0x23C, 0x233, 0x277, 0x240, 0x167, 0x1A1,
    };

    return items[index];
}

void Tusin_AddBravery(u8 partner, u8 amount) {
    gSaveData.playerStats.bravery += amount;
    if (gSaveData.playerStats.bravery > 999) {
        gSaveData.playerStats.bravery = 999;
    }
    if (partner == 0xFF) {
        return;
    }
    gSaveData.friendStats[partner].bravery += amount;
    if (gSaveData.friendStats[partner].bravery > 999) {
        gSaveData.friendStats[partner].bravery = 999;
    }
}

s32 Tusin_ClampMoney(TusinObject* tusin, u8 capLevel) {
    if (capLevel == 3) {
        if (tusin->money >= 9999999) {
            tusin->money = 9999999;
            return 1;
        }
    } else if (capLevel == 2) {
        if (tusin->money >= 999999) {
            tusin->money = 999999;
            return 1;
        }
    } else if (capLevel == 1) {
        if (tusin->money >= 99999) {
            tusin->money = 99999;
            return 1;
        }
    } else {
        if (tusin->money >= 99999) {
            tusin->money = 99999;
            return 1;
        }
    }
    return 0;
}

s32 Tusin_GetMoneyCapLevel(void) {
    if (Inventory_GetOwnedCount(ITEM_WALLET_FAT_CAT_WALLET) != 0) {
        return 3;
    }
    if (Inventory_GetOwnedCount(ITEM_WALLET_TRENDY_WALLET) != 0) {
        return 2;
    }
    if (Inventory_GetOwnedCount(ITEM_WALLET_MY_FIRST_WALLET) != 0) {
        return 1;
    }
    return 0;
}

#ifndef REGION_USA
s64 func_ov044_02085060(void) {
    RTCDate date;
    RTCTime time;

    func_020417e0(&date, &time);
    return func_02041f14(&date, &time);
}

s64 func_ov044_02085088(PackedDateTime* dateTime) {
    RTCDate date;
    RTCTime time;

    date.year   = dateTime->year;
    date.month  = dateTime->month;
    date.day    = dateTime->day;
    time.hour   = dateTime->hour;
    time.minute = dateTime->minute;
    time.second = dateTime->second;
    return func_02041f14(&date, &time);
}

s64 func_ov044_02085104(s64 a, s64 b) {
    if (a < b) {
        return 0;
    }
    return a - b;
}
#endif

u8 Tusin_GetBadgeSlotCount(void) {
    u8 count = Inventory_GetOwnedCount(ITEM_STICKER_EXTRA_SLOT) + 2;

    if (count > 6) {
        count = 6;
    }
    return count;
}

#ifdef REGION_USA
u32 Tusin_GetBadgeTotalPP(TusinObject* tusin, u16 index) {
    TusinBadge* badge = &tusin->badges[index];

    return badge->pp[2] + (badge->pp[0] + badge->pp[1]);
}

u32 Tusin_GetBadgeLevelPP(TusinObject* tusin, u16 index, s32 ppCurve) {
    return func_02023480(tusin->badges[index].level, ppCurve);
}

u32 Tusin_GetBadgeNextLevelPP(TusinObject* tusin, u16 index, s32 ppCurve) {
    TusinBadge* badge = &tusin->badges[index];

    if (badge->level == badge->maxLevel) {
        return 0xFFFF;
    }
    return func_02023480((u8)(badge->level + 1), ppCurve);
}
#endif

void Tusin_WriteLastProfile(void) {
    OSOwnerInfo ownerInfo;
    RTCDate     date;
    RTCTime     time;

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
    gSaveData.lastChapter         = gSaveData.chapter;
    gSaveData.lastArea            = gSaveData.currentArea;
    gSaveData.lastPlayerStats     = gSaveData.playerStats;

    for (u16 i = 0; i < 3; i++) {
        gSaveData.lastFriendStats[i] = gSaveData.friendStats[i];
    }
    for (u16 i = 0; i < 6; i++) {
        gSaveData.lastEquippedPins[i] = gSaveData.equippedPins[i];
    }

    gSaveData.lastBadgeSlots = Tusin_GetBadgeSlotCount();
    gSaveData.lastGiftItemId = gSaveData.mingleShop.giftItemId;
}

#ifdef REGION_USA
void Tusin_LoadEquipment(TusinObject* tusin) {
    RawPinData pinData;

    for (u16 i = 0; i < 6; i++) {
        u16 pinID = gSaveData.equippedPins[i].pinID;

        if (pinID == 0xFFFF) {
            tusin->badges[i] = Tusin_EmptyBadge;
        } else {
            Tusin_LoadPinData(&pinData, pinID);
            tusin->badges[i].pinID         = pinID;
            tusin->badges[i].brand         = pinData.brand;
            tusin->badges[i].ppCurve       = pinData.ppCurve;
            tusin->badges[i].pp[0]         = gSaveData.equippedPins[i].battlePP;
            tusin->badges[i].pp[1]         = gSaveData.equippedPins[i].minglePP;
            tusin->badges[i].pp[2]         = gSaveData.equippedPins[i].shutdownPP;
            tusin->badges[i].startPP[0]    = tusin->badges[i].pp[0];
            tusin->badges[i].startPP[1]    = tusin->badges[i].pp[1];
            tusin->badges[i].startPP[2]    = tusin->badges[i].pp[2];
            tusin->badges[i].level         = gSaveData.equippedPins[i].flags.bits.level;
            tusin->badges[i].maxLevel      = pinData.maxLevel;
            tusin->badges[i].levelPP       = Tusin_GetBadgeLevelPP(tusin, i, tusin->badges[i].ppCurve);
            tusin->badges[i].nextLevelPP   = Tusin_GetBadgeNextLevelPP(tusin, i, tusin->badges[i].ppCurve);
            tusin->badges[i].totalPP       = Tusin_GetBadgeTotalPP(tusin, i);
            tusin->badges[i].ppToNextLevel = tusin->badges[i].nextLevelPP - tusin->badges[i].totalPP;
            tusin->badges[i].slot          = i;
            tusin->badges[i].unk_2F        = gSaveData.equippedPins[i].unk_08;
            tusin->badges[i].unk_30        = 1;
            tusin->badges[i].unk_31        = gSaveData.equippedPins[i].flags.bits.unk_07;
            for (u16 j = 0; j < 2; j++) {
                tusin->badges[i].evolveLevel[j]     = pinData.evolveLevel[j];
                tusin->badges[i].evolveCondition[j] = pinData.evolveCondition[j];
                tusin->badges[i].evolvePinID[j]     = pinData.evolvePinID[j];
                tusin->badges[i].evolveChoice       = 2;
                tusin->badges[i].flags              = 0;
            }
        }
    }
}
#endif

void Tusin_InitProfile(TusinObject* tusin) {
    OSOwnerInfo ownerInfo;

    tusin->money         = gSaveData.playerStats.money;
    tusin->moneyCapLevel = Tusin_GetMoneyCapLevel();
    func_0203a96c(tusin->macAddress);
    OS_GetOwnerInfo(&ownerInfo);
    MI_CpuSet(tusin->nickName, 0, sizeof(tusin->nickName));
    MI_CpuCopyU8(ownerInfo.nickName, tusin->nickName, sizeof(tusin->nickName));
    MI_CpuSet(tusin->message, 0, sizeof(tusin->message));
    MI_CpuCopyU8(ownerInfo.message, tusin->message, sizeof(tusin->message));
    tusin->profile.experience = gSaveData.experience;
    tusin->profile.shop       = gSaveData.mingleShop;
    for (u16 i = 0; i < 50; i++) {
        tusin->profile.friends[i] = gSaveData.mingleFriends[i];
    }
    tusin->statusMsg = SYSMSG_MINGLE_NOW_MINGLING;
#ifdef REGION_USA
    Tusin_LoadEquipment(tusin);
#endif
}

#ifdef REGION_USA
u32 Tusin_CalcMinglePP(u16 civviesMet, u16 espersMet, u16 aliensMet) {
    u32 esperPP = espersMet * 50;
    u32 alienPP = aliensMet * 100;

    return (u16)alienPP + ((u16)(civviesMet * 20) + (u16)esperPP);
}

void Tusin_ExportSurePins(TusinObject* tusin) {
    u16 i;

    for (i = 0; i < 6; i++) {
        Tusin_SurePins[i] = NULL;
    }
    for (i = 0; i < 6; i++) {
        EquippedPin* pin = Mem_AllocHeapTail(&gMainHeap, sizeof(EquippedPin));

        Mem_SetSequence(&gMainHeap, pin, "glbSetBdgData");
        Tusin_SurePins[i] = pin;
    }
    for (i = 0; i < 6; i++) {
        Tusin_SurePins[i]->pinID             = gSaveData.equippedPins[i].pinID;
        Tusin_SurePins[i]->battlePP          = gSaveData.equippedPins[i].battlePP;
        Tusin_SurePins[i]->minglePP          = gSaveData.equippedPins[i].minglePP;
        Tusin_SurePins[i]->shutdownPP        = gSaveData.equippedPins[i].shutdownPP;
        Tusin_SurePins[i]->unk_08            = gSaveData.equippedPins[i].unk_08;
        Tusin_SurePins[i]->flags.bits.level  = gSaveData.equippedPins[i].flags.bits.level;
        Tusin_SurePins[i]->flags.bits.unk_07 = gSaveData.equippedPins[i].flags.bits.unk_07;
    }
}

u16 Tusin_GetDominantPPType(u32 battlePP, u32 minglePP, u32 shutdownPP) {
    u32 mingle = minglePP * 20;

    if (mingle >= shutdownPP * 9) {
        return (mingle >= battlePP) ? 1 : 0;
    }
    if (shutdownPP * 9 >= battlePP) {
        return 2;
    }
    return 0;
}

u8 Tusin_CheckBadgeEvolution(TusinObject* tusin, u16 index) {
    u16 dominant = Tusin_GetDominantPPType(tusin->badges[index].pp[0], tusin->badges[index].pp[1], tusin->badges[index].pp[2]);
    TusinBadge* badge = &tusin->badges[index];
    u8          i;
    u8          type  = badge->unk_31;
    u8          level = badge->level;

    for (i = 0; i < 2; i++) {
        u8 cond = badge->evolveCondition[i];

        if (level == badge->evolveLevel[i]) {
            switch (cond) {
                case 1:
                    if (type == 1 && dominant == 0) {
                        return i;
                    }
                    break;
                case 2:
                    if (type == 1 && dominant == 1) {
                        return i;
                    }
                    break;
                case 3:
                    if (type == 1 && dominant == 2) {
                        return i;
                    }
                    break;
            }
        }
    }
    return 2;
}

void Tusin_EvolveBadge(TusinObject* tusin, u16 index, u16 pinID) {
    RawPinData pinData;

    Tusin_LoadPinData(&pinData, pinID);
    tusin->badges[index].pinID         = pinID;
    tusin->badges[index].brand         = pinData.brand;
    tusin->badges[index].ppCurve       = pinData.ppCurve;
    tusin->badges[index].pp[0]         = 0;
    tusin->badges[index].pp[1]         = 0;
    tusin->badges[index].pp[2]         = 0;
    tusin->badges[index].startPP[0]    = 0;
    tusin->badges[index].startPP[1]    = 0;
    tusin->badges[index].startPP[2]    = 0;
    tusin->badges[index].level         = 1;
    tusin->badges[index].maxLevel      = pinData.maxLevel;
    tusin->badges[index].levelPP       = Tusin_GetBadgeLevelPP(tusin, index, tusin->badges[index].ppCurve);
    tusin->badges[index].nextLevelPP   = Tusin_GetBadgeNextLevelPP(tusin, index, tusin->badges[index].ppCurve);
    tusin->badges[index].totalPP       = Tusin_GetBadgeTotalPP(tusin, index);
    tusin->badges[index].ppToNextLevel = tusin->badges[index].nextLevelPP - tusin->badges[index].totalPP;
    tusin->badges[index].slot          = index;
    tusin->badges[index].unk_30        = 1;
    tusin->badges[index].unk_31        = 1;
    for (u16 i = 0; i < 2; i++) {
        tusin->badges[index].evolveLevel[i]     = pinData.evolveLevel[i];
        tusin->badges[index].evolveCondition[i] = pinData.evolveCondition[i];
        tusin->badges[index].evolvePinID[i]     = pinData.evolvePinID[i];
    }
}

void Tusin_AddBadgePP(TusinObject* tusin, u16 index, u32 pp) {
    if (tusin->badges[index].pinID == 0xFFFF) {
        return;
    }
    if (tusin->badges[index].level == tusin->badges[index].maxLevel) {
        return;
    }

    if (pp >= tusin->badges[index].ppToNextLevel) {
        tusin->badges[index].pp[1] += tusin->badges[index].ppToNextLevel;
        tusin->badges[index].evolveChoice = Tusin_CheckBadgeEvolution(tusin, index);
        if (tusin->badges[index].evolveChoice != 2) {
            Tusin_EvolveBadge(tusin, index, tusin->badges[index].evolvePinID[tusin->badges[index].evolveChoice]);
            return;
        }
        pp -= tusin->badges[index].ppToNextLevel;
        tusin->badges[index].level++;
        tusin->badges[index].levelPP       = Tusin_GetBadgeLevelPP(tusin, index, tusin->badges[index].ppCurve);
        tusin->badges[index].nextLevelPP   = Tusin_GetBadgeNextLevelPP(tusin, index, tusin->badges[index].ppCurve);
        tusin->badges[index].totalPP       = Tusin_GetBadgeTotalPP(tusin, index);
        tusin->badges[index].ppToNextLevel = tusin->badges[index].nextLevelPP - tusin->badges[index].totalPP;
        Tusin_AddBadgePP(tusin, index, pp);
    } else {
        tusin->badges[index].pp[1] += pp;
    }
}
#endif

void Tusin_CommitResults(TusinObject* tusin) {
    u32 total;
    u16 i;

    gSaveData.civviesMet = tusin->civviesMet;
    gSaveData.espersMet  = tusin->espersMet;
    gSaveData.aliensMet  = tusin->aliensMet;

    total = gSaveData.unk_1D80 + tusin->totalMet;
    if (total > 0xFFFF) {
        total = 0xFFFF;
    }
    gSaveData.unk_1D80 = total;
#ifdef REGION_USA
    // JP awards this experience on the Result screen instead (Result_CommitSure).
    Stats_AddExperience(tusin->totalMet);
#endif

    if (tusin->totalRakedMoney != 0) {
        tusin->money += tusin->totalRakedMoney;
        Tusin_ClampMoney(tusin, tusin->moneyCapLevel);
    }

    for (i = 0; i < 50; i++) {
        if (tusin->unk_4C2[i] == 1) {
            gSaveData.mingleFriends[i].unk_8 &= 0xFF000000;
        }
    }

    gSaveData.playerStats.money = tusin->money;
    Tusin_AddBravery(gSaveData.playerStats.activePartner, gSaveData.espersMet);

#ifdef REGION_USA
    {
        u32 pp = Tusin_CalcMinglePP(tusin->civviesMet, tusin->espersMet, tusin->aliensMet);
        u8  layout;

        Tusin_ExportSurePins(tusin);
        for (i = 0; i < 6; i++) {
            Tusin_AddBadgePP(tusin, i, pp);
        }

        for (i = 0; i < 6; i++) {
            gSaveData.equippedPins[i].pinID             = tusin->badges[i].pinID;
            gSaveData.equippedPins[i].battlePP          = tusin->badges[i].pp[0];
            gSaveData.equippedPins[i].minglePP          = tusin->badges[i].pp[1];
            gSaveData.equippedPins[i].shutdownPP        = tusin->badges[i].pp[2];
            gSaveData.equippedPins[i].unk_08            = tusin->badges[i].unk_2F;
            gSaveData.equippedPins[i].flags.bits.level  = tusin->badges[i].level;
            gSaveData.equippedPins[i].flags.bits.unk_07 = tusin->badges[i].unk_31;
        }

        layout = gSaveData.playerStats.pinDeck;
        for (i = 0; i < 6; i++) {
            gSaveData.pinLayouts[layout][i].pinID             = gSaveData.equippedPins[i].pinID;
            gSaveData.pinLayouts[layout][i].battlePP          = gSaveData.equippedPins[i].battlePP;
            gSaveData.pinLayouts[layout][i].minglePP          = gSaveData.equippedPins[i].minglePP;
            gSaveData.pinLayouts[layout][i].shutdownPP        = gSaveData.equippedPins[i].shutdownPP;
            gSaveData.pinLayouts[layout][i].unk_08            = gSaveData.equippedPins[i].unk_08;
            gSaveData.pinLayouts[layout][i].flags.bits.level  = gSaveData.equippedPins[i].flags.bits.level;
            gSaveData.pinLayouts[layout][i].flags.bits.unk_07 = gSaveData.equippedPins[i].flags.bits.unk_07;
        }
    }
#endif
}

s32 Tusin_IsPointOnBtnF(s16 x, s16 y) {
    // Never read. Their initializer templates still land in .rodata ahead of the other hit tests'.
    s16 unusedPos[2][2] = {
        {205, 2},
        {230, 2},
    };
    s16 unusedSize[2] = {23, 19};
    s16 pos[2]        = {32, 172};
    s16 size[2]       = {52, 14};

    if (Tusin_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 Tusin_IsPointOnBtn(s16 x, s16 y) {
    s16 pos[2]  = {31, 114};
    s16 size[2] = {52, 28};

    if (Tusin_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s16 Tusin_GetBtnLRAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {31, 143},
        {73, 143},
    };
    s16 size[2] = {10, 28};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (Tusin_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

void Tusin_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &Tusin_BinIdentifiers[binIndex]);
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

void Tusin_ReleaseBgResource(MenuBgResource* res, s32 engine) {
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

void Tusin_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    Tusin_ReleaseBgResource(res, engine);
    Tusin_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void Tusin_ClearBgResource(MenuBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void Tusin_LoadBackgrounds(TusinObject* tusin) {
    s32             i;
    MenuBgResource* mainRes = &tusin->resources[0];
    MenuBgResource* subRes  = &tusin->resources[4];

    for (i = 0; i < 4; i++) {
        Tusin_ClearBgResource(mainRes);
        Tusin_ClearBgResource(subRes);
        mainRes += 1;
        subRes += 1;
    }

    Tusin_LoadBgResource(&tusin->resources[6], DISPLAY_SUB, 2, 1, 15, 1);
    Tusin_LoadBgResource(&tusin->resources[7], DISPLAY_SUB, 3, 0, 0, 3);
    Tusin_LoadBgResource(&tusin->resources[3], DISPLAY_MAIN, 3, 3, 0, 1);
}

void Tusin_UpdateBackgrounds(TusinObject* tusin) {}

void Tusin_ReleaseBackgrounds(TusinObject* tusin) {
    Tusin_ReleaseBgResource(&tusin->resources[6], DISPLAY_SUB);
    Tusin_ReleaseBgResource(&tusin->resources[7], DISPLAY_SUB);
    Tusin_ReleaseBgResource(&tusin->resources[3], DISPLAY_MAIN);
}

#ifdef REGION_USA
// Pins handed over to the Result screen, which frees them. Defined last so it sorts after the hit-test templates.
EquippedPin* Tusin_SurePins[6];
#endif
