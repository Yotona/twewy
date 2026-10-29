#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32 func_ov003_020843b0(s32, s32);
extern s32 func_ov003_020843ec(s32, s32, s32);

s32 func_ov003_020b69f8(TaskPool* pool, Task* task, void* args) {
    Ov003ShockWaveBlur*     self = task->data;
    Ov003ShockWaveBlurArgs* arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    self->position.x   = arg->position.x;
    self->position.y   = arg->position.y;
    self->position.z   = arg->position.z;
    self->fixedOpacity = 0x13000;
    self->angle        = arg->angle;
    func_ov003_020a3498(&self->sprite, 2, 1, arg->animation, 0);
    return 1;
}

s32 func_ov003_020b6a60(TaskPool* pool, Task* task, void* args) {
    Ov003ShockWaveBlur* self = task->data;
    CombatSprite_Update(&self->sprite);
    self->fixedOpacity -= 0x1000;
    self->opacity = self->fixedOpacity >> 12;
    return self->opacity >= 5;
}

s32 func_ov003_020b6a98(TaskPool* pool, Task* task, void* args) {
    Ov003ShockWaveBlur* self = task->data;
    if (self->opacity > 0) {
        s32 x = func_ov003_020843b0(0, self->position.x);
        s32 y = func_ov003_020843ec(0, self->position.y, self->position.z);
        CombatSprite_SetPosition(&self->sprite, x, y);
        CombatSprite_SetAffineTransform(&self->sprite, self->angle, 0x1000, 0x1000, 0);
        CombatSprite_RenderWithPalette(&self->sprite, self->opacity);
    }
    return 1;
}

s32 func_ov003_020b6b0c(TaskPool* pool, Task* task, void* args) {
    Ov003ShockWaveBlur* self = task->data;
    CombatSprite_Release(&self->sprite);
    return 1;
}

const TaskStages data_ov003_020dcaf4 = {
    {func_ov003_020b69f8, func_ov003_020b6a60, func_ov003_020b6a98, func_ov003_020b6b0c}
};
