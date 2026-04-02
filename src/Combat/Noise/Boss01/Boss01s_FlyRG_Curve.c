#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss01.h"
#include "Engine/Math/Random.h"
#include <NitroSDK/fx.h>

typedef struct {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          unk_60;
    /* 0x6C */ s32          unk_6C;
    /* 0x70 */ s32          unk_70;
    /* 0x74 */ s32          unk_74;
    /* 0x78 */ s32          unk_78;
    /* 0x7C */ s32          unk_7C;
    /* 0x80 */ s32          unk_80;
    /* 0x84 */ s16          unk_84;
    /* 0x86 */ s16          unk_86;
    /* 0x88 */ s16          unk_88;
} Boss01s_FlyRG_Curve; // Size: 0x8C

s32 Boss01s_FlyRG_Curve_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern const BinIdentifier   data_ov016_02128f98;
static const SpriteAnimEntry data_ov016_02129010[] = {1, 3, 2, 0, 1, 3, 2, 3};

static const TaskHandle Tsk_Boss01s_FlyRG_Curve = {"Tsk_Boss01s_FlyRG_Curve", Boss01s_FlyRG_Curve_RunTask, 0x8C};

void Boss01s_FlyRG_Curve_Randomize(Boss01s_FlyRG_Curve* arg0) {
    arg0->unk_6C = RNG_Next(3278) + 3277;
    arg0->unk_74 = RNG_Next(2458) + 2458;
    arg0->unk_80 = -(RNG_Next(25) + 25);

    f32 var_r0;
    if (RNG_Next(F2I(data_ov003_020e71b8->unk3D824)) != 0) {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) + 0.5f;
    } else {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) - 0.5f;
    }
    arg0->unk_60.x = var_r0;
    arg0->unk_60.y = 0;

    s64 temp_r3    = ((s64)arg0->unk_74 * (s64)arg0->unk_74) + 0x800;
    s32 temp_r5_2  = RNG_Next(29) + 100;
    arg0->unk_60.z = -I2F(temp_r5_2) + (FX_Divide(F2I(temp_r3), arg0->unk_80) / 2);

    if (RNG_Next(4) == 0) {
        arg0->unk_6C *= -1;
        arg0->unk_60.x += 0x80000;
        arg0->unk_60.z -= 0x14000;
    } else {
        arg0->unk_60.x -= 0x80000;
    }
}

s32 Boss01s_FlyRG_Curve_Init(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyRG_Curve* curve = task->data;

    MI_CpuSet(curve, 0, sizeof(Boss01s_FlyRG_Curve));
    CombatSprite_LoadFromTable(1, &curve->sprite, &data_ov016_02128f98, data_ov016_02129010, RNG_Next(2), 4, 4);
    Boss01s_FlyRG_Curve_Randomize(curve);

    f32 var_r0;
    if (RNG_Next(F2I(data_ov003_020e71b8->unk3D824)) != 0) {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) + 0.5f;
    } else {
        var_r0 = I2F(RNG_Next(F2I(data_ov003_020e71b8->unk3D824))) - 0.5f;
    }

    curve->unk_60.x = var_r0;
    return 1;
}

s32 Boss01s_FlyRG_Curve_Update(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyRG_Curve* curve = task->data;

    if (func_ov003_020c9008() != 0) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    switch (curve->unk_88) {
        case 0:
            curve->unk_60.x += curve->unk_6C;
            curve->unk_74 += curve->unk_80;
            curve->unk_60.z += curve->unk_74;
            if (((curve->unk_6C < 0) && (curve->unk_60.x < -0x10000)) ||
                ((curve->unk_6C > 0) && (curve->unk_60.x > (data_ov003_020e71b8->unk3D824 + 0x10000))))
            {
                curve->unk_88++;
            }
            break;
        case 1:
            if (curve->unk_84 == 0) {
                curve->unk_86 = RNG_Next(180);
            }
            if (curve->unk_84 <= curve->unk_86) {
                curve->unk_84++;
            } else {
                Boss01s_FlyRG_Curve_Randomize(curve);
                curve->unk_84 = 0;
                curve->unk_88 = 0;
            }
            break;
    }
    CombatSprite_Update(&curve->sprite);
    return 1;
}

s32 Boss01s_FlyRG_Curve_Render(TaskPool* pool, Task* task, void* args) {
    s16                  spA, sp8;
    Boss01s_FlyRG_Curve* curve = task->data;

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02084348(func_ov003_020c3908(curve), &spA, &sp8, curve->unk_60.x, curve->unk_60.y, curve->unk_60.z);
    CombatSprite_SetPosition(&curve->sprite, spA, sp8);
    func_ov003_02082730(&curve->sprite, 0x7FFFFFFF);
    CombatSprite_Render(&curve->sprite);
    return 1;
}

s32 Boss01s_FlyRG_Curve_Destroy(TaskPool* pool, Task* task, void* args) {
    Boss01s_FlyRG_Curve* curve = task->data;
    CombatSprite_Release(&curve->sprite);
    return 1;
}

s32 Boss01s_FlyRG_Curve_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov016_02129000 = {
        .initialize = Boss01s_FlyRG_Curve_Init,
        .update     = Boss01s_FlyRG_Curve_Update,
        .render     = Boss01s_FlyRG_Curve_Render,
        .cleanup    = Boss01s_FlyRG_Curve_Destroy,
    };
    return data_ov016_02129000.iter[stage](pool, task, args);
}

void Boss01s_FlyRG_Curve_CreateTask(void) {
    for (s32 i = 0; i < 5; i++) {
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_Boss01s_FlyRG_Curve, NULL, 0, NULL, NULL);
    }
}
