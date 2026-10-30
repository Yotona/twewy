#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>

extern const Ov003ShockWaveKind data_ov003_020dcb10[27];

extern s32 func_ov003_020a632c(s32, s32, s32, s32);
extern s32 func_ov003_020a66f4(Ov003AttackStreak* streak, u16 attackId, s32 x, s32 y, s32 force, u16 amount);
void       func_ov003_020b716c(Ov003ShockWaveBody* self);
void       func_ov003_020b71a0(Ov003ShockWaveBody* self);
s32        func_ov003_020b7240(Ov003ShockWaveBody* self);
s32        func_ov003_020b72b8(Ov003ShockWaveBody* self);
void       func_ov003_020b76e4(Ov003ShockWaveBody* self);
void       func_ov003_020b7760(Ov003ShockWaveBody* self);
s32        func_ov003_020b77a8(Ov003ShockWaveBody* self);
void       func_ov003_020b78b0(Ov003ShockWaveBody* self);
s32        func_ov003_020b7a9c(Ov003ShockWaveBody* self);
s32        func_ov003_020b7ad8(Ov003ShockWaveBody* self);

s32 func_ov003_020b6b38(s32 kind) {
    return data_ov003_020dcb10[kind].effectAnimation;
}

u32 func_ov003_020b6b50(u32 angle, u32 limit) {
    if (angle < 0x4000U) {
        if (angle > limit)
            angle = limit;
        return angle;
    }
    if (angle < 0x8000U) {
        if (angle < 0x8000U - limit)
            angle = 0x8000U - limit;
        return angle;
    }
    if (angle < 0xC000U) {
        if (angle > limit + 0x8000U)
            angle = limit + 0x8000U;
        return angle;
    }
    if (angle < 0x10000U - limit)
        angle = 0x10000U - limit;
    return angle;
}

