#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatTouch.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlPsySpinM {
    Ov003EffectState* effect;
    s16               timer;
    s16               pad;
    s32               active;
} BtlPsySpinM;
void func_ov003_02095ed0(BtlPsySpinM* self) {
    if (func_ov003_020a50cc()) {
        func_ov003_02087310();
        func_ov003_02087250(self->effect, data_ov003_020e71b8->unk3D898, 0);
        self->timer = 8;
    }
    if (self->timer > 0)
        self->timer--;
    if (self->timer != 0)
        return;
    func_ov003_02087310();
    self->active = 0;
}
s32 func_ov003_02095f3c(TaskPool* pool, Task* task, void* args) {
    BtlPsySpinM*       self = task->data;
    Ov003EffectState** arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    data_ov003_020e71b8->unk3D8C8 = 1;
    self->effect                  = *arg;
    self->timer                   = 0;
    self->active                  = 1;
    func_ov003_02087250(self->effect, data_ov003_020e71b8->unk3D898, 0);
    return 1;
}
s32 func_ov003_02095fa0(TaskPool* pool, Task* task, void* args) {
    BtlPsySpinM* self = task->data;
    if (!data_ov003_020e71b8->unk3D8C8 || func_ov003_020a63a8(self->effect)) {
        func_ov003_02087310();
        return 0;
    }
    func_ov003_02095ed0(self);
    return self->active;
}
s32 func_ov003_02095ff0(TaskPool* pool, Task* task, void* args) {
    return 1;
}
s32 func_ov003_02095ff8(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32              func_ov003_02096000(TaskPool* pool, Task* task, void* args, s32 stage);
char             data_ov003_020e3c20[24] = "Tsk_BtlPsy_Sword_Spin_M";
const TaskHandle data_ov003_020d7e80     = {data_ov003_020e3c20, func_ov003_02096000, 0xC};
const TaskStages data_ov003_020d7e8c     = {
    {func_ov003_02095f3c, func_ov003_02095fa0, func_ov003_02095ff0, func_ov003_02095ff8}
};
s32 func_ov003_02096000(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7e8c;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_02096048(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a50cc())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020d7e80.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020d7e80;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}
