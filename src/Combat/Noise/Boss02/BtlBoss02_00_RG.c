#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss03.h"
#include "Engine/Math/Random.h"
#include "SpriteMgr.h"
#include "types.h"

typedef struct BtlBoss02_00_RG {
    /* 0x000 */ CombatActor  actor;
    /* 0x07C */ s32          unk_07C;
    /* 0x080 */ s32          unk_080;
    /* 0x084 */ CombatSprite unk_084;
    /* 0x0E4 */ char         unk_0E4[0x18C - 0xE4];
    /* 0x18C */ u16          unk_18C;
    /* 0x18E */ char         unk_18E[0x1AC - 0x18E];
    /* 0x1AC */ s32          unk_1AC;
    /* 0x1B0 */ s32          unk_1B0;
    /* 0x1B4 */ s32          unk_1B4;
    /* 0x1B8 */ char         unk_1B8[0x1C0 - 0x1B8];
    /* 0x1C0 */ s32          unk_1C0;
    /* 0x1C4 */ s32          unk_1C4;
    /* 0x1C8 */ s32          unk_1C8;
    /* 0x1CC */ void (*unk_1CC)(struct BtlBoss02_00_RG*);
    /* 0x1D0 */ s16           unk_1D0;
    /* 0x1D2 */ s16           unk_1D2;
    /* 0x1D4 */ s16           unk_1D4;
    /* 0x1D6 */ s16           unk_1D6;
    /* 0x1D8 */ CombatSprite  unk_1D8;
    /* 0x238 */ CombatSprite* unk_238;
    /* 0x23C */ s32           unk_23C;
    /* 0x240 */ s32           unk_240;
    /* 0x244 */ s32           unk_244;
    /* 0x248 */ s32           unk_248;
    /* 0x24C */ u16           unk_24C;
    /* 0x24E */ u16           unk_24E;
    /* 0x250 */ u32           unk_250;
    /* 0x254 */ u32           unk_254;
    /* 0x258 */ u16           unk_258;
    /* 0x25A */ u16           unk_25A;
    /* 0x25C */ u32           unk_25C;
    /* 0x260 */ u16           unk_260;
    /* 0x262 */ s16           unk_262;
} BtlBoss02_00_RG; // Size: 0x264

void func_ov017_02125c6c(BtlBoss02_00_RG* rg);
void func_ov017_02125e5c(BtlBoss02_00_RG* rg);
void func_ov017_02125fb4(BtlBoss02_00_RG* rg);
s32  BtlBoss02_00_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss02_00_RG = {"Tsk_BtlBoss02_00_RG", BtlBoss02_00_RG_RunTask, 0x264};

const Vec data_ov017_02141538[4] = {
    {-0x50000, 0,        0},
    { 0x50000, 0,        0},
    {-0x50000, 0, -0x41000},
    { 0x50000, 0, -0x41000},
};

extern const BinIdentifier data_ov017_02141520;
extern SpriteAnimEntry     data_ov017_021415a8;
extern s16                 data_ov017_0214398c[];
extern s16                 data_ov017_0214399c[];
extern TaskHandle          data_ov017_02141f30;
extern u16                 data_ov017_02143980;

void BtlBoss02_00_RG_SetState(BtlBoss02_00_RG* rg, void (*callback)(BtlBoss02_00_RG*)) {
    func_ov003_020c427c();
    rg->unk_1CC = callback;
    rg->unk_1C8 = 0;
    rg->unk_1C0 = 0;
}

s16 func_ov017_021256e4(BtlBoss02_00_RG* rg) {
    while (rg->unk_260 == RNG_Next(4)) {
    }
}

void func_ov017_02125708(BtlBoss02_00_RG* rg) {
    rg->unk_1AC = rg->actor.position.x = data_ov003_020e71b8->unk3D838 + data_ov017_02141538[rg->unk_260].x;
    rg->unk_1B0 = rg->actor.position.y = data_ov003_020e71b8->unk3D83C + data_ov017_02141538[rg->unk_260].y;
    rg->unk_1B4 = rg->actor.position.z = data_ov017_02141538[rg->unk_260].z;
}

