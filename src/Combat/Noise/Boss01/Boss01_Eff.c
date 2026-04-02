#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/Boss01.h"
#include "EasyFade.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Math/Random.h"
#include <NitroSDK/fx.h>

/// MARK: Boss01_Eff

typedef struct {
    /* 0x0 */ Vec unk_0;
    /* 0xC */ s16 unk_C;
    /* 0xE */ s16 unk_E;
} Boss01_Eff_Args;

typedef struct {
    /* 0x00 */ CombatSprite    sprite;
    /* 0x60 */ Boss01_Eff_Args unk_60;
    /* 0x70 */ s32             unk_70;
    /* 0x74 */ s32             unk_74;
    /* 0x78 */ s32             unk_78;
    /* 0x7C */ s32             unk_7C;
} Boss01_Eff; // Size: 0x80

extern const BinIdentifier data_ov016_02128f88;
extern const BinIdentifier data_ov016_02128f90;

s32 Boss01_Eff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle            Tsk_Boss01_Eff         = {"Tsk_Boss01_Eff", Boss01_Eff_RunTask, 0x80};
static const BinIdentifier* data_ov016_021291ac[3] = {&data_ov016_02128f88, &data_ov016_02128f88, &data_ov016_02128f90};

s32 Boss01_Eff_Init(TaskPool* pool, Task* task, void* args) {
    Boss01_Eff*      eff     = task->data;
    Boss01_Eff_Args* effArgs = args;

    MI_CpuSet(eff, 0, 0x80);

    eff->unk_60 = *effArgs;

    CombatSprite_LoadDirect(eff->unk_60.unk_C, &eff->sprite, data_ov016_021291ac[eff->unk_60.unk_C], 1, 3, 2, 4,
                            (RNG_Next(5) + 1));
    CombatSprite_SetFlip(&eff->sprite, RNG_Next(2));
    eff->unk_7C = (RNG_Next(1) + 0x1666);
    eff->unk_78 = -(RNG_Next(0x219B) + 0x666);
    return 1;
}

// Nonmatching: eff->unk_70 access differs
s32 Boss01_Eff_Update(TaskPool* pool, Task* task, void* args) {
    Boss01_Eff* eff = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    switch (eff->unk_74) {
        case 0:
            eff->unk_60.unk_0.x += eff->unk_7C;
            eff->unk_60.unk_0.z += eff->unk_78;
            eff->unk_78 += 286;

            if (eff->unk_78 > 737) {
                eff->unk_70++;
            } else {
                eff->unk_70 = 0;
                eff->unk_74++;
            }

        case 1:
            eff->unk_60.unk_0.x += eff->unk_7C;
            eff->unk_60.unk_0.z += eff->unk_78;

            eff->unk_78 -= 205;
            if (eff->unk_78 < 0) {
                eff->unk_78 = (RNG_Next(0xB35) + 0xCCC);
                if (eff->unk_7C > 0) {
                    eff->unk_7C = -(RNG_Next(0x1001) + 0x800);
                } else {
                    eff->unk_7C = (RNG_Next(0x1001) + 0x800);
                }
            }
            if (eff->unk_60.unk_0.z >= 0) {
                return 0;
            }

        default:
            CombatSprite_Update(&eff->sprite);
            return 1;
    }
}

s32 Boss01_Eff_Render(TaskPool* pool, Task* task, void* args) {
    s16         spA, sp8;
    Boss01_Eff* eff = task->data;

    func_ov003_02084348(func_ov003_020c3908(eff), &spA, &sp8, eff->unk_60.unk_0.x, eff->unk_60.unk_0.y, eff->unk_60.unk_0.z);
    CombatSprite_SetPosition(&eff->sprite, spA, sp8);
    CombatSprite_Render(&eff->sprite);
    return 1;
}

s32 Boss01_Eff_Destroy(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

s32 Boss01_Eff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02128fac = {
        .initialize = Boss01_Eff_Init,
        .update     = Boss01_Eff_Update,
        .render     = Boss01_Eff_Render,
        .cleanup    = Boss01_Eff_Destroy,
    };
    return data_ov016_02128fac.iter[stage](pool, task, args);
}

void Boss01_Eff_CreateTask(Boss01* boss) {
    Boss01_Eff_Args args;

    if (boss->unk_1A4 > boss->unk_1A6) {
        u32 temp_r4 = (u32)(boss->unk_84 << 0x1E) >> 0x1E;
        if (boss->unk_24 == 0) {
            args.unk_0.x = boss->unk_28 + ((RNG_Next(25) + 24) << 0xC);
        } else {
            args.unk_0.x = boss->unk_28 - ((RNG_Next(25) + 24) << 0xC);
        }
        args.unk_0.y = boss->unk_2C;
        args.unk_0.z = boss->unk_30 + ((RNG_Next(129) - 64) << 0xC);
        args.unk_C   = temp_r4;
        args.unk_E   = boss->unk_CA & 1;
        EasyTask_CreateTask(func_ov003_020c3ca4(boss), &Tsk_Boss01_Eff, 0, 0, 0, &args);
        boss->unk_1A4 = 0;
        boss->unk_1A6 = (RNG_Next(11) + 10);
    }
    boss->unk_1A4++;
}
