#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include <nitro/mi/cpumem.h>

// The blink streak of enemy 4 (`Task_BtlEnm004_Blink`): a sprite that slides along `dir` in `count` steps, from the
// owner's position forwards, or backwards from the far end when `reverse` is set.

/// The task parameter: the owner, the direction of the streak, its length in steps and whether it runs backwards.
typedef struct BtlEnm004BlinkArgs {
    /* 0x0 */ Ov003CutinActor* owner;
    /* 0x4 */ Vec*             dir;
    /* 0x8 */ s16              count;
    /* 0xA */ u16              reserved0A;
    /* 0xC */ s32              reverse;
} BtlEnm004BlinkArgs; // Size: 0x10

typedef struct BtlEnm004Blink {
    /* 0x00 */ CombatSprite     sprite; // the streak
    /* 0x60 */ CombatSprite     glow;
    /* 0xC0 */ Ov003CutinActor* owner;
    /* 0xC4 */ s32              reverse;
    /* 0xC8 */ Vec              position;
    /* 0xD4 */ Vec              dir;
    /* 0xE0 */ Vec              step; // `dir` normalised and scaled to one step
    /* 0xEC */ s16              frame;
    /* 0xEE */ s16              count;
} BtlEnm004Blink;                                 // Size: 0xF0

extern const BinIdentifier data_ov009_021232d8[]; // sprite pages of the group 4 owners
extern const BinIdentifier data_ov018_020fec90[]; // sprite pages of the group 0x20 owners

extern const TaskHandle data_ov003_020e0bf0;

/// Writes `src` scaled by `scale` to `dst`.
void func_ov003_020cac84(s32 scale, Vec* src, Vec* dst) {
    Vec zero;

    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    Vec_MultAdd(scale, src, &zero, dst);
}

s32 func_ov003_020cacb0(TaskPool* pool, Task* task, void* args) {
    BtlEnm004Blink*     self  = task->data;
    BtlEnm004BlinkArgs* spawn = args;
    Ov003CutinActor*    owner = spawn->owner;
    Vec                 offset;

    MI_CpuSet(self, 0, sizeof(*self));
    self->owner      = spawn->owner;
    self->dir        = *spawn->dir;
    self->count      = spawn->count;
    self->reverse    = spawn->reverse;
    self->position.x = owner->base.actor.position.x;
    self->position.y = owner->base.actor.position.y - 0x1000;
    self->position.z = owner->base.actor.position.z;
    if (owner->base.group == 4) {
        CombatSprite_LoadDirect(owner->base.sprite.sprite.bits_0_1, &self->sprite,
                                &data_ov009_021232d8[owner->base.spriteIndex], 7, 9, 8, 0xD, 0);
    }
    if (owner->base.group == 0x20) {
        CombatSprite_LoadDirect(owner->base.sprite.sprite.bits_0_1, &self->sprite,
                                &data_ov018_020fec90[owner->base.spriteIndex], 0xD, 0xF, 0xE, 0x13, 0);
    }
    CombatSprite_SetFlip(&self->sprite, owner->base.sprite.flags46 & 1);
    Vec_Normalize(&self->dir, &self->step);
    func_ov003_020cac84(0xA000 / self->count, &self->step, &self->step);
    if (self->count > 2) {
        self->sprite.flags46 |= 4;
    }
    if (self->reverse == 0) {
        func_ov003_020cac84(FX32_CONVERT(self->count * self->count) / 2, &self->step, &offset);
        Vec_Sub(&self->position, &offset, &self->position);
    }
    func_ov003_02087fe0(owner->base.sprite.sprite.bits_0_1, &self->glow, 2);
    return 1;
}

s32 func_ov003_020cae94(TaskPool* pool, Task* task, void* args) {
    BtlEnm004Blink* self = task->data;
    Vec             offset;

    if (self->frame == 0) {
        CombatSprite_SetAnim(&self->sprite, 0, 0);
    }
    if (self->reverse != 0) {
        func_ov003_020cac84(FX32_CONVERT(self->frame), &self->step, &offset);
        Vec_Add(&self->position, &offset, &self->position);
    } else {
        func_ov003_020cac84(FX32_CONVERT(self->count - self->frame), &self->step, &offset);
        Vec_Add(&self->position, &offset, &self->position);
    }
    if (self->frame == self->count - 1 && self->reverse == 0) {
        CombatSprite_SetAnim(&self->sprite, 0, 0);
    }
    CombatSprite_Update(&self->sprite);
    CombatSprite_Update(&self->glow);
    if (self->frame >= self->count) {
        return 0;
    }
    self->frame++;
    return 1;
}

s32 func_ov003_020cafd4(TaskPool* pool, Task* task, void* args) {
    BtlEnm004Blink* self = task->data;
    s32             x    = func_ov003_020843b0(func_ov003_020c3908(&self->sprite), self->position.x);
    s32             y    = func_ov003_020843ec(func_ov003_020c3908(&self->sprite), self->position.y, self->position.z);
    s32             sort = 0x7FFFFFFF - self->position.y;

    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, sort);
    CombatSprite_Render(&self->sprite);
    func_ov003_02088064(&self->glow, self->position.x, self->position.y);
    return 1;
}

s32 func_ov003_020cb04c(TaskPool* pool, Task* task, void* args) {
    BtlEnm004Blink* self = task->data;

    CombatSprite_Release(&self->sprite);
    CombatSprite_Release(&self->glow);
    return 1;
}

s32 func_ov003_020cb06c(TaskPool* pool, Task* task, void* args, s32 stage);

char             data_ov003_020e6bd0[0x18] = "Task_BtlEnm004_Blink";
const TaskHandle data_ov003_020e0bf0       = {data_ov003_020e6bd0, func_ov003_020cb06c, sizeof(BtlEnm004Blink)};

/// The stage table of the streak: build, update, render, release.
const TaskStages data_ov003_020e0bfc = {
    func_ov003_020cacb0,
    func_ov003_020cae94,
    func_ov003_020cafd4,
    func_ov003_020cb04c,
};

s32 func_ov003_020cb06c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020e0bfc;

    return stages.iter[stage](pool, task, args);
}

/// Starts the streak of `owner` in five lengths, 2 to 6 steps long.
void func_ov003_020cb0b4(Ov003CutinActor* owner, Vec* dir, s32 reverse) {
    BtlEnm004BlinkArgs args;
    s32                i;

    func_ov003_020c3cdc(owner);
    args.owner   = owner;
    args.dir     = dir;
    args.reverse = reverse;
    for (i = 0; i < 5; i++) {
        args.count = i + 2;
        EasyTask_CreateTask(func_ov003_020c3ca4(owner), &data_ov003_020e0bf0, NULL, 0, NULL, &args);
    }
}