void func_ov017_02125794(BtlBoss02_00_RG* rg) {
    CombatSprite_LoadFromTable(1, &rg->unk_1D8, &data_ov017_02141520, &data_ov017_021415a8, 0x10, 7, 0x80);
    rg->unk_238 = &rg->unk_1D8;
    rg->unk_23C = data_ov003_020e71b8->unk3D824 / 2;
    rg->unk_240 = data_ov003_020e71b8->unk3D828 / 2;
    rg->unk_244 = 0x40000;
    func_ov003_020c3acc(1, 762, rg->unk_23C);
    rg->unk_248 = 1;
}

void func_ov017_02125840(BtlBoss02_00_RG* rg) {
    if (rg->unk_238 != 0) {
        rg->unk_244 -= 0x8000;
        CombatSprite_Update(&rg->unk_1D8);

        s32 var_r1 = ((BtlBoss02_00_RG*)data_ov003_020e71b8->unk3D89C)->actor.position.z - rg->unk_244;
        if (var_r1 < 0) {
            var_r1 = -var_r1;
        }
        if (rg->unk_248 == 1 && F2I(var_r1) <= 0x10 &&
            func_ov003_0208a1e8(func_ov003_0208a114(240), &rg->unk_1D0, rg->unk_23C) != 0)
        {
            func_ov017_02130178(1, 1256, 1388, rg->actor.position.x);
            rg->unk_248 = 0;
        }
        if (rg->unk_244 <= -0x100000) {
            CombatSprite_Release(&rg->unk_1D8);
            rg->unk_238 = 0;
        }
    }
}

void func_ov017_02125908(BtlBoss02_00_RG* rg) {
    if (rg->unk_238 != 0) {
        CombatSprite_SetPosition(&rg->unk_1D8, func_ov003_020843b0(1, rg->unk_23C),
                                 func_ov003_020843ec(1, rg->unk_240, rg->unk_244));
        func_ov003_02082730(&rg->unk_1D8, 0x7FFFFFFE - rg->unk_240);
        CombatSprite_Render(&rg->unk_1D8);
    }
}

void func_ov017_0212596c(BtlBoss02_00_RG* rg) {
    if (rg->unk_238 != 0) {
        CombatSprite_Release(rg->unk_238);
    }
}

void func_ov017_02125984(BtlBoss02_00_RG* rg) {
    if (rg->unk_25C != 0) {
        s16 temp_r5 = rg->actor.maxHp * 3 / 100;
        func_ov017_02130178(1, 1254, 1388, rg->actor.position.x);
        func_ov003_0208b518(1, rg, temp_r5);
        func_ov003_0209b564(1, temp_r5, rg, 1);
    }
}

void func_ov017_021259fc(BtlBoss02_00_RG* rg) {
    rg->actor.flags &= ~0x40;
    func_ov017_02130178(1, 1255, 1388, rg->actor.position.x);
    func_ov003_0208a128(func_ov003_0208a114(241), &rg->unk_1D0, rg, rg->actor.position.x);
    rg->unk_262 = 120;
    rg->unk_24C = 1;
}

void func_ov017_02125a58(BtlBoss02_00_RG* rg) {
    if (rg->unk_238 == 0) {
        s32 var_r5 = 0;

        while (var_r5 < 1) {
            if (data_ov017_0214398c[var_r5] == 0) {
                func_ov017_02125794(rg);
                data_ov017_0214398c[var_r5] = -1;
                break;
            } else {
                var_r5++;
            }
        }
    } else {
        func_ov017_02125840(rg);
    }

    for (s32 i = 0; i < 3; i++) {
        if (data_ov017_0214399c[i] == 1) {
            func_ov017_02125984(rg);
            data_ov017_0214399c[i] = -1;
        }
    }

    for (s32 i = 0; i < 3; i++) {
        if (data_ov017_0214399c[i] == 2) {
            func_ov017_021259fc(rg);
            data_ov017_0214399c[i] = -1;
        }
    }
}

void func_ov017_02125b30(BtlBoss02_00_RG* rg) {
    if (func_ov003_020c5bfc(rg) == 0) {
        BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
    }
}

