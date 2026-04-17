#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"
#include "Combat/Noise/Private/Boss01.h"
#include "EasyFade.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Math/Random.h"
#include <NitroSDK/fx.h>

/// MARK: Boss01_UG

typedef struct Boss01_UG {
    /* 0x000 */ CombatActor  actor;
    /* 0x07C */ char         unk_7C[0x84 - 0x7C];
    /* 0x084 */ CombatSprite sprite;
    /* 0x0E4 */ char         unk_E4[0x188 - 0xE4];
    /* 0x188 */ void*        unk_188;
    /* 0x18C */ u16          unk_18C;
    /* 0x18E */ char         unk_18E[0x1C0 - 0x18E];
    /* 0x1C0 */ s16          unk_1C0;
    /* 0x1C2 */ char         unk_1C2[0x1C4 - 0x1C2];
    /* 0x1C4 */ s16          unk_1C4;
    /* 0x1C6 */ s16          unk_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct Boss01_UG*);
    /* 0x1CC */ s32               unk_1CC;
    /* 0x1D0 */ s32               unk_1D0;
    /* 0x1D4 */ s32               unk_1D4;
    /* 0x1D8 */ PaletteResource*  unk_1D8;
    /* 0x1DC */ s32               unk_1DC;
    /* 0x1E0 */ struct Boss01_UG* unk_1E0;
    /* 0x1E4 */ void*             unk_1E4;
    /* 0x1E8 */ void*             unk_1E8;
    /* 0x1EC */ s32               unk_1EC;
    /* 0x1F0 */ s32               unk_1F0;
} Boss01_UG; // Size: 0x1F4

void func_ov016_021266f4(Boss01_UG* bossUG);
void func_ov016_02126c24(Boss01_UG* bossUG);
void func_ov016_02126854(Boss01_UG* bossUG);
void func_ov016_02126d38(Boss01_UG* bossUG);
s32  Boss01_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern void* data_ov009_02123540;
extern void* data_ov009_021234cc;
static s32   data_ov016_021291d8 = 1;

#define OV003_LIST_3D8B4 ((void*)((u8*)data_ov003_020e71b8 + 0x3D8B4))

const TaskHandle Tsk_Boss01_UG = {"Tsk_Boss01_UG", Boss01_UG_RunTask, 0x1F4};

void func_ov016_021262d8(Boss01_UG* bossUG) {
    if (func_ov003_02088130() == 0) {
        return;
    }

    Data* temp_r0 = data_ov003_020e71b8->unk3D350;
    void* var_r8  = Data_GetPackEntryData(temp_r0, 66);

    PaletteResource* temp_r4 = bossUG->unk_1D8;

    u16 var_r0;
    if (bossUG->sprite.flags46 & 1) {
        var_r0 = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_EXTENDED], 0, bossUG->unk_1DC, bossUG->unk_1DC, 1);
    } else {
        var_r0 = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_EXTENDED], 0, bossUG->unk_1DC, bossUG->unk_1DC, 0);
    }

    bossUG->sprite.sprite.unk_0A.raw  = (u16)((bossUG->sprite.sprite.unk_0A.raw & ~0x3E0) | ((u32)(var_r0 << 0x1B) >> 0x16));
    s16               temp_r7         = bossUG->sprite.sprite.scaleY;
    PaletteResource*  temp_r6         = bossUG->sprite.sprite.paletteData;
    UnkSmallInternal* temp_r5         = bossUG->sprite.sprite.unk3C;
    u32               temp_r0_2       = -temp_r7;
    bossUG->sprite.sprite.paletteData = temp_r4;
    bossUG->sprite.sprite.unk3C       = var_r8;
    bossUG->sprite.sprite.scaleY      = (s16)((s32)(-temp_r7 + (-temp_r7 >> 0x1F)) >> 1);
    bossUG->sprite.sprite.unk_0A.raw  = (u16)((bossUG->sprite.sprite.unk_0A.raw & ~1) | 1);
    bossUG->sprite.sprite.unk_0A.raw &= ~2;
    bossUG->sprite.sprite.unk_0A.raw = (u16)((bossUG->sprite.sprite.unk_0A.raw & ~0xC) | 4);
    CombatSprite_RenderWithPalette(&bossUG->sprite, 8);
    bossUG->sprite.sprite.paletteData   = temp_r6;
    bossUG->sprite.sprite.unk3C         = temp_r5;
    bossUG->sprite.sprite.scaleY        = temp_r7;
    bossUG->sprite.sprite.unk_0A.unk_00 = 0;
    bossUG->sprite.sprite.unk_0A.unk_01 = 0;
    bossUG->sprite.sprite.unk_0A.unk_02 = 0;

    if (bossUG->sprite.flags46 & 1) {
        bossUG->sprite.sprite.unk_0A.raw =
            (u16)((bossUG->sprite.sprite.unk_0A.raw & ~0x3E0) |
                  ((u32)((u16)((bossUG->sprite.sprite.unk_0A.raw << 0x16 >> 0x1B) | 8) << 0x1B) >> 0x16));
    } else {
        bossUG->sprite.sprite.unk_0A.raw =
            (u16)((bossUG->sprite.sprite.unk_0A.raw & ~0x3E0) |
                  ((u32)((u16)((bossUG->sprite.sprite.unk_0A.raw << 0x16 >> 0x1B) & ~8) << 0x1B) >> 0x16));
    }
}

