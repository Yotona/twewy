#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dcea8;
// One contiguous kind table indexed by effect variant.
extern const Ov003StraightShotSKind data_ov003_020dceb4[10];

s32 func_ov003_020b87a0(Ov003EffectContext* context, Ov003EffectState* effect);
s32 func_ov003_020b8ab8(Ov003StraightShotS* self);

s32 func_ov003_020b87a0(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085ec0(0))
        return 0;
    Ov003PsychArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dcea8.taskName);
    arg->effect         = effect;
    context->handle     = &data_ov003_020dcea8;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}

// The phase-1 payload pointer lives in a block-local `state` so mwcc keeps it in r4,
// matching the target's allocation.
// Nonmatching: objdiff reports addend differences for calls to the local volley helper.
s32 func_ov003_020b87f8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*     arg = args;
    Ov003StraightShotS* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->timer  = 5;
            self->angle  = func_ov003_020a62d0(touch->startX, touch->startY, touch->endX, touch->endY);
            self->x      = touch->endX;
            self->angle  = func_ov003_020b6b50(self->angle, data_ov003_020dceb4[self->effect->index].angleLimit);
            self->taskId = -1;
            func_ov003_020a6494(self->x);
            func_ov003_020a6464(1);
            return 1;
        }
        case 1: {
            Ov003StraightShotS* state = task->data;
            if (func_ov003_020a63a8(state->effect) != 0)
                return 0;
            switch (state->phase) {
                case 0:
                    if (--state->timer == 0) {
                        if (!func_ov003_020b8ab8(state)) {
                            state->phase = 2;
                            state->timer = 12;
                            return 1;
                        }
                        state->phase      = 1;
                        state->timer      = 30;
                        state->burstTimer = 10;
                        state->burstCount = 0;
                    }
                    break;
                case 1:
                    if (--state->timer == 0)
                        return 0;
                    s32 relaunch = 0;
                    if (data_ov003_020dceb4[state->effect->index].repeat && state->burstCount < 2 && --state->burstTimer == 0)
                    {
                        relaunch = 1;
                        state->burstCount++;
                    } else if (func_ov003_02085ec0(0) != 0) {
                        BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
                        func_ov003_020a6494(touch->endX);
                        state->angle = func_ov003_020a62d0(touch->startX, touch->startY, touch->endX, touch->endY);
                        state->angle = func_ov003_020b6b50(state->angle, data_ov003_020dceb4[state->effect->index].angleLimit);
                        state->x     = touch->endX;
                        relaunch     = 1;
                        state->burstCount = 0;
                    }
                    if (relaunch != 0) {
                        if (!func_ov003_020b8ab8(state)) {
                            state->phase = 2;
                            state->timer = 12;
                            return 1;
                        }
                        state->timer      = 30;
                        state->burstTimer = 10;
                    }
                    break;
                case 2:
                    if (--state->timer == 0)
                        return 0;
                    break;
            }
            return 1;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

// Nonmatching: the args.kind load is scheduled one instruction earlier than in the target.
// Nonmatching: an independent kind-table load is scheduled one instruction earlier than retail.
s32 func_ov003_020b8ab8(Ov003StraightShotS* self) {
    // Declaration order picks which callee-saved register each live value gets.
    s32                           left;
    CombatActor*                  player;
    const Ov003StraightShotSKind* kind;
    s32                           targetX;
    s32                           originY;
    s32                           originX;
    kind    = &data_ov003_020dceb4[self->effect->index];
    left    = 0;
    player  = data_ov003_020e71b8->unk3D898;
    targetX = func_ov003_0208442c(0, self->x);
    originY = player->position.y;
    if (targetX < player->position.x) {
        originX = player->position.x - 0x18000;
        left    = 1;
    } else {
        originX = player->position.x + 0x18000;
    }
    Ov003ShockWaveArgs args;
    args.effect     = self->effect;
    args.kind       = kind->kind;
    args.position.x = originX;
    args.position.y = originY;
    args.position.z = -0x27000;
    if (kind->directAngle) {
        args.angle = self->angle;
    } else {
        args.angle = (u16)(self->angle - (kind->count / 2) * 0xD00);
    }
    args.unk_18 = 0;
    args.mode   = kind->mode;
    args.flag   = 0;
    args.trail  = 1;
    func_ov003_020b7b34(args.position.x, args.position.y, args.position.z, args.kind, left, &self->taskId);
    for (s32 i = 0; i < kind->count; i++) {
        func_ov003_020b6b20(&args);
        if (!kind->directAngle)
            args.angle = (u16)(args.angle + 0xD00);
        args.trail = 0;
    }
    func_ov003_020a5a94(self->effect);
    return func_ov003_020a5aa4(self->effect);
}

char                         data_ov003_020e4f88[28] = "Tsk_BtlPsy_StraightShot_S";
const TaskHandle             data_ov003_020dcea8     = {data_ov003_020e4f88, func_ov003_020b87f8, sizeof(Ov003StraightShotS)};
const Ov003StraightShotSKind data_ov003_020dceb4[10] = {
    {0x0C, 1, 1, 0, 0, 0x2000},
    {0x0D, 3, 0, 0, 0, 0x2000},
    {0x0D, 3, 0, 0, 1, 0x2000},
    {0x02, 1, 0, 0, 0, 0x2000},
    {0x0C, 3, 1, 0, 0, 0x2000},
    {0x0C, 3, 1, 0, 1, 0x2000},
    {0x0F, 4, 1, 1, 0, 0x2000},
    {0x08, 1, 1, 0, 0, 0x2000},
    {0x01, 1, 0, 0, 0, 0x2000},
    {0x0E, 2, 0, 1, 1, 0x2000},
};
