#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern s32 func_ov003_0208a114(u16);

extern BinIdentifier data_ov003_020d77e0;

typedef struct {
    /* 0x00 */ char unk_00[0x28];
    /* 0x28 */ s32  unk_28;
    /* 0x2C */ s32  unk_2C;
    /* 0x30 */ s32  unk_30;
    /* 0x34 */ char unk_34[0x76 - 0x34];
    /* 0x76 */ s16  unk_76;
} ArmUnk;

typedef struct BtlArm_Doll {
    /* 0x000 */ u32          unk_000;
    /* 0x004 */ char         unk_004[0x20];
    /* 0x024 */ s32          unk_024;
    /* 0x028 */ s32          unk_028;
    /* 0x02C */ s32          unk_02C;
    /* 0x030 */ s32          unk_030;
    /* 0x034 */ char         unk_034[0x38 - 0x34];
    /* 0x038 */ s32          unk_038;
    /* 0x03C */ s32          unk_03C;
    /* 0x040 */ char         unk_040[0x54 - 0x40];
    /* 0x054 */ s32          unk_054;
    /* 0x058 */ char         unk_058[0x76 - 0x58];
    /* 0x076 */ s16          unk_076;
    /* 0x078 */ char         unk_078[0x7C - 0x78];
    /* 0x07C */ CombatSprite unk_07C;
    /* 0x0DC */ CombatSprite unk_0DC;
    /* 0x13C */ s16          unk_13C;
    /* 0x13E */ char         unk_13E[0x140 - 0x13E];
    /* 0x140 */ s32          unk_140;
    /* 0x144 */ s32          unk_144;
    /* 0x148 */ ArmUnk*      unk_148;
    /* 0x14C */ u16*         unk_14C;
    /* 0x150 */ s16          unk_150;
    /* 0x152 */ char         unk_152[0x154 - 0x152];
    /* 0x154 */ s32          unk_154;
    /* 0x158 */ s32          unk_158;
    /* 0x15C */ s32          unk_15C;
    /* 0x160 */ s32          unk_160;
    /* 0x164 */ s32          unk_164;
    /* 0x168 */ s32          unk_168;
    /* 0x16C */ char         unk_16C[0x174 - 0x16C];
    /* 0x174 */ s32          unk_174;
    /* 0x178 */ void (*unk_178)(struct BtlArm_Doll*);
    /* 0x17C */ s16 unk_17C;
    /* 0x17E */ s16 unk_17E;
} BtlArm_Doll; // Size: 0x180

