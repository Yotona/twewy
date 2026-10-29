#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern s32  func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void func_ov003_02087f28(s32, s32);
extern s32  func_ov003_020843b0(s32, s32);
extern s32  func_ov003_020843ec(s32, s32, s32);

extern const TaskHandle data_ov003_020dc70c;

void func_ov003_020ab7f8(Ov003EffectState* effect, s32 x, s32 y, s32 z) {
    Ov003PsychPositionArgs args;
    args.effect     = effect;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    func_ov003_020a6000(&data_ov003_020dc70c, &args);
}

s32 func_ov003_020ab828(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychPositionArgs* arg = args;
    Ov003FireWorks*         self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect     = arg->effect;
            self->args.position.x = arg->position.x;
            self->args.position.y = arg->position.y;
            self->args.position.z = arg->position.z;
            self->particleTimer   = 0;
            self->phase           = 0;
            self->timer           = self->args.effect->params[0];
            func_ov003_020a3498(&self->sprite, 2, 1, 0x99, 1);
            return 1;
        case 1: {
            Ov003FireWorks*      self = task->data;
            Ov003PsychEffectArgs burst, particle;
            switch (self->phase) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        CombatSprite_Restart(&self->sprite);
                        func_ov003_02087f28(0x54, self->args.position.x);
                    }
                    if (--self->timer == 0) {
                        self->phase = 1;
                        self->timer = self->args.effect->params[1];
                        func_ov003_020a35b8(&self->sprite, 0x9A, 1);
                    }
                    if (self->particleTimer == 0) {
                        self->particleTimer = 5;
                        func_ov003_020a88c0(&particle, self->args.position.x, self->args.position.y, self->args.position.z, 2,
                                            0x9C);
                        func_ov003_020a899c(&particle, 0);
                        func_ov003_020a89b0(&particle, 30);
                        func_ov003_020a89c8(&particle, RNG_Next(16) << 12, 0x1000, 0x1000);
                        func_ov003_020a88a8(&particle);
                    }
                    self->particleTimer--;
                    break;
                case 1:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        CombatSprite_Restart(&self->sprite);
                        func_ov003_02087f28(0x54, self->args.position.x);
                    }
                    if (--self->timer == 0) {
                        s32 i, angle;
                        func_ov003_02087f28(0x55, self->args.position.x);
                        self->phase = 2;
                        func_ov003_020a35b8(&self->sprite, 0x9B, 1);
                        func_ov003_020a88c0(&burst, self->args.position.x, self->args.position.y, self->args.position.z, 2,
                                            0xA2);
                        func_ov003_020a899c(&burst, 0);
                        func_ov003_020a88a8(&burst);
                        for (i = 0, angle = 0; i < 32; i++, angle += 0x800) {
                            s32 speed = (RNG_Next(5) + 5) << 12;
                            func_ov003_020ab524(&self->args.effect->streak, 0, self->args.position.x, self->args.position.y,
                                                self->args.position.z, angle, speed);
                        }
                    }
                    break;
                case 2:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                        return 0;
                    break;
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        }
        case 2: {
            Ov003FireWorks* self = task->data;
            s32             x    = func_ov003_020843b0(0, self->args.position.x);
            s32             y    = func_ov003_020843ec(0, self->args.position.y, self->args.position.z);
            CombatSprite_SetPosition(&self->sprite, x, y);
            func_ov003_02082730(&self->sprite, 0x386000);
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

char             data_ov003_020e4ae0[24] = "SubTsk_BtlPsy_FireWorks";
const TaskHandle data_ov003_020dc70c     = {data_ov003_020e4ae0, func_ov003_020ab828, sizeof(Ov003FireWorks)};
