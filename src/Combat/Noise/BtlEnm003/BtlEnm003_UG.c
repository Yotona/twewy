#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SpriteMgr.h"

#include <NitroSDK/fx.h>
#include <NitroSDK/mi/cpumem.h>

typedef struct BtlEnm003_UG {
    /* 0x000 */ CombatActor          actor;
    /* 0x07C */ char                 unk_07C[0x80 - 0x7C];
    /* 0x080 */ u16                  unk_080;
    /* 0x082 */ char                 unk_082[0x84 - 0x82];
    /* 0x084 */ CombatSprite         sprite;
    /* 0x0E4 */ char                 unk_0E4[0x144 - 0x0E4];
    /* 0x144 */ s32                  unk_144;
    /* 0x148 */ char                 unk_148[0x184 - 0x148];
    /* 0x184 */ struct BtlEnm003_UG* unk_184;
    /* 0x188 */ u32                  unk_188;
    /* 0x18C */ u16                  unk_18C;
    /* 0x18E */ char                 unk_18E[0x1C0 - 0x18E];
    /* 0x1C0 */ s16                  unk_1C0;
    /* 0x1C2 */ u16                  unk_1C2;
    /* 0x1C4 */ u16                  unk_1C4;
    /* 0x1C6 */ s16                  unk_1C6;
    /* 0x1C8 */ s16                  unk_1C8;
    /* 0x1CA */ char                 unk_1CA[0x1CC - 0x1CA];
    /* 0x1CC */ void (*stateCb)(struct BtlEnm003_UG*);
    /* 0x1D0 */ s32  unk_1D0;
    /* 0x1D4 */ Vec  unk_1D4;
    /* 0x1E0 */ char unk_1E0[0x1E4 - 0x1E0];
    /* 0x1E4 */ s16  unk_1E4;
} BtlEnm003_UG; // Size: 0x1E8

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} Enm3UGUnk;

void func_ov009_02121930(BtlEnm003_UG* ug);
void func_ov009_02121998(BtlEnm003_UG* ug);
void func_ov009_02121afc(BtlEnm003_UG* ug);
void func_ov009_02121ba8(BtlEnm003_UG* ug);
void func_ov009_02121da8(BtlEnm003_UG* ug);
void func_ov009_02121fa4(BtlEnm003_UG* ug);
void func_ov009_021223a0(BtlEnm003_UG* ug);
void func_ov009_021225b0(BtlEnm003_UG* ug);
s32  BtlEnm003_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const Enm3UGUnk data_ov009_021247ec    = {0x1E001E, 0x140014};
const Enm3UGUnk data_ov009_02124804[3] = {
    {0x28003C, 0x000000},
    {0x280028, 0x000014},
    {0x1E001E, 0x140014}
};

void (*data_ov009_021255f8[])(BtlEnm003_UG*) = {
    func_ov009_02121ba8,
    func_ov009_02121da8,
    func_ov009_02121fa4,
    func_ov009_021223a0,
};

const TaskHandle Tsk_BtlEnm003_UG = {"Tsk_BtlEnm003_UG", BtlEnm003_UG_RunTask, 0x1E8};

void func_ov009_021214b8(s32 scalar, Vec* src, Vec* dest) {
    Vec vec;
    vec.x = 0;
    vec.y = 0;
    vec.z = 0;
    Vec_MultAdd(scalar, src, &vec, dest);
}

s16 func_ov009_021214e4(BtlEnm003_UG* ug) {
    u32 var_r0 = func_ov003_020c42ec();

    if (ug->unk_1C4 > 600) {
        var_r0 = (u32)((s32)((var_r0 * 3) + ((u32)((s32)(var_r0 * 3) >> 1) >> 0x1E)) >> 2);
    } else if (ug->unk_1C4 > 1800) {
        var_r0 = (u32)((s32)(var_r0 + (var_r0 >> 0x1F)) >> 1);
    }
    return var_r0;
}