void func_ov004_020e9754(BtlArm_Doll* doll);
void func_ov004_020e97f8(BtlArm_Doll* doll);
void func_ov004_020e9a34(BtlArm_Doll* doll);
void func_ov004_020e9b54(BtlArm_Doll* doll);
void func_ov004_020e9ce0(BtlArm_Doll* doll);
void func_ov004_020e9eac(BtlArm_Doll* doll);
void func_ov004_020ea1ac(BtlArm_Doll* doll);
void func_ov004_020ea304(BtlArm_Doll* doll);
s32  func_ov004_020ea5cc(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimEntry data_ov004_020f01d0[17] = {
    {0x27, 0x29, 0x28, 0},
    {0x2A, 0x2C, 0x2B, 0},
    {0x2A, 0x2C, 0x2B, 4},
    {0x2A, 0x2C, 0x2B, 1},
    {0x2A, 0x2C, 0x2B, 2},
    {0x2A, 0x2C, 0x2B, 3},
    {0x30, 0x32, 0x31, 0},
    {0x30, 0x32, 0x31, 1},
    {0x30, 0x32, 0x31, 2},
    {0x36, 0x38, 0x37, 0},
    {0x36, 0x38, 0x37, 0},
    {0x3C, 0x3E, 0x3D, 0},
    {0x3C, 0x3E, 0x3D, 1},
    {0x3C, 0x3E, 0x3D, 2},
    {0x3C, 0x3E, 0x3D, 3},
    {0x3C, 0x3E, 0x3D, 4},
    {0x3C, 0x3E, 0x3D, 5},
};

const TaskHandle Tsk_BtlArm_Doll = {"Tsk_BtlArm_Doll", func_ov004_020ea5cc, 0x180};

void func_ov004_020e927c(BtlArm_Doll* doll, void (*callback)(BtlArm_Doll*)) {
    doll->unk_03C = 0x400;
    doll->unk_178 = callback;
    doll->unk_17C = 0;
    doll->unk_17E = 0;
}

void func_ov004_020e929c(BtlArm_Doll* doll) {
    ArmUnk* temp_r2 = data_ov003_020e71b8->unk3D89C;
    if (doll->unk_15C != 0) {
        doll->unk_024 = 0;
        doll->unk_140 = temp_r2->unk_28 + 0x10000;
        doll->unk_144 = temp_r2->unk_2C + 0x8000;
    } else {
        doll->unk_024 = 1;
        doll->unk_140 = temp_r2->unk_28 - 0x10000;
        doll->unk_144 = temp_r2->unk_2C + 0x8000;
    }
}

s32 func_ov004_020e9304(BtlArm_Doll* doll, s32 arg1, s32 arg2) {
    s32 var_r2;
    u16 temp_lr = *doll->unk_14C;

    if (temp_lr != 0) {
        s32 var_r4     = 0;
        *doll->unk_14C = 0;
        if (((BtlArm_Doll*)data_ov003_020e71b8->unk3D89C)->unk_024 == 0) {
            var_r4 = 1;
        }
        doll->unk_15C = var_r4;
        doll->unk_158 = arg1;
        if (arg2 != 0) {
            doll->unk_150 = 0;
        }
    }
    switch (temp_lr) {
        case 7:
            func_ov004_020e927c(doll, func_ov004_020e97f8);
            return 1;
        case 3:
            doll->unk_15C = !doll->unk_15C;
            func_ov004_020e927c(doll, func_ov004_020ea304);
            return 1;
        case 1:
            doll->unk_154 = 0;
            func_ov004_020e927c(doll, func_ov004_020e9a34);
            return 1;
        case 2:
            doll->unk_154 = 1;
            if (doll->unk_158 == 0) {
                func_ov004_020e927c(doll, func_ov004_020e9a34);
            } else {
                func_ov004_020e927c(doll, func_ov004_020e9eac);
            }
            return 1;
        case 8:
            func_ov004_020e927c(doll, func_ov004_020ea1ac);
            return 1;
    }
    return 0;
}

void func_ov004_020e9430(BtlArm_Doll* doll) {
    ArmUnk* var_r0;

    if (doll->unk_158 != 0) {
        var_r0 = func_ov003_0208c810(doll->unk_028, doll->unk_15C, 1);
    } else {
        var_r0 = func_ov003_0208c810(doll->unk_028, doll->unk_15C, 0);
        if (var_r0 == NULL) {
            var_r0 = func_ov003_0208c810(doll->unk_028, doll->unk_15C, 1);
        }
    }
    if (var_r0 != NULL) {
        doll->unk_160 = var_r0->unk_28;
        doll->unk_164 = var_r0->unk_2C;
        doll->unk_168 = (var_r0->unk_30 - (var_r0->unk_76 << 0xC));

        if (doll->unk_15C != 0) {
            doll->unk_160 += 0x10000;
        } else {
            doll->unk_160 -= 0x10000;
        }
    } else {
        doll->unk_160 = ((BtlArm_Doll*)data_ov003_020e71b8->unk3D89C)->unk_028;
        doll->unk_164 = doll->unk_02C;
        doll->unk_168 = doll->unk_030;

        if (doll->unk_15C != 0) {
            doll->unk_160 -= 0x50000;
        } else {
            doll->unk_160 += 0x50000;
        }
    }

    // if (doll->unk_15C == 0) {
    //     doll->unk_160 = var_r1;
    // }
    doll->unk_148                 = var_r0;
    data_ov003_020e71b8->unk3D8A0 = var_r0;
}

void func_ov004_020e951c(BtlArm_Doll* doll) {
    BtlArm_Doll* var_r4;

    s32 var_r5 = 0;
    s32 var_r6 = 0;
    s16 var_r7 = 0;

    var_r4 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    while (var_r4 != NULL) {
        if (func_ov003_0208c79c(var_r4, doll->unk_15C) != 0) {
            var_r5 += var_r4->unk_02C;
            var_r6 += var_r4->unk_030 - (var_r4->unk_076 << 0xC);
            var_r7 += 1;
        }
        var_r4 = func_ov003_02084984(&var_r4->unk_178);
    }

    doll->unk_160 = ((BtlArm_Doll*)data_ov003_020e71b8->unk3D89C)->unk_028;
    if (var_r7 > 0) {
        doll->unk_164 = var_r5 / var_r7;
        doll->unk_168 = var_r6 / var_r7;
    } else {
        doll->unk_164 = doll->unk_02C;
        doll->unk_168 = doll->unk_030;
    }
}

// Nonmatching: "stack" assignment difference
s32 func_ov004_020e95ec(BtlArm_Doll* doll, u16 arg1, s32 arg2, s32 arg3) {
    s32 stack[3];
    s32 var_r5;

    if (doll->unk_024 == 0) {
        var_r5 = doll->unk_028 - (arg2 << 0xC);
    } else {
        var_r5 = doll->unk_028 + (arg2 << 0xC);
    }
    s32 temp_r4 = doll->unk_030 + (arg3 << 0xC);

    *stack = func_ov003_0208a114(arg1);

    if (func_ov003_020a3330(0x21) != 0) {
        stack[2] = (stack[2] & ~0xF80000) | ((u32)(func_ov003_020a3468(0x21) << 0x1B) >> 8);
    }

    if (func_ov003_0208a164(&stack, data_ov003_020e71b8->unk3D89C + 4, var_r5, doll->unk_02C, temp_r4) == 1) {
        return 1;
    }
    return 0;
}

void func_ov004_020e9694(BtlArm_Doll* doll, s32 arg1, s32 arg2) {
    s32 var_r1;

    if (doll->unk_024 == 0) {
        var_r1 = doll->unk_028 - (arg1 << 0xC);
    } else {
        var_r1 = doll->unk_028 + (arg1 << 0xC);
    }
    func_ov003_0209fc88(1, var_r1, doll->unk_02C, doll->unk_030 + (arg2 << 0xC), 0x10, 0);
}

void func_ov004_020e96e0(BtlArm_Doll* doll) {
    doll->unk_15C = (((BtlArm_Doll*)data_ov003_020e71b8->unk3D89C)->unk_024 == 0);

    func_ov004_020e929c(doll);
    doll->unk_028 = doll->unk_140;
    doll->unk_02C = doll->unk_144;
    if (data_ov003_020e71b8->unk3D874 == 2) {
        func_ov004_020e927c(doll, func_ov004_020e9754);
    }
}

void func_ov004_020e9754(BtlArm_Doll* doll) {
    if (doll->unk_17C == 0) {
        BtlArm_Doll* temp_ip = data_ov003_020e71b8->unk3D89C;
        temp_ip->unk_054 &= ~0x40000;

        doll->unk_054 &= ~2;
        *doll->unk_14C = 0;
        CombatSprite_SetAnimFromTable(&doll->unk_07C, 0, 0);
        doll->unk_17C++;
    }
    func_ov004_020e9304(doll, 0, 1);

    BtlArm_Doll* temp_r3 = data_ov003_020e71b8->unk3D89C;
    func_ov003_02083ab0(1, temp_r3->unk_028, temp_r3->unk_02C, temp_r3->unk_030);
}

void func_ov004_020e97f8(BtlArm_Doll* doll) {
    switch (doll->unk_17C) {
        case 0:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 11, 1);
                doll->unk_17E++;
            }
            if (SpriteMgr_IsAnimationFinished(&doll->unk_07C.sprite) != 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            break;
        case 1:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 12, 0);
                func_ov003_02082f7c(doll, 58);
                doll->unk_17E = 29;
            }
            doll->unk_17E--;
            if (doll->unk_17E <= 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            break;
        case 2:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 14, 1);
                doll->unk_17E = 4;
            }
            doll->unk_038 = 0;
            doll->unk_17E--;
            if (doll->unk_17E <= 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            break;
        case 3:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 15, 0);
                doll->unk_17E++;
            }
            if (doll->unk_030 >= 0) {
                doll->unk_17C++;

                doll->unk_17E = 0;
            }
            break;
        case 4:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 16, 1);
                doll->unk_17E++;
            }
            if (SpriteMgr_IsAnimationFinished(&doll->unk_07C.sprite) != 0) {
                func_ov004_020e927c(doll, func_ov004_020e9754);
            }
            break;
    }
    if (doll->unk_17C == 2) {
        func_ov004_020e9304(doll, 1, 1);
    }

    BtlArm_Doll* temp_r3 = data_ov003_020e71b8->unk3D89C;
    func_ov003_02083ab0(1, temp_r3->unk_028, temp_r3->unk_02C, temp_r3->unk_030);
}

