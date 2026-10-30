#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dcf2c;

s32 func_ov003_020b8c1c(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dcf2c.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dcf2c;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020b8c74(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003StraightShot* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
            MI_CpuSet(self, 0, sizeof(*self));
            self->phase        = 0;
            self->effect       = *arg;
            self->x            = touch->x;
            self->y            = touch->y;
            self->count        = 0;
            self->timer        = 5;
            self->effectTaskId = -1;
            func_ov003_020a6494(touch->x);
            func_ov003_020a6464(1);
            return 1;
        }
        case 1:
            self = task->data;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            switch (self->phase) {
                case 0:
                    if (--self->timer == 0) {
                        if (!func_ov003_020b8f3c(self)) {
                            self->phase = 2;
                            self->timer = 12;
                            return 1;
                        }
                        self->phase = 1;
                        self->timer = 30;
                    }
                    break;
                case 1:
                    if (--self->timer == 0)
                        return 0;
                    if (func_ov003_02085cfc(2) || func_ov003_02085cfc(3)) {
                        BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
                        func_ov003_020a6494(touch->x);
                        self->x = touch->x;
                        self->y = touch->y;
                        if (!func_ov003_020b8f3c(self)) {
                            self->phase = 2;
                            self->timer = 12;
                            return 1;
                        }
                        self->timer = 30;
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

char                        data_ov003_020e4fa4[28] = "Tsk_BtlPsy_StraightShot_T";
const TaskHandle            data_ov003_020dcf2c     = {data_ov003_020e4fa4, func_ov003_020b8c74, sizeof(Ov003StraightShot)};
const Ov003StraightShotKind data_ov003_020dcf38[10] = {
    { 5,  6, 0, {0}, 0x2000, 1, {0}},
    {10, 10, 0, {0}, 0x2000, 1, {0}},
    { 3,  3, 0, {0}, 0x2000, 1, {0}},
    { 9,  9, 1, {0}, 0x2000, 1, {0}},
    { 9,  4, 1, {0}, 0x2000, 1, {0}},
    { 2,  2, 0, {0}, 0x2000, 1, {0}},
    {16, 16, 1, {0}, 0x2000, 4, {0}},
    { 7,  7, 1, {0}, 0x2000, 1, {0}},
    { 0,  0, 0, {0}, 0x2000, 1, {0}},
    {11, 11, 0, {0}, 0x2000, 1, {0}}
};
