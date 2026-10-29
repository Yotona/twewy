#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

/// Hold-type fire psych: the held actor is engulfed for as long as the touch
/// stays on it, then the effect switches to its release loop.

extern void func_ov003_02087f28(s32, s32);

extern const TaskHandle data_ov003_020dc8f4;

s32 func_ov003_020b0cd4(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = func_ov003_020b0fa0();
    if (unit == NULL)
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc8f4.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dc8f4;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

s32 func_ov003_020b0d2c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs* arg = args;
    Ov003FireHold*      self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->unit   = arg->unit;
            func_ov003_020a6464(1);
            self->state         = 0;
            self->timer         = 0;
            self->particleTimer = 0;
            self->frames        = 0;
            self->holdTimer     = 0x3C;
            return 1;
        case 1: {
            Ov003FireHold* self = task->data;
            if (func_ov003_020a63a8(self->effect))
                return 0;
            if (self->state == 1 && !func_ov003_020a5aa4(self->effect))
                return 0;
            if (self->unit != func_ov003_020b0fa0()) {
                if (--self->holdTimer == 0)
                    return 0;
            } else {
                self->holdTimer = 0x3C;
                self->frames++;
            }
            switch (self->state) {
                case 0:
                    if (self->timer == 0) {
                        self->timer = 0x14;
                        func_ov003_020b0fdc(self->unit);
                        func_ov003_020b1024(self->unit);
                        func_ov003_02087f28(0xAC, self->unit->actor.position.x);
                    }
                    if (self->frames >= 0xF) {
                        self->frames        = 0;
                        self->timer         = 0;
                        self->particleTimer = 0;
                        self->state         = 1;
                    }
                    break;
                case 1:
                    if (self->timer == 0 && self->frames < 0x3C) {
                        self->timer = 0x14;
                        func_ov003_020b1024(self->unit);
                        func_ov003_02087f28(0xAC, self->unit->actor.position.x);
                    }
                    if (self->particleTimer == 0) {
                        self->particleTimer = 0x14;
                        func_ov003_020b1094(self->unit, self->effect->index == 2);
                        func_ov003_02087f28(0x90, self->unit->actor.position.x);
                    }
                    func_ov003_020a6568(&self->effect->streak, 0x168, self->unit, self->unit->actor.position.x, 0,
                                        self->effect->unk_08 >= 4 ? (u16)0x19 : (u16)0);
                    func_ov003_020a5a94(self->effect);
                    break;
            }
            func_ov003_020a647c(self->unit->actor.position.x);
            if (self->timer > 0)
                self->timer--;
            if (self->particleTimer > 0)
                self->particleTimer--;
            return 1;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

/// The actor currently held by the touch, if it is a live enemy.
Ov003EnemyActor* func_ov003_020b0fa0(void) {
    if (!func_ov003_02085cfc(7))
        return NULL;
    Ov003EnemyActor* unit = (Ov003EnemyActor*)func_ov003_020a62a0()->heldActor;
    if (unit == NULL)
        return NULL;
    if (unit->actor.unk_00 != 1)
        return NULL;
    return unit;
}

void func_ov003_020b0fdc(Ov003EnemyActor* unit) {
    Ov003PsychEffectArgs args;
    func_ov003_020a88c0(&args, unit->actor.position.x, unit->actor.position.y, unit->actor.position.z, 2, 0xC2);
    func_ov003_020a899c(&args, 2);
    func_ov003_020a88a8(&args);
}

void func_ov003_020b1024(Ov003EnemyActor* unit) {
    Ov003PsychEffectArgs args;
    func_ov003_020a88c0(&args, unit->actor.position.x, unit->actor.position.y, unit->actor.position.z, 2, RNG_Next(2) + 0xC3);
    func_ov003_020a899c(&args, 1);
    func_ov003_020a89f4(&args, 0x1F000, -FX_Divide(0x1F000, 0x50000));
    func_ov003_020a88a8(&args);
}

void func_ov003_020b1094(Ov003EnemyActor* unit, s32 charged) {
    Ov003PsychEffectArgs args;
    s32                  kind = 0xC5;
    if (charged != 0)
        kind = 0xC6;
    func_ov003_020a8958(&args, &unit->actor, 2, kind);
    func_ov003_020a899c(&args, 0);
    func_ov003_020a88a8(&args);
}

char             data_ov003_020e4d0c[20] = "Tsk_BtlPsy_FireHold";
const TaskHandle data_ov003_020dc8f4     = {data_ov003_020e4d0c, func_ov003_020b0d2c, sizeof(Ov003FireHold)};
