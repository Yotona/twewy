#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

extern const TaskHandle data_ov003_020dce4c;
// One contiguous kind table indexed by effect variant.
extern const Ov003BurstShotKind data_ov003_020dce58[5];

s32  func_ov003_020b8398(Ov003EffectContext* context, Ov003EffectState* effect);
void func_ov003_020b8654(Ov003BurstShot* self);

s32 func_ov003_020b8398(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020a63e4())
        return 0;
    Ov003PsychArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dce4c.taskName);
    arg->effect         = effect;
    context->handle     = (TaskHandle*)&data_ov003_020dce4c;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}

s32 func_ov003_020b83ec(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs* arg = args;
    Ov003BurstShot* self;
    switch (stage) {
        default:
        case 0: {
            self                 = task->data;
            CombatActor*  player = func_ov003_020a6438();
            BtlTouchWork* touch  = func_ov003_020a62a0();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect      = arg->effect;
            self->x           = touch->x;
            self->y           = touch->y;
            self->soundPlayed = 0;
            func_ov003_020a6494(touch->x);
            func_ov003_020a6464(1);
            self->originX    = touch->x < player->screenX ? player->screenX - 0x18 : player->screenX + 0x18;
            self->originY    = player->screenY - 0x27;
            self->phase      = 0;
            self->timer      = 30;
            self->burstTimer = 4;
            return 1;
        }
        case 1:
            self = task->data;
            if (func_ov003_020a63a8(self->effect) != 0)
                return 0;
            switch (self->phase) {
                case 0:
                    func_ov003_020a5a94(self->effect);
                    if (self->timer > 0xE) {
                        if (self->soundPlayed == 0) {
                            func_ov003_02087f28(0x6E, func_ov003_020a6438()->position.x);
                            self->soundPlayed = 1;
                        }
                        if (self->burstTimer != 0) {
                            self->burstTimer--;
                        } else {
                            Ov003PsychEffectArgs effect;
                            func_ov003_020a8908(&effect, self->originX, self->originY, 2,
                                                data_ov003_020dce58[self->effect->index].animation);
                            func_ov003_020a899c(&effect, 0);
                            func_ov003_020a89b0(&effect, 0x1E);
                            func_ov003_020a89c8(&effect, RNG_Next(0x10) << 0xC, 0x1000, 0x1000);
                            func_ov003_020a88a8(&effect);
                            self->burstTimer = 2;
                        }
                    }
                    if (func_ov003_020a63e4() == 0) {
                        self->phase = 2;
                        self->timer = 12;
                    } else if (--self->timer == 0) {
                        self->phase = 1;
                        func_ov003_020b8654(self);
                        func_ov003_020a5aa4(self->effect);
                    }
                    break;
                case 1:
                    self->phase = 2;
                    self->timer = 12;
                    // fallthrough
                case 2:
                    if (--self->timer == 0)
                        return 0;
                    break;
            }
            return 1;
        case 2:
            return 1;
        case 3:
            func_ov003_02085c78();
            func_ov003_020a6464(0);
            return 1;
    }
}

void func_ov003_020b8654(Ov003BurstShot* self) {
    // Declaration order picks which callee-saved register each live value gets.
    s32                       left = 0;
    CombatActor*              player;
    s32                       originX;
    s32                       originY;
    const Ov003BurstShotKind* kind;
    s32                       targetX;
    s32                       targetY;
    kind    = &data_ov003_020dce58[self->effect->index];
    targetX = func_ov003_0208442c(0, self->x);
    targetY = func_ov003_02084460(0, self->y, -0x27000);
    player  = data_ov003_020e71b8->unk3D898;
    originY = player->position.y;
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
    angle = func_ov003_020b6b50(angle, data_ov003_020dce58[self->effect->index].angleLimit);
    Ov003ShockWaveArgs args;
    args.effect     = self->effect;
    args.kind       = kind->kind;
    args.position.x = originX;
    args.position.y = originY;
    args.position.z = -0x27000;
    args.angle      = angle;
    args.unk_18     = 0;
    args.mode       = kind->mode;
    args.flag       = 0;
    args.trail      = 1;
    func_ov003_020b6b20(&args);
    func_ov003_020b7b34(originX, originY, -0x27000, args.kind, left, NULL);
}

char                     data_ov003_020e4f70[24] = "Tsk_BtlPsy_BurstShot_T";
const TaskHandle         data_ov003_020dce4c     = {data_ov003_020e4f70, func_ov003_020b83ec, sizeof(Ov003BurstShot)};
const Ov003BurstShotKind data_ov003_020dce58[5]  = {
    {0x15, 0, 0x54, 0x2000},
    {0x16, 0, 0x54, 0x2000},
    {0x15, 1, 0x54, 0x2000},
    {0x16, 1, 0x54, 0x2000},
    {0x18, 0, 0x59, 0x2000},
};
