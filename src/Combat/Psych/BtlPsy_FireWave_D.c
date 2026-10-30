#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

typedef struct FireWaveDef {
    s32 kind;
    s32 attackId;
} FireWaveDef;
extern const FireWaveDef data_ov003_020dc674[5];
extern const TaskHandle  data_ov003_020dc668;
extern s32               func_ov003_020a51c0(void);

// Nonmatching: mwcc merges the two identical task-handle literals.
s32 func_ov003_020a9a58(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc668.taskName);
    *arg                   = effect;
    context->handle        = &data_ov003_020dc668;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}

// Nonmatching: register allocation differs, and one true array owns the split configuration symbols.
s32 func_ov003_020a9ac4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003FireWave* wave;
    switch (stage) {
        default:
        case 0:
            wave = task->data;
            MI_CpuSet(wave, 0, sizeof(*wave));
            wave->effect     = *(Ov003EffectState**)args;
            wave->position.x = func_ov003_0208442c(0, data_ov003_020e71b8->touchWork.x);
            wave->position.y = func_ov003_02084460(0, data_ov003_020e71b8->touchWork.y, 0);
            wave->flags      = 0;
            wave->hasSpawned = 0;
            wave->exhausted  = 0;
            wave->seed       = 7;
            wave->multiplier = 0x5D588B656C078965ULL;
            wave->increment  = 0x269EC3;
            func_ov003_020a6464(1);
            func_ov003_020a647c(wave->position.x);
            return 1;
        case 1:
            wave = task->data;
            if (func_ov003_020a63a8(wave->effect))
                return 0;
            switch (wave->phase) {
                case 0:
                    wave->phase++;
                    wave->emitTimer = 0;
                    break;
                case 1:
                    if (wave->wasHeld == 0) {
                        wave->position.x  = func_ov003_0208442c(0, data_ov003_020e71b8->touchWork.x);
                        wave->position.y  = func_ov003_02084460(0, data_ov003_020e71b8->touchWork.y, 0);
                        wave->activeTicks = 0;
                        wave->flags |= 1;
                    }
                    wave->wasHeld = 0;
                    wave->phase++;
                    break;
                case 3:
                    wave->activeTicks = 0;
                    return 0;
            }
            wave->flags &= ~1;
            if (func_ov003_02085cfc(6) && wave->exhausted == 0) {
                if (!func_ov003_020a5aa4(wave->effect))
                    wave->exhausted = 1;
                wave->flags |= 1;
            }
            if (wave->flags & 1) {
                s32 x = func_ov003_0208442c(0, data_ov003_020e71b8->touchWork.x);
                s32 y = func_ov003_02084460(0, data_ov003_020e71b8->touchWork.y, 0);
                wave->position.x += (x - wave->position.x) >> 3;
                wave->position.y += (y - wave->position.y) >> 3;
                func_ov003_02083ca0(&wave->position.x, &wave->position.y, 0, 0);
                wave->emitTimer--;
                if (wave->emitTimer < 0) {
                    s32 attackId = data_ov003_020dc674[wave->effect->index].attackId;
                    if (attackId == 0x155 && func_ov003_020a51c0())
                        attackId = 0x156;
                    Ov003EffectState* effect   = wave->effect;
                    s32               speed    = effect->params[0];
                    s32               scale    = effect->params[1];
                    u16               angle    = RNG_Next(0x20) << 11;
                    u32               parentId = ((u32)task->generation << 16) | task->id;
                    func_ov003_020a94d8(effect, wave->position.x, wave->position.y, data_ov003_020dc674[effect->index].kind,
                                        attackId, scale, speed, angle, parentId);
                    func_ov003_020a5a94(wave->effect);
                    wave->seed       = wave->seed * wave->multiplier + wave->increment;
                    wave->emitTimer  = ((u64)(u32)(wave->seed >> 32) * 3 >> 32) + 1;
                    wave->hasSpawned = 1;
                }
                func_ov003_020a647c(wave->position.x);
                if (wave->hasSpawned)
                    func_ov003_02083ab0(0, wave->position.x, wave->position.y, 0);
                wave->activeTicks++;
            } else {
                if (wave->phase == 2)
                    wave->phase++;
                wave->wasHeld = 0;
            }
            return 1;
        case 2:
            return 1;
        case 3:
            func_ov003_020a6464(0);
            return 1;
    }
}

// Retail splits this table at the first attack id (data_ov003_020dc678).
// Keeping a real array avoids accesses beyond the first scalar object.
const FireWaveDef data_ov003_020dc674[5] = {
    {0x68, 0x151},
    {0x68, 0x152},
    {0x6A, 0x153},
    {0x68, 0x154},
    {0x69, 0x155}
};
char             data_ov003_020e4a24[24] = "Tsk_BtlPsy_FireWave_D";
const TaskHandle data_ov003_020dc668     = {data_ov003_020e4a24, func_ov003_020a9ac4, 0x38};
