#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

extern BinIdentifier data_ov003_020d7800;

extern void func_020265d4(void* value, s32 target, u16 frames);
extern s32  func_ov003_020843b0(s32 engine, s32 x);
extern s32  func_ov003_020843ec(s32 engine, s32 y, s32 z);

/// Task data of the damage-rate popup (`Tsk_BtlPop_Rate`): one sprite per rate digit,
/// the tens digit only being shown when the rate has a non-zero tens digit.
typedef struct BtlPop_Rate {
    /* 0x00 */ CombatSprite sprite;     // hundreds digit
    /* 0x60 */ CombatSprite tensSprite; // tens digit
    /* 0xC0 */ s32          unkC0;      // x offset (half-pixels) applied to the tens digit
    /* 0xC4 */ s32          showTens;   // tens digit sprite is visible
    /* 0xC8 */ s32          x;          // world position the popup tracks
    /* 0xCC */ s32          y;
    /* 0xD0 */ s32          z;          // display Z, eased towards `targetZ`
    /* 0xD4 */ s32          targetZ;
    /* 0xD8 */ s16          timer;      // Z-slide timer; the popup ends below -0x1E
    /* 0xDA */ s16          rate;       // rate displayed by this popup
    /* 0xDC */ s32          active;
} BtlPop_Rate;                          // Size: 0xE0

/// Parameters handed to the popup task by `func_ov003_0209c568`.
typedef struct BtlPop_RateArgs {
    /* 0x00 */ s32 engine; // screen selector, also the sprite loader flag
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s16 rate;
} BtlPop_RateArgs; // Size: 0x14

/// The rate popup picks its animation index conditionally and the retail call site passes it
/// through without narrowing it to `CombatSprite_LoadFromTable`'s `u16` parameter, so call it
/// through a plain-integer signature (same idiom as `CombatSprite_LoadDirectNoProto`).
#define CombatSprite_LoadFromTableNoProto                                                                                   \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, const SpriteAnimEntry*, s32, s32, s32))CombatSprite_LoadFromTable)

