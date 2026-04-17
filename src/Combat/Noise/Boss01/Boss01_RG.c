#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"
#include "Combat/Noise/Private/Boss01.h"
#include "EasyFade.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Math/Random.h"
#include <NitroSDK/fx.h>

typedef struct Boss01_RG {
    /* 0x000 */ CombatActor actor;
    // /* 0x000 */ char         unk_000[0x24];
    // /* 0x024 */ s32          unk_24;
    // /* 0x028 */ s32          unk_28;
    // /* 0x02C */ s32          unk_2C;
    // /* 0x030 */ s32          unk_30;
    // /* 0x034 */ s16          unk_34;
    // /* 0x036 */ s16          unk_36;
    // /* 0x038 */ char         unk_38[0x3C - 0x38];
    // /* 0x03C */ s32          unk_3C;
    // /* 0x040 */ char         unk_40[0x54 - 0x40];
    // /* 0x054 */ s32          actor.flags;
    // /* 0x058 */ char         unk_58[0x7C - 0x58];
    /* 0x07C */ s32          unk_7C;
    /* 0x080 */ char         unk_80[0x84 - 0x80];
    /* 0x084 */ CombatSprite unk_84;
    /* 0x0E4 */ char         unk_E4[0x178 - 0xE4];
    /* 0x178 */ s32          unk_178;
    /* 0x17C */ s32          unk_17C;
    /* 0x180 */ s32          unk_180;
    /* 0x184 */ void*        unk_184;
    /* 0x188 */ void*        unk_188;
    /* 0x18C */ u16          unk_18C;
    /* 0x18E */ char         unk_18E[0x1C0 - 0x18E];
    /* 0x1C0 */ s16          unk_1C0;
    /* 0x1C2 */ char         unk_1C2[0x1C8 - 0x1C2];
    /* 0x1C8 */ s16          unk_1C8;
    /* 0x1CA */ char         unk_1CA[0x1CC - 0x1CA];
    /* 0x1CC */ void (*unk_1CC)(struct Boss01_RG*);
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
} Boss01_RG; // Size: 0x1E0

s32 Boss01_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

#define OV003_LIST_3D8AC ((void*)((u8*)data_ov003_020e71b8 + 0x3D8AC))

static s32 data_ov016_021292c0[8];

const TaskHandle Tsk_Boss01_RG = {"Tsk_Boss01_RG", Boss01_RG_RunTask, sizeof(Boss01_RG)};

void Boss01_MoveState_FlyAcross(Boss01_RG* rg);

// Nonmatching: One branch points to wrong address
s32 Boss01_RG_IsInBounds(Boss01_RG* rg) {
    BOOL var_r3 = TRUE;
    BOOL var_ip = (rg->actor.isFlipped == 0) && (rg->actor.position.x < -0x80000);

    if (var_ip == FALSE) {
        var_ip = FALSE;
        if ((rg->actor.isFlipped != 0) && (rg->actor.position.x > (data_ov003_020e71b8->unk3D824 + 0x80000))) {
            var_ip = TRUE;
        }
    }

    if (var_ip == FALSE) {
        var_r3 = FALSE;
    }

    return var_r3;
}

void func_ov016_02125bb8(Boss01_RG* rg) {
    rg->unk_1D4 = (rg->actor.isFlipped != 0) ? 0x4000 : -0x4000;
}

void Boss01_RG_SetState(Boss01_RG* rg, void (*callback)(Boss01_RG*)) {
    func_ov003_020c427c();
    rg->unk_1CC = callback;
    rg->unk_1C8 = 0;
    rg->unk_1C0 = 0;
}

void Boss01_MoveState_WaitToFly(Boss01_RG* rg) {
    if (*data_ov016_021292c0 == 5) {
        Boss01_RG_SetState(rg, Boss01_MoveState_FlyAcross);
    }
}

