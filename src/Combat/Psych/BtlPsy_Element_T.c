#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern char             data_ov003_020e4c1c[24];
extern const TaskHandle data_ov003_020dc864;

s32 func_ov003_020af41c(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003ElementTArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc864.taskName);
    arg->effect            = effect;
    arg->kind              = effect->index == 4 ? 3 : 2;
    context->handle        = &data_ov003_020dc864;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

// Nonmatching: the final predicated return moves are emitted in reverse order.
s32 func_ov003_020af488(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003ElementTArgs* arg = args;
    Ov003ElementT*     self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect = arg->effect;
            self->args.kind   = arg->kind;
            self->timer       = 10;
            func_ov003_020a6464(1);
            Ov003EffectState* effect = self->args.effect;
            func_ov003_020ae414(effect, 0, self->args.kind, touch->x, touch->y, 0, 0, effect->params[1], effect->params[2]);
            func_ov003_020a6494(touch->x);
            return 1;
        }
        case 1:
            self = task->data;
            if (--self->timer == 0)
                return 0;
            return func_ov003_020a63a8(self->args.effect) ? 0 : 1;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4c34[24] = "Tsk_BtlPsy_Element_T";
const TaskHandle data_ov003_020dc864     = {data_ov003_020e4c34, func_ov003_020af488, sizeof(Ov003ElementT)};
