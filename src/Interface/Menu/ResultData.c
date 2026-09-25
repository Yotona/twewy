#include "Engine/Core/Memory.h"
#include "Engine/Math/Random.h"
#include "Interface/Menu/Result.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Player/Stats.h"
#include "Save.h"
#include "common_data.h"

s32                 FX_Divide(s32, s32);
void                func_020415a4(void);
s32                 func_02041680(RTCDate* date);
s32                 func_020417e0(RTCDate* date, RTCTime* time);
s64                 func_02041f14(RTCDate* date, RTCTime* time);
void                Stats_AddExperience(u32 exp);
BgResource*         BgResMgr_AllocChar32(BgResMgr* mgr, void* charData, u32 charBase, u32 offset, u32 size);
BgResource*         BgResMgr_AllocScreen(BgResMgr* mgr, void* screenData, u32 screenBase, u32 screenSize);
extern EquippedPin* data_ov038_0209a204[6];

// This TU called PaletteMgr_AllocPalette with no prototype in scope, so palStart reaches it as a plain
// int instead of being narrowed to the s16 parameter.
#define PaletteMgr_AllocPaletteNoProto ((PaletteResource * (*)()) PaletteMgr_AllocPalette)

const u16 data_ov044_0208ec32[7] = {260, 259, 258, 257, 256, 255, 254};
const u16 data_ov044_0208ec40[7] = {20, 18, 16, 15, 14, 13, 12};

// clang-format off
const char* const Result_BrandNames[15] = {
    [0] = "‘l",
    [1] = "‹",
    [2] = "ŒÕ",
    [3] = "“e",
    [4] = "—´",
    [5] = "ŽÖ",
    [6] = "”n",
    [7] = "—r",
    [8] = "‰Ž",
    [9] = "’¹",
    [10] = "Œ¢",
    [11] = "’–",
    [12] = "”L",
    [13] = "ƒxƒC",
    [14] = "–³‚µ",
};

const char* const Result_AreaNames[21] = {
    [0] = "ƒXƒNƒ‰ƒ“ƒuƒ‹Œð·“_",
    [1] = "‚P‚O‚Sƒrƒ‹‘O",
    [2] = "ƒZƒ“ƒ^[ŠX“ü‚èŒû",
    [3] = "’‰Œ¢ƒnƒ`Œö‘œ‘O",
    [4] = "‚`‚l‚w‘O",
    [5] = "ç’¹‰ïŠÙ‘O",
    [6] = "ƒXƒyƒCƒ“â",
    [7] = "a‹}ƒwƒbƒY‘O",
    [8] = "ƒ‚ƒ‹ƒR‘O",
    [9] = "‰F“cì’¬˜H’n— ",
    [10] = "a’Jƒfƒp[ƒg",
    [11] = "ƒJƒhƒC‘O",
    [12] = "ƒgƒEƒƒŒƒR[ƒh‘O",
    [13] = "‹{‰ºŒö‰€",
    [14] = "‹{‰ºŒö‰€ƒK[ƒh‰º",
    [15] = "ƒLƒƒƒbƒgƒXƒgƒŠ[ƒg",
    [16] = "¼ŒûƒoƒXƒ^[ƒ~ƒiƒ‹",
    [17] = "a’J‰wƒK[ƒh‰º",
    [18] = "“¹Œºâ",
    [19] = "‚`|‚d‚`‚r‚s",
    [20] = "a‹}–{“X‘O",
};
// clang-format on

void Result_SetSpriteFrame(Sprite* sprite, s16 frame) {
    Sprite_ChangeAnimation(sprite, Data_GetPackEntryData(sprite->resourceData, 3), frame,
                           Data_GetPackEntryData(sprite->resourceData, 2));
}

void Result_LoadPinData(RawPinData* pinData, u16 index) {
    DatMgr_ReleaseData(
        DatMgr_LoadRawDataWithOffset(1, pinData, sizeof(RawPinData), &Result_BinIdentifiers[30], index * sizeof(RawPinData)));
}

void Result_LoadItemData(RawItemData* itemData, u16 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, itemData, sizeof(RawItemData), &Result_BinIdentifiers[31],
                                                    index * sizeof(RawItemData)));
}

void Result_LoadFoodData(RawFoodData* foodData, u16 index) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, foodData, sizeof(RawFoodData), &Result_BinIdentifiers[32],
                                                    index * sizeof(RawFoodData)));
}

u32 Result_GetBadgeTotalPP(ResultObject* result, u16 index) {
    ResultBadge* badge = &result->badges[index];

    return badge->pp[2] + (badge->pp[0] + badge->pp[1]);
}

u32 Result_GetBadgeLevelPP(ResultObject* result, u16 index, s32 arg2) {
    return func_02023480(result->badges[index].level, arg2);
}

u32 Result_GetBadgeNextLevelPP(ResultObject* result, u16 index, s32 arg2) {
    ResultBadge* badge = &result->badges[index];

    if (badge->level == badge->maxLevel) {
        return 0xFFFF;
    }
    return func_02023480((u8)(badge->level + 1), arg2);
}

u16 Result_GetLevelsMultiplier(u32 arg0) {
    if (arg0 < 1) {
        return 10;
    }
    return arg0 + 9;
}

u16 Result_CalcTimeRank(u32 num, u32 denom, s32 unused) {
    s32 ratio = FX_Divide(num << 12, denom << 12);

    if (ratio < 0x1000) {
        return 0;
    } else if (ratio < 0x1400) {
        return 1;
    } else if (ratio < 0x1800) {
        return 2;
    } else if (ratio < 0x1C00) {
        return 3;
    } else if (ratio < 0x2000) {
        return 4;
    } else {
        return 5;
    }
}

u16 Result_GetTimeRankMultiplier(u32 rank, u8 arg1) {
    if (arg1 == 0) {
        return 10;
    }
    switch (rank) {
        case 0:
            return 15;
        case 1:
            return 13;
        case 2:
            return 12;
        case 3:
            return 11;
        case 4:
            return 10;
        default:
            return 8;
    }
}

u16 Result_CalcRateBRank(u8 num, u8 denom) {
    s32 ratio = FX_Divide(num << 12, denom << 12);

    if (ratio < 0x1000) {
        return 0;
    } else if (ratio < 0x1400) {
        return 1;
    } else if (ratio < 0x1800) {
        return 2;
    } else if (ratio < 0x1C00) {
        return 3;
    } else if (ratio < 0x2000) {
        return 4;
    } else {
        return 5;
    }
}

