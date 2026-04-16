#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/Input.h"
#include "Engine/Math/Random.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Save.h"
#include "SndMgr.h"
#include <NitroSDK/mi/cpumem.h>

extern BinIdentifier data_ov003_020d77e0;

typedef struct BtlPlayer01 {
    /* 0x000 */ char         unk_000[0x4];
    /* 0x004 */ s16          unk_004;
    /* 0x006 */ char         unk_008[0xC - 0x6];
    /* 0x00C */ s32          unk_00C;
    /* 0x010 */ s16          unk_010;
    /* 0x012 */ u16          unk_012;
    /* 0x014 */ char         unk_014[0x24 - 0x14];
    /* 0x024 */ s32          unk_024;
    /* 0x028 */ s32          unk_028;
    /* 0x02C */ s32          unk_02C;
    /* 0x030 */ s32          unk_030;
    /* 0x034 */ char         unk_034[0x38 - 0x34];
    /* 0x038 */ s32          unk_038;
    /* 0x03C */ char         unk_03C[0x40 - 0x3C];
    /* 0x040 */ s32          unk_040;
    /* 0x044 */ char         unk_044[0x54 - 0x44];
    /* 0x054 */ s32          unk_054;
    /* 0x058 */ char         unk_058[0x5C - 0x58];
    /* 0x05C */ u16          unk_05C;
    /* 0x05E */ char         unk_05E[0x66 - 0x5E];
    /* 0x066 */ s16          unk_066;
    /* 0x068 */ s16          unk_068;
    /* 0x06A */ char         unk_06A[0x70 - 0x6A];
    /* 0x070 */ s16          unk_070;
    /* 0x072 */ s16          unk_072;
    /* 0x074 */ s16          unk_074;
    /* 0x076 */ s16          unk_076;
    /* 0x078 */ char         unk_078[0x7C - 0x78];
    /* 0x07C */ CombatSprite unk_07C;
    /* 0x0DC */ CombatSprite unk_0DC;
    /* 0x13C */ s16          unk_13C;
    /* 0x13E */ s16          unk_13E;
    /* 0x140 */ u16          unk_140;
    /* 0x142 */ s16          unk_142;
    /* 0x144 */ s16          unk_144;
    /* 0x146 */ s16          unk_146;
    /* 0x148 */ s16          unk_148;
    /* 0x14A */ char         unk_14A[0x2];
    /* 0x14C */ void (*unk_14C)(struct BtlPlayer01*);
    /* 0x150 */ s16 unk_150;
    /* 0x152 */ s16 unk_152;
} BtlPlayer01; // Size: 0x154

void func_ov004_020e7f30(BtlPlayer01* player);
void func_ov004_020e8394(BtlPlayer01* player);
void func_ov004_020e84c4(BtlPlayer01* player);
void func_ov004_020e851c(BtlPlayer01* player);
void func_ov004_020e85d8(BtlPlayer01* player);
void func_ov004_020e8790(BtlPlayer01* player);
void func_ov004_020e8878(BtlPlayer01* player);
void func_ov004_020e8a00(BtlPlayer01* player);
void func_ov004_020e8b00(BtlPlayer01* player);
void func_ov004_020e8b94(BtlPlayer01* player);
void func_ov004_020e8d00(BtlPlayer01* player);
s32  BtlPlayer01_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlPlayer01 = {"Tsk_BtlPlayer01", BtlPlayer01_RunTask, 0x154};

static const u16 data_ov004_020f0068[4] = {3, 3, 4, 5};

static const SpriteAnimEntry data_ov004_020f008c[18] = {
    {1, 3, 2,  0},
    {1, 3, 2,  3},
    {1, 3, 2,  1},
    {1, 3, 2,  2},
    {4, 6, 5,  0},
    {4, 6, 5,  1},
    {4, 6, 5,  2},
    {4, 6, 5,  3},
    {4, 6, 5,  4},
    {4, 6, 5,  5},
    {4, 6, 5,  6},
    {4, 6, 5,  7},
    {4, 6, 5,  8},
    {4, 6, 5,  9},
    {4, 6, 5, 10},
    {4, 6, 5, 11},
    {4, 6, 5, 12},
    {4, 6, 5,  1},
};

