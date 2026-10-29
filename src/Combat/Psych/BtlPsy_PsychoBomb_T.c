#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern const TaskHandle data_ov003_020dc748;

s32 func_ov003_020ac4f4(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc748.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc748;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

// Nonmatching: the final conditional return emits its two predicated moves in reverse order.
s32 func_ov003_020ac54c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**   arg = args;
    Ov003PsychByteTimer* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            self->timer  = 8;
            func_ov003_020a6464(1);
            s32 y = func_ov003_02084460(0, touch->y, 0x1000);
            s32 x = func_ov003_0208442c(0, touch->x);
            func_ov003_020abe30(self->effect, x, y, 0x1000);
            func_ov003_020a6494(touch->x);
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

char             data_ov003_020e4b5c[24] = "Tsk_BtlPsy_PsychoBomb_T";
const TaskHandle data_ov003_020dc748     = {data_ov003_020e4b5c, func_ov003_020ac54c, sizeof(Ov003PsychByteTimer)};
