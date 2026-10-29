#ifndef SAVE_MAINDATA_H
#define SAVE_MAINDATA_H

#include "Field/FieldTypes.h"
#include "Player/Inventory.h"
#include "Player/Stats.h"

typedef struct {
    /* 0x0 */ union {
        s16 unk_0;
        struct {
            s8 minimumBattleLevel;
            s8 dropFlags;
        };
    };
    /* 0x2 */ u16 unk_2;
} UnkSmallStruct;

typedef struct {
    /* 0x0 */ u8 completedStars[4];
} SecretReport;

// A Noise symbol placed on the field map (scan mode). The field rebuilds the
// table on area entry and the battle request copies the selected record's
// encounter (see Fld_Luck.c / FieldBattleSetup.c).
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 category;
    /* 0x06 */ s16 state;  // 0 = cleared, 1 = placed, 2 = engaged
    /* 0x08 */ s16 mode;   // movement mode of the symbol task
    /* 0x0A */ s16 encounter;
    /* 0x0C */ s16 unk_0C; // 1: unk_10 is a scan-event index; otherwise a FldGate index or -1
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
} ScanSymbolRecord; // Size: 0x14

// Scan-event table, 67 records.
typedef struct {
    /* 0x0 */ s16 firstChapter;
    /* 0x2 */ s16 lastChapter;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 area;
    /* 0xA */ union {
        s16 encounter;
        u16 encounterId;
    };
    /* 0xC */ u16 flags;
} FieldScanEvent;

extern FieldScanEvent data_0205c5be[67];

typedef struct {
    /* 0x00 */ u16 scores[13];
    /* 0x1A */ u8  ranking[13];
    /* 0x27 */ u8  unk_27;
} BrandTrend; // Size: 0x28

typedef struct {
    /* 0x0 */ u8 year   : 7;
    /* 0x1 */ u8 month  : 4;
    /* 0x2 */ u8 day    : 5;
    /* 0x3 */ u8 hour   : 5;
    /* 0x4 */ u8 minute : 6;
    /* 0x5 */ u8 second : 6;
} PackedDateTime; // Size: 0x6

typedef struct {
    /* 0x0 */ u16 itemId;
    /* 0x2 */ s16 count;
} UnkItemCount; // Size: 0x4

typedef struct {
    /* 0x0 */ u8   macAddress[6];
    /* 0x6 */ char unk_6[0x8 - 0x6];
    /* 0x8 */ u32  unk_8; // Low 24 bits: money owed to the player, high 8 bits: shop id
} MingleFriend;           // Size: 0xC

typedef struct {
    /* 0x00 */ u16 shopId  : 6; // Mingle Mode shop settings
    u16            clerkId : 6;
    u16            musicId : 3;
    u16                    : 1;
    /* 0x02 */ u16 giftItemId; // Item handed to whoever mingles with this player
    /* 0x04 */ u16 unk_04[6];
    /* 0x10 */ u16 unk_10[16];
} MingleShop; // Size: 0x30

typedef struct Ov003TouchSlotTable {
    /* 0x00 */ u8  x[6];
    /* 0x06 */ u8  playerX;
    /* 0x07 */ u8  y[6];
    /* 0x0D */ u8  playerY;
    /* 0x0E */ u8  kind[6];
    /* 0x14 */ s16 unk_14[6];
    /* 0x20 */ s16 unk_20[6];
} Ov003TouchSlotTable; // Size: 0x2C

