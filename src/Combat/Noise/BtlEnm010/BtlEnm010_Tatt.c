#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

extern char data_ov011_0212cc68[20];

typedef struct BtlEnm010ArmFrame {
    /* 0x00 */ u16 unk_00; // heading upper bound
    /* 0x02 */ u16 unk_02; // anim
    /* 0x04 */ u16 unk_04; // flip
    /* 0x06 */ u16 unk_06;
} BtlEnm010ArmFrame;

typedef struct BtlEnm010CmdTbl {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8  unk_04;
    /* 0x05 */ u8  unk_05;
    /* 0x06 */ u8  unk_06;
    /* 0x07 */ u8  unk_07;
    /* 0x08 */ s32 unk_08;
} BtlEnm010CmdTbl;

void              func_ov011_02128e30(BtlEnm010Tatt* data);
void              func_ov011_02128eb0(BtlEnm010Tatt* data);
void              func_ov011_02128f10(BtlEnm010TattRec* p, u16 v);
BtlEnm010TattRec* func_ov011_02128f80(BtlEnm010Tatt* data);
void              func_ov011_02129188(BtlEnm010Tatt* p, BtlEnm010TattRec* rec);
void              func_ov011_021293a8(BtlEnm010Tatt* arg0, BtlEnm010TattRec* p);
void              func_ov011_02129410(BtlEnm010Tatt* p, BtlEnm010TattRec* rec);
s32               func_ov011_02129934(TaskPool* arg0, Task* arg1, void* arg2, s32 index);
s32               func_ov011_02129994(BtlEnm010Tatt* data, BtlEnm010Owner** arg1);
s32               func_ov011_02129b84(BtlEnm010Tatt* data);
s32               func_ov011_02129cec(BtlEnm010Tatt* data);
s32               func_ov011_02129ea4(BtlEnm010Tatt* p);

extern const BtlEnm010CmdTbl* func_ov003_0208a114(u16 idx);
extern s32                    func_ov003_0208a1a4(const BtlEnm010CmdTbl* rec, void* p, s32 x, s32 y);
extern void                   func_ov003_020cbd30(s32* out, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 t);
extern u16                    func_ov003_020cba14(s32 x0, s32 y0, s32 x1, s32 y1);
extern s32                    func_ov003_020c37f8(void* p);
extern s32                    func_ov003_020c3c28(void);
extern s16                    func_ov003_020843b0(s32 a, s32 b);
extern s32                    func_ov003_020843ec(s32 a, s32 b, s32 c);
extern s32                    func_ov003_020cbc50(s32* a, s32* b, u16 c, s32 d);
extern s32                    func_ov003_020cba2c(s32 a0, s32 a1, s32 a2, s32 a3);

#define NEXT_REC(p) ((p) += sizeof(BtlEnm010TattRec) / sizeof(*(p)))

#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

const s32 data_ov011_0212c230[4] = {-344064, 0, -65536, 0};

const TaskHandle data_ov011_0212c240 = {(const char*)data_ov011_0212cc68, func_ov011_02129934, 592};

const s32 data_ov011_0212c24c[4] = {-786432, -589824, -393216, -196608};

const s32 data_ov011_0212c25c[6] = {-81920, -294912, -81920, -458752, -344064, -458752};

const s32 data_ov011_0212c274[6] = {196608, -131072, 196608, -458752, -65536, -458752};

const BtlEnm010ArmFrame data_ov011_0212c28c[16] = {
    { 2366, 10, 1, 0},
    { 6371,  9, 1, 0},
    {10922,  8, 1, 0},
    {15109,  7, 1, 0},
    {17840,  6, 0, 0},
    {21845,  7, 0, 0},
    {26396,  8, 0, 0},
    {30583,  9, 0, 0},
    {35134,  0, 1, 0},
    {39139,  1, 1, 0},
    {43690,  2, 1, 0},
    {47877,  3, 1, 0},
    {50608,  4, 0, 0},
    {54613,  3, 0, 0},
    {59164,  2, 0, 0},
    {63351,  1, 0, 0},
};

