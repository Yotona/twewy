#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s32 func_ov003_020843b0(s32, s32);
extern s32 func_ov003_020843ec(s32, s32, s32);
extern s32 func_ov003_020a66f4(Ov003AttackStreak*, s32, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc724;

void func_ov003_020abe30(Ov003EffectState* effect, s32 x, s32 y, s32 z) {
    Ov003PsychPositionArgs args;
    args.effect     = effect;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    func_ov003_020a6000(&data_ov003_020dc724, &args);
}

// Nonmatching: sprite-loop register allocation, sort-key construction and sound-load scheduling differ.
s32 func_ov003_020abe60(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychPositionArgs* arg = args;
    Ov003PsychoBomb*        self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.effect     = arg->effect;
            self->args.position.x = arg->position.x;
            self->args.position.y = arg->position.y;
            self->args.position.z = arg->position.z;
            self->timer           = self->args.effect->params[0];
            func_ov003_020a5aa4(self->args.effect);
            func_ov003_020a5a94(self->args.effect);
            func_ov003_020a3498(&self->sprites[1], 2, 1, 0x90, 0);
            func_ov003_020a3498(&self->sprites[0], 2, 1, 0x93, 0);
            func_ov003_02087f28(0x96, self->args.position.x);
            return 1;
        case 1: {
            Ov003PsychoBomb* self   = task->data;
            s32              active = 1;
            if (self->phase < 3) {
                if (--self->timer == 0) {
                    self->phase++;
                    if (self->phase == 3) {
                        CombatSprite_Release(&self->sprites[0]);
                        CombatSprite_Release(&self->sprites[1]);
                        func_ov003_020a3498(&self->sprites[1], 2, 1, self->phase + 0x93, 1);
                        self->timer = 18;
                        func_ov003_02087f28(0xAD, self->args.position.x);
                    } else {
                        self->timer = self->args.effect->params[0];
                        CombatSprite_Release(&self->sprites[1]);
                        func_ov003_020a3498(&self->sprites[1], 2, 1, self->phase + 0x90, 0);
                        s32 sound = self->phase + 0x96;
                        if (sound < 0x56C)
                            func_ov003_02087f28(sound, self->args.position.x);
                    }
                }
            } else {
                s32 remaining = 0;
                if (self->timer > 0 && --self->timer == 0) {
                    Ov003PsychEffectArgs effect;
                    func_ov003_020a88c0(&effect, self->args.position.x, self->args.position.y, self->args.position.z, 2, 0x98);
                    func_ov003_020a899c(&effect, 10);
                    func_ov003_020a88a8(&effect);
                }
                for (s32 i = 0; i < 2; i++) {
                    if (Sprite_HasAnimation(&self->sprites[i].sprite)) {
                        if (!SpriteMgr_IsAnimationFinished(&self->sprites[i].sprite))
                            remaining++;
                        else
                            CombatSprite_Release(&self->sprites[i]);
                    }
                }
                if (remaining == 0)
                    active = 0;
                s32 y = func_ov003_020843ec(0, self->args.position.y, self->args.position.z);
                s32 x = func_ov003_020843b0(0, self->args.position.x);
                if (func_ov003_020a66f4(&self->args.effect->streak, 0x18E, x, y, 0, 0) == 1)
                    func_ov003_02087f28(0x56, self->args.position.x);
            }
            for (s32 i = 0; i < 2; i++) {
                if (Sprite_HasAnimation(&self->sprites[i].sprite))
                    CombatSprite_Update(&self->sprites[i]);
            }
            return active;
        }
        case 2: {
            Ov003PsychoBomb* self = task->data;
            s32              x    = func_ov003_020843b0(0, self->args.position.x);
            s32              y    = func_ov003_020843ec(0, self->args.position.y, self->args.position.z);
            for (s32 i = 0; i < 2; i++) {
                if (Sprite_HasAnimation(&self->sprites[i].sprite)) {
                    CombatSprite_SetPosition(&self->sprites[i], x, y);
                    func_ov003_02082730(&self->sprites[i], 0x384000 + i * 0x1000);
                    CombatSprite_Render(&self->sprites[i]);
                }
            }
            return 1;
        }
        case 3:
            self = task->data;
            for (s32 i = 0; i < 2; i++) {
                if (Sprite_HasAnimation(&self->sprites[i].sprite))
                    CombatSprite_Release(&self->sprites[i]);
            }
            return 1;
    }
}
char             data_ov003_020e4b10[28] = "SubTsk_BtlPsy_PsychoBomb";
const TaskHandle data_ov003_020dc724     = {data_ov003_020e4b10, func_ov003_020abe60, sizeof(Ov003PsychoBomb)};
