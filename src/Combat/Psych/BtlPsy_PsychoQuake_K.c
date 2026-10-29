#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern s32              func_ov003_020843b0(s32, s32);
extern s32              func_ov003_020843ec(s32, s32, s32);
extern const TaskHandle data_ov003_020dc8dc;

s32 func_ov003_020b05b8(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(7))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc8dc.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc8dc;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020b0610(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    Ov003PsychoQuake*  self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            func_ov003_02087804(0);
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            func_ov003_020a6464(1);
            func_ov003_020a64ac();
            func_ov003_020a4e84(&self->camera, 0, 0);
            func_ov003_020a4ed0(&self->camera, 6, 0);
            func_ov003_020a3498(&self->spriteA, 2, 1, 0xC0, 1);
            func_ov003_020a3498(&self->spriteB, 2, 1, 0xBF, 0);
            self->phase        = 0;
            self->spriteOffset = 20;
            self->opacity      = 0;
            return 1;
        case 1: {
            Ov003PsychoQuake* self   = task->data;
            s32               result = 1;
            if (func_ov003_020a63a8(self->effect)) {
                result = 0;
                goto done;
            }
            if (!SndMgr_IsSEPlaying(0x6B))
                func_ov003_02087ed8(0x6B);
            switch (self->phase) {
                case 0:
                    if (func_ov003_020a4ee8(&self->camera, 0x333, 0, 0x8000)) {
                        Ov003EnemyActor* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
                        while (unit != NULL) {
                            if (unit->actor.position.z == 0 && unit->actor.zGravity != 0 && unit->group != 4 &&
                                unit->group != 7 && unit->group != 8 && unit->group != 12 && unit->group != 15)
                                func_ov003_020a6164(unit, 4);
                            unit = func_ov003_02084984(&unit->node);
                        }
                        self->phase          = 1;
                        self->remainingTicks = self->effect->params[0];
                        self->releaseTimer   = 10;
                        self->sampleTimer    = 0;
                    }
                    break;
                case 1:
                    if (self->sampleTimer == 0) {
                        s16 x = RNG_Next(256);
                        s16 y = RNG_Next(192);
                        s32 worldY;
                        s32 worldX = func_ov003_0208442c(0, x);
                        worldY     = func_ov003_02084460(0, y, 0);
                        func_ov003_02083ca0(&worldX, &worldY, 0, 0);
                        func_ov003_020843b0(0, worldX);
                        func_ov003_020843ec(0, worldY, 0);
                        self->sampleTimer = 1;
                    } else {
                        self->sampleTimer--;
                    }
                    func_ov003_020a4ee8(&self->camera, 0, 0, 0x8000);
                    if (func_ov003_02085cfc(7))
                        self->releaseTimer = 10;
                    self->releaseTimer--;
                    self->remainingTicks--;
                    if (self->releaseTimer == 0 || self->remainingTicks == 0)
                        self->phase = 2;
                    break;
                case 2:
                    if (func_ov003_020a4ee8(&self->camera, -0x333, 0, 0x8000))
                        result = 0;
                    break;
            }
            switch (self->phase) {
                case 0:
                case 1:
                    if (self->opacity < 0x1F000)
                        self->opacity += 0x1000;
                    break;
                case 2:
                    if (self->opacity > 0)
                        self->opacity -= 0x1000;
                    break;
            }
            if (self->spriteOffset > 0)
                self->spriteOffset--;
            if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite))
                func_ov003_020a35b8(&self->spriteA, 0xC1, 0);
            CombatSprite_Update(&self->spriteA);
            CombatSprite_Update(&self->spriteB);
        done:
            return result;
        }
        case 2:
            self = task->data;
            CombatSprite_SetPosition(&self->spriteA, 128, 96);
            CombatSprite_SetPosition(&self->spriteB, 128, (s16)(self->spriteOffset + 96));
            func_ov003_02082730(&self->spriteA, 0x384000);
            func_ov003_02082730(&self->spriteB, 0x385000);
            switch (self->phase) {
                case 0:
                case 1:
                    if (self->opacity > 0) {
                        if (self->opacity > 0x14000)
                            CombatSprite_RenderWithPalette(&self->spriteA, self->opacity >> 12);
                        CombatSprite_RenderWithPalette(&self->spriteB, self->opacity >> 12);
                    }
                    break;
                case 2:
                    if (self->opacity > 0) {
                        CombatSprite_RenderWithPalette(&self->spriteA, self->opacity >> 12);
                        CombatSprite_RenderWithPalette(&self->spriteB, self->opacity >> 12);
                    }
                    break;
            }
            return 1;
        case 3: {
            Ov003PsychoQuake* self = task->data;
            func_ov003_020a6464(0);
            Ov003EffectState* effect = self->effect;
            Ov003EnemyActor*  unit   = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
            while (unit != NULL) {
                if (func_ov003_020a60a8(unit, 4)) {
                    func_ov003_020a6198(unit, 4);
                    func_ov003_020a6568(&effect->streak, 0x167, unit, unit->actor.position.x, 0, 0);
                }
                unit = func_ov003_02084984(&unit->node);
            }
            func_ov003_020a4fd0(&self->camera);
            SndMgr_StopPlayingSE(0x6B);
            CombatSprite_Release(&self->spriteA);
            CombatSprite_Release(&self->spriteB);
            return 1;
        }
    }
}

char             data_ov003_020e4cd4[28] = "Tsk_BtlPsy_PsychoQuake_K";
const TaskHandle data_ov003_020dc8dc     = {data_ov003_020e4cd4, func_ov003_020b0610, sizeof(Ov003PsychoQuake)};
