#include "Engine/Core/Memory.h"
#include "Engine/File/DatMgr.h"
#include "Save.h"
#include "common_data.h"
#include <types.h>

#include <NitroSDK/mi/cpumem.h>
#include <NitroSDK/os.h>

extern s32 func_02053098(f64, f64);
extern s32 func_02054a9c(f64);

struct SystemState data_02071cf0 = {0};

const s32           data_0205c120;
const BinIdentifier data_0205c128 = {30, "Apl_Tak/ItemData.bin"};
const BinIdentifier data_0205c130 = {30, "Apl_Tak/TreasureData.bin"};
const BinIdentifier data_0205c138 = {3, "Apl_Tak/ItemData.bin"};
const BinIdentifier data_0205c140 = {30, "Apl_Tak/ItemData.bin"};
const BinIdentifier data_0205c148;
const BinIdentifier data_0205c150 = {43, "../Data/BadgeData.bin"};
const BinIdentifier data_0205c158 = {30, "../Data/BadgeData.bin"};
const BinIdentifier data_0205c160;
const BinIdentifier data_0205c168 = {30, "../Data/BadgeData.bin"};
const BinIdentifier data_0205c170 = {3, "Apl_Tak/ItemData.bin"};
const BinIdentifier data_0205c178 = {30, "Apl_Tak/FoodData.bin"};
const BinIdentifier data_0205c180 = {3, "Apl_Tak/ItemData.bin"};
const BinIdentifier data_0205c188 = {30, "../Data/BadgeData.bin"};
const s32           data_0205c1e4;

const u16 SecretReportStarRequirements[22] = {
    1, 3, 4, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4,
};

const s32 data_0205c270[13];
const s32 data_0205c2dc;

static void Stats_ResetPlayerStats(PlayerStats* stats) {
    stats->unk_00         = 0;
    stats->baseHealth     = 0;
    stats->unk_04         = 1;
    stats->level          = stats->unk_04;
    stats->expToNextLevel = stats->level * 100;
    stats->money          = 0;
    stats->attack         = 10;
    stats->defense        = 0;
    stats->dropRate       = 1;
    stats->bravery        = 15;
    stats->activeFriend   = 255;

    stats->unk_19_0 = 1;
    stats->unk_19_2 = 2;
    stats->unk_19_4 = 0;
    stats->unk_19_6 = 0;
    stats->unk_19_7 = 0;

    stats->unk_1A_0 = 0;
    stats->unk_1A_1 = 2;

    for (s32 i = 0; i < 4; i++) {
        stats->equippedThreads[i] = 0xFFFF;
        stats->unk_24[i]          = 0;
    }

    stats->unk_28 = 0xffff;
    stats->unk_2A = 0;
    stats->unk_2B = 24;
}

static void Stats_ResetFriendStats(FriendStats* stats) {
    for (s32 friendIdx = 0; friendIdx < 3; friendIdx++) {
        stats->attack  = 10;
        stats->defense = 0;
        stats->sync    = 500;

        if (friendIdx == 0) {
            stats->bravery = 110; // Shiki
        } else if (friendIdx == 1) {
            stats->bravery = 40;  // Joshua
        } else {
            stats->bravery = 5;   // Beat
        }

        for (s32 j = 0; j < 4; j++) {
            stats->equippedThreads[j] = 0xFFFF;
            stats->unk_10[j]          = 0;
        }

        stats->unk_14 = 0xFFFF;
        stats->unk_16 = 0;
        stats->unk_17 = 24;
        stats++;
    }
}

static void Inventory_ResetEquippedPins(EquippedPin* equippedPins) {
    for (s32 i = 0; i < 6; i++) {
        equippedPins[i].pinID             = 0xFFFF;
        equippedPins[i].flags.bits.level  = 1;
        equippedPins[i].battlePP          = 0;
        equippedPins[i].minglePP          = 0;
        equippedPins[i].shutdownPP        = 0;
        equippedPins[i].unk_08            = 0;
        equippedPins[i].flags.bits.unk_07 = 1;
    }
}

static void Inventory_ResetStockpilePins(StockpilePin* stockpilePins) {
    for (s32 i = 0; i < 256; i++) {
        stockpilePins[i].pinID             = 0xFFFF;
        stockpilePins[i].flags.bits.level  = 1;
        stockpilePins[i].battlePP          = 0;
        stockpilePins[i].minglePP          = 0;
        stockpilePins[i].shutdownPP        = 0;
        stockpilePins[i].flags.bits.unk_07 = 1;
    }
}

static void Inventory_ResetMasteredPins(MasteredPin* masteredPins) {
    s32          i;
    MasteredPin* pMVar1 = masteredPins;

    for (i = 0; i < 304; i++) {
        masteredPins[i].pinID     = 0xffff;
        pMVar1->flags.bits.level  = 100;
        masteredPins[i].count     = 0;
        pMVar1->flags.bits.unk_07 = 1;
        pMVar1++;
    }
}

// Nonmatching: Flag manipulation is incorrect
static void Inventory_ResetStoredItems(InventoryItem* items) {
    InventoryItem* item = items;
    s32            i;
    for (i = 0; i < 472; i++) {
        items[i].itemID = 0xFFFF;
        item->flags     = ((u8)(item->flags & ~0xF) | 0x10) & ~0x20;
        item++;
    }
}

static void func_02022284(void*) {
    // Not yet implemented
}

static void func_02022424(s32* arg0) {
    for (s32 i = 0; i < 35; i++) {
        arg0[i] = 0;
    }
}

