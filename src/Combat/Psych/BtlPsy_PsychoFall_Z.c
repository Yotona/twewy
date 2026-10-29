#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_0208442c(s32, s32);
extern s32 func_ov003_02084460(s32, s32, s32);
extern s32 func_ov003_020a632c(s32, s32, s32, s32);

/// Drops a PsychoFall falling-sprite sub-task at the trail's current pivot.

extern char             data_ov003_020e4e9c[];
extern const TaskHandle data_ov003_020dca34;

s32 func_ov003_020b4b60(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dca34.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dca34;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020b4bcc(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003PsychoFallZ*  self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->trail.effect = *arg;
            self->phase        = 0;
            self->spawnCount   = 0;
            self->spawnTimer   = 0;
            func_ov003_020a4a34(&self->trail.trail);
            func_ov003_020a6464(1);
            func_ov003_020a64ac();
            return 1;
        case 1: {
            Ov003PsychoFallZ* self = task->data;
            if (func_ov003_020a63a8(self->trail.effect))
                return 0;
            switch (self->phase) {
                case 0:
                    switch (func_ov003_020a4aac(&self->trail.trail)) {
                        case 0:
                            self->phase = 1;
                            self->x     = self->trail.trail.pivotX;
                            self->y     = self->trail.trail.pivotY;
                            func_ov003_020a5aa4(self->trail.effect);
                            func_ov003_02087804(0);
                            return 1;
                        case 1:
                            return 1;
                        case 2:
                        default:
                            return 0;
                    }
                case 1: {
                    if (self->spawnTimer != 0) {
                        self->spawnTimer--;
                        return 1;
                    }
                    self->spawnTimer = 0x3C;
                    func_ov003_020a5a94(self->trail.effect);
                    s32 worldY = func_ov003_02084460(0, self->y, 0);
                    func_ov003_020b42b4(self->trail.effect, 1, func_ov003_0208442c(0, self->x), worldY, -0x190000, 0x5000);
                    // Retail's guard always fires on the first spawn, so the re-roll below is dead
                    // unless the counter wraps; kept to match the original control flow.
                    self->spawnCount++;
                    if (self->spawnCount >= 1)
                        return 0;
                    s32 x;
                    s32 y;
                    do {
                        x = RNG_Next(0xC1) + 0x20;
                        y = RNG_Next(0x81) + 0x20;
                    } while (func_ov003_020a632c(x, y, self->x, self->y) <= 0x40000);
                    self->x = x;
                    self->y = y;
                    return 1;
                }
                default:
                    return 1;
            }
        }
        case 2: {
            Ov003PsychoFallZ* self = task->data;
            if (self->phase == 0)
                func_ov003_020a4c8c(&self->trail.trail);
            return 1;
        }
        case 3: {
            Ov003PsychoFallZ* self = task->data;
            func_ov003_020a4d50(&self->trail.trail);
            func_ov003_020a6464(0);
            return 1;
        }
    }
}

char             data_ov003_020e4e84[0x18] = "Tsk_BtlPsy_PsychoFall_Z";
const TaskHandle data_ov003_020dca34       = {data_ov003_020e4e84, func_ov003_020b4bcc, sizeof(Ov003PsychoFallZ)};
