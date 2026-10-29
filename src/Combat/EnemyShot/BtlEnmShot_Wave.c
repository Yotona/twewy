#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020c37f8(CombatSprite*);
extern void             func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern const TaskHandle data_ov003_020e0ce0;

s32 func_ov003_020d2fb0(Ov003EnemyShotSource* source, u16 angle, s32 speed, s32 amplitude, u16 phaseStep) {
    Ov003EnemyShotWaveArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source    = source;
    args.angle     = angle;
    args.speed     = speed;
    args.amplitude = amplitude;
    args.phaseStep = phaseStep;
    TaskPool* pool;
    if (source->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0ce0, NULL, 0, NULL, &args);
}

s32 func_ov003_020d3044(Ov003EnemyShotWave* self, Ov003EnemyShotWaveArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    s32                         engine = args->source->core.owner->sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->angle     = args->angle;
    self->velocityX = FX_Mul(FX_CosIdx(args->angle), args->speed);
    self->velocityY = FX_Mul(FX_SinIdx(args->angle), args->speed);
    self->amplitude = args->amplitude;
    self->phaseStep = args->phaseStep;
    CombatSprite_LoadDirect(engine, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    func_ov003_020cdfbc(&self->source, args->source->core.owner->sprite.flags46 & 1, 0);
    func_ov003_020cccf0(engine, &self->source);
    return 1;
}

s32 func_ov003_020d3198(Ov003EnemyShotWave* self) {
    s32 engine = func_ov003_020c37f8(&self->sprite) ? 1 : 0;
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    func_ov003_020cdfbc(&self->source, 0, 1);
    s32 wave      = FX_Mul(self->amplitude, FX_SinIdx(self->phase));
    s32 offsetX   = FX_Mul(wave, FX_CosIdx((u16)(self->angle + 0x4000)));
    s32 offsetY   = FX_Mul(wave, FX_SinIdx((u16)(self->angle + 0x4000)));
    self->offsetX = offsetX;
    self->offsetY = offsetY;
    self->phase += self->phaseStep;
    self->source.core.position.x += self->velocityX;
    self->source.core.position.y += self->velocityY;
    s16 x, y;
    func_ov003_02084348(engine, &x, &y, self->source.core.position.x + self->offsetX,
                        self->source.core.position.y + self->offsetY, self->source.core.position.z);
    self->source.core.hitResult =
        func_ov003_0208a1a4(func_ov003_0208a114(self->source.core.attackId), &self->source.core.owner->actor.baseArgs, x, y);
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020d3304(Ov003EnemyShotWave* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x + self->offsetX,
                        self->source.core.position.y + self->offsetY, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020d338c(Ov003EnemyShotWave* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d33a0(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotWave* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d3044(self, args);
        case 1:
            return func_ov003_020d3198(self);
        case 2:
            return func_ov003_020d3304(self);
        case 3:
            return func_ov003_020d338c(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6dac[20] = "Tsk_BtlEnmShot_Wave";
const TaskHandle data_ov003_020e0ce0     = {data_ov003_020e6dac, func_ov003_020d33a0, sizeof(Ov003EnemyShotWave)};
