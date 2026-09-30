#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Noise/Private/BtlEnm006.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

typedef struct Enm006Swlo {
    /* 0x00 */ BtlEnm006* unk_00; // mode: non-zero selects unk_0C
    /* 0x04 */ BtlEnm006* unk_04;
    /* 0x08 */ BtlEnm006* unk_08;
    /* 0x0C */ BtlEnm006* unk_0C;
    /* 0x10 */ s32        unk_10;
    /* 0x14 */ s32        unk_14;
    /* 0x18 */ s32        unk_18;
    /* 0x1C */ s32        unk_1C;
    u8                    pad20[0x2C - 0x20];
    /* 0x2C */ u16        unk_2C; // clamped to 0xC000
    /* 0x2E */ u16        unk_2E;
    /* 0x30 */ u8         unk_30;
    u8                    pad31[0x34 - 0x31];
} Enm006Swlo; // Size: 0x34 (from the Tsk_BtlEnm006_Swlo TaskHandle)

extern s32  func_ov010_02127178(TaskPool*, Task*, void*, s32);
extern s32  func_ov010_021271c0(Enm006Swlo*, Enm006Spawn*);
extern s32  func_ov010_021272e0(Enm006Swlo*);
extern void func_ov003_020cbcb4(s32* outX, s32* outY, u16 angle, s32 radius, s32 scale);
extern s32  func_ov003_020c37f8(void*);
extern void func_ov003_02084694(void*, s32);
extern s32  func_ov003_020c5b0c(u16, void*, s32);
extern void func_ov003_020c4fc8(void*);
extern char data_ov010_021293cc[20];

const TaskHandle Tsk_BtlEnm006_Swlo      = {data_ov010_021293cc, func_ov010_02127178, 0x34};
char             data_ov010_021293cc[20] = "Tsk_BtlEnm006_Swlo";

s32 func_ov010_021270a8(BtlEnm006* owner, BtlEnm006* target) {
    TaskPool*   pool;
    Enm006Spawn args;
    args.unk_04 = owner;
    args.unk_00 = NULL;
    args.unk_08 = target;
    if (func_ov003_020c37f8(&owner->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    return EasyTask_CreateTask(pool, &Tsk_BtlEnm006_Swlo, 0, 0, 0, &args);
}

void func_ov010_02127110(BtlEnm006* owner, BtlEnm006* target) {
    TaskPool*   pool;
    Enm006Spawn args;
    args.unk_04 = owner;
    args.unk_00 = (BtlEnm006*)1;
    args.unk_0C = target;
    if (func_ov003_020c37f8(&owner->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm006_Swlo, 0, 0, 0, &args);
}

s32 func_ov010_02127178(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_021271c0(data, args);
        case 1:
            return func_ov010_021272e0((Enm006Swlo*)data);
        case 3:
            return func_ov010_02127458();
    }
    return 1;
}

s32 func_ov010_021271c0(Enm006Swlo* data, Enm006Spawn* args) {
    BtlEnm006* target;
    MI_CpuSet(data, 0, 0x34);
    data->unk_00 = args->unk_00;
    data->unk_04 = args->unk_04;
    if (data->unk_00 == NULL) {
        target       = args->unk_08;
        data->unk_08 = target;
        if (func_ov003_020c5b0c(func_ov003_020c37f8(&data->unk_04->sprite) != 0 ? 0x52 : 0x4D, data->unk_04,
                                data->unk_08->actor.position.x) != 1)
        {
            return 0;
        }
    } else {
        target       = args->unk_0C;
        data->unk_0C = target;
        func_ov003_02084694(&target->unk_144, 1);
        CombatActor_SetPendingCommand(&target->actor, 1);
        func_ov010_021256d0(data->unk_04, data->unk_0C);
    }
    data->unk_30 = (data->unk_30 & ~1) | 1;
    target->actor.flags |= 0x10000000;
    target->actor.unk_10 = 600;
    data->unk_10         = args->unk_04->actor.position.x;
    data->unk_14         = args->unk_04->actor.position.y;
    data->unk_2C         = 0x4000;
    data->unk_2E         = 0;
    data->unk_18         = (RNG_Next(9) - 4) << 12;
    data->unk_1C         = (RNG_Next(9) - 4) << 12;
    return 1;
}

s32 func_ov010_021272e0(Enm006Swlo* data) {
    data->unk_2C = data->unk_2C + 0x600;
    s32 result   = 1;
    if (data->unk_2C >= 0xC000) {
        data->unk_2C = 0xC000;
        result       = 0;
    }
    s32 x;
    s32 y;
    func_ov003_020cbcb4(&x, &y, data->unk_2C, 0x20000, 0x400);
    if (data->unk_04->actor.isFlipped == 0) {
        x = -x;
    }
    if (data->unk_00 == NULL) {
        data->unk_08->actor.position.x = x + data->unk_10 + data->unk_18;
        data->unk_08->actor.position.y = y + data->unk_14;
        data->unk_08->actor.position.z = data->unk_1C - 0x8000;
    } else {
        data->unk_0C->actor.position.x = x + data->unk_10 + data->unk_18;
        data->unk_0C->actor.position.y = y + data->unk_14;
        data->unk_0C->actor.position.z = data->unk_1C - 0x8000;
    }
    if (result == 0) {
        BtlEnm006* self;
        if (data->unk_00 == NULL) {
            self               = data->unk_08;
            self->actor.unk_10 = 0;
            if (data->unk_08->actor.isFlipped == 0) {
                data->unk_08->actor.xVelocity = 0x1800;
            } else {
                data->unk_08->actor.xVelocity = -0x1800;
            }
            data->unk_08->actor.zVelocity = -0x5000;
            CombatActor_SetPendingCommand(&data->unk_08->actor, 2);
        } else {
            self = data->unk_0C;
            self->unk_18C |= 0x1000;
            func_ov003_020c4fc8(data->unk_0C);
        }
        self->actor.flags &= ~0x10000000;
    }
    return result;
}

s32 func_ov010_02127458(void) {
    return 1;
}
