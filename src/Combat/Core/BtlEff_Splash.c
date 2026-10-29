#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Math/Random.h"
#include <nitro/fx.h>

// The task reserves sixteen 0x74-byte particles.
typedef struct BtlSplashParticle {
    CombatSprite sprite;
    s32          angle, speed;
    Vec          position;
} BtlSplashParticle;
typedef struct BtlEffSplash {
    s32               engine;
    BtlSplashParticle particles[16];
    s16               count;
    u16               pad746;
    Vec               position;
    s16               timer;
    u16               pad756;
    s32               independent;
    s32               active;
} BtlEffSplash;
typedef struct BtlEffSplashArgs {
    s32 engine;
    Vec position;
    s16 count;
    u16 pad;
    s32 independent;
} BtlEffSplashArgs;
extern const BinIdentifier data_ov003_020d77d8, data_ov003_020d77e0, data_ov003_020d77e8, data_ov003_020d77f0;
extern s32                 func_ov003_020843b0(s32, s32);
extern s32                 func_ov003_020843ec(s32, s32, s32);
extern void                func_ov003_0208b2fc(s32*, s32*, u16, s32);
void                       func_ov003_0209f8b0(BtlEffSplash*);
void                       func_ov003_0209f9c4(BtlEffSplash*);
void                       func_ov003_0209fa14(BtlEffSplash*);
void                       func_ov003_0209fae0(BtlEffSplash*);
char                       data_ov003_020e3dd4[20] = "Tsk_BtlEff_Splash";
const TaskHandle           data_ov003_020d86f0     = {data_ov003_020e3dd4, func_ov003_0209fc40, 0x760};
const TaskStages data_ov003_020d86fc = {func_ov003_0209fb1c, func_ov003_0209fb8c, func_ov003_0209fbec, func_ov003_0209fc2c};
void             func_ov003_0209f7c4(CombatSprite* sprite, u16 anim) {
    switch (data_ov003_020e71b8->unk3D88D) {
        case 1:
            CombatSprite_LoadDirect(1, sprite, &data_ov003_020d77e8, 0x28, 0x2a, 0x29, 0x2c, anim);
            break;
        case 2:
            CombatSprite_LoadDirect(1, sprite, &data_ov003_020d77f0, 0x2c, 0x2e, 0x2d, 0x2f, anim);
            break;
        case 0:
        default:
            CombatSprite_LoadDirect(1, sprite, &data_ov003_020d77e0, 0x23, 0x25, 0x24, 0x26, anim);
            break;
    }
}
void func_ov003_0209f8b0(BtlEffSplash* data) {
    s32                angle = RNG_Next(0x100) << 8;
    u16                anim;
    s32                i;
    BtlSplashParticle* particle = data->particles;
    for (i = 0; i < data->count; i++) {
        particle             = &data->particles[i];
        anim                 = RNG_Next(5);
        particle->position.x = data->position.x;
        particle->position.y = data->position.y;
        particle->position.z = data->position.z;
        particle->angle      = angle + (RNG_Next(0x100) << 8) / (data->count * 2);
        particle->speed      = (RNG_Next(13) + 6) << 12;
        if (data->engine == 1)
            func_ov003_0209f7c4(&particle->sprite, anim);
        else
            CombatSprite_LoadDirect(2, &particle->sprite, &data_ov003_020d77d8, 0x56, 0x58, 0x57, 0x59, anim);
        CombatSprite_SetAnim(&particle->sprite, anim, 1);
        angle += 0x10000 / data->count;
    }
}
// Nonmatching: use the callee's declared u16 angle rather than an incompatible call signature.
void func_ov003_0209f9c4(BtlEffSplash* data) {
    s32                i;
    BtlSplashParticle* particle;
    for (i = 0; i < data->count; i++) {
        particle = &data->particles[i];
        func_ov003_0208b2fc(&particle->position.x, &particle->position.y, particle->angle, particle->speed);
        CombatSprite_Update(&particle->sprite);
    }
}
void func_ov003_0209fa14(BtlEffSplash* data) {
    s32                x, y, key;
    s32                i;
    BtlSplashParticle* particle;
    for (i = 0; i < data->count; i++) {
        particle = &data->particles[i];

        if (particle->sprite.sprite.bits_0_1 == 1) {
            x   = func_ov003_020843b0(1, particle->position.x);
            y   = func_ov003_020843ec(1, particle->position.y, particle->position.z);
            key = 0x7fffffff - (particle->position.y + 0x8000);
        } else {
            x   = func_ov003_020843b0(0, particle->position.x);
            y   = func_ov003_020843ec(0, particle->position.y, particle->position.z);
            key = y + 8;
        }
        CombatSprite_SetPosition(&particle->sprite, x, y);
        func_ov003_02082730(&particle->sprite, key);
        CombatSprite_Render(&particle->sprite);
    }
}
void func_ov003_0209fae0(BtlEffSplash* data) {
    s32                i;
    BtlSplashParticle* particle;
    for (i = 0; i < data->count; i++) {
        particle = &data->particles[i];
        CombatSprite_Release(&particle->sprite);
    }
}
s32 func_ov003_0209fb1c(TaskPool* pool, Task* task, void* args) {
    BtlEffSplash*     data = task->data;
    BtlEffSplashArgs* p    = args;
    MI_CpuSet(data, 0, sizeof(*data));
    data->independent = p->independent;
    data->active      = 1;
    data->timer       = 20;
    data->engine      = p->engine;
    data->position.x  = p->position.x;
    data->position.y  = p->position.y;
    data->position.z  = p->position.z;
    data->count       = p->count;
    func_ov003_0209f8b0(data);
    return 1;
}
s32 func_ov003_0209fb8c(TaskPool* pool, Task* task, void* args) {
    BtlEffSplash* data = task->data;
    if (!data->independent && (data_ov003_020e71b8->unk3D878 & 6))
        return 1;
    if (data->timer > 0)
        data->timer--;
    else
        data->active = 0;
    func_ov003_0209f9c4(data);
    return data->active;
}
s32 func_ov003_0209fbec(TaskPool* pool, Task* task, void* args) {
    BtlEffSplash* data = task->data;
    if (!data->independent && (data_ov003_020e71b8->unk3D878 & 6))
        return 1;
    func_ov003_0209fa14(data);
    return 1;
}
s32 func_ov003_0209fc2c(TaskPool* pool, Task* task, void* args) {
    func_ov003_0209fae0(task->data);
    return 1;
}
s32 func_ov003_0209fc40(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d86fc;
    return stages.iter[stage](pool, task, args);
}
void func_ov003_0209fc88(s32 engine, s32 x, s32 y, s32 z, s16 count, s32 independent) {
    BtlEffSplashArgs p;
    p.engine      = engine;
    p.position.x  = x;
    p.position.y  = y;
    p.position.z  = z;
    p.count       = count;
    p.independent = independent;
    if (independent)
        EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &data_ov003_020d86f0, NULL, 0, NULL, &p);
    else {
        if (!(data_ov003_020e71b8->unk3D878 & 0x80))
            return;
        data_ov003_020e71b8->unk3D878 &= ~0x80;
        EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d86f0, NULL, 0, NULL, &p);
    }
}
