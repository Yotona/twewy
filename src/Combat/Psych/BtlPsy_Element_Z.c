#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dc870;

s32 func_ov003_020af590(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc870.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc870;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020af5fc(TaskPool* pool, Task* task, void* args, s32 stage) {
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
                case 0: {
                    Ov003EffectState* effect = self->effect;
                    s32               index  = effect->index;
                    s32               angle =
                        func_ov003_020a62d0(self->trail.pivotX, self->trail.pivotY, self->trail.points[self->trail.count][0],
                                            self->trail.points[self->trail.count][1]);
                    func_ov003_020ae414(effect, index + 2, index + 4, self->trail.pivotX, self->trail.pivotY, angle,
                                        effect->params[1], effect->params[2], effect->params[3]);
                    return 0;
                }
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

char             data_ov003_020e4c4c[24] = "Tsk_BtlPsy_Element_Z";
const TaskHandle data_ov003_020dc870     = {data_ov003_020e4c4c, func_ov003_020af5fc, sizeof(Ov003PsychTrail)};
