#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

#define MACHINE_GUN_ROUND(value) ((value) > 0 ? (f32)((value) << 12) + 0.5f : (f32)((value) << 12) - 0.5f)

extern s32              func_ov003_020c37f8(CombatSprite*);
extern s32              func_ov003_020c3c28(void);
extern const TaskHandle data_ov003_020e0c68;

s32 func_ov003_020cf81c(Ov003EnemyShotSource* source, s32 speed, u16 count, u16 interval, s32 fixedAim, s32 rotate, u16 angle,
                        s32 stopOnCrossing) {
    Ov003EnemyShotMachineGunArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source         = source;
    args.speed          = speed;
    args.count          = count;
    args.interval       = interval;
    args.fixedAim       = fixedAim;
    args.rotate         = rotate;
    args.angle          = angle;
    args.stopOnCrossing = stopOnCrossing;
    TaskPool* pool;
    if (source->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    source->core.flags &= ~4;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c68, NULL, 0, NULL, &args);
}

void func_ov003_020cf8d4(Ov003EnemyShotMachineGun* self) {
    TaskPool* pool;
    if (self->source.core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    s32 i;
    for (i = self->lastTask; i >= 0; i--) {
        s32 result = func_ov003_020ccca4(self->source.core.owner->sprite.sprite.bits_0_1, self->taskIds[i]);
        if (result != 0) {
            if (self->source.core.hitResult != 1)
                self->source.core.hitResult = 2;
            if (result == 1) {
                self->hitCount++;
                self->source.core.hitResult = 1;
            }
            EasyTask_DeleteTask(pool, self->taskIds[i]);
            self->taskIds[i] = -1;
        }
    }
}

s32 func_ov003_020cf990(Ov003EnemyShotMachineGun* self) {
    if (!func_ov003_020cce34(self->source.core.owner->sprite.sprite.bits_0_1))
        return 1;
    if (self->source.core.owner->actor.flags & 2)
        return 1;
    if (self->source.core.owner->actor.flags & 4)
        return 1;
    if (self->crossing != 0) {
        CombatActor* target;
        if (func_ov003_020c37f8(&self->source.core.owner->sprite))
            target = data_ov003_020e71b8->unk3D89C;
        else
            target = data_ov003_020e71b8->unk3D898;
        if (self->crossing == 2) {
            if (target->position.x <= self->source.core.position.x)
                return 1;
        }
        if (self->crossing == 1) {
            if (target->position.x >= self->source.core.position.x)
                return 1;
        }
    }
    if (self->remaining < 0) {
        TaskPool* pool;
        if (self->source.core.owner->sprite.sprite.bits_0_1 == 0)
            pool = &data_ov003_020e71b8->unk_00000;
        else
            pool = &data_ov003_020e71b8->taskPool;
        s32 empty = 1;
        s32 i;
        for (i = self->lastTask; i >= 0; i--) {
            if (EasyTask_GetTaskById(pool, self->taskIds[i]) != NULL) {
                empty = 0;
                break;
            }
        }
        if (empty)
            return 1;
    }
    return 0;
}

s32 func_ov003_020cfad8(Ov003EnemyShotMachineGun* self, Ov003EnemyShotMachineGunArgs* args) {
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->speed     = args->speed;
    self->remaining = args->count - 1;
    self->lastTask  = self->remaining;
    self->interval  = args->interval;
    self->timer     = 0;
    self->fixedAim  = args->fixedAim;
    self->rotate    = args->rotate;
    self->angle     = args->angle;
    s32 i;
    for (i = 0; i < 10; i++)
        self->taskIds[i] = -1;
    CombatActor* target;
    if (func_ov003_020c37f8(&self->source.core.owner->sprite))
        target = data_ov003_020e71b8->unk3D89C;
    else
        target = data_ov003_020e71b8->unk3D898;
    s32 height       = (s32)MACHINE_GUN_ROUND(target->unk_72);
    s32 targetZ      = target->position.z - (height >> 1);
    s32 dx           = target->position.x - self->source.core.position.x;
    s32 dz           = targetZ - self->source.core.position.z;
    s32 dy           = target->position.y - self->source.core.position.y;
    self->velocity.x = dx;
    self->velocity.y = dy;
    self->velocity.z = dz;
    Vec_Normalize(&self->velocity, &self->velocity);
    self->velocity.x = FX_Mul(self->velocity.x, self->speed);
    self->velocity.y = FX_Mul(self->velocity.y, self->speed);
    self->velocity.z = FX_Mul(self->velocity.z, self->speed);
    self->crossing   = 0;
    if (args->stopOnCrossing)
        self->crossing = self->velocity.x > 0 ? 2 : 1;
    return 1;
}

s32 func_ov003_020cfc9c(Ov003EnemyShotMachineGun* self) {
    if (func_ov003_020cf990(self))
        return 0;
    if (func_ov003_020c3c28())
        return 0;
    self->timer--;
    if (self->timer < 0 && self->remaining >= 0) {
        s8 result = self->source.core.hitResult;
        if (self->fixedAim == 0) {
            CombatActor* target;
            if (func_ov003_020c37f8(&self->source.core.owner->sprite))
                target = data_ov003_020e71b8->unk3D89C;
            else
                target = data_ov003_020e71b8->unk3D898;
            s32 height       = (s32)MACHINE_GUN_ROUND(target->unk_72);
            s32 targetZ      = target->position.z - (height >> 1);
            s32 dx           = target->position.x - self->source.core.position.x;
            s32 dz           = targetZ - self->source.core.position.z;
            s32 dy           = target->position.y - self->source.core.position.y;
            self->velocity.x = dx;
            self->velocity.y = dy;
            self->velocity.z = dz;
            Vec_Normalize(&self->velocity, &self->velocity);
            self->velocity.x = FX_Mul(self->velocity.x, self->speed);
            self->velocity.y = FX_Mul(self->velocity.y, self->speed);
            self->velocity.z = FX_Mul(self->velocity.z, self->speed);
        }
        self->source.core.hitResult    = 0;
        self->taskIds[self->remaining] = func_ov003_020d1eec(&self->source, self->velocity.x, self->velocity.y,
                                                             self->velocity.z, 0, self->rotate, self->angle);
        self->source.core.hitResult    = result;
        self->remaining--;
        self->timer = self->interval;
    }
    func_ov003_020cf8d4(self);
    return 1;
}

s32 func_ov003_020cfe7c(Ov003EnemyShotMachineGun* self) {
    return 1;
}
s32 func_ov003_020cfe84(Ov003EnemyShotMachineGun* self) {
    return 1;
}

s32 func_ov003_020cfe8c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotMachineGun* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020cfad8(self, args);
        case 1:
            return func_ov003_020cfc9c(self);
        case 2:
            return func_ov003_020cfe7c(self);
        case 3:
            return func_ov003_020cfe84(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6cac[28] = "Tsk_BtlEnmShot_MachineGun";
const TaskHandle data_ov003_020e0c68     = {data_ov003_020e6cac, func_ov003_020cfe8c, sizeof(Ov003EnemyShotMachineGun)};