BOOL func_ov004_020e7360(void) {
    return func_02023010(688) != 0;
}

// Nonmatching: External data access differences
u16 func_ov004_020e737c(void) {
    return data_ov004_020f0068[data_02071cf0.unk_20.unk_3141];
}

// Not yet implemented, unsure of correct structure being accessed
void func_ov004_020e739c(s16* arg0, s32 arg1) {
    // s16   var_r1;
    // s32   var_r5;
    // u16   temp_r1;
    // void* var_r0;

    // func_ov003_02086c04(&subroutine_arg0, func_ov004_020e737c());
    // var_r5 = 0;
    // var_r0 = func_ov003_0208495c(arg0 + 4);
    // if (var_r0 != NULL) {
    //     do {
    //         temp_r1 = (&subroutine_arg0)[var_r5];
    //         var_r5 += 1;
    //         var_r0->unk0  = temp_r1;
    //         var_r0->unkCA = 0;
    //         if (arg1 != 0) {
    //             var_r1 = 0;
    //         } else {
    //             var_r1 = 1;
    //         }
    //         var_r0->unkC4 = var_r1;
    //         var_r0->unkC8 = 0;
    //         var_r0->unkC6 = 0;
    //         var_r0        = func_ov003_02084984(var_r0 + 0xDC);
    //     } while (var_r0 != NULL);
    // }
    // *arg0 = 0;
}

// Not yet implemented, unsure of correct structure being accessed
void func_ov004_020e742c(s16* arg0, s16 arg1) {
    // void* var_r0;

    // var_r0 = func_ov003_0208495c(arg0 + 4);
    // if (var_r0 == NULL) {
    //     return;
    // }
    // loop_2:
    // if (var_r0->unkCC == arg1) {
    //     return;
    // }
    // var_r0 = func_ov003_02084984(var_r0 + 0xDC);
    // if (var_r0 == NULL) {
    //     return;
    // }
    // goto loop_2;
}

static void func_ov004_020e7464(BtlPlayer01* player) {
    if (player->unk_142 > 0) {
        player->unk_142--;
    }

    if (player->unk_144 > 0) {
        player->unk_144--;
    }
}

static void func_ov004_020e7490(BtlPlayer01* player, s32 arg1) {
    u16 var_r1;

    switch (arg1) {
        case 0:
            var_r1 = 4;
            break;
        case 1:
            var_r1 = 5;
            break;
        case 2:
            var_r1 = 6;
            break;
        default:
            var_r1 = 5;
            break;
    }
    if (((u32)(player->unk_012 << 0x19) >> 0x1F) == 1) {
        var_r1 += 3;
    }
    CombatSprite_SetAnimFromTable(&player->unk_07C, var_r1, 1);
    if (arg1 == 0) {
        CombatSprite_Restart(&player->unk_07C);
    }
}

static void func_ov004_020e750c(BtlPlayer01* player, s32 arg1) {
    if (arg1 != 0) {
        switch (RNG_Next(2)) {
            case 0:
                func_ov003_02087f64(917, player->unk_028);
                return;
            case 1:
                func_ov003_02087f64(918, player->unk_028);
                return;
        }
    } else if (player->unk_144 <= 0) {
        switch (RNG_Next(3)) {
            case 0:
                func_ov003_02087f64(914, player->unk_028);
                break;
            case 1:
                func_ov003_02087f64(915, player->unk_028);
                break;
            case 2:
                func_ov003_02087f64(916, player->unk_028);
                break;
        }
        player->unk_144 = 45;
    }
}

