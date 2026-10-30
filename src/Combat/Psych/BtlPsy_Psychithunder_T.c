#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dd068;

s32 func_ov003_020b9838(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003PsychArgs  value = {effect};
    Ov003PsychArgs* arg   = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dd068.taskName);
    *arg                  = value;
    context->handle       = &data_ov003_020dd068;
    context->arg          = arg;
    context->post         = NULL;
    return 1;
}

s32 func_ov003_020b9898(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*     arg = args;
    Ov003Psychithunder* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = arg->effect;
            self->x          = touch->x;
            self->y          = touch->y;
            self->taskIds[0] = -1;
            self->taskIds[1] = -1;
            func_ov003_020a6494(touch->x);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
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
                        func_ov003_020b9a14(self);
                        func_ov003_020a5aa4(self->effect);
                        self->phase = 1;
                    }
                    break;
                case 1:
                    if (!func_ov003_020a603c(&self->taskIds[0]))
                        return 0;
                    break;
            }
            return 1;
        case 2:
            return 1;
        case 3: {
            Ov003Psychithunder* data = task->data;
            for (s32 i = 0; i < 2; i++) {
                if (data->taskIds[i] != (u32)-1 && func_ov003_020a603c(&data->taskIds[i]))
                    func_ov003_020a6060(data->taskIds[i]);
            }
            func_ov003_020a6464(0);
            return 1;
        }
    }
}

void func_ov003_020b9a14(Ov003Psychithunder* self) {
    CombatActor*      player  = func_ov003_020a6438();
    s32               playerY = player->screenY;
    s32               x;
    s32               targetX = self->x;
    s32               targetY = self->y;
    Ov003EffectState* effect  = self->effect;
    if (targetX < player->screenX)
        x = player->screenX - 24;
    else
        x = player->screenX + 24;
    s32 y        = playerY - 39;
    s32 lifetime = effect->params[0];
    if (effect->index == 2) {
        s32 angle        = func_ov003_020a62d0(x, y, targetX, targetY);
        u16 first        = angle + 0x1000;
        s32 dx           = FX_Mul(FX_CosIdx(first), 0x64000);
        s32 dy           = FX_Mul(FX_SinIdx(first), 0x64000);
        self->taskIds[0] = func_ov003_020b9040(self->effect, 3, x, y, x + (dx >> 12), y + (dy >> 12), lifetime);
        u16 second       = angle - 0x1000;
        dx               = FX_Mul(FX_CosIdx(second), 0x64000);
        dy               = FX_Mul(FX_SinIdx(second), 0x64000);
        self->taskIds[1] = func_ov003_020b9040(self->effect, 3, x, y, x + (dx >> 12), y + (dy >> 12), lifetime);
        return;
    }
    self->taskIds[0] = func_ov003_020b9040(effect, 3, x, y, targetX, targetY, lifetime);
}

extern char      data_ov003_020e4fdc[28];
char             data_ov003_020e4ff8[28] = "Task_BtlPsy_Psychithunder_T";
const TaskHandle data_ov003_020dd05c     = {data_ov003_020e4fdc, func_ov003_020b9518, sizeof(Ov003LineThunder)};
const TaskHandle data_ov003_020dd068     = {data_ov003_020e4ff8, func_ov003_020b9898, sizeof(Ov003Psychithunder)};
