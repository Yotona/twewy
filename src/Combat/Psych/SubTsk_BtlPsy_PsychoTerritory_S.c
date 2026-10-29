#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern CombatActor*     func_ov003_02085dc8(s32);
extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void             func_ov003_02087ed8(s32);
extern s32              func_ov003_020843b0(s32, s32);
extern s32              func_ov003_020843ec(s32, s32, s32);
extern const TaskHandle data_ov003_020dd210;

s32 func_ov003_020ba6c8(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = (Ov003EnemyActor*)func_ov003_02085dc8(1);
    if (unit == NULL)
        return 0;
    func_ov003_020a6568(&effect->streak, 0x150, unit, 0, 0, 13);
    return 1;
}

s32 func_ov003_020ba714(Ov003EffectContext* context, Ov003EffectState* effect) {
    return 0;
}

s32 func_ov003_020ba71c(Ov003EffectState* effect, s32 x, s32 y, s32 angle) {
    Ov003PsychoTerritoryArgs args;
    args.effect = effect;
    args.x      = x;
    args.y      = y;
    args.angle  = angle;
    return func_ov003_020a6000(&data_ov003_020dd210, &args);
}

// Nonmatching: fixed-point endpoint register allocation and scheduling differ.
s32 func_ov003_020ba74c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychoTerritoryArgs* arg = args;
    Ov003PsychoTerritory*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = arg->effect;
            self->position.x = arg->x;
            self->position.y = arg->y;
            self->position.z = 0;
            self->angle      = arg->angle;
            self->phase      = 0;
            func_ov003_020a3498(&self->sprite, 2, 1, 0xE9, 1);
            func_ov003_02087ed8(0x3D);
            return 1;
        case 1:
            self = task->data;
            switch (self->phase) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        self->phase = 1;
                        func_ov003_020a35b8(&self->sprite, 0xEA, 1);
                        func_ov003_02087ed8(0x3E);
                    }
                    break;
                case 1:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        self->phase = 2;
                        self->timer = 0;
                    }
                    break;
                case 2:
                    s32 duration = self->effect->params[0];
                    if (++self->timer >= duration) {
                        self->phase = 3;
                        func_ov003_020a35b8(&self->sprite, 0xEB, 1);
                    }
                    break;
                case 3:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                        return 0;
                    break;
            }
            switch (self->phase) {
                case 1:
                case 2: {
                    s32 x      = func_ov003_020843b0(0, self->position.x);
                    s32 y      = func_ov003_020843ec(0, self->position.y, self->position.z);
                    s32 dy     = FX_Mul(FX_SinCosTable_[(self->angle >> 4) * 2], 0x12C000);
                    s32 dx     = FX_Mul(FX_SinCosTable_[(self->angle >> 4) * 2 + 1], 0x12C000);
                    s32 endY   = (dy + y * 0x1000) >> 12;
                    s32 startY = (y * 0x1000 - dy) >> 12;
                    s32 endX   = (dx + x * 0x1000) >> 12;
                    s32 startX = (x * 0x1000 - dx) >> 12;
                    if (func_ov003_020a6acc(&self->effect->streak, 0x16C, startX, startY, endX, endY, self->position.x) == 1)
                        func_ov003_02087ed8(0x3F);
                    break;
                }
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        case 2: {
            self  = task->data;
            s32 y = func_ov003_020843ec(0, self->position.y, self->position.z);
            s32 x = func_ov003_020843b0(0, self->position.x);
            CombatSprite_SetPosition(&self->sprite, x, y);
            func_ov003_02082730(&self->sprite, 0x384000);
            self->affine.rotation             = self->angle;
            self->affine.scaleX               = (self->sprite.flags46 & 1) ? -0x1000 : 0x1000;
            self->affine.scaleY               = 0x1000;
            self->affine.unk_0C               = 0;
            self->affine.unk_0E               = 0;
            self->sprite.affine               = &self->affine;
            self->sprite.sprite.unk_0A.unk_00 = 1;
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

char             data_ov003_020e5060[32] = "SubTsk_BtlPsy_PsychoTerritory_S";
const TaskHandle data_ov003_020dd210     = {data_ov003_020e5060, func_ov003_020ba74c, sizeof(Ov003PsychoTerritory)};
