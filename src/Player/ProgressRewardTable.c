#include "Engine/EasyTask.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Player/Progress.h"
#include "Player/Stats.h"
#include "Save.h"
#include "types.h"

typedef struct {
    /* 0x00000 */ char     unk_00000[0x11584];
    /* 0x11584 */ TaskPool taskPool;
    /* 0x11604 */ char     unk_11604[0x21630 - 0x11604];
    /* 0x21630 */ s32      unk_21630;
    /* 0x21634 */ char     unk_21634[0x2176C - 0x21634];
    /* 0x2176C */ u16      unk_2176C[2];
    /* 0x21770 */ char     unk_21770[0x2177C - 0x21770];
    /* 0x2177C */ s16      unk_2177C;
    /* 0x2177E */ char     unk_2177E[0x21788 - 0x2177E];
    /* 0x21788 */ u32      unk_21788[2];
    /* 0x21790 */ char     unk_21790[0x21ACC - 0x21790];
    /* 0x21ACC */ s32      unk_21ACC;
    /* 0x21AD0 */ s32      unk_21AD0;
    /* 0x21AD4 */ char     unk_21AD4[0x21AF0 - 0x21AD4];
    /* 0x21AF0 */ s32      unk_21AF0;
    /* 0x21AF4 */ s32      unk_21AF4;
    /* 0x21AF8 */ char     unk_21AF8[0x21AFC - 0x21AF8];
    /* 0x21AFC */ s16      unk_21AFC;
    /* 0x21AFE */ char     unk_21AFE[0x21B00 - 0x21AFE];
    /* 0x21B00 */ s32      unk_21B00;
    /* 0x21B04 */ char     unk_21B04[0x21B08 - 0x21B04];
    /* 0x21B08 */ s32      unk_21B08;
    /* 0x21B0C */ s32      unk_21B0C;
    /* 0x21B10 */ char     unk_21B10[0x21CF8 - 0x21B10];
    /* 0x21CF8 */ s32      unk_21CF8;
    /* 0x21CFC */ char     unk_21CFC[0x21D18 - 0x21CFC];
    /* 0x21D18 */ s32      unk_21D18;
    /* 0x21D1C */ s16      unk_21D1C;
    /* 0x21D1E */ s16      unk_21D1E;
    /* 0x21D20 */ s16      unk_21D20;
    /* 0x21D22 */ s16      unk_21D22;
    /* 0x21D24 */ void*    unk_21D24;
    /* 0x21D28 */ s16      unk_21D28;
} RewardTableObject;

extern s32  func_ov030_020aebb0(s32);
extern void func_ov030_020aeb34(s32);
extern void func_ov030_020a9c54(RewardTableObject*, s32, s32);
extern s32  func_ov030_020aed9c(s32);
extern void func_ov030_020ae96c(s32);
extern void func_ov030_020c26bc(s32);
extern s32  func_ov030_020848a4(s32, s32, s32, s32);
extern s32  func_ov030_020848e4(RewardTableObject*, s32);
extern void func_ov030_020a669c(s32); // param is NOT itemID, maybe story progress ID?
extern void func_ov030_020a6738(RewardTableObject*);
extern s32  func_ov030_02084978(RewardTableObject*);
extern s32  func_ov030_020a75c0(RewardTableObject*);

extern void func_ov030_020824a0(void*);
extern void func_ov030_02082c04(void*);
extern void func_ov030_02082bf0(void*);
extern void func_ov030_02082c68(void*);
extern void func_ov030_02082534(void*);
extern void func_ov030_020aec1c(s32);
extern void func_ov030_020aeb70(s32);

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

s32 func_ov030_02084a2c(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (func_ov030_020aebb0(0x20) == 0) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x437, 0);
            return 1;
        } else if ((func_ov030_020aebb0(33) == 0) && (func_ov030_020aed9c(-1) != 0)) {
            func_ov030_020aeb34(33);
            data_02071cf0.unk_20.unk_264C = 4;
            func_ov030_020a9c54(arg0, 0x57, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x59, 0);
            return 1;
        }

        if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(0x34) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x5F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) != 0) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0x2C) == 0)) {
            func_ov030_020aeb34(0x2C);
            if (func_ov030_020aebb0(39) == 0) {
                func_ov030_020aeb34(39);
                func_ov030_020a9c54(arg0, 0x60, 0);
                return 1;
            }
        }
        if ((func_ov030_020848e4(arg0, 3) != 0) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(45) == 0)) {
            func_ov030_020aeb34(45);
            if (func_ov030_020aebb0(39) == 0) {
                func_ov030_020aeb34(39);
                func_ov030_020a9c54(arg0, 0x60, 0);
                return 1;
            }
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(3);
            func_ov030_020a9c54(arg0, 0x61, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 3) {
        if (func_ov030_020aebb0(0x28) == 0) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x62, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(41) == 0)) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x63, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x64, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(43) == 0)) {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x65, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 5) {
        if (func_ov030_020aebb0(0x24) == 0) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x5B, 0);
            return 1;
        } else if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x5E, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 37) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(0x34) == 0)) {
        func_ov030_020aeb34(0x34);
        func_ov030_020a9c54(arg0, 0x805A, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_02084eb4(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x57:
            func_ov030_020aeb34(0x22);
            data_02071cf0.unk_20.unk_24BE = 0x67;
            func_ov030_020aec1c(10);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8058;
            data_02071cf0.unk_20.unk_3124 = 3;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x58:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84BA;
            func_ov030_020af364(2);
            return 1;
        case 0x59:
            data_02071cf0.unk_20.unk_3124 = 4;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
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
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x805C;
            data_02071cf0.unk_20.unk_3124 = 5;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x5C:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x805D;
            data_02071cf0.unk_20.unk_3124 = 6;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x5D:
            func_ov030_020aeb34(2);
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            data_02071cf0.unk_20.unk_24B4                 = 0;
            data_02071cf0.unk_20.unk_24B8                 = 1;
            data_02071cf0.unk_20.unk_24BC                 = 0x84B6;
            func_ov030_020af364(3);
            return 1;
        case 0x5E:
            func_ov030_020aeaa0(3, 4);
            data_02071cf0.unk_20.unk_2648 = 0;
            data_02071cf0.unk_20.unk_264A = 0;
            data_02071cf0.unk_20.unk_264E = 0;
            arg0->unk_21CF8               = 1;
            break;
        case 0x5F:
            arg0->unk_21CF8               = 1;
            data_02071cf0.unk_20.unk_2648 = 4;
            break;
        case 0x60:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x4B7;
            func_ov030_020af364(4);
            return 1;
        case 0x61:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x4B8;
            func_ov030_020af364(5);
            return 1;
        case 0x62:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x4B9;
            func_ov030_020af364(6);
            return 1;
        case 0x63:
            data_02071cf0.unk_20.unk_3124 = 7;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4BA:
            func_ov030_020aeb34(0x33);
            if (Inventory_HasRequiredQuantity(ITEM_WALLET_MY_FIRST_WALLET, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_WALLET_MY_FIRST_WALLET, 0);
            }
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK31, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_CD_TRACK31, 0);
            }
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_ESCAPING_FROM_BATTLE);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DASH);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4B6:
            func_ov030_020aeb34(47);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_DRAG);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_BOOT_REBOOT);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_BOTTOM_SCREEN_COMBAT);
                func_ov030_020aac28(arg0, 265);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        default:
            func_ov030_020aeb34(0x30);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_TOP_SCREEN_COMBAT);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4B8:
            func_ov030_020aeb34(0x31);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_FOLLOW_THAT_PUCK);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_DUAL_SCREEN_COMBAT);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4B9:
            func_ov030_020aeb34(0x32);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_SO_I_BEAT_THE_ENEMY);
            break;
        case 0x64:
            func_ov030_020aeb34(1);
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
        func_ov030_020aeb34(0);
    }
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x6A;
    data_02071cf0.unk_20.unk_3124 = 56;
    data_02071cf0.unk_20.unk_244C = 0x159000;
    data_02071cf0.unk_20.unk_2450 = 0xD2000;
}

