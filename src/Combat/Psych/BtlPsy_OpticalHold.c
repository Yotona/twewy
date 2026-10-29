#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

extern s32  func_ov003_0208442c(s32, s32);
extern s32  func_ov003_02084460(s32, s32, s32);
extern s32  func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void func_ov003_02087f28(s32, s32);
/// Owned by the IcePillar_S unit; only the task name string lives here.
extern const TaskHandle data_ov003_020dc90c;

/// Task payload of the OpticalHold effect: three ramp sprites over the held actor plus six
/// shared animation particles.

/// OpticalHold task entry: starts the effect once the touch helper has a held actor.
s32 func_ov003_020b15dc(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003EnemyActor* unit = func_ov003_020b1c50();
    if (unit == NULL)
        return 0;
    Ov003PsychUnitArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dc90c.taskName);
    arg->effect             = effect;
    arg->unit               = unit;
    context->handle         = (TaskHandle*)&data_ov003_020dc90c;
    context->arg            = arg;
    context->post           = NULL;
    return 1;
}

// objdiff flags the intra-TU `bl func_ov003_020b1c50` relocation only because this object emits one
// .text section per function while the delinked target keeps a single one; the encoded instruction
// (and its post-link displacement) is identical.
s32 func_ov003_020b1634(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychUnitArgs* arg = args;
    Ov003OpticalHold*   self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->unit   = arg->unit;
            func_ov003_020a6464(1);
            self->state      = 0;
            self->heldFrames = 0;
            self->holdTimer  = 5;
            self->spawnTimer = 0;
            func_ov003_020a3498(&self->sprites[2], 2, 1, 0xCD, 0);
            func_ov003_020a3498(&self->sprites[1], 2, 1, 0xCC, 0);
            func_ov003_020a3498(&self->sprites[0], 2, 1, 0xCC, 0);
            func_ov003_02087f28(0x4F, self->unit->actor.position.x);
            self->angle = 0;
            func_ov003_020a5d88(self->particles, 6, 7);
            return 1;
        case 1: {
            // Declared in reverse so each state keeps its own 0x54-byte particle argument block.
            Ov003PsychEffectArgs effectArgs1;
            Ov003PsychEffectArgs effectArgs0;
            self = task->data;
            if (func_ov003_020a63a8(self->effect) || (self->state == 2 && !func_ov003_020a5aa4(self->effect)) ||
                self->holdTimer == 0)
            {
                switch (self->state) {
                    case 0:
                        func_ov003_020a6464(0);
                        return 0;
                    case 1:
                    case 2:
                        self->state = 3;
                        func_ov003_020a6464(0);
                        break;
                    case 3:
                        break;
                }
            }
            if (self->unit != func_ov003_020b1c50()) {
                self->holdTimer--;
            } else {
                self->holdTimer = 5;
                self->heldFrames++;
            }
            switch (self->state) {
                default:
                    break;
                case 0:
                    if (self->heldFrames >= 0x1E)
                        self->state = 1;
                    if (self->spawnTimer == 0) {
                        s32 y;
                        s32 x;
                        self->spawnTimer = 10;
                        y                = func_ov003_02084460(0, (s16)(self->unit->actor.screenY - self->unit->actor.unk_76),
                                                               self->unit->actor.position.z);
                        x                = func_ov003_0208442c(0, self->unit->actor.screenX);
                        func_ov003_020a88c0(&effectArgs0, x, y, self->unit->actor.position.z, 2, 0xCE);
                        func_ov003_020a899c(&effectArgs0, 0);
                        func_ov003_020a89b0(&effectArgs0, 0x1E);
                        func_ov003_020a89c8(&effectArgs0, RNG_Next(0x10) << 0xC, 0x1000, 0x1000);
                        func_ov003_020a88a8(&effectArgs0);
                    }
                    self->spawnTimer--;
                    break;
                case 1:
                    self->angle += 0x52;
                    if (self->angle > 0x800) {
                        self->angle = 0x800;
                        self->state = 2;
                        func_ov003_02087f28(0x50, self->unit->actor.position.x);
                    }
                    if (self->spawnTimer == 0) {
                        s32 y;
                        s32 x;
                        self->spawnTimer = 5;
                        y                = func_ov003_02084460(0, (s16)(self->unit->actor.screenY - self->unit->actor.unk_76),
                                                               self->unit->actor.position.z);
                        x                = func_ov003_0208442c(0, self->unit->actor.screenX);
                        func_ov003_020a88c0(&effectArgs1, x, y, self->unit->actor.position.z, 2, 0xCE);
                        func_ov003_020a899c(&effectArgs1, 0);
                        func_ov003_020a89b0(&effectArgs1, 0x1E);
                        func_ov003_020a89c8(&effectArgs1, RNG_Next(0x10) << 0xC, 0x1000, 0x1000);
                        func_ov003_020a88a8(&effectArgs1);
                    }
                    self->spawnTimer--;
                    break;
                case 2:
                    if (func_ov003_020a6568(&self->effect->streak, 0x16A, self->unit, self->unit->actor.position.x, 0,
                                            self->effect->unk_08 >= 8 ? (u16)0x11 : (u16)0) == 1)
                        func_ov003_02087f28(0x51, self->unit->actor.position.x);
                    func_ov003_020a5a94(self->effect);
                    break;
                case 3:
                    return 0;
            }
            func_ov003_020a647c(self->unit->actor.position.x);
            for (s32 i = 0; i < 3; i++)
                CombatSprite_Update(&self->sprites[i]);
            switch (self->state) {
                case 0:
                case 1:
                    func_ov003_020a5db8(self->particles, 6, 0, 0x40000, 0x1800, 0x400);
                    break;
            }
            return 1;
        }
        case 2: {
            Ov003OpticalHold* self = task->data;
            s32               y;
            s32               screenX;
            s32               i = 0; // seeded in its declaration so both loop inductions start in order
            CombatSprite*     sprite;
            s32               sortKey = 0;
            y                         = (s16)(self->unit->actor.screenY - self->unit->actor.unk_76);
            screenX                   = self->unit->actor.screenX;
            sprite                    = self->sprites;
            for (; i < 3; i++, sprite++, sortKey += 0x1000) {
                CombatSprite_SetPosition(sprite, screenX, y);
                func_ov003_02082730(sprite, 0x384000 + sortKey);
                switch (i) {
                    case 0: {
                        s32 angle = self->angle;
                        s32 scale = 0x1800 - angle;
                        s32 alpha;
                        CombatSprite_SetAffineTransform(sprite, 0, scale, scale, 0);
                        // 20.12 angle ramp -> 0..0x10 palette level; retail rounds at 0x800 and
                        // shifts in two steps, so keep both shifts for matching codegen.
                        alpha = (s32)((((s64)(angle * 2) << 16) + 0x800) >> 12) >> 12;
                        if (alpha > 0)
                            CombatSprite_RenderWithPalette(sprite, alpha);
                        break;
                    }
                    case 1: {
                        s32 angle = self->angle;
                        s32 scale = angle + 0x800;
                        s32 alpha;
                        CombatSprite_SetAffineTransform(sprite, 0, scale, scale, 0);
                        alpha = (s32)((((s64)(angle * 2) << 15) + 0x800) >> 12) >> 12;
                        if (alpha > 0)
                            CombatSprite_RenderWithPalette(sprite, alpha);
                        break;
                    }
                    case 2:
                        if (self->state == 2)
                            CombatSprite_Render(sprite);
                        break;
                }
            }
            switch (self->state) {
                case 0:
                case 1:
                    func_ov003_020a5ed4(self->particles, 6, screenX, y);
                    break;
            }
            return 1;
        }
        case 3: {
            s32 i;
            self = task->data;
            func_ov003_020a5fc0(self->particles, 6);
            for (i = 0; i < 3; i++)
                CombatSprite_Release(&self->sprites[i]);
            if (SndMgr_IsSEPlaying(SEIDX_SE_PSY_OPTICALHOLD01_01))
                SndMgr_StopPlayingSE(SEIDX_SE_PSY_OPTICALHOLD01_01);
            return 1;
        }
    }
}

/// Returns the held actor while the optical-hold touch is active, or NULL when it is not.
Ov003EnemyActor* func_ov003_020b1c50(void) {
    BtlTouchWork*    touch;
    Ov003EnemyActor* unit;
    if (!func_ov003_02085cfc(6))
        return NULL;
    if (func_ov003_02085d3c(1))
        return NULL;
    touch = func_ov003_020a62a0();
    unit  = (Ov003EnemyActor*)touch->heldActor;
    if (unit == NULL)
        return NULL;
    if (unit->actor.unk_00 != 1)
        unit = NULL;
    return unit;
}

char             data_ov003_020e4d34[24] = "Tsk_BtlPsy_OpticalHold";
const TaskHandle data_ov003_020dc90c     = {data_ov003_020e4d34, func_ov003_020b1634, sizeof(Ov003OpticalHold)};