char data_ov011_0212cc68[20] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D,
                                0x30, 0x31, 0x30, 0x5F, 0x54, 0x61, 0x74, 0x74, 0x00, 0x00};

s32 func_ov011_02128c44(void* p) {
    BtlEnm010Owner* arg0;
    TaskPool*       pool;

    arg0 = p;
    if (func_ov003_020c37f8(&arg0->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    return EasyTask_CreateTask(pool, &data_ov011_0212c240, 0, 0, 0, (void*)&arg0);
}

void func_ov011_02128ca4(BtlEnm010Tatt* data, void (*arg1)(BtlEnm010Tatt*)) {
    data->unk_224 = arg1;
    data->unk_228 = 0;
    data->unk_22A = 0;
    data->unk_22C = 0;
}

void func_ov011_02128cc0(BtlEnm010Tatt* data) {
    s32               i;
    BtlEnm010TattRec* rec;

    if (data->unk_228 == 0) {
        data->rec[0].unk_84 = (data->rec[0].unk_84 & 0xFFFE) | 1;
        data->rec[1].unk_84 = (data->rec[1].unk_84 & 0xFFFE) | 1;
        data->rec[2].unk_84 = data->rec[2].unk_84 & 0xFFFE;
        data->rec[3].unk_84 = data->rec[3].unk_84 & 0xFFFE;
        data->rec[0].unk_84 = data->rec[0].unk_84 | 8;
        data->rec[1].unk_84 = data->rec[1].unk_84 | 8;
        data->rec[2].unk_84 = data->rec[2].unk_84 | 8;
        data->rec[3].unk_84 = data->rec[3].unk_84 | 8;
        data->rec[0].unk_78 = 0x800;
        data->rec[1].unk_78 = 0x800;
        data->rec[2].unk_78 = 0x1000;
        data->rec[3].unk_78 = 0x1000;
        func_ov011_02128f10(&data->rec[0], 0xEE38);
        func_ov011_02128f10(&data->rec[1], 0xEE38);
        func_ov011_02128f10(&data->rec[2], 0xFF49);
        func_ov011_02128f10(&data->rec[3], 0xFF49);
        rec = data->rec;
        for (i = 0; i < 4; i++) {
            rec->unk_7C = (((u32)(data->unk_24C << 31) >> 31) != 0) ? 0 - data_ov011_0212c24c[i] : data_ov011_0212c24c[i];
            rec->unk_80 = 0 - 0x70000;
            rec++;
        }
        data->unk_228 = data->unk_228 + 1;
    }
    rec = data->rec;
    for (i = 0; i < 4; i++) {
        if (((u32)(rec->unk_84 << 28) >> 31) == 1) {
            break;
        }
        rec++;
    }
    if (i != 4) {
        return;
    }
    func_ov011_02128ca4(data, func_ov011_02128e30);
}

void func_ov011_02128e30(BtlEnm010Tatt* data) {
    s32  i;
    u16* p;
    u16* q;
    s32  j;

    if (data->unk_228 == 0) {
        data->unk_228 = data->unk_228 + 1;
        p             = &data->rec[0].unk_84;
        for (i = 0; i < 4; i++) {
            *p = *p | 0x10;
            NEXT_REC(p);
        }
    }
    q = &data->rec[0].unk_84;
    for (j = 0; j < 4; j++) {
        if (((u32)(*q << 27) >> 31) == 1) {
            break;
        }
        NEXT_REC(q);
    }
    if (j != 4) {
        return;
    }
    func_ov011_02128ca4(data, func_ov011_02128eb0);
}

void func_ov011_02128eb0(BtlEnm010Tatt* data) {
    BtlEnm010TattRec* p;

    if (data->unk_228 % 60 == 0) {
        p = func_ov011_02128f80(data);
        if (p != NULL) {
            p->unk_84 = p->unk_84 | 0x20;
        }
    }
    data->unk_228 = data->unk_228 + 1;
}

void func_ov011_02128f10(BtlEnm010TattRec* p, u16 v) {
    s32 i;

    i = 0;
    do {
        if (v < data_ov011_0212c28c[i].unk_00) {
            break;
        }
        i++;
    } while (i < 0x10);
    if (i == 0x10) {
        i = 0;
    }
    p->unk_70 = v;
    CombatSprite_SetAnimFromTable(&p->sprite, data_ov011_0212c28c[i].unk_02, 0);
    CombatSprite_SetFlip(&p->sprite, data_ov011_0212c28c[i].unk_04);
}

BtlEnm010TattRec* func_ov011_02128f80(BtlEnm010Tatt* data) {
    s32               v;
    s32               d;
    s32               i;
    BtlEnm010TattRec* best;
    BtlEnm010TattRec* p;

    best = NULL;
    v    = 0x7FFFFFFF;
    if (((u32)(data->unk_24C << 30) >> 31) != 0) {
        d = (s32)(func_ov003_020843b0(1, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x) > 0
                      ? 0.5f + (f32)(func_ov003_020843b0(1, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x) << 12)
                      : (f32)(func_ov003_020843b0(1, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x) << 12) - 0.5f);
    } else {
        d = (s32)(func_ov003_020843b0(0, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x) > 0
                      ? 0.5f + (f32)(func_ov003_020843b0(0, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x) << 12)
                      : (f32)(func_ov003_020843b0(0, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x) << 12) - 0.5f);
    }
    p = data->rec;
    for (i = 0; i < 4; i++) {
        if ((((u32)(p->unk_84 << 30) >> 31) != 0) && (((u32)(p->unk_84 << 26) >> 31) == 0)) {
            d = d - (data->unk_23C + p->unk_68);
            if (d < 0) {
                d = 0 - d;
            }
            if (d < v) {
                v    = d;
                best = p;
            }
        }
        p++;
    }
    return best;
}

void func_ov011_02129110(BtlEnm010Tatt* p) {
    s32               i;
    BtlEnm010TattRec* rec;

    rec = p->rec;
    for (i = 0; i < 4; i++) {
        if ((u32)(rec->unk_84 << 28) >> 31) {
            func_ov011_02129188(p, rec);
        }
        if ((u32)(rec->unk_84 << 27) >> 31) {
            func_ov011_021293a8(p, rec);
        }
        if ((u32)(rec->unk_84 << 26) >> 31) {
            func_ov011_02129410(p, rec);
        }
        rec++;
    }
}

void func_ov011_02129188(BtlEnm010Tatt* p, BtlEnm010TattRec* rec) {
    struct {
        s32 m[2];
        s32 org[2];
    } f;
    s32        state;
    const s32* tpl;
    s32        limit;
    s32        count;
    s32        cur;
    s32        dst;
    s32        d;

    state = rec->unk_64;
    switch (state) {
        case 0:
            if (((u32)(rec->unk_84 << 31) >> 31) == 1) {
                f.org[0] = data_ov011_0212c230[0];
                f.org[1] = data_ov011_0212c230[1];
                tpl      = data_ov011_0212c25c;
            } else {
                f.org[0] = data_ov011_0212c230[2];
                f.org[1] = data_ov011_0212c230[3];
                tpl      = data_ov011_0212c274;
            }
            if (((u32)(p->unk_24C << 31) >> 31) != 0) {
                f.org[0] = 0 - f.org[0];
            }
            if (rec->unk_60 == 0) {
                rec->unk_62 = 0x14;
            }
            limit = rec->unk_62;
            count = rec->unk_60;
            if (count >= limit) {
                rec->unk_60 = 0;
                rec->unk_64 = 1;
            } else {
                func_ov003_020cbd30(f.m, tpl[0], tpl[1], tpl[2], tpl[3], tpl[4], tpl[5], _s32_div_f(ROUND(count), limit));
                rec->unk_68 = (((u32)(p->unk_24C << 31) >> 31) != 0) ? 0 - f.m[0] : f.m[0];
                rec->unk_6C = f.m[1];
                rec->unk_60 = rec->unk_60 + 1;
            }
            func_ov011_02128f10(rec, func_ov003_020cba14(f.org[0], f.org[1], rec->unk_68, rec->unk_6C));
            break;
        case 1:
            if (rec->unk_60 == 0) {
                rec->unk_60 = rec->unk_60 + 1;
                func_ov011_02128f10(rec, 0xC000);
            }
            cur = rec->unk_68;
            dst = rec->unk_7C;
            d   = cur - dst;
            if (d < 0) {
                d = 0 - d;
            }
            if (d < 0x4000) {
                rec->unk_68 = dst;
                rec->unk_84 = rec->unk_84 & ~8;
                rec->unk_60 = 0;
                rec->unk_64 = 0;
            } else if (cur > dst) {
                rec->unk_68 = rec->unk_68 - 0x4000;
            } else {
                rec->unk_68 = rec->unk_68 + 0x4000;
            }
            break;
    }
    rec->unk_78 = rec->unk_78 + 0x19A;
    if (rec->unk_78 > 0x1000) {
        rec->unk_78 = 0x1000;
    }
}

void func_ov011_021293a8(BtlEnm010Tatt* arg0, BtlEnm010TattRec* p) {
    if (p->unk_60 == 0) {
        p->unk_60 = p->unk_60 + 1;
        CombatSprite_SetAnimFromTable(&p->sprite, 5, 1);
    }
    if (SpriteMgr_IsAnimationFinished(&p->sprite.sprite) == 0) {
        return;
    }
    CombatSprite_SetAnimFromTable(&p->sprite, 6, 1);
    p->unk_84 = p->unk_84 & ~0x10;
    p->unk_60 = 0;
    p->unk_64 = 0;
}

void func_ov011_02129410(BtlEnm010Tatt* p, BtlEnm010TattRec* rec) {
    s32 vx;
    s32 vy;
    s32 idx;
    s32 bx;
    s32 by;
    s32 ox;
    s32 oy;
    s32 i;
    s32 off;

    if (((u32)(p->unk_24C << 30) >> 31) != 0) {
        vx = ROUND(func_ov003_020843b0(1, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x));
        vy = ROUND(func_ov003_020843ec(1, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.y,
                                       ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.z));
    } else {
        vx = ROUND(func_ov003_020843b0(0, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x));
        vy = ROUND(func_ov003_020843ec(0, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y,
                                       ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.z));
    }
    switch (rec->unk_64) {
        case 0:
            if (rec->unk_60 == 0) {
                rec->unk_62 = 0x3C;
                rec->unk_7C = vx - p->unk_23C;
                rec->unk_80 = vy - p->unk_240 - 0x10000;
                rec->unk_74 = func_ov003_020cba2c(p->unk_23C + rec->unk_68, p->unk_240 + rec->unk_6C, vx, vy);
                rec->unk_70 = func_ov003_020cba14(rec->unk_68, rec->unk_6C, rec->unk_7C, rec->unk_80);
                func_ov011_02128f10(rec, rec->unk_70);
            }
            if (rec->unk_60 < rec->unk_62) {
                rec->unk_60 = rec->unk_60 + 1;
                return;
            }
            rec->unk_60 = 0;
            rec->unk_64 = 1;
            return;
        case 1:
            if (rec->unk_60 == 0) {
                rec->unk_62 = 0xA;
                rec->unk_84 = rec->unk_84 | 4;
                rec->unk_68 = rec->unk_7C;
                rec->unk_6C = rec->unk_80;
                rec->unk_72 = rec->unk_70 + 0x8000;
            }
            if (rec->unk_60 >= rec->unk_62) {
                rec->unk_60 = 0;
                rec->unk_64 = 2;
                return;
            }
            rec->unk_74 = rec->unk_74 + 0x2000;
            rec->unk_60 = rec->unk_60 + 1;
            return;
        case 2:
            if (rec->unk_60 == 0) {
                rec->unk_60 = rec->unk_60 + 1;
                func_ov003_02087f00(0x1E1, (s16)((p->unk_23C + rec->unk_68) >> 12));
            }
            if (rec->unk_74 > 0) {
                rec->unk_74 = rec->unk_74 - 0x8000;
                if (rec->unk_74 < 0) {
                    rec->unk_74 = 0;
                    rec->unk_60 = 0;
                    rec->unk_64 = 3;
                }
            }
            if (((u32)(p->unk_24C << 30) >> 31) != 0) {
                idx = 0x5F;
            } else {
                idx = 0x58;
            }
            func_ov003_020cbc50(&bx, &by, rec->unk_72, rec->unk_74);
            bx  = bx + p->unk_23C + rec->unk_68;
            by  = by + p->unk_240 + rec->unk_6C;
            i   = -1;
            off = i - 0xF;
            do {
                func_ov003_020cbc50(&ox, &oy, rec->unk_72, off * 0x1000);
                func_ov003_0208a1a4(func_ov003_0208a114(idx), &p->unk_244, (s16)((bx + ox) >> 12), (s16)((by + oy) >> 12));
                off = off + 0x10;
                i   = i + 1;
            } while (i < 4);
            return;
        case 3:
            if (rec->unk_60 == 0) {
                rec->unk_62 = 0xA;
            }
            if (rec->unk_60 < rec->unk_62) {
                rec->unk_60 = rec->unk_60 + 1;
                return;
            }
            rec->unk_84 = rec->unk_84 & ~2;
    }
}

s32 func_ov011_02129934(TaskPool* arg0, Task* arg1, void* arg2, s32 index) {
    void* p;
    s32   r;

    p = arg1->data;
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_02129994(p, arg2);
            break;
        case 1:
            r = func_ov011_02129b84(p);
            break;
        case 2:
            r = func_ov011_02129cec(p);
            break;
        case 3:
            r = func_ov011_02129ea4(p);
            break;
    }
    return r;
}

s32 func_ov011_02129994(BtlEnm010Tatt* data, BtlEnm010Owner** arg1) {
    s32               r;
    s32               i;
    BtlEnm010TattRec* q;
    u16*              p;

    MI_CpuSet(data, 0, 0x250);
    r             = (func_ov003_020c37f8(&(*arg1)->sprite) != 0) ? 1 : 0;
    data->unk_000 = *arg1;
    data->unk_230 = (*arg1)->actor.position.x;
    data->unk_234 = (*arg1)->actor.position.y;
    data->unk_238 = (*arg1)->actor.position.z;
    data->unk_23C = (s32)(func_ov003_020843b0(r, (*arg1)->actor.position.x) > 0
                              ? 0.5f + (f32)(func_ov003_020843b0(r, (*arg1)->actor.position.x) << 12)
                              : (f32)(func_ov003_020843b0(r, (*arg1)->actor.position.x) << 12) - 0.5f);
    data->unk_240 =
        (s32)(func_ov003_020843ec(r, (*arg1)->actor.position.y, (*arg1)->actor.position.z) > 0
                  ? 0.5f + (f32)(func_ov003_020843ec(r, (*arg1)->actor.position.y, (*arg1)->actor.position.z) << 12)
                  : (f32)(func_ov003_020843ec(r, (*arg1)->actor.position.y, (*arg1)->actor.position.z) << 12) - 0.5f);
    q = data->rec;
    p = &data->rec[0].unk_84;
    i = 0;
    do {
        func_ov011_021258b4((*arg1)->sprite.sprite.bits_0_1, &q->sprite, 1);
        i++;
        p[0] = p[0] | 2;
        NEXT_REC(p);
        q++;
    } while (i < 4);
    func_ov011_02128ca4(data, func_ov011_02128cc0);
    if ((*arg1)->actor.isFlipped == 0) {
        data->unk_24C = data->unk_24C & ~1;
    } else {
        data->unk_24C = (u8)((data->unk_24C & ~1) | 1);
    }
    if (func_ov003_020c37f8(&(*arg1)->sprite) != 0) {
        data->unk_24C = data->unk_24C | 2;
    } else {
        data->unk_24C = data->unk_24C & ~2;
    }
    {
        BtlEnm010Owner* owner = *arg1;

        data->unk_244 = *(const BtlEnm010Pair16*)&owner->actor.unk_04;
        data->unk_248 = *(const BtlEnm010Pair16*)&owner->actor.unk_08;
    }
    return 1;
}

s32 func_ov011_02129b84(BtlEnm010Tatt* data) {
    s32               r;
    s32               dir;
    s32               i;
    u16*              p;
    BtlEnm010TattRec* q;

    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    {
        BtlEnm010Owner* owner = data->unk_000;

        if (owner != NULL && (owner->actor.flags & 4) != 0) {
            return r;
        }
    }
    dir = ((u32)(data->unk_24C << 30) >> 31) != 0 ? 1 : 0;
    data->unk_23C =
        (s32)(func_ov003_020843b0(dir, data->unk_230) > 0 ? 0.5f + (f32)(func_ov003_020843b0(dir, data->unk_230) << 12)
                                                          : (f32)(func_ov003_020843b0(dir, data->unk_230) << 12) - 0.5f);
    data->unk_240 = (s32)(func_ov003_020843ec(dir, data->unk_234, data->unk_238) > 0
                              ? 0.5f + (f32)(func_ov003_020843ec(dir, data->unk_234, data->unk_238) << 12)
                              : (f32)(func_ov003_020843ec(dir, data->unk_234, data->unk_238) << 12) - 0.5f);
    {
        void (*cb)(BtlEnm010Tatt*) = data->unk_224;

        if (cb != NULL) {
            cb(data);
        }
    }
    func_ov011_02129110(data);
    p = &data->rec[0].unk_84;
    q = data->rec;
    i = 0;
    do {
        if (((u32)(*p << 30) >> 31) == 1) {
            CombatSprite_Update(&q->sprite);
            r = 1;
        }
        NEXT_REC(p);
        q++;
        i++;
    } while (i < 4);
    return r;
}

s32 func_ov011_02129cec(BtlEnm010Tatt* data) {
    s32               dir;
    s32               vx;
    s32               vy;
    s32               i;
    s32               a;
    s32               b;
    BtlEnm010TattRec* p;

    dir = (s32)(((u32)(data->unk_24C << 30) >> 31) != 0) ? 1 : 0;
    vx  = (s32)(func_ov003_020843b0(dir, data->unk_230) > 0 ? 0.5f + (f32)(func_ov003_020843b0(dir, data->unk_230) << 12)
                                                            : (f32)(func_ov003_020843b0(dir, data->unk_230) << 12) - 0.5f);
    vy  = (s32)(func_ov003_020843ec(dir, data->unk_234, data->unk_238) > 0
                    ? 0.5f + (f32)(func_ov003_020843ec(dir, data->unk_234, data->unk_238) << 12)
                    : (f32)(func_ov003_020843ec(dir, data->unk_234, data->unk_238) << 12) - 0.5f);
    a   = 0;
    b   = 0;
    p   = data->rec;
    i   = 0;
    do {
        u16 fl = p->unk_84;

        if (((u32)(fl << 30) >> 31) != 0) {
            if (((u32)(fl << 29) >> 31) != 0) {
                func_ov003_020cbc50(&a, &b, p->unk_72, p->unk_74);
                a = vx + p->unk_68 + a;
                b = vy + p->unk_6C + b;
            } else {
                a = vx + p->unk_68;
                b = vy + p->unk_6C;
            }
            CombatSprite_SetPosition(&p->sprite, (b * 16) >> 16, (a * 16) >> 16);
            CombatSprite_SetAffineTransform(&p->sprite, i, p->unk_78, p->unk_78, i);
            CombatSprite_Render(&p->sprite);
        }
        i++;
        p++;
    } while (i < 4);
    return 1;
}

s32 func_ov011_02129ea4(BtlEnm010Tatt* p) {
    s32               i;
    BtlEnm010TattRec* sp;

    sp = p->rec;
    for (i = 0; i < 4; i++) {
        CombatSprite_Release(&sp->sprite);
        sp++;
    }
    return 1;
}
