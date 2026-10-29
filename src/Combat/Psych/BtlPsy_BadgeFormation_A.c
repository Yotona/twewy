#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32              func_ov003_020a62b8(s32, s32, s32, s32);
extern s32              func_ov003_020a66f4(Ov003AttackStreak*, s32, s32, s32, s32, u16);
extern const TaskHandle data_ov003_020dc69c;

s32 func_ov003_020a9ec0(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003BadgeFormationArgs initial;
    initial.effect = effect;
    if (!func_ov003_020a401c(func_ov003_020a5a34(effect)))
        return 0;
    if (!func_ov003_020a5ac4(initial.actors, 4, effect, 1))
        return 0;
    Ov003BadgeFormationArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc69c.taskName);
    *arg                         = initial;
    context->handle              = (TaskHandle*)&data_ov003_020dc69c;
    context->arg                 = arg;
    context->post                = NULL;
    return 1;
}

s32 func_ov003_020a9f64(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003BadgeFormationArgs* arg = args;
    Ov003BadgeFormation*     formation;
    switch (stage) {
        default:
        case 0: {
            Ov003TouchActor* owner;
            s32              i;
            formation = task->data;
            MI_CpuSet(formation, 0, sizeof(*formation));
            formation->effect = arg->effect;
            MI_CpuCopyU8(arg->actors, formation->actors, sizeof(formation->actors));
            formation->effect->unk_0A++;
            owner = func_ov003_020a5a34(formation->effect);
            func_ov003_02087f00(0x9B, (s16)(owner->x >> 12));
            formation->state = 0;
            owner            = func_ov003_020a5a34(formation->effect);
            for (i = 0; i < 4; i++) {
                if (owner == formation->actors[i])
                    break;
            }
            formation->index      = i;
            formation->firstIndex = i;
            formation->angle      = 0;
            func_ov003_020a3498(&formation->sprite, 2, 1, 0x101, 1);
            for (u32 j = 0; j < 4; j++)
                func_ov003_020a40bc(formation->actors[j]);
            return 1;
        }
        case 1:
            formation = task->data;
            for (u32 i = 0; i < 4; i++)
                func_ov003_020a40b0(formation->actors[i]);
            switch (formation->state) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&formation->sprite.sprite)) {
                        formation->index = (formation->index + 1) % 4;
                        if (formation->index == formation->firstIndex)
                            return 0;
                        formation->state = 1;
                        func_ov003_020a35b8(&formation->sprite, 0xFF, 1);
                        func_ov003_02087f00(0xB4, (s16)(formation->position.x >> 12));
                    }
                    break;
                case 1:
                    if (SpriteMgr_IsAnimationFinished(&formation->sprite.sprite))
                        func_ov003_020a35b8(&formation->sprite, 0x100, 0);
                    if (func_ov003_020a62ec(formation->position.x, formation->position.y,
                                            formation->actors[formation->index]->x,
                                            formation->actors[formation->index]->y) <= 0xA000)
                    {
                        formation->state = 0;
                        func_ov003_020a35b8(&formation->sprite, 0x101, 1);
                    }
                    break;
            }
            switch (formation->state) {
                case 0:
                    formation->position.x = formation->actors[formation->index]->x;
                    formation->position.y = formation->actors[formation->index]->y;
                    formation->angle      = 0;
                    break;
                case 1: {
                    s32 angle =
                        func_ov003_020a62b8(formation->position.x, formation->position.y,
                                            formation->actors[formation->index]->x, formation->actors[formation->index]->y);
                    formation->position.x += FX_Mul(FX_SinCosTable_[(angle >> 4) * 2 + 1], 0xA000);
                    formation->position.y += FX_Mul(FX_SinCosTable_[(angle >> 4) * 2], 0xA000);
                    formation->angle = angle;
                    break;
                }
            }
            func_ov003_020a66f4(&formation->effect->streak, 0x18D, (s16)(formation->position.x >> 12),
                                (s16)(formation->position.y >> 12), 0, 0);
            CombatSprite_Update(&formation->sprite);
            return 1;
        case 2:
            formation = task->data;
            CombatSprite_SetPosition(&formation->sprite, (s16)(formation->position.x >> 12),
                                     (s16)(formation->position.y >> 12));
            func_ov003_02082730(&formation->sprite, 0x384000);
            CombatSprite_SetAffineTransform(&formation->sprite, formation->angle, 0x1000, 0x1000, 0);
            CombatSprite_Render(&formation->sprite);
            return 1;
        case 3:
            formation = task->data;
            CombatSprite_Release(&formation->sprite);
            formation->effect->unk_0A--;
            return 1;
    }
}

char             data_ov003_020e4a3c[28] = "Tsk_BtlPsy_BadgeFormation_A";
const TaskHandle data_ov003_020dc69c     = {data_ov003_020e4a3c, func_ov003_020a9f64, sizeof(Ov003BadgeFormation)};