static void Stats_ResetExperience(Experience* experience) {
    experience->unk_0   = (experience->unk_0 & ~0x7F);
    experience->unk_0   = (experience->unk_0 & 0xFFFF007F);
    experience->unk_2   = (experience->unk_2 & (s16)(0xFFFF007F >> 7));
    experience->unk_2   = (experience->unk_2 & ~0xE00);
    experience->current = 0;
    experience->unk_8   = 0;
}

static void func_02022488(void*) {
    // Not yet implemented
}

static void func_020224f4(void*) {
    // Not yet implemented
}

static void func_02022534(void*) {
    // Not yet implemented
}

static void func_0202254c(void*) {
    // Not yet implemented
}

// Nonmatching: Incorrect registers used
static void func_0202255c(u8* arg0) {
    for (u16 i = 0; i < 280; i++) {
        arg0[i] = (u8)(arg0[i] & ~1) & ~2;
    }
}

static void func_0202258c(void*) {
    // Not yet implemented
}

static void func_020225c4(void*) {
    // Not yet implemented
}

static void func_020225ec(void*) {
    // Not yet implemented
}

static void Progress_ResetSecretReports(SecretReport* report) {
    for (s32 i = 0; i < 22; i++) {
        for (s32 j = 0; j < 4; j++) {
            report[i].completedStars[j] = 0;
        }
    }
}

static void func_02022640(void*) {
    // Not yet implemented
}

// Nonmatching: Major differences in data access and instruction order
void Savefile_ResetAllGameplay(MainData* arg0) {
    Stats_ResetPlayerStats(&arg0->playerStats);
    Stats_ResetFriendStats(arg0->friendStats);
    Inventory_ResetEquippedPins(arg0->equippedPins);
    Inventory_ResetStockpilePins(arg0->stockpilePins);
    Inventory_ResetMasteredPins(arg0->masteredPins);
    Inventory_ResetStoredItems(arg0->inventoryItems);
    func_02022284(&arg0->unk_16D0);
    func_02022424(arg0->unk_1A18);
    Stats_ResetExperience(&arg0->experience);
    arg0->unk_1AB0 = 0;
    arg0->unk_1AB1 = 0;
    arg0->unk_1AB2 = 1;
    arg0->unk_1AB3 = 0;
    arg0->unk_1AB4 = 1;
    arg0->unk_1AB6 = 0;
    arg0->unk_1AB8 = 0;

    for (u16 i = 0; i < ARRAY_COUNT(arg0->unk_1ABA); i++) {
        arg0->unk_1ABA[i] = -1;
    }

    arg0->unk_1AC0 = 0;
    arg0->unk_1AB9 = 0;
    func_02022488(&arg0->unk_1AC2);
    func_02022488(&arg0->unk_1AF2);
    func_020224f4(&arg0->unk_1B24);
    func_02022534(&arg0->unk_1D7C);
    func_0202254c(&arg0->unk_1D84);

    arg0->unk_1D94 = 0;

    for (u16 i = 0; i < ARRAY_COUNT(arg0->unk_1D98); i++) {
        arg0->unk_1D98[i] = 0;
    }

    for (u16 deck = 0; deck < 4; deck++) {
        for (u16 slot = 0; slot < 6; slot++) {
            arg0->pinLayouts[deck][slot].pinID      = 0xFFFF;
            arg0->pinLayouts[deck][slot].battlePP   = 0;
            arg0->pinLayouts[deck][slot].minglePP   = 0;
            arg0->pinLayouts[deck][slot].shutdownPP = 0;
            arg0->pinLayouts[deck][slot].unk_08     = 0;
            arg0->pinLayouts[deck][slot].flags.raw  = (arg0->pinLayouts[deck][slot].flags.raw & 0x80) | 0x81;
        }
    }

    func_0202255c(arg0->unk_1EB2);
    func_0202258c(&arg0->unk_323E);
    func_020225c4(&arg0->unk_1FCA);
    func_020225ec(&arg0->unk_214C);
    Progress_ResetSecretReports(arg0->secretReports);

    data_02071cf0.unk_20.unk_2324 = 0;

    for (u16 i = 0; i < ARRAY_COUNT(arg0->unk_2326); i++) {
        arg0->unk_2326[i] = 0;
    }

    func_02022640(&arg0->unk_2348);
    arg0->unk_242C = 0;
    arg0->unk_2430 = 0;
    arg0->unk_2432 = 0;
    arg0->unk_2434 = 0;
    arg0->unk_2436 = 0;
    arg0->unk_2438 = 0;
    arg0->unk_243A = 0;
    arg0->unk_243C = 0;
    arg0->unk_243E = 0;
}

u16 Inventory_GetCategorizedIndex(u16 itemIndex) {
    if (itemIndex == 0xffff) {
        return 0xffff;
    }
    if (itemIndex < 304) {
        return itemIndex;
    }
    if (itemIndex < 584) {
        return itemIndex - 304;
    }
    if (itemIndex < 626) {
        return itemIndex - 584;
    }
    return itemIndex - 626;
}

ItemCategory Inventory_GetCategory(u16 itemIndex) {
    ItemCategory category = ITEM_CATEGORY_INVALID;

    if (itemIndex != 0xffff) {
        if (itemIndex < 304) {
            category = ITEM_CATEGORY_PIN;
        } else if (itemIndex < 584) {
            category = ITEM_CATEGORY_THREAD;
        } else if (itemIndex < 626) {
            category = ITEM_CATEGORY_FOOD;
        } else {
            category = ITEM_CATEGORY_SWAG;
        }
    }
    return category;
}

