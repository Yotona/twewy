#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/Boss01.h"

typedef struct BtlEff_Fall {
    /* 0x00 */ char                unk_00[0x20];
    /* 0x20 */ s32                 unk_20;
    /* 0x24 */ char                unk_24[0x28 - 0x24];
    /* 0x28 */ s32                 unk_28;
    /* 0x2C */ s32                 unk_2C;
    /* 0x30 */ s32                 unk_30;
    /* 0x34 */ char                unk_34[0x60 - 0x34];
    /* 0x60 */ void*               unk_60;
    /* 0x64 */ struct BtlEff_Fall* unk_64;
    /* 0x68 */ s32                 unk_68;
    /* 0x6C */ Quad                unk_6C;
    /* 0x74 */ u32                 unk_74;
} BtlEff_Fall; // Size: 0x78;

/* Spawn metadata for effects/obstacles (dimensions and timing). */
typedef struct {
    /* 0x00 */ u16  unk0;
    /* 0x02 */ u16  unk2;
    /* 0x04 */ Quad unk_4;
    /* 0x06 */ u16  unk6;
    /* 0x08 */ u16  unk8;
    /* 0x0A */ u16  unkA;
} Ov016SpawnMeta;

typedef struct {
    u32   unk_0;
    u32   unk_4;
    void* unk_8;
    u32   unk_C;
} BtlEff_Fall_Args;

static const SpriteAnimEntry data_ov016_021290e4[] = {1, 3, 2, 0, 1, 3, 2, 1, 1, 3, 2, 2};

s32 BtlEff_Fall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Task_BtlEff_Fall = {"Task_BtlEff_Fall", BtlEff_Fall_RunTask, 0x78};

// Nonmatching
s32 BtlEff_Fall_Init(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall_Args* spawn_args = args;
    BtlEff_Fall*      temp_r4    = task->data;

    MI_CpuSet(temp_r4, 0, sizeof(BtlEff_Fall));
    temp_r4->unk_60 = spawn_args->unk_0;
    temp_r4->unk_64 = spawn_args->unk_4;
    temp_r4->unk_68 = spawn_args->unk_8;

    Ov016SpawnMeta* temp_r7 = (Ov016SpawnMeta*)spawn_args->unk_8;
    temp_r4->unk_6C         = temp_r7->unk_4;

    temp_r4->unk_74 = spawn_args->unk_C;
    CombatSprite_LoadFromTable(0, (CombatSprite*)temp_r4, &data_ov016_0212909c, data_ov016_021290e4, 0, 7, 0);

    s32 temp_r1 = ((Ov016SpriteParams*)temp_r4->unk_60)->unk2;
    if (temp_r1 <= 14) {
        switch (temp_r1 - 10) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                break;
            default:
                goto escape;
        }
    }
    if (temp_r1 != 25) {
    escape:
        func_ov003_020c38e8(temp_r4);
    }

    func_ov003_02087f28(500, ((Boss01*)temp_r4->unk_64)->unk_28);
    return 1;
}

// Nonmatching
s32 BtlEff_Fall_Update(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall* eff    = task->data;
    s32          var_r5 = 1;

    if (eff->unk_64->unk_30 >= 0) {
        var_r5 = 0;
        if (eff->unk_74 != 0x191) {
            if (eff->unk_74 == 0xE6) {
                BtlEff_Bomb_CreateTaskDefault(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28, eff->unk_64->unk_2C,
                                              eff->unk_64->unk_30);
            } else {
                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30, 0);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 + 0x20000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x10000, 4);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 - 0x28000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x30000, 7);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 + 0x18000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x40000, 14);

                BtlEff_Bomb_CreateTask(eff->unk_74, &eff->unk_6C, eff->unk_64->unk_28 - 0x10000, eff->unk_64->unk_2C,
                                       eff->unk_64->unk_30 - 0x68000, 14);
            }

            if (eff->unk_60 == NULL) {
                func_ov003_02082f1c(eff->unk_64, 3);
            } else if (((Ov016SpriteParams*)eff->unk_60)->unk2 == 0x19) {
                func_ov003_02082f1c((u8*)eff->unk_60 + 4, 1);
            } else {
                func_ov003_02086654(eff->unk_60);
            }
        }
    }
    CombatSprite_Update((CombatSprite*)eff);
    return var_r5;
}

s32 BtlEff_Fall_Render(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall* temp_r4 = task->data;
    s16          spA, sp8;

    func_ov003_02084348(func_ov003_020c3908(temp_r4), &spA, &sp8, temp_r4->unk_64->unk_28, temp_r4->unk_64->unk_2C,
                        temp_r4->unk_64->unk_30);
    CombatSprite_SetPosition((CombatSprite*)temp_r4, spA, sp8);
    func_ov003_02082730((CombatSprite*)temp_r4, 0x7FFFEFFF - temp_r4->unk_64->unk_2C);
    CombatSprite_Render((CombatSprite*)temp_r4);
    return 1;
}

s32 BtlEff_Fall_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlEff_Fall* temp_r4 = task->data;

    if (temp_r4->unk_20 != 0) {
        CombatSprite_Release((CombatSprite*)temp_r4);
    }
    return 1;
}

s32 BtlEff_Fall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_021290d4 = {
        .initialize = BtlEff_Fall_Init,
        .update     = BtlEff_Fall_Update,
        .render     = BtlEff_Fall_Render,
        .cleanup    = BtlEff_Fall_Destroy,
    };
    return data_ov016_021290d4.iter[stage](pool, task, args);
}

void BtlEff_Fall_CreateTask(UnkStruct_Boss01* arg0, Boss01* arg1, s32 arg2) {
    BtlEff_Fall_Args args;
    args.unk_8 = arg1;
    args.unk_C = arg2;
    args.unk_0 = arg0;
    args.unk_4 = (s32)((u8*)arg0 + 4);
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Task_BtlEff_Fall, 0, 0, 0, &args);
}
