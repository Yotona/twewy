#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc8ac;

s32 func_ov003_020afb64(s32 kind, s32 x, s32 y, s32 angle, s32 speed) {
    Ov003SnowArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.kind       = kind;
    args.position.x = x * 4096;
    args.position.y = y * 4096;
    args.angle      = angle;
    args.speed      = speed;
    return func_ov003_020a6000(&data_ov003_020dc8ac, &args);
}

s32 func_ov003_020afbc4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003SnowArgs* arg = args;
    Ov003Snow*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->kind         = arg->kind;
            self->position.x   = arg->position.x;
            self->position.y   = arg->position.y;
            self->angle        = arg->angle;
            self->speed        = arg->speed;
            self->acceleration = 0x11F;
            self->ticks        = 0;
            self->rotation     = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, self->kind + 0xFC, 0);
            return 1;
        case 1:
            self = task->data;
            if (self->ticks++ >= 30)
                return 0;
            self->position.x += FX_Mul(self->speed, FX_SinCosTable_[(self->angle >> 4) * 2 + 1]);
            self->position.y += FX_Mul(self->speed, FX_SinCosTable_[(self->angle >> 4) * 2]);
            self->speed += self->acceleration;
            if (self->angle != 0x4000) {
                if ((u32)self->angle < 0x4000) {
                    self->angle += 0x320;
                    if ((u32)self->angle > 0x4000)
                        self->angle = 0x4000;
                } else {
                    self->angle -= 0x320;
                    if ((u32)self->angle < 0x4000)
                        self->angle = 0x4000;
                }
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        case 2:
            self = task->data;
            CombatSprite_SetPosition(&self->sprite, (s16)(self->position.x >> 12), (s16)(self->position.y >> 12));
            func_ov003_02082730(&self->sprite, 0x385000);
            self->affine.rotation             = self->rotation;
            self->affine.scaleX               = (self->sprite.flags46 & 1) ? -0x1000 : 0x1000;
            self->affine.scaleY               = 0x1000;
            self->affine.unk_0C               = 0;
            self->affine.unk_0E               = 0;
            self->sprite.affine               = &self->affine;
            self->sprite.sprite.unk_0A.unk_00 = 1;
            CombatSprite_Render(&self->sprite);
            return 1;
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

extern char      data_ov003_020e4c7c[20];
const TaskHandle data_ov003_020dc8ac     = {data_ov003_020e4c7c, func_ov003_020afbc4, sizeof(Ov003Snow)};
char             data_ov003_020e4c7c[20] = "SubTsk_BtlPsy_Snow";