// Nonmatching: Regswaps
u16 Inventory_GetOpenPinStockpileCapacity(void) {
    MainData* mainData = &data_02071cf0.unk_20;

    RawPinData pinData;

    u16 var_r7 = 0;

    for (u16 deck = 0; deck < 4; deck++) {
        for (u16 slot = 0; slot < 6; slot++) {
            u32 deckOff = deck * 0x3C;
            u32 slotOff = slot * 0xA;
            u16 pinID   = mainData->pinLayouts[deck][slot].pinID;

            if (pinID != 0xFFFF) {
                Data_LoadToBuffer(1, pinData, &data_0205c188, pinID);
                if (pinData.unk_25 != data_02071cf0.unk_20.pinLayouts[deck][slot].flags.bits.level) {
                    var_r7 += 1;
                }
            }
        }
    }

    mainData = &data_02071cf0.unk_20;
    for (u16 i = 0; i < 256; i++) {
        if (mainData->stockpilePins[i].pinID != 0xFFFF) {
            var_r7++;
        }
    }

    if (var_r7 > 256) {
        return 0;
    }
    return 256 - var_r7;
}

BOOL Inventory_CanAddPin(u16 itemID, s32 arg1) {
    MainData*  mainData;
    RawPinData pinData;

    ItemCategory category = Inventory_GetCategory(itemID);

    if (itemID >= 776) {
        return FALSE;
    }

    if (arg1 == 1) {
        Data_LoadToBuffer(1, pinData, &data_0205c158, itemID);
        if (pinData.unk_25 == 1) {
            arg1 = 2;
        }
    }

    if (category == ITEM_CATEGORY_PIN) {
        if (arg1 == 1) {
            if (Inventory_GetOpenPinStockpileCapacity() != 0) {
                return TRUE;
            }
        } else if (arg1 == 2) {
            for (u16 i = 0; i < 304; i++) {
                mainData = &data_02071cf0.unk_20;
                if (itemID == mainData->masteredPins[i].pinID) {
                    return data_02071cf0.unk_20.masteredPins[i].count < 99;
                }
            }

            for (u16 i = 0; i < 304; i++) {
                mainData = &data_02071cf0.unk_20;
                if (mainData->masteredPins[i].pinID == 0xFFFF) {
                    return TRUE;
                }
            }
        }
    } else if (func_02023010(itemID) < 9) {
        return TRUE;
    }
    return FALSE;
}

// Nonmatching: bitfield is likely incorrect, or at least incorrectly accessed
static void Inventory_AddStockpiledPin(u16 pinIdx, s32 pinID, u32 flag) {
    data_02071cf0.unk_20.stockpilePins[pinIdx].pinID            = pinID;
    data_02071cf0.unk_20.stockpilePins[pinIdx].battlePP         = 0;
    data_02071cf0.unk_20.stockpilePins[pinIdx].minglePP         = 0;
    data_02071cf0.unk_20.stockpilePins[pinIdx].shutdownPP       = 0;
    data_02071cf0.unk_20.stockpilePins[pinIdx].flags.bits.level = 1;
    data_02071cf0.unk_20.stockpilePins[pinIdx].flags.raw &= ~0x80;
    data_02071cf0.unk_20.stockpilePins[pinIdx].flags.raw |= (flag >> 0x18);
}

// Nonmatching: bitfield is likely incorrect, or at least incorrectly accessed
static void Inventory_AddMasteredPin(u16 pinIdx, s32 pinID, u32 flag) {
    data_02071cf0.unk_20.masteredPins[pinIdx].pinID = pinID;

    data_02071cf0.unk_20.masteredPins[pinIdx].flags.raw &= ~0x80;
    data_02071cf0.unk_20.masteredPins[pinIdx].flags.raw |= ((flag << 0x1f) >> 0x18);

    data_02071cf0.unk_20.masteredPins[pinIdx].count++;
    if (data_02071cf0.unk_20.masteredPins[pinIdx].count > 99) {
        data_02071cf0.unk_20.masteredPins[pinIdx].count = 99;
    }
}

// Nonmatching: bitfield is likely incorrect, or at least incorrectly accessed
void Inventory_AddStockItem(u16 arg0, s32 arg1, s32 arg2) {
    data_02071cf0.unk_20.inventoryItems[arg0].itemID = arg1;

    u8 temp_r1 = ((data_02071cf0.unk_20.inventoryItems[arg0].flags & ~0x10) | ((u32)(arg2 << 0x1F) >> 0x1B)) & ~0x20;

    data_02071cf0.unk_20.inventoryItems[arg0].flags = (temp_r1 & ~0xF) | (((temp_r1 & 0xF) + 1) & 0xF);

    if (((u32)(data_02071cf0.unk_20.inventoryItems[arg0].flags << 0x1C) >> 0x1C) > 9) {
        data_02071cf0.unk_20.inventoryItems[arg0].flags = (data_02071cf0.unk_20.inventoryItems[arg0].flags & ~0xF) | 9;
    }
}

