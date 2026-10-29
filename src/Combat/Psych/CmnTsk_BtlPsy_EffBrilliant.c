#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc638;

s32 func_ov003_020a8e74(s32 x, s32 y, s32 engine, s32 kind, s32 radius, s32 fixedRadius, s32 lifetime) {
    Ov003BrilliantArgs args;
    args.position.x  = x;
    args.position.y  = y;
    args.engine      = engine;
    args.kind        = kind;
    args.sortKey     = -1;
    args.radius      = radius;
    args.fixedRadius = fixedRadius;
    args.lifetime    = lifetime;
    return func_ov003_020a6000(&data_ov003_020dc638, &args);
}

s32 func_ov003_020a8ec4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003BrilliantArgs*   arg = args;
    Ov003BrilliantEffect* effect;
    switch (stage) {
        default:
        case 0: {
            effect = task->data;
            MI_CpuSet(effect, 0, sizeof(*effect));
            effect->position    = arg->position;
            effect->kind        = arg->kind;
            effect->radius      = arg->radius;
            effect->fixedRadius = arg->fixedRadius;
            effect->lifetime    = arg->lifetime;
            func_ov003_020a3498(&effect->sprite, arg->engine, 1, arg->kind, 1);
            func_ov003_02082730(&effect->sprite, arg->sortKey);
            effect->offset = func_ov003_020a904c(effect->radius, effect->fixedRadius);
            return 1;
        }
        case 1:
            effect = task->data;
            CombatSprite_Update(&effect->sprite);
            if (SpriteMgr_IsAnimationFinished(&effect->sprite.sprite)) {
                effect->offset = func_ov003_020a904c(effect->radius, effect->fixedRadius);
                CombatSprite_Restart(&effect->sprite);
            }
            if (effect->lifetime > 0) {
                if (--effect->lifetime == 0)
                    return 0;
            }
            return 1;
        case 2:
            effect = task->data;
            CombatSprite_SetPosition(&effect->sprite, (s16)(effect->position.x + effect->offset.x),
                                     (s16)(effect->position.y + effect->offset.y));
            CombatSprite_Render(&effect->sprite);
            return 1;
        case 3:
            effect = task->data;
            CombatSprite_Release(&effect->sprite);
            return 1;
    }
}

Vec2p func_ov003_020a904c(s32 radius, s32 fixedRadius) {
    Vec2p offset;
    u16   angle;
    if (fixedRadius == 0)
        radius = RNG_Next(radius + 1);
    angle    = RNG_Next(0x10000);
    offset.x = (radius * FX_CosIdx(angle)) >> 12;
    offset.y = (radius * FX_SinIdx(angle)) >> 12;
    return offset;
}
void func_ov003_020a90c4(u32 id) {
    EasyTask_DeleteTask(&data_ov003_020e71b8->unk_201A4, id);
}
BOOL func_ov003_020a90e8(u32* id) {
    return EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_201A4, id);
}
void func_ov003_020a910c(u32 id, s32 x, s32 y) {
    Ov003BrilliantEffect* effect = func_ov003_020a6084(id);
    if (effect != NULL) {
        effect->position.x = x;
        effect->position.y = y;
    }
}
char             data_ov003_020e49bc[28] = "CmnTsk_BtlPsy_EffBrilliant";
const TaskHandle data_ov003_020dc638     = {data_ov003_020e49bc, func_ov003_020a8ec4, sizeof(Ov003BrilliantEffect)};