s32  func_ov003_0209c270(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209c378(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209c3dc(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209c498(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209c500(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov003_0209c1f0(BtlPop_Rate* pop, s32 rate);

char data_ov003_020e3cb4[0x10] = "Tsk_BtlPop_Rate";

const TaskHandle data_ov003_020d8034 = {data_ov003_020e3cb4, func_ov003_0209c500, 0xE0};

const TaskStages data_ov003_020d8040 = {
    func_ov003_0209c270,
    func_ov003_0209c378,
    func_ov003_0209c3dc,
    func_ov003_0209c498,
};

/// Animation table of the rate digits: every entry shares the same char, palette and
/// frame indices and only varies the animation index.
const SpriteAnimEntry data_ov003_020d8050[14] = {
    {0x0A, 0x0C, 0x0B, 0x0},
    {0x0A, 0x0C, 0x0B, 0x1},
    {0x0A, 0x0C, 0x0B, 0x2},
    {0x0A, 0x0C, 0x0B, 0x3},
    {0x0A, 0x0C, 0x0B, 0x4},
    {0x0A, 0x0C, 0x0B, 0x5},
    {0x0A, 0x0C, 0x0B, 0x6},
    {0x0A, 0x0C, 0x0B, 0x7},
    {0x0A, 0x0C, 0x0B, 0x8},
    {0x0A, 0x0C, 0x0B, 0x9},
    {0x0A, 0x0C, 0x0B, 0xA},
    {0x0A, 0x0C, 0x0B, 0xB},
    {0x0A, 0x0C, 0x0B, 0xC},
    {0x0A, 0x0C, 0x0B, 0xD},
};

/// Splits a rate into its hundreds and tens digit.
// Nonmatching: instruction sequence is right; mwcc picks r5/r3 for the quotient and the final
// digit instead of the retail r3/r4 pair (pure register allocation, no structural difference).
void func_ov003_0209c1a4(s32* digits, s32 rate) {
    digits[1] = rate / 100;
    digits[0] = rate % 100 / 10;
}

/// Selects the hundreds/tens digit animations and stores the horizontal offset between
/// the two digit sprites.
void func_ov003_0209c1f0(BtlPop_Rate* pop, s32 rate) {
    s32 digits[2];

    func_ov003_0209c1a4(digits, rate);
    pop->unkC0    = digits[1] - 2;
    pop->showTens = digits[0] != 0;
    CombatSprite_SetAnimFromTable(&pop->sprite, (s16)pop->unkC0, 0);
    CombatSprite_SetAnimFromTable(&pop->tensSprite, (s16)(digits[0] + 4), 0);
}

// Nonmatching: the retail first `CombatSprite_LoadFromTable` materialises its stack arguments in
// r0 and reloads `p->engine`, while this build keeps the engine in r0 and uses r1/r12 for the
// constants (argument-setup scheduling only; the call sequence itself is identical).
s32 func_ov003_0209c270(TaskPool* pool, Task* task, void* args) {
    BtlPop_Rate*     pop = task->data;
    BtlPop_RateArgs* p   = args;
    s32              animIndex;
    s32              z;

    MI_CpuSet(pop, 0, sizeof(BtlPop_Rate));
    pop->rate     = p->rate;
    pop->x        = p->x;
    pop->y        = p->y;
    z             = p->z;
    pop->z        = z;
    pop->targetZ  = z;
    pop->unkC0    = 2;
    pop->showTens = 0;
    pop->timer    = 0x14;
    pop->active   = 1;

    if (p->engine == 0) {
        animIndex = 0x3D;
    } else {
        animIndex = 0x3F;
    }

    CombatSprite_LoadFromTableNoProto(p->engine, &pop->sprite, &data_ov003_020d7800, data_ov003_020d8050, 0, animIndex, 0x24);
    func_ov003_02082738(&pop->sprite, 0);
    func_ov003_02082730(&pop->sprite, 1);
    CombatSprite_LoadFromTableNoProto(p->engine, &pop->tensSprite, &data_ov003_020d7800, data_ov003_020d8050, 4, animIndex, 4);
    func_ov003_02082738(&pop->tensSprite, 0);
    func_ov003_02082730(&pop->tensSprite, 1);
    func_ov003_0209c1f0(pop, p->rate);
    return 1;
}

s32 func_ov003_0209c378(TaskPool* pool, Task* task, void* args) {
    BtlPop_Rate* pop = task->data;

    if (pop->timer > 0) {
        func_020265d4(&pop->z, pop->targetZ - 0x16000, (u16)pop->timer);
    }
    pop->timer--;
    if (pop->timer < -0x1E) {
        pop->active = 0;
    }
    CombatSprite_Update(&pop->sprite);
    CombatSprite_Update(&pop->tensSprite);
    return pop->active;
}

s32 func_ov003_0209c3dc(TaskPool* pool, Task* task, void* args) {
    BtlPop_Rate* pop = task->data;
    s32          posX;
    s32          posY;

    if (pop->sprite.sprite.bits_0_1 == 0) {
        posX = func_ov003_020843b0(0, pop->x);
        posY = func_ov003_020843ec(0, pop->y, pop->z);
    } else {
        posX = func_ov003_020843b0(1, pop->x);
        posY = func_ov003_020843ec(1, pop->y, pop->z);
    }
    if (posX < 0x18) {
        posX = 0x18;
    }
    if (posX > 0xE8) {
        posX = 0xE8;
    }
    CombatSprite_SetPosition(&pop->sprite, posX, posY);
    CombatSprite_Render(&pop->sprite);
    if (pop->showTens) {
        CombatSprite_SetPosition(&pop->tensSprite, (s16)(posX + pop->unkC0 * 2), posY);
        CombatSprite_Render(&pop->tensSprite);
    }
    return 1;
}

s32 func_ov003_0209c498(TaskPool* pool, Task* task, void* args) {
    BtlPop_Rate* pop = task->data;

    if (pop->sprite.sprite.bits_0_1 == 0) {
        if (pop->rate == (s16)data_ov003_020e71b8->unk3D900) {
            data_ov003_020e71b8->unk3D900 = 0;
        }
    } else {
        if (pop->rate == (s16)data_ov003_020e71b8->unk3D902) {
            data_ov003_020e71b8->unk3D902 = 0;
        }
    }
    CombatSprite_Release(&pop->sprite);
    CombatSprite_Release(&pop->tensSprite);
    return 1;
}

s32 func_ov003_0209c500(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8040;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns a rate popup on the given screen when the current rate changed and is worth
/// showing (200% and up), anchored to that screen's level-up gauge actor.
void func_ov003_0209c568(s32 engine) {
    BtlPop_RateArgs args;
    s16             rate  = data_ov003_020e71b8->unk3D8EE;
    s32             spawn = 0;

    if (rate < 0xC8) {
        return;
    }

    if (engine == 0) {
        if ((s16)data_ov003_020e71b8->unk3D900 != rate) {
            spawn                         = 1;
            args.x                        = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x;
            args.y                        = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
            args.z                        = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.z - 0x20000;
            data_ov003_020e71b8->unk3D900 = data_ov003_020e71b8->unk3D8EE;
        }
    } else if ((s16)data_ov003_020e71b8->unk3D902 != rate) {
        spawn                         = 1;
        args.x                        = ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x;
        args.y                        = ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.y;
        args.z                        = ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.z - 0x20000;
        data_ov003_020e71b8->unk3D902 = data_ov003_020e71b8->unk3D8EE;
    }

    if (!spawn) {
        return;
    }
    args.engine = engine;
    args.rate   = data_ov003_020e71b8->unk3D8EE;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d8034, NULL, 0, NULL, &args);
}