s32 func_ov009_02121534(BtlEnm003_UG* ug, void* arg1) {
    // if ((func_ov003_020c5a60(ug, arg1 + 4) != 0) && (arg1->unk44 == 0) && (arg1->unk48 == 0) && (arg1->unk34 == 0)) {
    //     return 1;
    // }
    // return 0;
}

s32 func_ov009_02121574(BtlEnm003_UG* ug, CombatActor* arg1) {

    if (func_ov009_02121534(ug, arg1) == 0) {
        return 0;
    }

    s32 var_r0;
    s32 var_r1;
    if (ug->actor.isFlipped == 0) {
        var_r1 = ug->actor.position.x;
        var_r0 = arg1->position.y;
    } else {
        var_r1 = arg1->position.y;
        var_r0 = ug->actor.position.x;
    }

    s32 temp_r2 = var_r1 - var_r0;
    s32 var_r3  = (temp_r2 < 0) ? -temp_r2 : temp_r2;
    s32 temp_r3 = arg1->position.z - ug->actor.position.y;
    s32 temp_r5 = (temp_r2 - arg1->unk_74) - ug->actor.unk_70;
    s32 var_r2  = (temp_r3 < 0) ? -temp_r3 : temp_r3;

    s32 temp_r0   = arg1->unk_74 + ((u32)arg1->unk_74 >> 0x1F);
    s32 temp_r1   = ug->actor.unk_70 + ((u32)ug->actor.unk_70 >> 0x1F);
    s32 temp_r0_2 = (temp_r3 - (temp_r0 >> 1)) - (temp_r1 >> 1);
    if ((temp_r5 >= 0) && (temp_r5 <= 0x50000) && ((s32)((var_r2 - (temp_r0 >> 1)) - (temp_r1 >> 1)) <= 0x18000)) {
        return 1;
    }
    if (((s32)((var_r3 - arg1->unk_74) - ug->actor.unk_70) <= 0) && (temp_r0_2 >= 0) && (temp_r0_2 <= 0x18000)) {
        return 1;
    }
    return 0;
}

void* func_ov009_02121638(BtlEnm003_UG* ug) {
    void* var_r4 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8B4);

    while (var_r4 != NULL) {
        if (func_ov009_02121574(ug, var_r4) != 0) {
            return var_r4;
        }
        var_r4 = func_ov003_02084984(var_r4 + 0xB8);
    }
    return NULL;
}

void func_ov009_02121690(BtlEnm003_UG* ug, void* arg1) {
    s32 var_r3 = (ug->actor.isFlipped) ? 0x5000 : -0x5000;

    func_ov003_020c5ac0(ug, 65, arg1 + 4, var_r3, 0, -0x3000);
    ug->unk_1C2 = 0;
}

void func_ov009_021216e0(BtlEnm003_UG* ug) {
    void* var_r4 = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8B4);

    while (var_r4 != NULL) {
        if (func_ov009_02121574(ug, var_r4) != 0) {
            func_ov009_02121690(ug, var_r4);
        }
        var_r4 = func_ov003_02084984(var_r4 + 0xB8);
    }
}

void BtlEnm003_UG_SetState(BtlEnm003_UG* ug, void (*callback)(BtlEnm003_UG*)) {
    func_ov003_020c427c();
    ug->stateCb = callback;
    ug->unk_1C8 = 0;
    ug->unk_1C0 = 0;
    ug->unk_18C &= ~0x80;
}

void func_ov009_02121770(BtlEnm003_UG* ug) {
    if (ug->unk_184 != NULL && (ug->unk_184->unk_18C & 0x80)) {
        BtlEnm003_UG_SetState(ug, func_ov009_021223a0);
    }
}

void func_ov009_021217a0(BtlEnm003_UG* ug) {
    if (func_ov003_020c5bfc() == 0) {
        BtlEnm003_UG_SetState(ug, func_ov009_02121998);
    } else {
        ug->unk_1C0++;
    }
}

