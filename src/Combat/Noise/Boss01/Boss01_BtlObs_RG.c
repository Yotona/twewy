#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/Boss01.h"

typedef struct BtlObs_RG {
    /* 0x000 */ char         unk_000[0x28];
    /* 0x028 */ s32          unk_028;
    /* 0x02C */ u32          unk_02C;
    /* 0x030 */ u32          unk_030;
    /* 0x034 */ char         unk_34[0x38 - 0x34];
    /* 0x038 */ s32          unk_038;
    /* 0x03C */ char         unk_03C[0x40 - 0x3C];
    /* 0x040 */ s32          unk_040;
    /* 0x044 */ s32          unk_044;
    /* 0x048 */ char         unk_048[0x54 - 0x48];
    /* 0x054 */ s32          unk_054;
    /* 0x058 */ char         unk_058[0x84 - 0x58];
    /* 0x084 */ CombatSprite unk_084;
    /* 0x0E4 */ char         unk_0E4[0x18C - 0xE4];
    /* 0x18C */ u16          unk_18C;
    /* 0x18E */ char         unk_18E[0x1C0 - 0x18E];
    /* 0x1C0 */ u16          unk_1C0;
    /* 0x1C2 */ u16          unk_1C2;
    /* 0x1C4 */ char         unk_1C4[0x284 - 0x1C4];
    /* 0x284 */ Quad         unk_284;
    /* 0x28C */ void (*unk_28C)(struct BtlObs_RG*); // function pointer for boss sub-state
    /* 0x290 */ u16 unk_290;
    /* 0x292 */ s16 unk_292;
    /* 0x294 */ u32 unk_294;
} BtlObs_RG; // Size: 0x298

typedef struct {
    u16   unused;
    u16   unk_2;
    u32   unk_4;
    u32   unk_8;
    Quad* unk_C;
} BtlObs_RG_Args;

static s32  BtlObs_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static void BtlObs_RG_StateFall(BtlObs_RG* obs);

static const TaskHandle Tsk_BtlObs_RG = {"Tsk_BtlObs_RG", BtlObs_RG_RunTask, sizeof(BtlObs_RG)};

static void BtlObs_RG_SetState(BtlObs_RG* obs, void (*arg1)(BtlObs_RG*)) {
    obs->unk_28C = arg1;
    obs->unk_290 = 0;
    obs->unk_292 = 0;
}

static void BtlObs_RG_StateIdle(BtlObs_RG* obs) {
    return;
}

static void BtlObs_RG_StateHit(BtlObs_RG* obs) {
    obs->unk_054 |= 6;
    func_ov003_020c4a34(obs);

    if (data_ov003_020e71b8->unk3D8A0 == obs) {
        data_ov003_020e71b8->unk3D8A0 = NULL;
    }

    if (obs->unk_18C & 0x1000) {
        obs->unk_294 = 0;
        return;
    }

    if (func_ov009_020fa71c(&obs->unk_1C0) != 0) {
        BtlEff_Bomb_CreateTaskDefault(233, &obs->unk_284, obs->unk_028, obs->unk_02C, obs->unk_030);
        obs->unk_294 = 0;
    } else {
        BtlObs_RG_SetState(obs, BtlObs_RG_StateFall);
    }
}

static void BtlObs_RG_StateFall(BtlObs_RG* obs) {
    BtlObs_RG* temp_r5 = data_ov003_020e71b8->unk3D89C;

    if (obs->unk_292 == 0) {
        if (obs->unk_1C2 == 0) {
            CombatSprite_SetAnimFromTable(&obs->unk_084, 1, 0);
        }

        obs->unk_040 = (obs->unk_028 >= temp_r5->unk_028) ? 0x5000 : -0x5000;
        obs->unk_044 = 0;
        obs->unk_038 = 0;
    }
    if ((obs->unk_030 == 0) && (obs->unk_038 == 0)) {
        obs->unk_294 = 0;
        return;
    }
    obs->unk_292++;
}

static s32 BtlObs_RG_Init(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG_Args* spawnArgs = (BtlObs_RG_Args*)args;
    BtlObs_RG*      obs       = task->data;

    MI_CpuSet(obs, 0, sizeof(BtlObs_RG));

    obs->unk_284 = *spawnArgs->unk_C;

    func_ov003_02086404(1, &obs->unk_084, spawnArgs->unk_2);
    func_ov003_020864dc(obs, spawnArgs->unk_2);
    obs->unk_1C2 = spawnArgs->unk_2;

    if (RNG_Next(2) != 0) {
        CombatSprite_SetFlip(&obs->unk_084, 1);
    }

    func_ov003_020c4988(obs);
    obs->unk_294 = 1;
    BtlObs_RG_SetState(obs, BtlObs_RG_StateIdle);
    return 1;
}

static s32 BtlObs_RG_Update(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG* obs = task->data;

    if (data_ov003_020e71b8->unk3D878 & 2) {
        return 1;
    }

    s32 temp = func_ov003_02082f2c(obs);
    if ((temp == 1) || (temp == 3)) {
        BtlObs_RG_SetState(obs, BtlObs_RG_StateHit);
    }

    if (obs->unk_28C != NULL) {
        obs->unk_28C(obs);
    }

    func_ov003_02083074(obs);
    CombatSprite_Update(&obs->unk_084);
    return obs->unk_294;
}

static s32 BtlObs_RG_Render(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG* obs = task->data;

    if (func_ov003_0208656c() == 0) {
        return 1;
    }
    func_ov003_020831e4(obs, &obs->unk_084);
    return 1;
}

static s32 BtlObs_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlObs_RG* obs = task->data;

    CombatSprite_Release(&obs->unk_084);
    func_ov003_020c4a34(obs);
    if (data_ov003_020e71b8->unk3D8A0 == obs) {
        data_ov003_020e71b8->unk3D8A0 = NULL;
    }
    return 1;
}

static s32 BtlObs_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129148 = {
        .initialize = BtlObs_RG_Init,
        .update     = BtlObs_RG_Update,
        .render     = BtlObs_RG_Render,
        .cleanup    = BtlObs_RG_Destroy,
    };
    return data_ov016_02129148.iter[stage](pool, task, args);
}

s32 BtlObs_RG_CreateTask(u16 arg0, void* arg1) {
    BtlObs_RG_Args args;
    args.unk_C = arg1;
    args.unk_2 = arg0;
    return EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlObs_RG, NULL, 0, NULL, &args);
}