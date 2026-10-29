#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlFireWorksCharge {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               timer;
    /* 0x08 */ s32               reserved;
} BtlFireWorksCharge; // Size: 0xC

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern const TaskHandle data_ov003_020dc730;

s32 func_ov003_020ac1ec(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a63e4())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc730.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc730;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020ac240(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**  arg = args;
    BtlFireWorksCharge* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect   = *arg;
            self->timer    = 0;
            self->reserved = 0;
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
            func_ov003_02087f00(0x52, touch->x);
            return 1;
        }
        case 1: {
            BtlFireWorksCharge* self = task->data;
            if (!func_ov003_020a63e4())
                return 0;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            self->timer++;
            if (self->timer < 60)
                return 1;
            BtlTouchWork* touch = func_ov003_020a62a0();
            s32           y     = func_ov003_02084460(0, touch->y, 0x1000);
            s32           x     = func_ov003_0208442c(0, touch->x);
            func_ov003_020ab7f8(self->effect, x, y, 0x1000);
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            return 0;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_02087ed8(0x53);
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4b2c[24] = "Tsk_BtlPsy_FireWorks_P";
const TaskHandle data_ov003_020dc730     = {data_ov003_020e4b2c, func_ov003_020ac240, sizeof(BtlFireWorksCharge)};