typedef struct MainData {
    /* 0x0000 */ PlayerStats    playerStats;
    /* 0x002C */ FriendStats    friendStats[3];
    /* 0x0074 */ EquippedPin    equippedPins[6];
    /* 0x00B0 */ StockpilePin   stockpilePins[256];
    /* 0x0AB0 */ MasteredPin    masteredPins[304];
    /* 0x0F70 */ InventoryItem  inventoryItems[472];
    /* 0x16D0 */ BrandTrend     brandTrends[21];
    /* 0x1A18 */ s32            unk_1A18[35];
    /* 0x1AA4 */ Experience     experience;
    /* 0x1AB0 */ u8             chapter; // 0-6 Shiki's week, 7-13 Joshua's, 14-20 Beat's, 21+ post-game
    /* 0x1AB1 */ u8             currentArea;
    /* 0x1AB2 */ u8             unk_1AB2;
    /* 0x1AB3 */ u8             unk_1AB3;
    /* 0x1AB4 */ u16            unk_1AB4;
    /* 0x1AB6 */ u16            unk_1AB6;
    /* 0x1AB8 */ u8             unk_1AB8;
    /* 0x1AB9 */ u8             unk_1AB9;
    /* 0x1ABA */ u16            unk_1ABA[3];
    /* 0x1AC0 */ u16            unk_1AC0;
    /* 0x1AC2 */ MingleShop     mingleShop;
    /* 0x1AF2 */ MingleShop     unk_1AF2;
    /* 0x1B22 */ char           unk_1B22[0x1B24 - 0x1B22];
    /* 0x1B24 */ MingleFriend   mingleFriends[50];
    /* 0x1D7C */ u8             civviesMet;
    /* 0x1D7D */ u8             espersMet;
    /* 0x1D7E */ u8             aliensMet;
    /* 0x1D7F */ char           unk_1D7F;
    /* 0x1D80 */ u16            unk_1D80;
    /* 0x1D82 */ char           unk_1D82[0x1D84 - 0x1D82];
    /* 0x1D84 */ u8             unk_1D84;
    /* 0x1D85 */ char           unk_1D85[0x1D88 - 0x1D85];
    /* 0x1D88 */ u32            mabsBasePP;
    /* 0x1D8C */ char           unk_1D8C[0x1D94 - 0x1D8C];
    /* 0x1D94 */ u32            unk_1D94;
    /* 0x1D98 */ u8             unk_1D98[41];
    /* 0x1DC1 */ char           unk_1DC1[1];
    /* 0x1DC2 */ EquippedPin    pinLayouts[4][6];
    /* 0x1EB2 */ u8             unk_1EB2[280];
    /* 0x1FCA */ u16            unk_1FCA;
    /* 0x1FCC */ UnkSmallStruct unk_1FCC[96];
    /* 0x214C */ u32            bestDefeatTimes[96];
    /* 0x22CC */ SecretReport   secretReports[22];
    /* 0x2324 */ u16            unk_2324;
    /* 0x2326 */ u16            unk_2326[16];
    /* 0x2346 */ u16            unk_2346;
    /* 0x2348 */ u16            lastNickName[11]; // Snapshot written by mingle mode before it saves
    /* 0x235E */ u8             lastMacAddress[6];
    /* 0x2364 */ Experience     lastExperience;
    /* 0x2370 */ PackedDateTime lastSaveTime;
    /* 0x2376 */ u8             lastChapter;
    /* 0x2377 */ u8             lastArea;
    /* 0x2378 */ PlayerStats    lastPlayerStats;
    /* 0x23A4 */ FriendStats    lastFriendStats[3];
    /* 0x23EC */ EquippedPin    lastEquippedPins[6];
    /* 0x2428 */ u16            lastBadgeSlots;
    /* 0x242A */ u16            lastGiftItemId;
    /* 0x242C */ u32            unk_242C;
    /* 0x2430 */ u16            bgmFile; // ADX file resumed when returning to the menu
    /* 0x2432 */ u16            unk_2432;
    /* 0x2434 */ u16            unk_2434;
    /* 0x2436 */ u16            unk_2436;
    /* 0x2438 */ u16            unk_2438;
    /* 0x243A */ u16            unk_243A;
    /* 0x243C */ u16            unk_243C;
    /* 0x243E */ u16            unk_243E;
    /* 0x2440 */ char           unk_2440[0x244C - 0x2440];
    /* 0x244C */ FieldPos       fieldPos; // player position on the field map
    /* 0x2454 */ s32            unk_2454;
    /* 0x2458 */ s16            fieldArea;
    /* 0x245A */ u16            unk_245A;
    /* 0x245C */ u32            progressFlags[4]; // 128 flags
    /* 0x246C */ u32            unk_246C;
    /* 0x2470 */ s16            unk_2470;
    /* 0x2472 */ char           unk_2472[0x2474 - 0x2472];
    /* 0x2474 */ u32            unk_2474;
    /* 0x2478 */ u32            unk_2478;
    /* 0x247C */ u32            unk_247C;
    /* 0x2480 */ u32            unk_2480;
    /* 0x2484 */ u32            unk_2484;
    /* 0x2488 */ u32            unk_2488;
    /* 0x248C */ s32            unk_248C;
    /* 0x2490 */ s32            unk_2490;
    /* 0x2494 */ s32            unk_2494;
    /* 0x2498 */ s32            unk_2498;
    /* 0x249C */ s32            unk_249C;
    /* 0x24A0 */ s16*           unk_24A0;
    /* 0x24A4 */ u32            unk_24A4;
    /* 0x24A8 */ u16            unk_24A8;
    /* 0x24AA */ char           unk_24AA[0x24AC - 0x24AA];
    /* 0x24AC */ u32            unk_24AC;
    /* 0x24B0 */ s16*           unk_24B0;
    /* 0x24B4 */ u16            unk_24B4;
    /* 0x24B6 */ u16            unk_24B6;
    /* 0x24B8 */ u32            unk_24B8;
    /* 0x24BC */ u16            currentStoryEvent; // Most recently completed story event, for progress gating and rewards
    /* 0x24BE */ u16            mailEvent;
    /* 0x24C0 */ u16            unk_24C0;
    /* 0x24C2 */ u16            unk_24C2;
    /* 0x24C4 */ u32            unk_24C4;
    /* 0x24C8 */ s32            unk_24C8[16];
    /* 0x2508 */ s32            unk_2508[16]; // parallel to unk_24C8
    /* 0x2548 */ s16            areaPopulation[44];
    /* 0x25A0 */ char           unk_25A0[0x2648 - 0x25A0];
    union {
        /* 0x2648 */ s16 exitStates[75]; // per map exit, passable at >= 4; ids below 0x38 are Fld_Wall barriers
        struct {
            /* 0x2648 */ s16  unk_2648;
            /* 0x264A */ s16  unk_264A;
            /* 0x264C */ s16  unk_264C;
            /* 0x264E */ s16  unk_264E;
            /* 0x2650 */ u16  unk_2650;
            /* 0x2652 */ u16  unk_2652;
            /* 0x2654 */ u16  unk_2654;
            /* 0x2656 */ u16  unk_2656;
            /* 0x2658 */ u16  unk_2658;
            /* 0x265A */ u16  unk_265A;
            /* 0x265C */ u16  unk_265C;
            /* 0x265E */ u16  unk_265E;
            /* 0x2660 */ u16  unk_2660;
            /* 0x2662 */ u16  unk_2662;
            /* 0x2664 */ u16  unk_2664;
            /* 0x2666 */ u16  unk_2666;
            /* 0x2668 */ u16  unk_2668;
            /* 0x266A */ u16  unk_266A;
            /* 0x266C */ u16  unk_266C;
            /* 0x266E */ u16  unk_266E;
            /* 0x2670 */ u16  unk_2670;
            /* 0x2672 */ u16  unk_2672;
            /* 0x2674 */ u16  unk_2674;
            /* 0x2676 */ u16  unk_2676;
            /* 0x2678 */ u16  unk_2678;
            /* 0x267A */ u16  unk_267A;
            /* 0x267C */ s16  unk_267C;
            /* 0x267E */ s16  unk_267E;
            /* 0x2680 */ s16  unk_2680;
            /* 0x2682 */ s16  unk_2682;
            /* 0x2684 */ s16  unk_2684;
            /* 0x2686 */ s16  unk_2686;
            /* 0x2688 */ s16  unk_2688;
            /* 0x268A */ s16  unk_268A;
            /* 0x268C */ s16  unk_268C;
            /* 0x268E */ s16  unk_268E;
            /* 0x2690 */ s16  unk_2690;
            /* 0x2692 */ s16  unk_2692;
            /* 0x2694 */ s16  unk_2694;
            /* 0x2696 */ s16  unk_2696;
            /* 0x2698 */ s16  unk_2698;
            /* 0x269A */ s16  unk_269A;
            /* 0x269C */ s16  unk_269C;
            /* 0x269E */ s16  unk_269E;
            /* 0x26A0 */ s16  unk_26A0;
            /* 0x26A2 */ s16  unk_26A2;
            /* 0x26A4 */ s16  unk_26A4;
            /* 0x26A6 */ s16  unk_26A6;
            /* 0x26A8 */ s16  unk_26A8;
            /* 0x26AA */ u16  unk_26AA;
            /* 0x26AC */ char unk_26AC[0x26B0 - 0x26AC];
            /* 0x26B0 */ u16  unk_26B0;
            /* 0x26B2 */ u16  unk_26B2;
            /* 0x26B4 */ u16  unk_26B4;
            /* 0x26B6 */ u16  unk_26B6;
            /* 0x26B8 */ u16  unk_26B8;
            /* 0x26BA */ u16  unk_26BA;
            /* 0x26BC */ u16  unk_26BC;
            /* 0x26BE */ u16  unk_26BE;
            /* 0x26C0 */ u16  unk_26C0;
            /* 0x26C2 */ u16  unk_26C2;
            /* 0x26C4 */ u16  unk_26C4;
            /* 0x26C6 */ u16  unk_26C6;
            /* 0x26C8 */ u16  unk_26C8;
            /* 0x26CA */ u16  unk_26CA;
            /* 0x26CC */ u16  unk_26CC;
            /* 0x26CE */ u16  unk_26CE;
            /* 0x26D0 */ u16  unk_26D0;
            /* 0x26D2 */ u16  unk_26D2;
            /* 0x26D4 */ u16  unk_26D4;
            /* 0x26D6 */ u16  unk_26D6;
            /* 0x26D8 */ u16  unk_26D8;
            /* 0x26DA */ u16  unk_26DA;
            /* 0x26DC */ u16  unk_26DC;
        };
    };
    /* 0x26DE */ char                unk_26DE[0x26E0 - 0x26DE];
    /* 0x26E0 */ s32                 unk_26E0;
    /* 0x26E4 */ s16                 unk_26E4;
    /* 0x26E6 */ s16                 unk_26E6;
    /* 0x26E8 */ s16                 unk_26E8;
    /* 0x26EA */ char                unk_26EA[0x26EC - 0x26EA];
    /* 0x26EC */ s32                 unk_26EC;
    /* 0x26F0 */ s32                 unk_26F0;
    /* 0x26F4 */ char                unk_26F4[0x26F8 - 0x26F4];
    /* 0x26F8 */ ScanSymbolRecord    scanSymbols[128];
    /* 0x30F8 */ s16                 scanSymbolCount;
    /* 0x30FA */ s16                 scanSymbolSelection;
    /* 0x30FC */ s16                 unk_30FC;
    /* 0x30FE */ s16                 unk_30FE;
    /* 0x3100 */ s16                 unk_3100;
    /* 0x3102 */ s16                 unk_3102;
    /* 0x3104 */ s16                 noiseQuotaStep;
    /* 0x3106 */ u16                 unk_3106;
    /* 0x3108 */ s16                 unk_3108;
    /* 0x310A */ char                unk_310A[0x310C - 0x310A];
    /* 0x310C */ void*               unk_310C;
    /* 0x3110 */ s16                 noiseSymbolCount;
    /* 0x3112 */ s16                 encounterCursors[6]; // encounter cursors, a pair per FieldEncounter range table
    /* 0x311E */ s16                 unk_311E;
    /* 0x3120 */ s16                 unk_3120;
    /* 0x3122 */ char                unk_3122[0x3124 - 0x3122];
    /* 0x3124 */ s32                 flashSequence;
    /* 0x3128 */ s16                 flashExitMode;
    /* 0x312A */ char                unk_312A[0x312C - 0x312A];
    /* 0x312C */ s32                 unk_312C;
    /* 0x3130 */ s32                 unk_3130;
    /* 0x3134 */ s32                 unk_3134;
    /* 0x3138 */ s32                 unk_3138;
    /* 0x313C */ s32                 unk_313C;
    /* 0x3140 */ u8                  unk_3140;
    /* 0x3141 */ u8                  battleDifficulty;
    /* 0x3142 */ u16                 unk_3142;
    /* 0x3144 */ s16                 battleCount;
    /* 0x3146 */ u16                 battleEncounters[16]; // one per chained battle, indexed by battleCount
    /* 0x3166 */ u16                 battleDropRate;       // drop-rate snapshot taken with the battle request
    /* 0x3168 */ s16                 battleTimer;          // frames spent in the current chained battle
    /* 0x316A */ s16                 battleLevel;          // current level during the battle
    /* 0x316C */ s16                 battleMaxLevel;       // highest level reached during the battle
    /* 0x316E */ u16                 unk_316E;
    /* 0x3170 */ s32                 battleExpToNextLevel;
    /* 0x3174 */ s16                 battleHpGauge; // level-up gauge carried into the next chained battle
    /* 0x3176 */ s16                 battleHpGaugeSub0;
    /* 0x3178 */ s16                 battleHpGaugeSub1;
    /* 0x317A */ s8                  battleChargeProgress;
    /* 0x317B */ s8                  battleChargeStage;
    /* 0x317C */ s8                  unk_317C; // longest combo of the battle
    /* 0x317D */ u8                  unk_317D;
    /* 0x317E */ s16                 unk_317E; // highest damage dealt in the battle
    /* 0x3180 */ u8                  battlePartner;
    /* 0x3181 */ u8                  unk_3181;
    /* 0x3182 */ s16                 battleBasePP;
    /* 0x3184 */ u32                 battleParTime;
    /* 0x3188 */ u32                 battleClearTime;
    /* 0x318C */ s16                 battleMaxHits;
    /* 0x318E */ u8                  unk_318E;
    /* 0x318F */ u8                  unk_318F;
    /* 0x3190 */ u32                 battleBonusFlags;
    /* 0x3194 */ UnkItemCount        battleDrops[32];
    /* 0x3214 */ char                unk_3214[0x3232 - 0x3214];
    /* 0x3232 */ u16                 unk_3232;
    /* 0x3234 */ s32                 unk_3234;
    /* 0x3238 */ u32                 unk_3238;
    /* 0x323C */ char                unk_323C[0x323E - 0x323C];
    /* 0x323E */ Ov003TouchSlotTable battleTouchSlots;
    /* 0x326A */ UnkSmallStruct      unk_326A[96];            // battle enemy-record snapshot restored into unk_1FCC
    /* 0x33EA */ char                unk_33EA[0x33EC - 0x33EA];
    /* 0x33EC */ u8                  otosuParentBssid[6];     // Tin Pin Slammer: the wireless game to rejoin
    /* 0x33F2 */ u8                  otosuPlayerBssids[4][6]; // Tin Pin Slammer: every player's console, in player order
    /* 0x340A */ u8                  otosuGameKey; // Tin Pin Slammer: 0 opens the wireless menu, else the game's beacon key
    /* 0x340B */ u8                  otosuPlayerCount; // Tin Pin Slammer
    /* 0x340C */ u8                  otosuBoard;       // Tin Pin Slammer
    /* 0x340D */ char                unk_340D[0x3414 - 0x340D];
    /* 0x3414 */ u16                 otosuScores[4];   // Tin Pin Slammer: each player's last round score
    /* 0x341C */ u8                  unk_341C;         // Tin Pin Slammer: the single-player match the story sets up
} MainData;                                            // Size: 0x3420

#endif                                                 // SAVE_MAINDATA_H