void func_ov009_021217dc(BtlEnm003_UG* ug) {
    CombatActor* temp_r4 = data_ov003_020e71b8->unk3D898;
    s16          temp_r5 = RNG_Next(100);

    if (ug->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&ug->sprite, 0, 0);
        func_ov003_02084694(&ug->unk_144, 0);
        ug->unk_1C6 = func_ov009_021214e4(ug);
    }
    if (ug->unk_1C0 > ug->unk_1C6) {
        s32 var_r2 = temp_r4->position.x - ug->actor.position.x;
        s32 var_r0 = (var_r2 < 0) ? -var_r2 : var_r2;

        if (var_r0 < 0x40000) {
            s32 var_r0_2 = temp_r4->position.y - ug->actor.position.y;
            if (var_r0_2 < 0) {
                var_r0_2 = -var_r0_2;
            }
            if (var_r0_2 < 0x20000) {
                BtlEnm003_UG_SetState(ug, func_ov009_02121afc);
            } else {
                goto block_11;
            }
        } else {
        block_11:
            if (ug->unk_080 >= 1) {
                if (var_r2 < 0) {
                    var_r2 = -var_r2;
                }
                if (var_r2 < 0x80000) {
                    s32 var_r0_3 = temp_r4->position.y - ug->actor.position.y;
                    if (var_r0_3 < 0) {
                        var_r0_3 = -var_r0_3;
                    }
                    if ((var_r0_3 < 0x40000) && (temp_r5 < 50)) {
                        BtlEnm003_UG_SetState(ug, func_ov009_02121fa4);
                    } else {
                        goto block_22;
                    }
                } else {
                    goto block_22;
                }
            } else {
            block_22:
                if (temp_r5 < 80) {
                    BtlEnm003_UG_SetState(ug, func_ov009_02121998);
                } else {
                    BtlEnm003_UG_SetState(ug, func_ov009_02121930);
                }
            }
        }
    } else {
        ug->unk_1C0++;
    }
    func_ov009_02121770(ug);
}

void func_ov009_02121930(BtlEnm003_UG* ug) {
    if (ug->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&ug->sprite, 0, 0);
        func_ov003_02084694(&ug->unk_144, 0);
    }
    if (ug->unk_1C0 <= 60) {
        ug->unk_1C0++;
    } else {
        BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
    }
    func_ov009_02121770(ug);
}

void func_ov009_02121998(BtlEnm003_UG* ug) {
    CombatActor* temp_r4 = data_ov003_020e71b8->unk3D898;

    s32 var_r5 = ug->actor.position.x - temp_r4->position.x;
    if (var_r5 < 0) {
        var_r5 = -var_r5;
    }

    s32 var_r6 = ug->actor.position.y - temp_r4->position.y;
    if (var_r6 < 0) {
        var_r6 = -var_r6;
    }

    if (ug->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&ug->sprite, 1, 0);
        func_ov003_02084694(&ug->unk_144, 0);
        ug->unk_1C0++;
    }
    func_ov003_020c4b5c(ug);
    ug->unk_1D4.x = ((u32)(var_r5 << 11) / (var_r5 + var_r6));
    ug->unk_1D4.y = ((u32)(var_r6 << 11) / (var_r5 + var_r6));
    if (temp_r4->position.x < ug->actor.position.x) {
        ug->unk_1D4.x = -ug->unk_1D4.x;
    }
    if (temp_r4->position.y < ug->actor.position.y) {
        ug->unk_1D4.y = -ug->unk_1D4.y;
    }
    ug->actor.position.x += ug->unk_1D4.x;
    ug->actor.position.y += ug->unk_1D4.y;
    if (((u32)var_r5 < 0x40000) && ((u32)var_r6 < 0x20000)) {
        BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
    }
    if (RNG_Next(600) == 0) {
        BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
    }
    if ((func_ov009_02121638(ug) != NULL) && (ug->unk_1C2 >= 300)) {
        BtlEnm003_UG_SetState(ug, func_ov009_02121da8);
    }
    func_ov009_02121770(ug);
}

