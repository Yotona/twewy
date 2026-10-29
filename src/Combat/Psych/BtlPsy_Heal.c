#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern void                      func_ov003_0209b564(s32, s32, CombatActor*, s32);
extern void                      func_ov003_0208b3cc(s32, s16);
extern const TaskHandle          data_ov003_020dca9c;
extern const Ov003HealAnimations data_ov003_020dcaa8[2][4];
extern char                      data_ov003_020e4f00[12];

s32 func_ov003_020b6368(Ov003EffectContext* context, Ov003EffectState* effect) {
    return 0;
}

s32 func_ov003_020b6370(void* args) {
    Ov003HealArgs* arg    = args;
    CombatActor*   player = func_ov003_020a6438();
    s32            amount = data_ov003_020e71b8->unk3D8D2 * arg->effect->params[1] / 100;
    func_ov003_020a5a94(arg->effect);
    func_ov003_02087f28(0x14E, player->position.x);
    func_ov003_020b67dc(arg->effect, player);
    func_ov003_0209b564(0, amount, player, 1);
    if (arg->effect->state == 0x3F)
        CombatActor_ClearPeriodicEffect(player);
    func_ov003_0208b3cc(0, (s16)amount);
    return func_ov003_020a5aa4(arg->effect);
}

s32 func_ov003_020b641c(Ov003EffectContext* context, Ov003EffectState* effect) {
    func_ov003_020a6438();
    if (data_ov003_020e71b8->unk3D8D0 >= data_ov003_020e71b8->unk3D8D2 * effect->params[0] / 100)
        return 0;
    Ov003HealArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020e4f00);
    arg->effect        = effect;
    context->callback  = func_ov003_020b6370;
    context->arg       = arg;
    return 1;
}

s32 func_ov003_020b6498(Ov003EffectContext* context, Ov003EffectState* effect) {
    CombatActor* player    = func_ov003_020a6438();
    s32          threshold = data_ov003_020e71b8->unk3D8D2 * effect->params[0] / 100;
    if (player->periodicEffectMode == 0 && data_ov003_020e71b8->unk3D8D0 >= threshold)
        return 0;
    Ov003HealArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020e4f00);
    arg->effect        = effect;
    context->callback  = func_ov003_020b6370;
    context->arg       = arg;
    return 1;
}

Ov003TouchActor* func_ov003_020b6520(void) {
    Ov003TouchActor* best      = NULL;
    s32              threshold = 0;
    Ov003TouchActor* actor     = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC);
    while (actor != NULL) {
        if (actor->kind != 0xFFFE && actor->state == 3 && actor->effect.state == 0x3D) {
            if (threshold < actor->effect.params[0]) {
                threshold = actor->effect.params[0];
                best      = actor;
            }
        }
        actor = func_ov003_02084984(&actor->node);
    }
    if (best != NULL && data_ov003_020e71b8->unk3D8D0 >= data_ov003_020e71b8->unk3D8D2 * threshold / 100)
        best = NULL;
    return best;
}

s32 func_ov003_020b65d8(void* args) {
    Ov003HealArgs* arg = args;
    if (data_ov003_020e71b8->unk3D8D0 >= data_ov003_020e71b8->unk3D8D2) {
        if (!func_ov003_020b68ec(arg->effect, data_ov003_020e71b8->unk3D898) &&
            !func_ov003_020b68ec(arg->effect, data_ov003_020e71b8->unk3D89C))
            return 0;
    }
    return 1;
}

// Nonmatching: the compiler merges two identical task-handle literals.
s32 func_ov003_020b6644(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003TouchActor* actor = func_ov003_020b6520();
    if (actor == NULL && !func_ov003_020a3f20(func_ov003_020a5a34(effect)))
        return 0;
    Ov003HealArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dca9c.taskName);
    arg->effect        = effect;
    arg->actor         = actor;
    context->handle    = (TaskHandle*)&data_ov003_020dca9c;
    context->arg       = arg;
    context->post      = func_ov003_020b65d8;
    return 1;
}

// Nonmatching: the compiler merges two identical task-handle literals.
s32 func_ov003_020b66b4(Ov003EffectContext* context, Ov003EffectState* effect) {
    Ov003TouchActor* actor = func_ov003_020b6520();
    if (actor == NULL && !func_ov003_020a3f20(func_ov003_020a5a34(effect)))
        return 0;
    Ov003HealArgs* arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020dca9c.taskName);
    arg->effect        = effect;
    arg->actor         = actor;
    context->handle    = (TaskHandle*)&data_ov003_020dca9c;
    context->arg       = arg;
    context->post      = func_ov003_020b65d8;
    return 1;
}

s32 func_ov003_020b6724(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003HealArgs*  arg = args;
    Ov003PsychArgs* self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->effect = arg->effect;
            func_ov003_02087250(self->effect, data_ov003_020e71b8->unk3D898, 0);
            func_ov003_020a5a94(self->effect);
            if (arg->actor != NULL) {
                func_ov003_020a40b0(arg->actor);
                func_ov003_020a40bc(arg->actor);
            }
            func_ov003_02087f00(0x9B, (s16)(func_ov003_020a5a34(self->effect)->x >> 12));
            return 1;
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return 1;
    }
}

// Nonmatching: animation-selection registers and paired field loads differ.
void func_ov003_020b67dc(Ov003EffectState* effect, CombatActor* actor) {
    s32 kind, index;
    switch (effect->state) {
        case 0x37:
        case 0x38:
            kind  = effect->state;
            index = effect->index;
            break;
        case 0x3E:
            kind  = 0x37;
            index = 1;
            break;
        case 0x3F:
            kind  = 0x38;
            index = 3;
            break;
        case 0x18:
        case 0x19:
        default:
            kind  = 0x37;
            index = 0;
            break;
    }
    Ov003PsychEffectArgs args;
    s32                  background = data_ov003_020dcaa8[kind - 0x37][index].background;
    func_ov003_020a8958(&args, actor, 2, data_ov003_020dcaa8[kind - 0x37][index].foreground);
    func_ov003_020a899c(&args, 1);
    func_ov003_020a88a8(&args);
    if (background < 0x102) {
        func_ov003_020a8958(&args, actor, 2, background);
        func_ov003_020a899c(&args, 0);
        func_ov003_020a88a8(&args);
    }
}

s32 func_ov003_020b68ec(Ov003EffectState* effect, CombatActor* actor) {
    if (effect->state != 0x38)
        return 0;
    switch (effect->index) {
        case 0:
            return actor->periodicEffectMode == 4;
        case 1:
            return actor->periodicEffectMode == 1;
        case 2:
            return actor->periodicEffectMode == 2 || actor->periodicEffectMode == 3;
        case 3:
            return actor->periodicEffectMode != 0;
        default:
            return 0;
    }
}

const Ov003HealAnimations data_ov003_020dcaa8[2][4] = {
    {{0xF2, 0x102}, {0xF5, 0xF7}, {0xF5, 0xF7}, {0xF5, 0xF7}},
    { {0xF8, 0xFA}, {0xF8, 0xFA}, {0xF8, 0xFA}, {0xF9, 0xFA}}
};
char             data_ov003_020e4ef0[16] = "Tsk_BtlPsy_Heal";
char             data_ov003_020e4f00[12] = "ARG_Heal_AT";
const TaskHandle data_ov003_020dca9c     = {data_ov003_020e4ef0, func_ov003_020b6724, sizeof(Ov003PsychArgs)};
