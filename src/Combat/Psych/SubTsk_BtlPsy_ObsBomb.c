#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern s32                    func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void                   func_ov003_02087f28(s32, s32);
extern const Ov003AttackInfo* func_ov003_0208a114(u16);
extern s32                    func_ov003_0208a164(const Ov003AttackInfo*, Ov003AttackArgs*, s32, s32, s32);

extern const TaskHandle data_ov003_020dc718;

void func_ov003_020abb34(Ov003EffectState* effect, Ov003EnemyActor* unit) {
    Ov003PsychUnitArgs args;
    args.effect = effect;
    args.unit   = unit;
    func_ov003_020a6000(&data_ov003_020dc718, &args);
}

s32 func_ov003_020abb5c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs* arg = args;
    Ov003ObsBomb*       self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect = arg->effect;
            self->args.unit   = arg->unit;
            self->phase       = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, 0x9F, 1);
            return 1;
        case 1: {
            Ov003ObsBomb* self = task->data;
            switch (self->phase) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        self->phase = 1;
                        self->timer = 0;
                        func_ov003_020a35b8(&self->sprite, 0xA0, 1);
                        func_ov003_02087f28(0x43, self->args.unit->actor.position.x);
                    }
                    break;
                case 1:
                    s32 limit = self->args.effect->params[0];
                    self->timer++;
                    if (self->timer == limit)
                        return 0;
                    if ((self->args.unit->actor.flags & 0x10) || (self->args.unit->actor.flags & 4))
                        return 0;
                    if (func_ov003_020abdb8(self)) {
                        Ov003PsychEffectArgs effect;
                        func_ov003_020a88c0(&effect, self->args.unit->actor.position.x, self->args.unit->actor.position.y,
                                            self->args.unit->actor.position.z, 2, 0xA1);
                        func_ov003_020a899c(&effect, 0);
                        func_ov003_020a88a8(&effect);
                        func_ov003_020a61e4(self->args.unit, 0, 10, &self->args.effect->streak.args, RNG_Next(0xFFFF), 0x7000,
                                            -0x2000);
                        func_ov003_02087f28(0x44, self->args.unit->actor.position.x);
                        return 0;
                    }
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        func_ov003_02087f28(0x43, self->args.unit->actor.position.x);
                        CombatSprite_Restart(&self->sprite);
                    }
                    break;
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        }
        case 2: {
            Ov003ObsBomb*    self = task->data;
            Ov003EnemyActor* unit = self->args.unit;
            CombatSprite_SetPosition(&self->sprite, unit->actor.screenX, (s16)(unit->actor.screenY - unit->actor.unk_76));
            func_ov003_02082730(&self->sprite, unit->actor.position.y + 0x1F4000);
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

/// Adjusts the contact attack's dimensions to the selected object's dimensions.
s32 func_ov003_020abdb8(Ov003ObsBomb* self) {
    Ov003AttackInfo info  = *func_ov003_0208a114(0x190);
    s16             depth = self->args.unit->actor.unk_72;
    s16             width = self->args.unit->actor.unk_70;
    info.unk_05           = width;
    info.unk_06           = width / 2;
    info.unk_07           = depth;
    return func_ov003_0208a164(&info, &self->args.effect->streak.args, self->args.unit->actor.position.x,
                               self->args.unit->actor.position.y, self->args.unit->actor.position.z) == 1;
}

char             data_ov003_020e4af8[24] = "SubTsk_BtlPsy_ObsBomb";
const TaskHandle data_ov003_020dc718     = {data_ov003_020e4af8, func_ov003_020abb5c, sizeof(Ov003ObsBomb)};
