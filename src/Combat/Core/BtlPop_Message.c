#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

extern BinIdentifier data_ov003_020d7800;

extern void func_020265d4(void* value, s32 target, u16 frames);
extern s32  func_ov003_020843b0(s32 engine, s32 x);
extern s32  func_ov003_020843ec(s32 engine, s32 y, s32 z);

/// Task data of the short message popup (`Tsk_BtlPop_Message`): a single sprite that slides
/// down towards its target Z, fades in, then fades out.
typedef struct BtlPop_Message {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          x;     // world position the popup tracks
    /* 0x64 */ s32          y;
    /* 0x68 */ s32          z;     // display Z, eased towards `targetZ`
    /* 0x6C */ s32          targetZ;
    /* 0x70 */ s16          phase; // 0 = slide in, 1 = fade out
    /* 0x72 */ s16          timer;
    /* 0x74 */ s32          active;
} BtlPop_Message; // Size: 0x78

/// Parameters handed to the popup task by `func_ov003_0209c984`.
typedef struct BtlPop_MessageArgs {
    /* 0x00 */ s32 engine;   // screen selector, also the sprite loader flag
    /* 0x04 */ u16 animBase; // added to the base animation index of the message sprite
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 z;
} BtlPop_MessageArgs; // Size: 0x14

#define OV003_POP_COUNT_MAIN (data_ov003_020e71b8->unk3D8FC)
#define OV003_POP_COUNT_SUB  (data_ov003_020e71b8->unk3D8FD)

s32 func_ov003_0209c688(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209c794(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209c840(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209c8b8(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209c91c(TaskPool* pool, Task* task, void* args, s32 stage);

/// The popup picks its animation index conditionally and the retail call site passes it through
/// without narrowing it to `CombatSprite_LoadDirect`'s `u16` parameter, so call it through a
/// plain-integer signature (same idiom as `CombatSprite_LoadDirectNoProto` in `ov003_02087b7c.c`).
#define CombatSprite_LoadDirectNoProto                                                                     \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, s32, s32, s32, s32, s32))CombatSprite_LoadDirect)

char data_ov003_020e3cc4[0x14] = "Tsk_BtlPop_Message";

const TaskHandle data_ov003_020d80c0 = {data_ov003_020e3cc4, func_ov003_0209c91c, 0x78};

const TaskStages data_ov003_020d80cc = {
    func_ov003_0209c688,
    func_ov003_0209c794,
    func_ov003_0209c840,
    func_ov003_0209c8b8,
};

s32 func_ov003_0209c688(TaskPool* pool, Task* task, void* args) {
    BtlPop_Message*     pop = task->data;
    BtlPop_MessageArgs* p   = args;
    s32                 animIndex;
    s32                 z;

    MI_CpuSet(pop, 0, sizeof(BtlPop_Message));
    pop->active  = 1;
    pop->x       = p->x;
    pop->y       = p->y;
    z            = p->z;
    pop->z       = z;
    pop->targetZ = z - 0x20000;
    pop->phase   = 0;
    pop->timer   = 0;

    if (p->engine == 0) {
        animIndex = 0x3D;
        OV003_POP_COUNT_MAIN++;
    } else {
        animIndex = 0x3F;
        OV003_POP_COUNT_SUB++;
    }
    CombatSprite_LoadDirectNoProto(p->engine, &pop->sprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, animIndex,
                                   (u16)(p->animBase + 0x32));
    func_ov003_02082738(&pop->sprite, 0);
    func_ov003_02082730(&pop->sprite, 1);
    return 1;
}

s32 func_ov003_0209c794(TaskPool* pool, Task* task, void* args) {
    BtlPop_Message* pop = task->data;

    switch (pop->phase) {
        case 0:
            if (pop->timer == 0) {
                pop->timer = 0x1E;
            }
            func_020265d4(&pop->z, pop->targetZ, (u16)pop->timer);
            pop->timer--;
            if (pop->timer <= 0) {
                pop->phase++;
                pop->timer = 0;
            }
            break;

        case 1:
            if (pop->timer == 0) {
                pop->timer = 8;
            }
            pop->timer--;
            if (pop->timer <= 0) {
                pop->active = 0;
            }
            break;
    }
    CombatSprite_Update(&pop->sprite);
    return pop->active;
}

s32 func_ov003_0209c840(TaskPool* pool, Task* task, void* args) {
    BtlPop_Message* pop = task->data;
    s32             posX;
    s32             posY;

    if (pop->sprite.sprite.bits_0_1 == 0) {
        posX = func_ov003_020843b0(0, pop->x);
        posY = func_ov003_020843ec(0, pop->y, pop->z);
    } else {
        posX = func_ov003_020843b0(1, pop->x);
        posY = func_ov003_020843ec(1, pop->y, pop->z);
    }
    CombatSprite_SetPosition(&pop->sprite, posX, posY);
    CombatSprite_Render(&pop->sprite);
    return 1;
}

s32 func_ov003_0209c8b8(TaskPool* pool, Task* task, void* args) {
    BtlPop_Message* pop = task->data;

    if (pop->sprite.sprite.bits_0_1 == 0) {
        OV003_POP_COUNT_MAIN--;
    } else {
        OV003_POP_COUNT_SUB--;
    }
    CombatSprite_Release(&pop->sprite);
    return 1;
}

s32 func_ov003_0209c91c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d80cc;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns a message popup for the given screen unless the per-screen popup counter is already
/// saturated (10 live messages).
void func_ov003_0209c984(s32 engine, s32 animBase, s32 x, s32 y, s32 z) {
    BtlPop_MessageArgs args;
    s32                live;

    if (engine == 0) {
        live = data_ov003_020e71b8->unk3D8FC;
    } else {
        live = data_ov003_020e71b8->unk3D8FD;
    }
    // The retail code folds the room test back into the same variable.
    live = live < 0xA;
    if (live == 0) {
        return;
    }

    args.engine   = engine;
    args.animBase = animBase;
    args.x        = x;
    args.y        = y;
    args.z        = z;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d80c0, NULL, 0, NULL, &args);
}