s32 func_ov030_02085508(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if ((func_ov030_020aebb0(0x20) == 0) && (func_ov030_020848a4(0, 0x96, 0x1E, 250) != 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x6B, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(33) == 0) && (func_ov030_020aed9c(-2) != 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x6C, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(56) == 0)) {
            func_ov030_020aeb34(56);
            func_ov030_020a9c54(arg0, 0x4C0, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(0x39) == 0)) {
            func_ov030_020aeb34(0x39);
            func_ov030_020a9c54(arg0, 0x4C1, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x37) != 0) && (func_ov030_020aebb0(58) == 0)) {
            func_ov030_020aeb34(58);
            func_ov030_020a9c54(arg0, 0x4C2, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(0x37) != 0) &&
                   (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(3) == 0))
        {
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_266E = 4;
            func_ov030_020a9c54(arg0, 0x6D, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x11) {
        if (func_ov030_020aebb0(0x22) == 0) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x6E, 0);
            return 1;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(41) == 0)) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x438, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x76, 0);
            return 1;
        } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(43) == 0) && (func_ov030_020aebb0(5) == 0) &&
                   (func_ov030_020848a4(0, 0xC8, 0x190, 0x159) != 0) && (func_ov030_020aed9c(-2) != 0))
        {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x3CD, 0);
            return 1;
        } else if (((func_ov030_020aebb0(0x32) != 0) || (func_ov030_020aebb0(0x33) != 0)) &&
                   ((func_ov030_020aebb0(0x32) == 0) || (func_ov030_020aebb0(0x33) == 0)) && (func_ov030_020aebb0(43) == 0))
        {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x3CD, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(5) == 0)) {
            func_ov030_020aeb34(5);
            func_ov030_020a9c54(arg0, 0x77, 0);
            return 1;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(61) == 0)) {
            func_ov030_020aeb34(61);
            func_ov030_020a9c54(arg0, 0x442, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x2C) == 0)) {
            func_ov030_020aeb34(0x2C);
            func_ov030_020a9c54(arg0, 0x78, 0);
            return 1;
        } else if ((func_ov030_020aebb0(46) == 0) && (func_ov030_020aed9c(0x3707) != 0)) {
            func_ov030_020aeb34(46);
            func_ov030_020a9c54(arg0, 0x3CF, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 5) {
        if (func_ov030_020aebb0(45) == 0) {
            func_ov030_020aeb34(45);
            func_ov030_020a9c54(arg0, 0x79, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x9B, 0x64, 0x190, 0x12C) != 0) && (func_ov030_020aebb0(6) == 0) &&
                   (func_ov030_020aed9c(-2) != 0))
        {
            func_ov030_020aeb34(6);
            func_ov030_020a9c54(arg0, 0x7A, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x3B) != 0) && (func_ov030_020aebb0(0x30) == 0)) {
            func_ov030_020aeb34(0x30);
            func_ov030_020a9c54(arg0, 0x84, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x3C) != 0) && (func_ov030_020aebb0(0x31) == 0)) {
            func_ov030_020aeb34(0x31);
            func_ov030_020a9c54(arg0, 0x85, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02085b14(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35EF:
            if (func_ov030_020aebb0(0x23) == 0) {
                func_ov030_020aeb34(0x23);
                func_ov030_020a669c(0x70);
                func_ov030_020a6738(arg0);
                return 1;
            } else {
                func_ov030_020a669c(0x74);
                func_ov030_020a6738(arg0);
                return 1;
            }
        case 0x35F0:
            func_ov030_020aeb34(0x24);
            func_ov030_020a669c(0x71);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35F1:
            func_ov030_020aeb34(37);
            func_ov030_020a669c(0x72);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35F3:
            func_ov030_020aeb34(38);
            func_ov030_020a669c(0x73);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35F2:
            func_ov030_020a669c(0x75);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02085c24(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35F5:
            func_ov030_020a669c(0x7C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35F6:
            func_ov030_020a669c(0x7D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35F7:
            if (func_ov030_020aebb0(47) == 0) {
                func_ov030_020a669c(0x7E);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x80);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35F8:
            func_ov030_020a669c(0x82);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02085cf8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x6A:
            data_02071cf0.unk_20.unk_24BE = 0x87;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(5, 4);
            break;
        case 0x3CB:
            func_ov030_020aeb34(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6B:
            func_ov030_020aeb34(2);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_SCANNING);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6C:
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9acc;
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
            func_ov030_020a669c(0x6F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
            if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(37) != 0) &&
                (func_ov030_020aebb0(38) != 0))
            {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 5, 0x35EF, 0x35F0, 0x35F3, 0x35F1, 0x35F2);
            }
            if (func_ov030_020aebb0(0x24) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 4, 0x35EF, 0x35F0, 0x35F3, 0x35F1);
            }
            if (func_ov030_020aebb0(0x24) == 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02085b14, 0x35EE, 3, 0x35EF, 0x35F0, 0x35F3);
            }
            break;
        case 0x75:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_WEARING_PINS);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_PHONE_MENU);
            func_ov030_020aac28(arg0, 0x10B);
            func_ov030_020aeb34(4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x76:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x439);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x439:
            func_ov030_020c596c();
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9af4;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x77:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_EXTRA_SLOT);
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            data_02071cf0.unk_20.unk_2656 = 4;
            arg0->unk_21CF8               = 1;
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
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x86);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7A:
            arg0->unk_21CF8 = 1;
            break;
        case 0x7B:
            func_ov030_020aeb34(7);
        case 0x7D:
        case 0x7F:
            if (func_ov030_020aebb0(47) == 0) {
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
            func_ov030_020a669c(0x81);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x81:
            func_ov030_020aeb34(47);
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
            func_ov030_020aeb34(8);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x4C3;
            func_ov030_020af364(7);
            return 1;
        case 0x84:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84C4U;
            func_ov030_020af364(8);
            return 1;
        case 0x85:
            data_02071cf0.unk_20.unk_3124 = 8;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4C0:
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_TOUCH);
                func_ov030_020aac28(arg0, 0x10C);
                func_ov030_020aac28(arg0, 0x10E);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C1:
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_USE_OBSTACLES);
                func_ov030_020aac28(arg0, 0x10D);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C2:
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_SLASH);
                func_ov030_020aac28(arg0, 0x10A);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C3:
            func_ov030_020aeb34(0x3B);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4C4:
            func_ov030_020aeb34(0x3C);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0x86:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 2;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(23);
    data_02071cf0.unk_20.unk_244C = 0x12C000;
    data_02071cf0.unk_20.unk_2450 = 0x87000;
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x808A;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x39;
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_02086804(s32 arg0) {
    s16 var_r0;

    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            if (func_ov030_020aebb0(0x44) == 0) {
                func_ov030_020aeb34(0x44);
                func_ov030_020a9c54(arg0, 0x4091, 0);
                return 1;
            }
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x40AE, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 0x47) != 0) {
            if (func_ov030_020aebb0(0x3B) == 0) {
                if (func_ov030_020aebb0(0x44) == 0) {
                    func_ov030_020aeb34(0x44);
                    func_ov030_020a9c54(arg0, 0x4091, 0);
                    return 1;
                }
                func_ov030_020aeb34(0x3B);
                func_ov030_020a9c54(arg0, 0x43D9, 0);
                return 1;
            }
        } else {
            func_ov030_020aeb70(0x3B);
        }
        if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(0x48) == 0)) {
            func_ov030_020aeb34(0x48);
            func_ov030_020a9c54(arg0, 0x4CD, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(0x49) == 0)) {
            func_ov030_020aeb34(0x49);
            func_ov030_020a9c54(arg0, 0x43B, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(7) == 0)) {
            func_ov030_020aeb34(7);
            func_ov030_020a9c54(arg0, 0xAF, 0);
            return 1;
        } else if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x46) == 0)) {
            func_ov030_020aeb34(0x46);
            func_ov030_020a9c54(arg0, 0x4CB, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x46) != 0) && (func_ov030_020aebb0(9) == 0)) {
            func_ov030_020aeb34(9);
            func_ov030_020a9c54(arg0, 0xB1, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(18) != 0) && (func_ov030_020aebb0(39) == 0))
        {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x43DC, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x28) == 0) && (func_ov030_020aed9c(0x371B) != 0)) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x3DD, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x3C) == 0) &&
            ((func_ov030_020aebb0(42) != 0) || (func_ov030_020aebb0(43) != 0) || (func_ov030_020aebb0(0x2C) != 0)))
        {
            func_ov030_020aeb34(0x3C);
            func_ov030_020a9c54(arg0, 0xB7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(41) == 0))
        {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x43DF, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if (func_ov030_020aebb0(0x20) == 0) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x90, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x49) != 0) && (func_ov030_020aebb0(0x44) == 0)) {
            func_ov030_020aeb34(0x44);
            func_ov030_020a9c54(arg0, 0x4091, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(0x48) == 0)) {
            func_ov030_020aeb34(0x48);
            func_ov030_020a9c54(arg0, 0x4CD, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x45) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0xA8, 0);
            return 1;
        }
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (func_ov030_020aebb0(0x42) != 0) && (func_ov030_020aebb0(0x43) == 0)) {
            func_ov030_020aeb34(0x43);
        }
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(0x4C) == 0)) {
            func_ov030_020aeb34(0x4C);
            func_ov030_020a9c54(arg0, 0x443, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x16) {
        if (func_ov030_020aebb0(0x32) == 0) {
            func_ov030_020aeb34(0x32);
            func_ov030_020a9c54(arg0, 0x9A, 0);
            return 1;
        } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(11) == 0)) {
            func_ov030_020aeb34(11);
            func_ov030_020a9c54(arg0, 0xB3, 0);
            return 1;
        } else if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
            func_ov030_020aeb34(0xE);
            func_ov030_020a9c54(arg0, 0xB4, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 23) {
        if ((func_ov030_02084860(0x12C, 0x87, 0x96) == 0) && (func_ov030_020aebb0(45) == 0)) {
            func_ov030_020aeb34(45);
            func_ov030_020a9c54(arg0, 0x8C, 0);
            return 1;
        } else if ((func_ov030_020aed9c(0x3715) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x3DB, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x47) != 0) && (func_ov030_020aebb0(61) == 0)) {
            func_ov030_020aeb34(61);
            func_ov030_020a9c54(arg0, 0xBA, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02086f10(s32 arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x35F9:
            func_ov030_020a669c(0x3E4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FA:
            func_ov030_020a669c(0x3E5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FB:
            func_ov030_020a669c(0x3E6);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FC:
            func_ov030_020a669c(0x3E7);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            func_ov030_020a669c(0x3E3);
            func_ov030_020a6738(arg0);
            return 1;
    }
}

s32 func_ov030_02086fdc(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x35F9:
            if (func_ov030_020aebb0(38) != 0) {
                func_ov030_020a669c(0x3EC);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3EB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FA:
            func_ov030_020aeb34(42);
            func_ov030_020a669c(0x3ED);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FB:
            func_ov030_020aeb34(43);
            func_ov030_020a669c(0x3EE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FC:
            func_ov030_020aeb34(0x2C);
            func_ov030_020a669c(0x3EF);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            func_ov030_020a669c(0x3EA);
            func_ov030_020a6738(arg0);
            return 1;
    }
}

s32 func_ov030_020870ec(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x35F9:
            func_ov030_020a669c(0x3F3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FA:
            func_ov030_020aeb34(42);
            func_ov030_020a669c(0x3F4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FB:
            if (func_ov030_020aebb0(43) == 0) {
                func_ov030_020aeb34(43);
                func_ov030_020aeb34(13);
                func_ov030_020a669c(0x3EE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3F5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FC:
            if (func_ov030_020aebb0(0x2C) == 0) {
                func_ov030_020aeb34(0x2C);
                func_ov030_020aeb34(13);
                func_ov030_020a669c(0x3EF);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3F6);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            func_ov030_020a669c(0x3F2);
            func_ov030_020a6738(arg0);
            return 1;
    }
}

s32 func_ov030_02087238(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35FE:
            func_ov030_020aeb34(47);
            func_ov030_020a669c(0x93);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FF:
            func_ov030_020aeb34(0x30);
            func_ov030_020a669c(0x94);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3600:
            func_ov030_020aeb34(0x31);
            func_ov030_020a669c(0x95);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020872e8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3602:
            func_ov030_020a669c(0x97);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3603:
            func_ov030_020a669c(0x98);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087360(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            func_ov030_020aeb34(0x33);
            func_ov030_020a669c(0x9C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3606:
            func_ov030_020aeb34(0x34);
            func_ov030_020a669c(0x9D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3607:
            func_ov030_020aeb34(0x35);
            func_ov030_020a669c(0x9E);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087414(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3609:
            func_ov030_020aeb34(0x36);
            func_ov030_020a669c(0xA2);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x360A:
            func_ov030_020aeb34(0x37);
            func_ov030_020a669c(0xA3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x360B:
            func_ov030_020aeb34(56);
            func_ov030_020a669c(0xA4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x360C:
            func_ov030_020aeb34(0x39);
            func_ov030_020a669c(0xA6);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x360D:
            func_ov030_020a669c(0xA5);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_020874fc(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x360F:
            func_ov030_020a669c(0xAA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3610:
            func_ov030_020a669c(0xAB);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02087574(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3612:
            func_ov030_020a669c(0xAA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3613:
            func_ov030_020a669c(0xAD);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020875ec(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3615:
            func_ov030_020a669c(0x3D2);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3616:
            func_ov030_020a669c(0x3D3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3617:
            func_ov030_020a669c(0x3D4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3618:
            func_ov030_020a669c(0x3D5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3619:
            func_ov030_020a669c(0x3D6);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_020876c8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x8A:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x808B;
            data_02071cf0.unk_20.unk_3124 = 9;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
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
            func_ov030_020a669c(0x8D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x8D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x8E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x8E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x8F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x8F:
            data_02071cf0.unk_20.unk_24BE = 0xC1;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(0x15, 4);
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            data_02071cf0.unk_20.unk_267A                 = 4;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_EASY);
            break;
        case 0x90:
            ProgressReward_GrantItem(arg0, ITEM_BOOK_TALK_TO_YOUR_PARTNER);
            break;
        case 0x91:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x43A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x43A:
            if ((func_ov030_020848e4(arg0, 0x49) != 0) || (func_ov030_020848e4(arg0, 0x4A) != 0)) {
                arg0->unk_21630 = 5;
                DebugOvlDisp_Pop();
                return 0;
            }
            if (func_ov030_020848e4(arg0, 0x47) != 0) {
                func_ov030_020aeb34(0x3B);
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x3D9);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020848e4(arg0, 0x48) != 0) {
                func_ov030_020aeb34(0x24);
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0xAE);
                func_ov030_020a6738(arg0);
                return 1;
            }
        case 0x92:
            func_ov030_020aeb34(5);
        case 0x93:
        case 0x94:
        case 0x95:
            if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x31) != 0)) {
                func_ov030_020a669c(0x96);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020aebb0(0x30) != 0) {
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
            if (func_ov030_020aebb0(5) != 0) {
                func_ov030_020a669c(0x78C);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x78C:
            if (func_ov030_020aebb0(0x3F) == 0) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0xA0);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xA9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xA0:
            func_ov030_020aeb34(0x3F);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xA1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA6:
            if ((func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(0x37) != 0) && (func_ov030_020aebb0(56) != 0) &&
                (func_ov030_020aebb0(0x39) != 0))
            {
                return func_ov030_020a9f54(arg0, &func_ov030_02087414, 0x3608, 5, 0x3609, 0x360A, 0x360B, 0x360C, 0x360D);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087414, 0x3608, 4, 0x3609, 0x360A, 0x360B, 0x360C);
        case 0xA5:
            func_ov030_020a669c(0x4CA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xA8:
            func_ov030_020a669c(0xA9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xA9:
            return func_ov030_020a9f54(arg0, &func_ov030_020874fc, 0x360E, 2, 0x360F, 0x3610);
        case 0xAA:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0xACU;
            func_ov030_020af364(9);
            return 1;
        case 0xAC:
            return func_ov030_020a9f54(arg0, &func_ov030_02087574, 0x3611, 2, 0x3612, 0x3613);
        case 0xAD:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x78D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x78D:
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_2674 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x78E:
            if ((func_ov030_020aebb0(0x42) != 0) && (func_ov030_020aebb0(0x43) != 0)) {
                func_ov030_020a669c(0x790);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020aebb0(0x41) != 0) {
                func_ov030_020a669c(0x78F);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x78F:
            if (func_ov030_020aebb0(0x42) == 0) {
                func_ov030_020aeb34(0x42);
                data_02071cf0.unk_20.unk_30FC = 0;
                data_02071cf0.unk_20.unk_30FE = 1;
                data_02071cf0.unk_20.unk_3100 = 0x15;
                arg0->unk_21CF8               = 1;
            }
            break;
        case 0x790:
            func_ov030_020aeb34(4);
            data_02071cf0.unk_20.unk_2678 = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_JUMP);
            arg0->unk_21CF8 = 1;
            break;
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
            if ((func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(0x35) != 0)) {
                func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
                func_ov030_020a669c(0x9F);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020aebb0(0x33) != 0) {
                return func_ov030_020a9f54(arg0, &func_ov030_02087360, 0x3604, 3, 0x3605, 0x3606, 0x3607);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_02087360, 0x3604, 2, 0x3605, 0x3606);
        case 0x9F:
            func_ov030_020aeb34(18);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x43C);
            func_ov030_020a6738(arg0);
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
            func_ov030_020aeb34(6);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3D9:
            if (func_ov030_020aebb0(37) == 0) {
                if (func_ov030_020aebb0(58) == 0) {
                    func_ov030_020aeb34(58);
                    func_ov030_020a669c(0x3D1);
                    func_ov030_020a6738(arg0);
                    return 1;
                }
                func_ov030_020a669c(0x3D7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(0x10) == 0)) {
                func_ov030_020aeb34(0x10);
                func_ov030_020a669c(0x3DE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(0x13) == 0)) {
                func_ov030_020aeb34(0x13);
                func_ov030_020a669c(0x3E0);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020aebb0(58) == 0) {
                func_ov030_020aeb34(58);
                func_ov030_020a669c(0x3D1);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3D7);
            func_ov030_020a6738(arg0);
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
            func_ov030_020aeb34(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB0:
            func_ov030_020aeb34(8);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB1:
            arg0->unk_21CF8 = 1;
            break;
        case 0xB2:
            func_ov030_020aeb34(10);
            func_ov030_020aeb34(0x41);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB3:
            func_ov030_020aeb34(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E8:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3E9, 0x3EA, &func_ov030_02086fdc);
        case 0x3EF:
        case 0x3EE:
        case 0x3ED:
            func_ov030_020aeb34(37);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB4:
            func_ov030_020aeb34(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DB:
            func_ov030_020aeb34(38);
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
            func_ov030_020aeb34(0x10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3DF:
            func_ov030_020aeb34(12);
            func_ov030_020aeb70(13);
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0xC0);
                func_ov030_020a6738(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E0:
            func_ov030_020aeb34(0x13);
            func_ov030_020aeb34(0xC);
            func_ov030_020aeb70(0xD);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3E9:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x3E9, 0x3EA, &func_ov030_02086fdc);
        case 0x3F4:
            func_ov030_020aeb34(0xD);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB5:
            func_ov030_020aeb34(15);
            arg0->unk_21CF8 = 1;
            return;
        case 0xB6:
            func_ov030_020aeb34(15);
            func_ov030_020aac28(arg0, 0x106);
            arg0->unk_21CF8 = 1;
            break;
        case 0xB9:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84CCU;
            func_ov030_020af364(10);
            return 1;
        case 0xBA:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x80BBU;
            data_02071cf0.unk_20.unk_3124 = 10;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0xBB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xBC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xBC:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xBD);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xBD:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xBE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xBE:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xBF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xBF:
            data_02071cf0.unk_20.unk_3124 = 11;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x3EB:
            func_ov030_020aeb34(0x4A);
            break;
        case 0x3F2:
            func_ov030_020aeb34(0x4B);
            break;
        case 0x4CD:
            func_ov030_020aeb34(0x48);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_ITEM_ABILITIES);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_SHOP_CLERKS);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4CA:
            func_ov030_020aeb34(0x45);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_SHIKI);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_ESP_CARDS);
            }
            break;
        case 0x4CB:
            func_ov030_020aeb34(0x46);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_YELLOW_NOISE_SYMBOLS);
                ProgressReward_GrantItem(arg0, ITEM_BOOK_RED_NOISE_SYMBOLS);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4CC:
            func_ov030_020aeb34(0x47);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0x30);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0xC0:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(37);
    data_02071cf0.unk_20.unk_264C = 0x1DA000;
    data_02071cf0.unk_20.unk_2650 = 0xBE000;
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_26B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x80C4;
    data_02071cf0.unk_20.unk_3124 = 58;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_26B8 = 0;
    data_02071cf0.unk_20.unk_2648 = 2;
    data_02071cf0.unk_20.unk_2666 = 2;
}

s32 func_ov030_02088b60(RewardTableObject* arg0) {
    if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(2) == 0)) {
        if (func_ov030_020aebb0(0x34) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) != 0) {
                func_ov030_020aeb34(0x34);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) == 0) {
            func_ov030_020aeb34(0x35);
            func_ov030_020aeb70(0x34);
        }
    }
    if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(3) == 0)) {
        if (func_ov030_020aebb0(0x31) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) != 0) {
                func_ov030_020aeb34(0x31);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) == 0) {
            func_ov030_020aeb34(0x36);
            func_ov030_020aeb70(0x31);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 3) {
        if (func_ov030_020aebb0(0x37) == 0) {
            func_ov030_020aeb34(0x37);
            func_ov030_020a9c54(arg0, 0xC5, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(37) == 0) &&
            (func_ov030_020aebb0(0x2C) == 0))
        {
            func_ov030_020aeb34(0x2C);
            func_ov030_020a9c54(arg0, 0x3F7, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 0) == 0) {
            func_ov030_020aeb70(0x2C);
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (func_ov030_020aebb0(4) == 0)) {
            func_ov030_020aeb34(4);
            func_ov030_020a9c54(arg0, 0xC7, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 56) != 0) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x40C9, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x80CE, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020c1a9c() != 0) && (func_ov030_020aebb0(39) == 0)) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0xCF, 0);
            return 1;
        } else if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x32) == 0)) {
            func_ov030_020aeb34(0x32);
            func_ov030_020a9c54(arg0, 0x441, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 11) {
        if ((func_ov030_020848a4(0xAA, 0xB4, 0xB4, 0x140) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0xD0, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(56) == 0)) {
            func_ov030_020aeb34(56);
            func_ov030_020a9c54(arg0, 0x444, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
            func_ov030_020a9c54(arg0, 0x440, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 12) && (func_ov030_020848e4(arg0, 15) != 0) && (func_ov030_020aebb0(6) == 0)) {
        func_ov030_020aeb34(6);
        func_ov030_020a9c54(arg0, 0xD3, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 13) {
        if (func_ov030_020aebb0(41) == 0) {
            func_ov030_020aeb34(41);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x80D4;
            data_02071cf0.unk_20.unk_3124 = 12;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
        if ((func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0xD5, 0);
            return 1;
        } else if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(43) == 0)) {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0xD6, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020890c0(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            func_ov030_020aeb34(0x22);
            func_ov030_020a669c(0xCA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361C:
            func_ov030_020aeb34(0x23);
            func_ov030_020a669c(0xCB);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02089148(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            func_ov030_020aeb34(0x22);
            func_ov030_020a669c(0xCA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361C:
            func_ov030_020aeb34(0x23);
            func_ov030_020a669c(0xCB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361D:
            func_ov030_020aeb34(0x24);
            func_ov030_020a669c(0xCC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361E:
            func_ov030_020aeb34(37);
            func_ov030_020a669c(0xCD);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02089214(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x361B:
            func_ov030_020aeb34(0x22);
            func_ov030_020a669c(0xCA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361C:
            func_ov030_020aeb34(0x23);
            func_ov030_020a669c(0xCB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361D:
            func_ov030_020aeb34(0x24);
            func_ov030_020a669c(0xCC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361E:
            func_ov030_020aeb34(37);
            func_ov030_020a669c(0xCD);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x361F:
            func_ov030_020a669c(0x43D);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02089300(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0xC4:
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 3;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x1D8000;
            arg0->unk_21B0C = 0xC1000;
            break;
        case 0xC5:
            data_02071cf0.unk_20.unk_24BE = 0xD8;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(13, 4);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xC8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xC6:
            func_ov030_020aeb34(4);
            func_ov030_020aeb34(0x2C);
            data_02071cf0.unk_20.unk_26B8 = 2;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_NOISE_REPORT);
            arg0->unk_21CF8 = 1;
            break;
        case 0xC7:
            func_ov030_020aeb34(0x2C);
            data_02071cf0.unk_20.unk_26B8 = 2;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_NOISE_REPORT);
            arg0->unk_21CF8 = 1;
            break;
        case 0x3F7:
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0xD7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
            if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(0x24) != 0) &&
                (func_ov030_020aebb0(37) != 0))
            {
                return func_ov030_020a9f54(arg0, func_ov030_02089214, 0x361A, 5, 0x361B, 0x361C, 0x361D, 0x361E, 0x361F);
            }
            if (func_ov030_020aebb0(0x23) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_02089148, 0x361A, 4, 0x361B, 0x361C, 0x361D, 0x361E);
            }
            return func_ov030_020a9f54(arg0, func_ov030_020890c0, 0x361A, 2, 0x361B, 0x361C);
        case 0x43D:
            data_02071cf0.unk_20.unk_2648 = 4;
            data_02071cf0.unk_20.unk_26B8 = 4;
            arg0->unk_21630               = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0xCE:
            func_ov030_020a669c(0x43E);
            func_ov030_020a6738(arg0);
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
            func_ov030_020a669c(0xD1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xD1:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BACKLASH);
            break;
        case 0xD3:
            func_ov030_020aeb34(6);
            data_02071cf0.unk_20.unk_2666 = 4;
            ProgressReward_GrantItem(arg0, ITEM_STICKER_MINGLE_MODE);
            arg0->unk_21CF8 = 1;
            break;
        case 0xD4:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84D4U;
            func_ov030_020af364(12);
            return 1;
        case 0xD5:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84D5U;
            func_ov030_020af364(11);
            return 1;
        case 0xD6:
            data_02071cf0.unk_20.unk_3124 = 13;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0xD2:
            func_ov030_020aeb34(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x791:
            func_ov030_020a669c(0x792);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x792:
            func_ov030_020aeb34(47);
            if (Inventory_HasRequiredQuantity(PIN_1000_YEN, 2, 0) != 0) {
                func_ov030_020a669c(0x793);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x793:
            func_ov030_020aeb34(2);
            func_ov030_020aeb34(0x34);
            data_02071cf0.unk_20.unk_264E = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_EXTRA_SLOT);
            arg0->unk_21CF8 = 1;
            break;
        case 0x794:
            func_ov030_020a669c(0x795);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x795:
            func_ov030_020aeb34(0x30);
            if (Inventory_HasRequiredQuantity(PIN_500_YEN, 5, 0) != 0) {
                func_ov030_020a669c(0x796);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x796:
            func_ov030_020aeb34(3);
            func_ov030_020aeb34(0x31);
            data_02071cf0.unk_20.unk_2664 = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_CHAINED_BATTLES);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4D4:
            func_ov030_020aeb34(45);
            func_ov030_020aac28(arg0, 0x3B);
            break;
        case 0x4D5:
            func_ov030_020aeb34(46);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0x50);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0xD7:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(6);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_2658 = 4;
    data_02071cf0.unk_20.unk_3124 = 0x3B;
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x80DB;
}

s32 func_ov030_02089b28(s32 arg0) {
    s16 var_r0;
    s16 var_r0_2;

    if ((func_ov030_020aebb0(0x48) != 0) && (func_ov030_020aebb0(4) == 0)) {
        if (func_ov030_020aebb0(0x49) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) != 0) {
                func_ov030_020aeb34(0x49);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) == 0) {
            func_ov030_020aeb34(0x4A);
            func_ov030_020aeb70(0x49);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 2) {
        if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x45) == 0)) {
            func_ov030_020aeb34(0x45);
            func_ov030_020a9c54(arg0, 0x4DE, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x45) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0xE1, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (func_ov030_020aebb0(0x43) != 0) && (func_ov030_020aebb0(0x44) == 0)) {
            func_ov030_020aeb34(0x44);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0) {
        if (func_ov030_020aebb0(0x20) == 0) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0xDC, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3C) != 0) && (func_ov030_020aebb0(33) == 0) && (func_ov030_020aebb0(0x46) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x40DD, 0);
            return 1;
        }
        if ((data_02071cf0.unk_20.unk_3102 >= 3) && (func_ov030_020aebb0(0x48) != 0) && (func_ov030_020aebb0(0x49) == 0)) {
            func_ov030_020aeb34(0x49);
        }
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0_2 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0_2 = 0;
        }
        if (((s32)var_r0_2 >= 3) && (func_ov030_020aebb0(0x46) == 0)) {
            func_ov030_020aeb34(0x46);
            func_ov030_020a9c54(arg0, 0x4DF, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x46) != 0) && (func_ov030_020aebb0(5) == 0)) {
            func_ov030_020aeb34(5);
            func_ov030_020a9c54(arg0, 0xDE, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) != 0) &&
            (func_ov030_020aebb0(6) == 0))
        {
            func_ov030_020aeb34(6);
            func_ov030_020a9c54(arg0, 0xDF, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) != 0) &&
            (func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(9) == 0))
        {
            func_ov030_020aeb70(39);
            func_ov030_020aeb70(0x28);
            func_ov030_020aeb70(41);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
        }
        if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(8) == 0)) {
            func_ov030_020aeb34(8);
            func_ov030_020a9c54(arg0, 0xE6, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(9) == 0)) {
            func_ov030_020aeb34(9);
            func_ov030_020a9c54(arg0, 0xEA, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(0x4B) == 0)) {
            func_ov030_020aeb34(0x4B);
            func_ov030_020a9c54(arg0, 0x4E1, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x4B) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0xEB, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x47) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0xEC, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208a028(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3626:
            func_ov030_020a669c(0xE7);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3627:
            func_ov030_020a669c(0xE8);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a0a0(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x3620:
            func_ov030_020a669c(0x3FA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3621:
            func_ov030_020a669c(0x3FB);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a120(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x3620:
            func_ov030_020a669c(0x3FF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3621:
            func_ov030_020a669c(0x400);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208a19c(RewardTableObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (func_ov030_020aebb0(0x2C) != 0) {
                func_ov030_020a9db8(arg0, 0x460);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x45F);
            return 0;
        case 1:
            func_ov030_020a669c(0x451);
            return 1;
        case 0:
            if (func_ov030_020aebb0(47) == 0) {
                func_ov030_020aeb34(47);
                func_ov030_020a669c(0x44E);
                return 1;
            }
            if (func_ov030_020aebb0(0x30) == 0) {
                func_ov030_020aeb34(0x30);
                func_ov030_020a669c(0x44F);
                return 1;
            }
            func_ov030_020aeb70(0x30);
            func_ov030_020a669c(0x450);
            return 1;
        case 2:
            if (func_ov030_020aebb0(45) == 0) {
                func_ov030_020aeb34(45);
                func_ov030_020a669c(0x452);
                return 1;
            }
            if (func_ov030_020aebb0(46) == 0) {
                func_ov030_020aeb34(46);
                func_ov030_020a669c(0x453);
                return 1;
            }
            func_ov030_020aeb70(46);
            func_ov030_020a669c(0x454);
            return 1;
        case 3:
            func_ov030_020a669c(0x455);
            return 1;
        case 4:
            func_ov030_020a669c(0x456);
            return 1;
        case 5:
            func_ov030_020a669c(0x457);
            return 1;
        case 6:
            func_ov030_020a669c(0x458);
            return 1;
        case 7:
            func_ov030_020a669c(0x459);
            return 1;
        case 8:
            func_ov030_020a669c(0x45A);
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

s32 func_ov030_0208a3f0(RewardTableObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (func_ov030_020aebb0(0x31) != 0) {
                func_ov030_020a9db8(arg0, 0x476);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x475);
            return 0;
        case 1:
            func_ov030_020a669c(0x467);
            return 1;
        case 0:
            if (func_ov030_020aebb0(0x34) == 0) {
                func_ov030_020aeb34(0x34);
                func_ov030_020a669c(0x464);
                return 1;
            }
            if (func_ov030_020aebb0(0x35) == 0) {
                func_ov030_020aeb34(0x35);
                func_ov030_020a669c(0x465);
                return 1;
            }
            func_ov030_020aeb70(0x35);
            func_ov030_020a669c(0x466);
            return 1;
        case 2:
            if (func_ov030_020aebb0(0x32) == 0) {
                func_ov030_020aeb34(0x32);
                func_ov030_020a669c(0x468);
                return 1;
            }
            if (func_ov030_020aebb0(0x33) == 0) {
                func_ov030_020aeb34(0x33);
                func_ov030_020a669c(0x469);
                return 1;
            }
            func_ov030_020aeb70(0x33);
            func_ov030_020a669c(0x46A);
            return 1;
        case 3:
            func_ov030_020a669c(0x46B);
            return 1;
        case 4:
            func_ov030_020a669c(0x46C);
            return 1;
        case 5:
            func_ov030_020a669c(0x46D);
            return 1;
        case 6:
            func_ov030_020a669c(0x46E);
            return 1;
        case 7:
            func_ov030_020a669c(0x46F);
            return 1;
        case 8:
            func_ov030_020a669c(0x470);
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

s32 func_ov030_0208a648(RewardTableObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (func_ov030_020aebb0(0x36) != 0) {
                func_ov030_020a9db8(arg0, 0x48B);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x48A);
            return 0;
        case 1:
            func_ov030_020a669c(0x47C);
            return 1;
        case 0:
            if (func_ov030_020aebb0(0x39) == 0) {
                func_ov030_020aeb34(0x39);
                func_ov030_020a669c(0x479);
                return 1;
            }
            if (func_ov030_020aebb0(58) == 0) {
                func_ov030_020aeb34(58);
                func_ov030_020a669c(0x47A);
                return 1;
            }
            func_ov030_020aeb70(58);
            func_ov030_020a669c(0x47B);
            return 1;
        case 2:
            if (func_ov030_020aebb0(0x37) == 0) {
                func_ov030_020aeb34(0x37);
                func_ov030_020a669c(0x47D);
                return 1;
            }
            if (func_ov030_020aebb0(56) == 0) {
                func_ov030_020aeb34(56);
                func_ov030_020a669c(0x47E);
                return 1;
            }
            func_ov030_020aeb70(56);
            func_ov030_020a669c(0x47F);
            return 1;
        case 3:
            func_ov030_020a669c(0x480);
            return 1;
        case 4:
            func_ov030_020a669c(0x481);
            return 1;
        case 5:
            func_ov030_020a669c(0x482);
            return 1;
        case 6:
            func_ov030_020a669c(0x483);
            return 1;
        case 7:
            func_ov030_020a669c(0x484);
            return 1;
        case 8:
            func_ov030_020a669c(0x485);
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

s32 func_ov030_0208a8a0(RewardTableObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (func_ov030_020aebb0(0x3B) != 0) {
                func_ov030_020a9db8(arg0, 0x4A0);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x49F);
            return 0;
        case 1:
            func_ov030_020a669c(0x491);
            return 1;
        case 0:
            if (func_ov030_020aebb0(0x3E) == 0) {
                func_ov030_020aeb34(0x3E);
                func_ov030_020a669c(0x48E);
                return 1;
            } else if (func_ov030_020aebb0(0x3F) == 0) {
                func_ov030_020aeb34(0x3F);
                func_ov030_020a669c(0x48F);
                return 1;
            }
            func_ov030_020aeb70(0x3F);
            func_ov030_020a669c(0x490);
            return 1;
        case 2:
            if (func_ov030_020aebb0(0x3C) == 0) {
                func_ov030_020aeb34(0x3C);
                func_ov030_020a669c(0x492);
                return 1;
            } else if (func_ov030_020aebb0(0x3D) == 0) {
                func_ov030_020aeb34(0x3D);
                func_ov030_020a669c(0x493);
                return 1;
            }
            func_ov030_020aeb70(0x3D);
            func_ov030_020a669c(0x494);
            return 1;
        case 3:
            func_ov030_020a669c(0x495);
            return 1;
        case 4:
            func_ov030_020a669c(0x496);
            return 1;
        case 5:
            func_ov030_020a669c(0x497);
            return 1;
        case 6:
            func_ov030_020a669c(0x498);
            return 1;
        case 7:
            func_ov030_020a669c(0x499);
            return 1;
        case 8:
            func_ov030_020a669c(0x49A);
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

s32 func_ov030_0208aaf4(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0xDB:
            data_02071cf0.unk_20.unk_24BE = 0xEE;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(0, 4);
            break;
        case 0xDC:
            data_02071cf0.unk_20.unk_30FC = 0;
            data_02071cf0.unk_20.unk_30FE = 39;
            data_02071cf0.unk_20.unk_3100 = 0;
            break;
        case 0xDE:
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9b1c;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xDF:
            func_ov030_020aeb70(39);
            func_ov030_020aeb70(0x28);
            func_ov030_020aeb70(41);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE1:
            func_ov030_020aa9f4(arg0, 0x3621, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE0:
            func_ov030_020aeb34(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0xE2:
            func_ov030_020a669c(0xE3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x45F:
            func_ov030_020aeb34(0x2C);
        case 0x460:
            func_ov030_020aeb70(47);
            func_ov030_020aeb70(0x30);
            func_ov030_020aeb70(45);
            func_ov030_020aeb70(46);
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
            func_ov030_020a669c(0x462);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x461:
            func_ov030_020aeb34(10);
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
            func_ov030_020aeb34(38);
            break;
        case 0xE6:
        case 0xE9:
            func_ov030_020aeb34(8);
            return func_ov030_020a9f54(arg0, &func_ov030_0208a028, 0x3625, 2, 0x3626, 0x3627);
        case 0xE8:
            arg0->unk_21CF8 = 1;
            break;
        case 0x45D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x461);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x475:
            func_ov030_020aeb34(0x31);
        case 0x476:
            func_ov030_020aeb70(0x34);
            func_ov030_020aeb70(0x35);
            func_ov030_020aeb70(0x32);
            func_ov030_020aeb70(0x33);
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
            func_ov030_020aeb34(0x40);
        case 0x473:
        case 0x472:
            func_ov030_020a669c(0x477);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x48A:
            func_ov030_020aeb34(0x36);
        case 0x48B:
            func_ov030_020aeb70(0x39);
            func_ov030_020aeb70(58);
            func_ov030_020aeb70(0x37);
            func_ov030_020aeb70(56);
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
            func_ov030_020aeb34(0x41);
        case 0x488:
        case 0x487:
            func_ov030_020a669c(0x48C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x49F:
            func_ov030_020aeb34(0x3B);
        case 0x4A0:
            func_ov030_020aeb70(0x3E);
            func_ov030_020aeb70(0x3F);
            func_ov030_020aeb70(0x3C);
            func_ov030_020aeb70(61);
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
            func_ov030_020aeb34(0x42);
        case 0x49D:
        case 0x49C:
            func_ov030_020aeb34(37);
            func_ov030_020a669c(0x4A1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x4A1:
            arg0->unk_21CF8 = 1;
            break;
        case 0xEA:
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9b44;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0) && (func_ov030_020aebb0(0x40) != 0) &&
                (func_ov030_020aebb0(0x41) != 0) && (func_ov030_020aebb0(0x42) != 0))
            {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0xED);
                func_ov030_020a6738(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0xEB:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84E0;
            func_ov030_020af364(13);
            return 1;
        case 0xEC:
            data_02071cf0.unk_20.unk_3124 = 0xE;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x797:
            if (func_ov030_020aebb0(0x44) != 0) {
                func_ov030_020a669c(0x799);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x798);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x798:
            if (func_ov030_020aebb0(0x43) == 0) {
                func_ov030_020aeb34(0x43);
                data_02071cf0.unk_20.unk_30FC = 0;
                data_02071cf0.unk_20.unk_30FE = 2;
                data_02071cf0.unk_20.unk_3100 = 6;
            }
            break;
        case 0x799:
            func_ov030_020aeb34(3);
            func_ov030_020aeb34(0x44);
            data_02071cf0.unk_20.unk_265A = 4;
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_DURABLE_LEATHER);
            func_ov030_020aac28(arg0, 0x5C);
            arg0->unk_21CF8 = 1;
            break;
        case 0x79A:
            func_ov030_020a669c(0x79B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x79B:
            if (Inventory_HasRequiredQuantity(ITEM_CD_TRACK01, 1, 0) != 0) {
                func_ov030_020a669c(0x79C);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x79C:
            func_ov030_020aeb34(4);
            func_ov030_020aeb34(0x49);
            data_02071cf0.unk_20.unk_265E = 4;
            ProgressReward_GrantItem(arg0, ITEM_QUEST_CRIMSON_CHILI);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4DE:
            func_ov030_020aeb34(0x45);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x4DF:
            func_ov030_020aeb34(0x46);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BLOCK_SHIKI);
            break;
        case 0x4E0:
            func_ov030_020aeb34(0x47);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 5);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_RETRY_BATTLES);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            break;
        case 0x4E1:
            func_ov030_020aeb34(0x4B);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0xED:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x80F1;
    data_02071cf0.unk_20.unk_3124 = 0x3C;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_2648 = 4;
    data_02071cf0.unk_20.unk_264E = 4;
    data_02071cf0.unk_20.unk_2658 = 4;
    data_02071cf0.unk_20.unk_265A = 4;
    data_02071cf0.unk_20.unk_2664 = 4;
    data_02071cf0.unk_20.unk_2662 = 4;
    func_ov030_020aeb34(42);
}

s32 func_ov030_0208b97c(s32 arg0) {
    s16 var_r0;
    s16 var_r0_2;
    s16 var_r0_3;
    s16 var_r0_4;

    if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(4) == 0)) {
        if (func_ov030_020aebb0(0x33) == 0) {
            if (Inventory_HasRequiredQuantity(0, 0, 0) != 0) {
                func_ov030_020aeb34(0x33);
            }
        } else if (Inventory_HasRequiredQuantity(0, 0, 0) == 0) {
            func_ov030_020aeb34(0x43);
            func_ov030_020aeb70(0x33);
        }
    }
    if ((func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(6) == 0)) {
        if (func_ov030_020aebb0(0x35) == 0) {
            if (Inventory_HasRequiredQuantity(0, 0, 0) != 0) {
                func_ov030_020aeb34(0x35);
            }
        } else if (Inventory_HasRequiredQuantity(0, 0, 0) == 0) {
            func_ov030_020aeb34(0x44);
            func_ov030_020aeb70(0x35);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 3) && (func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(0x32) == 0)) {
            func_ov030_020aeb34(0x32);
        }
        if ((func_ov030_020aed9c(0x3747) != 0) && (func_ov030_020aebb0(12) == 0)) {
            func_ov030_020aeb34(12);
            func_ov030_020a9c54(arg0, 0xF5, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x36) == 0)) {
            func_ov030_020aeb34(0x36);
            func_ov030_020a9c54(arg0, 0x4E8, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(13) == 0)) {
            func_ov030_020aeb34(13);
            func_ov030_020a9c54(arg0, 0xF6, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(8) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
            func_ov030_020aeb34(0xE);
            func_ov030_020a9c54(arg0, 0xF7, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(15) == 0)) {
            func_ov030_020aeb34(15);
            func_ov030_020a9c54(arg0, 250, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(0x24) == 0) && (func_ov030_020aebb0(0x11) == 0))
        {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0xFD, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(0x24) != 0) &&
            (func_ov030_020aebb0(0x11) == 0))
        {
            func_ov030_020aeb70(0x24);
        }
        if ((func_02023588() >= 1U) && (func_ov030_020aebb0(18) != 0)) {
            func_ov030_020aeb34(0x16);
        }
        if ((func_02023588() >= 2U) && (func_ov030_020aebb0(18) != 0)) {
            func_ov030_020aeb34(23);
        }
        if ((func_02023588() >= 3U) && (func_ov030_020aebb0(18) != 0)) {
            func_ov030_020aeb34(0x18);
        }
        if ((func_02023588() >= 3U) && (func_ov030_020aebb0(18) != 0) && (func_ov030_020aebb0(0x37) == 0)) {
            func_ov030_020aeb34(0x37);
            func_ov030_020a9c54(arg0, 0x4E9, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x37) != 0) && (func_ov030_020aebb0(0x13) == 0)) {
            func_ov030_020aeb34(0x13);
            func_ov030_020a9c54(arg0, 0x100, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (func_ov030_020aebb0(0x15) != 0) && (func_ov030_020aebb0(38) == 0))
        {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x104, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (func_ov030_020aebb0(0x15) != 0) && (func_ov030_020aebb0(38) != 0))
        {
            func_ov030_020aeb70(38);
        }
        if ((func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(0x46) == 0)) {
            func_ov030_020aeb34(0x46);
            func_ov030_020a9c54(arg0, 0x10C, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 3) {
        if ((func_ov030_020aebb0(0x13) != 0) && (func_ov030_020aebb0(0x14) == 0)) {
            func_ov030_020aeb34(0x14);
            func_ov030_020a9c54(arg0, 0x101, 0);
            return 1;
        }
        if ((func_02023588() >= 1U) && (func_ov030_020aebb0(0x14) != 0)) {
            func_ov030_020aeb34(0x19);
        }
        if ((func_02023588() >= 2U) && (func_ov030_020aebb0(0x14) != 0)) {
            func_ov030_020aeb34(0x1A);
        }
        if ((func_02023588() >= 3U) && (func_ov030_020aebb0(0x14) != 0)) {
            func_ov030_020aeb34(0x1B);
        }
        if ((func_02023588() >= 3U) && (func_ov030_020aebb0(0x14) != 0) && (func_ov030_020aebb0(56) == 0)) {
            func_ov030_020aeb34(56);
            func_ov030_020a9c54(arg0, 0x4EA, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(56) != 0) && (func_ov030_020aebb0(0x15) == 0)) {
            func_ov030_020aeb34(0x15);
            func_ov030_020a9c54(arg0, 0x102, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 4) != 0) || (func_ov030_020848e4(arg0, 5) != 0)) && (func_ov030_020aebb0(0x15) != 0) &&
            (func_ov030_020aebb0(37) == 0))
        {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x103, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 4) == 0) && (func_ov030_020848e4(arg0, 5) == 0) && (func_ov030_020aebb0(0x15) != 0) &&
            (func_ov030_020aebb0(37) != 0))
        {
            func_ov030_020aeb70(37);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 4) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0_2 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0_2 = 0;
        }
        if (((s32)var_r0_2 >= 3) && (func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
        }
        if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0xF8, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(0x45) != 0) && (func_ov030_020aebb0(1) == 0)) {
            func_ov030_020aeb34(1);
            func_ov030_020a9c54(arg0, 0x10D, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0) {
        if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0xFB, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0xF9, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 8) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0_3 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0_3 = 0;
        }
        if (((s32)var_r0_3 >= 2) && (func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x34) == 0)) {
            func_ov030_020aeb34(0x34);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 12) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0_4 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0_4 = 0;
        }
        if (((s32)var_r0_4 >= 2) && (func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            func_ov030_020aeb34(0x35);
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(7);
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 37) && (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) == 0)) {
        func_ov030_020aeb34(41);
        func_ov030_020a9c54(arg0, 0x10A, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_0208c3d8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x3628:
            func_ov030_020a669c(0x404);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3629:
            func_ov030_020a669c(0x405);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362A:
            func_ov030_020a669c(0x406);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c480(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x3628:
            func_ov030_020a669c(0x40A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3629:
            func_ov030_020a669c(0x40B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362A:
            func_ov030_020a669c(0x40C);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c528(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x3628:
            func_ov030_020a669c(0x40A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3629:
            func_ov030_020a669c(0x40E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362A:
            func_ov030_020a669c(0x40C);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c5d0(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x3628:
            func_ov030_020a669c(0x412);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3629:
            func_ov030_020a669c(0x413);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362A:
            func_ov030_020a669c(0x414);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c678(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x362C:
            func_ov030_020aeb34(0x39);
            func_ov030_020a669c(0x7A3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362D:
            func_ov030_020aeb34(58);
            func_ov030_020a669c(0x7A3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362E:
            func_ov030_020aeb34(0x3B);
            func_ov030_020a669c(0x7A3);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c730(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3630:
            func_ov030_020aeb34(0x3C);
            func_ov030_020a669c(0x7A4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3631:
            func_ov030_020aeb34(61);
            func_ov030_020a669c(0x7A4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3632:
            func_ov030_020aeb34(0x3E);
            func_ov030_020a669c(0x7A4);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c7e8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3634:
            func_ov030_020aeb34(0x3F);
            func_ov030_020a669c(0x7A5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3635:
            func_ov030_020aeb34(0x40);
            func_ov030_020a669c(0x7A5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3636:
            func_ov030_020aeb34(0x41);
            func_ov030_020a669c(0x7A5);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208c8a0(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0xF1:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xF2);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xF2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0xF3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0xF3:
            data_02071cf0.unk_20.unk_24BE = 0x10E;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(1, 4);
            ProgressReward_GrantPin(arg0, PIN_RED_SKULL);
            break;
        case 0xF4:
            func_ov030_020aeb34(11);
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
            func_ov030_020aeb34(8);
            break;
        case 0x403:
            arg0->unk_21CF8 = 1;
            break;
        case 0x40F:
        case 0x410:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x410, 0x411, &func_ov030_0208c5d0);
        case 0x407:
        case 0x414:
            func_ov030_020aeb34(10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x412:
            func_ov030_020aeb34(0x45);
            break;
        case 0x40B:
        case 0x40D:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x40D, 0x409, &func_ov030_0208c528);
        case 0x408:
            return func_ov030_020aa0c8(arg0, 0xB7, 0x408, 0x409, &func_ov030_0208c480);
        case 0x40E:
            func_ov030_020aeb34(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0xFA:
            data_02071cf0.unk_20.unk_264A = 2;
            data_02071cf0.unk_20.unk_264E = 2;
            data_02071cf0.unk_20.unk_2648 = 2;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020aec38(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            arg0->unk_21CF8                               = 1;
            break;
        case 0xFC:
            func_ov030_020aeb34(0x10);
            arg0->unk_21CF8 = 1;
            break;
        case 0xFE:
            func_ov030_020aeb34(0x11);
            data_02071cf0.unk_20.unk_264A = 4;
            data_02071cf0.unk_20.unk_264E = 4;
            data_02071cf0.unk_20.unk_2648 = 4;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            arg0->unk_21CF8                               = 1;
            break;
        case 0xFF:
            func_02023598(0);
            data_02071cf0.unk_20.unk_312C = 1;
            func_ov030_020aeb34(18);
            func_ov030_020a669c(0x43F);
            func_ov030_020a6738(arg0);
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
            data_02071cf0.unk_20.unk_312C = 2;
            arg0->unk_21CF8               = 1;
            break;
        case 0x102:
            func_ov030_020aec38(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x104:
            data_02071cf0.unk_20.unk_2648 = 2;
            data_02071cf0.unk_20.unk_264A = 2;
            data_02071cf0.unk_20.unk_264E = 2;
            break;
        case 0x106:
            func_ov030_020aeb34(0x28);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x107);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x107:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x108);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x108:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x109);
            func_ov030_020a6738(arg0);
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
            func_ov030_020a669c(0x10B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x10B:
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_SHIKI;
            arg0->unk_21AD0                               = 1;
            arg0->unk_21ACC                               = 0;
            arg0->unk_21AFC                               = 1;
            arg0->unk_21B00                               = 0x66;
            arg0->unk_21B08                               = 0x15D000;
            arg0->unk_21B0C                               = 0xD6000;
            break;
        case 0x10C:
            data_02071cf0.unk_20.unk_3124 = 15;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x79D:
            if (func_ov030_020aebb0(46) == 0) {
                func_ov030_020aeb34(46);
                func_ov030_020a669c(0x79E);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x79F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x79F:
            func_ov030_020a669c(0x7A0);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7A0:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c678, 0x362B, 3, 0x362C, 0x362D, 0x362E);
        case 0x7A3:
            func_ov030_020a669c(0x7A1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7A1:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c730, 0x362F, 3, 0x3630, 0x3631, 0x3632);
        case 0x7A4:
            func_ov030_020a669c(0x7A2);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7A2:
            return func_ov030_020a9f54(arg0, &func_ov030_0208c7e8, 0x3633, 3, 0x3634, 0x3635, 0x3636);
        case 0x7A5:
            if ((func_ov030_020aebb0(0x3B) != 0) && (func_ov030_020aebb0(0x3C) != 0) && (func_ov030_020aebb0(0x41) != 0)) {
                func_ov030_020a669c(0x7A6);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7A7);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7A7:
            func_ov030_020aeb70(0x39);
            func_ov030_020aeb70(58);
            func_ov030_020aeb70(0x3B);
            func_ov030_020aeb70(0x3C);
            func_ov030_020aeb70(61);
            func_ov030_020aeb70(0x3E);
            func_ov030_020aeb70(0x3F);
            func_ov030_020aeb70(0x40);
            func_ov030_020aeb70(0x41);
            break;
        case 0x7A6:
            func_ov030_020aeb34(3);
            func_ov030_020aeb34(0x32);
            data_02071cf0.unk_20.unk_264A = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantPin(arg0, PIN_ORICHALCUM);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7A8:
            func_ov030_020a669c(0x7A9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7A9:
            func_ov030_020aeb34(47);
            if (Inventory_HasRequiredQuantity(0xFF, 3, 0) != 0) {
                func_ov030_020a669c(0x7AA);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7AA:
            func_ov030_020aeb34(4);
            func_ov030_020aeb34(0x33);
            data_02071cf0.unk_20.unk_2654 = 4;
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_SLEEK_SILK);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7AB:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(0x30) == 0)) {
                func_ov030_020a669c(0x7AF);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(7) != 0)) {
                func_ov030_020a669c(0x7AD);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(7) == 0)) {
                func_ov030_020a669c(0x7AE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7AC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7AC:
            func_ov030_020aeb34(0x30);
            break;
        case 0x7AF:
        case 0x7AD:
            func_ov030_020aeb34(5);
            func_ov030_020aeb34(0x34);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            data_02071cf0.unk_20.unk_2666 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7AE:
            func_ov030_020aeb34(5);
            func_ov030_020aeb34(0x34);
            data_02071cf0.unk_20.unk_2666 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7B0:
            func_ov030_020a669c(0x7B1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7B1:
            func_ov030_020aeb34(0x31);
            if (Inventory_HasRequiredQuantity(0x5C, 1, 3) != 0) {
                func_ov030_020a669c(0x7B2);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7B2:
            func_ov030_020aeb34(6);
            func_ov030_020aeb34(0x35);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_DURABLE_LEATHER);
            data_02071cf0.unk_20.unk_265E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x4E8:
            func_ov030_020aeb34(0x36);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_SHIKI);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4E9:
            func_ov030_020aeb34(0x37);
            ProgressReward_GrantPin(arg0, PIN_500_YEN);
            break;
        case 0x4EA:
            func_ov030_020aeb34(0x38);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x10D:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8111;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 61;
    data_02071cf0.unk_20.unk_264C = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_2648 = 4;
    data_02071cf0.unk_20.unk_264A = 4;
    data_02071cf0.unk_20.unk_264E = 4;
    data_02071cf0.unk_20.unk_2650 = 4;
    data_02071cf0.unk_20.unk_2652 = 4;
}

s32 func_ov030_0208d80c(RewardTableObject* arg0) {
    if ((func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(2) == 0)) {
        if (func_ov030_020aebb0(0x37) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) != 0) {
                func_ov030_020aeb34(0x37);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) == 0) {
            func_ov030_020aeb34(56);
            func_ov030_020aeb70(0x37);
        }
    }
    if ((func_ov030_020aebb0(0x39) != 0) && (func_ov030_020aebb0(4) == 0)) {
        if (func_ov030_020aebb0(58) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) != 0) {
                func_ov030_020aeb34(58);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_NATURAL_MAGNUM, 1, 0) == 0) {
            func_ov030_020aeb34(0x3B);
            func_ov030_020aeb70(58);
        }
    }
    if ((data_02071cf0.unk_20.unk_3102 >= 3) && (func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(0x2C) == 0)) {
        func_ov030_020aeb34(0x2C);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(37) == 0)) {
        func_ov030_020aeb34(37);
        data_02071cf0.unk_20.unk_3124 = 18;
        data_02071cf0.unk_20.unk_3128 = 1;
        arg0->unk_21630               = 4;
        DebugOvlDisp_Pop();
        return 0;
    }
    if ((data_02071cf0.unk_20.unk_2458 == 5) && (func_ov030_020848e4(arg0, 7) != 0) && (func_ov030_020aebb0(33) == 0) &&
        (func_ov030_020aebb0(43) == 0))
    {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 0x114, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x11) {
        if ((func_ov030_020848a4(0, 0x1EA, 0x190, 0x1F4) != 0) && (func_ov030_020aebb0(6) == 0)) {
            func_ov030_020a9c54(arg0, 0x117, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(39) == 0)) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x118, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if (func_ov030_020aebb0(0x20) == 0) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x11A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(0x23) == 0) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x811B, 0);
            return 1;
        } else if ((func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x11C, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208dbd8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3638:
            func_ov030_020aeb34(45);
            func_ov030_020a669c(0x7BC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3639:
            func_ov030_020aeb34(46);
            func_ov030_020a669c(0x7BC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x363A:
            func_ov030_020aeb34(47);
            func_ov030_020a669c(0x7BC);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208dc90(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x363C:
            func_ov030_020aeb34(0x30);
            func_ov030_020a669c(0x7BD);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x363D:
            func_ov030_020aeb34(0x31);
            func_ov030_020a669c(0x7BD);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x363E:
            func_ov030_020aeb34(0x32);
            func_ov030_020a669c(0x7BD);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208dd48(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3640:
            func_ov030_020aeb34(0x33);
            func_ov030_020a669c(0x7BE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3641:
            func_ov030_020aeb34(0x34);
            func_ov030_020a669c(0x7BE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3642:
            func_ov030_020aeb34(0x35);
            func_ov030_020a669c(0x7BE);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ddfc(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x111:
            data_02071cf0.unk_20.unk_24BE = 0x11F;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(18, 4);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x112);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x112:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x113);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7C7:
            if (func_ov030_020aebb0(0x2C) != 0) {
                func_ov030_020a669c(0x7C9);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7C8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7C8:
            if (func_ov030_020aebb0(43) == 0) {
                func_ov030_020aeb34(43);
                data_02071cf0.unk_20.unk_3102 = 0;
            }
            break;
        case 0x7C9:
            func_ov030_020aeb34(5);
            data_02071cf0.unk_20.unk_2656 = 4;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x115);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x115:
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_SHIKI);
            arg0->unk_21CF8 = 1;
            break;
        case 0x116:
        case 0x117:
            func_ov030_020aeb34(6);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84F2;
            func_ov030_020af364(15);
            return 1;
        case 0x118:
            func_ov030_020a669c(0x119);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x11B:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8415U;
            data_02071cf0.unk_20.unk_3124 = 0x10;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x415:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84F3U;
            func_ov030_020af364(0xE);
            return 1;
        case 0x11C:
            func_ov030_020aec38(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            data_02071cf0.unk_20.unk_24B4                 = 0;
            data_02071cf0.unk_20.unk_24B8                 = 1;
            data_02071cf0.unk_20.unk_24BC                 = 0x811D;
            data_02071cf0.unk_20.unk_3124                 = 0x11;
            data_02071cf0.unk_20.unk_3128                 = 0;
            arg0->unk_21630                               = 4;
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
            func_ov030_020a669c(0x7BB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7B4:
            if (Inventory_HasRequiredQuantity(ITEM_THREAD_FUNKY_SHADES, 1, 0) != 0) {
                func_ov030_020a669c(0x7B5);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7B5:
            func_ov030_020aeb34(2);
            func_ov030_020aeb34(0x37);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_CHOICE_MEAT);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            data_02071cf0.unk_20.unk_2648 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7B6:
            if (func_ov030_020aebb0(42) == 0) {
                func_ov030_020aeb34(42);
                func_ov030_020a669c(0x7B7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7B8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7B3:
            func_ov030_020a669c(0x7B4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7B9:
            return func_ov030_020a9f54(arg0, func_ov030_0208dbd8, 0x3637, 3, 0x3638, 0x3639, 0x363A);
        case 0x7B7:
            func_ov030_020a669c(0x7B9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7BA:
            return func_ov030_020a9f54(arg0, func_ov030_0208dc90, 0x363B, 3, 0x363C, 0x363D, 0x363E);
        case 0x7BC:
            func_ov030_020a669c(0x7BA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7BB:
            return func_ov030_020a9f54(arg0, func_ov030_0208dd48, 0x363F, 3, 0x3640, 0x3641, 0x3642);
        case 0x7BE:
            if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x35) != 0)) {
                func_ov030_020a669c(0x7BF);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7C0);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7C0:
            func_ov030_020aeb70(45);
            func_ov030_020aeb70(46);
            func_ov030_020aeb70(47);
            func_ov030_020aeb70(0x30);
            func_ov030_020aeb70(0x31);
            func_ov030_020aeb70(0x32);
            func_ov030_020aeb70(0x33);
            func_ov030_020aeb70(0x34);
            func_ov030_020aeb70(0x35);
            break;
        case 0x7BF:
            func_ov030_020aeb34(3);
            func_ov030_020aeb34(0x3C);
            data_02071cf0.unk_20.unk_264A = 4;
            ProgressReward_GrantItem(arg0, ITEM_QUEST_SWEET_SWEET_ONIONS);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7B8:
            func_ov030_020a669c(0x7B9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7C5:
            if (Inventory_HasRequiredQuantity(12, 1, 0) != 0) {
                func_ov030_020a669c(0x7C6);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7C6:
            func_ov030_020aeb34(4);
            func_ov030_020aeb34(58);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_STUNNING_CRYSTALS);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_COLORFUL_THREAD);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            data_02071cf0.unk_20.unk_264E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x4F2:
            func_ov030_020aeb34(0x28);
            func_ov030_020aac28(arg0, 39);
            break;
        case 0x4F3:
            func_ov030_020aeb34(41);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_SHIKI);
            func_ov030_020aac28(arg0, 4);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            break;
        case 0x11E:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_264C = 4;
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
    data_02071cf0.unk_20.unk_24B4                 = 0;
    data_02071cf0.unk_20.unk_24B8                 = 1;
    data_02071cf0.unk_20.unk_24BC                 = 0x8122;
    data_02071cf0.unk_20.unk_3124                 = 0x3E;
}

s32 func_ov030_0208e7e4(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020848e4(arg0, 2) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x8123, 0);
            return 1;
        } else if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(41) == 0)) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x446, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 3) {
        if ((func_ov030_020aebb0(37) == 0) && (func_ov030_020aebb0(0x23) != 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x8130, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x131, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 5) {
        if (func_ov030_020aebb0(4) == 0) {
            func_ov030_020aeb34(4);
            data_02071cf0.unk_20.unk_264C = 2;
            func_ov030_020a9c54(arg0, 0x124, 0);
            return 1;
        } else if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x126, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 2) != 0) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(0x24) != 0) &&
            (func_ov030_020aebb0(0x23) == 0) && (func_ov030_020aebb0(0x22) == 0))
        {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x128, 0);
            return 1;
        }
        if (func_ov030_020848e4(arg0, 2) == 0) {
            func_ov030_020aeb70(0x22);
        }
        if (func_ov030_02084860(0xEB, 0xAF, 0x64) != 0) {
            func_ov030_020aeb34(0x24);
        }
        if ((func_ov030_020aebb0(0x23) == 0) && (func_ov030_020aed9c(0x3767) != 0)) {
            func_ov030_020aeb34(0x23);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8129;
            data_02071cf0.unk_20.unk_264C = 4;
            data_02071cf0.unk_20.unk_3124 = 0x13;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        } else if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x445, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0208eb40(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3644:
            func_ov030_020a669c(0x12B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3645:
            func_ov030_020a669c(0x12C);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ebbc(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3647:
            func_ov030_020a669c(0x12E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3648:
            func_ov030_020a669c(0x12F);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208ec3c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x122:
            data_02071cf0.unk_20.unk_24BE = 0x133;
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
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8125U;
            data_02071cf0.unk_20.unk_3124 = 0x14;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x125:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84FCU;
            func_ov030_020af364(0x10);
            return 1;
        case 0x126:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x129:
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
            func_ov030_020aeb34(2);
            func_ov030_020aec38(9);
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7CA:
            if (func_ov030_020aebb0(0x23) != 0) {
                func_ov030_020a669c(0x7CB);
                func_ov030_020a6738(arg0);
                return 1;
            }
        case 0x7CB:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x12A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x12A:
            return func_ov030_020a9f54(arg0, func_ov030_0208eb40, 0x3643, 2, 0x3644, 0x3645);
        case 0x12B:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x812DU;
            func_ov030_020af364(18);
            return 1;
        case 0x12D:
            return func_ov030_020a9f54(arg0, func_ov030_0208ebbc, 0x3646, 2, 0x3647, 0x3648);
        case 0x12E:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x12DU;
            func_ov030_020af364(18);
            return 1;
        case 0x12F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x7CC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7CC:
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_2648 = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            func_ov030_020aac28(arg0, 0x68);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_CONTROLS_PRESS);
            arg0->unk_21CF8 = 1;
            break;
        case 0x130:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x84FDU;
            func_ov030_020af364(0x11);
            return 1;
        case 0x131:
            data_02071cf0.unk_20.unk_3124 = 0x15;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x4FC:
            func_ov030_020aeb34(39);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_JOSHUA);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_SIDESTEP);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_HIGH_AND_LOW_CARDS);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x4FD:
            func_ov030_020aeb34(0x28);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_HARD);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x132:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(37);
    data_02071cf0.unk_20.unk_264C = 0x15D000;
    data_02071cf0.unk_20.unk_2650 = 0xD6000;
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_26B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8136;
    data_02071cf0.unk_20.unk_3124 = 0x3F;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_264A = 2;
    data_02071cf0.unk_20.unk_264E = 2;
    data_02071cf0.unk_20.unk_2658 = 4;
    data_02071cf0.unk_20.unk_265A = 4;
    data_02071cf0.unk_20.unk_2662 = 4;
    data_02071cf0.unk_20.unk_26C4 = 2;
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_0208f288(RewardTableObject* arg0) {
    if ((func_ov030_020aebb0(55) != 0) && (func_ov030_020aebb0(6) == 0)) {
        if (func_ov030_020aebb0(56) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) != 0) {
                func_ov030_020aeb34(56);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) == 0) {
            func_ov030_020aeb34(57);
            func_ov030_020aeb70(56);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (func_ov030_020aebb0(0x20) == 0) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x137, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (func_ov030_020aebb0(33) == 0) &&
            (func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(0x22) == 0))
        {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x139, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 3) == 0)) {
            func_ov030_020aeb70(33);
        }
        if ((func_ov030_020aebb0(0x22) == 0) && (func_ov030_020aed9c(0x3777) != 0)) {
            func_ov030_020aeb34(0x22);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x813A;
            data_02071cf0.unk_20.unk_3124 = 0x16;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
        if ((func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(7) == 0)) {
            func_ov030_020aeb34(7);
            func_ov030_020a9c54(arg0, 0x13E, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 4) && (func_ov030_020848a4(0, 0xC8, 250, 0xF0) != 0) &&
        (func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(8) != 0) && (func_ov030_020aebb0(38) == 0))
    {
        func_ov030_020aeb34(38);
        func_ov030_020a9c54(arg0, 0x143, 0);
        return 1;
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0) && (func_ov030_020848e4(arg0, 0x3C) != 0) && (func_ov030_020aebb0(8) != 0) &&
        (func_ov030_020aebb0(39) == 0))
    {
        func_ov030_020aeb34(39);
        func_ov030_020a9c54(arg0, 0x4144, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 11) {
        if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x13F, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(46) == 0)) {
            func_ov030_020aeb34(46);
        }
        if ((func_ov030_020848e4(arg0, 0xE) != 0) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x8140, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 12) {
        if ((func_ov030_020848e4(arg0, 13) != 0) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x141, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(8) == 0)) {
            func_ov030_020aeb34(8);
            func_ov030_020a9c54(arg0, 0x142, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 8) {
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x145, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(43) == 0)) {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x152, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 11) != 0) || (func_ov030_020848e4(arg0, 13) != 0)) && (func_ov030_020aebb0(43) != 0) &&
            (func_ov030_020aebb0(0x2C) == 0))
        {
            func_ov030_020aeb34(0x2C);
            func_ov030_020a9c54(arg0, 0x153, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            func_ov030_020aeb34(0x35);
            data_02071cf0.unk_20.unk_3124 = 0x19;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
            func_ov030_020a9c54(arg0, 0x13B, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) == 0)) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x146, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(0x34) == 0)) {
            func_ov030_020aeb34(0x34);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8154;
            data_02071cf0.unk_20.unk_3124 = 0x18;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }
    return 0;
}

s32 func_ov030_0208fa28(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x364A:
            func_ov030_020a669c(0x14A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x364B:
            func_ov030_020a669c(0x14B);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0208faa4(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
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
            func_ov030_020a669c(0x13C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x13C:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x813DU;
            data_02071cf0.unk_20.unk_3124 = 23;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
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
            data_02071cf0.unk_20.unk_24BE = 0x157;
            func_ov030_020aec1c(10);
            func_ov030_020aeaa0(8, 4);
            data_02071cf0.unk_20.unk_264A = 4;
            data_02071cf0.unk_20.unk_264E = 4;
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
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
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x506U;
            func_ov030_020af364(0x13);
            return 1;
        case 0x142:
            data_02071cf0.unk_20.unk_2654 = 4;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 0x144:
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x156);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x146:
            func_ov030_020a669c(0x147);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x147:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x148;
            data_02071cf0.unk_20.unk_24C2 = 0x149;
            data_02071cf0.unk_20.unk_341C = 1;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x148:
        case 0x149:
            return func_ov030_020a9f54(arg0, func_ov030_0208fa28, 0x3649, 2, 0x364A, 0x364B);
        case 0x14A:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x148;
            data_02071cf0.unk_20.unk_24C2 = 0x149;
            data_02071cf0.unk_20.unk_341C = 1;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x14B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x14C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x14C:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x14D;
            data_02071cf0.unk_20.unk_24C2 = 0x14E;
            data_02071cf0.unk_20.unk_341C = 0;
            SndMgr_StartPlayingSE();
            DebugOvlDisp_Pop();
            return 0;
        case 0x14D:
        case 0x14E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x14F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x14F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x151);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x151:
            func_ov030_020aeb34(9);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        case 0x152:
            data_02071cf0.unk_20.unk_26C4 = 4;
            ProgressReward_GrantItem(arg0, ITEM_BOOK_TIN_PIN_SLAMMER);
            func_ov030_020aac28(arg0, 0xE6);
            func_ov030_020aac28(arg0, 0xE5);
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
            func_ov030_020a669c(0x155);
            func_ov030_020a6738(arg0);
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
            func_ov030_020aeb34(45);
            break;
        case 0x7D0:
            if (func_ov030_020aebb0(46) != 0) {
                func_ov030_020a669c(0x7D2);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7D1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7D1:
            if (func_ov030_020aebb0(4) == 0) {
                func_ov030_020aeb34(4);
                arg0->unk_21CF8 = 1;
            }
            break;
        case 0x7D2:
            func_ov030_020aeb34(3);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            data_02071cf0.unk_20.unk_2664 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7D3:
            if (func_ov030_020aebb0(0x22) != 0) {
                func_ov030_020a669c(0x7D4);
                func_ov030_020a6738(arg0);
                return 1;
            }
        case 0x7D4:
            func_ov030_020aeb34(47);
            if (func_02023f60(1, 10) != 0) {
                func_ov030_020a669c(0x7D5);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7D5:
            func_ov030_020aeb34(5);
            func_ov030_020aeb34(0x30);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_EXTRA_FINE_NOODLES);
            arg0->unk_21CF8 = 1;
            break;
        case 0x7D6:
            func_ov030_020a669c(0x7D7);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7D7:
            func_ov030_020aeb34(0x37);
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_DEFINITIVO_CHILI_DOG, 1, 0) != 0) {
                func_ov030_020a669c(0x7D8);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7D8:
            func_ov030_020aeb34(6);
            func_ov030_020aeb34(56);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_SPARKLING_SILVER);
            data_02071cf0.unk_20.unk_265E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x506:
            func_ov030_020aeb34(0x31);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0x56);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            break;
        case 0x156:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24BE = 0;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    data_02071cf0.unk_20.unk_26D4 = 2;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 0;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x15A;
    func_ov030_020c26bc(4);
    data_02071cf0.unk_20.unk_2648 = 0;
    data_02071cf0.unk_20.unk_264C = 0;
    data_02071cf0.unk_20.unk_2652 = 0;
    data_02071cf0.unk_20.unk_2678 = 0;
    data_02071cf0.unk_20.unk_2666 = 0;
    data_02071cf0.unk_20.unk_266A = 0;
    data_02071cf0.unk_20.unk_265C = 0;
    data_02071cf0.unk_20.unk_2676 = 0;
    data_02071cf0.unk_20.unk_2670 = 0;
    data_02071cf0.unk_20.unk_3124 = 0x40;
}

s32 func_ov030_02090520(RewardTableObject* arg0) {
    if ((func_ov030_020aebb0(0x59) != 0) && (func_ov030_020aebb0(4) == 0)) {
        if (func_ov030_020aebb0(0x5A) == 0) {
            if (func_ov030_020aaad8() == 10) {
                func_ov030_020aeb34(0x5A);
            }
        } else {
            s32 var_r0;
            if (func_ov030_020aaad8() == 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            if (var_r0 == 10) {
                func_ov030_020aeb34(0x5B);
                func_ov030_020aeb70(0x5A);
            }
        }
    }
    if ((func_ov030_020aebb0(0x48) != 0) && (func_ov030_020aebb0(6) == 0)) {
        if (func_ov030_020aebb0(0x5C) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) != 0) {
                func_ov030_020aeb34(0x5C);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_POP_PENDULUM, 1, 0) == 0) {
            func_ov030_020aeb34(0x5D);
            func_ov030_020aeb70(0x5C);
        }
    }
    if ((func_ov030_020aebb0(0x5E) != 0) && (func_ov030_020aebb0(7) == 0)) {
        if (func_ov030_020aebb0(0x5F) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) != 0) {
                func_ov030_020aeb34(0x5F);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_SUPPLY_FACTOR, 1, 0) == 0) {
            func_ov030_020aeb34(0x60);
            func_ov030_020aeb70(0x5F);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) == 0)) {
            func_ov030_020aeb34(10);
            func_ov030_020a9c54(arg0, 0x172, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(47) == 0)) {
            func_ov030_020aeb70(46);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 4) {
        if ((func_ov030_020848e4(arg0, 6) != 0) && (func_ov030_020aebb0(0x20) == 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x15B, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0) && (func_ov030_020aebb0(46) == 0)) {
            func_ov030_020aeb34(46);
            func_ov030_020a9c54(arg0, 0x173, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 5) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0) &&
        (func_ov030_020aebb0(46) == 0))
    {
        func_ov030_020aeb34(46);
        func_ov030_020a9c54(arg0, 0x173, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(12) == 0) &&
            (func_ov030_020aebb0(0x49) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(12) == 0) && (func_ov030_020aebb0(0x49) != 0) &&
            (func_ov030_020aebb0(0x4B) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (func_ov030_020aebb0(12) == 0) && (func_ov030_020aebb0(0x4B) != 0)) {
            func_ov030_020aeb70(0x4B);
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 11) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0) &&
        (func_ov030_020aebb0(46) == 0))
    {
        func_ov030_020aeb34(46);
        func_ov030_020a9c54(arg0, 0x173, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 12) {
        if (func_ov030_020aebb0(33) == 0) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x15C, 0);
            return 1;
        }
        s32 var_r0;
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 3) && (func_ov030_020aebb0(0x46) != 0) && (func_ov030_020aebb0(0x47) == 0)) {
            func_ov030_020aeb34(0x47);
        }
        if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x63) == 0)) {
            func_ov030_020aeb34(0x63);
            func_ov030_020a9c54(arg0, 0x447, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 13) {
        if ((func_ov030_020848e4(arg0, 15) != 0) && (func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(43) == 0)) {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x166, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x56) != 0) && (func_ov030_020aebb0(9) == 0)) {
            func_ov030_020aeb34(9);
            func_ov030_020a9c54(arg0, 0x167, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0)) {
            func_ov030_020aeb34(47);
            data_02071cf0.unk_20.unk_26D4 = 2;
            func_ov030_020a9c54(arg0, 0x174, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x30) == 0)) {
            func_ov030_020aeb34(0x30);
            func_ov030_020aeb34(0x31);
            func_ov030_020a9c54(arg0, 0x4175, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(0x31) == 0) &&
            (func_ov030_020aebb0(0x35) == 0))
        {
            func_ov030_020aeb34(0x31);
            func_ov030_020a9c54(arg0, 0x4177, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            func_ov030_020aeb70(0x31);
        }
        if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(11) == 0)) {
            func_ov030_020aeb34(11);
            func_ov030_020a9c54(arg0, 0x176, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x57) == 0)) {
            func_ov030_020aeb34(0x57);
            func_ov030_020a9c54(arg0, 0x511, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x57) != 0) && (func_ov030_020aebb0(0x32) == 0)) {
            func_ov030_020aeb34(0x32);
            func_ov030_020a9c54(arg0, 0x178, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
            func_ov030_020a9c54(arg0, 0x4179, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
            func_ov030_020a9c54(arg0, 0x4179, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(0x36) == 0)) {
            func_ov030_020aeb34(0x36);
            func_ov030_020a9c54(arg0, 0x17C, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(12) == 0)) {
            func_ov030_020aeb34(12);
            func_ov030_020a9c54(arg0, 0x417D, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(13) == 0)) {
            func_ov030_020aeb34(13);
            func_ov030_020a9c54(arg0, 0x17E, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(56) == 0)) {
            func_ov030_020aeb34(0x37);
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(0x37) != 0) &&
            (func_ov030_020aebb0(56) == 0))
        {
            func_ov030_020aeb34(56);
            func_ov030_020a9c54(arg0, 0x417F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(56) != 0) && (func_ov030_020aebb0(0x31) == 0)) {
            func_ov030_020aeb34(0x31);
            func_ov030_020a9c54(arg0, 0x4177, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (func_ov030_020aebb0(56) != 0) && (func_ov030_020aebb0(0x31) != 0)) {
            func_ov030_020aeb70(0x31);
        }
        if ((func_ov030_020aebb0(0x10) != 0) && (func_ov030_020aebb0(61) == 0)) {
            func_ov030_020aeb34(61);
            func_ov030_020a9c54(arg0, 0x18A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(0x11) != 0)) {
            if ((func_ov030_020aebb0(0x42) != 0) && (func_ov030_020aebb0(0x3E) == 0)) {
                func_ov030_020aeb34(0x3E);
                func_ov030_020a9c54(arg0, 0x418B, 0);
                return 1;
            }
            if ((func_ov030_020aebb0(0x43) != 0) && (func_ov030_020aebb0(0x39) == 0)) {
                func_ov030_020aeb34(0x39);
                func_ov030_020a9c54(arg0, 0x4180, 0);
                return 1;
            }
            if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(0x3B) == 0)) {
                func_ov030_020aeb34(0x3B);
                func_ov030_020a9c54(arg0, 0x4183, 0);
                return 1;
            }
        }
        if (func_ov030_020aebb0(0x3E) != 0) {
            func_ov030_020a9c54(arg0, 0x818C, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if ((func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(0x3C) == 0)) {
            func_ov030_020aeb34(0x3C);
            func_ov030_020a9c54(arg0, 0x187, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x58) == 0)) {
            func_ov030_020aeb34(0x58);
            func_ov030_020a9c54(arg0, 0x512, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x58) != 0) && (func_ov030_020aebb0(0x10) == 0)) {
            func_ov030_020aeb34(0x10);
            func_ov030_020a9c54(arg0, 0x189, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0xE) && (func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(45) == 0)) {
        data_02071cf0.unk_20.unk_26D2 = 4;
        func_ov030_020aeb34(45);
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x10) {
        if (func_ov030_020aebb0(0x22) == 0) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x15D, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x415E, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x15F, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(0x2C) == 0) &&
            (func_ov030_020aebb0(45) == 0))
        {
            func_ov030_020aeb34(0x2C);
            func_ov030_020a9c54(arg0, 0x416C, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(73) == 0) {
            func_ov030_020aeb34(73);
            func_ov030_020aeb34(75);
            func_ov030_020a9c54(arg0, 0x18D, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(73) != 0) && (func_ov030_020aebb0(75) == 0)) {
            func_ov030_020aeb34(75);
            func_ov030_020a9c54(arg0, 0x192, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209147c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3651:
            func_ov030_020aeb34(38);
            func_ov030_020a669c(0x161);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3652:
            func_ov030_020aeb34(39);
            func_ov030_020a669c(0x162);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3653:
            func_ov030_020aeb34(0x28);
            func_ov030_020a669c(0x163);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3654:
            func_ov030_020aeb34(41);
            func_ov030_020a669c(0x164);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091554(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3651:
            func_ov030_020aeb34(38);
            func_ov030_020a669c(0x161);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3652:
            func_ov030_020aeb34(39);
            func_ov030_020a669c(0x162);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3653:
            func_ov030_020aeb34(0x28);
            func_ov030_020a669c(0x163);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3654:
            func_ov030_020aeb34(41);
            func_ov030_020a669c(0x164);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3655:
            if (func_ov030_020aebb0(42) == 0) {
                func_ov030_020aeb34(42);
                func_ov030_020a669c(0x165);
                func_ov030_020a6738(arg0);
                return 1;
            }
    }
    return 0;
}

s32 func_ov030_02091664(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3657:
            func_ov030_020a669c(0x16E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3658:
            func_ov030_020a669c(0x16F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3659:
            func_ov030_020a669c(0x170);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x365A:
            func_ov030_020a669c(0x171);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209171c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x365C:
            func_ov030_020a669c(0x18E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x365D:
            func_ov030_020a669c(0x18F);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209179c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x365F:
            func_ov030_020aeb34(0x4D);
            func_ov030_020a669c(0x7ED);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3660:
            func_ov030_020aeb34(0x4E);
            func_ov030_020a669c(0x7ED);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3661:
            func_ov030_020aeb34(0x4F);
            func_ov030_020a669c(0x7ED);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02091854(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3663:
            func_ov030_020aeb34(0x50);
            func_ov030_020a669c(0x7EE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3664:
            func_ov030_020aeb34(0x51);
            func_ov030_020a669c(0x7EE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3665:
            func_ov030_020aeb34(0x52);
            func_ov030_020a669c(0x7EE);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209190c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3667:
            func_ov030_020aeb34(0x53);
            func_ov030_020a669c(0x7EF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3668:
            func_ov030_020aeb34(0x54);
            func_ov030_020a669c(0x7EF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3669:
            func_ov030_020aeb34(0x55);
            func_ov030_020a669c(0x7EF);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020919c4(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36EB:
            func_ov030_020a669c(0x51);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36EC:
            func_ov030_020a669c(0x52);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36ED:
            func_ov030_020a669c(0x53);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36EE:
            func_ov030_020a669c(0x54);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36EF:
            func_ov030_020a669c(0x55);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091a8c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x362C:
            func_ov030_020aeb34(0x3F);
            func_ov030_020aeb34(0x42);
            func_ov030_020a669c(0x419);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362D:
            if (func_ov030_020aebb0(0x40) == 0) {
                func_ov030_020aeb34(0x40);
                func_ov030_020aeb34(0x43);
                func_ov030_020a669c(0x41A);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x41C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362E:
            if (func_ov030_020aebb0(0x41) == 0) {
                func_ov030_020aeb34(0x41);
                func_ov030_020aeb34(0x44);
                func_ov030_020a669c(0x41B);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x41C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x362F:
            func_ov030_020a669c(0x41C);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02091bc8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
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
            func_ov030_020a669c(0x160);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x161:
        case 0x162:
        case 0x163:
        case 0x164:
            if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) != 0) &&
                (func_ov030_020aebb0(41) != 0))
            {
                return func_ov030_020a9f54(arg0, &func_ov030_02091554, 0x3650, 5, 0x3651, 0x3652, 0x3653, 0x3654, 0x3655);
            }
            return func_ov030_020a9f54(arg0, &func_ov030_0209147c, 0x3650, 4, 0x3651, 0x3652, 0x3653, 0x3654);
        case 0x165:
            func_ov030_020aeb34(0x45);
            break;
        case 0x166:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x510U;
            func_ov030_020af364(0x14);
            return 1;
        case 0x167:
            func_ov030_020aeb34(9);
            data_02071cf0.unk_20.unk_26BE = 2;
            break;
        case 0x168:
            func_ov030_020aeb34(18);
            func_ov030_020aeab8(8, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x169:
            func_ov030_020aeb34(0x13);
            func_ov030_020aeab8(0, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16A:
            func_ov030_020aeb34(0x14);
            func_ov030_020aeab8(6, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16B:
            func_ov030_020aeb34(0x15);
            arg0->unk_21CF8 = 1;
            break;
        case 0x16C:
            data_02071cf0.unk_20.unk_26D2 = 0;
            break;
        case 0x16D:
            func_ov030_020aeb34(0x16);
            return func_ov030_020a9f54(arg0, &func_ov030_02091664, 0x3656, 4, 0x3657, 0x3658, 0x3659, 0x365A);
        case 0x16E:
        case 0x16F:
        case 0x170:
        case 0x171:
            arg0->unk_21CF8 = 1;
            break;
        case 0x176:
            func_ov030_020aeb34(11);
            arg0->unk_21CF8 = 1;
            break;
        case 0x177:
            data_02071cf0.unk_20.unk_26D8 = 2;
            arg0->unk_21CF8               = 1;
            break;
        case 0x178:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x17A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x179:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x17B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x17A:
            func_ov030_020aeb34(0x35);
            data_02071cf0.unk_20.unk_26D8 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x17E:
            data_02071cf0.unk_20.unk_26BE = 0;
            data_02071cf0.unk_20.unk_26D8 = 0;
            break;
        case 0x17F:
            func_ov030_020aeb34(56);
            func_ov030_020aa9f4(arg0, 0x364D, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x180:
        case 0x181:
        case 0x183:
            func_ov030_020aeb70(0x11);
            if (func_ov030_020aebb0(58) == 0) {
                func_ov030_020aeb34(58);
                func_ov030_020a669c(0x181);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x182);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x182:
            if ((func_ov030_020aebb0(0x3B) != 0) && (func_ov030_020aebb0(0x61) == 0)) {
                func_ov030_020aeb34(0x61);
                func_ov030_020aac28(arg0, 0x110);
            }
            data_02071cf0.unk_20.unk_26D8 = 0;
            arg0->unk_21CF8               = 1;
            break;
        case 0x184:
            func_ov030_020aeb34(0xE);
            func_ov030_020aa9f4(arg0, 0x364E, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x185:
            func_ov030_020aeb34(23);
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
            func_ov030_020aeb34(0x11);
            data_02071cf0.unk_20.unk_26D8 = 2;
            arg0->unk_21CF8               = 1;
            break;
        case 0x188:
            func_ov030_020aeb34(15);
            arg0->unk_21CF8 = 1;
            break;
        case 0x18A:
            func_ov030_020aa9f4(arg0, 0x364C, 0x5DD1, 0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x18D:
            data_02071cf0.unk_20.unk_3124 = 0x1A;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x18E:
            func_ov030_020aeb34(0x49);
        case 0x192:
            return func_ov030_020a9f54(arg0, &func_ov030_0209171c, 0x365B, 2, 0x365C, 0x365D);
        case 0x18F:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x190;
            data_02071cf0.unk_20.unk_24C2 = 0x191;
            data_02071cf0.unk_20.unk_341C = 0x20;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x190:
            if (func_ov030_020aebb0(0x4A) == 0) {
                func_ov030_020aeb34(0x4A);
                func_ov030_020aac28(arg0, 0xE6);
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
            if (func_ov030_020aebb0(0x45) != 0) {
                func_ov030_020a669c(0x7DA);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7DA:
            func_ov030_020aeb34(0x59);
            if (func_ov030_020aaad8() == 10) {
                func_ov030_020a669c(0x7DB);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7DB:
            func_ov030_020aeb34(4);
            func_ov030_020aeb34(0x5A);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_FLUFFY_COTTON);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            data_02071cf0.unk_20.unk_2648 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7DC:
            if (func_ov030_020aebb0(0x47) != 0) {
                func_ov030_020a669c(0x7DE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7DD);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7DD:
            if (func_ov030_020aebb0(0x46) == 0) {
                func_ov030_020aeb34(0x46);
                data_02071cf0.unk_20.unk_30FC = 0;
                data_02071cf0.unk_20.unk_30FE = 1;
                data_02071cf0.unk_20.unk_3100 = 12;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x7DE:
            func_ov030_020aeb34(5);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            func_ov030_020aac28(arg0, 0x65);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_PHONE_CAMERA);
            data_02071cf0.unk_20.unk_2666 = 4;
            arg0->unk_21CF8               = 1;
        case 0x7DF:
            func_ov030_020a669c(0x7E0);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7E0:
            func_ov030_020aeb34(0x48);
            if (Inventory_HasRequiredQuantity(0x1C, 1, 0) != 0) {
                func_ov030_020a669c(0x7E1);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7E1:
            func_ov030_020aeb34(6);
            func_ov030_020aeb34(0x5C);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            data_02071cf0.unk_20.unk_266A = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7E2:
            func_ov030_020a669c(0x7E3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7E3:
            func_ov030_020aeb34(0x5E);
            if (Inventory_HasRequiredQuantity(56, 1, 0) != 0) {
                func_ov030_020a669c(0x7E4);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7E4:
            func_ov030_020aeb34(7);
            func_ov030_020aeb34(0x5F);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_HAMBURGER);
            data_02071cf0.unk_20.unk_2678 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7E5:
            if (func_ov030_020aebb0(0x45) != 0) {
                func_ov030_020a669c(0x7E7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7E6);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7E7:
            if (func_ov030_020aebb0(0x4C) == 0) {
                func_ov030_020aeb34(0x4C);
                func_ov030_020a669c(0x7E8);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7E9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7E9:
        case 0x7E8:
            func_ov030_020a669c(0x7EA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7EA:
            return func_ov030_020a9f54(arg0, &func_ov030_0209179c, 0x365E, 3, 0x365F, 0x3660, 0x3661);
        case 0x7ED:
            func_ov030_020a669c(0x7EB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7EB:
            return func_ov030_020a9f54(arg0, &func_ov030_02091854, 0x3662, 3, 0x3663, 0x3664, 0x3665);
        case 0x7EE:
            func_ov030_020a669c(0x7EC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7EC:
            return func_ov030_020a9f54(arg0, &func_ov030_0209190c, 0x3666, 3, 0x3667, 0x3668, 0x3669);
        case 0x7EF:
            if ((func_ov030_020aebb0(0x4D) != 0) && (func_ov030_020aebb0(0x52) != 0) && (func_ov030_020aebb0(0x54) != 0)) {
                func_ov030_020a669c(0x7F0);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7F1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7F1:
            func_ov030_020aeb70(0x4D);
            func_ov030_020aeb70(0x4E);
            func_ov030_020aeb70(0x4F);
            func_ov030_020aeb70(0x50);
            func_ov030_020aeb70(0x51);
            func_ov030_020aeb70(0x52);
            func_ov030_020aeb70(0x53);
            func_ov030_020aeb70(0x54);
            func_ov030_020aeb70(0x55);
            break;
        case 0x7F0:
            func_ov030_020aeb34(8);
            func_ov030_020aeb34(0x62);
            data_02071cf0.unk_20.unk_264C = 4;
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_COZY_DOWN);
            arg0->unk_21CF8 = 1;
            break;
        case 0x510:
            func_ov030_020aeb34(0x56);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, 0x75);
            break;
        case 0x511:
            func_ov030_020aeb34(0x57);
            func_ov030_020aac28(arg0, 0x7B);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case 0x512:
            func_ov030_020aeb34(0x58);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x193:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24BE = 2;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8197;
    func_ov030_020c26bc(4);
    data_02071cf0.unk_20.unk_264E = 0;
    data_02071cf0.unk_20.unk_2656 = 0;
    data_02071cf0.unk_20.unk_2654 = 0;
    data_02071cf0.unk_20.unk_2658 = 0;
    data_02071cf0.unk_20.unk_2676 = 0;
    data_02071cf0.unk_20.unk_2670 = 0;
    data_02071cf0.unk_20.unk_3124 = 0x41;
}

s32 func_ov030_02092d50(RewardTableObject* arg0) {
    s16 var_r0;

    if ((func_ov030_020aebb0(56) != 0) && (func_ov030_020aebb0(2) == 0)) {
        if (func_ov030_020aebb0(61) == 0) {
            if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) != 0) {
                func_ov030_020aeb34(61);
            }
        } else if (Inventory_HasRequiredQuantity(PIN_SEXY_PLUS, 1, 0) == 0) {
            func_ov030_020aeb34(0x3E);
            func_ov030_020aeb70(61);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(45) == 0)) {
            func_ov030_020aeb34(45);
            func_ov030_020a9c54(arg0, 0x1A7, 0);
            return 1;
        }
        if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
             (func_ov030_020848e4(arg0, 2) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) &&
            (func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(5) == 0))
        {
            func_ov030_020aeb34(5);
            func_ov030_020a9c54(arg0, 0x1AC, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 5) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 10) && (func_ov030_020aebb0(0x3B) != 0) && (func_ov030_020aebb0(0x3C) == 0)) {
            func_ov030_020aeb34(0x3C);
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 2) && (data_02071cf0.unk_20.unk_3102 >= 4) && (func_ov030_020aebb0(0x39) != 0) &&
        (func_ov030_020aebb0(58) == 0))
    {
        func_ov030_020aeb34(58);
    }
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if (func_ov030_020aebb0(39) == 0) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x19A, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (func_ov030_020aebb0(9) == 0)) {
            func_ov030_020aeb34(9);
            func_ov030_020a9c54(arg0, 0x19B, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x28) == 0) &&
            (func_ov030_020aebb0(15) == 0))
        {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x1A0, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) == 0) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x28) != 0)) {
            func_ov030_020aeb70(0x28);
        }
        if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0)) && (func_ov030_020aebb0(10) != 0) &&
            (func_ov030_020aebb0(41) == 0) && (func_ov030_020aebb0(11) == 0))
        {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x1A1, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 8) == 0) && (func_ov030_020848e4(arg0, 9) == 0) && (func_ov030_020aebb0(10) != 0) &&
            (func_ov030_020aebb0(41) != 0))
        {
            func_ov030_020aeb70(41);
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(0x37) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (func_ov030_020aebb0(0x37) != 0)) {
            func_ov030_020aeb70(0x37);
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0) && (func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(47) == 0)) {
        func_ov030_020aeb34(47);
        func_ov030_020a9c54(arg0, 0x1AD, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 7) {
        if (func_ov030_020aebb0(0x22) == 0) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x1D0, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x1D1, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 8) && (func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(33) == 0)) {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 0x1B2, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 9) {
        if (func_ov030_020aebb0(37) == 0) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x1D3, 0);
            return 1;
        }
        if ((func_ov030_020848a4(0x96, 0x15E, 0x190, 0x186) != 0) && (func_ov030_020aebb0(37) != 0) &&
            (func_ov030_020aebb0(6) == 0))
        {
            func_ov030_020aeb34(6);
            func_ov030_020a9c54(arg0, 0x81D4, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 12) != 0) && (func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x1D6, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 12) == 0) && (func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(38) != 0)) {
            func_ov030_020aeb70(38);
        }
        if ((func_ov030_020aed9c(0x3797) != 0) && (func_ov030_020aebb0(8) == 0)) {
            func_ov030_020aeb34(8);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x81D7;
            data_02071cf0.unk_20.unk_3124 = 0x1B;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 8) && (func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(33) == 0)) {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 0x1B2, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 13) && (func_ov030_020aebb0(0x32) == 0)) {
        func_ov030_020aeb34(0x32);
        func_ov030_020a9c54(arg0, 0x1CC, 0);
        return 1;
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x10) && (func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(0xE) != 0) &&
        (func_ov030_020aebb0(0x33) == 0))
    {
        func_ov030_020aeb34(0x33);
        func_ov030_020a9c54(arg0, 0x41CD, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(0x35) == 0) {
            func_ov030_020aeb34(0x35);
            func_ov030_020aeb34(0x37);
            func_ov030_020a9c54(arg0, 0x1D8, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(0x37) == 0)) {
            func_ov030_020aeb34(0x37);
            func_ov030_020a9c54(arg0, 0x1DD, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020936b8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366B:
            func_ov030_020a669c(0x19D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x366C:
            func_ov030_020a669c(0x19E);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093738(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            func_ov030_020aeb34(42);
            func_ov030_020a669c(0x1A3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x366F:
            func_ov030_020aeb34(43);
            func_ov030_020a669c(0x1A4);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020937c4(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            func_ov030_020aeb34(42);
            func_ov030_020a669c(0x1A3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x366F:
            func_ov030_020aeb34(43);
            func_ov030_020a669c(0x1A4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3670:
            func_ov030_020aeb34(0x2C);
            func_ov030_020a669c(0x1A5);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093880(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x366E:
            func_ov030_020aeb34(42);
            func_ov030_020a669c(0x1A3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x366F:
            func_ov030_020aeb34(43);
            func_ov030_020a669c(0x1A4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3670:
            func_ov030_020aeb34(0x2C);
            func_ov030_020a669c(0x1A5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3671:
            func_ov030_020aeb34(11);
            func_ov030_020a669c(0x1A6);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02093958(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3673:
            func_ov030_020a669c(0x1A9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3674:
            if (func_ov030_020aebb0(0xE) != 0) {
                func_ov030_020aeb34(12);
                func_ov030_020a669c(0x1AB);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x1AA);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093a0c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3676:
            func_ov030_020aeb34(0x30);
            func_ov030_020a669c(0x1AF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3677:
            func_ov030_020aeb34(0x31);
            func_ov030_020a669c(0x1B0);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093a98(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3679:
            func_ov030_020a669c(0x1B5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x367A:
            func_ov030_020a669c(0x1B6);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x367B:
            func_ov030_020a669c(0x1B7);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093b40(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x367E:
            func_ov030_020a669c(0x1BB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x367F:
            func_ov030_020a669c(0x1BC);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3680:
            func_ov030_020a669c(0x1BD);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093be0(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3682:
            func_ov030_020a669c(0x1BF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3683:
            func_ov030_020a669c(0x1C0);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3684:
            func_ov030_020a669c(0x1BF);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093c80(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3686:
            func_ov030_020a669c(0x1C5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3687:
            func_ov030_020a669c(0x1C5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3688:
            func_ov030_020a669c(0x1C5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3689:
            func_ov030_020a669c(0x1C6);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_02093d38(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x368B:
            func_ov030_020a669c(0x1D9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x368C:
            func_ov030_020a669c(0x1DA);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02093db8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x197:
            func_ov030_020aeaa0(6, 2);
            func_ov030_020aeaa0(9, 2);
            func_ov030_020aeaa0(13, 2);
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x198);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x198:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x199);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x199:
            func_ov030_020aeb34(0x20);
            break;
        case 0x19B:
        case 0x19C:
            func_ov030_020aeb34(9);
            return func_ov030_020a9f54(arg0, func_ov030_020936b8, 0x366A, 2, 0x366B, 0x366C);
        case 0x19D:
        case 0x19E:
            data_02071cf0.unk_20.unk_265C = 0;
            arg0->unk_21CF8               = 1;
            break;
        case 0x19F:
            func_ov030_020aeb34(10);
            data_02071cf0.unk_20.unk_2658 = 2;
            data_02071cf0.unk_20.unk_265A = 2;
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
            if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(0x2C) != 0)) {
                return func_ov030_020a9f54(arg0, func_ov030_02093880, 0x366D, 4, 0x366E, 0x366F, 0x3670, 0x3671);
            }
            if (func_ov030_020aebb0(43) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_020937c4, 0x366D, 3, 0x366E, 0x366F, 0x3670);
            }
            return func_ov030_020a9f54(arg0, func_ov030_02093738, 0x366D, 2, 0x366E, 0x366F);
        case 0x1A6:
            data_02071cf0.unk_20.unk_2650 = 4;
            data_02071cf0.unk_20.unk_2658 = 4;
            data_02071cf0.unk_20.unk_265A = 4;
            break;
        case 0x1A8:
            func_ov030_020aeb34(46);
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
            if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x31) != 0)) {
                func_ov030_020a669c(0x1B1);
                func_ov030_020a6738(arg0);
                return 1;
            }
            return func_ov030_020a9f54(arg0, func_ov030_02093a0c, 0x3675, 2, 0x3676, 0x3677);
        case 0x1B1:
            func_ov030_020aeb34(13);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1B2:
            func_ov030_020aac28(arg0, 0xF3);
            break;
        case 0x1B3:
            func_ov030_020aeb34(0xE);
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
            func_ov030_020a669c(0x1BA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1BA:
            return func_ov030_020a9f54(arg0, func_ov030_02093b40, 0x367D, 3, 0x367E, 0x367F, 0x3680);
        case 0x1BD:
        case 0x1BF:
            func_ov030_020a669c(0x1BF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1BE:
            return func_ov030_020a9f54(arg0, func_ov030_02093be0, 0x3681, 3, 0x3682, 0x3683, 0x3684);
        case 0x1C0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x1C1);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C1:
            func_ov030_020a669c(0x1C2);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C2:
            func_ov030_020a669c(0x1C3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C3:
            func_ov030_020a669c(0x1C4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C4:
        case 0x1C5:
            return func_ov030_020a9f54(arg0, func_ov030_02093c80, 0x3685, 4, 0x3686, 0x3687, 0x3688, 0x3689);
        case 0x1C6:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x1C7);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C7:
            func_ov030_020a669c(0x1C8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C8:
            func_ov030_020a669c(0x1C9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1C9:
            func_ov030_020a669c(0x1CA);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1CA:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x1CB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1CB:
            func_ov030_020aeb34(15);
            data_02071cf0.unk_20.unk_265C = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x1D0:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x851AU;
            func_ov030_020af364(0x15);
            return 1;
        case 0x1D4:
            func_ov030_020aec38(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            func_ov030_020aec1c(9);
            data_02071cf0.unk_20.unk_2660 = 2;
            func_ov030_020c596c();
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D5:
            func_ov030_020aeb34(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D7:
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
            func_ov030_020aec38(9);
            data_02071cf0.unk_20.unk_3124 = 0x1C;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1CC:
            func_ov030_020aeab8(13, 2);
            break;
        case 0x1CD:
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x1DE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x1CE:
            func_ov030_020aeb34(0x10);
            func_ov030_020aeab8(6, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1CF:
            func_ov030_020aeb34(0x11);
            func_ov030_020aeab8(1, 2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x1D8:
            func_ov030_020aeb34(0x35);
        case 0x1DD:
            return func_ov030_020a9f54(arg0, func_ov030_02093d38, 0x368A, 2, 0x368B, 0x368C);
        case 0x1D9:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_26C0 = 0x1DB;
            data_02071cf0.unk_20.unk_26C2 = 0x1DC;
            data_02071cf0.unk_20.unk_341C = 33;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1DB:
            if (func_ov030_020aebb0(0x36) == 0) {
                func_ov030_020aeb34(0x36);
                func_ov030_020aac28(arg0, 0xE9);
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
            func_ov030_020a669c(0x7F3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7F3:
            func_ov030_020aeb34(56);
            if (Inventory_HasRequiredQuantity(6, 1, 0) != 0) {
                func_ov030_020a669c(0x7F4);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x7F4:
            func_ov030_020aeb34(2);
            func_ov030_020aeb34(61);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            data_02071cf0.unk_20.unk_2654 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7F5:
            if (func_ov030_020aebb0(58) != 0) {
                func_ov030_020a669c(0x7F7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7F6);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7F6:
            if (func_ov030_020aebb0(0x39) == 0) {
                func_ov030_020aeb34(0x39);
                data_02071cf0.unk_20.unk_3102 = 0;
            }
            break;
        case 0x7F7:
            func_ov030_020aeb34(3);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            data_02071cf0.unk_20.unk_2658 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7F8:
            if (func_ov030_020aebb0(0x3C) != 0) {
                func_ov030_020a669c(0x7FA);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x7F9);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7F9:
            if (func_ov030_020aebb0(0x3B) == 0) {
                func_ov030_020aeb34(0x3B);
                data_02071cf0.unk_20.unk_30FC = 0;
                data_02071cf0.unk_20.unk_30FE = 39;
                data_02071cf0.unk_20.unk_3100 = 5;
            }
            break;
        case 0x7FA:
            func_ov030_020aeb34(4);
            ProgressReward_GrantItem(arg0, ITEM_QUEST_PRETTY_RIBBON);
            ProgressReward_GrantPin(arg0, PIN_1000_YEN);
            data_02071cf0.unk_20.unk_2656 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x51A:
            func_ov030_020aeb34(0x34);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0x9E);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case 0x1DE:
            func_ov030_020aeb34(1);
            if (Inventory_HasRequiredQuantity(0x233, 1, 0) != 0) {
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24BE = 2;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x81E2;
    func_ov030_020c26bc(4);
    data_02071cf0.unk_20.unk_2676 = 0;
    data_02071cf0.unk_20.unk_2670 = 0;
    data_02071cf0.unk_20.unk_3124 = 0x42;
}

s32 func_ov030_02094bfc(RewardTableObject* arg0) {
    s16 var_r0;

    if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(3) == 0)) {
        if (func_ov030_020aebb0(0x46) == 0) {
            if (func_ov030_020aaad8() == 1) {
                func_ov030_020aeb34(0x46);
            }
        } else if (func_ov030_020aaad8() != 1) {
            func_ov030_020aeb34(0x47);
            func_ov030_020aeb70(0x46);
        }
    }
    if ((func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(4) == 0)) {
        if (func_ov030_020aebb0(0x48) == 0) {
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) != 0) {
                func_ov030_020aeb34(0x48);
            }
        } else if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) == 0) {
            func_ov030_020aeb34(0x49);
            func_ov030_020aeb70(0x48);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (func_ov030_020aebb0(2) == 0)) {
            func_ov030_020aeb34(2);
            func_ov030_020a9c54(arg0, 0x1E4, 0);
            return 1;
        } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(39) == 0)) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x1ED, 0);
            return 1;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020848e4(arg0, 56) != 0) && (func_ov030_020aebb0(0x34) == 0)) {
        func_ov030_020aeb34(0x34);
        func_ov030_020a9c54(arg0, 0x4C, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(0x3F) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(0x3F) != 0) && (func_ov030_020aebb0(0x43) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (func_ov030_020aebb0(0x43) != 0)) {
            func_ov030_020aeb70(0x43);
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 12) && (func_ov030_020848e4(arg0, 15) != 0) && (func_ov030_020aebb0(5) != 0) &&
        (func_ov030_020aebb0(33) == 0))
    {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 0x1E7, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 13) && (func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(38) == 0)) {
        func_ov030_020aeb34(38);
        func_ov030_020a9c54(arg0, 0x1EC, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 15) {
        if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x1EA, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x1EB, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0xE) {
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if ((var_r0 >= 2) && (func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
        }
        if ((func_ov030_020848e4(arg0, 18) != 0) && (func_ov030_020aebb0(8) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x1E6, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x41E8, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x81E9, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x1EE, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x13) != 0) && (func_ov030_020aebb0(0x28) != 0) &&
                   (func_ov030_020aebb0(41) == 0))
        {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x1EF, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x45) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x1F0, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x13) && (func_ov030_020aebb0(0x35) == 0)) {
        func_ov030_020aeb34(0x35);
        func_ov030_020a9c54(arg0, 0x4D, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(0x3F) == 0) {
            func_ov030_020aeb34(0x3F);
            func_ov030_020aeb34(0x43);
            func_ov030_020a9c54(arg0, 0x1F2, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x3F) != 0) && (func_ov030_020aebb0(0x43) == 0)) {
            func_ov030_020aeb34(0x43);
            func_ov030_020a9c54(arg0, 0x1FA, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02095364(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x368E:
            func_ov030_020aeb34(0x36);
            func_ov030_020a669c(0x810);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x368F:
            func_ov030_020aeb34(0x37);
            func_ov030_020a669c(0x810);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3690:
            func_ov030_020aeb34(56);
            func_ov030_020a669c(0x810);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095418(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3692:
            func_ov030_020aeb34(0x39);
            func_ov030_020a669c(0x811);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3693:
            func_ov030_020aeb34(58);
            func_ov030_020a669c(0x811);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3694:
            func_ov030_020aeb34(0x3B);
            func_ov030_020a669c(0x811);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020954d0(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3696:
            func_ov030_020aeb34(0x3C);
            func_ov030_020a669c(0x812);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3697:
            func_ov030_020aeb34(61);
            func_ov030_020a669c(0x812);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3698:
            func_ov030_020aeb34(0x3E);
            func_ov030_020a669c(0x812);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095588(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x369A:
            func_ov030_020a669c(0x1F3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x369B:
            func_ov030_020a669c(0x1F4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x369C:
            func_ov030_020a669c(0x1F5);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209562c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x369E:
            func_ov030_020a669c(0x4A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x369F:
            func_ov030_020a669c(0x4A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36A0:
            func_ov030_020a669c(0x4A);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020956c8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A2:
            func_ov030_020a669c(0x4E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36A3:
            func_ov030_020a669c(0x4F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36A4:
            func_ov030_020a669c(0x4F);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02095764(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A6:
            func_ov030_020a669c(0x81A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36A7:
            func_ov030_020a669c(0x81B);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020957e0(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36A9:
            func_ov030_020a669c(0x820);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36AA:
            func_ov030_020a669c(0x821);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209585c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x1E2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x1E3);
            func_ov030_020a6738(arg0);
            return 1;
            break;
        case 0x1E4:
            data_02071cf0.unk_20.unk_264A = 0;
            data_02071cf0.unk_20.unk_264E = 0;
            data_02071cf0.unk_20.unk_2664 = 0;
            data_02071cf0.unk_20.unk_2662 = 0;
            data_02071cf0.unk_20.unk_2666 = 0;
            data_02071cf0.unk_20.unk_2668 = 0;
            data_02071cf0.unk_20.unk_266A = 0;
            data_02071cf0.unk_20.unk_266C = 0;
            arg0->unk_21CF8               = 1;
            break;
        case 0x7FE:
            func_ov030_020a669c(0x7FF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x7FF:
            func_ov030_020aeb34(43);
            break;
        case 0x801:
            func_ov030_020a669c(0x802);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x802:
            func_ov030_020aeb34(0x2C);
            if (func_ov030_020aaad8() == 1) {
                func_ov030_020a669c(0x803);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x803:
            func_ov030_020aeb34(3);
            func_ov030_020aeb34(0x46);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            data_02071cf0.unk_20.unk_264E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x804:
            func_ov030_020a669c(0x805);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x805:
            func_ov030_020aeb34(45);
            if (Inventory_HasRequiredQuantity(ITEM_FOOD_SHIO_RAMEN, 1, 0) != 0) {
                func_ov030_020a669c(0x806);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x806:
            func_ov030_020aeb34(4);
            func_ov030_020aeb34(0x48);
            func_ov030_020aac28(arg0, 0x4D);
            data_02071cf0.unk_20.unk_2664 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x807:
            func_ov030_020a669c(0x808);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x808:
            func_ov030_020aeb34(46);
            break;
        case 0x80A:
            if (func_ov030_020aebb0(47) == 0) {
                func_ov030_020aeb34(47);
                func_ov030_020a669c(0x80B);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x80C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x80B:
            func_ov030_020aeb34(47);
        case 0x80C:
            func_ov030_020a669c(0x80D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x80D:
            return func_ov030_020a9f54(arg0, func_ov030_02095364, 0x368D, 3, 0x368E, 0x368F, 0x3690);
        case 0x810:
            func_ov030_020a669c(0x80E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x80E:
            return func_ov030_020a9f54(arg0, func_ov030_02095418, 0x3691, 3, 0x3692, 0x3693, 0x3694);
        case 0x811:
            func_ov030_020a669c(0x80F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x80F:
            return func_ov030_020a9f54(arg0, func_ov030_020954d0, 0x3695, 3, 0x3696, 0x3697, 0x3698);
        case 0x812:
            if ((func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(0x3B) != 0) && (func_ov030_020aebb0(0x3C) != 0)) {
                func_ov030_020a669c(0x813);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x814);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x814:
            func_ov030_020aeb70(0x36);
            func_ov030_020aeb70(0x37);
            func_ov030_020aeb70(56);
            func_ov030_020aeb70(0x39);
            func_ov030_020aeb70(58);
            func_ov030_020aeb70(0x3B);
            func_ov030_020aeb70(0x3C);
            func_ov030_020aeb70(61);
            func_ov030_020aeb70(0x3E);
            break;
        case 0x813:
            func_ov030_020aeb34(5);
            data_02071cf0.unk_20.unk_2666 = 4;
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
            func_ov030_020a669c(0x815);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x816:
            func_ov030_020aeb34(0x30);
            if (func_02023e58(0) != 0) {
                func_ov030_020a669c(0x817);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x817:
            func_ov030_020aeb34(6);
            func_ov030_020aeb34(0x4A);
            data_02071cf0.unk_20.unk_2668 = 4;
            func_ov030_020aac28(arg0, 0x2C);
            arg0->unk_21CF8 = 1;
            break;
        case 0x6A9:
            if (func_02023e58(0) != 0) {
                func_ov030_020a669c(0x6A7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x69B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x818:
            func_ov030_020a669c(0x818);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x819:
            func_ov030_020aeb34(0x31);
            return func_ov030_020a9f54(arg0, func_ov030_02095764, 0x36A5, 2, 0x36A6, 0x36A7);
        case 0x81A:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x81C;
            data_02071cf0.unk_20.unk_24C2 = 0x81B;
            data_02071cf0.unk_20.unk_341C = 43;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x81D:
            func_ov030_020aac28(arg0, 0xEB);
            func_ov030_020aeb34(7);
            data_02071cf0.unk_20.unk_266A = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x81C:
            func_ov030_020aeb34(7);
            data_02071cf0.unk_20.unk_266A = 4;
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x1FC);
                func_ov030_020a6738(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x81E:
            func_ov030_020a669c(0x81F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x81F:
            func_ov030_020aeb34(0x32);
            return func_ov030_020a9f54(arg0, func_ov030_020957e0, 0x36A8, 2, 0x36A9, 0x36AA);
        case 0x820:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x822;
            func_ov030_020af364(0x18);
            return 1;
        case 0x822:
            func_ov030_020aeb34(8);
            data_02071cf0.unk_20.unk_266C = 2;
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x1E5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x1E5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x1E6:
            data_02071cf0.unk_20.unk_266C = 4;
            data_02071cf0.unk_20.unk_264A = 4;
            data_02071cf0.unk_20.unk_2662 = 4;
            arg0->unk_21AD0               = 1;
            arg0->unk_21ACC               = 1;
            arg0->unk_21AFC               = 0x10;
            arg0->unk_21B00               = 0x66;
            arg0->unk_21B08               = 0x1E000;
            arg0->unk_21B0C               = 0xF4000;
            break;
        case 0x1E8:
            func_ov030_020aeaa0(18, 2);
            data_02071cf0.unk_20.unk_26D2 = 0;
            break;
        case 0x1EA:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8524U;
            func_ov030_020af364(0x16);
            return 1;
        case 0x1EB:
            func_ov030_020aeb34(37);
            break;
        case 0x1EF:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8525U;
            func_ov030_020af364(23);
            return 1;
        case 0x1F0:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x81F1U;
            data_02071cf0.unk_20.unk_3124 = 0x1D;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F1:
            data_02071cf0.unk_20.unk_3124 = 0x1E;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x1FA:
            return func_ov030_020a9f54(arg0, func_ov030_02095588, 0x3699, 3, 0x369A, 0x369B, 0x369C);
        case 0x4A:
            func_ov030_020aeb34(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x4B:
            func_ov030_020aeb34(10);
            arg0->unk_21CF8 = 1;
            break;
        case 0x49:
            return func_ov030_020a9f54(arg0, func_ov030_0209562c, 0x369D, 3, 0x369E, 0x369F, 0x36A0);
        case 0x1F2:
            func_ov030_020aeb34(0x3F);
        case 0x4D:
            return func_ov030_020a9f54(arg0, func_ov030_020956c8, 0x36A1, 3, 0x36A2, 0x36A3, 0x36A4);

        case 0x1F3:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x1F6;
            data_02071cf0.unk_20.unk_24C2 = 0x1F7;
            data_02071cf0.unk_20.unk_341C = 0x23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F4:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x1F8;
            data_02071cf0.unk_20.unk_24C2 = 0x1F9;
            data_02071cf0.unk_20.unk_341C = 0x22;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x1F6:
            func_ov030_020aeb34(0x40);
            if ((func_ov030_020aebb0(0x40) != 0) && (func_ov030_020aebb0(0x41) != 0) && (func_ov030_020aebb0(0x42) == 0)) {
                func_ov030_020aeb34(0x42);
                func_ov030_020aac28(arg0, 0xEC);
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
            func_ov030_020aeb34(0x41);
            if ((func_ov030_020aebb0(0x40) != 0) && (func_ov030_020aebb0(0x41) != 0) && (func_ov030_020aebb0(0x42) == 0)) {
                func_ov030_020aeb34(0x42);
                func_ov030_020aac28(arg0, 0xEC);
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
            func_ov030_020aeb34(0x44);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_JOSHUA);
            break;
        case 0x525:
            func_ov030_020aeb34(0x45);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            break;
        case 0x1FC:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24BE = 2;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8200;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_264A = 4;
    data_02071cf0.unk_20.unk_2654 = 4;
    data_02071cf0.unk_20.unk_2658 = 4;
    data_02071cf0.unk_20.unk_265C = 4;
    data_02071cf0.unk_20.unk_2660 = 4;
    data_02071cf0.unk_20.unk_3124 = 0x43;
}

s32 func_ov030_02096888(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (func_ov030_020aebb0(56) == 0) {
            func_ov030_020aeb34(56);
            func_ov030_020a9c54(arg0, 0x448, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 1) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x201, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x202, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 4) && (func_ov030_020aebb0(0x22) == 0)) {
        func_ov030_020aeb34(0x22);
        func_ov030_020a9c54(arg0, 0x203, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 2) {
        if (func_ov030_020aebb0(0x23) == 0) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x204, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x207, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 8) != 0) && (func_ov030_020aebb0(37) == 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x208, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if (func_ov030_020aebb0(39) == 0) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x8209, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(3);
            func_ov030_020a9c54(arg0, 0x20C, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 10) != 0) && (func_ov030_020aebb0(41) == 0)) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x20D, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(0x2C) == 0) && (func_ov030_020aebb0(46) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(45) == 0) &&
            (func_ov030_020aebb0(46) == 0))
        {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(46) == 0)) {
            func_ov030_020aeb70(45);
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(47) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(47) != 0)) {
            func_ov030_020aeb70(47);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 7) {
        if (func_ov030_020aebb0(4) == 0) {
            func_ov030_020aeb34(4);
            func_ov030_020a9c54(arg0, 0x820E, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x36) != 0) && (func_ov030_020aebb0(5) == 0)) {
            func_ov030_020aeb34(5);
            func_ov030_020a9c54(arg0, 0x211, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 9) {
        if (func_ov030_020aebb0(42) == 0) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x212, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x37) != 0) && (func_ov030_020aebb0(43) == 0)) {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x213, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if ((func_ov030_020aebb0(0x2C) == 0) && (func_ov030_020aebb0(46) == 0)) {
            func_ov030_020aeb34(0x2C);
            func_ov030_020aeb34(45);
            func_ov030_020a9c54(arg0, 0x215, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(45) == 0) && (func_ov030_020aebb0(46) == 0)) {
            func_ov030_020aeb34(45);
            func_ov030_020a9c54(arg0, 0x21D, 0);
            return 1;
        } else if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(47) == 0)) {
            func_ov030_020aeb34(47);
            func_ov030_020a9c54(arg0, 0x222, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02096f2c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36AC:
            func_ov030_020a669c(0x205);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36AD:
            func_ov030_020aeb34(0x24);
            func_ov030_020a669c(0x206);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02096fb4(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36AF:
            func_ov030_020a669c(0x20A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36B0:
            func_ov030_020aeb34(0x28);
            func_ov030_020a669c(0x20B);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209703c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36B2:
            func_ov030_020a669c(0x20F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36B3:
            func_ov030_020a669c(0x210);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020970b8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36B5:
            func_ov030_020a669c(0x216U);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36B6:
            func_ov030_020a669c(0x2177CU >> 8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36B7:
            func_ov030_020a669c(0x21FU);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36B8:
            func_ov030_020a669c(0x218U);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209716c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x200:
            data_02071cf0.unk_20.unk_24BE = 0x224;
            func_ov030_020aec1c(10);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_LEVITATE);
            ProgressReward_GrantItem(arg0, ITEM_BOOK_BLACK_NOISE_SYMBOLS);
            break;
        case 0x201:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x52EU;
            func_ov030_020af364(0x19);
            return 1;
        case 0x204:
            return func_ov030_020a9f54(arg0, &func_ov030_02096f2c, 0x36AB, 2, 0x36AC, 0x36AD);
        case 0x205:
            func_ov030_020aeb34(2);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x52F;
            func_ov030_020af364(0x1C);
            return 1;
        case 0x207:
            func_ov030_020aac28(arg0, 0x91);
            break;
        case 0x208:
            func_ov030_020aeb34(2);
            arg0->unk_21CF8 = 1;
            break;
        case 0x209:
            return func_ov030_020a9f54(arg0, &func_ov030_02096fb4, 0x36AE, 2, 0x36AF, 0x36B0);
        case 0x20A:
        case 0x20B:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8530U;
            func_ov030_020af364(0x1A);
            return 1;
        case 0x20C:
            func_ov030_020aeb34(3);
            func_ov030_020aac28(arg0, 0x8A);
            arg0->unk_21CF8 = 1;
            break;
        case 0x20E:
            return func_ov030_020a9f54(arg0, &func_ov030_0209703c, 0x36B1, 2, 0x36B2, 0x36B3);
        case 0x20F:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8531U;
            func_ov030_020af364(0x1D);
            return 1;
        case 0x210:
            func_ov030_020aeb34(5);
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(0x28) != 0) &&
                (func_ov030_020aebb0(1) == 0))
            {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x223);
                func_ov030_020a6738(arg0);
                return 1;
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x211:
            func_ov030_020aeb34(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x212:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8532U;
            func_ov030_020af364(0x1B);
            return 1;
        case 0x213:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x214);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x214:
            data_02071cf0.unk_20.unk_3124 = 0x1F;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x215:
            func_ov030_020aeb34(0x2C);
        case 0x21D:
            return func_ov030_020a9f54(arg0, &func_ov030_020970b8, 0x36B4, 3, 0x36B5, 0x36B6, 0x36B8);
        case 0x216:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x219;
            data_02071cf0.unk_20.unk_24C2 = 0x21A;
            data_02071cf0.unk_20.unk_341C = 0x23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x217:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x21B;
            data_02071cf0.unk_20.unk_24C2 = 0x21C;
            data_02071cf0.unk_20.unk_341C = 0x22;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x219:
            func_ov030_020aeb34(0x30);
            if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(46) == 0)) {
                func_ov030_020a669c(0x21E);
                func_ov030_020a6738(arg0);
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
            func_ov030_020aeb34(0x31);
            if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(46) == 0)) {
                func_ov030_020a669c(0x21E);
                func_ov030_020a6738(arg0);
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
            func_ov030_020aeb34(46);
            func_ov030_020aeb34(47);
        case 0x222:
            return func_ov030_020a9f54(arg0, &func_ov030_020970b8, 0x36B4, 4, 0x36B5, 0x36B6, 0x36B7, 0x36B8);
        case 0x21F:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x220;
            data_02071cf0.unk_20.unk_24C2 = 0x221;
            data_02071cf0.unk_20.unk_341C = 0x24;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x220:
            if (func_ov030_020aebb0(0x32) == 0) {
                func_ov030_020aeb34(0x32);
                func_ov030_020aac28(arg0, 0xE7);
                func_ov030_020aac28(arg0, 0xED);
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
            func_ov030_020aeb34(0x33);
            func_ov030_020aac28(arg0, 45);
            break;
        case 0x52F:
            func_ov030_020aeb34(0x34);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x530:
            func_ov030_020aeb34(0x35);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            break;
        case 0x531:
            func_ov030_020aeb34(0x36);
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            break;
        case 0x532:
            func_ov030_020aeb34(0x37);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            break;
        case 0x223:
            func_ov030_020aeb34(1);
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
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8227;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_2648 = 4;
    data_02071cf0.unk_20.unk_2650 = 4;
    data_02071cf0.unk_20.unk_2652 = 4;
    data_02071cf0.unk_20.unk_2674 = 4;
    data_02071cf0.unk_20.unk_2676 = 4;
    data_02071cf0.unk_20.unk_2678 = 4;
    data_02071cf0.unk_20.unk_267A = 4;
    data_02071cf0.unk_20.unk_3124 = 0x44;
}

s32 func_ov030_02097acc(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 2) != 0) ||
             (func_ov030_020848e4(arg0, 3) != 0)) &&
            (func_ov030_020aebb0(0x20) == 0))
        {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x228, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 2) == 0) &&
            (func_ov030_020848e4(arg0, 3) == 0) && (func_ov030_020aebb0(0x20) != 0))
        {
            func_ov030_020aeb70(0x20);
        }
        if ((func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            data_02071cf0.unk_20.unk_3124 = 0x23;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(33) == 0)) {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 553, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 23) {
        if (func_ov030_020aebb0(46) == 0) {
            func_ov030_020aeb34(46);
            func_ov030_020a9c54(arg0, 554, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) != 0) && (func_ov030_020aebb0(0x31) == 0) &&
                   (func_ov030_020aebb0(47) == 0))
        {
            func_ov030_020aeb34(47);
            func_ov030_020a9c54(arg0, 555, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) == 0) && (func_ov030_020aebb0(0x31) != 0)) {
            func_ov030_020aeb70(0x31);
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x18) {
        if ((func_ov030_020aebb0(0x22) == 0) && (func_ov030_020aebb0(0x23) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 556, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            func_ov030_020aeb34(0x35);
            func_ov030_020a9c54(arg0, 562, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x19) {
        if ((func_ov030_020848e4(arg0, 0x1C) != 0) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(0x30) == 0)) {
            func_ov030_020aeb34(0x30);
            func_ov030_020a9c54(arg0, 0x231, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x1C) != 0) && (func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x32) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x1C) == 0) && (func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x32) != 0)) {
            func_ov030_020aeb70(0x32);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(0x23) == 0) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x8233, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x235, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02097f4c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x227:
            data_02071cf0.unk_20.unk_24BE = 0x238;
            func_ov030_020aec1c(10);
            break;
        case 0x229:
            data_02071cf0.unk_20.unk_24BE = 569;
            func_ov030_020aec1c(10);
            break;
        case 0x22D:
            func_ov030_020aeb34(6);
            arg0->unk_21CF8 = 1;
            break;
        case 0x22E:
            func_ov030_020aeb34(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x231:
            func_ov030_020aeb34(50);
            break;
        case 0x233:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8234;
            data_02071cf0.unk_20.unk_3124 = 33;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x234:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x841F;
            data_02071cf0.unk_20.unk_3124 = 0x20;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x41F:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8538;
            func_ov030_020af364(30);
            return 1;
        case 0x235:
            func_ov030_020aec38(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            data_02071cf0.unk_20.unk_24B4                 = 0;
            data_02071cf0.unk_20.unk_24B8                 = 1;
            data_02071cf0.unk_20.unk_24BC                 = 0x8236;
            data_02071cf0.unk_20.unk_3124                 = 0x22;
            data_02071cf0.unk_20.unk_3128                 = 0;
            arg0->unk_21630                               = 4;
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
            func_ov030_020aeb34(0x31);
            data_02071cf0.unk_20.unk_267A = 2;
            break;
        case 0x22B:
            data_02071cf0.unk_20.unk_267A = 4;
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(567);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020aac28(arg0, 2);
            break;
        case 0x826:
            if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(42) != 0)) {
                func_ov030_020a669c(0x828);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x827);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x827:
            if (func_ov030_020aebb0(3) == 0) {
                func_ov030_020aeb34(3);
                data_02071cf0.unk_20.unk_310C = &data_ov030_020d9c0c;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
            }
            break;
        case 0x828:
            func_ov030_020aeb34(2);
            data_02071cf0.unk_20.unk_267C = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x829:
            if ((func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(45) != 0)) {
                func_ov030_020a669c(0x82B);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x82A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x82A:
            if (func_ov030_020aebb0(5) == 0) {
                func_ov030_020aeb34(5);
                data_02071cf0.unk_20.unk_310C = &data_ov030_020d9b6c;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
            }
            break;
        case 0x82B:
            func_ov030_020aeb34(4);
            data_02071cf0.unk_20.unk_2680 = 2;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_JOSHUA);
            arg0->unk_21CF8 = 1;
            break;
        case 0x538:
            func_ov030_020aeb34(0x34);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, 0x45);
            break;
        case 0x237:
            func_ov030_020aeb34(1);
            func_ov030_020aac28(arg0, 2);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_24C0 = 4;
    data_02071cf0.unk_20.unk_24C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x823B;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_264C = 4;
    data_02071cf0.unk_20.unk_2656 = 4;
    data_02071cf0.unk_20.unk_266E = 4;
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
    data_02071cf0.unk_20.unk_3124                 = 0x45;
}

s32 func_ov030_02098604(RewardTableObject* arg0) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020848e4(arg0, 2) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
        func_ov030_020aeb34(0x20);
        func_ov030_020a9c54(arg0, 0x823C, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(40) == 0)) {
        func_ov030_020aeb34(40);
        func_ov030_020a9c54(arg0, 0x24F, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        if (func_ov030_020aebb0(33) == 0) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x823D, 0);
            return 1;
        } else if ((func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(34) == 0)) {
            func_ov030_020aeb34(34);
            func_ov030_020a9c54(arg0, 576, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x11) {
        if (func_ov030_020aebb0(35) == 0) {
            func_ov030_020aeb34(35);
            func_ov030_020a9c54(arg0, 0x8241, 0);
            return 1;
        } else if ((func_ov030_020aebb0(35) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x449, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 7) != 0) && (func_ov030_020aebb0(38) == 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 580, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 7) == 0) {
            func_ov030_020aeb70(0x24);
        }
        if ((func_ov030_020848e4(arg0, 0x13) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 581, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 582, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x13) != 0) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(39) == 0))
        {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x8248, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02098930(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36BA:
            func_ov030_020a669c(0x24A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36BB:
            func_ov030_020a669c(0x24B);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020989b0(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36BD:
            func_ov030_020a669c(0x24D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36BE:
            func_ov030_020a669c(0x24E);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_02098a30(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x23B:
            data_02071cf0.unk_20.unk_24BE = 594;
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
            data_02071cf0.unk_20.unk_24BE = 0x253;
            func_ov030_020aec1c(10);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x823E;
            func_ov030_020af364(0x1F);
            return 1;
        case 0x23E:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x823F;
            data_02071cf0.unk_20.unk_3124 = 0x24;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x23F:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8542;
            func_ov030_020aec1c(4);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_BEAT;
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
            func_ov030_020a669c(0x242);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x242:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x243);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x243:
            data_02071cf0.unk_20.unk_2656 = 2;
            break;
        case 0x246:
            data_02071cf0.unk_20.unk_2656 = 4;
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
            if (func_ov030_020aebb0(39) != 0) {
                func_ov030_020a669c(2093);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x82D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x249);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x249:
            return func_ov030_020a9f54(arg0, func_ov030_02098930, 0x36B9, 2, 0x36BA, 0x36BB);
        case 0x24A:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x24C;
            func_ov030_020af364(33);
            return 1;
        case 0x24C:
            return func_ov030_020a9f54(arg0, func_ov030_020989b0, 0x36BC, 2, 0x36BD, 0x36BE);
        case 0x24D:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x24C;
            func_ov030_020af364(33);
            return 1;
        case 0x24E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x82E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x82E:
            func_ov030_020aeb34(1);
            data_02071cf0.unk_20.unk_2648 = 4;
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, 0x6C);
            arg0->unk_21CF8 = 1;
        case 0x24F:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x250);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x250:
            data_02071cf0.unk_20.unk_3124 = 37;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x542: // Beat, Day 1 (Hachiko)
            func_ov030_020aeb34(41);
            if (Inventory_HasRequiredQuantity(ITEM_STICKER_AIR_TIME, 1, 0) == FALSE) {
                Inventory_AddItem(ITEM_STICKER_AIR_TIME, 0);
            }
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SUIT_CARDS);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_FUSION_BOOST_BEAT);
            break;
        case 0x251: // Beat, Day 1 (Secret Box at Statue of Hachiko)
            func_ov030_020aeb34(2);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8255;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_2648 = 4;
    data_02071cf0.unk_20.unk_264C = 4;
    data_02071cf0.unk_20.unk_2656 = 4;
    data_02071cf0.unk_20.unk_266E = 4;
    data_02071cf0.unk_20.unk_2674 = 4;
    data_02071cf0.unk_20.unk_2678 = 4;
    data_02071cf0.unk_20.unk_267A = 4;
    data_02071cf0.unk_20.unk_3124 = 0x46;
}

s32 func_ov030_0209910c(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (((func_ov030_020848e4(arg0, 1) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) && (func_ov030_020aebb0(0x20) == 0) &&
            (func_ov030_020aebb0(33) == 0))
        {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x257, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 1) == 0) && (func_ov030_020848e4(arg0, 3) == 0) &&
                   (func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(33) == 0))
        {
            func_ov030_020aeb70(0x20);
        }
        if ((func_ov030_020848e4(arg0, 0) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x259, 0);
            return 1;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020848e4(arg0, 4) != 0) && (func_ov030_020aebb0(33) == 0)) {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 0x258, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if (func_ov030_020aebb0(0x23) == 0) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x25A, 0);
            return 1;
        } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x25B, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x47) != 0) && (func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
            func_ov030_020aeb34(1);
            func_ov030_020a9c54(arg0, 0x4266, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x16) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 605, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if (func_ov030_020aebb0(38) == 0) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x25E, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x19) != 0) && (func_ov030_020aebb0(39) == 0) && (func_ov030_020aebb0(2) == 0)) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x25F, 0);
            return 1;
        } else if ((func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(2) == 0)) {
            func_ov030_020aeb34(2);
            func_ov030_020a9c54(arg0, 0x8261, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 23) {
        if (func_ov030_020aebb0(0x28) == 0) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x263, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(41) == 0)) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x265, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02099514(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x255:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8256U;
            data_02071cf0.unk_20.unk_3124 = 38;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x258:
            data_02071cf0.unk_20.unk_2650 = 4;
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
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x54CU;
            func_ov030_020af364(0x22);
            return 1;
        case 0x25B:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            data_02071cf0.unk_20.unk_3100 = 0x14;
            func_ov030_020a669c(0x25C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x25F:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x54DU;
            func_ov030_020af364(0x23);
            return 1;
        case 0x261:
            arg0->unk_21CF8 = 1;
            break;
        case 0x264:
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x54E;
            func_ov030_020af364(0x24);
            return 1;
        case 0x265:
            data_02071cf0.unk_20.unk_3124 = 39;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x54C:
            func_ov030_020aeb34(42);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, 0xC4);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x54D:
            func_ov030_020aeb34(43);
            func_ov030_020aac28(arg0, 0x99);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case 0x54E:
            func_ov030_020aeb34(0x2C);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0xC9);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_BLOCK_BEAT);
            break;
        case 0x266:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_26BE = 2;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x826A;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_2648 = 2;
    data_02071cf0.unk_20.unk_264A = 2;
    data_02071cf0.unk_20.unk_264C = 2;
    data_02071cf0.unk_20.unk_2656 = 4;
    data_02071cf0.unk_20.unk_266E = 0;
    data_02071cf0.unk_20.unk_2650 = 4;
    data_02071cf0.unk_20.unk_2652 = 4;
    data_02071cf0.unk_20.unk_2674 = 4;
    data_02071cf0.unk_20.unk_2678 = 4;
    data_02071cf0.unk_20.unk_267A = 4;
    data_02071cf0.unk_20.unk_2654 = 4;
    data_02071cf0.unk_20.unk_2658 = 4;
    data_02071cf0.unk_20.unk_265A = 4;
    data_02071cf0.unk_20.unk_265E = 4;
    data_02071cf0.unk_20.unk_3124 = 0x47;
    data_02071cf0.unk_20.unk_3100 = 0x14;
}

s32 func_ov030_02099990(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_02084860(0x157, 0xD6, 0x32) == 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x26B, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 0) != 0) || (func_ov030_020848e4(arg0, 1) != 0) ||
                    (func_ov030_020848e4(arg0, 2) != 0) || (func_ov030_020848e4(arg0, 3) != 0)) &&
                   (func_ov030_020aebb0(33) == 0) && (func_ov030_020aebb0(3) == 0))
        {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x26C, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0) == 0) && (func_ov030_020848e4(arg0, 1) == 0) &&
                   (func_ov030_020848e4(arg0, 2) == 0) && (func_ov030_020848e4(arg0, 3) == 0) &&
                   (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(3) == 0))
        {
            func_ov030_020aeb70(33);
        }
        if ((func_ov030_020848e4(arg0, 3) != 0) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x26D, 0);
            return 1;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(38) == 0) && (func_ov030_020aebb0(0x24) == 0)) {
        func_ov030_020aeb34(38);
        func_ov030_020a9c54(arg0, 0x272, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 4) && (func_ov030_020aebb0(0x24) == 0)) {
        func_ov030_020aeb34(0x24);
        func_ov030_020a9c54(arg0, 0x26F, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 5) && (func_ov030_020aebb0(37) == 0) && (func_ov030_020aebb0(0x24) == 0)) {
        func_ov030_020aeb34(37);
        func_ov030_020a9c54(arg0, 0x270, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 2) && (func_ov030_020aebb0(5) == 0)) {
        func_ov030_020aeb34(5);
        func_ov030_020a9c54(arg0, 0x273, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if (func_ov030_020aebb0(39) == 0) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x274, 0);
            return 1;
        } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x27B, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(0x42) == 0)) {
            func_ov030_020aeb34(0x42);
            func_ov030_020a9c54(arg0, 0x44A, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0)) &&
                   (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) == 0))
        {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x280, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x40) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x281, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(61) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) != 0) && (func_ov030_020aebb0(61) != 0) && (func_ov030_020aebb0(0x3F) == 0)) {
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3B) == 0) && (func_ov030_020aebb0(0x3F) != 0)) {
            func_ov030_020aeb70(0x3F);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(61) == 0) {
            func_ov030_020aeb34(0);
            func_ov030_020aeb34(0x3F);
            func_ov030_020a9c54(arg0, 642, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(0x3F) == 0)) {
            func_ov030_020aeb34(0x3F);
            func_ov030_020a9c54(arg0, 0x287, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_02099f8c(RewardTableObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 12:
            if (func_ov030_020aebb0(43) != 0) {
                func_ov030_020a9db8(arg0, 0x4B5);
                return 0;
            }
            func_ov030_020a9db8(arg0, 0x4B4);
            return 0;
        case 1:
            func_ov030_020a669c(0x4A6);
            return 1;
        case 0:
            if (func_ov030_020aebb0(46) == 0) {
                func_ov030_020aeb34(46);
                func_ov030_020a669c(0x4A3);
                return 1;
            }
            if (func_ov030_020aebb0(47) == 0) {
                func_ov030_020aeb34(47);
                func_ov030_020a669c(0x4A4);
                return 1;
            }
            func_ov030_020aeb70(47);
            func_ov030_020a669c(0x4A5);
            return 1;
        case 2:
            if (func_ov030_020aebb0(0x2C) == 0) {
                func_ov030_020aeb34(0x2C);
                func_ov030_020a669c(0x4A7);
                return 1;
            }
            if (func_ov030_020aebb0(45) == 0) {
                func_ov030_020aeb34(45);
                func_ov030_020a669c(0x4A8);
                return 1;
            }
            func_ov030_020aeb70(45);
            func_ov030_020a669c(0x4A9);
            return 1;
        case 3:
            func_ov030_020a669c(0x4AA);
            return 1;
        case 4:
            func_ov030_020a669c(0x4AB);
            return 1;
        case 5:
            func_ov030_020a669c(0x4AC);
            return 1;
        case 6:
            func_ov030_020a669c(0x4AD);
            return 1;
        case 7:
            func_ov030_020a669c(0x4AE);
            return 1;
        case 8:
            func_ov030_020a669c(0x4AF);
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

s32 func_ov030_0209a1e4(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C0:
            func_ov030_020aeb34(7);
            func_ov030_020a669c(0x276);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36C1:
            func_ov030_020aeb34(8);
            func_ov030_020a669c(0x27A);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a270(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C6:
            func_ov030_020aeb34(0x31);
            func_ov030_020a669c(0x835);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36C7:
            func_ov030_020aeb34(0x32);
            func_ov030_020a669c(0x835);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36C8:
            func_ov030_020aeb34(0x33);
            func_ov030_020a669c(0x835);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a328(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36CA:
            func_ov030_020aeb34(0x34);
            func_ov030_020a669c(0x836);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36CB:
            func_ov030_020aeb34(0x35);
            func_ov030_020a669c(0x836);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36CC:
            func_ov030_020aeb34(0x36);
            func_ov030_020a669c(0x836);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36CD:
            func_ov030_020aeb34(0x37);
            func_ov030_020a669c(0x836);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

s32 func_ov030_0209a3f8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36CF:
            func_ov030_020aeb34(56);
            func_ov030_020a669c(0x837);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D0:
            func_ov030_020aeb34(0x39);
            func_ov030_020a669c(0x837);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D1:
            func_ov030_020aeb34(58);
            func_ov030_020a669c(0x837);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D2:
            func_ov030_020aeb34(0x3B);
            func_ov030_020a669c(0x837);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D3:
            func_ov030_020aeb34(0x3C);
            func_ov030_020a669c(0x837);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a4ec(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36C3:
            func_ov030_020a669c(0x283);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36C4:
            func_ov030_020a669c(0x284);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209a564(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x26E:
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_2648 = 4;
            data_02071cf0.unk_20.unk_264C = 4;
            data_02071cf0.unk_20.unk_264A = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x271:
            func_ov030_020aeb34(4);
            arg0->unk_21CF8 = 1;
            break;
        case 0x275:
            func_ov030_020aeb34(6);
            return func_ov030_020a9f54(arg0, func_ov030_0209a1e4, 0x36BF, 2, 0x36C0, 0x36C1);
        case 0x4B4:
            func_ov030_020aeb34(43);
        case 0x4B5:
            func_ov030_020aeb70(46);
            func_ov030_020aeb70(47);
            func_ov030_020aeb70(0x2C);
            func_ov030_020aeb70(45);
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
            func_ov030_020a669c(0x27D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x4B0:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x278);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x4B1:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x277);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x277:
            func_ov030_020aeb34(9);
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x288);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x278:
        case 0x279:
            func_ov030_020aeb34(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x27A:
            func_ov030_020aeb34(9);
            if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10);
            }
            arg0->unk_21CF8 = 1;
            break;
        case 0x27B:
            func_ov030_020a669c(0x27C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x27D:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x27E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x27E:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x27F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x4B2:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x279);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x27F:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_AIR_CANCEL);
            break;
        case 0x280:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8556;
            func_ov030_020af364(37);
            return 1;
        case 0x281:
            data_02071cf0.unk_20.unk_3124 = 0x28;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x282:
            func_ov030_020aeb34(61);
        case 0x287:
            return func_ov030_020a9f54(arg0, func_ov030_0209a4ec, 0x36C2, 2, 0x36C3, 0x36C4);
        case 0x283:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = ITEM_CD_TRACK01;
            data_02071cf0.unk_20.unk_24C2 = 0x286;
            data_02071cf0.unk_20.unk_341C = 37;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x285:
            if (func_ov030_020aebb0(0x3E) == 0) {
                func_ov030_020aeb34(0x3E);
                func_ov030_020aac28(arg0, 0xEF);
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
            if (func_ov030_020aebb0(0x30) == 0) {
                func_ov030_020aeb34(0x30);
                func_ov030_020a669c(0x830);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x831);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x830:
        case 0x831:
            func_ov030_020a669c(0x832);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x832:
            return func_ov030_020a9f54(arg0, func_ov030_0209a270, 0x36C5, 3, 0x36C6, 0x36C7, 0x36C8);
        case 0x835:
            func_ov030_020a669c(0x832);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x833:
            return func_ov030_020a9f54(arg0, func_ov030_0209a328, 0x36C9, 4, 0x36CA, 0x36CB, 0x36CC, 0x36CD);
        case 0x836:
            func_ov030_020a669c(0x833);
            func_ov030_020a6738(arg0);
            return 1;

        case 0x834:
            return func_ov030_020a9f54(arg0, func_ov030_0209a3f8, 0x36CE, 5, 0x36CF, 0x36D0, 0x36D1, 0x36D2, 0x36D3);
        case 0x837:
            if ((func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(0x39) != 0)) {
                func_ov030_020a669c(0x838);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x839);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x839:
            func_ov030_020aeb70(0x31);
            func_ov030_020aeb70(0x32);
            func_ov030_020aeb70(0x33);
            func_ov030_020aeb70(0x34);
            func_ov030_020aeb70(0x35);
            func_ov030_020aeb70(0x36);
            func_ov030_020aeb70(0x37);
            func_ov030_020aeb70(56);
            func_ov030_020aeb70(0x39);
            func_ov030_020aeb70(58);
            func_ov030_020aeb70(0x3B);
            func_ov030_020aeb70(0x3C);
            break;
        case 0x838:
            func_ov030_020aeb34(2);
            func_ov030_020aeb34(0x41);
            data_02071cf0.unk_20.unk_266E = 4;
            ProgressReward_GrantPin(arg0, PIN_5000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_HOT_DOG);
            arg0->unk_21CF8 = 1;
            break;
        case 0x556:
            func_ov030_020aeb34(0x40);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0x1D);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x288:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(15);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x828C;
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x48;
    func_ov030_02084944(0xE, 0);
    func_ov030_02084944(15, 0);
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_0209b058(s32 arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 15) {
        if ((func_ov030_020848e4(arg0, 0x11) != 0) && (func_ov030_020aebb0(0x24) == 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x28E, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) == 0) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(3) == 0))
        {
            func_ov030_020aeb70(0x24);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (func_ov030_020aebb0(37) == 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x28F, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) == 0) && (func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb70(37);
        }
        if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x45) == 0)) {
            func_ov030_020aeb34(0x45);
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_BEAT;
        }
        if ((func_ov030_020848e4(arg0, 0x11) != 0) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(38) == 0) &&
            (func_ov030_020aebb0(4) == 0))
        {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x291, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) == 0) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(38) != 0) &&
                   (func_ov030_020aebb0(4) == 0))
        {
            func_ov030_020aeb70(38);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(4) == 0) &&
            (func_ov030_020aebb0(41) == 0))
        {
            func_ov030_020aeb34(4);
            func_ov030_020a9c54(arg0, 0x292, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x11) != 0) && (func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(39) == 0)) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x29C, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) != 0) && (func_ov030_020aebb0(5) != 0) &&
                   (func_ov030_020aebb0(0x28) == 0) && (func_ov030_020aebb0(41) == 0))
        {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x29D, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x10) == 0) && (func_ov030_020aebb0(5) != 0) &&
                   (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(41) == 0))
        {
            func_ov030_020aeb70(0x28);
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (func_ov030_020aebb0(8) != 0) && (func_ov030_020aebb0(9) == 0)) {
            func_ov030_020aeb34(9);
            func_ov030_020a9c54(arg0, 0x2B3, 0);
            return 1;
        } else if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x46) == 0)) {
            func_ov030_020aeb34(0x46);
            func_ov030_020aec1c(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_BEAT;
        }
        if ((func_ov030_020848e4(arg0, 0x10) != 0) && (func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x2B5, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0xE) && (func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(41) == 0)) {
        func_ov030_020aeb34(41);
        func_ov030_020a9c54(arg0, 0x29E, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 13) {
        if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x2B6, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 15) != 0) || (func_ov030_020848e4(arg0, 0x10) != 0)) &&
                   (func_ov030_020aebb0(0x23) == 0) && (func_ov030_020aebb0(33) == 0))
        {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x2B7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 15) == 0) && (func_ov030_020848e4(arg0, 0x10) == 0) &&
                   (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(33) == 0))
        {
            func_ov030_020aeb70(0x23);
        }
        if ((func_ov030_020aed9c(-2) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x2B8, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x43) != 0) && (func_ov030_020aebb0(0x2C) == 0)) {
            func_ov030_020aeb34(0x2C);
            func_ov030_020a9c54(arg0, 0x2BA, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(45) == 0)) {
            func_ov030_020aeb34(45);
            func_ov030_020a9c54(arg0, 0x2BB, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209b724(void) {
    if (data_02071cf0.unk_20.unk_2458 == 15) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(5) == 0)) {
            return 0x297;
        } else if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
            return 0x2A4;
        } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(8) == 0)) {
            return 0x2AD;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0xE) {
        if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
            return 0x2A4;
        } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(8) == 0)) {
            return 0x2AD;
        }
    }
    return 0;
}

