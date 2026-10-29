#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatTouch.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlPsyRushArgs {
    Ov003EffectState* effect;
    void*             target;
} BtlPsyRushArgs;
typedef struct BtlPsyRush {
    Ov003EffectState* effect;
    void*             target;
    s16               count;
    s16               unk_0A[4];
    s16               timer;
    s32               active;
} BtlPsyRush;
void func_ov003_02095b1c(BtlPsyRush* self) {
    if (self->timer == 0) {
        if (self->count >= 9) {
            func_ov003_02087310();
            func_ov003_02087250(self->effect, self->target, 1);
            self->active = 0;
            return;
        }
        func_ov003_02087310();
        func_ov003_02087250(self->effect, self->target, 0);
        self->count++;
        self->timer = 15;
    }
    self->timer--;
    if (data_ov003_020e71b8->touchInput.tapCount != 0)
        return;
    func_ov003_02087310();
}
s32 func_ov003_02095bb0(TaskPool* pool, Task* task, void* args) {
    BtlPsyRush*     self = task->data;
    BtlPsyRushArgs* arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    data_ov003_020e71b8->unk3D8C8 = 1;
    self->effect                  = arg->effect;
    self->target                  = arg->target;
    self->count                   = 0;
    self->active                  = 1;
    return 1;
}
s32 func_ov003_02095c04(TaskPool* pool, Task* task, void* args) {
    BtlPsyRush* self = task->data;
    if (!data_ov003_020e71b8->unk3D8C8 || func_ov003_020a63a8(self->effect)) {
        func_ov003_02087310();
        return 0;
    }
    func_ov003_02095b1c(self);
    return self->active;
}
s32 func_ov003_02095c54(TaskPool* pool, Task* task, void* args) {
    return 1;
}
s32 func_ov003_02095c5c(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32              func_ov003_02095c64(TaskPool*, Task*, void*, s32);
char             data_ov003_020e3bf0[24] = "Tsk_BtlPsy_Sword_Rush_R";
const TaskHandle data_ov003_020d7e48     = {data_ov003_020e3bf0, func_ov003_02095c64, 0x18};
const TaskStages data_ov003_020d7e54     = {
    {func_ov003_02095bb0, func_ov003_02095c04, func_ov003_02095c54, func_ov003_02095c5c}
};
s32 func_ov003_02095c64(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7e54;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_02095cac(Ov003EffectContext* context, Ov003EffectState* effect) {
    void* target = func_ov003_02086014(1);
    if (!target)
        return 0;
    BtlPsyRushArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020d7e48.taskName);
    arg->effect         = effect;
    arg->target         = target;
    context->handle     = (TaskHandle*)&data_ov003_020d7e48;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}