static void func_ov004_020e75d8(BtlPlayer01* player, s32 arg1) {
    if (arg1 != 0) {
        switch (RNG_Next(9)) {
            case 0:
                func_ov003_02087f64(899, player->unk_028);
                break;
            case 1:
                func_ov003_02087f64(900, player->unk_028);
                break;
            case 2:
                func_ov003_02087f64(901, player->unk_028);
                break;
            case 3:
                func_ov003_02087f64(902, player->unk_028);
                break;
            case 4:
                func_ov003_02087f64(903, player->unk_028);
                break;
            case 5:
                func_ov003_02087f64(904, player->unk_028);
                break;
            case 6:
                func_ov003_02087f64(905, player->unk_028);
                break;
            case 7:
                func_ov003_02087f64(906, player->unk_028);
                break;
            case 8:
                func_ov003_02087f64(907, player->unk_028);
                break;
        }
    } else if (player->unk_142 <= 0) {
        switch (RNG_Next(8)) {
            case 0:
                func_ov003_02087f64(891, player->unk_028);
                break;
            case 1:
                func_ov003_02087f64(892, player->unk_028);
                break;
            case 2:
                func_ov003_02087f64(893, player->unk_028);
                break;
            case 3:
                func_ov003_02087f64(894, player->unk_028);
                break;
            case 4:
                func_ov003_02087f64(895, player->unk_028);
                break;
            case 5:
                func_ov003_02087f64(896, player->unk_028);
                break;
            case 6:
                func_ov003_02087f64(897, player->unk_028);
                break;
            case 7:
                func_ov003_02087f64(898, player->unk_028);
                break;
        }
        player->unk_142 = 45;
    }
}

