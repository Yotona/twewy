#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"
#include <NitroSDK/fx.h>

/// MARK: Boss01s_FlyUG

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          unk_60;
    /* 0x6C */ Vec          unk_6C;
    /* 0x78 */ s16          unk_78;
    /* 0x7A */ s16          unk_7A;
    /* 0x7C */ s16          unk_7C;
} Boss01s_FlyUG; // Size: 0x80

extern void func_02026590(s32, s32, u16);
extern void func_020265d4(s32, s32, u16);

s32 Boss01s_FlyUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern const BinIdentifier data_ov016_02128f98;

static const TaskHandle      Tsk_Boss01s_FlyUG     = {"Tsk_Boss01s_FlyUG", Boss01s_FlyUG_RunTask, sizeof(Boss01s_FlyUG)};
static const SpriteAnimEntry data_ov016_0212903c[] = {1, 3, 2, 4, 1, 3, 2, 5, 1, 3, 2, 6};

void Boss01s_FlyUG_GetSpawnPos(Boss01s_FlyUG* arg0, Vec* arg1) {
    s32 width = F2I(data_ov003_020e71b8->unk3D7CC);

    if (RNG_Next(2) != 0) {
        arg1->x = I2F((width / 2 - 0x30) + RNG_Next(65));
        arg1->y = I2F(RNG_Next(17) - 38);
        arg1->z = 0;
    } else {
        arg1->x = I2F(width / 2 + 48 + RNG_Next(width - (width / 2 + 47)));
        arg1->y = I2F(RNG_Next(17) - 38);
        arg1->z = 0;
    }
}

s32 Boss01s_FlyUG_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyUG* flyUG = task->data;

    MI_CpuSet(flyUG, 0, sizeof(Boss01s_FlyUG));
    CombatSprite_LoadFromTable(0, &flyUG->sprite, &data_ov016_02128f98, data_ov016_0212903c, 0, 4, 4);
    Boss01s_FlyUG_GetSpawnPos(flyUG, &flyUG->unk_60);
    return 1;
}

s32 Boss01s_FlyUG_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyUG* flyUG = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    switch (flyUG->unk_7C) {
        case 0:
            if (flyUG->unk_78 == 0) {
                CombatSprite_SetAnimFromTable(&flyUG->sprite, (RNG_Next(2) + 1), 0);
                flyUG->unk_7A = (RNG_Next(301) + 300);
            }
            if (flyUG->unk_78 <= flyUG->unk_7A) {
                flyUG->unk_78++;
            } else {
                flyUG->unk_78 = 0;
                flyUG->unk_7C++;
            }
            break;
        case 1:
            if (flyUG->unk_78 == 0) {
                CombatSprite_SetAnimFromTable(&flyUG->sprite, 0, 0);
                flyUG->unk_6C.x = (RNG_Next(2) == 0) ? -0x1000 : 0x1000;
                flyUG->unk_6C.z = -0x2000;
            }

            flyUG->unk_6C.z -= 0x800;
            if (flyUG->unk_6C.z < -0x4000) {
                flyUG->unk_6C.z = -0x4000;
            }

            flyUG->unk_60.x += flyUG->unk_6C.x;
            flyUG->unk_60.z += flyUG->unk_6C.z;

            if (flyUG->unk_60.z < -0x60000) {
                flyUG->unk_78 = 0;
                flyUG->unk_7C++;
            } else {
                flyUG->unk_78++;
            }
            break;
        case 2:
            if (flyUG->unk_78 == 0) {
                flyUG->unk_7A = (s16)(RNG_Next(0x12D) + 0x12C);
            }
            if (flyUG->unk_78 <= flyUG->unk_7A) {
                flyUG->unk_78++;

            } else {
                flyUG->unk_78 = 0;
                flyUG->unk_7C++;
            }
            break;
        case 3:
            if (flyUG->unk_78 == 0) {
                Boss01s_FlyUG_GetSpawnPos(flyUG, &flyUG->unk_6C);
                if (RNG_Next(2) != 0) {
                    flyUG->unk_60.x = (flyUG->unk_6C.x + 0x20000);
                } else {
                    flyUG->unk_60.x = (flyUG->unk_6C.x - 0x20000);
                }
                flyUG->unk_78 = 0x1E;
            }
            if (flyUG->unk_78 > 0) {
                func_02026590(&flyUG->unk_60.x, flyUG->unk_6C.x, flyUG->unk_78);
                func_020265d4(&flyUG->unk_60.y, flyUG->unk_6C.y, flyUG->unk_78);
                func_020265d4(&flyUG->unk_60.z, flyUG->unk_6C.z, flyUG->unk_78);
                flyUG->unk_78--;
            }
            if (flyUG->unk_78 <= 0) {
                flyUG->unk_78 = 0;
                flyUG->unk_7C = 0;
            }
            break;
    }
    CombatSprite_Update(&flyUG->sprite);
    return 1;
}

s32 Boss01s_FlyUG_Render(TaskPool* pool, Task* task, void* args) {
    s16            spA, sp8;
    Boss01s_FlyUG* flyUG = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(flyUG), &spA, &sp8, flyUG->unk_60.x, flyUG->unk_60.y, flyUG->unk_60.z);
    CombatSprite_SetPosition(&flyUG->sprite, spA, sp8);
    func_ov003_02082730(&flyUG->sprite, 0x7FFFFFFF);
    CombatSprite_Render(&flyUG->sprite);
    return 1;
}

s32 Boss01s_FlyUG_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyUG* flyUG = task->data;

    CombatSprite_Release(&flyUG->sprite);
    return 1;
}

s32 Boss01s_FlyUG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_0212902c = {
        .initialize = Boss01s_FlyUG_Init,
        .update     = Boss01s_FlyUG_Update,
        .render     = Boss01s_FlyUG_Render,
        .cleanup    = Boss01s_FlyUG_Destroy,
    };
    return data_ov016_0212902c.iter[stage](pool, task, args);
}

void Boss01s_FlyUG_CreateTask(void) {
    for (s32 i = 0; i < 5; i++) {
        EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_Boss01s_FlyUG, 0, 0, 0, NULL);
    }
}
