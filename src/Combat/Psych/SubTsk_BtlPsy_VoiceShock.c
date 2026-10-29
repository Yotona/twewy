#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32   func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void* func_ov003_0208495c(CombatList*);
extern void* func_ov003_02084984(CombatListNode*);
extern s32   func_ov003_020a632c(s32, s32, s32, s32);

extern const TaskHandle data_ov003_020dd1d0;

s32 func_ov003_020ba08c(Ov003EffectState* effect, s32 x, s32 y) {
    Ov003VoiceShockArgs args;
    args.effect = effect;
    args.x      = x;
    args.y      = y;
    return func_ov003_020a6000(&data_ov003_020dd1d0, &args);
}

s32 func_ov003_020ba0b8(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003VoiceShockArgs* arg = args;
    Ov003VoiceShock*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = arg->effect;
            self->position.x = arg->x;
            self->position.y = arg->y;
            self->position.z = 0;
            self->scale      = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, self->effect->index + 0xE5, 0);
            return 1;
        case 1: {
            Ov003VoiceShock* shock = task->data;
            Ov003EnemyActor* unit;
            s32              y;
            s32              x = func_ov003_020843b0(0, shock->position.x);
            y                  = func_ov003_020843ec(0, shock->position.y, shock->position.z);
            shock->scale += 0x4CD;
            if (shock->scale >= 0xA000)
                return 0;
            unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
            while (unit != NULL) {
                s32 distance = func_ov003_020a632c(x, y, unit->actor.screenX, unit->actor.screenY - unit->actor.unk_76);
                s32 radius   = FX_Mul(shock->scale, 0x10000);
                if (radius + (unit->actor.unk_70 / 2) * 0x1000 >= distance)
                    func_ov003_020a6568(&shock->effect->streak, (u16)(shock->effect->index + 0x171), unit, shock->position.x,
                                        0, 0);
                unit = func_ov003_02084984(&unit->node);
            }
            CombatSprite_Update(&shock->sprite);
            return 1;
        }
        case 2: {
            self  = task->data;
            s32 x = func_ov003_020843b0(0, self->position.x);
            s32 y = func_ov003_020843ec(0, self->position.y, self->position.z);
            CombatSprite_SetAffineTransform(&self->sprite, 0, self->scale, self->scale, 1);
            CombatSprite_SetPosition(&self->sprite, x, y);
            func_ov003_02082730(&self->sprite, 0x384000);
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

extern char      data_ov003_020e502c[28];
const TaskHandle data_ov003_020dd1d0     = {data_ov003_020e502c, func_ov003_020ba0b8, sizeof(Ov003VoiceShock)};
char             data_ov003_020e502c[28] = "SubTsk_BtlPsy_VoiceShock";