static void func_ov004_020e77b4(BtlPlayer01* player, u32 arg1) {
    switch (arg1) {
        case 7: {
            if (func_02023010(694)) {
                player->unk_14C = func_ov004_020e8b94;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;

        case 6: {
            if (func_02023010(693) && player->unk_146 <= 0) {
                func_ov003_02086aa8(0);
                player->unk_14C = func_ov004_020e8b00;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;

        case 3: {
            if (func_02023010(692) != 0) {
                player->unk_14C = func_ov004_020e8a00;
                player->unk_150 = 0;
                player->unk_152 = 0;
            } else {
                player->unk_14C = func_ov004_020e84c4;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;

        case 8: {
            player->unk_14C = func_ov004_020e84c4;
            player->unk_150 = 0;
            player->unk_152 = 0;
        } break;

        case 1:
        case 4:
        case 5: {
            player->unk_14C = func_ov004_020e851c;
            player->unk_150 = 0;
            player->unk_152 = 0;
        } break;

        case 2: {
            player->unk_14C = func_ov004_020e85d8;
            player->unk_150 = 0;
            player->unk_152 = 0;
        } break;
    }
}

static void func_ov004_020e7918(BtlPlayer01* player, s32 arg1) {
    switch (arg1) {
        case 3:
        case 8:
            player->unk_14C = func_ov004_020e8d00;
            player->unk_150 = 0;
            player->unk_152 = 0;
            break;

        case 1:
        case 4:
        case 5:
            player->unk_14C = func_ov004_020e8790;
            player->unk_150 = 0;
            player->unk_152 = 0;
            break;

        case 2:
            player->unk_14C = func_ov004_020e8878;
            player->unk_150 = 0;
            player->unk_152 = 0;
            break;
    }
}

static void func_ov004_020e79a8(BtlPlayer01* player, s32 arg1, s32 arg2) {
    s32 temp_r0;

    if (player->unk_140 == 0) {
        temp_r0 = func_ov003_020869e4();
    }

    switch (func_ov003_02086a40()) {
        case 1:
            player->unk_024 = 0;
            break;
        case 2:
            player->unk_024 = 1;
            break;
    }
    if (arg2 != 0) {
        player->unk_13E = 0;
    }

    if (arg1 != 0) {
        func_ov004_020e7918(player, temp_r0);
    } else {
        func_ov004_020e77b4(player, temp_r0);
    }
}

// Nonmatching
void func_ov004_020e7a24(BtlPlayer01* player) {
    player->unk_024 = func_ov003_020843b0(1, player->unk_028) < 0x80;

    if (data_ov003_020e71b8->unk3D874 == 2) {
        player->unk_14C = func_ov004_020e8394;
        player->unk_150 = 0;
        player->unk_152 = 0;
    }
}

void func_ov004_020e7a84(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                data_ov003_020e71b8->unk3D8E0 = 1;
                player->unk_140               = 8;
                player->unk_054 |= 2;
                func_ov004_020e7490(player, 0);
                func_ov004_020e750c(player, 0);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_152 == 0) {
                func_ov004_020e7490(player, 1);
                player->unk_152 = player->unk_010;
            }
            if (player->unk_152 > 0) {
                player->unk_152--;
            }
            if (player->unk_152 <= 0 && player->unk_05C != 4) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 2: {
            if (player->unk_152 == 0) {
                func_ov004_020e7490(player, 2);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_054 &= ~2;
                player->unk_028 = data_ov003_020e71b8->unk3D838;
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e7c14(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                data_ov003_020e71b8->unk3D8E0 = 1;
                player->unk_140               = 8;
                player->unk_054 |= 0x4002;

                s32 val = player->unk_00C;
                if (player->unk_028 > val) {
                    player->unk_024 = 0;
                }
                if (player->unk_028 < val) {
                    player->unk_024 = 1;
                }

                CombatSprite_SetAnimFromTable(&player->unk_07C, 10, 1);
                CombatSprite_Restart(&player->unk_07C);
                func_ov004_020e750c(player, 1);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 11, 0);
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if ((InputStatus.buttonState.pressedButtons & 0xCF3) && !(player->unk_054 & 0x20000) && (func_02023010(689) != 0))
            {
                player->unk_054 |= 0x10;
                player->unk_054 &= 0xFFFFBFFD;
                player->unk_14C = func_ov004_020e7f30;
                player->unk_150 = 0;
                player->unk_152 = 0;
                return;
            }

            if (player->unk_152 == 0 && player->unk_038 >= 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 12, 0);
                player->unk_152++;
            }

            if (player->unk_030 >= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 2: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 0xDU, 1);
                player->unk_054 = (s32)((player->unk_054 | 0x10) & 0xFFFFBFFD);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite) == 0) {
                return;
            }
            player->unk_150++;
            player->unk_152 = 0;
        } break;

        case 3: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 14, 0);
                player->unk_152 = 30;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 4:
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 0xFU, 1);
                func_ov003_02087f28(0x369, ((BtlPlayer01*)data_ov003_020e71b8->unk3D898)->unk_028);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite) == 0) {
                return;
            }
            player->unk_14C = func_ov004_020e7f30;
            player->unk_150 = 0;
            player->unk_152 = 0;
            return;
    }
}

void func_ov004_020e7f30(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                s32 temp_r2 = data_ov003_020e71b8->unk3D838;
                if (player->unk_028 > temp_r2) {
                    player->unk_024 = 0;
                }
                if (player->unk_028 < temp_r2) {
                    player->unk_024 = 1;
                }
                CombatSprite_SetAnimFromTable(&player->unk_07C, 0x10, 1);
                player->unk_054 &= 0xFFFFBFFD;
                func_ov003_02082f7c(player, 0x10);
                player->unk_152 = 0x10;
            }
            if (player->unk_152 > 0) {
                func_02026590(&player->unk_028, data_ov003_020e71b8->unk3D838, (u16)player->unk_152);
                func_02026590(&player->unk_02C, data_ov003_020e71b8->unk3D83C, (u16)player->unk_152);
                player->unk_152--;
            }
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_030 >= 0) {
                player->unk_054 &= ~0x10;
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e808c(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            func_ov003_02087f64(0x85, player->unk_028);
            CombatSprite_SetAnimFromTable(&player->unk_07C, 30, 1);
            CombatSprite_Restart(&player->unk_07C);
            player->unk_150++;
        } break;

        case 1: {
            if (player->unk_040 == 0) {
                player->unk_150++;
            }
        } break;

        case 2: {
            if (player->unk_152 == 0) {
                player->unk_152 = 8;
            }
            if (player->unk_152 > 0) {
                func_02026590(&player->unk_028, data_ov003_020e71b8->unk3D838, (u16)player->unk_152);
                player->unk_152--;
            }
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 3: {
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_028 = (s32)data_ov003_020e71b8->unk3D838;
                player->unk_054 &= ~8;
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e81d0(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 17, 1);
                CombatSprite_Restart(&player->unk_07C);
            }
            player->unk_152 = 0;
            player->unk_150++;
        } break;

        case 1: {
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_152 = 0;
                player->unk_150++;
            } else {
                player->unk_152++;
            }
        } break;

        case 2:
            break;
    }
}

