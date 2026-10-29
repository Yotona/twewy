#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020c3c28(void);
extern const TaskHandle data_ov003_020e0c80;

s32 func_ov003_020d0518(Ov003EnemyShotSource* finalSource, s32 finalSpeed, u16 finalAngle, u16 finalSpread, u16 finalCount,
                        Ov003EnemyShotSource* initialSource, s32 initialSpeed, u16 initialAngle, u16 initialSpread,
                        u16 initialCount, s16 delay) {
    Ov003EnemyShotRapidSlowArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.finalSource   = finalSource;
    args.finalSpeed    = finalSpeed;
    args.finalAngle    = finalAngle;
    args.finalSpread   = finalSpread;
    args.finalCount    = finalCount;
    args.initialSource = initialSource;
    args.initialSpeed  = initialSpeed;
    args.initialAngle  = initialAngle;
    args.initialSpread = initialSpread;
    args.initialCount  = initialCount;
    args.delay         = delay;
    TaskPool* pool;
    if (finalSource->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c80, NULL, 0, NULL, &args);
}

s32 func_ov003_020d05dc(Ov003EnemyShotRapidSlow* self, Ov003EnemyShotRapidSlowArgs* args) {
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->finalSource, &self->finalShot.source, sizeof(self->finalShot.source));
    MI_CpuCopyU8(args->initialSource, &self->initialShot.source, sizeof(self->initialShot.source));
    self->finalShot.speed    = args->finalSpeed;
    self->finalShot.angle    = args->finalAngle;
    self->finalShot.spread   = args->finalSpread;
    self->finalShot.count    = args->finalCount;
    self->initialShot.speed  = args->initialSpeed;
    self->initialShot.angle  = args->initialAngle;
    self->initialShot.spread = args->initialSpread;
    self->initialShot.count  = args->initialCount;
    self->initialShot.delay  = args->delay;
    return 1;
}

s32 func_ov003_020d0664(Ov003EnemyShotRapidSlow* self) {
    if (!func_ov003_020cce34(self->finalShot.source.core.owner->sprite.sprite.bits_0_1))
        return 0;
    if (self->finalShot.source.core.owner->actor.flags & 2)
        return 0;
    if (self->finalShot.source.core.owner->actor.flags & 4)
        return 0;
    if (func_ov003_020c3c28())
        return 0;
    if (self->timer == 0) {
        func_ov003_020d1dc0(&self->initialShot.source, self->initialShot.speed, self->initialShot.angle,
                            self->initialShot.spread, self->initialShot.count);
    } else if (self->timer == self->initialShot.delay) {
        func_ov003_020d1dc0(&self->finalShot.source, self->finalShot.speed, self->finalShot.angle, self->finalShot.spread,
                            self->finalShot.count);
        return 0;
    }
    self->timer++;
    return 1;
}

s32 func_ov003_020d0748(Ov003EnemyShotRapidSlow* self) {
    return 1;
}
s32 func_ov003_020d0750(Ov003EnemyShotRapidSlow* self) {
    return 1;
}

s32 func_ov003_020d0758(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotRapidSlow* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d05dc(self, args);
        case 1:
            return func_ov003_020d0664(self);
        case 2:
            return func_ov003_020d0748(self);
        case 3:
            return func_ov003_020d0750(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6ce0[28] = "Tsk_BtlEnmShot_RapidSlow";
const TaskHandle data_ov003_020e0c80     = {data_ov003_020e6ce0, func_ov003_020d0758, sizeof(Ov003EnemyShotRapidSlow)};
