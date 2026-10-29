#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32       func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern const s32 data_ov003_020dc924[3];

extern const TaskHandle data_ov003_020dc918;

/// Starts the ice-pillar sibling task with the four arguments it consumes.
s32 func_ov003_020b1ca0(Ov003EffectState* effect, s32 x, s32 y, s32 attackId) {
    Ov003IcePillarArgs args;
    args.effect     = effect;
    args.position.x = x;
    args.position.y = y;
    args.attackId   = attackId;
    return func_ov003_020a6000(&data_ov003_020dc918, &args);
}

s32 func_ov003_020b1cd0(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003IcePillarArgs* arg = args;
    Ov003IcePillar*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect     = arg->effect;
            self->args.position.x = arg->position.x;
            self->args.position.y = arg->position.y;
            func_ov003_02083ca0(&self->args.position.x, &self->args.position.y, 0, 0);
            self->args.attackId = arg->attackId;
            func_ov003_020a3498(&self->sprites[0], 2, 1, data_ov003_020dc924[self->args.effect->index], 1);
            func_ov003_020a3498(&self->sprites[1], 2, 1, data_ov003_020dc924[self->args.effect->index], 1);
            func_ov003_020a3498(&self->sprites[2], 2, 1, data_ov003_020dc924[self->args.effect->index], 1);
            func_ov003_020a3498(&self->sprites[3], 2, 1, data_ov003_020dc924[self->args.effect->index], 1);
            CombatSprite_SetAnim(&self->sprites[0], 2, 1);
            CombatSprite_SetAnim(&self->sprites[1], 3, 1);
            CombatSprite_SetAnim(&self->sprites[2], 0, 1);
            CombatSprite_SetAnim(&self->sprites[3], 1, 1);
            return 1;
        case 1:
            self = task->data;
            CombatSprite_Update(&self->sprites[0]);
            CombatSprite_Update(&self->sprites[1]);
            CombatSprite_Update(&self->sprites[3]);
            CombatSprite_Update(&self->sprites[2]);
            func_ov003_020a6628(&self->args.effect->streak, (u16)self->args.attackId, self->args.position.x,
                                self->args.position.y, 0, 0, 0);
            return SpriteMgr_IsAnimationFinished(&self->sprites[0].sprite) ? 0 : 1;
        case 2: {
            self  = task->data;
            s32 x = func_ov003_020843b0(0, self->args.position.x);
            s32 y = func_ov003_020843ec(0, self->args.position.y, 0);
            CombatSprite_SetPosition(&self->sprites[2], x, y);
            CombatSprite_SetPosition(&self->sprites[0], x, y);
            CombatSprite_SetPosition(&self->sprites[1], x, y);
            CombatSprite_SetPosition(&self->sprites[3], x, y);
            func_ov003_02082730(&self->sprites[2], self->args.position.y + 0x1F3000);
            func_ov003_02082730(&self->sprites[0], self->args.position.y + 0x1F4000);
            func_ov003_02082730(&self->sprites[1], self->args.position.y + 0x1F5000);
            func_ov003_02082730(&self->sprites[3], self->args.position.y + 0x1F6000);
            if (!SpriteMgr_IsAnimationFinished(&self->sprites[2].sprite))
                CombatSprite_Render(&self->sprites[2]);
            if (!SpriteMgr_IsAnimationFinished(&self->sprites[0].sprite))
                CombatSprite_Render(&self->sprites[0]);
            if (!SpriteMgr_IsAnimationFinished(&self->sprites[1].sprite))
                CombatSprite_Render(&self->sprites[1]);
            if (!SpriteMgr_IsAnimationFinished(&self->sprites[3].sprite))
                CombatSprite_Render(&self->sprites[3]);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprites[2]);
            CombatSprite_Release(&self->sprites[0]);
            CombatSprite_Release(&self->sprites[1]);
            CombatSprite_Release(&self->sprites[3]);
            return 1;
    }
}

extern char      data_ov003_020e4d4c[28];
char             data_ov003_020e4d4c[28] = "SubTsk_BtlPsy_IcePillar_S";
const TaskHandle data_ov003_020dc918     = {data_ov003_020e4d4c, func_ov003_020b1cd0, sizeof(Ov003IcePillar)};
const s32        data_ov003_020dc924[3]  = {0x8B, 0x8B, 0x8C};
