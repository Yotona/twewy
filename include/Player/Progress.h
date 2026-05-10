#ifndef PLAYER_PROGRESS_H
#define PLAYER_PROGRESS_H

#include "Engine/EasyTask.h"
#include "Player/Progress/ProgressFlags.h"
#include <types.h>

typedef struct ProgressObject {
    /* 0x00000 */ char     unk_00000[0x11584];
    /* 0x11584 */ TaskPool taskPool;
    /* 0x11604 */ char     unk_11604[0x21614 - 0x11604];
    /* 0x21614 */ s32      unk_21614;
    /* 0x21618 */ char     unk_21618[0x21630 - 0x21618];
    /* 0x21630 */ s32      unk_21630;
    /* 0x21634 */ char     unk_21634[0x216B0 - 0x21634];
    /* 0x216B0 */ s32      unk_216B0;
    /* 0x216B4 */ s32      unk_216B4;
    /* 0x216B8 */ s32      unk_216B8;
    /* 0x216BC */ s32      unk_216BC;
    /* 0x216C0 */ u32      unk_216C0;
    /* 0x216C4 */ s32      unk_216C4;
    /* 0x216C8 */ s32      unk_216C8;
    /* 0x216CC */ u32      unk_216CC;
    /* 0x216D0 */ char     unk_216D0[0x216D4 - 0x216D0];
    /* 0x216D4 */ s32      unk_216D4;
    /* 0x216D8 */ char     unk_216D8[0x216DC - 0x216D8];
    /* 0x216DC */ s32      unk_216DC;
    /* 0x216E0 */ char     unk_216E0[0x21704 - 0x216E0];
    /* 0x21704 */ s32      unk_21704;
    /* 0x21708 */ s32      unk_21708;
    /* 0x2170C */ s32      unk_2170C;
    /* 0x21710 */ s32      unk_21710;
    /* 0x21714 */ s32      unk_21714;
    /* 0x21718 */ s16*     unk_21718;
    /* 0x2171C */ char     unk_2171C[0x21728 - 0x2171C];
    /* 0x21728 */ s16*     unk_21728;
    /* 0x2172C */ char     unk_2172C[0x21738 - 0x2172C];
    /* 0x21738 */ s16*     unk_21738;
    /* 0x2173C */ char     unk_2173C[0x21748 - 0x2173C];
    /* 0x21748 */ u16*     unk_21748;
    /* 0x2174C */ char     unk_2174C[0x21758 - 0x2174C];
    /* 0x21758 */ u16      unk_21758;
    /* 0x2175A */ u16      unk_2175A[2];
    /* 0x2175E */ char     unk_2175E[0x2176A - 0x2175E];
    /* 0x2176A */ u16      unk_2176A;
    /* 0x2176C */ u16      unk_2176C[2];
    /* 0x21770 */ char     unk_21770[0x2177C - 0x21770];
    /* 0x2177C */ s16      unk_2177C;
    /* 0x2177E */ char     unk_2177E[0x21780 - 0x2177E];
    /* 0x21780 */ s32      unk_21780;
    /* 0x21784 */ s32      unk_21784;
    /* 0x21788 */ u32      unk_21788[2];
    /* 0x21790 */ char     unk_21790[0x21918 - 0x21790];
    /* 0x21918 */ s32      unk_21918;
    /* 0x2191C */ char     unk_2191C[0x21A24 - 0x2191C];
    /* 0x21A24 */ s32      unk_21A24;
    /* 0x21A28 */ char     unk_21A28[0x21A5C - 0x21A28];
    /* 0x21A5C */ s32      unk_21A5C;
    /* 0x21A60 */ char     unk_21A60[0x21AAE - 0x21A60];
    /* 0x21AAE */ s16      unk_21AAE;
    /* 0x21AB0 */ s32      unk_21AB0;
    /* 0x21AB4 */ s32      unk_21AB4;
    /* 0x21AB8 */ char     unk_21AB8[0x21AC4 - 0x21AB8];
    /* 0x21AC4 */ s32      unk_21AC4;
    /* 0x21AC8 */ char     unk_21AC8[0x21ACC - 0x21AC8];
    /* 0x21ACC */ s32      unk_21ACC;
    /* 0x21AD0 */ s32      unk_21AD0;
    /* 0x21AD4 */ char     unk_21AD4[0x21AD8 - 0x21AD4];
    /* 0x21AD8 */ s32      unk_21AD8;
    /* 0x21ADC */ s32      unk_21ADC;
    /* 0x21AE0 */ s32      unk_21AE0;
    /* 0x21AE4 */ char     unk_21AE4[0x21AF0 - 0x21AE4];
    /* 0x21AF0 */ s32      unk_21AF0;
    /* 0x21AF4 */ s32      unk_21AF4;
    /* 0x21AF8 */ char     unk_21AF8[0x21AFC - 0x21AF8];
    /* 0x21AFC */ s16      unk_21AFC;
    /* 0x21AFE */ char     unk_21AFE[0x21B00 - 0x21AFE];
    /* 0x21B00 */ s32      unk_21B00;
    /* 0x21B04 */ char     unk_21B04[0x21B08 - 0x21B04];
    /* 0x21B08 */ s32      unk_21B08;
    /* 0x21B0C */ s32      unk_21B0C;
    /* 0x21B10 */ char     unk_21B10[0x21BBC - 0x21B10];
    /* 0x21BBC */ u32      unk_21BBC[35];
    /* 0x21C48 */ u32      unk_21C48[32];
    /* 0x21CC8 */ char     unk_21CC8[0x21CD0 - 0x21CC8];
    /* 0x21CD0 */ u32*     unk_21CD0;
    /* 0x21CD4 */ s32      unk_21CD4;
    /* 0x21CD8 */ s32      unk_21CD8;
    /* 0x21CDC */ s32      unk_21CDC;
    /* 0x21CE0 */ s32      unk_21CE0;
    /* 0x21CE4 */ s32      unk_21CE4;
    /* 0x21CE8 */ u16      unk_21CE8;
    /* 0x21CEA */ u16      unk_21CEA;
    /* 0x21CEC */ s32      unk_21CEC;
    /* 0x21CF0 */ s32      unk_21CF0;
    /* 0x21CF4 */ s32      unk_21CF4;
    /* 0x21CF8 */ s32      unk_21CF8;
    /* 0x21CFC */ s32      unk_21CFC;
    /* 0x21D00 */ s32      unk_21D00;
    /* 0x21D04 */ s16      unk_21D04;
    /* 0x21D06 */ s16      unk_21D06;
    /* 0x21D08 */ void (*unk_21D08)(void*);
    /* 0x21D0C */ s32 unk_21D0C;
    /* 0x21D10 */ s16 targetBrightness; // Target fade brightness
    /* 0x21D12 */ s16 unk_21D12;
    /* 0x21D14 */ s32 unk_21D14;
    /* 0x21D18 */ s32 unk_21D18;
    /* 0x21D1C */ s16 unk_21D1C;
    /* 0x21D1E */ s16 unk_21D1E;
    /* 0x21D20 */ s16 unk_21D20;
    /* 0x21D22 */ s16 unk_21D22;
    /* 0x21D24 */ s32 (*unk_21D24)(struct ProgressObject*);
    /* 0x21D28 */ s16  unk_21D28;
    /* 0x21D2A */ s16  unk_21D2A;
    /* 0x21D2C */ char unk_21D2C[0x21D30 - 0x21D2C];
    /* 0x21D30 */ s32  unk_21D30;
    /* 0x21D34 */ char unk_21D34[0x22E2C - 0x21D34];
    /* 0x22E2C */ s32  unk_22E2C;
} ProgressObject;

