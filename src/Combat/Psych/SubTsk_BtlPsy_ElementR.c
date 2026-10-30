#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include "SndMgrSeIdx.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern const TaskHandle data_ov003_020dc7a0;

s32 func_ov003_020ade84(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(3))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc7a0.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc7a0;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020adedc(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003ElementR*     self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = *arg;
            self->position.x = touch->x;
            self->position.y = touch->y;
            self->cancelled  = 0;
            func_ov003_020a3498(&self->sprites[0], 2, 1, 0x6C, 1);
            func_ov003_020a3498(&self->sprites[1], 2, 1, 0x6F, 0);
            func_ov003_020a3498(&self->sprites[2], 2, 1, 0x82, 0);
            for (s32 i = 0; i < 3; i++)
                func_ov003_020a8e74(self->position.x, self->position.y, 2, i + 0x71, 8, 0, 20);
            func_ov003_020ae3a4(self);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
            func_ov003_020a5a94(self->effect);
            self->phase       = 0;
            self->delay       = 20;
            self->auraOpacity = 0;
            return 1;
        }
        case 1: {
            Ov003ElementR* self = task->data;
            if (!self->cancelled && (func_ov003_020a63a8(self->effect) || !func_ov003_020a5aa4(self->effect))) {
                self->cancelled = 1;
                switch (self->phase) {
                    case 0:
                    case 1:
                    case 3:
                        func_ov003_020a6464(0);
                        return 0;
                    case 2:
                        func_ov003_020ae324(self);
                        break;
                }
            }
            switch (self->phase) {
                case 0:
                    if (--self->delay == 0)
                        self->phase = 1;
                    break;
                case 1:
                    if (SpriteMgr_IsAnimationFinished(&self->sprites[0].sprite)) {
                        func_ov003_020a35b8(&self->sprites[0], 0x6E, 0);
                        self->phase          = 2;
                        self->releasedFrames = 0;
                        self->auraOpacity    = 0x3000;
                    } else {
                        self->auraOpacity += 0x4CD;
                        if (self->auraOpacity > 0x3000)
                            self->auraOpacity = 0x3000;
                    }
                    break;
                case 2:
                    if (func_ov003_02085cfc(3))
                        self->releasedFrames = 0;
                    if (self->releasedFrames == 0) {
                        for (s32 i = 0; i < 3; i++)
                            func_ov003_020ae350(self);
                        func_ov003_020a5a94(self->effect);
                        func_ov003_02087f00(SEIDX_SE_PSY_EGENERATE_SHOT01_01, (s16)self->position.x);
                    }
                    if (++self->releasedFrames >= 30)
                        func_ov003_020ae324(self);
                    break;
                case 3:
                    if (SpriteMgr_IsAnimationFinished(&self->sprites[0].sprite))
                        return 0;
                    self->auraOpacity -= 0x4CD;
                    if (self->auraOpacity < 0)
                        self->auraOpacity = 0;
                    break;
            }
            if (!SndMgr_IsSEPlaying(SEIDX_SE_PSY_EGENERATE_BOOT01_01))
                func_ov003_02087f00(SEIDX_SE_PSY_EGENERATE_BOOT01_01, (s16)self->position.x);
            if (self->phase != 0) {
                CombatSprite_Update(&self->sprites[0]);
                CombatSprite_Update(&self->sprites[1]);
                CombatSprite_Update(&self->sprites[2]);
            }
            return 1;
        }
        case 2: {
            Ov003ElementR* self = task->data;
            s16            x    = self->position.x;
            s16            y    = self->position.y;
            if (self->phase == 0)
                return 1;
            CombatSprite_SetPosition(&self->sprites[0], x, y);
            func_ov003_02082730(&self->sprites[0], 0x383000);
            CombatSprite_Render(&self->sprites[0]);
            if (self->auraOpacity >= 0x1000) {
                CombatSprite_SetPosition(&self->sprites[2], x, y);
                func_ov003_02082730(&self->sprites[2], 0x382000);
                CombatSprite_SetAffineTransform(&self->sprites[2], 0, 0x199A, 0x199A, 0);
                CombatSprite_RenderWithPalette(&self->sprites[2], self->auraOpacity >> 12);
            }
            if (self->phase == 2) {
                CombatSprite_SetPosition(&self->sprites[1], x, y);
                func_ov003_02082730(&self->sprites[0], 0x384000);
                CombatSprite_Render(&self->sprites[1]);
            }
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprites[0]);
            CombatSprite_Release(&self->sprites[1]);
            CombatSprite_Release(&self->sprites[2]);
            SndMgr_StopPlayingSE(SEIDX_SE_PSY_EGENERATE_BOOT01_01);
            return 1;
    }
}

void func_ov003_020ae324(Ov003ElementR* self) {
    func_ov003_020a6464(0);
    func_ov003_020a35b8(&self->sprites[0], 0x6D, 1);
    self->phase = 3;
}

void func_ov003_020ae350(Ov003ElementR* self) {
    func_ov003_020ad280(&self->effect->streak, self->position.x, self->position.y,
                        self->directions[self->directionIndex] * 0x1000, 0x5000, 0x1000);
    self->directionIndex = (self->directionIndex + 1) % 16;
}

void func_ov003_020ae3a4(Ov003ElementR* self) {
    for (s32 i = 0; i < 16; i++)
        self->directions[i] = i;
    for (s32 i = 0; i < 16; i++) {
        u16 a               = RNG_Next(16);
        u16 b               = RNG_Next(16);
        u8  temp            = self->directions[a];
        self->directions[a] = self->directions[b];
        self->directions[b] = temp;
    }
    self->directionIndex = 0;
}

// mwcc emits these definitions in reverse order within their linked sections.
char             data_ov003_020e4bec[24] = "SubTsk_BtlPsy_ElementR";
const TaskHandle data_ov003_020dc7a0     = {data_ov003_020e4bec, func_ov003_020adedc, sizeof(Ov003ElementR)};
