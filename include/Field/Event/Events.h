#ifndef FIELD_EVENT_EVENTS_H
#define FIELD_EVENT_EVENTS_H

#include <nitro/types.h>

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

struct FieldEventManager;

s32 func_ov030_02084eb4(struct FieldEventManager* eventMgr);
s32 func_ov030_02085cf8(struct FieldEventManager* arg0);
s32 func_ov030_020876c8(struct FieldEventManager* arg0);
s32 func_ov030_02089300(struct FieldEventManager* arg0);
s32 func_ov030_0208aaf4(struct FieldEventManager* arg0);
s32 func_ov030_0208c8a0(struct FieldEventManager* arg0);
s32 func_ov030_0208ddfc(struct FieldEventManager* arg0);
s32 func_ov030_0208ec3c(struct FieldEventManager* arg0);
s32 func_ov030_0208faa4(struct FieldEventManager* arg0);
s32 func_ov030_02091bc8(struct FieldEventManager* arg0);
s32 func_ov030_02093db8(struct FieldEventManager* arg0);
s32 func_ov030_0209585c(struct FieldEventManager* arg0);
s32 func_ov030_0209716c(struct FieldEventManager* arg0);
s32 func_ov030_02097f4c(struct FieldEventManager* arg0);
s32 func_ov030_02098a30(struct FieldEventManager* arg0);
s32 func_ov030_02099514(struct FieldEventManager* arg0);
s32 func_ov030_0209a564(struct FieldEventManager* arg0);
s32 func_ov030_0209bacc(struct FieldEventManager* arg0);
s32 func_ov030_0209c680(struct FieldEventManager* arg0);
s32 func_ov030_0209cff0(struct FieldEventManager* arg0);
s32 func_ov030_0209dafc(struct FieldEventManager* arg0);
s32 func_ov030_0209f0e8(struct FieldEventManager* arg0);
s32 func_ov030_020a206c(struct FieldEventManager* arg0);
s32 func_ov030_020a21a4(struct FieldEventManager* arg0);
s32 func_ov030_020a2628(struct FieldEventManager* arg0);
s32 func_ov030_020a2c20(struct FieldEventManager* arg0);
s32 func_ov030_020a2de8(struct FieldEventManager* arg0);
s32 func_ov030_020a306c(struct FieldEventManager* arg0);
s32 func_ov030_020a30d4(struct FieldEventManager* arg0);
s32 func_ov030_020a317c(struct FieldEventManager* arg0);
s32 func_ov030_020a31bc(struct FieldEventManager* arg0);
s32 func_ov030_020a333c(struct FieldEventManager* arg0);
s32 func_ov030_020a3718(struct FieldEventManager* arg0);
s32 func_ov030_020a3750(struct FieldEventManager* arg0);
s32 func_ov030_020a3788(struct FieldEventManager* arg0);
s32 func_ov030_020a37c0(struct FieldEventManager* arg0);
s32 func_ov030_020a37f8(struct FieldEventManager* arg0);
s32 func_ov030_020a3830(struct FieldEventManager* arg0);
s32 func_ov030_020a3858(struct FieldEventManager* arg0);
s32 func_ov030_020a3890(struct FieldEventManager* arg0);
s32 func_ov030_020a38c8(struct FieldEventManager* arg0);
s32 func_ov030_020a3900(struct FieldEventManager* arg0);
s32 func_ov030_020a3938(struct FieldEventManager* arg0);

#endif // FIELD_EVENT_EVENTS_H
