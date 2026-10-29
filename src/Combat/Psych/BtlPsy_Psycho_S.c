#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern void             func_ov003_02087f28(s32, s32);
extern const TaskHandle data_ov003_020dca84;

s32 func_ov003_020b5d54(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(4))
        return 0;
    Ov003Unit* unit = func_ov003_020b5f04(effect->index);
    if (unit == NULL)
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dca84.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dca84;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

s32 func_ov003_020b5dc4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs*  arg = args;
    Ov003PsychUnitTimer* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect = arg->effect;
            self->args.unit   = arg->unit;
            self->timer       = 8;
            Ov003PsychEffectArgs effect;
            func_ov003_020a88c0(&effect, self->args.unit->actor.position.x, self->args.unit->actor.position.y,
                                self->args.unit->actor.position.z, 2, 0xAB);
            func_ov003_020a88a8(&effect);
            func_ov003_020a6464(1);
            func_ov003_020a5aa4(self->args.effect);
            func_ov003_020a5a94(self->args.effect);
            func_ov003_020a61e4(self->args.unit, 5, 0x189, &self->args.effect->streak.args, touch->input.angle,
                                self->args.effect->params[0], self->args.effect->params[1]);
            func_ov003_02087f28(0x67, self->args.unit->actor.position.x);
            func_ov003_020a647c(self->args.unit->actor.position.x);
            return 1;
        }
        case 1:
            self = task->data;
            return --self->timer != 0;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

Ov003Unit* func_ov003_020b5f04(s32 index) {
    Ov003Unit* secondary = NULL;
    Ov003Unit* primary   = (Ov003Unit*)func_ov003_02085ec0(2);
    if (primary != NULL && !func_ov003_020a60ec(primary, 5))
        primary = NULL;
    if (index == 2) {
        secondary = (Ov003Unit*)func_ov003_02085ec0(1);
        if (secondary != NULL && !func_ov003_020a60ec(secondary, 5))
            secondary = NULL;
    }
    if (primary != NULL)
        return primary;
    if (secondary != NULL)
        return secondary;
    return NULL;
}

char             data_ov003_020e4ec8[20] = "Tsk_BtlPsy_Psycho_S";
const TaskHandle data_ov003_020dca84     = {data_ov003_020e4ec8, func_ov003_020b5dc4, sizeof(Ov003PsychUnitTimer)};