// Nonmatching: objdiff reports an addend difference for the call to the local phase helper.
s32 func_ov003_020b6ba4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003ShockWaveArgs* arg = args;
    Ov003ShockWaveBody* self;
    s32                 animation;
    u16                 angle;

    switch (stage) {
        default:
        case 0: {
            s32 angleCopy;
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect        = arg->effect;
            self->kind          = arg->kind;
            self->x             = arg->position.x;
            self->y             = arg->position.y;
            self->z             = arg->position.z;
            self->trailIndex    = arg->flag;
            angleCopy           = arg->angle;
            self->angle         = angleCopy;
            self->renderAngle   = angleCopy;
            self->distance      = 0;
            self->distanceLimit = arg->unk_18;
            self->mode          = arg->mode;
            self->spinTimer     = 0;
            self->firstTick     = 1;
            self->speed         = data_ov003_020dcb10[self->kind].speed;
            self->acceleration  = data_ov003_020dcb10[self->kind].acceleration;
            self->scale         = 0x1000;
            switch (self->kind) {
                default:
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 21:
                case 22:
                case 24:
                case 25:
                    animation = data_ov003_020dcb10[self->kind].animation;
                    func_ov003_020a3498(&self->sprite, 2, 1, animation, 1);
                    self->animation = animation + 1;
                    break;
                case 17:
                case 18:
                case 19:
                case 20:
                case 23:
                case 26:
                    animation = data_ov003_020dcb10[self->kind].animation;
                    func_ov003_020a3498(&self->sprite, 2, 1, animation, 0);
                    self->animation = animation;
                    break;
            }
            switch (self->kind) {
                default:
                    break;
                case 17:
                case 18:
                    if (self->distanceLimit < 0x50000)
                        self->distanceLimit = 0x50000;
                    angle             = (u16)(self->angle + 0x800);
                    self->angle       = angle;
                    self->targetAngle = (u16)(angle - 0x9000);
                    self->phase       = 0;
                    break;
                case 19:
                case 20:
                    func_ov003_02087f28(0x57, self->x);
                    self->phase = 2;
                    break;
                case 7:
                    self->distanceLimit = 0x190000;
                    break;
                case 8:
                    self->distanceLimit = 0x190000;
                    break;
                case 5:
                    self->renderAngle   = 0;
                    self->distanceLimit = 0x78000;
                    break;
                case 6:
                    self->distanceLimit = 0x64000;
                    break;
                case 15:
                    self->acceleration = FX_Divide((RNG_Next(0x22) + 0xC) << 12, 0x64000);
                    angle              = (u16)(self->angle + (RNG_Next(0x3001) - 0x1800));
                    self->angle        = angle;
                    self->renderAngle  = angle;
                    break;
                case 16:
                    self->acceleration = FX_Divide((RNG_Next(0x22) + 0xC) << 12, 0x64000);
                    angle              = (u16)(self->angle + (RNG_Next(0x3001) - 0x1800));
                    self->angle        = angle;
                    self->renderAngle  = angle;
                    break;
                case 14:
                    self->acceleration = FX_Divide((RNG_Next(0x22) + 0xC) << 12, 0x64000);
                    angle              = (u16)(self->angle + (RNG_Next(0x3001) - 0x1800));
                    self->angle        = angle;
                    self->renderAngle  = angle;
                    break;
            }
            if (arg->trail != 0) {
                s32 sound = data_ov003_020dcb10[self->kind].trailSound;
                s32 sePan = self->x;
                if (sound < 0x56C)
                    func_ov003_02087f28(sound, sePan);
            }
            return 1;
        }
        case 1: {
            self = task->data;
            func_ov003_020b716c(self);
            if (self->firstTick != 0) {
                self->firstTick = 0;
                return 1;
            }
            func_ov003_020b71a0(self);
            if (!func_ov003_020b7240(self))
                return 0;
            if (!func_ov003_020b72b8(self))
                return 0;
            switch (self->kind) {
                default:
                case 0:
                case 1:
                case 3:
                case 4:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 21:
                case 22:
                case 24:
                case 25:
                    break;
                case 7:
                    func_ov003_020b76e4(self);
                    if (!func_ov003_020b7a9c(self))
                        return 0;
                    break;
                case 8:
                    func_ov003_020b7760(self);
                    if (!func_ov003_020b7a9c(self))
                        return 0;
                    break;
                case 2:
                    if (!func_ov003_020b77a8(self))
                        return 0;
                    break;
                case 5:
                case 6:
                    if (!func_ov003_020b7a9c(self))
                        return 0;
                    break;
                case 17:
                case 18:
                case 19:
                case 20:
                    func_ov003_020b78b0(self);
                    break;
                case 23:
                case 26:
                    if (!func_ov003_020b7ad8(self))
                        return 0;
                    break;
            }
            return 1;
        }
        case 2: {
            // The render path re-derives the on-screen position and mirrors the affine scale
            // for flipped sprites; the sprite keeps its position as 16-bit cells.
            self  = task->data;
            s32 x = func_ov003_020843b0(0, self->x);
            s32 y = func_ov003_020843ec(0, self->y, self->z);
            CombatSprite_SetPosition(&self->sprite, (s16)x, (s16)y);
            func_ov003_02082730(&self->sprite, self->y + 0x1F4000);
            s32 scale                         = self->scale;
            self->affine.rotation             = self->renderAngle;
            self->affine.scaleX               = (self->sprite.flags46 & 1) ? -scale : scale;
            self->affine.scaleY               = scale;
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

void func_ov003_020b716c(Ov003ShockWaveBody* self) {
    if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite) != 0)
        func_ov003_020a35b8(&self->sprite, self->animation, 0);
    CombatSprite_Update(&self->sprite);
}

void func_ov003_020b71a0(Ov003ShockWaveBody* self) {
    self->x += FX_Mul(self->speed, COS2(FX_SinCosTable_, self->angle));
    self->y += FX_Mul(self->speed, SIN2(FX_SinCosTable_, self->angle));
    self->distance += self->speed;
    self->speed += self->acceleration;
}

// Nonmatching: the two `return 1` sites are merged into one tail block in the target and the
// `if (phase == 4) return 0; return 1;` pair stays a branch there; mwcc if-converts it here.
s32 func_ov003_020b7240(Ov003ShockWaveBody* self) {
    s32 x = func_ov003_020843b0(0, self->x);
    s32 y = func_ov003_020843ec(0, self->y, self->z);

    if (func_ov003_020a6374(x, y, 0x30) == 0) {
        if (self->kind >= 0x11 && self->kind <= 0x14) {
            if (self->phase == 4)
                return 0;
            return 1;
        }
        return 0;
    }
    return 1;
}

