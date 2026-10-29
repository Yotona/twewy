#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEffects.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

extern const BinIdentifier data_ov003_020d77f8;

extern s32 func_ov003_020843b0(s32 engine, s32 x);
extern s32 func_ov003_020843ec(s32 engine, s32 y, s32 z);

/// Task data of the warp effect (`Tsk_BtlEff_Warp`): a pair of sprites, one left behind at the
/// previous position of the display actor and one at its new position.
typedef struct BtlEff_Warp {
    /* 0x00 */ CombatEffect halves[2];
} BtlEff_Warp; // Size: 0xE0

/// Parameters handed to the warp task by `func_ov003_0209f770`.
typedef struct BtlEff_WarpArgs {
    /* 0x00 */ volatile s32 mode; // display engine: 1 = sub screen, 0 = main screen
    /* 0x04 */ s32          x;    // previous world position of the display actor
    /* 0x08 */ s32          y;
    /* 0x0C */ s32          z;
} BtlEff_WarpArgs; // Size: 0x10

char data_ov003_020e3dc4[0x10] = "Tsk_BtlEff_Warp";

const TaskHandle data_ov003_020d86c4 = {data_ov003_020e3dc4, func_ov003_0209f708, sizeof(BtlEff_Warp)};

const TaskStages data_ov003_020d86d0 = {
    func_ov003_0209f5e4,
    func_ov003_0209f694,
    func_ov003_0209f6c8,
    func_ov003_0209f6e8,
};

const SpriteAnimEntry data_ov003_020d86e0[2] = {
    {0x35, 0x37, 0x36, 0},
    {0x35, 0x37, 0x36, 1},
};

/// Loads one half of the warp effect: animation `animIndex` of the warp sprite, flipped to match
/// the display actor it belongs to, and marks that half as playing.
s32 func_ov003_0209f4a0(s32 mode, CombatEffect* sprite, u16 animIndex, s32 flip) {
    CombatSprite_LoadFromTable(mode, &sprite->sprite, &data_ov003_020d77f8, data_ov003_020d86e0, animIndex, 1, 0x30);
    CombatSprite_SetAnimFromTable(&sprite->sprite, animIndex, 1);
    CombatSprite_SetFlip(&sprite->sprite, flip);
    sprite->active = 1;
    return 1;
}

/// Advances one half of the warp effect and stops it once its animation is over.
void func_ov003_0209f508(CombatEffect* sprite) {
    if (SpriteMgr_IsAnimationFinished(&sprite->sprite.sprite)) {
        sprite->active = 0;
    }
    CombatSprite_Update(&sprite->sprite);
}

/// Draws one half of the warp effect at its world position, depth-sorted by Y.
void func_ov003_0209f52c(CombatEffect* sprite) {
    s32 posX;
    s32 posY;

    if (sprite->active == 0) {
        return;
    }
    if (sprite->sprite.sprite.bits_0_1 == 1) {
        posX = func_ov003_020843b0(1, sprite->position.x);
        posY = func_ov003_020843ec(1, sprite->position.y, sprite->position.z);
    } else {
        posX = func_ov003_020843b0(0, sprite->position.x);
        posY = func_ov003_020843ec(0, sprite->position.y, sprite->position.z);
    }
    CombatSprite_SetPosition(&sprite->sprite, posX, posY);
    func_ov003_02082730(&sprite->sprite, 0x7FFFFFFF - (sprite->position.y + 0x8000));
    CombatSprite_Render(&sprite->sprite);
}

void func_ov003_0209f5c8(CombatSprite* sprite) {
    CombatSprite_Release(sprite);
}

/// Moves one half of the warp effect to a world position.
void func_ov003_0209f5d4(CombatEffect* sprite, s32 x, s32 y, s32 z) {
    sprite->position.x = x;
    sprite->position.y = y;
    sprite->position.z = z;
}

/// Builds the previous-position half and current-position half, both flipped like the actor.
s32 func_ov003_0209f5e4(TaskPool* pool, Task* task, void* args) {
    BtlEff_Warp*     eff = task->data;
    BtlEff_WarpArgs* p   = args;
    CombatActor*     actor;
    s32              flip;

    MI_CpuSet(eff, 0, sizeof(BtlEff_Warp));
    if (p->mode == 1) {
        actor = data_ov003_020e71b8->unk3D89C;
    } else {
        actor = data_ov003_020e71b8->unk3D898;
    }
    flip = actor->isFlipped != 0;
    // Volatile mode preserves the retail reload before each load call.
    func_ov003_0209f4a0(p->mode, &eff->halves[0], 0, flip);
    func_ov003_0209f4a0(p->mode, &eff->halves[1], 1, flip);
    func_ov003_0209f5d4(&eff->halves[0], p->x, p->y, p->z);
    func_ov003_0209f5d4(&eff->halves[1], actor->position.x, actor->position.y, actor->position.z);
    return 1;
}

/// Task update: advances both halves, keeping the task alive while either is still playing.
s32 func_ov003_0209f694(TaskPool* pool, Task* task, void* args) {
    BtlEff_Warp* eff = task->data;

    func_ov003_0209f508(&eff->halves[0]);
    func_ov003_0209f508(&eff->halves[1]);
    return eff->halves[0].active != 0 || eff->halves[1].active != 0;
}

/// Task render: draws both halves.
s32 func_ov003_0209f6c8(TaskPool* pool, Task* task, void* args) {
    BtlEff_Warp* eff = task->data;

    func_ov003_0209f52c(&eff->halves[0]);
    func_ov003_0209f52c(&eff->halves[1]);
    return 1;
}

/// Task destroy: releases both halves.
s32 func_ov003_0209f6e8(TaskPool* pool, Task* task, void* args) {
    BtlEff_Warp* eff = task->data;

    func_ov003_0209f5c8(&eff->halves[0].sprite);
    func_ov003_0209f5c8(&eff->halves[1].sprite);
    return 1;
}

/// Task dispatcher: calls the stage function for `stage` unless the encounter cutscene is running.
s32 func_ov003_0209f708(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d86d0;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns the warp effect that trails the display actor from (`x`, `y`, `z`) to its new position.
void func_ov003_0209f770(s32 mode, s32 x, s32 y, s32 z) {
    BtlEff_WarpArgs args;

    args.mode = mode;
    args.x    = x;
    args.y    = y;
    args.z    = z;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d86c4, NULL, 0, NULL, &args);
}