s32 func_ov030_0209b810(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(46) == 0) &&
                (func_ov030_020aebb0(0x30) == 0))
            {
                func_ov030_020a669c(0x298);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x299);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D6:
            func_ov030_020a669c(0x29A);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209b8e8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(56) != 0) &&
                (func_ov030_020aebb0(0x3B) != 0))
            {
                func_ov030_020a669c(0x2A5);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x2A6);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D6:
            func_ov030_020a669c(0x2A7);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209b9c4(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D5:
            if ((func_ov030_020aebb0(0x3C) == 0) && (func_ov030_020aebb0(61) != 0) && (func_ov030_020aebb0(0x3E) == 0) &&
                (func_ov030_020aebb0(0x3F) == 0) && (func_ov030_020aebb0(0x40) != 0) && (func_ov030_020aebb0(0x41) != 0) &&
                (func_ov030_020aebb0(0x42) == 0))
            {
                func_ov030_020a669c(686);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x2AF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D6:
            func_ov030_020a669c(0x2B0);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209bacc(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC - 0x28C) {
        case 4:
            func_ov030_020aeb34(3);
            arg0->unk_21CF8 = 1;
            break;
        case 6:
            data_02071cf0.unk_20.unk_24BE = 0x2BF;
            func_ov030_020aec1c(10);
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9b94;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 11:
            return func_ov030_020a9f54(arg0, func_ov030_0209b810, 0x36D4, 2, 0x36D5, 0x36D6);
        case 13:
            func_ov030_020aeb70(46);
            func_ov030_020aeb70(47);
            func_ov030_020aeb70(0x30);
            func_ov030_020aeb70(0x31);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 12:
            func_ov030_020a669c(0x29B);
            func_ov030_020a6738(arg0);
            return 1;
        case 15:
            func_ov030_020aeb34(5);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 16:
            data_02071cf0.unk_20.unk_266A = 4;
            break;
        case 19:
            func_ov030_020aeb34(6);
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9bbc;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 24:
            return func_ov030_020a9f54(arg0, func_ov030_0209b8e8, 0x36D4, 2, 0x36D5, 0x36D6);
        case 26:
            func_ov030_020aeb70(0x32);
            func_ov030_020aeb70(0x33);
            func_ov030_020aeb70(0x34);
            func_ov030_020aeb70(0x35);
            func_ov030_020aeb70(0x36);
            func_ov030_020aeb70(0x37);
            func_ov030_020aeb70(56);
            func_ov030_020aeb70(0x39);
            func_ov030_020aeb70(58);
            func_ov030_020aeb70(0x3B);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 25:
            func_ov030_020a669c(0x2A8);
            func_ov030_020a6738(arg0);
            return 1;
        case 28:
            func_ov030_020aeb34(7);
            data_02071cf0.unk_20.unk_310C = &data_ov030_020d9d34;
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 33:
            return func_ov030_020a9f54(arg0, func_ov030_0209b9c4, 0x36D4, 2, 0x36D5, 0x36D6);
        case 35:
            func_ov030_020aeb70(0x3C);
            func_ov030_020aeb70(61);
            func_ov030_020aeb70(0x3E);
            func_ov030_020aeb70(0x3F);
            func_ov030_020aeb70(0x40);
            func_ov030_020aeb70(0x41);
            func_ov030_020aeb70(0x42);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            break;
        case 34:
            func_ov030_020a669c(0x2B1);
            func_ov030_020a6738(arg0);
            return 1;
        case 37:
            func_ov030_020aeb34(8);
            func_ov030_020c1960();
            func_ov030_020acfc4(arg0);
            arg0->unk_21CF8 = 1;
            break;
        case 39:
            func_ov030_020aec38(1);
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            arg0->unk_21CF8                               = 1;
            break;
        case 40:
            func_ov030_020aeb34(2);
            arg0->unk_21CF8 = 1;
            break;
        case 41:
            data_02071cf0.unk_20.unk_2668 = 4;
            break;
        case 42:
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 44:
            func_ov030_020a669c(0x2B9);
            func_ov030_020a6738(arg0);
            return 1;
        case 45:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8560;
            func_ov030_020af364(39);
            return 1;
        case 46:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8561;
            func_ov030_020af364(38);
            return 1;
        case 47:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x2BC);
            func_ov030_020a6738(arg0);
            return 1;
        case 48:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x2BD);
            func_ov030_020a6738(arg0);
            return 1;
        case 49:
            data_02071cf0.unk_20.unk_3124 = 41;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x560:
            func_ov030_020aeb34(0x43);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x561:
            func_ov030_020aeb34(0x44);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            func_ov030_020aac28(arg0, 0x19);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case 50:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_24BC = 4;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x82C2;
    func_ov030_020c26bc(4);
    data_02071cf0.unk_20.unk_265C = 0;
    data_02071cf0.unk_20.unk_2660 = 2;
    data_02071cf0.unk_20.unk_266C = 0;
    data_02071cf0.unk_20.unk_2670 = 0;
    data_02071cf0.unk_20.unk_2676 = 0;
    func_ov030_02084944(7);
    data_02071cf0.unk_20.unk_3124 = 0x49;
    data_02071cf0.unk_20.unk_3100 = 0x14;
}

