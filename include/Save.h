#ifndef SAVE_H
#define SAVE_H

#include "Player/Inventory.h"
#include "Player/Progress.h"
#include "Player/Stats.h"

typedef struct {
    /* 0x0 */ s16 unk_0;
    /* 0x2 */ u16 unk_2;
} UnkSmallStruct;

typedef struct MainData {
    /* 0x0000 */ PlayerStats    playerStats;
    /* 0x002C */ FriendStats    friendStats[3];
    /* 0x0074 */ EquippedPin    equippedPins[6];
    /* 0x00B0 */ StockpilePin   stockpilePins[256];
    /* 0x0AB0 */ MasteredPin    masteredPins[304];
    /* 0x0F70 */ InventoryItem  inventoryItems[472];
    /* 0x16D0 */ s32            unk_16D0;
    /* 0x16D4 */ char           unk_16D4[0x1A18 - 0x16D4];
    /* 0x1A18 */ s32            unk_1A18[35];
    /* 0x1AA4 */ Experience     experience;
    /* 0x1AB0 */ u8             unk_1AB0;
    /* 0x1AB1 */ u8             unk_1AB1;
    /* 0x1AB2 */ u8             unk_1AB2;
    /* 0x1AB3 */ u8             unk_1AB3;
    /* 0x1AB4 */ u16            unk_1AB4;
    /* 0x1AB6 */ u16            unk_1AB6;
    /* 0x1AB8 */ u8             unk_1AB8;
    /* 0x1AB9 */ u8             unk_1AB9;
    /* 0x1ABA */ u16            unk_1ABA[3];
    /* 0x1AC0 */ u16            unk_1AC0;
    /* 0x1AC2 */ u16            unk_1AC2;
    /* 0x1AC4 */ char           unk_1AC4[0x1AF2 - 0x1AC4];
    /* 0x1AF2 */ u16            unk_1AF2;
    /* 0x1AF4 */ char           unk_1AF4[0x1B24 - 0x1AF4];
    /* 0x1B24 */ u16            unk_1B24;
    /* 0x1B26 */ char           unk_1B26[0x1D7C - 0x1B26];
    /* 0x1D7C */ u16            unk_1D7C;
    /* 0x1D7E */ char           unk_1D7E[0x1D84 - 0x1D7E];
    /* 0x1D84 */ u16            unk_1D84;
    /* 0x1D86 */ char           unk_1D86[0x1D94 - 0x1D86];
    /* 0x1D94 */ u32            unk_1D94;
    /* 0x1D98 */ u8             unk_1D98[41];
    /* 0x1DC1 */ char           unk_1DC1[1];
    /* 0x1DC2 */ EquippedPin    pinLayouts[4][6];
    /* 0x1EB2 */ u8             unk_1EB2[280];
    /* 0x1FCA */ u16            unk_1FCA;
    /* 0x1FCC */ UnkSmallStruct unk_1FCC[96];
    /* 0x214C */ u16            unk_214C;
    /* 0x214E */ char           unk_214E[0x22CC - 0x214E];
    /* 0x22CC */ SecretReport   secretReports[22];
    /* 0x2324 */ u16            unk_2324;
    /* 0x2326 */ u16            unk_2326[16];
    /* 0x2346 */ u16            unk_2346;
    /* 0x2348 */ u16            unk_2348;
    /* 0x234A */ char           unk_234A[0x242C - 0x234A];
    /* 0x242C */ u32            unk_242C;
    /* 0x2430 */ u16            unk_2430;
    /* 0x2432 */ u16            unk_2432;
    /* 0x2434 */ u16            unk_2434;
    /* 0x2436 */ u16            unk_2436;
    /* 0x2438 */ u16            unk_2438;
    /* 0x243A */ u16            unk_243A;
    /* 0x243C */ u16            unk_243C;
    /* 0x243E */ u16            unk_243E;
    /* 0x2440 */ char           unk_2440[0x246C - 0x2440];
    /* 0x246C */ u32            unk_246C;
    /* 0x2470 */ char           unk_2470[0x2474 - 0x2470];
    /* 0x2474 */ u32            unk_2474;
    /* 0x2478 */ u32            unk_2478;
    /* 0x247C */ u32            unk_247C;
    /* 0x2480 */ u32            unk_2480;
    /* 0x2484 */ u32            unk_2484;
    /* 0x2488 */ char           unk_2488[0x3106 - 0x2488];
    /* 0x3106 */ s16            unk_3106;
    /* 0x3108 */ s16            unk_3108;
    /* 0x310A */ char           unk_310A[0x3112 - 0x310A];
    /* 0x3112 */ u16            unk_3112;
    /* 0x3114 */ u16            unk_3114;
    /* 0x3116 */ u16            unk_3116;
    /* 0x3118 */ u16            unk_3118;
    /* 0x311A */ u16            unk_311A;
    /* 0x311C */ u16            unk_311C;
    /* 0x311E */ char           unk_311E[0x3124 - 0x311E];
    /* 0x3124 */ s32            unk_3124;
    /* 0x3128 */ char           unk_3128[0x312C - 0x3128];
    /* 0x312C */ s32            unk_312C;
    /* 0x3130 */ s32            unk_3130;
    /* 0x3134 */ char           unk_3134[0x313C - 0x3134];
    /* 0x313C */ s32            unk_313C;
    /* 0x3140 */ char           unk_3140[0x3234 - 0x3140];
    /* 0x3234 */ s32            unk_3234;
    /* 0x3238 */ u32            unk_3238;
    /* 0x323E */ u16            unk_323E;
} MainData;

// data_02071cf0 - system state structure
struct SystemState {
    /* 0x00 */ u16      unk_00;
    /* 0x02 */ u16      unk_02;
    /* 0x04 */ void*    globalFriendData;
    /* 0x08 */ s32      unk_08;
    /* 0x0C */ s32      lockID;
    /* 0x10 */ s32      unk_10;
    /* 0x14 */ s32      unk_14;
    /* 0x18 */ s32      unk_18;
    /* 0x1C */ s32      unk_1C;
    /* 0x20 */ MainData unk_20;
};

extern struct SystemState data_02071cf0;

/**
 * @brief Reset all gameplay-related data to their initial state. This includes all player and friend stats, all pins, all
 * equipment, all story progress, and all collections.
 */
void Savefile_ResetAllGameplay(MainData* arg0);

#endif // SAVE_H