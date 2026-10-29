#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/fx/fx_vector.h>
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern s32              func_ov003_020843b0(s32, s32);
extern s32              func_ov003_020843ec(s32, s32, s32);
extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern Ov003EnemyActor* func_ov003_0208495c(CombatList*);
extern Ov003EnemyActor* func_ov003_02084984(CombatListNode*);
extern void             func_ov003_02087f28(s32, s32);

extern const TaskHandle data_ov003_020dc6d0;

void func_ov003_020aacd4(Ov003EffectState* effect, s16 screenX, s16 screenY, s32 mode, s32 radius, s32 minSpeed, s32 maxSpeed,
                         s32 lifetime) {
    Ov003BlackHollArgs args;
    args.effect   = effect;
    args.screenX  = screenX;
    args.screenY  = screenY;
    args.mode     = mode;
    args.radius   = radius;
    args.minSpeed = minSpeed;
    args.maxSpeed = maxSpeed;
    args.lifetime = lifetime;
    func_ov003_020a6000(&data_ov003_020dc6d0, &args);
}

// Nonmatching: vector temporaries, render-coordinate narrowing and register allocation differ.
s32 func_ov003_020aad24(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003BlackHollArgs* arg = args;
    Ov003BlackHoll*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = arg->effect;
            self->mode       = arg->mode;
            self->lifetime   = arg->lifetime;
            self->minSpeed   = arg->minSpeed;
            self->position.x = func_ov003_0208442c(0, arg->screenX);
            self->position.z = -0x40000;
            self->position.y = func_ov003_02084460(0, arg->screenY, self->position.z);
            self->effect->unk_0A++;
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            func_ov003_020a3498(&self->sprites[0], 2, 1, 0xA4, 0);
            func_ov003_020a3498(&self->sprites[1], 2, 1, 0xA5, 1);
            self->slope     = FX_Divide(arg->maxSpeed - arg->minSpeed, -arg->radius);
            self->intercept = arg->maxSpeed - FX_Mul(self->slope, arg->radius);
            func_ov003_020a5d88(self->particles, 6, 7);
            return 1;
        case 1: {
            Ov003BlackHoll* self = task->data;
            if (--self->lifetime == 0)
                return 0;
            Ov003EnemyActor* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
            while (unit != NULL) {
                if (func_ov003_020a60a8(unit, 2)) {
                    Vec origin = {self->position.x, self->position.y, self->position.z};
                    Vec target = {unit->actor.position.x, unit->actor.position.y,
                                  unit->actor.position.z - FX32_CONVERT(unit->actor.unk_76)};
                    Vec direction;
                    unit->actor.zGravity  = 0;
                    unit->actor.zVelocity = 0;
                    s32 distance          = Vec_Distance(&origin, &target);
                    if (self->mode == 1 && distance <= 0x1000) {
                        func_ov003_020c5058((Ov003CutinActor*)unit);
                    } else {
                        Vec_Sub(&origin, &target, &direction);
                        Vec_Normalize(&direction, &direction);
                        s32 speed = self->intercept + FX_Mul(self->slope, distance);
                        if (speed < self->minSpeed)
                            speed = self->minSpeed;
                        if (speed > distance)
                            speed = distance;
                        unit->actor.position.x += FX_Mul(speed, direction.x);
                        unit->actor.position.y += FX_Mul(speed, direction.y);
                        unit->actor.position.z += FX_Mul(speed, direction.z);
                    }
                } else {
                    func_ov003_020a6164(unit, 2);
                }
                unit = func_ov003_02084984(&unit->node);
            }
            if (SpriteMgr_IsAnimationFinished(&self->sprites[1].sprite)) {
                CombatSprite_Restart(&self->sprites[1]);
                func_ov003_02087f28(0x93, self->position.x);
            }
            CombatSprite_Update(&self->sprites[0]);
            CombatSprite_Update(&self->sprites[1]);
            func_ov003_020a5db8(self->particles, 6, 1, 0x40000, 0x1800, 0x400);
            return 1;
        }
        case 2: {
            Ov003BlackHoll* self = task->data;
            s32             x    = func_ov003_020843b0(0, self->position.x);
            s32             y    = func_ov003_020843ec(0, self->position.y, self->position.z);
            CombatSprite_SetPosition(&self->sprites[0], (s16)x, (s16)y);
            CombatSprite_SetPosition(&self->sprites[1], (s16)x, (s16)y);
            func_ov003_02082730(&self->sprites[0], self->position.y + 0x1F4000);
            func_ov003_02082730(&self->sprites[1], self->position.y - 0x1000 + 0x1F4000);
            CombatSprite_Render(&self->sprites[0]);
            CombatSprite_Render(&self->sprites[1]);
            func_ov003_020a5ed4(self->particles, 6, x, y);
            return 1;
        }
        case 3: {
            self = task->data;
            CombatSprite_Release(&self->sprites[0]);
            CombatSprite_Release(&self->sprites[1]);
            func_ov003_020a5fc0(self->particles, 6);
            self->effect->unk_0A--;
            Ov003EnemyActor* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
            while (unit != NULL) {
                func_ov003_020a6198(unit, 2);
                unit = func_ov003_02084984(&unit->node);
            }
            return 1;
        }
    }
}
char             data_ov003_020e4a84[24] = "SubTsk_BtlPsy_BlackHoll";
const TaskHandle data_ov003_020dc6d0     = {data_ov003_020e4a84, func_ov003_020aad24, sizeof(Ov003BlackHoll)};