void func_ov016_02125c20(Boss01_RG* rg, s32 arg1) {
    if ((rg->unk_1D8 != 0 && arg1 != 0) || (rg->unk_1D8 == 0 && arg1 == 0)) {
        return;
    }
    rg->unk_1D8 = arg1;

    Boss01_RG* var_r4 = func_ov003_0208495c(OV003_LIST_3D8AC);
    while (var_r4 != NULL) {
        switch (var_r4->unk_7C) {
            case 7:
                if (arg1 != 0) {
                    var_r4->actor.flags |= 0x30;
                } else {
                    var_r4->actor.flags &= ~0x10;
                    var_r4->actor.flags &= ~0x20;
                }
                break;

            case 34:
                if (arg1 == 0) {
                    var_r4->actor.flags |= 0x30;
                } else {
                    var_r4->actor.flags &= ~0x10;
                    var_r4->actor.flags &= ~0x20;
                }
                break;

            default:
                if (arg1 != 0) {
                    data_ov003_020e71b8->unk3D8A0 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, rg->unk_1DC);
                    var_r4->actor.flags &= ~0x10;
                    var_r4->actor.flags &= ~0x20;
                } else {
                    var_r4->actor.flags |= 0x30;
                }
                break;
        }
        var_r4 = func_ov003_02084984(&var_r4->unk_178);
    }
}

void Boss01_MoveState_FlyAcross(Boss01_RG* rg) {
    Boss01_RG* temp_r4 = data_ov003_020e71b8->unk3D89C;
    Boss01*    temp_r6 = rg->unk_184;
    Boss01*    temp_r5 = temp_r6->unk_188;

    if (rg->unk_1C0 == 0) {
        if (temp_r6->unk_28 < 0) {
            rg->actor.position.x = -0x80000;
            rg->unk_1D4          = 0x4000;
        } else {
            rg->actor.position.x = (data_ov003_020e71b8->unk3D824 + 0x80000);
            rg->unk_1D4          = -0x4000;
        }

        rg->actor.position.y = temp_r4->actor.position.y;
        rg->actor.position.z = -0x40000;
        func_ov003_020c4c1c(rg);
        func_ov003_020c4988(rg);
        if (temp_r5 != NULL) {
            rg->unk_1DC  = BtlObs_RG_CreateTask(temp_r5->unk_002, (u8*)data_ov003_020e71b8->unk3D89C + 4);
            s32 var_r0_2 = I2F_RND(temp_r5->unk_76);
            if (var_r0_2 > 0x80000) {
                f32 var_r0_3         = I2F_RND(temp_r5->unk_76);
                rg->actor.position.z = ((-(s32)var_r0_3) - 0x40000);
            }

            s32 temp_r2 = temp_r5->unk_76 * (temp_r5->unk_74 << 0xD);
            s32 temp_r1 = (s32)(temp_r2 + ((u32)(temp_r2 >> 5) >> 0x1A)) >> 6;
            s32 var_r1  = (s32)(temp_r1 + ((u32)(temp_r1 >> 4) >> 0x1B)) >> 5;
            if (var_r1 > 0x2000) {
                var_r1 = 0x2000;
            }
            if (rg->actor.isFlipped == 0) {
                rg->unk_1D4 += var_r1;
            } else {
                rg->unk_1D4 -= var_r1;
            }
        }
    }
    rg->actor.position.x += rg->unk_1D4;
    s32 var_r0_4 = temp_r4->actor.position.x - rg->actor.position.x;
    if (var_r0_4 < 0) {
        var_r0_4 = -var_r0_4;
    }
    if (var_r0_4 < 0x80000) {
        func_ov016_02125c20(rg, 1);
    } else {
        func_ov016_02125c20(rg, 0);
    }
    if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, &rg->unk_1DC) != 0) {
        Boss01* temp_r0_2 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, rg->unk_1DC);
        if (!(temp_r0_2->unk_54 & 2)) {
            temp_r0_2->unk_28 = rg->actor.position.x;
            temp_r0_2->unk_2C = rg->actor.position.y;

            s32 var_r0_5      = I2F_RND(temp_r0_2->unk_72);
            temp_r0_2->unk_30 = rg->actor.position.z + var_r0_5;
        } else {
            func_ov016_02125bb8(rg);
        }
    } else {
        func_ov016_02125bb8(rg);
    }

    s32 var_r1_2 = rg->unk_1D4;
    s32 var_r0_6 = temp_r4->actor.position.x - rg->actor.position.x;
    if (var_r0_6 < 0) {
        var_r0_6 = -var_r0_6;
    }
    if (var_r1_2 < 0) {
        var_r1_2 = -var_r1_2;
    }
    if (var_r0_6 < var_r1_2) {
        func_ov003_020c5b0c(232, rg);
    }
    Boss01_Eff_CreateTask((Boss01*)rg);
    if (Boss01_RG_IsInBounds(rg) != 0) {
        func_ov003_020c4a34(rg);
        if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, &rg->unk_1DC) != 0) {
            Boss01* temp_r0_5 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, rg->unk_1DC);
            if (temp_r0_5->unk_54 & 2) {
                rg->unk_188 = NULL;
            } else {
                temp_r0_5->unk_18C |= 0x1000;
                func_ov003_020c4ff4();
            }
        } else {
            rg->unk_188 = NULL;
        }

        rg->actor.position.x = (rg->actor.position.x < 0) ? -0x80000 : (data_ov003_020e71b8->unk3D7CC + 0x80000);

        *data_ov016_021292c0 = 2;
        Boss01_RG_SetState(rg, Boss01_MoveState_WaitToFly);
        return;
    }
    rg->unk_1C0++;
}

