#ifndef FIELD_EVENTS_H
#define FIELD_EVENTS_H

#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Field/EventFlags.h"
#include "Field/FieldTypes.h"
#include <nitro/types.h>

// A map exit: an entry of an area's exit table (FieldArea). The field
// keeps the exit being taken in FieldEventManager::unk_21A58.
typedef struct FieldExit {
    /* 0x0 */ s32 region;     // trigger region kind, see func_ov030_020b7e94
    /* 0x4 */ s16 destArea;   // area the exit leads to, 0x46 = none
    /* 0x6 */ s16 unk_06;
    /* 0x8 */ s32 destRegion; // region to arrive at; 0x66 = FieldEventManager::unk_21B08/unk_21B0C
    /* 0xC */ s32 exitId;     // MainData::exitStates index
} FieldExit;                  // Size: 0x10

typedef struct FieldEventManager {
    /* 0x00000 */ ResourceManager         resources;
    /* 0x11580 */ char                    unk_11580[0x11584 - 0x11580];
    /* 0x11584 */ TaskPool                taskPool;
    /* 0x11604 */ char                    unk_11604[0x21610 - 0x11604];
    /* 0x21610 */ s32                     unk_21610;
    /* 0x21614 */ s32                     unk_21614;
    /* 0x21618 */ s32                     unk_21618;
    /* 0x2161C */ char                    unk_2161C[0x21620 - 0x2161C];
    /* 0x21620 */ struct Data*            unk_21620;
    /* 0x21624 */ FieldPos                origin; // camera origin on the field map
    /* 0x2162C */ s32                     unk_2162C;
    /* 0x21630 */ s32                     unk_21630;
    /* 0x21634 */ char                    unk_21634[0x216B0 - 0x21634];
    /* 0x216B0 */ s32                     unk_216B0;
    /* 0x216B4 */ s32                     unk_216B4;
    /* 0x216B8 */ s32                     taskId_KeywordMenu;
    /* 0x216BC */ s32                     unk_216BC;
    /* 0x216C0 */ u32                     unk_216C0;
    /* 0x216C4 */ s32                     unk_216C4;
    /* 0x216C8 */ s32                     unk_216C8;
    /* 0x216CC */ u32                     unk_216CC;
    /* 0x216D0 */ s32                     unk_216D0;
    /* 0x216D4 */ s32                     unk_216D4;
    /* 0x216D8 */ s32                     unk_216D8;
    /* 0x216DC */ s32                     taskId_UGFade;
    /* 0x216E0 */ s32                     unk_216E0;
    /* 0x216E4 */ s32                     unk_216E4;
    /* 0x216E8 */ s32                     unk_216E8;
    /* 0x216EC */ s32                     unk_216EC;
    /* 0x216F0 */ s32                     unk_216F0;
    /* 0x216F4 */ s32                     unk_216F4;
    /* 0x216F8 */ s32                     unk_216F8;
    /* 0x216FC */ s32                     unk_216FC;
    /* 0x21700 */ s32                     unk_21700;
    /* 0x21704 */ s32                     unk_21704;
    /* 0x21708 */ s32                     taskId_Mess;  // Task ID for message
    /* 0x2170C */ s32                     taskId_Plate; // Task ID for string plate
    /* 0x21710 */ s32                     taskId_Item;  // Task ID for items
    /* 0x21714 */ s32                     taskId_Arrow; // Task ID for which arrow
    /* 0x21718 */ s16*                    unk_21718;
    /* 0x2171C */ char                    unk_2171C[0x21728 - 0x2171C];
    /* 0x21728 */ s16*                    unk_21728;
    /* 0x2172C */ char                    unk_2172C[0x21738 - 0x2172C];
    /* 0x21738 */ s16*                    unk_21738;
    /* 0x2173C */ char                    unk_2173C[0x21748 - 0x2173C];
    /* 0x21748 */ u16*                    unk_21748;
    /* 0x2174C */ char                    unk_2174C[0x21758 - 0x2174C];
    /* 0x21758 */ u16                     unk_21758;
    /* 0x2175A */ u16                     unk_2175A[2];
    /* 0x2175E */ char                    unk_2175E[0x2176A - 0x2175E];
    /* 0x2176A */ u16                     unk_2176A;
    /* 0x2176C */ u16                     unk_2176C[8];
    /* 0x2177C */ s16                     unk_2177C;
    /* 0x2177E */ char                    unk_2177E[0x21780 - 0x2177E];
    /* 0x21780 */ u32                     taskId_Choice;
    /* 0x21784 */ s32                     unk_21784;
    /* 0x21788 */ u32                     unk_21788[2];
    /* 0x21790 */ char                    unk_21790[0x21918 - 0x21790];
    /* 0x21918 */ s32                     unk_21918;
    /* 0x2191C */ s32                     unk_2191C;
    /* 0x21920 */ u16                     unk_21920;
    /* 0x21922 */ u16                     unk_21922;
    /* 0x21924 */ u32                     unk_21924[64];
    /* 0x21A24 */ s32                     unk_21A24;
    /* 0x21A28 */ struct Data*            unk_21A28;
    /* 0x21A2C */ void*                   unk_21A2C;
    /* 0x21A30 */ struct PaletteResource* unk_21A30;
    /* 0x21A34 */ s32                     unk_21A34[8];
    /* 0x21A54 */ s32                     unk_21A54;
    /* 0x21A58 */ FieldExit*              unk_21A58;
    /* 0x21A5C */ s32                     unk_21A5C;
    /* 0x21A60 */ struct BgResource*      unk_21A60;
    /* 0x21A64 */ struct BgResource*      unk_21A64;
    /* 0x21A68 */ struct PaletteResource* unk_21A68;
    /* 0x21A6C */ s32                     unk_21A6C;
    /* 0x21A70 */ struct BgResource*      unk_21A70;
    /* 0x21A74 */ struct PaletteResource* unk_21A74;
    /* 0x21A78 */ s32                     unk_21A78;
    /* 0x21A7C */ struct BgResource*      unk_21A7C;
    /* 0x21A80 */ struct PaletteResource* unk_21A80;
    /* 0x21A84 */ struct BgResource*      unk_21A84;
    /* 0x21A88 */ struct BgResource*      unk_21A88;
    /* 0x21A8C */ struct PaletteResource* unk_21A8C;
    /* 0x21A90 */ s32                     unk_21A90;
    /* 0x21A94 */ s32                     unk_21A94;
    /* 0x21A98 */ s32                     unk_21A98;
    /* 0x21A9C */ struct ObjResource*     unk_21A9C;
    /* 0x21AA0 */ u32                     unk_21AA0;
    /* 0x21AA4 */ u32                     unk_21AA4;
    /* 0x21AA8 */ u32                     unk_21AA8;
    /* 0x21AAC */ u16                     unk_21AAC;
    /* 0x21AAE */ u16                     unk_21AAE;
    /* 0x21AB0 */ s32                     unk_21AB0;
    /* 0x21AB4 */ s32                     unk_21AB4;
    /* 0x21AB8 */ s32                     unk_21AB8;
    /* 0x21ABC */ s32                     unk_21ABC;
    /* 0x21AC0 */ s32                     unk_21AC0;
    /* 0x21AC4 */ s32                     unk_21AC4;
    /* 0x21AC8 */ s32                     unk_21AC8;
    /* 0x21ACC */ s32                     unk_21ACC;
    /* 0x21AD0 */ s32                     unk_21AD0;
    /* 0x21AD4 */ s32                     unk_21AD4;
    /* 0x21AD8 */ s32                     unk_21AD8;
    /* 0x21ADC */ s32                     unk_21ADC;
    /* 0x21AE0 */ s32                     unk_21AE0;
    /* 0x21AE4 */ s32                     unk_21AE4;
    /* 0x21AE8 */ s32                     unk_21AE8;
    /* 0x21AEC */ u16                     unk_21AEC;
    /* 0x21AEE */ char                    unk_21AEE[0x21AF0 - 0x21AEE];
    /* 0x21AF0 */ s32                     unk_21AF0;
    /* 0x21AF4 */ s16                     unk_21AF4;
    /* 0x21AF6 */ char                    unk_21AF6[0x21AF8 - 0x21AF6];
    /* 0x21AF8 */ FieldExit               unk_21AF8; // scripted exit, see func_ov030_020ab554
    /* 0x21B08 */ s32                     unk_21B08;
    /* 0x21B0C */ s32                     unk_21B0C;
    /* 0x21B10 */ s32                     unk_21B10;
    /* 0x21B14 */ s32                     unk_21B14;
    /* 0x21B18 */ s32                     unk_21B18;
    /* 0x21B1C */ s32                     unk_21B1C;
    /* 0x21B20 */ s32                     unk_21B20;
    /* 0x21B24 */ s32                     unk_21B24;
    /* 0x21B28 */ s32                     unk_21B28;
    /* 0x21B2C */ s32                     unk_21B2C;
    /* 0x21B30 */ s32                     unk_21B30;
    /* 0x21B34 */ u16                     unk_21B34;
    /* 0x21B36 */ char                    unk_21B36[2];
    /* 0x21B38 */ s32                     unk_21B38;
    /* 0x21B3C */ s32                     unk_21B3C[32];
    /* 0x21BBC */ u32                     unk_21BBC[35];
    /* 0x21C48 */ u32                     unk_21C48[32];
    /* 0x21CC8 */ char                    unk_21CC8[0x21CD0 - 0x21CC8];
    /* 0x21CD0 */ u32*                    unk_21CD0;
    /* 0x21CD4 */ s32                     unk_21CD4;
    /* 0x21CD8 */ s32                     unk_21CD8;
    /* 0x21CDC */ s32                     unk_21CDC;
    /* 0x21CE0 */ s32                     unk_21CE0;
    /* 0x21CE4 */ s32                     unk_21CE4;
    /* 0x21CE8 */ u16                     unk_21CE8;
    /* 0x21CEA */ u16                     unk_21CEA;
    /* 0x21CEC */ s32                     unk_21CEC;
    /* 0x21CF0 */ s32                     unk_21CF0;
    /* 0x21CF4 */ s32                     unk_21CF4;
    /* 0x21CF8 */ s32                     unk_21CF8;
    /* 0x21CFC */ s32                     unk_21CFC;
    /* 0x21D00 */ s32                     unk_21D00;
    /* 0x21D04 */ s16                     unk_21D04;
    /* 0x21D06 */ s16                     unk_21D06;
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
    /* 0x21D28 */ s16          unk_21D28;
    /* 0x21D2A */ s16          unk_21D2A;
    /* 0x21D2C */ struct Data* unk_21D2C;
    /* 0x21D30 */ s32          unk_21D30;
    /* 0x21D34 */ char         unk_21D34[0x22E14 - 0x21D34];
    /* 0x22E14 */ s16          unk_22E14;
    /* 0x22E16 */ char         unk_22E16[2];
    /* 0x22E18 */ s32          unk_22E18;
    /* 0x22E1C */ s16          unk_22E1C;
    /* 0x22E1E */ char         unk_22E1E[2];
    /* 0x22E20 */ s32          unk_22E20;
    /* 0x22E24 */ char         unk_22E24[0x22E2C - 0x22E24];
    /* 0x22E2C */ s32          unk_22E2C;
    /* 0x22E30 */ char         unk_22E30[4];
    /* 0x22E34 */ s32          unk_22E34;
    /* 0x22E38 */ s32          unk_22E38;
    /* 0x22E3C */ s32          unk_22E3C;
} FieldEventManager;

// The running field's event manager.
extern FieldEventManager* data_ov030_02109d60;

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

// Creates a choice window from count promoted integer message IDs.
s32 func_ov030_020a9f54(FieldEventManager* eventMgr, s32 (*callback)(FieldEventManager*), s32 mesNo, u16 count, ...);

#endif // FIELD_EVENTS_H
