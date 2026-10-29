#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlPsySmash {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ u16               tier;
    /* 0x06 */ s16               particleTimer;
    /* 0x08 */ void (*update)(struct BtlPsySmash*);
    /* 0x0C */ s16 state;
    /* 0x0E */ s16 timer;
    /* 0x10 */ s32 active;
} BtlPsySmash; // Size: 0x14

void func_ov003_020956c4(BtlPsySmash*);

void func_ov003_020953b4(BtlPsySmash* smash, void (*update)(BtlPsySmash*)) {
    smash->update = update;
    smash->state  = 0;
    smash->timer  = 0;
}
s32 func_ov003_020953c8(void) {
    if (func_ov003_02085cfc(6)) {
        Ov003Global* global = data_ov003_020e71b8;
        if (global->heldActor != NULL && global->heldActor->unk_00 == 0)
            return global->unk3D3C4 > 16;
    }
    return 0;
}
void func_ov003_02095428(s32 tier) {
    Ov003PsychEffectArgs args;
    CombatActor*         actor = data_ov003_020e71b8->unk3D898;
    s32                  kind;
    switch (tier) {
        case 0:
            kind = 0xD0;
            break;
        case 1:
            kind = 0xD1;
            break;
        case 2:
            kind = 0xD2;
            break;
        default:
            kind = 0xD2;
            break;
    }
    func_ov003_020a88c0(&args, actor->position.x, actor->position.y, actor->position.z - (actor->unk_76 << 12), 2, kind);
    func_ov003_020a899c(&args, 0);
    func_ov003_020a89b0(&args, 30);
    func_ov003_020a89c8(&args, RNG_Next(16) << 12, 0x1000, 0x1000);
    func_ov003_020a88a8(&args);
}
void func_ov003_020954e4(void) {
    Ov003PsychEffectArgs args;
    CombatActor*         actor = data_ov003_020e71b8->unk3D898;
    func_ov003_020a88c0(&args, actor->position.x, actor->position.y, actor->position.z - (actor->unk_76 << 12), 2, 0xD3);
    func_ov003_020a899c(&args, 0);
    func_ov003_020a88a8(&args);
}
void func_ov003_02095548(BtlPsySmash* smash) {
    if (smash->particleTimer == 0) {
        smash->particleTimer = 6;
        func_ov003_02095428(smash->tier);
    }
    smash->particleTimer--;
}
void func_ov003_0209557c(Ov003EffectState* effect, s32 tier) {
    switch (tier) {
        case 0:
            effect->unk_0C = effect->unk_0C * 10 / 100;
            break;
        case 1:
            effect->unk_0C = effect->unk_0C * 20 / 100;
            break;
        case 2:
            effect->unk_0C = effect->unk_0C * 50 / 100;
            break;
    }
}
void func_ov003_02095608(BtlPsySmash* smash) {
    func_ov003_02095548(smash);
    smash->timer++;
    if (smash->timer >= 60) {
        smash->tier = 3;
        func_ov003_020954e4();
        func_ov003_020953b4(smash, func_ov003_020956c4);
    } else if (smash->timer >= 40)
        smash->tier = 2;
    else if (smash->timer >= 20)
        smash->tier = 1;
    else
        smash->tier = 0;
    if (func_ov003_02085cfc(6))
        return;
    SndMgr_StopPlayingSE(0xB5);
    func_ov003_0209557c(smash->effect, smash->tier);
    func_ov003_02085c78();
    func_ov003_02087250(smash->effect, data_ov003_020e71b8->unk3D898, smash->tier);
    smash->active = 0;
}
void func_ov003_020956c4(BtlPsySmash* smash) {
    if (smash->timer == 0) {
        func_ov003_02087f28(0xB6, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x);
        smash->timer = 21;
    }
    if (smash->timer > 0)
        smash->timer--;
    if (smash->timer > 0 && func_ov003_02085cfc(6))
        return;
    func_ov003_0209557c(smash->effect, smash->tier);
    func_ov003_02085c78();
    func_ov003_02087250(smash->effect, data_ov003_020e71b8->unk3D898, smash->tier);
    smash->active = 0;
}
s32 func_ov003_02095764(TaskPool* pool, Task* task, void* args) {
    BtlPsySmash*       smash = task->data;
    Ov003EffectState** arg   = args;
    MI_CpuSet(smash, 0, sizeof(*smash));
    data_ov003_020e71b8->unk3D8C8 = 1;
    smash->effect                 = *arg;
    smash->tier                   = 0;
    smash->particleTimer          = 0;
    smash->active                 = 1;
    func_ov003_020953b4(smash, func_ov003_02095608);
    func_ov003_02087f28(0xB5, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x);
    return 1;
}
s32 func_ov003_020957e4(TaskPool* pool, Task* task, void* args) {
    BtlPsySmash* smash = task->data;
    if (func_ov003_020a63a8(smash->effect))
        return 0;
    if (smash->update != NULL)
        smash->update(smash);
    return smash->active;
}
s32 func_ov003_0209581c(TaskPool* pool, Task* task, void* args) {
    return 1;
}
s32 func_ov003_02095824(TaskPool* pool, Task* task, void* args) {
    data_ov003_020e71b8->unk3D8C8 = 0;
    return 1;
}

s32              func_ov003_02095844(TaskPool*, Task*, void*, s32);
char             data_ov003_020e3bb8[28] = "Tsk_BtlPsy_Sword_Smash_D";
const TaskHandle data_ov003_020d7e10     = {data_ov003_020e3bb8, func_ov003_02095844, sizeof(BtlPsySmash)};
const TaskStages data_ov003_020d7e1c = {func_ov003_02095764, func_ov003_020957e4, func_ov003_0209581c, func_ov003_02095824};
s32              func_ov003_02095844(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7e1c;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_0209588c(Ov003EffectContext* context, Ov003EffectState* effect) {
    if (!func_ov003_020953c8())
        return 0;
    Ov003EffectState** arg = func_ov003_020a5d2c(sizeof(*arg), data_ov003_020d7e10.taskName);
    *arg                   = effect;
    context->handle        = (TaskHandle*)&data_ov003_020d7e10;
    context->arg           = arg;
    context->post          = NULL;
    return 1;
}