s32 func_ov030_0209c208(s32 arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if (func_ov030_020aebb0(0x20) == 0) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x82C3, 0);
            return 1;
        } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x2C5, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 8) != 0) || (func_ov030_020848e4(arg0, 9) != 0) ||
                    (func_ov030_020848e4(arg0, 10) != 0)) &&
                   (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(0x22) == 0) && (func_ov030_020aebb0(3) == 0))
        {
            func_ov030_020aeb34(3);
            func_ov030_020a9c54(arg0, 0x2C6, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 8) == 0) && (func_ov030_020848e4(arg0, 9) == 0) &&
                   (func_ov030_020848e4(arg0, 10) == 0) && (func_ov030_020aebb0(33) != 0) &&
                   (func_ov030_020aebb0(0x22) == 0) && (func_ov030_020aebb0(3) != 0))
        {
            func_ov030_020aeb70(3);
        }
        if ((func_ov030_020aed9c(-2) != 0) && (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            func_ov030_020aeb34(0x22);
            func_ov030_020a9c54(arg0, 0x2C7, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 7) {
        if (func_ov030_020aebb0(0x23) == 0) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x2C8, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x64, 0x64, 0x96, 0x15E) != 0) && (func_ov030_020aebb0(6) == 0)) {
            func_ov030_020aeb34(6);
            func_ov030_020a9c54(arg0, 0x2CA, 0);
            return 1;
        } else if ((func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x2CB, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 12) != 0) && (func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x2CD, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 9) && (func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(41) == 0)) {
        func_ov030_020aeb34(41);
        func_ov030_020a9c54(arg0, 0x2D3, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_0209c57c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36D8:
            if (func_ov030_020aebb0(39) == 0) {
                func_ov030_020aeb34(39);
                func_ov030_020a669c(0x2CF);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x2D0);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36D9:
            if (func_ov030_020aebb0(0x28) == 0) {
                func_ov030_020aeb34(0x28);
                func_ov030_020a669c(0x2CF);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x2D0);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36DA:
            func_ov030_020a669c(0x2D1);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209c680(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x2C4:
            func_ov030_020aeb34(2);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x56A;
            func_ov030_020af364(41);
            return 1;
        case 0x2C5:
            data_02071cf0.unk_20.unk_2658 = 2;
            data_02071cf0.unk_20.unk_265A = 2;
            data_02071cf0.unk_20.unk_265C = 2;
            func_ov030_020aec1c(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2C7:
            data_02071cf0.unk_20.unk_2658 = 4;
            data_02071cf0.unk_20.unk_265A = 4;
            data_02071cf0.unk_20.unk_265C = 4;
            func_ov030_020aec38(9);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2CC:
            if ((func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
                if (func_ov030_020a75c0(arg0) != 0) {
                    return 1;
                }
                func_ov030_020a669c(0x2D6);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0X2CA:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x56BU;
            func_ov030_020af364(42);
            return 1;
        case 0x2CD:
            data_02071cf0.unk_20.unk_26C0 = 4;
            break;
        case 0x2CE:
        case 0x2CF:
        case 0x2D0:
            return func_ov030_020a9f54(arg0, func_ov030_0209c57c, 0x36D7, 3, 0x36D8, 0x36D9, 0x36DA);
        case 0x2D1:
            func_ov030_020aeb34(4);
            data_02071cf0.unk_20.unk_26C0 = 3;
            arg0->unk_21CF8               = 1;
            break;
        case 0x2D2:
            func_ov030_020aeb34(5);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x56C;
            func_ov030_020af364(0x28);
            return 1;
        case 0x2D3:
            data_02071cf0.unk_20.unk_3124 = 42;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x56A:
            func_ov030_020aeb34(42);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_SAFE_LANDING_BEAT);
            break;
        case 0x56B:
            func_ov030_020aeb34(43);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            break;
        case 0x56C:
            func_ov030_020aeb34(0x2C);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aac28(arg0, 0xC2);
            break;
        case 0x2D6:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_26BC = 4;
    data_02071cf0.unk_20.unk_26BE = 0;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 2;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        func_ov030_020aeb34(2);
        data_02071cf0.unk_20.unk_26D4 = 4;
    } else {
        func_ov030_020aec1c(4);
        data_02071cf0.unk_20.unk_26D4 = 0;
    }
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x82DA;
    data_02071cf0.unk_20.unk_3124 = 0x4A;
    func_ov030_020c26bc(4);
    data_02071cf0.unk_20.unk_2670 = 0;
    data_02071cf0.unk_20.unk_2676 = 0;
    func_ov030_020aec1c(5);
    data_02071cf0.unk_20.unk_3100 = 0x14;
    func_ov030_020aec1c(9);
}

s32 func_ov030_0209cb70(s32 arg0) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aed9c(-2) != 0) && (func_ov030_020aebb0(33) == 0) &&
        (func_ov030_020aebb0(0x20) == 0))
    {
        func_ov030_020aeb34(0x20);
        func_ov030_020a9c54(arg0, 0x2DB, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 11) && (func_ov030_020aed9c(-2) != 0) && (func_ov030_020aebb0(33) == 0) &&
               (func_ov030_020aebb0(0x20) == 0))
    {
        func_ov030_020aeb34(0x20);
        func_ov030_020a9c54(arg0, 0x2DB, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 12) {
        if ((func_ov030_020aed9c(-2) != 0) && (func_ov030_020aebb0(33) == 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x2DB, 0);
            return 1;
        } else if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x2E0, 0);
            return 1;
        } else if (((func_ov030_020848e4(arg0, 13) != 0) || (func_ov030_020848e4(arg0, 0xE) != 0)) &&
                   (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(39) == 0))
        {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x82E2, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x2E3, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 13) && (func_ov030_020aed9c(-2) != 0) && (func_ov030_020aebb0(33) == 0) &&
        (func_ov030_020aebb0(0x20) == 0))
    {
        func_ov030_020aeb34(0x20);
        func_ov030_020a9c54(arg0, 0x2DB, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x42E7, 0);
            return 1;
        } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
            func_ov030_020aeb34(1);
            func_ov030_020a9c54(arg0, 0x2E8, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0xE) && (func_ov030_020848e4(arg0, 18) != 0) && (func_ov030_020aebb0(33) == 0)) {
        func_ov030_020aeb34(33);
        func_ov030_020a9c54(arg0, 0x2DC, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(3);
            func_ov030_020a9c54(arg0, 0x42DD, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 0x45) != 0 && func_ov030_020aebb0(0x22) != 0 && func_ov030_020aebb0(0x23) == 0) {
            func_ov030_020aeb34(0x23);
            func_ov030_020a9c54(arg0, 0x42DF, 0);
            return 1;
        } else if (func_ov030_020848e4(arg0, 0x45) == 0 && func_ov030_020aebb0(0x22) != 0 && func_ov030_020aebb0(0x23) != 0) {
            func_ov030_020aeb70(0x23);
        }
    }
    return 0;
}