void func_ov004_020e8270(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            data_ov003_020e71b8->unk3D8E0 = 1;
            player->unk_140               = 8;
            player->unk_054 |= 0x10;
            CombatSprite_SetPaletteMode(&player->unk_07C, 2);
            CombatSprite_SetAnimFromTable(&player->unk_07C, 0x24, 1);
            player->unk_038 = -0x2000;
            player->unk_150++;
        } break;

        case 1: {
            if (player->unk_038 > 0) {
                player->unk_038 = 0;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 0, 0);
                CombatSprite_SetPaletteMode(&player->unk_07C, 0);
                player->unk_150++;
            }
        } break;

        case 2: {
            if ((data_ov003_020e71b8->unk3D878 & 8) == 0) {
                player->unk_054 &= ~0x10;
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e8394(BtlPlayer01* player) {
    if (player->unk_028 != data_ov003_020e71b8->unk3D838) {
        player->unk_14C = func_ov004_020e7f30;
        player->unk_150 = 0;
        player->unk_152 = 0;
        return;
    }

    func_ov003_02086ac4(0);
    if (player->unk_150 == 0) {
        player->unk_150++;
    } else if ((player->unk_054 & 0x40000) == 0) {
        func_ov003_02086aa8(1);
    }

    switch (func_ov003_0208765c(&player->unk_13C, data_ov003_020e71b8->unk3D8D4, player->unk_152)) {
        case 1:
            CombatSprite_SetAnimFromTable(&player->unk_07C, 0, 0);
            break;
        case 2:
            CombatSprite_SetAnimFromTable(&player->unk_07C, 1, 0);
            break;
        case 3:
            if (RNG_Next(2) != 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 2, 0);
            } else {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 3, 0);
            }
            break;
    }
    player->unk_152++;
    func_ov004_020e79a8(player, 0, 1);
}

void func_ov004_020e84c4(BtlPlayer01* player) {
    if (player->unk_152 == 0) {
        player->unk_140 = 8;
        player->unk_152 = 20;
    }
    player->unk_152--;
    if (player->unk_152 <= 0) {
        player->unk_14C = func_ov004_020e8394;
        player->unk_150 = 0;
        player->unk_152 = 0;
    }
}

void func_ov004_020e851c(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 19, 1);
                CombatSprite_Restart(&player->unk_07C);
                player->unk_140 = 1;
                func_ov004_020e75d8(player, 0);
                player->unk_13E++;
                player->unk_152 = 10;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            func_ov004_020e79a8(player, 0, 0);
        } break;
    }
}

void func_ov004_020e85d8(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                player->unk_140 = 2;
                player->unk_054 |= 0x10;
                player->unk_054 |= 0x40000;
                CombatSprite_SetAnimFromTable(&player->unk_07C, 20, 1);
                player->unk_152++;
            }
            if (player->unk_140 == 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_152 == 0) {
                player->unk_152 = 20;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 2: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 21, 0);
                func_ov003_02082f7c(player, 46);
                func_ov004_020e75d8(player, 1);
                player->unk_152 = 46;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 3: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 22, 1);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_054 &= ~0x10;
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e8790(BtlPlayer01* player) {
    player->unk_038 = 0;

    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, player->unk_13E % 3 + 23, 1);
                CombatSprite_Restart(&player->unk_07C);
                player->unk_140 = 1;
                func_ov004_020e75d8(player, 0);
                player->unk_13E++;
                player->unk_152 = 10;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            func_ov004_020e79a8(player, 1, 0);
        } break;
    }
}

