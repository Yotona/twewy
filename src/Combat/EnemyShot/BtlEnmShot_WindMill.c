#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020c3c28(void);
extern const TaskHandle data_ov003_020e0cec;

s32 func_ov003_020d33f0(Ov003EnemyShotSource* source, s32 speed, u16 angle, u16 angleStep, u16 count, u16 interval, u16 bursts,
                        s32 yScale) {
    Ov003EnemyShotWindMillArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source    = source;
    args.speed     = speed;
    args.angle     = angle;
    args.angleStep = angleStep;
    args.count     = count;
    args.interval  = interval;
    args.bursts    = bursts;
    args.yScale    = yScale;
    TaskPool* pool;
    if (source->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0cec, NULL, 0, NULL, &args);
}

s32 func_ov003_020d349c(Ov003EnemyShotWindMill* self, Ov003EnemyShotWindMillArgs* args) {
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->speed     = args->speed;
    self->angle     = args->angle;
    self->angleStep = args->angleStep;
    self->count     = args->count;
    self->interval  = args->interval;
    self->bursts    = args->bursts;
    self->yScale    = args->yScale;
    self->timer     = 0;
    return 1;
}

s32 func_ov003_020d350c(Ov003EnemyShotWindMill* self) {
    if (data_ov003_020e71b8->unk3D878 & 0x40)
        return 1;
    if (!func_ov003_020cce34(self->source.core.owner->sprite.sprite.bits_0_1))
        return 0;
    if (self->source.core.owner->actor.flags & 2)
        return 0;
    if (self->source.core.owner->actor.flags & 4)
        return 0;
    if (self->bursts <= 0)
        return 0;
    if (func_ov003_020c3c28())
        return 0;
    self->timer--;
    if (self->timer < 0) {
        self->bursts--;
        self->timer = self->interval;
        s32 count;
        s32 i;
        s32 step;
        count = self->count;
        step  = 0x10000 / count;
        i     = 0;
        if (count > 0) {
            s32 offset = 0;
            do {
                u16 angle = self->angle + offset;
                s32 x     = FX_Mul(self->speed, FX_CosIdx(angle));
                s32 y     = FX_Mul(self->speed, FX_SinIdx(angle));
                func_ov003_020d1ec4(&self->source, x, FX_Mul(y, self->yScale), 0, 1);
                i++;
                offset += step;
            } while (i < self->count);
        }
        self->angle += self->angleStep;
    }
    return 1;
}

s32 func_ov003_020d369c(Ov003EnemyShotWindMill* self) {
    return 1;
}

s32 func_ov003_020d36a4(Ov003EnemyShotWindMill* self) {
    return 1;
}

s32 func_ov003_020d36ac(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotWindMill* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d349c(self, args);
        case 1:
            return func_ov003_020d350c(self);
        case 2:
            return func_ov003_020d369c(self);
        case 3:
            return func_ov003_020d36a4(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6dc0[24] = "Tsk_BtlEnmShot_WindMill";
const TaskHandle data_ov003_020e0cec     = {data_ov003_020e6dc0, func_ov003_020d36ac, sizeof(Ov003EnemyShotWindMill)};
