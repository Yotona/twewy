#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm010Shot {
    /* 0x00 */ s32 unk_00; // distance along the heading
    /* 0x04 */ s32 unk_04; // speed
    /* 0x08 */ s32 unk_08; // acceleration
    /* 0x0C */ s16 unk_0C; // screen x
    /* 0x0E */ s16 unk_0E; // screen y
} BtlEnm010Shot;           // Size: 0x10

typedef struct BtlEnm010SingleShot {
    /* 0x00 */ BtlEnm010Owner* unk_000;
    /* 0x04 */ CombatSprite    sprite;
    /* 0x64 */ s16             unk_064; // launch SE played
    /* 0x66 */ u16             unk_066;
    /* 0x68 */ BtlEnm010Shot   shot[3];
    /* 0x98 */ s32             unk_098; // owner x at launch
    /* 0x9C */ s32             unk_09C; // origin
    /* 0xA0 */ s32             unk_0A0;
    /* 0xA4 */ s32             unk_0A4;
    /* 0xA8 */ s16             unk_0A8;    // launch delay
    /* 0xAA */ u16             unk_0AA;    // heading
    /* 0xAC */ u16             unk_0AC[4]; // copy of the owner's actor.unk_04..unk_0A
} BtlEnm010SingleShot;                     // Size: 0xB4

typedef struct BtlEnm010SingleShotArgs {
    /* 0x00 */ BtlEnm010Owner* field_00;
    /* 0x04 */ s32             field_04; // offset from the owner
    /* 0x08 */ s32             field_08;
    /* 0x0C */ s32             field_0C;
    /* 0x10 */ s32             field_10; // speed
    /* 0x14 */ u16             field_14; // heading
    /* 0x16 */ s16             field_16; // launch delay
} BtlEnm010SingleShotArgs;

extern char data_ov011_0212cc38[28];

typedef struct BtlEnm010CmdTbl {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8  unk_04;
    /* 0x05 */ u8  unk_05;
    /* 0x06 */ u8  unk_06;
    /* 0x07 */ u8  unk_07;
    /* 0x08 */ s32 unk_08;
} BtlEnm010CmdTbl;

s32 func_ov011_02128348(TaskPool* arg0, Task* arg1, void* arg2, s32 index);
s32 func_ov011_021283a8(BtlEnm010SingleShot* data, BtlEnm010SingleShotArgs* arg1);
s32 func_ov011_02128698(BtlEnm010SingleShot* p);
s32 func_ov011_02128704(BtlEnm010SingleShot* p);

extern const BtlEnm010CmdTbl* func_ov003_0208a114(u16 idx);
extern s32                    func_ov003_0208a1a4(const BtlEnm010CmdTbl* rec, void* p, s32 x, s32 y);
extern s32                    func_ov003_0208442c(s32 a0, s32 a1);
extern s32                    func_ov003_020c37f8(void* p);
extern s32                    func_ov003_020c3c28(void);
extern s16                    func_ov003_020843b0(s32 a, s32 b);
extern s32                    func_ov003_02084348(s32 a, s16* b, s16* c, s32 d, s32 e, s32 f);
extern s32                    func_ov003_020cbc50(s32* a, s32* b, u16 c, s32 d);
extern s32                    func_ov003_020cb744(s32 arg);
extern s32                    func_ov011_021283a8(BtlEnm010SingleShot* data, BtlEnm010SingleShotArgs* arg1);
extern s32                    func_ov011_021284bc(BtlEnm010SingleShot* p);
extern s32                    func_ov011_02128698(BtlEnm010SingleShot* p);
extern s32                    func_ov003_020cb744(s32 arg0);
extern s32                    func_ov003_02084348(s32 a0, s16* a1, s16* a2, s32 a3, s32 a4, s32 a5);
extern s32                    func_ov003_020c3c28(void);

const s32 data_ov011_0212c1ec[3] = {4096, 3584, 3072};

const TaskHandle data_ov011_0212c1f8 = {(const char*)data_ov011_0212cc38, func_ov011_02128348, 180};

const s32 data_ov011_0212c204[3] = {4096, 3277, 2458};

char data_ov011_0212cc38[28] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D, 0x30, 0x31, 0x30, 0x5F,
                                0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x53, 0x68, 0x6F, 0x74, 0x00, 0x00, 0x00, 0x00};