void func_ov004_020e8878(BtlPlayer01* player) {
    player->unk_038 = 0;

    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                player->unk_140 = 2;
                player->unk_054 |= 0x10;
                player->unk_152++;
            }
            if (player->unk_140 == 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 26, 1);
                player->unk_152 = 20;
            }
            if (player->unk_152 > 0) {
                func_020265d4(&player->unk_030, -0x50000, (u16)player->unk_152);
                player->unk_152--;
            }
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 2: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 27, 1);
                func_ov004_020e75d8(player, 1);
                player->unk_152 = 8;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_054 &= ~0x10;
                player->unk_14C = func_ov004_020e8d00;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e8a00(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                player->unk_140 = 3;
                CombatSprite_SetAnimFromTable(&player->unk_07C, 28, 1);
                CombatSprite_Restart(&player->unk_07C);
                func_ov003_02087f64(0x38C, player->unk_028);
                player->unk_152 = 30;
            }
            player->unk_152--;
            if (player->unk_152 <= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 29, 1);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_140 = 8;
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e8b00(BtlPlayer01* player) {
    if (player->unk_152 == 0) {
        player->unk_054 |= 8;
        CombatSprite_SetAnimFromTable(&player->unk_07C, 30, 1);
        CombatSprite_Restart(&player->unk_07C);
        player->unk_152 = 30;
    }
    player->unk_152--;
    if (player->unk_152 <= 0) {
        player->unk_054 &= ~8;
        player->unk_146 = 5;
        player->unk_14C = func_ov004_020e8394;
        player->unk_150 = 0;
        player->unk_152 = 0;
    }
}