void func_ov016_021260b4(Boss01_RG* bossRG) {
    if (func_ov003_020c6f40() == 0) {
        bossRG->unk_1D0 = 0;
    }
}

s32 Boss01_RG_Init(TaskPool* pool, Task* task, void* args) {
    SpriteAnimationEx anim;
    Boss01_RG*        bossRG = task->data;

    MI_CpuSet(bossRG, 0, sizeof(Boss01_RG));
    func_ov003_020c3efc(bossRG, args);
    func_ov016_021256c0(&anim, 1, 0);
    CombatSprite_Load(&bossRG->unk_84, &anim);
    func_ov003_020c4c1c(bossRG);
    bossRG->actor.flags |= 0x10000000;
    bossRG->actor.zGravity   = 0;
    bossRG->actor.position.x = -0x80000;
    bossRG->actor.position.y = 0;
    bossRG->actor.position.z = -0x40000;
    bossRG->unk_1DC          = -1;
    bossRG->unk_1D0          = 1;
    Boss01_RG_SetState(bossRG, Boss01_MoveState_WaitToFly);
    return 1;
}

s32 Boss01_RG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01_RG* bossRG = task->data;

    if (func_ov003_020c3bf0(bossRG) != 0) {
        return 1;
    }

    s32 temp_r0 = CombatActor_PopPendingCommand(&bossRG->actor);
    if ((temp_r0 != 1) && (temp_r0 == 3)) {
        Boss01_RG_SetState(bossRG, func_ov016_021260b4);
    }
    if (bossRG->unk_1CC != NULL) {
        bossRG->unk_1CC(bossRG);
    }
    CombatActor_UpdateEffects(1, &bossRG->actor);
    CombatSprite_Update(&bossRG->unk_84);
    return bossRG->unk_1D0;
}

s32 Boss01_RG_Render(TaskPool* pool, Task* task, void* args) {
    s16        spA, sp8;
    Boss01_RG* bossRG = task->data;

    if (!(bossRG->unk_18C & 1)) {
        func_ov003_02084348(1, &spA, &sp8, bossRG->actor.position.x, bossRG->actor.position.y, bossRG->actor.position.z);
        CombatSprite_SetPosition(&bossRG->unk_84, spA, sp8);
        func_ov003_02082730(&bossRG->unk_84, 0x7FF3FFFF);
        CombatSprite_Render(&bossRG->unk_84);
        bossRG->actor.screenX = spA;
        bossRG->actor.screenY = sp8 + 80;
    }
    return 1;
}

s32 Boss01_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01_RG* bossRG = task->data;

    CombatSprite_Release(&bossRG->unk_84);
    func_ov003_020c4a34(bossRG);
    return 1;
}

s32 Boss01_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02128fc8 = {
        .initialize = Boss01_RG_Init,
        .update     = Boss01_RG_Update,
        .render     = Boss01_RG_Render,
        .cleanup    = Boss01_RG_Destroy,
    };
    return data_ov016_02128fc8.iter[stage](pool, task, args);
}
