#include "Engine/Overlay/OverlayDispatcher.h"
#include "Player/Progress.h"
#include "Player/Stats.h"
#include "Save.h"
#include <nitro/types.h>

extern s32  func_ov030_020aed9c(s32);
extern void func_ov030_020ae96c(s32);
extern void func_ov030_020c26bc(s32);
extern s32  func_ov030_020848a4(s32, s32, s32, s32);
extern s32  func_ov030_020848e4(ProgressObject*, s32);
extern s32  func_ov030_02084978(ProgressObject*);
extern s32  func_ov030_020a75c0(ProgressObject*);
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

s32 func_ov030_02084a2c(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (Progress_Check(0x20) == 0) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x437, 0);
            return 1;
        } else if ((Progress_Check(33) == 0) && (func_ov030_020aed9c(-1) != 0)) {
            Progress_SetGate(33);
            gSaveState.unk_20.unk_264C = 4;
            Progress_BeginScriptedEvent(arg0, 0x57, 0);
            return 1;
        } else if ((Progress_Check(0x33) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x59, 0);
            return 1;
        }

        if ((Progress_Check(0x23) != 0) && (Progress_Check(0x34) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((Progress_Check(37) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x5F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) != 0) && (Progress_Check(38) != 0) && (Progress_Check(0x2C) == 0)) {
            Progress_SetGate(0x2C);
            if (Progress_Check(39) == 0) {
                Progress_SetGate(39);
                Progress_BeginScriptedEvent(arg0, 0x60, 0);
                return 1;
            }
        }
        if ((func_ov030_020848e4(arg0, 3) != 0) && (Progress_Check(38) != 0) && (Progress_Check(45) == 0)) {
            Progress_SetGate(45);
            if (Progress_Check(39) == 0) {
                Progress_SetGate(39);
                Progress_BeginScriptedEvent(arg0, 0x60, 0);
                return 1;
            }
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (Progress_Check(38) != 0) && (Progress_Check(3) == 0)) {
            Progress_SetGate(3);
            Progress_BeginScriptedEvent(arg0, 0x61, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 3) {
        if (Progress_Check(0x28) == 0) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x62, 0);
            return 1;
        } else if ((Progress_Check(0x30) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x63, 0);
            return 1;
        } else if ((Progress_Check(0x31) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x64, 0);
            return 1;
        } else if ((Progress_Check(0x32) != 0) && (Progress_Check(43) == 0)) {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0x65, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 5) {
        if (Progress_Check(0x24) == 0) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x5B, 0);
            return 1;
        } else if ((Progress_Check(47) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x5E, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 37) && (Progress_Check(0x23) != 0) && (Progress_Check(0x34) == 0)) {
        Progress_SetGate(0x34);
        Progress_BeginScriptedEvent(arg0, 0x805A, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_02084eb4(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x57:
            Progress_SetGate(0x22);
            gSaveState.unk_20.unk_24BE = 0x67;
            func_ov030_020aec1c(10);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8058;
            gSaveState.unk_20.unk_3124          = 3;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x58:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84BA;
            func_ov030_020af364(2);
            return 1;
        case 0x59:
            gSaveState.unk_20.unk_3124 = 4;
            gSaveState.unk_20.unk_3128 = 0;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x5A:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 5;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x139000;
            arg0->unk_21B0C = 0xE1000;
            break;
        case 0x5B:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x805C;
            gSaveState.unk_20.unk_3124          = 5;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x5C:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x805D;
            gSaveState.unk_20.unk_3124          = 6;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x5D:
            Progress_SetGate(2);
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            gSaveState.unk_20.unk_24B4                 = 0;
            gSaveState.unk_20.unk_24B8                 = 1;
            gSaveState.unk_20.currentStoryEvent        = 0x84B6;
            func_ov030_020af364(3);
            return 1;
        case 0x5E:
            func_ov030_020aeaa0(3, 4);
            gSaveState.unk_20.unk_2648 = 0;
            gSaveState.unk_20.unk_264A = 0;
            gSaveState.unk_20.unk_264E = 0;
            arg0->unk_21CF8            = 1;
            break;
        case 0x5F:
            arg0->unk_21CF8            = 1;
            gSaveState.unk_20.unk_2648 = 4;
            break;
        case 0x60:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x4B7;
            func_ov030_020af364(4);
            return 1;
        case 0x61:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x4B8;
            func_ov030_020af364(5);
            return 1;
        case 0x62:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x4B9;
            func_ov030_020af364(6);
            return 1;
        case 0x63:
            gSaveState.unk_20.unk_3124 = 7;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4BA:
            Progress_SetGate(0x33);
            if (Inventory_HasRequiredQuantity(ITEM_WALLET_MY_FIRST_WALLET, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_WALLET_MY_FIRST_WALLET, 0);
            }
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK31, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_CD_TRACK31, 0);
            }
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_ESCAPING_FROM_BATTLE);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DASH);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4B6:
            Progress_SetGate(47);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_DRAG);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_BOOT_REBOOT);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_BOTTOM_SCREEN_COMBAT);
                func_ov030_020aac28(arg0, PIN_PYROKINESIS);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        default:
            Progress_SetGate(0x30);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_TOP_SCREEN_COMBAT);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4B8:
            Progress_SetGate(0x31);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_FOLLOW_THAT_PUCK);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_DUAL_SCREEN_COMBAT);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4B9:
            Progress_SetGate(0x32);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_SO_I_BEAT_THE_ENEMY);
            break;
        case 0x64:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_NOSE_GLASSES, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_NOSE_GLASSES);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02085480(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != FALSE) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x6A;
    gSaveState.unk_20.unk_3124          = 56;
    gSaveState.unk_20.unk_244C          = 0x159000;
    gSaveState.unk_20.unk_2450          = 0xD2000;
}

