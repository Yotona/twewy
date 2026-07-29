#ifndef FIELD_EVENTS_H
#define FIELD_EVENTS_H

#include "Engine/EasyTask.h"
#include "Field/EventFlags.h"
#include <nitro/types.h>

typedef struct FieldEventManager {
    /* 0x00000 */ char     unk_00000[0x11584];
    /* 0x11584 */ TaskPool taskPool;
    /* 0x11604 */ char     unk_11604[0x21614 - 0x11604];
    /* 0x21614 */ s32      unk_21614;
    /* 0x21618 */ char     unk_21618[0x21630 - 0x21618];
    /* 0x21630 */ s32      unk_21630;
    /* 0x21634 */ char     unk_21634[0x216B0 - 0x21634];
    /* 0x216B0 */ s32      unk_216B0;
    /* 0x216B4 */ s32      unk_216B4;
    /* 0x216B8 */ s32      taskId_KeywordMenu;
    /* 0x216BC */ s32      unk_216BC;
    /* 0x216C0 */ u32      unk_216C0;
    /* 0x216C4 */ s32      unk_216C4;
    /* 0x216C8 */ s32      unk_216C8;
    /* 0x216CC */ u32      unk_216CC;
    /* 0x216D0 */ char     unk_216D0[0x216D4 - 0x216D0];
    /* 0x216D4 */ s32      unk_216D4;
    /* 0x216D8 */ char     unk_216D8[0x216DC - 0x216D8];
    /* 0x216DC */ s32      taskId_UGFade;
    /* 0x216E0 */ char     unk_216E0[0x21704 - 0x216E0];
    /* 0x21704 */ s32      unk_21704;
    /* 0x21708 */ s32      taskId_Mess;  // Task ID for message
    /* 0x2170C */ s32      taskId_Plate; // Task ID for string plate
    /* 0x21710 */ s32      taskId_Item;  // Task ID for items
    /* 0x21714 */ s32      taskId_Arrow; // Task ID for which arrow
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
    /* 0x21780 */ u32      taskId_Choice;
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
    /* 0x21AF4 */ s16      unk_21AF4;
    /* 0x21AF6 */ char     unk_21AF6[0x21AFC - 0x21AF6];
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
    /* 0x21D24 */ s32 (*unk_21D24)(struct FieldEventManager*);
    /* 0x21D28 */ s16  unk_21D28;
    /* 0x21D2A */ s16  unk_21D2A;
    /* 0x21D2C */ char unk_21D2C[0x21D30 - 0x21D2C];
    /* 0x21D30 */ s32  unk_21D30;
    /* 0x21D34 */ char unk_21D34[0x22E2C - 0x21D34];
    /* 0x22E2C */ s32  unk_22E2C;
} FieldEventManager;

typedef s32 (*FieldEventRewardFunc)(FieldEventManager*);
extern const FieldEventRewardFunc data_ov030_020da618[44];

typedef struct {
    /* 0x0 */ u8 completedStars[4];
} SecretReport;

