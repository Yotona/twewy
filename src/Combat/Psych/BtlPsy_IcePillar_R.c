#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void             func_ov003_02087f28(s32, s32);
extern s32              func_ov003_020843b0(s32, s32);
extern s32              func_ov003_020843ec(s32, s32, s32);
extern const TaskHandle data_ov003_020dc930;
extern const s32        data_ov003_020dc93c[3];

/// The three ice pillars spawned by IcePillar_R, plus the shared task state.
///
/// `count` pillars rise around the touch point: `x[i]`/`y[i]` are the beam
/// centres, `xOffset` sways them sideways and flips sign every step. `z` drops
/// the attack volume onto the floor over the last phase.

s32 func_ov003_020b1fd4(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EffectState** arg;

    if (!func_ov003_02085cfc(3))
        return 0;
    arg             = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc930.taskName);
    *arg            = effect;
    context->handle = (TaskHandle*)&data_ov003_020dc930;
    context->arg    = arg;
    context->post   = NULL;
    return 1;
}

// objdiff flags the four intra-TU `bl func_ov003_020b26cc` relocations only because this object
// emits one .text section per function while the delinked target keeps a single one; the encoded
// instructions (and their post-link displacement) are identical.
s32 func_ov003_020b202c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EffectState** arg = args;
    switch (stage) {
        default:
        case 0: {
            Ov003IcePillarR* self  = task->data;
            BtlTouchWork*    touch = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = *arg;
            self->count  = (u8)self->effect->params[0];
            self->x[0]   = func_ov003_0208442c(0, touch->x);
            self->y[0]   = func_ov003_02084460(0, touch->y, 0);
            func_ov003_02083ca0(&self->x[0], &self->y[0], 0, 0);
            self->z       = 0;
            self->xOffset = 0x2000;
            self->x[1]    = self->x[0] + 0x20000;
            self->y[1]    = self->y[0];
            func_ov003_02083ca0(&self->x[1], &self->y[1], 0, 0);
            self->x[2] = self->x[0] - 0x20000;
            self->y[2] = self->y[0];
            func_ov003_02083ca0(&self->x[2], &self->y[2], 0, 0);
            func_ov003_020a6464(1);
            func_ov003_020a6494(touch->x);
            func_ov003_020a5aa4(self->effect);
            func_ov003_020a5a94(self->effect);
            self->phase = 0;
            self->timer = 0x3C;
            func_ov003_020a3498(&self->spriteA, 2, 1, data_ov003_020dc93c[self->effect->index], 1);
            CombatSprite_SetAnim(&self->spriteA, 2, 1);
            func_ov003_020a3498(&self->spriteB, 2, 1, data_ov003_020dc93c[self->effect->index], 0);
            CombatSprite_SetAnim(&self->spriteB, 0, 0);
            func_ov003_02087804(0);
            func_ov003_02087f28(0x87, self->x[0]);
            return 1;
        }
        case 1: {
            Ov003IcePillarR* self = task->data;
            s32              i;
            CombatSprite_Update(&self->spriteA);
            CombatSprite_Update(&self->spriteB);
            if (Sprite_HasAnimation(&self->spriteC.sprite)) {
                if (SpriteMgr_IsAnimationFinished(&self->spriteC.sprite))
                    CombatSprite_Release(&self->spriteC);
                else
                    CombatSprite_Update(&self->spriteC);
            }
            if (func_ov003_020a63a8(self->effect))
                return 0;
            for (i = 0; i < self->count; i++)
                func_ov003_020a6628(&self->effect->streak, 0x149, self->x[i], self->y[i], self->z, 0, 0);
            switch (self->phase) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite)) {
                        if (--self->timer == 0)
                            return 0;
                        if (func_ov003_02085cfc(3)) {
                            self->phase++;
                            self->timer = 0x3C;
                            CombatSprite_SetAnim(&self->spriteA, 3, 1);
                            func_ov003_02087f28(0x87, self->x[0]);
                        }
                        if (func_ov003_02085cfc(3))
                            func_ov003_020b26cc(self);
                    }
                    break;
                case 1:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite)) {
                        if (--self->timer == 0)
                            return 0;
                        if (func_ov003_02085cfc(3)) {
                            self->phase++;
                            self->timer = 0x3C;
                            CombatSprite_SetAnim(&self->spriteA, 4, 1);
                            func_ov003_02087f28(0x87, self->x[0]);
                        }
                        if (func_ov003_02085cfc(3))
                            func_ov003_020b26cc(self);
                    }
                    break;
                case 2:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite)) {
                        if (--self->timer == 0)
                            return 0;
                        if (func_ov003_02085cfc(3)) {
                            self->phase++;
                            self->timer = 0x3C;
                            CombatSprite_SetAnim(&self->spriteA, 5, 1);
                            func_ov003_02087f28(0x87, self->x[0]);
                        }
                        if (func_ov003_02085cfc(3))
                            func_ov003_020b26cc(self);
                    }
                    break;
                case 3:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteA.sprite)) {
                        if (--self->timer == 0)
                            return 0;
                        if (func_ov003_02085cfc(3)) {
                            self->phase++;
                            self->timer = 0x3C;
                            CombatSprite_SetAnim(&self->spriteA, 6, 1);
                            CombatSprite_SetAnim(&self->spriteB, 1, 1);
                            func_ov003_02087f28(0x88, self->x[0]);
                            func_ov003_020a3498(&self->spriteC, 2, 1, data_ov003_020dc93c[self->effect->index], 1);
                            CombatSprite_SetAnim(&self->spriteC, 7, 1);
                        }
                        if (func_ov003_02085cfc(3))
                            func_ov003_020b26cc(self);
                    }
                    break;
                case 4:
                    // The retail code discards the animation result here and only advances the fall.
                    SpriteMgr_IsAnimationFinished(&self->spriteA.sprite);
                    self->z -= 0x7000;
                    if (self->z < -0xC8000)
                        return 0;
                    break;
            }
            return 1;
        }
        case 2: {
            Ov003IcePillarR* self = task->data;
            s32              i;
            for (i = 0; i < self->count; i++) {
                s32 x = func_ov003_020843b0(0, self->x[i]);
                s32 y = func_ov003_020843ec(0, self->y[i], 0);
                // The mid pillar renders only while the effect has not finished.
                if (self->phase >= 0 && !SpriteMgr_IsAnimationFinished(&self->spriteB.sprite)) {
                    CombatSprite_SetPosition(&self->spriteB, x, y);
                    func_ov003_02082730(&self->spriteB, self->y[i] + 0x1F6000);
                    CombatSprite_Render(&self->spriteB);
                }
                if (Sprite_HasAnimation(&self->spriteC.sprite)) {
                    CombatSprite_SetPosition(&self->spriteC, x, y);
                    func_ov003_02082730(&self->spriteC, self->y[i] + 0x1F5000);
                    CombatSprite_Render(&self->spriteC);
                }
                x = func_ov003_020843b0(0, self->x[i]);
                y = func_ov003_020843ec(0, self->y[i], self->z);
                CombatSprite_SetPosition(&self->spriteA, x, y);
                func_ov003_02082730(&self->spriteA, self->y[i] + 0x1F4000);
                CombatSprite_Render(&self->spriteA);
            }
            return 1;
        }
        case 3: {
            Ov003IcePillarR* self = task->data;
            CombatSprite_Release(&self->spriteB);
            CombatSprite_Release(&self->spriteA);
            if (Sprite_HasAnimation(&self->spriteC.sprite))
                CombatSprite_Release(&self->spriteC);
            func_ov003_020a6464(0);
            return 1;
        }
    }
}

/// Sways every pillar centre sideways and flips the sway direction for the
/// next step, so the three beams weave around the touch point.
void func_ov003_020b26cc(Ov003IcePillarR* self) {
    s32 i;
    for (i = 0; i < self->count; i++)
        self->x[i] += self->xOffset;
    self->xOffset = -self->xOffset;
}

char             data_ov003_020e4d68[0x18] = "Tsk_BtlPsy_IcePillar_R";
const TaskHandle data_ov003_020dc930       = {data_ov003_020e4d68, func_ov003_020b202c, sizeof(Ov003IcePillarR)};
const s32        data_ov003_020dc93c[3]    = {0x8D, 0x8D, 0x8E};
