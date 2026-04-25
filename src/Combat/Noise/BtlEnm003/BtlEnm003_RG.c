#include "Combat/Core//CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include <NitroSDK/mi/cpumem.h>

typedef struct BtlEnm003_RG {
    /* 0x000 */ CombatActor          actor;
    /* 0x07C */ char                 unk_07C[0x80 - 0x7C];
    /* 0x080 */ u16                  unk_080;
    /* 0x082 */ char                 unk_082[0x84 - 0x82];
    /* 0x084 */ CombatSprite         sprite;
    /* 0x0E4 */ char                 unk_0E4[0x184 - 0xE4];
    /* 0x184 */ struct BtlEnm003_RG* unk_184;
    /* 0x188 */ char                 unk_188[0x18C - 0x188];
    /* 0x18C */ u16                  unk_18C;
    /* 0x18E */ char                 unk_18E[0x1AC - 0x18E];
    /* 0x1AC */ s32                  unk_1AC;
    /* 0x1B0 */ s32                  unk_1B0;
    /* 0x1B4 */ char                 unk_1B4[0x1C0 - 0x1B4];
    /* 0x1C0 */ s16                  frameCount;    // Number of frames this actor has been in its current state
    /* 0x1C2 */ char                 unk_1C2[0x1C4 - 0x1C2];
    /* 0x1C4 */ u16                  lifetime;      // Total number of frames this actor has been alive for
    /* 0x1C6 */ s16                  stateDuration; // Frames until next state transition
    /* 0x1C8 */ s16                  statePhase;    // Sub-state used within the current state
    /* 0x1CA */ char                 unk_1CA[0x1CC - 0x1CA];
    /* 0x1CC */ void (*unk_1CC)(struct BtlEnm003_RG*);
    /* 0x1D0 */ BOOL isAlive;
    /* 0x1D4 */ s16  unk_1D4;
} BtlEnm003_RG; // Size: 0x1D8

void func_ov009_02120d18(BtlEnm003_RG* rg);
void func_ov009_02120d64(BtlEnm003_RG* rg);
void func_ov009_02120c70(BtlEnm003_RG* rg);
void func_ov009_02120dd0(BtlEnm003_RG* rg);
void func_ov009_02120f64(BtlEnm003_RG* rg);
void func_ov009_021210f8(BtlEnm003_RG* rg);
void func_ov009_021212a8(BtlEnm003_RG* rg);
s32  BtlEnm003_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static void (*data_ov009_021255d8[3])(BtlEnm003_RG*) = {
    func_ov009_02120dd0,
    func_ov009_02120f64,
    func_ov009_021210f8,
};

const TaskHandle Tsk_BtlEnm003_RG = {"Tsk_BtlEnm003_RG", BtlEnm003_RG_RunTask, 0x1D8};

const s32 data_ov009_021247b4[2] = {1966125, 25};
const s32 data_ov009_021247cc[5] = {2621500, 3932160, 40, 1966125, 25};

s16 func_ov009_02120b94(BtlEnm003_RG* rg) {
    s32 var_r0 = func_ov003_020c42ec();

    if (rg->lifetime > 600) {
        var_r0 *= 3;
        var_r0 /= 4;
    } else if (rg->lifetime > 1800) {
        var_r0 /= 2;
    }
    return var_r0;
}

void BtlEnm003_RG_SetState(BtlEnm003_RG* rg, void (*callback)(BtlEnm003_RG*)) {
    func_ov003_020c427c();
    rg->unk_1CC    = callback;
    rg->statePhase = 0;
    rg->frameCount = 0;
    rg->unk_18C &= ~0x80;
}

void func_ov009_02120c18(BtlEnm003_RG* rg) {
    if (rg->unk_184 != NULL && rg->unk_184->unk_18C & 0x80) {
        BtlEnm003_RG_SetState(rg, func_ov009_021210f8);
    }
}

void func_ov009_02120c48(BtlEnm003_RG* rg) {
    if (func_ov003_020c5bfc() == 0) {
        BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
    }
}

