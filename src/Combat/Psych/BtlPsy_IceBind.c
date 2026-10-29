#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dc8b8;

s32 func_ov003_020afdf0(Ov003EffectContext* context, Ov003EffectState* effect) {
    BtlTouchWork* touch = func_ov003_020a62a0();
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EnemyActor* unit = (Ov003EnemyActor*)touch->heldActor;
    if (unit == NULL || &unit->actor != touch->candidateActor || unit->actor.unk_00 != 1 || (unit->actor.flags & 0x10))
        return 0;
    if (func_ov003_020a60a8(unit, 3))
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc8b8.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dc8b8;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

s32 func_ov003_020afeb4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs* arg = args;
    Ov003IceBind*       self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect   = arg->effect;
            self->args.unit     = arg->unit;
            self->frames        = 0;
            self->particleTimer = 0;
            func_ov003_020a6464(1);
            return 1;
        case 1: {
            Ov003IceBind* self  = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            CombatActor*  held  = touch->heldActor;
            if (!func_ov003_02085cfc(6))
                return 0;
            if (func_ov003_020a63a8(self->args.effect))
                return 0;
            if (held != &self->args.unit->actor)
                return 0;
            if (++self->frames == 30) {
                func_ov003_020af75c(self->args.effect, self->args.unit, 1, self->args.effect->index + 0x18B);
                return 0;
            }
            if (self->particleTimer == 0) {
                s32 kind  = RNG_Next(3);
                s32 angle = RNG_Next(0x5000);
                func_ov003_020afb64(kind, touch->x, touch->y, angle + 0x1800, 0x1000);
                if (self->frames < 15)
                    self->particleTimer = 1;
                else if (self->frames < 21)
                    self->particleTimer = 2;
                else
                    self->particleTimer = 3;
            } else {
                self->particleTimer--;
            }
            func_ov003_020a647c(self->args.unit->actor.position.x);
            return 1;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4c90[20] = "Tsk_BtlPsy_IceBind";
const TaskHandle data_ov003_020dc8b8     = {data_ov003_020e4c90, func_ov003_020afeb4, sizeof(Ov003IceBind)};
