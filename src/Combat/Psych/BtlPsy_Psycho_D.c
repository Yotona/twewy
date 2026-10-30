#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern void             func_020265d4(s32*, s32, u16);
extern const TaskHandle data_ov003_020dca78;

s32  func_ov003_020b537c(Ov003EffectContext* context, Ov003EffectState* effect);
s32  func_ov003_020b5744(Ov003PsychoD* self);
s32  func_ov003_020b585c(Ov003PsychoD* self);
s32  func_ov003_020b5934(Ov003PsychoD* self);
s32  func_ov003_020b5c84(Ov003PsychoD* self);
void func_ov003_020b5d40(Ov003PsychoD* self, s32 phase);

s32 func_ov003_020b537c(Ov003EffectContext* context, Ov003EffectState* effect) {
    BtlTouchWork* touch = func_ov003_020a62a0();
    if (!func_ov003_02085cfc(6))
        return 0;
    if (func_ov003_02085d3c(1))
        return 0;
    Ov003EnemyActor* unit = (Ov003EnemyActor*)touch->held;
    if (unit == NULL || &unit->actor != touch->target || (u32)(unit->actor.unk_00 - 1) > 1 || (unit->actor.flags & 0x10))
        return 0;
    if (!func_ov003_020a60ec(unit, 5))
        return 0;
    switch (effect->index) {
        case 0:
        case 1:
            if (unit->actor.unk_00 != 2)
                return 0;
            break;
        case 2:
            break;
        default:
            return 0;
    }
    {
        Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dca78.taskName);
        arg->effect             = effect;
        arg->unit               = unit;
        context->handle         = &data_ov003_020dca78;
        context->arg            = arg;
        context->post           = NULL;
        return 1;
    }
}

// Nonmatching: mwcc allocates the stage-2 sprite x/y pair to r5/r6 in the opposite order from the
// target (x -> r6, y -> r5) and shuffles them into the arguments before the first SetPosition.
s32 func_ov003_020b5480(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs* arg = args;
    Ov003PsychoD*       self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->unit   = arg->unit;
            self->flags  = 0;
            self->angle  = 0;
            self->radius = 0;
            func_ov003_020a6464(1);
            func_ov003_020a5a94(self->effect);
            func_ov003_020a3498(&self->spriteB, 2, 1, 0xAC, 1);
            func_ov003_020a3498(&self->spriteA, 2, 1, 0xAD, 0);
            func_ov003_020b5d40(self, 0);
            return 1;
        case 1: {
            self = task->data;
            if (self->phase != 3) {
                if (func_ov003_020a63a8(self->effect)) {
                    func_ov003_020a6198(self->unit, 5);
                    return 0;
                }
                func_ov003_020a5a94(self->effect);
                if (!func_ov003_020a5aa4(self->effect)) {
                    func_ov003_020a6198(self->unit, 5);
                    return 0;
                }
            }
            if (self->phase != 0 && !func_ov003_020a60a8(self->unit, 5))
                return 0;
            func_ov003_020a647c(self->unit->actor.position.x);
            switch (self->phase) {
                case 1:
                case 2:
                    if (SpriteMgr_IsAnimationFinished(&self->spriteB.sprite)) {
                        CombatSprite_Restart(&self->spriteB);
                        func_ov003_02087f28(0x5D, self->unit->actor.position.x);
                    }
                    CombatSprite_Update(&self->spriteB);
                    CombatSprite_Update(&self->spriteA);
                    break;
            }
            switch (self->phase) {
                case 0:
                    return func_ov003_020b5744(self);
                case 1:
                    return func_ov003_020b585c(self);
                case 2:
                    return func_ov003_020b5934(self);
                case 3:
                    return func_ov003_020b5c84(self);
                default:
                    return 0;
            }
        }
        case 2: {
            self  = task->data;
            s32 x = self->unit->actor.screenX;
            s32 y = self->unit->actor.screenY - self->unit->actor.unk_76;
            switch (self->phase) {
                case 1:
                case 2:
                    CombatSprite_SetPosition(&self->spriteB, (s16)x, (s16)y);
                    func_ov003_02082730(&self->spriteB, 0x385000);
                    CombatSprite_Render(&self->spriteB);
                    CombatSprite_SetPosition(&self->spriteA, (s16)x, (s16)y);
                    func_ov003_02082730(&self->spriteA, 0x384000);
                    CombatSprite_Render(&self->spriteA);
                    break;
            }
            return 1;
        }
        case 3:
            self = task->data;
            func_ov003_02085c78();
            func_ov003_020a6464(0);
            CombatSprite_Release(&self->spriteB);
            CombatSprite_Release(&self->spriteA);
            return 1;
    }
}