void func_ov009_02120c70(BtlEnm003_RG* rg) {
    if (rg->frameCount == 0) {
        CombatSprite_SetAnimFromTable(&rg->sprite, 0, 0);
        rg->stateDuration = func_ov009_02120b94(rg);
    }
    if (rg->frameCount > rg->stateDuration) {
        if (func_ov003_020c4348(rg) != 0) {
            if (func_ov003_020c4e0c(rg) != 0) {
                BtlEnm003_RG_SetState(rg, func_ov009_02120d64);
            }
        } else {
            BtlEnm003_RG_SetState(rg, func_ov009_02120d18);
        }
    } else {
        rg->frameCount++;
    }
    func_ov009_02120c18(rg);
}

void func_ov009_02120d18(BtlEnm003_RG* rg) {
    if (func_ov003_020c5e98() == 0) {
        if (rg->unk_18C & 0x20) {
            BtlEnm003_RG_SetState(rg, func_ov009_021212a8);
        } else {
            BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
        }
    }
}

void func_ov009_02120d64(BtlEnm003_RG* rg) {
    u16 temp_r2 = RNG_Next(100);
    s32 var_r0  = (rg->unk_080 * 6) + (s32)&data_ov009_021247cc;

    if (rg->unk_080 == 1) {
        if (rg->actor.currentHp < (rg->actor.maxHp / 2)) {
            var_r0 = (s32)&data_ov009_021247b4;
        }
    }
    BtlEnm003_RG_SetState(rg, data_ov009_021255d8[func_ov003_020c3b30(var_r0, 3, temp_r2)]);
}

// Nonmatching: regswap
void func_ov009_02120dd0(BtlEnm003_RG* rg) {
    switch (rg->statePhase) {
        case 0: {
            if (func_ov003_020c6230() == 0) {
                rg->frameCount = 0;
                rg->statePhase++;
            }
        } break;

        case 1: {
            if (rg->frameCount == 0) {
                func_ov003_02087f64(415, rg->actor.position.x);
            }
            CombatSprite_SetAnimFromTable(&rg->sprite, 7, 1);
            if (SpriteMgr_IsAnimationFinished(&rg->sprite.sprite) != 0) {
                rg->frameCount = 0;
                rg->statePhase++;
            } else {
                rg->frameCount++;
            }
        } break;

        case 2: {
            if (rg->frameCount == 0) {
                CombatSprite_SetAnimFromTable(&rg->sprite, 8U, 1);
                func_ov009_02120b5c(rg, 0);
                if (func_ov003_020c5b2c(68, rg, rg->actor.position.x, rg->actor.position.y, rg->actor.position.z) == 1) {
                    func_ov003_02087f64(414, rg->actor.position.x);
                }
                func_ov003_02087f64(412, rg->actor.position.x);
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite.sprite) != 0) {
                rg->actor.position.x = rg->unk_1AC;
                rg->actor.position.y = rg->unk_1B0;
                func_ov003_020c4ee0(rg);
                BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
            } else {
                rg->frameCount++;
            }
        } break;
    }
}

// Nonmatching: regswap
void func_ov009_02120f64(BtlEnm003_RG* rg) {
    switch (rg->statePhase) {
        case 0: {
            if (func_ov003_020c6230() == 0) {
                rg->frameCount = 0;
                rg->statePhase++;
            }
        } break;

        case 1: {
            if (rg->frameCount == 0) {
                func_ov003_02087f64(415, rg->actor.position.x);
            }
            CombatSprite_SetAnimFromTable(&rg->sprite, 9, 1);
            if (SpriteMgr_IsAnimationFinished(&rg->sprite.sprite) != 0) {
                rg->frameCount = 0;
                rg->statePhase++;
            } else {
                rg->frameCount++;
            }
        } break;

        case 2: {
            if (rg->frameCount == 0) {
                CombatSprite_SetAnimFromTable(&rg->sprite, 10, 1);
                func_ov009_02120b5c(rg, 1);
                if (func_ov003_020c5b2c(69, rg, rg->actor.position.x, rg->actor.position.y, rg->actor.position.z) == 1) {
                    func_ov003_02087f64(414, rg->actor.position.x);
                }
                func_ov003_02087f64(412, rg->actor.position.x);
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite.sprite) != 0) {
                rg->actor.position.x = rg->unk_1AC;
                rg->actor.position.y = rg->unk_1B0;
                func_ov003_020c4ee0(rg);
                BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
            } else {
                rg->frameCount++;
            }
        } break;
    }
}