u16 Result_GetRateBMultiplier(u32 rank, u8 arg1) {
    if (arg1 == 0) {
        return 10;
    }
    switch (rank) {
        case 0:
            return 15;
        case 1:
            return 13;
        case 2:
            return 12;
        case 3:
            return 11;
        case 4:
            return 10;
        default:
            return 8;
    }
}

u16 Result_GetSpecialBonusMultiplier(u16 arg0) {
    return arg0 + 10;
}

// Nonmatching: basePP loaded into r0 instead of r12
u32 Result_CalcBattlePP(ResultObject* result) {
    return (result->specialBonusMultiplier *
            (result->rateBMultiplier * (result->timeMultiplier * (result->battleCountMultiplier * result->basePP)))) /
           10000;
}

u16 Result_CalcOverallRank(u16 rankA, u16 rankB) {
    return 5 - (u16)(((u16)(5 - rankA) + (u16)(5 - rankB)) / 2);
}

u16 Result_CalcMabsPinCountRank(u32 count) {
    switch (count) {
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        case 4:
            return 3;
        case 5:
            return 4;
        case 0:
        default:
            return 5;
    }
}

u16 Result_GetMabsPinCountMultiplier(u32 count) {
    switch (count) {
        case 1:
            return 60;
        case 2:
            return 30;
        case 3:
            return 20;
        case 4:
            return 15;
        case 5:
            return 12;
        case 0:
        default:
            return 10;
    }
}

u16 Result_CalcMabsRateBRank(u32 count) {
    if (count > 4) {
        return 0;
    } else if (count == 4) {
        return 1;
    } else if (count == 3) {
        return 2;
    } else if (count == 2) {
        return 3;
    } else if (count == 1) {
        return 4;
    } else {
        return 5;
    }
}

u16 Result_GetMabsRateBMultiplier(u32 count) {
    if (count > 4) {
        return 20;
    } else if (count == 4) {
        return 17;
    } else if (count == 3) {
        return 15;
    } else if (count == 2) {
        return 12;
    } else if (count == 1) {
        return 10;
    } else {
        return 5;
    }
}

// USA weights mingle PP more heavily than JP when deciding which PP type drove a badge's growth.
#ifdef REGION_USA
    #define MINGLE_PP_WEIGHT 20
#else
    #define MINGLE_PP_WEIGHT 9
#endif

u16 Result_GetDominantPPType(u32 battlePP, u32 minglePP, u32 shutdownPP) {
    u32 mingle = minglePP * MINGLE_PP_WEIGHT;

    if (mingle >= shutdownPP * 9) {
        return (mingle >= battlePP) ? 1 : 0;
    }
    if (shutdownPP * 9 >= battlePP) {
        return 2;
    }
    return 0;
}

u8 Result_CheckBadgeEvolution(ResultObject* result, u16 index, s32 kind) {
    u32 pp0 = result->badges[index].pp[0];
    u32 pp1 = result->badges[index].pp[1];
    u32 pp2 = result->badges[index].pp[2];
    u16 dominant;

    if (kind == 0) {
        if (result->earnedPP > result->badges[index].ppToNextLevel) {
            pp0 = result->badges[index].startPP[0] + result->badges[index].ppToNextLevel;
        } else {
            pp0 = result->earnedPP + result->badges[index].startPP[0];
        }
    } else if (kind == 1) {
        if (result->earnedPP > result->badges[index].ppToNextLevel) {
            pp1 = result->badges[index].startPP[1] + result->badges[index].ppToNextLevel;
        } else {
            pp1 = result->earnedPP + result->badges[index].startPP[1];
        }
    } else {
        if (result->earnedPP > result->badges[index].ppToNextLevel) {
            pp2 = result->badges[index].startPP[2] + result->badges[index].ppToNextLevel;
        } else {
            pp2 = result->earnedPP + result->badges[index].startPP[2];
        }
    }

    dominant = Result_GetDominantPPType(pp0, pp1, pp2);
    {
        ResultBadge* badge = &result->badges[index];
        u8           i;
        u8           type  = badge->unk_31;
        u8           level = badge->level;

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
                    case 4:
                        if (type == 0 && dominant == 0) {
                            return i;
                        }
                        break;
                    case 5:
                        if (type == 0 && dominant == 1) {
                            return i;
                        }
                        break;
                    case 6:
                        if (type == 0 && dominant == 2) {
                            return i;
                        }
                        break;
                }
            }
        }
    }
    return 2;
}

u8 Result_GetBadgeSlotCount(void) {
    u8 count = func_02023010(680) + 2;

    if (count > 6) {
        count = 6;
    }
    return count;
}

u16 Result_CountDroppedPins(ResultObject* result) {
    u16 i;
    u16 count = 0;

    for (i = 0; i < 32; i++) {
        if (result->dropPinIDs[i] != 0xFFFF) {
            count++;
        }
    }
    return count;
}

u16 Result_SumUnmasteredDrops(ResultObject* result) {
    u16 i;
    u16 total = 0;

    for (i = 0; i < 32; i++) {
        if (result->dropPinIDs[i] != 0xFFFF && result->dropIsMastered[i] == 0) {
            total += result->dropQuantities[i];
        }
    }
    return total;
}

