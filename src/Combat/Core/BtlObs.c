#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEffectActor.h"
#include "Engine/Math/Random.h"

typedef struct BtlObs {
    /* 0x000 */ Ov003EffectActor effect;
    /* 0x0C4 */ CombatSprite     sprites[2];
    /* 0x184 */ void (*update)(struct BtlObs*);
    /* 0x188 */ s16 state;
    /* 0x18A */ s16 timer;
    /* 0x18C */ s32 visible;
    /* 0x190 */ s32 active;
} BtlObs; // Size: 0x194

extern void func_ov003_02086680(Ov003EffectActor*, Ov003EffectSpawn*);
extern void func_ov003_020866f8(Ov003EffectActor*);
extern void func_ov003_02086770(Ov003EffectActor*);
extern void func_ov003_0208810c(CombatSprite*, CombatActor*);
extern s32  func_ov003_0208a25c(CombatActor*);
extern s32  func_ov003_020a3330(s32);
extern s32  func_ov003_020a3468(s32);

void func_ov003_020941ac(BtlObs* obs, void (*update)(BtlObs*)) {
    obs->update = update;
    obs->state  = 0;
    obs->timer  = 0;
    obs->effect.actor.flags &= ~0x200;
    obs->effect.actor.flags &= ~0x400;
}

s32 func_ov003_020941d4(CombatActor* actor) {
    if (actor->xVelocity != 0 || actor->yVelocity != 0)
        return 0;
    return actor->position.z >= 0;
}

void func_ov003_02094200(BtlObs* obs) {
    if (obs->state == 0) {
        if (obs->effect.sprite.flags & 1)
            CombatSprite_SetAnimFromTable(&obs->sprites[0], 2, 0);
        else
            CombatSprite_SetAnimFromTable(&obs->sprites[0], 0, 0);
        obs->state++;
    }
    if (obs->effect.sprite.collider.hitMask != 0) {
        obs->effect.actor.position.x += obs->effect.sprite.collider.pushX >> 5;
        obs->effect.actor.position.y += obs->effect.sprite.collider.pushY >> 5;
    }
}

void func_ov003_02094288(BtlObs* obs) {
    obs->active = 0;
}

void func_ov003_02094294(BtlObs* obs) {
    if (obs->state != 0)
        return;
    obs->effect.actor.flags |= 0x200;
    func_ov003_02084694(&obs->effect.sprite.collider, 1);
    CombatSprite_SetAnimFromTable(&obs->sprites[0], 1, 0);
    obs->state++;
}

void func_ov003_020942e8(BtlObs* obs) {
    if (obs->state == 0) {
        obs->effect.actor.flags |= 0x400;
        if (obs->effect.id == 0)
            CombatSprite_SetAnimFromTable(&obs->sprites[0], 2, 0);
        else
            CombatSprite_SetAnimFromTable(&obs->sprites[0], 1, 0);
        obs->state++;
    }
    if (func_ov003_020941d4(&obs->effect.actor)) {
        func_ov003_02086540(0, obs->effect.config, obs->effect.actor.position.x);
        obs->effect.actor.flags &= ~0x400;
        func_ov003_02084694(&obs->effect.sprite.collider, 0);
        if (obs->effect.id == 2)
            obs->effect.sprite.flags |= 1;
        func_ov003_020941ac(obs, func_ov003_02094200);
    }
    func_ov003_0208a25c(&obs->effect.actor);
}

// Nonmatching: mwcc predicates the final three stores instead of returning before them.
void func_ov003_020943ac(BtlObs* obs) {
    if (obs->timer == 0)
        obs->effect.actor.flags |= 0x10;
    if (obs->timer < 20) {
        obs->visible = obs->visible == 0;
    } else {
        if (obs->timer % 3 == 0)
            obs->visible = 1;
        else
            obs->visible = 0;
    }
    if (obs->timer <= 60) {
        obs->timer++;
        return;
    }
    obs->visible = 0;
    obs->active  = 0;
}

s32 func_ov003_02094444(TaskPool* pool, Task* task, void* args) {
    BtlObs*           obs   = task->data;
    Ov003EffectSpawn* spawn = args;
    func_ov003_02086680(&obs->effect, spawn);
    func_ov003_02086404(0, &obs->sprites[0], spawn->id);
    func_ov003_020864c0(0, &obs->sprites[1], spawn->id);
    obs->visible = 1;
    obs->active  = 1;
    func_ov003_020941ac(obs, func_ov003_02094200);
    if (func_ov003_020a3330(13))
        obs->effect.actor.currentHp *= (s16)func_ov003_020a3468(13);
    CombatSprite_SetAnimFromTable(&obs->sprites[0], 0, 0);
    if (RNG_Next(2))
        CombatSprite_SetFlip(&obs->sprites[0], 1);
    return 1;
}

s32 func_ov003_020944f0(TaskPool* pool, Task* task, void* args) {
    BtlObs* obs = task->data;
    switch (CombatActor_PopPendingCommand(&obs->effect.actor)) {
        case 1:
            func_ov003_020941ac(obs, func_ov003_020943ac);
            break;
        case 3:
            func_ov003_020941ac(obs, func_ov003_02094288);
            break;
        case 4:
            func_ov003_020941ac(obs, func_ov003_02094294);
            break;
        case 5:
            func_ov003_020941ac(obs, func_ov003_020942e8);
            break;
    }
    if (obs->update != NULL)
        obs->update(obs);
    func_ov003_020866f8(&obs->effect);
    CombatSprite_Update(&obs->sprites[0]);
    CombatSprite_Update(&obs->sprites[1]);
    return obs->active;
}

s32 func_ov003_020945a4(TaskPool* pool, Task* task, void* args) {
    BtlObs* obs = task->data;
    if (obs->visible) {
        CombatActor_Render(&obs->effect.actor, &obs->sprites[0]);
        func_ov003_0208810c(&obs->sprites[1], &obs->effect.actor);
    }
    return 1;
}

s32 func_ov003_020945d8(TaskPool* pool, Task* task, void* args) {
    BtlObs* obs = task->data;
    CombatSprite_Release(&obs->sprites[0]);
    CombatSprite_Release(&obs->sprites[1]);
    func_ov003_02086770(&obs->effect);
    return 1;
}

extern const TaskStages data_ov003_020d7d14;
s32                     func_ov003_02094600(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7d14;
    if (func_ov003_0208b690((u16)stage))
        return 1;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e3b4c[12] = "Tsk_BtlObs";
const TaskHandle data_ov003_020d7d08     = {data_ov003_020e3b4c, func_ov003_02094600, sizeof(BtlObs)};
const TaskStages data_ov003_020d7d14 = {func_ov003_02094444, func_ov003_020944f0, func_ov003_020945a4, func_ov003_020945d8};
