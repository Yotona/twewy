#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32  func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void func_ov003_02087fe0(s32, CombatSprite*, s32);
extern void func_ov003_02088064(CombatSprite*, s32, s32);
extern s32  func_ov003_020843b0(s32, s32);
extern s32  func_ov003_020843ec(s32, s32, s32);

extern const TaskHandle data_ov003_020dc9a8;

s32 func_ov003_020b4048(Ov003EffectState* effect, s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 animation, s32 trail) {
    Ov003BrokenMeteorArgs args;
    args.effect     = effect;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    args.velocity.x = vx;
    args.velocity.y = vy;
    args.velocity.z = vz;
    args.animation  = animation;
    args.trail      = trail;
    return func_ov003_020a6000(&data_ov003_020dc9a8, &args);
}

s32 func_ov003_020b40a0(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003BrokenMeteorArgs* arg = args;
    Ov003BrokenMeteor*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = arg->effect;
            self->position.x = arg->position.x;
            self->position.y = arg->position.y;
            self->position.z = self->baseZ = arg->position.z;
            self->velocity.x               = arg->velocity.x;
            self->velocity.y               = arg->velocity.y;
            self->velocity.z               = arg->velocity.z;
            self->trail                    = arg->trail;
            func_ov003_020a3498(&self->sprite, 2, 1, arg->animation, 1);
            self->loopAnimation = arg->animation - 1;
            func_ov003_02087fe0(0, &self->icon, 2);
            return 1;
        case 1:
            self = task->data;
            self->position.x += self->velocity.x;
            self->position.y += self->velocity.y;
            self->position.z += self->velocity.z;
            if (self->position.z >= self->baseZ)
                return 0;
            if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                func_ov003_020a35b8(&self->sprite, self->loopAnimation, 0);
            if (self->trail)
                func_ov003_020a6628(&self->effect->streak, 0x14F, self->position.x, self->position.y, self->position.z, 0, 0);
            self->velocity.z += 0x333;
            CombatSprite_Update(&self->sprite);
            CombatSprite_Update(&self->icon);
            return 1;
        case 2: {
            self  = task->data;
            s32 y = func_ov003_020843ec(0, self->position.y, self->position.z);
            s32 x = func_ov003_020843b0(0, self->position.x);
            CombatSprite_SetPosition(&self->sprite, x, y);
            func_ov003_02082730(&self->sprite, self->position.y + 0x1F4000);
            CombatSprite_Render(&self->sprite);
            func_ov003_02088064(&self->icon, self->position.x, self->position.y);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            CombatSprite_Release(&self->icon);
            return 1;
    }
}

char             data_ov003_020e4e34[28] = "SubTsk_BtlPsy_BrokenMeteor";
const TaskHandle data_ov003_020dc9a8     = {data_ov003_020e4e34, func_ov003_020b40a0, sizeof(Ov003BrokenMeteor)};
