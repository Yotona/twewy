#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32                      func_ov003_0208442c(s32, s32);
extern s32                      func_ov003_02084460(s32, s32, s32);
extern s32                      func_ov003_020a62b8(s32, s32, s32, s32);
extern const TaskHandle         data_ov003_020dce28;
extern const Ov003BoomerangKind data_ov003_020dce34[3];

s32 func_ov003_020b7f9c(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003PsychArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dce28.taskName);
    arg->effect         = effect;
    context->handle     = (TaskHandle*)&data_ov003_020dce28;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}

s32 func_ov003_020b7ff4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*      arg = args;
    Ov003BoomerangShotT* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->x      = touch->x;
            self->y      = touch->y;
            self->timer  = 5;
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
                        if (!func_ov003_020b81cc(self)) {
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
                        if (!func_ov003_020b81cc(self)) {
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

s32 func_ov003_020b81cc(Ov003BoomerangShotT* self) {
    s32          originY;
    CombatActor* player;
    s32          angle;
    s32          targetY;
    s32          originX;
    s32          targetX;
    s32          i;
    s32          count;
    s32          left;
    s32          distance;
    left    = 0;
    count   = self->effect->params[0];
    targetX = func_ov003_0208442c(0, self->x);
    targetY = func_ov003_02084460(0, self->y, -0x27000);
    player  = data_ov003_020e71b8->unk3D898;
    if (targetX < player->position.x) {
        originX = player->position.x - 0x18000;
        left    = 1;
    } else {
        originX = player->position.x + 0x18000;
    }
    originY = player->position.y;
    angle   = func_ov003_020a62b8(originX, originY, targetX, targetY);
    if (!left) {
        if ((player->position.x >> 12) < (targetX >> 12) && (targetX >> 12) <= (originX >> 12))
            angle = 0;
    } else {
        if ((player->position.x >> 12) > (targetX >> 12) && (targetX >> 12) >= (originX >> 12))
            angle = 0x8000;
    }
    distance = func_ov003_020a62ec(originX, originY, targetX, targetY);
    angle    = func_ov003_020b6b50(angle, data_ov003_020dce34[self->effect->index].angleLimit);
    angle    = (u16)(angle - (count / 2) * 0x1800);
    Ov003ShockWaveArgs args;
    args.effect     = self->effect;
    args.kind       = data_ov003_020dce34[self->effect->index].kind;
    args.position.x = originX;
    args.position.y = originY;
    args.position.z = -0x27000;
    args.unk_18     = distance;
    args.mode       = 1;
    args.flag       = 0;
    args.trail      = 1;
    for (i = 0; i < count; i++) {
        args.angle = angle;
        func_ov003_020b6b20(&args);
        angle      = (u16)(angle + 0x1800);
        args.trail = 0;
    }
    func_ov003_020b7b34(originX, originY, -0x27000, args.kind, left, NULL);
    func_ov003_020a5a94(self->effect);
    return func_ov003_020a5aa4(self->effect);
}

char                     data_ov003_020e4f54[28] = "Tsk_BtlPsy_BoomerangShot_T";
const TaskHandle         data_ov003_020dce28     = {data_ov003_020e4f54, func_ov003_020b7ff4, sizeof(Ov003BoomerangShotT)};
const Ov003BoomerangKind data_ov003_020dce34[3]  = {
    {17, 0x2000},
    {17, 0x2000},
    {18, 0x2000}
};
