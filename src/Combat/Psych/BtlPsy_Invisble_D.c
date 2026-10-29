#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatTouch.h"
#include <nitro/mi/cpumem.h>

#include "SpriteMgr.h"
extern const BinIdentifier data_ov003_020d77f8;
extern s16                 func_ov003_020843b0(s32, s32);
extern s16                 func_ov003_020843ec(s32, s32, s32);
typedef struct BtlPsyInvisible {
    Ov003EffectState* effect;
    CombatSprite      sprite;
    s32               x;
    s32               y;
    void (*update)(struct BtlPsyInvisible*);
    s16 state;
    s16 timer;
    s32 visible;
    s32 active;
} BtlPsyInvisible;
void func_ov003_02094e40(BtlPsyInvisible*);
void func_ov003_02094edc(BtlPsyInvisible*);
s32  func_ov003_02094f24(TaskPool*, Task*, void*);
s32  func_ov003_02094fb8(TaskPool*, Task*, void*);
s32  func_ov003_02094fe4(TaskPool*, Task*, void*);
s32  func_ov003_0209504c(TaskPool*, Task*, void*);
s32  func_ov003_02095064(TaskPool*, Task*, void*, s32);

char             data_ov003_020e3b84[24] = "Tsk_BtlPsy_Invisble_D";
const TaskHandle data_ov003_020d7dc8     = {data_ov003_020e3b84, func_ov003_02095064, 0x7C};
// Nonmatching: mwcc places the animation table before this stage table in .rodata.
const TaskStages data_ov003_020d7dd4 = {
    {func_ov003_02094f24, func_ov003_02094fb8, func_ov003_02094fe4, func_ov003_0209504c}
};
const SpriteAnimEntry data_ov003_020d7de4[2] = {
    {0x35, 0x37, 0x36, 0},
    {0x35, 0x37, 0x36, 1}
};
void func_ov003_02094da0(BtlPsyInvisible* self, void (*update)(BtlPsyInvisible*)) {
    self->update = update;
    self->state  = 0;
    self->timer  = 0;
}
void func_ov003_02094db4(BtlPsyInvisible* self) {
    if (self->state == 0) {
        self->x = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x;
        self->y = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
        CombatSprite_SetAnim(&self->sprite, 0, 1);
        self->state++;
    }
    if (!SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
        return;
    self->visible = 0;
    func_ov003_02094da0(self, func_ov003_02094e40);
}
void func_ov003_02094e40(BtlPsyInvisible* self) {
    func_ov003_020a5a94(self->effect);
    if (func_ov003_02085cfc(6) && func_ov003_020a5aa4(self->effect) && !func_ov003_020a63a8(self->effect))
        return;
    data_ov003_020e71b8->unk3D8C8 = 0;
    self->visible                 = 1;
    self->x                       = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x;
    self->y                       = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
    func_ov003_02094da0(self, func_ov003_02094edc);
}
void func_ov003_02094edc(BtlPsyInvisible* self) {
    if (self->state == 0) {
        CombatSprite_SetAnim(&self->sprite, 1, 1);
        self->state++;
    }
    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
        self->active = 0;
}
extern const SpriteAnimEntry data_ov003_020d7de4[2];
s32                          func_ov003_02094f24(TaskPool* pool, Task* task, void* args) {
    BtlPsyInvisible*   self = task->data;
    Ov003EffectState** arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    data_ov003_020e71b8->unk3D8C8 = 1;
    self->effect                  = *arg;
    self->visible                 = 1;
    self->active                  = 1;
    func_ov003_02094da0(self, func_ov003_02094db4);
    CombatSprite_LoadFromTable(0, &self->sprite, &data_ov003_020d77f8, data_ov003_020d7de4, 0, 1, 0x30);
    return 1;
}
s32 func_ov003_02094fb8(TaskPool* pool, Task* task, void* args) {
    BtlPsyInvisible* self = task->data;
    if (self->update)
        self->update(self);
    CombatSprite_Update(&self->sprite);
    return self->active;
}
s32 func_ov003_02094fe4(TaskPool* pool, Task* task, void* args) {
    BtlPsyInvisible* self = task->data;
    s16              x, y;
    if (self->visible) {
        x = func_ov003_020843b0(0, self->x);
        y = func_ov003_020843ec(0, self->y, 0);
        CombatSprite_SetPosition(&self->sprite, x, y);
        func_ov003_02082730(&self->sprite, 0x7FFFFFFE - self->y);
        CombatSprite_Render(&self->sprite);
    }
    return 1;
}
s32 func_ov003_0209504c(TaskPool* pool, Task* task, void* args) {
    BtlPsyInvisible* self = task->data;
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_02095064(TaskPool*, Task*, void*, s32);

s32 func_ov003_02095064(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7dd4;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_020950ac(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (data_ov003_020e71b8->unk3D894 < 100)
        return 0;
    if (!func_ov003_02087704())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020d7dc8.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020d7dc8;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}