void Result_LoadDroppedPins(ResultObject* result) {
    RawPinData pinData;
    u16        overflow = 0;
    u16        capacity;
    u16        i;

    for (i = 0; i < 32; i++) {
        u16 itemId = gSaveData.battleDrops[i].itemId;

        if (itemId == 0xFFFF) {
            result->dropPinIDs[i]     = 0xFFFF;
            result->dropIsMastered[i] = 0;
            result->dropQuantities[i] = 0;
        } else {
            if (itemId >= 1000) {
                result->dropPinIDs[i]     = itemId - 1000;
                result->dropIsMastered[i] = 1;
            } else {
                result->dropPinIDs[i] = itemId;
                Result_LoadPinData(&pinData, result->dropPinIDs[i]);
                if (pinData.maxLevel == 1) {
                    result->dropIsMastered[i] = 1;
                } else {
                    result->dropIsMastered[i] = 0;
                }
            }
            result->dropQuantities[i] = gSaveData.battleDrops[i].count;
        }
    }

    result->dropCount = Result_CountDroppedPins(result);
    capacity          = Inventory_GetOpenPinStockpileCapacity();
    Result_SumUnmasteredDrops(result);
    for (i = 0; i < result->dropCount; i++) {
        if (result->dropIsMastered[i] == 0) {
            if (capacity != 0) {
                if (result->dropQuantities[i] <= capacity) {
                    capacity -= result->dropQuantities[i];
                } else {
                    overflow                  = result->dropQuantities[i] - capacity;
                    result->dropQuantities[i] = capacity;
                    capacity                  = 0;
                }
            } else {
                result->dropPinIDs[i]     = 0xF5;
                result->dropIsMastered[i] = 1;
            }
        }
    }

    if (overflow != 0 && result->dropCount < 31) {
        result->dropPinIDs[result->dropCount]     = 0xF5;
        result->dropIsMastered[result->dropCount] = 1;
        result->dropQuantities[result->dropCount] = overflow;
        result->dropCount++;
    }
}

#ifdef REGION_USA
u32 Result_GetCurrentDate(void) {
    RTCDate date;

    func_02041680(&date);
    return date.day + ((date.year * 10000) + (date.month * 100));
}
#endif

void Result_DigestFood(ResultObject* result) {
    RawFoodData foodData;
    u8          partner = gSaveData.playerStats.activePartner;
    u16         expired = 0;
#ifdef REGION_USA
    u32 date;
#endif

    result->foodItems[0] = Inventory_GetCategorizedIndex(gSaveData.playerStats.foodItem);
    if (partner == 0xFF) {
        result->foodItems[1] = 0xFFFF;
    } else {
        result->foodItems[1] = Inventory_GetCategorizedIndex(gSaveData.friendStats[partner].foodItem);
    }
    result->foodFinished[0] = 0;
    result->foodFinished[1] = 0;

    if (result->foodItems[0] == 0xFFFF) {
        result->foodStat[0]      = 0;
        result->foodStatBonus[0] = 0;
    } else {
        Result_LoadFoodData(&foodData, result->foodItems[0]);
        result->foodStat[0]      = foodData.stat;
        result->foodStatBonus[0] = foodData.statBonus;
    }
    if (result->foodItems[1] == 0xFFFF) {
        result->foodStat[1]      = 0;
        result->foodStatBonus[1] = 0;
    } else {
        Result_LoadFoodData(&foodData, result->foodItems[1]);
        result->foodStat[1]      = foodData.stat;
        result->foodStatBonus[1] = foodData.statBonus;
    }

#ifdef REGION_USA
    date = Result_GetCurrentDate();
    if (date != gSaveData.unk_242C) {
        gSaveData.playerStats.foodCapacityLeft = 24;
        for (u16 i = 0; i < 3; i++) {
            gSaveData.friendStats[i].foodCapacityLeft = 24;
        }
    }
    gSaveData.unk_242C = date;

    if (gSaveData.playerStats.foodBytesLeft != 0) {
        if (gSaveData.playerStats.foodBytesLeft >= result->battleCount) {
            gSaveData.playerStats.foodCapacityLeft -= result->battleCount;
            gSaveData.playerStats.foodBytesLeft = gSaveData.playerStats.foodBytesLeft - result->battleCount;
        } else {
            gSaveData.playerStats.foodCapacityLeft -= gSaveData.playerStats.foodBytesLeft;
            gSaveData.playerStats.foodBytesLeft = 0;
        }
        if (gSaveData.playerStats.foodBytesLeft == 0) {
            gSaveData.playerStats.foodItem = 0xFFFF;
            result->foodFinished[0]        = 1;
            expired++;
        }
        if (gSaveData.playerStats.foodCapacityLeft < 6) {
            gSaveData.playerStats.foodCapacityLeft = 6;
        }
    }

    if (partner != 0xFF && gSaveData.friendStats[partner].foodBytesLeft != 0) {
        if (gSaveData.friendStats[partner].foodBytesLeft >= result->battleCount) {
            gSaveData.friendStats[partner].foodCapacityLeft -= result->battleCount;
            gSaveData.friendStats[partner].foodBytesLeft = gSaveData.friendStats[partner].foodBytesLeft - result->battleCount;
        } else {
            gSaveData.friendStats[partner].foodCapacityLeft -= gSaveData.friendStats[partner].foodBytesLeft;
            gSaveData.friendStats[partner].foodBytesLeft = 0;
        }
        if (gSaveData.friendStats[partner].foodBytesLeft == 0) {
            gSaveData.friendStats[partner].foodItem = 0xFFFF;
            result->foodFinished[1]                 = 1;
            expired++;
        }
        if (gSaveData.friendStats[partner].foodCapacityLeft < 6) {
            gSaveData.friendStats[partner].foodCapacityLeft = 6;
        }
    }

    result->foodFinishedCount = expired;
    if (func_02023010(715)) {
        gSaveData.playerStats.foodCapacityLeft          = 24;
        gSaveData.friendStats[partner].foodCapacityLeft = 24;
    }
#else
    if (gSaveData.playerStats.foodBytesLeft != 0) {
        if (gSaveData.playerStats.foodBytesLeft >= result->battleCount) {
            gSaveData.playerStats.foodBytesLeft = gSaveData.playerStats.foodBytesLeft - result->battleCount;
            gSaveData.playerStats.foodCapacityLeft -= result->battleCount;
        } else {
            gSaveData.playerStats.foodCapacityLeft -= gSaveData.playerStats.foodBytesLeft;
            gSaveData.playerStats.foodBytesLeft = 0;
        }
        if (gSaveData.playerStats.foodBytesLeft == 0) {
            gSaveData.playerStats.foodItem = 0xFFFF;
            result->foodFinished[0]        = 1;
            expired++;
        }
        if (gSaveData.playerStats.foodCapacityLeft < 6) {
            gSaveData.playerStats.foodCapacityLeft = 6;
        }
    }

    if (partner != 0xFF && gSaveData.friendStats[partner].foodBytesLeft != 0) {
        if (gSaveData.friendStats[partner].foodBytesLeft >= result->battleCount) {
            gSaveData.friendStats[partner].foodBytesLeft = gSaveData.friendStats[partner].foodBytesLeft - result->battleCount;
            gSaveData.friendStats[partner].foodCapacityLeft -= result->battleCount;
        } else {
            gSaveData.friendStats[partner].foodCapacityLeft -= gSaveData.friendStats[partner].foodBytesLeft;
            gSaveData.friendStats[partner].foodBytesLeft = 0;
        }
        if (gSaveData.friendStats[partner].foodBytesLeft == 0) {
            gSaveData.friendStats[partner].foodItem = 0xFFFF;
            result->foodFinished[1]                 = 1;
            expired++;
        }
        if (gSaveData.friendStats[partner].foodCapacityLeft < 6) {
            gSaveData.friendStats[partner].foodCapacityLeft = 6;
        }
    }

    result->foodFinishedCount = expired;
#endif
}