void func_ov004_020e9a34(BtlArm_Doll* doll) {
    doll->unk_038 = 0;
    if (doll->unk_17E == 0) {
        CombatSprite_SetAnimFromTable(&doll->unk_07C, 1, 0);
        func_ov004_020e929c(doll);
        func_ov004_020e9430(doll);
        doll->unk_17E = 4;
    }
    if (doll->unk_17E > 0) {
        func_02026590(&doll->unk_028, doll->unk_160, (u16)doll->unk_17E);
        func_02026590(&doll->unk_02C, doll->unk_164, (u16)doll->unk_17E);
        func_02026590(&doll->unk_030, doll->unk_168, (u16)doll->unk_17E);
        doll->unk_17E--;
    }
    if (doll->unk_17E <= 0) {
        if (doll->unk_148 == 0) {
            data_ov003_020e71b8->unk3D8E0 = 1;
            func_ov004_020e927c(doll, func_ov004_020ea1ac);
        } else if (doll->unk_154 != 0) {
            func_ov004_020e927c(doll, func_ov004_020e9ce0);
        } else {
            func_ov004_020e927c(doll, func_ov004_020e9b54);
        }
    }
}

void func_ov004_020e9b54(BtlArm_Doll* doll) {
    s32   var_r4;
    u32   var_r0;
    void* temp_r3_2;

    doll->unk_038 = 0;
    if (doll->unk_17C == 0) {
        u32 temp_r4 = doll->unk_150 % 3;
        CombatSprite_SetAnimFromTable(&doll->unk_07C, temp_r4 + 3, 1);
        CombatSprite_Restart(&doll->unk_07C);
        BtlEff_Doll_CreateTask(temp_r4, doll);
        doll->unk_150++;
        doll->unk_17C++;
    }

    if ((doll->unk_148 != NULL) && (doll->unk_17E == 8)) {
        var_r4 = 0;
        if (func_ov003_0208c79c(doll->unk_148, doll->unk_15C) != 0) {
            if (doll->unk_158 != 0) {
                var_r0 = 16;
            } else {
                var_r0 = 14;
            }
            var_r4 = func_ov003_0208a128(func_ov003_0208a114(var_r0), data_ov003_020e71b8->unk3D89C + 4, doll->unk_148,
                                         doll->unk_028);
            if (var_r4 == 1) {
                func_ov003_02087f64(0x157, doll->unk_148->unk_28);
                func_ov003_02083ab0(1, doll->unk_148->unk_28, doll->unk_148->unk_2C, doll->unk_148->unk_30);
            }
        }
        if (var_r4 != 1) {
            data_ov003_020e71b8->unk3D8E0 = 1;
        }
    }
    if ((doll->unk_17E < 8) || (func_ov004_020e9304(doll, doll->unk_158, 0) == 0)) {
        doll->unk_17E++;
    }
}

