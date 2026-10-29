#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32                      func_ov003_0208442c(s32, s32);
extern const TaskHandle         data_ov003_020dce04;
extern const Ov003BoomerangKind data_ov003_020dce10[3];

s32 func_ov003_020b7bf4(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085ec0(0))
        return 0;
    Ov003PsychArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dce04.taskName);
    arg->effect         = effect;
    context->handle     = (TaskHandle*)&data_ov003_020dce04;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}

s32 func_ov003_020b7c4c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*      arg = args;
    Ov003BoomerangShotS* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->timer  = 5;
            self->angle  = func_ov003_020a62d0(touch->input.x, touch->input.y, touch->input.previousX, touch->input.previousY);
            self->x      = touch->input.previousX;
            self->angle  = func_ov003_020b6b50(self->angle, data_ov003_020dce10[self->effect->index].angleLimit);
            func_ov003_020a6494(touch->input.previousX);
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
                        if (!func_ov003_020b7e68(self)) {
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
                    if (func_ov003_02085ec0(0)) {
                        BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
                        func_ov003_020a6494(touch->input.previousX);
                        self->angle = func_ov003_020a62d0(touch->input.x, touch->input.y, touch->input.previousX,
                                                          touch->input.previousY);
                        self->angle = func_ov003_020b6b50(self->angle, data_ov003_020dce10[self->effect->index].angleLimit);
                        self->x     = touch->input.previousX;
                        if (!func_ov003_020b7e68(self)) {
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

s32 func_ov003_020b7e68(Ov003BoomerangShotS* self) {
    s32          left;
    CombatActor* player;
    s32          targetX;
    s32          originY;
    s32          originX;
    s32          i;
    s32          count;
    left    = 0;
    player  = data_ov003_020e71b8->unk3D898;
    count   = self->effect->params[0];
    targetX = func_ov003_0208442c(0, self->x);
    originY = player->position.y;
    if (targetX < player->position.x) {
        originX = player->position.x - 0x18000;
        left    = 1;
    } else {
        originX = player->position.x + 0x18000;
    }
    self->angle = (u16)(self->angle - (count / 2) * 0x1000);
    Ov003ShockWaveArgs args;
    args.effect     = self->effect;
    args.kind       = data_ov003_020dce10[self->effect->index].kind;
    args.position.x = originX;
    args.position.y = originY;
    args.position.z = -0x27000;
    args.unk_18     = 0;
    args.mode       = 1;
    args.flag       = 0;
    args.trail      = 1;
    for (i = 0; i < count; i++) {
        args.angle = self->angle;
        func_ov003_020b6b20(&args);
        self->angle = (u16)(self->angle + 0x1000);
        args.trail  = 0;
    }
    func_ov003_020b7b34(originX, originY, -0x27000, args.kind, left, NULL);
    func_ov003_020a5a94(self->effect);
    return func_ov003_020a5aa4(self->effect);
}

char                     data_ov003_020e4f38[28] = "Tsk_BtlPsy_BoomerangShot_S";
const TaskHandle         data_ov003_020dce04     = {data_ov003_020e4f38, func_ov003_020b7c4c, sizeof(Ov003BoomerangShotS)};
const Ov003BoomerangKind data_ov003_020dce10[3]  = {
    {19, 0x2000},
    {19, 0x2000},
    {20, 0x2000}
};