s32 func_ov030_0209cff0(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x2DD:
            func_ov030_020a669c(0x2DE);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x2DE:
            func_ov030_020aeb34(0x22);
            func_ov030_020aeb34(0x23);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            break;
        case 0x2E1:
        case 0x2E2:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8574;
            func_ov030_020af364(43);
            return 1;
        case 0x2E3:
            data_02071cf0.unk_20.unk_3124 = 43;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x2E7:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x574:
            func_ov030_020aeb34(0x28);
            if (func_ov030_020aebb0(0) == 0) {
                ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
                ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
                ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            } else {
                ProgressReward_GrantPin(arg0, PIN_500_YEN);
            }
            break;
        case 0x2E8:
            func_ov030_020aeb34(1);
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
    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_26BC = 4;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 2;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        data_02071cf0.unk_20.unk_26D4 = 4;
    } else {
        func_ov030_020aec1c(4);
        data_02071cf0.unk_20.unk_26D4 = 0;
    }
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x82EC;
    data_02071cf0.unk_20.unk_3124 = 0x4B;
    func_ov030_020c26bc(4);
    func_ov030_020aec1c(5);
    data_02071cf0.unk_20.unk_2676 = 0;
    data_02071cf0.unk_20.unk_26B0 = 0;
    data_02071cf0.unk_20.unk_26B2 = 2;
    data_02071cf0.unk_20.unk_26B4 = 0;
    data_02071cf0.unk_20.unk_3100 = 0x14;
}