void func_ov004_020e9ce0(BtlArm_Doll* doll) {
    if (doll->unk_17C == 0) {
        doll->unk_038 = 0;
    }
    switch (doll->unk_17C) {
        case 0:
            if (doll->unk_17E == 0) {
                func_ov003_02087f64(0x158, doll->unk_028);
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 6, 1);
                doll->unk_150 = 0;
                doll->unk_17E++;
            }
            if (SpriteMgr_IsAnimationFinished(&doll->unk_07C.sprite) != 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            break;

        case 1:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 7, 1);
                BtlEff_Doll_CreateTask(3, doll);
                doll->unk_03C = 0x400;
                func_ov003_02082f7c(doll, 46);
                doll->unk_17E++;
            }
            if (func_ov004_020e95ec(doll, 15, 8, 0) != 0) {
                if (doll->unk_150 == 0) {
                    func_ov004_020e9694(doll, 8, 0);
                }
                doll->unk_150++;
            }
            if (doll->unk_038 >= 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            break;

        case 2:
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 8, 1);
                doll->unk_17E++;
            }
            if (doll->unk_030 >= 0) {
                func_ov004_020e927c(doll, func_ov004_020ea1ac);
            }
            break;
    }
    func_ov003_02083ab0(1, doll->unk_028, doll->unk_02C, doll->unk_030);
}

