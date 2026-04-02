#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"
#include "Combat/Noise/Private/Boss01.h"

typedef struct BtlObs_Bus {
    /* 0x000 */ char         unk_000[0x2];
    /* 0x002 */ s16          unk_002;
    /* 0x004 */ u32          unk_004;
    /* 0x008 */ char         unk_008[0x2C - 0x8];
    /* 0x02C */ s32          unk_02C;
    /* 0x030 */ s32          unk_030;
    /* 0x034 */ s32          unk_034;
    /* 0x038 */ char         unk_038[0x40 - 0x38];
    /* 0x040 */ s32          unk_040;
    /* 0x044 */ char         unk_044[0x4C - 0x44];
    /* 0x04C */ s32          unk_04C;
    /* 0x050 */ char         unk_050[0x58 - 0x50];
    /* 0x058 */ u32          unk_058;
    /* 0x05C */ char         unk_05C[0x74 - 0x5C];
    /* 0x074 */ s16          unk_074;
    /* 0x076 */ s16          unk_076;
    /* 0x078 */ char         unk_078[0xC4 - 0x78];
    /* 0x0C4 */ CombatSprite unk_0C4;
    /* 0x124 */ CombatSprite unk_124;
    /* 0x184 */ s32          unk_184;
    /* 0x188 */ void (*unk_188)(struct BtlObs_Bus*);
    /* 0x18C */ s16 unk_18C;
    /* 0x18E */ s16 unk_18E;
    /* 0x190 */ u32 unk_190;
    /* 0x194 */ u32 unk_194;
} BtlObs_Bus; // Size: 0x198

typedef struct {
    /* 0x0 */ u32 unk_0;
} BtlObs_Bus_Args;

s32 BtlObs_Bus_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const u16 data_ov016_02129120[] = {1, 3, 2, 0, 4, 6, 5, 0, 7, 9, 8, 0, 0x5000, 0};

static const BinIdentifier Iden_BtlObs_Bus = {3, "Apl_Mor/Grp_Boss01bus.bin"};

static const TaskHandle Tsk_BtlObs_Bus = {"Tsk_BtlObs_Bus", BtlObs_Bus_RunTask, 0x198};

extern s16 func_ov003_020843b0(s32, s32);
extern s16 func_ov003_020843ec(s32, s32, s32);

void BtlObs_Bus_SetState(BtlObs_Bus* arg0, void (*arg1)(BtlObs_Bus*)) {
    arg0->unk_188 = arg1;
    arg0->unk_18C = 0;
    arg0->unk_18E = 0;
}

void BtlObs_Bus_OnPhaseStart(BtlObs_Bus* obs) {
    if ((obs->unk_058 & 0x200) == 0) {
        return;
    }
    if ((*data_ov016_021292c0 == 2) && (obs->unk_124.sprite.resourceData == obs->unk_0C4.sprite.resourceData)) {
        obs->unk_074 = 60;
        obs->unk_076 = 100;
        CombatSprite_SetAnimFromTable(&obs->unk_0C4, 2, 0);
        CombatSprite_SetPaletteSource(&obs->unk_0C4, 11);
        CombatSprite_Release(&obs->unk_124);
        func_ov003_020880e4(0, &obs->unk_124, &obs->unk_004);
    }
}

void BtlObs_Bus_StateIdle(BtlObs_Bus* obs) {
    return;
}

void BtlObs_Bus_StateStop(BtlObs_Bus* obs) {
    obs->unk_194 = 0;
}

void BtlObs_Bus_StateEnd(BtlObs_Bus* obs) {
    return;
}

void BtlObs_Bus_StateHitFlash(BtlObs_Bus* obs) {
    if (obs->unk_18E == 0) {
        obs->unk_058 |= 0x10;
    }

    if (obs->unk_18E < 20) {
        obs->unk_190 = !obs->unk_190;
    } else if (obs->unk_18E % 3 == 0) {
        obs->unk_190 = 1;
    } else {
        obs->unk_190 = 0;
    }

    if (obs->unk_18E > 20) {
        obs->unk_190 = 0;
        obs->unk_194 = 0;
    } else {
        obs->unk_18E++;
    }
}

