#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/BtlEnm006.h"
#include "Engine/Core/System.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"

#include <nitro/mi/cpumem.h>

typedef struct Enm006Swirl {
    /* 0x00 */ BtlEnm006*   unk_00; // the owning instance
    /* 0x04 */ CombatSprite sprite;
    /* 0x64 */ s16          unk_64; // one-shot "has fired" flag
    u8                      pad66[0x68 - 0x66];
    /* 0x68 */ s32          unk_68; // position.x, biased by 0x20000
    /* 0x6C */ s32          unk_6C; // position.y
    /* 0x70 */ s32          unk_70;
    /* 0x74 */ s32          unk_74; // accumulator, clamped to 0x4000
    /* 0x78 */ s32          unk_78; // wrap flag
    /* 0x7C */ u16          unk_7C; // angle, clamped to 0x200
    /* 0x7E */ u16          unk_7E;
    /* 0x80 */ s32          unk_80[8];
} Enm006Swirl; // Size: 0xA0 (from the Tsk_BtlEnm006_Swirl TaskHandle)

typedef struct TaskHandleHead {
    /* 0x00 */ const char* taskName;
    /* 0x04 */ s32 (*taskFunc)(TaskPool*, Task*, void*, s32);
} TaskHandleHead; // Size: 0x8

extern s32  func_ov010_02126d04(TaskPool*, Task*, void*, s32);
extern s32  func_ov010_02126d54(Enm006Swirl*, Enm006Spawn*);
extern s32  func_ov010_02126e58(Enm006Swirl*);
extern s32  func_ov010_02126fdc(Enm006Swirl*);
extern void func_ov003_020cbcb4(s32* outX, s32* outY, u16 angle, s32 radius, s32 scale);
extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c3c28(void);
extern void func_ov003_02084348(s32, void*, void*, s32, s32, s32);
extern s16  func_ov003_020843b0(s32, s32);
extern s32  func_ov003_020c5b2c(u16, void*, s32, s32, s32);
extern s32  func_ov003_020cb744(s32);
extern s32  func_ov003_020cb7a4(s32);

const TaskHandleHead Tsk_BtlEnm006_Swirl    = {"Tsk_BtlEnm006_Swirl", func_ov010_02126d04};
const s16            data_ov010_021292b0[4] = {0x17, 0x17, 0x17, 0};
const u32            data_ov010_021292c0    = 0xA0;

const SpriteAnimEntry data_ov010_021292c4[3] = {
    {16, 18, 17, 4},
    {16, 18, 17, 4},
    {16, 18, 17, 4},
};

void func_ov010_02126c38(BtlEnm006* arg0) {
    BtlEnm006* self = arg0;
    TaskPool*  pool;
    if (func_ov003_020c37f8(&self->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(pool, (const TaskHandle*)&Tsk_BtlEnm006_Swirl, 0, 0, 0, &self);
}

void func_ov010_02126c94(Enm006Swirl* data, s32* outX, s32* outY, s32 index) {
    func_ov003_020cbcb4(outX, outY, data->unk_7C + index * 0x10000 / 8, data->unk_70, 0x800);
    *outX += data->unk_68;
    *outY += data->unk_6C;
}

s32 func_ov010_02126d04(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_02126d54(data, args);
        case 1:
            return func_ov010_02126e58(data);
        case 2:
            return func_ov010_02126fdc(data);
        case 3:
            return func_ov010_02127094(data);
    }
    return 1;
}

s32 func_ov010_02126d54(Enm006Swirl* data, Enm006Spawn* args) {
    s32 m;
    u16 v;
    s32 i;
    MI_CpuSet(data, 0, 0xA0);
    v = args->unk_00->unk_080;
    m = args->unk_00->sprite.sprite.bits_0_1;
    CombatSprite_LoadFromTable(m, &data->sprite, func_ov010_021256c0(v), data_ov010_021292c4, 0, data_ov010_021292b0[v], 0x30);
    CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
    data->unk_00 = args->unk_00;
    data->unk_68 = args->unk_00->actor.position.x;
    data->unk_6C = args->unk_00->actor.position.y;
    data->unk_74 = 0;
    data->unk_7E = 0;
    if (args->unk_00->actor.isFlipped == 0) {
        data->unk_78 = 1;
        data->unk_68 += 0x20000;
    } else {
        data->unk_78 = 0;
        data->unk_68 -= 0x20000;
    }
    for (i = 0; i < 8; i++) {
        data->unk_80[i] = 1;
    }
    return 1;
}

s32 func_ov010_02126e58(Enm006Swirl* data) {
    s32 count = 0;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    if (data->unk_64 == 0) {
        data->unk_64++;
        func_ov003_02087f00(0x1D0, func_ov003_020843b0(0, data->unk_00->actor.position.x));
    }
    s32 w = func_ov003_020cb744(0);
    s32 h = func_ov003_020cb7a4(0);
    CombatSprite_Update(&data->sprite);
    data->unk_70 += data->unk_74;
    s32 step = data->unk_74 + 0x9A;
    step += 0x100;
    data->unk_74 = step;
    if (data->unk_74 > 0x4000) {
        data->unk_74 = 0x4000;
    }
    if (data->unk_78 == 1) {
        data->unk_7C = data->unk_7C + data->unk_7E;
    } else {
        data->unk_7C = data->unk_7C - data->unk_7E;
    }
    data->unk_7E += 0x20;
    if (data->unk_7E > 0x200) {
        data->unk_7E = 0x200;
    }
    s32 i;
    for (i = 0; i < 8; i++) {
        if (data->unk_80[i] == 0) {
            continue;
        }
        s32 x;
        s32 y;
        func_ov010_02126c94(data, &x, &y, i);
        if (x - 0x10 < 0 || x + 0x10 >= w || y - 0x10 < 0 || y + 0x10 >= h) {
            data->unk_80[i] = 0;
        }
        func_ov003_020c5b2c(0x50, data->unk_00, x, y, 0);
        count++;
    }
    return count != 0;
}

s32 func_ov010_02126fdc(Enm006Swirl* data) {
    s32 flag = func_ov003_020c37f8(&data->sprite) != 0;
    s32 i;
    for (i = 0; i < 8; i++) {
        if (data->unk_80[i] == 0) {
            continue;
        }
        s32 x;
        s32 y;
        s16 sx;
        s16 sy;
        func_ov010_02126c94(data, &x, &y, i);
        func_ov003_02084348(flag, &sx, &sy, x, y, 0);
        CombatSprite_SetPosition(&data->sprite, sx, sy);
        func_ov003_02082730(&data->sprite, 0x7FFFFFFF - y);
        CombatSprite_Render(&data->sprite);
    }
    return 1;
}

s32 func_ov010_02127094(Enm006Swirl* data) {
    CombatSprite_Release(&data->sprite);
    return 1;
}
