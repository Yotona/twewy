#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void*            func_ov003_0208495c(CombatList*);
extern void*            func_ov003_02084984(CombatListNode*);
extern s32              func_ov003_020a632c(s32, s32, s32, s32);
extern const TaskHandle data_ov003_020dc990;

s32 func_ov003_020b39c0(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a63e4())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc990.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc990;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020b3a14(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState**   arg = args;
    Ov003LockOnThunderP* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            BtlTouchWork* touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            func_ov003_020b2fd8(&self->targets);
            self->launched = 0;
            self->finished = 0;
            self->x        = touch->x;
            self->y        = touch->y;
            self->scaleX   = self->effect->params[0];
            self->scaleY   = self->effect->params[1];
            self->opacity  = 0x1000;
            self->angle    = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, self->effect->index + 0x12, 1);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
            return 1;
        }
        case 1: {
            self = task->data;
            s32 result;
            if (self->launched)
                result = func_ov003_020b3d5c(self);
            else
                result = func_ov003_020b3bdc(self);
            if (Sprite_HasAnimation(&self->sprite.sprite)) {
                if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                    func_ov003_02087f00(0x66, self->x);
                    CombatSprite_Restart(&self->sprite);
                }
                CombatSprite_Update(&self->sprite);
            }
            return result;
        }
        case 2:
            self = task->data;
            if (Sprite_HasAnimation(&self->sprite.sprite)) {
                CombatSprite_SetPosition(&self->sprite, self->x, self->y);
                CombatSprite_SetAffineTransform(&self->sprite, self->angle, self->scaleX, self->scaleY, 1);
                CombatSprite_RenderWithPalette(&self->sprite, self->opacity >> 12);
            }
            return 1;
        case 3:
            self = task->data;
            func_ov003_020b2fe4(&self->targets);
            if (Sprite_HasAnimation(&self->sprite.sprite))
                CombatSprite_Release(&self->sprite);
            return 1;
    }
}

s32 func_ov003_020b3bdc(Ov003LockOnThunderP* self) {
    s32 stop = 0;
    if (!func_ov003_020a5aa4(self->effect) || func_ov003_020a63a8(self->effect)) {
        func_ov003_020a6464(0);
        stop = 1;
    }
    if (!func_ov003_020a63e4() || stop) {
        func_ov003_020a6464(0);
        if (func_ov003_020b30d8(&self->targets))
            return 0;
        func_ov003_020b3108(self->effect, &self->targets, &self->finished);
        self->launched = 1;
        return 1;
    }
    func_ov003_020a5a94(self->effect);
    Ov003Unit* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    while (unit != NULL) {
        s32 distance  = func_ov003_020a632c(unit->actor.screenX, unit->actor.screenY - unit->actor.unk_76, self->x, self->y);
        s32 radius    = FX_Mul(self->scaleX, 0x14000);
        s32 halfWidth = unit->actor.unk_70 / 2;
        if (distance < radius + halfWidth * 0x1000) {
            if (!func_ov003_020b30f0(&self->targets, &unit->actor))
                func_ov003_020b3014(&self->targets, &unit->actor);
        } else {
            if (func_ov003_020b30f0(&self->targets, &unit->actor))
                func_ov003_020b3034(&self->targets, &unit->actor);
        }
        unit = func_ov003_02084984(&unit->node);
    }
    self->opacity += 0x400;
    if (self->opacity > 0x14000)
        self->opacity = 0x14000;
    return 1;
}

s32 func_ov003_020b3d5c(Ov003LockOnThunderP* self) {
    if (self->finished)
        return 0;
    if (Sprite_HasAnimation(&self->sprite.sprite)) {
        self->opacity -= 0x400;
        if (self->opacity < 0x1000)
            CombatSprite_Release(&self->sprite);
    }
    return 1;
}

char             data_ov003_020e4dfc[28] = "Tsk_BtlPsy_LockOnThunder_P";
const TaskHandle data_ov003_020dc990     = {data_ov003_020e4dfc, func_ov003_020b3a14, sizeof(Ov003LockOnThunderP)};
