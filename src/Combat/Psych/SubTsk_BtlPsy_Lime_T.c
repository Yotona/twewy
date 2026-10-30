#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "SndMgrSeIdx.h"
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32              func_ov003_020a62b8(s32, s32, s32, s32);
extern const TaskHandle data_ov003_020dc960;

s32 func_ov003_020b292c(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (func_ov003_020a5b18(effect->state))
        return 0;
    if (!func_ov003_02085cfc(2))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc960.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc960;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

// Nonmatching: the initial animation call schedules its sprite address before the final zero argument.
s32 func_ov003_020b2998(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003Lime*         self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect  = *arg;
            self->phase   = 0;
            self->entered = 0;
            func_ov003_020a6464(1);
            self->target.z = -0x8000;
            self->target.x = func_ov003_0208442c(0, touch->x);
            self->target.y = func_ov003_02084460(0, touch->y, self->target.z);
            func_ov003_02083ca0(&self->target.x, &self->target.y, 0, 0);
            self->position.z = -0x20000;
            self->position.x = func_ov003_0208442c(0, 0);
            self->position.y = self->target.y;
            func_ov003_02083ca0(&self->position.x, &self->position.y, 0, 0);
            self->angle = func_ov003_020a62b8(self->position.x, self->position.z, self->target.x, self->target.z);
            self->speed = self->effect->params[0];
            func_ov003_020a3498(&self->sprite, 2, 1, 0xAF, 0);
            CombatSprite_SetAnim(&self->sprite, 0, 0);
            func_ov003_02087fe0(0, &self->icon, 2);
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            func_ov003_020a6494(touch->x);
            self->effect->unk_0A++;
            func_ov003_02087f00(SEIDX_SE_PSY_RHYME_FLY01_01, touch->x);
            return 1;
        }
        case 1:
            self = task->data;
            if (self->phase == 0 || self->phase == 2) {
                self->position.x += FX_Mul(self->speed, COS2(FX_SinCosTable_, self->angle));
                self->position.z += FX_Mul(self->speed, SIN2(FX_SinCosTable_, self->angle));
            }
            switch (self->phase) {
                case 0:
                    if (!self->entered && func_ov003_020843b0(0, self->position.x) > 0 &&
                        func_ov003_020843ec(0, self->position.y, self->position.z) > 0)
                    {
                        func_ov003_02087f28(0xA2, self->position.x);
                        self->entered = 1;
                        break;
                    }
                    if (func_ov003_020a62ec(self->position.x, self->position.z, self->target.x, self->target.z) <= 0x4000)
                        self->phase = 1;
                    break;
                case 1:
                    if (func_ov003_020a6628(&self->effect->streak, 0x150, self->position.x, self->position.y, self->position.z,
                                            0, 0) == 1)
                    {
                        self->phase = 3;
                        CombatSprite_SetAnim(&self->sprite, 2, 1);
                        func_ov003_02087f28(0xAF, self->position.x);
                        break;
                    }
                    self->phase = 2;
                    self->angle = (u16)(0x10000 - self->angle);
                    CombatSprite_SetAnim(&self->sprite, 1, 0);
                    func_ov003_02087f28(0xA3, self->position.x);
                    break;
                case 3:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        CombatSprite_SetAnim(&self->sprite, 3, 1);
                        self->phase = 4;
                    }
                    break;
                case 4:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        self->phase = 2;
                        self->angle = (u16)(0x10000 - self->angle);
                        CombatSprite_SetAnim(&self->sprite, 1, 0);
                        func_ov003_02087f28(0xA3, self->position.x);
                    }
                    break;
                case 2:
                    if (func_ov003_020843b0(0, self->position.x) > 0x140 ||
                        func_ov003_020843ec(0, self->position.y, self->position.z) < 0)
                        return 0;
                    break;
            }
            CombatSprite_Update(&self->sprite);
            CombatSprite_Update(&self->icon);
            return 1;
        case 2: {
            self  = task->data;
            s32 y = func_ov003_020843ec(0, self->position.y, self->position.z);
            s32 x = func_ov003_020843b0(0, self->position.x);
            CombatSprite_SetPosition(&self->sprite, x, y);
            CombatSprite_Render(&self->sprite);
            func_ov003_02088064(&self->icon, self->position.x, self->position.y);
            return 1;
        }
        case 3:
            self = task->data;
            func_ov003_020a6464(0);
            CombatSprite_Release(&self->sprite);
            CombatSprite_Release(&self->icon);
            self->effect->unk_0A--;
            return 1;
    }
}

char             data_ov003_020e4d98[24] = "SubTsk_BtlPsy_Lime_T";
const TaskHandle data_ov003_020dc960     = {data_ov003_020e4d98, func_ov003_020b2998, sizeof(Ov003Lime)};
