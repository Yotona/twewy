#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatTouch.h"
#include <nitro/mi/cpumem.h>

s32 func_ov003_02095d08(TaskPool* pool, Task* task, void* args) {
    Ov003PsychTrail*   self = task->data;
    Ov003EffectState** arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    self->effect = *arg;
    func_ov003_020a4a34(&self->trail);
    data_ov003_020e71b8->unk3D8C8 = 1;
    return 1;
}
s32 func_ov003_02095d50(TaskPool* pool, Task* task, void* args) {
    Ov003PsychTrail* self = task->data;
    if (func_ov003_020a63a8(self->effect))
        return 0;
    switch (func_ov003_020a4aac(&self->trail)) {
        case 0:
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            func_ov003_02087250(self->effect, data_ov003_020e71b8->unk3D898, 0);
            return 0;
        case 1:
            return 1;
        case 2:
            break;
    }
    return 0;
}
s32 func_ov003_02095dd4(TaskPool* pool, Task* task, void* args) {
    Ov003PsychTrail* self = task->data;
    func_ov003_020a4c8c(&self->trail);
    return 1;
}
s32 func_ov003_02095dec(TaskPool* pool, Task* task, void* args) {
    Ov003PsychTrail* self = task->data;
    func_ov003_020a4d50(&self->trail);
    data_ov003_020e71b8->unk3D8C8 = 0;
    return 1;
}

s32              func_ov003_02095e1c(TaskPool* pool, Task* task, void* args, s32 stage);
char             data_ov003_020e3c08[24] = "Tsk_BtlPsy_Sword_Spin_Z";
const TaskHandle data_ov003_020d7e64     = {data_ov003_020e3c08, func_ov003_02095e1c, 0x2B4};
const TaskStages data_ov003_020d7e70     = {
    {func_ov003_02095d08, func_ov003_02095d50, func_ov003_02095dd4, func_ov003_02095dec}
};
s32 func_ov003_02095e1c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7e70;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_02095e64(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020d7e64.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020d7e64;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}
