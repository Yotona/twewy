#include "Field/Events.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Player/Stats.h"
#include "Save.h"
#include <nitro/types.h>

extern s32  func_ov030_020aed9c(s32);
extern void func_ov030_020ae96c(s32);
extern void func_ov030_020c26bc(s32);
extern s32  func_ov030_020848a4(s32, s32, s32, s32);
extern s32  func_ov030_020848e4(FieldEventManager*, s32);
extern s32  func_ov030_02084978(FieldEventManager*);
extern s32  func_ov030_020a75c0(FieldEventManager*);
extern void func_ov030_020824a0(void*);
extern void func_ov030_02082c04(void*);
extern void func_ov030_02082bf0(void*);
extern void func_ov030_02082c68(void*);
extern void func_ov030_02082534(void*);

extern s32 data_ov030_020d9b6c;
extern s32 data_ov030_020d9c0c;
extern s32 data_ov030_020d9b1c;
extern s32 data_ov030_020d9b44;
extern s32 data_ov030_020d9acc;
extern s32 data_ov030_020d9af4;
extern s32 data_ov030_020d9be4;
extern s32 data_ov030_020d9b94;
extern s32 data_ov030_020d9bbc;
extern s32 data_ov030_020d9d34;

s32 func_ov030_02084a2c(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if (FldGate_Get(0x20) == 0) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x437, 0);
            return 1;
        } else if ((FldGate_Get(33) == 0) && (func_ov030_020aed9c(-1) != 0)) {
            FldGate_Set(33);
            gSaveData.unk_264C = 4;
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_USE_PLAYER_PIN, 0);
            return 1;
        } else if ((FldGate_Get(0x33) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_ESCAPE_FROGS, 0);
            return 1;
        }

        if ((FldGate_Get(0x23) != 0) && (FldGate_Get(0x34) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((FldGate_Get(37) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_BACK_IN_CROSSING, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) != 0) && (FldGate_Get(38) != 0) && (FldGate_Get(0x2C) == 0)) {
            FldGate_Set(0x2C);
            if (FldGate_Get(39) == 0) {
                FldGate_Set(39);
                FldEvent_BeginScript(arg0, EVENT_SHIKI1_UNKNOWN_96, 0);
                return 1;
            }
        }
        if ((func_ov030_020848e4(arg0, 3) != 0) && (FldGate_Get(38) != 0) && (FldGate_Get(45) == 0)) {
            FldGate_Set(45);
            if (FldGate_Get(39) == 0) {
                FldGate_Set(39);
                FldEvent_BeginScript(arg0, EVENT_SHIKI1_UNKNOWN_96, 0);
                return 1;
            }
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (FldGate_Get(38) != 0) && (FldGate_Get(3) == 0)) {
            FldGate_Set(3);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_RUN_INTO_WALL, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 3) {
        if (FldGate_Get(0x28) == 0) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_GET_TO_104, 0);
            return 1;
        } else if ((FldGate_Get(0x30) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_BEAT_104_FROGS, 0);
            return 1;
        } else if ((FldGate_Get(0x31) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_BEAT_GRIZZLY, 0);
            return 1;
        } else if ((FldGate_Get(0x32) != 0) && (FldGate_Get(43) == 0)) {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_DAY_END, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 5) {
        if (FldGate_Get(0x24) == 0) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_NOISE_BY_HACHIKO, 0);
            return 1;
        } else if ((FldGate_Get(47) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, EVENT_SHIKI1_BEAT_HACHIKO_FROGS, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 37) && (FldGate_Get(0x23) != 0) && (FldGate_Get(0x34) == 0)) {
        FldGate_Set(0x34);
        FldEvent_BeginScript(arg0, EVENT_SHIKI1_KARIYA_UZUKI_DIALOG | EVENTFLAG_NOBGM, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_02084eb4(FieldEventManager* eventMgr) {
    switch (gSaveData.currentStoryEvent) {
        case EVENT_SHIKI1_USE_PLAYER_PIN:
            FldGate_Set(0x22);
            gSaveData.unk_24BE = 0x67;
            func_ov030_020aec1c(10);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_READ_MISSION_TEXT | EVENTFLAG_NOBGM;
            gSaveData.unk_3124          = 3;
            gSaveData.unk_3128          = 0;
            eventMgr->unk_21630         = 4;
            DebugOvlDisp_Pop();
            return 0;

        case EVENT_SHIKI1_READ_MISSION_TEXT:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_RUN_DASH_ESCAPE_TUTORIAL | EVENTFLAG_NOBGM;
            func_ov030_020af364(2);
            return 1;

        case EVENT_SHIKI1_ESCAPE_FROGS:
            gSaveData.unk_3124  = 4;
            gSaveData.unk_3128  = 0;
            eventMgr->unk_21630 = 4;
            DebugOvlDisp_Pop();
            return 0;

        case EVENT_SHIKI1_KARIYA_UZUKI_DIALOG:
            eventMgr->unk_21AD0 = 1;
            eventMgr->unk_21ACC = 1;
            eventMgr->unk_21AFC = 5;
            eventMgr->unk_21B00 = 0x66;
            eventMgr->unk_21B08 = 0x139000;
            eventMgr->unk_21B0C = 0xE1000;
            break;

        case EVENT_SHIKI1_NOISE_BY_HACHIKO:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_HACHIKO_FROGS | EVENTFLAG_NOBGM;
            gSaveData.unk_3124          = 5;
            gSaveData.unk_3128          = 0;
            eventMgr->unk_21630         = 4;
            DebugOvlDisp_Pop();
            return 0;

        case EVENT_SHIKI1_HACHIKO_FROGS:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_PACT_WITH_SHIKI | EVENTFLAG_NOBGM;
            gSaveData.unk_3124          = 6;
            gSaveData.unk_3128          = 0;
            eventMgr->unk_21630         = 4;
            DebugOvlDisp_Pop();
            return 0;

        case EVENT_SHIKI1_PACT_WITH_SHIKI:
            FldGate_Set(2);
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_SHIKI;
            gSaveData.unk_24B4                 = 0;
            gSaveData.unk_24B8                 = 1;
            gSaveData.currentStoryEvent        = EVENT_SHIKI1_DRAG_AND_NEKU_COMBAT_TUTORIAL | EVENTFLAG_NOBGM;
            func_ov030_020af364(3);
            return 1;

        case EVENT_SHIKI1_BEAT_HACHIKO_FROGS:
            func_ov030_020aeaa0(3, 4);
            gSaveData.unk_2648  = 0;
            gSaveData.unk_264A  = 0;
            gSaveData.unk_264E  = 0;
            eventMgr->unk_21CF8 = 1;
            break;

        case EVENT_SHIKI1_RUN_INTO_WALL:
            eventMgr->unk_21CF8 = 1;
            gSaveData.unk_2648  = 4;
            break;

        case EVENT_SHIKI1_GET_TO_104:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_PARTNER_TUTORIAL;
            func_ov030_020af364(4);
            return 1;

        case EVENT_SHIKI1_BEAT_104_FROGS:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_104_FROG_REWARD;
            func_ov030_020af364(5);
            return 1;

        case EVENT_SHIKI1_BEAT_GRIZZLY:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_SHIKI1_GRIZZLY_BATTLE_REWARD;
            func_ov030_020af364(6);
            return 1;

        case EVENT_SHIKI1_DAY_END:
            gSaveData.unk_3124  = 7;
            gSaveData.unk_3128  = 1;
            eventMgr->unk_21630 = 4;
            DebugOvlDisp_Pop();
            return 0;

        case EVENT_SHIKI1_RUN_DASH_ESCAPE_TUTORIAL:
            FldGate_Set(GATE_UNKNOWN51);
            // First playthrough silently inserts the starter wallet into inventory after fleeing the very first (solo) battle
            if (Inventory_HasRequiredQuantity(ITEM_WALLET_MY_FIRST_WALLET, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_WALLET_MY_FIRST_WALLET, 0);
            }
            // Track 31: "It's So Wonderful", also silently inserted into the inventory after this tutorial
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK31, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_CD_TRACK31, 0);
            }
            // Book and sticker on first playthrough, 500 yen on replays
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_ESCAPING_FROM_BATTLE);
                FldEvent_GrantItem(eventMgr, ITEM_STICKER_DASH);
            } else {
                FldEvent_GrantPin(eventMgr, PIN_500_YEN);
            }
            break;

        case EVENT_SHIKI1_DRAG_AND_NEKU_COMBAT_TUTORIAL:
            FldGate_Set(GATE_UNKNOWN47);
            // Books and pin on first playthrough, 500 yen on replays
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_CONTROLS_DRAG);
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_BOOT_REBOOT);
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_BOTTOM_SCREEN_COMBAT);
                func_ov030_020aac28(eventMgr, PIN_PYROKINESIS);
            } else {
                FldEvent_GrantPin(eventMgr, PIN_500_YEN);
            }
            break;

        case EVENT_SHIKI1_PARTNER_TUTORIAL:
            FldGate_Set(GATE_UNKNOWN48);
            // Book on first playthrough, 500 yen on replays
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_TOP_SCREEN_COMBAT);
            } else {
                FldEvent_GrantPin(eventMgr, PIN_500_YEN);
            }
            break;

        case EVENT_SHIKI1_104_FROG_REWARD:
            FldGate_Set(GATE_UNKNOWN49);
            // Books on first playthrough, 500 yen on replays
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_FOLLOW_THAT_PUCK);
                FldEvent_GrantItem(eventMgr, ITEM_BOOK_DUAL_SCREEN_COMBAT);
            } else {
                FldEvent_GrantPin(eventMgr, PIN_500_YEN);
            }
            break;

        case EVENT_SHIKI1_GRIZZLY_BATTLE_REWARD:
            FldGate_Set(GATE_UNKNOWN50);
            FldEvent_GrantPin(eventMgr, PIN_SCARLETITE);
            FldEvent_GrantPin(eventMgr, PIN_500_YEN);
            FldEvent_GrantItem(eventMgr, ITEM_BOOK_SO_I_BEAT_THE_ENEMY);
            break;

        case EVENT_SHIKI1_SECRET_BOX_HACHIKO: // Only available on replays
            FldGate_Set(GATE_UNKNOWN01);
            // Thread the first time secret box is opened, 500 yen on replays
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_NOSE_GLASSES, 1, 0)) {
                FldEvent_GrantPin(eventMgr, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(eventMgr, ITEM_THREAD_NOSE_GLASSES);
            }
            eventMgr->unk_21CF8 = 1;
    }

    return 0;
}

void func_ov030_02085480(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = 0x6A;
    gSaveData.unk_3124          = 56;
    gSaveData.unk_244C          = 0x159000;
    gSaveData.unk_2450          = 0xD2000;
}