// Nonmatching: Difference in arg1 arithmetic
BOOL Inventory_AddItem(u16 itemID, s32 arg1) {
    RawPinData pinData;

    ItemCategory category = Inventory_GetCategory(itemID);

    if (itemID >= 776) {
        return FALSE;
    }
    if (arg1 == 1) {
        Data_LoadToBuffer(1, pinData, &data_0205c160, itemID);
        if (pinData.unk_25 == 1) {
            arg1 = 2;
        }
    }

    s32 temp_r0 = arg1 & 0xF000;
    if (temp_r0 != 0x1000) {
        return temp_r0 != 0;
    }
    s32 temp_r0_2 = arg1 & 0xF;

    if (category == ITEM_CATEGORY_PIN) {
        if (temp_r0_2 == 1) {
            if (Inventory_GetOpenPinStockpileCapacity() == 0) {
                return FALSE;
            }
            for (u16 i = 0; i < 256; i++) {
                if (data_02071cf0.unk_20.stockpilePins[i].pinID == 0xFFFF) {
                    Inventory_AddStockpiledPin(i, itemID, arg1);
                    return TRUE;
                }
            }
        } else if (temp_r0_2 == 2) {
            for (u16 i = 0; i < 304; i++) {
                if (itemID == data_02071cf0.unk_20.masteredPins[i].pinID) {
                    Inventory_AddMasteredPin(i, itemID, arg1);
                    return TRUE;
                }
            }

            for (u16 i = 0; i < 304; i++) {
                if (data_02071cf0.unk_20.masteredPins[i].pinID == 0xFFFF) {
                    Inventory_AddMasteredPin(i, itemID, arg1);
                    return TRUE;
                }
            }
        }
        return FALSE;
    } else {
        for (u16 i = 0; i < 472; i++) {
            if (itemID == data_02071cf0.unk_20.inventoryItems[i].itemID) {
                Inventory_AddStockItem(i, itemID, arg1);
                return TRUE;
            }
        }

        for (u16 i = 0; i < 472; i++) {
            if (data_02071cf0.unk_20.inventoryItems[i].itemID == 0xFFFF) {
                Inventory_AddStockItem(i, itemID, arg1);
                return TRUE;
            }
        }
        return FALSE;
    }
}

// Nonmatching: Instruction reordering
u32 func_02023010(u16 arg0) {
    MainData* mainData;

    ItemCategory category = Inventory_GetCategory(arg0);

    u16 var_r3 = 0;

    if (arg0 >= 776) {
        return 0;
    }

    if (category == ITEM_CATEGORY_PIN) {
        for (u16 deck = 0; deck < 4; deck++) {
            for (u16 slot = 0; slot < 6; slot++) {
                mainData = &data_02071cf0.unk_20;
                if (arg0 == mainData->pinLayouts[deck][slot].pinID) {
                    var_r3++;
                }
            }
        }

        for (u16 i = 0; i < 256; i++) {
            mainData = &data_02071cf0.unk_20;
            if (arg0 == mainData->stockpilePins[i].pinID) {
                var_r3++;
            }
        }

        for (u16 i = 0; i < 304; i++) {
            mainData = &data_02071cf0.unk_20;
            if (arg0 == mainData->masteredPins[i].pinID) {
                var_r3 += data_02071cf0.unk_20.masteredPins[i].count;
                break;
            }
        }
    } else {
        for (u16 i = 0; i < 4; i++) {
            mainData = &data_02071cf0.unk_20;
            if (arg0 == mainData->playerStats.equippedThreads[i]) {
                var_r3 += 1;
            }
        }

        for (u16 friendIdx = 0; friendIdx < 3; friendIdx++) {
            for (u16 slot = 0; slot < 4; slot++) {
                mainData = &data_02071cf0.unk_20;
                if (arg0 == mainData->friendStats[friendIdx].equippedThreads[slot]) {
                    var_r3++;
                }
            }
        }

        for (u16 i = 0; i < 472; i++) {
            mainData = &data_02071cf0.unk_20;
            if (arg0 == mainData->inventoryItems[i].itemID) {
                var_r3 += ((u32)(data_02071cf0.unk_20.inventoryItems[i].flags << 0x1C) >> 0x1C);
                break;
            }
        }
    }
    return var_r3;
}

// Nonmatching: Some missing/reordered instructions, data access differences
s32 func_02023208(u16 arg0, u32 arg1, s32 arg2) {
    ItemCategory category = Inventory_GetCategory(arg0);
    u32          owned    = 0;

    if (arg0 > 776) {
        return 0;
    }

    if (category == ITEM_CATEGORY_PIN) {
        for (u16 deck = 0; deck < 4; deck++) {
            for (u16 slot = 0; slot < 6; slot++) {
                u32 deckOff = deck * 0x3C;
                u32 slotOff = slot * 0xA;

                if ((data_02071cf0.unk_20.pinLayouts[deck][slot].pinID == arg0) &&
                    (arg2 <= data_02071cf0.unk_20.pinLayouts[deck][slot].flags.bits.level))
                {
                    owned = (owned + 1) & 0xFFFF;
                }
            }
        }

        for (u16 i = 0; i < 256; i++) {
            if ((data_02071cf0.unk_20.stockpilePins[i].pinID == arg0) &&
                (arg2 <= data_02071cf0.unk_20.stockpilePins[i].flags.bits.level))
            {
                owned = (owned + 1) & 0xFFFF;
            }
        }

        for (u16 i = 0; i < 304; i++) {
            if ((data_02071cf0.unk_20.masteredPins[i].pinID == arg0) &&
                (data_02071cf0.unk_20.masteredPins[i].flags.bits.level >= arg2))
            {
                owned = (owned + data_02071cf0.unk_20.masteredPins[i].count) & 0xFFFF;
                break;
            }
        }
    } else {
        for (u16 i = 0; i < 4; i++) {
            if (data_02071cf0.unk_20.playerStats.equippedThreads[i] == arg0) {
                owned = (owned + 1) & 0xFFFF;
            }
        }

        for (u16 ch = 0; ch < 3; ch++) {
            for (u16 slot = 0; slot < 4; slot++) {
                if (data_02071cf0.unk_20.friendStats[ch].equippedThreads[slot] == arg0) {
                    owned = (owned + 1) & 0xFFFF;
                }
            }
        }

        for (u16 i = 0; i < 472; i++) {
            if (data_02071cf0.unk_20.inventoryItems[i].itemID == arg0) {
                owned = (owned + ((u32)(data_02071cf0.unk_20.inventoryItems[i].flags << 0x1C) >> 0x1C)) & 0xFFFF;
                break;
            }
        }
    }

    return (arg1 <= owned);
}