void func_ov004_020e8b94(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                player->unk_140 = 7;
                func_ov003_02086ac4(1);
                CombatSprite_SetAnimFromTable(&player->unk_07C, 31, 1);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 32, 0);
                func_ov003_02082f7c(player, 0x2C);
                player->unk_152++;
            }
            if (player->unk_038 >= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 2: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 33, 0);
                player->unk_152 = 4;
            }
            player->unk_038 = 0;
            player->unk_152--;
            if (player->unk_152 > 0) {
                func_ov004_020e79a8(player, 1, 1);
            } else {
                player->unk_14C = func_ov004_020e8d00;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

void func_ov004_020e8d00(BtlPlayer01* player) {
    switch (player->unk_150) {
        case 0: {
            if (player->unk_152 == 0) {
                data_ov003_020e71b8->unk3D8E0 = 1;
                player->unk_140               = 8;
                func_ov003_02086ac4(0);
                CombatSprite_SetAnimFromTable(&player->unk_07C, 34, 1);
                player->unk_152++;
            }
            if (player->unk_030 >= 0) {
                player->unk_150++;
                player->unk_152 = 0;
            }
        } break;

        case 1: {
            if (player->unk_152 == 0) {
                CombatSprite_SetAnimFromTable(&player->unk_07C, 35, 1);
                player->unk_152++;
            }
            if (SpriteMgr_IsAnimationFinished(&player->unk_07C.sprite)) {
                player->unk_14C = func_ov004_020e8394;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
        } break;
    }
}

s32 BtlPlayer01_Init(TaskPool* pool, Task* task, void* args) {
    BtlPlayer01* player = task->data;

    MI_CpuSet(player, 0, sizeof(BtlPlayer01));
    func_ov003_02083118(player, 0);
    data_ov003_020e71b8->unk3D89C = player;
    player->unk_13C               = 0;
    player->unk_13E               = 0;
    player->unk_146               = 0;
    player->unk_148               = 0;
    player->unk_142               = 0;
    player->unk_144               = 0;
    player->unk_024               = 0;
    player->unk_028               = data_ov003_020e71b8->unk3D838;
    player->unk_02C               = data_ov003_020e71b8->unk3D83C;
    player->unk_030               = 0;
    player->unk_070               = 0xC;
    player->unk_072               = 0x30;
    player->unk_076               = 0x18;
    player->unk_066               = func_ov003_0208ca54(1);
    player->unk_068               = func_ov003_0208cae0(1);
    if (func_ov003_020a3350(1) != 0) {
        player->unk_066 += (s16)func_ov003_020a3488(1);
    }
    if (func_ov003_020a3350(2) != 0) {
        player->unk_068 += (s16)func_ov003_020a3488(2);
    }
    player->unk_004 = player->unk_066;
    player->unk_140 = 0;
    func_ov003_02083ab0(1, player->unk_028, player->unk_02C, player->unk_030);
    player->unk_14C = func_ov004_020e7a24;
    player->unk_150 = 0;
    player->unk_152 = 0;
    if (RNG_Next(2) != 0) {
        CombatSprite_LoadFromTable(1, &player->unk_07C, &data_ov003_020d77e0, data_ov004_020f008c, 2, 34, 100);
    } else {
        CombatSprite_LoadFromTable(1, &player->unk_07C, &data_ov003_020d77e0, data_ov004_020f008c, 3, 34, 100);
    }
    func_ov003_020880e4(1, &player->unk_0DC, player);
    EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlArm_Doll, NULL, 0, NULL, &player->unk_140);
    return 1;
}

s32 BtlPlayer01_Update(TaskPool* pool, Task* task, void* args) {
    BtlPlayer01* player = task->data;

    switch (func_ov003_02082f2c(player)) {
        case 1:
            if (func_ov003_020a3350(55) != 0) {
                player->unk_038 = -0x3800;
                player->unk_14C = func_ov004_020e7c14;
                player->unk_150 = 0;
                player->unk_152 = 0;
            } else {
                player->unk_14C = func_ov004_020e7a84;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
            break;

        case 2:
            if (!(player->unk_054 & 0x20000) && !(player->unk_054 & 0x4000) && (func_ov003_020a3350(54) != 0)) {
                player->unk_038 = 0;
                player->unk_14C = func_ov004_020e7a84;
                player->unk_150 = 0;
                player->unk_152 = 0;
            } else {
                player->unk_14C = func_ov004_020e7c14;
                player->unk_150 = 0;
                player->unk_152 = 0;
            }
            break;

        case 9:
            player->unk_14C = func_ov004_020e808c;
            player->unk_150 = 0;
            player->unk_152 = 0;
            break;

        case 3:
            player->unk_14C = func_ov004_020e81d0;
            player->unk_150 = 0;
            player->unk_152 = 0;
            break;

        case 8:
            player->unk_14C = func_ov004_020e8270;
            player->unk_150 = 0;
            player->unk_152 = 0;
            break;
    }

    if (player->unk_146 > 0) {
        player->unk_146--;
    }

    if (player->unk_14C != NULL) {
        player->unk_14C(player);
    }

    func_ov003_02083000(1, player);
    func_ov003_02083074(player);
    func_ov003_02087458(1, &player->unk_148);
    func_ov003_02087540(1);
    CombatSprite_Update(&player->unk_07C);
    CombatSprite_Update(&player->unk_0DC);
    func_ov004_020e7464(player);
    return 1;
}

s32 BtlPlayer01_Render(TaskPool* pool, Task* task, void* args) {
    BtlPlayer01* player = task->data;

    func_ov003_020831e4(player, &player->unk_07C);
    func_ov003_0208810c(&player->unk_0DC, player);
    return 1;
}

s32 BtlPlayer01_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlPlayer01* player = task->data;

    CombatSprite_Release(&player->unk_07C);
    CombatSprite_Release(&player->unk_0DC);
    return 1;
}

s32 BtlPlayer01_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlPlayer01_Init,
        .update     = BtlPlayer01_Update,
        .render     = BtlPlayer01_Render,
        .cleanup    = BtlPlayer01_Destroy,
    };
    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