s32 func_ov003_020b5744(Ov003PsychoD* self) {
    Ov003EnemyActor* unit = self->unit;
    if (!func_ov003_02085cfc(6)) {
        func_ov003_020a6198(self->unit, 5);
        func_ov003_02087f28(0x5C, unit->actor.position.x);
        return 0;
    }
    if (self->timer == 0) {
        if (!func_ov003_020a6164(unit, 5))
            return 0;
        func_ov003_02087f28(0x5B, unit->actor.position.x);
        self->x     = unit->actor.position.x;
        self->y     = unit->actor.position.y;
        self->timer = 0xA;
    }
    if (self->timer > 0) {
        unit->actor.position.x = self->x;
        unit->actor.position.y = self->y;
        if (self->timer % 4 <= 1)
            unit->actor.position.x += 0x1000;
        else
            unit->actor.position.x -= 0x1000;
        func_020265d4(&unit->actor.position.z, -0x10000, (u16)self->timer);
        unit->actor.zVelocity = 0;
        self->timer--;
    }
    if (self->timer <= 0) {
        func_ov003_02087f28(0x5C, unit->actor.position.x);
        func_ov003_020b5d40(self, 1);
    }
    return 1;
}

// Nonmatching: mwcc if-converts the `dy <= 4` result to movle/movgt where the target keeps a
// branchy 0/1 assignment.
s32 func_ov003_020b585c(Ov003PsychoD* self) {
    Ov003EnemyActor* unit = self->unit;
    if (!func_ov003_02085cfc(6)) {
        func_ov003_020a6198(self->unit, 5);
        return 0;
    }
    unit->actor.position.x = self->x;
    unit->actor.position.y = self->y;
    unit->actor.position.z = -0x10000;
    unit->actor.zVelocity  = 0;
    s32 dx                 = data_ov003_020e71b8->touchWork.startX - data_ov003_020e71b8->touchWork.x;
    if (dx < 0)
        dx = -dx;
    s32 near;
    if (dx <= 4) {
        s32 dy = data_ov003_020e71b8->touchWork.startY - data_ov003_020e71b8->touchWork.y;
        if (dy < 0)
            dy = -dy;
        near = dy <= 4;
    } else {
        near = 0;
    }
    if (!near)
        func_ov003_020b5d40(self, 2);
    return 1;
}

