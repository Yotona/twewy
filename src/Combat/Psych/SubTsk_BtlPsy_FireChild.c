#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc650;

void func_ov003_020a94d8(Ov003EffectState* effect, s32 x, s32 y, s32 kind, s32 attackId, s32 scale, s32 speed, s32 angle,
                         u32 parentTaskId) {
    Ov003FireChildArgs args;
    args.effect       = effect;
    args.position.x   = x;
    args.position.y   = y;
    args.kind         = kind;
    args.attackId     = attackId;
    args.scale        = scale;
    args.speed        = speed;
    args.angle        = angle;
    args.parentTaskId = parentTaskId;
    func_ov003_020a6000(&data_ov003_020dc650, &args);
}

s32 func_ov003_020a9530(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003FireChildArgs* arg = args;
    Ov003FireChild*     child;
    switch (stage) {
        default:
        case 0:
            child = task->data;
            MI_CpuSet(child, 0, sizeof(*child));
            child->state        = 0;
            child->effect       = arg->effect;
            child->position.x   = arg->position.x;
            child->position.y   = arg->position.y;
            child->scale        = arg->scale;
            child->speed        = arg->speed;
            child->angle        = arg->angle;
            child->parentTaskId = arg->parentTaskId;
            child->attackId     = arg->attackId;
            child->scale -= RNG_Next(0x334) + 0x19A;
            child->opacity = 0x1F000;
            func_ov003_020a3498(&child->sprite, 2, 1, arg->kind, 1);
            if (RNG_Next(2))
                CombatSprite_SetFlip(&child->sprite, 1);
            func_ov003_02087f28(0x149, child->position.x);
            return 1;
        case 1:
            child = task->data;
            if (SpriteMgr_IsAnimationFinished(&child->sprite.sprite))
                return 0;
            switch (child->state) {
                case 0:
                    child->state = 1;
                    break;
                case 1:
                    if (!func_ov003_020a603c(&child->parentTaskId)) {
                        child->state = 2;
                        break;
                    }
                    child->position.x += FX_Mul(child->speed, COS2(FX_SinCosTable_, child->angle));
                    child->position.y += FX_Mul(child->speed, SIN2(FX_SinCosTable_, child->angle));
                    if (func_ov003_020a6628(&child->effect->streak, (u16)child->attackId, child->position.x, child->position.y,
                                            0, 0, 0) == 1)
                        child->speed = 0;
                    break;
                case 2:
                    child->opacity -= 0x1800;
                    if (child->opacity < 0x1000)
                        return 0;
                    break;
            }
            CombatSprite_Update(&child->sprite);
            return 1;
        case 2: {
            s32 y;
            child                              = task->data;
            child->affine.rotation             = 0;
            child->affine.scaleX               = (child->sprite.flags46 & 1) ? -child->scale : child->scale;
            child->affine.scaleY               = child->scale;
            child->affine.unk_0C               = 0;
            child->affine.unk_0E               = 0;
            child->sprite.sprite.unk_0A.unk_00 = 1;
            child->sprite.affine               = &child->affine;
            y                                  = func_ov003_020843ec(0, child->position.y, 0);
            CombatSprite_SetPosition(&child->sprite, func_ov003_020843b0(0, child->position.x), y);
            func_ov003_02082730(&child->sprite, child->position.y + 0x1F4000);
            if (child->opacity < 0x1F000)
                CombatSprite_RenderWithPalette(&child->sprite, child->opacity >> 12);
            else
                CombatSprite_Render(&child->sprite);
            return 1;
        }
        case 3:
            child = task->data;
            CombatSprite_Release(&child->sprite);
            return 1;
    }
}
char             data_ov003_020e49f0[24] = "SubTsk_BtlPsy_FireChild";
const TaskHandle data_ov003_020dc650     = {data_ov003_020e49f0, func_ov003_020a9530, 0x98};
