#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/BtlEnm006.h"

#include <nitro/mi/cpumem.h>

typedef struct Enm006DeadEff {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          position;
} Enm006DeadEff; // Size: 0x6C (from the Tsk_BtlEnm006_DeadEff TaskHandle)

typedef struct Enm006AnimEntryPad2 {
    /* 0x00 */ SpriteAnimEntry entry;
    /* 0x08 */ u8              pad[2];
} Enm006AnimEntryPad2; // Size: 0xA

extern s32  func_ov010_02125730(TaskPool*, Task*, void*, s32);
extern s32  func_ov010_02125780(Enm006DeadEff*, Enm006Spawn*);
extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c3c28(void);
extern void func_ov003_02084348(s32, void*, void*, s32, s32, s32);
extern char data_ov010_0212938c[24];

const s16 data_ov010_02129200[3] = {0x17, 0x17, 0x17};

const SpriteAnimEntry data_ov010_02129206[1] = {
    {16, 18, 17, 0},
};

const SpriteAnimEntry data_ov010_0212920e[1] = {
    {16, 18, 17, 0},
};

const Enm006AnimEntryPad2 data_ov010_02129216 = {
    {16, 18, 17, 0},
    {0, 0},
};

const TaskHandle Tsk_BtlEnm006_DeadEff = {data_ov010_0212938c, func_ov010_02125730, 0x6C};

SpriteAnimEntry* data_ov010_02129380[3] = {
    (SpriteAnimEntry*)&data_ov010_02129216.entry,
    &data_ov010_0212920e[0],
    &data_ov010_02129206[0],
};

char data_ov010_0212938c[24] = "Tsk_BtlEnm006_DeadEff";

void func_ov010_021256d0(BtlEnm006* owner, BtlEnm006* target) {
    BtlEnm006* args[2];
    TaskPool*  pool;
    args[0] = owner;
    args[1] = target;
    if (func_ov003_020c37f8(&target->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm006_DeadEff, 0, 0, 0, args);
}

s32 func_ov010_02125730(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_02125780(data, args);
        case 1:
            return func_ov010_02125854(data);
        case 2:
            return func_ov010_02125878(data);
        case 3:
            return func_ov010_02125900(data);
    }
    return 1;
}

s32 func_ov010_02125780(Enm006DeadEff* data, Enm006Spawn* args) {
    s32 m;
    u16 v;
    MI_CpuSet(data, 0, 0x6C);
    v = args->unk_00->unk_080;
    m = args->unk_00->sprite.sprite.bits_0_1;
    CombatSprite_LoadFromTable(m, &data->sprite, func_ov010_021256c0(v), data_ov010_02129380[v], 0, data_ov010_02129200[v],
                               0x30);
    CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
    CombatSprite_SetFlip(&data->sprite, args->unk_00->actor.isFlipped == 1);
    data->position.x = args->unk_00->actor.position.x;
    data->position.y = args->unk_00->actor.position.y;
    data->position.z = args->unk_00->actor.position.z;
    return 1;
}

s32 func_ov010_02125854(Enm006DeadEff* data) {
    CombatSprite_Update(&data->sprite);
    return SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0;
}

s32 func_ov010_02125878(Enm006DeadEff* data) {
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    s16 x;
    s16 y;
    s32 flag = func_ov003_020c37f8(&data->sprite) != 0;
    func_ov003_02084348(flag, &x, &y, data->position.x, data->position.y, data->position.z);
    CombatSprite_SetPosition(&data->sprite, x, y);
    func_ov003_02082730(&data->sprite, 0x80000000 - data->position.y);
    CombatSprite_Render(&data->sprite);
    return 1;
}

s32 func_ov010_02125900(Enm006DeadEff* data) {
    CombatSprite_Release(&data->sprite);
    return 1;
}