// Nonmatching: the final "above" flag is materialised as a branchy 0/1 value in the target; mwcc
// if-converts it to movgt here and reloads `unit` in a different order.
s32 func_ov003_020b5934(Ov003PsychoD* self) {
    BtlTouchWork*    touch = &data_ov003_020e71b8->touchWork;
    Ov003EnemyActor* unit  = self->unit;
    if (!func_ov003_02085cfc(6) && !func_ov003_02085cfc(7)) {
        s16 half = unit->actor.unk_70;
        if (func_ov003_02083af8(unit->actor.position.x, unit->actor.position.y, half << 12, (half / 2) << 12)) {
            func_ov003_020b5d40(self, 3);
            return 1;
        }
        func_ov003_020a6198(self->unit, 5);
        return 0;
    }
    switch (self->subPhase) {
        case 0: {
            if (self->timer == 0)
                self->timer = 0xA;
            s32 x = func_ov003_0208442c(0, data_ov003_020e71b8->touchWork.x);
            s32 y = func_ov003_02084460(0, (s16)(data_ov003_020e71b8->touchWork.y - (0xC - unit->actor.unk_72) / 2), -0x10000);
            if (self->timer > 0) {
                func_020265d4(&unit->actor.position.x, x, (u16)self->timer);
                func_020265d4(&unit->actor.position.y, y, (u16)self->timer);
                unit->actor.position.z = -0x10000;
                unit->actor.zVelocity  = 0;
                self->timer--;
            }
            if (self->timer <= 0) {
                self->flags |= 1;
                self->subPhase++;
                self->timer = 0;
            }
            break;
        }
        case 1:
            unit->actor.position.x = func_ov003_0208442c(0, data_ov003_020e71b8->touchWork.x);
            unit->actor.position.y =
                func_ov003_02084460(0, (s16)(data_ov003_020e71b8->touchWork.y - (0xC - unit->actor.unk_72) / 2), -0x10000);
            unit->actor.position.z = -0x10000;
            unit->actor.zVelocity  = 0;
            break;
    }
    self->angle = FX_Atan2Idx(touch->velocityY, touch->velocityX);
    self->radius -= 0x1000;
    if (self->radius < 0)
        self->radius = 0;
    s32 vx = touch->velocityX * 0x19A;
    s32 vy = touch->velocityY * 0x19A;
    self->radius += FX_Sqrt(FX_Mul(vx, vx) + FX_Mul(vy, vy));
    if (self->radius > 0x5000)
        self->radius = 0x5000;
    s16 half = unit->actor.unk_70;
    if (func_ov003_02083b8c(&unit->actor.position.y, (half / 2) << 12, unit->actor.position.x)) {
        s32 above = 0;
        if (self->flags & 1)
            above = ((self->unit->actor.screenY - self->unit->actor.unk_72) - 0x20) > data_ov003_020e71b8->touchWork.y;
        if (above) {
            func_ov003_020a6198(self->unit, 5);
            return 0;
        }
    }
    if (self->radius >= 0x1000 && func_ov003_020a6b84(&self->effect->streak, 0x187, unit) == 1)
        func_ov003_02087f28(0x65, self->unit->actor.position.x);
    func_ov003_02083ab0(0, unit->actor.position.x, unit->actor.position.y, unit->actor.position.z);
    return 1;
}

s32 func_ov003_020b5c84(Ov003PsychoD* self) {
    Ov003EnemyActor* unit = self->unit;
    if (self->timer == 0) {
        self->timer           = 0x10;
        unit->actor.xVelocity = 0;
        unit->actor.yVelocity = 0;
        self->x               = unit->actor.position.x;
        self->y               = unit->actor.position.y;
        s16 half              = unit->actor.unk_70;
        func_ov003_02083ca0(&self->x, &self->y, half << 12, (half / 2) << 12);
    }
    if (self->timer > 0) {
        func_020265d4(&unit->actor.position.x, self->x, (u16)self->timer);
        func_020265d4(&unit->actor.position.y, self->y, (u16)self->timer);
        self->timer--;
    }
    if (self->timer > 0)
        return 1;
    func_ov003_020a6198(self->unit, 5);
    return 0;
}

void func_ov003_020b5d40(Ov003PsychoD* self, s32 phase) {
    self->phase    = phase;
    self->subPhase = 0;
    self->timer    = 0;
}

char             data_ov003_020e4eb4[0x14] = "Tsk_BtlPsy_Psycho_D";
const TaskHandle data_ov003_020dca78       = {data_ov003_020e4eb4, func_ov003_020b5480, sizeof(Ov003PsychoD)};
