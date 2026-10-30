#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020e0c18;

s32 func_ov003_020cd6f4(Ov003EnemyShotSource* source, s32 opacity, s32 decay) {
    Ov003EnemyShotBlurArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    args.source  = source;
    args.opacity = opacity;
    args.decay   = decay;
    TaskPool* pool;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020e0c18, NULL, 0, NULL, &args);
}

s32 func_ov003_020cd778(Ov003EnemyShotBlur* self, Ov003EnemyShotBlurArgs* args) {
    const Ov003EnemyShotVisual* visual = args->source->visual;
    const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
    func_ov003_020c37f8(&args->source->core.owner->base.sprite);
    MI_CpuSet(self, 0, sizeof(*self));
    self->position.x = args->source->core.position.x;
    self->position.y = args->source->core.position.y;
    self->position.z = args->source->core.position.z;
    self->opacity    = args->opacity * 32 - 0x1000;
    self->decay      = args->decay * 32 - 0x1000;
    CombatSprite_LoadDirect(func_ov003_020c37f8(&args->source->core.owner->base.sprite) ? 1 : 0, &self->sprite, visual->file,
                            entry->charDataIndex, entry->paletteDataIndex, entry->frameDataIndex, visual->paletteSlot,
                            entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, args->source->animation, args->source->animationMode);
    return 1;
}

s32 func_ov003_020cd868(Ov003EnemyShotBlur* self) {
    if (func_ov003_020c3c28())
        return 0;
    CombatSprite_Update(&self->sprite);
    self->opacity -= self->decay;
    return self->opacity > 0x1000;
}

s32 func_ov003_020cd8a8(Ov003EnemyShotBlur* self) {
    if (self->opacity > 0x1000) {
        s16 x, y;
        func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->position.x, self->position.y,
                            self->position.z);
        CombatSprite_SetPosition(&self->sprite, x, y);
        func_ov003_02082730(&self->sprite, 0x7FFFFFFF);
        CombatSprite_RenderWithPalette(&self->sprite, self->opacity >> 12);
    }
    return 1;
}

s32 func_ov003_020cd928(Ov003EnemyShotBlur* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020cd938(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotBlur* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020cd778(self, args);
        case 1:
            return func_ov003_020cd868(self);
        case 2:
            return func_ov003_020cd8a8(self);
        case 3:
            return func_ov003_020cd928(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6c00[20] = "Tsk_BtlEnmShot_Blur";
const TaskHandle data_ov003_020e0c18     = {data_ov003_020e6c00, func_ov003_020cd938, sizeof(Ov003EnemyShotBlur)};
