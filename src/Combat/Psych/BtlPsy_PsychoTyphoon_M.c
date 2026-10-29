#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern Ov003EnemyActor* func_ov003_0208495c(CombatList*);
extern Ov003EnemyActor* func_ov003_02084984(CombatListNode*);
extern const TaskHandle data_ov003_020dc8e8;

/// Spawner state of the PsychoTyphoon effect. It walks the sub-screen unit list once and
/// hands the shared release flag at 0x10 to each per-unit sub-effect.

s32 func_ov003_020b0b14(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a50cc())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc8e8.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc8e8;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020b0b68(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**  arg = args;
    Ov003PsychoTyphoon* self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            func_ov003_020a6464(1);
            func_ov003_020a64ac();
            self->phase          = 0;
            self->remainingTicks = self->effect->params[0];
            self->releaseTimer   = 30;
            self->releaseFlag    = 0;
            {
                Ov003EnemyActor* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
                while (unit != NULL) {
                    func_ov003_020b0188(self->effect, unit, &self->releaseFlag);
                    unit = func_ov003_02084984(&unit->node);
                }
            }
            func_ov003_02087804(0);
            return 1;
        case 1:
            self = task->data;
            switch (self->phase) {
                case 0:
                    if (func_ov003_020a63a8(self->effect))
                        self->remainingTicks = 0;
                    if (func_ov003_020a50cc())
                        self->releaseTimer = 30;
                    if (self->releaseTimer <= 0 || self->remainingTicks <= 0) {
                        self->phase       = 1;
                        self->releaseFlag = 1;
                    } else {
                        self->releaseTimer--;
                        self->remainingTicks--;
                    }
                    break;
                case 1:
                    return 0;
            }
            return 1;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4cf0[28] = "Tsk_BtlPsy_PsychoTyphoon_M";
const TaskHandle data_ov003_020dc8e8     = {data_ov003_020e4cf0, func_ov003_020b0b68, sizeof(Ov003PsychoTyphoon)};