// Nonmatching: Stack
void func_ov017_02125b58(BtlBoss02_00_RG* rg) {
    Vec vec;
    s16 sp14;

    if (rg->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&rg->unk_084, 6, 1);
    }
    if (rg->unk_1C0 == 48) {
        vec.x = data_ov003_020e71b8->unk3D838 + data_ov017_02141538[rg->unk_260].x;
        vec.y = data_ov003_020e71b8->unk3D83C + data_ov017_02141538[rg->unk_260].y;
        vec.z = data_ov017_02141538[rg->unk_260].z;
        sp14  = (rg->unk_084.flags46 & 1);
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f30, 0, 0, 0, &vec);
    }
    rg->unk_1C0++;
    if (SpriteMgr_IsAnimationFinished(&rg->unk_084.sprite)) {
        BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
    }
}

void func_ov017_02125c6c(BtlBoss02_00_RG* rg) {
    func_ov003_020c4b1c(rg);
    if (rg->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&rg->unk_084, 0, 0);
        rg->unk_1C4 = func_ov003_020c42ec(rg);
    }
    rg->unk_1C0++;
    if (rg->unk_1C0 >= rg->unk_1C4) {
        if (func_ov003_020c4348(rg) != 0) {
            BtlBoss02_00_RG_SetState(rg, func_ov017_02125b58);
        } else {
            rg->unk_260 = func_ov017_021256e4(rg);
            func_ov017_02125708(rg);
            rg->unk_262 = 36;
            BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
        }
    }
    if (rg->unk_262 > 0) {
        rg->unk_262--;
    }
    if (rg->unk_262 == 1) {
        if (RNG_Next(100) < 50) {
            BtlBoss02_00_RG_SetState(rg, func_ov017_02125b58);
            rg->unk_262 = 0;
        }
        rg->actor.flags |= 0x40;
    }
}

void func_ov017_02125d6c(BtlBoss02_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        rg->unk_084.sprite.animationMode = 2;
        if (rg->unk_250 == 1) {
            func_ov017_02130178(1, 1248, 1249, rg->actor.position.x);
        } else {
            func_ov017_02130178(1, 1250, 1388, rg->actor.position.x);
        }
    }
    rg->unk_1C0++;
    if (rg->unk_24C == 0) {
        if (func_ov003_020c62c4(rg, 3) == 0) {
            BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
            rg->unk_262 = 0;
            rg->actor.flags |= 0x40;
        }
    } else {
        BtlBoss02_00_RG_SetState(rg, func_ov017_02125e5c);
    }
}

void func_ov017_02125e38(BtlBoss02_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        rg->unk_25C = 0;
        rg->unk_1C0++;
    }
}

void func_ov017_02125e5c(BtlBoss02_00_RG* rg) {
    switch (rg->unk_1C8) {
        case 0: {
            CombatSprite_SetAnimFromTable(&rg->unk_084, 3, 1);
            func_ov003_020c4b1c(rg);
            rg->unk_1C8++;
        } break;

        case 1: {
            if (SpriteMgr_IsAnimationFinished(&rg->unk_084.sprite)) {
                CombatSprite_SetAnimFromTable(&rg->unk_084, 4, 0);
                rg->unk_1C8++;
            }
        } break;

        case 2: {
            rg->unk_262--;
            if (rg->unk_262 == 0) {
                CombatSprite_SetAnimFromTable(&rg->unk_084, 5, 1);
                rg->unk_1C8++;
            }
        } break;

        case 3: {
            if (SpriteMgr_IsAnimationFinished(&rg->unk_084.sprite)) {
                BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
                rg->unk_1C8++;
            }
        } break;
    }
}

void func_ov017_02125f48(BtlBoss02_00_RG* rg) {
    rg->unk_260 = func_ov017_021256e4(rg);
    func_ov017_02125708(rg);
    rg->unk_262 = 36;
    rg->actor.flags &= ~0x40;
    rg->actor.flags |= 0x10;
    rg->unk_18C |= 1;
    BtlBoss02_00_RG_SetState(rg, func_ov017_02125fb4);
    func_ov003_020c3acc(1, 0x2FF, rg->actor.position.x);
}

void func_ov017_02125fb4(BtlBoss02_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&rg->unk_084, 0, 0);
    }
    rg->unk_1C0++;
    if (rg->unk_1C0 > 30) {
        func_ov003_020c3acc(1, 0x300, rg->actor.position.x);
        rg->actor.flags &= ~0x10;
        rg->unk_18C &= ~1;
        BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
    }
}

