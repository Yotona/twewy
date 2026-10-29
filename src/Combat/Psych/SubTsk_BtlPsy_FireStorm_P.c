#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlFireStorm {
    Ov003EffectState* effect;
    s32               cooldown;
} BtlFireStorm; // Size: 0x8

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern const TaskHandle data_ov003_020dc65c;

s32 func_ov003_020a9830(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a63e4())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc65c.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020dc65c;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

s32 func_ov003_020a9884(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlFireStorm* storm;
    switch (stage) {
        default:
        case 0:
            storm = task->data;
            MI_CpuSet(storm, 0, sizeof(*storm));
            storm->effect   = *(Ov003EffectState**)args;
            storm->cooldown = 0;
            func_ov003_020a6464(1);
            return 1;
        case 1: {
            BtlFireStorm* storm = task->data;
            s32           x, y, radius, offsetY;
            u32           parentId;
            if (!func_ov003_020a63e4())
                return 0;
            if (func_ov003_020a63a8(storm->effect))
                return 0;
            if (!func_ov003_020a5aa4(storm->effect))
                return 0;
            if (storm->cooldown > 0) {
                storm->cooldown--;
                return 1;
            }
            storm->cooldown = storm->effect->params[1];
            x               = func_ov003_0208442c(0, data_ov003_020e71b8->unk3D3AA);
            y               = func_ov003_02084460(0, data_ov003_020e71b8->unk3D3AC, 0);
            radius          = storm->effect->params[0];
            parentId        = ((u32)task->generation << 16) | task->id;
            offsetY         = -radius + RNG_Next(radius * 2 + 1);
            func_ov003_020a94d8(storm->effect, x + ((-radius + RNG_Next(radius * 2 + 1)) << 12), y + (offsetY << 12), 0x6B,
                                0x157, 0x1000, 0, 0, parentId);
            func_ov003_020a6494(data_ov003_020e71b8->unk3D3AA);
            func_ov003_020a5a94(storm->effect);
            return 1;
        }
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

extern char      data_ov003_020e4a24[24];
char             data_ov003_020e4a08[28] = "SubTsk_BtlPsy_FireStorm_P";
const TaskHandle data_ov003_020dc65c     = {data_ov003_020e4a08, func_ov003_020a9884, sizeof(BtlFireStorm)};
