#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

extern s32  func_02026528(s32, s32);
extern void func_02026590(s32*, s32, u16);
extern void func_020265d4(s32*, s32, u16);
extern s32  func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern s16  func_ov003_020a40cc(Ov003TouchActor*, s32);
extern s32  func_ov003_020a62b8(s32, s32, s32, s32);

/// Task handle owned by the neighbouring `BtlPsy_PsychoFall_Z` unit.
extern const TaskHandle data_ov003_020dca40;

// One bounded table replaces retail's three interior symbol views.
const Ov003PsychoTuningTable data_ov003_020dca4c = {
    {{{10, 10, 10}, {30, 60, 90}, 0x400}, {{10, 10, 10}, {30, 60, 90}, 0x400}, {{10, 10, 10}, {30, 60, 90}, 0x400}},
    0,
};

/// Size class of an enemy: 0 = small, 1 = medium, 2 = large.
s32 func_ov003_020b4e2c(u32 size) {
    if (size < 0xC)
        return 0;
    if (size < 0x20)
        return 1;
    return 2;
}

void func_ov003_020b4e48(Ov003EnemyActor* unit, Ov003PsychoHitContext* context) {
    Ov003PsychoUnitArgs args;

    if (!func_ov003_020a60ec(unit, 5))
        return;
    args.unit    = unit;
    args.context = context;
    func_ov003_020a6000(&data_ov003_020dca40, &args);
}

// Nonmatching: bounded tuning records replace the retail table's interior symbol views.
s32 func_ov003_020b4e8c(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychoUnitArgs* arg = args;
    Ov003PsychoUnit*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->unit    = arg->unit;
            self->context = arg->context;
            func_ov003_020a3498(&self->sprite, 2, 1, 0xAE, 1);
            func_ov003_020a6164(self->unit, 5);
            self->phase = 0;
            self->timer = data_ov003_020dca4c.rows[self->context->effect->index]
                              .holdFrames[func_ov003_020b4e2c(self->unit->actor.unk_70)];
            func_ov003_020a40cc(func_ov003_020a5a34(self->context->effect),
                                data_ov003_020dca4c.rows[self->context->effect->index]
                                    .lifetimeCost[func_ov003_020b4e2c(self->unit->actor.unk_70)]);
            self->context->refCount++;
            func_ov003_02087f28(0x9C, self->unit->actor.position.x);
            return 1;
        case 1: {
            s32 result = 0;
            self       = task->data;
            if (self->context->finished)
                goto done;
            if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
                result = 1;
                CombatSprite_Restart(&self->sprite);
            }
            if (result)
                func_ov003_02087f28(0x9D, self->unit->actor.position.x);
            CombatSprite_Update(&self->sprite);
            if (self->phase >= 2) {
                self->unit->actor.zVelocity = 0;
                if (!func_ov003_020a60a8(self->unit, 5)) {
                    result = 0;
                    goto done;
                }
            }
            switch (self->phase) {
                case 0:
                    self->phase = 2;
                    break;
                case 2:
                    if (self->timer % 4 <= 1)
                        self->unit->actor.position.x += 0x2000;
                    else
                        self->unit->actor.position.x -= 0x2000;
                    func_020265d4(&self->unit->actor.position.z, -0x40000, self->timer);
                    if (self->context->activated) {
                        self->phase = 3;
                    } else {
                        self->timer--;
                        if (self->timer == 0)
                            self->phase = 3;
                    }
                    break;
                case 3:
                    if (self->context->activated) {
                        self->phase = 4;
                        self->timer = 0xF;
                        if (self->context->target != NULL) {
                            self->context->targetPosition.x = self->context->target->position.x;
                            self->context->targetPosition.y = self->context->target->position.y;
                            self->context->targetPosition.z = self->context->target->position.z;
                        }
                        self->angle  = func_ov003_020a62b8(self->unit->actor.position.x, self->unit->actor.position.y,
                                                           self->context->targetPosition.x, self->context->targetPosition.y);
                        self->unk_74 = self->context->effect->params[1];
                    }
                    break;
                case 4:
                    if (self->context->target != NULL) {
                        s32 delta, step;
                        self->context->targetPosition.x = self->context->target->position.x;
                        self->context->targetPosition.y = self->context->target->position.y;
                        self->context->targetPosition.z = self->context->target->position.z;
                        delta = func_02026528(func_ov003_020a62b8(self->unit->actor.position.x, self->unit->actor.position.y,
                                                                  self->context->targetPosition.x,
                                                                  self->context->targetPosition.y),
                                              self->angle);
                        step  = data_ov003_020dca4c.rows[self->context->effect->index].turnStep;
                        if (delta > 0)
                            self->angle = (u16)(self->angle + step);
                        else
                            self->angle = (u16)(self->angle - step);
                    }
                    func_02026590(&self->unit->actor.position.x, self->context->targetPosition.x, self->timer);
                    func_02026590(&self->unit->actor.position.y, self->context->targetPosition.y, self->timer);
                    func_02026590(&self->unit->actor.position.z, self->context->targetPosition.z, self->timer);
                    func_ov003_02083c50(&self->unit->actor.position.x, 0);
                    func_ov003_02083bfc(&self->unit->actor.position.y, 0, self->unit->actor.position.x);
                    self->timer--;
                    if (func_ov003_020a6b84(&self->context->effect->streak, 0x188, self->unit) == 1) {
                        self->phase = 1;
                        result      = 1;
                        goto done;
                    } else if (self->timer == 0) {
                        self->phase = 1;
                        result      = 1;
                        goto done;
                    }
                    break;
                case 1:
                    result = 0;
                    goto done;
                default:
                    break;
            }
            result = 1;
        done:
            return result;
        }
        case 2: {
            Ov003EnemyActor* unit;
            s32              x, y;
            self = task->data;
            unit = self->unit;
            x    = unit->actor.screenX;
            y    = unit->actor.screenY - unit->actor.unk_76;
            if (self->phase != 3 && self->phase != 4)
                return 1;
            CombatSprite_SetPosition(&self->sprite, (s16)x, (s16)y);
            func_ov003_02082730(&self->sprite, self->unit->actor.position.y + 0x1F5000);
            CombatSprite_Render(&self->sprite);
            return 1;
        }
        case 3:
            self = task->data;
            self->context->refCount--;
            CombatSprite_Release(&self->sprite);
            func_ov003_020a6198(self->unit, 5);
            return 1;
    }
}

char             data_ov003_020e4e9c[24] = "SubTsk_BtlPsy_Psycho_T";
const TaskHandle data_ov003_020dca40     = {data_ov003_020e4e9c, func_ov003_020b4e8c, sizeof(Ov003PsychoUnit)};
