#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

#define OPTION_ROUND(value) ((value) > 0 ? (f32)((value) << 12) + 0.5f : (f32)((value) << 12) - 0.5f)

extern s32                    func_ov003_020c37f8(CombatSprite*);
extern void                   func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern const Ov003AttackInfo* func_ov003_0208a114(u16);
extern s32                    func_ov003_0208a164(const Ov003AttackInfo*, Ov003AttackArgs*, s32, s32, s32);
extern const TaskHandle       data_ov003_020e0c74;

s32 func_ov003_020cfedc(Ov003EnemyShotSource* source, u16 angleStep, s16 lifetime, s16 radius, s16 heightRange,
                        s16 minHeight) {
    Ov003EnemyShotOptionArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source      = source;
    args.angleStep   = angleStep;
    args.lifetime    = lifetime;
    args.radius      = radius;
    args.heightRange = heightRange;
    args.minHeight   = minHeight;
    source->core.flags &= ~1;
    source->core.flags |= 0x40;
    TaskPool* pool;
    if (source->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c74, NULL, 0, NULL, &args);
}

s32 func_ov003_020cff90(Ov003EnemyShotOption* self, Ov003EnemyShotOptionArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->angleStep         = args->angleStep;
    self->radius            = args->radius;
    self->heightRange       = args->heightRange;
    self->minHeight         = args->minHeight;
    self->source.core.timer = args->lifetime;
    self->zOffset           = self->source.core.position.z;
    CombatSprite_LoadDirect(self->source.core.owner->sprite.sprite.bits_0_1, &self->sprite, visual->file, entry->charDataIndex,
                            entry->paletteDataIndex, entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    func_ov003_020cdfbc(&self->source, args->source->core.owner->sprite.flags46 & 1, 0);
    return 1;
}

s32 func_ov003_020d0084(Ov003EnemyShotOption* self) {
    s32 radius    = (s32)OPTION_ROUND(self->radius);
    s16 minHeight = self->minHeight;
    s16 maxHeight = minHeight + self->heightRange;
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    func_ov003_020cccf0(self->sprite.sprite.bits_0_1, &self->source);
    func_ov003_020cdfbc(&self->source, 0, 1);
    self->source.core.position.x = self->source.core.owner->actor.position.x + FX_Mul(FX_CosIdx(self->angle), radius);
    self->source.core.position.y = self->source.core.owner->actor.position.y + FX_Mul(FX_SinIdx(self->angle), radius >> 1);
    self->angle += self->angleStep;
    self->periodTimer--;
    if (self->periodTimer < 0) {
        self->periodTimer = 0x10000 / self->angleStep;
        self->zVelocity   = (s32)OPTION_ROUND(RNG_Next(self->heightRange) - (self->heightRange >> 1));
        if (self->zVelocity != 0)
            self->zVelocity /= self->periodTimer;
        f32 minBound = OPTION_ROUND(minHeight);
        s32 offset   = self->zOffset;
        if ((offset >= -(s32)minBound && self->zVelocity > 0) ||
            (offset <= -(s32)OPTION_ROUND(maxHeight) && self->zVelocity < 0))
        {
            self->zVelocity = -self->zVelocity;
        }
    }
    self->zOffset += self->zVelocity;
    if (self->zOffset > -(s32)OPTION_ROUND(minHeight)) {
        self->zOffset = -(s32)OPTION_ROUND(minHeight);
    } else if (self->zOffset < -(s32)OPTION_ROUND(maxHeight)) {
        self->zOffset = -(s32)OPTION_ROUND(maxHeight);
    }
    self->source.core.position.z = self->zOffset + self->source.core.owner->actor.position.z;
    self->source.core.hitResult =
        func_ov003_0208a164(func_ov003_0208a114(self->source.core.attackId), &self->source.core.owner->actor.baseArgs,
                            self->source.core.position.x, self->source.core.position.y, self->source.core.position.z);
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020d043c(Ov003EnemyShotOption* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020d04b4(Ov003EnemyShotOption* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d04c8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotOption* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020cff90(self, args);
        case 1:
            return func_ov003_020d0084(self);
        case 2:
            return func_ov003_020d043c(self);
        case 3:
            return func_ov003_020d04b4(self);
        default:
            return 0;
    }
}

char             data_ov003_020e6cc8[24] = "Tsk_BtlEnmShot_Option";
const TaskHandle data_ov003_020e0c74     = {data_ov003_020e6cc8, func_ov003_020d04c8, sizeof(Ov003EnemyShotOption)};