s32 func_ov030_0209d318(s32 arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x10) {
        if ((func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x42ED, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x45) == 0) && (func_ov030_020aebb0(0x20) != 0)) {
            func_ov030_020aeb70(0x20);
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(47) == 0)) {
            func_ov030_020aeb34(47);
            func_ov030_020a9c54(arg0, 0x4312, 0);
            return 1;
        } else if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(1) == 0)) {
            func_ov030_020aeb34(1);
            func_ov030_020a9c54(arg0, 0x4311, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 18) {
        data_02071cf0.unk_20.unk_3100 = 20;
        if ((func_ov030_020848e4(arg0, 20) != 0) && (func_ov030_020aebb0(33) == 0)) {
            func_ov030_020aeb34(33);
            func_ov030_020a9c54(arg0, 0x2EE, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 38) {
        data_02071cf0.unk_20.unk_3100 = 0;
        if (func_ov030_020aebb0(0x32) == 0) {
            func_ov030_020aeb34(0x32);
            func_ov030_020a9c54(arg0, 0x2EF, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 39) {
        if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(4) == 0)) {
            func_ov030_020aeb34(4);
            func_ov030_020a9c54(arg0, 0x2F3, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) != 0) && (func_ov030_020aebb0(0x22) == 0) &&
                   (func_ov030_020aebb0(0x23) == 0))
        {
            func_ov030_020aeb34(0x22);
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x2F4, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) != 0) && (func_ov030_020aebb0(0x23) != 0) &&
                   (func_ov030_020aebb0(0x24) == 0))
        {
            func_ov030_020aeb34(0x24);
            func_ov030_020a9c54(arg0, 0x2F7, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x34) == 0) && (func_ov030_020aebb0(0x23) != 0) &&
                   (func_ov030_020aebb0(0x24) != 0))
        {
            func_ov030_020aeb70(0x24);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x28) {
        if ((func_ov030_020848a4(0x12C, 0xB4, 0x15E, 0x140) != 0) && (func_ov030_020aebb0(37) == 0)) {
            func_ov030_020aeb34(37);
            func_ov030_020a9c54(arg0, 0x2F8, 0);
            return 1;
        } else if ((func_ov030_020848a4(0x2A8, 0xB4, 0x2DA, 0x140) != 0) && (func_ov030_020aebb0(38) == 0)) {
            func_ov030_020aeb34(38);
            func_ov030_020a9c54(arg0, 0x2F9, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x35) != 0) && (func_ov030_020aebb0(5) == 0) && (func_ov030_020aebb0(39) == 0)) {
            func_ov030_020aeb34(39);
            func_ov030_020a9c54(arg0, 0x2FB, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x35) == 0) && (func_ov030_020aebb0(5) == 0) && (func_ov030_020aebb0(39) != 0)) {
            func_ov030_020aeb70(39);
        }
        if ((func_ov030_020848e4(arg0, 0x35) != 0) && (func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            func_ov030_020aeb34(0x28);
            func_ov030_020a9c54(arg0, 0x2FC, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 41) {
        if (func_ov030_020aebb0(41) == 0) {
            func_ov030_020aeb34(41);
            func_ov030_020a9c54(arg0, 0x82FD, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(42) == 0)) {
            func_ov030_020aeb34(42);
            func_ov030_020a9c54(arg0, 0x2FF, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 42) {
        if ((func_ov030_020848a4(0x2A8, 0xB4, 0x2DA, 0x140) != 0) && (func_ov030_020aebb0(43) == 0)) {
            func_ov030_020aeb34(43);
            func_ov030_020a9c54(arg0, 0x8301, 0);
            return 1;
        } else if ((func_ov030_020848e4(arg0, 0x37) != 0) && (func_ov030_020aebb0(0x2C) == 0)) {
            func_ov030_020aeb34(0x2C);
            func_ov030_020a9c54(arg0, 0x302, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 43) && (func_ov030_020848a4(0, 0x96, 0x12C, 0xC8) != 0) &&
        (func_ov030_020aebb0(45) == 0))
    {
        func_ov030_020aeb34(45);
        func_ov030_020a9c54(arg0, 0x8303, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_0209d974(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36DC:
            func_ov030_020a669c(0x2F5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36DD:
            func_ov030_020aeb34(0x23);
            func_ov030_020a669c(0x2F6);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209d9fc(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36DF:
            func_ov030_020a669c(0x304);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36E0:
            func_ov030_020a669c(0x305);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209da78(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E2:
            func_ov030_020aeb34(46);
            func_ov030_020a669c(775);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36E3:
            func_ov030_020a669c(776);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209dafc(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x2EF:
            data_02071cf0.unk_20.unk_24BE = 0x313;
            func_ov030_020aec1c(10);
            break;
        case 0x2F0:
            func_ov030_020aeb34(2);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x82F1;
            data_02071cf0.unk_20.unk_3124 = 0x2C;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x2F1:
            func_ov030_020aeb34(3);
            func_ov030_020aec1c(6);
            func_ov030_020a669c(0x2F2);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x2F2:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8420U;
            data_02071cf0.unk_20.unk_3124 = 45;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x70F:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x857E;
            func_ov030_020af364(0x2C);
            return 1;
        case 0x2F3:
            data_02071cf0.unk_20.unk_24BE = 0x314;
            func_ov030_020aec1c(10);
            data_02071cf0.unk_20.unk_26B0 = 2;
            arg0->unk_21CF8               = 1;
            break;
        case 0x2F4:
        case 0x2F7:
            return func_ov030_020a9f54(arg0, func_ov030_0209d974, 0x36DB, 2, 0x36DC, 0x36DD);
        case 0x2F5:
            data_02071cf0.unk_20.unk_26B0 = 0;
            arg0->unk_21AD0               = 1;
            arg0->unk_21ACC               = 1;
            arg0->unk_21AFC               = 0x28;
            arg0->unk_21B00               = 0x66;
            arg0->unk_21B08               = 0x2A000;
            arg0->unk_21B0C               = 0xF6000;
            break;
        case 0x2FA:
            ProgressReward_GrantItem(arg0, ITEM_THREAD_PI_FACES_CAP);
            func_ov030_020aeb34(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x2FC:
            data_02071cf0.unk_20.unk_26B2 = 4;
            break;
        case 0x2FE:
            func_ov030_020aeb34(6);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x857F;
            func_ov030_020af364(45);
            return 1;
        case 0x2FF:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8300;
            data_02071cf0.unk_20.unk_26B2 = 0;
            data_02071cf0.unk_20.unk_26B4 = 4;
            data_02071cf0.unk_20.unk_26B6 = 4;
            func_ov030_020aec38();
            data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
            data_02071cf0.unk_20.unk_3124                 = 46;
            data_02071cf0.unk_20.unk_3128                 = 0;
            arg0->unk_21630                               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x303:
            return func_ov030_020a9f54(arg0, func_ov030_0209d9fc, 0x36DE, 2, 0x36DF, 0x36E0);
        case 0x304:
        case 0x305:
            func_ov030_020a669c(0x306);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x306:
            return func_ov030_020a9f54(arg0, func_ov030_0209da78, 0x36E1, 2, 0x36E2, 0x36E3);
        case 0x307:
        case 0x308:
            func_ov030_020a669c(0x309);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x309:
            data_02071cf0.unk_20.unk_3124 = 47;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x30A:
            func_ov030_020a669c(0x30C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x30B:
            if (func_ov030_020aebb0(46) != 0) {
                func_ov030_020a669c(0x30E);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x30D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x30D:
        case 0x30E:
            func_ov030_020a669c(0x30F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x30F:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x8310;
            data_02071cf0.unk_20.unk_3124 = 0x32;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x310:
            data_02071cf0.unk_20.unk_3124 = 0x33;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x57E:
            func_ov030_020aeb34(0x30);
            ProgressReward_GrantPin(arg0, PIN_SCARLETITE);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_DEF_BOOST_BEAT);
            ProgressReward_GrantItem(arg0, ITEM_STICKER_ATK_BOOST_BEAT);
            break;
        case 0x57F:
            func_ov030_020aeb34(0x31);
            func_ov030_020aac28(arg0, 0xBB);
            break;
        case 0x312:
            arg0->unk_21630 = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x311:
            func_ov030_020aeb34(1);
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

    if (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) != 0) {
        func_ov030_020aeb34(0);
    }
    data_02071cf0.unk_20.unk_26B8 = 4;
    data_02071cf0.unk_20.unk_26BA = 4;
    data_02071cf0.unk_20.unk_26BC = 4;
    data_02071cf0.unk_20.unk_26C0 = 4;
    data_02071cf0.unk_20.unk_26C2 = 4;
    data_02071cf0.unk_20.unk_26C4 = 4;
    data_02071cf0.unk_20.unk_26C6 = 4;
    data_02071cf0.unk_20.unk_26C8 = 4;
    data_02071cf0.unk_20.unk_26CA = 4;
    data_02071cf0.unk_20.unk_26CC = 4;
    data_02071cf0.unk_20.unk_26CE = 4;
    data_02071cf0.unk_20.unk_26D0 = 4;
    data_02071cf0.unk_20.unk_26D2 = 4;
    if (Inventory_HasRequiredQuantity(ITEM_BOOK_BLACK_CAT_ATLAS_VOL_10, 1, 0) != 0) {
        func_ov030_020aeb34(0x1F);
        var_r1 = 2;
    } else {
        func_ov030_020aec1c(4);
        var_r1 = 0;
    }
    data_02071cf0.unk_20.unk_26D4 = var_r1;
    data_02071cf0.unk_20.unk_26D6 = 4;
    data_02071cf0.unk_20.unk_26D8 = 4;
    data_02071cf0.unk_20.unk_26DA = 4;
    data_02071cf0.unk_20.unk_26DC = 4;
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(4);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x317;
    data_02071cf0.unk_20.unk_2670 = 2;
    data_02071cf0.unk_20.unk_2676 = 2;
    data_02071cf0.unk_20.unk_267E = 0;
    data_02071cf0.unk_20.unk_267C = 0;
    data_02071cf0.unk_20.unk_2680 = 0;
    data_02071cf0.unk_20.unk_2682 = 0;
    data_02071cf0.unk_20.unk_2686 = 0;
    data_02071cf0.unk_20.unk_268A = 0;
    data_02071cf0.unk_20.unk_268E = 0;
    data_02071cf0.unk_20.unk_2692 = 0;
    data_02071cf0.unk_20.unk_2696 = 0;
    data_02071cf0.unk_20.unk_269A = 0;
    data_02071cf0.unk_20.unk_269E = 0;
    data_02071cf0.unk_20.unk_26A2 = 0;
    data_02071cf0.unk_20.unk_26A6 = 0;
    data_02071cf0.unk_20.unk_26AA = 0;
    func_ov030_020aec1c(6);
    if (func_ov030_020aebb0(0) == 0) {
        func_ov030_020aeb34(127);
        func_ov030_020c26bc(0);
        data_02071cf0.unk_20.unk_26BA = 0;
        func_ov030_020ae96c(43);
        data_02071cf0.unk_20.unk_24B4 = 0;
        data_02071cf0.unk_20.unk_24B8 = 1;
        data_02071cf0.unk_20.unk_24BC = 0x588;
    }
}

s32 func_ov030_0209e314(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(0x66) != 0) && (func_ov030_020aebb0(0x7B) == 0)) {
            func_ov030_020aeb34(0x7B);
            func_ov030_020a9c54(arg0, 0x370, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x7C) == 0)) {
            func_ov030_020aeb34(0x7C);
            func_ov030_020a9c54(arg0, 0x370, 0);
            return 1;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 8) {
        if (func_ov030_020aebb0(0x30) == 0) {
            func_ov030_020aeb34(0x30);
            func_ov030_020a9c54(arg0, 0x318, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (func_ov030_020aebb0(2) == 0)) {
            func_ov030_020aeb34(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) != 0) && (func_ov030_020aebb0(0x32) == 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
        if ((func_ov030_020848e4(arg0, 0x3E) == 0) && (func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb70(0x32);
        }
        if ((func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            func_ov030_020aeb34(0x35);
            func_ov030_020a9c54(arg0, 0x34D, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x10) && (func_ov030_020848e4(arg0, 0x45) != 0) && (func_ov030_020aebb0(4) != 0) &&
        (func_ov030_020aebb0(0x34) == 0) && (func_ov030_020aebb0(0x5D) == 0) && (func_ov030_020aebb0(0x5A) == 0))
    {
        func_ov030_020aeb34(0x5A);
        func_ov030_020a9c54(arg0, 0x3C2, 0);
        return 1;
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(0x1F) != 0) && (func_ov030_020aebb0(47) == 0)) {
            func_ov030_020aeb34(0x1E);
            func_ov030_020a9c54(arg0, 0x4371, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x46) == 0) && (func_ov030_020aebb0(0x1F) != 0) && (func_ov030_020aebb0(47) == 0) &&
            (func_ov030_020aebb0(0x1E) != 0))
        {
            func_ov030_020aeb70(0x1E);
        }
        if ((func_ov030_020848e4(arg0, 0x46) != 0) && (func_ov030_020aebb0(0x1F) != 0) && (func_ov030_020aebb0(47) != 0) &&
            (func_ov030_020aebb0(0x1D) == 0))
        {
            func_ov030_020aeb34(0x1D);
            func_ov030_020a9c54(arg0, 0x4372, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            func_ov030_020aeb34(0x33);
            func_ov030_020a9c54(arg0, 0x4326, 0);
            return 1;
        }
        if ((func_ov030_020848e4(arg0, 0x48) != 0) && (func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            func_ov030_020aeb34(0x20);
            func_ov030_020a9c54(arg0, 0x432E, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(47) == 0) && (func_ov030_020848e4(arg0, 23) != 0) &&
            (func_ov030_020aebb0(0x5E) == 0))
        {
            func_ov030_020aeb34(0x5E);
            func_ov030_020a9c54(arg0, 0x357, 0);
            return 1;
        }
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(47) == 0) && (func_ov030_020848e4(arg0, 23) == 0) &&
            (func_ov030_020aebb0(0x5E) != 0))
        {
            func_ov030_020aeb70(0x5E);
        }
        if (((func_ov030_020aebb0(4) == 0) || (func_ov030_020aebb0(47) != 0)) && (func_ov030_020848e4(arg0, 23) != 0) &&
            (func_ov030_020aebb0(0x5D) == 0))
        {
            func_ov030_020aeb34(0x5D);
            func_ov030_020a9c54(arg0, 0x356, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x18) {
        if (func_ov030_020aebb0(0x5F) == 0) {
            func_ov030_020aeb34(0x5F);
            func_ov030_020a9c54(arg0, 0x358, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(0x6E);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x19) {
        if (func_ov030_020aebb0(0x60) == 0) {
            func_ov030_020aeb34(0x60);
            func_ov030_020a9c54(arg0, 0x359, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(0x6F);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x1C) {
        if (func_ov030_020aebb0(0x61) == 0) {
            func_ov030_020aeb34(0x61);
            func_ov030_020a9c54(arg0, 0x35A, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(0x72);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x1F) {
        if (func_ov030_020aebb0(0x62) == 0) {
            func_ov030_020aeb34(0x62);
            func_ov030_020a9c54(arg0, 0x35B, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(0x75);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x22) {
        if (func_ov030_020aebb0(0x63) == 0) {
            func_ov030_020aeb34(0x63);
            func_ov030_020a9c54(arg0, 0x35C, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(0x78);
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x24) {
        if (func_ov030_020aebb0(0x64) == 0) {
            func_ov030_020aeb34(0x64);
            func_ov030_020a9c54(arg0, 0x35D, 0);
            return 1;
        }
        if (func_ov030_020c1a9c() != 0) {
            func_ov030_020aeb34(0x7A);
        }
        if ((func_ov030_020848e4(arg0, 0x31) != 0) && (func_ov030_020aebb0(0x16) != 0) && (func_ov030_020aebb0(0x65) == 0)) {
            func_ov030_020aeb34(0x7E);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 37;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x15D000;
            arg0->unk_21B0C = 0xD6000;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 37) {
        if (func_ov030_020aebb0(0x7D) != 0) {
            if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(2) == 0)) {
                func_ov030_020aeb34(2);
                func_ov030_020a9c54(arg0, 0x319, 0);
                return 1;
            }
            if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(0x32) == 0)) {
                func_ov030_020aeb34(0x32);
                func_ov030_020a9c54(arg0, 0x31C, 0);
                return 1;
            }
        }
        if ((func_ov030_020aebb0(0x7E) != 0) && (func_ov030_020aebb0(0x64) != 0) && (func_ov030_020aebb0(0x65) == 0)) {
            func_ov030_020aeb34(0x65);
            func_ov030_020a9c54(arg0, 0x35E, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x1A) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x70);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x1B) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x71);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x1D) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x73);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x1E) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x74);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x20) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x76);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 33) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x77);
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x23) && (func_ov030_020c1a9c() != 0)) {
        func_ov030_020aeb34(0x79);
    }
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if (((func_ov030_020aebb0(4) == 0) || (func_ov030_020aebb0(47) != 0)) && (func_ov030_020848e4(arg0, 0x14) != 0) &&
            (func_ov030_020aebb0(0x68) == 0))
        {
            func_ov030_020aeb34(0x68);
            func_ov030_020a9c54(arg0, 0x361, 0);
            return 1;
        }
        if (((func_ov030_020aebb0(4) == 0) || (func_ov030_020aebb0(47) != 0)) && (func_ov030_020848e4(arg0, 0x14) == 0) &&
            (func_ov030_020aebb0(0x68) != 0))
        {
            func_ov030_020aeb70(0x68);
        }
        if ((func_ov030_020848e4(arg0, 0x14) != 0) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(47) == 0) &&
            (func_ov030_020aebb0(0x67) == 0))
        {
            func_ov030_020aeb34(0x67);
            func_ov030_020a9c54(arg0, 0x360, 0);
            return 1;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 39) && (func_ov030_020848a4(0x17C, 0, 0x190, 0x12C) != 0) &&
        (func_ov030_020aebb0(0x69) == 0))
    {
        func_ov030_020aeb34(0x69);
        func_ov030_020a9c54(arg0, 0x362, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 41) && (func_ov030_020aebb0(0x6A) == 0)) {
        func_ov030_020aeb34(0x6A);
        func_ov030_020a9c54(arg0, 0x363, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 42) && (func_ov030_020aebb0(0x6B) == 0)) {
        func_ov030_020aeb34(0x6B);
        func_ov030_020a9c54(arg0, 0x364, 0);
        return 1;
    } else if (data_02071cf0.unk_20.unk_2458 == 43) {
        if (func_ov030_020aebb0(0) == 0) {
            func_ov030_020aeb34(0);
            func_ov030_020a9c54(arg0, 0x316, 0);
            return 1;
        }
        if ((func_ov030_020848a4(0, 0, 0x12C, 0xE6) != 0) && (func_ov030_020aebb0(0x6B) != 0) &&
            (func_ov030_020aebb0(0x6C) == 0))
        {
            func_ov030_020aeb34(0x6C);
            func_ov030_020a9c54(arg0, 0x365, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_0209efe4(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E5:
            func_ov030_020a669c(0x31A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36E6:
            func_ov030_020aeb34(0x32);
            func_ov030_020a669c(0x31B);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209f06c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x36E8:
            func_ov030_020a669c(0x367);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x36E9:
            func_ov030_020a669c(0x368);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_0209f0e8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x588:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_GAME_CLEARED);
            break;
        case 0x372:
            data_02071cf0.unk_20.unk_26D4 = 4;
            arg0->unk_21630               = 5;
            DebugOvlDisp_Pop();
            return 0;
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
            return func_ov030_020a9f54(arg0, func_ov030_020919c4, 0x36EA, 5, 0x36EB, 0x36EC, 0x36ED, 0x36EE, 0x36EF);
        case 0x317:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x370;
            data_02071cf0.unk_20.unk_3124 = 0x4C;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x319:
            return func_ov030_020a9f54(arg0, func_ov030_0209efe4, 0x36E4, 2, 0x36E5, 0x36E6);
        case 0x324:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x325);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x31A:
            func_ov030_020aeb70(0x7D);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 0;
            arg0->unk_21AFC = 8;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x168000;
            arg0->unk_21B0C = 0xFF000;
            break;
        // case 0x31B:
        //     func_ov030_020aeb70(0x7D);
        //     arg0->unk_21AD0 = 1;
        //     arg0->unk_21ACC = 0;
        //     arg0->unk_21AFC = 8;
        //     arg0->unk_21B00 = 0x66;
        //     arg0->unk_21B08 = 0x168000;
        //     arg0->unk_21B0C = 0xFF000;
        //     return 0;
        case 0x31D:
            func_ov030_020aeb34(0x31);
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x31E;
            data_02071cf0.unk_20.unk_24C2 = 0x31F;
            data_02071cf0.unk_20.unk_341C = 6;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x31E:
            func_ov030_020a669c(0x320);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x320:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x321;
            data_02071cf0.unk_20.unk_24C2 = 0x322;
            data_02071cf0.unk_20.unk_341C = 7;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x321:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x323);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x326:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x324);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x325:
            func_ov030_020aeb34(3);
            func_ov030_020aeb70(0x7D);
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
            func_ov030_020a669c(0x327);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x327:
            func_ov030_020aeb34(4);
            data_02071cf0.unk_20.unk_2676 = 2;
            break;
        case 0x328:
            func_ov030_020aeb34(5);
            arg0->unk_21CF8 = 1;
            break;
        case 0x42F:
            if (func_ov030_020aebb0(6) == 0) {
                func_ov030_020aeb34(6);
                func_ov030_020a669c(0x329);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x32B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x329:
        case 0x32B:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x32C;
            data_02071cf0.unk_20.unk_24C2 = 0x32A;
            data_02071cf0.unk_20.unk_341C = 0x16;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x32C:
            func_ov030_020aeb34(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x32F:
            func_ov030_020aeb34(33);
            arg0->unk_21CF8 = 1;
            break;
        case 0x430:
            if (func_ov030_020aebb0(39) == 0) {
                func_ov030_020aeb34(39);
                func_ov030_020a669c(0x330);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x332);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x330:
        case 0x332:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x333;
            data_02071cf0.unk_20.unk_24C2 = 0x331;
            data_02071cf0.unk_20.unk_341C = 23;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x333:
            ProgressReward_GrantPin(arg0, PIN_5_YEN);
            func_ov030_020aeb34(0x22);
            arg0->unk_21CF8 = 1;
            break;
        case 0x431:
            if (func_ov030_020aebb0(0x28) == 0) {
                func_ov030_020aeb34(0x28);
                func_ov030_020a669c(0x335);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x337);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x335:
        case 0x337:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x338;
            data_02071cf0.unk_20.unk_24C2 = 0x336;
            data_02071cf0.unk_20.unk_341C = 0x18;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x338:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            func_ov030_020aeb34(0x23);
            arg0->unk_21CF8 = 1;
            break;
        case 0x432:
            if (func_ov030_020aebb0(41) == 0) {
                func_ov030_020aeb34(41);
                func_ov030_020a669c(0x33A);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x33C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x33A:
        case 0x33C:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x33D;
            data_02071cf0.unk_20.unk_24C2 = 0x33B;
            data_02071cf0.unk_20.unk_341C = 0x19;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x33D:
            func_ov030_020aac28(arg0, 0xEE);
            func_ov030_020aeb34(0x24);
            arg0->unk_21CF8 = 1;
            break;
        case 0x433:
            if (func_ov030_020aebb0(42) == 0) {
                func_ov030_020aeb34(42);
                func_ov030_020a669c(0x33F);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x341);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x33F:
        case 0x341:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x342;
            data_02071cf0.unk_20.unk_24C2 = 0x340;
            data_02071cf0.unk_20.unk_341C = 0x1A;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x342:
            func_ov030_020aac28(arg0, 0xF0);
            func_ov030_020aeb34(37);
            arg0->unk_21CF8 = 1;
            break;
        case 0x434:
            if (func_ov030_020aebb0(43) == 0) {
                func_ov030_020aeb34(43);
                func_ov030_020a669c(0x344);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x346);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x344:
        case 0x346:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x347;
            data_02071cf0.unk_20.unk_24C2 = 0x345;
            data_02071cf0.unk_20.unk_341C = 0x1B;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x347:
            func_ov030_020aeb34(38);
            arg0->unk_21CF8 = 1;
            break;
        case 0x435:
            if (func_ov030_020aebb0(0x2C) == 0) {
                func_ov030_020aeb34(0x2C);
                func_ov030_020a669c(0x348);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x34A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x348:
        case 0x34A:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x34B;
            data_02071cf0.unk_20.unk_24C2 = 0x349;
            data_02071cf0.unk_20.unk_341C = 0x1C;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x34B:
            func_ov030_020aac28(arg0, 0xF3);
            func_ov030_020aeb34(0x34);
            arg0->unk_21CF8 = 1;
            break;
        case 0x34D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x34E:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x34F;
            data_02071cf0.unk_20.unk_24C2 = 0x350;
            data_02071cf0.unk_20.unk_341C = 0x1D;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x34F:
            func_ov030_020aac28(arg0, 0xF1);
        case 0x350:
            func_ov030_020aeb34(45);
            arg0->unk_21CF8 = 1;
            break;
        case 0x436:
            if (func_ov030_020aebb0(46) == 0) {
                func_ov030_020aeb34(46);
                func_ov030_020a669c(0x351);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x353);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x351:
        case 0x353:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x354;
            data_02071cf0.unk_20.unk_24C2 = 0x352;
            data_02071cf0.unk_20.unk_341C = 0x1E;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x354:
            func_ov030_020aac28(arg0, 0xF4);
            ProgressReward_GrantItem(arg0, ITEM_THREAD_WIZARDS_GLASSES);
            func_ov030_020aeb34(47);
            data_02071cf0.unk_20.unk_2676 = 4;
            data_02071cf0.unk_20.unk_2670 = 2;
            arg0->unk_21AD0               = 1;
            arg0->unk_21ACC               = 1;
            arg0->unk_21AFC               = 1;
            arg0->unk_21B00               = 0x66;
            arg0->unk_21B08               = 0x157000;
            arg0->unk_21B0C               = 0xD6000;
            break;
        case 0x373:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x374;
            data_02071cf0.unk_20.unk_24C2 = 0x375;
            data_02071cf0.unk_20.unk_341C = 2;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x374:
        case 0x375:
            func_ov030_020aeb34(56);
            arg0->unk_21CF8 = 1;
            break;
        case 0x376:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x377;
            data_02071cf0.unk_20.unk_24C2 = 0x378;
            data_02071cf0.unk_20.unk_341C = 3;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x377:
        case 0x378:
            func_ov030_020aeb34(0x39);
            arg0->unk_21CF8 = 1;
            break;
        case 0x379:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x37A;
            data_02071cf0.unk_20.unk_24C2 = 0x37B;
            data_02071cf0.unk_20.unk_341C = 38;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x37A:
        case 0x37B:
            func_ov030_020aeb34(58);
            arg0->unk_21CF8 = 1;
            break;
        case 0x37C:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x37D;
            data_02071cf0.unk_20.unk_24C2 = 0x37E;
            data_02071cf0.unk_20.unk_341C = 39;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x37D:
        case 0x37E:
            func_ov030_020aeb34(0x3B);
            arg0->unk_21CF8 = 1;
            break;
        case 0x37F:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x380;
            data_02071cf0.unk_20.unk_24C2 = 0x381;
            data_02071cf0.unk_20.unk_341C = 42;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x380:
        case 0x381:
            func_ov030_020aeb34(0x3C);
            arg0->unk_21CF8 = 1;
            break;
        case 0x382:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x383;
            data_02071cf0.unk_20.unk_24C2 = 0x384;
            data_02071cf0.unk_20.unk_341C = 0x28;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x383:
        case 0x384:
            func_ov030_020aeb34(61);
            arg0->unk_21CF8 = 1;
            break;
        case 0x385:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x386;
            data_02071cf0.unk_20.unk_24C2 = 0x387;
            data_02071cf0.unk_20.unk_341C = 41;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x386:
        case 0x387:
            func_ov030_020aeb34(0x3E);
            arg0->unk_21CF8 = 1;
            break;
        case 0x388:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x389;
            data_02071cf0.unk_20.unk_24C2 = 0x38A;
            data_02071cf0.unk_20.unk_341C = 4;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x389:
        case 0x38A:
            func_ov030_020aeb34(0x3F);
            arg0->unk_21CF8 = 1;
            break;
        case 0x38B:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x38C;
            data_02071cf0.unk_20.unk_24C2 = 0x38D;
            data_02071cf0.unk_20.unk_341C = 5;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x38C:
        case 0x38D:
            func_ov030_020aeb34(0x40);
            arg0->unk_21CF8 = 1;
            break;
        case 0x38E:
            func_ov030_020aeb34(0x41);
            arg0->unk_21CF8 = 1;
            break;
        case 0x421:
            if (func_ov030_020aebb0(0x4E) == 0) {
                func_ov030_020aeb34(0x4E);
                func_ov030_020a669c(0x392);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x393);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x392:
        case 0x393:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x394;
            data_02071cf0.unk_20.unk_24C2 = 0x395;
            data_02071cf0.unk_20.unk_341C = 8;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x394:
            func_ov030_020aeb34(0x42);
            func_ov030_020aac28(arg0, 0x86);
            arg0->unk_21CF8 = 1;
        case 0x395:
            arg0->unk_21CF8 = 1;
            break;
        case 0x422:
            if (func_ov030_020aebb0(0x4F) == 0) {
                func_ov030_020aeb34(0x4F);
                func_ov030_020a669c(0x396);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x397);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x396:
        case 0x397:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x398;
            data_02071cf0.unk_20.unk_24C2 = 0x399;
            data_02071cf0.unk_20.unk_341C = 11;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x398:
            func_ov030_020aeb34(0x43);
            func_ov030_020aac28(arg0, 0x92);
            arg0->unk_21CF8 = 1;
        case 0x399:
            arg0->unk_21CF8 = 1;
            break;
        case 0x423:
            if (func_ov030_020aebb0(0x50) == 0) {
                func_ov030_020aeb34(0x50);
                func_ov030_020a669c(0x39A);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x39B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x39A:
        case 0x39B:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x39C;
            data_02071cf0.unk_20.unk_24C2 = 0x39D;
            data_02071cf0.unk_20.unk_341C = 12;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x39C:
            func_ov030_020aeb34(0x44);
            func_ov030_020aac28(arg0, 0x107);
            arg0->unk_21CF8 = 1;
        case 0x39D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x424:
            if (func_ov030_020aebb0(0x51) == 0) {
                func_ov030_020aeb34(0x51);
                func_ov030_020a669c(0x39E);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x39F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x39E:
        case 0x39F:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3A0;
            data_02071cf0.unk_20.unk_24C2 = 0x3A1;
            data_02071cf0.unk_20.unk_341C = 9;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A0:
            func_ov030_020aeb34(0x45);
            func_ov030_020aac28(arg0, 0xB0);
            arg0->unk_21CF8 = 1;
        case 0x3A1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x425:
            if (func_ov030_020aebb0(0x52) == 0) {
                func_ov030_020aeb34(0x52);
                func_ov030_020a669c(0x3A2);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3A3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3A2:
        case 0x3A3:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3A4;
            data_02071cf0.unk_20.unk_24C2 = 0x3A5;
            data_02071cf0.unk_20.unk_341C = 13;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A4:
            func_ov030_020aeb34(0x46);
            func_ov030_020aac28(arg0, 0x108);
            arg0->unk_21CF8 = 1;
        case 0x3A5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x426:
            if (func_ov030_020aebb0(0x53) == 0) {
                func_ov030_020aeb34(0x53);
                func_ov030_020a669c(0x3A6);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3A7);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3A6:
        case 0x3A7:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3A8;
            data_02071cf0.unk_20.unk_24C2 = 0x3A9;
            data_02071cf0.unk_20.unk_341C = 0x11;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3A8:
            func_ov030_020aeb34(8);
            func_ov030_020aac28(arg0, 0x97);
            arg0->unk_21CF8 = 1;
        case 0x3A9:
            arg0->unk_21CF8 = 1;
            break;
        case 0x427:
            if (func_ov030_020aebb0(0x54) == 0) {
                func_ov030_020aeb34(0x54);
                func_ov030_020a669c(0x3AA);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3AB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3AA:
        case 0x3AB:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3AC;
            data_02071cf0.unk_20.unk_24C2 = 0x3AD;
            data_02071cf0.unk_20.unk_341C = 0xE;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3AC:
            func_ov030_020aeb34(0x47);
            func_ov030_020aac28(arg0, 0x106);
            arg0->unk_21CF8 = 1;
        case 0x3AD:
            arg0->unk_21CF8 = 1;
            break;
        case 0x428:
            if (func_ov030_020aebb0(0x55) == 0) {
                func_ov030_020aeb34(0x55);
                func_ov030_020a669c(0x3AE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3AF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3AE:
        case 0x3AF:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3B0;
            data_02071cf0.unk_20.unk_24C2 = 0x3B1;
            data_02071cf0.unk_20.unk_341C = 18;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B0:
            func_ov030_020aeb34(0x48);
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            arg0->unk_21CF8 = 1;
        case 0x3B1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x429:
            if (func_ov030_020aebb0(0x56) == 0) {
                func_ov030_020aeb34(0x56);
                func_ov030_020a669c(0x3B2);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3B3);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3B2:
        case 0x3B3:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3B4;
            data_02071cf0.unk_20.unk_24C2 = 0x3B5;
            data_02071cf0.unk_20.unk_341C = 15;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B4:
            func_ov030_020aeb34(0x49);
            ProgressReward_GrantPin(arg0, PIN_10_YEN);
            arg0->unk_21CF8 = 1;
        case 0x3B5:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42A:
            if (func_ov030_020aebb0(0x57) == 0) {
                func_ov030_020aeb34(0x57);
                func_ov030_020a669c(0x3B6);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3B7);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3B6:
        case 0x3B7:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3B8;
            data_02071cf0.unk_20.unk_24C2 = 0x3B9;
            data_02071cf0.unk_20.unk_341C = 0x10;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3B8:
            func_ov030_020aeb34(0x4A);
            func_ov030_020aac28(arg0, 0x67);
            arg0->unk_21CF8 = 1;
        case 0x3B9:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42B:
            if (func_ov030_020aebb0(0x58) == 0) {
                func_ov030_020aeb34(0x58);
                func_ov030_020a669c(0x3BA);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3BB);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3BA:
        case 0x3BB:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3BC;
            data_02071cf0.unk_20.unk_24C2 = 0x3BD;
            data_02071cf0.unk_20.unk_341C = 10;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3BC:
            func_ov030_020aeb34(0x4B);
            func_ov030_020aac28(arg0, 0x89);
            arg0->unk_21CF8 = 1;
        case 0x3BD:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42C:
            if (func_ov030_020aebb0(0x59) == 0) {
                func_ov030_020aeb34(0x59);
                func_ov030_020a669c(0x3BE);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3BF);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3BE:
        case 0x3BF:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3C0;
            data_02071cf0.unk_20.unk_24C2 = 0x3C1;
            data_02071cf0.unk_20.unk_341C = 0x14;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C0:
            func_ov030_020aeb34(0x4C);
            func_ov030_020aac28(arg0, 0xF2);
            arg0->unk_21CF8 = 1;
        case 0x3C1:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42D:
            if (func_ov030_020aebb0(0x5B) == 0) {
                func_ov030_020aeb34(0x5B);
                func_ov030_020a669c(0x3C3);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3C4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3C3:
        case 0x3C4:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3C5;
            data_02071cf0.unk_20.unk_24C2 = 0x3C6;
            data_02071cf0.unk_20.unk_341C = 0x13;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C5:
            func_ov030_020aeb34(9);
            func_ov030_020aac28(arg0, 0xE8);
            arg0->unk_21CF8 = 1;
        case 0x3C6:
            arg0->unk_21CF8 = 1;
            break;
        case 0x42E:
            if (func_ov030_020aebb0(0x5C) == 0) {
                func_ov030_020aeb34(0x5C);
                func_ov030_020a669c(0x3C7);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x3C8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3C7:
        case 0x3C8:
            arg0->unk_21630               = 6;
            data_02071cf0.unk_20.unk_24C0 = 0x3C9;
            data_02071cf0.unk_20.unk_24C2 = 0x3CA;
            data_02071cf0.unk_20.unk_341C = 0x15;
            SndMgr_StartPlayingSE(0);
            DebugOvlDisp_Pop();
            return 0;
        case 0x3C9:
            func_ov030_020aeb34(0x4D);
            func_ov030_020aac28(arg0, 0xEA);
            arg0->unk_21CF8 = 1;
        case 0x3CA:
            arg0->unk_21CF8 = 1;
            break;
        case 0x360:
            data_02071cf0.unk_20.unk_2670 = 4;
            break;
        case 0x31C:
        case 0x31F:
        case 0x322:
            return func_ov030_020a9f54(arg0, func_ov030_0209efe4, 0x36E4, 2, 0x36E5, 0x36E6);
        case 0x367:
            if (func_ov030_020aebb0(0x6D) != 0) {
                data_02071cf0.unk_20.unk_24B4 = 0;
                data_02071cf0.unk_20.unk_24B8 = 1;
                data_02071cf0.unk_20.unk_24BC = 0x36B;
                func_ov030_020af364(0x31);
                return 1;
            }
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x369;
            func_ov030_020af364(0x31);
            return 1;
        case 0x369:
            func_ov030_020aeb34(0x6D);
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
            data_02071cf0.unk_20.unk_26D2 = 0;
            data_02071cf0.unk_20.unk_2676 = 4;
            break;
        case 0x35E:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x35FU;
            func_ov030_020af364(0x30);
            return 1;
        case 0x35F:
            func_ov030_020aeb34(0x66);
            ProgressReward_GrantItem(arg0, ITEM_THREAD_MY_PHONES);
            data_02071cf0.unk_20.unk_26AA = 0;
            func_ov030_020aeb70(0x7E);
            arg0->unk_21AD0 = 1;
            arg0->unk_21ACC = 1;
            arg0->unk_21AFC = 1;
            arg0->unk_21B00 = 0x66;
            arg0->unk_21B08 = 0x157000;
            arg0->unk_21B0C = 0xD6000;
            break;
        case 0x83A:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x6E) != 0)) {
                func_ov030_020a669c(0x83C);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x6E) == 0)) {
                func_ov030_020a669c(0x861);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x83B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x83C:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_HAMBURGER);
        case 0x861:
            func_ov030_020aeb34(10);
            data_02071cf0.unk_20.unk_267E = 4;
            data_02071cf0.unk_20.unk_267C = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x83D:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x6F) != 0)) {
                func_ov030_020a669c(0x83F);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x6F) == 0)) {
                func_ov030_020a669c(0x862);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x83E);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x83F:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_SPECIAL_DOG);
        case 0x862:
            func_ov030_020aeb34(11);
            data_02071cf0.unk_20.unk_2680 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x840:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x70) != 0)) {
                func_ov030_020a669c(0x842);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x70) == 0)) {
                func_ov030_020a669c(0x863);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x841);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x842:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_DONUTS);
        case 0x863:
            func_ov030_020aeb34(12);
            data_02071cf0.unk_20.unk_2682 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x843:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x71) != 0)) {
                func_ov030_020a669c(0x845);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x71) == 0)) {
                func_ov030_020a669c(0x864);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x844);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x845:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_SHADOW_STEAK_RAMEN);
        case 0x864:
            func_ov030_020aeb34(13);
            data_02071cf0.unk_20.unk_2686 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x846:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x72) != 0)) {
                func_ov030_020a669c(0x848);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x72) == 0)) {
                func_ov030_020a669c(0x865);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x847);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x848:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_ATK_CAPSULES);
        case 0x865:
            func_ov030_020aeb34(0xE);
            data_02071cf0.unk_20.unk_268A = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x849:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x73) != 0)) {
                func_ov030_020a669c(0x84B);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x73) == 0)) {
                func_ov030_020a669c(0x866);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x84A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x84B:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_DEF_MEDICINE);
        case 0x866:
            func_ov030_020aeb34(15);
            data_02071cf0.unk_20.unk_268E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x84C:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x74) != 0)) {
                func_ov030_020a669c(0x84E);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x74) == 0)) {
                func_ov030_020a669c(0x867);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x84D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x84E:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_THE_TATSUMI_BURGER);
        case 0x867:
            func_ov030_020aeb34(0x10);
            data_02071cf0.unk_20.unk_2692 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x84F:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x75) != 0)) {
                func_ov030_020a669c(0x851);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x75) == 0)) {
                func_ov030_020a669c(0x868);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x850);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x851:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_DEFINITIVO_CHILI_DOG);
        case 0x868:
            func_ov030_020aeb34(0x11);
            data_02071cf0.unk_20.unk_2696 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x852:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x76) != 0)) {
                func_ov030_020a669c(0x854);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x76) == 0)) {
                func_ov030_020a669c(0x869);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x853);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x854:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_MYSTIC_RAMEN);
        case 0x869:
            func_ov030_020aeb34(18);
            data_02071cf0.unk_20.unk_269A = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x855:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x77) != 0)) {
                func_ov030_020a669c(0x857);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x77) == 0)) {
                func_ov030_020a669c(0x86A);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x856);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x857:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_ABSOLUTE_SHADOW_RAMEN);
        case 0x86A:
            func_ov030_020aeb34(0x13);
            data_02071cf0.unk_20.unk_269E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x858:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x78) != 0)) {
                func_ov030_020a669c(0x85A);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x78) == 0)) {
                func_ov030_020a669c(0x86B);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x859);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x85A:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_VIPER_DRINK);
        case 0x86B:
            func_ov030_020aeb34(0x14);
            data_02071cf0.unk_20.unk_26A2 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x85B:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x79) != 0)) {
                func_ov030_020a669c(0x85D);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x79) == 0)) {
                func_ov030_020a669c(0x86C);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x85C);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x85D:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_MAKO_SYNERGY);
        case 0x86C:
            func_ov030_020aeb34(0x15);
            data_02071cf0.unk_20.unk_26A6 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x85E:
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x7A) != 0)) {
                func_ov030_020a669c(0x860);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if ((func_ov030_020c1a9c() == 0) && (func_ov030_020aebb0(0x7A) == 0)) {
                func_ov030_020a669c(0x86D);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020a669c(0x85F);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x860:
            ProgressReward_GrantPin(arg0, PIN_10000_YEN);
            ProgressReward_GrantItem(arg0, ITEM_FOOD_CURIOUS_MUSHROOMS);
        case 0x86D:
            func_ov030_020aeb34(0x16);
            data_02071cf0.unk_20.unk_26AA = 2;
            arg0->unk_21CF8               = 1;
            break;
        case 0x36C:
            func_ov030_020aeb34(1);
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
    data_02071cf0.unk_20.unk_3124 = 0x37;
}

s32 func_ov030_020a1ed0(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 9) {
        if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(4) == 0)) {
            func_ov030_020aeb34(4);
            func_ov030_020a9c54(arg0, 0x592, 0);
            return 1;
        }
        if (func_ov030_020aebb0(3) == 0) {
            func_ov030_020aeb34(3);
            arg0->unk_21AF4 = 0x96;
            arg0->unk_21AF0 = 1;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 18) && (func_ov030_02084860(0x181, 0xC2, 0x14) != 0)) {
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

s32 func_ov030_020a1fdc(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x35EA:
            func_ov030_020a669c(0x595);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35EB:
            func_ov030_020a669c(0x596);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            func_ov030_020a669c(0x598);
            func_ov030_020a6738(arg0);
            return 1;
    }
}

