#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

extern char data_ov011_0212cc24[20];
s32         func_ov011_0212801c(TaskPool* arg0, Task* arg1, void* arg2, s32 index);

extern s32 func_ov003_020c37f8(void* p);
extern s32 func_ov003_020c3c28(void);
extern s16 func_ov003_020843b0(s32 a, s32 b);

typedef struct BtlEnm010SWayArgs {
    /* 0x00 */ BtlEnm010Owner* field_00;
    /* 0x04 */ s32             field_04;
    /* 0x08 */ s32             field_08;
    /* 0x0C */ s32             field_0C;
    /* 0x10 */ s32             field_10;
    /* 0x14 */ u16             field_14; // centre angle
    /* 0x16 */ s16             field_16;
    /* 0x18 */ s32             field_18; // shot count, at most 5
    /* 0x1C */ u16             field_1C; // spread
    /* 0x1E */ u16             pad_1E;
} BtlEnm010SWayArgs;

typedef struct BtlEnm010SWay {
    /* 0x00 */ BtlEnm010Owner* unk_00;
    /* 0x04 */ s32             unk_04; // sub engine
    /* 0x08 */ s16             unk_08;
    /* 0x0A */ s16             unk_0A;
    /* 0x0C */ s32             unk_0C[5]; // SingleShot task ids
} BtlEnm010SWay;                          // Size: 0x20

s32 func_ov011_02128070(BtlEnm010SWay* p, BtlEnm010SWayArgs* a);
s32 func_ov011_02128150(BtlEnm010SWay* p);
s32 func_ov011_02128250(BtlEnm010SWay* p);

extern s32 func_ov003_020c3c28(void);

const TaskHandle data_ov011_0212c1e0 = {(const char*)data_ov011_0212cc24, func_ov011_0212801c, 32};

char data_ov011_0212cc24[20] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D,
                                0x30, 0x31, 0x30, 0x5F, 0x53, 0x57, 0x61, 0x79, 0x00, 0x00};

s32 func_ov011_02127f6c(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5, u16 arg6, s32 arg7, s16 arg8) {
    BtlEnm010SWayArgs t;
    TaskPool*         pool;

    if (func_ov003_020c37f8(&((BtlEnm010Owner*)arg0)->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    if (arg4 == 1) {
        arg6 = 0;
    }
    t.field_00 = arg0;
    t.field_04 = arg1;
    t.field_08 = arg2;
    t.field_0C = arg3;
    t.field_10 = arg7;
    t.field_14 = arg5;
    t.field_16 = arg8;
    t.field_18 = arg4;
    t.field_1C = arg6;
    return EasyTask_CreateTask(pool, &data_ov011_0212c1e0, 0, 0, 0, (void*)&t);
}

s32 func_ov011_0212801c(TaskPool* arg0, Task* arg1, void* arg2, s32 index) {
    void* p;
    s32   r;

    p = arg1->data;
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_02128070(p, arg2);
            break;
        case 1:
            r = func_ov011_02128150(p);
            break;
        case 3:
            r = func_ov011_02128250(p);
            break;
    }
    return r;
}

s32 func_ov011_02128070(BtlEnm010SWay* p, BtlEnm010SWayArgs* a) {
    s32 i;

    MI_CpuSet(p, 0, 0x20);
    for (i = 0; i < a->field_18; i++) {
        s32 v;

        v            = (s16)((s16)_s32_div_f(a->field_1C * i, a->field_18 - 1) + (s16)((s32)a->field_14 - a->field_1C / 2));
        p->unk_0C[i] = func_ov011_021282b8(a->field_00, a->field_04, a->field_08, a->field_0C, v, a->field_10, a->field_16);
    }
    for (; i < 5; i++) {
        p->unk_0C[i] = -1;
    }
    p->unk_04 = (func_ov003_020c37f8(&a->field_00->sprite) != 0) ? 1 : 0;
    p->unk_00 = a->field_00;
    return 1;
}

s32 func_ov011_02128150(BtlEnm010SWay* p) {
    BtlEnm010Owner* o;
    TaskPool*       pool;
    s32             r;
    s32             i;

    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    if (p->unk_00 != NULL && (p->unk_00->actor.flags & 4) != 0) {
        return r;
    }
    if (p->unk_08 == 0) {
        o = p->unk_00;
        s32 pan;
        if (func_ov003_020c37f8(&o->sprite) != 0) {
            pan = func_ov003_020843b0(1, o->actor.position.x);
        } else {
            pan = func_ov003_020843b0(0, o->actor.position.x);
        }
        func_ov003_02087f00(0x1D9, pan);
        p->unk_08 = p->unk_08 + 1;
    }
    if (p->unk_04 == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    for (i = 0; i < 5; i++) {
        EasyTask_ValidateTaskId(pool, (u32*)&p->unk_0C[i]);
        if (p->unk_0C[i] == -1) {
            r++;
        }
    }
    return (r == 5) ? 0 : 1;
}

s32 func_ov011_02128250(BtlEnm010SWay* p) {
    TaskPool* pool;
    s32       i;

    if (p->unk_04 == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    for (i = 0; i < 5; i++) {
        Task* t = EasyTask_GetTaskById(pool, p->unk_0C[i]);
        if (t != NULL) {
            t->flags = t->flags | 0x10;
        }
    }
    return 1;
}
