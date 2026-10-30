#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlBlackHollCharge {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               timer;
    /* 0x08 */ u8                reserved[0x60];
} BtlBlackHollCharge; // Size: 0x68

extern const TaskHandle data_ov003_020dc6dc;

s32 func_ov003_020ab1c4(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a63e4())
        return 0;
    if (func_ov003_020a5b18(0x16) || func_ov003_020a5b18(0x15))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc6dc.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc6dc;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020ab240(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**  arg = args;
    BtlBlackHollCharge* charge;
    switch (stage) {
        default:
        case 0:
            charge = task->data;
            MI_CpuSet(charge, 0, sizeof(*charge));
            charge->effect = *arg;
            charge->timer  = 0;
            func_ov003_020a6464(1);
            return 1;
        case 1: {
            BtlBlackHollCharge* charge = task->data;
            BtlTouchWork*       touch  = func_ov003_020a62a0();
            if (func_ov003_020a63a8(charge->effect))
                return 0;
            if (!func_ov003_020a63e4())
                return 0;
            func_ov003_020a6494(touch->x);
            charge->timer++;
            if (charge->timer < 60)
                return 1;
            func_ov003_020aacd4(charge->effect, touch->x, touch->y, 0, charge->effect->params[1], charge->effect->params[2],
                                charge->effect->params[3], charge->effect->params[0]);
            return 0;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4a9c[24] = "Tsk_BtlPsy_BlackHoll";
const TaskHandle data_ov003_020dc6dc     = {data_ov003_020e4a9c, func_ov003_020ab240, sizeof(BtlBlackHollCharge)};
