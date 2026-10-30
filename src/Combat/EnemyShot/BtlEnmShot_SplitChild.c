#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020e0cc8;

s32 func_ov003_020d281c(Ov003EnemyShotSource* source, s32 x, s32 y, s32 z, s32 damping) {
    Ov003EnemyShotSplitChildArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source     = source;
    args.velocity.x = x;
    args.velocity.y = y;
    args.velocity.z = z;
    args.damping    = damping;
    TaskPool* pool;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0cc8, NULL, 0, NULL, &args);
}

s32 func_ov003_020d28b0(Ov003EnemyShotSplitChild* self, Ov003EnemyShotSplitChildArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    s32                         engine = args->source->core.owner->base.sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->velocity.x    = args->velocity.x;
    self->velocity.y    = args->velocity.y;
    self->velocity.z    = args->velocity.z;
    self->dampingStep.x = FX_Mul(args->velocity.x, args->damping);
    self->dampingStep.y = FX_Mul(args->velocity.y, args->damping);
    self->dampingStep.z = FX_Mul(args->velocity.z, args->damping);
    CombatSprite_LoadDirect(engine, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    return 1;
}

s32 func_ov003_020d29d4(Ov003EnemyShotSplitChild* self) {
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    self->source.core.position.x += self->velocity.x;
    self->source.core.position.y += self->velocity.y;
    self->source.core.position.z += self->velocity.z;
    if (self->velocity.x > 0) {
        if (self->velocity.x - self->dampingStep.x > 0)
            self->velocity.x -= self->dampingStep.x;
    } else if (self->velocity.x - self->dampingStep.x < 0) {
        self->velocity.x -= self->dampingStep.x;
    }
    if (self->velocity.y > 0) {
        if (self->velocity.y - self->dampingStep.y > 0)
            self->velocity.y -= self->dampingStep.y;
    } else if (self->velocity.y - self->dampingStep.y < 0) {
        self->velocity.y -= self->dampingStep.y;
    }
    if (self->velocity.z > 0) {
        if (self->velocity.z - self->dampingStep.z > 0)
            self->velocity.z -= self->dampingStep.z;
    } else if (self->velocity.z - self->dampingStep.z < 0) {
        self->velocity.z -= self->dampingStep.z;
    }
    self->source.core.hitResult =
        func_ov003_0208a164(func_ov003_0208a114(self->source.core.attackId), &self->source.core.owner->base.actor.baseArgs,
                            self->source.core.position.x, self->source.core.position.y, self->source.core.position.z);
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020d2adc(Ov003EnemyShotSplitChild* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite,
                        self->source.core.position.y <= 0 ? 0x7FFFFFFF : 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020d2b5c(Ov003EnemyShotSplitChild* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d2b70(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotSplitChild* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d28b0(self, args);
        case 1:
            return func_ov003_020d29d4(self);
        case 2:
            return func_ov003_020d2adc(self);
        case 3:
            return func_ov003_020d2b5c(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6d78[28] = "Tsk_BtlEnmShot_SplitChild";
const TaskHandle data_ov003_020e0cc8     = {data_ov003_020e6d78, func_ov003_020d2b70, sizeof(Ov003EnemyShotSplitChild)};