s32 func_ov030_020a206c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
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
    data_02071cf0.unk_20.unk_3124 = 56;
}

s32 func_ov030_020a2120(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if (func_ov030_020aebb0(0) != 0) {
            func_ov030_020aeb70(0);
            func_ov030_020a9c54(arg0, 0x599, 0);
            return 1;
        } else if (func_ov030_020aebb0(1) != 0) {
            func_ov030_020a9c54(arg0, 0x59B, 0);
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020a21a4(RewardTableObject* arg0) {
    if (data_02071cf0.unk_20.unk_24BC == 0x59A) {
        func_ov030_020b5d14(EasyTask_GetTaskById(&arg0->taskPool, arg0->unk_21788[data_02071cf0.unk_20.unk_24A4]), 1, 0x5DDB,
                            0x10000);
    }
    return 0;
}

void func_ov030_020a2208(RewardTableObject* arg0) {
    func_ov030_020ae96c(23);
    data_02071cf0.unk_20.unk_244C = 0x12C000;
    data_02071cf0.unk_20.unk_2450 = 0x87000;
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x886E;
    func_ov030_020c26bc(arg0);
    data_02071cf0.unk_20.unk_267A = 4;
}

s32 func_ov030_020a2268(RewardTableObject* arg0) {
    s16 var_r0;

    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if (func_ov030_020aebb0(0) == 0) {
            func_ov030_020aeb34(0);
            func_ov030_020a9c54(arg0, 0x86F, 0);
            return 1;
        }
        if (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) {
            var_r0 = data_02071cf0.unk_20.unk_30FC;
        } else {
            var_r0 = 0;
        }
        if (((s32)var_r0 >= 2) && (func_ov030_020aebb0(3) == 0)) {
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_2678 = 4;
            func_ov030_020a9c54(arg0, 0x874, 0);
            return 1;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x16) {
        if (func_ov030_020aebb0(4) == 0) {
            func_ov030_020aeb34(4);
            func_ov030_020a9c54(arg0, 0x877, 0);
            return 1;
        } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(6) == 0)) {
            func_ov030_020aeb34(6);
            func_ov030_020a9c54(arg0, 0x880, 0);
            return 1;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 23) && (func_ov030_020aed9c(0x3715) != 0) && (func_ov030_020aebb0(15) == 0)) {
        func_ov030_020aeb34(15);
        func_ov030_020a9c54(arg0, 0x87F, 0);
        return 1;
    }
    return 0;
}

s32 func_ov030_020a240c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24C8[data_02071cf0.unk_20.unk_24C4]) {
        case 0x35F9:
            if (func_ov030_020aebb0(15) != 0) {
                func_ov030_020a669c(0x87E);
                func_ov030_020a6738(arg0);
                return 1;
            }
            func_ov030_020aeb34(5);
            func_ov030_020a669c(0x87D);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FA:
            func_ov030_020aeb34(42);
            func_ov030_020a669c(0x888);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            func_ov030_020a669c(0x887);
            func_ov030_020a6738(arg0);
            return 1;
    }
}

s32 func_ov030_020a24d8(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            func_ov030_020aeb34(9);
            func_ov030_020a669c(0x879);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3606:
            func_ov030_020aeb34(10);
            func_ov030_020a669c(0x87A);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020a2568(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x3605:
            func_ov030_020aeb34(9);
            func_ov030_020a669c(0x879);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3606:
            func_ov030_020aeb34(10);
            func_ov030_020a669c(0x87A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x3607:
            func_ov030_020aeb34(11);
            func_ov030_020a669c(0x87B);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            return 0;
    }
}

s32 func_ov030_020a2628(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x86E:
            func_ov030_020a669c(0x889);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x86F:
            func_ov030_020a669c(0x88A);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x88A:
            ProgressReward_GrantPin(arg0, PIN_PYROKINESIS);
            ProgressReward_GrantPin(arg0, PIN_PSYCHOKINESIS);
            ProgressReward_GrantPin(arg0, PIN_THUNDERBOLT);
            ProgressReward_GrantPin(arg0, PIN_SHOCKWAVE);
            ProgressReward_GrantPin(arg0, PIN_CURE_DRINK);
            break;
        case 0x870:
            func_ov030_020aeb34(1);
            arg0->unk_21CF8 = 1;
            break;
        case 0x872:
            func_ov030_020aeb34(2);
            data_02071cf0.unk_20.unk_30FC = 0;
            data_02071cf0.unk_20.unk_30FE = 39;
            data_02071cf0.unk_20.unk_3100 = data_02071cf0.unk_20.unk_2458;
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
            if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(11) != 0)) {
                func_ov030_020a669c(0x87C);
                func_ov030_020a6738(arg0);
                return 1;
            }
            if (func_ov030_020aebb0(9) != 0) {
                return func_ov030_020a9f54(arg0, func_ov030_020a2568, 0x3604, 3, 0x3605, 0x3606, 0x3607);
            }
            return func_ov030_020a9f54(arg0, func_ov030_020a24d8, 0x3604, 2, 0x3605, 0x3606);
        case 0x87C:
            func_ov030_020aeb34(12);
            arg0->unk_21CF8 = 1;
            func_ov030_020aa9f4(arg0, 0x35F9, 0x5DD1, 0);
            break;
        case 0x885:
        case 0x886:
            return func_ov030_020aa0c8(arg0, 0xB0, 0x886, 0x887, func_ov030_020a240c);
        case 0x888:
            func_ov030_020aeb34(0xE);
            arg0->unk_21CF8 = 1;
            break;
        case 0x880:
            arg0->unk_21CF8 = 1;
            break;
        case 0x87F:
            func_ov030_020aeb34(15);
            func_ov030_020aa9f4(arg0, 0x35FA, 0x5DD1, 0);
            break;
        case 0x881:
            func_ov030_020aeb34(7);
            arg0->unk_21CF8 = 1;
            break;
        case 0x882:
            func_ov030_020aeb34(8);
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x883;
            func_ov030_020af364(10);
            return 1;
        case 0x883:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 0x884;
            data_02071cf0.unk_20.unk_3124 = 10;
            data_02071cf0.unk_20.unk_3128 = 0;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
        case 0x884:
            data_02071cf0.unk_20.unk_3124 = 0x4D;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
            DebugOvlDisp_Pop();
            return 0;
    }
    return 0;
}

