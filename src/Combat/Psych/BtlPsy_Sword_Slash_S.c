#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatTouch.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlPsySlashArgs {
    Ov003EffectState* effect;
    void*             target;
    u16               direction;
} BtlPsySlashArgs;
typedef struct BtlPsySlash {
    Ov003EffectState* effect;
    void*             target;
    s16               state;
    s16               timer;
    void (*update)(struct BtlPsySlash*);
    s32 active;
} BtlPsySlash;
u16 func_ov003_02095120(void) {
    Ov003Global* global = data_ov003_020e71b8;
    s32          x      = global->touchWork.startX;
    s32          y      = global->touchWork.startY;
    s32          px     = global->touchWork.endX;
    s32          py     = global->touchWork.endY;
    s32          dy     = y - py;
    s32          dx;
    if (dy < 0)
        dy = -dy;
    dx = x - px;
    if (dx < 0)
        dx = -dx;
    if (dx > dy)
        return x < px ? 1 : 2;
    return y < py ? 0 : 3;
}
void func_ov003_0209519c(BtlPsySlash* self) {
    void* target;
    if (!func_ov003_020a5ab4(self->effect))
        return;
    if (func_ov003_02085d60(self->target)) {
        func_ov003_02087250(self->effect, self->target, func_ov003_02095120());
        return;
    }
    target = func_ov003_02085ec0(1);
    if (!target)
        return;
    func_ov003_02087250(self->effect, self->target, func_ov003_02095120());
    // Retail launches this attack at the old target before saving the replacement.
    self->target = target;
}
s32 func_ov003_02095200(TaskPool* pool, Task* task, void* args) {
    BtlPsySlash*     self = task->data;
    BtlPsySlashArgs* arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    self->effect = arg->effect;
    self->target = arg->target;
    self->active = 1;
    self->update = func_ov003_0209519c;
    self->timer  = 0;
    self->state  = 0;
    func_ov003_02087250(self->effect, self->target, arg->direction);
    data_ov003_020e71b8->unk3D8C8 = 1;
    return 1;
}
s32 func_ov003_02095274(TaskPool* pool, Task* task, void* args) {
    BtlPsySlash* self = task->data;
    if (!data_ov003_020e71b8->unk3D8C8 || func_ov003_020a63a8(self->effect)) {
        self->update = NULL;
        self->timer  = 0;
        self->state  = 0;
        self->active = 0;
    }
    if (self->update)
        self->update(self);
    return self->active;
}
s32 func_ov003_020952d8(TaskPool* pool, Task* task, void* args) {
    return 1;
}
s32 func_ov003_020952e0(TaskPool* pool, Task* task, void* args) {
    data_ov003_020e71b8->unk3D8C8 = 0;
    func_ov003_02087310();
    return 1;
}

s32              func_ov003_02095308(TaskPool* pool, Task* task, void* args, s32 stage);
char             data_ov003_020e3b9c[28] = "Tsk_BtlPsy_Sword_Slash_S";
const TaskHandle data_ov003_020d7df4     = {data_ov003_020e3b9c, func_ov003_02095308, 0x14};
const TaskStages data_ov003_020d7e00     = {
    {func_ov003_02095200, func_ov003_02095274, func_ov003_020952d8, func_ov003_020952e0}
};
s32 func_ov003_02095308(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7e00;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_02095350(Ov003EffectContext* context, Ov003EffectState* effect) {
    void* target = func_ov003_02085ec0(1);
    if (!target)
        return 0;
    BtlPsySlashArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020d7df4.taskName);
    arg->effect          = effect;
    arg->target          = target;
    arg->direction       = func_ov003_02095120();
    context->handle      = &data_ov003_020d7df4;
    context->arg         = arg;
    context->post        = NULL;
    return 1;
}
