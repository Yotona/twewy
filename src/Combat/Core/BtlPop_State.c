#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"

// The battle status popup ("SD"/"ATK" style labels): a sprite that follows one battle unit and
// swaps its animation whenever the unit's periodic effect mode changes.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 8-byte payload `func_ov003_0209ccc0` hands to the task: which engine draws it and the unit
/// it follows.
typedef struct BtlPop_StateArgs {
    /* 0x00 */ s32          engine; // display engine index, also the sprite's `bits_0_1`
    /* 0x04 */ CombatActor* actor;
} BtlPop_StateArgs;                 // Size: 0x8

/// Task data of the status popup.
typedef struct BtlPop_State {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ CombatActor* actor;
    /* 0x64 */ u16          effectMode; // actor->periodicEffectMode the sprite currently shows
    /* 0x66 */ u16          pad_66;
    /* 0x68 */ s32          active;
} BtlPop_State; // Size: 0x6C

// ---------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------

void func_ov003_0209ca18(BtlPop_State* data);
s32  func_ov003_0209ca90(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209cb24(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209ccc0(s32 engine, CombatActor* actor);

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3cd8[0x14] = "Tsk_BtlPop_State";

const TaskHandle data_ov003_020d80dc = {data_ov003_020e3cd8, func_ov003_0209cc58, 0x6C};

const TaskStages data_ov003_020d80e8 = {
    .initialize = func_ov003_0209ca90,
    .update     = func_ov003_0209cb24,
    .render     = func_ov003_0209cb70,
    .cleanup    = func_ov003_0209cc44,
};

/// One entry per periodic effect mode: the label sprites of the status icon.
const SpriteAnimEntry data_ov003_020d80f8[5] = {
    {0x3A, 0x3C, 0x3B, 0},
    {0x3A, 0x3C, 0x3B, 1},
    {0x3A, 0x3C, 0x3B, 2},
    {0x3A, 0x3C, 0x3B, 3},
    {0x3A, 0x3C, 0x3B, 4},
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

extern BinIdentifier data_ov003_020d7800;

/// The status popup picks its sprite index conditionally and the retail call site passes it
/// through without narrowing it to `CombatSprite_LoadFromTable`'s `u16` parameter, so call it
/// through a plain-integer signature (same idiom as `CombatSprite_LoadDirectNoProto`).
#define CombatSprite_LoadFromTableNoProto                                                                                   \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, const SpriteAnimEntry*, s32, s32, s32))CombatSprite_LoadFromTable)

/// Picks the label sprite for the unit's current periodic effect mode. Mode 0 (no effect) hides
/// the popup by clearing `active`; modes 1..4 map onto the four sprite table entries.
void func_ov003_0209ca18(BtlPop_State* data) {
    if (data->effectMode == 0) {
        data->active = 0;
        return;
    }
    switch (data->effectMode) {
        case 1:
            CombatSprite_SetAnimFromTable(&data->sprite, 1, 0);
            break;
        case 2:
            CombatSprite_SetAnimFromTable(&data->sprite, 2, 0);
            break;
        case 3:
            CombatSprite_SetAnimFromTable(&data->sprite, 3, 0);
            break;
        case 4:
            CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
            break;
    }
}

// Nonmatching: the two stack arguments of the LoadFromTable call are evaluated in the opposite
// order, so mwcc materialises them in r1/r12 instead of the consecutive r0/r1 that let the
// target merge the pair into one `stmia`. Code size and every operand are identical.
s32 func_ov003_0209ca90(TaskPool* pool, Task* task, void* args) {
    BtlPop_State*     data    = task->data;
    BtlPop_StateArgs* popArgs = args;

    MI_CpuSet(data, 0, sizeof(BtlPop_State));
    data->actor      = popArgs->actor;
    data->effectMode = data->actor->periodicEffectMode;
    data->active     = 1;
    CombatSprite_LoadFromTableNoProto(popArgs->engine, &data->sprite, &data_ov003_020d7800, data_ov003_020d80f8, 0,
                                      popArgs->engine == 0 ? 0x3D : 0x3F, 4);
    func_ov003_02082738(&data->sprite, 0);
    func_ov003_0209ca18(data);
    return 1;
}

s32 func_ov003_0209cb24(TaskPool* pool, Task* task, void* args) {
    BtlPop_State* data = task->data;

    if (data->actor->flags & 4) {
        data->active = 0;
    }
    if (data->effectMode != data->actor->periodicEffectMode) {
        data->effectMode = data->actor->periodicEffectMode;
        func_ov003_0209ca18(data);
    }
    CombatSprite_Update(&data->sprite);
    return data->active;
}

s32 func_ov003_0209cb70(TaskPool* pool, Task* task, void* args) {
    BtlPop_State* data  = task->data;
    CombatActor*  actor = data->actor;
    s32           posX;
    s32           posY;

    if (data->sprite.sprite.bits_0_1 == 0) {
        posX = func_ov003_020843b0(0, actor->position.x + (actor->unk_74 << 12));
        posY = func_ov003_020843ec(0, actor->position.y, actor->position.z);
        if (data->actor->unk_00 == 0) {
            posY = (s16)(posY - 0x30);
        } else {
            posY = (s16)(posY - data->actor->unk_72);
        }
    } else {
        posX = func_ov003_020843b0(1, actor->position.x + (actor->unk_74 << 12));
        posY = func_ov003_020843ec(1, actor->position.y, actor->position.z);
        posY = (s16)(posY - data->actor->unk_72);
    }
    CombatSprite_SetPosition(&data->sprite, posX, posY);
    func_ov003_02082730(&data->sprite, 10);
    CombatSprite_Render(&data->sprite);
    return 1;
}

s32 func_ov003_0209cc44(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

s32 func_ov003_0209cc58(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d80e8;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns the status popup on `engine` following `actor`.
s32 func_ov003_0209ccc0(s32 engine, CombatActor* actor) {
    BtlPop_StateArgs args;

    args.engine = engine;
    args.actor  = actor;
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d80dc, NULL, 0, NULL, &args);
}
