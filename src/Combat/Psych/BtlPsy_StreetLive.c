#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern s32                          func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern const TaskHandle             data_ov003_020dd1dc;
extern const Ov003StreetLiveSpeaker data_ov003_020dd1e8[5];

s32 func_ov003_020ba2e8(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (func_ov003_020a5b18(0x36))
        return 0;
    if (!func_ov003_020a50cc())
        return 0;
    Ov003PsychArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dd1dc.taskName);
    arg->effect         = effect;
    context->handle     = &data_ov003_020dd1dc;
    context->arg        = arg;
    context->post       = NULL;
    return 1;
}

s32 func_ov003_020ba350(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003PsychArgs*  arg = args;
    Ov003StreetLive* self;
    switch (stage) {
        default:
        case 0: {
            self                = task->data;
            CombatActor* player = func_ov003_020a6438();
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            self->effect->unk_0A++;
            func_ov003_020a6464(1);
            func_ov003_020a64ac();
            func_ov003_020a3498(&self->sprite, 2, 1, self->effect->index + 0xDC, 0);
            self->speakerPhase = 0;
            for (u32 i = 0; i < 5; i++) {
                const Ov003StreetLiveSpeaker* speaker = &data_ov003_020dd1e8[i];
                func_ov003_020b9be0(self->effect, player->position.x + speaker->x * 0x1000,
                                    player->position.y + speaker->y * 0x1000, speaker->kind, &self->speakerPhase);
            }
            func_ov003_020a4e84(&self->camera, 0, 0);
            func_ov003_020a4ed0(&self->camera, 4, 0x1800);
            self->phase = 0;
            self->timer = 0;
            func_ov003_02087f28(0x5A, player->position.x - 0x1E000);
            func_ov003_02087804(0);
            return 1;
        }
        case 1:
            self = task->data;
            func_ov003_020a5a94(self->effect);
            switch (self->phase) {
                case 0:
                    if (++self->timer == 10) {
                        self->phase        = 1;
                        self->quietFrames  = 0;
                        self->timer        = 0;
                        self->speakerPhase = 1;
                        switch (self->effect->index) {
                            case 0:
                                func_ov003_020a35b8(&self->sprite, 0xE0, 0);
                                break;
                            case 1:
                                func_ov003_020a35b8(&self->sprite, 0xE1, 1);
                                break;
                            case 2:
                                func_ov003_020a35b8(&self->sprite, 0xE2, 0);
                                break;
                            case 3:
                                func_ov003_020a35b8(&self->sprite, 0xE3, 0);
                                break;
                        }
                        func_ov003_020ba68c(self);
                    }
                    break;
                case 1:
                    if (func_ov003_020a50cc()) {
                        self->quietFrames = 0;
                    } else {
                        if (++self->quietFrames >= 60) {
                            self->phase        = 2;
                            self->speakerPhase = 2;
                        }
                    }
                    func_ov003_020a4ee8(&self->camera, 0, 0x1800, 0x1800);
                    s32 interval = self->effect->params[0];
                    if (++self->timer == interval) {
                        self->timer = 0;
                        func_ov003_020ba68c(self);
                    }
                    break;
                case 2:
                    return 0;
            }
            if (!func_ov003_020a5aa4(self->effect) || func_ov003_020a63a8(self->effect)) {
                self->phase        = 2;
                self->speakerPhase = 2;
            }
            if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                func_ov003_020a35b8(&self->sprite, 0xE4, 0);
            CombatSprite_Update(&self->sprite);
            return 1;
        case 2:
            self = task->data;
            CombatSprite_SetPosition(&self->sprite, 128, 96);
            func_ov003_02082730(&self->sprite, 0x385000);
            CombatSprite_Render(&self->sprite);
            return 1;
        case 3:
            self = task->data;
            func_ov003_020a4fd0(&self->camera);
            CombatSprite_Release(&self->sprite);
            func_ov003_020a6464(0);
            self->effect->unk_0A--;
            return 1;
    }
}

void func_ov003_020ba68c(Ov003StreetLive* self) {
    CombatActor* player = func_ov003_020a6438();
    func_ov003_020ba08c(self->effect, player->position.x - 0x1E000, player->position.y - 0x52000);
    func_ov003_02087f28(0x5A, player->position.x - 0x1E000);
}

char                         data_ov003_020e5048[24] = "Tsk_BtlPsy_StreetLive";
const TaskHandle             data_ov003_020dd1dc     = {data_ov003_020e5048, func_ov003_020ba350, sizeof(Ov003StreetLive)};
const Ov003StreetLiveSpeaker data_ov003_020dd1e8[5]  = {
    {-30, -58, 0},
    { 10, -55, 1},
    { 44, -42, 1},
    {-57, -36, 1},
    {-71, -12, 1}
};
