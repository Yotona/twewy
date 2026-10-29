#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dc6e8;

s32 func_ov003_020ab35c(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (func_ov003_020a5b18(0x16) || func_ov003_020a5b18(0x15))
        return 0;
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc6e8.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc6e8;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020ab3f0(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003PsychTrail*   self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            func_ov003_020a4a34(&self->trail);
            func_ov003_020a6464(1);
            func_ov003_020a64ac();
            return 1;
        case 1: {
            Ov003PsychTrail* self = task->data;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            switch (func_ov003_020a4aac(&self->trail)) {
                case 0:
                    func_ov003_020aacd4(self->effect, self->trail.pivotX, self->trail.pivotY, 1, self->effect->params[1],
                                        self->effect->params[2], self->effect->params[3], self->effect->params[0]);
                    return 0;
                case 1:
                    return 1;
                case 2:
                default:
                    return 0;
            }
        }
        case 2:
            self = task->data;
            func_ov003_020a4c8c(&self->trail);
            return 1;
        case 3:
            self = task->data;
            func_ov003_020a4d50(&self->trail);
            func_ov003_020a6464(0);
            return 1;
    }
}

extern char      data_ov003_020e4acc[];
char             data_ov003_020e4ab4[24] = "Tsk_BtlPsy_BlackHoll_Z";
const TaskHandle data_ov003_020dc6e8     = {data_ov003_020e4ab4, func_ov003_020ab3f0, sizeof(Ov003PsychTrail)};