s32 func_ov003_020b72b8(Ov003ShockWaveBody* self) {
    Ov003PsychEffectArgs anim15;
    Ov003PsychEffectArgs anim16;
    Ov003PsychEffectArgs anim18;
    Ov003PsychEffectArgs anim19;
    Ov003ShockWaveArgs   args15;
    Ov003ShockWaveArgs   args16;
    Ov003ShockWaveArgs   args18;
    Ov003ShockWaveArgs   args19;
    s32                  x;
    s32                  y;
    s32                  kind;
    s32                  i;
    s32                  offset;
    s16                  targetX;
    s16                  targetY;

    x    = func_ov003_020843b0(0, self->x);
    y    = func_ov003_020843ec(0, self->y, self->z);
    kind = data_ov003_020dcb10[self->kind].attackKind;
    if (kind == 0x180 && func_ov003_020a51c0() != 0)
        kind = 0x181;
    if (func_ov003_020a66f4(&self->effect->streak, (u16)kind, (s16)x, (s16)y, 0, 0) == 1) {
        s32 sound = data_ov003_020dcb10[self->kind].spawnSound;
        s32 sePan = self->x;

        if (sound < 0x56C)
            func_ov003_02087f28(sound, sePan);
        switch (self->kind - 0x15) {
            case 0:
                args15.effect     = self->effect;
                args15.kind       = 0x17;
                args15.position.x = self->x;
                args15.position.y = self->y;
                args15.position.z = self->z;
                args15.unk_18     = 0x46000;
                args15.mode       = self->mode;
                args15.trail      = 0;
                for (i = 0, offset = 0; i < 4; i++) {
                    args15.angle = (u16)(self->angle + 0x2000 + offset);
                    func_ov003_020b6b20(&args15);
                    offset += 0x4000;
                }
                func_ov003_020a88c0(&anim15, self->x, self->y, self->z, 2, 0x53);
                func_ov003_020a899c(&anim15, 0);
                func_ov003_020a88a8(&anim15);
                func_ov003_02087f28(0x69, self->x);
                return 0;
            case 1:
                func_ov003_020a88c0(&anim16, self->x, self->y, self->z, 2, 0x53);
                func_ov003_020a899c(&anim16, 0);
                func_ov003_020a88a8(&anim16);
                for (i = 0, offset = 0; i < 8; i++) {
                    args16.effect     = self->effect;
                    args16.kind       = 0x17;
                    args16.position.x = self->x;
                    args16.position.y = self->y;
                    args16.position.z = self->z;
                    args16.angle      = (u16)(self->angle + offset);
                    args16.unk_18     = 0x78000;
                    args16.mode       = self->mode;
                    args16.trail      = 0;
                    func_ov003_020b6b20(&args16);
                    offset += 0x2000;
                }
                func_ov003_02087f28(0x69, self->x);
                return 0;
            case 3:
                func_ov003_020a88c0(&anim18, self->x, self->y, self->z, 2, 0x58);
                func_ov003_020a899c(&anim18, 0);
                func_ov003_020a88a8(&anim18);
                for (i = 0, offset = 0; i < 4; i++) {
                    args18.effect     = self->effect;
                    args18.kind       = 0x1A;
                    args18.position.x = self->x;
                    args18.position.y = self->y;
                    args18.position.z = self->z;
                    args18.angle      = (u16)(self->angle + 0x2000 + offset);
                    args18.unk_18     = 0x46000;
                    args18.mode       = self->mode;
                    args18.trail      = 0;
                    func_ov003_020b6b20(&args18);
                    offset += 0x4000;
                }
                func_ov003_02087f28(0x69, self->x);
                return 0;
            case 4:
                func_ov003_020a88c0(&anim19, self->x, self->y, self->z, 2, 0x58);
                func_ov003_020a899c(&anim19, 0);
                func_ov003_020a88a8(&anim19);
                for (i = 0, offset = 0; i < 8; i++) {
                    args19.effect     = self->effect;
                    args19.kind       = 0x1A;
                    args19.position.x = self->x;
                    args19.position.y = self->y;
                    args19.position.z = self->z;
                    args19.angle      = (u16)(self->angle + offset);
                    args19.unk_18     = 0x78000;
                    args19.mode       = self->mode;
                    args19.trail      = 0;
                    func_ov003_020b6b20(&args19);
                    offset += 0x2000;
                }
                func_ov003_02087f28(0x69, self->x);
                return 0;
            default:
            case 2:
                if (self->mode == 0)
                    return 0;
                break;
        }
    }
    return 1;
}