s32 func_ov030_02085508(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 18) {
        if ((FldGate_Get(0x20) == 0) && (func_ov030_020848a4(0, 0x96, 0x1E, 250) != 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x6B, 0);
            return 1;
        } else if ((FldGate_Get(0x20) != 0) && (FldGate_Get(33) == 0) && (func_ov030_020aed9c(-2) != 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x6C, 0);
            return 1;
        } else if ((FldGate_Get(0x35) != 0) && (FldGate_Get(56) == 0)) {
            FldGate_Set(56);
            FldEvent_BeginScript(arg0, 0x4C0, 0);
            return 1;
        } else if ((FldGate_Get(0x36) != 0) && (FldGate_Get(0x39) == 0)) {
            FldGate_Set(0x39);
            FldEvent_BeginScript(arg0, 0x4C1, 0);
            return 1;
        } else if ((FldGate_Get(0x37) != 0) && (FldGate_Get(58) == 0)) {
            FldGate_Set(58);
            FldEvent_BeginScript(arg0, 0x4C2, 0);
            return 1;
        } else if ((FldGate_Get(0x35) != 0) && (FldGate_Get(0x36) != 0) && (FldGate_Get(0x37) != 0) &&
                   (FldGate_Get(33) != 0) && (FldGate_Get(3) == 0))
        {
            FldGate_Set(3);
            gSaveData.unk_266E = 4;
            FldEvent_BeginScript(arg0, 0x6D, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x11) {
        if (FldGate_Get(0x22) == 0) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x6E, 0);
            return 1;
        } else if ((FldGate_Get(4) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x438, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x76, 0);
            return 1;
        } else if ((FldGate_Get(42) != 0) && (FldGate_Get(43) == 0) && (FldGate_Get(5) == 0) &&
                   (func_ov030_020848a4(0, 0xC8, 0x190, 0x159) != 0) && (func_ov030_020aed9c(-2) != 0))
        {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, 0x3CD, 0);
            return 1;
        } else if (((FldGate_Get(0x32) != 0) || (FldGate_Get(0x33) != 0)) &&
                   ((FldGate_Get(0x32) == 0) || (FldGate_Get(0x33) == 0)) && (FldGate_Get(43) == 0))
        {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, 0x3CD, 0);
            return 1;
        } else if ((FldGate_Get(0x32) != 0) && (FldGate_Get(0x33) != 0) && (FldGate_Get(5) == 0)) {
            FldGate_Set(5);
            FldEvent_BeginScript(arg0, 0x77, 0);
            return 1;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(61) == 0)) {
            FldGate_Set(61);
            FldEvent_BeginScript(arg0, 0x442, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (FldGate_Get(5) != 0) && (FldGate_Get(0x2C) == 0)) {
            FldGate_Set(0x2C);
            FldEvent_BeginScript(arg0, 0x78, 0);
            return 1;
        } else if ((FldGate_Get(46) == 0) && (func_ov030_020aed9c(0x3707) != 0)) {
            FldGate_Set(46);
            FldEvent_BeginScript(arg0, 0x3CF, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 5) {
        if (FldGate_Get(45) == 0) {
            FldGate_Set(45);
            FldEvent_BeginScript(arg0, 0x79, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x9B, 0x64, 0x190, 0x12C) != 0) && (FldGate_Get(6) == 0) &&
                   (func_ov030_020aed9c(-2) != 0))
        {
            FldGate_Set(6);
            FldEvent_BeginScript(arg0, 0x7A, 0);
            return 1;
        } else if ((FldGate_Get(0x3B) != 0) && (FldGate_Get(0x30) == 0)) {
            FldGate_Set(0x30);
            FldEvent_BeginScript(arg0, 0x84, 0);
            return 1;
        } else if ((FldGate_Get(0x3C) != 0) && (FldGate_Get(0x31) == 0)) {
            FldGate_Set(0x31);
            FldEvent_BeginScript(arg0, 0x85, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02085b14(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35EF:
            if (FldGate_Get(0x23) == 0) {
                FldGate_Set(0x23);
                FldEvent_SetCurrent(0x70);
                FldEvent_AdvanceScript(arg0);
                return 1;
            } else {
                FldEvent_SetCurrent(0x74);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
        case 0x35F0:
            FldGate_Set(0x24);
            FldEvent_SetCurrent(0x71);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35F1:
            FldGate_Set(37);
            FldEvent_SetCurrent(0x72);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35F3:
            FldGate_Set(38);
            FldEvent_SetCurrent(0x73);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35F2:
            FldEvent_SetCurrent(0x75);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02085c24(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35F5:
            FldEvent_SetCurrent(0x7C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35F6:
            FldEvent_SetCurrent(0x7D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35F7:
            if (FldGate_Get(47) == 0) {
                FldEvent_SetCurrent(0x7E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x80);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35F8:
            FldEvent_SetCurrent(0x82);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02085cf8(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x6A:
            gSaveData.unk_24BE = 0x87;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(5, 4);
            break;
        case 0x3CB:
            FldGate_Set(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6B:
            FldGate_Set(2);
            FldEvent_GrantItem(arg0, ITEM_BOOK_SCANNING);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6C:
            gSaveData.unk_310C = &data_ov030_020d9acc;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            FldEvent_GrantItem(arg0, ITEM_BOOK_ENEMY_ENCOUNTERS);
            break;
        case 0x6D:
            FldEvent_GrantPin(arg0, PIN_500_YEN);
            FldEvent_GrantItem(arg0, ITEM_BOOK_MAIL_ICON);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x6F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
            if ((FldGate_Get(0x23) != 0) && (FldGate_Get(0x24) != 0) && (FldGate_Get(37) != 0) && (FldGate_Get(38) != 0)) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 5, 0x35EF, 0x35F0, 0x35F3, 0x35F1, 0x35F2);
            }
            if (FldGate_Get(0x24) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 4, 0x35EF, 0x35F0, 0x35F3, 0x35F1);
            }
            if (FldGate_Get(0x24) == 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 3, 0x35EF, 0x35F0, 0x35F3);
            }
            break;
        case 0x75:
            FldEvent_GrantItem(arg0, ITEM_BOOK_WEARING_PINS);
            FldEvent_GrantItem(arg0, ITEM_STICKER_PHONE_MENU);
            func_ov030_020aac28(arg0, PIN_CURE_DRINK);
            FldGate_Set(4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x76:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x439);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x439:
            func_ov030_020c596c();
            gSaveData.unk_310C = &data_ov030_020d9af4;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x77:
            FldEvent_GrantItem(arg0, ITEM_STICKER_EXTRA_SLOT);
            FldEvent_GrantPin(arg0, PIN_500_YEN);
            gSaveData.unk_2656 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x78:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 5;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x84000;
            arg0->unk_21B0C = 0x98000;
            break;
        case 0x3CE:
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0x86);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7A:
            arg0->unk_21CF8 = 1;
            break;
        case 0x7B:
            FldGate_Set(7);
        case 0x7D:
        case 0x7F:
            if (FldGate_Get(47) == 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085c24, 0x35F4, 3, 0x35F5, 0x35F6, 0x35F7);
            }
            return func_ov030_020a9f54(arg0, func_ov030_02085c24, 0x35F4, 2, 0x35F8, 0x35F7);
        case 0x7C:
            if (func_ov030_020a9d00(arg0, 0x44B) == 0) {
                return 1;
            }
            arg0->unk_21D1E = 0x7B;
            arg0->unk_21D20 = 0x7B;
            arg0->unk_21D22 = 5;
            arg0->unk_21D24 = 0;
            if (arg0->unk_21D22 == 1) {
                func_ov030_020aec1c(3);
                func_ov030_020aec1c(2);
            } else if (arg0->unk_21D22 == 2) {
                func_ov030_020aec38(3);
                func_ov030_020aec1c(2);
            } else if (arg0->unk_21D22 == 4) {
                func_ov030_020aec1c(3);
                func_ov030_020aec38(2);
            } else if (arg0->unk_21D22 == 5) {
                func_ov030_020aec38(3);
                func_ov030_020aec38(2);
            }
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x3CF:
            FldEvent_SetCurrent(0x81);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x81:
            FldGate_Set(47);
            arg0->unk_21CF8 = 1;
            break;
        case 0x82:
            if (func_ov030_020a9d00(arg0, 0x44C) == 0) {
                return 1;
            }
            arg0->unk_21D1E = 0x83;
            arg0->unk_21D20 = 0x83;
            arg0->unk_21D22 = 4;
            arg0->unk_21D24 = 0;
            if (arg0->unk_21D22 == 1) {
                func_ov030_020aec1c(3);
                func_ov030_020aec1c(2);
            } else if (arg0->unk_21D22 == 2) {
                func_ov030_020aec38(3);
                func_ov030_020aec1c(2);
            } else if (arg0->unk_21D22 == 4) {
                func_ov030_020aec1c(3);
                func_ov030_020aec38(2);
            } else if (arg0->unk_21D22 == 5) {
                func_ov030_020aec38(3);
                func_ov030_020aec38(2);
            }
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x83:
            FldGate_Set(8);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x4C3;
            func_ov030_020af364(7);
            return 1;
        case 0x84:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4C4;
            func_ov030_020af364(8);
            return 1;
        case 0x85:
            gSaveData.unk_3124 = 8;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4C0:
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_BOOK_CONTROLS_TOUCH);
                func_ov030_020aac28(arg0, PIN_FORCE_ROUNDS);
                func_ov030_020aac28(arg0, PIN_THUNDERBOLT);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C1:
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_BOOK_USE_OBSTACLES);
                func_ov030_020aac28(arg0, PIN_PSYCHOKINESIS);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C2:
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_BOOK_CONTROLS_SLASH);
                func_ov030_020aac28(arg0, PIN_SHOCKWAVE);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C3:
            FldGate_Set(0x3B);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C4:
            FldGate_Set(0x3C);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_ICE_BLOW);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0x86:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SAILOR_SUIT, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_SAILOR_SUIT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02086700(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 2;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(23);
    gSaveData.unk_244C          = 0x12C000;
    gSaveData.unk_2450          = 0x87000;
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x8A;
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x39;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_02086804(s32 arg0) {
    s16 var_r0;

    if (gSaveData.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(0x24) == 0)) {
            if (FldGate_Get(0x44) == 0) {
                FldGate_Set(0x44);
                FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x91, 0);
                return 1;
            }
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0xAE, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 0x47) != 0) {
            if (FldGate_Get(0x3B) == 0) {
                if (FldGate_Get(0x44) == 0) {
                    FldGate_Set(0x44);
                    FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x91, 0);
                    return 1;
                }
                FldGate_Set(0x3B);
                FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x3D9, 0);
                return 1;
            }
        } else {
            FldGate_Clear(0x3B);
        }
        if ((FldGate_Get(0x44) != 0) && (FldGate_Get(0x48) == 0)) {
            FldGate_Set(0x48);
            FldEvent_BeginScript(arg0, 0x4CD, 0);
            return 1;
        } else if ((FldGate_Get(0x24) != 0) && (FldGate_Get(0x49) == 0)) {
            FldGate_Set(0x49);
            FldEvent_BeginScript(arg0, 0x43B, 0);
            return 1;
        } else if ((FldGate_Get(0x24) != 0) && (FldGate_Get(7) == 0)) {
            FldGate_Set(7);
            FldEvent_BeginScript(arg0, 0xAF, 0);
            return 1;
        } else if ((FldGate_Get(2) != 0) && (FldGate_Get(0x46) == 0)) {
            FldGate_Set(0x46);
            FldEvent_BeginScript(arg0, 0x4CB, 0);
            return 1;
        } else if ((FldGate_Get(0x46) != 0) && (FldGate_Get(9) == 0)) {
            FldGate_Set(9);
            FldEvent_BeginScript(arg0, 0xB1, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(18) != 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x3DC, 0);
            return 1;
        } else if ((FldGate_Get(0x28) == 0) && (func_ov030_020aed9c(0x371B) != 0)) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x3DD, 0);
            return 1;
        }
        if ((FldGate_Get(0x3C) == 0) && ((FldGate_Get(42) != 0) || (FldGate_Get(43) != 0) || (FldGate_Get(0x2C) != 0))) {
            FldGate_Set(0x3C);
            FldEvent_BeginScript(arg0, 0xB7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(43) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x3DF, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 0x15) {
        if (FldGate_Get(0x20) == 0) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x90, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x49) != 0) && (FldGate_Get(0x44) == 0)) {
            FldGate_Set(0x44);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x91, 0);
            return 1;
        } else if ((FldGate_Get(0x44) != 0) && (FldGate_Get(0x48) == 0)) {
            FldGate_Set(0x48);
            FldEvent_BeginScript(arg0, 0x4CD, 0);
            return 1;
        } else if ((FldGate_Get(0x45) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0xA8, 0);
            return 1;
        }
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (FldGate_Get(0x42) != 0) && (FldGate_Get(0x43) == 0)) {
            FldGate_Set(0x43);
        }
        if ((FldGate_Get(4) != 0) && (FldGate_Get(0x4C) == 0)) {
            FldGate_Set(0x4C);
            FldEvent_BeginScript(arg0, 0x443, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 0x16) {
        if (FldGate_Get(0x32) == 0) {
            FldGate_Set(0x32);
            FldEvent_BeginScript(arg0, 0x9A, 0);
            return 1;
        } else if ((FldGate_Get(10) != 0) && (FldGate_Get(6) != 0) && (FldGate_Get(11) == 0)) {
            FldGate_Set(11);
            FldEvent_BeginScript(arg0, 0xB3, 0);
            return 1;
        } else if ((FldGate_Get(37) != 0) && (FldGate_Get(0xE) == 0)) {
            FldGate_Set(0xE);
            FldEvent_BeginScript(arg0, 0xB4, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 23) {
        if ((func_ov030_02084860(0x12C, 0x87, 0x96) == 0) && (FldGate_Get(45) == 0)) {
            FldGate_Set(45);
            FldEvent_BeginScript(arg0, 0x8C, 0);
            return 1;
        } else if ((func_ov030_020aed9c(0x3715) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x3DB, 0);
            return 1;
        } else if ((FldGate_Get(0x47) != 0) && (FldGate_Get(61) == 0)) {
            FldGate_Set(61);
            FldEvent_BeginScript(arg0, 0xBA, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02086f10(s32 arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x35F9:
            FldEvent_SetCurrent(0x3E4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FA:
            FldEvent_SetCurrent(0x3E5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FB:
            FldEvent_SetCurrent(0x3E6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FC:
            FldEvent_SetCurrent(0x3E7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            FldEvent_SetCurrent(0x3E3);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
}

s32 func_ov030_02086fdc(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x35F9:
            if (FldGate_Get(38) != 0) {
                FldEvent_SetCurrent(0x3EC);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3EB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FA:
            FldGate_Set(42);
            FldEvent_SetCurrent(0x3ED);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FB:
            FldGate_Set(43);
            FldEvent_SetCurrent(0x3EE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FC:
            FldGate_Set(0x2C);
            FldEvent_SetCurrent(0x3EF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            FldEvent_SetCurrent(0x3EA);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
}

s32 func_ov030_020870ec(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x35F9:
            FldEvent_SetCurrent(0x3F3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FA:
            FldGate_Set(42);
            FldEvent_SetCurrent(0x3F4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FB:
            if (FldGate_Get(43) == 0) {
                FldGate_Set(43);
                FldGate_Set(13);
                FldEvent_SetCurrent(0x3EE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3F5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FC:
            if (FldGate_Get(0x2C) == 0) {
                FldGate_Set(0x2C);
                FldGate_Set(13);
                FldEvent_SetCurrent(0x3EF);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3F6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            FldEvent_SetCurrent(0x3F2);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
}

s32 func_ov030_02087238(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35FE:
            FldGate_Set(47);
            FldEvent_SetCurrent(0x93);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FF:
            FldGate_Set(0x30);
            FldEvent_SetCurrent(0x94);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3600:
            FldGate_Set(0x31);
            FldEvent_SetCurrent(0x95);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020872e8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3602:
            FldEvent_SetCurrent(0x97);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3603:
            FldEvent_SetCurrent(0x98);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087360(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            FldGate_Set(0x33);
            FldEvent_SetCurrent(0x9C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3606:
            FldGate_Set(0x34);
            FldEvent_SetCurrent(0x9D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3607:
            FldGate_Set(0x35);
            FldEvent_SetCurrent(0x9E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087414(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3609:
            FldGate_Set(0x36);
            FldEvent_SetCurrent(0xA2);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x360A:
            FldGate_Set(0x37);
            FldEvent_SetCurrent(0xA3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x360B:
            FldGate_Set(56);
            FldEvent_SetCurrent(0xA4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x360C:
            FldGate_Set(0x39);
            FldEvent_SetCurrent(0xA6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x360D:
            FldEvent_SetCurrent(0xA5);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_020874fc(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x360F:
            FldEvent_SetCurrent(0xAA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3610:
            FldEvent_SetCurrent(0xAB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087574(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3612:
            FldEvent_SetCurrent(0xAA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3613:
            FldEvent_SetCurrent(0xAD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020875ec(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3615:
            FldEvent_SetCurrent(0x3D2);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3616:
            FldEvent_SetCurrent(0x3D3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3617:
            FldEvent_SetCurrent(0x3D4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3618:
            FldEvent_SetCurrent(0x3D5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3619:
            FldEvent_SetCurrent(0x3D6);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_020876c8(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x8A:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x8B;
            gSaveData.unk_3124          = 9;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x8B:
            FldEvent_GrantItem(arg0, ITEM_BOOK_SHUTDOWN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_SHUTDOWN);
            break;
        case 0x8C:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x8D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x8D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x8E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x8E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x8F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x8F:
            gSaveData.unk_24BE = 0xC1;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(0x15, 4);
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_SHIKI;
            gSaveData.unk_267A                 = 4;
            FldEvent_GrantItem(arg0, ITEM_STICKER_EASY);
            break;
        case 0x90:
            FldEvent_GrantItem(arg0, ITEM_BOOK_TALK_TO_YOUR_PARTNER);
            break;
        case 0x91:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x43A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x43A:
            if ((func_ov030_020848e4(arg0, 0x49) != 0) || (func_ov030_020848e4(arg0, 0x4A) != 0)) {
                arg0->unk_21630 = 5;
                DebugOvlDisp_Pop();
                return 0;
            }
            if (func_ov030_020848e4(arg0, 0x47) != 0) {
                FldGate_Set(0x3B);
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0x3D9);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (func_ov030_020848e4(arg0, 0x48) != 0) {
                FldGate_Set(0x24);
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0xAE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x92:
            FldGate_Set(5);
        case 0x93:
        case 0x94:
        case 0x95:
            if ((FldGate_Get(47) != 0) && (FldGate_Get(0x30) != 0) && (FldGate_Get(0x31) != 0)) {
                FldEvent_SetCurrent(0x96);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (FldGate_Get(0x30) != 0) {
                return func_ov030_020a9f54(arg0, &func_ov030_02087238, 0x35FD, 3, 0x35FE, 0x35FF, 0x3600);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087238, 0x35FD, 2, 0x35FE, 0x35FF);
        case 0x96:
            return func_ov030_020a9f54(arg0, &func_ov030_020872e8, 0x3601, 2, 0x3602, 0x3603);
        case 0x97:
        case 0x98:
            arg0->unk_21CF8 = 1;
            break;
        case 0x78B:
            if (FldGate_Get(5) != 0) {
                FldEvent_SetCurrent(0x78C);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x78C:
            if (FldGate_Get(0x3F) == 0) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0xA0);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xA9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xA0:
            FldGate_Set(0x3F);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xA1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA6:
            if ((FldGate_Get(0x36) != 0) && (FldGate_Get(0x37) != 0) && (FldGate_Get(56) != 0) && (FldGate_Get(0x39) != 0)) {
                return func_ov030_020a9f54(arg0, &func_ov030_02087414, 0x3608, 5, 0x3609, 0x360A, 0x360B, 0x360C, 0x360D);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087414, 0x3608, 4, 0x3609, 0x360A, 0x360B, 0x360C);
        case 0xA5:
            FldEvent_SetCurrent(0x4CA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xA8:
            FldEvent_SetCurrent(0xA9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xA9:
            return func_ov030_020a9f54(arg0, &func_ov030_020874fc, 0x360E, 2, 0x360F, 0x3610);
        case 0xAA:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0xACU;
            func_ov030_020af364(9);
            return 1;
        case 0xAC:
            return func_ov030_020a9f54(arg0, &func_ov030_02087574, 0x3611, 2, 0x3612, 0x3613);
        case 0xAD:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x78D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x78D:
            FldGate_Set(3);
            gSaveData.unk_2674 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x78E:
            if ((FldGate_Get(0x42) != 0) && (FldGate_Get(0x43) != 0)) {
                FldEvent_SetCurrent(0x790);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (FldGate_Get(0x41) != 0) {
                FldEvent_SetCurrent(0x78F);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x78F:
            if (FldGate_Get(0x42) == 0) {
                FldGate_Set(0x42);
                gSaveData.unk_30FC = 0;
                gSaveData.unk_30FE = 1;
                gSaveData.unk_3100 = 0x15;
                arg0->unk_21CF8    = 1;
            }
            break;
        case 0x790:
            FldGate_Set(4);
            gSaveData.unk_2678 = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_JUMP);
            arg0->unk_21CF8 = 1;
            break;
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
            if ((FldGate_Get(0x33) != 0) && (FldGate_Get(0x34) != 0) && (FldGate_Get(0x35) != 0)) {
                func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
                FldEvent_SetCurrent(0x9F);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (FldGate_Get(0x33) != 0) {
                return func_ov030_020a9f54(arg0, &func_ov030_02087360, 0x3604, 3, 0x3605, 0x3606, 0x3607);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087360, 0x3604, 2, 0x3605, 0x3606);
        case 0x9F:
            FldGate_Set(18);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x43C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x43C:
            FldEvent_GrantItem(arg0, ITEM_BOOK_MEMES);
            arg0->unk_21CF8 = 1;
            break;

        case 0x3E1:
        case 0x3E2:
            return func_ov030_020aa0c8(arg0, 0xA2, 0x3E2, 0x3E3, &func_ov030_02086f10);
        case 0x3E7:
        case 0x3E6:
        case 0x3E5:
        case 0x3E4:
            FldGate_Set(6);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3D9:
            if (FldGate_Get(37) == 0) {
                if (FldGate_Get(58) == 0) {
                    FldGate_Set(58);
                    FldEvent_SetCurrent(0x3D1);
                    FldEvent_AdvanceScript(arg0);
                    return 1;
                }
                FldEvent_SetCurrent(0x3D7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((FldGate_Get(42) != 0) && (FldGate_Get(0x10) == 0)) {
                FldGate_Set(0x10);
                FldEvent_SetCurrent(0x3DE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(0x13) == 0)) {
                FldGate_Set(0x13);
                FldEvent_SetCurrent(0x3E0);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (FldGate_Get(58) == 0) {
                FldGate_Set(58);
                FldEvent_SetCurrent(0x3D1);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3D7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3D1:
        case 0x3D2:
        case 0x3D3:
        case 0x3D4:
        case 0x3D7:
        case 0x3D5:
            return func_ov030_020a9f54(arg0, &func_ov030_020875ec, 0x3614, 5, 0x3615, 0x3616, 0x3617, 0x3618, 0x3619);
        case 0xAE:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0xAF:
            FldGate_Set(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB0:
            FldGate_Set(8);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB1:
            arg0->unk_21CF8 = 1;
            break;
        case 0xB2:
            FldGate_Set(10);
            FldGate_Set(0x41);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB3:
            FldGate_Set(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E8:
        case 0x3E9:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3E9, 0x3EA, &func_ov030_02086fdc);
        case 0x3EF:
        case 0x3EE:
        case 0x3ED:
            FldGate_Set(37);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB4:
            FldGate_Set(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DB:
            FldGate_Set(38);
            func_ov030_020aa9f4(arg0, 0x35FA, 0x5DD1, 0);
            break;
        case 0x3DC:
            func_ov030_020aa9f4(arg0, 0x35FB, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DD:
            func_ov030_020aa9f4(arg0, 0x35FC, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DE:
            FldGate_Set(0x10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DF:
            FldGate_Set(12);
            FldGate_Clear(13);
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0xC0);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E0:
            FldGate_Set(0x13);
            FldGate_Set(0xC);
            FldGate_Clear(0xD);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3F0:
        case 0x3F1:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3F1, 0x3F2, &func_ov030_020870ec);
        case 0x3F4:
            FldGate_Set(0xD);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB5:
            FldGate_Set(15);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB6:
            FldGate_Set(15);
            func_ov030_020aac28(arg0, PIN_LUCKY_STAR);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB9:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4CC;
            func_ov030_020af364(10);
            return 1;
        case 0xBA:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0xBB;
            gSaveData.unk_3124          = 10;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0xBB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xBC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xBC:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xBD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xBD:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xBE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xBE:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xBF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xBF:
            gSaveData.unk_3124 = 11;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x3EB:
            FldGate_Set(0x4A);
            break;
        case 0x3F2:
            FldGate_Set(0x4B);
            break;
        case 0x4CD:
            FldGate_Set(0x48);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_BOOK_ITEM_ABILITIES);
                FldEvent_GrantItem(arg0, ITEM_BOOK_SHOP_CLERKS);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4CA:
            FldGate_Set(0x45);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_SHIKI);
                FldEvent_GrantItem(arg0, ITEM_STICKER_ESP_CARDS);
            }
            break;
        case 0x4CB:
            FldGate_Set(0x46);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_BOOK_YELLOW_NOISE_SYMBOLS);
                FldEvent_GrantItem(arg0, ITEM_BOOK_RED_NOISE_SYMBOLS);
                FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4CC:
            FldGate_Set(0x47);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_POISON_BONES);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0xC0:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_YUKATA, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_YUKATA);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02088a5c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(37);
    gSaveData.unk_244C          = 0x1DA000;
    gSaveData.unk_2450          = 0xBE000;
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0xC4;
    gSaveData.unk_3124          = 58;
    func_ov030_020c26bc(0);
    gSaveData.unk_26B8 = 0;
    gSaveData.unk_2648 = 2;
    gSaveData.unk_2666 = 2;
}

s32 func_ov030_02088b60(FieldEventManager* arg0) {
    if ((FldGate_Get(47) != 0) && (FldGate_Get(2) == 0)) {
        if (FldGate_Get(0x34) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) != 0) {
                FldGate_Set(0x34);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) == 0) {
            FldGate_Set(0x35);
            FldGate_Clear(0x34);
        }
    }
    if ((FldGate_Get(0x30) != 0) && (FldGate_Get(3) == 0)) {
        if (FldGate_Get(0x31) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) != 0) {
                FldGate_Set(0x31);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) == 0) {
            FldGate_Set(0x36);
            FldGate_Clear(0x31);
        }
    }
    if (gSaveData.unk_2458 == 3) {
        if (FldGate_Get(0x37) == 0) {
            FldGate_Set(0x37);
            FldEvent_BeginScript(arg0, 0xC5, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (FldGate_Get(4) != 0) && (FldGate_Get(37) == 0) && (FldGate_Get(0x2C) == 0))
        {
            FldGate_Set(0x2C);
            FldEvent_BeginScript(arg0, 0x3F7, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 0) == 0) {
            FldGate_Clear(0x2C);
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (FldGate_Get(4) == 0)) {
            FldGate_Set(4);
            FldEvent_BeginScript(arg0, 0xC7, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 56) != 0) && (FldGate_Get(4) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0xC9, 0);
            return 1;
        }
        if ((FldGate_Get(37) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0xCE, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 1) {
        if ((func_ov030_020c1a9c() != 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0xCF, 0);
            return 1;
        } else if ((FldGate_Get(47) != 0) && (FldGate_Get(0x32) == 0)) {
            FldGate_Set(0x32);
            FldEvent_BeginScript(arg0, 0x441, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 11) {
        if ((func_ov030_020848a4(0xAA, 0xB4, 0xB4, 0x140) != 0) && (FldGate_Get(0x28) == 0)) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0xD0, 0);
            return 1;
        } else if ((FldGate_Get(0x28) != 0) && (FldGate_Get(56) == 0)) {
            FldGate_Set(56);
            FldEvent_BeginScript(arg0, 0x444, 0);
            return 1;
        } else if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
            FldEvent_BeginScript(arg0, 0x440, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 12) && (func_ov030_020848e4(arg0, 15) != 0) && (FldGate_Get(6) == 0)) {
        FldGate_Set(6);
        FldEvent_BeginScript(arg0, 0xD3, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 13) {
        if (FldGate_Get(41) == 0) {
            FldGate_Set(41);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0xD4;
            gSaveData.unk_3124          = 12;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
        if ((FldGate_Get(45) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0xD5, 0);
            return 1;
        } else if ((FldGate_Get(46) != 0) && (FldGate_Get(43) == 0)) {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, 0xD6, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020890c0(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            FldGate_Set(0x22);
            FldEvent_SetCurrent(0xCA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361C:
            FldGate_Set(0x23);
            FldEvent_SetCurrent(0xCB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02089148(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            FldGate_Set(0x22);
            FldEvent_SetCurrent(0xCA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361C:
            FldGate_Set(0x23);
            FldEvent_SetCurrent(0xCB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361D:
            FldGate_Set(0x24);
            FldEvent_SetCurrent(0xCC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361E:
            FldGate_Set(37);
            FldEvent_SetCurrent(0xCD);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02089214(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            FldGate_Set(0x22);
            FldEvent_SetCurrent(0xCA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361C:
            FldGate_Set(0x23);
            FldEvent_SetCurrent(0xCB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361D:
            FldGate_Set(0x24);
            FldEvent_SetCurrent(0xCC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361E:
            FldGate_Set(37);
            FldEvent_SetCurrent(0xCD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x361F:
            FldEvent_SetCurrent(0x43D);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02089300(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0xC4:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 3;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x1D8000;
            arg0->unk_21B0C = 0xC1000;
            break;
        case 0xC5:
            gSaveData.unk_24BE = 0xD8;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(13, 4);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xC8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xC6:
            FldGate_Set(4);
            FldGate_Set(0x2C);
            gSaveData.unk_26B8 = 2;
            FldEvent_GrantItem(arg0, ITEM_STICKER_NOISE_REPORT);
            arg0->unk_21CF8 = 1;
            break;
        case 0xC7:
            FldGate_Set(0x2C);
            gSaveData.unk_26B8 = 2;
            FldEvent_GrantItem(arg0, ITEM_STICKER_NOISE_REPORT);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3F7:
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0xD7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
            if ((FldGate_Get(0x22) != 0) && (FldGate_Get(0x23) != 0) && (FldGate_Get(0x24) != 0) && (FldGate_Get(37) != 0)) {
                return func_ov030_020a9f54(arg0, func_ov030_02089214, 0x361A, 5, 0x361B, 0x361C, 0x361D, 0x361E, 0x361F);
            }
            if (FldGate_Get(0x23) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02089148, 0x361A, 4, 0x361B, 0x361C, 0x361D, 0x361E);
            }
            return func_ov030_020a9f54(arg0, func_ov030_020890c0, 0x361A, 2, 0x361B, 0x361C);
        case 0x43D:
            gSaveData.unk_2648 = 4;
            gSaveData.unk_26B8 = 4;
            arg0->unk_21630    = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0xCE:
            FldEvent_SetCurrent(0x43E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x43E:
            FldEvent_GrantItem(arg0, ITEM_BOOK_SHOP_QUESTS);
            FldEvent_GrantItem(arg0, ITEM_STICKER_BRAND_AWARENESS);
            break;
        case 0xCF:
            FldEvent_GrantItem(arg0, ITEM_BOOK_PIN_GROWTH_EVOLUTION);
            FldEvent_GrantItem(arg0, ITEM_BOOK_GREEN_NOISE_SYMBOLS);
            break;
        case 0xD0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xD1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xD1:
            FldEvent_GrantItem(arg0, ITEM_STICKER_BACKLASH);
            break;
        case 0xD3:
            FldGate_Set(6);
            gSaveData.unk_2666 = 4;
            FldEvent_GrantItem(arg0, ITEM_STICKER_MINGLE_MODE);
            arg0->unk_21CF8 = 1;
            break;
        case 0xD4:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4D4;
            func_ov030_020af364(12);
            return 1;
        case 0xD5:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4D5;
            func_ov030_020af364(11);
            return 1;
        case 0xD6:
            gSaveData.unk_3124 = 13;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0xD2:
            FldGate_Set(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x791:
            FldEvent_SetCurrent(0x792);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x792:
            FldGate_Set(47);
            if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) != 0) {
                FldEvent_SetCurrent(0x793);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x793:
            FldGate_Set(2);
            FldGate_Set(0x34);
            gSaveData.unk_264E = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            FldEvent_GrantItem(arg0, ITEM_STICKER_EXTRA_SLOT);
            arg0->unk_21CF8 = 1;
            break;
        case 0x794:
            FldEvent_SetCurrent(0x795);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x795:
            FldGate_Set(0x30);
            if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) != 0) {
                FldEvent_SetCurrent(0x796);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x796:
            FldGate_Set(3);
            FldGate_Set(0x31);
            gSaveData.unk_2664 = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_BOOK_CHAINED_BATTLES);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4D4:
            FldGate_Set(45);
            func_ov030_020aac28(arg0, PIN_LIGHTNING_MOON);
            break;
        case 0x4D5:
            FldGate_Set(46);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_MASAMUNE);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0xD7:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_MRS_CLAUS, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_MRS_CLAUS);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02089a40(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(6);
    func_ov030_020c26bc(0);
    gSaveData.unk_2658          = 4;
    gSaveData.unk_3124          = 0x3B;
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0xDB;
}

s32 func_ov030_02089b28(s32 arg0) {
    s16 var_r0;
    s16 var_r0_2;

    if ((FldGate_Get(0x48) != 0) && (FldGate_Get(4) == 0)) {
        if (FldGate_Get(0x49) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) != 0) {
                FldGate_Set(0x49);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) == 0) {
            FldGate_Set(0x4A);
            FldGate_Clear(0x49);
        }
    }
    if (gSaveData.unk_2458 == 2) {
        if ((FldGate_Get(2) != 0) && (FldGate_Get(0x45) == 0)) {
            FldGate_Set(0x45);
            FldEvent_BeginScript(arg0, 0x4DE, 0);
            return 1;
        } else if ((FldGate_Get(0x45) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0xE1, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 6) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (FldGate_Get(0x43) != 0) && (FldGate_Get(0x44) == 0)) {
            FldGate_Set(0x44);
        }
    }
    if (gSaveData.unk_2458 == 0) {
        if (FldGate_Get(0x20) == 0) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0xDC, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3C) != 0) && (FldGate_Get(33) == 0) && (FldGate_Get(0x46) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0xDD, 0);
            return 1;
        }
        if ((gSaveData.unk_3102 >= 3) && (FldGate_Get(0x48) != 0) && (FldGate_Get(0x49) == 0)) {
            FldGate_Set(0x49);
        }
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0_2 = gSaveData.unk_30FC;
        } else {
            var_r0_2 = 0;
        }
        if (((s32)var_r0_2 >= 3) && (FldGate_Get(0x46) == 0)) {
            FldGate_Set(0x46);
            FldEvent_BeginScript(arg0, 0x4DF, 0);
            return 1;
        }
        if ((FldGate_Get(0x46) != 0) && (FldGate_Get(5) == 0)) {
            FldGate_Set(5);
            FldEvent_BeginScript(arg0, 0xDE, 0);
            return 1;
        }
        if ((FldGate_Get(39) != 0) && (FldGate_Get(0x28) != 0) && (FldGate_Get(41) != 0) && (FldGate_Get(6) == 0)) {
            FldGate_Set(6);
            FldEvent_BeginScript(arg0, 0xDF, 0);
            return 1;
        }
        if ((FldGate_Get(39) != 0) && (FldGate_Get(0x28) != 0) && (FldGate_Get(41) != 0) && (FldGate_Get(6) != 0) &&
            (FldGate_Get(9) == 0))
        {
            FldGate_Clear(39);
            FldGate_Clear(0x28);
            FldGate_Clear(41);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
        }
        if ((FldGate_Get(38) != 0) && (FldGate_Get(8) == 0)) {
            FldGate_Set(8);
            FldEvent_BeginScript(arg0, 0xE6, 0);
            return 1;
        }
        if ((FldGate_Get(37) != 0) && (FldGate_Get(9) == 0)) {
            FldGate_Set(9);
            FldEvent_BeginScript(arg0, 0xEA, 0);
            return 1;
        }
        if ((FldGate_Get(42) != 0) && (FldGate_Get(0x4B) == 0)) {
            FldGate_Set(0x4B);
            FldEvent_BeginScript(arg0, 0x4E1, 0);
            return 1;
        }
        if ((FldGate_Get(0x4B) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0xEB, 0);
            return 1;
        }
        if ((FldGate_Get(0x47) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0xEC, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208a028(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3626:
            FldEvent_SetCurrent(0xE7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3627:
            FldEvent_SetCurrent(0xE8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a0a0(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x3620:
            FldEvent_SetCurrent(0x3FA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3621:
            FldEvent_SetCurrent(0x3FB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a120(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x3620:
            FldEvent_SetCurrent(0x3FF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3621:
            FldEvent_SetCurrent(0x400);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a19c(FieldEventManager* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (FldGate_Get(0x2C) != 0) {
                func_ov030_020a9db8(arg0, 0x460);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x45F);
            return 0;
        case 1:
            FldEvent_SetCurrent(0x451);
            return 1;
        case 0:
            if (FldGate_Get(47) == 0) {
                FldGate_Set(47);
                FldEvent_SetCurrent(0x44E);
                return 1;
            }
            if (FldGate_Get(0x30) == 0) {
                FldGate_Set(0x30);
                FldEvent_SetCurrent(0x44F);
                return 1;
            }
            FldGate_Clear(0x30);
            FldEvent_SetCurrent(0x450);
            return 1;
        case 2:
            if (FldGate_Get(45) == 0) {
                FldGate_Set(45);
                FldEvent_SetCurrent(0x452);
                return 1;
            }
            if (FldGate_Get(46) == 0) {
                FldGate_Set(46);
                FldEvent_SetCurrent(0x453);
                return 1;
            }
            FldGate_Clear(46);
            FldEvent_SetCurrent(0x454);
            return 1;
        case 3:
            FldEvent_SetCurrent(0x455);
            return 1;
        case 4:
            FldEvent_SetCurrent(0x456);
            return 1;
        case 5:
            FldEvent_SetCurrent(0x457);
            return 1;
        case 6:
            FldEvent_SetCurrent(0x458);
            return 1;
        case 7:
            FldEvent_SetCurrent(0x459);
            return 1;
        case 8:
            FldEvent_SetCurrent(0x45A);
            return 1;
        case 9:
            func_ov030_020a9db8(arg0, 0x45B);
            return 0;
        case 10:
            func_ov030_020a9db8(arg0, 0x45C);
            return 0;
        case 11:
            func_ov030_020a9db8(arg0, 0x45D);
            return 0;
        case 13:
            func_ov030_020a9db8(arg0, 0x45E);
            return 0;
    }
    return 0;
}

s32 func_ov030_0208a3f0(FieldEventManager* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (FldGate_Get(0x31) != 0) {
                func_ov030_020a9db8(arg0, 0x476);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x475);
            return 0;
        case 1:
            FldEvent_SetCurrent(0x467);
            return 1;
        case 0:
            if (FldGate_Get(0x34) == 0) {
                FldGate_Set(0x34);
                FldEvent_SetCurrent(0x464);
                return 1;
            }
            if (FldGate_Get(0x35) == 0) {
                FldGate_Set(0x35);
                FldEvent_SetCurrent(0x465);
                return 1;
            }
            FldGate_Clear(0x35);
            FldEvent_SetCurrent(0x466);
            return 1;
        case 2:
            if (FldGate_Get(0x32) == 0) {
                FldGate_Set(0x32);
                FldEvent_SetCurrent(0x468);
                return 1;
            }
            if (FldGate_Get(0x33) == 0) {
                FldGate_Set(0x33);
                FldEvent_SetCurrent(0x469);
                return 1;
            }
            FldGate_Clear(0x33);
            FldEvent_SetCurrent(0x46A);
            return 1;
        case 3:
            FldEvent_SetCurrent(0x46B);
            return 1;
        case 4:
            FldEvent_SetCurrent(0x46C);
            return 1;
        case 5:
            FldEvent_SetCurrent(0x46D);
            return 1;
        case 6:
            FldEvent_SetCurrent(0x46E);
            return 1;
        case 7:
            FldEvent_SetCurrent(0x46F);
            return 1;
        case 8:
            FldEvent_SetCurrent(0x470);
            return 1;
        case 9:
            func_ov030_020a9db8(arg0, 0x471);
            return 0;
        case 10:
            func_ov030_020a9db8(arg0, 0x472);
            return 0;
        case 11:
            func_ov030_020a9db8(arg0, 0x473);
            return 0;
        case 13:
            func_ov030_020a9db8(arg0, 0x474);
            return 0;
    }
    return 0;
}

s32 func_ov030_0208a648(FieldEventManager* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (FldGate_Get(0x36) != 0) {
                func_ov030_020a9db8(arg0, 0x48B);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x48A);
            return 0;
        case 1:
            FldEvent_SetCurrent(0x47C);
            return 1;
        case 0:
            if (FldGate_Get(0x39) == 0) {
                FldGate_Set(0x39);
                FldEvent_SetCurrent(0x479);
                return 1;
            }
            if (FldGate_Get(58) == 0) {
                FldGate_Set(58);
                FldEvent_SetCurrent(0x47A);
                return 1;
            }
            FldGate_Clear(58);
            FldEvent_SetCurrent(0x47B);
            return 1;
        case 2:
            if (FldGate_Get(0x37) == 0) {
                FldGate_Set(0x37);
                FldEvent_SetCurrent(0x47D);
                return 1;
            }
            if (FldGate_Get(56) == 0) {
                FldGate_Set(56);
                FldEvent_SetCurrent(0x47E);
                return 1;
            }
            FldGate_Clear(56);
            FldEvent_SetCurrent(0x47F);
            return 1;
        case 3:
            FldEvent_SetCurrent(0x480);
            return 1;
        case 4:
            FldEvent_SetCurrent(0x481);
            return 1;
        case 5:
            FldEvent_SetCurrent(0x482);
            return 1;
        case 6:
            FldEvent_SetCurrent(0x483);
            return 1;
        case 7:
            FldEvent_SetCurrent(0x484);
            return 1;
        case 8:
            FldEvent_SetCurrent(0x485);
            return 1;
        case 9:
            func_ov030_020a9db8(arg0, 0x486);
            return 0;
        case 10:
            func_ov030_020a9db8(arg0, 0x487);
            return 0;
        case 11:
            func_ov030_020a9db8(arg0, 0x488);
            return 0;
        case 13:
            func_ov030_020a9db8(arg0, 0x489);
            return 0;
    }
    return 0;
}

s32 func_ov030_0208a8a0(FieldEventManager* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (FldGate_Get(0x3B) != 0) {
                func_ov030_020a9db8(arg0, 0x4A0);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x49F);
            return 0;
        case 1:
            FldEvent_SetCurrent(0x491);
            return 1;
        case 0:
            if (FldGate_Get(0x3E) == 0) {
                FldGate_Set(0x3E);
                FldEvent_SetCurrent(0x48E);
                return 1;
            } else if (FldGate_Get(0x3F) == 0) {
                FldGate_Set(0x3F);
                FldEvent_SetCurrent(0x48F);
                return 1;
            }
            FldGate_Clear(0x3F);
            FldEvent_SetCurrent(0x490);
            return 1;
        case 2:
            if (FldGate_Get(0x3C) == 0) {
                FldGate_Set(0x3C);
                FldEvent_SetCurrent(0x492);
                return 1;
            } else if (FldGate_Get(0x3D) == 0) {
                FldGate_Set(0x3D);
                FldEvent_SetCurrent(0x493);
                return 1;
            }
            FldGate_Clear(0x3D);
            FldEvent_SetCurrent(0x494);
            return 1;
        case 3:
            FldEvent_SetCurrent(0x495);
            return 1;
        case 4:
            FldEvent_SetCurrent(0x496);
            return 1;
        case 5:
            FldEvent_SetCurrent(0x497);
            return 1;
        case 6:
            FldEvent_SetCurrent(0x498);
            return 1;
        case 7:
            FldEvent_SetCurrent(0x499);
            return 1;
        case 8:
            FldEvent_SetCurrent(0x49A);
            return 1;
        case 9:
            func_ov030_020a9db8(arg0, 0x49B);
            return 0;
        case 10:
            func_ov030_020a9db8(arg0, 0x49C);
            return 0;
        case 11:
            func_ov030_020a9db8(arg0, 0x49D);
            return 0;
        case 13:
            func_ov030_020a9db8(arg0, 0x49E);
            return 0;
    }
    return 0;
}

s32 func_ov030_0208aaf4(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0xDB:
            gSaveData.unk_24BE = 0xEE;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(0, 4);
            break;
        case 0xDC:
            gSaveData.unk_30FC = 0;
            gSaveData.unk_30FE = 39;
            gSaveData.unk_3100 = 0;
            break;
        case 0xDE:
            gSaveData.unk_310C = &data_ov030_020d9b1c;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xDF:
            FldGate_Clear(39);
            FldGate_Clear(0x28);
            FldGate_Clear(41);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE1:
            func_ov030_020aa9f4(arg0, 0x3621, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE0:
            FldGate_Set(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE2:
            FldEvent_SetCurrent(0xE3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x45F:
            FldGate_Set(0x2C);
        case 0x460:
            FldGate_Clear(47);
            FldGate_Clear(0x30);
            FldGate_Clear(45);
            FldGate_Clear(46);
        case 0x45E:
        case 0xE3:
            if (func_ov030_020a9d00(arg0, 0x44D) == 0) {
                return 1;
            }
            arg0->unk_21D22 = 3;
            arg0->unk_21D24 = func_ov030_0208a19c;
            arg0->unk_21D28 = 0x5ED4;
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x45B:
        case 0x45C:
        case 0x45D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x461);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x461:
            FldGate_Set(10);
            func_ov030_020aa9f4(arg0, 0x3620, 0x5DD1, 0);
            break;
        case 0x3F8:
        case 0x3F9:
        case 0x3FB:
            return func_ov030_020aa0c8(arg0, 0x9A, 0x3F9, 0x3FC, &func_ov030_0208a0a0);
        case 0x3FC:
        case 0x3FD:
        case 0x3FF:
        case 0x400:
            return func_ov030_020aa0c8(arg0, 0x92, 0x3FD, 0x3FE, &func_ov030_0208a120);
        case 0x3FA:
            FldGate_Set(38);
            break;
        case 0xE6:
        case 0xE9:
            FldGate_Set(8);
            return func_ov030_020a9f54(arg0, &func_ov030_0208a028, 0x3625, 2, 0x3626, 0x3627);
        case 0xE8:
            arg0->unk_21CF8 = 1;
            break;
        case 0xE7:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x462);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x475:
            FldGate_Set(0x31);
        case 0x476:
            FldGate_Clear(0x34);
            FldGate_Clear(0x35);
            FldGate_Clear(0x32);
            FldGate_Clear(0x33);
        case 0x474:
        case 0x462:
            if (func_ov030_020a9d00(arg0, 0x463) == 0) {
                return 1;
            }
            arg0->unk_21D22 = 3;
            arg0->unk_21D24 = func_ov030_0208a3f0;
            arg0->unk_21D28 = 0x5F1F;
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x471:
            FldGate_Set(0x40);
        case 0x473:
        case 0x472:
            FldEvent_SetCurrent(0x477);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x48A:
            FldGate_Set(0x36);
        case 0x48B:
            FldGate_Clear(0x39);
            FldGate_Clear(58);
            FldGate_Clear(0x37);
            FldGate_Clear(56);
        case 0x477:
        case 0x489:
            if (func_ov030_020a9d00(arg0, 0x478) == 0) {
                return 1;
            }
            arg0->unk_21D22 = 3;
            arg0->unk_21D24 = func_ov030_0208a648;
            arg0->unk_21D28 = 0x5F76;
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x486:
            FldGate_Set(0x41);
        case 0x488:
        case 0x487:
            FldEvent_SetCurrent(0x48C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x49F:
            FldGate_Set(0x3B);
        case 0x4A0:
            FldGate_Clear(0x3E);
            FldGate_Clear(0x3F);
            FldGate_Clear(0x3C);
            FldGate_Clear(61);
        case 0x49E:
        case 0x48C:
            if (func_ov030_020a9d00(arg0, 0x48D) == 0) {
                return 1;
            }
            arg0->unk_21D22 = 3;
            arg0->unk_21D24 = func_ov030_0208a8a0;
            arg0->unk_21D28 = 0x5FB1;
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x49B:
            FldGate_Set(0x42);
        case 0x49D:
        case 0x49C:
            FldGate_Set(37);
            FldEvent_SetCurrent(0x4A1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x4A1:
            arg0->unk_21CF8 = 1;
            break;
        case 0xEA:
            gSaveData.unk_310C = &data_ov030_020d9b44;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0) && (FldGate_Get(0x40) != 0) &&
                (FldGate_Get(0x41) != 0) && (FldGate_Get(0x42) != 0))
            {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0xED);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0xEB:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4E0;
            func_ov030_020af364(13);
            return 1;
        case 0xEC:
            gSaveData.unk_3124 = 0xE;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x797:
            if (FldGate_Get(0x44) != 0) {
                FldEvent_SetCurrent(0x799);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x798);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x798:
            if (FldGate_Get(0x43) == 0) {
                FldGate_Set(0x43);
                gSaveData.unk_30FC = 0;
                gSaveData.unk_30FE = 2;
                gSaveData.unk_3100 = 6;
            }
            break;
        case 0x799:
            FldGate_Set(3);
            FldGate_Set(0x44);
            gSaveData.unk_265A = 4;
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_QUEST_DURABLE_LEATHER);
            func_ov030_020aac28(arg0, PIN_MURASAME);
            arg0->unk_21CF8 = 1;
            break;
        case 0x79A:
            FldEvent_SetCurrent(0x79B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x79B:
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) != 0) {
                FldEvent_SetCurrent(0x79C);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x79C:
            FldGate_Set(4);
            FldGate_Set(0x49);
            gSaveData.unk_265E = 4;
            FldEvent_GrantItem(arg0, ITEM_QUEST_CRIMSON_CHILI);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4DE:
            FldGate_Set(0x45);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x4DF:
            FldGate_Set(0x46);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_BLOCK_SHIKI);
            break;
        case 0x4E0:
            FldGate_Set(0x47);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_SEXY_D);
            FldEvent_GrantItem(arg0, ITEM_STICKER_RETRY_BATTLES);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0x4E1:
            FldGate_Set(0x4B);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0xED:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_MR_DUCKY, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_MR_DUCKY);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208b87c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0xF1;
    gSaveData.unk_3124          = 0x3C;
    func_ov030_020c26bc(0);
    gSaveData.unk_2648 = 4;
    gSaveData.unk_264E = 4;
    gSaveData.unk_2658 = 4;
    gSaveData.unk_265A = 4;
    gSaveData.unk_2664 = 4;
    gSaveData.unk_2662 = 4;
    FldGate_Set(42);
}

s32 func_ov030_0208b97c(s32 arg0) {
    s16 var_r0;
    s16 var_r0_2;
    s16 var_r0_3;
    s16 var_r0_4;

    if ((FldGate_Get(47) != 0) && (FldGate_Get(4) == 0)) {
        if (FldGate_Get(0x33) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_RARE_METAL, 3, 0) != 0) {
                FldGate_Set(0x33);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_RARE_METAL, 3, 0) == 0) {
            FldGate_Set(0x43);
            FldGate_Clear(0x33);
        }
    }
    if ((FldGate_Get(0x31) != 0) && (FldGate_Get(6) == 0)) {
        if (FldGate_Get(0x35) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_MURASAME, 1, 3) != 0) {
                FldGate_Set(0x35);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_MURASAME, 1, 3) == 0) {
            FldGate_Set(0x44);
            FldGate_Clear(0x35);
        }
    }
    if (gSaveData.unk_2458 == 1) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 3) && (FldGate_Get(46) != 0) && (FldGate_Get(0x32) == 0)) {
            FldGate_Set(0x32);
        }
        if ((func_ov030_020aed9c(0x3747) != 0) && (FldGate_Get(12) == 0)) {
            FldGate_Set(12);
            FldEvent_BeginScript(arg0, 0xF5, 0);
            return 1;
        }
        if ((FldGate_Get(2) != 0) && (FldGate_Get(0x36) == 0)) {
            FldGate_Set(0x36);
            FldEvent_BeginScript(arg0, 0x4E8, 0);
            return 1;
        }
        if ((FldGate_Get(0x36) != 0) && (FldGate_Get(13) == 0)) {
            FldGate_Set(13);
            FldEvent_BeginScript(arg0, 0xF6, 0);
            return 1;
        }
        if ((FldGate_Get(8) != 0) && (FldGate_Get(0xE) == 0)) {
            FldGate_Set(0xE);
            FldEvent_BeginScript(arg0, 0xF7, 0);
            return 1;
        }
        if ((FldGate_Get(0x22) != 0) && (FldGate_Get(15) == 0)) {
            FldGate_Set(15);
            FldEvent_BeginScript(arg0, 250, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (FldGate_Get(15) != 0) && (FldGate_Get(0x24) == 0) && (FldGate_Get(0x11) == 0))
        {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0xFD, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (FldGate_Get(15) != 0) && (FldGate_Get(0x24) != 0) &&
            (FldGate_Get(0x11) == 0))
        {
            FldGate_Clear(0x24);
        }
        if ((func_02023588() >= 1U) && (FldGate_Get(18) != 0)) {
            FldGate_Set(0x16);
        }
        if ((func_02023588() >= 2U) && (FldGate_Get(18) != 0)) {
            FldGate_Set(23);
        }
        if ((func_02023588() >= 3U) && (FldGate_Get(18) != 0)) {
            FldGate_Set(0x18);
        }
        if ((func_02023588() >= 3U) && (FldGate_Get(18) != 0) && (FldGate_Get(0x37) == 0)) {
            FldGate_Set(0x37);
            FldEvent_BeginScript(arg0, 0x4E9, 0);
            return 1;
        }
        if ((FldGate_Get(0x37) != 0) && (FldGate_Get(0x13) == 0)) {
            FldGate_Set(0x13);
            FldEvent_BeginScript(arg0, 0x100, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (FldGate_Get(0x15) != 0) && (FldGate_Get(38) == 0))
        {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x104, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (FldGate_Get(0x15) != 0) && (FldGate_Get(38) != 0))
        {
            FldGate_Clear(38);
        }
        if ((FldGate_Get(41) != 0) && (FldGate_Get(0x46) == 0)) {
            FldGate_Set(0x46);
            FldEvent_BeginScript(arg0, 0x10C, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 3) {
        if ((FldGate_Get(0x13) != 0) && (FldGate_Get(0x14) == 0)) {
            FldGate_Set(0x14);
            FldEvent_BeginScript(arg0, 0x101, 0);
            return 1;
        }
        if ((func_02023588() >= 1U) && (FldGate_Get(0x14) != 0)) {
            FldGate_Set(0x19);
        }
        if ((func_02023588() >= 2U) && (FldGate_Get(0x14) != 0)) {
            FldGate_Set(0x1A);
        }
        if ((func_02023588() >= 3U) && (FldGate_Get(0x14) != 0)) {
            FldGate_Set(0x1B);
        }
        if ((func_02023588() >= 3U) && (FldGate_Get(0x14) != 0) && (FldGate_Get(56) == 0)) {
            FldGate_Set(56);
            FldEvent_BeginScript(arg0, 0x4EA, 0);
            return 1;
        }
        if ((FldGate_Get(56) != 0) && (FldGate_Get(0x15) == 0)) {
            FldGate_Set(0x15);
            FldEvent_BeginScript(arg0, 0x102, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 4) != 0) || (func_ov030_020848e4(arg0, 5) != 0)) && (FldGate_Get(0x15) != 0) &&
            (FldGate_Get(37) == 0))
        {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x103, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 4) == 0) && (func_ov030_020848e4(arg0, 5) == 0) && (FldGate_Get(0x15) != 0) &&
            (FldGate_Get(37) != 0))
        {
            FldGate_Clear(37);
        }
    }
    if (gSaveData.unk_2458 == 4) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0_2 = gSaveData.unk_30FC;
        } else {
            var_r0_2 = 0;
        }
        if (((s32)var_r0_2 >= 3) && (FldGate_Get(47) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
        }
        if ((FldGate_Get(0xE) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0xF8, 0);
            return 1;
        }
        if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(0x45) != 0) && (FldGate_Get(1) == 0)) {
            FldGate_Set(1);
            FldEvent_BeginScript(arg0, 0x10D, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0) {
        if ((FldGate_Get(10) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0xFB, 0);
            return 1;
        }
        if ((FldGate_Get(9) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0xF9, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 8) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0_3 = gSaveData.unk_30FC;
        } else {
            var_r0_3 = 0;
        }
        if (((s32)var_r0_3 >= 2) && (FldGate_Get(0x30) != 0) && (FldGate_Get(0x34) == 0)) {
            FldGate_Set(0x34);
        }
    }
    if (gSaveData.unk_2458 == 12) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0_4 = gSaveData.unk_30FC;
        } else {
            var_r0_4 = 0;
        }
        if (((s32)var_r0_4 >= 2) && (FldGate_Get(0x31) != 0) && (FldGate_Get(0x35) == 0)) {
            FldGate_Set(0x35);
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(7);
        }
    }
    if ((gSaveData.unk_2458 == 37) && (FldGate_Get(0x28) != 0) && (FldGate_Get(41) == 0)) {
        FldGate_Set(41);
        FldEvent_BeginScript(arg0, 0x10A, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_0208c3d8(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x3628:
            FldEvent_SetCurrent(0x404);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3629:
            FldEvent_SetCurrent(0x405);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x362A:
            FldEvent_SetCurrent(0x406);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c480(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x3628:
            FldEvent_SetCurrent(0x40A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3629:
            FldEvent_SetCurrent(0x40B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x362A:
            FldEvent_SetCurrent(0x40C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c528(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x3628:
            FldEvent_SetCurrent(0x40A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3629:
            FldEvent_SetCurrent(0x40E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x362A:
            FldEvent_SetCurrent(0x40C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c5d0(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x3628:
            FldEvent_SetCurrent(0x412);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3629:
            FldEvent_SetCurrent(0x413);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x362A:
            FldEvent_SetCurrent(0x414);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c678(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x362C:
            FldGate_Set(0x39);
            FldEvent_SetCurrent(0x7A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x362D:
            FldGate_Set(58);
            FldEvent_SetCurrent(0x7A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x362E:
            FldGate_Set(0x3B);
            FldEvent_SetCurrent(0x7A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c730(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3630:
            FldGate_Set(0x3C);
            FldEvent_SetCurrent(0x7A4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3631:
            FldGate_Set(61);
            FldEvent_SetCurrent(0x7A4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3632:
            FldGate_Set(0x3E);
            FldEvent_SetCurrent(0x7A4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c7e8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3634:
            FldGate_Set(0x3F);
            FldEvent_SetCurrent(0x7A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3635:
            FldGate_Set(0x40);
            FldEvent_SetCurrent(0x7A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3636:
            FldGate_Set(0x41);
            FldEvent_SetCurrent(0x7A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c8a0(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0xF1:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xF2);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xF2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0xF3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0xF3:
            gSaveData.unk_24BE = 0x10E;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(1, 4);
            FldEvent_GrantPin(arg0, PIN_RED_SKULL);
            break;
        case 0xF4:
            FldGate_Set(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0xF5:
            arg0->unk_21CF8 = 1;
            break;
        case 0xF6:
            func_ov030_020aa9f4(arg0, 0x3628, 0x5DD1, 0);
            func_ov030_020aa9f4(arg0, 0x3629, 0x5DD1, 0);
            func_ov030_020aa9f4(arg0, 0x362A, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x401:
        case 0x402:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x402, 0x403, &func_ov030_0208c3d8);
        case 0x404:
            FldGate_Set(8);
            break;
        case 0xF7:
            arg0->unk_21CF8 = 1;
            break;
        case 0x40F:
        case 0x410:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x410, 0x411, &func_ov030_0208c5d0);
        case 0x414:
            FldGate_Set(10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x412:
            FldGate_Set(0x45);
            break;
        case 0x407:
        case 0x408:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x408, 0x409, &func_ov030_0208c480);
        case 0x40B:
        case 0x40D:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x40D, 0x409, &func_ov030_0208c528);
        case 0x40E:
            FldGate_Set(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0xFA:
            gSaveData.unk_264A = 2;
            gSaveData.unk_264E = 2;
            gSaveData.unk_2648 = 2;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            arg0->unk_21CF8                    = 1;
            break;
        case 0xFC:
            FldGate_Set(0x10);
            arg0->unk_21CF8 = 1;
            break;
        case 0xFE:
            FldGate_Set(0x11);
            gSaveData.unk_264A = 4;
            gSaveData.unk_264E = 4;
            gSaveData.unk_2648 = 4;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_SHIKI;
            arg0->unk_21CF8                    = 1;
            break;
        case 0xFF:
            func_02023598(0);
            gSaveData.unk_312C = 1;
            FldGate_Set(18);
            FldEvent_SetCurrent(0x43F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x43F:
            FldEvent_GrantItem(arg0, ITEM_BOOK_BE_A_TRENDSETTER);
            arg0->unk_21CF8 = 1;
            break;
        case 0x100:
            arg0->unk_21CF8 = 1;
            break;
        case 0x101:
            func_02023598(0);
            gSaveData.unk_312C = 2;
            arg0->unk_21CF8    = 1;
            break;
        case 0x102:
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x104:
            gSaveData.unk_2648 = 2;
            gSaveData.unk_264A = 2;
            gSaveData.unk_264E = 2;
            break;
        case 0x106:
            FldGate_Set(0x28);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x107);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x107:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x108);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x108:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x109);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x109:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x10A:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x10B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x10B:
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_SHIKI;
            arg0->unk_21AD0                    = 1;
            arg0->unk_21ACC                    = 0;
            arg0->unk_21AFC                    = 1;
            arg0->unk_21B00                    = 0x66;
            arg0->unk_21B08                    = 0x15D000;
            arg0->unk_21B0C                    = 0xD6000;
            break;
        case 0x10C:
            gSaveData.unk_3124 = 15;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x79D:
            if (FldGate_Get(46) == 0) {
                FldGate_Set(46);
                FldEvent_SetCurrent(0x79E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x79F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x79E:
        case 0x79F:
            FldEvent_SetCurrent(0x7A0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7A0:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c678, 0x362B, 3, 0x362C, 0x362D, 0x362E);
        case 0x7A3:
            FldEvent_SetCurrent(0x7A1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7A1:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c730, 0x362F, 3, 0x3630, 0x3631, 0x3632);
        case 0x7A4:
            FldEvent_SetCurrent(0x7A2);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7A2:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c7e8, 0x3633, 3, 0x3634, 0x3635, 0x3636);
        case 0x7A5:
            if ((FldGate_Get(0x3B) != 0) && (FldGate_Get(0x3C) != 0) && (FldGate_Get(0x41) != 0)) {
                FldEvent_SetCurrent(0x7A6);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7A7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7A7:
            FldGate_Clear(0x39);
            FldGate_Clear(58);
            FldGate_Clear(0x3B);
            FldGate_Clear(0x3C);
            FldGate_Clear(61);
            FldGate_Clear(0x3E);
            FldGate_Clear(0x3F);
            FldGate_Clear(0x40);
            FldGate_Clear(0x41);
            break;
        case 0x7A6:
            FldGate_Set(3);
            FldGate_Set(0x32);
            gSaveData.unk_264A = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantPin(arg0, PIN_ORICHALCUM);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7A8:
            FldEvent_SetCurrent(0x7A9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7A9:
            FldGate_Set(47);
            if (Inventory_HasRequiredQuantity(PIN_RARE_METAL, 3, 0) != 0) {
                FldEvent_SetCurrent(0x7AA);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7AA:
            FldGate_Set(4);
            FldGate_Set(0x33);
            gSaveData.unk_2654 = 4;
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_QUEST_SLEEK_SILK);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7AB:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(7) != 0) && (FldGate_Get(0x30) == 0)) {
                FldEvent_SetCurrent(0x7AF);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(7) != 0)) {
                FldEvent_SetCurrent(0x7AD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(7) == 0)) {
                FldEvent_SetCurrent(0x7AE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7AC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7AC:
            FldGate_Set(0x30);
            break;
        case 0x7AF:
        case 0x7AD:
            FldGate_Set(5);
            FldGate_Set(0x34);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            gSaveData.unk_2666 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7AE:
            FldGate_Set(5);
            FldGate_Set(0x34);
            gSaveData.unk_2666 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7B0:
            FldEvent_SetCurrent(0x7B1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7B1:
            FldGate_Set(0x31);
            if (Inventory_HasRequiredQuantity(PIN_MURASAME, 1, 3) != 0) {
                FldEvent_SetCurrent(0x7B2);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7B2:
            FldGate_Set(6);
            FldGate_Set(0x35);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            FldEvent_GrantItem(arg0, ITEM_QUEST_DURABLE_LEATHER);
            gSaveData.unk_265E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x4E8:
            FldGate_Set(0x36);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_SHIKI);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4E9:
            FldGate_Set(0x37);
            FldEvent_GrantPin(arg0, PIN_500_YEN);
            break;
        case 0x4EA:
            FldGate_Set(0x38);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_SHIKI6_SECRET_BOX_CENTER_ST:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SOLID_SLAMMER, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_SOLID_SLAMMER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208d708(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x111;
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 61;
    gSaveData.unk_264C = 4;
    gSaveData.unk_266E = 4;
    gSaveData.unk_2654 = 4;
    gSaveData.unk_2658 = 4;
    gSaveData.unk_265A = 4;
    gSaveData.unk_265E = 4;
    gSaveData.unk_2664 = 4;
    gSaveData.unk_2662 = 4;
    gSaveData.unk_2666 = 4;
}

s32 func_ov030_0208d80c(FieldEventManager* arg0) {
    if ((FldGate_Get(0x36) != 0) && (FldGate_Get(2) == 0)) {
        if (FldGate_Get(0x37) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) != 0) {
                FldGate_Set(0x37);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) == 0) {
            FldGate_Set(56);
            FldGate_Clear(0x37);
        }
    }
    if ((FldGate_Get(0x39) != 0) && (FldGate_Get(4) == 0)) {
        if (FldGate_Get(58) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) != 0) {
                FldGate_Set(58);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) == 0) {
            FldGate_Set(0x3B);
            FldGate_Clear(58);
        }
    }
    if ((gSaveData.unk_3102 >= 3) && (FldGate_Get(43) != 0) && (FldGate_Get(0x2C) == 0)) {
        FldGate_Set(0x2C);
    }
    if ((gSaveData.unk_2458 == 1) && (FldGate_Get(0x24) != 0) && (FldGate_Get(37) == 0)) {
        FldGate_Set(37);
        gSaveData.unk_3124 = 18;
        gSaveData.unk_3128 = 1;
        arg0->unk_21630    = 4;
        DebugOvlDisp_Pop();
        return 0;
    }
    if ((gSaveData.unk_2458 == 5) && (func_ov030_020848e4(arg0, 7) != 0) && (FldGate_Get(33) == 0) && (FldGate_Get(43) == 0)) {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 0x114, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 0x11) {
        if ((func_ov030_020848a4(0, 0x1EA, 0x190, 0x1F4) != 0) && (FldGate_Get(6) == 0)) {
            FldEvent_BeginScript(arg0, 0x117, 0);
            return 1;
        } else if ((FldGate_Get(0x28) != 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x118, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 18) {
        if (FldGate_Get(0x20) == 0) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x11A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (FldGate_Get(0x23) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
    }
    if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(0x23) == 0) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x11B, 0);
            return 1;
        } else if ((FldGate_Get(41) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x11C, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208dbd8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3638:
            FldGate_Set(45);
            FldEvent_SetCurrent(0x7BC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3639:
            FldGate_Set(46);
            FldEvent_SetCurrent(0x7BC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x363A:
            FldGate_Set(47);
            FldEvent_SetCurrent(0x7BC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208dc90(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x363C:
            FldGate_Set(0x30);
            FldEvent_SetCurrent(0x7BD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x363D:
            FldGate_Set(0x31);
            FldEvent_SetCurrent(0x7BD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x363E:
            FldGate_Set(0x32);
            FldEvent_SetCurrent(0x7BD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208dd48(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3640:
            FldGate_Set(0x33);
            FldEvent_SetCurrent(0x7BE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3641:
            FldGate_Set(0x34);
            FldEvent_SetCurrent(0x7BE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3642:
            FldGate_Set(0x35);
            FldEvent_SetCurrent(0x7BE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ddfc(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x111:
            gSaveData.unk_24BE = 0x11F;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(18, 4);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x112);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x112:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x113);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x113:
            break;
        case 0x7C7:
            if (FldGate_Get(0x2C) != 0) {
                FldEvent_SetCurrent(0x7C9);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7C8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7C8:
            if (FldGate_Get(43) == 0) {
                FldGate_Set(43);
                gSaveData.unk_3102 = 0;
            }
            break;
        case 0x7C9:
            FldGate_Set(5);
            gSaveData.unk_2656 = 4;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x115);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x115:
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x116:
        case 0x117:
            FldGate_Set(6);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4F2;
            func_ov030_020af364(15);
            return 1;
        case 0x118:
            FldEvent_SetCurrent(0x119);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x11B:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x415;
            gSaveData.unk_3124          = 0x10;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x415:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4F3;
            func_ov030_020af364(0xE);
            return 1;
        case 0x11C:
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            gSaveData.unk_24B4                 = 0;
            gSaveData.unk_24B8                 = 1;
            gSaveData.currentStoryEvent        = EVENTFLAG_NOBGM | 0x11D;
            gSaveData.unk_3124                 = 0x11;
            gSaveData.unk_3128                 = 0;
            arg0->unk_21630                    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x11D:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x157000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x7B3:
            FldEvent_SetCurrent(0x7B4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7B4:
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) != FALSE) {
                FldEvent_SetCurrent(EVENT_SHIKI7_104_REAPER);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case EVENT_SHIKI7_104_REAPER:
            FldGate_Set(2);
            FldGate_Set(0x37);
            FldEvent_GrantItem(arg0, ITEM_QUEST_CHOICE_MEAT);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            gSaveData.unk_2648 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7B6:
            if (FldGate_Get(42) == 0) {
                FldGate_Set(42);
                FldEvent_SetCurrent(0x7B7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7B8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7B7:
        case 0x7B8:
            FldEvent_SetCurrent(0x7B9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7B9:
            return func_ov030_020a9f54(arg0, func_ov030_0208dbd8, 0x3637, 3, 0x3638, 0x3639, 0x363A);
        case 0x7BC:
            FldEvent_SetCurrent(0x7BA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7BA:
            return func_ov030_020a9f54(arg0, func_ov030_0208dc90, 0x363B, 3, 0x363C, 0x363D, 0x363E);
        case 0x7BD:
            FldEvent_SetCurrent(0x7BB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7BB:
            return func_ov030_020a9f54(arg0, func_ov030_0208dd48, 0x363F, 3, 0x3640, 0x3641, 0x3642);
        case 0x7BE:
            if ((FldGate_Get(46) != 0) && (FldGate_Get(0x32) != 0) && (FldGate_Get(0x35) != 0)) {
                FldEvent_SetCurrent(EVENT_SHIKI7_SCRAMBLE_REAPER);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7C0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7C0:
            FldGate_Clear(45);
            FldGate_Clear(46);
            FldGate_Clear(47);
            FldGate_Clear(0x30);
            FldGate_Clear(0x31);
            FldGate_Clear(0x32);
            FldGate_Clear(0x33);
            FldGate_Clear(0x34);
            FldGate_Clear(0x35);
            break;
        case EVENT_SHIKI7_SCRAMBLE_REAPER:
            FldGate_Set(3);
            FldGate_Set(0x3C);
            gSaveData.unk_264A = 4;
            FldEvent_GrantItem(arg0, ITEM_QUEST_SWEET_SWEET_ONIONS);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7C4:
            FldEvent_SetCurrent(0x7C5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7C5:
            if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) != FALSE) {
                FldEvent_SetCurrent(EVENT_SHIKI7_SCRAMBLE_MAGNUM);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case EVENT_SHIKI7_SCRAMBLE_MAGNUM:
            FldGate_Set(4);
            FldGate_Set(58);
            FldEvent_GrantItem(arg0, ITEM_QUEST_STUNNING_CRYSTALS);
            FldEvent_GrantItem(arg0, ITEM_QUEST_COLORFUL_THREAD);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            gSaveData.unk_264E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x4F2:
            FldGate_Set(0x28);
            func_ov030_020aac28(arg0, PIN_METEOR_MAGNET);
            break;
        case 0x4F3:
            FldGate_Set(41);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            func_ov030_020aac28(arg0, PIN_EARTHSHAKE);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            break;
        case EVENT_SHIKI7_SECRET_BOX_MOLCO:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_MR_MEW_SUIT, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_MR_MEW_SUIT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208e6e4(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(0);
    gSaveData.unk_264C = 4;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
    gSaveData.unk_24B4                 = 0;
    gSaveData.unk_24B8                 = 1;
    gSaveData.currentStoryEvent        = EVENTFLAG_NOBGM | 0x122;
    gSaveData.unk_3124                 = 0x3E;
}

s32 func_ov030_0208e7e4(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if ((func_ov030_020848e4(arg0, 2) != 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x123, 0);
            return 1;
        } else if ((FldGate_Get(3) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x446, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 3) {
        if ((FldGate_Get(37) == 0) && (FldGate_Get(0x23) != 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x130, 0);
            return 1;
        } else if ((FldGate_Get(0x28) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x131, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 5) {
        if (FldGate_Get(4) == 0) {
            FldGate_Set(4);
            gSaveData.unk_264C = 2;
            FldEvent_BeginScript(arg0, 0x124, 0);
            return 1;
        } else if ((FldGate_Get(39) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x126, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 2) != 0) && (FldGate_Get(4) != 0) && (FldGate_Get(0x24) != 0) &&
            (FldGate_Get(0x23) == 0) && (FldGate_Get(0x22) == 0))
        {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x128, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 2) == 0) {
            FldGate_Clear(0x22);
        }
        if (func_ov030_02084860(0xEB, 0xAF, 0x64) != 0) {
            FldGate_Set(0x24);
        }
        if ((FldGate_Get(0x23) == 0) && (func_ov030_020aed9c(0x3767) != 0)) {
            FldGate_Set(0x23);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x129;
            gSaveData.unk_264C          = 4;
            gSaveData.unk_3124          = 0x13;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        } else if ((FldGate_Get(0x23) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x445, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208eb40(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3644:
            FldEvent_SetCurrent(0x12B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3645:
            FldEvent_SetCurrent(0x12C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ebbc(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3647:
            FldEvent_SetCurrent(0x12E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3648:
            FldEvent_SetCurrent(0x12F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ec3c(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x122:
            gSaveData.unk_24BE = 0x133;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(3, 4);
            break;
        case 0x123:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 5;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x139000;
            arg0->unk_21B0C = 0xE1000;
            break;
        case 0x124:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x125;
            gSaveData.unk_3124          = 0x14;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x125:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4FC;
            func_ov030_020af364(0x10);
            return 1;
        case 0x126:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x129:
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_JOSHUA;
            FldGate_Set(2);
            func_ov030_020aec38(9);
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7CA:
            if (FldGate_Get(0x23) != 0) {
                FldEvent_SetCurrent(0x7CB);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7CB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x12A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x12A:
            return func_ov030_020a9f54(arg0, func_ov030_0208eb40, 0x3643, 2, 0x3644, 0x3645);
        case 0x12B:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x12D;
            func_ov030_020af364(18);
            return 1;
        case 0x12D:
            return func_ov030_020a9f54(arg0, func_ov030_0208ebbc, 0x3646, 2, 0x3647, 0x3648);
        case 0x12E:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x12DU;
            func_ov030_020af364(18);
            return 1;
        case 0x12F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x7CC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7CC:
            FldGate_Set(3);
            gSaveData.unk_2648 = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            func_ov030_020aac28(arg0, PIN_LONG_LIVE_THE_ICE);
            FldEvent_GrantItem(arg0, ITEM_BOOK_CONTROLS_PRESS);
            arg0->unk_21CF8 = 1;
            break;
        case 0x130:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x4FD;
            func_ov030_020af364(0x11);
            return 1;
        case 0x131:
            gSaveData.unk_3124 = 0x15;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4FC:
            FldGate_Set(39);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_JOSHUA);
                FldEvent_GrantItem(arg0, ITEM_STICKER_SIDESTEP);
                FldEvent_GrantItem(arg0, ITEM_STICKER_HIGH_AND_LOW_CARDS);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_JOSHUA1_104:
            FldGate_Set(0x28);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantPin(arg0, PIN_SCARLETITE);
                FldEvent_GrantItem(arg0, ITEM_STICKER_HARD);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_JOSHUA1_SECRET_BOX_HACHIKO:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_POTTED_PLANT, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_POTTED_PLANT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208f160(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(37);
    gSaveData.unk_244C          = 0x15D000;
    gSaveData.unk_2450          = 0xD6000;
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x136;
    gSaveData.unk_3124          = 0x3F;
    func_ov030_020c26bc(0);
    gSaveData.unk_264A = 2;
    gSaveData.unk_264E = 2;
    gSaveData.unk_2658 = 4;
    gSaveData.unk_265A = 4;
    gSaveData.unk_2662 = 4;
    gSaveData.unk_26C4 = 2;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_0208f288(FieldEventManager* arg0) {
    if ((FldGate_Get(55) != 0) && (FldGate_Get(6) == 0)) {
        if (FldGate_Get(56) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) != 0) {
                FldGate_Set(56);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) == 0) {
            FldGate_Set(57);
            FldGate_Clear(56);
        }
    }
    if (gSaveData.unk_2458 == 1) {
        if (FldGate_Get(0x20) == 0) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x137, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (FldGate_Get(33) == 0) &&
            (FldGate_Get(0x20) != 0) && (FldGate_Get(0x22) == 0))
        {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x139, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 3) == 0)) {
            FldGate_Clear(33);
        }
        if ((FldGate_Get(0x22) == 0) && (func_ov030_020aed9c(0x3777) != 0)) {
            FldGate_Set(0x22);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x13A;
            gSaveData.unk_3124          = 0x16;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
        if ((FldGate_Get(0x33) != 0) && (FldGate_Get(7) == 0)) {
            FldGate_Set(7);
            FldEvent_BeginScript(arg0, 0x13E, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 4) && (func_ov030_020848a4(0, 0xC8, 250, 0xF0) != 0) && (FldGate_Get(45) != 0) &&
        (FldGate_Get(8) != 0) && (FldGate_Get(38) == 0))
    {
        FldGate_Set(38);
        FldEvent_BeginScript(arg0, 0x143, 0);
        return 1;
    }
    if ((gSaveData.unk_2458 == 0) && (func_ov030_020848e4(arg0, 0x3C) != 0) && (FldGate_Get(8) != 0) && (FldGate_Get(39) == 0))
    {
        FldGate_Set(39);
        FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x144, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 11) {
        if ((FldGate_Get(10) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x13F, 0);
            return 1;
        }
        if ((FldGate_Get(10) != 0) && (FldGate_Get(11) != 0) && (FldGate_Get(46) == 0)) {
            FldGate_Set(46);
        }
        if ((func_ov030_020848e4(arg0, 0xE) != 0) && (FldGate_Get(3) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x140, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 12) {
        if ((func_ov030_020848e4(arg0, 13) != 0) && (FldGate_Get(0x24) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x141, 0);
            return 1;
        }
        if ((FldGate_Get(0x31) != 0) && (FldGate_Get(8) == 0)) {
            FldGate_Set(8);
            FldEvent_BeginScript(arg0, 0x142, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 8) {
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (FldGate_Get(0x28) == 0)) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x145, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (FldGate_Get(0x28) != 0) && (FldGate_Get(41) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((FldGate_Get(9) != 0) && (FldGate_Get(43) == 0)) {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, EVENT_JOSHUA2_TIN_PIN, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 11) != 0) || (func_ov030_020848e4(arg0, 13) != 0)) && (FldGate_Get(43) != 0) &&
            (FldGate_Get(0x2C) == 0))
        {
            FldGate_Set(0x2C);
            FldEvent_BeginScript(arg0, 0x153, 0);
            return 1;
        }
        if ((FldGate_Get(0x34) != 0) && (FldGate_Get(0x35) == 0)) {
            FldGate_Set(0x35);
            gSaveData.unk_3124 = 0x19;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }

    if (gSaveData.unk_2458 == 37) {
        if ((FldGate_Get(0x22) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
            FldEvent_BeginScript(arg0, 0x13B, 0);
            return 1;
        }
        if ((FldGate_Get(0x28) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x146, 0);
            return 1;
        }
        if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(0x34) == 0)) {
            FldGate_Set(0x34);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x154;
            gSaveData.unk_3124          = 0x18;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }
    return 0;
}

s32 func_ov030_0208fa28(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x364A:
            FldEvent_SetCurrent(0x14A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x364B:
            FldEvent_SetCurrent(0x14B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208faa4(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x136:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x16F000;
            arg0->unk_21B0C = 0xE1000;
            break;
        case 0x137:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x13A:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x13B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x13C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x13C:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x13D;
            gSaveData.unk_3124          = 23;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x13D:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x16F000;
            arg0->unk_21B0C = 0xE1000;
            break;
        case 0x13E:
            gSaveData.unk_24BE = 0x157;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(8, 4);
            gSaveData.unk_264A = 4;
            gSaveData.unk_264E = 4;
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_JOSHUA;
            func_ov030_020aec38(9);
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x13F:
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case 0x140:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 12;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x32000;
            arg0->unk_21B0C = 0xFA000;
            break;
        case 0x141:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x506U;
            func_ov030_020af364(0x13);
            return 1;
        case 0x142:
            gSaveData.unk_2654 = 4;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x144:
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(EVENT_JOSHUA2_SECRET_BOX_SPAIN_HILL);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x146:
            FldEvent_SetCurrent(0x147);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x147:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x148;
            gSaveData.unk_24C2 = 0x149;
            gSaveData.unk_341C = 1;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x148:
        case 0x149:
            return func_ov030_020a9f54(arg0, func_ov030_0208fa28, 0x3649, 2, 0x364A, 0x364B);
        case 0x14A:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x148;
            gSaveData.unk_24C2 = 0x149;
            gSaveData.unk_341C = 1;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x14B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x14C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x14C:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x14D;
            gSaveData.unk_24C2 = 0x14E;
            gSaveData.unk_341C = 0;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x14D:
        case 0x14E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x14F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x14F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x151);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x151:
            FldGate_Set(9);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        case EVENT_JOSHUA2_TIN_PIN:
            gSaveData.unk_26C4 = 4;
            FldEvent_GrantItem(arg0, ITEM_BOOK_TIN_PIN_SLAMMER);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_CUSTOM);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_FIRE);
            break;
        case 0x153:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x154:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x155);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x155:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x126000;
            arg0->unk_21B0C = 0x17E000;
            break;
        case 0x7CD:
            FldGate_Set(45);
            break;
        case 0x7D0:
            if (FldGate_Get(46) != 0) {
                FldEvent_SetCurrent(0x7D2);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7D1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7D1:
            if (FldGate_Get(4) == 0) {
                FldGate_Set(4);
                arg0->unk_21CF8 = 1;
            }
            break;
        case 0x7D2:
            FldGate_Set(3);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            gSaveData.unk_2664 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7D3:
            if (FldGate_Get(0x22) != 0) {
                FldEvent_SetCurrent(0x7D4);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7D4:
            FldGate_Set(47);
            if (func_02023f60(1, 10) != 0) {
                FldEvent_SetCurrent(EVENT_JOSHUA2_SCRAMBLE_NP_JOSHUA);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case EVENT_JOSHUA2_SCRAMBLE_NP_JOSHUA:
            FldGate_Set(5);
            FldGate_Set(0x30);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            FldEvent_GrantItem(arg0, ITEM_QUEST_EXTRA_FINE_NOODLES);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7D6:
            FldEvent_SetCurrent(0x7D7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7D7:
            FldGate_Set(0x37);
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) != 0) {
                FldEvent_SetCurrent(EVENT_JOSHUA2_MOLCO_REAPER);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case EVENT_JOSHUA2_MOLCO_REAPER:
            FldGate_Set(6);
            FldGate_Set(56);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            FldEvent_GrantItem(arg0, ITEM_QUEST_SPARKLING_SILVER);
            gSaveData.unk_265E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x506:
            FldGate_Set(0x31);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_ICHIMONJI);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            break;
        case EVENT_JOSHUA2_SECRET_BOX_SPAIN_HILL:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SUPERMASKS_MASK, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_SUPERMASKS_MASK);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209041c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26BE = 0;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    gSaveData.unk_26D4 = 2;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 0;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = 0x15A;
    func_ov030_020c26bc(4);
    gSaveData.unk_2648 = 0;
    gSaveData.unk_264C = 0;
    gSaveData.unk_2652 = 0;
    gSaveData.unk_2678 = 0;
    gSaveData.unk_2666 = 0;
    gSaveData.unk_266A = 0;
    gSaveData.unk_265C = 0;
    gSaveData.unk_2676 = 0;
    gSaveData.unk_2670 = 0;
    gSaveData.unk_3124 = 0x40;
}

s32 func_ov030_02090520(FieldEventManager* arg0) {
    if ((FldGate_Get(0x59) != 0) && (FldGate_Get(4) == 0)) {
        if (FldGate_Get(0x5A) == 0) {
            if (func_ov030_020aaad8() == 10) {
                FldGate_Set(0x5A);
            }
        } else {
            s32 var_r0;
            if (func_ov030_020aaad8() == 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            if (var_r0 == 10) {
                FldGate_Set(0x5B);
                FldGate_Clear(0x5A);
            }
        }
    }
    if ((FldGate_Get(0x48) != 0) && (FldGate_Get(6) == 0)) {
        if (FldGate_Get(0x5C) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) != 0) {
                FldGate_Set(0x5C);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) == 0) {
            FldGate_Set(0x5D);
            FldGate_Clear(0x5C);
        }
    }
    if ((FldGate_Get(0x5E) != 0) && (FldGate_Get(7) == 0)) {
        if (FldGate_Get(0x5F) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) != 0) {
                FldGate_Set(0x5F);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) == 0) {
            FldGate_Set(0x60);
            FldGate_Clear(0x5F);
        }
    }
    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(9) != 0) && (FldGate_Get(10) == 0)) {
            FldGate_Set(10);
            FldEvent_BeginScript(arg0, 0x172, 0);
            return 1;
        }
        if ((FldGate_Get(46) != 0) && (FldGate_Get(47) == 0)) {
            FldGate_Clear(46);
        }
    }
    if (gSaveData.unk_2458 == 4) {
        if ((func_ov030_020848e4(arg0, 6) != 0) && (FldGate_Get(0x20) == 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x15B, 0);
            return 1;
        }
        if ((FldGate_Get(10) != 0) && (FldGate_Get(47) == 0) && (FldGate_Get(46) == 0)) {
            FldGate_Set(46);
            FldEvent_BeginScript(arg0, 0x173, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 5) && (FldGate_Get(10) != 0) && (FldGate_Get(47) == 0) && (FldGate_Get(46) == 0)) {
        FldGate_Set(46);
        FldEvent_BeginScript(arg0, 0x173, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 6) {
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(9) != 0) && (FldGate_Get(12) == 0) &&
            (FldGate_Get(0x49) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(12) == 0) && (FldGate_Get(0x49) != 0) &&
            (FldGate_Get(0x4B) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (FldGate_Get(12) == 0) && (FldGate_Get(0x4B) != 0)) {
            FldGate_Clear(0x4B);
        }
    }
    if ((gSaveData.unk_2458 == 11) && (FldGate_Get(10) != 0) && (FldGate_Get(47) == 0) && (FldGate_Get(46) == 0)) {
        FldGate_Set(46);
        FldEvent_BeginScript(arg0, 0x173, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 12) {
        if (FldGate_Get(33) == 0) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x15C, 0);
            return 1;
        }
        s32 var_r0;
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 3) && (FldGate_Get(0x46) != 0) && (FldGate_Get(0x47) == 0)) {
            FldGate_Set(0x47);
        }
        if ((FldGate_Get(5) != 0) && (FldGate_Get(0x63) == 0)) {
            FldGate_Set(0x63);
            FldEvent_BeginScript(arg0, 0x447, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 13) {
        if ((func_ov030_020848e4(arg0, 15) != 0) && (FldGate_Get(42) != 0) && (FldGate_Get(43) == 0)) {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, 0x166, 0);
            return 1;
        }
        if ((FldGate_Get(0x56) != 0) && (FldGate_Get(9) == 0)) {
            FldGate_Set(9);
            FldEvent_BeginScript(arg0, 0x167, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x13) {
        if ((FldGate_Get(10) != 0) && (FldGate_Get(47) == 0)) {
            FldGate_Set(47);
            gSaveData.unk_26D4 = 2;
            FldEvent_BeginScript(arg0, 0x174, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(47) != 0) && (FldGate_Get(0x30) == 0)) {
            FldGate_Set(0x30);
            FldGate_Set(0x31);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x175, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(11) != 0) && (FldGate_Get(0x31) == 0) &&
            (FldGate_Get(0x35) == 0))
        {
            FldGate_Set(0x31);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x177, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (FldGate_Get(11) != 0) && (FldGate_Get(0x35) == 0)) {
            FldGate_Clear(0x31);
        }
        if ((FldGate_Get(0x30) != 0) && (FldGate_Get(11) == 0)) {
            FldGate_Set(11);
            FldEvent_BeginScript(arg0, 0x176, 0);
            return 1;
        }
        if ((FldGate_Get(2) != 0) && (FldGate_Get(0x57) == 0)) {
            FldGate_Set(0x57);
            FldEvent_BeginScript(arg0, 0x511, 0);
            return 1;
        }
        if ((FldGate_Get(0x57) != 0) && (FldGate_Get(0x32) == 0)) {
            FldGate_Set(0x32);
            FldEvent_BeginScript(arg0, 0x178, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(0x32) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x179, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(0x32) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x179, 0);
            return 1;
        }
        if ((FldGate_Get(0x35) != 0) && (FldGate_Get(0x36) == 0)) {
            FldGate_Set(0x36);
            FldEvent_BeginScript(arg0, 0x17C, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(0x35) != 0) && (FldGate_Get(12) == 0)) {
            FldGate_Set(12);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x17D, 0);
            return 1;
        }
        if ((FldGate_Get(12) != 0) && (FldGate_Get(13) == 0)) {
            FldGate_Set(13);
            FldEvent_BeginScript(arg0, 0x17E, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (FldGate_Get(13) != 0) && (FldGate_Get(56) == 0)) {
            FldGate_Set(0x37);
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(13) != 0) && (FldGate_Get(0x37) != 0) &&
            (FldGate_Get(56) == 0))
        {
            FldGate_Set(56);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x17F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(56) != 0) && (FldGate_Get(0x31) == 0)) {
            FldGate_Set(0x31);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x177, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (FldGate_Get(56) != 0) && (FldGate_Get(0x31) != 0)) {
            FldGate_Clear(0x31);
        }
        if ((FldGate_Get(0x10) != 0) && (FldGate_Get(61) == 0)) {
            FldGate_Set(61);
            FldEvent_BeginScript(arg0, 0x18A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(0x11) != 0)) {
            if ((FldGate_Get(0x42) != 0) && (FldGate_Get(0x3E) == 0)) {
                FldGate_Set(0x3E);
                FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x18B, 0);
                return 1;
            }
            if ((FldGate_Get(0x43) != 0) && (FldGate_Get(0x39) == 0)) {
                FldGate_Set(0x39);
                FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x180, 0);
                return 1;
            }
            if ((FldGate_Get(0x44) != 0) && (FldGate_Get(0x3B) == 0)) {
                FldGate_Set(0x3B);
                FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x183, 0);
                return 1;
            }
        }
        if (FldGate_Get(0x3E) != 0) {
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x18C, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x15) {
        if ((FldGate_Get(13) != 0) && (FldGate_Get(0x3C) == 0)) {
            FldGate_Set(0x3C);
            FldEvent_BeginScript(arg0, 0x187, 0);
            return 1;
        }
        if ((FldGate_Get(3) != 0) && (FldGate_Get(0x58) == 0)) {
            FldGate_Set(0x58);
            FldEvent_BeginScript(arg0, 0x512, 0);
            return 1;
        }
        if ((FldGate_Get(0x58) != 0) && (FldGate_Get(0x10) == 0)) {
            FldGate_Set(0x10);
            FldEvent_BeginScript(arg0, 0x189, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 0xE) && (FldGate_Get(0x2C) != 0) && (FldGate_Get(45) == 0)) {
        gSaveData.unk_26D2 = 4;
        FldGate_Set(45);
    }
    if (gSaveData.unk_2458 == 0x10) {
        if (FldGate_Get(0x22) == 0) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x15D, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x15E, 0);
            return 1;
        }
        if ((FldGate_Get(0x23) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x15F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(9) != 0) && (FldGate_Get(0x2C) == 0) &&
            (FldGate_Get(45) == 0))
        {
            FldGate_Set(0x2C);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x16C, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(73) == 0) {
            FldGate_Set(73);
            FldGate_Set(75);
            FldEvent_BeginScript(arg0, 0x18D, 0);
            return 1;
        }
        if ((FldGate_Get(73) != 0) && (FldGate_Get(75) == 0)) {
            FldGate_Set(75);
            FldEvent_BeginScript(arg0, 0x192, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209147c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3651:
            FldGate_Set(38);
            FldEvent_SetCurrent(0x161);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3652:
            FldGate_Set(39);
            FldEvent_SetCurrent(0x162);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3653:
            FldGate_Set(0x28);
            FldEvent_SetCurrent(0x163);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3654:
            FldGate_Set(41);
            FldEvent_SetCurrent(0x164);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091554(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3651:
            FldGate_Set(38);
            FldEvent_SetCurrent(0x161);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3652:
            FldGate_Set(39);
            FldEvent_SetCurrent(0x162);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3653:
            FldGate_Set(0x28);
            FldEvent_SetCurrent(0x163);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3654:
            FldGate_Set(41);
            FldEvent_SetCurrent(0x164);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3655:
            if (FldGate_Get(42) == 0) {
                FldGate_Set(42);
                FldEvent_SetCurrent(0x165);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
    }
    return 0;
}

s32 func_ov030_02091664(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3657:
            FldEvent_SetCurrent(0x16E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3658:
            FldEvent_SetCurrent(0x16F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3659:
            FldEvent_SetCurrent(0x170);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x365A:
            FldEvent_SetCurrent(0x171);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209171c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x365C:
            FldEvent_SetCurrent(0x18E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x365D:
            FldEvent_SetCurrent(0x18F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209179c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x365F:
            FldGate_Set(0x4D);
            FldEvent_SetCurrent(0x7ED);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3660:
            FldGate_Set(0x4E);
            FldEvent_SetCurrent(0x7ED);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3661:
            FldGate_Set(0x4F);
            FldEvent_SetCurrent(0x7ED);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02091854(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3663:
            FldGate_Set(0x50);
            FldEvent_SetCurrent(0x7EE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3664:
            FldGate_Set(0x51);
            FldEvent_SetCurrent(0x7EE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3665:
            FldGate_Set(0x52);
            FldEvent_SetCurrent(0x7EE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209190c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3667:
            FldGate_Set(0x53);
            FldEvent_SetCurrent(0x7EF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3668:
            FldGate_Set(0x54);
            FldEvent_SetCurrent(0x7EF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3669:
            FldGate_Set(0x55);
            FldEvent_SetCurrent(0x7EF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020919c4(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36EB:
            FldEvent_SetCurrent(0x51);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36EC:
            FldEvent_SetCurrent(0x52);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36ED:
            FldEvent_SetCurrent(0x53);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36EE:
            FldEvent_SetCurrent(0x54);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36EF:
            FldEvent_SetCurrent(0x55);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091a8c(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x364C:
            FldGate_Set(0x3F);
            FldGate_Set(0x42);
            FldEvent_SetCurrent(0x419);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x364D:
            if (FldGate_Get(0x40) == 0) {
                FldGate_Set(0x40);
                FldGate_Set(0x43);
                FldEvent_SetCurrent(0x41A);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x41C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x364E:
            if (FldGate_Get(0x41) == 0) {
                FldGate_Set(0x41);
                FldGate_Set(0x44);
                FldEvent_SetCurrent(0x41B);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x41C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x364F:
            FldEvent_SetCurrent(0x41C);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091bc8(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x15A:
            func_ov030_020aeaa0(0x10, 4);
            break;
        case 0x15E:
            func_ov030_020aeab8(0x10, 4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x15F:
            func_ov030_020aeaa0(0x13, 2);
            func_ov030_020aeaa0(6, 2);
            func_ov030_020aeaa0(0, 2);
            func_ov030_020aeaa0(8, 2);
            FldEvent_SetCurrent(0x160);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x160:
        case 0x161:
        case 0x162:
        case 0x163:
        case 0x164:
            if ((FldGate_Get(38) != 0) && (FldGate_Get(39) != 0) && (FldGate_Get(0x28) != 0) && (FldGate_Get(41) != 0)) {
                return func_ov030_020a9f54(arg0, &func_ov030_02091554, 0x3650, 5, 0x3651, 0x3652, 0x3653, 0x3654, 0x3655);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_0209147c, 0x3650, 4, 0x3651, 0x3652, 0x3653, 0x3654);
        case 0x165:
            FldGate_Set(0x45);
            break;
        case 0x166:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x510U;
            func_ov030_020af364(0x14);
            return 1;
        case 0x167:
            FldGate_Set(9);
            gSaveData.unk_26BE = 2;
            break;
        case 0x168:
            FldGate_Set(18);
            func_ov030_020aeab8(8, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x169:
            FldGate_Set(0x13);
            func_ov030_020aeab8(0, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16A:
            FldGate_Set(0x14);
            func_ov030_020aeab8(6, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16B:
            FldGate_Set(0x15);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16C:
            gSaveData.unk_26D2 = 0;
            break;
        case 0x16D:
            FldGate_Set(0x16);
            return func_ov030_020a9f54(arg0, &func_ov030_02091664, 0x3656, 4, 0x3657, 0x3658, 0x3659, 0x365A);
        case 0x16E:
        case 0x16F:
        case 0x170:
        case 0x171:
            arg0->unk_21CF8 = 1;
            break;
        case 0x176:
            FldGate_Set(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0x178:
            gSaveData.unk_26D8 = 2;
            arg0->unk_21CF8    = 1;
            break;
        case 0x179:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x17A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x17A:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x17B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x17B:
            FldGate_Set(0x35);
            gSaveData.unk_26D8 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x17E:
            gSaveData.unk_26BE = 0;
            gSaveData.unk_26D8 = 0;
            break;
        case 0x17F:
            FldGate_Set(56);
            func_ov030_020aa9f4(arg0, 0x364D, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x180:
        case 0x181:
        case 0x183:
            FldGate_Clear(0x11);
            if (FldGate_Get(58) == 0) {
                FldGate_Set(58);
                FldEvent_SetCurrent(0x181);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(EVENT_JOSHUA3_IMPRINT_YELLOW);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case EVENT_JOSHUA3_IMPRINT_YELLOW:
            if ((FldGate_Get(0x3B) != 0) && (FldGate_Get(0x61) == 0)) {
                FldGate_Set(0x61);
                func_ov030_020aac28(arg0, PIN_I_LIVE_FOR_FOOD);
            }
            gSaveData.unk_26D8 = 0;
            arg0->unk_21CF8    = 1;
            break;
        case 0x184:
            FldGate_Set(0xE);
            func_ov030_020aa9f4(arg0, 0x364E, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x185:
            FldGate_Set(23);
            func_ov030_020aa9f4(arg0, 0x364F, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x416:
        case 0x417:
            return func_ov030_020aa0c8(arg0, 0xA9, 0x417, 0x418, &func_ov030_02091a8c);
        case 0x419:
        case 0x41A:
        case 0x41B:
            FldGate_Set(0x11);
            gSaveData.unk_26D8 = 2;
            arg0->unk_21CF8    = 1;
            break;
        case 0x188:
            FldGate_Set(15);
            arg0->unk_21CF8 = 1;
            break;
        case 0x189:
            func_ov030_020aa9f4(arg0, 0x364C, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x18C:
            gSaveData.unk_3124 = 0x1A;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x18D:
            FldGate_Set(0x49);
        case 0x192:
            return func_ov030_020a9f54(arg0, &func_ov030_0209171c, 0x365B, 2, 0x365C, 0x365D);
        case 0x18E:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x190;
            gSaveData.unk_24C2 = 0x191;
            gSaveData.unk_341C = 0x20;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x190:
            if (FldGate_Get(0x4A) == 0) {
                FldGate_Set(0x4A);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_CUSTOM);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
        case 0x18F:
        case 0x191:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
            return func_ov030_020a9f54(arg0, &func_ov030_020919c4, 0x36EA, 5, 0x36EB, 0x36EC, 0x36ED, 0x36EE, 0x36EF);
        case 0x7D9:
            if (FldGate_Get(0x45) != 0) {
                FldEvent_SetCurrent(0x7DA);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7DA:
            FldGate_Set(0x59);
            if (func_ov030_020aaad8() == 10) {
                FldEvent_SetCurrent(EVENT_JOSHUA3_SCRAMBLE_NP_TREND);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case EVENT_JOSHUA3_SCRAMBLE_NP_TREND:
            FldGate_Set(4);
            FldGate_Set(0x5A);
            FldEvent_GrantItem(arg0, ITEM_QUEST_FLUFFY_COTTON);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            gSaveData.unk_2648 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7DC:
            if (FldGate_Get(0x47) != 0) {
                FldEvent_SetCurrent(0x7DE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7DD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7DD:
            if (FldGate_Get(0x46) == 0) {
                FldGate_Set(0x46);
                gSaveData.unk_30FC = 0;
                gSaveData.unk_30FE = 1;
                gSaveData.unk_3100 = 12;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x7DE:
            FldGate_Set(5);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            func_ov030_020aac28(arg0, PIN_KONOHANA_SAKUYA);
            FldEvent_GrantItem(arg0, ITEM_STICKER_PHONE_CAMERA);
            gSaveData.unk_2666 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7DF:
            FldEvent_SetCurrent(0x7E0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7E0:
            FldGate_Set(0x48);
            if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) != 0) {
                FldEvent_SetCurrent(0x7E1);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7E1:
            FldGate_Set(6);
            FldGate_Set(0x5C);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            gSaveData.unk_266A = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7E2:
            FldEvent_SetCurrent(0x7E3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7E3:
            FldGate_Set(0x5E);
            if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) != 0) {
                FldEvent_SetCurrent(0x7E4);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7E4:
            FldGate_Set(7);
            FldGate_Set(0x5F);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_HAMBURGER);
            gSaveData.unk_2678 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7E5:
            if (FldGate_Get(0x45) != 0) {
                FldEvent_SetCurrent(0x7E7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7E6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7E7:
            if (FldGate_Get(0x4C) == 0) {
                FldGate_Set(0x4C);
                FldEvent_SetCurrent(0x7E8);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7E9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7E9:
        case 0x7E8:
            FldEvent_SetCurrent(0x7EA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7EA:
            return func_ov030_020a9f54(arg0, &func_ov030_0209179c, 0x365E, 3, 0x365F, 0x3660, 0x3661);
        case 0x7ED:
            FldEvent_SetCurrent(0x7EB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7EB:
            return func_ov030_020a9f54(arg0, &func_ov030_02091854, 0x3662, 3, 0x3663, 0x3664, 0x3665);
        case 0x7EE:
            FldEvent_SetCurrent(0x7EC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7EC:
            return func_ov030_020a9f54(arg0, &func_ov030_0209190c, 0x3666, 3, 0x3667, 0x3668, 0x3669);
        case 0x7EF:
            if ((FldGate_Get(0x4D) != 0) && (FldGate_Get(0x52) != 0) && (FldGate_Get(0x54) != 0)) {
                FldEvent_SetCurrent(EVENT_JOSHUA3_SCRAMBLE_REAPER);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7F1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7F1:
            FldGate_Clear(0x4D);
            FldGate_Clear(0x4E);
            FldGate_Clear(0x4F);
            FldGate_Clear(0x50);
            FldGate_Clear(0x51);
            FldGate_Clear(0x52);
            FldGate_Clear(0x53);
            FldGate_Clear(0x54);
            FldGate_Clear(0x55);
            break;
        case EVENT_JOSHUA3_SCRAMBLE_REAPER:
            FldGate_Set(8);
            FldGate_Set(0x62);
            gSaveData.unk_264C = 4;
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            FldEvent_GrantItem(arg0, ITEM_QUEST_COZY_DOWN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x510:
            FldGate_Set(0x56);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_OCTO_SQUEEZE);
            break;
        case 0x511:
            FldGate_Set(0x57);
            func_ov030_020aac28(arg0, PIN_CRACKLE_POP_BARRIER);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case 0x512:
            FldGate_Set(0x58);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantPin(arg0, PIN_SCARLETITE);
                FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_JOSHUA3_SECRET_BOX_CONCERT_STAGE:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_PIRATE_HAT, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_PIRATE_HAT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02092c4c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26BE = 2;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x197;
    func_ov030_020c26bc(4);
    gSaveData.unk_264E = 0;
    gSaveData.unk_2656 = 0;
    gSaveData.unk_2654 = 0;
    gSaveData.unk_2658 = 0;
    gSaveData.unk_2676 = 0;
    gSaveData.unk_2670 = 0;
    gSaveData.unk_3124 = 0x41;
}

s32 func_ov030_02092d50(FieldEventManager* arg0) {
    s16 var_r0;

    if ((FldGate_Get(56) != 0) && (FldGate_Get(2) == 0)) {
        if (FldGate_Get(61) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) != 0) {
                FldGate_Set(61);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) == 0) {
            FldGate_Set(0x3E);
            FldGate_Clear(61);
        }
    }
    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(11) != 0) && (FldGate_Get(45) == 0)) {
            FldGate_Set(45);
            FldEvent_BeginScript(arg0, 0x1A7, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 2) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) &&
            (FldGate_Get(12) != 0) && (FldGate_Get(5) == 0))
        {
            FldGate_Set(5);
            FldEvent_BeginScript(arg0, 0x1AC, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 5) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 10) && (FldGate_Get(0x3B) != 0) && (FldGate_Get(0x3C) == 0)) {
            FldGate_Set(0x3C);
        }
    }
    if ((gSaveData.unk_2458 == 2) && (gSaveData.unk_3102 >= 4) && (FldGate_Get(0x39) != 0) && (FldGate_Get(58) == 0)) {
        FldGate_Set(58);
    }
    if (gSaveData.unk_2458 == 6) {
        if (FldGate_Get(39) == 0) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x19A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (FldGate_Get(9) == 0)) {
            FldGate_Set(9);
            FldEvent_BeginScript(arg0, 0x19B, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (FldGate_Get(10) != 0) && (FldGate_Get(0x28) == 0) &&
            (FldGate_Get(15) == 0))
        {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x1A0, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) == 0) && (FldGate_Get(10) != 0) && (FldGate_Get(0x28) != 0)) {
            FldGate_Clear(0x28);
        }
        if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0)) && (FldGate_Get(10) != 0) &&
            (FldGate_Get(41) == 0) && (FldGate_Get(11) == 0))
        {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x1A1, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 8) == 0) && (func_ov030_020848e4(arg0, 9) == 0) && (FldGate_Get(10) != 0) &&
            (FldGate_Get(41) != 0))
        {
            FldGate_Clear(41);
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(0x35) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(0x35) != 0) && (FldGate_Get(0x37) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (FldGate_Get(0x37) != 0)) {
            FldGate_Clear(0x37);
        }
    }
    if ((gSaveData.unk_2458 == 0) && (FldGate_Get(11) != 0) && (FldGate_Get(47) == 0)) {
        FldGate_Set(47);
        FldEvent_BeginScript(arg0, 0x1AD, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 7) {
        if (FldGate_Get(0x22) == 0) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x1D0, 0);
            return 1;
        }
        if ((FldGate_Get(0x34) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x1D1, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 8) && (FldGate_Get(11) != 0) && (FldGate_Get(33) == 0)) {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 0x1B2, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 9) {
        if (FldGate_Get(37) == 0) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x1D3, 0);
            return 1;
        }
        if ((func_ov030_020848a4(0x96, 0x15E, 0x190, 0x186) != 0) && (FldGate_Get(37) != 0) && (FldGate_Get(6) == 0)) {
            FldGate_Set(6);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x1D4, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 12) != 0) && (FldGate_Get(7) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x1D6, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 12) == 0) && (FldGate_Get(7) != 0) && (FldGate_Get(38) != 0)) {
            FldGate_Clear(38);
        }
        if ((func_ov030_020aed9c(0x3797) != 0) && (FldGate_Get(8) == 0)) {
            FldGate_Set(8);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x1D7;
            gSaveData.unk_3124          = 0x1B;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }
    if ((gSaveData.unk_2458 == 8) && (FldGate_Get(11) != 0) && (FldGate_Get(33) == 0)) {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 0x1B2, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 13) && (FldGate_Get(0x32) == 0)) {
        FldGate_Set(0x32);
        FldEvent_BeginScript(arg0, 0x1CC, 0);
        return 1;
    }
    if ((gSaveData.unk_2458 == 0x10) && (func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(0xE) != 0) &&
        (FldGate_Get(0x33) == 0))
    {
        FldGate_Set(0x33);
        FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x1CD, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(0x35) == 0) {
            FldGate_Set(0x35);
            FldGate_Set(0x37);
            FldEvent_BeginScript(arg0, 0x1D8, 0);
            return 1;
        } else if ((FldGate_Get(0x35) != 0) && (FldGate_Get(0x37) == 0)) {
            FldGate_Set(0x37);
            FldEvent_BeginScript(arg0, 0x1DD, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020936b8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366B:
            FldEvent_SetCurrent(0x19D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x366C:
            FldEvent_SetCurrent(0x19E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093738(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            FldGate_Set(42);
            FldEvent_SetCurrent(0x1A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x366F:
            FldGate_Set(43);
            FldEvent_SetCurrent(0x1A4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020937c4(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            FldGate_Set(42);
            FldEvent_SetCurrent(0x1A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x366F:
            FldGate_Set(43);
            FldEvent_SetCurrent(0x1A4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3670:
            FldGate_Set(0x2C);
            FldEvent_SetCurrent(0x1A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093880(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            FldGate_Set(42);
            FldEvent_SetCurrent(0x1A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x366F:
            FldGate_Set(43);
            FldEvent_SetCurrent(0x1A4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3670:
            FldGate_Set(0x2C);
            FldEvent_SetCurrent(0x1A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3671:
            FldGate_Set(11);
            FldEvent_SetCurrent(0x1A6);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02093958(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3673:
            FldEvent_SetCurrent(0x1A9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3674:
            if (FldGate_Get(0xE) != 0) {
                FldGate_Set(12);
                FldEvent_SetCurrent(0x1AB);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x1AA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093a0c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3676:
            FldGate_Set(0x30);
            FldEvent_SetCurrent(0x1AF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3677:
            FldGate_Set(0x31);
            FldEvent_SetCurrent(0x1B0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093a98(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3679:
            FldEvent_SetCurrent(0x1B5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x367A:
            FldEvent_SetCurrent(0x1B6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x367B:
            FldEvent_SetCurrent(0x1B7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093b40(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x367E:
            FldEvent_SetCurrent(0x1BB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x367F:
            FldEvent_SetCurrent(0x1BC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3680:
            FldEvent_SetCurrent(0x1BD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093be0(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3682:
            FldEvent_SetCurrent(0x1BF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3683:
            FldEvent_SetCurrent(0x1C0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3684:
            FldEvent_SetCurrent(0x1BF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093c80(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3686:
            FldEvent_SetCurrent(0x1C5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3687:
            FldEvent_SetCurrent(0x1C5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3688:
            FldEvent_SetCurrent(0x1C5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3689:
            FldEvent_SetCurrent(0x1C6);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02093d38(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x368B:
            FldEvent_SetCurrent(0x1D9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x368C:
            FldEvent_SetCurrent(0x1DA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093db8(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x197:
            func_ov030_020aeaa0(6, 2);
            func_ov030_020aeaa0(9, 2);
            func_ov030_020aeaa0(13, 2);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x198);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x198:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x199);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x199:
            FldGate_Set(0x20);
            break;
        case 0x19B:
        case 0x19C:
            FldGate_Set(9);
            return func_ov030_020a9f54(arg0, func_ov030_020936b8, 0x366A, 2, 0x366B, 0x366C);
        case 0x19D:
        case 0x19E:
            gSaveData.unk_265C = 0;
            arg0->unk_21CF8    = 1;
            break;
        case 0x19F:
            FldGate_Set(10);
            gSaveData.unk_2658 = 2;
            gSaveData.unk_265A = 2;
            FldEvent_GrantPin(arg0, PIN_5_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1A2:
        case 0x1A3:
        case 0x1A4:
        case 0x1A5:
            if ((FldGate_Get(42) != 0) && (FldGate_Get(43) != 0) && (FldGate_Get(0x2C) != 0)) {
                return func_ov030_020a9f54(arg0, func_ov030_02093880, 0x366D, 4, 0x366E, 0x366F, 0x3670, 0x3671);
            }
            if (FldGate_Get(43) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_020937c4, 0x366D, 3, 0x366E, 0x366F, 0x3670);
            }
            return func_ov030_020a9f54(arg0, func_ov030_02093738, 0x366D, 2, 0x366E, 0x366F);
        case 0x1A6:
            gSaveData.unk_264E = 4;
            gSaveData.unk_2658 = 4;
            gSaveData.unk_265A = 4;
            break;
        case 0x1A8:
            FldGate_Set(46);
        case 0x1A9:
            return func_ov030_020a9f54(arg0, func_ov030_02093958, 0x3672, 2, 0x3673, 0x3674);
        case 0x1AB:
            arg0->unk_21CF8 = 1;
            break;
        case 0x1AC:
            func_ov030_020aeaa0(1, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1AE:
        case 0x1AF:
        case 0x1B0:
            if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x31) != 0)) {
                FldEvent_SetCurrent(0x1B1);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            return func_ov030_020a9f54(arg0, func_ov030_02093a0c, 0x3675, 2, 0x3676, 0x3677);
        case 0x1B1:
            FldGate_Set(13);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1B2:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_GOLEM);
            break;
        case 0x1B3:
            FldGate_Set(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1B4:
            return func_ov030_020a9f54(arg0, func_ov030_02093a98, 0x3678, 3, 0x3679, 0x367A, 0x367B);
        case 0x1B9:
        case 0x1BB:
        case 0x1BC:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x1BA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1BA:
            return func_ov030_020a9f54(arg0, func_ov030_02093b40, 0x367D, 3, 0x367E, 0x367F, 0x3680);
        case 0x1BD:
        case 0x1BF:
            FldEvent_SetCurrent(0x1BE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1BE:
            return func_ov030_020a9f54(arg0, func_ov030_02093be0, 0x3681, 3, 0x3682, 0x3683, 0x3684);
        case 0x1C0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x1C1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C1:
            FldEvent_SetCurrent(0x1C2);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C2:
            FldEvent_SetCurrent(0x1C3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C3:
            FldEvent_SetCurrent(0x1C4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C4:
        case 0x1C5:
            return func_ov030_020a9f54(arg0, func_ov030_02093c80, 0x3685, 4, 0x3686, 0x3687, 0x3688, 0x3689);
        case 0x1C6:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x1C7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C7:
            FldEvent_SetCurrent(0x1C8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C8:
            FldEvent_SetCurrent(0x1C9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1C9:
            FldEvent_SetCurrent(0x1CA);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1CA:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x1CB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1CB:
            FldGate_Set(15);
            gSaveData.unk_265C = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x1D0:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x51A;
            func_ov030_020af364(0x15);
            return 1;
        case 0x1D4:
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            func_ov030_020aec1c(9);
            gSaveData.unk_2660 = 2;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D5:
            FldGate_Set(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D7:
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_JOSHUA;
            func_ov030_020aec38(9);
            gSaveData.unk_3124 = 0x1C;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1CC:
            func_ov030_020aeab8(13, 2);
            break;
        case 0x1CD:
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(0x1DE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x1CE:
            FldGate_Set(0x10);
            func_ov030_020aeab8(6, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1CF:
            FldGate_Set(0x11);
            func_ov030_020aeab8(1, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D8:
            FldGate_Set(0x35);
        case 0x1DD:
            return func_ov030_020a9f54(arg0, func_ov030_02093d38, 0x368A, 2, 0x368B, 0x368C);
        case 0x1D9:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x1DB;
            gSaveData.unk_24C2 = 0x1DC;
            gSaveData.unk_341C = 33;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1DB:
            if (FldGate_Get(0x36) == 0) {
                FldGate_Set(0x36);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_WHEEL);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
        case 0x1DA:
        case 0x1DC:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x7F2:
            FldEvent_SetCurrent(0x7F3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7F3:
            FldGate_Set(56);
            if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) != 0) {
                FldEvent_SetCurrent(0x7F4);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x7F4:
            FldGate_Set(2);
            FldGate_Set(61);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            gSaveData.unk_2654 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7F5:
            if (FldGate_Get(58) != 0) {
                FldEvent_SetCurrent(0x7F7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7F6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7F6:
            if (FldGate_Get(0x39) == 0) {
                FldGate_Set(0x39);
                gSaveData.unk_3102 = 0;
            }
            break;
        case 0x7F7:
            FldGate_Set(3);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            gSaveData.unk_2658 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7F8:
            if (FldGate_Get(0x3C) != 0) {
                FldEvent_SetCurrent(EVENT_JOSHUA4_HACHIKO_REAPER);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x7F9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7F9:
            if (FldGate_Get(0x3B) == 0) {
                FldGate_Set(0x3B);
                gSaveData.unk_30FC = 0;
                gSaveData.unk_30FE = 39;
                gSaveData.unk_3100 = 5;
            }
            break;
        case 0x7FB:
            break;
        case EVENT_JOSHUA4_HACHIKO_REAPER:
            FldGate_Set(4);
            FldEvent_GrantItem(arg0, ITEM_QUEST_PRETTY_RIBBON);
            FldEvent_GrantPin(arg0, PIN_1000_YEN);
            gSaveData.unk_2656 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x51A:
            FldGate_Set(0x34);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_LIVE_SLOW_DIE_FAST);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case EVENT_JOSHUA4_SECRET_BOX_CAT_STREET:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SAMURAI_WIG, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_SAMURAI_WIG);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02094b08(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26BE = 2;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x1E2;
    func_ov030_020c26bc(4);
    gSaveData.unk_2676 = 0;
    gSaveData.unk_2670 = 0;
    gSaveData.unk_3124 = 0x42;
}

s32 func_ov030_02094bfc(FieldEventManager* arg0) {
    s16 var_r0;

    if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(3) == 0)) {
        if (FldGate_Get(0x46) == 0) {
            if (func_ov030_020aaad8() == 1) {
                FldGate_Set(0x46);
            }
        } else if (func_ov030_020aaad8() != 1) {
            FldGate_Set(0x47);
            FldGate_Clear(0x46);
        }
    }
    if ((FldGate_Get(45) != 0) && (FldGate_Get(4) == 0)) {
        if (FldGate_Get(0x48) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) != 0) {
                FldGate_Set(0x48);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) == 0) {
            FldGate_Set(0x49);
            FldGate_Clear(0x48);
        }
    }
    if (gSaveData.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (FldGate_Get(2) == 0)) {
            FldGate_Set(2);
            FldEvent_BeginScript(arg0, 0x1E4, 0);
            return 1;
        } else if ((FldGate_Get(38) != 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x1ED, 0);
            return 1;
        }
    }

    if ((gSaveData.unk_2458 == 3) && (func_ov030_020848e4(arg0, 56) != 0) && (FldGate_Get(0x34) == 0)) {
        FldGate_Set(0x34);
        FldEvent_BeginScript(arg0, 0x4C, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 6) {
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(0x3F) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(0x3F) != 0) && (FldGate_Get(0x43) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (FldGate_Get(0x43) != 0)) {
            FldGate_Clear(0x43);
        }
    }
    if ((gSaveData.unk_2458 == 12) && (func_ov030_020848e4(arg0, 15) != 0) && (FldGate_Get(5) != 0) && (FldGate_Get(33) == 0))
    {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 0x1E7, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 13) && (FldGate_Get(37) != 0) && (FldGate_Get(38) == 0)) {
        FldGate_Set(38);
        FldEvent_BeginScript(arg0, 0x1EC, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 15) {
        if ((FldGate_Get(0x23) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x1EA, 0);
            return 1;
        } else if ((FldGate_Get(0x44) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x1EB, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0xE) {
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if ((var_r0 >= 2) && (FldGate_Get(0x32) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
        }
        if ((func_ov030_020848e4(arg0, 18) != 0) && (FldGate_Get(8) != 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x1E6, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x1E8, 0);
            return 1;
        } else if ((FldGate_Get(0x22) != 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x1E9, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 18) {
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (FldGate_Get(38) != 0) && (FldGate_Get(0x28) == 0)) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x1EE, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x13) != 0) && (FldGate_Get(0x28) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x1EF, 0);
            return 1;
        } else if ((FldGate_Get(0x45) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x1F0, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 0x13) && (FldGate_Get(0x35) == 0)) {
        FldGate_Set(0x35);
        FldEvent_BeginScript(arg0, 0x4D, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(0x3F) == 0) {
            FldGate_Set(0x3F);
            FldGate_Set(0x43);
            FldEvent_BeginScript(arg0, 0x1F2, 0);
            return 1;
        } else if ((FldGate_Get(0x3F) != 0) && (FldGate_Get(0x43) == 0)) {
            FldGate_Set(0x43);
            FldEvent_BeginScript(arg0, 0x1FA, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02095364(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x368E:
            FldGate_Set(0x36);
            FldEvent_SetCurrent(0x810);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x368F:
            FldGate_Set(0x37);
            FldEvent_SetCurrent(0x810);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3690:
            FldGate_Set(56);
            FldEvent_SetCurrent(0x810);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095418(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3692:
            FldGate_Set(0x39);
            FldEvent_SetCurrent(0x811);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3693:
            FldGate_Set(58);
            FldEvent_SetCurrent(0x811);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3694:
            FldGate_Set(0x3B);
            FldEvent_SetCurrent(0x811);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020954d0(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3696:
            FldGate_Set(0x3C);
            FldEvent_SetCurrent(0x812);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3697:
            FldGate_Set(61);
            FldEvent_SetCurrent(0x812);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3698:
            FldGate_Set(0x3E);
            FldEvent_SetCurrent(0x812);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095588(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x369A:
            FldEvent_SetCurrent(0x1F3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x369B:
            FldEvent_SetCurrent(0x1F4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x369C:
            FldEvent_SetCurrent(0x1F5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209562c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x369E:
            FldEvent_SetCurrent(0x4A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x369F:
            FldEvent_SetCurrent(0x4A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36A0:
            FldEvent_SetCurrent(0x4A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020956c8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A2:
            FldEvent_SetCurrent(0x4E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36A3:
            FldEvent_SetCurrent(0x4F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36A4:
            FldEvent_SetCurrent(0x4F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095764(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A6:
            FldEvent_SetCurrent(0x81A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36A7:
            FldEvent_SetCurrent(0x81B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020957e0(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A9:
            FldEvent_SetCurrent(0x820);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36AA:
            FldEvent_SetCurrent(0x821);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209585c(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x1E2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x1E3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1E3:
            break;
        case 0x1E4:
            gSaveData.unk_264A = 0;
            gSaveData.unk_264E = 0;
            gSaveData.unk_2664 = 0;
            gSaveData.unk_2662 = 0;
            gSaveData.unk_2666 = 0;
            gSaveData.unk_2668 = 0;
            gSaveData.unk_266A = 0;
            gSaveData.unk_266C = 0;
            arg0->unk_21CF8    = 1;
            break;
        case 0x7FE:
            FldEvent_SetCurrent(0x7FF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x7FF:
            FldGate_Set(43);
            break;
        case 0x801:
            FldEvent_SetCurrent(0x802);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x802:
            FldGate_Set(0x2C);
            if (func_ov030_020aaad8() == 1) {
                FldEvent_SetCurrent(0x803);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x803:
            FldGate_Set(3);
            FldGate_Set(0x46);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            gSaveData.unk_264E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x804:
            FldEvent_SetCurrent(0x805);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x805:
            FldGate_Set(45);
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) != 0) {
                FldEvent_SetCurrent(0x806);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x806:
            FldGate_Set(4);
            FldGate_Set(0x48);
            func_ov030_020aac28(arg0, PIN_PSYCH_SUPPORT);
            gSaveData.unk_2664 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x807:
            FldEvent_SetCurrent(0x808);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x808:
            FldGate_Set(46);
            break;
        case 0x80A:
            if (FldGate_Get(47) == 0) {
                FldGate_Set(47);
                FldEvent_SetCurrent(0x80B);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x80C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x80B:
            FldGate_Set(47);
        case 0x80C:
            FldEvent_SetCurrent(0x80D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x80D:
            return func_ov030_020a9f54(arg0, func_ov030_02095364, 0x368D, 3, 0x368E, 0x368F, 0x3690);
        case 0x810:
            FldEvent_SetCurrent(0x80E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x80E:
            return func_ov030_020a9f54(arg0, func_ov030_02095418, 0x3691, 3, 0x3692, 0x3693, 0x3694);
        case 0x811:
            FldEvent_SetCurrent(0x80F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x80F:
            return func_ov030_020a9f54(arg0, func_ov030_020954d0, 0x3695, 3, 0x3696, 0x3697, 0x3698);
        case 0x812:
            if ((FldGate_Get(0x36) != 0) && (FldGate_Get(0x3B) != 0) && (FldGate_Get(0x3C) != 0)) {
                FldEvent_SetCurrent(0x813);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x814);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x814:
            FldGate_Clear(0x36);
            FldGate_Clear(0x37);
            FldGate_Clear(56);
            FldGate_Clear(0x39);
            FldGate_Clear(58);
            FldGate_Clear(0x3B);
            FldGate_Clear(0x3C);
            FldGate_Clear(61);
            FldGate_Clear(0x3E);
            break;
        case 0x813:
            FldGate_Set(5);
            gSaveData.unk_2666 = 4;
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1E7:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 13;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x1E2000;
            arg0->unk_21B0C = 0x143000;
            break;
        case 0x815:
            FldEvent_SetCurrent(0x816);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x816:
            FldGate_Set(0x30);
            if (func_02023e58(0) != 0) {
                FldEvent_SetCurrent(0x817);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x817:
            FldGate_Set(6);
            FldGate_Set(0x4A);
            gSaveData.unk_2668 = 4;
            func_ov030_020aac28(arg0, PIN_STRONG_BODY);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6A9:
            if (func_02023e58(0) != 0) {
                FldEvent_SetCurrent(0x6A7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x69B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x818:
            FldEvent_SetCurrent(0x819);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x819:
            FldGate_Set(0x31);
            return func_ov030_020a9f54(arg0, func_ov030_02095764, 0x36A5, 2, 0x36A6, 0x36A7);
        case 0x81A:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x81D;
            gSaveData.unk_24C2 = 0x81C;
            gSaveData.unk_341C = 43;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x81D:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_THRIFT);
            FldGate_Set(7);
            gSaveData.unk_266A = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x81C:
            FldGate_Set(7);
            gSaveData.unk_266A = 4;
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(EVENT_JOSHUA5_SECRET_BOX_MIYASHITA_UNDERPASS);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x81E:
            FldEvent_SetCurrent(0x81F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x81F:
            FldGate_Set(0x32);
            return func_ov030_020a9f54(arg0, func_ov030_020957e0, 0x36A8, 2, 0x36A9, 0x36AA);
        case 0x820:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x822;
            func_ov030_020af364(0x18);
            return 1;
        case 0x822:
            FldGate_Set(8);
            gSaveData.unk_266C = 2;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x1E5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x1E5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x1E6:
            gSaveData.unk_266C = 4;
            gSaveData.unk_264A = 4;
            gSaveData.unk_2662 = 4;
            arg0->unk_21AD0    = 1;
            arg0->unk_21ACC    = 1;
            arg0->unk_21AFC    = 0x10;
            arg0->unk_21B00    = 0x66;
            arg0->unk_21B08    = 0x1E000;
            arg0->unk_21B0C    = 0xF4000;
            break;
        case 0x1E8:
            func_ov030_020aeaa0(18, 2);
            gSaveData.unk_26D2 = 0;
            break;
        case 0x1EA:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x524;
            func_ov030_020af364(0x16);
            return 1;
        case 0x1EB:
            FldGate_Set(37);
            break;
        case 0x1EF:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x525;
            func_ov030_020af364(23);
            return 1;
        case 0x1F0:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x1F1;
            gSaveData.unk_3124          = 0x1D;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F1:
            gSaveData.unk_3124 = 0x1E;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x49:
            return func_ov030_020a9f54(arg0, func_ov030_0209562c, 0x369D, 3, 0x369E, 0x369F, 0x36A0);
        case 0x4A:
            FldGate_Set(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4B:
            FldGate_Set(10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4D:
            return func_ov030_020a9f54(arg0, func_ov030_020956c8, 0x36A1, 3, 0x36A2, 0x36A3, 0x36A4);
        case 0x1F2:
            FldGate_Set(0x3F);
        case 0x1FA:
            return func_ov030_020a9f54(arg0, func_ov030_02095588, 0x3699, 3, 0x369A, 0x369B, 0x369C);

        case 0x1F3:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x1F6;
            gSaveData.unk_24C2 = 0x1F7;
            gSaveData.unk_341C = 0x23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F4:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x1F8;
            gSaveData.unk_24C2 = 0x1F9;
            gSaveData.unk_341C = 0x22;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F6:
            FldGate_Set(0x40);
            if ((FldGate_Get(0x40) != 0) && (FldGate_Get(0x41) != 0) && (FldGate_Get(0x42) == 0)) {
                FldGate_Set(0x42);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_HELLFIRE);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x1F8:
            FldGate_Set(0x41);
            if ((FldGate_Get(0x40) != 0) && (FldGate_Get(0x41) != 0) && (FldGate_Get(0x42) == 0)) {
                FldGate_Set(0x42);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_HELLFIRE);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x1F5:
        case 0x1F7:
        case 0x1F9:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x524:
            FldGate_Set(0x44);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_JOSHUA);
            break;
        case 0x525:
            FldGate_Set(0x45);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            break;
        case EVENT_JOSHUA5_SECRET_BOX_MIYASHITA_UNDERPASS:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_BIKER_GLOVES, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_BIKER_GLOVES);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02096790(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26BE = 2;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x200;
    func_ov030_020c26bc(0);
    gSaveData.unk_264A = 4;
    gSaveData.unk_2654 = 4;
    gSaveData.unk_2658 = 4;
    gSaveData.unk_265C = 4;
    gSaveData.unk_2660 = 4;
    gSaveData.unk_3124 = 0x43;
}

s32 func_ov030_02096888(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if (FldGate_Get(56) == 0) {
            FldGate_Set(56);
            FldEvent_BeginScript(arg0, 0x448, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 1) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x201, 0);
            return 1;
        } else if ((FldGate_Get(0x33) != 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x202, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 4) && (FldGate_Get(0x22) == 0)) {
        FldGate_Set(0x22);
        FldEvent_BeginScript(arg0, 0x203, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 2) {
        if (FldGate_Get(0x23) == 0) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x204, 0);
            return 1;
        } else if ((FldGate_Get(0x34) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x207, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 8) != 0) && (FldGate_Get(37) == 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x208, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 6) {
        if (FldGate_Get(39) == 0) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x209, 0);
            return 1;
        }
        if ((FldGate_Get(0x35) != 0) && (FldGate_Get(3) == 0)) {
            FldGate_Set(3);
            FldEvent_BeginScript(arg0, 0x20C, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x20D, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(0x2C) == 0) && (FldGate_Get(46) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(0x2C) != 0) && (FldGate_Get(45) == 0) &&
            (FldGate_Get(46) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (FldGate_Get(45) != 0) && (FldGate_Get(46) == 0)) {
            FldGate_Clear(45);
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(46) != 0) && (FldGate_Get(47) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (FldGate_Get(46) != 0) && (FldGate_Get(47) != 0)) {
            FldGate_Clear(47);
        }
    }
    if (gSaveData.unk_2458 == 7) {
        if (FldGate_Get(4) == 0) {
            FldGate_Set(4);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x20E, 0);
            return 1;
        } else if ((FldGate_Get(0x36) != 0) && (FldGate_Get(5) == 0)) {
            FldGate_Set(5);
            FldEvent_BeginScript(arg0, 0x211, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 9) {
        if (FldGate_Get(42) == 0) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x212, 0);
            return 1;
        } else if ((FldGate_Get(0x37) != 0) && (FldGate_Get(43) == 0)) {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, 0x213, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 37) {
        if ((FldGate_Get(0x2C) == 0) && (FldGate_Get(46) == 0)) {
            FldGate_Set(0x2C);
            FldGate_Set(45);
            FldEvent_BeginScript(arg0, 0x215, 0);
            return 1;
        } else if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(45) == 0) && (FldGate_Get(46) == 0)) {
            FldGate_Set(45);
            FldEvent_BeginScript(arg0, 0x21D, 0);
            return 1;
        } else if ((FldGate_Get(46) != 0) && (FldGate_Get(47) == 0)) {
            FldGate_Set(47);
            FldEvent_BeginScript(arg0, 0x222, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02096f2c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36AC:
            FldEvent_SetCurrent(0x205);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36AD:
            FldGate_Set(0x24);
            FldEvent_SetCurrent(0x206);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02096fb4(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36AF:
            FldEvent_SetCurrent(0x20A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36B0:
            FldGate_Set(0x28);
            FldEvent_SetCurrent(0x20B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209703c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36B2:
            FldEvent_SetCurrent(0x20F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36B3:
            FldEvent_SetCurrent(0x210);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020970b8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36B5:
            FldEvent_SetCurrent(0x216U);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36B6:
            FldEvent_SetCurrent(0x2177CU >> 8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36B7:
            FldEvent_SetCurrent(0x21FU);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36B8:
            FldEvent_SetCurrent(0x218U);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209716c(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x200:
            gSaveData.unk_24BE = 0x224;
            func_ov030_020aec1c(10);
            FldEvent_GrantItem(arg0, ITEM_STICKER_LEVITATE);
            FldEvent_GrantItem(arg0, ITEM_BOOK_BLACK_NOISE_SYMBOLS);
            break;
        case 0x201:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x52EU;
            func_ov030_020af364(0x19);
            return 1;
        case 0x204:
            return func_ov030_020a9f54(arg0, &func_ov030_02096f2c, 0x36AB, 2, 0x36AC, 0x36AD);
        case 0x205:
            FldGate_Set(2);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x52F;
            func_ov030_020af364(0x1C);
            return 1;
        case 0x207:
            func_ov030_020aac28(arg0, PIN_LIVE);
            break;
        case 0x208:
            FldGate_Set(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x209:
            return func_ov030_020a9f54(arg0, &func_ov030_02096fb4, 0x36AE, 2, 0x36AF, 0x36B0);
        case 0x20A:
        case 0x20B:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x530;
            func_ov030_020af364(0x1A);
            return 1;
        case 0x20C:
            FldGate_Set(3);
            func_ov030_020aac28(arg0, PIN_VELOCITY_ATTACK);
            arg0->unk_21CF8 = 1;
            break;
        case 0x20E:
            return func_ov030_020a9f54(arg0, &func_ov030_0209703c, 0x36B1, 2, 0x36B2, 0x36B3);
        case 0x20F:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x531;
            func_ov030_020af364(0x1D);
            return 1;
        case 0x210:
            FldGate_Set(5);
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(0x24) != 0) && (FldGate_Get(0x28) != 0) &&
                (FldGate_Get(1) == 0))
            {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(EVENT_JOSHUA6_SECRET_BOX_SHIBU_Q_HEADS);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x211:
            FldGate_Set(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x212:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x532;
            func_ov030_020af364(0x1B);
            return 1;
        case 0x213:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x214);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x214:
            gSaveData.unk_3124 = 0x1F;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x215:
            FldGate_Set(0x2C);
        case 0x21D:
            return func_ov030_020a9f54(arg0, &func_ov030_020970b8, 0x36B4, 3, 0x36B5, 0x36B6, 0x36B8);
        case 0x216:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x219;
            gSaveData.unk_24C2 = 0x21A;
            gSaveData.unk_341C = 0x23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x217:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x21B;
            gSaveData.unk_24C2 = 0x21C;
            gSaveData.unk_341C = 0x22;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x219:
            FldGate_Set(0x30);
            if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x31) != 0) && (FldGate_Get(46) == 0)) {
                FldEvent_SetCurrent(0x21E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x21B:
            FldGate_Set(0x31);
            if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x31) != 0) && (FldGate_Get(46) == 0)) {
                FldEvent_SetCurrent(0x21E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x21E:
            FldGate_Set(46);
            FldGate_Set(47);
        case 0x222:
            return func_ov030_020a9f54(arg0, &func_ov030_020970b8, 0x36B4, 4, 0x36B5, 0x36B6, 0x36B7, 0x36B8);
        case 0x21F:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x220;
            gSaveData.unk_24C2 = 0x221;
            gSaveData.unk_341C = 0x24;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x220:
            if (FldGate_Get(0x32) == 0) {
                FldGate_Set(0x32);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_WIND);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_SUNSCORCH);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x218:
        case 0x21A:
        case 0x21C:
        case 0x221:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x52E:
            FldGate_Set(0x33);
            func_ov030_020aac28(arg0, PIN_STRONG_N_PROUD);
            break;
        case 0x52F:
            FldGate_Set(0x34);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x530:
            FldGate_Set(0x35);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            break;
        case 0x531:
            FldGate_Set(0x36);
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x532:
            FldGate_Set(0x37);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case EVENT_JOSHUA6_SECRET_BOX_SHIBU_Q_HEADS:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_NINJA_GARB, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_NINJA_GARB);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_020979d0(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x227;
    func_ov030_020c26bc(0);
    gSaveData.unk_2648 = 4;
    gSaveData.unk_2650 = 4;
    gSaveData.unk_2652 = 4;
    gSaveData.unk_2674 = 4;
    gSaveData.unk_2676 = 4;
    gSaveData.unk_2678 = 4;
    gSaveData.unk_267A = 4;
    gSaveData.unk_3124 = 0x44;
}

s32 func_ov030_02097acc(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 2) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (FldGate_Get(0x20) == 0))
        {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x228, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 2) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (FldGate_Get(0x20) != 0))
        {
            FldGate_Clear(0x20);
        }
        if ((FldGate_Get(0x24) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            gSaveData.unk_3124 = 0x23;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }

    if ((gSaveData.unk_2458 == 3) && (FldGate_Get(33) == 0)) {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 553, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 23) {
        if (FldGate_Get(46) == 0) {
            FldGate_Set(46);
            FldEvent_BeginScript(arg0, 554, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) != 0) && (FldGate_Get(0x31) == 0) && (FldGate_Get(47) == 0)) {
            FldGate_Set(47);
            FldEvent_BeginScript(arg0, 555, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) == 0) && (FldGate_Get(0x31) != 0)) {
            FldGate_Clear(0x31);
        }
    }

    if (gSaveData.unk_2458 == 0x18) {
        if ((FldGate_Get(0x22) == 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 556, 0);
            return 1;
        }
        if ((FldGate_Get(2) != 0) && (FldGate_Get(0x35) == 0)) {
            FldGate_Set(0x35);
            FldEvent_BeginScript(arg0, 562, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x19) {
        if ((func_ov030_020848e4(arg0, 0x1C) != 0) && (FldGate_Get(4) != 0) && (FldGate_Get(0x30) == 0)) {
            FldGate_Set(0x30);
            FldEvent_BeginScript(arg0, 0x231, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x1C) != 0) && (FldGate_Get(0x30) != 0) && (FldGate_Get(0x32) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x1C) == 0) && (FldGate_Get(0x30) != 0) && (FldGate_Get(0x32) != 0)) {
            FldGate_Clear(0x32);
        }
    }
    if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(0x23) == 0) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x233, 0);
            return 1;
        }
        if ((FldGate_Get(0x34) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x235, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02097f4c(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x227:
            gSaveData.unk_24BE = 0x238;
            func_ov030_020aec1c(10);
            break;
        case 0x229:
            gSaveData.unk_24BE = 569;
            func_ov030_020aec1c(10);
            break;
        case 0x22D:
            FldGate_Set(6);
            arg0->unk_21CF8 = 1;
            break;
        case 0x22E:
            FldGate_Set(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x231:
            FldGate_Set(50);
            break;
        case 0x233:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x234;
            gSaveData.unk_3124          = 33;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x234:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x41F;
            gSaveData.unk_3124          = 0x20;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x41F:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x538;
            func_ov030_020af364(30);
            return 1;
        case 0x235:
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            gSaveData.unk_24B4                 = 0;
            gSaveData.unk_24B8                 = 1;
            gSaveData.currentStoryEvent        = EVENTFLAG_NOBGM | 0x236;
            gSaveData.unk_3124                 = 0x22;
            gSaveData.unk_3128                 = 0;
            arg0->unk_21630                    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x236:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x157000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x22A:
            FldGate_Set(0x31);
            gSaveData.unk_267A = 2;
            break;
        case 0x22B:
            gSaveData.unk_267A = 4;
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(567);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            func_ov030_020aac28(arg0, PIN_FROZEN_COOL);
            break;
        case 0x826:
            if ((FldGate_Get(0x28) != 0) && (FldGate_Get(41) != 0) && (FldGate_Get(42) != 0)) {
                FldEvent_SetCurrent(0x828);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x827);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x827:
            if (FldGate_Get(3) == 0) {
                FldGate_Set(3);
                gSaveData.unk_310C = &data_ov030_020d9c0c;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
            }
            break;
        case 0x828:
            FldGate_Set(2);
            gSaveData.unk_267C = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x829:
            if ((FldGate_Get(43) != 0) && (FldGate_Get(0x2C) != 0) && (FldGate_Get(45) != 0)) {
                FldEvent_SetCurrent(0x82B);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x82A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x82A:
            if (FldGate_Get(5) == 0) {
                FldGate_Set(5);
                gSaveData.unk_310C = &data_ov030_020d9b6c;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
            }
            break;
        case 0x82B:
            FldGate_Set(4);
            gSaveData.unk_2680 = 2;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x538:
            FldGate_Set(0x34);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_CANDLE_SERVICE);
            break;
        case EVENT_JOSHUA7_SECRET_BOX_CONCERT_STAGE:
            FldGate_Set(1);
            func_ov030_020aac28(arg0, PIN_FROZEN_COOL);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_BOYS_UNIFORM_WITH_BLAZER, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_BOYS_UNIFORM_WITH_BLAZER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02098500(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x23B;
    func_ov030_020c26bc(0);
    gSaveData.unk_264C = 4;
    gSaveData.unk_2656 = 4;
    gSaveData.unk_266E = 4;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
    gSaveData.unk_3124                 = 0x45;
}

s32 func_ov030_02098604(FieldEventManager* arg0) {
    if ((gSaveData.unk_2458 == 1) && (func_ov030_020848e4(arg0, 2) != 0) && (FldGate_Get(0x20) == 0)) {
        FldGate_Set(0x20);
        FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x23C, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 3) && (FldGate_Get(39) != 0) && (FldGate_Get(40) == 0)) {
        FldGate_Set(40);
        FldEvent_BeginScript(arg0, 0x24F, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 5) {
        if (FldGate_Get(33) == 0) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x23D, 0);
            return 1;
        } else if ((FldGate_Get(41) != 0) && (FldGate_Get(34) == 0)) {
            FldGate_Set(34);
            FldEvent_BeginScript(arg0, 576, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 0x11) {
        if (FldGate_Get(35) == 0) {
            FldGate_Set(35);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x241, 0);
            return 1;
        } else if ((FldGate_Get(35) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x449, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (FldGate_Get(38) == 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 580, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 7) == 0) {
            FldGate_Clear(0x24);
        }
        if ((func_ov030_020848e4(arg0, 0x13) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 581, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 18) {
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 582, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x13) != 0) && (FldGate_Get(38) != 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x248, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02098930(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36BA:
            FldEvent_SetCurrent(0x24A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36BB:
            FldEvent_SetCurrent(0x24B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020989b0(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36BD:
            FldEvent_SetCurrent(0x24D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36BE:
            FldEvent_SetCurrent(0x24E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02098a30(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x23B:
            gSaveData.unk_24BE = 594;
            func_ov030_020aec1c(10);
            break;
        case 0x23C:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 5;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x139000;
            arg0->unk_21B0C = 0xE1000;
            break;
        case 0x23D:
            gSaveData.unk_24BE = 0x253;
            func_ov030_020aec1c(10);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x23E;
            func_ov030_020af364(0x1F);
            return 1;
        case 0x23E:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x23F;
            gSaveData.unk_3124          = 0x24;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x23F:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x542;
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_BEAT;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020af364(0x20);
            return 1;
        case 0x240:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 0x11;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x5B000;
            arg0->unk_21B0C = 0xF4000;
            break;
        case 0x241:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x242);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x242:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x243);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x243:
            gSaveData.unk_2656 = 2;
            break;
        case 0x246:
            gSaveData.unk_2656 = 4;
            break;
        case 0x248:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 0x11;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x246000;
            arg0->unk_21B0C = 0x244000;
            break;
        case 0x82C:
            if (FldGate_Get(39) != 0) {
                FldEvent_SetCurrent(2093);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x82D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x249);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x249:
            return func_ov030_020a9f54(arg0, func_ov030_02098930, 0x36B9, 2, 0x36BA, 0x36BB);
        case 0x24A:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x24C;
            func_ov030_020af364(33);
            return 1;
        case 0x24C:
            return func_ov030_020a9f54(arg0, func_ov030_020989b0, 0x36BC, 2, 0x36BD, 0x36BE);
        case 0x24D:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x24C;
            func_ov030_020af364(33);
            return 1;
        case 0x24E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x82E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x82E:
            FldGate_Set(1);
            gSaveData.unk_2648 = 4;
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_HOT_GAZE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x24F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x250);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x250:
            gSaveData.unk_3124 = 37;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_BEAT1_HACHIKO:
            FldGate_Set(41);
            if (Inventory_HasRequiredQuantity(ITEM_STICKER_AIR_TIME, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_STICKER_AIR_TIME, 0);
            }
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_SUIT_CARDS);
            FldEvent_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_BEAT);
            break;
        case EVENT_BEAT1_SECRET_BOX_HACHIKO:
            FldGate_Set(2);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_GIRLS_UNIFORM_WITH_BLAZER, 1, 0)) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_GIRLS_UNIFORM_WITH_BLAZER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02099010(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x255;
    func_ov030_020c26bc(0);
    gSaveData.unk_2648 = 4;
    gSaveData.unk_264C = 4;
    gSaveData.unk_2656 = 4;
    gSaveData.unk_266E = 4;
    gSaveData.unk_2674 = 4;
    gSaveData.unk_2678 = 4;
    gSaveData.unk_267A = 4;
    gSaveData.unk_3124 = 0x46;
}

s32 func_ov030_0209910c(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (FldGate_Get(0x20) == 0) &&
            (FldGate_Get(33) == 0))
        {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x257, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 3) == 0) && (FldGate_Get(0x20) != 0) &&
                   (FldGate_Get(33) == 0))
        {
            FldGate_Clear(0x20);
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x259, 0);
            return 1;
        }
    }

    if ((gSaveData.unk_2458 == 3) && (func_ov030_020848e4(arg0, 4) != 0) && (FldGate_Get(33) == 0)) {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 0x258, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 0x13) {
        if (FldGate_Get(0x23) == 0) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x25A, 0);
            return 1;
        } else if ((FldGate_Get(42) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x25B, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x47) != 0) && (FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
            FldGate_Set(1);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x266, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x16) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 605, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x15) {
        if (FldGate_Get(38) == 0) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x25E, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) != 0) && (FldGate_Get(39) == 0) && (FldGate_Get(2) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x25F, 0);
            return 1;
        } else if ((FldGate_Get(43) != 0) && (FldGate_Get(2) == 0)) {
            FldGate_Set(2);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x261, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 23) {
        if (FldGate_Get(0x28) == 0) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x263, 0);
            return 1;
        } else if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(41) == 0)) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x265, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02099514(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x255:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x256;
            gSaveData.unk_3124          = 38;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x258:
            gSaveData.unk_2650 = 4;
            break;
        case 0x259:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 3;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x11C000;
            arg0->unk_21B0C = 0x102000;
            break;
        case 0x25A:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x54CU;
            func_ov030_020af364(0x22);
            return 1;
        case 0x25B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            gSaveData.unk_3120 = 0x14;
            FldEvent_SetCurrent(0x25C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x25F:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x54DU;
            func_ov030_020af364(0x23);
            return 1;
        case 0x261:
            arg0->unk_21CF8 = 1;
            break;
        case 0x264:
            FldGate_Set(3);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_BEAT2_CONCERT_STAGE;
            func_ov030_020af364(0x24);
            return 1;
        case 0x265:
            gSaveData.unk_3124 = 39;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x54C:
            FldGate_Set(42);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_KEWL_LINE);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x54D:
            FldGate_Set(43);
            func_ov030_020aac28(arg0, PIN_SELF_FOUND_OTHERS_LOST);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case EVENT_BEAT2_CONCERT_STAGE:
            FldGate_Set(0x2C);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_MICROCOSMIC_PULL);
            FldEvent_GrantItem(arg0, ITEM_STICKER_BLOCK_BEAT);
            break;
        case EVENT_BEAT2_SECRET_BOX_DOGENZAKA:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FALSE_TEETH, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_FALSE_TEETH);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02099858(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26BE = 2;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x26A;
    func_ov030_020c26bc(0);
    gSaveData.unk_2648 = 2;
    gSaveData.unk_264A = 2;
    gSaveData.unk_264C = 2;
    gSaveData.unk_2656 = 4;
    gSaveData.unk_266E = 0;
    gSaveData.unk_2650 = 4;
    gSaveData.unk_2652 = 4;
    gSaveData.unk_2674 = 4;
    gSaveData.unk_2678 = 4;
    gSaveData.unk_267A = 4;
    gSaveData.unk_2654 = 4;
    gSaveData.unk_2658 = 4;
    gSaveData.unk_265A = 4;
    gSaveData.unk_265E = 4;
    gSaveData.unk_3124 = 0x47;
    gSaveData.unk_3120 = 0x14;
}

s32 func_ov030_02099990(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if ((func_ov030_02084860(0x157, 0xD6, 0x32) == 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x26B, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
                    (func_ov030_020848e4(arg0, 2) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) &&
                   (FldGate_Get(33) == 0) && (FldGate_Get(3) == 0))
        {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x26C, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
                   (func_ov030_020848e4(arg0, 2) == 0) && (func_ov030_020848e4(arg0, 3) == 0) && (FldGate_Get(33) != 0) &&
                   (FldGate_Get(3) == 0))
        {
            FldGate_Clear(33);
        }
        if ((func_ov030_020848e4(arg0, 3) != 0) && (FldGate_Get(3) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x26D, 0);
            return 1;
        }
    }

    if ((gSaveData.unk_2458 == 3) && (FldGate_Get(38) == 0) && (FldGate_Get(0x24) == 0)) {
        FldGate_Set(38);
        FldEvent_BeginScript(arg0, 0x272, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 4) && (FldGate_Get(0x24) == 0)) {
        FldGate_Set(0x24);
        FldEvent_BeginScript(arg0, 0x26F, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 5) && (FldGate_Get(37) == 0) && (FldGate_Get(0x24) == 0)) {
        FldGate_Set(37);
        FldEvent_BeginScript(arg0, 0x270, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 2) && (FldGate_Get(5) == 0)) {
        FldGate_Set(5);
        FldEvent_BeginScript(arg0, 0x273, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 6) {
        if (FldGate_Get(39) == 0) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x274, 0);
            return 1;
        } else if ((FldGate_Get(9) != 0) && (FldGate_Get(0x28) == 0)) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x27B, 0);
            return 1;
        } else if ((FldGate_Get(0x28) != 0) && (FldGate_Get(0x42) == 0)) {
            FldGate_Set(0x42);
            FldEvent_BeginScript(arg0, 0x44A, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0)) && (FldGate_Get(0x28) != 0) &&
                   (FldGate_Get(41) == 0))
        {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, 0x280, 0);
            return 1;
        } else if ((FldGate_Get(0x40) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x281, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(61) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (FldGate_Get(61) != 0) && (FldGate_Get(0x3F) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (FldGate_Get(0x3F) != 0)) {
            FldGate_Clear(0x3F);
        }
    }
    if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(61) == 0) {
            FldGate_Set(61);
            FldGate_Set(0x3F);
            FldEvent_BeginScript(arg0, 642, 0);
            return 1;
        } else if ((FldGate_Get(61) != 0) && (FldGate_Get(0x3F) == 0)) {
            FldGate_Set(0x3F);
            FldEvent_BeginScript(arg0, 0x287, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02099f8c(FieldEventManager* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (FldGate_Get(43) != 0) {
                func_ov030_020a9db8(arg0, 0x4B5);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x4B4);
            return 0;
        case 1:
            FldEvent_SetCurrent(0x4A6);
            return 1;
        case 0:
            if (FldGate_Get(46) == 0) {
                FldGate_Set(46);
                FldEvent_SetCurrent(0x4A3);
                return 1;
            }
            if (FldGate_Get(47) == 0) {
                FldGate_Set(47);
                FldEvent_SetCurrent(0x4A4);
                return 1;
            }
            FldGate_Clear(47);
            FldEvent_SetCurrent(0x4A5);
            return 1;
        case 2:
            if (FldGate_Get(0x2C) == 0) {
                FldGate_Set(0x2C);
                FldEvent_SetCurrent(0x4A7);
                return 1;
            }
            if (FldGate_Get(45) == 0) {
                FldGate_Set(45);
                FldEvent_SetCurrent(0x4A8);
                return 1;
            }
            FldGate_Clear(45);
            FldEvent_SetCurrent(0x4A9);
            return 1;
        case 3:
            FldEvent_SetCurrent(0x4AA);
            return 1;
        case 4:
            FldEvent_SetCurrent(0x4AB);
            return 1;
        case 5:
            FldEvent_SetCurrent(0x4AC);
            return 1;
        case 6:
            FldEvent_SetCurrent(0x4AD);
            return 1;
        case 7:
            FldEvent_SetCurrent(0x4AE);
            return 1;
        case 8:
            FldEvent_SetCurrent(0x4AF);
            return 1;
        case 9:
            func_ov030_020a9db8(arg0, 0x4B0);
            return 0;
        case 10:
            func_ov030_020a9db8(arg0, 0x4B1);
            return 0;
        case 11:
            func_ov030_020a9db8(arg0, 0x4B2);
            return 0;
        case 13:
            func_ov030_020a9db8(arg0, 0x4B3);
            return 0;
    }
    return 0;
}

s32 func_ov030_0209a1e4(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C0:
            FldGate_Set(7);
            FldEvent_SetCurrent(0x276);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36C1:
            FldGate_Set(8);
            FldEvent_SetCurrent(EVENT_BEAT3_TIPSY_TOSE_HALL_REAPER_CREEPER);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a270(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C6:
            FldGate_Set(0x31);
            FldEvent_SetCurrent(0x835);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36C7:
            FldGate_Set(0x32);
            FldEvent_SetCurrent(0x835);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36C8:
            FldGate_Set(0x33);
            FldEvent_SetCurrent(0x835);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a328(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36CA:
            FldGate_Set(0x34);
            FldEvent_SetCurrent(0x836);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36CB:
            FldGate_Set(0x35);
            FldEvent_SetCurrent(0x836);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36CC:
            FldGate_Set(0x36);
            FldEvent_SetCurrent(0x836);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36CD:
            FldGate_Set(0x37);
            FldEvent_SetCurrent(0x836);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209a3f8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36CF:
            FldGate_Set(56);
            FldEvent_SetCurrent(0x837);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D0:
            FldGate_Set(0x39);
            FldEvent_SetCurrent(0x837);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D1:
            FldGate_Set(58);
            FldEvent_SetCurrent(0x837);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D2:
            FldGate_Set(0x3B);
            FldEvent_SetCurrent(0x837);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D3:
            FldGate_Set(0x3C);
            FldEvent_SetCurrent(0x837);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a4ec(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C3:
            FldEvent_SetCurrent(0x283);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36C4:
            FldEvent_SetCurrent(0x284);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a564(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x26E:
            FldGate_Set(3);
            gSaveData.unk_2648 = 4;
            gSaveData.unk_264C = 4;
            gSaveData.unk_264A = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x271:
            FldGate_Set(4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x275:
            FldGate_Set(6);
            return func_ov030_020a9f54(arg0, func_ov030_0209a1e4, 0x36BF, 2, 0x36C0, 0x36C1);
        case 0x4B4:
            FldGate_Set(43);
        case 0x4B5:
            FldGate_Clear(46);
            FldGate_Clear(47);
            FldGate_Clear(0x2C);
            FldGate_Clear(45);
        case 0x276:
        case 0x4B3:
            if (func_ov030_020a9d00(arg0, 0x4A2) == 0) {
                return 1;
            }
            arg0->unk_21D22 = 3;
            arg0->unk_21D24 = func_ov030_02099f8c;
            arg0->unk_21D28 = 0x5FEA;
            arg0->unk_21D18 = 0;
            DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
            DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
            return 1;
        case 0x4B1:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x277);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x4B0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x278);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x4B2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x279);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x277:
            FldGate_Set(9);
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(EVENT_BEAT3_SECRET_BOX_TISPY_TOSE_HALL);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x278:
        case 0x279:
            FldGate_Set(9);
            arg0->unk_21CF8 = 1;
            break;
        case EVENT_BEAT3_TIPSY_TOSE_HALL_REAPER_CREEPER:
            FldGate_Set(9);
            if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10);
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x27B:
            FldEvent_SetCurrent(0x27C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x27C:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x27D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x27D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x27E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x27E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(EVENT_BEAT3_TIPSY_TOSE_HALL);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case EVENT_BEAT3_TIPSY_TOSE_HALL:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            FldEvent_GrantItem(arg0, ITEM_STICKER_AIR_CANCEL);
            break;
        case 0x280:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x556;
            func_ov030_020af364(37);
            return 1;
        case 0x281:
            gSaveData.unk_3124 = 0x28;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x282:
            FldGate_Set(61);
        case 0x287:
            return func_ov030_020a9f54(arg0, func_ov030_0209a4ec, 0x36C2, 2, 0x36C3, 0x36C4);
        case 0x283:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = ITEM_CD_TRACK01;
            gSaveData.unk_24C2 = 0x286;
            gSaveData.unk_341C = 37;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x285:
            if (FldGate_Get(0x3E) == 0) {
                FldGate_Set(0x3E);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_ARTIST);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
        case 0x284:
        case 0x286:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x82F:
            if (FldGate_Get(0x30) == 0) {
                FldGate_Set(0x30);
                FldEvent_SetCurrent(0x830);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x831);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x830:
        case 0x831:
            FldEvent_SetCurrent(0x832);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x832:
            return func_ov030_020a9f54(arg0, func_ov030_0209a270, 0x36C5, 3, 0x36C6, 0x36C7, 0x36C8);
        case 0x835:
            FldEvent_SetCurrent(0x833);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x833:
            return func_ov030_020a9f54(arg0, func_ov030_0209a328, 0x36C9, 4, 0x36CA, 0x36CB, 0x36CC, 0x36CD);
        case 0x836:
            FldEvent_SetCurrent(0x834);
            FldEvent_AdvanceScript(arg0);
            return 1;

        case 0x834:
            return func_ov030_020a9f54(arg0, func_ov030_0209a3f8, 0x36CE, 5, 0x36CF, 0x36D0, 0x36D1, 0x36D2, 0x36D3);
        case 0x837:
            if ((FldGate_Get(0x31) != 0) && (FldGate_Get(0x35) != 0) && (FldGate_Get(0x39) != 0)) {
                FldEvent_SetCurrent(0x838);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x839);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x839:
            FldGate_Clear(0x31);
            FldGate_Clear(0x32);
            FldGate_Clear(0x33);
            FldGate_Clear(0x34);
            FldGate_Clear(0x35);
            FldGate_Clear(0x36);
            FldGate_Clear(0x37);
            FldGate_Clear(56);
            FldGate_Clear(0x39);
            FldGate_Clear(58);
            FldGate_Clear(0x3B);
            FldGate_Clear(0x3C);
            break;
        case 0x838:
            FldGate_Set(2);
            FldGate_Set(0x41);
            gSaveData.unk_266E = 4;
            FldEvent_GrantPin(arg0, PIN_5000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_HOT_DOG);
            arg0->unk_21CF8 = 1;
            break;
        case 0x556:
            FldGate_Set(0x40);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_CASUAL_PENDULUM);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case EVENT_BEAT3_SECRET_BOX_TISPY_TOSE_HALL:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_BLACK_BOOK_BAG, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_BLACK_BOOK_BAG);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209af50(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(15);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x28C;
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x48;
    func_ov030_02084944(0xE, 0);
    func_ov030_02084944(15, 0);
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_0209b058(s32 arg0) {
    if (gSaveData.unk_2458 == 15) {
        if ((func_ov030_020848e4(arg0, 0x11) != 0) && (FldGate_Get(0x24) == 0) && (FldGate_Get(3) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x28E, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) == 0) && (FldGate_Get(0x24) != 0) && (FldGate_Get(3) == 0)) {
            FldGate_Clear(0x24);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (FldGate_Get(37) == 0) && (FldGate_Get(3) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x28F, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) == 0) && (FldGate_Get(37) != 0) && (FldGate_Get(3) == 0)) {
            FldGate_Clear(37);
        }
        if ((FldGate_Get(3) != 0) && (FldGate_Get(0x45) == 0)) {
            FldGate_Set(0x45);
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_BEAT;
        }
        if ((func_ov030_020848e4(arg0, 0x11) != 0) && (FldGate_Get(3) != 0) && (FldGate_Get(38) == 0) && (FldGate_Get(4) == 0))
        {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x291, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) == 0) && (FldGate_Get(3) != 0) && (FldGate_Get(38) != 0) &&
                   (FldGate_Get(4) == 0))
        {
            FldGate_Clear(38);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (FldGate_Get(3) != 0) && (FldGate_Get(4) == 0) && (FldGate_Get(41) == 0))
        {
            FldGate_Set(4);
            FldEvent_BeginScript(arg0, 0x292, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) != 0) && (FldGate_Get(5) != 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x29C, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) != 0) && (FldGate_Get(5) != 0) && (FldGate_Get(0x28) == 0) &&
                   (FldGate_Get(41) == 0))
        {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x29D, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) == 0) && (FldGate_Get(5) != 0) && (FldGate_Get(0x28) != 0) &&
                   (FldGate_Get(41) == 0))
        {
            FldGate_Clear(0x28);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (FldGate_Get(8) != 0) && (FldGate_Get(9) == 0)) {
            FldGate_Set(9);
            FldEvent_BeginScript(arg0, 0x2B3, 0);
            return 1;
        } else if ((FldGate_Get(2) != 0) && (FldGate_Get(0x46) == 0)) {
            FldGate_Set(0x46);
            func_ov030_020aec1c(1);
            gSaveData.playerStats.activeFriend = FRIEND_BEAT;
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (FldGate_Get(2) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x2B5, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 0xE) && (FldGate_Get(39) != 0) && (FldGate_Get(41) == 0)) {
        FldGate_Set(41);
        FldEvent_BeginScript(arg0, 0x29E, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 13) {
        if ((FldGate_Get(2) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x2B6, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 15) != 0) || (func_ov030_020848e4(arg0, 0x10) != 0)) &&
                   (FldGate_Get(0x23) == 0) && (FldGate_Get(33) == 0))
        {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x2B7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 15) == 0) && (func_ov030_020848e4(arg0, 0x10) == 0) &&
                   (FldGate_Get(0x23) != 0) && (FldGate_Get(33) == 0))
        {
            FldGate_Clear(0x23);
        }
        if ((func_ov030_020aed9c(-2) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x2B8, 0);
            return 1;
        } else if ((FldGate_Get(0x43) != 0) && (FldGate_Get(0x2C) == 0)) {
            FldGate_Set(0x2C);
            FldEvent_BeginScript(arg0, 0x2BA, 0);
            return 1;
        } else if ((FldGate_Get(0x44) != 0) && (FldGate_Get(45) == 0)) {
            FldGate_Set(45);
            FldEvent_BeginScript(arg0, 0x2BB, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209b724(void) {
    if (gSaveData.unk_2458 == 15) {
        if ((FldGate_Get(4) != 0) && (FldGate_Get(5) == 0)) {
            return 0x297;
        } else if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
            return 0x2A4;
        } else if ((FldGate_Get(7) != 0) && (FldGate_Get(8) == 0)) {
            return 0x2AD;
        }
    }
    if (gSaveData.unk_2458 == 0xE) {
        if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
            return 0x2A4;
        } else if ((FldGate_Get(7) != 0) && (FldGate_Get(8) == 0)) {
            return 0x2AD;
        }
    }
    return 0;
}

s32 func_ov030_0209b810(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((FldGate_Get(47) != 0) && (FldGate_Get(0x31) != 0) && (FldGate_Get(46) == 0) && (FldGate_Get(0x30) == 0)) {
                FldEvent_SetCurrent(0x298);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x299);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D6:
            FldEvent_SetCurrent(0x29A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209b8e8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((FldGate_Get(0x32) != 0) && (FldGate_Get(0x35) != 0) && (FldGate_Get(56) != 0) && (FldGate_Get(0x3B) != 0)) {
                FldEvent_SetCurrent(0x2A5);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x2A6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D6:
            FldEvent_SetCurrent(0x2A7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209b9c4(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((FldGate_Get(0x3C) == 0) && (FldGate_Get(61) != 0) && (FldGate_Get(0x3E) == 0) && (FldGate_Get(0x3F) == 0) &&
                (FldGate_Get(0x40) != 0) && (FldGate_Get(0x41) != 0) && (FldGate_Get(0x42) == 0))
            {
                FldEvent_SetCurrent(686);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x2AF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D6:
            FldEvent_SetCurrent(0x2B0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209bacc(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x28C:
            break;
        case 0x290:
            FldGate_Set(3);
            arg0->unk_21CF8 = 1;
            break;
        case 0x292:
            gSaveData.unk_24BE = 0x2BF;
            func_ov030_020aec1c(10);
            gSaveData.unk_310C = &data_ov030_020d9b94;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x297:
            return func_ov030_020a9f54(arg0, func_ov030_0209b810, 0x36D4, 2, 0x36D5, 0x36D6);
        case 0x299:
            FldGate_Clear(46);
            FldGate_Clear(47);
            FldGate_Clear(0x30);
            FldGate_Clear(0x31);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x298:
            FldEvent_SetCurrent(0x29B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x29B:
            FldGate_Set(5);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x29C:
            gSaveData.unk_266A = 4;
            break;
        case 0x29F:
            FldGate_Set(6);
            gSaveData.unk_310C = &data_ov030_020d9bbc;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2A4:
            return func_ov030_020a9f54(arg0, func_ov030_0209b8e8, 0x36D4, 2, 0x36D5, 0x36D6);
        case 0x2A6:
            FldGate_Clear(0x32);
            FldGate_Clear(0x33);
            FldGate_Clear(0x34);
            FldGate_Clear(0x35);
            FldGate_Clear(0x36);
            FldGate_Clear(0x37);
            FldGate_Clear(56);
            FldGate_Clear(0x39);
            FldGate_Clear(58);
            FldGate_Clear(0x3B);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x2A5:
            FldEvent_SetCurrent(0x2A8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2A8:
            FldGate_Set(7);
            gSaveData.unk_310C = &data_ov030_020d9d34;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2AD:
            return func_ov030_020a9f54(arg0, func_ov030_0209b9c4, 0x36D4, 2, 0x36D5, 0x36D6);
        case 0x2AF:
            FldGate_Clear(0x3C);
            FldGate_Clear(61);
            FldGate_Clear(0x3E);
            FldGate_Clear(0x3F);
            FldGate_Clear(0x40);
            FldGate_Clear(0x41);
            FldGate_Clear(0x42);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x2AE:
            FldEvent_SetCurrent(0x2B1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2B1:
            FldGate_Set(8);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2B3:
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            arg0->unk_21CF8                    = 1;
            break;
        case 0x2B4:
            FldGate_Set(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2B5:
            gSaveData.unk_2668 = 4;
            break;
        case 0x2B6:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2B8:
            FldEvent_SetCurrent(0x2B9);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2B9:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x560;
            func_ov030_020af364(39);
            return 1;
        case 0x2BA:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x561;
            func_ov030_020af364(38);
            return 1;
        case 0x2BB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x2BC);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2BC:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x2BD);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2BD:
            gSaveData.unk_3124 = 41;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x560:
            FldGate_Set(0x43);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x561:
            FldGate_Set(0x44);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_PLAYMATE_BEAM);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case EVENT_BEAT4_SECRET_BOX_MIYASHITA_PARK:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_GAKURAN, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_GAKURAN);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209c0f4(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x2C2;

    u64 temp = 7;
    func_ov030_020c26bc(4);
    gSaveData.unk_265C = 0;
    gSaveData.unk_2660 = 2;
    gSaveData.unk_266C = 0;
    gSaveData.unk_2670 = 0;
    gSaveData.unk_2676 = 0;
    func_ov030_02084944(temp);
    gSaveData.unk_3124 = 0x49;
    gSaveData.unk_3120 = 0x14;
}

s32 func_ov030_0209c208(s32 arg0) {
    if (gSaveData.unk_2458 == 6) {
        if (FldGate_Get(0x20) == 0) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x2C3, 0);
            return 1;
        } else if ((FldGate_Get(42) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x2C5, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0) ||
                    (func_ov030_020848e4(arg0, 10) != 0)) &&
                   (FldGate_Get(33) != 0) && (FldGate_Get(0x22) == 0) && (FldGate_Get(3) == 0))
        {
            FldGate_Set(3);
            FldEvent_BeginScript(arg0, 0x2C6, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 8) == 0) && (func_ov030_020848e4(arg0, 9) == 0) &&
                   (func_ov030_020848e4(arg0, 10) == 0) && (FldGate_Get(33) != 0) && (FldGate_Get(0x22) == 0) &&
                   (FldGate_Get(3) != 0))
        {
            FldGate_Clear(3);
        }
        if ((func_ov030_020aed9c(-2) != 0) && (FldGate_Get(33) != 0) && (FldGate_Get(0x22) == 0)) {
            FldGate_Set(0x22);
            FldEvent_BeginScript(arg0, 0x2C7, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 7) {
        if (FldGate_Get(0x23) == 0) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, 0x2C8, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x64, 0x64, 0x96, 0x15E) != 0) && (FldGate_Get(6) == 0)) {
            FldGate_Set(6);
            FldEvent_BeginScript(arg0, 0x2CA, 0);
            return 1;
        } else if ((FldGate_Get(43) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x2CB, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 12) != 0) && (FldGate_Get(6) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x2CD, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 9) && (FldGate_Get(0x2C) != 0) && (FldGate_Get(41) == 0)) {
        FldGate_Set(41);
        FldEvent_BeginScript(arg0, 0x2D3, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_0209c57c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D8:
            if (FldGate_Get(39) == 0) {
                FldGate_Set(39);
                FldEvent_SetCurrent(0x2CF);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x2D0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36D9:
            if (FldGate_Get(0x28) == 0) {
                FldGate_Set(0x28);
                FldEvent_SetCurrent(0x2CF);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x2D0);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36DA:
            FldEvent_SetCurrent(0x2D1);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209c680(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x2C4:
            FldGate_Set(2);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_BEAT1_TIPSY_TOSE_HALL;
            func_ov030_020af364(41);
            return 1;
        case 0x2C5:
            gSaveData.unk_2658 = 2;
            gSaveData.unk_265A = 2;
            gSaveData.unk_265C = 2;
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2C7:
            gSaveData.unk_2658 = 4;
            gSaveData.unk_265A = 4;
            gSaveData.unk_265C = 4;
            func_ov030_020aec38(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2CC:
            if ((FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                FldEvent_SetCurrent(EVENT_BEAT5_SECRET_BOX_SHIBU_Q_HEADS);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0X2CA:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x56BU;
            func_ov030_020af364(42);
            return 1;
        case 0x2CD:
            gSaveData.unk_2660 = 4;
            break;
        case 0x2CE:
        case 0x2CF:
        case 0x2D0:
            return func_ov030_020a9f54(arg0, func_ov030_0209c57c, 0x36D7, 3, 0x36D8, 0x36D9, 0x36DA);
        case 0x2D1:
            FldGate_Set(4);
            gSaveData.unk_2660 = 3;
            arg0->unk_21CF8    = 1;
            break;
        case 0x2D2:
            FldGate_Set(5);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x56C;
            func_ov030_020af364(0x28);
            return 1;
        case 0x2D3:
            gSaveData.unk_3124 = 42;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_BEAT1_TIPSY_TOSE_HALL:
            FldGate_Set(42);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            FldEvent_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_BEAT);
            break;
        case 0x56B:
            FldGate_Set(43);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case 0x56C:
            FldGate_Set(0x2C);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_FLOWER_OF_FLAME);
            break;
        case EVENT_BEAT5_SECRET_BOX_SHIBU_Q_HEADS:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_HEAD_HONCHO, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_HEAD_HONCHO);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209ca24(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26BE = 0;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 2;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        FldGate_Set(2);
        gSaveData.unk_26D4 = 4;
    } else {
        func_ov030_020aec1c(4);
        gSaveData.unk_26D4 = 0;
    }
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x2DA;
    gSaveData.unk_3124          = 0x4A;
    func_ov030_020c26bc(4);
    gSaveData.unk_2670 = 0;
    gSaveData.unk_2676 = 0;
    func_ov030_020aec1c(5);
    gSaveData.unk_3120 = 0x14;
    func_ov030_020aec1c(9);
}

s32 func_ov030_0209cb70(s32 arg0) {
    if ((gSaveData.unk_2458 == 1) && (func_ov030_020aed9c(-2) != 0) && (FldGate_Get(33) == 0) && (FldGate_Get(0x20) == 0)) {
        FldGate_Set(0x20);
        FldEvent_BeginScript(arg0, 0x2DB, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 11) && (func_ov030_020aed9c(-2) != 0) && (FldGate_Get(33) == 0) &&
               (FldGate_Get(0x20) == 0))
    {
        FldGate_Set(0x20);
        FldEvent_BeginScript(arg0, 0x2DB, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 12) {
        if ((func_ov030_020aed9c(-2) != 0) && (FldGate_Get(33) == 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, 0x2DB, 0);
            return 1;
        } else if ((FldGate_Get(3) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x2E0, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 13) != 0) || (func_ov030_020848e4(arg0, 0xE) != 0)) &&
                   (FldGate_Get(0x24) != 0) && (FldGate_Get(39) == 0))
        {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x2E2, 0);
            return 1;
        } else if ((FldGate_Get(0x28) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x2E3, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 13) && (func_ov030_020aed9c(-2) != 0) && (FldGate_Get(33) == 0) && (FldGate_Get(0x20) == 0)) {
        FldGate_Set(0x20);
        FldEvent_BeginScript(arg0, 0x2DB, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(2) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x2E7, 0);
            return 1;
        } else if ((FldGate_Get(38) != 0) && (FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
            FldGate_Set(1);
            FldEvent_BeginScript(arg0, 0x2E8, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 0xE) && (func_ov030_020848e4(arg0, 18) != 0) && (FldGate_Get(33) == 0)) {
        FldGate_Set(33);
        FldEvent_BeginScript(arg0, 0x2DC, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(3) == 0)) {
            FldGate_Set(3);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x2DD, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 0x45) != 0 && FldGate_Get(0x22) != 0 && FldGate_Get(0x23) == 0) {
            FldGate_Set(0x23);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x2DF, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 0x45) == 0 && FldGate_Get(0x22) != 0 && FldGate_Get(0x23) != 0) {
            FldGate_Clear(0x23);
        }
    }
    return 0;
}

s32 func_ov030_0209cff0(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x2DD:
            FldEvent_SetCurrent(0x2DE);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2DE:
            FldGate_Set(0x22);
            FldGate_Set(0x23);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            break;
        case 0x2E1:
        case 0x2E2:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x574;
            func_ov030_020af364(43);
            return 1;
        case 0x2E3:
            gSaveData.unk_3124 = 43;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x2E7:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x574:
            FldGate_Set(0x28);
            if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
                FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
                FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
                FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            } else {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_BEAT6_SECRET_BOX_DOGENZAKA:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_LUCKY_MALLET, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_LUCKY_MALLET);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209d1d8(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 2;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        gSaveData.unk_26D4 = 4;
    } else {
        func_ov030_020aec1c(4);
        gSaveData.unk_26D4 = 0;
    }
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x2EC;
    gSaveData.unk_3124          = 0x4B;
    func_ov030_020c26bc(4);
    func_ov030_020aec1c(5);
    gSaveData.unk_2676 = 0;
    gSaveData.unk_26B0 = 0;
    gSaveData.unk_26B2 = 2;
    gSaveData.unk_26B4 = 0;
    gSaveData.unk_3120 = 0x14;
}

s32 func_ov030_0209d318(s32 arg0) {
    if (gSaveData.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x2ED, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x45) == 0) && (FldGate_Get(0x20) != 0)) {
            FldGate_Clear(0x20);
        }
    }

    if (gSaveData.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(47) == 0)) {
            FldGate_Set(47);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x312, 0);
            return 1;
        } else if ((FldGate_Get(47) != 0) && (FldGate_Get(GATE_GAME_CLEARED) != 0) && (FldGate_Get(1) == 0)) {
            FldGate_Set(1);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x311, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 18) {
        gSaveData.unk_3120 = 20;
        if ((func_ov030_020848e4(arg0, 20) != 0) && (FldGate_Get(33) == 0)) {
            FldGate_Set(33);
            FldEvent_BeginScript(arg0, 0x2EE, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 38) {
        gSaveData.unk_3120 = 0;
        if (FldGate_Get(0x32) == 0) {
            FldGate_Set(0x32);
            FldEvent_BeginScript(arg0, 0x2EF, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 39) {
        if ((FldGate_Get(0x30) != 0) && (FldGate_Get(4) == 0)) {
            FldGate_Set(4);
            FldEvent_BeginScript(arg0, 0x2F3, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) != 0) && (FldGate_Get(0x22) == 0) && (FldGate_Get(0x23) == 0)) {
            FldGate_Set(0x22);
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x2F4, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) != 0) && (FldGate_Get(0x23) != 0) && (FldGate_Get(0x24) == 0)) {
            FldGate_Set(0x24);
            FldEvent_BeginScript(arg0, 0x2F7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) == 0) && (FldGate_Get(0x23) != 0) && (FldGate_Get(0x24) != 0)) {
            FldGate_Clear(0x24);
        }
    }
    if (gSaveData.unk_2458 == 0x28) {
        if ((func_ov030_020848a4(0x12C, 0xB4, 0x15E, 0x140) != 0) && (FldGate_Get(37) == 0)) {
            FldGate_Set(37);
            FldEvent_BeginScript(arg0, 0x2F8, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x2A8, 0xB4, 0x2DA, 0x140) != 0) && (FldGate_Get(38) == 0)) {
            FldGate_Set(38);
            FldEvent_BeginScript(arg0, 0x2F9, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x35) != 0) && (FldGate_Get(5) == 0) && (FldGate_Get(39) == 0)) {
            FldGate_Set(39);
            FldEvent_BeginScript(arg0, 0x2FB, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x35) == 0) && (FldGate_Get(5) == 0) && (FldGate_Get(39) != 0)) {
            FldGate_Clear(39);
        }
        if ((func_ov030_020848e4(arg0, 0x35) != 0) && (FldGate_Get(5) != 0) && (FldGate_Get(0x28) == 0)) {
            FldGate_Set(0x28);
            FldEvent_BeginScript(arg0, 0x2FC, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 41) {
        if (FldGate_Get(41) == 0) {
            FldGate_Set(41);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x2FD, 0);
            return 1;
        } else if ((FldGate_Get(0x31) != 0) && (FldGate_Get(42) == 0)) {
            FldGate_Set(42);
            FldEvent_BeginScript(arg0, 0x2FF, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 42) {
        if ((func_ov030_020848a4(0x2A8, 0xB4, 0x2DA, 0x140) != 0) && (FldGate_Get(43) == 0)) {
            FldGate_Set(43);
            FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x301, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x37) != 0) && (FldGate_Get(0x2C) == 0)) {
            FldGate_Set(0x2C);
            FldEvent_BeginScript(arg0, 0x302, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 43) && (func_ov030_020848a4(0, 0x96, 0x12C, 0xC8) != 0) && (FldGate_Get(45) == 0)) {
        FldGate_Set(45);
        FldEvent_BeginScript(arg0, EVENTFLAG_NOBGM | 0x303, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_0209d974(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36DC:
            FldEvent_SetCurrent(0x2F5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36DD:
            FldGate_Set(0x23);
            FldEvent_SetCurrent(0x2F6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209d9fc(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36DF:
            FldEvent_SetCurrent(0x304);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36E0:
            FldEvent_SetCurrent(0x305);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209da78(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E2:
            FldGate_Set(46);
            FldEvent_SetCurrent(775);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36E3:
            FldEvent_SetCurrent(776);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209dafc(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x2EF:
            gSaveData.unk_24BE = 0x313;
            func_ov030_020aec1c(10);
            break;
        case 0x2F0:
            FldGate_Set(2);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x2F1;
            gSaveData.unk_3124          = 0x2C;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x2F1:
            FldGate_Set(3);
            func_ov030_020aec1c(6);
            FldEvent_SetCurrent(0x2F2);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x2F2:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x420;
            gSaveData.unk_3124          = 45;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x420:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x57E;
            func_ov030_020af364(0x2C);
            return 1;
        case 0x2F3:
            gSaveData.unk_24BE = 0x314;
            func_ov030_020aec1c(10);
            gSaveData.unk_26B0 = 2;
            arg0->unk_21CF8    = 1;
            break;
        case 0x2F4:
        case 0x2F7:
            return func_ov030_020a9f54(arg0, func_ov030_0209d974, 0x36DB, 2, 0x36DC, 0x36DD);
        case 0x2F5:
            gSaveData.unk_26B0 = 0;
            arg0->unk_21AD0    = 1;
            arg0->unk_21ACC    = 1;
            arg0->unk_21AFC    = 0x28;
            arg0->unk_21B00    = 0x66;
            arg0->unk_21B08    = 0x2A000;
            arg0->unk_21B0C    = 0xF6000;
            break;
        case EVENT_BEAT7_TRAIL_OF_THE_BYGONE:
            FldEvent_GrantItem(arg0, ITEM_THREAD_PI_FACES_CAP);
            FldGate_Set(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2FC:
            gSaveData.unk_26B2 = 4;
            break;
        case 0x2FE:
            FldGate_Set(6);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x57F;
            func_ov030_020af364(45);
            return 1;
        case 0x2FF:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x300;
            gSaveData.unk_26B2          = 0;
            gSaveData.unk_26B4          = 4;
            gSaveData.unk_26B6          = 4;
            func_ov030_020aec38(1);
            gSaveData.playerStats.activeFriend = FRIEND_NONE;
            gSaveData.unk_3124                 = 46;
            gSaveData.unk_3128                 = 0;
            arg0->unk_21630                    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x303:
            return func_ov030_020a9f54(arg0, func_ov030_0209d9fc, 0x36DE, 2, 0x36DF, 0x36E0);
        case 0x304:
        case 0x305:
            FldEvent_SetCurrent(0x306);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x306:
            return func_ov030_020a9f54(arg0, func_ov030_0209da78, 0x36E1, 2, 0x36E2, 0x36E3);
        case 0x307:
        case 0x308:
            FldEvent_SetCurrent(0x309);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x309:
            gSaveData.unk_3124 = 47;
            gSaveData.unk_3128 = 0;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x30B:
            FldEvent_SetCurrent(0x30C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x30C:
            if (FldGate_Get(46) != 0) {
                FldEvent_SetCurrent(0x30E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x30D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x30D:
        case 0x30E:
            FldEvent_SetCurrent(0x30F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x30F:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x310;
            gSaveData.unk_3124          = 0x32;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x310:
            gSaveData.unk_3124 = 0x33;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x57E:
            FldGate_Set(0x30);
            FldEvent_GrantPin(arg0, PIN_SCARLETITE);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            FldEvent_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x57F:
            FldGate_Set(0x31);
            func_ov030_020aac28(arg0, PIN_LIGHTNING_ROOK);
            break;
        case 0x312:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_BEAT7_SECRET_BOX_DOGENZAKA:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SAMURAI_HELM, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_SAMURAI_HELM);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209e16c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    gSaveData.unk_26B8 = 4;
    gSaveData.unk_26BA = 4;
    gSaveData.unk_26BC = 4;
    gSaveData.unk_26C0 = 4;
    gSaveData.unk_26C2 = 4;
    gSaveData.unk_26C4 = 4;
    gSaveData.unk_26C6 = 4;
    gSaveData.unk_26C8 = 4;
    gSaveData.unk_26CA = 4;
    gSaveData.unk_26CC = 4;
    gSaveData.unk_26CE = 4;
    gSaveData.unk_26D0 = 4;
    gSaveData.unk_26D2 = 4;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        FldGate_Set(0x1F);
        gSaveData.unk_26D4 = 2;
    } else {
        func_ov030_020aec1c(4);
        gSaveData.unk_26D4 = 0;
    }
    gSaveData.unk_26D6 = 4;
    gSaveData.unk_26D8 = 4;
    gSaveData.unk_26DA = 4;
    gSaveData.unk_26DC = 4;
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(4);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = 0x317;
    gSaveData.unk_2670          = 2;
    gSaveData.unk_2676          = 2;
    gSaveData.unk_267E          = 0;
    gSaveData.unk_267C          = 0;
    gSaveData.unk_2680          = 0;
    gSaveData.unk_2682          = 0;
    gSaveData.unk_2686          = 0;
    gSaveData.unk_268A          = 0;
    gSaveData.unk_268E          = 0;
    gSaveData.unk_2692          = 0;
    gSaveData.unk_2696          = 0;
    gSaveData.unk_269A          = 0;
    gSaveData.unk_269E          = 0;
    gSaveData.unk_26A2          = 0;
    gSaveData.unk_26A6          = 0;
    gSaveData.unk_26AA          = 0;
    func_ov030_020aec1c(6);
    if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
        FldGate_Set(127);
        func_ov030_020c26bc(0);
        gSaveData.unk_26BA = 0;
        func_ov030_020ae96c(43);
        gSaveData.unk_24B4          = 0;
        gSaveData.unk_24B8          = 1;
        gSaveData.currentStoryEvent = 0x588;
    }
}

s32 func_ov030_0209e314(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(0x66) != 0) && (FldGate_Get(0x7B) == 0)) {
            FldGate_Set(0x7B);
            FldEvent_BeginScript(arg0, 0x370, 0);
            return 1;
        }
        if ((FldGate_Get(47) != 0) && (FldGate_Get(0x7C) == 0)) {
            FldGate_Set(0x7C);
            FldEvent_BeginScript(arg0, 0x370, 0);
            return 1;
        }
    }

    if (gSaveData.unk_2458 == 8) {
        if (FldGate_Get(0x30) == 0) {
            FldGate_Set(0x30);
            FldEvent_BeginScript(arg0, 0x318, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (FldGate_Get(2) == 0)) {
            FldGate_Set(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (FldGate_Get(0x32) == 0) && (FldGate_Get(3) == 0)) {
            FldGate_Set(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) == 0) && (FldGate_Get(0x32) != 0) && (FldGate_Get(3) == 0)) {
            FldGate_Clear(0x32);
        }
        if ((FldGate_Get(0x34) != 0) && (FldGate_Get(0x35) == 0)) {
            FldGate_Set(0x35);
            FldEvent_BeginScript(arg0, 0x34D, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 0x10) && (func_ov030_020848e4(arg0, 0x45) != 0) && (FldGate_Get(4) != 0) &&
        (FldGate_Get(0x34) == 0) && (FldGate_Get(0x5D) == 0) && (FldGate_Get(0x5A) == 0))
    {
        FldGate_Set(0x5A);
        FldEvent_BeginScript(arg0, 0x3C2, 0);
        return 1;
    }
    if (gSaveData.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(0x1F) != 0) && (FldGate_Get(47) == 0)) {
            FldGate_Set(0x1E);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x371, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (FldGate_Get(0x1F) != 0) && (FldGate_Get(47) == 0) &&
            (FldGate_Get(0x1E) != 0))
        {
            FldGate_Clear(0x1E);
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (FldGate_Get(0x1F) != 0) && (FldGate_Get(47) != 0) &&
            (FldGate_Get(0x1D) == 0))
        {
            FldGate_Set(0x1D);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x372, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(3) != 0) && (FldGate_Get(0x33) == 0)) {
            FldGate_Set(0x33);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x326, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (FldGate_Get(7) != 0) && (FldGate_Get(0x20) == 0)) {
            FldGate_Set(0x20);
            FldEvent_BeginScript(arg0, EVENTFLAG_SHOP | 0x32E, 0);
            return 1;
        }
        if ((FldGate_Get(4) != 0) && (FldGate_Get(47) == 0) && (func_ov030_020848e4(arg0, 23) != 0) &&
            (FldGate_Get(0x5E) == 0))
        {
            FldGate_Set(0x5E);
            FldEvent_BeginScript(arg0, 0x357, 0);
            return 1;
        }
        if ((FldGate_Get(4) != 0) && (FldGate_Get(47) == 0) && (func_ov030_020848e4(arg0, 23) == 0) &&
            (FldGate_Get(0x5E) != 0))
        {
            FldGate_Clear(0x5E);
        }
        if (((FldGate_Get(4) == 0) || (FldGate_Get(47) != 0)) && (func_ov030_020848e4(arg0, 23) != 0) &&
            (FldGate_Get(0x5D) == 0))
        {
            FldGate_Set(0x5D);
            FldEvent_BeginScript(arg0, 0x356, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x18) {
        if (FldGate_Get(0x5F) == 0) {
            FldGate_Set(0x5F);
            FldEvent_BeginScript(arg0, 0x358, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(0x6E);
        }
    }
    if (gSaveData.unk_2458 == 0x19) {
        if (FldGate_Get(0x60) == 0) {
            FldGate_Set(0x60);
            FldEvent_BeginScript(arg0, 0x359, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(0x6F);
        }
    }
    if (gSaveData.unk_2458 == 0x1C) {
        if (FldGate_Get(0x61) == 0) {
            FldGate_Set(0x61);
            FldEvent_BeginScript(arg0, 0x35A, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(0x72);
        }
    }
    if (gSaveData.unk_2458 == 0x1F) {
        if (FldGate_Get(0x62) == 0) {
            FldGate_Set(0x62);
            FldEvent_BeginScript(arg0, 0x35B, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(0x75);
        }
    }
    if (gSaveData.unk_2458 == 0x22) {
        if (FldGate_Get(0x63) == 0) {
            FldGate_Set(0x63);
            FldEvent_BeginScript(arg0, 0x35C, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(0x78);
        }
    }
    if (gSaveData.unk_2458 == 0x24) {
        if (FldGate_Get(0x64) == 0) {
            FldGate_Set(0x64);
            FldEvent_BeginScript(arg0, 0x35D, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            FldGate_Set(0x7A);
        }
        if ((func_ov030_020848e4(arg0, 0x31) != 0) && (FldGate_Get(0x16) != 0) && (FldGate_Get(0x65) == 0)) {
            FldGate_Set(0x7E);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
    }
    if (gSaveData.unk_2458 == 37) {
        if (FldGate_Get(0x7D) != 0) {
            if ((FldGate_Get(0x30) != 0) && (FldGate_Get(2) == 0)) {
                FldGate_Set(2);
                FldEvent_BeginScript(arg0, 0x319, 0);
                return 1;
            }
            if ((FldGate_Get(2) != 0) && (FldGate_Get(0x32) == 0)) {
                FldGate_Set(0x32);
                FldEvent_BeginScript(arg0, 0x31C, 0);
                return 1;
            }
        }
        if ((FldGate_Get(0x7E) != 0) && (FldGate_Get(0x64) != 0) && (FldGate_Get(0x65) == 0)) {
            FldGate_Set(0x65);
            FldEvent_BeginScript(arg0, 0x35E, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 0x1A) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x70);
    }
    if ((gSaveData.unk_2458 == 0x1B) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x71);
    }
    if ((gSaveData.unk_2458 == 0x1D) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x73);
    }
    if ((gSaveData.unk_2458 == 0x1E) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x74);
    }
    if ((gSaveData.unk_2458 == 0x20) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x76);
    }
    if ((gSaveData.unk_2458 == 33) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x77);
    }
    if ((gSaveData.unk_2458 == 0x23) && (func_ov030_020c1a9c() != 0)) {
        FldGate_Set(0x79);
    }
    if (gSaveData.unk_2458 == 18) {
        if (((FldGate_Get(4) == 0) || (FldGate_Get(47) != 0)) && (func_ov030_020848e4(arg0, 0x14) != 0) &&
            (FldGate_Get(0x68) == 0))
        {
            FldGate_Set(0x68);
            FldEvent_BeginScript(arg0, 0x361, 0);
            return 1;
        }
        if (((FldGate_Get(4) == 0) || (FldGate_Get(47) != 0)) && (func_ov030_020848e4(arg0, 0x14) == 0) &&
            (FldGate_Get(0x68) != 0))
        {
            FldGate_Clear(0x68);
        }
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (FldGate_Get(4) != 0) && (FldGate_Get(47) == 0) &&
            (FldGate_Get(0x67) == 0))
        {
            FldGate_Set(0x67);
            FldEvent_BeginScript(arg0, 0x360, 0);
            return 1;
        }
    }
    if ((gSaveData.unk_2458 == 39) && (func_ov030_020848a4(0x17C, 0, 0x190, 0x12C) != 0) && (FldGate_Get(0x69) == 0)) {
        FldGate_Set(0x69);
        FldEvent_BeginScript(arg0, 0x362, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 41) && (FldGate_Get(0x6A) == 0)) {
        FldGate_Set(0x6A);
        FldEvent_BeginScript(arg0, 0x363, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 42) && (FldGate_Get(0x6B) == 0)) {
        FldGate_Set(0x6B);
        FldEvent_BeginScript(arg0, 0x364, 0);
        return 1;
    } else if (gSaveData.unk_2458 == 43) {
        if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
            FldGate_Set(GATE_GAME_CLEARED);
            FldEvent_BeginScript(arg0, 0x316, 0);
            return 1;
        }
        if ((func_ov030_020848a4(0, 0, 0x12C, 0xE6) != 0) && (FldGate_Get(0x6B) != 0) && (FldGate_Get(0x6C) == 0)) {
            FldGate_Set(0x6C);
            FldEvent_BeginScript(arg0, 0x365, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209efe4(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E5:
            FldEvent_SetCurrent(0x31A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36E6:
            FldGate_Set(0x32);
            FldEvent_SetCurrent(0x31B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209f06c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E8:
            FldEvent_SetCurrent(0x367);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x36E9:
            FldEvent_SetCurrent(0x368);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209f0e8(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x588:
            FldEvent_GrantItem(arg0, ITEM_STICKER_GAME_CLEARED);
            break;
        case 0x372:
            gSaveData.unk_26D4 = 4;
            arg0->unk_21630    = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
            return func_ov030_020a9f54(arg0, func_ov030_020919c4, 0x36EA, 5, 0x36EB, 0x36EC, 0x36ED, 0x36EE, 0x36EF);
        case 0x317:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x370;
            gSaveData.unk_3124          = 0x4C;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x319:
        case 0x31C:
        case 0x31F:
        case 0x322:
            return func_ov030_020a9f54(arg0, func_ov030_0209efe4, 0x36E4, 2, 0x36E5, 0x36E6);
        case 0x31A:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x31D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x31B:
            FldGate_Clear(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        case 0x31D:
            FldGate_Set(0x31);
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x31E;
            gSaveData.unk_24C2 = 0x31F;
            gSaveData.unk_341C = 6;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x31E:
            FldEvent_SetCurrent(0x320);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x320:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x321;
            gSaveData.unk_24C2 = 0x322;
            gSaveData.unk_341C = 7;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x321:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x323);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x323:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x324);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x324:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x325);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x325:
            FldGate_Set(3);
            FldGate_Clear(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        case 0x326:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x327);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x327:
            FldGate_Set(4);
            gSaveData.unk_2676 = 2;
            break;
        case 0x328:
            FldGate_Set(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x42F:
            if (FldGate_Get(6) == 0) {
                FldGate_Set(6);
                FldEvent_SetCurrent(0x329);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x32B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x329:
        case 0x32B:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x32C;
            gSaveData.unk_24C2 = 0x32A;
            gSaveData.unk_341C = 0x16;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x32C:
            FldGate_Set(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x32F:
            FldGate_Set(33);
            arg0->unk_21CF8 = 1;
            break;
        case 0x430:
            if (FldGate_Get(39) == 0) {
                FldGate_Set(39);
                FldEvent_SetCurrent(0x330);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x332);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x330:
        case 0x332:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x333;
            gSaveData.unk_24C2 = 0x331;
            gSaveData.unk_341C = 23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x333:
            FldEvent_GrantPin(arg0, PIN_5_YEN);
            FldGate_Set(0x22);
            arg0->unk_21CF8 = 1;
            break;
        case 0x431:
            if (FldGate_Get(0x28) == 0) {
                FldGate_Set(0x28);
                FldEvent_SetCurrent(0x335);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x337);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x335:
        case 0x337:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x338;
            gSaveData.unk_24C2 = 0x336;
            gSaveData.unk_341C = 0x18;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x338:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldGate_Set(0x23);
            arg0->unk_21CF8 = 1;
            break;
        case 0x432:
            if (FldGate_Get(41) == 0) {
                FldGate_Set(41);
                FldEvent_SetCurrent(0x33A);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x33C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x33A:
        case 0x33C:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x33D;
            gSaveData.unk_24C2 = 0x33B;
            gSaveData.unk_341C = 0x19;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x33D:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_ROCKER);
            FldGate_Set(0x24);
            arg0->unk_21CF8 = 1;
            break;
        case 0x433:
            if (FldGate_Get(42) == 0) {
                FldGate_Set(42);
                FldEvent_SetCurrent(0x33F);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x341);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x33F:
        case 0x341:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x342;
            gSaveData.unk_24C2 = 0x340;
            gSaveData.unk_341C = 0x1A;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x342:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_DEVIL);
            FldGate_Set(37);
            arg0->unk_21CF8 = 1;
            break;
        case 0x434:
            if (FldGate_Get(43) == 0) {
                FldGate_Set(43);
                FldEvent_SetCurrent(0x344);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x346);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x344:
        case 0x346:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x347;
            gSaveData.unk_24C2 = 0x345;
            gSaveData.unk_341C = 0x1B;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x347:
            FldGate_Set(38);
            arg0->unk_21CF8 = 1;
            break;
        case 0x435:
            if (FldGate_Get(0x2C) == 0) {
                FldGate_Set(0x2C);
                FldEvent_SetCurrent(0x348);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x34A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x348:
        case 0x34A:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x34B;
            gSaveData.unk_24C2 = 0x349;
            gSaveData.unk_341C = 0x1C;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x34B:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_GOLEM);
            FldGate_Set(0x34);
            arg0->unk_21CF8 = 1;
            break;
        case 0x34D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x34E:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x34F;
            gSaveData.unk_24C2 = 0x350;
            gSaveData.unk_341C = 0x1D;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x34F:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_IFRIT);
        case 0x350:
            FldGate_Set(45);
            arg0->unk_21CF8 = 1;
            break;
        case 0x436:
            if (FldGate_Get(46) == 0) {
                FldGate_Set(46);
                FldEvent_SetCurrent(0x351);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x353);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x351:
        case 0x353:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x354;
            gSaveData.unk_24C2 = 0x352;
            gSaveData.unk_341C = 0x1E;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_ANOTHERDAY_WIZARD_OF_SLAM:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_BAHAMUT);
            FldEvent_GrantItem(arg0, ITEM_THREAD_WIZARDS_GLASSES);
            FldGate_Set(47);
            gSaveData.unk_2676 = 4;
            gSaveData.unk_2670 = 2;
            arg0->unk_21AD0    = 1;
            arg0->unk_21ACC    = 1;
            arg0->unk_21AFC    = 1;
            arg0->unk_21B00    = 0x66;
            arg0->unk_21B08    = 0x157000;
            arg0->unk_21B0C    = 0xD6000;
            break;
        case 0x373:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x374;
            gSaveData.unk_24C2 = 0x375;
            gSaveData.unk_341C = 2;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x374:
        case 0x375:
            FldGate_Set(56);
            arg0->unk_21CF8 = 1;
            break;
        case 0x376:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x377;
            gSaveData.unk_24C2 = 0x378;
            gSaveData.unk_341C = 3;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x377:
        case 0x378:
            FldGate_Set(0x39);
            arg0->unk_21CF8 = 1;
            break;
        case 0x379:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x37A;
            gSaveData.unk_24C2 = 0x37B;
            gSaveData.unk_341C = 38;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x37A:
        case 0x37B:
            FldGate_Set(58);
            arg0->unk_21CF8 = 1;
            break;
        case 0x37C:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x37D;
            gSaveData.unk_24C2 = 0x37E;
            gSaveData.unk_341C = 39;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x37D:
        case 0x37E:
            FldGate_Set(0x3B);
            arg0->unk_21CF8 = 1;
            break;
        case 0x37F:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x380;
            gSaveData.unk_24C2 = 0x381;
            gSaveData.unk_341C = 42;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x380:
        case 0x381:
            FldGate_Set(0x3C);
            arg0->unk_21CF8 = 1;
            break;
        case 0x382:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x383;
            gSaveData.unk_24C2 = 0x384;
            gSaveData.unk_341C = 0x28;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x383:
        case 0x384:
            FldGate_Set(61);
            arg0->unk_21CF8 = 1;
            break;
        case 0x385:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x386;
            gSaveData.unk_24C2 = 0x387;
            gSaveData.unk_341C = 41;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x386:
        case 0x387:
            FldGate_Set(0x3E);
            arg0->unk_21CF8 = 1;
            break;
        case 0x388:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x389;
            gSaveData.unk_24C2 = 0x38A;
            gSaveData.unk_341C = 4;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x389:
        case 0x38A:
            FldGate_Set(0x3F);
            arg0->unk_21CF8 = 1;
            break;
        case 0x38B:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x38C;
            gSaveData.unk_24C2 = 0x38D;
            gSaveData.unk_341C = 5;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x38C:
        case 0x38D:
            FldGate_Set(0x40);
            arg0->unk_21CF8 = 1;
            break;
        case 0x38E:
            FldGate_Set(0x41);
            arg0->unk_21CF8 = 1;
            break;
        case 0x421:
            if (FldGate_Get(0x4E) == 0) {
                FldGate_Set(0x4E);
                FldEvent_SetCurrent(0x392);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x393);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x392:
        case 0x393:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x394;
            gSaveData.unk_24C2 = 0x395;
            gSaveData.unk_341C = 8;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x394:
            FldGate_Set(0x42);
            func_ov030_020aac28(arg0, PIN_SPARKLE_CHARGE);
            arg0->unk_21CF8 = 1;
        case 0x395:
            arg0->unk_21CF8 = 1;
            break;
        case 0x422:
            if (FldGate_Get(0x4F) == 0) {
                FldGate_Set(0x4F);
                FldEvent_SetCurrent(0x396);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x397);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x396:
        case 0x397:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x398;
            gSaveData.unk_24C2 = 0x399;
            gSaveData.unk_341C = 11;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x398:
            FldGate_Set(0x43);
            func_ov030_020aac28(arg0, PIN_PEACE_FULL);
            arg0->unk_21CF8 = 1;
        case 0x399:
            arg0->unk_21CF8 = 1;
            break;
        case 0x423:
            if (FldGate_Get(0x50) == 0) {
                FldGate_Set(0x50);
                FldEvent_SetCurrent(0x39A);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x39B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x39A:
        case 0x39B:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x39C;
            gSaveData.unk_24C2 = 0x39D;
            gSaveData.unk_341C = 12;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x39C:
            FldGate_Set(0x44);
            func_ov030_020aac28(arg0, PIN_LADY_LUCK);
            arg0->unk_21CF8 = 1;
        case 0x39D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x424:
            if (FldGate_Get(0x51) == 0) {
                FldGate_Set(0x51);
                FldEvent_SetCurrent(0x39E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x39F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x39E:
        case 0x39F:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3A0;
            gSaveData.unk_24C2 = 0x3A1;
            gSaveData.unk_341C = 9;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A0:
            FldGate_Set(0x45);
            func_ov030_020aac28(arg0, PIN_LOLITA_SKULL);
            arg0->unk_21CF8 = 1;
        case 0x3A1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x425:
            if (FldGate_Get(0x52) == 0) {
                FldGate_Set(0x52);
                FldEvent_SetCurrent(0x3A2);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3A3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3A2:
        case 0x3A3:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3A4;
            gSaveData.unk_24C2 = 0x3A5;
            gSaveData.unk_341C = 13;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A4:
            FldGate_Set(0x46);
            func_ov030_020aac28(arg0, PIN_LUCKY_PANDA);
            arg0->unk_21CF8 = 1;
        case 0x3A5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x426:
            if (FldGate_Get(0x53) == 0) {
                FldGate_Set(0x53);
                FldEvent_SetCurrent(0x3A6);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3A7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3A6:
        case 0x3A7:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3A8;
            gSaveData.unk_24C2 = 0x3A9;
            gSaveData.unk_341C = 0x11;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A8:
            FldGate_Set(8);
            func_ov030_020aac28(arg0, PIN_ONE_STROKE_VAST_WEALTH);
            arg0->unk_21CF8 = 1;
        case 0x3A9:
            arg0->unk_21CF8 = 1;
            break;
        case 0x427:
            if (FldGate_Get(0x54) == 0) {
                FldGate_Set(0x54);
                FldEvent_SetCurrent(0x3AA);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3AB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3AA:
        case 0x3AB:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3AC;
            gSaveData.unk_24C2 = 0x3AD;
            gSaveData.unk_341C = 0xE;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3AC:
            FldGate_Set(0x47);
            func_ov030_020aac28(arg0, PIN_LUCKY_STAR);
            arg0->unk_21CF8 = 1;
        case 0x3AD:
            arg0->unk_21CF8 = 1;
            break;
        case 0x428:
            if (FldGate_Get(0x55) == 0) {
                FldGate_Set(0x55);
                FldEvent_SetCurrent(0x3AE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3AF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3AE:
        case 0x3AF:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3B0;
            gSaveData.unk_24C2 = 0x3B1;
            gSaveData.unk_341C = 18;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B0:
            FldGate_Set(0x48);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            arg0->unk_21CF8 = 1;
        case 0x3B1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x429:
            if (FldGate_Get(0x56) == 0) {
                FldGate_Set(0x56);
                FldEvent_SetCurrent(0x3B2);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3B3);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3B2:
        case 0x3B3:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3B4;
            gSaveData.unk_24C2 = 0x3B5;
            gSaveData.unk_341C = 15;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B4:
            FldGate_Set(0x49);
            FldEvent_GrantPin(arg0, PIN_10_YEN);
            arg0->unk_21CF8 = 1;
        case 0x3B5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42A:
            if (FldGate_Get(0x57) == 0) {
                FldGate_Set(0x57);
                FldEvent_SetCurrent(0x3B6);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3B7);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3B6:
        case 0x3B7:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3B8;
            gSaveData.unk_24C2 = 0x3B9;
            gSaveData.unk_341C = 0x10;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B8:
            FldGate_Set(0x4A);
            func_ov030_020aac28(arg0, PIN_LONG_LIVE_THE_ROCK);
            arg0->unk_21CF8 = 1;
        case 0x3B9:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42B:
            if (FldGate_Get(0x58) == 0) {
                FldGate_Set(0x58);
                FldEvent_SetCurrent(0x3BA);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3BB);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3BA:
        case 0x3BB:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3BC;
            gSaveData.unk_24C2 = 0x3BD;
            gSaveData.unk_341C = 10;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3BC:
            FldGate_Set(0x4B);
            func_ov030_020aac28(arg0, PIN_DISTORTION);
            arg0->unk_21CF8 = 1;
        case 0x3BD:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42C:
            if (FldGate_Get(0x59) == 0) {
                FldGate_Set(0x59);
                FldEvent_SetCurrent(0x3BE);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3BF);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3BE:
        case 0x3BF:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3C0;
            gSaveData.unk_24C2 = 0x3C1;
            gSaveData.unk_341C = 0x14;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C0:
            FldGate_Set(0x4C);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_SHIVA);
            arg0->unk_21CF8 = 1;
        case 0x3C1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42D:
            if (FldGate_Get(0x5B) == 0) {
                FldGate_Set(0x5B);
                FldEvent_SetCurrent(0x3C3);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3C4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3C3:
        case 0x3C4:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3C5;
            gSaveData.unk_24C2 = 0x3C6;
            gSaveData.unk_341C = 0x13;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C5:
            FldGate_Set(9);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_BLADE);
            arg0->unk_21CF8 = 1;
        case 0x3C6:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42E:
            if (FldGate_Get(0x5C) == 0) {
                FldGate_Set(0x5C);
                FldEvent_SetCurrent(0x3C7);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x3C8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3C7:
        case 0x3C8:
            arg0->unk_21630    = 6;
            gSaveData.unk_24C0 = 0x3C9;
            gSaveData.unk_24C2 = 0x3CA;
            gSaveData.unk_341C = 0x15;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C9:
            FldGate_Set(0x4D);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_DASH);
            arg0->unk_21CF8 = 1;
        case 0x3CA:
            arg0->unk_21CF8 = 1;
            break;
        case 0x360:
            gSaveData.unk_2670 = 4;
            break;
        case 0x366:
        case 0x36A:
            return func_ov030_020a9f54(arg0, func_ov030_0209f06c, 0x36E7, 2, 0x36E8, 0x36E9);
        case 0x367:
            if (FldGate_Get(0x6D) != 0) {
                gSaveData.unk_24B4          = 0;
                gSaveData.unk_24B8          = 1;
                gSaveData.currentStoryEvent = 0x36B;
                func_ov030_020af364(0x31);
                return 1;
            }
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_ANOTHERDAY_FINALTIMEATTACK_REWARD;
            func_ov030_020af364(0x31);
            return 1;
        case EVENT_ANOTHERDAY_FINALTIMEATTACK_REWARD:
            FldGate_Set(0x6D);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_THREAD_ANGEL_FEATHER);
            Inventory_AddItem(PIN_PYROKINESIS, 2);
            Inventory_AddItem(PIN_SHOCKWAVE, 2);
            Inventory_AddItem(PIN_CURE_DRINK, 2);
            Inventory_AddItem(PIN_FORCE_ROUNDS, 2);
            Inventory_AddItem(PIN_PSYCHOKINESIS, 2);
            Inventory_AddItem(PIN_THUNDERBOLT, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x356:
            gSaveData.unk_26D2 = 0;
            gSaveData.unk_2676 = 4;
            break;
        case 0x35E:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = EVENT_ANOTHERDAY_PORKCITY_ROOF_LEVEL;
            func_ov030_020af364(0x30);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_ROOF_LEVEL:
            FldGate_Set(0x66);
            FldEvent_GrantItem(arg0, ITEM_THREAD_MY_PHONES);
            gSaveData.unk_26AA = 0;
            FldGate_Clear(0x7E);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x157000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x83A:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x6E) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_1ST_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x6E) == 0)) {
                FldEvent_SetCurrent(0x861);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x83B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x83B:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_1ST_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_HAMBURGER);
        case 0x861:
            FldGate_Set(10);
            gSaveData.unk_267E = 4;
            gSaveData.unk_267C = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x83D:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x6F) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_2ND_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x6F) == 0)) {
                FldEvent_SetCurrent(0x862);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x83E);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x83E:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_2ND_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_SPECIAL_DOG);
        case 0x862:
            FldGate_Set(11);
            gSaveData.unk_2680 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x840:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x70) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_3RD_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x70) == 0)) {
                FldEvent_SetCurrent(0x863);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x841);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x841:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_3RD_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_DONUTS);
        case 0x863:
            FldGate_Set(12);
            gSaveData.unk_2682 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x843:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x71) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_4TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x71) == 0)) {
                FldEvent_SetCurrent(0x864);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x844);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x844:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_4TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_SHADOW_STEAK_RAMEN);
        case 0x864:
            FldGate_Set(13);
            gSaveData.unk_2686 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x846:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x72) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_5TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x72) == 0)) {
                FldEvent_SetCurrent(0x865);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x847);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x847:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_5TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_ATK_CAPSULES);
        case 0x865:
            FldGate_Set(0xE);
            gSaveData.unk_268A = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x849:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x73) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_6TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x73) == 0)) {
                FldEvent_SetCurrent(0x866);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x84A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x84A:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_6TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_DEF_MEDICINE);
        case 0x866:
            FldGate_Set(15);
            gSaveData.unk_268E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x84C:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x74) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_7TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x74) == 0)) {
                FldEvent_SetCurrent(0x867);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x84D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x84D:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_7TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_THE_TATSUMI_BURGER);
        case 0x867:
            FldGate_Set(0x10);
            gSaveData.unk_2692 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x84F:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x75) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_8TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x75) == 0)) {
                FldEvent_SetCurrent(0x868);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x850);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x850:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_8TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_DEFINITIVO_CHILI_DOG);
        case 0x868:
            FldGate_Set(0x11);
            gSaveData.unk_2696 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x852:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x76) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_9TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x76) == 0)) {
                FldEvent_SetCurrent(0x869);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x853);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x853:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_9TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_MYSTIC_RAMEN);
        case 0x869:
            FldGate_Set(18);
            gSaveData.unk_269A = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x855:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x77) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_10TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x77) == 0)) {
                FldEvent_SetCurrent(0x86A);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x856);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x856:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_10TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_ABSOLUTE_SHADOW_RAMEN);
        case 0x86A:
            FldGate_Set(0x13);
            gSaveData.unk_269E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x858:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x78) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_11TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x78) == 0)) {
                FldEvent_SetCurrent(0x86B);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x859);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x859:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_11TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_VIPER_DRINK);
        case 0x86B:
            FldGate_Set(0x14);
            gSaveData.unk_26A2 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x85B:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x79) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_12TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x79) == 0)) {
                FldEvent_SetCurrent(0x86C);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x85C);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x85C:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_12TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_MAKO_SYNERGY);
        case 0x86C:
            FldGate_Set(0x15);
            gSaveData.unk_26A6 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x85E:
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x7A) != 0)) {
                FldEvent_SetCurrent(EVENT_ANOTHERDAY_PORKCITY_13TH_REWARD);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (FldGate_Get(0x7A) == 0)) {
                FldEvent_SetCurrent(0x86D);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldEvent_SetCurrent(0x85F);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x85F:
            break;
        case EVENT_ANOTHERDAY_PORKCITY_13TH_REWARD:
            FldEvent_GrantPin(arg0, PIN_10000_YEN);
            FldEvent_GrantItem(arg0, ITEM_FOOD_CURIOUS_MUSHROOMS);
        case 0x86D:
            FldGate_Set(0x16);
            gSaveData.unk_26AA = 2;
            arg0->unk_21CF8    = 1;
            break;
        case EVENT_ANOTHERDAY_SECRET_BOX_CONCERT_STAGE:
            FldGate_Set(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_GIRLS_UNIFORM_WITH_SWEATER, 1, 0) != 0) {
                FldEvent_GrantPin(arg0, PIN_500_YEN);
            } else {
                FldEvent_GrantItem(arg0, ITEM_THREAD_GIRLS_UNIFORM_WITH_SWEATER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_020a1ea8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x37;
}

s32 func_ov030_020a1ed0(FieldEventManager* arg0) {
    if (gSaveData.unk_2458 == 9) {
        if ((FldGate_Get(3) != 0) && (FldGate_Get(4) == 0)) {
            FldGate_Set(4);
            FldEvent_BeginScript(arg0, 0x592, 0);
            return 1;
        }
        if (FldGate_Get(3) == 0) {
            FldGate_Set(3);
            arg0->unk_21AF4 = 0x96;
            arg0->unk_21AF0 = 1;
        }
    }

    if ((gSaveData.unk_2458 == 18) && (func_ov030_02084860(0x181, 0xC2, 0x14) != 0)) {
        arg0->unk_21AF0 = 1;
        arg0->unk_21AD0 = 1;
        arg0->unk_21ACC = 0;
        arg0->unk_21AFC = 9;
        arg0->unk_21B00 = 0x66;
        arg0->unk_21B08 = 0x109000;
        arg0->unk_21B0C = 0x11F000;
    }
    return 0;
}

s32 func_ov030_020a1fdc(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x35EA:
            FldEvent_SetCurrent(0x595);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35EB:
            FldEvent_SetCurrent(0x596);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            FldEvent_SetCurrent(0x598);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
}

s32 func_ov030_020a206c(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x593:
            func_ov030_020aa9f4(arg0, 0x35EB, 0x5DD1, 0);
            break;
        case 0x594:
        case 0x598:
            return func_ov030_020aa0c8(arg0, 3, 0x598, 0x597, func_ov030_020a1fdc);
        case 0x592:
        default:
            break;
    }
    return 0;
}

void func_ov030_020a20f8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 56;
}

s32 func_ov030_020a2120(FieldEventManager* eventMgr) {
    if (gSaveData.unk_2458 == 18) {
        if (FldGate_Get(GATE_GAME_CLEARED)) {
            FldGate_Clear(GATE_GAME_CLEARED);
            FldEvent_BeginScript(eventMgr, 0x599, 0);
            return 1;
        } else if (FldGate_Get(1) != 0) {
            FldEvent_BeginScript(eventMgr, 0x59B, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020a21a4(FieldEventManager* arg0) {
    if (gSaveData.currentStoryEvent == 0x59A) {
        func_ov030_020b5d14(EasyTask_GetTaskById(&arg0->taskPool, arg0->unk_21788[gSaveData.unk_24A4]), 1, 0x5DDB, 0x10000);
    }
    return 0;
}

void func_ov030_020a2208(FieldEventManager* arg0) {
    func_ov030_020ae96c(23);
    gSaveData.unk_244C          = 0x12C000;
    gSaveData.unk_2450          = 0x87000;
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = EVENTFLAG_NOBGM | 0x86E;
    func_ov030_020c26bc(0);
    gSaveData.unk_267A = 4;
}

s32 func_ov030_020a2268(FieldEventManager* arg0) {
    s16 var_r0;

    if (gSaveData.unk_2458 == 0x15) {
        if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
            FldGate_Set(GATE_GAME_CLEARED);
            FldEvent_BeginScript(arg0, 0x86F, 0);
            return 1;
        }
        if (gSaveData.unk_3100 == gSaveData.unk_2458) {
            var_r0 = gSaveData.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (FldGate_Get(3) == 0)) {
            FldGate_Set(3);
            gSaveData.unk_2678 = 4;
            FldEvent_BeginScript(arg0, 0x874, 0);
            return 1;
        }
    }
    if (gSaveData.unk_2458 == 0x16) {
        if (FldGate_Get(4) == 0) {
            FldGate_Set(4);
            FldEvent_BeginScript(arg0, 0x877, 0);
            return 1;
        } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(6) == 0)) {
            FldGate_Set(6);
            FldEvent_BeginScript(arg0, 0x880, 0);
            return 1;
        }
    }

    if ((gSaveData.unk_2458 == 23) && (func_ov030_020aed9c(0x3715) != 0) && (FldGate_Get(15) == 0)) {
        FldGate_Set(15);
        FldEvent_BeginScript(arg0, 0x87F, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_020a240c(FieldEventManager* arg0) {
    switch (gSaveData.unk_24C8[gSaveData.unk_24C4]) {
        case 0x35F9:
            if (FldGate_Get(15) != 0) {
                FldEvent_SetCurrent(0x87E);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            FldGate_Set(5);
            FldEvent_SetCurrent(0x87D);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FA:
            FldGate_Set(42);
            FldEvent_SetCurrent(0x888);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            FldEvent_SetCurrent(0x887);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
}

s32 func_ov030_020a24d8(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            FldGate_Set(9);
            FldEvent_SetCurrent(0x879);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3606:
            FldGate_Set(10);
            FldEvent_SetCurrent(0x87A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020a2568(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            FldGate_Set(9);
            FldEvent_SetCurrent(0x879);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3606:
            FldGate_Set(10);
            FldEvent_SetCurrent(0x87A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x3607:
            FldGate_Set(11);
            FldEvent_SetCurrent(0x87B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020a2628(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x86E:
            FldEvent_SetCurrent(0x889);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x86F:
            FldEvent_SetCurrent(0x88A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x88A:
            FldEvent_GrantPin(arg0, PIN_PYROKINESIS);
            FldEvent_GrantPin(arg0, PIN_PSYCHOKINESIS);
            FldEvent_GrantPin(arg0, PIN_THUNDERBOLT);
            FldEvent_GrantPin(arg0, PIN_SHOCKWAVE);
            FldEvent_GrantPin(arg0, PIN_CURE_DRINK);
            break;
        case 0x870:
            FldGate_Set(1);
            arg0->unk_21CF8 = 1;
            break;
        case 0x872:
            FldGate_Set(2);
            s16 temp           = gSaveData.unk_2458;
            gSaveData.unk_30FC = 0;
            gSaveData.unk_30FE = 39;
            gSaveData.unk_3100 = temp;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x874:
            arg0->unk_21CF8 = 1;
            break;
        case 0x878:
        case 0x879:
        case 0x87A:
        case 0x87B:
            if ((FldGate_Get(9) != 0) && (FldGate_Get(10) != 0) && (FldGate_Get(11) != 0)) {
                FldEvent_SetCurrent(0x87C);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            if (FldGate_Get(9) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_020a2568, 0x3604, 3, 0x3605, 0x3606, 0x3607);
            }
            return func_ov030_020a9f54(arg0, func_ov030_020a24d8, 0x3604, 2, 0x3605, 0x3606);
        case 0x87C:
            FldGate_Set(12);
            arg0->unk_21CF8 = 1;
            func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
            break;
        case 0x885:
        case 0x886:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x886, 0x887, func_ov030_020a240c);
        case 0x888:
            FldGate_Set(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x880:
            arg0->unk_21CF8 = 1;
            break;
        case 0x87F:
            FldGate_Set(15);
            func_ov030_020aa9f4(arg0, 0x35FA, 0x5DD1, 0);
            break;
        case 0x881:
            FldGate_Set(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x882:
            FldGate_Set(8);
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x883;
            func_ov030_020af364(10);
            return 1;
        case 0x883:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 0x884;
            gSaveData.unk_3124          = 10;
            gSaveData.unk_3128          = 0;
            arg0->unk_21630             = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x884:
            gSaveData.unk_3124 = 0x4D;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
    }
    return 0;
}

void func_ov030_020a2a44(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 58;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_020a2a84(void) {
    return 0;
}

s32 func_ov030_020a2a8c(FieldEventManager* arg0) {
    switch (arg0->unk_21D1C) {
        case 0:
            FldEvent_SetCurrent(0x44E);
            return 1;
        case 1:
            if (FldGate_Get(2) != 0) {
                func_ov030_020a9db8(arg0, 0x45F);
                return 0;
            }
            FldGate_Set(2);
            FldEvent_SetCurrent(0x451);
            return 1;
        case 2:
            FldEvent_SetCurrent(0x452);
            return 1;
        case 3:
            FldEvent_SetCurrent(0x455);
            return 1;
        case 4:
            FldEvent_SetCurrent(0x456);
            return 1;
        case 5:
            FldEvent_SetCurrent(0x457);
            return 1;
        case 6:
            FldEvent_SetCurrent(0x458);
            return 1;
        case 7:
            FldEvent_SetCurrent(0x459);
            return 1;
        case 8:
            FldEvent_SetCurrent(0x45A);
            return 1;
        case 9:
            FldEvent_SetCurrent(0x45B);
            return 1;
        case 10:
            FldEvent_SetCurrent(0x45C);
            return 1;
        case 11:
            FldEvent_SetCurrent(0x45D);
            return 1;
        case 13:
            func_ov030_020a9db8(arg0, 0x45E);
            return 0;
        default:
        case 12:
            return 0;
    }
}

s32 func_ov030_020a2c20(FieldEventManager* arg0) {
    if ((gSaveData.currentStoryEvent == 0xE3) || (gSaveData.currentStoryEvent == 0x45F)) {
        if (func_ov030_020a9d00(arg0, 0x44D) == 0) {
            return 1;
        }
        arg0->unk_21D22 = 3;
        arg0->unk_21D24 = func_ov030_020a2a8c;
        arg0->unk_21D28 = 0x5ED6;
        arg0->unk_21D18 = 0;
        DebugOvlDisp_Push(&func_ov030_020824a0, arg0, 0);
        DebugOvlDisp_Push(&func_ov030_02082c04, arg0, 0);
        DebugOvlDisp_Push(&func_ov030_02082bf0, arg0, 0);
        DebugOvlDisp_Push(&func_ov030_02082c68, arg0, 0);
        DebugOvlDisp_Push(&func_ov030_02082534, arg0, 0);
        return 1;
    }
    return 0;
}

void func_ov030_020a2d0c(void) {
    func_ov030_020ae96c(0x18);
    func_ov030_020c26bc(0);
    gSaveData.unk_267C = 4;
    gSaveData.unk_2680 = 4;
    gSaveData.unk_2682 = 4;
    func_ov030_020aec38(1);
    gSaveData.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_020a2d54(void) {
    return 0;
}

s32 func_ov030_020a2d5c(FieldEventManager* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35FA:
            FldEvent_SetCurrent(0x5A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x35FB:
            FldEvent_SetCurrent(0x5A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            FldEvent_SetCurrent(0x5A5);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
}

s32 func_ov030_020a2de8(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x5A0:
            func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
            break;
        case 0x5A3:
            return func_ov030_020a9f54(arg0, func_ov030_020a2d5c, 0x35F9, 2, 0x35FA, 0x35FB);
        case 0x5A6:
            FldEvent_GrantItem(arg0, ITEM_THREAD_BLACK_BOOK_BAG);
            FldEvent_GrantItem(arg0, ITEM_THREAD_FALSE_TEETH);
            Inventory_AddItem(PIN_PYROKINESIS, 2);
            Inventory_AddItem(PIN_SHOCKWAVE, 2);
            Inventory_AddItem(PIN_CURE_DRINK, 2);
            Inventory_AddItem(PIN_FORCE_ROUNDS, 2);
            Inventory_AddItem(PIN_PSYCHOKINESIS, 2);
            Inventory_AddItem(PIN_THUNDERBOLT, 2);
            break;
        case 0x5A7:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x5A8);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x5A9:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            FldEvent_SetCurrent(0x5AA);
            FldEvent_AdvanceScript(arg0);
            return 1;
    }
    return 0;
}

void func_ov030_020a2f74(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124          = 0x3C;
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = 0x8A;
}

s32 func_ov030_020a2fc4(void) {
    if (gSaveData.unk_2458 == 18) {
        if ((FldGate_Get(15) != 0) && (FldGate_Get(2) == 0)) {
            FldGate_Set(2);
        }

        s32 var_r0 = (gSaveData.unk_3100 == gSaveData.unk_2458) ? gSaveData.unk_30FC : 0;
        if ((var_r0 >= 2) && (FldGate_Get(1) == 0)) {
            FldGate_Set(1);
            FldGate_Set(0x1E);
            FldGate_Set(0x1F);
            FldGate_Set(0x20);
        }
    }
    return 0;
}

s32 func_ov030_020a306c(FieldEventManager* arg0) {
    if ((gSaveData.currentStoryEvent == 0x8A) && (FldGate_Get(GATE_GAME_CLEARED) == FALSE)) {
        FldGate_Set(GATE_GAME_CLEARED);
    }
    return 0;
}

void func_ov030_020a30a4(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 61;
}

s32 func_ov030_020a30cc(FieldEventManager* arg0) {
    return 0;
}

s32 func_ov030_020a30d4(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x5B3:
            FldEvent_SetCurrent(0x5B4);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x5B4:
            FldEvent_SetCurrent(0x5B5);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x5B5:
            FldEvent_SetCurrent(0x5B6);
            FldEvent_AdvanceScript(arg0);
            return 1;
        default:
            return 0;
    }
}

void func_ov030_020a315c(void) {
    func_ov030_020ae96c(0x10);
    func_ov030_020c26bc(4);
}

s32 func_ov030_020a3174(void) {
    return 0;
}

s32 func_ov030_020a317c(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3184(void) {
    func_ov030_020aec1c(4);
    gSaveData.unk_26D4 = 0;
    func_ov030_020ae96c(0x10);
    func_ov030_020c26bc(4);
}

s32 func_ov030_020a31b4(void) {
    return 0;
}

s32 func_ov030_020a31bc(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a31c4(void) {
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(0);
    gSaveData.unk_24B4          = 0;
    gSaveData.unk_24B8          = 1;
    gSaveData.currentStoryEvent = 0x88B;
    func_ov030_020aec1c(1);
    gSaveData.playerStats.activeFriend = FRIEND_JOSHUA;
}

s32 func_ov030_020a321c(s32 arg0) {
    if ((gSaveData.unk_2458 == 1) && FldGate_Get(GATE_GAME_CLEARED) && (FldGate_Get(2) == 0)) {
        FldGate_Set(2);
        FldEvent_BeginScript(arg0, 0x88E, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 15) && (FldGate_Get(10) != 0) && (FldGate_Get(11) != 0) && (FldGate_Get(12) != 0) &&
               (FldGate_Get(7) == 0))
    {
        FldGate_Set(7);
        FldEvent_BeginScript(arg0, 0x896, 0);
        return 1;
    } else if ((gSaveData.unk_2458 == 0x10) && (FldGate_Get(9) == 0)) {
        FldGate_Set(9);
        FldEvent_BeginScript(arg0, 0x899, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_020a333c(FieldEventManager* arg0) {
    switch (gSaveData.currentStoryEvent) {
        case 0x88C:
            FldGate_Set(GATE_GAME_CLEARED);
            FldEvent_SetCurrent(0x89A);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x89A:
            FldEvent_GrantPin(arg0, PIN_LOVELY_BEAM);
            FldEvent_GrantPin(arg0, PIN_ICHIMONJI);
            FldEvent_GrantPin(arg0, PIN_ONE_STONE_MANY_BIRDS);
            FldEvent_GrantPin(arg0, PIN_EARTHSHAKE);
            FldEvent_GrantPin(arg0, PIN_MASAMUNE);
            FldEvent_GrantPin(arg0, PIN_HEALING_BUNNY);
            arg0->unk_21CF8 = 1;
            break;
        case 0x88E:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 2189;
            func_ov030_020af364(0x39);
            return 1;
        case 0x88D:
            FldGate_Set(1);
            gSaveData.unk_264E = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x88F:
            FldEvent_SetCurrent(0x89B);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x89B:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 2192;
            func_ov030_020af364(0x35);
            return 1;
        case 0x890:
            FldGate_Set(3);
            gSaveData.unk_2664 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x891:
            FldEvent_SetCurrent(2204);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x89C:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 2194;
            func_ov030_020af364(54);
            return 1;
        case 0x892:
            FldGate_Set(4);
            gSaveData.unk_2666 = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x893:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 2196;
            func_ov030_020af364(55);
            return 1;
        case 0x894:
            FldGate_Set(5);
            gSaveData.unk_2668 = 4;
            FldEvent_SetCurrent(2205);
            FldEvent_AdvanceScript(arg0);
            return 1;
        case 0x89D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x895:
            if (FldGate_Get(6) == 0) {
                FldGate_Set(6);
                gSaveData.unk_310C = &data_ov030_020d9be4;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
                FldEvent_SetCurrent(2206);
                FldEvent_AdvanceScript(arg0);
                return 1;
            }
            break;
        case 0x89E:
            FldEvent_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            break;
        case 0x896:
            FldGate_Set(7);
            gSaveData.unk_266A = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x897:
            gSaveData.unk_24B4          = 0;
            gSaveData.unk_24B8          = 1;
            gSaveData.currentStoryEvent = 2200;
            func_ov030_020af364(56);
            return 1;
        case 0x898:
            FldGate_Set(8);
            gSaveData.unk_266C = 4;
            arg0->unk_21CF8    = 1;
            break;
        case 0x899:
            gSaveData.unk_3124 = 0x4D;
            gSaveData.unk_3128 = 1;
            arg0->unk_21630    = 4;
            DebugOvlDisp_Pop();
            return 0;
    }
    return 0;
}

void func_ov030_020a36d8(void) {
    func_ov030_020ae96c(0x10);
    func_ov030_020c26bc(0);
}

s32 func_ov030_020a36f0(s32 arg0) {
    if (gSaveData.unk_2458 == 0x10) {
        func_ov030_020848e4(arg0, 0x44);
    }
    return 0;
}

s32 func_ov030_020a3718(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3720(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x42;
}

s32 func_ov030_020a3748(void) {
    return 0;
}

s32 func_ov030_020a3750(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3758(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x43;
}

s32 func_ov030_020a3780(void) {
    return 0;
}

s32 func_ov030_020a3788(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3790(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x44;
}

s32 func_ov030_020a37b8(void) {
    return 0;
}

s32 func_ov030_020a37c0(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a37c8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x45;
}

s32 func_ov030_020a37f0(void) {
    return 0;
}

s32 func_ov030_020a37f8(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3800(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x46;
}

s32 func_ov030_020a3828(void) {
    return 0;
}

s32 func_ov030_020a3830(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3838(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
}

s32 func_ov030_020a3850(void) {
    return 0;
}

s32 func_ov030_020a3858(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3860(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x48;
}

s32 func_ov030_020a3888(void) {
    return 0;
}

s32 func_ov030_020a3890(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3898(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x49;
}

s32 func_ov030_020a38c0(void) {
    return 0;
}

s32 func_ov030_020a38c8(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a38d0(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x4A;
}

s32 func_ov030_020a38f8(void) {
    return 0;
}

s32 func_ov030_020a3900(FieldEventManager* arg0) {
    return 0;
}

void func_ov030_020a3908(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveData.unk_3124 = 0x4B;
}

s32 func_ov030_020a3930(void) {
    return 0;
}

s32 func_ov030_020a3938(FieldEventManager* arg0) {
    return 0;
}

s32 func_ov030_020a3940(void) {
    if (gSaveData.unk_2458 == 1) {
        return 0x28;
    } else if (gSaveData.unk_2458 == 3) {
        return 41;
    } else {
        return 0;
    }
}

s32 func_ov030_020a3968(void) {
    if (gSaveData.unk_2458 == 1) {
        if (FldGate_Get(33) == 0) {
            return 0x5C0;
        } else if ((FldGate_Get(38) != 0) && (FldGate_Get(0x28) == 0)) {
            return 0x5BF;
        }
    }

    if ((gSaveData.unk_2458 == 5) && (FldGate_Get(38) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x5BF;
    } else {
        return 0;
    }
}

s32 func_ov030_020a39f8(void) {
    if (gSaveData.unk_2458 == 5) {
        if ((FldGate_Get(45) != 0) && (FldGate_Get(6) == 0)) {
            return 0x5C8;
        } else if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
            return 0x5CC;
        }
    }

    if (FldGate_Get(0x20) == 0) {
        return 0x5CD;
    } else if ((FldGate_Get(0x20) != 0) && (FldGate_Get(33) == 0)) {
        return 0x5C4;
    } else if ((FldGate_Get(33) != 0) && (FldGate_Get(3) == 0)) {
        return 0x5C5;
    } else if ((FldGate_Get(0x22) != 0) && (FldGate_Get(42) == 0)) {
        return 0x5CB;
    } else if ((FldGate_Get(42) != 0) && (FldGate_Get(43) == 0) && (FldGate_Get(5) == 0)) {
        return 0x5C6;
    } else if ((FldGate_Get(43) != 0) && (FldGate_Get(5) == 0)) {
        return 0x5C7;
    } else if ((FldGate_Get(5) != 0) && (FldGate_Get(6) == 0)) {
        return 0x5CE;
    } else if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
        return 0x5CF;
    } else if ((FldGate_Get(7) != 0) && (FldGate_Get(47) == 0)) {
        return 0x5C9;
    } else if ((FldGate_Get(47) != 0) && (FldGate_Get(8) == 0)) {
        return 0x5CA;
    } else {
        return 0;
    }
}

s32 func_ov030_020a3bf8(void) {
    if (gSaveData.unk_2458 == 0x13) {
        if ((FldGate_Get(5) != 0) && (FldGate_Get(0x3F) != 0) && (FldGate_Get(0x24) == 0)) {
            return 0x5DC;
        } else if ((FldGate_Get(7) != 0) && (FldGate_Get(8) == 0)) {
            return 0x5E9;
        } else if ((FldGate_Get(8) != 0) && (FldGate_Get(2) == 0)) {
            return 0x5DD;
        } else if ((FldGate_Get(9) != 0) && (FldGate_Get(10) == 0)) {
            return 0x5DE;
        } else if ((FldGate_Get(10) != 0) && (FldGate_Get(11) == 0)) {
            return 0x5DF;
        } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(0x10) == 0) && (FldGate_Get(41) == 0) && (FldGate_Get(0x13) == 0)) {
            return 0x5E7;
        } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(13) != 0) && (FldGate_Get(0x10) == 0)) {
            return 0x5E7;
        } else if ((FldGate_Get(0x10) != 0) && (FldGate_Get(15) == 0)) {
            return 0x5E8;
        }
    }

    if (gSaveData.unk_2458 == 0x15) {
        if ((FldGate_Get(0x20) != 0) && (FldGate_Get(5) == 0) && (FldGate_Get(10) == 0)) {
            return 0x5D9;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(0x24) == 0)) {
            return 0x5DA;
        } else if ((FldGate_Get(9) != 0) && (FldGate_Get(10) == 0)) {
            return 0x5F2;
        } else if ((FldGate_Get(0x42) != 0) && (FldGate_Get(0x43) == 0)) {
            return 0x5DB;
        } else if ((FldGate_Get(0x43) != 0) && (FldGate_Get(4) == 0)) {
            return 0x5F1;
        } else if ((FldGate_Get(10) != 0) && (FldGate_Get(11) == 0)) {
            return 0x5DF;
        } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(0x10) == 0) && (FldGate_Get(41) == 0) && (FldGate_Get(0x13) == 0)) {
            return 0x5E6;
        } else if ((FldGate_Get(15) != 0) && (FldGate_Get(0x11) == 0)) {
            return 0x5EA;
        }
    }

    if (gSaveData.unk_2458 == 0x16) {
        if ((FldGate_Get(0x32) != 0) && (FldGate_Get(0x33) == 0)) {
            return 0x5EC;
        } else if ((FldGate_Get(0x33) != 0) && (FldGate_Get(6) == 0)) {
            return 0x5E0;
        } else if ((FldGate_Get(11) != 0) && (FldGate_Get(0x4A) == 0) && (FldGate_Get(37) == 0)) {
            return 0x5E2;
        } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(0x10) == 0) && (FldGate_Get(41) == 0) && (FldGate_Get(0x13) == 0)) {
            return 0x5E6;
        } else if ((FldGate_Get(0x10) != 0) && (FldGate_Get(15) == 0)) {
            return 0x5E8;
        }
    }

    if (gSaveData.unk_2458 == 23) {
        if ((FldGate_Get(45) != 0) && (FldGate_Get(0x20) == 0)) {
            return 0x5D8;
        } else if ((FldGate_Get(9) != 0) && (FldGate_Get(10) == 0)) {
            return 0x5EB;
        } else if ((FldGate_Get(0x10) != 0) && (FldGate_Get(15) == 0)) {
            return 0x5EB;
        } else if (FldGate_Get(15) != 0) {
            return 0x5ED;
        } else if ((FldGate_Get(0x4A) != 0) && (FldGate_Get(38) == 0)) {
            return 0x5E4;
        } else if ((FldGate_Get(38) != 0) && (FldGate_Get(0xE) == 0)) {
            return 0x5E5;
        } else if (FldGate_Get(45) != 0) {
            return 0x5EF;
        }
    }

    if ((FldGate_Get(7) != 0) && (FldGate_Get(9) == 0)) {
        return 0x5F0;
    } else if ((FldGate_Get(11) != 0) && (FldGate_Get(0x4A) == 0) && (FldGate_Get(37) == 0)) {
        return 0x5F3;
    } else if ((FldGate_Get(0x4A) != 0) && (FldGate_Get(38) == 0)) {
        return 0x5E3;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(38) == 0) && (FldGate_Get(13) == 0)) {
        return 0x5EE;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(38) != 0) && (FldGate_Get(13) == 0)) {
        return 0x5E5;
    } else if ((FldGate_Get(38) != 0) && (FldGate_Get(0xE) == 0)) {
        return 0x5E5;
    } else if ((FldGate_Get(38) != 0) && (FldGate_Get(0xE) != 0) && (FldGate_Get(12) != 0) && (FldGate_Get(13) == 0)) {
        return 0x5E5;
    } else if ((FldGate_Get(15) != 0) && (FldGate_Get(0x11) == 0)) {
        return 0x5EA;
    } else {
        return 0;
    }
}

s32 func_ov030_020a428c(void) {
    if (gSaveData.unk_2458 == 3) {
        if (FldGate_Get(4) == 0) {
            return 0x5F6;
        } else if ((FldGate_Get(4) != 0) && (FldGate_Get(33) == 0)) {
            return 0x5F7;
        }
    }

    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(47) != 0) && (FldGate_Get(0x34) == 0)) {
            return 0x5F8;
        } else if ((FldGate_Get(0x34) != 0) && (FldGate_Get(2) == 0)) {
            return 0x5FA;
        }
    }

    if (gSaveData.unk_2458 == 11) {
        if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x31) == 0)) {
            return 0x5F9;
        } else if ((FldGate_Get(0x31) != 0) && (FldGate_Get(3) == 0)) {
            return 0x5FB;
        }
    }

    if ((FldGate_Get(33) != 0) && (FldGate_Get(6) == 0)) {
        return 0x5FC;
    } else {
        return 0;
    }
}

s32 func_ov030_020a43d4(void) {
    if (gSaveData.unk_2458 == 6) {
        if ((FldGate_Get(0x43) != 0) && (FldGate_Get(0x44) == 0)) {
            return 0x60A;
        } else if ((FldGate_Get(0x44) != 0) && (FldGate_Get(3) == 0)) {
            return 0x60F;
        }
    }

    if (gSaveData.unk_2458 == 0) {
        if ((FldGate_Get(0x20) != 0) && (FldGate_Get(5) == 0)) {
            return 0x60B;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(6) == 0)) {
            return 0x60C;
        } else if ((FldGate_Get(9) != 0) && (FldGate_Get(0x23) == 0)) {
            return 0x60E;
        }
    }
    if ((FldGate_Get(6) != 0) && (FldGate_Get(8) == 0)) {
        return 0x60D;
    } else {
        return 0;
    }
}

s32 func_ov030_020a44f4(void) {
    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(11) != 0) && (FldGate_Get(12) == 0)) {
            return 0x61F;
        } else if ((FldGate_Get(12) != 0) && (FldGate_Get(13) == 0)) {
            return 0x621;
        } else if ((FldGate_Get(13) != 0) && (FldGate_Get(0xE) == 0)) {
            return 0x622;
        } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(33) == 0)) {
            return 0x623;
        } else if ((FldGate_Get(15) != 0) && (FldGate_Get(0x10) == 0)) {
            return 0x626;
        } else if ((FldGate_Get(0x10) != 0) && (FldGate_Get(0x11) == 0)) {
            return 0x627;
        } else if ((FldGate_Get(0x11) != 0) && (FldGate_Get(18) == 0)) {
            return 0x628;
        } else if ((FldGate_Get(18) != 0) && (FldGate_Get(0x13) == 0)) {
            return 0x629;
        }
    }
    if (gSaveData.unk_2458 == 4) {
        if ((FldGate_Get(33) != 0) && (FldGate_Get(10) == 0)) {
            return 0x624;
        } else if ((FldGate_Get(47) != 0) && (FldGate_Get(0x33) == 0)) {
            return 0x62C;
        } else if ((FldGate_Get(0x33) != 0) && (FldGate_Get(4) == 0)) {
            return 0x62D;
        }
    }

    if ((gSaveData.unk_2458 == 0) && (FldGate_Get(0x23) != 0) && (FldGate_Get(9) == 0)) {
        return 0x624;
    } else if ((gSaveData.unk_2458 == 3) && (FldGate_Get(0x14) != 0) && (FldGate_Get(0x15) == 0)) {
        return 0x62B;
    } else if ((gSaveData.unk_2458 == 2) && (FldGate_Get(10) != 0) && (FldGate_Get(0x23) == 0)) {
        return 0x62E;
    } else if ((gSaveData.unk_2458 == 6) && (FldGate_Get(10) != 0) && (FldGate_Get(0x23) == 0)) {
        return 0x62E;
    } else if (FldGate_Get(11) == 0) {
        return 0x61E;
    } else if ((FldGate_Get(11) != 0) && (FldGate_Get(12) == 0)) {
        return 0x620;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(0xE) == 0)) {
        return 0x631;
    } else if ((FldGate_Get(0xE) != 0) && (FldGate_Get(33) == 0)) {
        return 0x623;
    } else if ((FldGate_Get(10) != 0) && (FldGate_Get(0x23) == 0)) {
        return 0x625;
    } else if ((FldGate_Get(0x11) != 0) && (FldGate_Get(18) == 0)) {
        return 0x62F;
    } else if ((FldGate_Get(18) != 0) && (FldGate_Get(0x13) == 0)) {
        return 0x630;
    } else if ((FldGate_Get(0x13) != 0) && (FldGate_Get(0x14) == 0)) {
        return 0x62A;
    } else {
        return 0;
    }
}

s32 func_ov030_020a48d8(void) {
    if (gSaveData.unk_2458 == 5) {
        if ((FldGate_Get(43) != 0) && (FldGate_Get(0x2C) == 0)) {
            return 0x633;
        } else if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(5) == 0)) {
            return 0x634;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(0x20) == 0)) {
            return 0x635;
        }
    }
    if ((gSaveData.unk_2458 == 0x11) && (FldGate_Get(5) != 0) && (FldGate_Get(0x20) == 0)) {
        return 0x635;
    } else if (FldGate_Get(5) != 0) {
        return 0;
    } else
        return 0x632;
}

s32 func_ov030_020a49b8(void) {
    if ((gSaveData.unk_2458 == 1) && (FldGate_Get(0x23) != 0) && (FldGate_Get(37) == 0)) {
        return 0x646;
    } else if ((gSaveData.unk_2458 == 5) && (FldGate_Get(0x23) != 0) && (FldGate_Get(37) == 0)) {
        return 0x646;
    } else {
        return 0;
    }
}

s32 func_ov030_020a4a34(void) {
    if ((gSaveData.unk_2458 == 1) && (FldGate_Get(0x20) != 0) && (FldGate_Get(0x22) == 0)) {
        return 0x64B;
    }
    if (gSaveData.unk_2458 == 11) {
        if ((FldGate_Get(4) != 0) && (FldGate_Get(46) == 0)) {
            return 0x64D;
        } else if ((FldGate_Get(46) != 0) && (FldGate_Get(3) == 0)) {
            return 0x651;
        }
    }

    if ((gSaveData.unk_2458 == 12) && (FldGate_Get(3) != 0) && (FldGate_Get(37) == 0)) {
        return 0x64E;
    } else if ((gSaveData.unk_2458 == 8) && (FldGate_Get(37) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x650;
    } else if ((FldGate_Get(7) != 0) && (FldGate_Get(37) == 0)) {
        return 0x64C;
    } else if ((FldGate_Get(37) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x64F;
    } else {
        return 0;
    }
}

s32 func_ov030_020a4b98(void) {
    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(0x20) == 0) && (FldGate_Get(33) == 0)) {
            return 0x65F;
        } else if ((FldGate_Get(10) != 0) && (FldGate_Get(47) == 0)) {
            return 0x668;
        }
    }

    if ((gSaveData.unk_2458 == 3) && (FldGate_Get(10) != 0) && (FldGate_Get(47) == 0)) {
        return 0x668;
    }
    if (gSaveData.unk_2458 == 12) {
        if ((FldGate_Get(33) != 0) && (FldGate_Get(0x46) == 0)) {
            return 0x660;
        } else if ((FldGate_Get(0x46) != 0) && (FldGate_Get(0x47) == 0)) {
            return 0x661;
        } else if ((FldGate_Get(0x47) != 0) && (FldGate_Get(5) == 0)) {
            return 0x672;
        }
    }

    if ((gSaveData.unk_2458 == 13) && (FldGate_Get(5) != 0) && (FldGate_Get(0x48) == 0)) {
        return 0x662;
    }
    if (gSaveData.unk_2458 == 15) {
        if ((FldGate_Get(5) != 0) && (FldGate_Get(0x48) == 0)) {
            return 0x662;
        } else if ((FldGate_Get(0x48) != 0) && (FldGate_Get(0x5C) == 0)) {
            return 0x663;
        } else if ((FldGate_Get(0x5C) != 0) && (FldGate_Get(6) == 0)) {
            return 0x673;
        }
    }

    if ((gSaveData.unk_2458 == 0xE) && (FldGate_Get(6) != 0) && (FldGate_Get(0x22) == 0)) {
        return 0x662;
    }
    if (gSaveData.unk_2458 == 0x13) {
        if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x32) == 0)) {
            return 0x66A;
        } else if ((FldGate_Get(0x32) != 0) && (FldGate_Get(0x35) == 0)) {
            return 0x66C;
        } else if ((FldGate_Get(0x35) != 0) && (FldGate_Get(12) == 0)) {
            return 0x66D;
        } else if ((FldGate_Get(0x42) != 0) && (FldGate_Get(0x3E) == 0)) {
            return 0x671;
        } else if ((FldGate_Get(0x43) != 0) && (FldGate_Get(0x39) == 0)) {
            return 0x671;
        } else if ((FldGate_Get(0x44) != 0) && (FldGate_Get(0x3B) == 0)) {
            return 0x671;
        }
    }

    if (gSaveData.unk_2458 == 0x15) {
        if ((FldGate_Get(0x3C) != 0) && (FldGate_Get(15) == 0)) {
            return 0x66F;
        } else if ((FldGate_Get(15) != 0) && (FldGate_Get(0x10) == 0)) {
            return 0x670;
        }
    }
    if ((FldGate_Get(0x20) == 0) && (FldGate_Get(33) == 0)) {
        return 0x65F;
    } else if ((FldGate_Get(0x22) != 0) && (FldGate_Get(0x23) == 0)) {
        return 0x664;
    } else if ((FldGate_Get(0x23) != 0) && (FldGate_Get(9) == 0)) {
        return 0x665;
    } else if ((FldGate_Get(9) != 0) && (FldGate_Get(10) == 0)) {
        return 0x666;
    } else if ((FldGate_Get(10) != 0) && (FldGate_Get(47) == 0)) {
        return 0x667;
    } else if ((FldGate_Get(47) != 0) && (FldGate_Get(0x30) == 0)) {
        return 0x669;
    } else if ((FldGate_Get(0x30) != 0) && (FldGate_Get(12) == 0)) {
        return 0x66B;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(0x3E) == 0)) {
        return 0x66E;
    }
    return 0;
}

s32 func_ov030_020a5040(void) {
    if (gSaveData.unk_2458 == 4) {
        if ((FldGate_Get(56) != 0) && (FldGate_Get(61) == 0)) {
            return 0x68E;
        } else if ((FldGate_Get(61) != 0) && (FldGate_Get(2) == 0)) {
            return 0x691;
        }
    }

    if (gSaveData.unk_2458 == 2) {
        if ((FldGate_Get(0x39) != 0) && (FldGate_Get(58) == 0)) {
            return 0x679;
        } else if ((FldGate_Get(58) != 0) && (FldGate_Get(3) == 0)) {
            return 0x692;
        }
    }

    if (gSaveData.unk_2458 == 6) {
        if ((FldGate_Get(39) != 0) && (FldGate_Get(9) == 0)) {
            return 0x67B;
        } else if ((FldGate_Get(10) != 0) && (FldGate_Get(42) == 0)) {
            return 0x67D;
        } else if ((FldGate_Get(12) != 0) && (FldGate_Get(13) != 0) && (FldGate_Get(15) == 0)) {
            return 0x687;
        } else if (FldGate_Get(15) != 0) {
            return 1672;
        }
    }

    if ((gSaveData.unk_2458 == 8) && (FldGate_Get(33) != 0) && (FldGate_Get(0xE) == 0)) {
        return 0x684;
    } else if ((gSaveData.unk_2458 == 7) && (FldGate_Get(0x23) != 0)) {
        return 0x689;
    } else if (gSaveData.unk_2458 == 9) {
        if ((FldGate_Get(37) != 0) && (FldGate_Get(6) == 0)) {
            return 0x68A;
        } else if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
            return 0x68C;
        } else if (FldGate_Get(7) != 0) {
            return 0x68B;
        }
    }

    if (FldGate_Get(39) == 0) {
        return 0x678;
    } else if ((FldGate_Get(39) != 0) && (FldGate_Get(9) == 0)) {
        return 0x67A;
    } else if ((FldGate_Get(9) != 0) && (FldGate_Get(10) == 0)) {
        return 0x67C;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(13) != 0) && (FldGate_Get(15) == 0)) {
        return 0x686;
    } else if ((FldGate_Get(46) != 0) && (FldGate_Get(0xE) == 0)) {
        return 0x68D;
    } else if ((FldGate_Get(46) != 0) && (FldGate_Get(0xE) != 0) && (FldGate_Get(12) == 0)) {
        return 0x685;
    } else if ((FldGate_Get(42) != 0) && (FldGate_Get(45) == 0) && (FldGate_Get(47) == 0)) {
        return 0x67E;
    } else if ((FldGate_Get(42) != 0) && (FldGate_Get(45) != 0) && (FldGate_Get(47) == 0) && (FldGate_Get(46) == 0)) {
        return 0x67F;
    } else if ((FldGate_Get(42) != 0) && (FldGate_Get(45) == 0) && (FldGate_Get(47) != 0) && (FldGate_Get(13) == 0)) {
        return 0x680;
    }
    if ((FldGate_Get(42) != 0) && (FldGate_Get(45) != 0) && (FldGate_Get(47) != 0) && (FldGate_Get(13) == 0) &&
        (FldGate_Get(46) != 0) && (FldGate_Get(12) == 0))
    {
        return 0x683;
    } else if ((FldGate_Get(42) != 0) && (FldGate_Get(45) != 0) && (FldGate_Get(47) != 0) && (FldGate_Get(13) == 0) &&
               (FldGate_Get(46) == 0))
    {
        return 0x681;
    } else if ((FldGate_Get(42) != 0) && (FldGate_Get(45) != 0) && (FldGate_Get(47) != 0) && (FldGate_Get(13) != 0) &&
               (FldGate_Get(46) == 0))
    {
        return 1666;
    } else if ((FldGate_Get(13) != 0) && (FldGate_Get(45) == 0)) {
        return 1683;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(47) == 0)) {
        return 1679;
    } else if ((FldGate_Get(12) != 0) && (FldGate_Get(47) != 0) && (FldGate_Get(13) == 0)) {
        return 1680;
    } else if (FldGate_Get(15) != 0) {
        return 1672;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5608(void) {
    if (gSaveData.unk_2458 == 1) {
        if ((FldGate_Get(0x2C) != 0) && (FldGate_Get(0x46) == 0)) {
            return 0x698;
        }
        if ((FldGate_Get(0x46) != 0) && (FldGate_Get(3) == 0)) {
            return 0x6A6;
        }
    }

    if ((gSaveData.unk_2458 == 5) && (FldGate_Get(39) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x6A3;
    }
    if ((gSaveData.unk_2458 == 0x11) && (FldGate_Get(39) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x6A3;
    }
    if (gSaveData.unk_2458 == 18) {
        if ((FldGate_Get(39) != 0) && (FldGate_Get(0x28) == 0)) {
            return 0x6A4;
        }
        if ((FldGate_Get(0x28) != 0) && (FldGate_Get(42) == 0)) {
            return 0x6A5;
        }
    }

    if ((gSaveData.unk_2458 == 15) && (FldGate_Get(0x31) != 0) && (FldGate_Get(7) == 0)) {
        return 0x69C;
    }
    if ((gSaveData.unk_2458 == 0xE) && (FldGate_Get(0x32) != 0) && (FldGate_Get(8) == 0)) {
        return 0x69D;
    }
    if ((FldGate_Get(45) != 0) && (FldGate_Get(4) == 0)) {
        return 0x699;
    }
    if ((FldGate_Get(47) != 0) && (FldGate_Get(5) == 0)) {
        return 0x69A;
    }
    if ((FldGate_Get(0x30) != 0) && (FldGate_Get(0x4A) == 0)) {
        return 0x6A9;
    }
    if ((FldGate_Get(0x4A) != 0) && (FldGate_Get(6) == 0)) {
        return 0x6A9;
    }
    if ((FldGate_Get(0x22) == 0) && (FldGate_Get(2) == 0)) {
        return 0x696;
    }
    if ((FldGate_Get(2) != 0) && (FldGate_Get(8) == 0)) {
        return 0x697;
    }
    if ((FldGate_Get(8) != 0) && (FldGate_Get(0x22) == 0)) {
        return 0x69E;
    }
    if ((FldGate_Get(0x22) != 0) && (FldGate_Get(0x24) == 0)) {
        return 0x69F;
    }
    if ((FldGate_Get(0x24) != 0) && (FldGate_Get(38) == 0)) {
        return 0x6A0;
    }
    if ((FldGate_Get(38) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6A1;
    }
    if ((FldGate_Get(39) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x6A2;
    }
    return 0;
}

s32 func_ov030_020a5964(void) {
    if ((gSaveData.unk_2458 == 1) && (FldGate_Get(33) == 0)) {
        return 0x6AA;
    } else if ((gSaveData.unk_2458 == 2) && (FldGate_Get(0x23) != 0) && (FldGate_Get(2) == 0) && (FldGate_Get(38) == 0)) {
        return 0x6AC;
    } else if (FldGate_Get(33) == 0) {
        return 0x6AA;
    } else if ((FldGate_Get(33) != 0) && (FldGate_Get(0x23) == 0)) {
        return 0x6AB;
    } else if ((FldGate_Get(0x23) != 0) && (FldGate_Get(2) == 0) && (FldGate_Get(38) == 0)) {
        return 0x6AD;
    } else if ((FldGate_Get(0x23) != 0) && (FldGate_Get(2) == 0) && (FldGate_Get(38) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6AE;
    } else if ((FldGate_Get(0x23) != 0) && (FldGate_Get(2) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6AE;
    } else if ((FldGate_Get(39) != 0) && (FldGate_Get(41) == 0)) {
        return 0x6AF;
    } else if ((FldGate_Get(41) != 0) && (FldGate_Get(42) == 0)) {
        return 0x6B0;
    } else
        return 0;
}

s32 func_ov030_020a5b20(void) {
    if ((gSaveData.unk_2458 == 0x18) && (FldGate_Get(2) != 0)) {
        return 0x6C1;
    } else if ((gSaveData.unk_2458 == 0x19) && (FldGate_Get(0x30) != 0)) {
        return 0x6C2;
    } else if (FldGate_Get(33) == 0) {
        return 0x6BE;
    } else if ((FldGate_Get(33) != 0) && (FldGate_Get(33) != 0)) {
        return 0x6BF;
    } else if (FldGate_Get(0x30) != 0) {
        return 0x6C3;
    } else
        return 0;
}

s32 func_ov030_020a5bd4(void) {
    if ((gSaveData.unk_2458 == 0x11) && (FldGate_Get(0x23) != 0) && (FldGate_Get(38) == 0)) {
        return 0x6D2;
    } else if ((gSaveData.unk_2458 == 18) && (FldGate_Get(0x23) != 0) && (FldGate_Get(38) == 0)) {
        return 0x6D3;
    } else if (FldGate_Get(38) != 0) {
        return 0x6D4;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5c68(void) {
    if ((gSaveData.unk_2458 == 3) && (FldGate_Get(0x22) != 0) && (FldGate_Get(37) == 0)) {
        return 0x6D8;
    } else if ((gSaveData.unk_2458 == 0x13) && (FldGate_Get(0x22) != 0) && (FldGate_Get(37) == 0)) {
        return 0x6D8;
    } else if ((gSaveData.unk_2458 == 23) && (FldGate_Get(0x28) != 0) && (FldGate_Get(3) == 0)) {
        return 0x6DC;
    } else if (FldGate_Get(37) == 0) {
        return 0x6D7;
    } else if ((FldGate_Get(37) != 0) && (FldGate_Get(38) == 0)) {
        return 0x6D9;
    } else if ((FldGate_Get(38) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6DA;
    } else if ((FldGate_Get(39) != 0) && (FldGate_Get(3) == 0)) {
        return 0x6DB;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5dac(void) {
    if (gSaveData.unk_2458 == 1) {
        if (FldGate_Get(0x20) == 0) {
            return 0x6EB;
        } else if ((FldGate_Get(3) != 0) && (FldGate_Get(0x24) == 0)) {
            return 0x6EE;
        }
    }

    if ((gSaveData.unk_2458 == 4) && (FldGate_Get(0x24) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6EF;
    } else if ((gSaveData.unk_2458 == 2) && (FldGate_Get(0x24) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6EF;
    } else if (gSaveData.unk_2458 == 6) {
        if ((FldGate_Get(39) != 0) && (FldGate_Get(0x28) == 0)) {
            return 0x6F2;
        } else if (FldGate_Get(0x28) != 0) {
            return 0x6F3;
        }
    }

    if ((FldGate_Get(0x20) != 0) && (FldGate_Get(3) == 0)) {
        return 0x6EC;
    } else if ((FldGate_Get(3) != 0) && (FldGate_Get(0x24) == 0)) {
        return 0x6ED;
    } else if ((FldGate_Get(0x24) != 0) && (FldGate_Get(39) == 0)) {
        return 0x6F0;
    } else if ((FldGate_Get(39) != 0) && (FldGate_Get(0x28) == 0)) {
        return 0x6F1;
    }
    return 0;
}

s32 func_ov030_020a5f64(void) {
    if (gSaveData.unk_2458 == 6) {
        if ((FldGate_Get(0x20) != 0) && (FldGate_Get(33) == 0)) {
            return 0x714;
        } else if ((FldGate_Get(33) != 0) && (FldGate_Get(0x22) == 0)) {
            return 0x715;
        }
    }
    if (gSaveData.unk_2458 == 7) {
        if ((FldGate_Get(0x23) != 0) && (FldGate_Get(37) == 0)) {
            return 0x717;
        } else if ((FldGate_Get(38) != 0) && (FldGate_Get(4) == 0)) {
            return 0x719;
        }
    }
    if (gSaveData.unk_2458 == 9) {
        if ((FldGate_Get(38) != 0) && (FldGate_Get(4) == 0)) {
            return 0x71A;
        } else if ((FldGate_Get(4) != 0) && (FldGate_Get(41) == 0)) {
            return 0x71B;
        }
    }
    if (FldGate_Get(0x22) == 0) {
        return 0x713;
    } else if ((FldGate_Get(0x22) != 0) && (FldGate_Get(37) == 0)) {
        return 0x716;
    } else if ((FldGate_Get(37) != 0) && (FldGate_Get(4) == 0)) {
        return 0x718;
    } else {
        return 0;
    }
}

s32 func_ov030_020a60fc(void) {
    if ((gSaveData.unk_2458 == 0x10) && (FldGate_Get(33) != 0) && (FldGate_Get(3) == 0)) {
        return 0x728;
    } else if ((gSaveData.unk_2458 == 0xE) && (FldGate_Get(33) != 0) && (FldGate_Get(3) == 0)) {
        return 0x728;
    } else if ((gSaveData.unk_2458 == 12) && (FldGate_Get(3) != 0) && (FldGate_Get(37) == 0)) {
        return 0x72A;
    } else if (FldGate_Get(3) == 0) {
        return 0x727;
    } else if ((FldGate_Get(3) != 0) && (FldGate_Get(37) == 0)) {
        return 0x729;
    } else {
        return 0;
    }
}

s32 func_ov030_020a61f0(void) {
    if (gSaveData.unk_2458 == 41) {
        if ((FldGate_Get(41) != 0) && (FldGate_Get(6) == 0)) {
            return 1857;
        } else if ((FldGate_Get(42) != 0) && (FldGate_Get(43) == 0)) {
            return 1858;
        }
    }
    if (FldGate_Get(33) == 0) {
        return 1851;
    } else if ((FldGate_Get(33) != 0) && (FldGate_Get(3) == 0)) {
        return 1852;
    } else if ((FldGate_Get(3) != 0) && (FldGate_Get(37) == 0)) {
        return 1853;
    } else if ((FldGate_Get(37) != 0) && (FldGate_Get(38) == 0)) {
        return 1854;
    } else if ((FldGate_Get(38) != 0) && (FldGate_Get(5) == 0)) {
        return 1855;
    } else if ((FldGate_Get(5) != 0) && (FldGate_Get(6) == 0)) {
        return 1856;
    } else
        return 0;
}

s32 func_ov030_020a633c(void) {
    if ((gSaveData.unk_2458 == 43) && (FldGate_Get(127) != 0)) {
        return 1871;
    } else {
        return 0;
    }
}

s32 func_ov030_020a6374(void) {
    if (gSaveData.unk_2458 == 23) {
        if (FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
            return 1891;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(15) == 0)) {
            return 1898;
        } else if ((FldGate_Get(15) != 0) && (FldGate_Get(6) == 0)) {
            return 1899;
        } else if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
            return 1901;
        } else if ((FldGate_Get(7) != 0) && (FldGate_Get(8) == 0)) {
            return 1903;
        }
    }

    if (gSaveData.unk_2458 == 0x15) {
        if (FldGate_Get(1) == 0) {
            return 1892;
        } else if ((FldGate_Get(1) != 0) && (FldGate_Get(2) == 0)) {
            return 1893;
        } else if ((FldGate_Get(2) != 0) && (FldGate_Get(3) == 0)) {
            return 1894;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(15) == 0)) {
            return 1897;
        } else if ((FldGate_Get(15) != 0) && (FldGate_Get(6) == 0)) {
            return 1899;
        } else if ((FldGate_Get(7) != 0) && (FldGate_Get(8) == 0)) {
            return 1902;
        }
    }

    if (gSaveData.unk_2458 == 0x16) {
        if ((FldGate_Get(4) != 0) && (FldGate_Get(12) == 0)) {
            return 1895;
        } else if ((FldGate_Get(12) != 0) && (FldGate_Get(5) == 0) && (FldGate_Get(6) == 0)) {
            return 1896;
        } else if ((FldGate_Get(5) != 0) && (FldGate_Get(15) == 0)) {
            return 1897;
        } else if ((FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
            return 1900;
        }
    }

    return 0;
}

s32 func_ov030_020a65f4(void) {
    if ((gSaveData.unk_2458 == 1) && FldGate_Get(GATE_GAME_CLEARED) == FALSE) {
        return 1911;
    } else if ((gSaveData.unk_2458 == 11) && (FldGate_Get(1) != 0) && (FldGate_Get(3) == 0)) {
        return 1912;
    } else if ((gSaveData.unk_2458 == 15) && (FldGate_Get(6) != 0) && (FldGate_Get(7) == 0)) {
        return 1915;
    } else {
        return 0;
    }
}

const FieldEventRewardFunc data_ov030_020da618[44] = {
    func_ov030_02084eb4, func_ov030_02085cf8, func_ov030_020876c8, func_ov030_02089300, func_ov030_0208aaf4,
    func_ov030_0208c8a0, func_ov030_0208ddfc, func_ov030_0208ec3c, func_ov030_0208faa4, func_ov030_02091bc8,
    func_ov030_02093db8, func_ov030_0209585c, func_ov030_0209716c, func_ov030_02097f4c, func_ov030_02098a30,
    func_ov030_02099514, func_ov030_0209a564, func_ov030_0209bacc, func_ov030_0209c680, func_ov030_0209cff0,
    func_ov030_0209dafc, func_ov030_0209f0e8, func_ov030_020a206c, func_ov030_020a21a4, func_ov030_020a2628,
    func_ov030_020a2c20, func_ov030_020a2de8, func_ov030_020a306c, func_ov030_020a30d4, func_ov030_020a317c,
    func_ov030_020a31bc, func_ov030_020a333c, func_ov030_020a3718, func_ov030_020a3750, func_ov030_020a3788,
    func_ov030_020a37c0, func_ov030_020a37f8, func_ov030_020a3830, func_ov030_020a3858, func_ov030_020a3890,
    func_ov030_020a38c8, func_ov030_020a3900, func_ov030_020a3938, func_ov030_02084978,
};