void func_ov009_02121afc(BtlEnm003_UG* ug) {
    u16        temp_r2 = RNG_Next(100);
    Enm3UGUnk* var_r0  = &data_ov009_02124804[ug->unk_080];

    if ((ug->unk_080 == 1) && ug->actor.currentHp < (ug->actor.maxHp / 2)) {
        var_r0 = &data_ov009_021247ec;
    }

    s32 temp_r0 = func_ov003_020c3b30(var_r0, 4, temp_r2);
    BtlEnm003_UG_SetState(ug, data_ov009_021255f8[temp_r0]);
    if ((ug->unk_1E4 > 0) && (data_ov009_021255f8[temp_r0] == func_ov009_021223a0)) {
        BtlEnm003_UG_SetState(ug, data_ov009_021255f8[RNG_Next(2)]);
    }
}

void func_ov009_02121ba8(BtlEnm003_UG* ug) {
    switch (ug->unk_1C8) {
        case 0: {
            if (ug->unk_1C0 == 0) {
                if (func_ov003_020c4e7c() == 0) {
                    BtlEnm003_UG_SetState(ug, func_ov009_02121998);
                    return;
                }
                CombatSprite_SetAnimFromTable(&ug->sprite, 7, 1);
                func_ov003_02084694(&ug->unk_144, 0);
                func_ov003_020c4b5c(ug);
                func_ov003_02087f28(0x19F, ug->actor.position.x);
            }
            if (ug->unk_1C0 <= 20) {
                ug->unk_1C0++;
            } else {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            }
        } break;

        case 1: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 8, 1);
                func_ov009_02120b5c(ug, 0);
                func_ov003_02084694(&ug->unk_144, 1);
                func_ov003_02087f28(412, ug->actor.position.x);
            }
            ug->unk_1C0++;
            if ((ug->unk_1C0 == 3) &&
                (func_ov003_020c5b2c(63, ug, ug->actor.position.x, ug->actor.position.y, ug->actor.position.z) == 1))
            {
                func_ov003_02087f28(414, ug->actor.position.x);
            }
            if (ug->unk_1C0 <= 30) {
                ug->unk_1C0++;
            } else {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            }
        } break;

        case 2: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 0, 0);
            }
            if (ug->unk_1C0 <= 60) {
                ug->unk_1C0++;
            } else {
                func_ov003_020c4b5c(ug);
                func_ov003_02084694(&ug->unk_144, 0);
                func_ov003_020c4f00(ug);
                BtlEnm003_UG_SetState(ug, func_ov009_02121998);
            }
        } break;
    }
}

void func_ov009_02121da8(BtlEnm003_UG* ug) {
    switch (ug->unk_1C8) {
        case 0: {
            if (ug->unk_1C0 == 0) {
                if (func_ov003_020c4e7c() == 0) {
                    BtlEnm003_UG_SetState(ug, func_ov009_02121998);
                    return;
                }
                CombatSprite_SetAnimFromTable(&ug->sprite, 9, 1);
                func_ov003_02084694(&ug->unk_144, 0);
                func_ov003_020c4b5c(ug);
                func_ov003_02087f28(415, ug->actor.position.x);
            }
            if (ug->unk_1C0 <= 40) {
                ug->unk_1C0++;
            } else {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            }
        } break;

        case 1: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 10, 1);
                func_ov009_02120b5c(ug, 1);
                func_ov003_02087f28(412, ug->actor.position.x);
                func_ov003_02084694(&ug->unk_144, 1);
            }
            if (ug->unk_1C0 == 3) {
                if (func_ov003_020c5b2c(64, ug, ug->actor.position.x, ug->actor.position.y, ug->actor.position.z) == 1) {
                    func_ov003_02087f28(414, ug->actor.position.x);
                }
                func_ov009_021216e0(ug);
            }
            if (ug->unk_1C0 <= 30) {
                ug->unk_1C0++;
            } else {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            }
        } break;

        case 2: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 0, 0);
            }
            if (ug->unk_1C0 <= 120) {
                ug->unk_1C0++;
            } else {
                func_ov003_020c4b5c(ug);
                func_ov003_02084694(&ug->unk_144, 0);
                func_ov003_020c4f00(ug);
                BtlEnm003_UG_SetState(ug, func_ov009_02121998);
            }
        } break;
    }
}