typedef s32 (*ProgressRewardFunc)(ProgressObject*);
extern const ProgressRewardFunc data_ov030_020da618[44];

typedef struct {
    /* 0x0 */ u8 completedStars[4];
} SecretReport;

typedef enum {

    EVENT_SHIKI6_SECRET_BOX_CENTER_ST = 269,            // Shiki, Day 6, Secret Box at Center Street

    EVENT_SHIKI7_SECRET_BOX_MOLCO = 286,                // Shiki, Day 7, Secret Box at Molco

    EVENT_JOSHUA1_SECRET_BOX_HACHIKO = 306,             // Joshua, Day 1, Secret Box at Statue of Hachiko

    EVENT_JOSHUA2_TIN_PIN = 338,                        // Joshua, Day 2, Complete the Tin Pin event

    EVENT_JOSHUA2_SECRET_BOX_SPAIN_HILL = 342,          // Joshua, Day 2, Secret Box at Spain Hill

    EVENT_JOSHUA3_IMPRINT_YELLOW = 386,                 // Joshua, Day 3, Imprint "Yellow" on Ken Doi

    EVENT_JOSHUA3_SECRET_BOX_CONCERT_STAGE = 403,       // Joshua, Day 3, Secret Box at Concert Stage

    EVENT_JOSHUA4_SECRET_BOX_CAT_STREET = 478,          // Joshua, Day 4, Secret Box at Cat Street

    EVENT_JOSHUA5_SECRET_BOX_MIYASHITA_UNDERPASS = 508, // Joshua, Day 5, Secret Box at Miyashita Park Underpass

    EVENT_JOSHUA6_SECRET_BOX_SHIBU_Q_HEADS = 547,       // Joshua, Day 6, Secret Box at Shibu-Q Heads

    EVENT_JOSHUA7_SECRET_BOX_CONCERT_STAGE = 567,       // Joshua, Day 7, Secret Box at Concert Stage

    EVENT_BEAT1_SECRET_BOX_HACHIKO = 593,               // Beat, Day 1, Secret Box at Statue of Hachiko

    EVENT_BEAT2_SECRET_BOX_DOGENZAKA = 614,             // Beat, Day 2, Secret Box at Dogenzaka

    EVENT_BEAT3_TIPSY_TOSE_HALL_REAPER_CREEPER = 634,   // Beat, Day 3, Tispy Tose Hall, opt out of Reaper Creeper

    EVENT_BEAT3_TIPSY_TOSE_HALL = 639,                  // Beat, Day 3, Tispy Tose Hall

    EVENT_BEAT3_SECRET_BOX_TISPY_TOSE_HALL = 648,       // Beat, Day 3, Secret Box at Tispy Tose Hall

    EVENT_BEAT4_SECRET_BOX_MIYASHITA_PARK = 702,        // Beat, Day 4, Secret Box at Miyashita Park

    EVENT_BEAT5_SECRET_BOX_SHIBU_Q_HEADS = 726,         // Beat, Day 5, Secret Box at Shibu-Q Heads

    EVENT_BEAT6_SECRET_BOX_DOGENZAKA = 744,             // Beat, Day 6, Secret Box at Dogenzaka

    EVENT_BEAT7_TRAIL_OF_THE_BYGONE = 762,              // Beat, Day 7, Trail of the Bygone, Inspect the trash heap

    EVENT_BEAT7_SECRET_BOX_DOGENZAKA = 785,             // Beat, Day 7, Secret Box at Dogenzaka

    EVENT_ANOTHERDAY_WIZARD_OF_SLAM = 852,              // Another Day, Molco, Defeat the Wizard of Slam

    EVENT_ANOTHERDAY_PORKCITY_ROOF_LEVEL = 863,         // Another Day, Pork City Roof Level, Eliminate Panthera Cantus

    EVENT_ANOTHERDAY_FINALTIMEATTACK_REWARD = 873,      // Another Day, Final Time Attack, Reward for first time completion

    EVENT_ANOTHERDAY_SECRET_BOX_CONCERT_STAGE = 876,    // Another Day, Secret Box at Concert Stage

    EVENT_JOSHUA1_104 = 1277,                           // Joshua, Day 1, 104 Building

    EVENT_BEAT1_HACHIKO = 1346,                         // Beat, Day 1, Hachiko

    EVENT_BEAT2_CONCERT_STAGE = 1358,                   // Beat, Day 2, Concert Stage

    EVENT_BEAT1_TIPSY_TOSE_HALL = 1386,                 // Beat, Day 1, Tispy Tose Hall

    EVENT_SHIKI7_104_REAPER = 1973,                     // Shiki, Day 7, Reaper to 104, own Funky Shades

    EVENT_SHIKI7_SCRAMBLE_REAPER = 1983,                // Shiki, Day 7, Scramble <> Center St reaper

    EVENT_SHIKI7_SCRAMBLE_MAGNUM = 1990,                // Shiki, Day 7, Scramble reaper, give Natural Magnum

    EVENT_JOSHUA2_SCRAMBLE_NP_JOSHUA = 2005,            // Joshua, Day 2, Scramble <> 104 reaper, Joshua in all Natural Puppy

    EVENT_JOSHUA2_MOLCO_REAPER = 2008,                  // Joshua, Day 2, Molco <> Spain Hill reaper, give Definitivo Chili Dog

    EVENT_JOSHUA3_SCRAMBLE_NP_TREND = 2011,             // Joshua, Day 2, Scramble <> 104 reaper, #1 trend Natural Puppy

    EVENT_JOSHUA3_SCRAMBLE_REAPER = 2032,               // Joshua, Day 3, Scramble <> Hachiko reaper

    EVENT_JOSHUA4_HACHIKO_REAPER = 2042,                // Joshua, Day 4, Defeat 10 Noise symbols for reaper in Hachiko area

    EVENT_ANOTHERDAY_PORKCITY_1ST_REWARD = 2108,        // Another Day, Pork City 1st Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_2ND_REWARD = 2111,        // Another Day, Pork City 2nd Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_3RD_REWARD = 2114,        // Another Day, Pork City 3rd Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_4TH_REWARD = 2117,        // Another Day, Pork City 4th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_5TH_REWARD = 2120,        // Another Day, Pork City 5th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_6TH_REWARD = 2123,        // Another Day, Pork City 6th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_7TH_REWARD = 2126,        // Another Day, Pork City 7th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_8TH_REWARD = 2129,        // Another Day, Pork City 8th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_9TH_REWARD = 2132,        // Another Day, Pork City 9th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_10TH_REWARD = 2135,       // Another Day, Pork City 10th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_11TH_REWARD = 2138,       // Another Day, Pork City 11th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_12TH_REWARD = 2141,       // Another Day, Pork City 12th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_13TH_REWARD = 2144,       // Another Day, Pork City 13th Floor Reward
} ProgressEvent;

void Progress_SetCurrentEvent(ProgressEvent event);

void Progress_AdvanceEventScript(ProgressObject* progress);

void Progress_InitState(ProgressObject* progress);

void ProgressReward_GrantItem(ProgressObject* arg0, u16 itemID);

void ProgressReward_GrantPin(ProgressObject* arg0, u16 itemID);

void Progress_BeginScriptedEvent(ProgressObject* progress, ProgressEvent event, s32 arg2);

void func_ov030_020aac28(ProgressObject* arg0, u16 itemID);

void func_ov030_020aec38(s32 arg0);

s32 func_ov030_020aeca4(s32 arg0);

#endif // PLAYER_PROGRESS_H