void Result_LoadSpecialBonuses(ResultObject* result) {
    u16 i;
    u16 count = 0;
    u32 flags = gSaveData.battleBonusFlags;

    result->specialBonuses[0] = 0xFFFF;
    for (i = 0; i < 16; i++) {
        if (flags & (1 << i)) {
            result->specialBonuses[count] = i;
            count++;
        }
    }
    result->specialBonusCount = count;
}

void Result_EvolveBadge(ResultObject* result, u16 index, u16 pinID) {
    RawPinData pinData;

    Result_LoadPinData(&pinData, pinID);
    result->badges[index].pinID         = pinID;
    result->badges[index].brand         = pinData.brand;
    result->badges[index].ppCurve       = pinData.ppCurve;
    result->badges[index].pp[0]         = 0;
    result->badges[index].pp[1]         = 0;
    result->badges[index].pp[2]         = 0;
    result->badges[index].startPP[0]    = 0;
    result->badges[index].startPP[1]    = 0;
    result->badges[index].startPP[2]    = 0;
    result->badges[index].level         = 1;
    result->badges[index].maxLevel      = pinData.maxLevel;
    result->badges[index].levelPP       = Result_GetBadgeLevelPP(result, index, result->badges[index].ppCurve);
    result->badges[index].nextLevelPP   = Result_GetBadgeNextLevelPP(result, index, result->badges[index].ppCurve);
    result->badges[index].totalPP       = Result_GetBadgeTotalPP(result, index);
    result->badges[index].ppToNextLevel = result->badges[index].nextLevelPP - result->badges[index].totalPP;
    result->badges[index].slot          = index;
    result->badges[index].unk_30        = 1;
    result->badges[index].unk_31        = 1;
    for (u16 i = 0; i < 2; i++) {
        result->badges[index].evolveLevel[i]     = pinData.evolveLevel[i];
        result->badges[index].evolveCondition[i] = pinData.evolveCondition[i];
        result->badges[index].evolvePinID[i]     = pinData.evolvePinID[i];
    }
}

// Nonmatching: partner-thread loop registers rotated; one constant scheduled early
void Result_LoadEquipment(ResultObject* result) {
    RawPinData  pinData;
    RawItemData itemData;
    u8          partner;

    for (u16 i = 0; i < 6; i++) {
        u16 pinID = gSaveData.equippedPins[i].pinID;

        if (pinID == 0xFFFF) {
            result->badges[i] = Result_EmptyBadge;
        } else {
            ResultBadge* badge;

            Result_LoadPinData(&pinData, pinID);
            result->badges[i].pinID         = pinID;
            badge                           = &result->badges[i];
            badge->brand                    = pinData.brand;
            badge->ppCurve                  = pinData.ppCurve;
            badge->pp[0]                    = gSaveData.equippedPins[i].battlePP;
            badge->pp[1]                    = gSaveData.equippedPins[i].minglePP;
            badge->pp[2]                    = gSaveData.equippedPins[i].shutdownPP;
            badge->startPP[0]               = badge->pp[0];
            badge->startPP[1]               = badge->pp[1];
            badge->startPP[2]               = badge->pp[2];
            badge->level                    = gSaveData.equippedPins[i].flags.bits.level;
            badge->maxLevel                 = pinData.maxLevel;
            result->badges[i].levelPP       = Result_GetBadgeLevelPP(result, i, badge->ppCurve);
            result->badges[i].nextLevelPP   = Result_GetBadgeNextLevelPP(result, i, result->badges[i].ppCurve);
            result->badges[i].totalPP       = Result_GetBadgeTotalPP(result, i);
            result->badges[i].ppToNextLevel = result->badges[i].nextLevelPP - result->badges[i].totalPP;
            result->badges[i].slot          = i;
            result->badges[i].unk_2F        = gSaveData.equippedPins[i].unk_08;
            result->badges[i].unk_30        = 1;
            result->badges[i].unk_31        = gSaveData.equippedPins[i].flags.bits.unk_07;
            for (u16 j = 0; j < 2; j++) {
                result->badges[i].evolveLevel[j]     = pinData.evolveLevel[j];
                result->badges[i].evolveCondition[j] = pinData.evolveCondition[j];
                result->badges[i].evolvePinID[j]     = pinData.evolvePinID[j];
                result->badges[i].evolveChoice       = 2;
                result->badges[i].flags              = 0;
            }
        }
    }

    for (u16 i = 0; i < 4; i++) {
        u16 itemId = gSaveData.playerStats.equippedThreads[i];

        if (itemId == 0xFFFF) {
            result->threads[0][i].itemId = 0xFFFF;
            result->threads[0][i].brand  = 14;
        } else {
            Result_LoadItemData(&itemData, Inventory_GetCategorizedIndex(itemId));
            result->threads[0][i].itemId = itemId;
            result->threads[0][i].brand  = itemData.brand;
        }
    }

    partner = gSaveData.playerStats.activePartner;
    for (u16 i = 0; i < 4; i++) {
        u16 itemId = gSaveData.friendStats[partner].equippedThreads[i];

        if (partner == 0xFF || itemId == 0xFFFF) {
            result->threads[1][i].itemId = 0xFFFF;
            result->threads[1][i].brand  = 14;
        } else {
            Result_LoadItemData(&itemData, Inventory_GetCategorizedIndex(itemId));
            result->threads[1][i].itemId = itemId;
            result->threads[1][i].brand  = itemData.brand;
        }
    }

    result->flashStep  = 0;
    result->flashTimer = 20;
    result->unk_CFE    = 2;
    result->flashLevel = 0;
    result->badgeCount = Result_GetBadgeSlotCount();
}