s32 BtlObs_Bus_Init(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus*      obs     = task->data;
    BtlObs_Bus_Args* busArgs = (BtlObs_Bus_Args*)args;

    MI_CpuSet(obs, 0, sizeof(BtlObs_Bus));
    obs->unk_184 = busArgs->unk_0;
    obs->unk_190 = 1;
    obs->unk_194 = 1;
    func_ov003_02083118((u8*)obs + 4, 2);
    obs->unk_002 = 0x19;
    obs->unk_02C = 0x180000;
    obs->unk_030 = 0x9000;
    obs->unk_034 = 0;
    obs->unk_074 = 0x64;
    obs->unk_076 = 0x64;
    obs->unk_040 = 0xCD;
    obs->unk_04C = 0xE66;
    CombatSprite_LoadFromTable(0, &obs->unk_0C4, &Iden_BtlObs_Bus, (SpriteAnimEntry*)data_ov016_02129120, 0, 10, 0x180);
    CombatSprite_LoadFromTable(0, &obs->unk_124, &Iden_BtlObs_Bus, (SpriteAnimEntry*)data_ov016_02129120, 1, 10, 0);
    CombatSprite_SetAnimFromTable(&obs->unk_0C4, 0, 0);
    CombatSprite_SetAnimFromTable(&obs->unk_124, 1, 0);
    BtlObs_Bus_SetState(obs, BtlObs_Bus_StateIdle);
    return 1;
}

s32 BtlObs_Bus_Update(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus* obs = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 1;
    }
    switch (func_ov003_02082f2c(&obs->unk_004)) {
        case 1:
            BtlObs_Bus_SetState(obs, BtlObs_Bus_StateHitFlash);
            break;
        case 3:
            BtlObs_Bus_SetState(obs, BtlObs_Bus_StateStop);
            break;
        case 5:
            BtlObs_Bus_SetState(obs, BtlObs_Bus_StateEnd);
            break;
    }
    if (obs->unk_188 != NULL) {
        obs->unk_188(obs);
    }
    func_ov003_02083074(&obs->unk_004);
    BtlObs_Bus_OnPhaseStart(obs);
    CombatSprite_Update(&obs->unk_0C4);
    CombatSprite_Update(&obs->unk_124);
    return obs->unk_194;
}

s32 BtlObs_Bus_Render(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus* obs = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 1;
    }
    if (obs->unk_190 == 0) {
        return 1;
    }

    s16 posX   = func_ov003_020843b0(0, obs->unk_02C);
    s16 posY   = func_ov003_020843ec(0, obs->unk_030, obs->unk_034);
    s32 posY32 = posY;
    if (obs->unk_0C4.animTableIndex == 0) {
        s16 posYOff = posY + 8;

        CombatSprite_SetPosition(&obs->unk_124, posX, posYOff);
        func_ov003_02082730(&obs->unk_124, 0x7FFFEFFF);
        CombatSprite_Render(&obs->unk_124);
        CombatSprite_SetPosition(&obs->unk_0C4, posX, posY);
        func_ov003_02082730(&obs->unk_0C4, 0x7FFFEFFF);
        CombatSprite_Render(&obs->unk_0C4);
    } else {
        CombatSprite_SetPosition(&obs->unk_0C4, posX, (s16)posY32);
        func_ov003_02082730(&obs->unk_0C4, 0x7FFFFFFF - obs->unk_030);
        CombatSprite_Render(&obs->unk_0C4);
        func_ov003_0208810c(&obs->unk_124, &obs->unk_004);
    }
    return 1;
}

s32 BtlObs_Bus_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlObs_Bus* obs = task->data;

    CombatSprite_Release(&obs->unk_0C4);
    CombatSprite_Release(&obs->unk_124);
    return 1;
}

s32 BtlObs_Bus_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129110 = {
        .initialize = BtlObs_Bus_Init,
        .update     = BtlObs_Bus_Update,
        .render     = BtlObs_Bus_Render,
        .cleanup    = BtlObs_Bus_Destroy,
    };
    return data_ov016_02129110.iter[stage](pool, task, args);
}

void* BtlObs_Bus_CreateTask(void* bossUG) {
    void* sp8 = bossUG;

    s32 taskId = EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_BtlObs_Bus, NULL, 0, NULL, &sp8);
    return EasyTask_GetTaskData(&data_ov003_020e71b8->unk_00000, taskId);
}