void func_ov030_020a2a44(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 58;
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_020a2a84(void) {
    return 0;
}

s32 func_ov030_020a2a8c(RewardTableObject* arg0) {
    switch (arg0->unk_21D1C) {
        case 0:
            func_ov030_020a669c(0x44E);
            return 1;
        case 1:
            if (func_ov030_020aebb0(2) != 0) {
                func_ov030_020a9db8(arg0, 0x45F);
                return 0;
            }
            func_ov030_020aeb34(2);
            func_ov030_020a669c(0x451);
            return 1;
        case 2:
            func_ov030_020a669c(0x452);
            return 1;
        case 3:
            func_ov030_020a669c(0x455);
            return 1;
        case 4:
            func_ov030_020a669c(0x456);
            return 1;
        case 5:
            func_ov030_020a669c(0x457);
            return 1;
        case 6:
            func_ov030_020a669c(0x458);
            return 1;
        case 7:
            func_ov030_020a669c(0x459);
            return 1;
        case 8:
            func_ov030_020a669c(0x45A);
            return 1;
        case 9:
            func_ov030_020a669c(0x45B);
            return 1;
        case 10:
            func_ov030_020a669c(0x45C);
            return 1;
        case 11:
            func_ov030_020a669c(0x45D);
            return 1;
        case 13:
            func_ov030_020a9db8(arg0, 0x45E);
            return 0;
        default:
        case 12:
            return 0;
    }
}

s32 func_ov030_020a2c20(RewardTableObject* arg0) {
    if ((data_02071cf0.unk_20.unk_24BC == 0xE3) || (data_02071cf0.unk_20.unk_24BC == 0x45F)) {
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
    data_02071cf0.unk_20.unk_267C = 4;
    data_02071cf0.unk_20.unk_2680 = 4;
    data_02071cf0.unk_20.unk_2682 = 4;
    func_ov030_020aec38(1);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_NONE;
}

s32 func_ov030_020a2d54(void) {
    return 0;
}

s32 func_ov030_020a2d5c(RewardTableObject* arg0) {
    switch (arg0->unk_2176C[arg0->unk_2177C]) {
        case 0x35FA:
            func_ov030_020a669c(0x5A5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x35FB:
            func_ov030_020a669c(0x5A5);
            func_ov030_020a6738(arg0);
            return 1;
        default:
            func_ov030_020a669c(0x5A5);
            func_ov030_020a6738(arg0);
            return 1;
    }
}

s32 func_ov030_020a2de8(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
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
            func_ov030_020a669c(0x5A8);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x5A9:
            if (func_ov030_020a75c0(arg0) != 0) {
                return 1;
            }
            func_ov030_020a669c(0x5AA);
            func_ov030_020a6738(arg0);
            return 1;
    }
    return 0;
}

void func_ov030_020a2f74(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x3C;
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x8A;
}

// Nonmatching
s32 func_ov030_020a2fc4(void) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(2) == 0)) {
            func_ov030_020aeb34(2);
        }

        s32 var_r0 = (data_02071cf0.unk_20.unk_3100 == data_02071cf0.unk_20.unk_2458) ? data_02071cf0.unk_20.unk_30FC : 0;
        if ((var_r0 >= 2) && (func_ov030_020aebb0(1) == 0)) {
            func_ov030_020aeb34(1);
            func_ov030_020aeb34(0x1E);
            func_ov030_020aeb34(0x1F);
            func_ov030_020aeb34(0x20);
        }
    }
    return 0;
}

s32 func_ov030_020a306c(RewardTableObject* arg0) {
    if ((data_02071cf0.unk_20.unk_24BC == 0x8A) && (func_ov030_020aebb0(0) == 0)) {
        func_ov030_020aeb34(0);
    }
    return 0;
}

void func_ov030_020a30a4(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 61;
}

s32 func_ov030_020a30cc(RewardTableObject* arg0) {
    return 0;
}

s32 func_ov030_020a30d4(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x5B3:
            func_ov030_020a669c(0x5B4);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x5B4:
            func_ov030_020a669c(0x5B5);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x5B5:
            func_ov030_020a669c(0x5B6);
            func_ov030_020a6738(arg0);
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

s32 func_ov030_020a317c(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3184(void) {
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.unk_26D4 = 0;
    func_ov030_020ae96c(0x10);
    func_ov030_020c26bc(4);
}

s32 func_ov030_020a31b4(void) {
    return 0;
}

s32 func_ov030_020a31bc(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a31c4(void) {
    func_ov030_020ae96c(1);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_24B4 = 0;
    data_02071cf0.unk_20.unk_24B8 = 1;
    data_02071cf0.unk_20.unk_24BC = 0x88B;
    func_ov030_020aec1c(4);
    data_02071cf0.unk_20.playerStats.activeFriend = FRIEND_JOSHUA;
}

s32 func_ov030_020a321c(s32 arg0) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aebb0(0) != 0) && (func_ov030_020aebb0(2) == 0)) {
        func_ov030_020aeb34(2);
        func_ov030_020a9c54(arg0, 0x88E, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 15) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(11) != 0) &&
               (func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(7) == 0))
    {
        func_ov030_020aeb34(7);
        func_ov030_020a9c54(arg0, 0x896, 0);
        return 1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 0x10) && (func_ov030_020aebb0(9) == 0)) {
        func_ov030_020aeb34(9);
        func_ov030_020a9c54(arg0, 0x899, 0);
        return 1;
    } else {
        return 0;
    }
}

s32 func_ov030_020a333c(RewardTableObject* arg0) {
    switch (data_02071cf0.unk_20.unk_24BC) {
        case 0x88C:
            func_ov030_020aeb34(0);
            func_ov030_020a669c(0x89A);
            func_ov030_020a6738(arg0);
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
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 2189;
            func_ov030_020af364(0x39);
            return 1;
        case 0x88D:
            func_ov030_020aeb34(1);
            data_02071cf0.unk_20.unk_264E = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x88F:
            func_ov030_020a669c(0x89B);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x89B:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 2192;
            func_ov030_020af364(0x35);
            return 1;
        case 0x890:
            func_ov030_020aeb34(3);
            data_02071cf0.unk_20.unk_2664 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x891:
            func_ov030_020a669c(2204);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x89C:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 2194;
            func_ov030_020af364(54);
            return 1;
        case 0x892:
            func_ov030_020aeb34(4);
            data_02071cf0.unk_20.unk_2666 = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x893:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 2196;
            func_ov030_020af364(55);
            return 1;
        case 0x894:
            func_ov030_020aeb34(5);
            data_02071cf0.unk_20.unk_2668 = 4;
            func_ov030_020a669c(2205);
            func_ov030_020a6738(arg0);
            return 1;
        case 0x89D:
            arg0->unk_21CF8 = 1;
            break;
        case 0x895:
            if (func_ov030_020aebb0(6) == 0) {
                func_ov030_020aeb34(6);
                data_02071cf0.unk_20.unk_310C = &data_ov030_020d9be4;
                func_ov030_020c1960();
                func_ov030_020acfc4(arg0);
                func_ov030_020a669c(2206);
                func_ov030_020a6738(arg0);
                return 1;
            }
            break;
        case 0x89E:
            ProgressReward_GrantItem(arg0, ITEM_STICKER_CHAIN_4);
            break;
        case 0x896:
            func_ov030_020aeb34(7);
            data_02071cf0.unk_20.unk_266A = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x897:
            data_02071cf0.unk_20.unk_24B4 = 0;
            data_02071cf0.unk_20.unk_24B8 = 1;
            data_02071cf0.unk_20.unk_24BC = 2200;
            func_ov030_020af364(56);
            return 1;
        case 0x898:
            func_ov030_020aeb34(8);
            data_02071cf0.unk_20.unk_266C = 4;
            arg0->unk_21CF8               = 1;
            break;
        case 0x899:
            data_02071cf0.unk_20.unk_3124 = 0x4D;
            data_02071cf0.unk_20.unk_3128 = 1;
            arg0->unk_21630               = 4;
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
    if (data_02071cf0.unk_20.unk_2458 == 0x10) {
        func_ov030_020848e4(arg0, 0x44);
    }
    return 0;
}

s32 func_ov030_020a3718(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3720(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x42;
}

s32 func_ov030_020a3748(void) {
    return 0;
}

s32 func_ov030_020a3750(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3758(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x43;
}

s32 func_ov030_020a3780(void) {
    return 0;
}

s32 func_ov030_020a3788(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3790(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x44;
}

s32 func_ov030_020a37b8(void) {
    return 0;
}

s32 func_ov030_020a37c0(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a37c8(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x45;
}

s32 func_ov030_020a37f0(void) {
    return 0;
}

s32 func_ov030_020a37f8(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3800(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x46;
}

s32 func_ov030_020a3828(void) {
    return 0;
}

s32 func_ov030_020a3830(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3838(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
}

s32 func_ov030_020a3850(void) {
    return 0;
}

s32 func_ov030_020a3858(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3860(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x48;
}

s32 func_ov030_020a3888(void) {
    return 0;
}

s32 func_ov030_020a3890(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3898(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x49;
}

s32 func_ov030_020a38c0(void) {
    return 0;
}

s32 func_ov030_020a38c8(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a38d0(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x4A;
}

s32 func_ov030_020a38f8(void) {
    return 0;
}

s32 func_ov030_020a3900(RewardTableObject* arg0) {
    return 0;
}

void func_ov030_020a3908(void) {
    func_ov030_020ae96c(18);
    func_ov030_020c26bc(0);
    data_02071cf0.unk_20.unk_3124 = 0x4B;
}

s32 func_ov030_020a3930(void) {
    return 0;
}

s32 func_ov030_020a3938(RewardTableObject* arg0) {
    return 0;
}

s32 func_ov030_020a3940(void) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        return 0x28;
    } else if (data_02071cf0.unk_20.unk_2458 == 3) {
        return 41;
    } else {
        return 0;
    }
}

s32 func_ov030_020a3968(void) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (func_ov030_020aebb0(33) == 0) {
            return 0x5C0;
        } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            return 0x5BF;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 5) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x5BF;
    } else {
        return 0;
    }
}

s32 func_ov030_020a39f8(void) {
    if (data_02071cf0.unk_20.unk_2458 == 5) {
        if ((func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 0x5C8;
        } else if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
            return 0x5CC;
        }
    }

    if (func_ov030_020aebb0(0x20) == 0) {
        return 0x5CD;
    } else if ((func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(33) == 0)) {
        return 0x5C4;
    } else if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 0x5C5;
    } else if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(42) == 0)) {
        return 0x5CB;
    } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(43) == 0) && (func_ov030_020aebb0(5) == 0)) {
        return 0x5C6;
    } else if ((func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(5) == 0)) {
        return 0x5C7;
    } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(6) == 0)) {
        return 0x5CE;
    } else if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
        return 0x5CF;
    } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(47) == 0)) {
        return 0x5C9;
    } else if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(8) == 0)) {
        return 0x5CA;
    } else {
        return 0;
    }
}

s32 func_ov030_020a3bf8(void) {
    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x3F) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            return 0x5DC;
        } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(8) == 0)) {
            return 0x5E9;
        } else if ((func_ov030_020aebb0(8) != 0) && (func_ov030_020aebb0(2) == 0)) {
            return 0x5DD;
        } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) == 0)) {
            return 0x5DE;
        } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(11) == 0)) {
            return 0x5DF;
        } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(0x10) == 0) && (func_ov030_020aebb0(41) == 0) &&
                   (func_ov030_020aebb0(0x13) == 0))
        {
            return 0x5E7;
        } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(0x10) == 0)) {
            return 0x5E7;
        } else if ((func_ov030_020aebb0(0x10) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 0x5E8;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if ((func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(5) == 0) && (func_ov030_020aebb0(10) == 0)) {
            return 0x5D9;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            return 0x5DA;
        } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) == 0)) {
            return 0x5F2;
        } else if ((func_ov030_020aebb0(0x42) != 0) && (func_ov030_020aebb0(0x43) == 0)) {
            return 0x5DB;
        } else if ((func_ov030_020aebb0(0x43) != 0) && (func_ov030_020aebb0(4) == 0)) {
            return 0x5F1;
        } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(11) == 0)) {
            return 0x5DF;
        } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(0x10) == 0) && (func_ov030_020aebb0(41) == 0) &&
                   (func_ov030_020aebb0(0x13) == 0))
        {
            return 0x5E6;
        } else if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(0x11) == 0)) {
            return 0x5EA;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x16) {
        if ((func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            return 0x5EC;
        } else if ((func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 0x5E0;
        } else if ((func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(0x4A) == 0) && (func_ov030_020aebb0(37) == 0)) {
            return 0x5E2;
        } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(0x10) == 0) && (func_ov030_020aebb0(41) == 0) &&
                   (func_ov030_020aebb0(0x13) == 0))
        {
            return 0x5E6;
        } else if ((func_ov030_020aebb0(0x10) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 0x5E8;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 23) {
        if ((func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            return 0x5D8;
        } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) == 0)) {
            return 0x5EB;
        } else if ((func_ov030_020aebb0(0x10) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 0x5EB;
        } else if (func_ov030_020aebb0(15) != 0) {
            return 0x5ED;
        } else if ((func_ov030_020aebb0(0x4A) != 0) && (func_ov030_020aebb0(38) == 0)) {
            return 0x5E4;
        } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
            return 0x5E5;
        } else if (func_ov030_020aebb0(45) != 0) {
            return 0x5EF;
        }
    }

    if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(9) == 0)) {
        return 0x5F0;
    } else if ((func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(0x4A) == 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x5F3;
    } else if ((func_ov030_020aebb0(0x4A) != 0) && (func_ov030_020aebb0(38) == 0)) {
        return 0x5E3;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(38) == 0) && (func_ov030_020aebb0(13) == 0)) {
        return 0x5EE;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(13) == 0)) {
        return 0x5E5;
    } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
        return 0x5E5;
    } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(12) != 0) &&
               (func_ov030_020aebb0(13) == 0))
    {
        return 0x5E5;
    } else if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(0x11) == 0)) {
        return 0x5EA;
    } else {
        return 0;
    }
}

s32 func_ov030_020a428c(void) {
    if (data_02071cf0.unk_20.unk_2458 == 3) {
        if (func_ov030_020aebb0(4) == 0) {
            return 0x5F6;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(33) == 0)) {
            return 0x5F7;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x34) == 0)) {
            return 0x5F8;
        } else if ((func_ov030_020aebb0(0x34) != 0) && (func_ov030_020aebb0(2) == 0)) {
            return 0x5FA;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 11) {
        if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x31) == 0)) {
            return 0x5F9;
        } else if ((func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(3) == 0)) {
            return 0x5FB;
        }
    }

    if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(6) == 0)) {
        return 0x5FC;
    } else {
        return 0;
    }
}

s32 func_ov030_020a43d4(void) {
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020aebb0(0x43) != 0) && (func_ov030_020aebb0(0x44) == 0)) {
            return 0x60A;
        } else if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(3) == 0)) {
            return 0x60F;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0) {
        if ((func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(5) == 0)) {
            return 0x60B;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 0x60C;
        } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
            return 0x60E;
        }
    }
    if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(8) == 0)) {
        return 0x60D;
    } else {
        return 0;
    }
}

s32 func_ov030_020a44f4(void) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(12) == 0)) {
            return 0x61F;
        } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(13) == 0)) {
            return 0x621;
        } else if ((func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
            return 0x622;
        } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(33) == 0)) {
            return 0x623;
        } else if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(0x10) == 0)) {
            return 0x626;
        } else if ((func_ov030_020aebb0(0x10) != 0) && (func_ov030_020aebb0(0x11) == 0)) {
            return 0x627;
        } else if ((func_ov030_020aebb0(0x11) != 0) && (func_ov030_020aebb0(18) == 0)) {
            return 0x628;
        } else if ((func_ov030_020aebb0(18) != 0) && (func_ov030_020aebb0(0x13) == 0)) {
            return 0x629;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 4) {
        if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(10) == 0)) {
            return 0x624;
        } else if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x33) == 0)) {
            return 0x62C;
        } else if ((func_ov030_020aebb0(0x33) != 0) && (func_ov030_020aebb0(4) == 0)) {
            return 0x62D;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 0) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(9) == 0)) {
        return 0x624;
    } else if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(0x14) != 0) && (func_ov030_020aebb0(0x15) == 0)) {
        return 0x62B;
    } else if ((data_02071cf0.unk_20.unk_2458 == 2) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
        return 0x62E;
    } else if ((data_02071cf0.unk_20.unk_2458 == 6) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
        return 0x62E;
    } else if (func_ov030_020aebb0(11) == 0) {
        return 0x61E;
    } else if ((func_ov030_020aebb0(11) != 0) && (func_ov030_020aebb0(12) == 0)) {
        return 0x620;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
        return 0x631;
    } else if ((func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(33) == 0)) {
        return 0x623;
    } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
        return 0x625;
    } else if ((func_ov030_020aebb0(0x11) != 0) && (func_ov030_020aebb0(18) == 0)) {
        return 0x62F;
    } else if ((func_ov030_020aebb0(18) != 0) && (func_ov030_020aebb0(0x13) == 0)) {
        return 0x630;
    } else if ((func_ov030_020aebb0(0x13) != 0) && (func_ov030_020aebb0(0x14) == 0)) {
        return 0x62A;
    } else {
        return 0;
    }
}

s32 func_ov030_020a48d8(void) {
    if (data_02071cf0.unk_20.unk_2458 == 5) {
        if ((func_ov030_020aebb0(43) != 0) && (func_ov030_020aebb0(0x2C) == 0)) {
            return 0x633;
        } else if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(5) == 0)) {
            return 0x634;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
            return 0x635;
        }
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x11) && (func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x20) == 0)) {
        return 0x635;
    } else if (func_ov030_020aebb0(5) != 0) {
        return 0;
    } else
        return 0x632;
}

s32 func_ov030_020a49b8(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x646;
    } else if ((data_02071cf0.unk_20.unk_2458 == 5) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x646;
    } else {
        return 0;
    }
}

s32 func_ov030_020a4a34(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
        return 0x64B;
    }
    if (data_02071cf0.unk_20.unk_2458 == 11) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(46) == 0)) {
            return 0x64D;
        } else if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(3) == 0)) {
            return 0x651;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 12) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x64E;
    } else if ((data_02071cf0.unk_20.unk_2458 == 8) && (func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x650;
    } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x64C;
    } else if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x64F;
    } else {
        return 0;
    }
}

s32 func_ov030_020a4b98(void) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(0x20) == 0) && (func_ov030_020aebb0(33) == 0)) {
            return 0x65F;
        } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0)) {
            return 0x668;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0)) {
        return 0x668;
    }
    if (data_02071cf0.unk_20.unk_2458 == 12) {
        if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(0x46) == 0)) {
            return 0x660;
        } else if ((func_ov030_020aebb0(0x46) != 0) && (func_ov030_020aebb0(0x47) == 0)) {
            return 0x661;
        } else if ((func_ov030_020aebb0(0x47) != 0) && (func_ov030_020aebb0(5) == 0)) {
            return 0x672;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 13) && (func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x48) == 0)) {
        return 0x662;
    }
    if (data_02071cf0.unk_20.unk_2458 == 15) {
        if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(0x48) == 0)) {
            return 0x662;
        } else if ((func_ov030_020aebb0(0x48) != 0) && (func_ov030_020aebb0(0x5C) == 0)) {
            return 0x663;
        } else if ((func_ov030_020aebb0(0x5C) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 0x673;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 0xE) && (func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
        return 0x662;
    }
    if (data_02071cf0.unk_20.unk_2458 == 0x13) {
        if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x32) == 0)) {
            return 0x66A;
        } else if ((func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(0x35) == 0)) {
            return 0x66C;
        } else if ((func_ov030_020aebb0(0x35) != 0) && (func_ov030_020aebb0(12) == 0)) {
            return 0x66D;
        } else if ((func_ov030_020aebb0(0x42) != 0) && (func_ov030_020aebb0(0x3E) == 0)) {
            return 0x671;
        } else if ((func_ov030_020aebb0(0x43) != 0) && (func_ov030_020aebb0(0x39) == 0)) {
            return 0x671;
        } else if ((func_ov030_020aebb0(0x44) != 0) && (func_ov030_020aebb0(0x3B) == 0)) {
            return 0x671;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if ((func_ov030_020aebb0(0x3C) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 0x66F;
        } else if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(0x10) == 0)) {
            return 0x670;
        }
    }
    if ((func_ov030_020aebb0(0x20) == 0) && (func_ov030_020aebb0(33) == 0)) {
        return 0x65F;
    } else if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
        return 0x664;
    } else if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(9) == 0)) {
        return 0x665;
    } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) == 0)) {
        return 0x666;
    } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(47) == 0)) {
        return 0x667;
    } else if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(0x30) == 0)) {
        return 0x669;
    } else if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(12) == 0)) {
        return 0x66B;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(0x3E) == 0)) {
        return 0x66E;
    }
    return 0;
}

s32 func_ov030_020a5040(void) {
    if (data_02071cf0.unk_20.unk_2458 == 4) {
        if ((func_ov030_020aebb0(56) != 0) && (func_ov030_020aebb0(61) == 0)) {
            return 0x68E;
        } else if ((func_ov030_020aebb0(61) != 0) && (func_ov030_020aebb0(2) == 0)) {
            return 0x691;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 2) {
        if ((func_ov030_020aebb0(0x39) != 0) && (func_ov030_020aebb0(58) == 0)) {
            return 0x679;
        } else if ((func_ov030_020aebb0(58) != 0) && (func_ov030_020aebb0(3) == 0)) {
            return 0x692;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(9) == 0)) {
            return 0x67B;
        } else if ((func_ov030_020aebb0(10) != 0) && (func_ov030_020aebb0(42) == 0)) {
            return 0x67D;
        } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 0x687;
        } else if (func_ov030_020aebb0(15) != 0) {
            return 1672;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 8) && (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
        return 0x684;
    } else if ((data_02071cf0.unk_20.unk_2458 == 7) && (func_ov030_020aebb0(0x23) != 0)) {
        return 0x689;
    } else if (data_02071cf0.unk_20.unk_2458 == 9) {
        if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 0x68A;
        } else if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
            return 0x68C;
        } else if (func_ov030_020aebb0(7) != 0) {
            return 0x68B;
        }
    }

    if (func_ov030_020aebb0(39) == 0) {
        return 0x678;
    } else if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(9) == 0)) {
        return 0x67A;
    } else if ((func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(10) == 0)) {
        return 0x67C;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(15) == 0)) {
        return 0x686;
    } else if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(0xE) == 0)) {
        return 0x68D;
    } else if ((func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(0xE) != 0) && (func_ov030_020aebb0(12) == 0)) {
        return 0x685;
    } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(45) == 0) && (func_ov030_020aebb0(47) == 0)) {
        return 0x67E;
    } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(47) == 0) &&
               (func_ov030_020aebb0(46) == 0))
    {
        return 0x67F;
    } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(45) == 0) && (func_ov030_020aebb0(47) != 0) &&
               (func_ov030_020aebb0(13) == 0))
    {
        return 0x680;
    }
    if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(47) != 0) &&
        (func_ov030_020aebb0(13) == 0) && (func_ov030_020aebb0(46) != 0) && (func_ov030_020aebb0(12) == 0))
    {
        return 0x683;
    } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(47) != 0) &&
               (func_ov030_020aebb0(13) == 0) && (func_ov030_020aebb0(46) == 0))
    {
        return 0x681;
    } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(47) != 0) &&
               (func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(46) == 0))
    {
        return 1666;
    } else if ((func_ov030_020aebb0(13) != 0) && (func_ov030_020aebb0(45) == 0)) {
        return 1683;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(47) == 0)) {
        return 1679;
    } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(13) == 0)) {
        return 1680;
    } else if (func_ov030_020aebb0(15) != 0) {
        return 1672;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5608(void) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(0x2C) != 0) && (func_ov030_020aebb0(0x46) == 0)) {
            return 0x698;
        }
        if ((func_ov030_020aebb0(0x46) != 0) && (func_ov030_020aebb0(3) == 0)) {
            return 0x6A6;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 5) && (func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x6A3;
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0x11) && (func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x6A3;
    }
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            return 0x6A4;
        }
        if ((func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(42) == 0)) {
            return 0x6A5;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 15) && (func_ov030_020aebb0(0x31) != 0) && (func_ov030_020aebb0(7) == 0)) {
        return 0x69C;
    }
    if ((data_02071cf0.unk_20.unk_2458 == 0xE) && (func_ov030_020aebb0(0x32) != 0) && (func_ov030_020aebb0(8) == 0)) {
        return 0x69D;
    }
    if ((func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(4) == 0)) {
        return 0x699;
    }
    if ((func_ov030_020aebb0(47) != 0) && (func_ov030_020aebb0(5) == 0)) {
        return 0x69A;
    }
    if ((func_ov030_020aebb0(0x30) != 0) && (func_ov030_020aebb0(0x4A) == 0)) {
        return 0x6A9;
    }
    if ((func_ov030_020aebb0(0x4A) != 0) && (func_ov030_020aebb0(6) == 0)) {
        return 0x6A9;
    }
    if ((func_ov030_020aebb0(0x22) == 0) && (func_ov030_020aebb0(2) == 0)) {
        return 0x696;
    }
    if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(8) == 0)) {
        return 0x697;
    }
    if ((func_ov030_020aebb0(8) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
        return 0x69E;
    }
    if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
        return 0x69F;
    }
    if ((func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(38) == 0)) {
        return 0x6A0;
    }
    if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(39) == 0)) {
        return 0x6A1;
    }
    if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x6A2;
    }
    return 0;
}

s32 func_ov030_020a5964(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aebb0(33) == 0)) {
        return 0x6AA;
    } else if ((data_02071cf0.unk_20.unk_2458 == 2) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(2) == 0) &&
               (func_ov030_020aebb0(38) == 0))
    {
        return 0x6AC;
    } else if (func_ov030_020aebb0(33) == 0) {
        return 0x6AA;
    } else if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(0x23) == 0)) {
        return 0x6AB;
    } else if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(2) == 0) && (func_ov030_020aebb0(38) == 0)) {
        return 0x6AD;
    } else if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(2) == 0) && (func_ov030_020aebb0(38) != 0) &&
               (func_ov030_020aebb0(39) == 0))
    {
        return 0x6AE;
    } else if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(39) == 0)) {
        return 0x6AE;
    } else if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(41) == 0)) {
        return 0x6AF;
    } else if ((func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(42) == 0)) {
        return 0x6B0;
    } else
        return 0;
}

s32 func_ov030_020a5b20(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 0x18) && (func_ov030_020aebb0(2) != 0)) {
        return 0x6C1;
    } else if ((data_02071cf0.unk_20.unk_2458 == 0x19) && (func_ov030_020aebb0(0x30) != 0)) {
        return 0x6C2;
    } else if (func_ov030_020aebb0(33) == 0) {
        return 0x6BE;
    } else if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(33) != 0)) {
        return 0x6BF;
    } else if (func_ov030_020aebb0(0x30) != 0) {
        return 0x6C3;
    } else
        return 0;
}

s32 func_ov030_020a5bd4(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 0x11) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(38) == 0)) {
        return 0x6D2;
    } else if ((data_02071cf0.unk_20.unk_2458 == 18) && (func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(38) == 0)) {
        return 0x6D3;
    } else if (func_ov030_020aebb0(38) != 0) {
        return 0x6D4;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5c68(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x6D8;
    } else if ((data_02071cf0.unk_20.unk_2458 == 0x13) && (func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x6D8;
    } else if ((data_02071cf0.unk_20.unk_2458 == 23) && (func_ov030_020aebb0(0x28) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 0x6DC;
    } else if (func_ov030_020aebb0(37) == 0) {
        return 0x6D7;
    } else if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(38) == 0)) {
        return 0x6D9;
    } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(39) == 0)) {
        return 0x6DA;
    } else if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 0x6DB;
    } else {
        return 0;
    }
}

s32 func_ov030_020a5dac(void) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if (func_ov030_020aebb0(0x20) == 0) {
            return 0x6EB;
        } else if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
            return 0x6EE;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 4) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(39) == 0)) {
        return 0x6EF;
    } else if ((data_02071cf0.unk_20.unk_2458 == 2) && (func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(39) == 0)) {
        return 0x6EF;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
            return 0x6F2;
        } else if (func_ov030_020aebb0(0x28) != 0) {
            return 0x6F3;
        }
    }

    if ((func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 0x6EC;
    } else if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(0x24) == 0)) {
        return 0x6ED;
    } else if ((func_ov030_020aebb0(0x24) != 0) && (func_ov030_020aebb0(39) == 0)) {
        return 0x6F0;
    } else if ((func_ov030_020aebb0(39) != 0) && (func_ov030_020aebb0(0x28) == 0)) {
        return 0x6F1;
    }
    return 0;
}

s32 func_ov030_020a5f64(void) {
    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020aebb0(0x20) != 0) && (func_ov030_020aebb0(33) == 0)) {
            return 0x714;
        } else if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(0x22) == 0)) {
            return 0x715;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 7) {
        if ((func_ov030_020aebb0(0x23) != 0) && (func_ov030_020aebb0(37) == 0)) {
            return 0x717;
        } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(4) == 0)) {
            return 0x719;
        }
    }
    if (data_02071cf0.unk_20.unk_2458 == 9) {
        if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(4) == 0)) {
            return 0x71A;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(41) == 0)) {
            return 0x71B;
        }
    }
    if (func_ov030_020aebb0(0x22) == 0) {
        return 0x713;
    } else if ((func_ov030_020aebb0(0x22) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x716;
    } else if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(4) == 0)) {
        return 0x718;
    } else {
        return 0;
    }
}

s32 func_ov030_020a60fc(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 0x10) && (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 0x728;
    } else if ((data_02071cf0.unk_20.unk_2458 == 0xE) && (func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 0x728;
    } else if ((data_02071cf0.unk_20.unk_2458 == 12) && (func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x72A;
    } else if (func_ov030_020aebb0(3) == 0) {
        return 0x727;
    } else if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 0x729;
    } else {
        return 0;
    }
}

s32 func_ov030_020a61f0(void) {
    if (data_02071cf0.unk_20.unk_2458 == 41) {
        if ((func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 1857;
        } else if ((func_ov030_020aebb0(42) != 0) && (func_ov030_020aebb0(43) == 0)) {
            return 1858;
        }
    }
    if (func_ov030_020aebb0(33) == 0) {
        return 1851;
    } else if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 1852;
    } else if ((func_ov030_020aebb0(3) != 0) && (func_ov030_020aebb0(37) == 0)) {
        return 1853;
    } else if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(38) == 0)) {
        return 1854;
    } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(5) == 0)) {
        return 1855;
    } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(6) == 0)) {
        return 1856;
    } else
        return 0;
}

s32 func_ov030_020a633c(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 43) && (func_ov030_020aebb0(127) != 0)) {
        return 1871;
    } else {
        return 0;
    }
}

s32 func_ov030_020a6374(void) {
    if (data_02071cf0.unk_20.unk_2458 == 23) {
        if (func_ov030_020aebb0(0) == 0) {
            return 1891;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 1898;
        } else if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 1899;
        } else if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
            return 1901;
        } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(8) == 0)) {
            return 1903;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        if (func_ov030_020aebb0(1) == 0) {
            return 1892;
        } else if ((func_ov030_020aebb0(1) != 0) && (func_ov030_020aebb0(2) == 0)) {
            return 1893;
        } else if ((func_ov030_020aebb0(2) != 0) && (func_ov030_020aebb0(3) == 0)) {
            return 1894;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 1897;
        } else if ((func_ov030_020aebb0(15) != 0) && (func_ov030_020aebb0(6) == 0)) {
            return 1899;
        } else if ((func_ov030_020aebb0(7) != 0) && (func_ov030_020aebb0(8) == 0)) {
            return 1902;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 0x16) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(12) == 0)) {
            return 1895;
        } else if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(5) == 0) && (func_ov030_020aebb0(6) == 0)) {
            return 1896;
        } else if ((func_ov030_020aebb0(5) != 0) && (func_ov030_020aebb0(15) == 0)) {
            return 1897;
        } else if ((func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
            return 1900;
        }
    }

    return 0;
}

s32 func_ov030_020a65f4(void) {
    if ((data_02071cf0.unk_20.unk_2458 == 1) && (func_ov030_020aebb0(0) == 0)) {
        return 1911;
    } else if ((data_02071cf0.unk_20.unk_2458 == 11) && (func_ov030_020aebb0(1) != 0) && (func_ov030_020aebb0(3) == 0)) {
        return 1912;
    } else if ((data_02071cf0.unk_20.unk_2458 == 15) && (func_ov030_020aebb0(6) != 0) && (func_ov030_020aebb0(7) == 0)) {
        return 1915;
    } else {
        return 0;
    }
}

typedef s32 (*ProgressRewardFunc)(RewardTableObject*);

const ProgressRewardFunc data_ov030_020da618[] = {
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