// Nonmatching: Significant differences, needs reworked
s32 func_02023480(u32 arg0, s32 arg1) {
    u32  buffer[48]; // 0xc0 bytes = 192 bytes = 48 words
    u32* src;
    u32* dst;
    s32  i;
    s32  offset;
    f64  temp;
    f64  val0;
    f64  val1;
    f64  val2;
    u32  temp0, temp1, temp2, temp3;

    // Copy data using a loop to match original assembly
    src = (u32*)&data_0205c2dc;
    dst = buffer;

    for (i = 12; i != 0; i--) {
        temp0  = src[0];
        temp1  = src[1];
        temp2  = src[2];
        temp3  = src[3];
        dst[0] = temp0;
        dst[1] = temp1;
        dst[2] = temp2;
        dst[3] = temp3;
        src += 4;
        dst += 4;
    }

    if (arg0 <= 1) {
        return 0;
    }

    temp   = (f64)(arg0 - 1);
    offset = arg1 * 0x18;

    val0 = temp * *(f64*)((u8*)buffer + 0x10 + offset);
    val1 = temp * *(f64*)((u8*)buffer + 0x8 + offset);
    val2 = temp * *(f64*)((u8*)buffer + 0x0 + offset);

    return func_02054a9c(val2 + func_02053098(val0, val1));
}

// Nonmatching: Arithmetic differences
s32 func_0202353c(u16 arg0) {
    u16 temp_r5 = (u16)((arg0 >> 0x1F) + (((arg0 << 0x1C) - (arg0 >> 0x1F)) >> 0x1C));

    if (arg0 >= 0x2F) {
        OS_WaitForever();
    }

    return (data_02071cf0.unk_20.unk_1ABA[arg0] & (1 << temp_r5)) != 0;
}

u16 func_02023588(void) {
    return data_02071cf0.unk_20.unk_1AC0;
}

void func_02023598(s16 arg0) {
    data_02071cf0.unk_20.unk_1AC0 = arg0;
}

void func_020235a8(void) {
    if (data_02071cf0.unk_20.unk_1D94 < 216000000) {
        data_02071cf0.unk_20.unk_1D94++;
    }
}

void func_020235cc(u32 arg0, s32 arg1) {
    if (arg0 >= 41) {
        OS_WaitForever();
    }
    data_02071cf0.unk_20.unk_1D98[arg0] |= arg1;
}

void func_020235fc(u32 arg0, s32 arg1) {
    if (arg0 >= 41) {
        OS_WaitForever();
    }
    data_02071cf0.unk_20.unk_1D98[arg0] &= ~arg1;
}

void func_02023630(s32 arg0) {
    MainData* mainData = &data_02071cf0.unk_20;

    for (u16 i = 0; i < 41; i++) {
        mainData->unk_1D98[i] &= ~arg0;
    }
}

s32 func_0202366c(u32 arg0, s32 arg1) {
    if (arg0 >= 41) {
        OS_WaitForever();
    }
    if (arg1 & data_02071cf0.unk_20.unk_1D98[arg0]) {
        return 1;
    }
    return 0;
}

s32 func_020236a0(void) {
    MainData* mainData = &data_02071cf0.unk_20;

    for (u16 i = 0; i < 41; i++) {
        if ((mainData->unk_1D98[i] & 0x6) != 0) {
            return 1;
        }
    }
    return 0;
}

