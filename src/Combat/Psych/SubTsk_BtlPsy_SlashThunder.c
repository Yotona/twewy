#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32              func_ov003_0208442c(s32, s32);
extern s32              func_ov003_02084460(s32, s32, s32);
extern s32              func_ov003_020843b0(s32, s32);
extern s32              func_ov003_020843ec(s32, s32, s32);
extern s32              func_ov003_020a62b8(s32, s32, s32, s32);
extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void             func_ov003_02087f28(s32, s32);
extern s32              func_ov003_020a51c0(void);
extern const TaskHandle data_ov003_020dd000;

// Nonmatching: coordinate and side-selection register scheduling differs.
s32 func_ov003_020b8e60(Ov003ShockWaveArgs* args, s32 x, s32 y) {
    s32          left    = 0;
    s32          targetX = func_ov003_0208442c(0, x);
    s32          targetY = func_ov003_02084460(0, y, -0x27000);
    CombatActor* player  = data_ov003_020e71b8->unk3D898;
    s32          originX;
    s32          originY = player->position.y;
    if (targetX < player->position.x) {
        originX = player->position.x - 0x18000;
        left    = 1;
    } else {
        originX = player->position.x + 0x18000;
    }
    s32 angle = func_ov003_020a62b8(originX, originY, targetX, targetY);
    if (!left) {
        if ((player->position.x >> 12) < (targetX >> 12) && (targetX >> 12) <= (originX >> 12))
            angle = 0;
    } else {
        if ((player->position.x >> 12) > (targetX >> 12) && (targetX >> 12) >= (originX >> 12))
            angle = 0x8000;
    }
    args->position.x = originX;
    args->position.y = originY;
    args->position.z = -0x27000;
    args->angle      = angle;
    args->unk_18     = 0;
    args->flag       = 0;
    args->trail      = 1;
    return left;
}

// Nonmatching: bounded descriptor-field access and local-call scheduling differ.
s32 func_ov003_020b8f3c(Ov003StraightShot* self) {
    Ov003TouchActor*             actor  = func_ov003_020a5a34(self->effect);
    Ov003EffectState*            effect = self->effect;
    const Ov003StraightShotKind* kind   = &data_ov003_020dcf38[effect->index];
    Ov003ShockWaveArgs           args;
    args.effect = effect;
    s32 left    = func_ov003_020b8e60(&args, self->x, self->y);
    args.angle  = func_ov003_020b6b50(args.angle, data_ov003_020dcf38[self->effect->index].angleLimit);
    args.mode   = kind->mode;
    args.kind   = actor->params.unk_05 == 0xE ? kind->alternateKind : kind->kind;
    for (s32 i = 0; i < kind->count; i++) {
        func_ov003_020b6b20(&args);
        args.trail = 0;
    }
    func_ov003_020b7b34(args.position.x, args.position.y, args.position.z, args.kind, left, &self->effectTaskId);
    self->count++;
    func_ov003_020a40b0(actor);
    return func_ov003_020a40bc(actor);
}

s32 func_ov003_020b9040(Ov003EffectState* effect, s32 kind, s32 x, s32 y, s32 previousX, s32 previousY, s32 lifetime) {
    Ov003SlashThunderArgs args;
    args.effect    = effect;
    args.kind      = kind;
    args.x         = x;
    args.y         = y;
    args.previousX = previousX;
    args.previousY = previousY;
    args.lifetime  = lifetime;
    return func_ov003_020a6000(&data_ov003_020dd000, &args);
}

// Nonmatching: bounded descriptor-table fields and fixed-point scheduling differ.
s32 func_ov003_020b9088(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003SlashThunderArgs* arg = args;
    Ov003SlashThunder*     self;
    switch (stage) {
        default:
        case 0: {
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect     = arg->effect;
            self->phase      = 0;
            self->kind       = arg->kind;
            self->lifetime   = arg->lifetime;
            self->position.x = func_ov003_0208442c(0, (s16)arg->x);
            self->position.y = func_ov003_02084460(0, (s16)arg->y, 0);
            self->angle      = func_ov003_020a62d0(arg->x, arg->y, arg->previousX, arg->previousY);
            func_ov003_020a3498(&self->sprite, 2, 1, data_ov003_020dd00c[self->kind].animations[self->phase], 1);
            s32 offset             = data_ov003_020dd00c[self->kind].offset;
            s32 dy                 = FX_Mul(offset, FX_SinCosTable_[(self->angle >> 4) * 2]);
            s32 dx                 = FX_Mul(offset, FX_SinCosTable_[(self->angle >> 4) * 2 + 1]);
            s16 y                  = arg->y + (dy >> 12);
            self->renderPosition.x = func_ov003_0208442c(0, (s16)(arg->x + (dx >> 12)));
            self->renderPosition.y = func_ov003_02084460(0, y, 0);
            func_ov003_02087f28(0x59, self->renderPosition.x);
            return 1;
        }
        case 1:
            self = task->data;
            switch (self->phase) {
                case 0:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        self->phase++;
                        func_ov003_020a35b8(&self->sprite, data_ov003_020dd00c[self->kind].animations[self->phase], 1);
                        func_ov003_02087f28(0x59, self->renderPosition.x);
                    }
                    break;
                case 1: {
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                        CombatSprite_Restart(&self->sprite);
                        func_ov003_02087f28(0x59, self->renderPosition.x);
                    }
                    if (--self->lifetime == 0) {
                        self->phase++;
                        func_ov003_020a35b8(&self->sprite, data_ov003_020dd00c[self->kind].animations[self->phase], 1);
                    }
                    s32 x        = func_ov003_020843b0(0, self->position.x);
                    s32 y        = func_ov003_020843ec(0, self->position.y, 0);
                    s32 dx       = FX_Mul(FX_SinCosTable_[(self->angle >> 4) * 2 + 1], 0x12C000);
                    s32 dy       = FX_Mul(FX_SinCosTable_[(self->angle >> 4) * 2], 0x12C000);
                    s32 endX     = (dx + x * 0x1000) >> 12;
                    s32 endY     = (dy + y * 0x1000) >> 12;
                    s32 attackId = data_ov003_020dd00c[self->kind].attackId;
                    if (attackId == 0x159 && func_ov003_020a51c0())
                        attackId = 0x15A;
                    func_ov003_020a6acc(&self->effect->streak, (u16)attackId, x, y, endX, endY, self->position.x);
                    break;
                }
                case 2:
                    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                        return 0;
                    break;
            }
            CombatSprite_Update(&self->sprite);
            return 1;
        case 2: {
            self  = task->data;
            s32 y = func_ov003_020843ec(0, self->renderPosition.y, 0);
            s32 x = func_ov003_020843b0(0, self->renderPosition.x);
            CombatSprite_SetPosition(&self->sprite, x, y);
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

char                        data_ov003_020e4fc0[28] = "SubTsk_BtlPsy_SlashThunder";
const TaskHandle            data_ov003_020dd000     = {data_ov003_020e4fc0, func_ov003_020b9088, sizeof(Ov003SlashThunder)};
const Ov003SlashThunderKind data_ov003_020dd00c[4]  = {
    {{0x15, 0x16, 0x17}, 0x158, 0x9E000},
    {{0x18, 0x19, 0x1A}, 0x159, 0x9E000},
    {{0x1B, 0x1C, 0x1D}, 0x158, 0x9E000},
    {{0x1E, 0x1F, 0x20}, 0x15B, 0x70000}
};
