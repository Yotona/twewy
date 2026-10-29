#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020c37f8(CombatSprite*);
extern void             func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern const TaskHandle data_ov003_020e0c5c;

s32 func_ov003_020cf34c(Ov003EnemyShotSource* source, u16 angle, s32 initialSpeed, s32 targetSpeed, u16 turnFrames,
                        s16 lifetime) {
    Ov003EnemyShotHomingArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source       = source;
    args.angle        = angle;
    args.initialSpeed = initialSpeed;
    args.targetSpeed  = targetSpeed;
    args.turnFrames   = turnFrames;
    args.lifetime     = lifetime;
    source->core.flags &= ~1;
    source->core.flags |= 0x44;
    TaskPool* pool;
    if (source->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c5c, NULL, 0, NULL, &args);
}

s32 func_ov003_020cf400(Ov003EnemyShotHoming* self, Ov003EnemyShotHomingArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    s32                         engine = args->source->core.owner->sprite.sprite.bits_0_1;
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    self->targetSpeed       = args->targetSpeed;
    self->turnFrames        = args->turnFrames;
    self->source.core.timer = args->lifetime;
    self->velocityX         = FX_Mul(FX_CosIdx(args->angle), args->initialSpeed);
    self->velocityY         = FX_Mul(FX_SinIdx(args->angle), args->initialSpeed);
    CombatSprite_LoadDirect(engine, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                            entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    func_ov003_020cdfbc(&self->source, args->source->core.owner->sprite.flags46 & 1, 0);
    func_ov003_020cccf0(engine, &self->source);
    return 1;
}

s32 func_ov003_020cf554(Ov003EnemyShotHoming* self) {
    CombatActor* target;
    if (func_ov003_020c37f8(&self->sprite))
        target = data_ov003_020e71b8->unk3D89C;
    else
        target = data_ov003_020e71b8->unk3D898;
    s32 engine = func_ov003_020c37f8(&self->sprite) ? 1 : 0;
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    func_ov003_020cdfbc(&self->source, 0, 1);
    s16 x, y, targetX, targetY;
    func_ov003_02084348(engine, &x, &y, self->source.core.position.x, self->source.core.position.y,
                        self->source.core.position.z);
    func_ov003_02084348(engine, &targetX, &targetY, target->position.x, target->position.y, target->position.z);
    u16 angle     = FX_Atan2Idx(targetY - y, targetX - x);
    s32 velocityY = FX_Mul(FX_SinIdx(angle), self->targetSpeed);
    s32 velocityX = FX_Mul(FX_CosIdx(angle), self->targetSpeed);
    s32 dx        = velocityX - self->velocityX;
    if (dx != 0)
        self->velocityX += dx / self->turnFrames;
    s32 dy = velocityY - self->velocityY;
    if (dy != 0)
        self->velocityY += dy / self->turnFrames;
    self->source.core.position.x += self->velocityX;
    self->source.core.position.y += self->velocityY;
    func_ov003_02084348(engine, &x, &y, self->source.core.position.x, self->source.core.position.y,
                        self->source.core.position.z);
    self->source.core.hitResult =
        func_ov003_0208a1a4(func_ov003_0208a114(self->source.core.attackId), &self->source.core.owner->actor.baseArgs, x, y);
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020cf740(Ov003EnemyShotHoming* self) {
    s16 x, y;
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020cf7b8(Ov003EnemyShotHoming* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020cf7cc(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotHoming* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020cf400(self, args);
        case 1:
            return func_ov003_020cf554(self);
        case 2:
            return func_ov003_020cf740(self);
        case 3:
            return func_ov003_020cf7b8(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6c94[24] = "Tsk_BtlEnmShot_Homing";
const TaskHandle data_ov003_020e0c5c     = {data_ov003_020e6c94, func_ov003_020cf7cc, sizeof(Ov003EnemyShotHoming)};