void func_ov017_0212602c(BtlBoss02_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        rg->unk_1C0++;
    }

    void* temp_r0_2 = func_ov003_020c72b4(rg, 0, 3);
    rg->actor.flags &= ~0x40;
    rg->actor.zVelocity = 0;
    rg->actor.zGravity  = 0;
    if (temp_r0_2 == NULL) {
        rg->unk_262 = 10;
        BtlBoss02_00_RG_SetState(rg, func_ov017_02125c6c);
    }
}

// Nonmatching: Instruction reordering
s32 BtlBoss02_00_RG_Init(TaskPool* pool, Task* task, void* args) {
    BtlBoss02_00_RG* rg = task->data;

    MI_CpuSet(rg, 0, sizeof(BtlBoss02_00_RG));
    func_ov003_020c3efc(rg, args);
    data_ov017_02143980 = rg->unk_07C;
    rg->unk_254         = 1;
    rg->unk_25C         = 1;
    rg->unk_262         = 0;
    rg->unk_260         = 0;
    rg->unk_238         = 0;
    rg->unk_24C         = 0;
    rg->unk_1D0         = 100;
    rg->unk_1D4         = 100;
    rg->unk_1D6         = 100;

    rg->unk_1AC = rg->actor.position.x = data_ov003_020e71b8->unk3D838 + data_ov017_02141538[0].x;
    rg->unk_1B0 = rg->actor.position.y = data_ov003_020e71b8->unk3D83C + data_ov017_02141538[0].y;
    rg->unk_1B4 = rg->actor.position.z = data_ov017_02141538[0].z;

    rg->actor.zVelocity = 0;
    rg->actor.zGravity  = 0;
    func_ov003_020c44ac(rg);
    func_ov003_020c4b1c(rg);
    BtlBoss02_00_RG_SetState(rg, func_ov017_02125b30);
    data_ov017_0214398c[0] = -1;

    for (s32 i = 0; i < 3; i++) {
        data_ov017_0214399c[i] = -1;
    }

    rg->actor.flags |= 0x08000040;
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 |= 0x200000;
    func_ov017_02130148();
    return 1;
}

s32 BtlBoss02_00_RG_Update(TaskPool* pool, Task* task, void* args) {
    BtlBoss02_00_RG* rg = task->data;

    if (data_ov003_020e71b8->unk3D8D8 < ((data_ov003_020e71b8->unk3D8DA * 75) / 100) && rg->unk_1CC != func_ov017_02125e38) {
        rg->actor.pendingCommand = 3;
        data_ov003_020e71b8->unk3D878 |= 0x20000000;
        func_ov017_021301e0();
    }

    rg->unk_250 = CombatActor_PopPendingCommand(&rg->actor);
    switch (rg->unk_250) {
        case 1:
        case 2:
            BtlBoss02_00_RG_SetState(rg, func_ov017_02125d6c);
            break;

        case 7:
            BtlBoss02_00_RG_SetState(rg, func_ov017_02125f48);
            break;

        case 3:
            if (rg->unk_18C & 0x10) {
                rg->unk_18C |= 0x20;
            } else {
                BtlBoss02_00_RG_SetState(rg, func_ov017_02125e38);
            }
            break;

        case 6:
            BtlBoss02_00_RG_SetState(rg, func_ov017_0212602c);
            break;

        case 12:
        default:
            break;
    }
    if (rg->unk_1CC != NULL) {
        rg->unk_1CC(rg);
    }
    rg->unk_24C = 0;
    func_ov017_02125a58(rg);
    func_ov003_020c4628(rg);
    func_ov017_0213015c();
    return rg->unk_254;
}

s32 BtlBoss02_00_RG_Render(TaskPool* pool, Task* task, void* args) {
    BtlBoss02_00_RG* rg = task->data;

    func_ov017_02125908(rg);
    func_ov003_020c4878(rg);
    return 1;
}

s32 BtlBoss02_00_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlBoss02_00_RG* rg = task->data;

    func_ov017_0212596c(rg);
    func_ov003_020c48fc(rg);
    return 1;
}

s32 BtlBoss02_00_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlBoss02_00_RG_Init,
        .update     = BtlBoss02_00_RG_Update,
        .render     = BtlBoss02_00_RG_Render,
        .cleanup    = BtlBoss02_00_RG_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}