s32 func_ov016_02126468(Boss01_UG* bossUG) {
    s32 temp_lr;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_r3;

    temp_lr = bossUG->actor.isFlipped;
    var_r3  = 1;
    var_ip  = 0;
    if ((temp_lr == 0) && ((s32)bossUG->actor.position.x < -0x80000)) {
        var_ip = 1;
    }
    if (var_ip == 0) {
        var_ip_2 = 0;
        if ((temp_lr != 0) && ((s32)bossUG->actor.position.x > (s32)(data_ov003_020e71b8->unk3D7CC + 0x80000))) {
            var_ip_2 = 1;
        }
        if (var_ip_2 == 0) {
            var_r3 = 0;
        }
    }
    return var_r3;
}

void func_ov016_021264d8(void*) {
    u8 subroutine[0x14];

    func_ov003_020c3c44(subroutine, 7, 3);
    func_ov003_020c3cec(&data_ov009_02123540, &data_ov009_021234cc, subroutine, subroutine);
}

s32 func_ov016_02126514(Boss01_UG* bossUG, Boss01_UG* arg1) {
    if ((func_ov003_020c5a28(bossUG, (u8*)arg1 + 4) != 0) && (arg1->actor.yVelocity == 0) &&
        (arg1->actor.linearDampingStep == 0) && (arg1->actor.screenX == 0))
    {
        return 1;
    }
    return 0;
}

s32 func_ov016_02126554(Boss01_UG* bossUG, Boss01_UG* arg1, s32 arg2) {
    if ((arg2 != 0) && (func_ov009_020fa71c(arg1) == 0)) {
        return 0;
    }
    if ((arg2 == 0) && (func_ov009_020fa71c(arg1) != 0)) {
        return 0;
    }
    return func_ov016_02126514(bossUG, arg1);
}

Boss01_UG* func_ov016_021265a8(Boss01_UG* bossUG, s32 arg1) {
    Boss01_UG* var_r4  = NULL;
    s32        temp_r5 = RNG_Next(func_ov003_020cc5fc(0, 0x19));
    Boss01_UG* var_r8  = func_ov003_0208495c(OV003_LIST_3D8B4);

    for (s32 var_r9 = 0; var_r8 != NULL; var_r9++) {
        if (func_ov016_02126554(bossUG, var_r8, arg1) != 0) {
            if (var_r9 < temp_r5) {
                var_r4 = var_r8;
                break;
            }
        }
        var_r8 = func_ov003_02084984((u8*)var_r8 + 0xB8);
    }
    return var_r4;
}

Boss01_UG* func_ov016_02126638(Boss01_UG* bossUG) {
    s16        temp_r1_2;
    Boss01_UG* temp_r1;
    Boss01_UG* temp_r4;
    Boss01_UG* var_r0;

    temp_r4   = func_ov016_021265a8(bossUG, 0);
    var_r0    = func_ov016_021265a8(bossUG, 1);
    temp_r1_2 = bossUG->actor.maxHp;
    if ((s32)bossUG->actor.currentHp <= (s32)((s32)(temp_r1_2 + ((u32)temp_r1_2 >> 0x1F)) >> 1)) {
        temp_r1 = bossUG->unk_1E4;
        if (temp_r1 != NULL) {
            return temp_r1;
        }
    }
    if (var_r0 == NULL) {
        var_r0 = temp_r4;
    }
    return var_r0;
}