s16 Stats_GetEffectiveValue(ActiveFriend activeFriend, StatType statType) {
    MainData*    mainData;
    RawItemData  itemData;
    u16          itemID;
    ItemCategory itemCategory;

    s16 stat = 0;

    if (statType == STAT_ATTACK) {
        stat += data_02071cf0.unk_20.playerStats.attack;
    } else if (statType == STAT_DEFENSE) {
        stat += data_02071cf0.unk_20.playerStats.defense;
    } else {
        stat += data_02071cf0.unk_20.playerStats.baseHealth;
    }

    u16 i;
    for (i = 0; i < 4; i++) {
        mainData     = &data_02071cf0.unk_20;
        itemID       = mainData->playerStats.equippedThreads[i];
        itemCategory = Inventory_GetCategory(itemID);

        if ((itemID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
            Data_LoadToBuffer(1, itemData, &data_0205c170, Inventory_GetCategorizedIndex(itemID));

            if (statType == STAT_ATTACK) {
                stat += itemData.attack;
            } else if (statType == STAT_DEFENSE) {
                stat += itemData.defense;
            } else {
                stat += itemData.health;
            }
        }
    }

    if ((statType == STAT_HEALTH) && (activeFriend != FRIEND_NONE)) {
        for (i = 0; i < 4; i++) {
            mainData     = &data_02071cf0.unk_20;
            itemID       = mainData->friendStats[activeFriend].equippedThreads[i];
            itemCategory = Inventory_GetCategory(itemID);

            if ((itemID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
                Data_LoadToBuffer(1, itemData, &data_0205c170, Inventory_GetCategorizedIndex(itemID));
                stat += itemData.health;
            }
        }
    }
    if (stat < 0) {
        stat = 0;
    }
    return stat;
}

// Nonmatching: Differences in arithmetic, data access, and instruction order
u32 Stats_GetMaxHealth(ActiveFriend activeFriend, u16 playerLevel) {
    RawItemData itemData;

    u16 health = (((playerLevel - 1) * 50) + 200 + data_02071cf0.unk_20.playerStats.baseHealth);

    for (u16 i = 0; i < 4; i++) {
        u16          itemID       = data_02071cf0.unk_20.playerStats.equippedThreads[i];
        ItemCategory itemCategory = Inventory_GetCategory(itemID);
        if ((itemID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
            Data_LoadToBuffer(1, itemData, &data_0205c180, Inventory_GetCategorizedIndex(itemID));
            health += itemData.health;
        }
    }

    if (activeFriend != FRIEND_NONE) {
        for (u16 i = 0; i < 4; i++) {
            u16          itemID       = data_02071cf0.unk_20.friendStats[activeFriend].equippedThreads[i];
            ItemCategory itemCategory = Inventory_GetCategory(itemID);
            if ((itemID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
                Data_LoadToBuffer(1, itemData, &data_0205c180, Inventory_GetCategorizedIndex(itemID));
                health += itemData.health;
            }
        }
    }
    if (health < 1) {
        health = 1;
    }
    return health;
}

s16 Stats_GetEffectiveFriendValue(ActiveFriend activeFriend, StatType statType) {
    MainData*    mainData;
    RawItemData  itemData;
    u16          itemID;
    ItemCategory itemCategory;

    s16 stat = 0;

    if ((statType != STAT_HEALTH) && (activeFriend != FRIEND_NONE)) {
        if (statType == STAT_ATTACK) {
            stat += data_02071cf0.unk_20.friendStats[activeFriend].attack;
        } else if (statType == STAT_DEFENSE) {
            stat += data_02071cf0.unk_20.friendStats[activeFriend].defense;
        }

        mainData = &data_02071cf0.unk_20;
        for (u16 i = 0; i < 4; i++) {
            itemID       = mainData->friendStats[activeFriend].equippedThreads[i];
            itemCategory = Inventory_GetCategory(itemID);

            if ((itemID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
                Data_LoadToBuffer(1, itemData, &data_0205c138, Inventory_GetCategorizedIndex(itemID));
                if (statType == STAT_ATTACK) {
                    stat += itemData.attack;
                } else if (statType == STAT_DEFENSE) {
                    stat += itemData.defense;
                }
            }
        }
    }

    if (stat < 0) {
        stat = 0;
    }
    return stat;
}

s32 func_02023b7c(u16 itemID) {
    u32          itemIdx  = Inventory_GetCategorizedIndex(itemID);
    ItemCategory category = Inventory_GetCategory(itemID);

    if ((itemID == 0xFFFF) || (category != ITEM_CATEGORY_THREAD)) {
        return 0;
    }
    if (itemIdx > 279) {
        return 0;
    }
    return (((u32)(data_02071cf0.unk_20.unk_1EB2[itemIdx] << 0x1F) >> 0x1F) != 0);
}

s32 func_02023be8(u16 itemID, s32 arg1) {
    RawPinData      pinData;
    RawItemData     itemData;
    RawFoodData     foodData;
    RawTreasureData treasureData;

    u32 itemIdx = Inventory_GetCategorizedIndex(itemID);

    s32 var_r5;
    switch (Inventory_GetCategory(itemID)) {
        case ITEM_CATEGORY_PIN: {
            Data* data = Data_Load(1, pinData, &data_0205c168, itemIdx);
            if (arg1 == 1) {
                var_r5 = pinData.unk_08;
            } else {
                var_r5 = ((pinData.unk_25 - 1) * pinData.unk_0C) + pinData.unk_08;
            }
            DatMgr_ReleaseData(data);
        } break;

        case ITEM_CATEGORY_THREAD: {
            Data* data = Data_Load(1, itemData, &data_0205c128, itemIdx);
            var_r5     = itemData.unk_04;
            DatMgr_ReleaseData(data);
        } break;

        case ITEM_CATEGORY_FOOD: {
            Data* data = Data_Load(1, foodData, &data_0205c178, itemIdx);
            var_r5     = foodData.unk_04;
            DatMgr_ReleaseData(data);
        } break;

        case ITEM_CATEGORY_SWAG: {
            Data* data = Data_Load(1, treasureData, &data_0205c130, itemIdx);
            var_r5     = treasureData.unk_04;
            DatMgr_ReleaseData(data);
        } break;

        default:
            OS_WaitForever();
            break;
    }
    return var_r5;
}

void func_02023d00(s32 arg0) {
    data_02071cf0.unk_20.unk_2324 |= (1 << arg0);
}

s32 func_02023d1c(s32 arg0) {
    s32 var_r2 = 1;
    if ((data_02071cf0.unk_20.unk_2324 & (1 << arg0)) == 0) {
        var_r2 = 0;
    }
    return var_r2;
}

// Nonmatching: Arithmetic differences
void func_02023d3c(u32 arg0) {
    u16 temp_r5;
    u32 temp_r2;
    u32 temp_r4;

    temp_r2 = arg0 >> 0x1F;
    temp_r4 = (arg0 << 0xC) >> 0x10;
    temp_r5 = (u16)(temp_r2 + (((arg0 << 0x1C) - temp_r2) >> 0x1C));
    if (arg0 >= 0x100) {
        OS_WaitForever();
    }
    data_02071cf0.unk_20.unk_2326[temp_r4] |= (1 << temp_r5);
}

// Nonmatching: Arithmetic differences
s32 func_02023d88(u32 arg0) {
    s32 var_r0;
    u16 temp_r5;
    u32 temp_r2;

    temp_r2 = arg0 >> 0x1F;
    temp_r5 = (u16)(temp_r2 + (((arg0 << 0x1C) - temp_r2) >> 0x1C));
    if (arg0 >= 0x100) {
        OS_WaitForever();
    }
    var_r0 = 1;
    if (!(data_02071cf0.unk_20.unk_2326[(arg0 << 0xC) >> 0x10] & (1 << temp_r5))) {
        var_r0 = 0;
    }
    return var_r0;
}

BOOL func_02023dd4(u32 arg0) {
    MainData* mainData = &data_02071cf0.unk_20;
    u16       i;

    // Check if all player equipped slots are empty
    for (i = 0; i < 4; i++) {
        if (mainData->playerStats.equippedThreads[i] != 0xFFFF) {
            return FALSE;
        }
    }

    // If character index valid, check if all friend equipped slots are empty
    if (arg0 < 3) {
        for (i = 0; i < 4; i++) {
            if (mainData->friendStats[arg0].equippedThreads[i] != 0xFFFF) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

// Nonmatching: Unknown local stack object differences
s32 func_02023e58(u32 arg0) {
    MainData* mainData = &data_02071cf0.unk_20;

    RawItemData itemData;

    u8 sp4[4];
    sp4[0] = *(u8*)((s32)&data_0205c120 + 4);
    sp4[1] = *(u8*)((s32)&data_0205c120 + 5);
    sp4[2] = *(u8*)((s32)&data_0205c120 + 6);
    sp4[3] = *(u8*)((s32)&data_0205c120 + 7);

    if (arg0 >= 13) {
        return 0;
    }

    for (u16 i = 0; i < 4; i++) {
        u16          equippedID   = mainData->playerStats.equippedThreads[i];
        u32          itemIdx      = Inventory_GetCategorizedIndex(equippedID);
        ItemCategory itemCategory = Inventory_GetCategory(equippedID);

        if ((equippedID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
            Data_LoadToBuffer(1, itemData, &data_0205c140, itemIdx);
            sp4[i] = itemData.unk_02;
        }
    }

    for (u16 i = 0; i < 4; i++) {
        if (sp4[i] != arg0) {
            return 0;
        }
    }
    return 1;
}

// Nonmatching: Unknown local stack object differences
s32 func_02023f60(u32 arg0, u32 arg1) {
    RawItemData itemData;

    u8 sp4[4];
    sp4[0] = *(u8*)((s32)&data_0205c120 + 0);
    sp4[1] = *(u8*)((s32)&data_0205c120 + 1);
    sp4[2] = *(u8*)((s32)&data_0205c120 + 2);
    sp4[3] = *(u8*)((s32)&data_0205c120 + 3);

    if (arg1 >= 0xD) {
        return 0;
    }
    if (arg0 >= 3) {
        return 0;
    }

    for (u16 i = 0; i < 4; i++) {
        u16          equippedID   = data_02071cf0.unk_20.friendStats[arg0].equippedThreads[i];
        u32          itemIdx      = Inventory_GetCategorizedIndex(equippedID);
        ItemCategory itemCategory = Inventory_GetCategory(equippedID);

        if ((equippedID != 0xFFFF) && (itemCategory == ITEM_CATEGORY_THREAD)) {
            Data_LoadToBuffer(1, itemData, &data_0205c148, itemIdx);
            sp4[i] = itemData.unk_02;
        }
    }

    for (u16 i = 0; i < 4; i++) {
        if (sp4[i] != arg1) {
            return 0;
        }
    }
    return 1;
}

u32 func_02024080(void) {
    MainData* mainData = &data_02071cf0.unk_20;

    u32 var_r0 = 0;
    for (u16 i = 0; i < 96; i++) {
        if (mainData->unk_1FCC[i].unk_0 > 0) {
            var_r0 = (u16)(var_r0 + 1);
        }
    }

    u32 temp_r1 = data_02071cf0.unk_20.experience.unk_0 << 0x19;
    if (var_r0 <= (temp_r1 >> 0x19)) {
        var_r0 = temp_r1 >> 0x19;
    }

    return var_r0;
}

s32 func_020240e0(u16 arg0, void* arg1) {
    for (u16 deck = 0; deck < 4; deck++) {
        for (u16 slot = 0; slot < 6; slot++) {
            u32 deckOff = deck * 0x3C;
            u32 slotOff = slot * 0xA;

            if ((arg0 == data_02071cf0.unk_20.pinLayouts[deck][slot].pinID) &&
                (*(u8*)((u32)arg1 + 0x25) == data_02071cf0.unk_20.pinLayouts[deck][slot].flags.bits.level))
            {
                return 1;
            }
        }
    }

    for (u16 i = 0; i < 304; i++) {
        if (arg0 == data_02071cf0.unk_20.masteredPins[i].pinID) {
            return 1;
        }
    }

    return 0;
}

u16 func_020241b0(void) {
    u16 i;

    u16   var_r5    = 0;
    void* badgeData = Mem_AllocHeapTail(&gDebugHeap, 0x3DC0);

    Mem_SetSequence(&gDebugHeap, badgeData, "badge_data");
    DatMgr_ReleaseData(DatMgr_LoadRawData(1, badgeData, 0x3DC0, &data_0205c150));

    for (i = 0; i < 304; i++) {
        if (func_020240e0(i, badgeData) == 1) {
            var_r5++;
        }
    }

    Mem_Free(&gDebugHeap, badgeData);
    return var_r5;
}

// Nonmatching: u16/u32 cast difference
u32 func_02024244(void) {
    u32 var_r0 = 0;

    MainData* mainData = &data_02071cf0.unk_20;

    for (u16 i = 0; i < 472; i++) {
        if (mainData->inventoryItems[i].itemID != 0xFFFF) {
            var_r0 = (u16)(var_r0 + 1);
        }
    }

    u32 temp_r1 = data_02071cf0.unk_20.experience.unk_2 << 0x17;
    if (var_r0 <= (temp_r1 >> 0x17)) {
        var_r0 = (temp_r1 >> 0x17);
    }
    return var_r0;
}

// ============ func_020242ac ============
// Gets a value from data table by index
// arg0: table index
// Returns value from data_0205c1e4 table
u16 func_020242ac(s32 arg0) {
    u16 sp0[0x16];
    s32 var_r3;

    for (var_r3 = 0; var_r3 < 0x16; var_r3++) {
        sp0[var_r3] = *(u16*)((s32)&data_0205c1e4 + var_r3 * 2);
    }
    return sp0[arg0];
}

// Nonmatching: Regswaps
BOOL Progress_HasAcquiredAllSecretReports(void) {
    u16 starsRequired[22];
    starsRequired = SecretReportStarRequirements;

    MainData* mainData = &data_02071cf0.unk_20;

    for (u16 reportIdx = 0; reportIdx < 22; reportIdx++) {
        for (u16 starIdx = 0; starIdx < starsRequired[reportIdx]; starIdx++) {
            if (mainData->secretReports[reportIdx].completedStars[starIdx] == 0) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

void Stats_AddExperience(s32 exp) {
    data_02071cf0.unk_20.experience.current += exp;
    if (data_02071cf0.unk_20.experience.current > 65535) {
        data_02071cf0.unk_20.experience.current = 65535;
    }
}

// Nonmatching: Small instruction/data differences
s32 func_020243d4(u32 arg0) {
    s32 sp0[13];
    sp0 = data_0205c270;

    if ((arg0 < 22) || (arg0 > 34)) {
        OS_WaitForever();
    }
    return sp0[arg0 - 22];
}

// Nonmatching: Significant order differences
s32 func_02024434(s32 arg0) {
    s32 val = 0;
    switch (arg0) {
        case 226:
            val = 100;
            break;
        case 9:
        case 53:
        case 129:
            val = 70;
            break;
        case 100:
        case 127:
        case 146:
        case 175:
        case 267:
            val = 50;
            break;
        case 101:
        case 128:
        case 130:
        case 131:
        case 132:
        case 147:
        case 176:
        case 272:
            val = 30;
            break;
        case 133:
            val = 10;
            break;
    }
    return val;
}

// Nonmatching: Significant differences, needs rework
void func_02024558(void) {
    for (u16 i = 0; i < 472; i++) {
        u16 itemID = i + 304;

        // Skip special item IDs
        if ((itemID == 690) || (itemID == 691) || (itemID == 698) || (itemID == 699)) {
            continue;
        }

        data_02071cf0.unk_20.inventoryItems[i].itemID = 0xFFFF;
        data_02071cf0.unk_20.inventoryItems[i].flags  = ((data_02071cf0.unk_20.inventoryItems[i].flags & ~0xF) | 0x10) & ~0x20;
    }
}

void Inventory_SetStarterPins(void) {
    data_02071cf0.unk_20.equippedPins[0].pinID  = PIN_PYROKINESIS;
    data_02071cf0.unk_20.equippedPins[0].unk_08 = 0;
    data_02071cf0.unk_20.equippedPins[1].pinID  = PIN_SHOCKWAVE;
    data_02071cf0.unk_20.equippedPins[1].unk_08 = 0;
    data_02071cf0.unk_20.equippedPins[2].pinID  = PIN_CURE_DRINK;
    data_02071cf0.unk_20.equippedPins[2].unk_08 = 0;
    data_02071cf0.unk_20.equippedPins[3].pinID  = PIN_FORCE_ROUNDS;
    data_02071cf0.unk_20.equippedPins[3].unk_08 = 0;
    data_02071cf0.unk_20.equippedPins[4].pinID  = PIN_PSYCHOKINESIS;
    data_02071cf0.unk_20.equippedPins[4].unk_08 = 1;
    data_02071cf0.unk_20.equippedPins[5].pinID  = PIN_THUNDERBOLT;
    data_02071cf0.unk_20.equippedPins[5].unk_08 = 1;
}

BOOL Inventory_AreAllEquippedPinSlotsEmpty(void) {
    MainData* mainData = &data_02071cf0.unk_20;

    for (u16 i = 0; i < 6; i++) {
        if (mainData->equippedPins[i].pinID != 0xFFFF) {
            return FALSE;
        }
    }
    return TRUE;
}
