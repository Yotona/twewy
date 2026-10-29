#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"

#include "Combat/Core/CombatApi.h"
typedef struct BtlTarget {
    /* 0x00 */ CombatSprite sprites[2];
    /* 0xC0 */ CombatActor* target;
    /* 0xC4 */ void (*update)(struct BtlTarget*);
    /* 0xC8 */ s32 active;
} BtlTarget; // Size: 0xCC

extern s32                   func_ov003_020843b0(s32 engine, s32 x);
extern s32                   func_ov003_020843ec(s32 engine, s32 y, s32 z);
extern const BinIdentifier   data_ov003_020d7800;
extern const SpriteAnimEntry data_ov003_020d7fec[2];

void func_ov003_0209abdc(BtlTarget* target, void (*update)(BtlTarget*)) {
    target->update = update;
}

void func_ov003_0209abe4(CombatSprite* sprite, CombatActor* actor) {
    s32 x;
    s32 y;
    if (actor->flags & 0x20)
        return;
    s32 positionX = actor->position.x + (actor->unk_74 << 12);
    if (sprite->sprite.bits_0_1 == 0) {
        x = func_ov003_020843b0(0, positionX);
        y = func_ov003_020843ec(0, actor->position.y, actor->position.z - (actor->unk_76 << 12));
    } else {
        x = func_ov003_020843b0(1, positionX);
        y = func_ov003_020843ec(1, actor->position.y, actor->position.z - (actor->unk_76 << 12));
    }
    CombatSprite_SetPosition(sprite, x, y);
    func_ov003_02082730(sprite, 10);
    CombatSprite_Render(sprite);
}

void func_ov003_0209ac8c(BtlTarget* target) {
    if (!SpriteMgr_IsAnimationFinished(&target->sprites[0].sprite))
        return;
    CombatSprite_SetAnimFromTable(&target->sprites[0], 0, 0);
    CombatSprite_SetAnimFromTable(&target->sprites[1], 0, 0);
    func_ov003_0209abdc(target, NULL);
}

s32 func_ov003_0209acd0(TaskPool* pool, Task* task, void* args) {
    BtlTarget* target = task->data;
    target->active    = 1;
    target->target    = NULL;
    func_ov003_0209abdc(target, NULL);
    CombatSprite_LoadFromTable(1, &target->sprites[0], &data_ov003_020d7800, data_ov003_020d7fec, 0, 0x3F, 0x10);
    CombatSprite_LoadFromTable(0, &target->sprites[1], &data_ov003_020d7800, data_ov003_020d7fec, 0, 0x3D, 0x10);
    CombatSprite_SetAnimFromTable(&target->sprites[0], 1, 1);
    CombatSprite_SetAnimFromTable(&target->sprites[1], 1, 1);
    return 1;
}

s32 func_ov003_0209ad7c(TaskPool* pool, Task* task, void* args) {
    BtlTarget*   target = task->data;
    CombatActor* actor  = data_ov003_020e71b8->unk3D8A0;
    if (actor != NULL && ((actor->flags & 4) || (actor->flags & 0x10))) {
        data_ov003_020e71b8->unk3D8A0 = NULL;
    }
    if (target->target != data_ov003_020e71b8->unk3D8A0) {
        target->target = data_ov003_020e71b8->unk3D8A0;
        CombatSprite_SetAnim(&target->sprites[0], 2, 1);
        CombatSprite_SetAnim(&target->sprites[1], 2, 1);
        CombatSprite_Restart(&target->sprites[0]);
        CombatSprite_Restart(&target->sprites[1]);
        func_ov003_0209abdc(target, func_ov003_0209ac8c);
    }
    if (target->update != NULL)
        target->update(target);
    CombatSprite_Update(&target->sprites[0]);
    CombatSprite_Update(&target->sprites[1]);
    return target->active;
}

s32 func_ov003_0209ae4c(TaskPool* pool, Task* task, void* args) {
    BtlTarget* target = task->data;
    if (data_ov003_020e71b8->unk3D874 != 2)
        return 1;
    if (target->target != NULL) {
        func_ov003_0209abe4(&target->sprites[0], target->target);
        if (target->target->unk_78 != 0) {
            func_ov003_0209abe4(&target->sprites[1], (CombatActor*)target->target->unk_78);
        }
    }
    return 1;
}

s32 func_ov003_0209aea8(TaskPool* pool, Task* task, void* args) {
    BtlTarget* target = task->data;
    CombatSprite_Release(&target->sprites[0]);
    CombatSprite_Release(&target->sprites[1]);
    return 1;
}

extern const TaskStages data_ov003_020d7fdc;
s32                     func_ov003_0209aec8(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7fdc;
    if (func_ov003_0208b690((u16)stage))
        return 1;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e3c80[16] = "Tsk_BtlTarget";
const TaskHandle data_ov003_020d7fd0     = {data_ov003_020e3c80, func_ov003_0209aec8, sizeof(BtlTarget)};
const TaskStages data_ov003_020d7fdc     = {
    func_ov003_0209acd0,
    func_ov003_0209ad7c,
    func_ov003_0209ae4c,
    func_ov003_0209aea8,
};
const SpriteAnimEntry data_ov003_020d7fec[2] = {
    {49, 51, 50, 1},
    {49, 51, 50, 2}
};