void Boss01_UG_SetState(Boss01_UG* bossUG, void (*arg1)(Boss01_UG*)) {
    func_ov003_020c427c();
    bossUG->unk_1C8 = arg1;
    bossUG->unk_1C4 = 0;
    bossUG->unk_1C0 = 0;
    bossUG->unk_1EC = 0;
}

void func_ov016_021266b4(Boss01_UG* bossUG) {
    if (func_ov003_020cb3c4(0, 7) == 0) {
        *data_ov016_021292c0 = 1;
        Boss01_UG_SetState(bossUG, func_ov016_021266f4);
    }
}

void func_ov016_021266f4(Boss01_UG* bossUG) {
    switch (bossUG->unk_1C4) {
        case 0: {
            if (bossUG->unk_1C0 <= 180) {
                bossUG->unk_1C0++;
            } else {
                bossUG->unk_1C0 = 0;
                bossUG->unk_1C4++;
            }
        } break;

        case 1: {
            bossUG->unk_1C4++;
        } break;

        case 2: {
            Boss01_UG_SetState(bossUG, func_ov016_02126c24);
        } break;

        case 3: {
            EasyFade_FadeBothDisplays(FADER_LINEAR, -16, 0x400);
            if (EasyFade_IsFading() == FALSE) {
                bossUG->unk_1C4++;
            }
        } break;

        case 4: {
            if (bossUG->unk_1C0 == 0) {
                EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x400);
                func_ov003_020c3ec0(bossUG);
                Boss01s_LandRG_CreateTask();
                Boss01s_LandUG_CreateTask();
                Boss01s_FlyRG_Curve_CreateTask();
                Boss01s_FlyUG_CreateTask();
                for (s32 i = 0; i < 4; i++) {
                    func_ov016_021264d8(bossUG);
                }
            }
            if (EasyFade_IsFading() == FALSE) {
                *data_ov016_021292c0 = 2;
                Boss01_UG_SetState(bossUG, func_ov016_02126854);
            } else {
                bossUG->unk_1C0++;
            }
        } break;
    }
}

void func_ov016_0212682c(Boss01_UG* bossUG) {
    if (*data_ov016_021292c0 == 2) {
        Boss01_UG_SetState(bossUG, func_ov016_02126854);
    }
}