void func_ov004_020e9eac(BtlArm_Doll* doll) {
    switch (doll->unk_17C) {
        case 0: {
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 2, 0);
                func_ov004_020e951c(doll);
                doll->unk_150 = 0;
                doll->unk_17E = 0x10;
            }
            if (doll->unk_17E > 0) {
                func_020265d4(&doll->unk_028, doll->unk_160, (u16)doll->unk_17E);
                func_020265d4(&doll->unk_02C, doll->unk_164, (u16)doll->unk_17E);
                func_02026590(&doll->unk_030, doll->unk_168, (u16)doll->unk_17E);
                doll->unk_17E--;
            }
            if (doll->unk_17E <= 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            doll->unk_038 = 0;
        } break;

        case 1: {
            if (doll->unk_17E == 0) {
                func_ov003_02087f64(0x161, doll->unk_028);
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 9, 1);
                BtlEff_Doll_CreateTask(4, doll);
                if (doll->unk_024 == 0) {
                    doll->unk_160 = -0x20000;
                } else {
                    doll->unk_160 = data_ov003_020e71b8->unk3D824 + 0x20000;
                }
                doll->unk_17E = 0x10;
            }
            if (doll->unk_17E > 0) {
                func_02026590(&doll->unk_028, doll->unk_160, (u16)doll->unk_17E);
                doll->unk_17E--;
            }
            if (func_ov004_020e95ec(doll, 0x11U, 0, -0x18) != 0) {
                func_ov003_02087f64(0x162, doll->unk_028);
                if (doll->unk_150 == 0) {
                    func_ov004_020e9694(doll, 0x18, -8);
                }
                doll->unk_150++;
            }
            if (doll->unk_17E <= 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
            doll->unk_038 = 0;
            func_ov003_02083ab0(1, doll->unk_028, doll->unk_02C, doll->unk_030);
        } break;

        case 2: {
            if (doll->unk_17E == 0) {
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 2, 0);
                if (doll->unk_024 == 0) {
                    doll->unk_160 += 0x28000;
                } else {
                    doll->unk_160 -= 0x28000;
                }
                func_ov003_02082f7c(doll, 16);
                doll->unk_17E = 16;
            }

            if (doll->unk_17E > 0) {
                func_020265d4(&doll->unk_028, doll->unk_160, (u16)doll->unk_17E);
                doll->unk_17E--;
            }
            if (doll->unk_17E <= 0) {
                func_ov004_020e927c(doll, func_ov004_020ea1ac);
            }
            BtlArm_Doll* temp_r3 = data_ov003_020e71b8->unk3D89C;
            func_ov003_02083ab0(1, temp_r3->unk_028, temp_r3->unk_02C, temp_r3->unk_030);
        } break;
    }
}

void func_ov004_020ea1ac(BtlArm_Doll* doll) {
    if ((doll->unk_17C == 0) && (doll->unk_17E == 0) && (doll->unk_028 == doll->unk_140) && (doll->unk_02C == doll->unk_144)) {
        func_ov004_020e927c(doll, func_ov004_020e9754);
        return;
    }
    switch (doll->unk_17C) {
        case 0: {
            if (doll->unk_17E == 0) {
                doll->unk_054 |= 2;
                CombatSprite_SetAnimFromTable(&doll->unk_07C, 2, 1);
                doll->unk_03C = 0x2800;
                func_ov003_02082f7c(doll, 0xC);
                doll->unk_174 = doll->unk_030;
                doll->unk_030 = 0;
                doll->unk_17E = 0xC;
            }
            if (doll->unk_17E > 0) {
                func_02026590(&doll->unk_028, doll->unk_140, (u16)doll->unk_17E);
                func_02026590(&doll->unk_02C, doll->unk_144, (u16)doll->unk_17E);
                func_02026590(&doll->unk_174, 0, (u16)doll->unk_17E);
                doll->unk_17E--;
            }
            if (doll->unk_17E <= 0) {
                doll->unk_17C++;
                doll->unk_17E = 0;
            }
        } break;

        case 1: {
            if (doll->unk_030 >= 0) {
                func_ov004_020e927c(doll, func_ov004_020e9754);
            }
        } break;
    }
}

