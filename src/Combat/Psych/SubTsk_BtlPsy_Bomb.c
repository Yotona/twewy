#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlBombKind {
    s32 attackId;
    s32 animation;
    s32 burstAnimation;
} BtlBombKind;

// Retail delinking names the three words separately; they form one indexed record.
const BtlBombKind data_ov003_020dc700[1] = {
    {0x18F, 0x9D, 0x9E}
};

extern s32 func_ov003_020843b0(s32, s32);
extern s32 func_ov003_020843ec(s32, s32, s32);
extern s32 func_ov003_020a66f4(Ov003AttackStreak*, s32, s32, s32, s32, u16);
extern s32 func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc6f4;

void func_ov003_020ab524(Ov003AttackStreak* streak, s32 kind, s32 x, s32 y, s32 z, s32 angle, s32 speed) {
    Ov003BombArgs args;
    args.streak     = streak;
    args.kind       = kind;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    args.angle      = angle;
    args.speed      = speed;
    func_ov003_020a6000(&data_ov003_020dc6f4, &args);
}

// Nonmatching: merged descriptor symbols and rodata object ordering differ.
s32 func_ov003_020ab56c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003BombArgs* arg = args;
    Ov003Bomb*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->args.streak     = arg->streak;
            self->args.kind       = arg->kind;
            self->args.position.x = arg->position.x;
            self->args.position.y = arg->position.y;
            self->args.position.z = arg->position.z;
            self->args.angle      = arg->angle;
            self->args.speed      = arg->speed;
            func_ov003_020a3498(&self->sprite, 2, 1, data_ov003_020dc700[self->args.kind].animation, 0);
            return 1;
        case 1: {
            Ov003Bomb* self = task->data;
            s32        x    = func_ov003_020843b0(0, self->args.position.x);
            s32        y    = func_ov003_020843ec(0, self->args.position.y, self->args.position.z);
            if (!func_ov003_020a6374(x, y, 32))
                return 0;
            if (func_ov003_020a66f4(self->args.streak, (u16)data_ov003_020dc700[self->args.kind].attackId, x, y, 0, 0) == 1) {
                Ov003PsychEffectArgs effect;
                func_ov003_020a88c0(&effect, self->args.position.x, self->args.position.y, self->args.position.z, 2,
                                    data_ov003_020dc700[self->args.kind].burstAnimation);
                func_ov003_020a88a8(&effect);
                func_ov003_02087f28(0x56, self->args.position.x);
                return 0;
            }
            self->args.position.x += FX_Mul(self->args.speed, FX_SinCosTable_[(self->args.angle >> 4) * 2 + 1]);
            self->args.position.y += FX_Mul(self->args.speed, FX_SinCosTable_[(self->args.angle >> 4) * 2]);
            CombatSprite_Update(&self->sprite);
            return 1;
        }
        case 2: {
            Ov003Bomb* self = task->data;
            s32        x    = func_ov003_020843b0(0, self->args.position.x);
            s32        y    = func_ov003_020843ec(0, self->args.position.y, self->args.position.z);
            CombatSprite_SetPosition(&self->sprite, x, y);
            func_ov003_02082730(&self->sprite, 0x38E000);
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

char             data_ov003_020e4acc[20] = "SubTsk_BtlPsy_Bomb";
const TaskHandle data_ov003_020dc6f4     = {data_ov003_020e4acc, func_ov003_020ab56c, 0x7C};
