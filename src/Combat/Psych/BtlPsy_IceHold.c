#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

/// Hold-type ice psych: two sprites are attached to the held actor, the first
/// one plays the freeze-in animation while the second one trails behind.

extern void func_ov003_02087f28(s32, s32);
extern s32  func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc900;

s32 func_ov003_020b10d8(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = func_ov003_020b15a0();
    if (unit == NULL)
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc900.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dc900;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

// Nonmatching: angle/actor temporary registers in the particle block and one local call relocation differ.
s32 func_ov003_020b1130(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs* arg = args;
    Ov003IceHold*       self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->unit   = arg->unit;
            func_ov003_020a6464(1);
            self->state         = 0;
            self->frames        = 0;
            self->timer         = 0x3C;
            self->particleTimer = 0;
            func_ov003_020a3498(&self->spriteA, 2, 1, 0xC7, 1);
            func_ov003_020a3498(&self->spriteB, 2, 1, 0xCB, 0);
            func_ov003_02087f28(0x91, self->unit->actor.position.x);
            return 1;
        case 1: {
            Ov003IceHold* self = task->data;
            if (func_ov003_020a63a8(self->effect) || (self->state == 2 && !func_ov003_020a5aa4(self->effect)) ||
                self->timer == 0)
            {
                switch (self->state) {
                    case 0:
                        func_ov003_020a6464(0);
                        return 0;
                    case 1:
                    case 2:
                        func_ov003_020a35b8(&self->spriteA, 0xCA, 1);
                        self->state = 3;
                        func_ov003_020a6464(0);
                        break;
                    case 3:
                        break;
                }
            }
            if (self->unit != func_ov003_020b15a0()) {
                self->timer--;
            } else {
                BtlTouchWork* touch = func_ov003_020a62a0();
                self->timer         = 0x3C;
                self->frames++;
                s32 kind  = RNG_Next(3);
                s32 angle = RNG_Next(0x5000) + 0x1800;
                func_ov003_020afb64(kind, touch->x, touch->y, angle, 0x1000);
            }
            switch (self->state) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite)) {
                        func_ov003_020a35b8(&self->spriteA, 0xC8, 0);
                        self->state = 1;
                    }
                    break;
                case 1:
                    if (self->frames >= 0xF) {
                        func_ov003_020a35b8(&self->spriteA, 0xC9, 0);
                        self->state = 2;
                    }
                    break;
                case 2: {
                    Ov003PsychEffectArgs args;
                    s32                  x, index;
                    s16                  y;
                    u16                  attackFlag;
                    if (self->particleTimer != 0) {
                        self->particleTimer--;
                    } else {
                        self->particleTimer = 12;
                        func_ov003_02087f28(0x92, self->unit->actor.position.x);
                    }
                    attackFlag = self->effect->unk_08 >= 4 ? (u16)0x1D : (u16)0;
                    index      = ((s32)(RNG_Next(0x10) << 12) >> 4) * 2;
                    x          = self->unit->actor.screenX;
                    y          = self->unit->actor.screenY - self->unit->actor.unk_76;
                    func_ov003_020a8908(&args, x, y, 2, RNG_Next(4) + 0xFB);
                    func_ov003_020a89f4(&args, 0x1F000, -0x1000);
                    func_ov003_020a8a0c(&args, FX_Mul(FX_SinCosTable_[index + 1], 0x3000),
                                        FX_Mul(FX_SinCosTable_[index], 0x3000), 0);
                    func_ov003_020a899c(&args, 2);
                    func_ov003_020a8a28(&args, 0x1E);
                    func_ov003_020a88a8(&args);
                    func_ov003_020a6568(&self->effect->streak, 0x169, self->unit, self->unit->actor.position.x, 0, attackFlag);
                    func_ov003_020a5a94(self->effect);
                    break;
                }
                case 3:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite))
                        return 0;
                    break;
            }
            func_ov003_020a647c(self->unit->actor.position.x);
            CombatSprite_Update(&self->spriteA);
            CombatSprite_Update(&self->spriteB);
            return 1;
        }
        case 2: {
            Ov003IceHold* self = task->data;
            s32           y    = (s16)(self->unit->actor.screenY - self->unit->actor.unk_76);
            s32           x    = self->unit->actor.screenX;
            CombatSprite_SetPosition(&self->spriteA, x, y);
            CombatSprite_SetPosition(&self->spriteB, x, y);
            func_ov003_02082730(&self->spriteA, 0x384000);
            func_ov003_02082730(&self->spriteB, 0x385000);
            CombatSprite_Render(&self->spriteA);
            if (self->state == 2)
                CombatSprite_Render(&self->spriteB);
            return 1;
        }
        case 3: {
            Ov003IceHold* self = task->data;
            CombatSprite_Release(&self->spriteA);
            CombatSprite_Release(&self->spriteB);
            return 1;
        }
    }
}

/// The actor tracked by the ice psych.
Ov003EnemyActor* func_ov003_020b15a0(void) {
    if (!func_ov003_02085cfc(3))
        return NULL;
    Ov003EnemyActor* unit = (Ov003EnemyActor*)func_ov003_020a62a0()->trackedActor;
    if (unit == NULL)
        return NULL;
    if (unit->actor.unk_00 != 1)
        return NULL;
    return unit;
}

char             data_ov003_020e4d20[20] = "Tsk_BtlPsy_IceHold";
const TaskHandle data_ov003_020dc900     = {data_ov003_020e4d20, func_ov003_020b1130, sizeof(Ov003IceHold)};