void func_ov009_021210f8(BtlEnm003_RG* rg) {
    BtlEnm003_RG* temp_r2 = rg->unk_184;

    switch (rg->statePhase) {
        case 0: {
            if (rg->frameCount == 0) {
                rg->unk_18C |= 0x80;
            }
            if (temp_r2 == NULL || temp_r2->unk_18C & 0x80) {
                rg->frameCount = 0;
                rg->statePhase++;
            } else {
                rg->frameCount++;
            }
        } break;

        case 1: {
            if (rg->frameCount == 0) {
                CombatSprite_SetAnimFromTable(&rg->sprite, 12, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite.sprite) != 0) {
                rg->frameCount = 0;
                rg->statePhase++;
            } else {
                rg->frameCount++;
            }
        } break;

        case 2: {
            if (rg->frameCount == 0) {
                CombatSprite_SetAnimFromTable(&rg->sprite, 13, 1);
                rg->unk_18C &= ~0x80;
                if (rg->unk_1D4 <= 0) {
                    rg->actor.unk_08 *= 2;
                }
                rg->unk_1D4 = ((RNG_Next(6) + 15) * 60);
                func_ov003_02087f64(420, rg->actor.position.x);
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite.sprite) != 0) {
                BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
            } else {
                rg->frameCount++;
            }
        } break;
    }
}

void func_ov009_0212127c(BtlEnm003_RG* rg) {
    if (func_ov003_020c62c4(rg, 2) == 0) {
        BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
    }
}

void func_ov009_021212a8(BtlEnm003_RG* rg) {
    if (func_ov003_020c703c() == 0) {
        rg->isAlive = FALSE;
    }
}

void func_ov009_021212c4(BtlEnm003_RG* rg) {
    if (func_ov003_020c72b4(rg, 0, 5) == 0) {
        BtlEnm003_RG_SetState(rg, func_ov009_02120c70);
    }
}

s32 BtlEnm003_RG_Init(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_RG* rg = task->data;

    MI_CpuSet(rg, 0, sizeof(BtlEnm003_RG));
    func_ov003_020c3efc(rg, args);
    func_ov003_020c44ac(rg);
    func_ov003_020c4b1c(rg);
    rg->actor.flags |= 0x40000000;
    rg->isAlive = TRUE;
    BtlEnm003_RG_SetState(rg, func_ov009_02120c48);
    return 1;
}

s32 BtlEnm003_RG_Update(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_RG* rg = task->data;

    switch (CombatActor_PopPendingCommand(&rg->actor)) {
        case 1:
        case 2:
            BtlEnm003_RG_SetState(rg, func_ov009_0212127c);
            break;

        case 3:
            if (rg->unk_18C & 0x10) {
                rg->unk_18C |= 0x20;
            } else {
                BtlEnm003_RG_SetState(rg, func_ov009_021212a8);
            }
            break;

        case 6:
            BtlEnm003_RG_SetState(rg, func_ov009_021212c4);
            break;
    }
    if (rg->unk_1CC != NULL) {
        rg->unk_1CC(rg);
    }
    func_ov003_020c4628(rg);
    if (rg->unk_1D4 > 0) {
        rg->unk_1D4--;
        if (rg->unk_1D4 == 0) {
            rg->actor.unk_08 /= 2;
        }
    }
    rg->lifetime++;
    return rg->isAlive;
}

s32 BtlEnm003_RG_Render(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_RG* rg = task->data;

    func_ov003_020c4878(rg);
    return 1;
}

s32 BtlEnm003_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlEnm003_RG* rg = task->data;

    func_ov003_020c48fc(rg);
    return 1;
}

s32 BtlEnm003_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlEnm003_RG_Init,
        .update     = BtlEnm003_RG_Update,
        .render     = BtlEnm003_RG_Render,
        .cleanup    = BtlEnm003_RG_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
