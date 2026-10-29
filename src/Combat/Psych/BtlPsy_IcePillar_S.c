#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern s32              func_ov003_020a51c0(void);
extern void             func_ov003_02087f28(s32, s32);
extern const TaskHandle data_ov003_020dc948;
extern const s32        data_ov003_020dc954[3];

/// The small ice pillar that walks along the floor in the direction of the
/// swipe. `count` is how many pillars remain to be spawned; each hit nudges
/// `x` by `step` and arms `timer` before the next one.

s32 func_ov003_020b2710(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EffectState** arg;
    BtlTouchWork*      touch;

    if (!func_ov003_02085cfc(4))
        return 0;
    touch = func_ov003_020a62a0();
    if (touch->input.previousY > touch->input.y)
        return 0;
    arg             = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc948.taskName);
    *arg            = effect;
    context->handle = (TaskHandle*)&data_ov003_020dc948;
    context->arg    = arg;
    context->post   = NULL;
    return 1;
}

s32 func_ov003_020b2780(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    switch (stage) {
        default:
        case 0: {
            Ov003IcePillarS* self  = task->data;
            BtlTouchWork*    touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            self->x      = func_ov003_0208442c(0, touch->input.x);
            self->y      = func_ov003_02084460(0, touch->input.y, 0);
            func_ov003_02083ca0(&self->x, &self->y, 0, 0);
            self->count = (u8)self->effect->params[0];
            self->timer = 0;
            func_ov003_020a6464(1);
            func_ov003_020a5aa4(self->effect);
            if (touch->input.x >= touch->input.previousX)
                self->step = -0x18000;
            else
                self->step = 0x18000;
            func_ov003_02087804(0);
            return 1;
        }
        case 1: {
            Ov003IcePillarS* self = task->data;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            if (self->timer == 0) {
                s32 attackId;
                if (self->count == 0)
                    return 0;
                attackId = data_ov003_020dc954[self->effect->index];
                if (attackId == 0x145 && func_ov003_020a51c0())
                    attackId = 0x147;
                func_ov003_020b1ca0(self->effect, self->x, self->y, attackId);
                func_ov003_020a647c(self->x);
                func_ov003_020a5a94(self->effect);
                func_ov003_02087f28(0x34, self->x);
                self->count--;
                self->timer = 0x10;
                self->x += self->step;
            } else {
                self->timer--;
            }
            return 1;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4d80[0x18] = "Tsk_BtlPsy_IcePillar_S";
const TaskHandle data_ov003_020dc948       = {data_ov003_020e4d80, func_ov003_020b2780, sizeof(Ov003IcePillarS)};
const s32        data_ov003_020dc954[3]    = {0x143, 0x144, 0x145};
