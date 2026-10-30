#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dc858;

// Nonmatching: Compiler pools the task-handle literal.
s32 func_ov003_020af280(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085ec0(0))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc858.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc858;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

// Nonmatching: Initialization registers and predicated update return differ.
s32 func_ov003_020af2d8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**   arg = args;
    Ov003PsychByteTimer* self;
    switch (stage) {
        default:
        case 0: {
            self                 = task->data;
            BtlTouchWork* touch  = func_ov003_020a62a0();
            CombatActor*  player = func_ov003_020a6438();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            self->timer  = 8;
            func_ov003_020a6464(1);
            Ov003EffectState* effect = self->effect;
            s32               kind   = effect->index == 3 ? 1 : 0;
            s32               angle  = func_ov003_020a62d0(touch->startX, touch->startY, touch->endX, touch->endY);
            func_ov003_020ae414(self->effect, 1, kind, player->screenX, (s16)(player->screenY - player->unk_76), angle,
                                effect->params[1], effect->params[2], effect->params[3]);
            func_ov003_020a6494(touch->endX);
            return 1;
        }
        case 1:
            self = task->data;
            if (--self->timer == 0)
                return 0;
            return func_ov003_020a63a8(self->effect) ? 0 : 1;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

// Descriptor words at 020dc7c0/020dc7c4/020dc7c8 belong to the unified array in
// SubTsk_BtlPsy_Element.c rather than separate, cross-indexed C objects here.
char data_ov003_020e4c1c[24] = "Tsk_BtlPsy_Element_S";

const TaskHandle data_ov003_020dc858 = {data_ov003_020e4c1c, func_ov003_020af2d8, sizeof(Ov003PsychByteTimer)};