void func_ov016_02126854(Boss01_UG* bossUG) {
    s32        sp8;
    s16        temp_r1_2;
    s32        temp_r3;
    s32        temp_r3_2;
    u32        temp_r1;
    Boss01_UG* temp_r4 = data_ov003_020e71b8->unk3D898;

    if (bossUG->unk_1C0 == 0) {
        func_ov003_020c4c5c(bossUG);
        bossUG->actor.flags |= 0x10;
        bossUG->unk_1DC          = 0x666;
        bossUG->actor.position.y = temp_r4->actor.position.y;
        bossUG->actor.position.z = -0x200000;
        bossUG->unk_1D0          = (bossUG->actor.isFlipped != 0) ? 0x3000 : -0x3000;

        if ((bossUG->unk_188 == 0) && (bossUG->unk_1E0 != NULL)) {
            func_ov003_02086654();
            bossUG->unk_1E8 = 0;
            bossUG->unk_1E0 = NULL;
        }
        if (bossUG->unk_1E8 != 0) {
            bossUG->unk_1EC = (RNG_Next(2) == 0) ? 1 : 0;
            if (bossUG->unk_1EC != 0) {
                temp_r3         = FX_Divide(0x2800, bossUG->unk_1E0->actor.dampingScale) >> 0xC;
                temp_r1         = temp_r3 * (bossUG->unk_1E0->actor.dampingScale * temp_r3);
                bossUG->unk_1F0 = (s32)((temp_r3 * 0x2800) - ((s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1));
            }
        }
    }
    bossUG->actor.position.x += bossUG->unk_1D0;
    if (bossUG->unk_1E8 != 0) {
        sp8       = bossUG->actor.position.x;
        temp_r1_2 = bossUG->unk_1E0->actor.unk_74;
        func_ov003_02083bfc((u8*)bossUG + 0x2C, ((s32)(temp_r1_2 + ((u32)temp_r1_2 >> 0x1F)) >> 1) << 0xC,
                            bossUG->actor.position.x);
        bossUG->unk_1E0->actor.position.y = bossUG->actor.position.x;
        bossUG->unk_1E0->actor.position.z = bossUG->actor.position.y;
        bossUG->unk_1E0->actor.screenX    = bossUG->actor.position.z;
        if ((bossUG->unk_1EC != 0) &&
            (((temp_r3_2 = bossUG->unk_1D0, (temp_r3_2 > 0)) &&
              ((s32)bossUG->actor.position.x > (s32)(temp_r4->actor.position.x - bossUG->unk_1F0))) ||
             ((temp_r3_2 < 0) && ((s32)bossUG->actor.position.x < (s32)(temp_r4->actor.position.x + bossUG->unk_1F0)))) &&
            (func_ov003_020c3960(&sp8, bossUG->unk_1E0->actor.unk_74 << 0xC) == 0))
        {
            s32 var_r3 = (bossUG->actor.isFlipped != 0) ? 0x2800 : -0x2800;
            func_ov003_020c5ac0(bossUG, 0xE5, (u8*)bossUG->unk_1E0 + 4, var_r3);

            if (bossUG->unk_1E0 == bossUG->unk_1E4) {
                BtlEff_Fall_CreateTask((UnkStruct_Boss01*)bossUG->unk_1E0, (Boss01*)bossUG, 231);
                bossUG->unk_1E0->actor.screenX = -0x120000;
                bossUG->unk_1E4                = 0;
            } else {
                if (func_ov009_020fa71c((UnkStruct_Boss01*)bossUG->unk_1E0) != 0) {
                    BtlEff_Fall_CreateTask((UnkStruct_Boss01*)bossUG->unk_1E0, (Boss01*)bossUG, 230);
                } else {
                    BtlEff_Fall_CreateTask((UnkStruct_Boss01*)bossUG->unk_1E0, (Boss01*)bossUG, 401);
                }
            }
            bossUG->unk_1E0 = NULL;
            bossUG->unk_188 = 0;
            bossUG->unk_1E8 = 0;
        }
    }
    if (func_ov016_02126468(bossUG) != 0) {
        Boss01_UG* temp_r0_3 = func_ov016_02126638(bossUG);
        if (bossUG->unk_1E8 != 0) {
            *data_ov016_021292c0 = 2;
            Boss01_UG_SetState(bossUG, func_ov016_02126854);
            return;
        }
        if ((temp_r0_3 == bossUG->unk_1E4) && (temp_r0_3 != NULL)) {
            if ((s32)bossUG->actor.position.x < 0) {
                *data_ov016_021292c0 = 2;
                Boss01_UG_SetState(bossUG, func_ov016_02126854);
                return;
            }
            *data_ov016_021292c0 = 3;
            Boss01_UG_SetState(bossUG, func_ov016_02126c24);
            return;
        }
        if (func_ov003_0208495c(OV003_LIST_3D8B4) != 0) {
            *data_ov016_021292c0 = 3;
            Boss01_UG_SetState(bossUG, func_ov016_02126c24);
            return;
        }
        if (RNG_Next(2) != 0) {
            *data_ov016_021292c0 = 2;
            Boss01_UG_SetState(bossUG, func_ov016_02126854);
            return;
        }
        *data_ov016_021292c0 = 3;
        Boss01_UG_SetState(bossUG, func_ov016_02126c24);
        return;
    }
    bossUG->unk_1C0++;
}

void func_ov016_02126c24(Boss01_UG* bossUG) {
    Boss01* temp_r5 = data_ov003_020e71b8->unk3D898;

    if (bossUG->unk_1C0 == 0) {
        func_ov003_020c4c5c(bossUG);
        bossUG->actor.flags      = (s32)(bossUG->actor.flags | 0x10);
        bossUG->unk_1DC          = 0xCCD;
        bossUG->actor.position.y = (s32)temp_r5->unk_2C;
        bossUG->actor.position.z = -0x12C000;
        if (bossUG->actor.isFlipped == 0) {
            bossUG->unk_1D0 = -0x4000;
        } else {
            bossUG->unk_1D0 = 0x4000;
        }
        if (*data_ov016_021292c0 == 1) {
            func_ov003_020c4cc4(bossUG, 498);
        }
    }
    bossUG->actor.position.x += bossUG->unk_1D0;
    if (func_ov016_02126468(bossUG) != 0) {
        if (*data_ov016_021292c0 == 1) {
            Boss01_UG_SetState(bossUG, func_ov016_021266f4);
            bossUG->unk_1C4 = 3;
            return;
        }
        *data_ov016_021292c0 = 4;
        Boss01_UG_SetState(bossUG, func_ov016_02126d38);
        return;
    }
    bossUG->unk_1C0++;
}

void func_ov016_02126d38(Boss01_UG* bossUG) {
    UnkStruct_Boss01* temp_r4 = data_ov003_020e71b8->unk3D898;

    if (bossUG->unk_1C0 == 0) {
        bossUG->unk_1E0 = func_ov016_02126638(bossUG);
        func_ov003_020c4c5c(bossUG);
        bossUG->actor.flags &= ~0x10;
        bossUG->unk_1DC = 0x1000;
        bossUG->unk_1D4 = 0;
        if (bossUG->unk_1E0 != NULL) {
            bossUG->actor.position.y = (s32)bossUG->unk_1E0->actor.position.z;
            s32 temp_r0_3            = bossUG->unk_1E0->actor.unk_76 << 0xC;

            f32 var_r0;
            if ((s32)bossUG->unk_1E0->actor.unk_76 > 0) {
                var_r0 = (f32)temp_r0_3 + 0.5f;
            } else {
                var_r0 = (f32)temp_r0_3 - 0.5f;
            }
            bossUG->actor.position.z = -(s32)var_r0 - 0x8000;
        } else {
            bossUG->actor.position.y = (s32)temp_r4->unk_2C;
            bossUG->actor.position.z = -0x40000;
        }

        bossUG->unk_1D0 = (bossUG->actor.isFlipped != 0) ? 0x4000 : -0x4000;

        func_ov003_020c4cc4(bossUG, 0x1F2);
    }
    bossUG->actor.position.x += bossUG->unk_1D0;
    if (bossUG->unk_1E0 != NULL) {
        if (bossUG->unk_1E8 != 0) {
            bossUG->unk_1E0->actor.position.y = (s32)bossUG->actor.position.x;
            bossUG->unk_1E0->actor.position.z = (s32)bossUG->actor.position.y;
            bossUG->unk_1E0->actor.screenX    = -0x8000;
        }

        if ((bossUG->actor.position.x >= (s32)(bossUG->unk_1E0->actor.position.y - 0x4000)) &&
            (bossUG->actor.position.x < (s32)(bossUG->unk_1E0->actor.position.y + 0x4000)) &&
            (func_ov016_02126514(bossUG, bossUG->unk_1E0) != 0))
        {
            func_ov003_020c5a00((u8*)bossUG->unk_1E0 + 4);
            bossUG->unk_1E0->actor.periodicEffectMode = 6;
            bossUG->unk_1E8                           = 1;
            bossUG->unk_188                           = (void*)bossUG->unk_1E0;
            func_ov003_020c4cc4(bossUG, 0x1F3);

            s32 temp_r2_3 = bossUG->unk_1E0->actor.unk_76 * (bossUG->unk_1E0->actor.unk_74 << 0xD);
            s32 temp_r1_2 = (s32)(temp_r2_3 + ((u32)(temp_r2_3 >> 5) >> 0x1A)) >> 6;
            s32 var_r1    = (s32)(temp_r1_2 + ((u32)(temp_r1_2 >> 4) >> 0x1B)) >> 5;

            if (var_r1 > 0x2000) {
                var_r1 = 0x2000;
            }
            if (bossUG->actor.isFlipped == 0) {
                bossUG->unk_1D0 += var_r1;
            } else {
                bossUG->unk_1D0 -= var_r1;
            }
        }
    }
    if (bossUG->unk_1D4 == 0) {
        bossUG->unk_1D4 =
            func_ov003_020c5b2c(0xE4, bossUG, bossUG->actor.position.x, bossUG->actor.position.y, bossUG->actor.position.z);
    }
    Boss01_Eff_CreateTask((Boss01*)bossUG);
    if (func_ov016_02126468(bossUG) != 0) {
        bossUG->actor.flags |= 0x10;
        if (bossUG->unk_1E8 == 0) {
            bossUG->unk_1E0 = NULL;
        }
        if ((bossUG->unk_1E4 != 0) && (bossUG->unk_1E0 == bossUG->unk_1E4)) {
            *data_ov016_021292c0 = 2;
            Boss01_UG_SetState(bossUG, func_ov016_0212682c);
            return;
        }
        *data_ov016_021292c0 = 5;
        Boss01_UG_SetState(bossUG, func_ov016_0212682c);
        return;
    }
    bossUG->unk_1C0++;
}

void func_ov016_02126ff0(Boss01_UG* bossUG) {
    SpriteAnimationEx subroutine;

    if (bossUG->unk_1C0 == 0) {
        func_ov016_021256c0(&subroutine, 0, bossUG->sprite.flags46 & 1);
        func_ov003_020c9064(&subroutine);
    }
    s32 temp_r5 = func_ov003_020c9338(bossUG);
    if ((func_ov003_020c9008() != 0) && (data_ov016_021291d8 != 0)) {
        CombatSprite_Release(&bossUG->sprite);
        data_ov016_021291d8 = 0;
    }
    if (temp_r5 == 0) {
        func_ov003_020c50bc();
    }
    if ((temp_r5 == 0) && (EasyFade_IsFading() == 0)) {
        bossUG->unk_1CC = NULL;
        return;
    }
    bossUG->unk_1C0++;
}

s32 Boss01_UG_Init(TaskPool* pool, Task* task, void* args) {
    SpriteAnimationEx subroutine;
    Boss01_UG*        bossUG = task->data;

    MI_CpuSet(bossUG, 0, 0x1F4);

    void* src       = Data_GetPackEntryData(data_ov003_020e71b8->unk3D350, 66);
    bossUG->unk_1D8 = PaletteMgr_AcquireContiguous(g_PaletteManagers[DISPLAY_EXTENDED], src, 5, 1);

    func_ov003_020c3efc(bossUG, args);
    func_ov016_021256c0(&subroutine, 2, 0);
    CombatSprite_Load(&bossUG->sprite, &subroutine);
    func_ov003_020c49c8(bossUG);
    func_ov003_020c4c5c(bossUG);
    bossUG->actor.flags |= 0x10000010;
    bossUG->actor.zGravity   = 0;
    bossUG->actor.position.x = -0x80000;
    bossUG->actor.position.y = 0;
    bossUG->actor.position.z = -0x40000;
    bossUG->unk_1DC          = 0x1000;
    bossUG->unk_1CC          = 1;
    bossUG->unk_1E4          = BtlObs_Bus_CreateTask(bossUG);
    *data_ov016_021292c0     = 0;
    Boss01_UG_SetState(bossUG, func_ov016_021266b4);
    return 1;
}

s32 Boss01_UG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01_UG* bossUG = task->data;

    if (func_ov003_020c3bf0(bossUG) != 0) {
        return 1;
    }

    s32 temp_r0 = CombatActor_PopPendingCommand(&bossUG->actor);
    if ((temp_r0 != 1) && (temp_r0 == 3)) {
        Boss01_UG_SetState(bossUG, func_ov016_02126ff0);
    }

    if (bossUG->unk_1C8 != NULL) {
        bossUG->unk_1C8(bossUG);
    }

    if (((s32)*data_ov016_021292c0 > 1) && (func_ov003_020cb3c4(0, 7) < 4) && (RNG_Next(180) == 0) &&
        !(bossUG->actor.flags & 4))
    {
        func_ov016_021264d8(bossUG);
    }
    CombatActor_UpdateEffects(0, &bossUG->actor);
    CombatSprite_Update(&bossUG->sprite);
    return bossUG->unk_1CC;
}