void func_ov004_020ea304(BtlArm_Doll* doll) {
    doll->unk_038 = 0;
    if (doll->unk_17E == 0) {
        CombatSprite_SetAnimFromTable(&doll->unk_07C, 9, 0);
        func_ov004_020e929c(doll);
        func_ov004_020e9430(doll);
        doll->unk_17E = 8;
    }
    if (doll->unk_17E > 0) {
        func_020265d4(&doll->unk_028, doll->unk_160, (u16)doll->unk_17E);
        func_020265d4(&doll->unk_02C, doll->unk_164, (u16)doll->unk_17E);
        func_02026590(&doll->unk_030, doll->unk_168, (u16)doll->unk_17E);
        doll->unk_17E--;
    }
    if (doll->unk_17E > 0) {
        return;
    }
    if (doll->unk_148 != NULL) {
        func_ov003_0208a128(func_ov003_0208a114(14), data_ov003_020e71b8->unk3D89C + 4, doll->unk_148, doll->unk_028);
    }
    func_ov004_020e927c(doll, func_ov004_020ea1ac);
}

s32 BtlArm_Doll_Init(TaskPool* pool, Task* task, void* args) {
    BtlArm_Doll* doll    = task->data;
    BtlArm_Doll* temp_r5 = data_ov003_020e71b8->unk3D89C;

    MI_CpuSet(doll, 0, sizeof(BtlArm_Doll));
    func_ov003_02083118(doll, 0);
    doll->unk_14C = args;
    doll->unk_024 = 0;
    doll->unk_028 = (temp_r5->unk_028 + 0x10000);
    doll->unk_02C = (temp_r5->unk_02C + 0x8000);
    doll->unk_030 = 0;
    doll->unk_03C = 0x400;
    CombatSprite_LoadFromTable(1, &doll->unk_07C, &data_ov003_020d77e0, data_ov004_020f01d0, 0U, 63, 20);
    func_ov003_020880e4(1, &doll->unk_0DC, doll);
    doll->unk_140 = doll->unk_028;
    doll->unk_144 = doll->unk_02C;
    doll->unk_174 = 0;
    doll->unk_13C = 0;
    doll->unk_148 = 0;
    doll->unk_15C = 1;
    doll->unk_150 = 0;
    doll->unk_154 = 0;
    doll->unk_158 = 0;
    func_ov004_020e927c(doll, func_ov004_020e96e0);
    return 1;
}

s32 BtlArm_Doll_Update(TaskPool* pool, Task* task, void* args) {
    BtlArm_Doll* doll = task->data;

    if (doll->unk_178 != NULL) {
        doll->unk_178(doll);
    }
    func_ov003_02083074(doll);
    CombatSprite_Update(&doll->unk_07C);
    CombatSprite_Update(&doll->unk_0DC);
    return 1;
}

s32 BtlArm_Doll_Render(TaskPool* pool, Task* task, void* args) {
    BtlArm_Doll* doll = task->data;

    if (data_ov003_020e71b8->unk3D878 & 4) {
        return 1;
    }
    doll->unk_030 += doll->unk_174;
    func_ov003_020831e4(doll, &doll->unk_07C);
    func_ov003_0208810c(&doll->unk_0DC, doll);
    doll->unk_030 -= doll->unk_174;
    return 1;
}

s32 BtlArm_Doll_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlArm_Doll* doll = task->data;

    CombatSprite_Release(&doll->unk_07C);
    CombatSprite_Release(&doll->unk_0DC);
    return 1;
}

s32 func_ov004_020ea5cc(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlArm_Doll_Init,
        .update     = BtlArm_Doll_Update,
        .render     = BtlArm_Doll_Render,
        .cleanup    = BtlArm_Doll_Destroy,
    };
    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