void Result_InitBattleMode(ResultObject* result) {
    Result_LoadSpecialBonuses(result);
    result->basePP                 = gSaveData.battleBasePP;
    result->battleCount            = gSaveData.battleCount;
    result->timeRank               = Result_CalcTimeRank(gSaveData.battleClearTime, gSaveData.battleParTime, 0);
    result->rateBRank              = Result_CalcRateBRank(gSaveData.unk_318F, gSaveData.unk_318E);
    result->specialBonusLevel      = result->specialBonusCount;
    result->clearTime              = gSaveData.battleClearTime;
    result->maxHits                = gSaveData.battleMaxHits;
    result->battleCountMultiplier  = Result_GetLevelsMultiplier(result->battleCount);
    result->timeMultiplier         = Result_GetTimeRankMultiplier(result->timeRank, gSaveData.unk_3141);
    result->rateBMultiplier        = Result_GetRateBMultiplier(result->rateBRank, gSaveData.unk_3141);
    result->specialBonusMultiplier = Result_GetSpecialBonusMultiplier(result->specialBonusLevel);
    result->displayedPP            = 0;
    result->earnedPP               = Result_CalcBattlePP(result);
    result->overallRank            = Result_CalcOverallRank(result->timeRank, result->rateBRank);
    result->currentArea            = gSaveData.unk_1AB1;
    result->trendRotation          = gSaveData.unk_1AB8;
    {
        u8 brands[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

        for (u16 i = 0; i < 21; i++) {
            for (u16 j = 0; j < 13; j++) {
                result->trendScores[i][j]  = gSaveData.brandTrends[i].scores[j];
                result->trendGains[i][j]   = 0;
                result->trendRanking[i][j] = brands[j];
            }
        }
    }
    Result_DigestFood(result);
    Result_LoadDroppedPins(result);
    result->battlePartner = gSaveData.battlePartner;
}

#ifdef REGION_USA
void Result_ImportSurePins(ResultObject* result) {
    for (u16 i = 0; i < 6; i++) {
        EquippedPin* src = data_ov038_0209a204[i];

        gSaveData.equippedPins[i].pinID             = src->pinID;
        gSaveData.equippedPins[i].battlePP          = src->battlePP;
        gSaveData.equippedPins[i].minglePP          = src->minglePP;
        gSaveData.equippedPins[i].shutdownPP        = src->shutdownPP;
        gSaveData.equippedPins[i].unk_08            = src->unk_08;
        gSaveData.equippedPins[i].flags.bits.level  = src->flags.bits.level;
        gSaveData.equippedPins[i].flags.bits.unk_07 = src->flags.bits.unk_07;
    }
    for (u16 i = 0; i < 6; i++) {
        if (data_ov038_0209a204[i] != NULL) {
            Mem_Free(&gMainHeap, data_ov038_0209a204[i]);
        }
    }
}
#endif

// Mingle PP per esper / civvy / alien met.
#ifdef REGION_USA
    #define SURE_PP_ESPER 50
    #define SURE_PP_CIVVY 20
    #define SURE_PP_ALIEN 100
#else
    #define SURE_PP_ESPER 20
    #define SURE_PP_CIVVY 3
    #define SURE_PP_ALIEN 20
#endif

void Result_InitSureMode(ResultObject* result) {
    result->espersMet   = gSaveData.espersMet;
    result->civviesMet  = gSaveData.civviesMet;
    result->aliensMet   = gSaveData.aliensMet;
    result->civvyPP     = result->civviesMet * SURE_PP_CIVVY;
    result->esperPP     = result->espersMet * SURE_PP_ESPER;
    result->alienPP     = result->aliensMet * SURE_PP_ALIEN;
    result->displayedPP = 0;
    result->earnedPP    = result->alienPP + (result->esperPP + result->civvyPP);
}

void Result_InitSleepMode(ResultObject* result) {
    RTCDate date;
    RTCTime time;
    s64     now;
    s64     then;

    func_020417e0(&date, &time);
    now         = func_02041f14(&date, &time);
    date.year   = gSaveData.lastSaveTime.year;
    date.month  = gSaveData.lastSaveTime.month;
    date.day    = gSaveData.lastSaveTime.day;
    date.unk    = 0;
    time.hour   = gSaveData.lastSaveTime.hour;
    time.minute = gSaveData.lastSaveTime.minute;
    time.second = gSaveData.lastSaveTime.second;
    then        = func_02041f14(&date, &time);

    if (then < now) {
        s64 elapsed = now - then;
        s64 rem;

        if (elapsed > 0x52A91E4) {
            elapsed = 0x52A91E4;
        }
        result->elapsedDays    = elapsed / 86400;
        rem                    = elapsed % 86400;
        result->elapsedHours   = rem / 3600;
        rem                    = rem % 3600;
        result->elapsedMinutes = rem / 60;
        result->elapsedSeconds = rem % 60;
        if (result->elapsedDays > 999) {
            result->elapsedDays    = 999;
            result->elapsedHours   = 23;
            result->elapsedMinutes = 59;
            result->elapsedSeconds = 0;
        }

        for (u16 i = 0; i < 7; i++) {
            result->dailyPP[i] = 0;
        }

        if (elapsed <= 86400) {
            result->dailyPP[0] = elapsed / 600;
        } else if (elapsed <= 86400 * 2) {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = (elapsed - 86400) / 1200;
        } else if (elapsed <= 86400 * 3) {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = 72;
            result->dailyPP[2] = (elapsed - 86400 * 2) / 2400;
        } else if (elapsed <= 86400 * 4) {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = 72;
            result->dailyPP[2] = 36;
            result->dailyPP[3] = (elapsed - 86400 * 3) / 4800;
        } else if (elapsed <= 86400 * 5) {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = 72;
            result->dailyPP[2] = 36;
            result->dailyPP[3] = 18;
            result->dailyPP[4] = (elapsed - 86400 * 4) / 9600;
        } else if (elapsed <= 86400 * 6) {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = 72;
            result->dailyPP[2] = 36;
            result->dailyPP[3] = 18;
            result->dailyPP[4] = 9;
            result->dailyPP[5] = (elapsed - 86400 * 5) / 9600;
        } else if (elapsed <= 86400 * 7) {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = 72;
            result->dailyPP[2] = 36;
            result->dailyPP[3] = 18;
            result->dailyPP[4] = 9;
            result->dailyPP[5] = 9;
            result->dailyPP[6] = (elapsed - 86400 * 6) / 9600;
        } else {
            result->dailyPP[0] = 144;
            result->dailyPP[1] = 72;
            result->dailyPP[2] = 36;
            result->dailyPP[3] = 18;
            result->dailyPP[4] = 9;
            result->dailyPP[5] = 9;
            result->dailyPP[6] = 9;
        }
    } else {
        result->elapsedDays    = 0;
        result->elapsedHours   = 0;
        result->elapsedMinutes = 0;
        result->elapsedSeconds = 0;
        for (u16 i = 0; i < 7; i++) {
            result->dailyPP[i] = 0;
        }
    }

    result->displayedPP = 0;
    result->earnedPP    = 0;
    for (u16 i = 0; i < 7; i++) {
        result->earnedPP += result->dailyPP[i];
    }
}

void Result_InitMabsMode(ResultObject* result) {
    u16 count = 0;
    u8  unk_1D84;

    for (u16 i = 0; i < 6; i++) {
        if (gSaveData.equippedPins[i].pinID != 0xFFFF) {
            count++;
        }
    }
    result->basePP                 = gSaveData.mabsBasePP;
    unk_1D84                       = gSaveData.unk_1D84;
    result->mabsPinCountRank       = Result_CalcMabsPinCountRank(count);
    result->mabsPinCountMultiplier = Result_GetMabsPinCountMultiplier(count);
    result->mabsRateBRank          = Result_CalcMabsRateBRank(unk_1D84);
    result->mabsRateBMultiplier    = Result_GetMabsRateBMultiplier(unk_1D84);
    result->displayedPP            = 0;
    result->earnedPP               = (result->mabsRateBMultiplier * (result->basePP * result->mabsPinCountMultiplier)) / 100;
}

BOOL Result_IncrementMasteredPin(u16 pinID) {
    for (u16 i = 0; i < 304; i++) {
        if (pinID == gSaveData.masteredPins[i].pinID) {
            gSaveData.masteredPins[i].count++;
            if (gSaveData.masteredPins[i].count > 99) {
                gSaveData.masteredPins[i].count = 99;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Result_AddMasteredPin(u16 pinID) {
    for (u16 i = 0; i < 304; i++) {
        if (gSaveData.masteredPins[i].pinID == 0xFFFF) {
            gSaveData.masteredPins[i].pinID = pinID;
            gSaveData.masteredPins[i].count = 1;
            gSaveData.masteredPins[i].flags.raw |= 0x80;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL Result_StoreMasteredPin(u16 pinID) {
    if (Result_IncrementMasteredPin(pinID) == TRUE) {
        return TRUE;
    }
    if (Result_AddMasteredPin(pinID) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL Result_AddStockpilePin(u16 pinID) {
    for (u16 i = 0; i < 256; i++) {
        if (gSaveData.stockpilePins[i].pinID == 0xFFFF) {
            gSaveData.stockpilePins[i].pinID             = pinID;
            gSaveData.stockpilePins[i].battlePP          = 0;
            gSaveData.stockpilePins[i].minglePP          = 0;
            gSaveData.stockpilePins[i].shutdownPP        = 0;
            gSaveData.stockpilePins[i].flags.bits.level  = 1;
            gSaveData.stockpilePins[i].flags.bits.unk_07 = 1;
            return TRUE;
        }
    }
    return FALSE;
}

void Result_AddEquipmentBrandTrend(ResultObject* result, u16 row) {
    if (row >= 21) {
        return;
    }
    for (u16 i = 0; i < 6; i++) {
        if (result->badges[i].pinID != 0xFFFF && result->badges[i].brand <= 12) {
            result->trendGains[row][result->badges[i].brand] += 200;
        }
    }
    for (u16 i = 0; i < 2; i++) {
        for (u16 j = 0; j < 4; j++) {
            if (result->threads[i][j].itemId != 0xFFFF && result->threads[i][j].brand <= 12) {
                result->trendGains[row][result->threads[i][j].brand] += 100;
            }
        }
    }
}

u8 Result_GetAreaFavoriteBrand(ResultObject* result, u16 row) {
    u8 brands[21] = {14, 1, 14, 14, 2, 0, 14, 14, 7, 11, 6, 10, 5, 14, 14, 8, 14, 14, 14, 3, 4};

    return brands[row];
}

u8 Result_GetEvenHourBrand(ResultObject* result) {
    if (result->time.hour & 1) {
        return 14;
    }
    return (result->trendRotation + (result->time.hour >> 1)) % 13;
}

u8 Result_GetOddHourBrand(ResultObject* result) {
    if ((result->time.hour & 1) != 1) {
        return 14;
    }
    return (result->trendRotation + (result->time.hour >> 1) + 7) % 13;
}

void Result_AddRandomBrandTrend(ResultObject* result, u16 row, u8 brandA, u8 brandB) {
    u8 favorite = Result_GetAreaFavoriteBrand(result, row);

    for (u16 i = 0; i < 13; i++) {
        result->trendGains[row][i] += (s32)RNG_Next(0xFFFF) % 1001;
        if (i == favorite) {
            result->trendGains[row][i] += 300;
        }
    }
}

void Result_ApplyBrandTrendGain(ResultObject* result, u16 row) {
    for (u16 i = 0; i < 13; i++) {
        result->trendGains[row][i] = (result->trendGains[row][i] * 0x800) >> 12;
        result->trendScores[row][i] += result->trendGains[row][i];
    }
}

void Result_NormalizeBrandTrends(ResultObject* result, u16 row) {
    u16 i;
    u64 total = 0;

    for (i = 0; i < 13; i++) {
        total += result->trendScores[row][i];
    }
    for (i = 0; i < 13; i++) {
        result->trendScores[row][i] = (u64)result->trendScores[row][i] * 10000 / total;
    }
}

void Result_SortBrandRanking(ResultObject* result, u16 row) {
    u32 values[13];
    u16 i;
    u16 j;

    for (i = 0; i < 13; i++) {
        values[i] = result->trendScores[row][i];
    }
    for (i = 0; i < 12; i++) {
        for (j = 12; j > i; j--) {
            if (values[j - 1] < values[j]) {
                u32 tmp;
                u8  brand;

                tmp                              = values[j];
                values[j]                        = values[j - 1];
                values[j - 1]                    = tmp;
                brand                            = result->trendRanking[row][j];
                result->trendRanking[row][j]     = result->trendRanking[row][j - 1];
                result->trendRanking[row][j - 1] = brand;
            }
        }
    }
}

void Result_UpdateBrandTrends(ResultObject* result) {
    u16 i;
    u8  brandA;
    u8  brandB;

    func_020415a4();
    func_020417e0(&result->date, &result->time);
    brandA = Result_GetEvenHourBrand(result);
    brandB = Result_GetOddHourBrand(result);
    for (i = 0; i < 21; i++) {
        if (i == result->currentArea) {
            Result_AddEquipmentBrandTrend(result, i);
        }
        Result_AddRandomBrandTrend(result, i, brandA, brandB);
        Result_ApplyBrandTrendGain(result, i);
        Result_NormalizeBrandTrends(result, i);
        Result_SortBrandRanking(result, i);
    }
}

void Result_SaveEquippedPins(ResultObject* result) {
    u16 i;
    u8  layout;

    for (i = 0; i < 6; i++) {
        gSaveData.equippedPins[i].pinID             = result->badges[i].pinID;
        gSaveData.equippedPins[i].battlePP          = result->badges[i].pp[0];
        gSaveData.equippedPins[i].minglePP          = result->badges[i].pp[1];
        gSaveData.equippedPins[i].shutdownPP        = result->badges[i].pp[2];
        gSaveData.equippedPins[i].unk_08            = result->badges[i].unk_2F;
        gSaveData.equippedPins[i].flags.bits.level  = result->badges[i].level;
        gSaveData.equippedPins[i].flags.bits.unk_07 = result->badges[i].unk_31;
    }

#ifdef REGION_USA
    layout = gSaveData.playerStats.unk_19_4;
    for (i = 0; i < 6; i++) {
        gSaveData.pinLayouts[layout][i].pinID             = gSaveData.equippedPins[i].pinID;
        gSaveData.pinLayouts[layout][i].flags.bits.level  = gSaveData.equippedPins[i].flags.bits.level;
        gSaveData.pinLayouts[layout][i].battlePP          = gSaveData.equippedPins[i].battlePP;
        gSaveData.pinLayouts[layout][i].minglePP          = gSaveData.equippedPins[i].minglePP;
        gSaveData.pinLayouts[layout][i].shutdownPP        = gSaveData.equippedPins[i].shutdownPP;
        gSaveData.pinLayouts[layout][i].unk_08            = gSaveData.equippedPins[i].unk_08;
        gSaveData.pinLayouts[layout][i].flags.bits.unk_07 = gSaveData.equippedPins[i].flags.bits.unk_07;
    }
#endif
}

// Partner sync lost per unit of gSaveData.unk_318F.
#ifdef REGION_USA
    #define SYNC_PENALTY 5
#else
    #define SYNC_PENALTY 10
#endif

// Nonmatching: final badge loop counter register (r1 vs r3)
void Result_CommitBattle(ResultObject* result) {
    u8 partner = gSaveData.playerStats.activePartner;

    Stats_AddExperience(result->battleCount);
    if (func_02023010(684)) {
        Result_UpdateBrandTrends(result);
        for (u16 i = 0; i < 21; i++) {
            for (u16 j = 0; j < 13; j++) {
                gSaveData.brandTrends[i].scores[j]  = result->trendScores[i][j];
                gSaveData.brandTrends[i].ranking[j] = result->trendRanking[i][j];
            }
        }
    }

    for (u16 i = 0; i < result->dropCount; i++) {
        for (u16 j = 0; j < result->dropQuantities[i]; j++) {
            if (result->dropIsMastered[i] == 0) {
                Result_AddStockpilePin(result->dropPinIDs[i]);
            } else {
                Result_StoreMasteredPin(result->dropPinIDs[i]);
            }
        }
    }

    if (result->foodFinished[0] == 1) {
        switch (result->foodStat[0]) {
            case 0:
                gSaveData.playerStats.attack += result->foodStatBonus[0];
                if (gSaveData.playerStats.attack > 100) {
                    gSaveData.playerStats.attack = 100;
                }
                break;
            case 1:
                gSaveData.playerStats.defense += result->foodStatBonus[0];
                if (gSaveData.playerStats.defense > 100) {
                    gSaveData.playerStats.defense = 100;
                }
                break;
            case 2:
                gSaveData.playerStats.baseHealth += result->foodStatBonus[0];
                if (gSaveData.playerStats.baseHealth > 4849) {
                    gSaveData.playerStats.baseHealth = 4849;
                }
                break;
            case 3:
                gSaveData.playerStats.bravery += result->foodStatBonus[0];
                if (gSaveData.playerStats.bravery > 999) {
                    gSaveData.playerStats.bravery = 999;
                }
                break;
            case 4: {
                u16 levelBonus = gSaveData.playerStats.level - gSaveData.playerStats.unk_04;
                u16 dropRate   = result->foodStatBonus[0] + (u16)(gSaveData.playerStats.dropRate - levelBonus);

                if (dropRate > 900) {
                    dropRate = 900;
                }
                gSaveData.playerStats.dropRate = dropRate + levelBonus;
            } break;
            case 5:
                if (partner != 0xFF) {
                    gSaveData.friendStats[partner].sync += result->foodStatBonus[0] * 10;
                    if (gSaveData.friendStats[partner].sync > 1000) {
                        gSaveData.friendStats[partner].sync = 1000;
                    }
                }
                break;
        }
    }

    if (partner != 0xFF && result->foodFinished[1] == 1) {
        switch (result->foodStat[1]) {
            case 0:
                gSaveData.friendStats[partner].attack += result->foodStatBonus[1];
                if (gSaveData.friendStats[partner].attack > 100) {
                    gSaveData.friendStats[partner].attack = 100;
                }
                break;
            case 1:
                gSaveData.friendStats[partner].defense += result->foodStatBonus[1];
                if (gSaveData.friendStats[partner].defense > 100) {
                    gSaveData.friendStats[partner].defense = 100;
                }
                break;
            case 2:
                gSaveData.playerStats.baseHealth += result->foodStatBonus[1];
                if (gSaveData.playerStats.baseHealth > 4849) {
                    gSaveData.playerStats.baseHealth = 4849;
                }
                break;
            case 3:
                gSaveData.friendStats[partner].bravery += result->foodStatBonus[1];
                if (gSaveData.friendStats[partner].bravery > 999) {
                    gSaveData.friendStats[partner].bravery = 999;
                }
                break;
            case 4: {
                u16 levelBonus = gSaveData.playerStats.level - gSaveData.playerStats.unk_04;
                u16 dropRate   = result->foodStatBonus[1] + (u16)(gSaveData.playerStats.dropRate - levelBonus);

                if (dropRate > 900) {
                    dropRate = 900;
                }
                gSaveData.playerStats.dropRate = dropRate + levelBonus;
            } break;
            case 5:
                gSaveData.friendStats[partner].sync += result->foodStatBonus[1] * 10;
                if (gSaveData.friendStats[partner].sync > 1000) {
                    gSaveData.friendStats[partner].sync = 1000;
                }
                break;
        }
    }

    if (gSaveData.playerStats.dropRate > 999) {
        gSaveData.playerStats.dropRate = 999;
    }
    if (gSaveData.playerStats.attack > 100) {
        gSaveData.playerStats.attack = 100;
    }
    if (gSaveData.playerStats.defense > 100) {
        gSaveData.playerStats.defense = 100;
    }
    if (partner != 0xFF) {
        if (gSaveData.friendStats[partner].attack > 100) {
            gSaveData.friendStats[partner].attack = 100;
        }
        if (gSaveData.friendStats[partner].defense > 100) {
            gSaveData.friendStats[partner].defense = 100;
        }
    }
    if (partner != 0xFF) {
        if (gSaveData.friendStats[partner].sync > (u16)(gSaveData.unk_318F * SYNC_PENALTY)) {
            gSaveData.friendStats[partner].sync -= (u16)(gSaveData.unk_318F * SYNC_PENALTY);
        } else {
            gSaveData.friendStats[partner].sync = 0;
        }
    }

    for (u16 i = 0; i < 6; i++) {
        if (result->badges[i].pinID == 227 && gSaveData.unk_312C == gSaveData.unk_3138) {
            if (gSaveData.unk_1AC0 < 0xFFFF - result->battleCount) {
                gSaveData.unk_1AC0 += result->battleCount;
            }
            break;
        }
    }
}

void Result_CommitSure(ResultObject* result) {
#ifdef REGION_JP
    Stats_AddExperience(result->aliensMet + (result->espersMet + result->civviesMet));
#endif
}

void Result_CommitSleep(ResultObject* result) {}

void Result_CommitMabs(ResultObject* result) {
    gSaveData.mabsBasePP = 0;
    gSaveData.unk_1D84   = 0;
    Stats_AddExperience(3);
}

void Result_LoadBgResource(ResultBgResource* res, s32 engine, s32 layer, u8 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &Result_BinIdentifiers[binIndex]);
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

void Result_LoadBgResourceIndexed(ResultBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                  s32 screenIndex, s32 palIndex) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &Result_BinIdentifiers[binIndex]);
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

void Result_ReleaseBgResource(ResultBgResource* res, s32 engine) {
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

void Result_ClearBgResource(ResultBgResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void Result_LoadBackgrounds(ResultObject* result) {
    s32               i;
    ResultBgResource* mainRes = &result->bgResources[0];
    ResultBgResource* subRes  = &result->bgResources[4];
    u8                bin;

    for (i = 0; i < 4; i++) {
        Result_ClearBgResource(mainRes);
        Result_ClearBgResource(subRes);
        mainRes += 1;
        subRes += 1;
    }

    if (gSaveData.unk_1AB4 & 8) {
        Result_LoadBgResource(&result->bgResources[5], 0, 1, 34, 15, 1);
        Result_LoadBgResourceIndexed(&result->bgResources[6], 0, 2, 0, 10, 4, 2, 0);
        Result_LoadBgResource(&result->bgResources[7], 0, 3, 9, 0, 10);
        Result_LoadBgResource(&result->bgResources[3], 1, 3, 22, 0, 10);
    } else if (gSaveData.unk_1AB4 & 0x10) {
        Result_LoadBgResource(&result->bgResources[5], 0, 1, 34, 15, 1);
        Result_LoadBgResourceIndexed(&result->bgResources[6], 0, 2, 0, 10, 4, 3, 0);
        Result_LoadBgResource(&result->bgResources[7], 0, 3, 11, 0, 10);
        Result_LoadBgResource(&result->bgResources[3], 1, 3, 24, 0, 10);
    } else if (gSaveData.unk_1AB4 & 0x60) {
        Result_LoadBgResource(&result->bgResources[5], 0, 1, 34, 15, 1);
        Result_LoadBgResourceIndexed(&result->bgResources[6], 0, 2, 0, 10, 4, 4, 0);
        Result_LoadBgResource(&result->bgResources[7], 0, 3, 10, 0, 10);
        Result_LoadBgResource(&result->bgResources[3], 1, 3, 23, 0, 10);
    } else {
        if (result->overallRank <= 3) {
            if (result->battlePartner == 0xFF) {
                bin = 0;
            } else {
                bin = (result->battlePartner * 2) + 2;
            }
        } else if (result->battlePartner == 0xFF) {
            bin = 1;
        } else {
            bin = (result->battlePartner * 2) + 3;
        }
        Result_LoadBgResource(&result->bgResources[5], 0, 1, 34, 15, 1);
        Result_LoadBgResource(&result->bgResources[6], 0, 2, 0, 10, 4);
        Result_LoadBgResource(&result->bgResources[7], 0, 3, bin + 1, 0, 10);
        Result_LoadBgResource(&result->bgResources[3], 1, 3, bin + 14, 0, 10);
    }
}

void Result_UpdateBackgrounds(ResultObject* result) {}

void Result_ReleaseBackgrounds(ResultObject* result) {
    Result_ReleaseBgResource(&result->bgResources[5], 0);
    Result_ReleaseBgResource(&result->bgResources[6], 0);
    Result_ReleaseBgResource(&result->bgResources[7], 0);
    Result_ReleaseBgResource(&result->bgResources[3], 1);
}
