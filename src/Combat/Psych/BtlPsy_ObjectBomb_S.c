#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "SndMgrSeIdx.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlObjectBombCharge {
    /* 0x00 */ Ov003PsychUnitArgs args;
    /* 0x08 */ s32                timer;
} BtlObjectBombCharge; // Size: 0xC

extern const TaskHandle data_ov003_020dc73c;

s32 func_ov003_020ac36c(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = (Ov003EnemyActor*)func_ov003_02085e78(2);
    if (unit == NULL || !func_ov003_020a63e4())
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc73c.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = &data_ov003_020dc73c;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

s32 func_ov003_020ac3d8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs*  arg = args;
    BtlObjectBombCharge* self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect = arg->effect;
            self->args.unit   = arg->unit;
            self->timer       = 0;
            func_ov003_020a6464(1);
            func_ov003_020a647c(self->args.unit->actor.position.x);
            func_ov003_02087f28(0x40, self->args.unit->actor.position.x);
            return 1;
        case 1: {
            BtlObjectBombCharge* self = task->data;
            if (!func_ov003_020a63e4())
                return 0;
            if (func_ov003_020a63a8(self->args.effect))
                return 0;
            if (++self->timer >= 0) {
                func_ov003_020abb34(self->args.effect, self->args.unit);
                func_ov003_020a5aa4(self->args.effect);
                func_ov003_020a5a94(self->args.effect);
                return 0;
            }
            if (!SndMgr_IsSEPlaying(SEIDX_SE_PSY_OBJBOMB01_01))
                func_ov003_02087f28(0x40, self->args.unit->actor.position.x);
            return 1;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_02087ed8(SEIDX_SE_PSY_OBJBOMB01_01_CANCEL);
            func_ov003_020a6464(0);
            return 1;
    }
}

char             data_ov003_020e4b44[24] = "Tsk_BtlPsy_ObjectBomb_S";
const TaskHandle data_ov003_020dc73c     = {data_ov003_020e4b44, func_ov003_020ac3d8, sizeof(BtlObjectBombCharge)};
