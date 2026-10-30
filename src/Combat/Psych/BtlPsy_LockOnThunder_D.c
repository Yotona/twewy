#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include "SndMgrSeIdx.h"
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32              func_ov003_020a62b8(s32, s32, s32, s32);
extern const TaskHandle data_ov003_020dc984;

void func_ov003_020b3284(Ov003Thunder* self) {
    if (func_ov003_020a6568(&self->args.effect->streak, 0x16B, (Ov003EnemyActor*)self->target, self->position.x, 0, 0) == 1) {
        Ov003PsychEffectArgs effect;
        func_ov003_020a8908(&effect, (s16)(self->position.x >> 12), (s16)(self->position.y >> 12), 2,
                            self->args.effect->index + 0xC);
        func_ov003_020a88a8(&effect);
    }
    func_ov003_020b3078(self->args.targets);
}

void func_ov003_020b3308(Ov003Thunder* self) {
    self->target = func_ov003_020b3094(self->args.targets)->actor;
}

s32 func_ov003_020b3324(Ov003Thunder* self) {
    Ov003TargetEntry* entry = func_ov003_020b3094(self->args.targets);
    if (entry == NULL)
        return 0;
    return entry->actor == self->target;
}

void func_ov003_020b3358(Ov003Thunder* self) {
    if (func_ov003_020b30d8(self->args.targets)) {
        func_ov003_020b3698(self);
        return;
    }
    CombatActor* previous = self->target;
    func_ov003_020b3308(self);
    if (self->target == previous) {
        func_ov003_020b35c0(self);
        return;
    }
    func_ov003_020b3464(self);
}

void func_ov003_020b33a4(Ov003Thunder* self) {
    self->phase = 0;
    func_ov003_020a3498(&self->sprite, 2, 1, self->args.effect->index, 1);
    self->angle = 0x4000;
    func_ov003_020b3308(self);
    func_ov003_02087f28(0x14B, self->target->position.x);
}

void func_ov003_020b3400(Ov003Thunder* self) {
    if (!func_ov003_020b3324(self)) {
        func_ov003_020b3358(self);
        return;
    }
    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
        func_ov003_020b3284(self);
        func_ov003_020b3358(self);
        return;
    }
    CombatActor* target = self->target;
    self->position.x    = target->screenX * 0x1000;
    self->position.y    = target->screenY * 0x1000;
}

void func_ov003_020b3464(Ov003Thunder* self) {
    self->phase = 1;
    CombatSprite_Release(&self->sprite);
    func_ov003_020a3498(&self->sprite, 2, 1, self->args.effect->index + 4, 0);
    func_ov003_02087f28(0x14B, self->target->position.x);
}

void func_ov003_020b34c0(Ov003Thunder* self) {
    if (!func_ov003_020b3324(self)) {
        func_ov003_020b3358(self);
        return;
    }
    s32 x = self->target->screenX * 0x1000;
    s32 y = (self->target->screenY - self->target->unk_76) * 0x1000;
    if (func_ov003_020a62ec(self->position.x, self->position.y, x, y) < self->speed) {
        func_ov003_020b3284(self);
        func_ov003_020b3358(self);
        return;
    }
    self->angle = func_ov003_020a62b8(self->position.x, self->position.y, x, y);
    self->position.x += FX_Mul(self->speed, COS2(FX_SinCosTable_, self->angle));
    self->position.y += FX_Mul(self->speed, SIN2(FX_SinCosTable_, self->angle));
}

void func_ov003_020b35c0(Ov003Thunder* self) {
    self->phase = 2;
    CombatSprite_Release(&self->sprite);
    func_ov003_020a3498(&self->sprite, 2, 1, self->args.effect->index + 8, 1);
    self->angle = (u16)(self->angle + 0x4000);
    func_ov003_02087f28(0x14B, self->target->position.x);
}

void func_ov003_020b362c(Ov003Thunder* self) {
    if (!func_ov003_020b3324(self)) {
        func_ov003_020b3358(self);
        return;
    }
    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
        func_ov003_020b3284(self);
        func_ov003_020b3358(self);
        return;
    }
    CombatActor* target = self->target;
    self->position.x    = target->screenX * 0x1000;
    self->position.y    = (target->screenY - target->unk_76) * 0x1000;
}

void func_ov003_020b3698(Ov003Thunder* self) {
    self->phase = 3;
    CombatSprite_Release(&self->sprite);
    func_ov003_020a3498(&self->sprite, 2, 1, self->args.effect->index + 4, 0);
    self->angle = (u16)(self->angle + (RNG_Next(0x1001) + 0x1000));
    func_ov003_02087f00(SEIDX_SE_BOY_THUNDER, (s16)(self->position.x >> 12));
}

void func_ov003_020b371c(Ov003Thunder* self) {
    if (!func_ov003_020a6374(self->position.x >> 12, self->position.y >> 12, 0x40)) {
        self->phase = 4;
        return;
    }
    self->position.x += FX_Mul(self->speed, COS2(FX_SinCosTable_, self->angle));
    self->position.y += FX_Mul(self->speed, SIN2(FX_SinCosTable_, self->angle));
}

s32 func_ov003_020b37c8(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc984.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc984;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020b3834(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**  arg = args;
    Ov003LockOnThunder* self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            func_ov003_020b2fd8(&self->targets);
            self->launched = 0;
            self->finished = 0;
            func_ov003_020a6464(1);
            return 1;
        case 1:
            self = task->data;
            if (self->launched)
                return func_ov003_020b39ac(self);
            return func_ov003_020b38d4(self);
        case 2:
            return 1;
        case 3:
            self = task->data;
            func_ov003_020b2fe4(&self->targets);
            return 1;
    }
}

s32 func_ov003_020b38d4(Ov003LockOnThunder* self) {
    BtlTouchWork* touch = func_ov003_020a62a0();
    s32           stop  = 0;
    if (!func_ov003_020a5aa4(self->effect) || func_ov003_020a63a8(self->effect)) {
        func_ov003_020a6464(0);
        stop = 1;
    }
    if (!func_ov003_02085cfc(6) || stop) {
        func_ov003_020a6464(0);
        if (func_ov003_020b30d8(&self->targets))
            return 0;
        func_ov003_020b3108(self->effect, &self->targets, &self->finished);
        self->launched = 1;
        return 1;
    }
    func_ov003_020a5a94(self->effect);
    func_ov003_020a6494(touch->x);
    CombatActor* actor = func_ov003_02085e78(1);
    if (actor != NULL && !func_ov003_020b30f0(&self->targets, actor))
        func_ov003_020b3014(&self->targets, actor);
    return 1;
}

s32 func_ov003_020b39ac(Ov003LockOnThunder* self) {
    return self->finished == 0;
}

char             data_ov003_020e4de0[28] = "Tsk_BtlPsy_LockOnThunder_D";
const TaskHandle data_ov003_020dc984     = {data_ov003_020e4de0, func_ov003_020b3834, sizeof(Ov003LockOnThunder)};
