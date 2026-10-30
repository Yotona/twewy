#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020e0cbc;

s32 func_ov003_020d23e8(Ov003EnemyShotSource* source, u16 angle, s32 speed, s32 acceleration, u16 angleStep, s32 yScale) {
    Ov003EnemyShotSpiralArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source       = source;
    args.angle        = angle;
    args.speed        = speed;
    args.acceleration = acceleration;
    args.angleStep    = angleStep;
    args.yScale       = yScale;
    TaskPool* pool;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0cbc, NULL, 0, NULL, &args);
}

s32 func_ov003_020d2484(Ov003EnemyShotSpiral* self, Ov003EnemyShotSpiralArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    s32                         engine = args->source->core.owner->base.sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->angle        = args->angle;
    self->angleStep    = args->angleStep;
    self->yScale       = args->yScale;
    self->speed        = args->speed;
    self->acceleration = args->acceleration;
    self->velocityX    = FX_Mul(FX_CosIdx(self->angle), self->speed);
    self->velocityY    = FX_Mul(FX_SinIdx(self->angle), self->speed);
    CombatSprite_LoadDirect(engine, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    func_ov003_020cdfbc(&self->source, args->source->core.owner->base.sprite.flags46 & 1, 0);
    func_ov003_020cccf0(engine, &self->source);
    return 1;
}

s32 func_ov003_020d25e8(Ov003EnemyShotSpiral* self) {
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    func_ov003_020cdfbc(&self->source, 0, 1);
    self->source.core.position.x += self->velocityX;
    self->source.core.position.y += FX_Mul(self->velocityY, self->yScale);
    self->angle += self->angleStep;
    self->speed += self->acceleration;
    self->velocityX = FX_Mul(FX_CosIdx(self->angle), self->speed);
    self->velocityY = FX_Mul(FX_SinIdx(self->angle), self->speed);
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    self->source.core.hitResult = func_ov003_0208a1a4(func_ov003_0208a114(self->source.core.attackId),
                                                      &self->source.core.owner->base.actor.baseArgs, x, y);
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020d2740(Ov003EnemyShotSpiral* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020d27b8(Ov003EnemyShotSpiral* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d27cc(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotSpiral* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d2484(self, args);
        case 1:
            return func_ov003_020d25e8(self);
        case 2:
            return func_ov003_020d2740(self);
        case 3:
            return func_ov003_020d27b8(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6d60[24] = "Tsk_BtlEnmShot_Spiral";
const TaskHandle data_ov003_020e0cbc     = {data_ov003_020e6d60, func_ov003_020d27cc, sizeof(Ov003EnemyShotSpiral)};
