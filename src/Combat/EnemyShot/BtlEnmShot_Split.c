#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020c37f8(CombatSprite*);
extern void             func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern void             func_020265d4(void*, s32, u16);
extern const TaskHandle data_ov003_020e0cd4;

s32 func_ov003_020d2bc0(Ov003EnemyShotSplitArgs* args) {
    TaskPool* pool;
    if (args->source->core.owner->sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0cd4, NULL, 0, NULL, args);
}

s32 func_ov003_020d2c24(Ov003EnemyShotSplit* self, Ov003EnemyShotSplitArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->source, sizeof(self->source));
    MI_CpuCopyU8(args->childSource, &self->childSource, sizeof(self->childSource));
    self->travelFrames      = args->travelFrames;
    self->target.x          = args->target.x;
    self->target.y          = args->target.y;
    self->target.z          = args->target.z;
    self->pattern.delay     = args->pattern.delay;
    self->pattern.count     = args->pattern.count;
    self->pattern.speed     = args->pattern.speed;
    self->pattern.velocityZ = args->pattern.velocityZ;
    self->pattern.damping   = args->pattern.damping;
    self->yScale            = args->yScale;
    if (visual != NULL) {
        const SpriteAnimEntry* entry = &visual->entries[visual->entry];
        CombatSprite_LoadDirect(self->source.core.owner->sprite.sprite.bits_0_1, &self->sprite, visual->file,
                                entry->charDataIndex, entry->paletteDataIndex, entry->frameDataIndex, visual->paletteSlot,
                                entry->animDataIndex);
        CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    }
    return 1;
}

s32 func_ov003_020d2d2c(Ov003EnemyShotSplit* self) {
    if (func_ov003_020ccb9c(&self->source, &self->sprite))
        return 0;
    func_020265d4(&self->source.core.position.x, self->target.x, (u16)self->travelFrames);
    func_020265d4(&self->source.core.position.y, self->target.y, (u16)self->travelFrames);
    func_020265d4(&self->source.core.position.z, self->target.z, (u16)self->travelFrames);
    if (self->travelFrames <= 0) {
        if (self->pattern.delay <= 0) {
            s32 i;
            s32 step = 0x10000 / self->pattern.count;
            i        = 0;
            if (i < self->pattern.count) {
                s32 angle = 0;
                do {
                    self->childSource.core.position.x = self->source.core.position.x;
                    self->childSource.core.position.y = self->source.core.position.y;
                    self->childSource.core.position.z = self->source.core.position.z;
                    func_ov003_020d281c(&self->childSource, FX_Mul(self->pattern.speed, FX_CosIdx((u16)angle)),
                                        FX_Mul(FX_Mul(self->pattern.speed, FX_SinIdx((u16)angle)), self->yScale),
                                        FX_Mul(self->pattern.velocityZ, FX32_ONE), self->pattern.damping);
                    i++;
                    angle += step;
                } while (i < self->pattern.count);
            }
            return 0;
        }
        self->pattern.delay--;
    } else {
        self->travelFrames--;
    }
    if (self->source.visual != NULL)
        CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020d2ebc(Ov003EnemyShotSplit* self) {
    if (self->source.visual != NULL) {
        s16 x, y;
        func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                            self->source.core.position.y, self->source.core.position.z);
        CombatSprite_SetPosition(&self->sprite, x, y);
        func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
        CombatSprite_Render(&self->sprite);
    }
    return 1;
}

s32 func_ov003_020d2f40(Ov003EnemyShotSplit* self) {
    if (self->source.visual != NULL)
        CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d2f60(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotSplit* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d2c24(self, args);
        case 1:
            return func_ov003_020d2d2c(self);
        case 2:
            return func_ov003_020d2ebc(self);
        case 3:
            return func_ov003_020d2f40(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6d94[24] = "Tsk_BtlEnmShot_Split";
const TaskHandle data_ov003_020e0cd4     = {data_ov003_020e6d94, func_ov003_020d2f60, sizeof(Ov003EnemyShotSplit)};
