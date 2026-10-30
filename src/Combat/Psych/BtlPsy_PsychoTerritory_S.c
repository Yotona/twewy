#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dd21c;

s32 func_ov003_020baa58(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(4))
        return 0;
    Ov003PsychArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dd21c.taskName);
    arg->effect         = effect;
    context->handle     = &data_ov003_020dd21c;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}

// Nonmatching: midpoint-register allocation and boolean instruction ordering differ.
s32 func_ov003_020baab0(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*      arg = args;
    Ov003PsychByteTimer* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            s16           midX  = touch->startX + (touch->endX - touch->startX) / 2;
            s16           midY  = touch->startY + (touch->endY - touch->startY) / 2;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->timer  = 8;
            s32 y        = func_ov003_02084460(0, midY, 0);
            s32 x        = func_ov003_0208442c(0, midX);
            s32 angle    = func_ov003_020a62d0(touch->startX, touch->startY, touch->endX, touch->endY);
            func_ov003_020ba71c(self->effect, x, y, angle);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->endX);
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            return 1;
        }
        case 1:
            self = task->data;
            if (--self->timer == 0)
                return 0;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            return 1;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e5080[32]    = "Tsk_BtlPsy_PsychoTerritory_S";
const TaskHandle data_ov003_020dd21c        = {data_ov003_020e5080, func_ov003_020baab0, sizeof(Ov003PsychByteTimer)};
const s32        data_ov003_020dd228[13][2] = {
    {0x42, 0x11},
    {0x43, 0x12},
    {0x42, 0x13},
    {0x42, 0x14},
    {0x44, 0x15},
    {0x45, 0x17},
    {0x45, 0x16},
    {0x46, 0x18},
    {0x47, 0x19},
    {0x48, 0x1A},
    {0x49, 0x1B},
    {0x4A, 0x1C},
    {0x4B, 0x1D}
};

const s32 data_ov003_020dd290[39][2] = {
    { 0x0,  0x1},
    { 0x9,  0x0},
    { 0xB,  0x2},
    { 0xE,  0x3},
    {0x11,  0x4},
    {0x13,  0x8},
    {0x17,  0xE},
    {0x19,  0xD},
    {0x1C,  0xC},
    {0x20,  0x9},
    {0x26, 0x10},
    {0x29,  0xF},
    {0x2C,  0x6},
    {0x30,  0xA},
    {0x34, 0x1F},
    {0x39,  0x7},
    {0x3C,  0x5},
    {0x3F,  0xB},
    {0x42, 0x11},
    {0x4C, 0x1E},
    {0x4D, 0x1E},
    {0x4E, 0x1E},
    {0x4F, 0x35},
    {0x50, 0x39},
    {0x51, 0x39},
    {0x52, 0x37},
    {0x53, 0x37},
    {0x54, 0x2B},
    {0x55, 0x2C},
    {0x56, 0x36},
    {0x57, 0x21},
    {0x58, 0x21},
    {0x59, 0x22},
    {0x5A, 0x30},
    {0x5B, 0x2D},
    {0x5C, 0x23},
    {0x5D, 0x31},
    {0x5E, 0x33},
    {0x5F, 0x2F}
};