void func_ov003_020b76e4(Ov003ShockWaveBody* self) {
    BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;
    s32           x     = func_ov003_020843b0(0, self->x);
    s32           y     = func_ov003_020843ec(0, self->y, self->z);

    if (!func_ov003_02085cfc(2))
        return;
    func_ov003_02087f28(0xA5, self->x);
    self->renderAngle = self->angle = func_ov003_020a62d0(x, y, touch->x, touch->y);
}

void func_ov003_020b7760(Ov003ShockWaveBody* self) {
    BtlTouchWork* touch = &data_ov003_020e71b8->touchWork;

    if (!func_ov003_02085cfc(4))
        return;
    func_ov003_02087f28(0xA5, self->x);
    self->renderAngle = self->angle = touch->angle;
}

s32 func_ov003_020b77a8(Ov003ShockWaveBody* self) {
    Ov003ShockWaveArgs args;
    u32                angle = self->angle;

    if (angle >= 0x4000 && angle <= 0xC000)
        self->renderAngle = (u16)(self->renderAngle - 0x2000);
    else
        self->renderAngle = (u16)(self->renderAngle + 0x2000);
    if (self->trailIndex <= 2 && self->distance >= 0x18000) {
        args.effect     = self->effect;
        args.kind       = self->kind;
        args.position.x = self->x;
        args.position.y = self->y;
        args.position.z = self->z;
        args.unk_18     = 0;
        args.mode       = self->mode;
        args.flag       = self->trailIndex + 1;
        args.trail      = 0;
        args.angle      = (u16)(self->angle + 0x1800);
        func_ov003_020b6b20(&args);
        args.angle = (u16)(self->angle - 0x1800);
        func_ov003_020b6b20(&args);
        func_ov003_02087f28(0xA5, self->x);
        return 0;
    }
    return 1;
}

void func_ov003_020b78b0(Ov003ShockWaveBody* self) {
    CombatActor* actor = data_ov003_020e71b8->unk3D898;
    s32          x;
    s32          y;
    s16          targetX;
    s16          targetY;

    self->spinTimer = (self->spinTimer + 1) % 8;
    if (self->spinTimer == 0)
        func_ov003_020b6978(self->animation, self->x, self->y, self->z, self->renderAngle);
    self->renderAngle = (u16)(self->renderAngle - 0x1800);
    switch (self->phase) {
        case 0:
            if (self->distance < self->distanceLimit)
                return;
            self->phase        = 1;
            self->acceleration = -self->acceleration;
            return;
        case 1:
            self->angle = (u16)(self->angle - 0x800);
            if (self->targetAngle != self->angle)
                return;
            self->phase    = 3;
            self->distance = 0;
            return;
        case 2:
            if (self->speed > 0)
                return;
            self->phase         = 3;
            self->distanceLimit = self->distance;
            self->distance      = 0;
            self->acceleration  = -self->acceleration;
            return;
        case 3:
            x = func_ov003_020843b0(0, self->x);
            y = func_ov003_020843ec(0, self->y, self->z);
            func_ov003_0208b370(&targetX, &targetY, actor->position.x, actor->position.y,
                                actor->position.z - ((actor->unk_72 / 2) << 12));
            self->angle = func_ov003_020a62d0(x, y, targetX, targetY);
            if (self->distance >= self->distanceLimit)
                self->phase = 4;
            if (func_ov003_020a632c(targetX, targetY, x, y) <= 0x10000)
                self->phase = 4;
            return;
        case 4:
            self->acceleration = 0xCD;
    }
}