void func_ov009_02121fa4(BtlEnm003_UG* ug) {
    CombatActor* temp_r4 = data_ov003_020e71b8->unk3D898;

    switch (ug->unk_1C8) {
        case 0: {
            if (ug->unk_1C0 == 0) {
                if (func_ov003_020c4e7c() == 0) {
                    BtlEnm003_UG_SetState(ug, func_ov009_02121998);
                    return;
                }
                CombatSprite_SetAnimFromTable(&ug->sprite, 11, 1);
                func_ov003_020c4b5c(ug);
                func_ov003_02084694(&ug->unk_144, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 1: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnim(&ug->sprite, 1, 1);
                func_ov003_020c5b2c(66, ug, ug->actor.position.x, ug->actor.position.y, ug->actor.position.z);
                func_ov003_02087f28(418, ug->actor.position.x);
            }
            if (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 2: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnim(&ug->sprite, 2, 1);
                func_ov003_02087f28(419, ug->actor.position.x);
                ug->unk_1D4.x = temp_r4->position.x - ug->actor.position.x;
                ug->unk_1D4.y = temp_r4->position.y - ug->actor.position.y;
                ug->unk_1D4.z = 0;
                if (Vec_Magnitude(&ug->unk_1D4) > 0x28000) {
                    Vec_Normalize(&ug->unk_1D4, &ug->unk_1D4);
                    func_ov009_021214b8(0x28000, &ug->unk_1D4, &ug->unk_1D4);
                }
            }
            if (ug->unk_1C0 >= 3) {
                ug->actor.position.x += ug->unk_1D4.x / 31;
                ug->actor.position.y += ug->unk_1D4.y / 31;
            }
            if (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 3: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnim(&ug->sprite, 3, 1);
                func_ov009_02120b5c(ug, 2);
                func_ov009_02120b5c(ug, 3);
                func_ov003_02087f28(413, ug->actor.position.x);
                if (func_ov003_020c5b2c(67, ug, ug->actor.position.x, ug->actor.position.y, ug->actor.position.z) == 1) {
                    func_ov003_02087f28(414, ug->actor.position.x);
                }
                ug->unk_1C6 = 70;
            }
            if ((ug->unk_1C0 > ug->unk_1C6) && (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0)) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 4: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnim(&ug->sprite, 4, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 5: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 0, 0);
            }
            if (ug->unk_1C0 <= 60) {
                ug->unk_1C0++;
            } else {
                func_ov003_020c4b5c(ug);
                func_ov003_02084694(&ug->unk_144, 0);
                func_ov003_020c4f00(ug);
                BtlEnm003_UG_SetState(ug, func_ov009_02121998);
            }
        } break;
    }
}

void func_ov009_021223a0(BtlEnm003_UG* ug) {
    BtlEnm003_UG* temp_r2 = ug->unk_184;

    switch (ug->unk_1C8) {
        case 0: {
            if (ug->unk_1C0 == 0) {
                ug->unk_18C |= 0x80;
            }
            if ((temp_r2 == NULL) || (temp_r2->unk_18C & 0x80)) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 1: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 12, 1);
                func_ov003_02084694(&ug->unk_144, 0);
            }
            if (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0) {
                ug->unk_1C0 = 0;
                ug->unk_1C8++;
            } else {
                ug->unk_1C0++;
            }
        } break;

        case 2: {
            if (ug->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&ug->sprite, 13, 1);
                ug->unk_18C &= ~0x80;
                if (ug->unk_1E4 <= 0) {
                    ug->actor.unk_08 *= 2;
                }
                ug->unk_1E4 = RNG_Next(301) + 900;
                func_ov003_02087f28(420, ug->actor.position.x);
            }
            if (SpriteMgr_IsAnimationFinished(&ug->sprite.sprite) != 0) {
                BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
            } else {
                ug->unk_1C0++;
            }
        } break;
    }
}