s32 func_ov030_02085508(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 18) {
        if ((Progress_Check(0x20) == 0) && (func_ov030_020848a4(0, 0x96, 0x1E, 250) != 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x6B, 0);
            return 1;
        } else if ((Progress_Check(0x20) != 0) && (Progress_Check(33) == 0) && (func_ov030_020aed9c(-2) != 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x6C, 0);
            return 1;
        } else if ((Progress_Check(0x35) != 0) && (Progress_Check(56) == 0)) {
            Progress_SetGate(56);
            Progress_BeginScriptedEvent(arg0, 0x4C0, 0);
            return 1;
        } else if ((Progress_Check(0x36) != 0) && (Progress_Check(0x39) == 0)) {
            Progress_SetGate(0x39);
            Progress_BeginScriptedEvent(arg0, 0x4C1, 0);
            return 1;
        } else if ((Progress_Check(0x37) != 0) && (Progress_Check(58) == 0)) {
            Progress_SetGate(58);
            Progress_BeginScriptedEvent(arg0, 0x4C2, 0);
            return 1;
        } else if ((Progress_Check(0x35) != 0) && (Progress_Check(0x36) != 0) && (Progress_Check(0x37) != 0) &&
                   (Progress_Check(33) != 0) && (Progress_Check(3) == 0))
        {
            Progress_SetGate(3);
            gSaveState.unk_20.unk_266E = 4;
            Progress_BeginScriptedEvent(arg0, 0x6D, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x11) {
        if (Progress_Check(0x22) == 0) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x6E, 0);
            return 1;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x438, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x76, 0);
            return 1;
        } else if ((Progress_Check(42) != 0) && (Progress_Check(43) == 0) && (Progress_Check(5) == 0) &&
                   (func_ov030_020848a4(0, 0xC8, 0x190, 0x159) != 0) && (func_ov030_020aed9c(-2) != 0))
        {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0x3CD, 0);
            return 1;
        } else if (((Progress_Check(0x32) != 0) || (Progress_Check(0x33) != 0)) &&
                   ((Progress_Check(0x32) == 0) || (Progress_Check(0x33) == 0)) && (Progress_Check(43) == 0))
        {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0x3CD, 0);
            return 1;
        } else if ((Progress_Check(0x32) != 0) && (Progress_Check(0x33) != 0) && (Progress_Check(5) == 0)) {
            Progress_SetGate(5);
            Progress_BeginScriptedEvent(arg0, 0x77, 0);
            return 1;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(61) == 0)) {
            Progress_SetGate(61);
            Progress_BeginScriptedEvent(arg0, 0x442, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (Progress_Check(5) != 0) && (Progress_Check(0x2C) == 0)) {
            Progress_SetGate(0x2C);
            Progress_BeginScriptedEvent(arg0, 0x78, 0);
            return 1;
        } else if ((Progress_Check(46) == 0) && (func_ov030_020aed9c(0x3707) != 0)) {
            Progress_SetGate(46);
            Progress_BeginScriptedEvent(arg0, 0x3CF, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 5) {
        if (Progress_Check(45) == 0) {
            Progress_SetGate(45);
            Progress_BeginScriptedEvent(arg0, 0x79, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x9B, 0x64, 0x190, 0x12C) != 0) && (Progress_Check(6) == 0) &&
                   (func_ov030_020aed9c(-2) != 0))
        {
            Progress_SetGate(6);
            Progress_BeginScriptedEvent(arg0, 0x7A, 0);
            return 1;
        } else if ((Progress_Check(0x3B) != 0) && (Progress_Check(0x30) == 0)) {
            Progress_SetGate(0x30);
            Progress_BeginScriptedEvent(arg0, 0x84, 0);
            return 1;
        } else if ((Progress_Check(0x3C) != 0) && (Progress_Check(0x31) == 0)) {
            Progress_SetGate(0x31);
            Progress_BeginScriptedEvent(arg0, 0x85, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02085b14(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35EF:
            if (Progress_Check(0x23) == 0) {
                Progress_SetGate(0x23);
                Progress_SetCurrentEvent(0x70);
                Progress_AdvanceEventScript(arg0);
                return 1;
            } else {
                Progress_SetCurrentEvent(0x74);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
        case 0x35F0:
            Progress_SetGate(0x24);
            Progress_SetCurrentEvent(0x71);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35F1:
            Progress_SetGate(37);
            Progress_SetCurrentEvent(0x72);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35F3:
            Progress_SetGate(38);
            Progress_SetCurrentEvent(0x73);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35F2:
            Progress_SetCurrentEvent(0x75);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02085c24(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35F5:
            Progress_SetCurrentEvent(0x7C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35F6:
            Progress_SetCurrentEvent(0x7D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35F7:
            if (Progress_Check(47) == 0) {
                Progress_SetCurrentEvent(0x7E);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x80);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35F8:
            Progress_SetCurrentEvent(0x82);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02085cf8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x6A:
            gSaveState.unk_20.unk_24BE = 0x87;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(5, 4);
            break;
        case 0x3CB:
            Progress_SetGate(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6B:
            Progress_SetGate(2);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_SCANNING);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6C:
            gSaveState.unk_20.unk_310C = &data_ov030_020d9acc;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_ENEMY_ENCOUNTERS);
            break;
        case 0x6D:
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_MAIL_ICON);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x6F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
            if ((Progress_Check(0x23) != 0) && (Progress_Check(0x24) != 0) && (Progress_Check(37) != 0) &&
                (Progress_Check(38) != 0))
            {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 5, 0x35EF, 0x35F0, 0x35F3, 0x35F1, 0x35F2);
            }
            if (Progress_Check(0x24) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 4, 0x35EF, 0x35F0, 0x35F3, 0x35F1);
            }
            if (Progress_Check(0x24) == 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 3, 0x35EF, 0x35F0, 0x35F3);
            }
            break;
        case 0x75:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_WEARING_PINS);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_PHONE_MENU);
            func_ov030_020aac28(arg0, PIN_CURE_DRINK);
            Progress_SetGate(4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x76:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x439);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x439:
            func_ov030_020c596c();
            gSaveState.unk_20.unk_310C = &data_ov030_020d9af4;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x77:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_EXTRA_SLOT);
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            gSaveState.unk_20.unk_2656 = 4;
            arg0->unk_21CF8            = 1;
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
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0x86);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7A:
            arg0->unk_21CF8 = 1;
            break;
        case 0x7B:
            Progress_SetGate(7);
        case 0x7D:
        case 0x7F:
            if (Progress_Check(47) == 0) {
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
            Progress_SetCurrentEvent(0x81);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x81:
            Progress_SetGate(47);
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
            Progress_SetGate(8);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x4C3;
            func_ov030_020af364(7);
            return 1;
        case 0x84:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84C4U;
            func_ov030_020af364(8);
            return 1;
        case 0x85:
            gSaveState.unk_20.unk_3124 = 8;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4C0:
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_TOUCH);
                func_ov030_020aac28(arg0, PIN_FORCE_ROUNDS);
                func_ov030_020aac28(arg0, PIN_THUNDERBOLT);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C1:
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_USE_OBSTACLES);
                func_ov030_020aac28(arg0, PIN_PSYCHOKINESIS);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C2:
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_SLASH);
                func_ov030_020aac28(arg0, PIN_SHOCKWAVE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C3:
            Progress_SetGate(0x3B);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C4:
            Progress_SetGate(0x3C);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_ICE_BLOW);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0x86:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SAILOR_SUIT, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_SAILOR_SUIT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02086700(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 2;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(23);
    gSaveState.unk_20.unk_244C          = 0x12C000;
    gSaveState.unk_20.unk_2450          = 0x87000;
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x808A;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x39;
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_02086804(s32 arg0) {
    s16 var_r0;

    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(0x24) == 0)) {
            if (Progress_Check(0x44) == 0) {
                Progress_SetGate(0x44);
                Progress_BeginScriptedEvent(arg0, 0x4091, 0);
                return 1;
            }
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x40AE, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 0x47) != 0) {
            if (Progress_Check(0x3B) == 0) {
                if (Progress_Check(0x44) == 0) {
                    Progress_SetGate(0x44);
                    Progress_BeginScriptedEvent(arg0, 0x4091, 0);
                    return 1;
                }
                Progress_SetGate(0x3B);
                Progress_BeginScriptedEvent(arg0, 0x43D9, 0);
                return 1;
            }
        } else {
            Progress_ClearGate(0x3B);
        }
        if ((Progress_Check(0x44) != 0) && (Progress_Check(0x48) == 0)) {
            Progress_SetGate(0x48);
            Progress_BeginScriptedEvent(arg0, 0x4CD, 0);
            return 1;
        } else if ((Progress_Check(0x24) != 0) && (Progress_Check(0x49) == 0)) {
            Progress_SetGate(0x49);
            Progress_BeginScriptedEvent(arg0, 0x43B, 0);
            return 1;
        } else if ((Progress_Check(0x24) != 0) && (Progress_Check(7) == 0)) {
            Progress_SetGate(7);
            Progress_BeginScriptedEvent(arg0, 0xAF, 0);
            return 1;
        } else if ((Progress_Check(2) != 0) && (Progress_Check(0x46) == 0)) {
            Progress_SetGate(0x46);
            Progress_BeginScriptedEvent(arg0, 0x4CB, 0);
            return 1;
        } else if ((Progress_Check(0x46) != 0) && (Progress_Check(9) == 0)) {
            Progress_SetGate(9);
            Progress_BeginScriptedEvent(arg0, 0xB1, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(18) != 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x43DC, 0);
            return 1;
        } else if ((Progress_Check(0x28) == 0) && (func_ov030_020aed9c(0x371B) != 0)) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x3DD, 0);
            return 1;
        }
        if ((Progress_Check(0x3C) == 0) &&
            ((Progress_Check(42) != 0) || (Progress_Check(43) != 0) || (Progress_Check(0x2C) != 0)))
        {
            Progress_SetGate(0x3C);
            Progress_BeginScriptedEvent(arg0, 0xB7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(43) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x43DF, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if (Progress_Check(0x20) == 0) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x90, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x49) != 0) && (Progress_Check(0x44) == 0)) {
            Progress_SetGate(0x44);
            Progress_BeginScriptedEvent(arg0, 0x4091, 0);
            return 1;
        } else if ((Progress_Check(0x44) != 0) && (Progress_Check(0x48) == 0)) {
            Progress_SetGate(0x48);
            Progress_BeginScriptedEvent(arg0, 0x4CD, 0);
            return 1;
        } else if ((Progress_Check(0x45) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0xA8, 0);
            return 1;
        }
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (Progress_Check(0x42) != 0) && (Progress_Check(0x43) == 0)) {
            Progress_SetGate(0x43);
        }
        if ((Progress_Check(4) != 0) && (Progress_Check(0x4C) == 0)) {
            Progress_SetGate(0x4C);
            Progress_BeginScriptedEvent(arg0, 0x443, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x16) {
        if (Progress_Check(0x32) == 0) {
            Progress_SetGate(0x32);
            Progress_BeginScriptedEvent(arg0, 0x9A, 0);
            return 1;
        } else if ((Progress_Check(10) != 0) && (Progress_Check(6) != 0) && (Progress_Check(11) == 0)) {
            Progress_SetGate(11);
            Progress_BeginScriptedEvent(arg0, 0xB3, 0);
            return 1;
        } else if ((Progress_Check(37) != 0) && (Progress_Check(0xE) == 0)) {
            Progress_SetGate(0xE);
            Progress_BeginScriptedEvent(arg0, 0xB4, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 23) {
        if ((func_ov030_02084860(0x12C, 0x87, 0x96) == 0) && (Progress_Check(45) == 0)) {
            Progress_SetGate(45);
            Progress_BeginScriptedEvent(arg0, 0x8C, 0);
            return 1;
        } else if ((func_ov030_020aed9c(0x3715) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x3DB, 0);
            return 1;
        } else if ((Progress_Check(0x47) != 0) && (Progress_Check(61) == 0)) {
            Progress_SetGate(61);
            Progress_BeginScriptedEvent(arg0, 0xBA, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02086f10(s32 arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x35F9:
            Progress_SetCurrentEvent(0x3E4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FA:
            Progress_SetCurrentEvent(0x3E5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FB:
            Progress_SetCurrentEvent(0x3E6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FC:
            Progress_SetCurrentEvent(0x3E7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            Progress_SetCurrentEvent(0x3E3);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
}

s32 func_ov030_02086fdc(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x35F9:
            if (Progress_Check(38) != 0) {
                Progress_SetCurrentEvent(0x3EC);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3EB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FA:
            Progress_SetGate(42);
            Progress_SetCurrentEvent(0x3ED);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FB:
            Progress_SetGate(43);
            Progress_SetCurrentEvent(0x3EE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FC:
            Progress_SetGate(0x2C);
            Progress_SetCurrentEvent(0x3EF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            Progress_SetCurrentEvent(0x3EA);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
}

s32 func_ov030_020870ec(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x35F9:
            Progress_SetCurrentEvent(0x3F3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FA:
            Progress_SetGate(42);
            Progress_SetCurrentEvent(0x3F4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FB:
            if (Progress_Check(43) == 0) {
                Progress_SetGate(43);
                Progress_SetGate(13);
                Progress_SetCurrentEvent(0x3EE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3F5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FC:
            if (Progress_Check(0x2C) == 0) {
                Progress_SetGate(0x2C);
                Progress_SetGate(13);
                Progress_SetCurrentEvent(0x3EF);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3F6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            Progress_SetCurrentEvent(0x3F2);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
}

s32 func_ov030_02087238(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35FE:
            Progress_SetGate(47);
            Progress_SetCurrentEvent(0x93);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FF:
            Progress_SetGate(0x30);
            Progress_SetCurrentEvent(0x94);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3600:
            Progress_SetGate(0x31);
            Progress_SetCurrentEvent(0x95);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020872e8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3602:
            Progress_SetCurrentEvent(0x97);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3603:
            Progress_SetCurrentEvent(0x98);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087360(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            Progress_SetGate(0x33);
            Progress_SetCurrentEvent(0x9C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3606:
            Progress_SetGate(0x34);
            Progress_SetCurrentEvent(0x9D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3607:
            Progress_SetGate(0x35);
            Progress_SetCurrentEvent(0x9E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087414(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3609:
            Progress_SetGate(0x36);
            Progress_SetCurrentEvent(0xA2);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x360A:
            Progress_SetGate(0x37);
            Progress_SetCurrentEvent(0xA3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x360B:
            Progress_SetGate(56);
            Progress_SetCurrentEvent(0xA4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x360C:
            Progress_SetGate(0x39);
            Progress_SetCurrentEvent(0xA6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x360D:
            Progress_SetCurrentEvent(0xA5);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_020874fc(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x360F:
            Progress_SetCurrentEvent(0xAA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3610:
            Progress_SetCurrentEvent(0xAB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087574(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3612:
            Progress_SetCurrentEvent(0xAA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3613:
            Progress_SetCurrentEvent(0xAD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020875ec(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3615:
            Progress_SetCurrentEvent(0x3D2);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3616:
            Progress_SetCurrentEvent(0x3D3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3617:
            Progress_SetCurrentEvent(0x3D4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3618:
            Progress_SetCurrentEvent(0x3D5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3619:
            Progress_SetCurrentEvent(0x3D6);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_020876c8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x8A:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x808B;
            gSaveState.unk_20.unk_3124          = 9;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x8B:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_SHUTDOWN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SHUTDOWN);
            break;
        case 0x8C:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x8D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x8D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x8E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x8E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x8F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x8F:
            gSaveState.unk_20.unk_24BE = 0xC1;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(0x15, 4);
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            gSaveState.unk_20.unk_267A                 = 4;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_EASY);
            break;
        case 0x90:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_TALK_TO_YOUR_PARTNER);
            break;
        case 0x91:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x43A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x43A:
            if ((func_ov030_020848e4(arg0, 0x49) != 0) || (func_ov030_020848e4(arg0, 0x4A) != 0)) {
                arg0->unk_21630 = 5;
                DebugOvlDisp_Pop();
                return 0;
            }
            if (func_ov030_020848e4(arg0, 0x47) != 0) {
                Progress_SetGate(0x3B);
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0x3D9);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (func_ov030_020848e4(arg0, 0x48) != 0) {
                Progress_SetGate(0x24);
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0xAE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
        case 0x92:
            Progress_SetGate(5);
        case 0x93:
        case 0x94:
        case 0x95:
            if ((Progress_Check(47) != 0) && (Progress_Check(0x30) != 0) && (Progress_Check(0x31) != 0)) {
                Progress_SetCurrentEvent(0x96);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (Progress_Check(0x30) != 0) {
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
            if (Progress_Check(5) != 0) {
                Progress_SetCurrentEvent(0x78C);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x78C:
            if (Progress_Check(0x3F) == 0) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0xA0);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xA9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xA0:
            Progress_SetGate(0x3F);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xA1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA6:
            if ((Progress_Check(0x36) != 0) && (Progress_Check(0x37) != 0) && (Progress_Check(56) != 0) &&
                (Progress_Check(0x39) != 0))
            {
                return func_ov030_020a9f54(arg0, &func_ov030_02087414, 0x3608, 5, 0x3609, 0x360A, 0x360B, 0x360C, 0x360D);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087414, 0x3608, 4, 0x3609, 0x360A, 0x360B, 0x360C);
        case 0xA5:
            Progress_SetCurrentEvent(0x4CA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xA8:
            Progress_SetCurrentEvent(0xA9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xA9:
            return func_ov030_020a9f54(arg0, &func_ov030_020874fc, 0x360E, 2, 0x360F, 0x3610);
        case 0xAA:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0xACU;
            func_ov030_020af364(9);
            return 1;
        case 0xAC:
            return func_ov030_020a9f54(arg0, &func_ov030_02087574, 0x3611, 2, 0x3612, 0x3613);
        case 0xAD:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x78D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x78D:
            Progress_SetGate(3);
            gSaveState.unk_20.unk_2674 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x78E:
            if ((Progress_Check(0x42) != 0) && (Progress_Check(0x43) != 0)) {
                Progress_SetCurrentEvent(0x790);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (Progress_Check(0x41) != 0) {
                Progress_SetCurrentEvent(0x78F);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x78F:
            if (Progress_Check(0x42) == 0) {
                Progress_SetGate(0x42);
                gSaveState.unk_20.unk_30FC = 0;
                gSaveState.unk_20.unk_30FE = 1;
                gSaveState.unk_20.unk_3100 = 0x15;
                arg0->unk_21CF8            = 1;
            }
            break;
        case 0x790:
            Progress_SetGate(4);
            gSaveState.unk_20.unk_2678 = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_JUMP);
            arg0->unk_21CF8 = 1;
            break;
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
            if ((Progress_Check(0x33) != 0) && (Progress_Check(0x34) != 0) && (Progress_Check(0x35) != 0)) {
                func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
                Progress_SetCurrentEvent(0x9F);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (Progress_Check(0x33) != 0) {
                return func_ov030_020a9f54(arg0, &func_ov030_02087360, 0x3604, 3, 0x3605, 0x3606, 0x3607);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087360, 0x3604, 2, 0x3605, 0x3606);
        case 0x9F:
            Progress_SetGate(18);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x43C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x43C:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_MEMES);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E1:
        // return func_ov030_020aa0c8(arg0, 0xA2, 0x3E2, 0x3E3, &func_ov030_02086f10);
        case 0x3F1:
        case 0x3F0:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3F1, 0x3F2, &func_ov030_020870ec);
        case 0x3E2:
        case 0x3E7:
        case 0x3E6:
        case 0x3E5:
        case 0x3E4:
            Progress_SetGate(6);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3D9:
            if (Progress_Check(37) == 0) {
                if (Progress_Check(58) == 0) {
                    Progress_SetGate(58);
                    Progress_SetCurrentEvent(0x3D1);
                    Progress_AdvanceEventScript(arg0);
                    return 1;
                }
                Progress_SetCurrentEvent(0x3D7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((Progress_Check(42) != 0) && (Progress_Check(0x10) == 0)) {
                Progress_SetGate(0x10);
                Progress_SetCurrentEvent(0x3DE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((Progress_Check(0x2C) != 0) && (Progress_Check(0x13) == 0)) {
                Progress_SetGate(0x13);
                Progress_SetCurrentEvent(0x3E0);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (Progress_Check(58) == 0) {
                Progress_SetGate(58);
                Progress_SetCurrentEvent(0x3D1);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3D7);
            Progress_AdvanceEventScript(arg0);
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
            Progress_SetGate(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB0:
            Progress_SetGate(8);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB1:
            arg0->unk_21CF8 = 1;
            break;
        case 0xB2:
            Progress_SetGate(10);
            Progress_SetGate(0x41);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB3:
            Progress_SetGate(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E8:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3E9, 0x3EA, &func_ov030_02086fdc);
        case 0x3EF:
        case 0x3EE:
        case 0x3ED:
            Progress_SetGate(37);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB4:
            Progress_SetGate(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DB:
            Progress_SetGate(38);
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
            Progress_SetGate(0x10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DF:
            Progress_SetGate(12);
            Progress_ClearGate(13);
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0xC0);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E0:
            Progress_SetGate(0x13);
            Progress_SetGate(0xC);
            Progress_ClearGate(0xD);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E9:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3E9, 0x3EA, &func_ov030_02086fdc);
        case 0x3F4:
            Progress_SetGate(0xD);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB5:
            Progress_SetGate(15);
            arg0->unk_21CF8 = 1;
            return;
        case 0xB6:
            Progress_SetGate(15);
            func_ov030_020aac28(arg0, PIN_LUCKY_STAR);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB9:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84CC;
            func_ov030_020af364(10);
            return 1;
        case 0xBA:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x80BB;
            gSaveState.unk_20.unk_3124          = 10;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0xBB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xBC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xBC:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xBD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xBD:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xBE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xBE:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xBF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xBF:
            gSaveState.unk_20.unk_3124 = 11;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x3EB:
            Progress_SetGate(0x4A);
            break;
        case 0x3F2:
            Progress_SetGate(0x4B);
            break;
        case 0x4CD:
            Progress_SetGate(0x48);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_ITEM_ABILITIES);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_SHOP_CLERKS);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4CA:
            Progress_SetGate(0x45);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_SHIKI);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_ESP_CARDS);
            }
            break;
        case 0x4CB:
            Progress_SetGate(0x46);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_YELLOW_NOISE_SYMBOLS);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_RED_NOISE_SYMBOLS);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4CC:
            Progress_SetGate(0x47);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_POISON_BONES);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0xC0:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_YUKATA, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_YUKATA);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02088a5c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(37);
    gSaveState.unk_20.unk_264C          = 0x1DA000;
    gSaveState.unk_20.unk_2650          = 0xBE000;
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_26B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x80C4;
    gSaveState.unk_20.unk_3124          = 58;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_26B8 = 0;
    gSaveState.unk_20.unk_2648 = 2;
    gSaveState.unk_20.unk_2666 = 2;
}

s32 func_ov030_02088b60(ProgressObject* arg0) {
    if ((Progress_Check(47) != 0) && (Progress_Check(2) == 0)) {
        if (Progress_Check(0x34) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) != 0) {
                Progress_SetGate(0x34);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) == 0) {
            Progress_SetGate(0x35);
            Progress_ClearGate(0x34);
        }
    }
    if ((Progress_Check(0x30) != 0) && (Progress_Check(3) == 0)) {
        if (Progress_Check(0x31) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) != 0) {
                Progress_SetGate(0x31);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) == 0) {
            Progress_SetGate(0x36);
            Progress_ClearGate(0x31);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 3) {
        if (Progress_Check(0x37) == 0) {
            Progress_SetGate(0x37);
            Progress_BeginScriptedEvent(arg0, 0xC5, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (Progress_Check(4) != 0) && (Progress_Check(37) == 0) &&
            (Progress_Check(0x2C) == 0))
        {
            Progress_SetGate(0x2C);
            Progress_BeginScriptedEvent(arg0, 0x3F7, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 0) == 0) {
            Progress_ClearGate(0x2C);
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (Progress_Check(4) == 0)) {
            Progress_SetGate(4);
            Progress_BeginScriptedEvent(arg0, 0xC7, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 56) != 0) && (Progress_Check(4) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x40C9, 0);
            return 1;
        }
        if ((Progress_Check(37) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x80CE, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((func_ov030_020c1a9c() != 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0xCF, 0);
            return 1;
        } else if ((Progress_Check(47) != 0) && (Progress_Check(0x32) == 0)) {
            Progress_SetGate(0x32);
            Progress_BeginScriptedEvent(arg0, 0x441, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 11) {
        if ((func_ov030_020848a4(0xAA, 0xB4, 0xB4, 0x140) != 0) && (Progress_Check(0x28) == 0)) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0xD0, 0);
            return 1;
        } else if ((Progress_Check(0x28) != 0) && (Progress_Check(56) == 0)) {
            Progress_SetGate(56);
            Progress_BeginScriptedEvent(arg0, 0x444, 0);
            return 1;
        } else if ((Progress_Check(0x30) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
            Progress_BeginScriptedEvent(arg0, 0x440, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 12) && (func_ov030_020848e4(arg0, 15) != 0) && (Progress_Check(6) == 0)) {
        Progress_SetGate(6);
        Progress_BeginScriptedEvent(arg0, 0xD3, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 13) {
        if (Progress_Check(41) == 0) {
            Progress_SetGate(41);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x80D4;
            gSaveState.unk_20.unk_3124          = 12;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
        if ((Progress_Check(45) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0xD5, 0);
            return 1;
        } else if ((Progress_Check(46) != 0) && (Progress_Check(43) == 0)) {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0xD6, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020890c0(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            Progress_SetGate(0x22);
            Progress_SetCurrentEvent(0xCA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361C:
            Progress_SetGate(0x23);
            Progress_SetCurrentEvent(0xCB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02089148(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            Progress_SetGate(0x22);
            Progress_SetCurrentEvent(0xCA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361C:
            Progress_SetGate(0x23);
            Progress_SetCurrentEvent(0xCB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361D:
            Progress_SetGate(0x24);
            Progress_SetCurrentEvent(0xCC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361E:
            Progress_SetGate(37);
            Progress_SetCurrentEvent(0xCD);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02089214(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            Progress_SetGate(0x22);
            Progress_SetCurrentEvent(0xCA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361C:
            Progress_SetGate(0x23);
            Progress_SetCurrentEvent(0xCB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361D:
            Progress_SetGate(0x24);
            Progress_SetCurrentEvent(0xCC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361E:
            Progress_SetGate(37);
            Progress_SetCurrentEvent(0xCD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x361F:
            Progress_SetCurrentEvent(0x43D);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02089300(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0xC4:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 3;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x1D8000;
            arg0->unk_21B0C = 0xC1000;
            break;
        case 0xC5:
            gSaveState.unk_20.unk_24BE = 0xD8;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(13, 4);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xC8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xC6:
            Progress_SetGate(4);
            Progress_SetGate(0x2C);
            gSaveState.unk_20.unk_26B8 = 2;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_NOISE_REPORT);
            arg0->unk_21CF8 = 1;
            break;
        case 0xC7:
            Progress_SetGate(0x2C);
            gSaveState.unk_20.unk_26B8 = 2;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_NOISE_REPORT);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3F7:
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0xD7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
            if ((Progress_Check(0x22) != 0) && (Progress_Check(0x23) != 0) && (Progress_Check(0x24) != 0) &&
                (Progress_Check(37) != 0))
            {
                return func_ov030_020a9f54(arg0, func_ov030_02089214, 0x361A, 5, 0x361B, 0x361C, 0x361D, 0x361E, 0x361F);
            }
            if (Progress_Check(0x23) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02089148, 0x361A, 4, 0x361B, 0x361C, 0x361D, 0x361E);
            }
            return func_ov030_020a9f54(arg0, func_ov030_020890c0, 0x361A, 2, 0x361B, 0x361C);
        case 0x43D:
            gSaveState.unk_20.unk_2648 = 4;
            gSaveState.unk_20.unk_26B8 = 4;
            arg0->unk_21630            = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0xCE:
            Progress_SetCurrentEvent(0x43E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x43E:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_SHOP_QUESTS);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BRAND_AWARENESS);
            break;
        case 0xCF:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_PIN_GROWTH_EVOLUTION);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_GREEN_NOISE_SYMBOLS);
            break;
        case 0xD0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xD1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xD1:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BACKLASH);
            break;
        case 0xD3:
            Progress_SetGate(6);
            gSaveState.unk_20.unk_2666 = 4;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_MINGLE_MODE);
            arg0->unk_21CF8 = 1;
            break;
        case 0xD4:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84D4U;
            func_ov030_020af364(12);
            return 1;
        case 0xD5:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84D5U;
            func_ov030_020af364(11);
            return 1;
        case 0xD6:
            gSaveState.unk_20.unk_3124 = 13;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0xD2:
            Progress_SetGate(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x791:
            Progress_SetCurrentEvent(0x792);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x792:
            Progress_SetGate(47);
            if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) != 0) {
                Progress_SetCurrentEvent(0x793);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x793:
            Progress_SetGate(2);
            Progress_SetGate(0x34);
            gSaveState.unk_20.unk_264E = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_EXTRA_SLOT);
            arg0->unk_21CF8 = 1;
            break;
        case 0x794:
            Progress_SetCurrentEvent(0x795);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x795:
            Progress_SetGate(0x30);
            if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) != 0) {
                Progress_SetCurrentEvent(0x796);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x796:
            Progress_SetGate(3);
            Progress_SetGate(0x31);
            gSaveState.unk_20.unk_2664 = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_CHAINED_BATTLES);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4D4:
            Progress_SetGate(45);
            func_ov030_020aac28(arg0, PIN_LIGHTNING_MOON);
            break;
        case 0x4D5:
            Progress_SetGate(46);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_MASAMUNE);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0xD7:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_MRS_CLAUS, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_MRS_CLAUS);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02089a40(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(6);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_2658          = 4;
    gSaveState.unk_20.unk_3124          = 0x3B;
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x80DB;
}

s32 func_ov030_02089b28(s32 arg0) {
    s16 var_r0;
    s16 var_r0_2;

    if ((Progress_Check(0x48) != 0) && (Progress_Check(4) == 0)) {
        if (Progress_Check(0x49) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) != 0) {
                Progress_SetGate(0x49);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) == 0) {
            Progress_SetGate(0x4A);
            Progress_ClearGate(0x49);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 2) {
        if ((Progress_Check(2) != 0) && (Progress_Check(0x45) == 0)) {
            Progress_SetGate(0x45);
            Progress_BeginScriptedEvent(arg0, 0x4DE, 0);
            return 1;
        } else if ((Progress_Check(0x45) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0xE1, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 6) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (Progress_Check(0x43) != 0) && (Progress_Check(0x44) == 0)) {
            Progress_SetGate(0x44);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0) {
        if (Progress_Check(0x20) == 0) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0xDC, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3C) != 0) && (Progress_Check(33) == 0) && (Progress_Check(0x46) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x40DD, 0);
            return 1;
        }
        if ((gSaveState.unk_20.unk_3102 >= 3) && (Progress_Check(0x48) != 0) && (Progress_Check(0x49) == 0)) {
            Progress_SetGate(0x49);
        }
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0_2 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0_2 = 0;
        }
        if (((s32)var_r0_2 >= 3) && (Progress_Check(0x46) == 0)) {
            Progress_SetGate(0x46);
            Progress_BeginScriptedEvent(arg0, 0x4DF, 0);
            return 1;
        }
        if ((Progress_Check(0x46) != 0) && (Progress_Check(5) == 0)) {
            Progress_SetGate(5);
            Progress_BeginScriptedEvent(arg0, 0xDE, 0);
            return 1;
        }
        if ((Progress_Check(39) != 0) && (Progress_Check(0x28) != 0) && (Progress_Check(41) != 0) && (Progress_Check(6) == 0))
        {
            Progress_SetGate(6);
            Progress_BeginScriptedEvent(arg0, 0xDF, 0);
            return 1;
        }
        if ((Progress_Check(39) != 0) && (Progress_Check(0x28) != 0) && (Progress_Check(41) != 0) &&
            (Progress_Check(6) != 0) && (Progress_Check(9) == 0))
        {
            Progress_ClearGate(39);
            Progress_ClearGate(0x28);
            Progress_ClearGate(41);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
        }
        if ((Progress_Check(38) != 0) && (Progress_Check(8) == 0)) {
            Progress_SetGate(8);
            Progress_BeginScriptedEvent(arg0, 0xE6, 0);
            return 1;
        }
        if ((Progress_Check(37) != 0) && (Progress_Check(9) == 0)) {
            Progress_SetGate(9);
            Progress_BeginScriptedEvent(arg0, 0xEA, 0);
            return 1;
        }
        if ((Progress_Check(42) != 0) && (Progress_Check(0x4B) == 0)) {
            Progress_SetGate(0x4B);
            Progress_BeginScriptedEvent(arg0, 0x4E1, 0);
            return 1;
        }
        if ((Progress_Check(0x4B) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0xEB, 0);
            return 1;
        }
        if ((Progress_Check(0x47) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0xEC, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208a028(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3626:
            Progress_SetCurrentEvent(0xE7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3627:
            Progress_SetCurrentEvent(0xE8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a0a0(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x3620:
            Progress_SetCurrentEvent(0x3FA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3621:
            Progress_SetCurrentEvent(0x3FB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a120(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x3620:
            Progress_SetCurrentEvent(0x3FF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3621:
            Progress_SetCurrentEvent(0x400);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a19c(ProgressObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (Progress_Check(0x2C) != 0) {
                func_ov030_020a9db8(arg0, 0x460);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x45F);
            return 0;
        case 1:
            Progress_SetCurrentEvent(0x451);
            return 1;
        case 0:
            if (Progress_Check(47) == 0) {
                Progress_SetGate(47);
                Progress_SetCurrentEvent(0x44E);
                return 1;
            }
            if (Progress_Check(0x30) == 0) {
                Progress_SetGate(0x30);
                Progress_SetCurrentEvent(0x44F);
                return 1;
            }
            Progress_ClearGate(0x30);
            Progress_SetCurrentEvent(0x450);
            return 1;
        case 2:
            if (Progress_Check(45) == 0) {
                Progress_SetGate(45);
                Progress_SetCurrentEvent(0x452);
                return 1;
            }
            if (Progress_Check(46) == 0) {
                Progress_SetGate(46);
                Progress_SetCurrentEvent(0x453);
                return 1;
            }
            Progress_ClearGate(46);
            Progress_SetCurrentEvent(0x454);
            return 1;
        case 3:
            Progress_SetCurrentEvent(0x455);
            return 1;
        case 4:
            Progress_SetCurrentEvent(0x456);
            return 1;
        case 5:
            Progress_SetCurrentEvent(0x457);
            return 1;
        case 6:
            Progress_SetCurrentEvent(0x458);
            return 1;
        case 7:
            Progress_SetCurrentEvent(0x459);
            return 1;
        case 8:
            Progress_SetCurrentEvent(0x45A);
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

s32 func_ov030_0208a3f0(ProgressObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (Progress_Check(0x31) != 0) {
                func_ov030_020a9db8(arg0, 0x476);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x475);
            return 0;
        case 1:
            Progress_SetCurrentEvent(0x467);
            return 1;
        case 0:
            if (Progress_Check(0x34) == 0) {
                Progress_SetGate(0x34);
                Progress_SetCurrentEvent(0x464);
                return 1;
            }
            if (Progress_Check(0x35) == 0) {
                Progress_SetGate(0x35);
                Progress_SetCurrentEvent(0x465);
                return 1;
            }
            Progress_ClearGate(0x35);
            Progress_SetCurrentEvent(0x466);
            return 1;
        case 2:
            if (Progress_Check(0x32) == 0) {
                Progress_SetGate(0x32);
                Progress_SetCurrentEvent(0x468);
                return 1;
            }
            if (Progress_Check(0x33) == 0) {
                Progress_SetGate(0x33);
                Progress_SetCurrentEvent(0x469);
                return 1;
            }
            Progress_ClearGate(0x33);
            Progress_SetCurrentEvent(0x46A);
            return 1;
        case 3:
            Progress_SetCurrentEvent(0x46B);
            return 1;
        case 4:
            Progress_SetCurrentEvent(0x46C);
            return 1;
        case 5:
            Progress_SetCurrentEvent(0x46D);
            return 1;
        case 6:
            Progress_SetCurrentEvent(0x46E);
            return 1;
        case 7:
            Progress_SetCurrentEvent(0x46F);
            return 1;
        case 8:
            Progress_SetCurrentEvent(0x470);
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

s32 func_ov030_0208a648(ProgressObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (Progress_Check(0x36) != 0) {
                func_ov030_020a9db8(arg0, 0x48B);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x48A);
            return 0;
        case 1:
            Progress_SetCurrentEvent(0x47C);
            return 1;
        case 0:
            if (Progress_Check(0x39) == 0) {
                Progress_SetGate(0x39);
                Progress_SetCurrentEvent(0x479);
                return 1;
            }
            if (Progress_Check(58) == 0) {
                Progress_SetGate(58);
                Progress_SetCurrentEvent(0x47A);
                return 1;
            }
            Progress_ClearGate(58);
            Progress_SetCurrentEvent(0x47B);
            return 1;
        case 2:
            if (Progress_Check(0x37) == 0) {
                Progress_SetGate(0x37);
                Progress_SetCurrentEvent(0x47D);
                return 1;
            }
            if (Progress_Check(56) == 0) {
                Progress_SetGate(56);
                Progress_SetCurrentEvent(0x47E);
                return 1;
            }
            Progress_ClearGate(56);
            Progress_SetCurrentEvent(0x47F);
            return 1;
        case 3:
            Progress_SetCurrentEvent(0x480);
            return 1;
        case 4:
            Progress_SetCurrentEvent(0x481);
            return 1;
        case 5:
            Progress_SetCurrentEvent(0x482);
            return 1;
        case 6:
            Progress_SetCurrentEvent(0x483);
            return 1;
        case 7:
            Progress_SetCurrentEvent(0x484);
            return 1;
        case 8:
            Progress_SetCurrentEvent(0x485);
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

s32 func_ov030_0208a8a0(ProgressObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (Progress_Check(0x3B) != 0) {
                func_ov030_020a9db8(arg0, 0x4A0);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x49F);
            return 0;
        case 1:
            Progress_SetCurrentEvent(0x491);
            return 1;
        case 0:
            if (Progress_Check(0x3E) == 0) {
                Progress_SetGate(0x3E);
                Progress_SetCurrentEvent(0x48E);
                return 1;
            } else if (Progress_Check(0x3F) == 0) {
                Progress_SetGate(0x3F);
                Progress_SetCurrentEvent(0x48F);
                return 1;
            }
            Progress_ClearGate(0x3F);
            Progress_SetCurrentEvent(0x490);
            return 1;
        case 2:
            if (Progress_Check(0x3C) == 0) {
                Progress_SetGate(0x3C);
                Progress_SetCurrentEvent(0x492);
                return 1;
            } else if (Progress_Check(0x3D) == 0) {
                Progress_SetGate(0x3D);
                Progress_SetCurrentEvent(0x493);
                return 1;
            }
            Progress_ClearGate(0x3D);
            Progress_SetCurrentEvent(0x494);
            return 1;
        case 3:
            Progress_SetCurrentEvent(0x495);
            return 1;
        case 4:
            Progress_SetCurrentEvent(0x496);
            return 1;
        case 5:
            Progress_SetCurrentEvent(0x497);
            return 1;
        case 6:
            Progress_SetCurrentEvent(0x498);
            return 1;
        case 7:
            Progress_SetCurrentEvent(0x499);
            return 1;
        case 8:
            Progress_SetCurrentEvent(0x49A);
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

s32 func_ov030_0208aaf4(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0xDB:
            gSaveState.unk_20.unk_24BE = 0xEE;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(0, 4);
            break;
        case 0xDC:
            gSaveState.unk_20.unk_30FC = 0;
            gSaveState.unk_20.unk_30FE = 39;
            gSaveState.unk_20.unk_3100 = 0;
            break;
        case 0xDE:
            gSaveState.unk_20.unk_310C = &data_ov030_020d9b1c;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xDF:
            Progress_ClearGate(39);
            Progress_ClearGate(0x28);
            Progress_ClearGate(41);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE1:
            func_ov030_020aa9f4(arg0, 0x3621, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE0:
            Progress_SetGate(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE2:
            Progress_SetCurrentEvent(0xE3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x45F:
            Progress_SetGate(0x2C);
        case 0x460:
            Progress_ClearGate(47);
            Progress_ClearGate(0x30);
            Progress_ClearGate(45);
            Progress_ClearGate(46);
        case 0x45E:
        case 8:
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
        case 0xE7:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x462);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x461:
            Progress_SetGate(10);
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
            Progress_SetGate(38);
            break;
        case 0xE6:
        case 0xE9:
            Progress_SetGate(8);
            return func_ov030_020a9f54(arg0, &func_ov030_0208a028, 0x3625, 2, 0x3626, 0x3627);
        case 0xE8:
            arg0->unk_21CF8 = 1;
            break;
        case 0x45D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x461);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x475:
            Progress_SetGate(0x31);
        case 0x476:
            Progress_ClearGate(0x34);
            Progress_ClearGate(0x35);
            Progress_ClearGate(0x32);
            Progress_ClearGate(0x33);
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
            Progress_SetGate(0x40);
        case 0x473:
        case 0x472:
            Progress_SetCurrentEvent(0x477);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x48A:
            Progress_SetGate(0x36);
        case 0x48B:
            Progress_ClearGate(0x39);
            Progress_ClearGate(58);
            Progress_ClearGate(0x37);
            Progress_ClearGate(56);
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
            Progress_SetGate(0x41);
        case 0x488:
        case 0x487:
            Progress_SetCurrentEvent(0x48C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x49F:
            Progress_SetGate(0x3B);
        case 0x4A0:
            Progress_ClearGate(0x3E);
            Progress_ClearGate(0x3F);
            Progress_ClearGate(0x3C);
            Progress_ClearGate(61);
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
            Progress_SetGate(0x42);
        case 0x49D:
        case 0x49C:
            Progress_SetGate(37);
            Progress_SetCurrentEvent(0x4A1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x4A1:
            arg0->unk_21CF8 = 1;
            break;
        case 0xEA:
            gSaveState.unk_20.unk_310C = &data_ov030_020d9b44;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0) && (Progress_Check(0x40) != 0) &&
                (Progress_Check(0x41) != 0) && (Progress_Check(0x42) != 0))
            {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0xED);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0xEB:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84E0;
            func_ov030_020af364(13);
            return 1;
        case 0xEC:
            gSaveState.unk_20.unk_3124 = 0xE;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x797:
            if (Progress_Check(0x44) != 0) {
                Progress_SetCurrentEvent(0x799);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x798);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x798:
            if (Progress_Check(0x43) == 0) {
                Progress_SetGate(0x43);
                gSaveState.unk_20.unk_30FC = 0;
                gSaveState.unk_20.unk_30FE = 2;
                gSaveState.unk_20.unk_3100 = 6;
            }
            break;
        case 0x799:
            Progress_SetGate(3);
            Progress_SetGate(0x44);
            gSaveState.unk_20.unk_265A = 4;
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_DURABLE_LEATHER);
            func_ov030_020aac28(arg0, PIN_MURASAME);
            arg0->unk_21CF8 = 1;
            break;
        case 0x79A:
            Progress_SetCurrentEvent(0x79B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x79B:
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) != 0) {
                Progress_SetCurrentEvent(0x79C);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x79C:
            Progress_SetGate(4);
            Progress_SetGate(0x49);
            gSaveState.unk_20.unk_265E = 4;
            ProgressReward_GrantItem(arg0, ITEM_QUEST_CRIMSON_CHILI);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4DE:
            Progress_SetGate(0x45);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x4DF:
            Progress_SetGate(0x46);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BLOCK_SHIKI);
            break;
        case 0x4E0:
            Progress_SetGate(0x47);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_SEXY_D);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_RETRY_BATTLES);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0x4E1:
            Progress_SetGate(0x4B);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0xED:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_MR_DUCKY, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_MR_DUCKY);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208b87c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x80F1;
    gSaveState.unk_20.unk_3124          = 0x3C;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_2648 = 4;
    gSaveState.unk_20.unk_264E = 4;
    gSaveState.unk_20.unk_2658 = 4;
    gSaveState.unk_20.unk_265A = 4;
    gSaveState.unk_20.unk_2664 = 4;
    gSaveState.unk_20.unk_2662 = 4;
    Progress_SetGate(42);
}

s32 func_ov030_0208b97c(s32 arg0) {
    s16 var_r0;
    s16 var_r0_2;
    s16 var_r0_3;
    s16 var_r0_4;

    if ((Progress_Check(47) != 0) && (Progress_Check(4) == 0)) {
        if (Progress_Check(0x33) == 0) {
            if (Inventory_HasRequiredQuantity(0, 0, 0) != 0) {
                Progress_SetGate(0x33);
            }
        } else if (Inventory_HasRequiredQuantity(0, 0, 0) == 0) {
            Progress_SetGate(0x43);
            Progress_ClearGate(0x33);
        }
    }
    if ((Progress_Check(0x31) != 0) && (Progress_Check(6) == 0)) {
        if (Progress_Check(0x35) == 0) {
            if (Inventory_HasRequiredQuantity(0, 0, 0) != 0) {
                Progress_SetGate(0x35);
            }
        } else if (Inventory_HasRequiredQuantity(0, 0, 0) == 0) {
            Progress_SetGate(0x44);
            Progress_ClearGate(0x35);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 3) && (Progress_Check(46) != 0) && (Progress_Check(0x32) == 0)) {
            Progress_SetGate(0x32);
        }
        if ((func_ov030_020aed9c(0x3747) != 0) && (Progress_Check(12) == 0)) {
            Progress_SetGate(12);
            Progress_BeginScriptedEvent(arg0, 0xF5, 0);
            return 1;
        }
        if ((Progress_Check(2) != 0) && (Progress_Check(0x36) == 0)) {
            Progress_SetGate(0x36);
            Progress_BeginScriptedEvent(arg0, 0x4E8, 0);
            return 1;
        }
        if ((Progress_Check(0x36) != 0) && (Progress_Check(13) == 0)) {
            Progress_SetGate(13);
            Progress_BeginScriptedEvent(arg0, 0xF6, 0);
            return 1;
        }
        if ((Progress_Check(8) != 0) && (Progress_Check(0xE) == 0)) {
            Progress_SetGate(0xE);
            Progress_BeginScriptedEvent(arg0, 0xF7, 0);
            return 1;
        }
        if ((Progress_Check(0x22) != 0) && (Progress_Check(15) == 0)) {
            Progress_SetGate(15);
            Progress_BeginScriptedEvent(arg0, 250, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (Progress_Check(15) != 0) && (Progress_Check(0x24) == 0) && (Progress_Check(0x11) == 0))
        {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0xFD, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (Progress_Check(15) != 0) && (Progress_Check(0x24) != 0) &&
            (Progress_Check(0x11) == 0))
        {
            Progress_ClearGate(0x24);
        }
        if ((func_02023588() >= 1U) && (Progress_Check(18) != 0)) {
            Progress_SetGate(0x16);
        }
        if ((func_02023588() >= 2U) && (Progress_Check(18) != 0)) {
            Progress_SetGate(23);
        }
        if ((func_02023588() >= 3U) && (Progress_Check(18) != 0)) {
            Progress_SetGate(0x18);
        }
        if ((func_02023588() >= 3U) && (Progress_Check(18) != 0) && (Progress_Check(0x37) == 0)) {
            Progress_SetGate(0x37);
            Progress_BeginScriptedEvent(arg0, 0x4E9, 0);
            return 1;
        }
        if ((Progress_Check(0x37) != 0) && (Progress_Check(0x13) == 0)) {
            Progress_SetGate(0x13);
            Progress_BeginScriptedEvent(arg0, 0x100, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (Progress_Check(0x15) != 0) && (Progress_Check(38) == 0))
        {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x104, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (Progress_Check(0x15) != 0) && (Progress_Check(38) != 0))
        {
            Progress_ClearGate(38);
        }
        if ((Progress_Check(41) != 0) && (Progress_Check(0x46) == 0)) {
            Progress_SetGate(0x46);
            Progress_BeginScriptedEvent(arg0, 0x10C, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 3) {
        if ((Progress_Check(0x13) != 0) && (Progress_Check(0x14) == 0)) {
            Progress_SetGate(0x14);
            Progress_BeginScriptedEvent(arg0, 0x101, 0);
            return 1;
        }
        if ((func_02023588() >= 1U) && (Progress_Check(0x14) != 0)) {
            Progress_SetGate(0x19);
        }
        if ((func_02023588() >= 2U) && (Progress_Check(0x14) != 0)) {
            Progress_SetGate(0x1A);
        }
        if ((func_02023588() >= 3U) && (Progress_Check(0x14) != 0)) {
            Progress_SetGate(0x1B);
        }
        if ((func_02023588() >= 3U) && (Progress_Check(0x14) != 0) && (Progress_Check(56) == 0)) {
            Progress_SetGate(56);
            Progress_BeginScriptedEvent(arg0, 0x4EA, 0);
            return 1;
        }
        if ((Progress_Check(56) != 0) && (Progress_Check(0x15) == 0)) {
            Progress_SetGate(0x15);
            Progress_BeginScriptedEvent(arg0, 0x102, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 4) != 0) || (func_ov030_020848e4(arg0, 5) != 0)) && (Progress_Check(0x15) != 0) &&
            (Progress_Check(37) == 0))
        {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x103, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 4) == 0) && (func_ov030_020848e4(arg0, 5) == 0) && (Progress_Check(0x15) != 0) &&
            (Progress_Check(37) != 0))
        {
            Progress_ClearGate(37);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 4) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0_2 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0_2 = 0;
        }
        if (((s32)var_r0_2 >= 3) && (Progress_Check(47) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
        }
        if ((Progress_Check(0xE) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0xF8, 0);
            return 1;
        }
        if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(0x45) != 0) && (Progress_Check(1) == 0)) {
            Progress_SetGate(1);
            Progress_BeginScriptedEvent(arg0, 0x10D, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0) {
        if ((Progress_Check(10) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0xFB, 0);
            return 1;
        }
        if ((Progress_Check(9) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0xF9, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 8) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0_3 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0_3 = 0;
        }
        if (((s32)var_r0_3 >= 2) && (Progress_Check(0x30) != 0) && (Progress_Check(0x34) == 0)) {
            Progress_SetGate(0x34);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 12) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0_4 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0_4 = 0;
        }
        if (((s32)var_r0_4 >= 2) && (Progress_Check(0x31) != 0) && (Progress_Check(0x35) == 0)) {
            Progress_SetGate(0x35);
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(7);
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 37) && (Progress_Check(0x28) != 0) && (Progress_Check(41) == 0)) {
        Progress_SetGate(41);
        Progress_BeginScriptedEvent(arg0, 0x10A, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_0208c3d8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x3628:
            Progress_SetCurrentEvent(0x404);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3629:
            Progress_SetCurrentEvent(0x405);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362A:
            Progress_SetCurrentEvent(0x406);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c480(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x3628:
            Progress_SetCurrentEvent(0x40A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3629:
            Progress_SetCurrentEvent(0x40B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362A:
            Progress_SetCurrentEvent(0x40C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c528(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x3628:
            Progress_SetCurrentEvent(0x40A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3629:
            Progress_SetCurrentEvent(0x40E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362A:
            Progress_SetCurrentEvent(0x40C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c5d0(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x3628:
            Progress_SetCurrentEvent(0x412);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3629:
            Progress_SetCurrentEvent(0x413);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362A:
            Progress_SetCurrentEvent(0x414);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c678(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x362C:
            Progress_SetGate(0x39);
            Progress_SetCurrentEvent(0x7A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362D:
            Progress_SetGate(58);
            Progress_SetCurrentEvent(0x7A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362E:
            Progress_SetGate(0x3B);
            Progress_SetCurrentEvent(0x7A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c730(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3630:
            Progress_SetGate(0x3C);
            Progress_SetCurrentEvent(0x7A4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3631:
            Progress_SetGate(61);
            Progress_SetCurrentEvent(0x7A4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3632:
            Progress_SetGate(0x3E);
            Progress_SetCurrentEvent(0x7A4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c7e8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3634:
            Progress_SetGate(0x3F);
            Progress_SetCurrentEvent(0x7A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3635:
            Progress_SetGate(0x40);
            Progress_SetCurrentEvent(0x7A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3636:
            Progress_SetGate(0x41);
            Progress_SetCurrentEvent(0x7A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c8a0(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0xF1:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xF2);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xF2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0xF3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0xF3:
            gSaveState.unk_20.unk_24BE = 0x10E;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(1, 4);
            ProgressReward_GrantPin(arg0, PIN_RED_SKULL);
            break;
        case 0xF4:
            Progress_SetGate(11);
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
            Progress_SetGate(8);
            break;
        case 0x403:
            arg0->unk_21CF8 = 1;
            break;
        case 0x40F:
        case 0x410:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x410, 0x411, &func_ov030_0208c5d0);
        case 0x407:
        case 0x414:
            Progress_SetGate(10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x412:
            Progress_SetGate(0x45);
            break;
        case 0x40B:
        case 0x40D:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x40D, 0x409, &func_ov030_0208c528);
        case 0x408:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x408, 0x409, &func_ov030_0208c480);
        case 0x40E:
            Progress_SetGate(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0xFA:
            gSaveState.unk_20.unk_264A = 2;
            gSaveState.unk_20.unk_264E = 2;
            gSaveState.unk_20.unk_2648 = 2;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020aec38(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            arg0->unk_21CF8                            = 1;
            break;
        case 0xFC:
            Progress_SetGate(0x10);
            arg0->unk_21CF8 = 1;
            break;
        case 0xFE:
            Progress_SetGate(0x11);
            gSaveState.unk_20.unk_264A = 4;
            gSaveState.unk_20.unk_264E = 4;
            gSaveState.unk_20.unk_2648 = 4;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            arg0->unk_21CF8                            = 1;
            break;
        case 0xFF:
            func_02023598(0);
            gSaveState.unk_20.unk_312C = 1;
            Progress_SetGate(18);
            Progress_SetCurrentEvent(0x43F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x43F:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_BE_A_TRENDSETTER);
            arg0->unk_21CF8 = 1;
            break;
        case 0x100:
            arg0->unk_21CF8 = 1;
            break;
        case 0x101:
            func_02023598(0);
            gSaveState.unk_20.unk_312C = 2;
            arg0->unk_21CF8            = 1;
            break;
        case 0x102:
            func_ov030_020aec38(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x104:
            gSaveState.unk_20.unk_2648 = 2;
            gSaveState.unk_20.unk_264A = 2;
            gSaveState.unk_20.unk_264E = 2;
            break;
        case 0x106:
            Progress_SetGate(0x28);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x107);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x107:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x108);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x108:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x109);
            Progress_AdvanceEventScript(arg0);
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
            Progress_SetCurrentEvent(0x10B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x10B:
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            arg0->unk_21AD0                            = 1;
            arg0->unk_21ACC                            = 0;
            arg0->unk_21AFC                            = 1;
            arg0->unk_21B00                            = 0x66;
            arg0->unk_21B08                            = 0x15D000;
            arg0->unk_21B0C                            = 0xD6000;
            break;
        case 0x10C:
            gSaveState.unk_20.unk_3124 = 15;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x79D:
            if (Progress_Check(46) == 0) {
                Progress_SetGate(46);
                Progress_SetCurrentEvent(0x79E);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x79F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x79F:
            Progress_SetCurrentEvent(0x7A0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7A0:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c678, 0x362B, 3, 0x362C, 0x362D, 0x362E);
        case 0x7A3:
            Progress_SetCurrentEvent(0x7A1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7A1:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c730, 0x362F, 3, 0x3630, 0x3631, 0x3632);
        case 0x7A4:
            Progress_SetCurrentEvent(0x7A2);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7A2:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c7e8, 0x3633, 3, 0x3634, 0x3635, 0x3636);
        case 0x7A5:
            if ((Progress_Check(0x3B) != 0) && (Progress_Check(0x3C) != 0) && (Progress_Check(0x41) != 0)) {
                Progress_SetCurrentEvent(0x7A6);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7A7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7A7:
            Progress_ClearGate(0x39);
            Progress_ClearGate(58);
            Progress_ClearGate(0x3B);
            Progress_ClearGate(0x3C);
            Progress_ClearGate(61);
            Progress_ClearGate(0x3E);
            Progress_ClearGate(0x3F);
            Progress_ClearGate(0x40);
            Progress_ClearGate(0x41);
            break;
        case 0x7A6:
            Progress_SetGate(3);
            Progress_SetGate(0x32);
            gSaveState.unk_20.unk_264A = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantPin(arg0, PIN_ORICHALCUM);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7A8:
            Progress_SetCurrentEvent(0x7A9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7A9:
            Progress_SetGate(47);
            if (Inventory_HasRequiredQuantity(0xFF, 3, 0) != 0) {
                Progress_SetCurrentEvent(0x7AA);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7AA:
            Progress_SetGate(4);
            Progress_SetGate(0x33);
            gSaveState.unk_20.unk_2654 = 4;
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_SLEEK_SILK);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7AB:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(7) != 0) && (Progress_Check(0x30) == 0)) {
                Progress_SetCurrentEvent(0x7AF);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(7) != 0)) {
                Progress_SetCurrentEvent(0x7AD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(7) == 0)) {
                Progress_SetCurrentEvent(0x7AE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7AC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7AC:
            Progress_SetGate(0x30);
            break;
        case 0x7AF:
        case 0x7AD:
            Progress_SetGate(5);
            Progress_SetGate(0x34);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            gSaveState.unk_20.unk_2666 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7AE:
            Progress_SetGate(5);
            Progress_SetGate(0x34);
            gSaveState.unk_20.unk_2666 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7B0:
            Progress_SetCurrentEvent(0x7B1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7B1:
            Progress_SetGate(0x31);
            if (Inventory_HasRequiredQuantity(0x5C, 1, 3) != 0) {
                Progress_SetCurrentEvent(0x7B2);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7B2:
            Progress_SetGate(6);
            Progress_SetGate(0x35);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_DURABLE_LEATHER);
            gSaveState.unk_20.unk_265E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x4E8:
            Progress_SetGate(0x36);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4E9:
            Progress_SetGate(0x37);
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            break;
        case 0x4EA:
            Progress_SetGate(0x38);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_SHIKI6_SECRET_BOX_CENTER_ST:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SOLID_SLAMMER, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_SOLID_SLAMMER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208d708(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8111;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 61;
    gSaveState.unk_20.unk_264C = 4;
    gSaveState.unk_20.unk_26CE = 4;
    gSaveState.unk_20.unk_2648 = 4;
    gSaveState.unk_20.unk_264A = 4;
    gSaveState.unk_20.unk_264E = 4;
    gSaveState.unk_20.unk_2650 = 4;
    gSaveState.unk_20.unk_2652 = 4;
}

s32 func_ov030_0208d80c(ProgressObject* arg0) {
    if ((Progress_Check(0x36) != 0) && (Progress_Check(2) == 0)) {
        if (Progress_Check(0x37) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) != 0) {
                Progress_SetGate(0x37);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) == 0) {
            Progress_SetGate(56);
            Progress_ClearGate(0x37);
        }
    }
    if ((Progress_Check(0x39) != 0) && (Progress_Check(4) == 0)) {
        if (Progress_Check(58) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) != 0) {
                Progress_SetGate(58);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) == 0) {
            Progress_SetGate(0x3B);
            Progress_ClearGate(58);
        }
    }
    if ((gSaveState.unk_20.unk_3102 >= 3) && (Progress_Check(43) != 0) && (Progress_Check(0x2C) == 0)) {
        Progress_SetGate(0x2C);
    }
    if ((gSaveState.unk_20.unk_2458 == 1) && (Progress_Check(0x24) != 0) && (Progress_Check(37) == 0)) {
        Progress_SetGate(37);
        gSaveState.unk_20.unk_3124 = 18;
        gSaveState.unk_20.unk_3128 = 1;
        arg0->unk_21630            = 4;
        DebugOvlDisp_Pop();
        return 0;
    }
    if ((gSaveState.unk_20.unk_2458 == 5) && (func_ov030_020848e4(arg0, 7) != 0) && (Progress_Check(33) == 0) &&
        (Progress_Check(43) == 0))
    {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 0x114, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 0x11) {
        if ((func_ov030_020848a4(0, 0x1EA, 0x190, 0x1F4) != 0) && (Progress_Check(6) == 0)) {
            Progress_BeginScriptedEvent(arg0, 0x117, 0);
            return 1;
        } else if ((Progress_Check(0x28) != 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x118, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 18) {
        if (Progress_Check(0x20) == 0) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x11A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (Progress_Check(0x23) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(0x23) == 0) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x811B, 0);
            return 1;
        } else if ((Progress_Check(41) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x11C, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208dbd8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3638:
            Progress_SetGate(45);
            Progress_SetCurrentEvent(0x7BC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3639:
            Progress_SetGate(46);
            Progress_SetCurrentEvent(0x7BC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x363A:
            Progress_SetGate(47);
            Progress_SetCurrentEvent(0x7BC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208dc90(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x363C:
            Progress_SetGate(0x30);
            Progress_SetCurrentEvent(0x7BD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x363D:
            Progress_SetGate(0x31);
            Progress_SetCurrentEvent(0x7BD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x363E:
            Progress_SetGate(0x32);
            Progress_SetCurrentEvent(0x7BD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208dd48(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3640:
            Progress_SetGate(0x33);
            Progress_SetCurrentEvent(0x7BE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3641:
            Progress_SetGate(0x34);
            Progress_SetCurrentEvent(0x7BE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3642:
            Progress_SetGate(0x35);
            Progress_SetCurrentEvent(0x7BE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ddfc(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x111:
            gSaveState.unk_20.unk_24BE = 0x11F;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(18, 4);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x112);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x112:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x113);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7C7:
            if (Progress_Check(0x2C) != 0) {
                Progress_SetCurrentEvent(0x7C9);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7C8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7C8:
            if (Progress_Check(43) == 0) {
                Progress_SetGate(43);
                gSaveState.unk_20.unk_3102 = 0;
            }
            break;
        case 0x7C9:
            Progress_SetGate(5);
            gSaveState.unk_20.unk_2656 = 4;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x115);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x115:
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x116:
        case 0x117:
            Progress_SetGate(6);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84F2;
            func_ov030_020af364(15);
            return 1;
        case 0x118:
            Progress_SetCurrentEvent(0x119);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x11B:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8415U;
            gSaveState.unk_20.unk_3124          = 0x10;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x415:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84F3U;
            func_ov030_020af364(0xE);
            return 1;
        case 0x11C:
            func_ov030_020aec38(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            gSaveState.unk_20.unk_24B4                 = 0;
            gSaveState.unk_20.unk_24B8                 = 1;
            gSaveState.unk_20.currentStoryEvent        = 0x811D;
            gSaveState.unk_20.unk_3124                 = 0x11;
            gSaveState.unk_20.unk_3128                 = 0;
            arg0->unk_21630                            = 4;
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
        case 0x7BD:
            Progress_SetCurrentEvent(0x7BB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7B4:
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) != FALSE) {
                Progress_SetCurrentEvent(EVENT_SHIKI7_104_REAPER);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case EVENT_SHIKI7_104_REAPER:
            Progress_SetGate(2);
            Progress_SetGate(0x37);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_CHOICE_MEAT);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            gSaveState.unk_20.unk_2648 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7B6:
            if (Progress_Check(42) == 0) {
                Progress_SetGate(42);
                Progress_SetCurrentEvent(0x7B7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7B8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7B3:
            Progress_SetCurrentEvent(0x7B4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7B9:
            return func_ov030_020a9f54(arg0, func_ov030_0208dbd8, 0x3637, 3, 0x3638, 0x3639, 0x363A);
        case 0x7B7:
            Progress_SetCurrentEvent(0x7B9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7BA:
            return func_ov030_020a9f54(arg0, func_ov030_0208dc90, 0x363B, 3, 0x363C, 0x363D, 0x363E);
        case 0x7BC:
            Progress_SetCurrentEvent(0x7BA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7BB:
            return func_ov030_020a9f54(arg0, func_ov030_0208dd48, 0x363F, 3, 0x3640, 0x3641, 0x3642);
        case 0x7BE:
            if ((Progress_Check(46) != 0) && (Progress_Check(0x32) != 0) && (Progress_Check(0x35) != 0)) {
                Progress_SetCurrentEvent(EVENT_SHIKI7_SCRAMBLE_REAPER);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7C0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7C0:
            Progress_ClearGate(45);
            Progress_ClearGate(46);
            Progress_ClearGate(47);
            Progress_ClearGate(0x30);
            Progress_ClearGate(0x31);
            Progress_ClearGate(0x32);
            Progress_ClearGate(0x33);
            Progress_ClearGate(0x34);
            Progress_ClearGate(0x35);
            break;
        case EVENT_SHIKI7_SCRAMBLE_REAPER:
            Progress_SetGate(3);
            Progress_SetGate(0x3C);
            gSaveState.unk_20.unk_264A = 4;
            ProgressReward_GrantItem(arg0, ITEM_QUEST_SWEET_SWEET_ONIONS);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7B8:
            Progress_SetCurrentEvent(0x7B9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7C5:
            if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) != FALSE) {
                Progress_SetCurrentEvent(EVENT_SHIKI7_SCRAMBLE_MAGNUM);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case EVENT_SHIKI7_SCRAMBLE_MAGNUM:
            Progress_SetGate(4);
            Progress_SetGate(58);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_STUNNING_CRYSTALS);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_COLORFUL_THREAD);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            gSaveState.unk_20.unk_264E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x4F2:
            Progress_SetGate(0x28);
            func_ov030_020aac28(arg0, PIN_METEOR_MAGNET);
            break;
        case 0x4F3:
            Progress_SetGate(41);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            func_ov030_020aac28(arg0, PIN_EARTHSHAKE);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            break;
        case EVENT_SHIKI7_SECRET_BOX_MOLCO:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_MR_MEW_SUIT, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_MR_MEW_SUIT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208e6e4(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_264C = 4;
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
    gSaveState.unk_20.unk_24B4                 = 0;
    gSaveState.unk_20.unk_24B8                 = 1;
    gSaveState.unk_20.currentStoryEvent        = 0x8122;
    gSaveState.unk_20.unk_3124                 = 0x3E;
}

s32 func_ov030_0208e7e4(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((func_ov030_020848e4(arg0, 2) != 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x8123, 0);
            return 1;
        } else if ((Progress_Check(3) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x446, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 3) {
        if ((Progress_Check(37) == 0) && (Progress_Check(0x23) != 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x8130, 0);
            return 1;
        } else if ((Progress_Check(0x28) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x131, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 5) {
        if (Progress_Check(4) == 0) {
            Progress_SetGate(4);
            gSaveState.unk_20.unk_264C = 2;
            Progress_BeginScriptedEvent(arg0, 0x124, 0);
            return 1;
        } else if ((Progress_Check(39) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x126, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 2) != 0) && (Progress_Check(4) != 0) && (Progress_Check(0x24) != 0) &&
            (Progress_Check(0x23) == 0) && (Progress_Check(0x22) == 0))
        {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x128, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 2) == 0) {
            Progress_ClearGate(0x22);
        }
        if (func_ov030_02084860(0xEB, 0xAF, 0x64) != 0) {
            Progress_SetGate(0x24);
        }
        if ((Progress_Check(0x23) == 0) && (func_ov030_020aed9c(0x3767) != 0)) {
            Progress_SetGate(0x23);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8129;
            gSaveState.unk_20.unk_264C          = 4;
            gSaveState.unk_20.unk_3124          = 0x13;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        } else if ((Progress_Check(0x23) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x445, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208eb40(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3644:
            Progress_SetCurrentEvent(0x12B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3645:
            Progress_SetCurrentEvent(0x12C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ebbc(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3647:
            Progress_SetCurrentEvent(0x12E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3648:
            Progress_SetCurrentEvent(0x12F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ec3c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x122:
            gSaveState.unk_20.unk_24BE = 0x133;
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
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8125U;
            gSaveState.unk_20.unk_3124          = 0x14;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x125:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84FCU;
            func_ov030_020af364(0x10);
            return 1;
        case 0x126:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x129:
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
            Progress_SetGate(2);
            func_ov030_020aec38(9);
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7CA:
            if (Progress_Check(0x23) != 0) {
                Progress_SetCurrentEvent(0x7CB);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
        case 0x7CB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x12A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x12A:
            return func_ov030_020a9f54(arg0, func_ov030_0208eb40, 0x3643, 2, 0x3644, 0x3645);
        case 0x12B:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x812DU;
            func_ov030_020af364(18);
            return 1;
        case 0x12D:
            return func_ov030_020a9f54(arg0, func_ov030_0208ebbc, 0x3646, 2, 0x3647, 0x3648);
        case 0x12E:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x12DU;
            func_ov030_020af364(18);
            return 1;
        case 0x12F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x7CC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7CC:
            Progress_SetGate(3);
            gSaveState.unk_20.unk_2648 = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            func_ov030_020aac28(arg0, PIN_LONG_LIVE_THE_ICE);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_PRESS);
            arg0->unk_21CF8 = 1;
            break;
        case 0x130:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x84FDU;
            func_ov030_020af364(0x11);
            return 1;
        case 0x131:
            gSaveState.unk_20.unk_3124 = 0x15;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4FC:
            Progress_SetGate(39);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_JOSHUA);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_SIDESTEP);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_HIGH_AND_LOW_CARDS);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_JOSHUA1_104:
            Progress_SetGate(0x28);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_HARD);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_JOSHUA1_SECRET_BOX_HACHIKO:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_POTTED_PLANT, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_POTTED_PLANT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0208f160(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(37);
    gSaveState.unk_20.unk_264C          = 0x15D000;
    gSaveState.unk_20.unk_2650          = 0xD6000;
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_26B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8136;
    gSaveState.unk_20.unk_3124          = 0x3F;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_264A = 2;
    gSaveState.unk_20.unk_264E = 2;
    gSaveState.unk_20.unk_2658 = 4;
    gSaveState.unk_20.unk_265A = 4;
    gSaveState.unk_20.unk_2662 = 4;
    gSaveState.unk_20.unk_26C4 = 2;
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_0208f288(ProgressObject* arg0) {
    if ((Progress_Check(55) != 0) && (Progress_Check(6) == 0)) {
        if (Progress_Check(56) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) != 0) {
                Progress_SetGate(56);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) == 0) {
            Progress_SetGate(57);
            Progress_ClearGate(56);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (Progress_Check(0x20) == 0) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x137, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (Progress_Check(33) == 0) &&
            (Progress_Check(0x20) != 0) && (Progress_Check(0x22) == 0))
        {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x139, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 3) == 0)) {
            Progress_ClearGate(33);
        }
        if ((Progress_Check(0x22) == 0) && (func_ov030_020aed9c(0x3777) != 0)) {
            Progress_SetGate(0x22);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x813A;
            gSaveState.unk_20.unk_3124          = 0x16;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
        if ((Progress_Check(0x33) != 0) && (Progress_Check(7) == 0)) {
            Progress_SetGate(7);
            Progress_BeginScriptedEvent(arg0, 0x13E, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 4) && (func_ov030_020848a4(0, 0xC8, 250, 0xF0) != 0) && (Progress_Check(45) != 0) &&
        (Progress_Check(8) != 0) && (Progress_Check(38) == 0))
    {
        Progress_SetGate(38);
        Progress_BeginScriptedEvent(arg0, 0x143, 0);
        return 1;
    }
    if ((gSaveState.unk_20.unk_2458 == 0) && (func_ov030_020848e4(arg0, 0x3C) != 0) && (Progress_Check(8) != 0) &&
        (Progress_Check(39) == 0))
    {
        Progress_SetGate(39);
        Progress_BeginScriptedEvent(arg0, 0x4144, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 11) {
        if ((Progress_Check(10) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x13F, 0);
            return 1;
        }
        if ((Progress_Check(10) != 0) && (Progress_Check(11) != 0) && (Progress_Check(46) == 0)) {
            Progress_SetGate(46);
        }
        if ((func_ov030_020848e4(arg0, 0xE) != 0) && (Progress_Check(3) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x8140, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 12) {
        if ((func_ov030_020848e4(arg0, 13) != 0) && (Progress_Check(0x24) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x141, 0);
            return 1;
        }
        if ((Progress_Check(0x31) != 0) && (Progress_Check(8) == 0)) {
            Progress_SetGate(8);
            Progress_BeginScriptedEvent(arg0, 0x142, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 8) {
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (Progress_Check(0x28) == 0)) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x145, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (Progress_Check(0x28) != 0) && (Progress_Check(41) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((Progress_Check(9) != 0) && (Progress_Check(43) == 0)) {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, EVENT_JOSHUA2_TIN_PIN, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 11) != 0) || (func_ov030_020848e4(arg0, 13) != 0)) && (Progress_Check(43) != 0) &&
            (Progress_Check(0x2C) == 0))
        {
            Progress_SetGate(0x2C);
            Progress_BeginScriptedEvent(arg0, 0x153, 0);
            return 1;
        }
        if ((Progress_Check(0x34) != 0) && (Progress_Check(0x35) == 0)) {
            Progress_SetGate(0x35);
            gSaveState.unk_20.unk_3124 = 0x19;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 37) {
        if ((Progress_Check(0x22) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
            Progress_BeginScriptedEvent(arg0, 0x13B, 0);
            return 1;
        }
        if ((Progress_Check(0x28) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x146, 0);
            return 1;
        }
        if ((Progress_Check(0x2C) != 0) && (Progress_Check(0x34) == 0)) {
            Progress_SetGate(0x34);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8154;
            gSaveState.unk_20.unk_3124          = 0x18;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }
    return 0;
}

s32 func_ov030_0208fa28(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x364A:
            Progress_SetCurrentEvent(0x14A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x364B:
            Progress_SetCurrentEvent(0x14B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208faa4(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
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
            Progress_SetCurrentEvent(0x13C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x13C:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x813DU;
            gSaveState.unk_20.unk_3124          = 23;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
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
            gSaveState.unk_20.unk_24BE = 0x157;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(8, 4);
            gSaveState.unk_20.unk_264A = 4;
            gSaveState.unk_20.unk_264E = 4;
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
            func_ov030_020aec38(9);
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x13F:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
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
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x506U;
            func_ov030_020af364(0x13);
            return 1;
        case 0x142:
            gSaveState.unk_20.unk_2654 = 4;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x144:
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(EVENT_JOSHUA2_SECRET_BOX_SPAIN_HILL);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x146:
            Progress_SetCurrentEvent(0x147);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x147:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x148;
            gSaveState.unk_20.unk_24C2 = 0x149;
            gSaveState.unk_20.unk_341C = 1;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x148:
        case 0x149:
            return func_ov030_020a9f54(arg0, func_ov030_0208fa28, 0x3649, 2, 0x364A, 0x364B);
        case 0x14A:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x148;
            gSaveState.unk_20.unk_24C2 = 0x149;
            gSaveState.unk_20.unk_341C = 1;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x14B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x14C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x14C:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x14D;
            gSaveState.unk_20.unk_24C2 = 0x14E;
            gSaveState.unk_20.unk_341C = 0;
            SndMgr_StartPlayingSE();
            DebugOvlDisp_Pop();
            return 0;
        case 0x14D:
        case 0x14E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x14F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x14F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x151);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x151:
            Progress_SetGate(9);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        case EVENT_JOSHUA2_TIN_PIN:
            gSaveState.unk_20.unk_26C4 = 4;
            ProgressReward_GrantItem(arg0, ITEM_BOOK_TIN_PIN_SLAMMER);
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
            Progress_SetCurrentEvent(0x155);
            Progress_AdvanceEventScript(arg0);
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
            Progress_SetGate(45);
            break;
        case 0x7D0:
            if (Progress_Check(46) != 0) {
                Progress_SetCurrentEvent(0x7D2);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7D1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7D1:
            if (Progress_Check(4) == 0) {
                Progress_SetGate(4);
                arg0->unk_21CF8 = 1;
            }
            break;
        case 0x7D2:
            Progress_SetGate(3);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            gSaveState.unk_20.unk_2664 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7D3:
            if (Progress_Check(0x22) != 0) {
                Progress_SetCurrentEvent(0x7D4);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
        case 0x7D4:
            Progress_SetGate(47);
            if (func_02023f60(1, 10) != 0) {
                Progress_SetCurrentEvent(EVENT_JOSHUA2_SCRAMBLE_NP_JOSHUA);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case EVENT_JOSHUA2_SCRAMBLE_NP_JOSHUA:
            Progress_SetGate(5);
            Progress_SetGate(0x30);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_EXTRA_FINE_NOODLES);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7D6:
            Progress_SetCurrentEvent(0x7D7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7D7:
            Progress_SetGate(0x37);
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) != 0) {
                Progress_SetCurrentEvent(EVENT_JOSHUA2_MOLCO_REAPER);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case EVENT_JOSHUA2_MOLCO_REAPER:
            Progress_SetGate(6);
            Progress_SetGate(56);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_SPARKLING_SILVER);
            gSaveState.unk_20.unk_265E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x506:
            Progress_SetGate(0x31);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_ICHIMONJI);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            break;
        case EVENT_JOSHUA2_SECRET_BOX_SPAIN_HILL:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SUPERMASKS_MASK, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_SUPERMASKS_MASK);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209041c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24BE          = 0;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    gSaveState.unk_20.unk_26D4          = 2;
    gSaveState.unk_20.unk_26D6          = 4;
    gSaveState.unk_20.unk_26D8          = 0;
    gSaveState.unk_20.unk_26DA          = 4;
    gSaveState.unk_20.unk_26DC          = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x15A;
    func_ov030_020c26bc(4);
    gSaveState.unk_20.unk_2648 = 0;
    gSaveState.unk_20.unk_264C = 0;
    gSaveState.unk_20.unk_2652 = 0;
    gSaveState.unk_20.unk_2678 = 0;
    gSaveState.unk_20.unk_2666 = 0;
    gSaveState.unk_20.unk_266A = 0;
    gSaveState.unk_20.unk_265C = 0;
    gSaveState.unk_20.unk_2676 = 0;
    gSaveState.unk_20.unk_2670 = 0;
    gSaveState.unk_20.unk_3124 = 0x40;
}

s32 func_ov030_02090520(ProgressObject* arg0) {
    if ((Progress_Check(0x59) != 0) && (Progress_Check(4) == 0)) {
        if (Progress_Check(0x5A) == 0) {
            if (func_ov030_020aaad8() == 10) {
                Progress_SetGate(0x5A);
            }
        } else {
            s32 var_r0;
            if (func_ov030_020aaad8() == 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            if (var_r0 == 10) {
                Progress_SetGate(0x5B);
                Progress_ClearGate(0x5A);
            }
        }
    }
    if ((Progress_Check(0x48) != 0) && (Progress_Check(6) == 0)) {
        if (Progress_Check(0x5C) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) != 0) {
                Progress_SetGate(0x5C);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) == 0) {
            Progress_SetGate(0x5D);
            Progress_ClearGate(0x5C);
        }
    }
    if ((Progress_Check(0x5E) != 0) && (Progress_Check(7) == 0)) {
        if (Progress_Check(0x5F) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) != 0) {
                Progress_SetGate(0x5F);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) == 0) {
            Progress_SetGate(0x60);
            Progress_ClearGate(0x5F);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(9) != 0) && (Progress_Check(10) == 0)) {
            Progress_SetGate(10);
            Progress_BeginScriptedEvent(arg0, 0x172, 0);
            return 1;
        }
        if ((Progress_Check(46) != 0) && (Progress_Check(47) == 0)) {
            Progress_ClearGate(46);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 4) {
        if ((func_ov030_020848e4(arg0, 6) != 0) && (Progress_Check(0x20) == 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x15B, 0);
            return 1;
        }
        if ((Progress_Check(10) != 0) && (Progress_Check(47) == 0) && (Progress_Check(46) == 0)) {
            Progress_SetGate(46);
            Progress_BeginScriptedEvent(arg0, 0x173, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 5) && (Progress_Check(10) != 0) && (Progress_Check(47) == 0) &&
        (Progress_Check(46) == 0))
    {
        Progress_SetGate(46);
        Progress_BeginScriptedEvent(arg0, 0x173, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 6) {
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(9) != 0) && (Progress_Check(12) == 0) &&
            (Progress_Check(0x49) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(12) == 0) && (Progress_Check(0x49) != 0) &&
            (Progress_Check(0x4B) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (Progress_Check(12) == 0) && (Progress_Check(0x4B) != 0)) {
            Progress_ClearGate(0x4B);
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 11) && (Progress_Check(10) != 0) && (Progress_Check(47) == 0) &&
        (Progress_Check(46) == 0))
    {
        Progress_SetGate(46);
        Progress_BeginScriptedEvent(arg0, 0x173, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 12) {
        if (Progress_Check(33) == 0) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x15C, 0);
            return 1;
        }
        s32 var_r0;
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 3) && (Progress_Check(0x46) != 0) && (Progress_Check(0x47) == 0)) {
            Progress_SetGate(0x47);
        }
        if ((Progress_Check(5) != 0) && (Progress_Check(0x63) == 0)) {
            Progress_SetGate(0x63);
            Progress_BeginScriptedEvent(arg0, 0x447, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 13) {
        if ((func_ov030_020848e4(arg0, 15) != 0) && (Progress_Check(42) != 0) && (Progress_Check(43) == 0)) {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0x166, 0);
            return 1;
        }
        if ((Progress_Check(0x56) != 0) && (Progress_Check(9) == 0)) {
            Progress_SetGate(9);
            Progress_BeginScriptedEvent(arg0, 0x167, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((Progress_Check(10) != 0) && (Progress_Check(47) == 0)) {
            Progress_SetGate(47);
            gSaveState.unk_20.unk_26D4 = 2;
            Progress_BeginScriptedEvent(arg0, 0x174, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(47) != 0) && (Progress_Check(0x30) == 0)) {
            Progress_SetGate(0x30);
            Progress_SetGate(0x31);
            Progress_BeginScriptedEvent(arg0, 0x4175, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(11) != 0) && (Progress_Check(0x31) == 0) &&
            (Progress_Check(0x35) == 0))
        {
            Progress_SetGate(0x31);
            Progress_BeginScriptedEvent(arg0, 0x4177, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (Progress_Check(11) != 0) && (Progress_Check(0x35) == 0)) {
            Progress_ClearGate(0x31);
        }
        if ((Progress_Check(0x30) != 0) && (Progress_Check(11) == 0)) {
            Progress_SetGate(11);
            Progress_BeginScriptedEvent(arg0, 0x176, 0);
            return 1;
        }
        if ((Progress_Check(2) != 0) && (Progress_Check(0x57) == 0)) {
            Progress_SetGate(0x57);
            Progress_BeginScriptedEvent(arg0, 0x511, 0);
            return 1;
        }
        if ((Progress_Check(0x57) != 0) && (Progress_Check(0x32) == 0)) {
            Progress_SetGate(0x32);
            Progress_BeginScriptedEvent(arg0, 0x178, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(0x32) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
            Progress_BeginScriptedEvent(arg0, 0x4179, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(0x32) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
            Progress_BeginScriptedEvent(arg0, 0x4179, 0);
            return 1;
        }
        if ((Progress_Check(0x35) != 0) && (Progress_Check(0x36) == 0)) {
            Progress_SetGate(0x36);
            Progress_BeginScriptedEvent(arg0, 0x17C, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(0x35) != 0) && (Progress_Check(12) == 0)) {
            Progress_SetGate(12);
            Progress_BeginScriptedEvent(arg0, 0x417D, 0);
            return 1;
        }
        if ((Progress_Check(12) != 0) && (Progress_Check(13) == 0)) {
            Progress_SetGate(13);
            Progress_BeginScriptedEvent(arg0, 0x17E, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (Progress_Check(13) != 0) && (Progress_Check(56) == 0)) {
            Progress_SetGate(0x37);
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(13) != 0) && (Progress_Check(0x37) != 0) &&
            (Progress_Check(56) == 0))
        {
            Progress_SetGate(56);
            Progress_BeginScriptedEvent(arg0, 0x417F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(56) != 0) && (Progress_Check(0x31) == 0)) {
            Progress_SetGate(0x31);
            Progress_BeginScriptedEvent(arg0, 0x4177, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (Progress_Check(56) != 0) && (Progress_Check(0x31) != 0)) {
            Progress_ClearGate(0x31);
        }
        if ((Progress_Check(0x10) != 0) && (Progress_Check(61) == 0)) {
            Progress_SetGate(61);
            Progress_BeginScriptedEvent(arg0, 0x18A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(0x11) != 0)) {
            if ((Progress_Check(0x42) != 0) && (Progress_Check(0x3E) == 0)) {
                Progress_SetGate(0x3E);
                Progress_BeginScriptedEvent(arg0, 0x418B, 0);
                return 1;
            }
            if ((Progress_Check(0x43) != 0) && (Progress_Check(0x39) == 0)) {
                Progress_SetGate(0x39);
                Progress_BeginScriptedEvent(arg0, 0x4180, 0);
                return 1;
            }
            if ((Progress_Check(0x44) != 0) && (Progress_Check(0x3B) == 0)) {
                Progress_SetGate(0x3B);
                Progress_BeginScriptedEvent(arg0, 0x4183, 0);
                return 1;
            }
        }
        if (Progress_Check(0x3E) != 0) {
            Progress_BeginScriptedEvent(arg0, 0x818C, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if ((Progress_Check(13) != 0) && (Progress_Check(0x3C) == 0)) {
            Progress_SetGate(0x3C);
            Progress_BeginScriptedEvent(arg0, 0x187, 0);
            return 1;
        }
        if ((Progress_Check(3) != 0) && (Progress_Check(0x58) == 0)) {
            Progress_SetGate(0x58);
            Progress_BeginScriptedEvent(arg0, 0x512, 0);
            return 1;
        }
        if ((Progress_Check(0x58) != 0) && (Progress_Check(0x10) == 0)) {
            Progress_SetGate(0x10);
            Progress_BeginScriptedEvent(arg0, 0x189, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0xE) && (Progress_Check(0x2C) != 0) && (Progress_Check(45) == 0)) {
        gSaveState.unk_20.unk_26D2 = 4;
        Progress_SetGate(45);
    }
    if (gSaveState.unk_20.unk_2458 == 0x10) {
        if (Progress_Check(0x22) == 0) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x15D, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x415E, 0);
            return 1;
        }
        if ((Progress_Check(0x23) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x15F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(9) != 0) && (Progress_Check(0x2C) == 0) &&
            (Progress_Check(45) == 0))
        {
            Progress_SetGate(0x2C);
            Progress_BeginScriptedEvent(arg0, 0x416C, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(73) == 0) {
            Progress_SetGate(73);
            Progress_SetGate(75);
            Progress_BeginScriptedEvent(arg0, 0x18D, 0);
            return 1;
        }
        if ((Progress_Check(73) != 0) && (Progress_Check(75) == 0)) {
            Progress_SetGate(75);
            Progress_BeginScriptedEvent(arg0, 0x192, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209147c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3651:
            Progress_SetGate(38);
            Progress_SetCurrentEvent(0x161);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3652:
            Progress_SetGate(39);
            Progress_SetCurrentEvent(0x162);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3653:
            Progress_SetGate(0x28);
            Progress_SetCurrentEvent(0x163);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3654:
            Progress_SetGate(41);
            Progress_SetCurrentEvent(0x164);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091554(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3651:
            Progress_SetGate(38);
            Progress_SetCurrentEvent(0x161);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3652:
            Progress_SetGate(39);
            Progress_SetCurrentEvent(0x162);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3653:
            Progress_SetGate(0x28);
            Progress_SetCurrentEvent(0x163);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3654:
            Progress_SetGate(41);
            Progress_SetCurrentEvent(0x164);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3655:
            if (Progress_Check(42) == 0) {
                Progress_SetGate(42);
                Progress_SetCurrentEvent(0x165);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
    }
    return 0;
}

s32 func_ov030_02091664(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3657:
            Progress_SetCurrentEvent(0x16E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3658:
            Progress_SetCurrentEvent(0x16F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3659:
            Progress_SetCurrentEvent(0x170);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x365A:
            Progress_SetCurrentEvent(0x171);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209171c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x365C:
            Progress_SetCurrentEvent(0x18E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x365D:
            Progress_SetCurrentEvent(0x18F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209179c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x365F:
            Progress_SetGate(0x4D);
            Progress_SetCurrentEvent(0x7ED);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3660:
            Progress_SetGate(0x4E);
            Progress_SetCurrentEvent(0x7ED);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3661:
            Progress_SetGate(0x4F);
            Progress_SetCurrentEvent(0x7ED);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02091854(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3663:
            Progress_SetGate(0x50);
            Progress_SetCurrentEvent(0x7EE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3664:
            Progress_SetGate(0x51);
            Progress_SetCurrentEvent(0x7EE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3665:
            Progress_SetGate(0x52);
            Progress_SetCurrentEvent(0x7EE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209190c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3667:
            Progress_SetGate(0x53);
            Progress_SetCurrentEvent(0x7EF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3668:
            Progress_SetGate(0x54);
            Progress_SetCurrentEvent(0x7EF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3669:
            Progress_SetGate(0x55);
            Progress_SetCurrentEvent(0x7EF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020919c4(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36EB:
            Progress_SetCurrentEvent(0x51);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36EC:
            Progress_SetCurrentEvent(0x52);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36ED:
            Progress_SetCurrentEvent(0x53);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36EE:
            Progress_SetCurrentEvent(0x54);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36EF:
            Progress_SetCurrentEvent(0x55);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091a8c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x362C:
            Progress_SetGate(0x3F);
            Progress_SetGate(0x42);
            Progress_SetCurrentEvent(0x419);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362D:
            if (Progress_Check(0x40) == 0) {
                Progress_SetGate(0x40);
                Progress_SetGate(0x43);
                Progress_SetCurrentEvent(0x41A);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x41C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362E:
            if (Progress_Check(0x41) == 0) {
                Progress_SetGate(0x41);
                Progress_SetGate(0x44);
                Progress_SetCurrentEvent(0x41B);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x41C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x362F:
            Progress_SetCurrentEvent(0x41C);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091bc8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
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
            Progress_SetCurrentEvent(0x160);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x161:
        case 0x162:
        case 0x163:
        case 0x164:
            if ((Progress_Check(38) != 0) && (Progress_Check(39) != 0) && (Progress_Check(0x28) != 0) &&
                (Progress_Check(41) != 0))
            {
                return func_ov030_020a9f54(arg0, &func_ov030_02091554, 0x3650, 5, 0x3651, 0x3652, 0x3653, 0x3654, 0x3655);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_0209147c, 0x3650, 4, 0x3651, 0x3652, 0x3653, 0x3654);
        case 0x165:
            Progress_SetGate(0x45);
            break;
        case 0x166:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x510U;
            func_ov030_020af364(0x14);
            return 1;
        case 0x167:
            Progress_SetGate(9);
            gSaveState.unk_20.unk_26BE = 2;
            break;
        case 0x168:
            Progress_SetGate(18);
            func_ov030_020aeab8(8, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x169:
            Progress_SetGate(0x13);
            func_ov030_020aeab8(0, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16A:
            Progress_SetGate(0x14);
            func_ov030_020aeab8(6, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16B:
            Progress_SetGate(0x15);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16C:
            gSaveState.unk_20.unk_26D2 = 0;
            break;
        case 0x16D:
            Progress_SetGate(0x16);
            return func_ov030_020a9f54(arg0, &func_ov030_02091664, 0x3656, 4, 0x3657, 0x3658, 0x3659, 0x365A);
        case 0x16E:
        case 0x16F:
        case 0x170:
        case 0x171:
            arg0->unk_21CF8 = 1;
            break;
        case 0x176:
            Progress_SetGate(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0x177:
            gSaveState.unk_20.unk_26D8 = 2;
            arg0->unk_21CF8            = 1;
            break;
        case 0x178:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x17A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x179:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x17B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x17A:
            Progress_SetGate(0x35);
            gSaveState.unk_20.unk_26D8 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x17E:
            gSaveState.unk_20.unk_26BE = 0;
            gSaveState.unk_20.unk_26D8 = 0;
            break;
        case 0x17F:
            Progress_SetGate(56);
            func_ov030_020aa9f4(arg0, 0x364D, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x180:
        case 0x181:
        case 0x183:
            Progress_ClearGate(0x11);
            if (Progress_Check(58) == 0) {
                Progress_SetGate(58);
                Progress_SetCurrentEvent(0x181);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(EVENT_JOSHUA3_IMPRINT_YELLOW);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_JOSHUA3_IMPRINT_YELLOW:
            if ((Progress_Check(0x3B) != 0) && (Progress_Check(0x61) == 0)) {
                Progress_SetGate(0x61);
                func_ov030_020aac28(arg0, PIN_I_LIVE_FOR_FOOD);
            }
            gSaveState.unk_20.unk_26D8 = 0;
            arg0->unk_21CF8            = 1;
            break;
        case 0x184:
            Progress_SetGate(0xE);
            func_ov030_020aa9f4(arg0, 0x364E, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x185:
            Progress_SetGate(23);
            func_ov030_020aa9f4(arg0, 0x364F, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x416:
            return func_ov030_020aa0c8(arg0, 0xA9, 0x417, 0x418, &func_ov030_02091a8c);
            // case 0x417:
            //     return func_ov030_020aa0c8(arg0, 0xA9, 0x417, 0x418, &func_ov030_02091a8c);
        case 0x419:
        case 0x41A:
        case 0x41B:
            Progress_SetGate(0x11);
            gSaveState.unk_20.unk_26D8 = 2;
            arg0->unk_21CF8            = 1;
            break;
        case 0x188:
            Progress_SetGate(15);
            arg0->unk_21CF8 = 1;
            break;
        case 0x18A:
            func_ov030_020aa9f4(arg0, 0x364C, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x18D:
            gSaveState.unk_20.unk_3124 = 0x1A;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x18E:
            Progress_SetGate(0x49);
        case 0x192:
            return func_ov030_020a9f54(arg0, &func_ov030_0209171c, 0x365B, 2, 0x365C, 0x365D);
        case 0x18F:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x190;
            gSaveState.unk_20.unk_24C2 = 0x191;
            gSaveState.unk_20.unk_341C = 0x20;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x190:
            if (Progress_Check(0x4A) == 0) {
                Progress_SetGate(0x4A);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_CUSTOM);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
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
            if (Progress_Check(0x45) != 0) {
                Progress_SetCurrentEvent(0x7DA);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7DA:
            Progress_SetGate(0x59);
            if (func_ov030_020aaad8() == 10) {
                Progress_SetCurrentEvent(EVENT_JOSHUA3_SCRAMBLE_NP_TREND);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case EVENT_JOSHUA3_SCRAMBLE_NP_TREND:
            Progress_SetGate(4);
            Progress_SetGate(0x5A);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_FLUFFY_COTTON);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            gSaveState.unk_20.unk_2648 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7DC:
            if (Progress_Check(0x47) != 0) {
                Progress_SetCurrentEvent(0x7DE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7DD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7DD:
            if (Progress_Check(0x46) == 0) {
                Progress_SetGate(0x46);
                gSaveState.unk_20.unk_30FC = 0;
                gSaveState.unk_20.unk_30FE = 1;
                gSaveState.unk_20.unk_3100 = 12;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x7DE:
            Progress_SetGate(5);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            func_ov030_020aac28(arg0, PIN_KONOHANA_SAKUYA);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_PHONE_CAMERA);
            gSaveState.unk_20.unk_2666 = 4;
            arg0->unk_21CF8            = 1;
        case 0x7DF:
            Progress_SetCurrentEvent(0x7E0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7E0:
            Progress_SetGate(0x48);
            if (Inventory_HasRequiredQuantity(0x1C, 1, 0) != 0) {
                Progress_SetCurrentEvent(0x7E1);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7E1:
            Progress_SetGate(6);
            Progress_SetGate(0x5C);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            gSaveState.unk_20.unk_266A = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7E2:
            Progress_SetCurrentEvent(0x7E3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7E3:
            Progress_SetGate(0x5E);
            if (Inventory_HasRequiredQuantity(56, 1, 0) != 0) {
                Progress_SetCurrentEvent(0x7E4);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7E4:
            Progress_SetGate(7);
            Progress_SetGate(0x5F);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_HAMBURGER);
            gSaveState.unk_20.unk_2678 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7E5:
            if (Progress_Check(0x45) != 0) {
                Progress_SetCurrentEvent(0x7E7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7E6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7E7:
            if (Progress_Check(0x4C) == 0) {
                Progress_SetGate(0x4C);
                Progress_SetCurrentEvent(0x7E8);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7E9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7E9:
        case 0x7E8:
            Progress_SetCurrentEvent(0x7EA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7EA:
            return func_ov030_020a9f54(arg0, &func_ov030_0209179c, 0x365E, 3, 0x365F, 0x3660, 0x3661);
        case 0x7ED:
            Progress_SetCurrentEvent(0x7EB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7EB:
            return func_ov030_020a9f54(arg0, &func_ov030_02091854, 0x3662, 3, 0x3663, 0x3664, 0x3665);
        case 0x7EE:
            Progress_SetCurrentEvent(0x7EC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7EC:
            return func_ov030_020a9f54(arg0, &func_ov030_0209190c, 0x3666, 3, 0x3667, 0x3668, 0x3669);
        case 0x7EF:
            if ((Progress_Check(0x4D) != 0) && (Progress_Check(0x52) != 0) && (Progress_Check(0x54) != 0)) {
                Progress_SetCurrentEvent(EVENT_JOSHUA3_SCRAMBLE_REAPER);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7F1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7F1:
            Progress_ClearGate(0x4D);
            Progress_ClearGate(0x4E);
            Progress_ClearGate(0x4F);
            Progress_ClearGate(0x50);
            Progress_ClearGate(0x51);
            Progress_ClearGate(0x52);
            Progress_ClearGate(0x53);
            Progress_ClearGate(0x54);
            Progress_ClearGate(0x55);
            break;
        case EVENT_JOSHUA3_SCRAMBLE_REAPER:
            Progress_SetGate(8);
            Progress_SetGate(0x62);
            gSaveState.unk_20.unk_264C = 4;
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_COZY_DOWN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x510:
            Progress_SetGate(0x56);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_OCTO_SQUEEZE);
            break;
        case 0x511:
            Progress_SetGate(0x57);
            func_ov030_020aac28(arg0, PIN_CRACKLE_POP_BARRIER);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case 0x512:
            Progress_SetGate(0x58);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_JOSHUA3_SECRET_BOX_CONCERT_STAGE:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_PIRATE_HAT, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_PIRATE_HAT);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02092c4c(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24BE          = 2;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8197;
    func_ov030_020c26bc(4);
    gSaveState.unk_20.unk_264E = 0;
    gSaveState.unk_20.unk_2656 = 0;
    gSaveState.unk_20.unk_2654 = 0;
    gSaveState.unk_20.unk_2658 = 0;
    gSaveState.unk_20.unk_2676 = 0;
    gSaveState.unk_20.unk_2670 = 0;
    gSaveState.unk_20.unk_3124 = 0x41;
}

s32 func_ov030_02092d50(ProgressObject* arg0) {
    s16 var_r0;

    if ((Progress_Check(56) != 0) && (Progress_Check(2) == 0)) {
        if (Progress_Check(61) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) != 0) {
                Progress_SetGate(61);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) == 0) {
            Progress_SetGate(0x3E);
            Progress_ClearGate(61);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(11) != 0) && (Progress_Check(45) == 0)) {
            Progress_SetGate(45);
            Progress_BeginScriptedEvent(arg0, 0x1A7, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 2) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) &&
            (Progress_Check(12) != 0) && (Progress_Check(5) == 0))
        {
            Progress_SetGate(5);
            Progress_BeginScriptedEvent(arg0, 0x1AC, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 5) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 10) && (Progress_Check(0x3B) != 0) && (Progress_Check(0x3C) == 0)) {
            Progress_SetGate(0x3C);
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 2) && (gSaveState.unk_20.unk_3102 >= 4) && (Progress_Check(0x39) != 0) &&
        (Progress_Check(58) == 0))
    {
        Progress_SetGate(58);
    }
    if (gSaveState.unk_20.unk_2458 == 6) {
        if (Progress_Check(39) == 0) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x19A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (Progress_Check(9) == 0)) {
            Progress_SetGate(9);
            Progress_BeginScriptedEvent(arg0, 0x19B, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (Progress_Check(10) != 0) && (Progress_Check(0x28) == 0) &&
            (Progress_Check(15) == 0))
        {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x1A0, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) == 0) && (Progress_Check(10) != 0) && (Progress_Check(0x28) != 0)) {
            Progress_ClearGate(0x28);
        }
        if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0)) && (Progress_Check(10) != 0) &&
            (Progress_Check(41) == 0) && (Progress_Check(11) == 0))
        {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x1A1, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 8) == 0) && (func_ov030_020848e4(arg0, 9) == 0) && (Progress_Check(10) != 0) &&
            (Progress_Check(41) != 0))
        {
            Progress_ClearGate(41);
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(0x35) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(0x35) != 0) && (Progress_Check(0x37) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (Progress_Check(0x37) != 0)) {
            Progress_ClearGate(0x37);
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0) && (Progress_Check(11) != 0) && (Progress_Check(47) == 0)) {
        Progress_SetGate(47);
        Progress_BeginScriptedEvent(arg0, 0x1AD, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 7) {
        if (Progress_Check(0x22) == 0) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x1D0, 0);
            return 1;
        }
        if ((Progress_Check(0x34) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x1D1, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 8) && (Progress_Check(11) != 0) && (Progress_Check(33) == 0)) {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 0x1B2, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 9) {
        if (Progress_Check(37) == 0) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x1D3, 0);
            return 1;
        }
        if ((func_ov030_020848a4(0x96, 0x15E, 0x190, 0x186) != 0) && (Progress_Check(37) != 0) && (Progress_Check(6) == 0)) {
            Progress_SetGate(6);
            Progress_BeginScriptedEvent(arg0, 0x81D4, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 12) != 0) && (Progress_Check(7) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x1D6, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 12) == 0) && (Progress_Check(7) != 0) && (Progress_Check(38) != 0)) {
            Progress_ClearGate(38);
        }
        if ((func_ov030_020aed9c(0x3797) != 0) && (Progress_Check(8) == 0)) {
            Progress_SetGate(8);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x81D7;
            gSaveState.unk_20.unk_3124          = 0x1B;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 8) && (Progress_Check(11) != 0) && (Progress_Check(33) == 0)) {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 0x1B2, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 13) && (Progress_Check(0x32) == 0)) {
        Progress_SetGate(0x32);
        Progress_BeginScriptedEvent(arg0, 0x1CC, 0);
        return 1;
    }
    if ((gSaveState.unk_20.unk_2458 == 0x10) && (func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(0xE) != 0) &&
        (Progress_Check(0x33) == 0))
    {
        Progress_SetGate(0x33);
        Progress_BeginScriptedEvent(arg0, 0x41CD, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(0x35) == 0) {
            Progress_SetGate(0x35);
            Progress_SetGate(0x37);
            Progress_BeginScriptedEvent(arg0, 0x1D8, 0);
            return 1;
        } else if ((Progress_Check(0x35) != 0) && (Progress_Check(0x37) == 0)) {
            Progress_SetGate(0x37);
            Progress_BeginScriptedEvent(arg0, 0x1DD, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020936b8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366B:
            Progress_SetCurrentEvent(0x19D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x366C:
            Progress_SetCurrentEvent(0x19E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093738(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            Progress_SetGate(42);
            Progress_SetCurrentEvent(0x1A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x366F:
            Progress_SetGate(43);
            Progress_SetCurrentEvent(0x1A4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020937c4(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            Progress_SetGate(42);
            Progress_SetCurrentEvent(0x1A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x366F:
            Progress_SetGate(43);
            Progress_SetCurrentEvent(0x1A4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3670:
            Progress_SetGate(0x2C);
            Progress_SetCurrentEvent(0x1A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093880(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            Progress_SetGate(42);
            Progress_SetCurrentEvent(0x1A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x366F:
            Progress_SetGate(43);
            Progress_SetCurrentEvent(0x1A4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3670:
            Progress_SetGate(0x2C);
            Progress_SetCurrentEvent(0x1A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3671:
            Progress_SetGate(11);
            Progress_SetCurrentEvent(0x1A6);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02093958(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3673:
            Progress_SetCurrentEvent(0x1A9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3674:
            if (Progress_Check(0xE) != 0) {
                Progress_SetGate(12);
                Progress_SetCurrentEvent(0x1AB);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x1AA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093a0c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3676:
            Progress_SetGate(0x30);
            Progress_SetCurrentEvent(0x1AF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3677:
            Progress_SetGate(0x31);
            Progress_SetCurrentEvent(0x1B0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093a98(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3679:
            Progress_SetCurrentEvent(0x1B5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x367A:
            Progress_SetCurrentEvent(0x1B6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x367B:
            Progress_SetCurrentEvent(0x1B7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093b40(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x367E:
            Progress_SetCurrentEvent(0x1BB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x367F:
            Progress_SetCurrentEvent(0x1BC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3680:
            Progress_SetCurrentEvent(0x1BD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093be0(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3682:
            Progress_SetCurrentEvent(0x1BF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3683:
            Progress_SetCurrentEvent(0x1C0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3684:
            Progress_SetCurrentEvent(0x1BF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093c80(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3686:
            Progress_SetCurrentEvent(0x1C5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3687:
            Progress_SetCurrentEvent(0x1C5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3688:
            Progress_SetCurrentEvent(0x1C5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3689:
            Progress_SetCurrentEvent(0x1C6);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02093d38(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x368B:
            Progress_SetCurrentEvent(0x1D9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x368C:
            Progress_SetCurrentEvent(0x1DA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093db8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x197:
            func_ov030_020aeaa0(6, 2);
            func_ov030_020aeaa0(9, 2);
            func_ov030_020aeaa0(13, 2);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x198);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x198:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x199);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x199:
            Progress_SetGate(0x20);
            break;
        case 0x19B:
        case 0x19C:
            Progress_SetGate(9);
            return func_ov030_020a9f54(arg0, func_ov030_020936b8, 0x366A, 2, 0x366B, 0x366C);
        case 0x19D:
        case 0x19E:
            gSaveState.unk_20.unk_265C = 0;
            arg0->unk_21CF8            = 1;
            break;
        case 0x19F:
            Progress_SetGate(10);
            gSaveState.unk_20.unk_2658 = 2;
            gSaveState.unk_20.unk_265A = 2;
            ProgressReward_GrantPin(arg0, PIN_5_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1A2:
        case 0x1A3:
        case 0x1A4:
        case 0x1A5:
            if ((Progress_Check(42) != 0) && (Progress_Check(43) != 0) && (Progress_Check(0x2C) != 0)) {
                return func_ov030_020a9f54(arg0, func_ov030_02093880, 0x366D, 4, 0x366E, 0x366F, 0x3670, 0x3671);
            }
            if (Progress_Check(43) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_020937c4, 0x366D, 3, 0x366E, 0x366F, 0x3670);
            }
            return func_ov030_020a9f54(arg0, func_ov030_02093738, 0x366D, 2, 0x366E, 0x366F);
        case 0x1A6:
            gSaveState.unk_20.unk_2650 = 4;
            gSaveState.unk_20.unk_2658 = 4;
            gSaveState.unk_20.unk_265A = 4;
            break;
        case 0x1A8:
            Progress_SetGate(46);
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
            if ((Progress_Check(0x30) != 0) && (Progress_Check(0x31) != 0)) {
                Progress_SetCurrentEvent(0x1B1);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            return func_ov030_020a9f54(arg0, func_ov030_02093a0c, 0x3675, 2, 0x3676, 0x3677);
        case 0x1B1:
            Progress_SetGate(13);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1B2:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_GOLEM);
            break;
        case 0x1B3:
            Progress_SetGate(0xE);
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
            Progress_SetCurrentEvent(0x1BA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1BA:
            return func_ov030_020a9f54(arg0, func_ov030_02093b40, 0x367D, 3, 0x367E, 0x367F, 0x3680);
        case 0x1BD:
        case 0x1BF:
            Progress_SetCurrentEvent(0x1BF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1BE:
            return func_ov030_020a9f54(arg0, func_ov030_02093be0, 0x3681, 3, 0x3682, 0x3683, 0x3684);
        case 0x1C0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x1C1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C1:
            Progress_SetCurrentEvent(0x1C2);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C2:
            Progress_SetCurrentEvent(0x1C3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C3:
            Progress_SetCurrentEvent(0x1C4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C4:
        case 0x1C5:
            return func_ov030_020a9f54(arg0, func_ov030_02093c80, 0x3685, 4, 0x3686, 0x3687, 0x3688, 0x3689);
        case 0x1C6:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x1C7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C7:
            Progress_SetCurrentEvent(0x1C8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C8:
            Progress_SetCurrentEvent(0x1C9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1C9:
            Progress_SetCurrentEvent(0x1CA);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1CA:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x1CB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1CB:
            Progress_SetGate(15);
            gSaveState.unk_20.unk_265C = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x1D0:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x851AU;
            func_ov030_020af364(0x15);
            return 1;
        case 0x1D4:
            func_ov030_020aec38(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            func_ov030_020aec1c(9);
            gSaveState.unk_20.unk_2660 = 2;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D5:
            Progress_SetGate(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D7:
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
            func_ov030_020aec38(9);
            gSaveState.unk_20.unk_3124 = 0x1C;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1CC:
            func_ov030_020aeab8(13, 2);
            break;
        case 0x1CD:
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(0x1DE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x1CE:
            Progress_SetGate(0x10);
            func_ov030_020aeab8(6, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1CF:
            Progress_SetGate(0x11);
            func_ov030_020aeab8(1, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D8:
            Progress_SetGate(0x35);
        case 0x1DD:
            return func_ov030_020a9f54(arg0, func_ov030_02093d38, 0x368A, 2, 0x368B, 0x368C);
        case 0x1D9:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_26C0 = 0x1DB;
            gSaveState.unk_20.unk_26C2 = 0x1DC;
            gSaveState.unk_20.unk_341C = 33;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1DB:
            if (Progress_Check(0x36) == 0) {
                Progress_SetGate(0x36);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_WHEEL);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
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
            Progress_SetCurrentEvent(0x7F3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7F3:
            Progress_SetGate(56);
            if (Inventory_HasRequiredQuantity(6, 1, 0) != 0) {
                Progress_SetCurrentEvent(0x7F4);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x7F4:
            Progress_SetGate(2);
            Progress_SetGate(61);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            gSaveState.unk_20.unk_2654 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7F5:
            if (Progress_Check(58) != 0) {
                Progress_SetCurrentEvent(0x7F7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7F6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7F6:
            if (Progress_Check(0x39) == 0) {
                Progress_SetGate(0x39);
                gSaveState.unk_20.unk_3102 = 0;
            }
            break;
        case 0x7F7:
            Progress_SetGate(3);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            gSaveState.unk_20.unk_2658 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7F8:
            if (Progress_Check(0x3C) != 0) {
                Progress_SetCurrentEvent(EVENT_JOSHUA4_HACHIKO_REAPER);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x7F9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7F9:
            if (Progress_Check(0x3B) == 0) {
                Progress_SetGate(0x3B);
                gSaveState.unk_20.unk_30FC = 0;
                gSaveState.unk_20.unk_30FE = 39;
                gSaveState.unk_20.unk_3100 = 5;
            }
            break;
        case EVENT_JOSHUA4_HACHIKO_REAPER:
            Progress_SetGate(4);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_PRETTY_RIBBON);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            gSaveState.unk_20.unk_2656 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x51A:
            Progress_SetGate(0x34);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_LIVE_SLOW_DIE_FAST);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case EVENT_JOSHUA4_SECRET_BOX_CAT_STREET:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SAMURAI_WIG, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_SAMURAI_WIG);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02094b08(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24BE          = 2;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x81E2;
    func_ov030_020c26bc(4);
    gSaveState.unk_20.unk_2676 = 0;
    gSaveState.unk_20.unk_2670 = 0;
    gSaveState.unk_20.unk_3124 = 0x42;
}

s32 func_ov030_02094bfc(ProgressObject* arg0) {
    s16 var_r0;

    if ((Progress_Check(0x2C) != 0) && (Progress_Check(3) == 0)) {
        if (Progress_Check(0x46) == 0) {
            if (func_ov030_020aaad8() == 1) {
                Progress_SetGate(0x46);
            }
        } else if (func_ov030_020aaad8() != 1) {
            Progress_SetGate(0x47);
            Progress_ClearGate(0x46);
        }
    }
    if ((Progress_Check(45) != 0) && (Progress_Check(4) == 0)) {
        if (Progress_Check(0x48) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) != 0) {
                Progress_SetGate(0x48);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) == 0) {
            Progress_SetGate(0x49);
            Progress_ClearGate(0x48);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (Progress_Check(2) == 0)) {
            Progress_SetGate(2);
            Progress_BeginScriptedEvent(arg0, 0x1E4, 0);
            return 1;
        } else if ((Progress_Check(38) != 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x1ED, 0);
            return 1;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 3) && (func_ov030_020848e4(arg0, 56) != 0) && (Progress_Check(0x34) == 0)) {
        Progress_SetGate(0x34);
        Progress_BeginScriptedEvent(arg0, 0x4C, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 6) {
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(0x3F) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(0x3F) != 0) && (Progress_Check(0x43) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (Progress_Check(0x43) != 0)) {
            Progress_ClearGate(0x43);
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 12) && (func_ov030_020848e4(arg0, 15) != 0) && (Progress_Check(5) != 0) &&
        (Progress_Check(33) == 0))
    {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 0x1E7, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 13) && (Progress_Check(37) != 0) && (Progress_Check(38) == 0)) {
        Progress_SetGate(38);
        Progress_BeginScriptedEvent(arg0, 0x1EC, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 15) {
        if ((Progress_Check(0x23) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x1EA, 0);
            return 1;
        } else if ((Progress_Check(0x44) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x1EB, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0xE) {
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if ((var_r0 >= 2) && (Progress_Check(0x32) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
        }
        if ((func_ov030_020848e4(arg0, 18) != 0) && (Progress_Check(8) != 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x1E6, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x41E8, 0);
            return 1;
        } else if ((Progress_Check(0x22) != 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x81E9, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 18) {
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (Progress_Check(38) != 0) && (Progress_Check(0x28) == 0)) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x1EE, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x13) != 0) && (Progress_Check(0x28) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x1EF, 0);
            return 1;
        } else if ((Progress_Check(0x45) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x1F0, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0x13) && (Progress_Check(0x35) == 0)) {
        Progress_SetGate(0x35);
        Progress_BeginScriptedEvent(arg0, 0x4D, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(0x3F) == 0) {
            Progress_SetGate(0x3F);
            Progress_SetGate(0x43);
            Progress_BeginScriptedEvent(arg0, 0x1F2, 0);
            return 1;
        } else if ((Progress_Check(0x3F) != 0) && (Progress_Check(0x43) == 0)) {
            Progress_SetGate(0x43);
            Progress_BeginScriptedEvent(arg0, 0x1FA, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02095364(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x368E:
            Progress_SetGate(0x36);
            Progress_SetCurrentEvent(0x810);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x368F:
            Progress_SetGate(0x37);
            Progress_SetCurrentEvent(0x810);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3690:
            Progress_SetGate(56);
            Progress_SetCurrentEvent(0x810);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095418(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3692:
            Progress_SetGate(0x39);
            Progress_SetCurrentEvent(0x811);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3693:
            Progress_SetGate(58);
            Progress_SetCurrentEvent(0x811);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3694:
            Progress_SetGate(0x3B);
            Progress_SetCurrentEvent(0x811);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020954d0(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3696:
            Progress_SetGate(0x3C);
            Progress_SetCurrentEvent(0x812);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3697:
            Progress_SetGate(61);
            Progress_SetCurrentEvent(0x812);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3698:
            Progress_SetGate(0x3E);
            Progress_SetCurrentEvent(0x812);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095588(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x369A:
            Progress_SetCurrentEvent(0x1F3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x369B:
            Progress_SetCurrentEvent(0x1F4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x369C:
            Progress_SetCurrentEvent(0x1F5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209562c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x369E:
            Progress_SetCurrentEvent(0x4A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x369F:
            Progress_SetCurrentEvent(0x4A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36A0:
            Progress_SetCurrentEvent(0x4A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020956c8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A2:
            Progress_SetCurrentEvent(0x4E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36A3:
            Progress_SetCurrentEvent(0x4F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36A4:
            Progress_SetCurrentEvent(0x4F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095764(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A6:
            Progress_SetCurrentEvent(0x81A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36A7:
            Progress_SetCurrentEvent(0x81B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020957e0(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A9:
            Progress_SetCurrentEvent(0x820);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36AA:
            Progress_SetCurrentEvent(0x821);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209585c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x1E2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x1E3);
            Progress_AdvanceEventScript(arg0);
            return 1;
            break;
        case 0x1E4:
            gSaveState.unk_20.unk_264A = 0;
            gSaveState.unk_20.unk_264E = 0;
            gSaveState.unk_20.unk_2664 = 0;
            gSaveState.unk_20.unk_2662 = 0;
            gSaveState.unk_20.unk_2666 = 0;
            gSaveState.unk_20.unk_2668 = 0;
            gSaveState.unk_20.unk_266A = 0;
            gSaveState.unk_20.unk_266C = 0;
            arg0->unk_21CF8            = 1;
            break;
        case 0x7FE:
            Progress_SetCurrentEvent(0x7FF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x7FF:
            Progress_SetGate(43);
            break;
        case 0x801:
            Progress_SetCurrentEvent(0x802);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x802:
            Progress_SetGate(0x2C);
            if (func_ov030_020aaad8() == 1) {
                Progress_SetCurrentEvent(0x803);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x803:
            Progress_SetGate(3);
            Progress_SetGate(0x46);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            gSaveState.unk_20.unk_264E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x804:
            Progress_SetCurrentEvent(0x805);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x805:
            Progress_SetGate(45);
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) != 0) {
                Progress_SetCurrentEvent(0x806);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x806:
            Progress_SetGate(4);
            Progress_SetGate(0x48);
            func_ov030_020aac28(arg0, PIN_PSYCH_SUPPORT);
            gSaveState.unk_20.unk_2664 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x807:
            Progress_SetCurrentEvent(0x808);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x808:
            Progress_SetGate(46);
            break;
        case 0x80A:
            if (Progress_Check(47) == 0) {
                Progress_SetGate(47);
                Progress_SetCurrentEvent(0x80B);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x80C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x80B:
            Progress_SetGate(47);
        case 0x80C:
            Progress_SetCurrentEvent(0x80D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x80D:
            return func_ov030_020a9f54(arg0, func_ov030_02095364, 0x368D, 3, 0x368E, 0x368F, 0x3690);
        case 0x810:
            Progress_SetCurrentEvent(0x80E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x80E:
            return func_ov030_020a9f54(arg0, func_ov030_02095418, 0x3691, 3, 0x3692, 0x3693, 0x3694);
        case 0x811:
            Progress_SetCurrentEvent(0x80F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x80F:
            return func_ov030_020a9f54(arg0, func_ov030_020954d0, 0x3695, 3, 0x3696, 0x3697, 0x3698);
        case 0x812:
            if ((Progress_Check(0x36) != 0) && (Progress_Check(0x3B) != 0) && (Progress_Check(0x3C) != 0)) {
                Progress_SetCurrentEvent(0x813);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x814);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x814:
            Progress_ClearGate(0x36);
            Progress_ClearGate(0x37);
            Progress_ClearGate(56);
            Progress_ClearGate(0x39);
            Progress_ClearGate(58);
            Progress_ClearGate(0x3B);
            Progress_ClearGate(0x3C);
            Progress_ClearGate(61);
            Progress_ClearGate(0x3E);
            break;
        case 0x813:
            Progress_SetGate(5);
            gSaveState.unk_20.unk_2666 = 4;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
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
            Progress_SetCurrentEvent(0x815);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x816:
            Progress_SetGate(0x30);
            if (func_02023e58(0) != 0) {
                Progress_SetCurrentEvent(0x817);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x817:
            Progress_SetGate(6);
            Progress_SetGate(0x4A);
            gSaveState.unk_20.unk_2668 = 4;
            func_ov030_020aac28(arg0, PIN_STRONG_BODY);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6A9:
            if (func_02023e58(0) != 0) {
                Progress_SetCurrentEvent(0x6A7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x69B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x818:
            Progress_SetCurrentEvent(0x818);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x819:
            Progress_SetGate(0x31);
            return func_ov030_020a9f54(arg0, func_ov030_02095764, 0x36A5, 2, 0x36A6, 0x36A7);
        case 0x81A:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x81C;
            gSaveState.unk_20.unk_24C2 = 0x81B;
            gSaveState.unk_20.unk_341C = 43;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x81D:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_THRIFT);
            Progress_SetGate(7);
            gSaveState.unk_20.unk_266A = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x81C:
            Progress_SetGate(7);
            gSaveState.unk_20.unk_266A = 4;
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(EVENT_JOSHUA5_SECRET_BOX_MIYASHITA_UNDERPASS);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x81E:
            Progress_SetCurrentEvent(0x81F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x81F:
            Progress_SetGate(0x32);
            return func_ov030_020a9f54(arg0, func_ov030_020957e0, 0x36A8, 2, 0x36A9, 0x36AA);
        case 0x820:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x822;
            func_ov030_020af364(0x18);
            return 1;
        case 0x822:
            Progress_SetGate(8);
            gSaveState.unk_20.unk_266C = 2;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x1E5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x1E5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x1E6:
            gSaveState.unk_20.unk_266C = 4;
            gSaveState.unk_20.unk_264A = 4;
            gSaveState.unk_20.unk_2662 = 4;
            arg0->unk_21AD0            = 1;
            arg0->unk_21ACC            = 1;
            arg0->unk_21AFC            = 0x10;
            arg0->unk_21B00            = 0x66;
            arg0->unk_21B08            = 0x1E000;
            arg0->unk_21B0C            = 0xF4000;
            break;
        case 0x1E8:
            func_ov030_020aeaa0(18, 2);
            gSaveState.unk_20.unk_26D2 = 0;
            break;
        case 0x1EA:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8524U;
            func_ov030_020af364(0x16);
            return 1;
        case 0x1EB:
            Progress_SetGate(37);
            break;
        case 0x1EF:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8525U;
            func_ov030_020af364(23);
            return 1;
        case 0x1F0:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x81F1U;
            gSaveState.unk_20.unk_3124          = 0x1D;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F1:
            gSaveState.unk_20.unk_3124 = 0x1E;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1FA:
            return func_ov030_020a9f54(arg0, func_ov030_02095588, 0x3699, 3, 0x369A, 0x369B, 0x369C);
        case 0x4A:
            Progress_SetGate(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4B:
            Progress_SetGate(10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x49:
            return func_ov030_020a9f54(arg0, func_ov030_0209562c, 0x369D, 3, 0x369E, 0x369F, 0x36A0);
        case 0x1F2:
            Progress_SetGate(0x3F);
        case 0x4D:
            return func_ov030_020a9f54(arg0, func_ov030_020956c8, 0x36A1, 3, 0x36A2, 0x36A3, 0x36A4);

        case 0x1F3:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x1F6;
            gSaveState.unk_20.unk_24C2 = 0x1F7;
            gSaveState.unk_20.unk_341C = 0x23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F4:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x1F8;
            gSaveState.unk_20.unk_24C2 = 0x1F9;
            gSaveState.unk_20.unk_341C = 0x22;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F6:
            Progress_SetGate(0x40);
            if ((Progress_Check(0x40) != 0) && (Progress_Check(0x41) != 0) && (Progress_Check(0x42) == 0)) {
                Progress_SetGate(0x42);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_HELLFIRE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 6;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0xA0000;
            arg0->unk_21B0C = 0xDA000;
            break;
        case 0x1F8:
            Progress_SetGate(0x41);
            if ((Progress_Check(0x40) != 0) && (Progress_Check(0x41) != 0) && (Progress_Check(0x42) == 0)) {
                Progress_SetGate(0x42);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_HELLFIRE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
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
            Progress_SetGate(0x44);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_JOSHUA);
            break;
        case 0x525:
            Progress_SetGate(0x45);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            break;
        case EVENT_JOSHUA5_SECRET_BOX_MIYASHITA_UNDERPASS:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_BIKER_GLOVES, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_BIKER_GLOVES);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02096790(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24BE          = 2;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8200;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_264A = 4;
    gSaveState.unk_20.unk_2654 = 4;
    gSaveState.unk_20.unk_2658 = 4;
    gSaveState.unk_20.unk_265C = 4;
    gSaveState.unk_20.unk_2660 = 4;
    gSaveState.unk_20.unk_3124 = 0x43;
}

s32 func_ov030_02096888(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (Progress_Check(56) == 0) {
            Progress_SetGate(56);
            Progress_BeginScriptedEvent(arg0, 0x448, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 1) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x201, 0);
            return 1;
        } else if ((Progress_Check(0x33) != 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x202, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 4) && (Progress_Check(0x22) == 0)) {
        Progress_SetGate(0x22);
        Progress_BeginScriptedEvent(arg0, 0x203, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 2) {
        if (Progress_Check(0x23) == 0) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x204, 0);
            return 1;
        } else if ((Progress_Check(0x34) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x207, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 8) != 0) && (Progress_Check(37) == 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x208, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 6) {
        if (Progress_Check(39) == 0) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x8209, 0);
            return 1;
        }
        if ((Progress_Check(0x35) != 0) && (Progress_Check(3) == 0)) {
            Progress_SetGate(3);
            Progress_BeginScriptedEvent(arg0, 0x20C, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x20D, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(0x2C) == 0) && (Progress_Check(46) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(0x2C) != 0) && (Progress_Check(45) == 0) &&
            (Progress_Check(46) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (Progress_Check(45) != 0) && (Progress_Check(46) == 0)) {
            Progress_ClearGate(45);
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(46) != 0) && (Progress_Check(47) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (Progress_Check(46) != 0) && (Progress_Check(47) != 0)) {
            Progress_ClearGate(47);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 7) {
        if (Progress_Check(4) == 0) {
            Progress_SetGate(4);
            Progress_BeginScriptedEvent(arg0, 0x820E, 0);
            return 1;
        } else if ((Progress_Check(0x36) != 0) && (Progress_Check(5) == 0)) {
            Progress_SetGate(5);
            Progress_BeginScriptedEvent(arg0, 0x211, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 9) {
        if (Progress_Check(42) == 0) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x212, 0);
            return 1;
        } else if ((Progress_Check(0x37) != 0) && (Progress_Check(43) == 0)) {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0x213, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 37) {
        if ((Progress_Check(0x2C) == 0) && (Progress_Check(46) == 0)) {
            Progress_SetGate(0x2C);
            Progress_SetGate(45);
            Progress_BeginScriptedEvent(arg0, 0x215, 0);
            return 1;
        } else if ((Progress_Check(0x2C) != 0) && (Progress_Check(45) == 0) && (Progress_Check(46) == 0)) {
            Progress_SetGate(45);
            Progress_BeginScriptedEvent(arg0, 0x21D, 0);
            return 1;
        } else if ((Progress_Check(46) != 0) && (Progress_Check(47) == 0)) {
            Progress_SetGate(47);
            Progress_BeginScriptedEvent(arg0, 0x222, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02096f2c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36AC:
            Progress_SetCurrentEvent(0x205);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36AD:
            Progress_SetGate(0x24);
            Progress_SetCurrentEvent(0x206);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02096fb4(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36AF:
            Progress_SetCurrentEvent(0x20A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36B0:
            Progress_SetGate(0x28);
            Progress_SetCurrentEvent(0x20B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209703c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36B2:
            Progress_SetCurrentEvent(0x20F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36B3:
            Progress_SetCurrentEvent(0x210);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020970b8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36B5:
            Progress_SetCurrentEvent(0x216U);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36B6:
            Progress_SetCurrentEvent(0x2177CU >> 8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36B7:
            Progress_SetCurrentEvent(0x21FU);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36B8:
            Progress_SetCurrentEvent(0x218U);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209716c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x200:
            gSaveState.unk_20.unk_24BE = 0x224;
            func_ov030_020aec1c(10);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_LEVITATE);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_BLACK_NOISE_SYMBOLS);
            break;
        case 0x201:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x52EU;
            func_ov030_020af364(0x19);
            return 1;
        case 0x204:
            return func_ov030_020a9f54(arg0, &func_ov030_02096f2c, 0x36AB, 2, 0x36AC, 0x36AD);
        case 0x205:
            Progress_SetGate(2);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x52F;
            func_ov030_020af364(0x1C);
            return 1;
        case 0x207:
            func_ov030_020aac28(arg0, PIN_LIVE);
            break;
        case 0x208:
            Progress_SetGate(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x209:
            return func_ov030_020a9f54(arg0, &func_ov030_02096fb4, 0x36AE, 2, 0x36AF, 0x36B0);
        case 0x20A:
        case 0x20B:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8530U;
            func_ov030_020af364(0x1A);
            return 1;
        case 0x20C:
            Progress_SetGate(3);
            func_ov030_020aac28(arg0, PIN_VELOCITY_ATTACK);
            arg0->unk_21CF8 = 1;
            break;
        case 0x20E:
            return func_ov030_020a9f54(arg0, &func_ov030_0209703c, 0x36B1, 2, 0x36B2, 0x36B3);
        case 0x20F:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8531U;
            func_ov030_020af364(0x1D);
            return 1;
        case 0x210:
            Progress_SetGate(5);
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(0x24) != 0) && (Progress_Check(0x28) != 0) &&
                (Progress_Check(1) == 0))
            {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(EVENT_JOSHUA6_SECRET_BOX_SHIBU_Q_HEADS);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x211:
            Progress_SetGate(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x212:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8532U;
            func_ov030_020af364(0x1B);
            return 1;
        case 0x213:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x214);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x214:
            gSaveState.unk_20.unk_3124 = 0x1F;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x215:
            Progress_SetGate(0x2C);
        case 0x21D:
            return func_ov030_020a9f54(arg0, &func_ov030_020970b8, 0x36B4, 3, 0x36B5, 0x36B6, 0x36B8);
        case 0x216:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x219;
            gSaveState.unk_20.unk_24C2 = 0x21A;
            gSaveState.unk_20.unk_341C = 0x23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x217:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x21B;
            gSaveState.unk_20.unk_24C2 = 0x21C;
            gSaveState.unk_20.unk_341C = 0x22;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x219:
            Progress_SetGate(0x30);
            if ((Progress_Check(0x30) != 0) && (Progress_Check(0x31) != 0) && (Progress_Check(46) == 0)) {
                Progress_SetCurrentEvent(0x21E);
                Progress_AdvanceEventScript(arg0);
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
            Progress_SetGate(0x31);
            if ((Progress_Check(0x30) != 0) && (Progress_Check(0x31) != 0) && (Progress_Check(46) == 0)) {
                Progress_SetCurrentEvent(0x21E);
                Progress_AdvanceEventScript(arg0);
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
            Progress_SetGate(46);
            Progress_SetGate(47);
        case 0x222:
            return func_ov030_020a9f54(arg0, &func_ov030_020970b8, 0x36B4, 4, 0x36B5, 0x36B6, 0x36B7, 0x36B8);
        case 0x21F:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x220;
            gSaveState.unk_20.unk_24C2 = 0x221;
            gSaveState.unk_20.unk_341C = 0x24;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x220:
            if (Progress_Check(0x32) == 0) {
                Progress_SetGate(0x32);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_WIND);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_SUNSCORCH);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
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
            Progress_SetGate(0x33);
            func_ov030_020aac28(arg0, PIN_STRONG_N_PROUD);
            break;
        case 0x52F:
            Progress_SetGate(0x34);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x530:
            Progress_SetGate(0x35);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            break;
        case 0x531:
            Progress_SetGate(0x36);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x532:
            Progress_SetGate(0x37);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case EVENT_JOSHUA6_SECRET_BOX_SHIBU_Q_HEADS:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_NINJA_GARB, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_NINJA_GARB);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_020979d0(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8227;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_2648 = 4;
    gSaveState.unk_20.unk_2650 = 4;
    gSaveState.unk_20.unk_2652 = 4;
    gSaveState.unk_20.unk_2674 = 4;
    gSaveState.unk_20.unk_2676 = 4;
    gSaveState.unk_20.unk_2678 = 4;
    gSaveState.unk_20.unk_267A = 4;
    gSaveState.unk_20.unk_3124 = 0x44;
}

s32 func_ov030_02097acc(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 2) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (Progress_Check(0x20) == 0))
        {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x228, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 2) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (Progress_Check(0x20) != 0))
        {
            Progress_ClearGate(0x20);
        }
        if ((Progress_Check(0x24) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            gSaveState.unk_20.unk_3124 = 0x23;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 3) && (Progress_Check(33) == 0)) {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 553, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 23) {
        if (Progress_Check(46) == 0) {
            Progress_SetGate(46);
            Progress_BeginScriptedEvent(arg0, 554, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) != 0) && (Progress_Check(0x31) == 0) && (Progress_Check(47) == 0)) {
            Progress_SetGate(47);
            Progress_BeginScriptedEvent(arg0, 555, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) == 0) && (Progress_Check(0x31) != 0)) {
            Progress_ClearGate(0x31);
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x18) {
        if ((Progress_Check(0x22) == 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 556, 0);
            return 1;
        }
        if ((Progress_Check(2) != 0) && (Progress_Check(0x35) == 0)) {
            Progress_SetGate(0x35);
            Progress_BeginScriptedEvent(arg0, 562, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x19) {
        if ((func_ov030_020848e4(arg0, 0x1C) != 0) && (Progress_Check(4) != 0) && (Progress_Check(0x30) == 0)) {
            Progress_SetGate(0x30);
            Progress_BeginScriptedEvent(arg0, 0x231, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x1C) != 0) && (Progress_Check(0x30) != 0) && (Progress_Check(0x32) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x1C) == 0) && (Progress_Check(0x30) != 0) && (Progress_Check(0x32) != 0)) {
            Progress_ClearGate(0x32);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(0x23) == 0) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x8233, 0);
            return 1;
        }
        if ((Progress_Check(0x34) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x235, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02097f4c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x227:
            gSaveState.unk_20.unk_24BE = 0x238;
            func_ov030_020aec1c(10);
            break;
        case 0x229:
            gSaveState.unk_20.unk_24BE = 569;
            func_ov030_020aec1c(10);
            break;
        case 0x22D:
            Progress_SetGate(6);
            arg0->unk_21CF8 = 1;
            break;
        case 0x22E:
            Progress_SetGate(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x231:
            Progress_SetGate(50);
            break;
        case 0x233:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8234;
            gSaveState.unk_20.unk_3124          = 33;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x234:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x841F;
            gSaveState.unk_20.unk_3124          = 0x20;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x41F:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8538;
            func_ov030_020af364(30);
            return 1;
        case 0x235:
            func_ov030_020aec38(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            gSaveState.unk_20.unk_24B4                 = 0;
            gSaveState.unk_20.unk_24B8                 = 1;
            gSaveState.unk_20.currentStoryEvent        = 0x8236;
            gSaveState.unk_20.unk_3124                 = 0x22;
            gSaveState.unk_20.unk_3128                 = 0;
            arg0->unk_21630                            = 4;
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
            Progress_SetGate(0x31);
            gSaveState.unk_20.unk_267A = 2;
            break;
        case 0x22B:
            gSaveState.unk_20.unk_267A = 4;
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(567);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            func_ov030_020aac28(arg0, PIN_FROZEN_COOL);
            break;
        case 0x826:
            if ((Progress_Check(0x28) != 0) && (Progress_Check(41) != 0) && (Progress_Check(42) != 0)) {
                Progress_SetCurrentEvent(0x828);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x827);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x827:
            if (Progress_Check(3) == 0) {
                Progress_SetGate(3);
                gSaveState.unk_20.unk_310C = &data_ov030_020d9c0c;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
            }
            break;
        case 0x828:
            Progress_SetGate(2);
            gSaveState.unk_20.unk_267C = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x829:
            if ((Progress_Check(43) != 0) && (Progress_Check(0x2C) != 0) && (Progress_Check(45) != 0)) {
                Progress_SetCurrentEvent(0x82B);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x82A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x82A:
            if (Progress_Check(5) == 0) {
                Progress_SetGate(5);
                gSaveState.unk_20.unk_310C = &data_ov030_020d9b6c;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
            }
            break;
        case 0x82B:
            Progress_SetGate(4);
            gSaveState.unk_20.unk_2680 = 2;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x538:
            Progress_SetGate(0x34);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_CANDLE_SERVICE);
            break;
        case EVENT_JOSHUA7_SECRET_BOX_CONCERT_STAGE:
            Progress_SetGate(1);
            func_ov030_020aac28(arg0, PIN_FROZEN_COOL);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_BOYS_UNIFORM_WITH_BLAZER, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_BOYS_UNIFORM_WITH_BLAZER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02098500(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_24C0          = 4;
    gSaveState.unk_20.unk_24C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x823B;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_264C = 4;
    gSaveState.unk_20.unk_2656 = 4;
    gSaveState.unk_20.unk_266E = 4;
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
    gSaveState.unk_20.unk_3124                 = 0x45;
}

s32 func_ov030_02098604(ProgressObject* arg0) {
    if ((gSaveState.unk_20.unk_2458 == 1) && (func_ov030_020848e4(arg0, 2) != 0) && (Progress_Check(0x20) == 0)) {
        Progress_SetGate(0x20);
        Progress_BeginScriptedEvent(arg0, 0x823C, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 3) && (Progress_Check(39) != 0) && (Progress_Check(40) == 0)) {
        Progress_SetGate(40);
        Progress_BeginScriptedEvent(arg0, 0x24F, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 5) {
        if (Progress_Check(33) == 0) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x823D, 0);
            return 1;
        } else if ((Progress_Check(41) != 0) && (Progress_Check(34) == 0)) {
            Progress_SetGate(34);
            Progress_BeginScriptedEvent(arg0, 576, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x11) {
        if (Progress_Check(35) == 0) {
            Progress_SetGate(35);
            Progress_BeginScriptedEvent(arg0, 0x8241, 0);
            return 1;
        } else if ((Progress_Check(35) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x449, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (Progress_Check(38) == 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 580, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 7) == 0) {
            Progress_ClearGate(0x24);
        }
        if ((func_ov030_020848e4(arg0, 0x13) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 581, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 18) {
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 582, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x13) != 0) && (Progress_Check(38) != 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x8248, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02098930(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36BA:
            Progress_SetCurrentEvent(0x24A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36BB:
            Progress_SetCurrentEvent(0x24B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020989b0(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36BD:
            Progress_SetCurrentEvent(0x24D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36BE:
            Progress_SetCurrentEvent(0x24E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02098a30(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x23B:
            gSaveState.unk_20.unk_24BE = 594;
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
            gSaveState.unk_20.unk_24BE = 0x253;
            func_ov030_020aec1c(10);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x823E;
            func_ov030_020af364(0x1F);
            return 1;
        case 0x23E:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x823F;
            gSaveState.unk_20.unk_3124          = 0x24;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x23F:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8542;
            func_ov030_020aec1c(4);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_BEAT;
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
            Progress_SetCurrentEvent(0x242);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x242:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x243);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x243:
            gSaveState.unk_20.unk_2656 = 2;
            break;
        case 0x246:
            gSaveState.unk_20.unk_2656 = 4;
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
            if (Progress_Check(39) != 0) {
                Progress_SetCurrentEvent(2093);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x82D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x249);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x249:
            return func_ov030_020a9f54(arg0, func_ov030_02098930, 0x36B9, 2, 0x36BA, 0x36BB);
        case 0x24A:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x24C;
            func_ov030_020af364(33);
            return 1;
        case 0x24C:
            return func_ov030_020a9f54(arg0, func_ov030_020989b0, 0x36BC, 2, 0x36BD, 0x36BE);
        case 0x24D:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x24C;
            func_ov030_020af364(33);
            return 1;
        case 0x24E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x82E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x82E:
            Progress_SetGate(1);
            gSaveState.unk_20.unk_2648 = 4;
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_HOT_GAZE);
            arg0->unk_21CF8 = 1;
        case 0x24F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x250);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x250:
            gSaveState.unk_20.unk_3124 = 37;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_BEAT1_HACHIKO:
            Progress_SetGate(41);
            if (Inventory_HasRequiredQuantity(ITEM_STICKER_AIR_TIME, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_STICKER_AIR_TIME, 0);
            }
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SUIT_CARDS);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_BEAT);
            break;
        case EVENT_BEAT1_SECRET_BOX_HACHIKO:
            Progress_SetGate(2);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_GIRLS_UNIFORM_WITH_BLAZER, 1, 0)) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_GIRLS_UNIFORM_WITH_BLAZER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02099010(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_26C0          = 4;
    gSaveState.unk_20.unk_26C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8255;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_2648 = 4;
    gSaveState.unk_20.unk_264C = 4;
    gSaveState.unk_20.unk_2656 = 4;
    gSaveState.unk_20.unk_266E = 4;
    gSaveState.unk_20.unk_2674 = 4;
    gSaveState.unk_20.unk_2678 = 4;
    gSaveState.unk_20.unk_267A = 4;
    gSaveState.unk_20.unk_3124 = 0x46;
}

s32 func_ov030_0209910c(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (Progress_Check(0x20) == 0) &&
            (Progress_Check(33) == 0))
        {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x257, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 3) == 0) && (Progress_Check(0x20) != 0) &&
                   (Progress_Check(33) == 0))
        {
            Progress_ClearGate(0x20);
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x259, 0);
            return 1;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 3) && (func_ov030_020848e4(arg0, 4) != 0) && (Progress_Check(33) == 0)) {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 0x258, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if (Progress_Check(0x23) == 0) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x25A, 0);
            return 1;
        } else if ((Progress_Check(42) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x25B, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x47) != 0) && (Progress_Check(GATE_GAME_CLEARED) != 0) &&
                   (Progress_Check(1) == 0))
        {
            Progress_SetGate(1);
            Progress_BeginScriptedEvent(arg0, 0x4266, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x16) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 605, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if (Progress_Check(38) == 0) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x25E, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) != 0) && (Progress_Check(39) == 0) && (Progress_Check(2) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x25F, 0);
            return 1;
        } else if ((Progress_Check(43) != 0) && (Progress_Check(2) == 0)) {
            Progress_SetGate(2);
            Progress_BeginScriptedEvent(arg0, 0x8261, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 23) {
        if (Progress_Check(0x28) == 0) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x263, 0);
            return 1;
        } else if ((Progress_Check(0x2C) != 0) && (Progress_Check(41) == 0)) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x265, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02099514(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x255:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8256U;
            gSaveState.unk_20.unk_3124          = 38;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x258:
            gSaveState.unk_20.unk_2650 = 4;
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
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x54CU;
            func_ov030_020af364(0x22);
            return 1;
        case 0x25B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            gSaveState.unk_20.unk_3100 = 0x14;
            Progress_SetCurrentEvent(0x25C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x25F:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x54DU;
            func_ov030_020af364(0x23);
            return 1;
        case 0x261:
            arg0->unk_21CF8 = 1;
            break;
        case 0x264:
            Progress_SetGate(3);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = EVENT_BEAT2_CONCERT_STAGE;
            func_ov030_020af364(0x24);
            return 1;
        case 0x265:
            gSaveState.unk_20.unk_3124 = 39;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x54C:
            Progress_SetGate(42);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_KEWL_LINE);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x54D:
            Progress_SetGate(43);
            func_ov030_020aac28(arg0, PIN_SELF_FOUND_OTHERS_LOST);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case EVENT_BEAT2_CONCERT_STAGE:
            Progress_SetGate(0x2C);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_MICROCOSMIC_PULL);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BLOCK_BEAT);
            break;
        case EVENT_BEAT2_SECRET_BOX_DOGENZAKA:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FALSE_TEETH, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_FALSE_TEETH);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_02099858(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_26BE          = 2;
    gSaveState.unk_20.unk_26C0          = 4;
    gSaveState.unk_20.unk_26C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x826A;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_2648 = 2;
    gSaveState.unk_20.unk_264A = 2;
    gSaveState.unk_20.unk_264C = 2;
    gSaveState.unk_20.unk_2656 = 4;
    gSaveState.unk_20.unk_266E = 0;
    gSaveState.unk_20.unk_2650 = 4;
    gSaveState.unk_20.unk_2652 = 4;
    gSaveState.unk_20.unk_2674 = 4;
    gSaveState.unk_20.unk_2678 = 4;
    gSaveState.unk_20.unk_267A = 4;
    gSaveState.unk_20.unk_2654 = 4;
    gSaveState.unk_20.unk_2658 = 4;
    gSaveState.unk_20.unk_265A = 4;
    gSaveState.unk_20.unk_265E = 4;
    gSaveState.unk_20.unk_3124 = 0x47;
    gSaveState.unk_20.unk_3100 = 0x14;
}

s32 func_ov030_02099990(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((func_ov030_02084860(0x157, 0xD6, 0x32) == 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x26B, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
                    (func_ov030_020848e4(arg0, 2) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) &&
                   (Progress_Check(33) == 0) && (Progress_Check(3) == 0))
        {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x26C, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
                   (func_ov030_020848e4(arg0, 2) == 0) && (func_ov030_020848e4(arg0, 3) == 0) && (Progress_Check(33) != 0) &&
                   (Progress_Check(3) == 0))
        {
            Progress_ClearGate(33);
        }
        if ((func_ov030_020848e4(arg0, 3) != 0) && (Progress_Check(3) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x26D, 0);
            return 1;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 3) && (Progress_Check(38) == 0) && (Progress_Check(0x24) == 0)) {
        Progress_SetGate(38);
        Progress_BeginScriptedEvent(arg0, 0x272, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 4) && (Progress_Check(0x24) == 0)) {
        Progress_SetGate(0x24);
        Progress_BeginScriptedEvent(arg0, 0x26F, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 5) && (Progress_Check(37) == 0) && (Progress_Check(0x24) == 0)) {
        Progress_SetGate(37);
        Progress_BeginScriptedEvent(arg0, 0x270, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 2) && (Progress_Check(5) == 0)) {
        Progress_SetGate(5);
        Progress_BeginScriptedEvent(arg0, 0x273, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 6) {
        if (Progress_Check(39) == 0) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x274, 0);
            return 1;
        } else if ((Progress_Check(9) != 0) && (Progress_Check(0x28) == 0)) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x27B, 0);
            return 1;
        } else if ((Progress_Check(0x28) != 0) && (Progress_Check(0x42) == 0)) {
            Progress_SetGate(0x42);
            Progress_BeginScriptedEvent(arg0, 0x44A, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0)) &&
                   (Progress_Check(0x28) != 0) && (Progress_Check(41) == 0))
        {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x280, 0);
            return 1;
        } else if ((Progress_Check(0x40) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x281, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(61) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (Progress_Check(61) != 0) && (Progress_Check(0x3F) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (Progress_Check(0x3F) != 0)) {
            Progress_ClearGate(0x3F);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(61) == 0) {
            Progress_SetGate(GATE_GAME_CLEARED);
            Progress_SetGate(0x3F);
            Progress_BeginScriptedEvent(arg0, 642, 0);
            return 1;
        } else if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(0x3F) == 0)) {
            Progress_SetGate(0x3F);
            Progress_BeginScriptedEvent(arg0, 0x287, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02099f8c(ProgressObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (Progress_Check(43) != 0) {
                func_ov030_020a9db8(arg0, 0x4B5);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x4B4);
            return 0;
        case 1:
            Progress_SetCurrentEvent(0x4A6);
            return 1;
        case 0:
            if (Progress_Check(46) == 0) {
                Progress_SetGate(46);
                Progress_SetCurrentEvent(0x4A3);
                return 1;
            }
            if (Progress_Check(47) == 0) {
                Progress_SetGate(47);
                Progress_SetCurrentEvent(0x4A4);
                return 1;
            }
            Progress_ClearGate(47);
            Progress_SetCurrentEvent(0x4A5);
            return 1;
        case 2:
            if (Progress_Check(0x2C) == 0) {
                Progress_SetGate(0x2C);
                Progress_SetCurrentEvent(0x4A7);
                return 1;
            }
            if (Progress_Check(45) == 0) {
                Progress_SetGate(45);
                Progress_SetCurrentEvent(0x4A8);
                return 1;
            }
            Progress_ClearGate(45);
            Progress_SetCurrentEvent(0x4A9);
            return 1;
        case 3:
            Progress_SetCurrentEvent(0x4AA);
            return 1;
        case 4:
            Progress_SetCurrentEvent(0x4AB);
            return 1;
        case 5:
            Progress_SetCurrentEvent(0x4AC);
            return 1;
        case 6:
            Progress_SetCurrentEvent(0x4AD);
            return 1;
        case 7:
            Progress_SetCurrentEvent(0x4AE);
            return 1;
        case 8:
            Progress_SetCurrentEvent(0x4AF);
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

s32 func_ov030_0209a1e4(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C0:
            Progress_SetGate(7);
            Progress_SetCurrentEvent(0x276);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36C1:
            Progress_SetGate(8);
            Progress_SetCurrentEvent(EVENT_BEAT3_TIPSY_TOSE_HALL_REAPER_CREEPER);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a270(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C6:
            Progress_SetGate(0x31);
            Progress_SetCurrentEvent(0x835);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36C7:
            Progress_SetGate(0x32);
            Progress_SetCurrentEvent(0x835);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36C8:
            Progress_SetGate(0x33);
            Progress_SetCurrentEvent(0x835);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a328(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36CA:
            Progress_SetGate(0x34);
            Progress_SetCurrentEvent(0x836);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36CB:
            Progress_SetGate(0x35);
            Progress_SetCurrentEvent(0x836);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36CC:
            Progress_SetGate(0x36);
            Progress_SetCurrentEvent(0x836);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36CD:
            Progress_SetGate(0x37);
            Progress_SetCurrentEvent(0x836);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209a3f8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36CF:
            Progress_SetGate(56);
            Progress_SetCurrentEvent(0x837);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D0:
            Progress_SetGate(0x39);
            Progress_SetCurrentEvent(0x837);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D1:
            Progress_SetGate(58);
            Progress_SetCurrentEvent(0x837);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D2:
            Progress_SetGate(0x3B);
            Progress_SetCurrentEvent(0x837);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D3:
            Progress_SetGate(0x3C);
            Progress_SetCurrentEvent(0x837);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a4ec(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C3:
            Progress_SetCurrentEvent(0x283);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36C4:
            Progress_SetCurrentEvent(0x284);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a564(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x26E:
            Progress_SetGate(3);
            gSaveState.unk_20.unk_2648 = 4;
            gSaveState.unk_20.unk_264C = 4;
            gSaveState.unk_20.unk_264A = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x271:
            Progress_SetGate(4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x275:
            Progress_SetGate(6);
            return func_ov030_020a9f54(arg0, func_ov030_0209a1e4, 0x36BF, 2, 0x36C0, 0x36C1);
        case 0x4B4:
            Progress_SetGate(43);
        case 0x4B5:
            Progress_ClearGate(46);
            Progress_ClearGate(47);
            Progress_ClearGate(0x2C);
            Progress_ClearGate(45);
        case 8:
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
        case 0x27C:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x27D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x4B0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x278);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x4B1:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x277);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x277:
            Progress_SetGate(9);
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(EVENT_BEAT3_SECRET_BOX_TISPY_TOSE_HALL);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x278:
        case 0x279:
            Progress_SetGate(9);
            arg0->unk_21CF8 = 1;
            break;
        case EVENT_BEAT3_TIPSY_TOSE_HALL_REAPER_CREEPER:
            Progress_SetGate(9);
            if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10);
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x27B:
            Progress_SetCurrentEvent(0x27C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x27D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x27E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x27E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(EVENT_BEAT3_TIPSY_TOSE_HALL);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x4B2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x279);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_BEAT3_TIPSY_TOSE_HALL:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_AIR_CANCEL);
            break;
        case 0x280:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8556;
            func_ov030_020af364(37);
            return 1;
        case 0x281:
            gSaveState.unk_20.unk_3124 = 0x28;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x282:
            Progress_SetGate(61);
        case 0x287:
            return func_ov030_020a9f54(arg0, func_ov030_0209a4ec, 0x36C2, 2, 0x36C3, 0x36C4);
        case 0x283:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = ITEM_CD_TRACK01;
            gSaveState.unk_20.unk_24C2 = 0x286;
            gSaveState.unk_20.unk_341C = 37;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x285:
            if (Progress_Check(0x3E) == 0) {
                Progress_SetGate(0x3E);
                func_ov030_020aac28(arg0, PIN_TIN_PIN_ARTIST);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
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
            if (Progress_Check(0x30) == 0) {
                Progress_SetGate(0x30);
                Progress_SetCurrentEvent(0x830);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x831);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x830:
        case 0x831:
            Progress_SetCurrentEvent(0x832);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x832:
            return func_ov030_020a9f54(arg0, func_ov030_0209a270, 0x36C5, 3, 0x36C6, 0x36C7, 0x36C8);
        case 0x835:
            Progress_SetCurrentEvent(0x832);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x833:
            return func_ov030_020a9f54(arg0, func_ov030_0209a328, 0x36C9, 4, 0x36CA, 0x36CB, 0x36CC, 0x36CD);
        case 0x836:
            Progress_SetCurrentEvent(0x833);
            Progress_AdvanceEventScript(arg0);
            return 1;

        case 0x834:
            return func_ov030_020a9f54(arg0, func_ov030_0209a3f8, 0x36CE, 5, 0x36CF, 0x36D0, 0x36D1, 0x36D2, 0x36D3);
        case 0x837:
            if ((Progress_Check(0x31) != 0) && (Progress_Check(0x35) != 0) && (Progress_Check(0x39) != 0)) {
                Progress_SetCurrentEvent(0x838);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x839);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x839:
            Progress_ClearGate(0x31);
            Progress_ClearGate(0x32);
            Progress_ClearGate(0x33);
            Progress_ClearGate(0x34);
            Progress_ClearGate(0x35);
            Progress_ClearGate(0x36);
            Progress_ClearGate(0x37);
            Progress_ClearGate(56);
            Progress_ClearGate(0x39);
            Progress_ClearGate(58);
            Progress_ClearGate(0x3B);
            Progress_ClearGate(0x3C);
            break;
        case 0x838:
            Progress_SetGate(2);
            Progress_SetGate(0x41);
            gSaveState.unk_20.unk_266E = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_HOT_DOG);
            arg0->unk_21CF8 = 1;
            break;
        case 0x556:
            Progress_SetGate(0x40);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_CASUAL_PENDULUM);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case EVENT_BEAT3_SECRET_BOX_TISPY_TOSE_HALL:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_BLACK_BOOK_BAG, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_BLACK_BOOK_BAG);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209af50(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_26C0          = 4;
    gSaveState.unk_20.unk_26C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(15);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x828C;
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x48;
    func_ov030_02084944(0xE, 0);
    func_ov030_02084944(15, 0);
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_0209b058(s32 arg0) {
    if (gSaveState.unk_20.unk_2458 == 15) {
        if ((func_ov030_020848e4(arg0, 0x11) != 0) && (Progress_Check(0x24) == 0) && (Progress_Check(3) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x28E, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) == 0) && (Progress_Check(0x24) != 0) && (Progress_Check(3) == 0)) {
            Progress_ClearGate(0x24);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (Progress_Check(37) == 0) && (Progress_Check(3) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x28F, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) == 0) && (Progress_Check(37) != 0) && (Progress_Check(3) == 0)) {
            Progress_ClearGate(37);
        }
        if ((Progress_Check(3) != 0) && (Progress_Check(0x45) == 0)) {
            Progress_SetGate(0x45);
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_BEAT;
        }
        if ((func_ov030_020848e4(arg0, 0x11) != 0) && (Progress_Check(3) != 0) && (Progress_Check(38) == 0) &&
            (Progress_Check(4) == 0))
        {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x291, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) == 0) && (Progress_Check(3) != 0) && (Progress_Check(38) != 0) &&
                   (Progress_Check(4) == 0))
        {
            Progress_ClearGate(38);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (Progress_Check(3) != 0) && (Progress_Check(4) == 0) &&
            (Progress_Check(41) == 0))
        {
            Progress_SetGate(4);
            Progress_BeginScriptedEvent(arg0, 0x292, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) != 0) && (Progress_Check(5) != 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x29C, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) != 0) && (Progress_Check(5) != 0) && (Progress_Check(0x28) == 0) &&
                   (Progress_Check(41) == 0))
        {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x29D, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) == 0) && (Progress_Check(5) != 0) && (Progress_Check(0x28) != 0) &&
                   (Progress_Check(41) == 0))
        {
            Progress_ClearGate(0x28);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (Progress_Check(8) != 0) && (Progress_Check(9) == 0)) {
            Progress_SetGate(9);
            Progress_BeginScriptedEvent(arg0, 0x2B3, 0);
            return 1;
        } else if ((Progress_Check(2) != 0) && (Progress_Check(0x46) == 0)) {
            Progress_SetGate(0x46);
            func_ov030_020aec1c(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_BEAT;
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (Progress_Check(2) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x2B5, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0xE) && (Progress_Check(39) != 0) && (Progress_Check(41) == 0)) {
        Progress_SetGate(41);
        Progress_BeginScriptedEvent(arg0, 0x29E, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 13) {
        if ((Progress_Check(2) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x2B6, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 15) != 0) || (func_ov030_020848e4(arg0, 0x10) != 0)) &&
                   (Progress_Check(0x23) == 0) && (Progress_Check(33) == 0))
        {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x2B7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 15) == 0) && (func_ov030_020848e4(arg0, 0x10) == 0) &&
                   (Progress_Check(0x23) != 0) && (Progress_Check(33) == 0))
        {
            Progress_ClearGate(0x23);
        }
        if ((func_ov030_020aed9c(-2) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x2B8, 0);
            return 1;
        } else if ((Progress_Check(0x43) != 0) && (Progress_Check(0x2C) == 0)) {
            Progress_SetGate(0x2C);
            Progress_BeginScriptedEvent(arg0, 0x2BA, 0);
            return 1;
        } else if ((Progress_Check(0x44) != 0) && (Progress_Check(45) == 0)) {
            Progress_SetGate(45);
            Progress_BeginScriptedEvent(arg0, 0x2BB, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209b724(void) {
    if (gSaveState.unk_20.unk_2458 == 15) {
        if ((Progress_Check(4) != 0) && (Progress_Check(5) == 0)) {
            return 0x297;
        } else if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
            return 0x2A4;
        } else if ((Progress_Check(7) != 0) && (Progress_Check(8) == 0)) {
            return 0x2AD;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0xE) {
        if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
            return 0x2A4;
        } else if ((Progress_Check(7) != 0) && (Progress_Check(8) == 0)) {
            return 0x2AD;
        }
    }
    return 0;
}

s32 func_ov030_0209b810(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((Progress_Check(47) != 0) && (Progress_Check(0x31) != 0) && (Progress_Check(46) == 0) &&
                (Progress_Check(0x30) == 0))
            {
                Progress_SetCurrentEvent(0x298);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x299);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D6:
            Progress_SetCurrentEvent(0x29A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209b8e8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((Progress_Check(0x32) != 0) && (Progress_Check(0x35) != 0) && (Progress_Check(56) != 0) &&
                (Progress_Check(0x3B) != 0))
            {
                Progress_SetCurrentEvent(0x2A5);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x2A6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D6:
            Progress_SetCurrentEvent(0x2A7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209b9c4(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((Progress_Check(0x3C) == 0) && (Progress_Check(61) != 0) && (Progress_Check(0x3E) == 0) &&
                (Progress_Check(0x3F) == 0) && (Progress_Check(0x40) != 0) && (Progress_Check(0x41) != 0) &&
                (Progress_Check(0x42) == 0))
            {
                Progress_SetCurrentEvent(686);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x2AF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D6:
            Progress_SetCurrentEvent(0x2B0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209bacc(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x290:
            Progress_SetGate(3);
            arg0->unk_21CF8 = 1;
            break;
        case 0x292:
            gSaveState.unk_20.unk_24BE = 0x2BF;
            func_ov030_020aec1c(10);
            gSaveState.unk_20.unk_310C = &data_ov030_020d9b94;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x297:
            return func_ov030_020a9f54(arg0, func_ov030_0209b810, 0x36D4, 2, 0x36D5, 0x36D6);
        case 0x299:
            Progress_ClearGate(46);
            Progress_ClearGate(47);
            Progress_ClearGate(0x30);
            Progress_ClearGate(0x31);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x298:
            Progress_SetCurrentEvent(0x29B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x29B:
            Progress_SetGate(5);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x29C:
            gSaveState.unk_20.unk_266A = 4;
            break;
        case 0x29F:
            Progress_SetGate(6);
            gSaveState.unk_20.unk_310C = &data_ov030_020d9bbc;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2A4:
            return func_ov030_020a9f54(arg0, func_ov030_0209b8e8, 0x36D4, 2, 0x36D5, 0x36D6);
        case 0x2A6:
            Progress_ClearGate(0x32);
            Progress_ClearGate(0x33);
            Progress_ClearGate(0x34);
            Progress_ClearGate(0x35);
            Progress_ClearGate(0x36);
            Progress_ClearGate(0x37);
            Progress_ClearGate(56);
            Progress_ClearGate(0x39);
            Progress_ClearGate(58);
            Progress_ClearGate(0x3B);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x2A5:
            Progress_SetCurrentEvent(0x2A8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2A8:
            Progress_SetGate(7);
            gSaveState.unk_20.unk_310C = &data_ov030_020d9d34;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2AD:
            return func_ov030_020a9f54(arg0, func_ov030_0209b9c4, 0x36D4, 2, 0x36D5, 0x36D6);
        case 0x2AF:
            Progress_ClearGate(0x3C);
            Progress_ClearGate(61);
            Progress_ClearGate(0x3E);
            Progress_ClearGate(0x3F);
            Progress_ClearGate(0x40);
            Progress_ClearGate(0x41);
            Progress_ClearGate(0x42);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x2AE:
            Progress_SetCurrentEvent(0x2B1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2B1:
            Progress_SetGate(8);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2B3:
            func_ov030_020aec38(1);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            arg0->unk_21CF8                            = 1;
            break;
        case 0x2B4:
            Progress_SetGate(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2B5:
            gSaveState.unk_20.unk_2668 = 4;
            break;
        case 0x2B6:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2B8:
            Progress_SetCurrentEvent(0x2B9);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2B9:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8560;
            func_ov030_020af364(39);
            return 1;
        case 0x2BA:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8561;
            func_ov030_020af364(38);
            return 1;
        case 0x2BB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x2BC);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2BC:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x2BD);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2BD:
            gSaveState.unk_20.unk_3124 = 41;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x560:
            Progress_SetGate(0x43);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x561:
            Progress_SetGate(0x44);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, PIN_PLAYMATE_BEAM);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case EVENT_BEAT4_SECRET_BOX_MIYASHITA_PARK:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_GAKURAN, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_GAKURAN);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209c0f4(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8          = 4;
    gSaveState.unk_20.unk_26BA          = 4;
    gSaveState.unk_20.currentStoryEvent = 4;
    gSaveState.unk_20.unk_26C0          = 4;
    gSaveState.unk_20.unk_26C2          = 4;
    gSaveState.unk_20.unk_26C4          = 4;
    gSaveState.unk_20.unk_26C6          = 4;
    gSaveState.unk_20.unk_26C8          = 4;
    gSaveState.unk_20.unk_26CA          = 4;
    gSaveState.unk_20.unk_26CC          = 4;
    gSaveState.unk_20.unk_26CE          = 4;
    gSaveState.unk_20.unk_26D0          = 4;
    gSaveState.unk_20.unk_26D2          = 4;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x82C2;
    func_ov030_020c26bc(4);
    gSaveState.unk_20.unk_265C = 0;
    gSaveState.unk_20.unk_2660 = 2;
    gSaveState.unk_20.unk_266C = 0;
    gSaveState.unk_20.unk_2670 = 0;
    gSaveState.unk_20.unk_2676 = 0;
    func_ov030_02084944(7);
    gSaveState.unk_20.unk_3124 = 0x49;
    gSaveState.unk_20.unk_3100 = 0x14;
}

s32 func_ov030_0209c208(s32 arg0) {
    if (gSaveState.unk_20.unk_2458 == 6) {
        if (Progress_Check(0x20) == 0) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x82C3, 0);
            return 1;
        } else if ((Progress_Check(42) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x2C5, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0) ||
                    (func_ov030_020848e4(arg0, 10) != 0)) &&
                   (Progress_Check(33) != 0) && (Progress_Check(0x22) == 0) && (Progress_Check(3) == 0))
        {
            Progress_SetGate(3);
            Progress_BeginScriptedEvent(arg0, 0x2C6, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 8) == 0) && (func_ov030_020848e4(arg0, 9) == 0) &&
                   (func_ov030_020848e4(arg0, 10) == 0) && (Progress_Check(33) != 0) && (Progress_Check(0x22) == 0) &&
                   (Progress_Check(3) != 0))
        {
            Progress_ClearGate(3);
        }
        if ((func_ov030_020aed9c(-2) != 0) && (Progress_Check(33) != 0) && (Progress_Check(0x22) == 0)) {
            Progress_SetGate(0x22);
            Progress_BeginScriptedEvent(arg0, 0x2C7, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 7) {
        if (Progress_Check(0x23) == 0) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x2C8, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x64, 0x64, 0x96, 0x15E) != 0) && (Progress_Check(6) == 0)) {
            Progress_SetGate(6);
            Progress_BeginScriptedEvent(arg0, 0x2CA, 0);
            return 1;
        } else if ((Progress_Check(43) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x2CB, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 12) != 0) && (Progress_Check(6) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x2CD, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 9) && (Progress_Check(0x2C) != 0) && (Progress_Check(41) == 0)) {
        Progress_SetGate(41);
        Progress_BeginScriptedEvent(arg0, 0x2D3, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_0209c57c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D8:
            if (Progress_Check(39) == 0) {
                Progress_SetGate(39);
                Progress_SetCurrentEvent(0x2CF);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x2D0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36D9:
            if (Progress_Check(0x28) == 0) {
                Progress_SetGate(0x28);
                Progress_SetCurrentEvent(0x2CF);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x2D0);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36DA:
            Progress_SetCurrentEvent(0x2D1);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209c680(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x2C4:
            Progress_SetGate(2);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = EVENT_BEAT1_TIPSY_TOSE_HALL;
            func_ov030_020af364(41);
            return 1;
        case 0x2C5:
            gSaveState.unk_20.unk_2658 = 2;
            gSaveState.unk_20.unk_265A = 2;
            gSaveState.unk_20.unk_265C = 2;
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2C7:
            gSaveState.unk_20.unk_2658 = 4;
            gSaveState.unk_20.unk_265A = 4;
            gSaveState.unk_20.unk_265C = 4;
            func_ov030_020aec38(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2CC:
            if ((Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                Progress_SetCurrentEvent(EVENT_BEAT5_SECRET_BOX_SHIBU_Q_HEADS);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0X2CA:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x56BU;
            func_ov030_020af364(42);
            return 1;
        case 0x2CD:
            gSaveState.unk_20.unk_26C0 = 4;
            break;
        case 0x2CE:
        case 0x2CF:
        case 0x2D0:
            return func_ov030_020a9f54(arg0, func_ov030_0209c57c, 0x36D7, 3, 0x36D8, 0x36D9, 0x36DA);
        case 0x2D1:
            Progress_SetGate(4);
            gSaveState.unk_20.unk_26C0 = 3;
            arg0->unk_21CF8            = 1;
            break;
        case 0x2D2:
            Progress_SetGate(5);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x56C;
            func_ov030_020af364(0x28);
            return 1;
        case 0x2D3:
            gSaveState.unk_20.unk_3124 = 42;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_BEAT1_TIPSY_TOSE_HALL:
            Progress_SetGate(42);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_BEAT);
            break;
        case 0x56B:
            Progress_SetGate(43);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case 0x56C:
            Progress_SetGate(0x2C);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, PIN_FLOWER_OF_FLAME);
            break;
        case EVENT_BEAT5_SECRET_BOX_SHIBU_Q_HEADS:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_HEAD_HONCHO, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_HEAD_HONCHO);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209ca24(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8 = 4;
    gSaveState.unk_20.unk_26BA = 4;
    gSaveState.unk_20.unk_26BC = 4;
    gSaveState.unk_20.unk_26BE = 0;
    gSaveState.unk_20.unk_26C0 = 4;
    gSaveState.unk_20.unk_26C2 = 4;
    gSaveState.unk_20.unk_26C4 = 4;
    gSaveState.unk_20.unk_26C6 = 4;
    gSaveState.unk_20.unk_26C8 = 4;
    gSaveState.unk_20.unk_26CA = 4;
    gSaveState.unk_20.unk_26CC = 4;
    gSaveState.unk_20.unk_26CE = 4;
    gSaveState.unk_20.unk_26D0 = 4;
    gSaveState.unk_20.unk_26D2 = 2;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        Progress_SetGate(2);
        gSaveState.unk_20.unk_26D4 = 4;
    } else {
        func_ov030_020aec1c(4);
        gSaveState.unk_20.unk_26D4 = 0;
    }
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x82DA;
    gSaveState.unk_20.unk_3124          = 0x4A;
    func_ov030_020c26bc(4);
    gSaveState.unk_20.unk_2670 = 0;
    gSaveState.unk_20.unk_2676 = 0;
    func_ov030_020aec1c(5);
    gSaveState.unk_20.unk_3100 = 0x14;
    func_ov030_020aec1c(9);
}

s32 func_ov030_0209cb70(s32 arg0) {
    if ((gSaveState.unk_20.unk_2458 == 1) && (func_ov030_020aed9c(-2) != 0) && (Progress_Check(33) == 0) &&
        (Progress_Check(0x20) == 0))
    {
        Progress_SetGate(0x20);
        Progress_BeginScriptedEvent(arg0, 0x2DB, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 11) && (func_ov030_020aed9c(-2) != 0) && (Progress_Check(33) == 0) &&
               (Progress_Check(0x20) == 0))
    {
        Progress_SetGate(0x20);
        Progress_BeginScriptedEvent(arg0, 0x2DB, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 12) {
        if ((func_ov030_020aed9c(-2) != 0) && (Progress_Check(33) == 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x2DB, 0);
            return 1;
        } else if ((Progress_Check(3) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x2E0, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 13) != 0) || (func_ov030_020848e4(arg0, 0xE) != 0)) &&
                   (Progress_Check(0x24) != 0) && (Progress_Check(39) == 0))
        {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x82E2, 0);
            return 1;
        } else if ((Progress_Check(0x28) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x2E3, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 13) && (func_ov030_020aed9c(-2) != 0) && (Progress_Check(33) == 0) &&
        (Progress_Check(0x20) == 0))
    {
        Progress_SetGate(0x20);
        Progress_BeginScriptedEvent(arg0, 0x2DB, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(2) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x42E7, 0);
            return 1;
        } else if ((Progress_Check(38) != 0) && (Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
            Progress_SetGate(1);
            Progress_BeginScriptedEvent(arg0, 0x2E8, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0xE) && (func_ov030_020848e4(arg0, 18) != 0) && (Progress_Check(33) == 0)) {
        Progress_SetGate(33);
        Progress_BeginScriptedEvent(arg0, 0x2DC, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(3) == 0)) {
            Progress_SetGate(3);
            Progress_BeginScriptedEvent(arg0, 0x42DD, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 0x45) != 0 && Progress_Check(0x22) != 0 && Progress_Check(0x23) == 0) {
            Progress_SetGate(0x23);
            Progress_BeginScriptedEvent(arg0, 0x42DF, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 0x45) == 0 && Progress_Check(0x22) != 0 && Progress_Check(0x23) != 0) {
            Progress_ClearGate(0x23);
        }
    }
    return 0;
}

s32 func_ov030_0209cff0(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x2DD:
            Progress_SetCurrentEvent(0x2DE);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2DE:
            Progress_SetGate(0x22);
            Progress_SetGate(0x23);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            break;
        case 0x2E1:
        case 0x2E2:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8574;
            func_ov030_020af364(43);
            return 1;
        case 0x2E3:
            gSaveState.unk_20.unk_3124 = 43;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x2E7:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x574:
            Progress_SetGate(0x28);
            if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case EVENT_BEAT6_SECRET_BOX_DOGENZAKA:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_LUCKY_MALLET, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_LUCKY_MALLET);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209d1d8(void) {
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8 = 4;
    gSaveState.unk_20.unk_26BA = 4;
    gSaveState.unk_20.unk_26BC = 4;
    gSaveState.unk_20.unk_26C0 = 4;
    gSaveState.unk_20.unk_26C2 = 4;
    gSaveState.unk_20.unk_26C4 = 4;
    gSaveState.unk_20.unk_26C6 = 4;
    gSaveState.unk_20.unk_26C8 = 4;
    gSaveState.unk_20.unk_26CA = 4;
    gSaveState.unk_20.unk_26CC = 4;
    gSaveState.unk_20.unk_26CE = 4;
    gSaveState.unk_20.unk_26D0 = 4;
    gSaveState.unk_20.unk_26D2 = 2;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        gSaveState.unk_20.unk_26D4 = 4;
    } else {
        func_ov030_020aec1c(4);
        gSaveState.unk_20.unk_26D4 = 0;
    }
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x82EC;
    gSaveState.unk_20.unk_3124          = 0x4B;
    func_ov030_020c26bc(4);
    func_ov030_020aec1c(5);
    gSaveState.unk_20.unk_2676 = 0;
    gSaveState.unk_20.unk_26B0 = 0;
    gSaveState.unk_20.unk_26B2 = 2;
    gSaveState.unk_20.unk_26B4 = 0;
    gSaveState.unk_20.unk_3100 = 0x14;
}

s32 func_ov030_0209d318(s32 arg0) {
    if (gSaveState.unk_20.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x42ED, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x45) == 0) && (Progress_Check(0x20) != 0)) {
            Progress_ClearGate(0x20);
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(47) == 0)) {
            Progress_SetGate(47);
            Progress_BeginScriptedEvent(arg0, 0x4312, 0);
            return 1;
        } else if ((Progress_Check(47) != 0) && (Progress_Check(GATE_GAME_CLEARED) != 0) && (Progress_Check(1) == 0)) {
            Progress_SetGate(1);
            Progress_BeginScriptedEvent(arg0, 0x4311, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 18) {
        gSaveState.unk_20.unk_3100 = 20;
        if ((func_ov030_020848e4(arg0, 20) != 0) && (Progress_Check(33) == 0)) {
            Progress_SetGate(33);
            Progress_BeginScriptedEvent(arg0, 0x2EE, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 38) {
        gSaveState.unk_20.unk_3100 = 0;
        if (Progress_Check(0x32) == 0) {
            Progress_SetGate(0x32);
            Progress_BeginScriptedEvent(arg0, 0x2EF, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 39) {
        if ((Progress_Check(0x30) != 0) && (Progress_Check(4) == 0)) {
            Progress_SetGate(4);
            Progress_BeginScriptedEvent(arg0, 0x2F3, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) != 0) && (Progress_Check(0x22) == 0) && (Progress_Check(0x23) == 0)) {
            Progress_SetGate(0x22);
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x2F4, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) != 0) && (Progress_Check(0x23) != 0) && (Progress_Check(0x24) == 0)) {
            Progress_SetGate(0x24);
            Progress_BeginScriptedEvent(arg0, 0x2F7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) == 0) && (Progress_Check(0x23) != 0) && (Progress_Check(0x24) != 0)) {
            Progress_ClearGate(0x24);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x28) {
        if ((func_ov030_020848a4(0x12C, 0xB4, 0x15E, 0x140) != 0) && (Progress_Check(37) == 0)) {
            Progress_SetGate(37);
            Progress_BeginScriptedEvent(arg0, 0x2F8, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x2A8, 0xB4, 0x2DA, 0x140) != 0) && (Progress_Check(38) == 0)) {
            Progress_SetGate(38);
            Progress_BeginScriptedEvent(arg0, 0x2F9, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x35) != 0) && (Progress_Check(5) == 0) && (Progress_Check(39) == 0)) {
            Progress_SetGate(39);
            Progress_BeginScriptedEvent(arg0, 0x2FB, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x35) == 0) && (Progress_Check(5) == 0) && (Progress_Check(39) != 0)) {
            Progress_ClearGate(39);
        }
        if ((func_ov030_020848e4(arg0, 0x35) != 0) && (Progress_Check(5) != 0) && (Progress_Check(0x28) == 0)) {
            Progress_SetGate(0x28);
            Progress_BeginScriptedEvent(arg0, 0x2FC, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 41) {
        if (Progress_Check(41) == 0) {
            Progress_SetGate(41);
            Progress_BeginScriptedEvent(arg0, 0x82FD, 0);
            return 1;
        } else if ((Progress_Check(0x31) != 0) && (Progress_Check(42) == 0)) {
            Progress_SetGate(42);
            Progress_BeginScriptedEvent(arg0, 0x2FF, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 42) {
        if ((func_ov030_020848a4(0x2A8, 0xB4, 0x2DA, 0x140) != 0) && (Progress_Check(43) == 0)) {
            Progress_SetGate(43);
            Progress_BeginScriptedEvent(arg0, 0x8301, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x37) != 0) && (Progress_Check(0x2C) == 0)) {
            Progress_SetGate(0x2C);
            Progress_BeginScriptedEvent(arg0, 0x302, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 43) && (func_ov030_020848a4(0, 0x96, 0x12C, 0xC8) != 0) && (Progress_Check(45) == 0)) {
        Progress_SetGate(45);
        Progress_BeginScriptedEvent(arg0, 0x8303, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_0209d974(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36DC:
            Progress_SetCurrentEvent(0x2F5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36DD:
            Progress_SetGate(0x23);
            Progress_SetCurrentEvent(0x2F6);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209d9fc(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36DF:
            Progress_SetCurrentEvent(0x304);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36E0:
            Progress_SetCurrentEvent(0x305);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209da78(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E2:
            Progress_SetGate(46);
            Progress_SetCurrentEvent(775);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36E3:
            Progress_SetCurrentEvent(776);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209dafc(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x2EF:
            gSaveState.unk_20.unk_24BE = 0x313;
            func_ov030_020aec1c(10);
            break;
        case 0x2F0:
            Progress_SetGate(2);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x82F1;
            gSaveState.unk_20.unk_3124          = 0x2C;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x2F1:
            Progress_SetGate(3);
            func_ov030_020aec1c(6);
            Progress_SetCurrentEvent(0x2F2);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x2F2:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8420U;
            gSaveState.unk_20.unk_3124          = 45;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x70F:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x857E;
            func_ov030_020af364(0x2C);
            return 1;
        case 0x2F3:
            gSaveState.unk_20.unk_24BE = 0x314;
            func_ov030_020aec1c(10);
            gSaveState.unk_20.unk_26B0 = 2;
            arg0->unk_21CF8            = 1;
            break;
        case 0x2F4:
        case 0x2F7:
            return func_ov030_020a9f54(arg0, func_ov030_0209d974, 0x36DB, 2, 0x36DC, 0x36DD);
        case 0x2F5:
            gSaveState.unk_20.unk_26B0 = 0;
            arg0->unk_21AD0            = 1;
            arg0->unk_21ACC            = 1;
            arg0->unk_21AFC            = 0x28;
            arg0->unk_21B00            = 0x66;
            arg0->unk_21B08            = 0x2A000;
            arg0->unk_21B0C            = 0xF6000;
            break;
        case EVENT_BEAT7_TRAIL_OF_THE_BYGONE:
            ProgressReward_GrantItem(arg0, ITEM_THREAD_PI_FACES_CAP);
            Progress_SetGate(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2FC:
            gSaveState.unk_20.unk_26B2 = 4;
            break;
        case 0x2FE:
            Progress_SetGate(6);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x857F;
            func_ov030_020af364(45);
            return 1;
        case 0x2FF:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8300;
            gSaveState.unk_20.unk_26B2          = 0;
            gSaveState.unk_20.unk_26B4          = 4;
            gSaveState.unk_20.unk_26B6          = 4;
            func_ov030_020aec38(4);
            gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
            gSaveState.unk_20.unk_3124                 = 46;
            gSaveState.unk_20.unk_3128                 = 0;
            arg0->unk_21630                            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x303:
            return func_ov030_020a9f54(arg0, func_ov030_0209d9fc, 0x36DE, 2, 0x36DF, 0x36E0);
        case 0x304:
        case 0x305:
            Progress_SetCurrentEvent(0x306);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x306:
            return func_ov030_020a9f54(arg0, func_ov030_0209da78, 0x36E1, 2, 0x36E2, 0x36E3);
        case 0x307:
        case 0x308:
            Progress_SetCurrentEvent(0x309);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x309:
            gSaveState.unk_20.unk_3124 = 47;
            gSaveState.unk_20.unk_3128 = 0;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x30A:
            Progress_SetCurrentEvent(0x30C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x30B:
            if (Progress_Check(46) != 0) {
                Progress_SetCurrentEvent(0x30E);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x30D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x30D:
        case 0x30E:
            Progress_SetCurrentEvent(0x30F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x30F:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x8310;
            gSaveState.unk_20.unk_3124          = 0x32;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x310:
            gSaveState.unk_20.unk_3124 = 0x33;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x57E:
            Progress_SetGate(0x30);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x57F:
            Progress_SetGate(0x31);
            func_ov030_020aac28(arg0, PIN_LIGHTNING_ROOK);
            break;
        case 0x312:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_BEAT7_SECRET_BOX_DOGENZAKA:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_SAMURAI_HELM, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_SAMURAI_HELM);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_0209e16c(void) {
    s16 var_r1;

    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    gSaveState.unk_20.unk_26B8 = 4;
    gSaveState.unk_20.unk_26BA = 4;
    gSaveState.unk_20.unk_26BC = 4;
    gSaveState.unk_20.unk_26C0 = 4;
    gSaveState.unk_20.unk_26C2 = 4;
    gSaveState.unk_20.unk_26C4 = 4;
    gSaveState.unk_20.unk_26C6 = 4;
    gSaveState.unk_20.unk_26C8 = 4;
    gSaveState.unk_20.unk_26CA = 4;
    gSaveState.unk_20.unk_26CC = 4;
    gSaveState.unk_20.unk_26CE = 4;
    gSaveState.unk_20.unk_26D0 = 4;
    gSaveState.unk_20.unk_26D2 = 4;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        Progress_SetGate(0x1F);
        var_r1 = 2;
    } else {
        func_ov030_020aec1c(4);
        var_r1 = 0;
    }
    gSaveState.unk_20.unk_26D4 = var_r1;
    gSaveState.unk_20.unk_26D6 = 4;
    gSaveState.unk_20.unk_26D8 = 4;
    gSaveState.unk_20.unk_26DA = 4;
    gSaveState.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(4);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x317;
    gSaveState.unk_20.unk_2670          = 2;
    gSaveState.unk_20.unk_2676          = 2;
    gSaveState.unk_20.unk_267E          = 0;
    gSaveState.unk_20.unk_267C          = 0;
    gSaveState.unk_20.unk_2680          = 0;
    gSaveState.unk_20.unk_2682          = 0;
    gSaveState.unk_20.unk_2686          = 0;
    gSaveState.unk_20.unk_268A          = 0;
    gSaveState.unk_20.unk_268E          = 0;
    gSaveState.unk_20.unk_2692          = 0;
    gSaveState.unk_20.unk_2696          = 0;
    gSaveState.unk_20.unk_269A          = 0;
    gSaveState.unk_20.unk_269E          = 0;
    gSaveState.unk_20.unk_26A2          = 0;
    gSaveState.unk_20.unk_26A6          = 0;
    gSaveState.unk_20.unk_26AA          = 0;
    func_ov030_020aec1c(6);
    if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
        Progress_SetGate(127);
        func_ov030_020c26bc(0);
        gSaveState.unk_20.unk_26BA = 0;
        func_ov030_020ae96c(43);
        gSaveState.unk_20.unk_24B4          = 0;
        gSaveState.unk_20.unk_24B8          = 1;
        gSaveState.unk_20.currentStoryEvent = 0x588;
    }
}

s32 func_ov030_0209e314(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(0x66) != 0) && (Progress_Check(0x7B) == 0)) {
            Progress_SetGate(0x7B);
            Progress_BeginScriptedEvent(arg0, 0x370, 0);
            return 1;
        }
        if ((Progress_Check(47) != 0) && (Progress_Check(0x7C) == 0)) {
            Progress_SetGate(0x7C);
            Progress_BeginScriptedEvent(arg0, 0x370, 0);
            return 1;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 8) {
        if (Progress_Check(0x30) == 0) {
            Progress_SetGate(0x30);
            Progress_BeginScriptedEvent(arg0, 0x318, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (Progress_Check(2) == 0)) {
            Progress_SetGate(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (Progress_Check(0x32) == 0) && (Progress_Check(3) == 0)) {
            Progress_SetGate(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) == 0) && (Progress_Check(0x32) != 0) && (Progress_Check(3) == 0)) {
            Progress_ClearGate(0x32);
        }
        if ((Progress_Check(0x34) != 0) && (Progress_Check(0x35) == 0)) {
            Progress_SetGate(0x35);
            Progress_BeginScriptedEvent(arg0, 0x34D, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0x10) && (func_ov030_020848e4(arg0, 0x45) != 0) && (Progress_Check(4) != 0) &&
        (Progress_Check(0x34) == 0) && (Progress_Check(0x5D) == 0) && (Progress_Check(0x5A) == 0))
    {
        Progress_SetGate(0x5A);
        Progress_BeginScriptedEvent(arg0, 0x3C2, 0);
        return 1;
    }
    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(0x1F) != 0) && (Progress_Check(47) == 0)) {
            Progress_SetGate(0x1E);
            Progress_BeginScriptedEvent(arg0, 0x4371, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (Progress_Check(0x1F) != 0) && (Progress_Check(47) == 0) &&
            (Progress_Check(0x1E) != 0))
        {
            Progress_ClearGate(0x1E);
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (Progress_Check(0x1F) != 0) && (Progress_Check(47) != 0) &&
            (Progress_Check(0x1D) == 0))
        {
            Progress_SetGate(0x1D);
            Progress_BeginScriptedEvent(arg0, 0x4372, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(3) != 0) && (Progress_Check(0x33) == 0)) {
            Progress_SetGate(0x33);
            Progress_BeginScriptedEvent(arg0, 0x4326, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (Progress_Check(7) != 0) && (Progress_Check(0x20) == 0)) {
            Progress_SetGate(0x20);
            Progress_BeginScriptedEvent(arg0, 0x432E, 0);
            return 1;
        }
        if ((Progress_Check(4) != 0) && (Progress_Check(47) == 0) && (func_ov030_020848e4(arg0, 23) != 0) &&
            (Progress_Check(0x5E) == 0))
        {
            Progress_SetGate(0x5E);
            Progress_BeginScriptedEvent(arg0, 0x357, 0);
            return 1;
        }
        if ((Progress_Check(4) != 0) && (Progress_Check(47) == 0) && (func_ov030_020848e4(arg0, 23) == 0) &&
            (Progress_Check(0x5E) != 0))
        {
            Progress_ClearGate(0x5E);
        }
        if (((Progress_Check(4) == 0) || (Progress_Check(47) != 0)) && (func_ov030_020848e4(arg0, 23) != 0) &&
            (Progress_Check(0x5D) == 0))
        {
            Progress_SetGate(0x5D);
            Progress_BeginScriptedEvent(arg0, 0x356, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x18) {
        if (Progress_Check(0x5F) == 0) {
            Progress_SetGate(0x5F);
            Progress_BeginScriptedEvent(arg0, 0x358, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(0x6E);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x19) {
        if (Progress_Check(0x60) == 0) {
            Progress_SetGate(0x60);
            Progress_BeginScriptedEvent(arg0, 0x359, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(0x6F);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x1C) {
        if (Progress_Check(0x61) == 0) {
            Progress_SetGate(0x61);
            Progress_BeginScriptedEvent(arg0, 0x35A, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(0x72);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x1F) {
        if (Progress_Check(0x62) == 0) {
            Progress_SetGate(0x62);
            Progress_BeginScriptedEvent(arg0, 0x35B, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(0x75);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x22) {
        if (Progress_Check(0x63) == 0) {
            Progress_SetGate(0x63);
            Progress_BeginScriptedEvent(arg0, 0x35C, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(0x78);
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x24) {
        if (Progress_Check(0x64) == 0) {
            Progress_SetGate(0x64);
            Progress_BeginScriptedEvent(arg0, 0x35D, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            Progress_SetGate(0x7A);
        }
        if ((func_ov030_020848e4(arg0, 0x31) != 0) && (Progress_Check(0x16) != 0) && (Progress_Check(0x65) == 0)) {
            Progress_SetGate(0x7E);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 37) {
        if (Progress_Check(0x7D) != 0) {
            if ((Progress_Check(0x30) != 0) && (Progress_Check(2) == 0)) {
                Progress_SetGate(2);
                Progress_BeginScriptedEvent(arg0, 0x319, 0);
                return 1;
            }
            if ((Progress_Check(2) != 0) && (Progress_Check(0x32) == 0)) {
                Progress_SetGate(0x32);
                Progress_BeginScriptedEvent(arg0, 0x31C, 0);
                return 1;
            }
        }
        if ((Progress_Check(0x7E) != 0) && (Progress_Check(0x64) != 0) && (Progress_Check(0x65) == 0)) {
            Progress_SetGate(0x65);
            Progress_BeginScriptedEvent(arg0, 0x35E, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0x1A) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x70);
    }
    if ((gSaveState.unk_20.unk_2458 == 0x1B) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x71);
    }
    if ((gSaveState.unk_20.unk_2458 == 0x1D) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x73);
    }
    if ((gSaveState.unk_20.unk_2458 == 0x1E) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x74);
    }
    if ((gSaveState.unk_20.unk_2458 == 0x20) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x76);
    }
    if ((gSaveState.unk_20.unk_2458 == 33) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x77);
    }
    if ((gSaveState.unk_20.unk_2458 == 0x23) && (func_ov030_020c1a9c() != 0)) {
        Progress_SetGate(0x79);
    }
    if (gSaveState.unk_20.unk_2458 == 18) {
        if (((Progress_Check(4) == 0) || (Progress_Check(47) != 0)) && (func_ov030_020848e4(arg0, 0x14) != 0) &&
            (Progress_Check(0x68) == 0))
        {
            Progress_SetGate(0x68);
            Progress_BeginScriptedEvent(arg0, 0x361, 0);
            return 1;
        }
        if (((Progress_Check(4) == 0) || (Progress_Check(47) != 0)) && (func_ov030_020848e4(arg0, 0x14) == 0) &&
            (Progress_Check(0x68) != 0))
        {
            Progress_ClearGate(0x68);
        }
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (Progress_Check(4) != 0) && (Progress_Check(47) == 0) &&
            (Progress_Check(0x67) == 0))
        {
            Progress_SetGate(0x67);
            Progress_BeginScriptedEvent(arg0, 0x360, 0);
            return 1;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 39) && (func_ov030_020848a4(0x17C, 0, 0x190, 0x12C) != 0) &&
        (Progress_Check(0x69) == 0))
    {
        Progress_SetGate(0x69);
        Progress_BeginScriptedEvent(arg0, 0x362, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 41) && (Progress_Check(0x6A) == 0)) {
        Progress_SetGate(0x6A);
        Progress_BeginScriptedEvent(arg0, 0x363, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 42) && (Progress_Check(0x6B) == 0)) {
        Progress_SetGate(0x6B);
        Progress_BeginScriptedEvent(arg0, 0x364, 0);
        return 1;
    } else if (gSaveState.unk_20.unk_2458 == 43) {
        if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
            Progress_SetGate(GATE_GAME_CLEARED);
            Progress_BeginScriptedEvent(arg0, 0x316, 0);
            return 1;
        }
        if ((func_ov030_020848a4(0, 0, 0x12C, 0xE6) != 0) && (Progress_Check(0x6B) != 0) && (Progress_Check(0x6C) == 0)) {
            Progress_SetGate(0x6C);
            Progress_BeginScriptedEvent(arg0, 0x365, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209efe4(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E5:
            Progress_SetCurrentEvent(0x31A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36E6:
            Progress_SetGate(0x32);
            Progress_SetCurrentEvent(0x31B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209f06c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E8:
            Progress_SetCurrentEvent(0x367);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x36E9:
            Progress_SetCurrentEvent(0x368);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209f0e8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x588:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_GAME_CLEARED);
            break;
        case 0x372:
            gSaveState.unk_20.unk_26D4 = 4;
            arg0->unk_21630            = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
            return func_ov030_020a9f54(arg0, func_ov030_020919c4, 0x36EA, 5, 0x36EB, 0x36EC, 0x36ED, 0x36EE, 0x36EF);
        case 0x317:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x370;
            gSaveState.unk_20.unk_3124          = 0x4C;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x319:
            return func_ov030_020a9f54(arg0, func_ov030_0209efe4, 0x36E4, 2, 0x36E5, 0x36E6);
        case 0x324:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x325);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x31A:
            Progress_ClearGate(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        // case 0x31B:
        //     Progress_ClearGate(0x7D);
        //     arg0->unk_21AD0 = 1;
        //     arg0->unk_21ACC = 0;
        //     arg0->unk_21AFC = 8;
        //     arg0->unk_21B00 = 0x66;
        //     arg0->unk_21B08 = 0x168000;
        //     arg0->unk_21B0C = 0xFF000;
        //     return 0;
        case 0x31D:
            Progress_SetGate(0x31);
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x31E;
            gSaveState.unk_20.unk_24C2 = 0x31F;
            gSaveState.unk_20.unk_341C = 6;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x31E:
            Progress_SetCurrentEvent(0x320);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x320:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x321;
            gSaveState.unk_20.unk_24C2 = 0x322;
            gSaveState.unk_20.unk_341C = 7;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x321:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x323);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x326:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x324);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x325:
            Progress_SetGate(3);
            Progress_ClearGate(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        case 0x323:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x327);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x327:
            Progress_SetGate(4);
            gSaveState.unk_20.unk_2676 = 2;
            break;
        case 0x328:
            Progress_SetGate(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x42F:
            if (Progress_Check(6) == 0) {
                Progress_SetGate(6);
                Progress_SetCurrentEvent(0x329);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x32B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x329:
        case 0x32B:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x32C;
            gSaveState.unk_20.unk_24C2 = 0x32A;
            gSaveState.unk_20.unk_341C = 0x16;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x32C:
            Progress_SetGate(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x32F:
            Progress_SetGate(33);
            arg0->unk_21CF8 = 1;
            break;
        case 0x430:
            if (Progress_Check(39) == 0) {
                Progress_SetGate(39);
                Progress_SetCurrentEvent(0x330);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x332);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x330:
        case 0x332:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x333;
            gSaveState.unk_20.unk_24C2 = 0x331;
            gSaveState.unk_20.unk_341C = 23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x333:
            ProgressReward_GrantPin(arg0, PIN_5_YEN);
            Progress_SetGate(0x22);
            arg0->unk_21CF8 = 1;
            break;
        case 0x431:
            if (Progress_Check(0x28) == 0) {
                Progress_SetGate(0x28);
                Progress_SetCurrentEvent(0x335);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x337);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x335:
        case 0x337:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x338;
            gSaveState.unk_20.unk_24C2 = 0x336;
            gSaveState.unk_20.unk_341C = 0x18;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x338:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            Progress_SetGate(0x23);
            arg0->unk_21CF8 = 1;
            break;
        case 0x432:
            if (Progress_Check(41) == 0) {
                Progress_SetGate(41);
                Progress_SetCurrentEvent(0x33A);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x33C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x33A:
        case 0x33C:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x33D;
            gSaveState.unk_20.unk_24C2 = 0x33B;
            gSaveState.unk_20.unk_341C = 0x19;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x33D:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_ROCKER);
            Progress_SetGate(0x24);
            arg0->unk_21CF8 = 1;
            break;
        case 0x433:
            if (Progress_Check(42) == 0) {
                Progress_SetGate(42);
                Progress_SetCurrentEvent(0x33F);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x341);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x33F:
        case 0x341:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x342;
            gSaveState.unk_20.unk_24C2 = 0x340;
            gSaveState.unk_20.unk_341C = 0x1A;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x342:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_DEVIL);
            Progress_SetGate(37);
            arg0->unk_21CF8 = 1;
            break;
        case 0x434:
            if (Progress_Check(43) == 0) {
                Progress_SetGate(43);
                Progress_SetCurrentEvent(0x344);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x346);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x344:
        case 0x346:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x347;
            gSaveState.unk_20.unk_24C2 = 0x345;
            gSaveState.unk_20.unk_341C = 0x1B;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x347:
            Progress_SetGate(38);
            arg0->unk_21CF8 = 1;
            break;
        case 0x435:
            if (Progress_Check(0x2C) == 0) {
                Progress_SetGate(0x2C);
                Progress_SetCurrentEvent(0x348);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x34A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x348:
        case 0x34A:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x34B;
            gSaveState.unk_20.unk_24C2 = 0x349;
            gSaveState.unk_20.unk_341C = 0x1C;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x34B:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_GOLEM);
            Progress_SetGate(0x34);
            arg0->unk_21CF8 = 1;
            break;
        case 0x34D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x34E:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x34F;
            gSaveState.unk_20.unk_24C2 = 0x350;
            gSaveState.unk_20.unk_341C = 0x1D;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x34F:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_IFRIT);
        case 0x350:
            Progress_SetGate(45);
            arg0->unk_21CF8 = 1;
            break;
        case 0x436:
            if (Progress_Check(46) == 0) {
                Progress_SetGate(46);
                Progress_SetCurrentEvent(0x351);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x353);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x351:
        case 0x353:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x354;
            gSaveState.unk_20.unk_24C2 = 0x352;
            gSaveState.unk_20.unk_341C = 0x1E;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case EVENT_ANOTHERDAY_WIZARD_OF_SLAM:
            func_ov030_020aac28(arg0, PIN_TIN_PIN_BAHAMUT);
            ProgressReward_GrantItem(arg0, ITEM_THREAD_WIZARDS_GLASSES);
            Progress_SetGate(47);
            gSaveState.unk_20.unk_2676 = 4;
            gSaveState.unk_20.unk_2670 = 2;
            arg0->unk_21AD0            = 1;
            arg0->unk_21ACC            = 1;
            arg0->unk_21AFC            = 1;
            arg0->unk_21B00            = 0x66;
            arg0->unk_21B08            = 0x157000;
            arg0->unk_21B0C            = 0xD6000;
            break;
        case 0x373:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x374;
            gSaveState.unk_20.unk_24C2 = 0x375;
            gSaveState.unk_20.unk_341C = 2;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x374:
        case 0x375:
            Progress_SetGate(56);
            arg0->unk_21CF8 = 1;
            break;
        case 0x376:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x377;
            gSaveState.unk_20.unk_24C2 = 0x378;
            gSaveState.unk_20.unk_341C = 3;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x377:
        case 0x378:
            Progress_SetGate(0x39);
            arg0->unk_21CF8 = 1;
            break;
        case 0x379:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x37A;
            gSaveState.unk_20.unk_24C2 = 0x37B;
            gSaveState.unk_20.unk_341C = 38;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x37A:
        case 0x37B:
            Progress_SetGate(58);
            arg0->unk_21CF8 = 1;
            break;
        case 0x37C:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x37D;
            gSaveState.unk_20.unk_24C2 = 0x37E;
            gSaveState.unk_20.unk_341C = 39;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x37D:
        case 0x37E:
            Progress_SetGate(0x3B);
            arg0->unk_21CF8 = 1;
            break;
        case 0x37F:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x380;
            gSaveState.unk_20.unk_24C2 = 0x381;
            gSaveState.unk_20.unk_341C = 42;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x380:
        case 0x381:
            Progress_SetGate(0x3C);
            arg0->unk_21CF8 = 1;
            break;
        case 0x382:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x383;
            gSaveState.unk_20.unk_24C2 = 0x384;
            gSaveState.unk_20.unk_341C = 0x28;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x383:
        case 0x384:
            Progress_SetGate(61);
            arg0->unk_21CF8 = 1;
            break;
        case 0x385:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x386;
            gSaveState.unk_20.unk_24C2 = 0x387;
            gSaveState.unk_20.unk_341C = 41;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x386:
        case 0x387:
            Progress_SetGate(0x3E);
            arg0->unk_21CF8 = 1;
            break;
        case 0x388:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x389;
            gSaveState.unk_20.unk_24C2 = 0x38A;
            gSaveState.unk_20.unk_341C = 4;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x389:
        case 0x38A:
            Progress_SetGate(0x3F);
            arg0->unk_21CF8 = 1;
            break;
        case 0x38B:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x38C;
            gSaveState.unk_20.unk_24C2 = 0x38D;
            gSaveState.unk_20.unk_341C = 5;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x38C:
        case 0x38D:
            Progress_SetGate(0x40);
            arg0->unk_21CF8 = 1;
            break;
        case 0x38E:
            Progress_SetGate(0x41);
            arg0->unk_21CF8 = 1;
            break;
        case 0x421:
            if (Progress_Check(0x4E) == 0) {
                Progress_SetGate(0x4E);
                Progress_SetCurrentEvent(0x392);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x393);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x392:
        case 0x393:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x394;
            gSaveState.unk_20.unk_24C2 = 0x395;
            gSaveState.unk_20.unk_341C = 8;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x394:
            Progress_SetGate(0x42);
            func_ov030_020aac28(arg0, PIN_SPARKLE_CHARGE);
            arg0->unk_21CF8 = 1;
        case 0x395:
            arg0->unk_21CF8 = 1;
            break;
        case 0x422:
            if (Progress_Check(0x4F) == 0) {
                Progress_SetGate(0x4F);
                Progress_SetCurrentEvent(0x396);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x397);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x396:
        case 0x397:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x398;
            gSaveState.unk_20.unk_24C2 = 0x399;
            gSaveState.unk_20.unk_341C = 11;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x398:
            Progress_SetGate(0x43);
            func_ov030_020aac28(arg0, PIN_PEACE_FULL);
            arg0->unk_21CF8 = 1;
        case 0x399:
            arg0->unk_21CF8 = 1;
            break;
        case 0x423:
            if (Progress_Check(0x50) == 0) {
                Progress_SetGate(0x50);
                Progress_SetCurrentEvent(0x39A);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x39B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x39A:
        case 0x39B:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x39C;
            gSaveState.unk_20.unk_24C2 = 0x39D;
            gSaveState.unk_20.unk_341C = 12;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x39C:
            Progress_SetGate(0x44);
            func_ov030_020aac28(arg0, PIN_LADY_LUCK);
            arg0->unk_21CF8 = 1;
        case 0x39D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x424:
            if (Progress_Check(0x51) == 0) {
                Progress_SetGate(0x51);
                Progress_SetCurrentEvent(0x39E);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x39F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x39E:
        case 0x39F:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3A0;
            gSaveState.unk_20.unk_24C2 = 0x3A1;
            gSaveState.unk_20.unk_341C = 9;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A0:
            Progress_SetGate(0x45);
            func_ov030_020aac28(arg0, PIN_LOLITA_SKULL);
            arg0->unk_21CF8 = 1;
        case 0x3A1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x425:
            if (Progress_Check(0x52) == 0) {
                Progress_SetGate(0x52);
                Progress_SetCurrentEvent(0x3A2);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3A3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3A2:
        case 0x3A3:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3A4;
            gSaveState.unk_20.unk_24C2 = 0x3A5;
            gSaveState.unk_20.unk_341C = 13;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A4:
            Progress_SetGate(0x46);
            func_ov030_020aac28(arg0, PIN_LUCKY_PANDA);
            arg0->unk_21CF8 = 1;
        case 0x3A5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x426:
            if (Progress_Check(0x53) == 0) {
                Progress_SetGate(0x53);
                Progress_SetCurrentEvent(0x3A6);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3A7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3A6:
        case 0x3A7:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3A8;
            gSaveState.unk_20.unk_24C2 = 0x3A9;
            gSaveState.unk_20.unk_341C = 0x11;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A8:
            Progress_SetGate(8);
            func_ov030_020aac28(arg0, PIN_ONE_STROKE_VAST_WEALTH);
            arg0->unk_21CF8 = 1;
        case 0x3A9:
            arg0->unk_21CF8 = 1;
            break;
        case 0x427:
            if (Progress_Check(0x54) == 0) {
                Progress_SetGate(0x54);
                Progress_SetCurrentEvent(0x3AA);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3AB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3AA:
        case 0x3AB:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3AC;
            gSaveState.unk_20.unk_24C2 = 0x3AD;
            gSaveState.unk_20.unk_341C = 0xE;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3AC:
            Progress_SetGate(0x47);
            func_ov030_020aac28(arg0, PIN_LUCKY_STAR);
            arg0->unk_21CF8 = 1;
        case 0x3AD:
            arg0->unk_21CF8 = 1;
            break;
        case 0x428:
            if (Progress_Check(0x55) == 0) {
                Progress_SetGate(0x55);
                Progress_SetCurrentEvent(0x3AE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3AF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3AE:
        case 0x3AF:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3B0;
            gSaveState.unk_20.unk_24C2 = 0x3B1;
            gSaveState.unk_20.unk_341C = 18;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B0:
            Progress_SetGate(0x48);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            arg0->unk_21CF8 = 1;
        case 0x3B1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x429:
            if (Progress_Check(0x56) == 0) {
                Progress_SetGate(0x56);
                Progress_SetCurrentEvent(0x3B2);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3B3);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3B2:
        case 0x3B3:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3B4;
            gSaveState.unk_20.unk_24C2 = 0x3B5;
            gSaveState.unk_20.unk_341C = 15;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B4:
            Progress_SetGate(0x49);
            ProgressReward_GrantPin(arg0, PIN_10_YEN);
            arg0->unk_21CF8 = 1;
        case 0x3B5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42A:
            if (Progress_Check(0x57) == 0) {
                Progress_SetGate(0x57);
                Progress_SetCurrentEvent(0x3B6);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3B7);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3B6:
        case 0x3B7:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3B8;
            gSaveState.unk_20.unk_24C2 = 0x3B9;
            gSaveState.unk_20.unk_341C = 0x10;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B8:
            Progress_SetGate(0x4A);
            func_ov030_020aac28(arg0, PIN_LONG_LIVE_THE_ROCK);
            arg0->unk_21CF8 = 1;
        case 0x3B9:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42B:
            if (Progress_Check(0x58) == 0) {
                Progress_SetGate(0x58);
                Progress_SetCurrentEvent(0x3BA);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3BB);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3BA:
        case 0x3BB:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3BC;
            gSaveState.unk_20.unk_24C2 = 0x3BD;
            gSaveState.unk_20.unk_341C = 10;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3BC:
            Progress_SetGate(0x4B);
            func_ov030_020aac28(arg0, PIN_DISTORTION);
            arg0->unk_21CF8 = 1;
        case 0x3BD:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42C:
            if (Progress_Check(0x59) == 0) {
                Progress_SetGate(0x59);
                Progress_SetCurrentEvent(0x3BE);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3BF);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3BE:
        case 0x3BF:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3C0;
            gSaveState.unk_20.unk_24C2 = 0x3C1;
            gSaveState.unk_20.unk_341C = 0x14;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C0:
            Progress_SetGate(0x4C);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_SHIVA);
            arg0->unk_21CF8 = 1;
        case 0x3C1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42D:
            if (Progress_Check(0x5B) == 0) {
                Progress_SetGate(0x5B);
                Progress_SetCurrentEvent(0x3C3);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3C4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3C3:
        case 0x3C4:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3C5;
            gSaveState.unk_20.unk_24C2 = 0x3C6;
            gSaveState.unk_20.unk_341C = 0x13;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C5:
            Progress_SetGate(9);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_BLADE);
            arg0->unk_21CF8 = 1;
        case 0x3C6:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42E:
            if (Progress_Check(0x5C) == 0) {
                Progress_SetGate(0x5C);
                Progress_SetCurrentEvent(0x3C7);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x3C8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3C7:
        case 0x3C8:
            arg0->unk_21630            = 6;
            gSaveState.unk_20.unk_24C0 = 0x3C9;
            gSaveState.unk_20.unk_24C2 = 0x3CA;
            gSaveState.unk_20.unk_341C = 0x15;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C9:
            Progress_SetGate(0x4D);
            func_ov030_020aac28(arg0, PIN_TIN_PIN_DASH);
            arg0->unk_21CF8 = 1;
        case 0x3CA:
            arg0->unk_21CF8 = 1;
            break;
        case 0x360:
            gSaveState.unk_20.unk_2670 = 4;
            break;
        case 0x31C:
        case 0x31F:
        case 0x322:
            return func_ov030_020a9f54(arg0, func_ov030_0209efe4, 0x36E4, 2, 0x36E5, 0x36E6);
        case 0x367:
            if (Progress_Check(0x6D) != 0) {
                gSaveState.unk_20.unk_24B4          = 0;
                gSaveState.unk_20.unk_24B8          = 1;
                gSaveState.unk_20.currentStoryEvent = 0x36B;
                func_ov030_020af364(0x31);
                return 1;
            }
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = EVENT_ANOTHERDAY_FINALTIMEATTACK_REWARD;
            func_ov030_020af364(0x31);
            return 1;
        case EVENT_ANOTHERDAY_FINALTIMEATTACK_REWARD:
            Progress_SetGate(0x6D);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_THREAD_ANGEL_FEATHER);
            Inventory_AddItem(PIN_PYROKINESIS, 2);
            Inventory_AddItem(PIN_SHOCKWAVE, 2);
            Inventory_AddItem(PIN_CURE_DRINK, 2);
            Inventory_AddItem(PIN_FORCE_ROUNDS, 2);
            Inventory_AddItem(PIN_PSYCHOKINESIS, 2);
            Inventory_AddItem(PIN_THUNDERBOLT, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x356:
            gSaveState.unk_20.unk_26D2 = 0;
            gSaveState.unk_20.unk_2676 = 4;
            break;
        case 0x35E:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = EVENT_ANOTHERDAY_PORKCITY_ROOF_LEVEL;
            func_ov030_020af364(0x30);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_ROOF_LEVEL:
            Progress_SetGate(0x66);
            ProgressReward_GrantItem(arg0, ITEM_THREAD_MY_PHONES);
            gSaveState.unk_20.unk_26AA = 0;
            Progress_ClearGate(0x7E);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x157000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x83A:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x6E) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_1ST_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x6E) == 0)) {
                Progress_SetCurrentEvent(0x861);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x83B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_1ST_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_HAMBURGER);
        case 0x861:
            Progress_SetGate(10);
            gSaveState.unk_20.unk_267E = 4;
            gSaveState.unk_20.unk_267C = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x83D:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x6F) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_2ND_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x6F) == 0)) {
                Progress_SetCurrentEvent(0x862);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x83E);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_2ND_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_SPECIAL_DOG);
        case 0x862:
            Progress_SetGate(11);
            gSaveState.unk_20.unk_2680 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x840:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x70) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_3RD_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x70) == 0)) {
                Progress_SetCurrentEvent(0x863);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x841);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_3RD_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_DONUTS);
        case 0x863:
            Progress_SetGate(12);
            gSaveState.unk_20.unk_2682 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x843:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x71) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_4TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x71) == 0)) {
                Progress_SetCurrentEvent(0x864);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x844);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_4TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_SHADOW_STEAK_RAMEN);
        case 0x864:
            Progress_SetGate(13);
            gSaveState.unk_20.unk_2686 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x846:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x72) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_5TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x72) == 0)) {
                Progress_SetCurrentEvent(0x865);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x847);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_5TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_ATK_CAPSULES);
        case 0x865:
            Progress_SetGate(0xE);
            gSaveState.unk_20.unk_268A = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x849:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x73) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_6TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x73) == 0)) {
                Progress_SetCurrentEvent(0x866);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x84A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_6TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_DEF_MEDICINE);
        case 0x866:
            Progress_SetGate(15);
            gSaveState.unk_20.unk_268E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x84C:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x74) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_7TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x74) == 0)) {
                Progress_SetCurrentEvent(0x867);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x84D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_7TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_THE_TATSUMI_BURGER);
        case 0x867:
            Progress_SetGate(0x10);
            gSaveState.unk_20.unk_2692 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x84F:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x75) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_8TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x75) == 0)) {
                Progress_SetCurrentEvent(0x868);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x850);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_8TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_DEFINITIVO_CHILI_DOG);
        case 0x868:
            Progress_SetGate(0x11);
            gSaveState.unk_20.unk_2696 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x852:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x76) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_9TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x76) == 0)) {
                Progress_SetCurrentEvent(0x869);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x853);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_9TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_MYSTIC_RAMEN);
        case 0x869:
            Progress_SetGate(18);
            gSaveState.unk_20.unk_269A = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x855:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x77) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_10TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x77) == 0)) {
                Progress_SetCurrentEvent(0x86A);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x856);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_10TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_ABSOLUTE_SHADOW_RAMEN);
        case 0x86A:
            Progress_SetGate(0x13);
            gSaveState.unk_20.unk_269E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x858:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x78) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_11TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x78) == 0)) {
                Progress_SetCurrentEvent(0x86B);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x859);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_11TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_VIPER_DRINK);
        case 0x86B:
            Progress_SetGate(0x14);
            gSaveState.unk_20.unk_26A2 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x85B:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x79) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_12TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x79) == 0)) {
                Progress_SetCurrentEvent(0x86C);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x85C);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_12TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_MAKO_SYNERGY);
        case 0x86C:
            Progress_SetGate(0x15);
            gSaveState.unk_20.unk_26A6 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x85E:
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x7A) != 0)) {
                Progress_SetCurrentEvent(EVENT_ANOTHERDAY_PORKCITY_13TH_REWARD);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (Progress_Check(0x7A) == 0)) {
                Progress_SetCurrentEvent(0x86D);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetCurrentEvent(0x85F);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case EVENT_ANOTHERDAY_PORKCITY_13TH_REWARD:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_CURIOUS_MUSHROOMS);
        case 0x86D:
            Progress_SetGate(0x16);
            gSaveState.unk_20.unk_26AA = 2;
            arg0->unk_21CF8            = 1;
            break;
        case EVENT_ANOTHERDAY_SECRET_BOX_CONCERT_STAGE:
            Progress_SetGate(1);
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_GIRLS_UNIFORM_WITH_SWEATER, 1, 0) != 0) {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            } else {
                ProgressReward_GrantItem(arg0, ITEM_THREAD_GIRLS_UNIFORM_WITH_SWEATER);
            }
            arg0->unk_21CF8 = 1;
            break;
    }
    return 0;
}

void func_ov030_020a1ea8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x37;
}

s32 func_ov030_020a1ed0(ProgressObject* arg0) {
    if (gSaveState.unk_20.unk_2458 == 9) {
        if ((Progress_Check(3) != 0) && (Progress_Check(4) == 0)) {
            Progress_SetGate(4);
            Progress_BeginScriptedEvent(arg0, 0x592, 0);
            return 1;
        }
        if (Progress_Check(3) == 0) {
            Progress_SetGate(3);
            arg0->unk_21AF4 = 0x96;
            arg0->unk_21AF0 = 1;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 18) && (func_ov030_02084860(0x181, 0xC2, 0x14) != 0)) {
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

s32 func_ov030_020a1fdc(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x35EA:
            Progress_SetCurrentEvent(0x595);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35EB:
            Progress_SetCurrentEvent(0x596);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            Progress_SetCurrentEvent(0x598);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
}

s32 func_ov030_020a206c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x592:
            func_ov030_020aa9f4(arg0, 0x35EB, 0x5DD1, 0);
            break;
        case 0x594:
        case 0x598:
            return func_ov030_020aa0c8(arg0, 3, 0x598, 0x597, func_ov030_020a1fdc);
        case 0x597:
        default:
            break;
    }
    return 0;
}

void func_ov030_020a20f8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 56;
}

s32 func_ov030_020a2120(ProgressObject* progress) {
    if (gSaveState.unk_20.unk_2458 == 18) {
        if (Progress_Check(GATE_GAME_CLEARED)) {
            Progress_ClearGate(GATE_GAME_CLEARED);
            Progress_BeginScriptedEvent(progress, 0x599, 0);
            return 1;
        } else if (Progress_Check(1) != 0) {
            Progress_BeginScriptedEvent(progress, 0x59B, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020a21a4(ProgressObject* arg0) {
    if (gSaveState.unk_20.currentStoryEvent == 0x59A) {
        func_ov030_020b5d14(EasyTask_GetTaskById(&arg0->taskPool, arg0->unk_21788[gSaveState.unk_20.unk_24A4]), 1, 0x5DDB,
                            0x10000);
    }
    return 0;
}

void func_ov030_020a2208(ProgressObject* arg0) {
    func_ov030_020ae96c(23);
    gSaveState.unk_20.unk_244C          = 0x12C000;
    gSaveState.unk_20.unk_2450          = 0x87000;
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x886E;
    func_ov030_020c26bc(arg0);
    gSaveState.unk_20.unk_267A = 4;
}

s32 func_ov030_020a2268(ProgressObject* arg0) {
    s16 var_r0;

    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
            Progress_SetGate(GATE_GAME_CLEARED);
            Progress_BeginScriptedEvent(arg0, 0x86F, 0);
            return 1;
        }
        if (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) {
            var_r0 = gSaveState.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (Progress_Check(3) == 0)) {
            Progress_SetGate(3);
            gSaveState.unk_20.unk_2678 = 4;
            Progress_BeginScriptedEvent(arg0, 0x874, 0);
            return 1;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 0x16) {
        if (Progress_Check(4) == 0) {
            Progress_SetGate(4);
            Progress_BeginScriptedEvent(arg0, 0x877, 0);
            return 1;
        } else if ((Progress_Check(0xE) != 0) && (Progress_Check(6) == 0)) {
            Progress_SetGate(6);
            Progress_BeginScriptedEvent(arg0, 0x880, 0);
            return 1;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 23) && (func_ov030_020aed9c(0x3715) != 0) && (Progress_Check(15) == 0)) {
        Progress_SetGate(15);
        Progress_BeginScriptedEvent(arg0, 0x87F, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_020a240c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.unk_24C8[gSaveState.unk_20.unk_24C4]) {
        case 0x35F9:
            if (Progress_Check(15) != 0) {
                Progress_SetCurrentEvent(0x87E);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            Progress_SetGate(5);
            Progress_SetCurrentEvent(0x87D);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FA:
            Progress_SetGate(42);
            Progress_SetCurrentEvent(0x888);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            Progress_SetCurrentEvent(0x887);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
}

s32 func_ov030_020a24d8(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            Progress_SetGate(9);
            Progress_SetCurrentEvent(0x879);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3606:
            Progress_SetGate(10);
            Progress_SetCurrentEvent(0x87A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020a2568(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            Progress_SetGate(9);
            Progress_SetCurrentEvent(0x879);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3606:
            Progress_SetGate(10);
            Progress_SetCurrentEvent(0x87A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x3607:
            Progress_SetGate(11);
            Progress_SetCurrentEvent(0x87B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020a2628(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x86E:
            Progress_SetCurrentEvent(0x889);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x86F:
            Progress_SetCurrentEvent(0x88A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x88A:
            ProgressReward_GrantPin(arg0, PIN_PYROKINESIS);
            ProgressReward_GrantPin(arg0, PIN_PSYCHOKINESIS);
            ProgressReward_GrantPin(arg0, PIN_THUNDERBOLT);
            ProgressReward_GrantPin(arg0, PIN_SHOCKWAVE);
            ProgressReward_GrantPin(arg0, PIN_CURE_DRINK);
            break;
        case 0x870:
            Progress_SetGate(1);
            arg0->unk_21CF8 = 1;
            break;
        case 0x872:
            Progress_SetGate(2);
            gSaveState.unk_20.unk_30FC = 0;
            gSaveState.unk_20.unk_30FE = 39;
            gSaveState.unk_20.unk_3100 = gSaveState.unk_20.unk_2458;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            ProgressReward_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x874:
            arg0->unk_21CF8 = 1;
            break;
        case 0x878:
        case 0x879:
        case 0x87A:
        case 0x87B:
            if ((Progress_Check(9) != 0) && (Progress_Check(10) != 0) && (Progress_Check(11) != 0)) {
                Progress_SetCurrentEvent(0x87C);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            if (Progress_Check(9) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_020a2568, 0x3604, 3, 0x3605, 0x3606, 0x3607);
            }
            return func_ov030_020a9f54(arg0, func_ov030_020a24d8, 0x3604, 2, 0x3605, 0x3606);
        case 0x87C:
            Progress_SetGate(12);
            arg0->unk_21CF8 = 1;
            func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
            break;
        case 0x885:
        case 0x886:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x886, 0x887, func_ov030_020a240c);
        case 0x888:
            Progress_SetGate(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x880:
            arg0->unk_21CF8 = 1;
            break;
        case 0x87F:
            Progress_SetGate(15);
            func_ov030_020aa9f4(arg0, 0x35FA, 0x5DD1, 0);
            break;
        case 0x881:
            Progress_SetGate(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x882:
            Progress_SetGate(8);
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x883;
            func_ov030_020af364(10);
            return 1;
        case 0x883:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 0x884;
            gSaveState.unk_20.unk_3124          = 10;
            gSaveState.unk_20.unk_3128          = 0;
            arg0->unk_21630                     = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x884:
            gSaveState.unk_20.unk_3124 = 0x4D;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
            DebugOvlDisp_Pop();
            return 0;
    }
    return 0;
}

void func_ov030_020a2a44(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 58;
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_020a2a84(void) {
    return 0;
}

s32 func_ov030_020a2a8c(ProgressObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 0:
            Progress_SetCurrentEvent(0x44E);
            return 1;
        case 1:
            if (Progress_Check(2) != 0) {
                func_ov030_020a9db8(arg0, 0x45F);
                return 0;
            }
            Progress_SetGate(2);
            Progress_SetCurrentEvent(0x451);
            return 1;
        case 2:
            Progress_SetCurrentEvent(0x452);
            return 1;
        case 3:
            Progress_SetCurrentEvent(0x455);
            return 1;
        case 4:
            Progress_SetCurrentEvent(0x456);
            return 1;
        case 5:
            Progress_SetCurrentEvent(0x457);
            return 1;
        case 6:
            Progress_SetCurrentEvent(0x458);
            return 1;
        case 7:
            Progress_SetCurrentEvent(0x459);
            return 1;
        case 8:
            Progress_SetCurrentEvent(0x45A);
            return 1;
        case 9:
            Progress_SetCurrentEvent(0x45B);
            return 1;
        case 10:
            Progress_SetCurrentEvent(0x45C);
            return 1;
        case 11:
            Progress_SetCurrentEvent(0x45D);
            return 1;
        case 13:
            func_ov030_020a9db8(arg0, 0x45E);
            return 0;
        default:
        case 12:
            return 0;
    }
}

s32 func_ov030_020a2c20(ProgressObject* arg0) {
    if ((gSaveState.unk_20.currentStoryEvent == 0xE3) || (gSaveState.unk_20.currentStoryEvent == 0x45F)) {
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
    gSaveState.unk_20.unk_267C = 4;
    gSaveState.unk_20.unk_2680 = 4;
    gSaveState.unk_20.unk_2682 = 4;
    func_ov030_020aec38(1);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_020a2d54(void) {
    return 0;
}

s32 func_ov030_020a2d5c(ProgressObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35FA:
            Progress_SetCurrentEvent(0x5A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x35FB:
            Progress_SetCurrentEvent(0x5A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        default:
            Progress_SetCurrentEvent(0x5A5);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
}

s32 func_ov030_020a2de8(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x5A0:
            func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
            break;
        case 0x5A3:
            return func_ov030_020a9f54(arg0, func_ov030_020a2d5c, 0x35F9, 2, 0x35FA, 0x35FB);
        case 0x5A6:
            ProgressReward_GrantItem(arg0, ITEM_THREAD_BLACK_BOOK_BAG);
            ProgressReward_GrantItem(arg0, ITEM_THREAD_FALSE_TEETH);
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
            Progress_SetCurrentEvent(0x5A8);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x5A9:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            Progress_SetCurrentEvent(0x5AA);
            Progress_AdvanceEventScript(arg0);
            return 1;
    }
    return 0;
}

void func_ov030_020a2f74(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124          = 0x3C;
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x8A;
}

// Nonmatching
s32 func_ov030_020a2fc4(void) {
    if (gSaveState.unk_20.unk_2458 == 18) {
        if ((Progress_Check(15) != 0) && (Progress_Check(2) == 0)) {
            Progress_SetGate(2);
        }

        s32 var_r0 = (gSaveState.unk_20.unk_3100 == gSaveState.unk_20.unk_2458) ? gSaveState.unk_20.unk_30FC : 0;
        if ((var_r0 >= 2) && (Progress_Check(1) == 0)) {
            Progress_SetGate(1);
            Progress_SetGate(0x1E);
            Progress_SetGate(0x1F);
            Progress_SetGate(0x20);
        }
    }
    return 0;
}

s32 func_ov030_020a306c(ProgressObject* arg0) {
    if ((gSaveState.unk_20.currentStoryEvent == 0x8A) && (Progress_Check(GATE_GAME_CLEARED) == FALSE)) {
        Progress_SetGate(GATE_GAME_CLEARED);
    }
    return 0;
}

void func_ov030_020a30a4(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 61;
}

s32 func_ov030_020a30cc(ProgressObject* arg0) {
    return 0;
}

s32 func_ov030_020a30d4(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x5B3:
            Progress_SetCurrentEvent(0x5B4);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x5B4:
            Progress_SetCurrentEvent(0x5B5);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x5B5:
            Progress_SetCurrentEvent(0x5B6);
            Progress_AdvanceEventScript(arg0);
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

s32 func_ov030_020a317c(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3184(void) {
    func_ov030_020aec1c(4);
    gSaveState.unk_20.unk_26D4 = 0;
    func_ov030_020ae96c(0x10);
    func_ov030_020c26bc(4);
}

s32 func_ov030_020a31b4(void) {
    return 0;
}

s32 func_ov030_020a31bc(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a31c4(void) {
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_24B4          = 0;
    gSaveState.unk_20.unk_24B8          = 1;
    gSaveState.unk_20.currentStoryEvent = 0x88B;
    func_ov030_020aec1c(4);
    gSaveState.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
}

s32 func_ov030_020a321c(s32 arg0) {
    if ((gSaveState.unk_20.unk_2458 == 1) && Progress_Check(GATE_GAME_CLEARED) && (Progress_Check(2) == 0)) {
        Progress_SetGate(2);
        Progress_BeginScriptedEvent(arg0, 0x88E, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 15) && (Progress_Check(10) != 0) && (Progress_Check(11) != 0) &&
               (Progress_Check(12) != 0) && (Progress_Check(7) == 0))
    {
        Progress_SetGate(7);
        Progress_BeginScriptedEvent(arg0, 0x896, 0);
        return 1;
    } else if ((gSaveState.unk_20.unk_2458 == 0x10) && (Progress_Check(9) == 0)) {
        Progress_SetGate(9);
        Progress_BeginScriptedEvent(arg0, 0x899, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_020a333c(ProgressObject* arg0) {
    switch (gSaveState.unk_20.currentStoryEvent) {
        case 0x88C:
            Progress_SetGate(GATE_GAME_CLEARED);
            Progress_SetCurrentEvent(0x89A);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x89A:
            ProgressReward_GrantPin(arg0, PIN_LOVELY_BEAM);
            ProgressReward_GrantPin(arg0, PIN_ICHIMONJI);
            ProgressReward_GrantPin(arg0, PIN_ONE_STONE_MANY_BIRDS);
            ProgressReward_GrantPin(arg0, PIN_EARTHSHAKE);
            ProgressReward_GrantPin(arg0, PIN_MASAMUNE);
            ProgressReward_GrantPin(arg0, PIN_HEALING_BUNNY);
            arg0->unk_21CF8 = 1;
            break;
        case 0x88E:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 2189;
            func_ov030_020af364(0x39);
            return 1;
        case 0x88D:
            Progress_SetGate(1);
            gSaveState.unk_20.unk_264E = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x88F:
            Progress_SetCurrentEvent(0x89B);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x89B:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 2192;
            func_ov030_020af364(0x35);
            return 1;
        case 0x890:
            Progress_SetGate(3);
            gSaveState.unk_20.unk_2664 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x891:
            Progress_SetCurrentEvent(2204);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x89C:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 2194;
            func_ov030_020af364(54);
            return 1;
        case 0x892:
            Progress_SetGate(4);
            gSaveState.unk_20.unk_2666 = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x893:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 2196;
            func_ov030_020af364(55);
            return 1;
        case 0x894:
            Progress_SetGate(5);
            gSaveState.unk_20.unk_2668 = 4;
            Progress_SetCurrentEvent(2205);
            Progress_AdvanceEventScript(arg0);
            return 1;
        case 0x89D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x895:
            if (Progress_Check(6) == 0) {
                Progress_SetGate(6);
                gSaveState.unk_20.unk_310C = &data_ov030_020d9be4;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
                Progress_SetCurrentEvent(2206);
                Progress_AdvanceEventScript(arg0);
                return 1;
            }
            break;
        case 0x89E:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            break;
        case 0x896:
            Progress_SetGate(7);
            gSaveState.unk_20.unk_266A = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x897:
            gSaveState.unk_20.unk_24B4          = 0;
            gSaveState.unk_20.unk_24B8          = 1;
            gSaveState.unk_20.currentStoryEvent = 2200;
            func_ov030_020af364(56);
            return 1;
        case 0x898:
            Progress_SetGate(8);
            gSaveState.unk_20.unk_266C = 4;
            arg0->unk_21CF8            = 1;
            break;
        case 0x899:
            gSaveState.unk_20.unk_3124 = 0x4D;
            gSaveState.unk_20.unk_3128 = 1;
            arg0->unk_21630            = 4;
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
    if (gSaveState.unk_20.unk_2458 == 0x10) {
        func_ov030_020848e4(arg0, 0x44);
    }
    return 0;
}

s32 func_ov030_020a3718(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3720(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x42;
}

s32 func_ov030_020a3748(void) {
    return 0;
}

s32 func_ov030_020a3750(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3758(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x43;
}

s32 func_ov030_020a3780(void) {
    return 0;
}

s32 func_ov030_020a3788(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3790(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x44;
}

s32 func_ov030_020a37b8(void) {
    return 0;
}

s32 func_ov030_020a37c0(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a37c8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x45;
}

s32 func_ov030_020a37f0(void) {
    return 0;
}

s32 func_ov030_020a37f8(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3800(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x46;
}

s32 func_ov030_020a3828(void) {
    return 0;
}

s32 func_ov030_020a3830(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3838(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
}

s32 func_ov030_020a3850(void) {
    return 0;
}

s32 func_ov030_020a3858(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3860(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x48;
}

s32 func_ov030_020a3888(void) {
    return 0;
}

s32 func_ov030_020a3890(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3898(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x49;
}

s32 func_ov030_020a38c0(void) {
    return 0;
}

s32 func_ov030_020a38c8(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a38d0(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x4A;
}

s32 func_ov030_020a38f8(void) {
    return 0;
}

s32 func_ov030_020a3900(ProgressObject* arg0) {
    return 0;
}

void func_ov030_020a3908(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    gSaveState.unk_20.unk_3124 = 0x4B;
}

s32 func_ov030_020a3930(void) {
    return 0;
}

s32 func_ov030_020a3938(ProgressObject* arg0) {
    return 0;
}

s32 func_ov030_020a3940(void) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        return 0x28;
    } else if (gSaveState.unk_20.unk_2458 == 3) {
        return 41;
    } else {
        return 0;
    }
}

s32 func_ov030_020a3968(void) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (Progress_Check(33) == 0) {
            return 0x5C0;
        } else if ((Progress_Check(38) != 0) && (Progress_Check(0x28) == 0)) {
            return 0x5BF;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 5) && (Progress_Check(38) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x5BF;
    } else {
        return 0;
    }
}

s32 func_ov030_020a39f8(void) {
    if (gSaveState.unk_20.unk_2458 == 5) {
        if ((Progress_Check(45) != 0) && (Progress_Check(6) == 0)) {
            return 0x5C8;
        } else if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
            return 0x5CC;
        }
    }

    if (Progress_Check(0x20) == 0) {
        return 0x5CD;
    } else if ((Progress_Check(0x20) != 0) && (Progress_Check(33) == 0)) {
        return 0x5C4;
    } else if ((Progress_Check(33) != 0) && (Progress_Check(3) == 0)) {
        return 0x5C5;
    } else if ((Progress_Check(0x22) != 0) && (Progress_Check(42) == 0)) {
        return 0x5CB;
    } else if ((Progress_Check(42) != 0) && (Progress_Check(43) == 0) && (Progress_Check(5) == 0)) {
        return 0x5C6;
    } else if ((Progress_Check(43) != 0) && (Progress_Check(5) == 0)) {
        return 0x5C7;
    } else if ((Progress_Check(5) != 0) && (Progress_Check(6) == 0)) {
        return 0x5CE;
    } else if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
        return 0x5CF;
    } else if ((Progress_Check(7) != 0) && (Progress_Check(47) == 0)) {
        return 0x5C9;
    } else if ((Progress_Check(47) != 0) && (Progress_Check(8) == 0)) {
        return 0x5CA;
    } else {
        return 0;
    }
}

s32 func_ov030_020a3bf8(void) {
    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((Progress_Check(5) != 0) && (Progress_Check(0x3F) != 0) && (Progress_Check(0x24) == 0)) {
            return 0x5DC;
        } else if ((Progress_Check(7) != 0) && (Progress_Check(8) == 0)) {
            return 0x5E9;
        } else if ((Progress_Check(8) != 0) && (Progress_Check(2) == 0)) {
            return 0x5DD;
        } else if ((Progress_Check(9) != 0) && (Progress_Check(10) == 0)) {
            return 0x5DE;
        } else if ((Progress_Check(10) != 0) && (Progress_Check(11) == 0)) {
            return 0x5DF;
        } else if ((Progress_Check(0xE) != 0) && (Progress_Check(0x10) == 0) && (Progress_Check(41) == 0) &&
                   (Progress_Check(0x13) == 0))
        {
            return 0x5E7;
        } else if ((Progress_Check(0xE) != 0) && (Progress_Check(13) != 0) && (Progress_Check(0x10) == 0)) {
            return 0x5E7;
        } else if ((Progress_Check(0x10) != 0) && (Progress_Check(15) == 0)) {
            return 0x5E8;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if ((Progress_Check(0x20) != 0) && (Progress_Check(5) == 0) && (Progress_Check(10) == 0)) {
            return 0x5D9;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(0x24) == 0)) {
            return 0x5DA;
        } else if ((Progress_Check(9) != 0) && (Progress_Check(10) == 0)) {
            return 0x5F2;
        } else if ((Progress_Check(0x42) != 0) && (Progress_Check(0x43) == 0)) {
            return 0x5DB;
        } else if ((Progress_Check(0x43) != 0) && (Progress_Check(4) == 0)) {
            return 0x5F1;
        } else if ((Progress_Check(10) != 0) && (Progress_Check(11) == 0)) {
            return 0x5DF;
        } else if ((Progress_Check(0xE) != 0) && (Progress_Check(0x10) == 0) && (Progress_Check(41) == 0) &&
                   (Progress_Check(0x13) == 0))
        {
            return 0x5E6;
        } else if ((Progress_Check(15) != 0) && (Progress_Check(0x11) == 0)) {
            return 0x5EA;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x16) {
        if ((Progress_Check(0x32) != 0) && (Progress_Check(0x33) == 0)) {
            return 0x5EC;
        } else if ((Progress_Check(0x33) != 0) && (Progress_Check(6) == 0)) {
            return 0x5E0;
        } else if ((Progress_Check(11) != 0) && (Progress_Check(0x4A) == 0) && (Progress_Check(37) == 0)) {
            return 0x5E2;
        } else if ((Progress_Check(0xE) != 0) && (Progress_Check(0x10) == 0) && (Progress_Check(41) == 0) &&
                   (Progress_Check(0x13) == 0))
        {
            return 0x5E6;
        } else if ((Progress_Check(0x10) != 0) && (Progress_Check(15) == 0)) {
            return 0x5E8;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 23) {
        if ((Progress_Check(45) != 0) && (Progress_Check(0x20) == 0)) {
            return 0x5D8;
        } else if ((Progress_Check(9) != 0) && (Progress_Check(10) == 0)) {
            return 0x5EB;
        } else if ((Progress_Check(0x10) != 0) && (Progress_Check(15) == 0)) {
            return 0x5EB;
        } else if (Progress_Check(15) != 0) {
            return 0x5ED;
        } else if ((Progress_Check(0x4A) != 0) && (Progress_Check(38) == 0)) {
            return 0x5E4;
        } else if ((Progress_Check(38) != 0) && (Progress_Check(0xE) == 0)) {
            return 0x5E5;
        } else if (Progress_Check(45) != 0) {
            return 0x5EF;
        }
    }

    if ((Progress_Check(7) != 0) && (Progress_Check(9) == 0)) {
        return 0x5F0;
    } else if ((Progress_Check(11) != 0) && (Progress_Check(0x4A) == 0) && (Progress_Check(37) == 0)) {
        return 0x5F3;
    } else if ((Progress_Check(0x4A) != 0) && (Progress_Check(38) == 0)) {
        return 0x5E3;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(38) == 0) && (Progress_Check(13) == 0)) {
        return 0x5EE;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(38) != 0) && (Progress_Check(13) == 0)) {
        return 0x5E5;
    } else if ((Progress_Check(38) != 0) && (Progress_Check(0xE) == 0)) {
        return 0x5E5;
    } else if ((Progress_Check(38) != 0) && (Progress_Check(0xE) != 0) && (Progress_Check(12) != 0) &&
               (Progress_Check(13) == 0))
    {
        return 0x5E5;
    } else if ((Progress_Check(15) != 0) && (Progress_Check(0x11) == 0)) {
        return 0x5EA;
    } else {
        return 0;
    }
}

s32 func_ov030_020a428c(void) {
    if (gSaveState.unk_20.unk_2458 == 3) {
        if (Progress_Check(4) == 0) {
            return 0x5F6;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(33) == 0)) {
            return 0x5F7;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(47) != 0) && (Progress_Check(0x34) == 0)) {
            return 0x5F8;
        } else if ((Progress_Check(0x34) != 0) && (Progress_Check(2) == 0)) {
            return 0x5FA;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 11) {
        if ((Progress_Check(0x30) != 0) && (Progress_Check(0x31) == 0)) {
            return 0x5F9;
        } else if ((Progress_Check(0x31) != 0) && (Progress_Check(3) == 0)) {
            return 0x5FB;
        }
    }

    if ((Progress_Check(33) != 0) && (Progress_Check(6) == 0)) {
        return 0x5FC;
    } else {
        return 0;
    }
}

s32 func_ov030_020a43d4(void) {
    if (gSaveState.unk_20.unk_2458 == 6) {
        if ((Progress_Check(0x43) != 0) && (Progress_Check(0x44) == 0)) {
            return 0x60A;
        } else if ((Progress_Check(0x44) != 0) && (Progress_Check(3) == 0)) {
            return 0x60F;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0) {
        if ((Progress_Check(0x20) != 0) && (Progress_Check(5) == 0)) {
            return 0x60B;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(6) == 0)) {
            return 0x60C;
        } else if ((Progress_Check(9) != 0) && (Progress_Check(0x23) == 0)) {
            return 0x60E;
        }
    }
    if ((Progress_Check(6) != 0) && (Progress_Check(8) == 0)) {
        return 0x60D;
    } else {
        return 0;
    }
}

s32 func_ov030_020a44f4(void) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(11) != 0) && (Progress_Check(12) == 0)) {
            return 0x61F;
        } else if ((Progress_Check(12) != 0) && (Progress_Check(13) == 0)) {
            return 0x621;
        } else if ((Progress_Check(13) != 0) && (Progress_Check(0xE) == 0)) {
            return 0x622;
        } else if ((Progress_Check(0xE) != 0) && (Progress_Check(33) == 0)) {
            return 0x623;
        } else if ((Progress_Check(15) != 0) && (Progress_Check(0x10) == 0)) {
            return 0x626;
        } else if ((Progress_Check(0x10) != 0) && (Progress_Check(0x11) == 0)) {
            return 0x627;
        } else if ((Progress_Check(0x11) != 0) && (Progress_Check(18) == 0)) {
            return 0x628;
        } else if ((Progress_Check(18) != 0) && (Progress_Check(0x13) == 0)) {
            return 0x629;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 4) {
        if ((Progress_Check(33) != 0) && (Progress_Check(10) == 0)) {
            return 0x624;
        } else if ((Progress_Check(47) != 0) && (Progress_Check(0x33) == 0)) {
            return 0x62C;
        } else if ((Progress_Check(0x33) != 0) && (Progress_Check(4) == 0)) {
            return 0x62D;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 0) && (Progress_Check(0x23) != 0) && (Progress_Check(9) == 0)) {
        return 0x624;
    } else if ((gSaveState.unk_20.unk_2458 == 3) && (Progress_Check(0x14) != 0) && (Progress_Check(0x15) == 0)) {
        return 0x62B;
    } else if ((gSaveState.unk_20.unk_2458 == 2) && (Progress_Check(10) != 0) && (Progress_Check(0x23) == 0)) {
        return 0x62E;
    } else if ((gSaveState.unk_20.unk_2458 == 6) && (Progress_Check(10) != 0) && (Progress_Check(0x23) == 0)) {
        return 0x62E;
    } else if (Progress_Check(11) == 0) {
        return 0x61E;
    } else if ((Progress_Check(11) != 0) && (Progress_Check(12) == 0)) {
        return 0x620;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(0xE) == 0)) {
        return 0x631;
    } else if ((Progress_Check(0xE) != 0) && (Progress_Check(33) == 0)) {
        return 0x623;
    } else if ((Progress_Check(10) != 0) && (Progress_Check(0x23) == 0)) {
        return 0x625;
    } else if ((Progress_Check(0x11) != 0) && (Progress_Check(18) == 0)) {
        return 0x62F;
    } else if ((Progress_Check(18) != 0) && (Progress_Check(0x13) == 0)) {
        return 0x630;
    } else if ((Progress_Check(0x13) != 0) && (Progress_Check(0x14) == 0)) {
        return 0x62A;
    } else {
        return 0;
    }
}

s32 func_ov030_020a48d8(void) {
    if (gSaveState.unk_20.unk_2458 == 5) {
        if ((Progress_Check(43) != 0) && (Progress_Check(0x2C) == 0)) {
            return 0x633;
        } else if ((Progress_Check(0x2C) != 0) && (Progress_Check(5) == 0)) {
            return 0x634;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(0x20) == 0)) {
            return 0x635;
        }
    }
    if ((gSaveState.unk_20.unk_2458 == 0x11) && (Progress_Check(5) != 0) && (Progress_Check(0x20) == 0)) {
        return 0x635;
    } else if (Progress_Check(5) != 0) {
        return 0;
    } else
        return 0x632;
}

s32 func_ov030_020a49b8(void) {
    if ((gSaveState.unk_20.unk_2458 == 1) && (Progress_Check(0x23) != 0) && (Progress_Check(37) == 0)) {
        return 0x646;
    } else if ((gSaveState.unk_20.unk_2458 == 5) && (Progress_Check(0x23) != 0) && (Progress_Check(37) == 0)) {
        return 0x646;
    } else {
        return 0;
    }
}

s32 func_ov030_020a4a34(void) {
    if ((gSaveState.unk_20.unk_2458 == 1) && (Progress_Check(0x20) != 0) && (Progress_Check(0x22) == 0)) {
        return 0x64B;
    }
    if (gSaveState.unk_20.unk_2458 == 11) {
        if ((Progress_Check(4) != 0) && (Progress_Check(46) == 0)) {
            return 0x64D;
        } else if ((Progress_Check(46) != 0) && (Progress_Check(3) == 0)) {
            return 0x651;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 12) && (Progress_Check(3) != 0) && (Progress_Check(37) == 0)) {
        return 0x64E;
    } else if ((gSaveState.unk_20.unk_2458 == 8) && (Progress_Check(37) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x650;
    } else if ((Progress_Check(7) != 0) && (Progress_Check(37) == 0)) {
        return 0x64C;
    } else if ((Progress_Check(37) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x64F;
    } else {
        return 0;
    }
}

s32 func_ov030_020a4b98(void) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(0x20) == 0) && (Progress_Check(33) == 0)) {
            return 0x65F;
        } else if ((Progress_Check(10) != 0) && (Progress_Check(47) == 0)) {
            return 0x668;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 3) && (Progress_Check(10) != 0) && (Progress_Check(47) == 0)) {
        return 0x668;
    }
    if (gSaveState.unk_20.unk_2458 == 12) {
        if ((Progress_Check(33) != 0) && (Progress_Check(0x46) == 0)) {
            return 0x660;
        } else if ((Progress_Check(0x46) != 0) && (Progress_Check(0x47) == 0)) {
            return 0x661;
        } else if ((Progress_Check(0x47) != 0) && (Progress_Check(5) == 0)) {
            return 0x672;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 13) && (Progress_Check(5) != 0) && (Progress_Check(0x48) == 0)) {
        return 0x662;
    }
    if (gSaveState.unk_20.unk_2458 == 15) {
        if ((Progress_Check(5) != 0) && (Progress_Check(0x48) == 0)) {
            return 0x662;
        } else if ((Progress_Check(0x48) != 0) && (Progress_Check(0x5C) == 0)) {
            return 0x663;
        } else if ((Progress_Check(0x5C) != 0) && (Progress_Check(6) == 0)) {
            return 0x673;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 0xE) && (Progress_Check(6) != 0) && (Progress_Check(0x22) == 0)) {
        return 0x662;
    }
    if (gSaveState.unk_20.unk_2458 == 0x13) {
        if ((Progress_Check(0x30) != 0) && (Progress_Check(0x32) == 0)) {
            return 0x66A;
        } else if ((Progress_Check(0x32) != 0) && (Progress_Check(0x35) == 0)) {
            return 0x66C;
        } else if ((Progress_Check(0x35) != 0) && (Progress_Check(12) == 0)) {
            return 0x66D;
        } else if ((Progress_Check(0x42) != 0) && (Progress_Check(0x3E) == 0)) {
            return 0x671;
        } else if ((Progress_Check(0x43) != 0) && (Progress_Check(0x39) == 0)) {
            return 0x671;
        } else if ((Progress_Check(0x44) != 0) && (Progress_Check(0x3B) == 0)) {
            return 0x671;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if ((Progress_Check(0x3C) != 0) && (Progress_Check(15) == 0)) {
            return 0x66F;
        } else if ((Progress_Check(15) != 0) && (Progress_Check(0x10) == 0)) {
            return 0x670;
        }
    }
    if ((Progress_Check(0x20) == 0) && (Progress_Check(33) == 0)) {
        return 0x65F;
    } else if ((Progress_Check(0x22) != 0) && (Progress_Check(0x23) == 0)) {
        return 0x664;
    } else if ((Progress_Check(0x23) != 0) && (Progress_Check(9) == 0)) {
        return 0x665;
    } else if ((Progress_Check(9) != 0) && (Progress_Check(10) == 0)) {
        return 0x666;
    } else if ((Progress_Check(10) != 0) && (Progress_Check(47) == 0)) {
        return 0x667;
    } else if ((Progress_Check(47) != 0) && (Progress_Check(0x30) == 0)) {
        return 0x669;
    } else if ((Progress_Check(0x30) != 0) && (Progress_Check(12) == 0)) {
        return 0x66B;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(0x3E) == 0)) {
        return 0x66E;
    }
    return 0;
}

s32 func_ov030_020a5040(void) {
    if (gSaveState.unk_20.unk_2458 == 4) {
        if ((Progress_Check(56) != 0) && (Progress_Check(61) == 0)) {
            return 0x68E;
        } else if ((Progress_Check(61) != 0) && (Progress_Check(2) == 0)) {
            return 0x691;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 2) {
        if ((Progress_Check(0x39) != 0) && (Progress_Check(58) == 0)) {
            return 0x679;
        } else if ((Progress_Check(58) != 0) && (Progress_Check(3) == 0)) {
            return 0x692;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 6) {
        if ((Progress_Check(39) != 0) && (Progress_Check(9) == 0)) {
            return 0x67B;
        } else if ((Progress_Check(10) != 0) && (Progress_Check(42) == 0)) {
            return 0x67D;
        } else if ((Progress_Check(12) != 0) && (Progress_Check(13) != 0) && (Progress_Check(15) == 0)) {
            return 0x687;
        } else if (Progress_Check(15) != 0) {
            return 1672;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 8) && (Progress_Check(33) != 0) && (Progress_Check(0xE) == 0)) {
        return 0x684;
    } else if ((gSaveState.unk_20.unk_2458 == 7) && (Progress_Check(0x23) != 0)) {
        return 0x689;
    } else if (gSaveState.unk_20.unk_2458 == 9) {
        if ((Progress_Check(37) != 0) && (Progress_Check(6) == 0)) {
            return 0x68A;
        } else if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
            return 0x68C;
        } else if (Progress_Check(7) != 0) {
            return 0x68B;
        }
    }

    if (Progress_Check(39) == 0) {
        return 0x678;
    } else if ((Progress_Check(39) != 0) && (Progress_Check(9) == 0)) {
        return 0x67A;
    } else if ((Progress_Check(9) != 0) && (Progress_Check(10) == 0)) {
        return 0x67C;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(13) != 0) && (Progress_Check(15) == 0)) {
        return 0x686;
    } else if ((Progress_Check(46) != 0) && (Progress_Check(0xE) == 0)) {
        return 0x68D;
    } else if ((Progress_Check(46) != 0) && (Progress_Check(0xE) != 0) && (Progress_Check(12) == 0)) {
        return 0x685;
    } else if ((Progress_Check(42) != 0) && (Progress_Check(45) == 0) && (Progress_Check(47) == 0)) {
        return 0x67E;
    } else if ((Progress_Check(42) != 0) && (Progress_Check(45) != 0) && (Progress_Check(47) == 0) &&
               (Progress_Check(46) == 0))
    {
        return 0x67F;
    } else if ((Progress_Check(42) != 0) && (Progress_Check(45) == 0) && (Progress_Check(47) != 0) &&
               (Progress_Check(13) == 0))
    {
        return 0x680;
    }
    if ((Progress_Check(42) != 0) && (Progress_Check(45) != 0) && (Progress_Check(47) != 0) && (Progress_Check(13) == 0) &&
        (Progress_Check(46) != 0) && (Progress_Check(12) == 0))
    {
        return 0x683;
    } else if ((Progress_Check(42) != 0) && (Progress_Check(45) != 0) && (Progress_Check(47) != 0) &&
               (Progress_Check(13) == 0) && (Progress_Check(46) == 0))
    {
        return 0x681;
    } else if ((Progress_Check(42) != 0) && (Progress_Check(45) != 0) && (Progress_Check(47) != 0) &&
               (Progress_Check(13) != 0) && (Progress_Check(46) == 0))
    {
        return 1666;
    } else if ((Progress_Check(13) != 0) && (Progress_Check(45) == 0)) {
        return 1683;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(47) == 0)) {
        return 1679;
    } else if ((Progress_Check(12) != 0) && (Progress_Check(47) != 0) && (Progress_Check(13) == 0)) {
        return 1680;
    } else if (Progress_Check(15) != 0) {
        return 1672;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5608(void) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if ((Progress_Check(0x2C) != 0) && (Progress_Check(0x46) == 0)) {
            return 0x698;
        }
        if ((Progress_Check(0x46) != 0) && (Progress_Check(3) == 0)) {
            return 0x6A6;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 5) && (Progress_Check(39) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x6A3;
    }
    if ((gSaveState.unk_20.unk_2458 == 0x11) && (Progress_Check(39) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x6A3;
    }
    if (gSaveState.unk_20.unk_2458 == 18) {
        if ((Progress_Check(39) != 0) && (Progress_Check(0x28) == 0)) {
            return 0x6A4;
        }
        if ((Progress_Check(0x28) != 0) && (Progress_Check(42) == 0)) {
            return 0x6A5;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 15) && (Progress_Check(0x31) != 0) && (Progress_Check(7) == 0)) {
        return 0x69C;
    }
    if ((gSaveState.unk_20.unk_2458 == 0xE) && (Progress_Check(0x32) != 0) && (Progress_Check(8) == 0)) {
        return 0x69D;
    }
    if ((Progress_Check(45) != 0) && (Progress_Check(4) == 0)) {
        return 0x699;
    }
    if ((Progress_Check(47) != 0) && (Progress_Check(5) == 0)) {
        return 0x69A;
    }
    if ((Progress_Check(0x30) != 0) && (Progress_Check(0x4A) == 0)) {
        return 0x6A9;
    }
    if ((Progress_Check(0x4A) != 0) && (Progress_Check(6) == 0)) {
        return 0x6A9;
    }
    if ((Progress_Check(0x22) == 0) && (Progress_Check(2) == 0)) {
        return 0x696;
    }
    if ((Progress_Check(2) != 0) && (Progress_Check(8) == 0)) {
        return 0x697;
    }
    if ((Progress_Check(8) != 0) && (Progress_Check(0x22) == 0)) {
        return 0x69E;
    }
    if ((Progress_Check(0x22) != 0) && (Progress_Check(0x24) == 0)) {
        return 0x69F;
    }
    if ((Progress_Check(0x24) != 0) && (Progress_Check(38) == 0)) {
        return 0x6A0;
    }
    if ((Progress_Check(38) != 0) && (Progress_Check(39) == 0)) {
        return 0x6A1;
    }
    if ((Progress_Check(39) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x6A2;
    }
    return 0;
}

s32 func_ov030_020a5964(void) {
    if ((gSaveState.unk_20.unk_2458 == 1) && (Progress_Check(33) == 0)) {
        return 0x6AA;
    } else if ((gSaveState.unk_20.unk_2458 == 2) && (Progress_Check(0x23) != 0) && (Progress_Check(2) == 0) &&
               (Progress_Check(38) == 0))
    {
        return 0x6AC;
    } else if (Progress_Check(33) == 0) {
        return 0x6AA;
    } else if ((Progress_Check(33) != 0) && (Progress_Check(0x23) == 0)) {
        return 0x6AB;
    } else if ((Progress_Check(0x23) != 0) && (Progress_Check(2) == 0) && (Progress_Check(38) == 0)) {
        return 0x6AD;
    } else if ((Progress_Check(0x23) != 0) && (Progress_Check(2) == 0) && (Progress_Check(38) != 0) &&
               (Progress_Check(39) == 0))
    {
        return 0x6AE;
    } else if ((Progress_Check(0x23) != 0) && (Progress_Check(2) != 0) && (Progress_Check(39) == 0)) {
        return 0x6AE;
    } else if ((Progress_Check(39) != 0) && (Progress_Check(41) == 0)) {
        return 0x6AF;
    } else if ((Progress_Check(41) != 0) && (Progress_Check(42) == 0)) {
        return 0x6B0;
    } else
        return 0;
}

s32 func_ov030_020a5b20(void) {
    if ((gSaveState.unk_20.unk_2458 == 0x18) && (Progress_Check(2) != 0)) {
        return 0x6C1;
    } else if ((gSaveState.unk_20.unk_2458 == 0x19) && (Progress_Check(0x30) != 0)) {
        return 0x6C2;
    } else if (Progress_Check(33) == 0) {
        return 0x6BE;
    } else if ((Progress_Check(33) != 0) && (Progress_Check(33) != 0)) {
        return 0x6BF;
    } else if (Progress_Check(0x30) != 0) {
        return 0x6C3;
    } else
        return 0;
}

s32 func_ov030_020a5bd4(void) {
    if ((gSaveState.unk_20.unk_2458 == 0x11) && (Progress_Check(0x23) != 0) && (Progress_Check(38) == 0)) {
        return 0x6D2;
    } else if ((gSaveState.unk_20.unk_2458 == 18) && (Progress_Check(0x23) != 0) && (Progress_Check(38) == 0)) {
        return 0x6D3;
    } else if (Progress_Check(38) != 0) {
        return 0x6D4;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5c68(void) {
    if ((gSaveState.unk_20.unk_2458 == 3) && (Progress_Check(0x22) != 0) && (Progress_Check(37) == 0)) {
        return 0x6D8;
    } else if ((gSaveState.unk_20.unk_2458 == 0x13) && (Progress_Check(0x22) != 0) && (Progress_Check(37) == 0)) {
        return 0x6D8;
    } else if ((gSaveState.unk_20.unk_2458 == 23) && (Progress_Check(0x28) != 0) && (Progress_Check(3) == 0)) {
        return 0x6DC;
    } else if (Progress_Check(37) == 0) {
        return 0x6D7;
    } else if ((Progress_Check(37) != 0) && (Progress_Check(38) == 0)) {
        return 0x6D9;
    } else if ((Progress_Check(38) != 0) && (Progress_Check(39) == 0)) {
        return 0x6DA;
    } else if ((Progress_Check(39) != 0) && (Progress_Check(3) == 0)) {
        return 0x6DB;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5dac(void) {
    if (gSaveState.unk_20.unk_2458 == 1) {
        if (Progress_Check(0x20) == 0) {
            return 0x6EB;
        } else if ((Progress_Check(3) != 0) && (Progress_Check(0x24) == 0)) {
            return 0x6EE;
        }
    }

    if ((gSaveState.unk_20.unk_2458 == 4) && (Progress_Check(0x24) != 0) && (Progress_Check(39) == 0)) {
        return 0x6EF;
    } else if ((gSaveState.unk_20.unk_2458 == 2) && (Progress_Check(0x24) != 0) && (Progress_Check(39) == 0)) {
        return 0x6EF;
    } else if (gSaveState.unk_20.unk_2458 == 6) {
        if ((Progress_Check(39) != 0) && (Progress_Check(0x28) == 0)) {
            return 0x6F2;
        } else if (Progress_Check(0x28) != 0) {
            return 0x6F3;
        }
    }

    if ((Progress_Check(0x20) != 0) && (Progress_Check(3) == 0)) {
        return 0x6EC;
    } else if ((Progress_Check(3) != 0) && (Progress_Check(0x24) == 0)) {
        return 0x6ED;
    } else if ((Progress_Check(0x24) != 0) && (Progress_Check(39) == 0)) {
        return 0x6F0;
    } else if ((Progress_Check(39) != 0) && (Progress_Check(0x28) == 0)) {
        return 0x6F1;
    }
    return 0;
}

s32 func_ov030_020a5f64(void) {
    if (gSaveState.unk_20.unk_2458 == 6) {
        if ((Progress_Check(0x20) != 0) && (Progress_Check(33) == 0)) {
            return 0x714;
        } else if ((Progress_Check(33) != 0) && (Progress_Check(0x22) == 0)) {
            return 0x715;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 7) {
        if ((Progress_Check(0x23) != 0) && (Progress_Check(37) == 0)) {
            return 0x717;
        } else if ((Progress_Check(38) != 0) && (Progress_Check(4) == 0)) {
            return 0x719;
        }
    }
    if (gSaveState.unk_20.unk_2458 == 9) {
        if ((Progress_Check(38) != 0) && (Progress_Check(4) == 0)) {
            return 0x71A;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(41) == 0)) {
            return 0x71B;
        }
    }
    if (Progress_Check(0x22) == 0) {
        return 0x713;
    } else if ((Progress_Check(0x22) != 0) && (Progress_Check(37) == 0)) {
        return 0x716;
    } else if ((Progress_Check(37) != 0) && (Progress_Check(4) == 0)) {
        return 0x718;
    } else {
        return 0;
    }
}

s32 func_ov030_020a60fc(void) {
    if ((gSaveState.unk_20.unk_2458 == 0x10) && (Progress_Check(33) != 0) && (Progress_Check(3) == 0)) {
        return 0x728;
    } else if ((gSaveState.unk_20.unk_2458 == 0xE) && (Progress_Check(33) != 0) && (Progress_Check(3) == 0)) {
        return 0x728;
    } else if ((gSaveState.unk_20.unk_2458 == 12) && (Progress_Check(3) != 0) && (Progress_Check(37) == 0)) {
        return 0x72A;
    } else if (Progress_Check(3) == 0) {
        return 0x727;
    } else if ((Progress_Check(3) != 0) && (Progress_Check(37) == 0)) {
        return 0x729;
    } else {
        return 0;
    }
}

s32 func_ov030_020a61f0(void) {
    if (gSaveState.unk_20.unk_2458 == 41) {
        if ((Progress_Check(41) != 0) && (Progress_Check(6) == 0)) {
            return 1857;
        } else if ((Progress_Check(42) != 0) && (Progress_Check(43) == 0)) {
            return 1858;
        }
    }
    if (Progress_Check(33) == 0) {
        return 1851;
    } else if ((Progress_Check(33) != 0) && (Progress_Check(3) == 0)) {
        return 1852;
    } else if ((Progress_Check(3) != 0) && (Progress_Check(37) == 0)) {
        return 1853;
    } else if ((Progress_Check(37) != 0) && (Progress_Check(38) == 0)) {
        return 1854;
    } else if ((Progress_Check(38) != 0) && (Progress_Check(5) == 0)) {
        return 1855;
    } else if ((Progress_Check(5) != 0) && (Progress_Check(6) == 0)) {
        return 1856;
    } else
        return 0;
}

s32 func_ov030_020a633c(void) {
    if ((gSaveState.unk_20.unk_2458 == 43) && (Progress_Check(127) != 0)) {
        return 1871;
    } else {
        return 0;
    }
}

s32 func_ov030_020a6374(void) {
    if (gSaveState.unk_20.unk_2458 == 23) {
        if (Progress_Check(GATE_GAME_CLEARED) == FALSE) {
            return 1891;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(15) == 0)) {
            return 1898;
        } else if ((Progress_Check(15) != 0) && (Progress_Check(6) == 0)) {
            return 1899;
        } else if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
            return 1901;
        } else if ((Progress_Check(7) != 0) && (Progress_Check(8) == 0)) {
            return 1903;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x15) {
        if (Progress_Check(1) == 0) {
            return 1892;
        } else if ((Progress_Check(1) != 0) && (Progress_Check(2) == 0)) {
            return 1893;
        } else if ((Progress_Check(2) != 0) && (Progress_Check(3) == 0)) {
            return 1894;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(15) == 0)) {
            return 1897;
        } else if ((Progress_Check(15) != 0) && (Progress_Check(6) == 0)) {
            return 1899;
        } else if ((Progress_Check(7) != 0) && (Progress_Check(8) == 0)) {
            return 1902;
        }
    }

    if (gSaveState.unk_20.unk_2458 == 0x16) {
        if ((Progress_Check(4) != 0) && (Progress_Check(12) == 0)) {
            return 1895;
        } else if ((Progress_Check(12) != 0) && (Progress_Check(5) == 0) && (Progress_Check(6) == 0)) {
            return 1896;
        } else if ((Progress_Check(5) != 0) && (Progress_Check(15) == 0)) {
            return 1897;
        } else if ((Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
            return 1900;
        }
    }

    return 0;
}

s32 func_ov030_020a65f4(void) {
    if ((gSaveState.unk_20.unk_2458 == 1) && Progress_Check(GATE_GAME_CLEARED) == FALSE) {
        return 1911;
    } else if ((gSaveState.unk_20.unk_2458 == 11) && (Progress_Check(1) != 0) && (Progress_Check(3) == 0)) {
        return 1912;
    } else if ((gSaveState.unk_20.unk_2458 == 15) && (Progress_Check(6) != 0) && (Progress_Check(7) == 0)) {
        return 1915;
    } else {
        return 0;
    }
}

const ProgressRewardFunc data_ov030_020da618[44] = {
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