s32 func_ov003_020b7a9c(Ov003ShockWaveBody* self) {
    if (self->distance >= self->distanceLimit)
        return 0;
    if (self->distance >= self->distanceLimit / 2)
        self->scale = 0x1800 - FX_Divide(self->distance, self->distanceLimit);
    return 1;
}

s32 func_ov003_020b7ad8(Ov003ShockWaveBody* self) {
    if (self->mode == 0) {
        if (self->distance >= self->distanceLimit)
            return 0;
        if (self->distance >= self->distanceLimit * 7 / 10) {
            self->scale -= 0x1F;
            self->scale -= 0x100;
        }
    }
    return 1;
}

void func_ov003_020b7b34(s32 x, s32 y, s32 z, s32 kind, s32 flip, u32* out) {
    Ov003PsychEffectArgs args;

    if (out != NULL && *out != (u32)-1) {
        if (func_ov003_020a603c(out) != 0) {
            func_ov003_020a6060(*out);
            *out = (u32)-1;
        }
    }
    func_ov003_020a88c0(&args, x, y, z, 2, func_ov003_020b6b38(kind));
    func_ov003_020a899c(&args, 0);
    if (flip != 0)
        func_ov003_020a89e4(&args);
    if (out != NULL) {
        *out = func_ov003_020a88a8(&args);
        return;
    }
    func_ov003_020a88a8(&args);
}

// Twenty-seven contiguous records, indexed by kind.
const Ov003ShockWaveKind data_ov003_020dcb10[27] = {
    {   0x0,  0x19A, 0x29, 0x28, 0x175,  0x6C, 0x6D},
    {   0x0,  0x19A, 0x29, 0x28, 0x176,  0x6C, 0x6D},
    {0x3800,    0x0, 0x2C, 0x2B, 0x177,  0xA4, 0x6D},
    {0x4000,   0xCD, 0x2F, 0x2E, 0x178,  0xA8, 0x6D},
    {0x3333,   0xCD, 0x32, 0x31, 0x179,  0xAA, 0x6D},
    {0x6000, -0x266, 0x35, 0x34, 0x17A,  0xA7, 0x6D},
    {0x4800,    0x0, 0x3C, 0x4A, 0x17B,  0x6C, 0x6D},
    {0x2000,    0x0, 0x38, 0x37, 0x17C,  0xA4, 0x6D},
    {0x2000,    0x0, 0x38, 0x37, 0x17D,  0xA4, 0x6D},
    {0x9000,    0x0, 0x3A, 0x4A, 0x17E,  0xA6, 0x6D},
    {0x5800,    0x0, 0x3C, 0x4A, 0x17F,  0x6C, 0x6D},
    {0x5800,    0x0, 0x3E, 0x4B, 0x180,  0xA9, 0x6D},
    {0x1CCD,  0x733, 0x40, 0x4A, 0x182,  0x6C, 0x6D},
    {0x8000,  -0xCD, 0x42, 0x4A, 0x183,  0xA6, 0x6D},
    {0x8000,  -0xCD, 0x44, 0x4A, 0x184,  0xA6, 0x6D},
    {0x24CD,    0x0, 0x46, 0x4A, 0x185,  0xA6, 0x6D},
    {   0x0,    0x0, 0x48, 0x4A, 0x186,  0xA6, 0x6D},
    {0x7000,    0x0, 0x4C, 0x4A, 0x142,  0x57, 0x58},
    {0x7000,    0x0, 0x4D, 0x4A, 0x142,  0x57, 0x58},
    {0xA000, -0x4CD, 0x4E, 0x4A, 0x142,  0x57, 0x58},
    {0xA000, -0x4CD, 0x4F, 0x4A, 0x142,  0x57, 0x58},
    {0x7000,    0x0, 0x50, 0x5A, 0x140,  0x68, 0x6A},
    {0x7000,    0x0, 0x50, 0x5A, 0x140,  0x68, 0x6A},
    {0x6000,    0x0, 0x52, 0x5A, 0x141, 0x56C, 0x6A},
    {0x7000,    0x0, 0x55, 0x5B, 0x140,  0x68, 0x6A},
    {0x7000,    0x0, 0x55, 0x5B, 0x140,  0x68, 0x6A},
    {0x6000,    0x0, 0x57, 0x5B, 0x141, 0x56C, 0x6A},
};
