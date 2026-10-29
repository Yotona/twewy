#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlAbsorbEffect {
    /* 0x00 */ Ov003EffectSlots* slots;
    /* 0x04 */ CombatSprite      sprite;
    /* 0x64 */ Vec               position;
} BtlAbsorbEffect; // Size: 0x70

extern s32 func_ov003_0208442c(s32, s32);
extern s32 func_ov003_02084460(s32, s32, s32);
extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc644;

void func_ov003_020a912c(Ov003EffectSlots* slots, s32 value) {
    Ov003AbsorbArgs args;
    args.slots = slots;
    args.value = value;
    func_ov003_020a6000(&data_ov003_020dc644, &args);
}

// Nonmatching: temporary vector stack slots and register allocation differ.
s32 func_ov003_020a9154(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlAbsorbEffect* effect;
    switch (stage) {
        default:
        case 0: {
            Ov003AbsorbArgs* arg = args;
            BtlTouchWork*    touch;
            effect = task->data;
            MI_CpuSet(effect, 0, sizeof(*effect));
            effect->slots      = arg->slots;
            touch              = &data_ov003_020e71b8->touchWork;
            effect->position.x = func_ov003_0208442c(0, touch->x);
            effect->position.y = func_ov003_02084460(0, touch->y, 0x1000);
            effect->position.z = 0x1000;
            effect->slots->unk_15++;
            // FX32_CONVERT deliberately evaluates RNG_Next twice per axis, as in retail.
            effect->position.x += FX32_CONVERT((RNG_Next(5) - 2) << 3);
            effect->position.y += FX32_CONVERT((RNG_Next(5) - 2) << 3);
            func_ov003_020a3498(&effect->sprite, 2, 1, RNG_Next(5) + 0xB0, 0);
            return 1;
        }
        case 1: {
            Vec current;
            Vec target;
            Vec direction;
            s32 distance;
            effect   = task->data;
            current  = (Vec) {effect->position.x, effect->position.y, effect->position.z};
            target   = effect->slots->position;
            distance = Vec_Distance(&current, &target);
            if (distance <= 0x1000)
                return 0;
            if (effect->slots->onAbsorb == NULL)
                return 0;
            Vec_Sub(&target, &current, &direction);
            Vec_Normalize(&direction, &direction);
            effect->position.x += FX_Mul(direction.x, 0x5000);
            effect->position.y += FX_Mul(direction.y, 0x5000);
            effect->position.z += FX_Mul(direction.z, 0x5000);
            if (distance < 0x5000) {
                effect->position.x = effect->slots->position.x;
                effect->position.y = effect->slots->position.y;
                effect->position.z = effect->slots->position.z;
            }
            CombatSprite_Update(&effect->sprite);
            return 1;
        }
        case 2: {
            s32 x, y;
            effect = task->data;
            x      = func_ov003_020843b0(0, effect->position.x);
            y      = func_ov003_020843ec(0, effect->position.y, effect->position.z);
            CombatSprite_SetPosition(&effect->sprite, x, y);
            func_ov003_02082730(&effect->sprite, effect->position.y + 0x1F4000);
            CombatSprite_Render(&effect->sprite);
            return 1;
        }
        case 3:
            effect = task->data;
            CombatSprite_Release(&effect->sprite);
            if (effect->slots->onAbsorb != NULL)
                effect->slots->onAbsorb(effect->slots->callbackArg);
            effect->slots->unk_15--;
            return 1;
    }
}

char             data_ov003_020e49d8[24] = "Tsk_BtlPsy_AbsorbEffect";
const TaskHandle data_ov003_020dc644     = {data_ov003_020e49d8, func_ov003_020a9154, sizeof(BtlAbsorbEffect)};
