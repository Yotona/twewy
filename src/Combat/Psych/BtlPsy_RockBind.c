#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern CombatActor*     func_ov003_02085dc8(s32);
extern const TaskHandle data_ov003_020dc8c4;

s32 func_ov003_020b0058(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = (Ov003EnemyActor*)func_ov003_02085dc8(1);
    if (unit == NULL)
        return 0;
    if (func_ov003_020a60a8(unit, 3))
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc8c4.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dc8c4;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

// Nonmatching: the final conditional return moves are emitted in reverse order.
s32 func_ov003_020b00c8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs*  arg = args;
    Ov003PsychUnitTimer* self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect = arg->effect;
            self->args.unit   = arg->unit;
            self->timer       = 8;
            func_ov003_020a6464(1);
            func_ov003_020af75c(self->args.effect, self->args.unit, 0, 0x18A);
            return 1;
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

char             data_ov003_020e4ca4[20] = "Tsk_BtlPsy_RockBind";
const TaskHandle data_ov003_020dc8c4     = {data_ov003_020e4ca4, func_ov003_020b00c8, sizeof(Ov003PsychUnitTimer)};
