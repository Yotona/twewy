#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32 func_ov003_020a66f4(Ov003AttackStreak* streak, u16 attackId, s32 x, s32 y, s32 force, u16 amount);

extern const TaskHandle data_ov003_020dc788;

// Nonmatching: fixed-point product register allocation and argument-store scheduling differ.
s32 func_ov003_020ad280(Ov003AttackStreak* streak, s32 x, s32 y, s32 angle, s32 speed, s32 scale) {
    Ov003SparkArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.streak       = streak;
    args.angle        = angle;
    args.scale        = scale;
    args.speed        = speed;
    args.animation    = 0x70;
    args.position.x   = FX_Mul(0x20000, COS2(FX_SinCosTable_, angle)) + (s32)((u32)x << 12);
    args.position.y   = FX_Mul(0x20000, SIN2(FX_SinCosTable_, angle)) + (s32)((u32)y << 12);
    args.acceleration = 0x333;
    args.range        = 0x80000;
    return func_ov003_020a6000(&data_ov003_020dc788, &args);
}

s32 func_ov003_020ad364(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003SparkArgs* arg = args;
    Ov003Spark*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->streak       = arg->streak;
            self->position.x   = arg->position.x;
            self->position.y   = arg->position.y;
            self->angle        = arg->angle;
            self->scale        = arg->scale;
            self->speed        = arg->speed;
            self->acceleration = arg->acceleration;
            self->lifetime     = arg->lifetime;
            self->range        = arg->range;
            self->ticks        = 0;
            self->traveled     = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, arg->animation, 0);
            for (s32 i = 0; i < 3; i++)
                self->trailIds[i] =
                    func_ov003_020a8e74(self->position.x >> 12, self->position.y >> 12, 2, i + 0x71, 16, 0, -1);
            return 1;
        case 1: {
            Ov003Spark* self = task->data;
            self->position.x += FX_Mul(self->speed, COS2(FX_SinCosTable_, self->angle));
            self->position.y += FX_Mul(self->speed, SIN2(FX_SinCosTable_, self->angle));
            self->traveled += self->speed;
            self->speed += self->acceleration;
            CombatSprite_Update(&self->sprite);
            if (func_ov003_020a66f4(self->streak, 0x15C, (s16)(self->position.x >> 12), (s16)(self->position.y >> 12), 0, 0) ==
                1)
                return 0;
            if (!func_ov003_020a6374(self->position.x >> 12, self->position.y >> 12, 32))
                return 0;
            if (self->range > 0 && self->traveled >= self->range)
                return 0;
            if (self->lifetime != 0 && self->ticks >= self->lifetime)
                return 0;
            for (s32 i = 0; i < 3; i++) {
                func_ov003_020a90e8(&self->trailIds[i]);
                s32 x = self->position.x - FX_Mul(self->speed, COS2(FX_SinCosTable_, self->angle));
                s32 y = self->position.y - FX_Mul(self->speed, SIN2(FX_SinCosTable_, self->angle));
                func_ov003_020a910c(self->trailIds[i], x >> 12, y >> 12);
            }
            return 1;
        }
        case 2: {
            self = task->data;
            CombatSprite_SetPosition(&self->sprite, (s16)(self->position.x >> 12), (s16)(self->position.y >> 12));
            func_ov003_02082730(&self->sprite, 0x385000);
            s32 scale                         = self->scale;
            self->affine.rotation             = self->angle;
            self->affine.scaleX               = (self->sprite.flags46 & 1) ? -scale : scale;
            self->affine.scaleY               = scale;
            self->affine.unk_0C               = 0;
            self->affine.unk_0E               = 0;
            self->sprite.affine               = &self->affine;
            self->sprite.sprite.unk_0A.unk_00 = 1;
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3: {
            Ov003Spark* self = task->data;
            CombatSprite_Release(&self->sprite);
            for (s32 i = 0; i < 3; i++)
                func_ov003_020a90c4(self->trailIds[i]);
            return 1;
        }
    }
}

char             data_ov003_020e4bc0[20] = "SubTsk_BtlPsy_Spark";
const TaskHandle data_ov003_020dc788     = {data_ov003_020e4bc0, func_ov003_020ad364, sizeof(Ov003Spark)};