void func_ov009_0212252c(BtlEnm003_UG* ug) {
    if (func_ov003_020c65cc(ug, 2) == 0) {
        BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
    }
}

void func_ov009_02122558(BtlEnm003_UG* ug) {
    if (func_ov003_020c6b8c(ug, 5) == 0) {
        BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
    }
}

void func_ov009_02122584(BtlEnm003_UG* ug) {
    if (func_ov003_020c6bc8(ug, 5) == 0) {
        BtlEnm003_UG_SetState(ug, func_ov009_021225b0);
    }
}

void func_ov009_021225b0(BtlEnm003_UG* ug) {
    if (func_ov003_020c6c2c(ug, 5) != 0) {
        return;
    }
    BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
}

void func_ov009_021225dc(BtlEnm003_UG* ug) {
    if (func_ov003_020c7070() == 0) {
        ug->unk_1D0 = 0;
    }
}

void func_ov009_021225f8(BtlEnm003_UG* ug) {
    if (func_ov003_020c72b4(ug, 0, 5) == 0) {
        BtlEnm003_UG_SetState(ug, func_ov009_021217dc);
    }
}

s32 BtlEnm003_UG_Init(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_UG* ug = task->data;

    MI_CpuSet(ug, 0, sizeof(BtlEnm003_UG));
    func_ov003_020c3efc(ug, args);
    func_ov003_020c4520(ug);
    func_ov003_020c4b5c(ug);
    ug->actor.flags |= 0x40000000;
    ug->unk_1D0 = 1;
    BtlEnm003_UG_SetState(ug, func_ov009_021217a0);
    return 1;
}

s32 BtlEnm003_UG_Update(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_UG* ug = task->data;

    switch (CombatActor_PopPendingCommand(&ug->actor)) {
        case 1:
            func_ov003_02084694(&ug->unk_144, 0);
            BtlEnm003_UG_SetState(ug, func_ov009_0212252c);
            break;
        case 2:
            func_ov003_02084694(&ug->unk_144, 0);
            BtlEnm003_UG_SetState(ug, func_ov009_02122558);
            break;
        case 3:
            func_ov003_02084694(&ug->unk_144, 1);
            BtlEnm003_UG_SetState(ug, func_ov009_021225dc);
            break;
        case 4:
            func_ov003_02084694(&ug->unk_144, 0);
            BtlEnm003_UG_SetState(ug, func_ov009_02122584);
            break;
        case 5:
            func_ov003_02084694(&ug->unk_144, 0);
            BtlEnm003_UG_SetState(ug, func_ov009_021225b0);
            break;
        case 6:
            func_ov003_02084694(&ug->unk_144, 0);
            BtlEnm003_UG_SetState(ug, func_ov009_021225f8);
            break;
    }

    if (ug->stateCb != NULL) {
        ug->stateCb(ug);
    }

    func_ov003_020c4668(ug);
    if (ug->unk_1E4 > 0) {
        ug->unk_1E4--;
        if (ug->unk_1E4 == 0) {
            ug->actor.unk_08 = ((s32)(ug->actor.unk_08 + ((u32)ug->actor.unk_08 >> 0x1F)) >> 1);
        }
    }
    ug->unk_1C2++;
    ug->unk_1C4++;
    return ug->unk_1D0;
}

s32 BtlEnm003_UG_Render(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_UG* ug = task->data;

    func_ov003_020c48b0(ug);
    return 1;
}

s32 BtlEnm003_UG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_UG* ug = task->data;

    func_ov003_020c492c(ug);
    return 1;
}

s32 BtlEnm003_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlEnm003_UG_Init,
        .update     = BtlEnm003_UG_Update,
        .render     = BtlEnm003_UG_Render,
        .cleanup    = BtlEnm003_UG_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
