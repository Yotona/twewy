#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern const s16        FX_SinCosTable_[];
extern const TaskHandle data_ov003_020dca28;

/// Spawner state of the PsychoFall effect: positions itself on the touch point and drops one
/// meteor shard per `effect->params[0]` while the touch is held.

s32 func_ov003_020b47fc(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dca28.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dca28;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

// objdiff flags the two intra-TU `bl func_ov003_020b4a44` relocations only because this object
// emits one .text section per function while the delinked target keeps a single one; the encoded
// instructions (and their post-link displacement) are identical.
s32 func_ov003_020b4854(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003PsychoFallT*  self;
    BtlTouchWork*      touch;
    switch (stage) {
        default:
        case 0:
            self  = task->data;
            touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->state  = 0;
            self->effect = *arg;
            self->x      = touch->x;
            self->y      = touch->y;
            self->timer  = 1;
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
            func_ov003_02087f00(0xAB, self->x);
            func_ov003_02087804(0);
            return 1;
        case 1:
            self = task->data;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            func_ov003_020a5a94(self->effect);
            switch (self->state) {
                case 0:
                    if (--self->timer == 0) {
                        if (!func_ov003_020b4a44(self)) {
                            self->state = 2;
                            self->timer = 0xC;
                            return 1;
                        }
                        self->state = 1;
                        self->timer = 0x2D;
                    }
                    break;
                case 1:
                    if (--self->timer == 0)
                        return 0;
                    if (func_ov003_02085cfc(2) || func_ov003_02085cfc(3)) {
                        BtlTouchWork* held = &data_ov003_020e71b8->touchWork;
                        func_ov003_020a6494(held->x);
                        self->x = held->x;
                        self->y = held->y;
                        if (!func_ov003_020b4a44(self)) {
                            self->state = 2;
                            self->timer = 0xC;
                            return 1;
                        }
                        self->timer = 0x2D;
                    }
                    break;
                case 2:
                    if (--self->timer == 0)
                        return 0;
                    break;
            }
            return 1;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

/// Spawns `effect->params[0]` falling shards around the anchored position; the first one lands
/// dead centre and the rest are scattered along a random horizontal angle.
s32 func_ov003_020b4a44(Ov003PsychoFallT* self) {
    s32 i;
    s32 count = self->effect->params[0];

    for (i = 0; i < count; i++) {
        s32 x = func_ov003_0208442c(0, self->x);
        s32 y = func_ov003_02084460(0, self->y, 0);
        s32 z = -0x190000;
        if (i > 0) {
            s32 angle = RNG_Next(0x20) << 11;
            x += (s32)(((s64)FX_SinCosTable_[(angle >> 4) * 2 + 1] * 0x20000 + 0x800) >> 0xC);
            y += (s32)(((s64)FX_SinCosTable_[(angle >> 4) * 2] * 0x20000 + 0x800) >> 0xC);
            z += (RNG_Next(0x21) - 0x40) * 4096;
        }
        func_ov003_020b42b4(self->effect, 0, x, y, z, 0x5000);
    }
    return func_ov003_020a5aa4(self->effect);
}

char             data_ov003_020e4e6c[24] = "Tsk_BtlPsy_PsychoFall_T";
const TaskHandle data_ov003_020dca28     = {data_ov003_020e4e6c, func_ov003_020b4854, sizeof(Ov003PsychoFallT)};
