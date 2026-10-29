#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dd05c;

// Nonmatching: the compiler merges two identical task-handle literals.
s32 func_ov003_020b94b8(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(4))
        return 0;
    Ov003PsychArgs  value = {effect};
    Ov003PsychArgs* arg   = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dd05c.taskName);
    *arg                  = value;
    context->handle       = (TaskHandle*)&data_ov003_020dd05c;
    context->arg          = arg;
    context->post         = NULL;
    return 1;
}

// Nonmatching: local call relocation representation differs in objdiff.
s32 func_ov003_020b9518(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*   arg = args;
    Ov003LineThunder* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect    = arg->effect;
            self->x         = touch->input.x;
            self->y         = touch->input.y;
            self->previousX = touch->input.previousX;
            self->previousY = touch->input.previousY;
            self->timer     = 5;
            func_ov003_020a6494(touch->input.previousX);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->input.previousX);
            self->phase = 0;
            self->timer = 5;
            return 1;
        }
        case 1:
            self = task->data;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            func_ov003_020a5a94(self->effect);
            switch (self->phase) {
                case 0:
                    if (--self->timer == 0) {
                        func_ov003_020b96d8(self);
                        if (!func_ov003_020a5aa4(self->effect))
                            return 0;
                        self->phase = 1;
                        self->timer = 30;
                    }
                    break;
                case 1:
                    if (--self->timer == 0)
                        return 0;
                    if (func_ov003_02085cfc(4)) {
                        BtlTouchWork* touch = func_ov003_020a62a0();
                        self->x             = touch->input.x;
                        self->y             = touch->input.y;
                        self->previousX     = touch->input.previousX;
                        self->previousY     = touch->input.previousY;
                        func_ov003_020b96d8(self);
                        if (!func_ov003_020a5aa4(self->effect))
                            return 0;
                        func_ov003_020a6494(touch->input.previousX);
                        self->phase = 1;
                        self->timer = 30;
                    }
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

// Nonmatching: local call relocation representation differs in objdiff.
void func_ov003_020b96d8(Ov003LineThunder* self) {
    s32 x, y;
    func_ov003_020b9764(&x, &y, self->x, self->y, self->previousX, self->previousY);
    Ov003EffectState* effect    = self->effect;
    s32               previousX = self->previousX;
    s32               previousY = self->previousY;
    s32               lifetime  = effect->params[0];
    s32               kind;
    switch (effect->index) {
        default:
        case 0:
        case 1:
            kind = 0;
            break;
        case 2:
            kind = 1;
            break;
        case 3:
            kind = 2;
            break;
    }
    func_ov003_020b9040(effect, kind, x, y, previousX, previousY, lifetime);
}

// Nonmatching: coordinate/register allocation and conditional-add scheduling differ.
void func_ov003_020b9764(s32* outX, s32* outY, s32 x1, s32 y1, s32 x2, s32 y2) {
    s32 startX = x1 * 0x1000;
    s32 startY = y1 * 0x1000;
    s32 endX   = x2 * 0x1000;
    y2 *= 0x1000;
    if (startX == endX)
        endX += 0x19A;
    if (startY == y2)
        y2 += 0x19A;
    s32 dx        = endX - startX;
    s32 dy        = y2 - startY;
    s32 slope     = FX_Divide(dy, dx);
    s32 intercept = y2 - FX_Mul(slope, endX);
    s32 x         = 0;
    if (dx <= 0)
        x = 0x100000;
    s32 y = intercept + FX_Mul(slope, x);
    if (y < 0 || y > 0xC0000) {
        if (dy > 0)
            y = 0;
        else
            y = 0xC0000;
        x = FX_Divide(y - intercept, slope);
    }
    *outX = x >> 12;
    *outY = y >> 12;
}

char data_ov003_020e4fdc[28] = "Task_BtlPsy_LineThunder_S";