s32 Boss01_UG_Render(TaskPool* pool, Task* task, void* args) {
    s16        spA, sp8;
    Boss01_UG* bossUG = task->data;

    if (bossUG->actor.flags & 1) {
        bossUG->actor.flags &= ~1;
    }

    if (!(bossUG->unk_18C & 1)) {
        func_ov003_02084348(0, &spA, &sp8, bossUG->actor.position.x, bossUG->actor.position.y, bossUG->actor.position.z);
        CombatSprite_SetPosition(&bossUG->sprite, spA, sp8);
        CombatSprite_Render(&bossUG->sprite);
        bossUG->actor.screenX = spA;
        bossUG->actor.screenY = sp8 + 80;
        func_ov003_02084348(0, &spA, &sp8, bossUG->actor.position.x, bossUG->actor.position.y, 0);
        CombatSprite_SetPosition(&bossUG->sprite, spA, sp8);
        func_ov016_021262d8(bossUG);
    }
    return 1;
}

s32 Boss01_UG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01_UG* bossUG = task->data;

    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_EXTENDED], bossUG->unk_1D8);
    CombatSprite_Release(&bossUG->sprite);
    func_ov003_020c4a74(bossUG);
    return 1;
}

s32 Boss01_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02128fe4 = {
        .initialize = Boss01_UG_Init,
        .update     = Boss01_UG_Update,
        .render     = Boss01_UG_Render,
        .cleanup    = Boss01_UG_Destroy,
    };
    return data_ov016_02128fe4.iter[stage](pool, task, args);
}
