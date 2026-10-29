#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern CombatActor*     func_ov003_02085dc8(s32);
extern const TaskHandle data_ov003_020dc99c;

s32 func_ov003_020b3da8(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = (Ov003EnemyActor*)func_ov003_02085dc8(1);
    if (unit == NULL)
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc99c.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dc99c;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

s32 func_ov003_020b3e04(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs*  arg = args;
    Ov003LockOnThunderT* self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            func_ov003_020b2fd8(&self->targets);
            self->launched  = 0;
            self->finished  = 0;
            self->cancelled = 0;
            func_ov003_020b3014(&self->targets, &arg->unit->actor);
            self->ready = func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            self->timer = 60;
            func_ov003_020a6464(1);
            func_ov003_020a647c(arg->unit->actor.position.x);
            return 1;
        case 1:
            self = task->data;
            if (self->launched)
                return func_ov003_020b3fac(self);
            return func_ov003_020b3edc(self);
        case 2:
            return 1;
        case 3:
            self = task->data;
            func_ov003_020b2fe4(&self->targets);
            return 1;
    }
}

s32 func_ov003_020b3edc(Ov003LockOnThunderT* self) {
    if (!self->ready || func_ov003_020a63a8(self->effect)) {
        func_ov003_020a6464(0);
        self->cancelled = 1;
    }
    if (--self->timer == 0 || self->cancelled) {
        if (func_ov003_020b30d8(&self->targets)) {
            func_ov003_020a6464(0);
            return 0;
        }
        func_ov003_020b3108(self->effect, &self->targets, &self->finished);
        self->launched = 1;
        return 1;
    }
    CombatActor* actor = func_ov003_02085dc8(1);
    if (actor != NULL) {
        func_ov003_020b3014(&self->targets, actor);
        self->ready = func_ov003_020a5aa4(self->effect);
        func_ov003_020a5a94(self->effect);
        func_ov003_020a647c(actor->position.x);
    }
    return 1;
}

s32 func_ov003_020b3fac(Ov003LockOnThunderT* self) {
    if (!self->cancelled) {
        if (self->finished) {
            func_ov003_020a6464(0);
            return 0;
        }
        if (!self->ready || func_ov003_020a63a8(self->effect)) {
            func_ov003_020a6464(0);
            self->cancelled = 1;
        } else {
            CombatActor* actor = func_ov003_02085dc8(1);
            if (actor != NULL) {
                func_ov003_020b3014(&self->targets, actor);
                self->ready = func_ov003_020a5aa4(self->effect);
            }
        }
    } else if (self->finished) {
        return 0;
    }
    return 1;
}

char             data_ov003_020e4e18[28] = "Tsk_BtlPsy_LockOnThunder_T";
const TaskHandle data_ov003_020dc99c     = {data_ov003_020e4e18, func_ov003_020b3e04, sizeof(Ov003LockOnThunderT)};
