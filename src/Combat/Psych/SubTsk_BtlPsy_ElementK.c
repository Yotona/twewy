#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include "SndMgrSeIdx.h"
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32              func_ov003_020a66f4(Ov003AttackStreak* streak, u16 attackId, s32 x, s32 y, s32 force, u16 amount);
extern const TaskHandle data_ov003_020dc794;

s32 func_ov003_020ad720(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(7))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc794.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc794;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020ad778(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003ElementK*     self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect    = *arg;
            self->x         = touch->sweepCenterX;
            self->y         = touch->sweepCenterY;
            self->cancelled = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, 0x74, 1);
            func_ov003_020a3498(&self->aura, 2, 1, 0x82, 0);
            for (s32 i = 0; i < 3; i++)
                func_ov003_020adcb4(&self->rays[i]);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
            func_ov003_020a5a94(self->effect);
            self->phase       = 0;
            self->auraOpacity = 0;
            self->auraScale   = 0;
            return 1;
        }
        case 1: {
            Ov003ElementK* self = task->data;
            s32            i;
            if (!self->cancelled && (func_ov003_020a63a8(self->effect) || !func_ov003_020a5aa4(self->effect))) {
                self->cancelled = 1;
                switch (self->phase) {
                    case 0:
                        func_ov003_020a6464(0);
                        return 0;
                    case 5:
                        break;
                    default:
                        func_ov003_020adc8c(self);
                        break;
                }
            }
            switch (self->phase) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                        func_ov003_020adbec(self);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    self->auraScale = FX_Mul((self->phase - 1) * 0x1000, 0x333) + 0x1666;
                    self->auraOpacity += 0x4CD;
                    if (self->auraOpacity > 0x3000)
                        self->auraOpacity = 0x3000;
                    func_ov003_020a66f4(&self->effect->streak, 0x160, self->x, self->y, 0, 0);
                    self->emitTimer++;
                    if (self->emitTimer == 6) {
                        self->emitTimer = 0;
                        for (i = 0; i < 3; i++) {
                            if (!self->rays[i].active) {
                                func_ov003_020adcd8(&self->rays[i], self->phase - 1, self->directions[self->directionIndex]);
                                self->directionIndex = (self->directionIndex + 1) % 8;
                            }
                        }
                    }
                    if (func_ov003_02085cfc(7)) {
                        if (self->phase < 4 && ++self->heldFrames == 25)
                            func_ov003_020adbec(self);
                    } else if (++self->releasedFrames == 30) {
                        func_ov003_020adc8c(self);
                    }
                    break;
                case 5:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                        return 0;
                    self->auraOpacity -= 0x4CD;
                    if (self->auraOpacity < 0)
                        self->auraOpacity = 0;
                    break;
            }
            for (i = 0; i < 3; i++)
                func_ov003_020add40(&self->rays[i], self->x, self->y, self->effect);
            if (!SndMgr_IsSEPlaying(SEIDX_SE_PSY_EGENERATE_BOOT01_01))
                func_ov003_02087f00(SEIDX_SE_PSY_EGENERATE_BOOT01_01, self->x);
            CombatSprite_Update(&self->sprite);
            CombatSprite_Update(&self->aura);
            return 1;
        }
        case 2: {
            Ov003ElementK* self = task->data;
            CombatSprite_SetPosition(&self->sprite, self->x, self->y);
            func_ov003_02082730(&self->sprite, 0x384000);
            CombatSprite_Render(&self->sprite);
            if (self->auraOpacity >= 0x1000) {
                CombatSprite_SetPosition(&self->aura, self->x, self->y);
                func_ov003_02082730(&self->aura, 0x383000);
                CombatSprite_SetAffineTransform(&self->aura, 0, self->auraScale, self->auraScale, 0);
                CombatSprite_RenderWithPalette(&self->aura, self->auraOpacity >> 12);
            }
            for (s32 i = 0; i < 3; i++)
                func_ov003_020ade3c(&self->rays[i], self->x, self->y);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            CombatSprite_Release(&self->aura);
            for (s32 i = 0; i < 3; i++)
                func_ov003_020ade74(&self->rays[i]);
            SndMgr_StopPlayingSE(SEIDX_SE_PSY_EGENERATE_BOOT01_01);
            return 1;
    }
}

void func_ov003_020adbec(Ov003ElementK* self) {
    self->phase++;
    self->heldFrames     = 0;
    self->releasedFrames = 0;
    self->emitTimer      = 0;
    func_ov003_020a35b8(&self->sprite, self->phase + 0x74, 0);
    for (u8 i = 0; i < 8; i++)
        self->directions[i] = i;
    for (u8 i = 0; i < 16; i++) {
        u16 a               = RNG_Next(8);
        u16 b               = RNG_Next(8);
        u8  temp            = self->directions[a];
        self->directions[a] = self->directions[b];
        self->directions[b] = temp;
    }
    self->directionIndex = 0;
}

void func_ov003_020adc8c(Ov003ElementK* self) {
    self->phase = 5;
    func_ov003_020a35b8(&self->sprite, 0x79, 1);
    func_ov003_020a6464(0);
}

void func_ov003_020adcb4(Ov003ElementKRay* ray) {
    ray->active = 0;
    func_ov003_020a3498(&ray->sprite, 2, 1, 0x7A, 1);
}

void func_ov003_020adcd8(Ov003ElementKRay* ray, s32 level, s32 direction) {
    ray->active   = 1;
    ray->angle    = direction * 0x2000;
    s32 animation = direction & 1 ? 0x7A : 0x7E;
    animation += level;
    func_ov003_020a35b8(&ray->sprite, animation, 1);
    CombatSprite_Restart(&ray->sprite);
    CombatSprite_SetAffineTransform(&ray->sprite, (direction / 2) * 0x4000, 0x1000, 0x1000, 0);
}

void func_ov003_020add40(Ov003ElementKRay* ray, s16 x, s16 y, Ov003EffectState* effect) {
    if (!ray->active)
        return;
    if (SpriteMgr_IsAnimationFinished(&ray->sprite.sprite)) {
        ray->active = 0;
        return;
    }
    s16 offsetX = FX_Mul(COS2(FX_SinCosTable_, ray->angle), 0x24000) >> 12;
    s16 offsetY = FX_Mul(SIN2(FX_SinCosTable_, ray->angle), 0x24000) >> 12;
    func_ov003_020a66f4(&effect->streak, 0x161, (s16)(x + offsetX), (s16)(y + offsetY), 0, 0);
    CombatSprite_Update(&ray->sprite);
}

void func_ov003_020ade3c(Ov003ElementKRay* ray, s32 x, s32 y) {
    if (!ray->active)
        return;
    CombatSprite_SetPosition(&ray->sprite, x, y);
    func_ov003_02082730(&ray->sprite, 0x385000);
    CombatSprite_Render(&ray->sprite);
}

void func_ov003_020ade74(Ov003ElementKRay* ray) {
    CombatSprite_Release(&ray->sprite);
}

char             data_ov003_020e4bd4[24] = "SubTsk_BtlPsy_ElementK";
const TaskHandle data_ov003_020dc794     = {data_ov003_020e4bd4, func_ov003_020ad778, sizeof(Ov003ElementK)};
