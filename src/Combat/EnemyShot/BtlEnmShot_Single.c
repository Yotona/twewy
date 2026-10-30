#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020e0cb0;

s32 func_ov003_020d1eec(Ov003EnemyShotSource* source, s32 x, s32 y, s32 z, s32 screenHit, s32 rotate, u16 angle) {
    Ov003EnemyShotSingleArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source     = source;
    args.velocity.x = x;
    args.velocity.y = y;
    args.velocity.z = z;
    args.screenHit  = screenHit;
    args.rotate     = rotate;
    args.angle      = angle;
    TaskPool* pool;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0cb0, NULL, 0, NULL, &args);
}

// Nonmatching: the two mutually exclusive projection-engine moves are emitted in reverse order.
s32 func_ov003_020d1f90(Ov003EnemyShotSingle* self, Ov003EnemyShotSingleArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    s32                         engine = args->source->core.owner->base.sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    func_ov003_020cc8b0(&self->source, args->source);
    self->velocity.x = args->velocity.x;
    self->velocity.y = args->velocity.y;
    self->velocity.z = args->velocity.z;
    if (args->screenHit)
        self->flags |= 1;
    CombatSprite_LoadDirect(engine, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, args->source->animation, args->source->animationMode);
    self->source.sprite = &self->sprite;
    self->angle         = args->angle;
    if (args->rotate) {
        s16 nextX, nextY, x, y;
        s32 projectionEngine = !!self->source.owner->base.sprite.sprite.bits_0_1;
        self->flags |= 2;
        func_ov003_02084348(projectionEngine, &nextX, &nextY, self->source.position.x + args->velocity.x,
                            self->source.position.y + args->velocity.y, self->source.position.z + args->velocity.z);
        func_ov003_02084348(projectionEngine, &x, &y, self->source.position.x, self->source.position.y,
                            self->source.position.z);
        f32 dx = I2F_RND(nextX - x);
        f32 dy = I2F_RND(nextY - y);
        self->angle += FX_Atan2Idx((s32)dy, (s32)dx);
    }
    func_ov003_020cdff4(&self->source, args->source->core.owner->base.sprite.flags46 & 1, 0);
    func_ov003_020cccf0(engine, args->source);
    return 1;
}

s32 func_ov003_020d21c0(Ov003EnemyShotSingle* self) {
    if (data_ov003_020e71b8->unk3D878 & 0x40)
        return 1;
    if (func_ov003_020ccc00(&self->source, &self->sprite))
        return 0;
    func_ov003_020cdff4(&self->source, 0, 1);
    self->source.position.x += self->velocity.x;
    self->source.position.y += self->velocity.y;
    self->source.position.z += self->velocity.z;
    if (self->flags & 1) {
        s16 x, y;
        func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.position.x,
                            self->source.position.y, self->source.position.z);
        self->source.hitResult =
            func_ov003_0208a1a4(func_ov003_0208a114(self->source.attackId), &self->source.owner->base.actor.baseArgs, x, y);
    } else {
        self->source.hitResult =
            func_ov003_0208a164(func_ov003_0208a114(self->source.attackId), &self->source.owner->base.actor.baseArgs,
                                self->source.position.x, self->source.position.y, self->source.position.z);
    }
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020d22e4(Ov003EnemyShotSingle* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.position.x, self->source.position.y,
                        self->source.position.z);
    if (self->flags & 2)
        CombatSprite_SetAffineTransform(&self->sprite, self->angle, 0x1000, 0x1000, 0);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020d2384(Ov003EnemyShotSingle* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d2398(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotSingle* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d1f90(self, args);
        case 1:
            return func_ov003_020d21c0(self);
        case 2:
            return func_ov003_020d22e4(self);
        case 3:
            return func_ov003_020d2384(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6d48[24] = "Tsk_BtlEnmShot_Single";
const TaskHandle data_ov003_020e0cb0     = {data_ov003_020e6d48, func_ov003_020d2398, sizeof(Ov003EnemyShotSingle)};