s32 func_ov011_021282b8(void* arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s32 arg5, s16 arg6) {
    BtlEnm010SingleShotArgs t;
    TaskPool*               pool;

    if (func_ov003_020c37f8(&((BtlEnm010Owner*)arg0)->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    t.field_00 = arg0;
    t.field_04 = arg1;
    t.field_08 = arg2;
    t.field_0C = arg3;
    t.field_10 = arg5;
    t.field_16 = arg6;
    t.field_14 = arg4;
    return EasyTask_CreateTask(pool, &data_ov011_0212c1f8, 0, 0, 0, (void*)&t);
}

s32 func_ov011_02128348(TaskPool* arg0, Task* arg1, void* arg2, s32 index) {
    void* p;
    s32   r;

    p = arg1->data;
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_021283a8(p, arg2);
            break;
        case 1:
            r = func_ov011_021284bc(p);
            break;
        case 2:
            r = func_ov011_02128698(p);
            break;
        case 3:
            r = func_ov011_02128704(p);
            break;
    }
    return r;
}

s32 func_ov011_021283a8(BtlEnm010SingleShot* data, BtlEnm010SingleShotArgs* arg1) {
    s32             i;
    s32             v;
    BtlEnm010Owner* o;

    MI_CpuSet(data, 0, 0xB4);
    func_ov011_021258b4(arg1->field_00->sprite.sprite.bits_0_1, &data->sprite, 2);
    CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
    for (i = 0; i < 3; i++) {
        data->shot[i].unk_08 = (s32)((((long long)data_ov011_0212c204[i] * (long long)(arg1->field_10)) + 0x800) >> 12);
    }
    data->unk_000    = arg1->field_00;
    data->unk_098    = arg1->field_00->actor.position.x;
    data->unk_09C    = arg1->field_00->actor.position.x + arg1->field_04;
    data->unk_0A0    = arg1->field_00->actor.position.y + arg1->field_08;
    data->unk_0A4    = arg1->field_00->actor.position.z + arg1->field_0C;
    data->unk_0A8    = arg1->field_16;
    data->unk_0AA    = arg1->field_14;
    o                = arg1->field_00;
    data->unk_0AC[0] = o->actor.unk_04;
    data->unk_0AC[1] = o->actor.unk_06;
    data->unk_0AC[2] = o->actor.unk_08;
    data->unk_0AC[3] = o->actor.unk_0A;
}

s32 func_ov011_021284bc(BtlEnm010SingleShot* p) {
    s32            r;
    s32            mode;
    u16            id;
    s32            pan;
    s32            oa;
    s32            ob;
    s16            va;
    s16            vb;
    s32            i;
    BtlEnm010Shot* rec;

    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    if (p->unk_000 != NULL && (p->unk_000->actor.flags & 4) != 0) {
        return r;
    }
    CombatSprite_Update(&p->sprite);
    if (func_ov003_020c37f8(&p->sprite) != 0) {
        mode = 1;
        id   = 0x5C;
        pan  = func_ov003_020843b0(mode, p->unk_098);
    } else {
        mode = 0;
        id   = 0x54;
        pan  = func_ov003_020843b0(mode, p->unk_098);
    }
    if (p->unk_064 == 0) {
        p->unk_064 = p->unk_064 + 1;
        func_ov003_02087f00(0x1D9, pan);
    }
    func_ov003_02084348(mode, &va, &vb, p->unk_09C, p->unk_0A0, p->unk_0A4);
    rec = p->shot;
    for (i = 0; i < 3; i++) {
        if (p->unk_0A8 > 0) {
            p->unk_0A8 = p->unk_0A8 - 1;
        } else {
            rec->unk_00 = rec->unk_00 + rec->unk_04;
            rec->unk_04 = rec->unk_04 + rec->unk_08;
        }
        func_ov003_020cbc50(&oa, &ob, p->unk_0AA, rec->unk_00);
        rec->unk_0C = (oa + (va << 12)) >> 12;
        rec->unk_0E = (ob + (vb << 12)) >> 12;
        if (func_ov003_0208a1a4(func_ov003_0208a114(id), p->unk_0AC, rec->unk_0C, rec->unk_0E) == 1) {
            func_ov003_02087f00(0x1DA, rec->unk_0C);
        }
        {
            s32 v = func_ov003_0208442c(mode, rec->unk_0C);
            if (v < 0 - 0x8000 || v > func_ov003_020cb744(mode) + 0x8000) {
                r++;
            }
        }
        rec++;
    }
    return (r == 3) ? 0 : 1;
}

s32 func_ov011_02128698(BtlEnm010SingleShot* p) {
    BtlEnm010SingleShot* data;
    s32                  base;
    s32                  i;
    BtlEnm010Shot*       rec;

    data = p;
    base = 0;
    rec  = data->shot;
    for (i = 0; i < 3; i++) {
        CombatSprite_SetAffineTransform(&data->sprite, base, data_ov011_0212c1ec[i], data_ov011_0212c1ec[i], base);
        CombatSprite_SetPosition(&data->sprite, rec->unk_0C, rec->unk_0E);
        CombatSprite_Render(&data->sprite);
        rec++;
    }
    return 1;
}

s32 func_ov011_02128704(BtlEnm010SingleShot* p) {
    CombatSprite_Release(&p->sprite);
    return 1;
}