/// @brief Field event identifier.
/// @note This decompilation intentionally follows a "more readable" naming convention than the original binary appeared to use
typedef enum {

    // Shiki, Day 1
    EVENT_SHIKI1_BEGIN_GAME          = 86,  // Shiki, Day 1, Begin the game
    EVENT_SHIKI1_USE_PLAYER_PIN      = 87,  // Shiki, Day 1, Use the Player Pin for the first time
    EVENT_SHIKI1_READ_MISSION_TEXT   = 88,  // Shiki, Day 1, Read the mission text
    EVENT_SHIKI1_ESCAPE_FROGS        = 89,  // Shiki, Day 1, Escape the solo fight against some frogs
    EVENT_SHIKI1_KARIYA_UZUKI_DIALOG = 90,  // Shiki, Day 1, Read the dialogue between Kariya and Uzuki
    EVENT_SHIKI1_NOISE_BY_HACHIKO    = 91,  // Shiki, Day 1, Watch the noise erase players by Statue of Hachiko
    EVENT_SHIKI1_HACHIKO_FROGS       = 92,  // Shiki, Day 1, Watch Neku be alone and attacked by frogs again
    EVENT_SHIKI1_PACT_WITH_SHIKI     = 93,  // Shiki, Day 1, Make a pact with Shiki
    EVENT_SHIKI1_BEAT_HACHIKO_FROGS  = 94,  // Shiki, Day 1, Beat the frogs at Statue of Hachiko
    EVENT_SHIKI1_BACK_IN_CROSSING    = 95,  // Shiki, Day 1, Neku leaves the area and returns to Scramble Crossing
    EVENT_SHIKI1_UNKNOWN_96          = 96,  // Shiki, Day 1, Unknown event
    EVENT_SHIKI1_RUN_INTO_WALL       = 97,  // Shiki, Day 1, Run into the invisible wall at Scramble Crossing
    EVENT_SHIKI1_GET_TO_104          = 98,  // Shiki, Day 1, Get to the 104 Building
    EVENT_SHIKI1_BEAT_104_FROGS      = 99,  // Shiki, Day 1, Beat the frogs a reaper summons at the 104 Building
    EVENT_SHIKI1_BEAT_GRIZZLY        = 100, // Shiki, Day 1, Beat the Mosh Grizzly
    EVENT_SHIKI1_DAY_END             = 101, // Shiki, Day 1, End the day
    EVENT_SHIKI1_SECRET_BOX_HACHIKO  = 102, // Shiki, Day 1, Secret Box at Statue of Hachiko

    // Shiki, Day 2

    // Shiki, Day 3

    // Shiki, Day 4

    // Shiki, Day 5

    // Shiki, Day 6

    EVENT_SHIKI6_SECRET_BOX_CENTER_ST = 269, // Shiki, Day 6, Secret Box at Center Street

    // Shiki, Day 7

    EVENT_SHIKI7_SECRET_BOX_MOLCO = 286, // Shiki, Day 7, Secret Box at Molco

    // Joshua, Day 1

    EVENT_JOSHUA1_SECRET_BOX_HACHIKO = 306, // Joshua, Day 1, Secret Box at Statue of Hachiko

    // Joshua, Day 2

    EVENT_JOSHUA2_TIN_PIN = 338,               // Joshua, Day 2, Complete the Tin Pin event

    EVENT_JOSHUA2_SECRET_BOX_SPAIN_HILL = 342, // Joshua, Day 2, Secret Box at Spain Hill

    // Joshua, Day 3

    EVENT_JOSHUA3_IMPRINT_YELLOW = 386,           // Joshua, Day 3, Imprint "Yellow" on Ken Doi

    EVENT_JOSHUA3_SECRET_BOX_CONCERT_STAGE = 403, // Joshua, Day 3, Secret Box at Concert Stage

    // Joshua, Day 4

    EVENT_JOSHUA4_SECRET_BOX_CAT_STREET = 478, // Joshua, Day 4, Secret Box at Cat Street

    // Joshua, Day 5

    EVENT_JOSHUA5_SECRET_BOX_MIYASHITA_UNDERPASS = 508, // Joshua, Day 5, Secret Box at Miyashita Park Underpass

    // Joshua, Day 6

    EVENT_JOSHUA6_SECRET_BOX_SHIBU_Q_HEADS = 547, // Joshua, Day 6, Secret Box at Shibu-Q Heads

    // Joshua, Day 7

    EVENT_JOSHUA7_SECRET_BOX_CONCERT_STAGE = 567, // Joshua, Day 7, Secret Box at Concert Stage

    // Beat, Day 1

    EVENT_BEAT1_SECRET_BOX_HACHIKO = 593, // Beat, Day 1, Secret Box at Statue of Hachiko

    // Beat, Day 2

    EVENT_BEAT2_SECRET_BOX_DOGENZAKA = 614, // Beat, Day 2, Secret Box at Dogenzaka

    // Beat, Day 3

    EVENT_BEAT3_TIPSY_TOSE_HALL_REAPER_CREEPER = 634, // Beat, Day 3, Tispy Tose Hall, opt out of Reaper Creeper

    EVENT_BEAT3_TIPSY_TOSE_HALL = 639,                // Beat, Day 3, Tispy Tose Hall

    EVENT_BEAT3_SECRET_BOX_TISPY_TOSE_HALL = 648,     // Beat, Day 3, Secret Box at Tispy Tose Hall

    // Beat, Day 4

    EVENT_BEAT4_SECRET_BOX_MIYASHITA_PARK = 702, // Beat, Day 4, Secret Box at Miyashita Park

    // Beat, Day 5

    EVENT_BEAT5_SECRET_BOX_SHIBU_Q_HEADS = 726, // Beat, Day 5, Secret Box at Shibu-Q Heads

    // Beat, Day 6

    EVENT_BEAT6_SECRET_BOX_DOGENZAKA = 744, // Beat, Day 6, Secret Box at Dogenzaka

    // Beat, Day 7

    EVENT_BEAT7_TRAIL_OF_THE_BYGONE = 762,  // Beat, Day 7, Trail of the Bygone, Inspect the trash heap

    EVENT_BEAT7_SECRET_BOX_DOGENZAKA = 785, // Beat, Day 7, Secret Box at Dogenzaka

    // Another Day

    EVENT_ANOTHERDAY_WIZARD_OF_SLAM = 852,           // Another Day, Molco, Defeat the Wizard of Slam

    EVENT_ANOTHERDAY_PORKCITY_ROOF_LEVEL = 863,      // Another Day, Pork City Roof Level, Eliminate Panthera Cantus

    EVENT_ANOTHERDAY_FINALTIMEATTACK_REWARD = 873,   // Another Day, Final Time Attack, Reward for first time completion

    EVENT_ANOTHERDAY_SECRET_BOX_CONCERT_STAGE = 876, // Another Day, Secret Box at Concert Stage

    // Rewards
    EVENT_SHIKI1_DRAG_AND_NEKU_COMBAT_TUTORIAL = 1206, // Shiki, Day 1, Read the Drag and Neku Combat Tutorial
    EVENT_SHIKI1_PARTNER_TUTORIAL              = 1207, // Shiki, Day 1, Read the Partner Tutorial
    EVENT_SHIKI1_104_FROG_REWARD               = 1208, // Shiki, Day 1, Beat the frogs a reaper summons at 104 Building
    EVENT_SHIKI1_GRIZZLY_BATTLE_REWARD         = 1209, // Shiki, Day 1, Win the Grizzly Battle
    EVENT_SHIKI1_RUN_DASH_ESCAPE_TUTORIAL      = 1210, // Shiki, Day 1, Run Dash & Escape Tutorial

    EVENT_JOSHUA1_104 = 1277,                          // Joshua, Day 1, 104 Building

    EVENT_BEAT1_HACHIKO = 1346,                        // Beat, Day 1, Hachiko

    EVENT_BEAT2_CONCERT_STAGE = 1358,                  // Beat, Day 2, Concert Stage

    EVENT_BEAT1_TIPSY_TOSE_HALL = 1386,                // Beat, Day 1, Tispy Tose Hall

    EVENT_SHIKI7_104_REAPER = 1973,                    // Shiki, Day 7, Reaper to 104, own Funky Shades

    EVENT_SHIKI7_SCRAMBLE_REAPER = 1983,               // Shiki, Day 7, Scramble <> Center St reaper

    EVENT_SHIKI7_SCRAMBLE_MAGNUM = 1990,               // Shiki, Day 7, Scramble reaper, give Natural Magnum

    EVENT_JOSHUA2_SCRAMBLE_NP_JOSHUA = 2005,           // Joshua, Day 2, Scramble <> 104 reaper, Joshua in all Natural Puppy

    EVENT_JOSHUA2_MOLCO_REAPER = 2008,                 // Joshua, Day 2, Molco <> Spain Hill reaper, give Definitivo Chili Dog

    EVENT_JOSHUA3_SCRAMBLE_NP_TREND = 2011,            // Joshua, Day 2, Scramble <> 104 reaper, #1 trend Natural Puppy

    EVENT_JOSHUA3_SCRAMBLE_REAPER = 2032,              // Joshua, Day 3, Scramble <> Hachiko reaper

    EVENT_JOSHUA4_HACHIKO_REAPER = 2042,               // Joshua, Day 4, Defeat 10 Noise symbols for reaper in Hachiko area

    EVENT_ANOTHERDAY_PORKCITY_1ST_REWARD = 2108,       // Another Day, Pork City 1st Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_2ND_REWARD = 2111,       // Another Day, Pork City 2nd Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_3RD_REWARD = 2114,       // Another Day, Pork City 3rd Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_4TH_REWARD = 2117,       // Another Day, Pork City 4th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_5TH_REWARD = 2120,       // Another Day, Pork City 5th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_6TH_REWARD = 2123,       // Another Day, Pork City 6th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_7TH_REWARD = 2126,       // Another Day, Pork City 7th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_8TH_REWARD = 2129,       // Another Day, Pork City 8th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_9TH_REWARD = 2132,       // Another Day, Pork City 9th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_10TH_REWARD = 2135,      // Another Day, Pork City 10th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_11TH_REWARD = 2138,      // Another Day, Pork City 11th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_12TH_REWARD = 2141,      // Another Day, Pork City 12th Floor Reward

    EVENT_ANOTHERDAY_PORKCITY_13TH_REWARD = 2144,      // Another Day, Pork City 13th Floor Reward

    // Optional flags that can be combined with story events to trigger specific behaviors
    EVENTFLAG_TALK  = 0x2000, // TODO: Assumed from Solo Remix. View a conversation?
    EVENTFLAG_SHOP  = 0x4000, // TODO: Assumed from Solo Remix. View a store?
    EVENTFLAG_NOBGM = 0x8000, // TODO: Assumed from Solo Remix. Silence the background music?
} FieldEvent;

void FldEvent_SetCurrent(FieldEvent event);

void FldEvent_AdvanceScript(FieldEventManager* eventMgr);

void FldEvent_InitState(FieldEventManager* eventMgr);

void FldEvent_GrantItem(FieldEventManager* eventMgr, u16 itemID);

void FldEvent_GrantPin(FieldEventManager* eventMgr, u16 itemID);

void FldEvent_BeginScript(FieldEventManager* eventMgr, FieldEvent event, s32 arg2);

void func_ov030_020aac28(FieldEventManager* eventMgr, u16 itemID);

void func_ov030_020aec38(s32 arg0);

s32 func_ov030_020aeca4(s32 arg0);

#endif // FIELD_EVENTS_H
