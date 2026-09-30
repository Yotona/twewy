#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Noise/Private/BtlEnm010.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

typedef s32 (*BtlEnm010Fn)(void*, void*, void*);

typedef struct BtlEnm010FnTable {
    BtlEnm010Fn f[4];
} BtlEnm010FnTable;

typedef struct BtlEnm010Cooldown {
    /* 0x00 */ s16 unk_00; // base
    /* 0x02 */ s16 unk_02; // spread
} BtlEnm010Cooldown;

typedef struct BtlEnm010RngeShot {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
} BtlEnm010RngeShot; // Size: 0x10

typedef struct Ov011Rec_PPW {
    void* w0;
    void* w1;
    u32   w2;
} Ov011Rec_PPW;

extern char data_ov011_0212cc8c[20];
void        func_ov011_0212a134(BtlEnm010UG* data);
void        func_ov011_0212a2ec(BtlEnm010UG* data);
void        func_ov011_0212a420(BtlEnm010UG* data);
void        func_ov011_0212a634(BtlEnm010UG* data);
void        func_ov011_0212a674(BtlEnm010UG* data);
void        func_ov011_0212aa20(BtlEnm010UG* data);
void        func_ov011_0212aee8(BtlEnm010UG* data);
void        func_ov011_0212af94(BtlEnm010UG* data);
void        func_ov011_0212b0a4(BtlEnm010UG* data);
void        func_ov011_0212b168(BtlEnm010UG* data);
void        func_ov011_0212b388(BtlEnm010UG* data);
s32         func_ov011_0212b5d8(BtlEnm010UG* data, s32 arg1);
s32         func_ov011_0212b6e0(BtlEnm010UG* data, s32 arg1);
s32         func_ov011_0212b800(BtlEnm010UG* data, s32 arg1);
s32         func_ov011_0212b890(BtlEnm010UG* data, s16* p, s16* q, s32 arg3);
s32         func_ov011_0212b99c(void* arg0, void* arg1, void* arg2, s32 index);
void        func_ov011_0212b9e4(void* arg0, Task* arg1, BtlEnm010Spawn* arg2);
s32         func_ov011_0212bac8(void* arg0, Task* arg1);
s32         func_ov011_0212bc84(void* arg0, Task* arg1);
s32         func_ov011_0212bcc8(void* arg0, Task* arg1);
s16         func_ov011_0212bce0(BtlEnm010UG* data, s32 arg1);
void        func_ov011_0212bd3c(BtlEnm010UG* p, s32 i);
void        func_ov011_0212bd90(BtlEnm010UG* p, s32 i);
s32         func_ov011_0212bdbc(BtlEnm010UG* p, s32 i);

extern s16  func_ov003_020843b0(s32 a, s32 b);
extern s32  func_ov003_02084348(s32 a, s16* b, s16* c, s32 d, s32 e, s32 f);
extern void func_ov003_020c427c(void* p, void* arg1);
extern s32  func_ov003_020c5bfc(void);
extern s32  func_ov003_020c42ec(void* p);
extern s32  func_ov003_020cb744(s32 arg);
extern s32  func_ov003_020cb7a4(s32 arg);
extern s32  func_ov003_020c4c9c(void* p);
extern void func_ov003_020c48b0(void* p);
extern void func_ov003_020c492c(void* p);
extern s32  func_ov003_020cb520(void* p, s32 arg1);
extern s32  func_ov003_020cb594(void* p, s32 arg1);
extern s32  func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);
extern s32  func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32  func_ov003_020c3efc(void* p, void* arg1);
extern void func_ov003_020c4748(void* p);
extern s32  func_ov003_0208810c(void* p, void* arg1);
extern s32  func_ov003_020cb910(void* a0, void* a1, void* a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
extern void func_ov011_0212a134(BtlEnm010UG* data);
extern s32  func_ov011_0212b800(BtlEnm010UG* data, s32 arg1);
extern s32  func_ov003_020c6bc8(BtlEnm010UG* data, s32 arg1);
extern void func_ov011_0212b388(BtlEnm010UG* data);
extern s32  func_ov003_020c6c2c(BtlEnm010UG* data, s32 arg1);
extern s32  func_ov003_020c7070(BtlEnm010UG* data);
extern s32  func_ov003_020c4c9c(void* p);
extern void func_ov003_020c48b0(void* p);
extern void func_ov003_020c492c(void* p);
extern s32  func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);
extern void func_ov003_020c4520(void* p);
extern void func_ov003_020c4b5c(void* p);
extern s32  func_ov011_0212b890(BtlEnm010UG* data, s16* arg1, s16* arg2, s32 arg3);
extern s32  func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32  func_ov003_020cb888(void* p, s32 arg1, s32 arg2);
extern void func_ov011_0212a134(BtlEnm010UG* data);
extern void func_ov011_0212a2ec(BtlEnm010UG* data);
extern void func_ov011_0212a420(BtlEnm010UG* data);
extern void func_ov011_0212a634(BtlEnm010UG* data);
extern s32  func_ov003_020cb594(void* p, s32 arg1);
extern s16  func_ov011_0212bce0(BtlEnm010UG* data, s32 arg1);
extern s32  func_ov003_020cb744(s32 arg0);
extern s32  func_ov003_020cb7a4(s32 arg0);
extern s32  func_ov003_020c6b8c(BtlEnm010UG* data, s32 arg1);
extern void func_ov011_0212aa20(BtlEnm010UG* data);
extern s32  func_ov011_0212b6e0(BtlEnm010UG* data, s32 arg1);
extern s32  func_ov011_0212b5d8(BtlEnm010UG* data, s32 arg1);
extern s32  func_ov003_02084348(s32 a0, s16* a1, s16* a2, s32 a3, s32 a4, s32 a5);
extern s32  func_ov003_020cc7c0(s32 a0, s32 a1, s32 a2);
extern s32  func_ov003_020c42ec(void* p);
extern s32  func_ov003_020c4348(void* p);
extern void func_ov011_0212b168(BtlEnm010UG* data);
extern void func_ov011_0212a78c(BtlEnm010UG* data);
extern void func_ov011_0212ac0c(BtlEnm010UG* data);
extern void func_ov011_0212aee8(BtlEnm010UG* data);
extern s32  func_ov011_0212bdbc(BtlEnm010UG* p, s32 i);
extern void func_ov003_02084694(void* p, s32 arg1);
extern s32  func_ov003_020c3bf0(void* p);
extern s32  func_ov003_020c4668(void* p);
extern s32  func_ov003_020cba2c(s32 a0, s32 a1, s32 a2, s32 a3);

/// Per-slot cooldown: a random value in `unk_00 +/- unk_02` frames.
const BtlEnm010Cooldown data_ov011_0212c30c[1] = {
    {1200, 60},
};

const char data_ov011_0212c310[8] = {0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00};

const char data_ov011_0212c318[8] = {0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00};

const char data_ov011_0212c320[8] = {0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00};

const char data_ov011_0212c328[8] = {0x00, 0x00, 0x32, 0x00, 0x32, 0x00, 0x00, 0x00};

const s32 data_ov011_0212c330[3] = {4, 7, 2};

const s32 data_ov011_0212c33c[4] = {0, 1, 0, 3};

const s32 data_ov011_0212c34c[4] = {1, 1, 0, 2};

const s32 data_ov011_0212c35c[4] = {30, 40, 50, 60};

const BtlEnm010FnTable data_ov011_0212c36c = {
    {(BtlEnm010Fn)func_ov011_0212b9e4, (BtlEnm010Fn)func_ov011_0212bac8, (BtlEnm010Fn)func_ov011_0212bc84,
     (BtlEnm010Fn)func_ov011_0212bcc8}
};

const s32 data_ov011_0212c37c[5] = {1, 1, 2, 1, 0};

const s32 data_ov011_0212c390[6] = {3, 1, 2, 1, 3, 0};

const s32 data_ov011_0212c3a8[4][2] = {
    {1, 0},
    {1, 0},
    {2, 2},
    {3, 3},
};

/// `Tsk_BtlEnm010_Rnge` burst parameters, per variant slot and shot.
const BtlEnm010RngeShot data_ov011_0212c3c8[4][2] = {
    {{3, 8192, 0, 819, 10, 0},        {0, 0, 0, 0, 0, 0}},
    {{5, 8192, 0, 819, 10, 0},        {0, 0, 0, 0, 0, 0}},
    {      {0, 0, 0, 0, 0, 0},        {0, 0, 0, 0, 0, 0}},
    {{4, 8192, 0, 819, 10, 0}, {3, 5462, 0, 1638, 30, 0}},
};

const Ov011Rec_PPW data_ov011_0212c448 = {data_ov011_0212cc8c, func_ov011_0212b99c, 0x00000214};

const void* data_ov011_0212cc7c[4] = {data_ov011_0212c310, data_ov011_0212c328, data_ov011_0212c318, data_ov011_0212c320};

char data_ov011_0212cc8c[20] = {0x54, 0x61, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E,
                                0x6D, 0x30, 0x31, 0x30, 0x5F, 0x55, 0x47, 0x00, 0x00, 0x00};

void func_ov011_02129ed0(BtlEnm010UG* data, void (*func)(BtlEnm010UG*)) {
    func_ov003_020c427c(data, (void*)func);
    data->unk_1C8 = func;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

void func_ov011_02129ef8(BtlEnm010UG* data) {
    void (*f)(BtlEnm010UG*);

    f = NULL;
    switch (func_ov003_020cb888(data_ov011_0212cc7c[data->unk_080], 4, RNG_Next(0x64))) {
        case 0:
            f = func_ov011_0212a134;
            break;
        case 1:
            f = func_ov011_0212a2ec;
            break;
        case 2:
            f = func_ov011_0212a420;
            break;
        case 3:
            f = func_ov011_0212a634;
            break;
    }
    func_ov011_02129ed0(data, f);
}

void func_ov011_02129f80(BtlEnm010UG* data) {
    void (*f)(BtlEnm010UG*);
    s32 v;

    v = 0;
    f = NULL;
    switch (data->unk_080) {
        case 0:
            v = data_ov011_0212c37c[(u32)(data->unk_1F6) % 5];
            break;
        case 1:
            v = data_ov011_0212c390[(u32)(data->unk_1F6) % 6];
            break;
        case 2:
            if (func_ov011_0212bdbc(data, 0) != 0) {
                v = 5;
            } else {
                v = data_ov011_0212c330[RNG_Next(3)];
            }
            break;
        case 3:
            v = data_ov011_0212c3a8[((u32)data->unk_1F6 << 30) >> 30][(u32)RNG_Next(2)];
            break;
    }
    data->unk_1F6 = data->unk_1F6 + 1;
    switch (v) {
        case 0:
            f = func_ov011_0212a674;
            break;
        case 1:
            f = func_ov011_0212a78c;
            break;
        case 2:
            f = func_ov011_0212aa20;
            break;
        case 3:
            f = func_ov011_0212ac0c;
            break;
        case 4:
            f = func_ov011_0212aee8;
            break;
        case 5:
            f = func_ov011_0212af94;
            break;
        case 6:
            f = func_ov011_0212b0a4;
            break;
        case 7:
            f = func_ov011_0212b168;
            break;
    }
    func_ov011_02129ed0(data, f);
}

void func_ov011_0212a10c(BtlEnm010UG* data) {
    if (func_ov003_020c5bfc() != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212a134(BtlEnm010UG* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            data->unk_1C2 = func_ov003_020c42ec(data);
            data->unk_1FC = 0;
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            break;
        case 1:
            if (func_ov011_0212b800(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 2;
                break;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            break;
        case 2:
            v = data->actor.isFlipped;
            if ((v == 0 && data->actor.position.x < ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x) ||
                (v == 1 && data->actor.position.x > ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x))
            {
                if (data->unk_1FC == 3) {
                    func_ov011_02129ed0(data, func_ov011_0212aa20);
                    data->unk_1FC = 0;
                    return;
                }
                data->unk_1FC = data->unk_1FC + 1;
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(0, &data->sprite, 0);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
                CombatSprite_Restart(&data->sprite);
            }
            if (data->unk_1C2 > 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
            } else if (func_ov003_020c4348(data) != 0) {
                func_ov011_02129f80(data);
            } else {
                func_ov011_02129ef8(data);
            }
            break;
    }
    data->unk_1C2 = data->unk_1C2 - 1;
}

void func_ov011_0212a2ec(BtlEnm010UG* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1DC = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                data->unk_1E0 = RNG_Next((func_ov003_020cb7a4(0) >> 12) + 1) << 12;
                data->unk_1E4 = 0;
                v             = data->actor.isFlipped;
                if ((v == 0 && data->unk_1DC < data->actor.position.x) || (v == 1 && data->unk_1DC >= data->actor.position.x))
                {
                    data->unk_1C0 = 0;
                    data->unk_1C4 = 1;
                    return;
                }
            }
            if (func_ov011_0212b800(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0) {
                func_ov011_02129ed0(data, func_ov011_0212a134);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

void func_ov011_0212a420(BtlEnm010UG* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1DC = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                data->unk_1E0 = data->actor.position.y;
                data->unk_1E4 = 0;
                v             = data->actor.isFlipped;
                if ((v == 0 && data->unk_1DC < data->actor.position.x) || (v == 1 && data->unk_1DC >= data->actor.position.x))
                {
                    data->unk_1C0 = 0;
                    data->unk_1C4 = 1;
                    return;
                }
            }
            if (func_ov011_0212b800(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (func_ov011_0212b6e0(data, data->unk_1C0) == 0) {
                func_ov011_02129ed0(data, func_ov011_0212a134);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

void func_ov011_0212a540(BtlEnm010UG* data) {
    s32 v;

    if (data->unk_1C0 == 0) {
        func_ov011_02125750(0, &data->sprite, 6);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
        func_ov003_020c4b5c(data);
        v                      = 0 - 0x40000;
        data->actor.position.z = v;
        data->unk_1D0          = (data->actor.isFlipped == 0) ? v + 0x38000 : 0x8000;
        data->unk_1C0          = data->unk_1C0 + 1;
        func_ov003_02087f00(0x1E3, func_ov003_020843b0(0, data->actor.position.x));
    }
    if (data->sprite.sprite.cellIndex == 3 && data->sprite.sprite.frameTimer == 1) {
        data->unk_1D8 = 0x2800;
    }
    if (data->actor.position.z < 0) {
        return;
    }
    data->unk_1D8     = 0;
    data->unk_1D0     = 0;
    data->actor.flags = data->actor.flags & ~0x10000000;
    func_ov003_020cb520(data, 1);
    func_ov003_020cb594(data, 1);
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212a634(BtlEnm010UG* data) {
    s32 r;

    r             = func_ov011_0212b800(data, data->unk_1C0);
    data->unk_1C0 = data->unk_1C0 + 1;
    if (r != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212a674(BtlEnm010UG* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            v             = data->actor.position.x;
            data->unk_1DC = v;
            data->unk_1E0 = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
            v             = 0;
            data->unk_1E4 = v;
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0 || data->unk_1C0 >= 0x3C) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x1E;
                data->unk_1FA = 0xBD;
            }
            {
                s32 r;
                s32 old;

                old           = data->unk_1C0;
                data->unk_1C0 = old + 1;
                r             = func_ov011_0212b890(data, (s16*)&data->unk_1F8, &data->unk_1FA, old);
                if (data->unk_1F8 == 0 && data->unk_1FA == 0xBD) {
                    data->unk_208 = func_ov011_02127c84(data);
                }
                if (r != 0) {
                    return;
                }
                func_ov011_02129ed0(data, func_ov011_0212a134);
            }
            return;
    }
}

void func_ov011_0212a78c(BtlEnm010UG* data) {
    s32 v;
    s16 limit;
    s32 r;

    switch (data->unk_1C4) {
        case 0:
            v             = data->actor.position.x;
            data->unk_1DC = v;
            data->unk_1E0 = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
            v             = 0;
            data->unk_1E4 = v;
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0 || data->unk_1C0 >= 0x3C) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            limit = data_ov011_0212c34c[data->unk_080] * 10 + 0x14;
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x14;
                data->unk_1FA = limit;
            }
            {
                s32 old;

                old           = data->unk_1C0;
                data->unk_1C0 = old + 1;
                r             = func_ov011_0212b890(data, (s16*)&data->unk_1F8, &data->unk_1FA, old);
            }
            if (data->unk_1F8 == 0 && data->unk_1FA % 10 == 0) {
                s32 q = (limit - data->unk_1FA) / 10;

                if (q < data_ov011_0212c34c[data->unk_080]) {
                    s32 bias;
                    u16 a5;

                    if (data->actor.isFlipped != 0) {
                        data->unk_1D0 = 0 - 0x4000;
                        bias          = 0x40000;
                        a5            = 0;
                    } else {
                        data->unk_1D0 = 0x4000;
                        bias          = 0 - 0x40000;
                        a5            = 0x8000;
                    }
                    data->unk_1E8 = 0;
                    {
                        s32 slot = data->unk_080;

                        data->unk_208 =
                            func_ov011_02127f6c(data, bias, 0, 0 - 0x10000, data_ov011_0212c3c8[slot][q].unk_00, a5,
                                                data_ov011_0212c3c8[slot][q].unk_04, data_ov011_0212c3c8[slot][q].unk_08,
                                                data_ov011_0212c3c8[slot][q].unk_0C);
                    }
                }
            }
            if (data->unk_1D0 > 0) {
                data->unk_1E8 = data->unk_1E8 - 0x33;
                data->unk_1E8 = data->unk_1E8 - 0x300;
                if (data->unk_1D0 + data->unk_1E8 <= 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            } else if (data->unk_1D0 < 0) {
                data->unk_1E8 = data->unk_1E8 + 0x33;
                data->unk_1E8 = data->unk_1E8 + 0x300;
                if (data->unk_1D0 + data->unk_1E8 >= 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            }
            if (r != 0) {
                return;
            }
            data->unk_1D0 = 0;
            data->unk_1E8 = 0;
            func_ov011_02129ed0(data, func_ov011_0212a134);
            return;
    }
}

void func_ov011_0212aa20(BtlEnm010UG* data) {
    s32 bias;

    bias = (data->actor.isFlipped == 0) ? 0 - 0x60000 : 0x60000;
    if (data->unk_1C4 == 0 && data->unk_1C0 == 0) {
        data->unk_1C4 = (func_ov003_020cba2c(data->actor.position.x, data->actor.position.y,
                                             ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x,
                                             ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y) < 0x80000)
                            ? 1
                            : 0;
        data->unk_1C0 = 0;
    }
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1DC = (data->actor.position.x < ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x)
                                    ? ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x - 0x60000
                                    : ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x + 0x60000;
                data->unk_1E0 = data->actor.position.y;
                data->unk_1E4 = 0;
            }
            if (func_ov011_0212b6e0(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(0, &data->sprite, 3);
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 1);
                func_ov003_02087f00(0x1D7, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 6 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D8, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex >= 5 && data->sprite.sprite.cellIndex <= 7) {
                func_ov003_020c5b2c(0x55, (s32)(u32)data, data->actor.position.x + bias, data->actor.position.y,
                                    data->actor.position.z);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            func_ov011_02129ed0(data, func_ov011_0212a134);
            return;
    }
}

void func_ov011_0212ac0c(BtlEnm010UG* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            v             = data->actor.position.x;
            data->unk_1DC = v;
            data->unk_1E0 = ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.y;
            v             = 0;
            data->unk_1E4 = v;
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0 || data->unk_1C0 >= 0x3C) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(0, &data->sprite, 9);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&data->sprite, 2, 0);
                data->unk_208 = func_ov011_02125b98(data, 0);
                data->unk_1C2 = 0x4B;
            }
            if (data->unk_1C0 >= 0x28 && (data->unk_1C0 - 0x28) % 15 == 0) {
                s32 q = (data->unk_1C0 - 0x28) / 15;

                if (q > 0 && q < data_ov011_0212c33c[data->unk_080]) {
                    data->unk_208 = func_ov011_02125b98(data, q);
                }
            }
            if (data->unk_1C0 >= 0x3C && (data->unk_1C0 - 0x3C) % 15 == 0) {
                s32 q = (data->unk_1C0 - 0x3C) / 15;

                if (q < data_ov011_0212c33c[data->unk_080]) {
                    data->unk_1D0 = (data->actor.isFlipped != 0) ? 0 - 0x4000 : 0x4000;
                    data->unk_1E8 = 0;
                }
                if (q == data_ov011_0212c33c[data->unk_080] - 1) {
                    CombatSprite_SetAnimFromTable(&data->sprite, 3, 1);
                }
            }
            if (data->unk_1D0 > 0) {
                data->unk_1E8 = data->unk_1E8 - 0x33;
                data->unk_1E8 = data->unk_1E8 - 0x300;
                if (data->unk_1D0 + data->unk_1E8 <= 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            } else if (data->unk_1D0 < 0) {
                data->unk_1E8 = data->unk_1E8 + 0x33;
                data->unk_1E8 = data->unk_1E8 + 0x300;
                if (data->unk_1D0 + data->unk_1E8 >= 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                func_ov011_02129ed0(data, func_ov011_0212a134);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

void func_ov011_0212aee8(BtlEnm010UG* data) {
    s32 r;

    if (data->unk_1C0 == 0) {
        data->unk_1F8 = 0x14;
        data->unk_1FA = 0x1E;
    }
    {
        s32 old;

        old           = data->unk_1C0;
        data->unk_1C0 = old + 1;
        r             = func_ov011_0212b890(data, (s16*)&data->unk_1F8, &data->unk_1FA, old);
    }
    if (data->unk_1F8 == 0 && data->unk_1FA == 0x1E) {
        data->unk_208 = func_ov011_02128718(data);
        func_ov003_02087f00(0x1DE, func_ov003_020843b0(0, data->actor.position.x));
    }
    if (r != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212af94(BtlEnm010UG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(0, &data->sprite, 8);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
                func_ov003_02087f00(0x1E0, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->unk_1C0 == 0x1C) {
                data->unk_208 = func_ov011_02128c44(data);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            func_ov011_02129ed0(data, func_ov011_0212a134);
            func_ov011_0212bd3c(data, 0);
            return;
    }
}

void func_ov011_0212b0a4(BtlEnm010UG* data) {
    s32 bias;

    bias = (data->actor.isFlipped == 0) ? 0 - 0x80000 : 0x80000;
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(0, &data->sprite, 4);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
        func_ov003_02087f00(0x1DF, func_ov003_020843b0(0, data->actor.position.x));
    }
    if (data->sprite.sprite.cellIndex >= 3 && data->sprite.sprite.cellIndex <= 4) {
        func_ov003_020c5b2c(0x59, (s32)(u32)data, data->actor.position.x + bias, data->actor.position.y,
                            data->actor.position.z);
    }
    if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212b168(BtlEnm010UG* data) {
    s16 a;
    s16 b;

    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(0, &data->sprite, 5);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
                func_ov003_020cb520(data, 0);
                func_ov003_020cb594(data, 0);
                data->actor.flags = data->actor.flags | 0x10000000;
                data->unk_1C0     = data->unk_1C0 + 1;
                func_ov003_02087f00(0x1E2, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 0);
                data->unk_1D0 = (data->actor.isFlipped == 0) ? 0 - 0x8000 : 0x8000;
                func_ov003_02087f00(0x1E3, func_ov003_020843b0(0, data->actor.position.x));
                data->unk_1C0 = data->unk_1C0 + 1;
            }
            func_ov003_020c5b2c(0x5A, (s32)(u32)data, data->actor.position.x, data->actor.position.y, data->actor.position.z);
            func_ov003_02084348(0, &a, &b, data->actor.position.x, data->actor.position.y, data->actor.position.z);
            if (func_ov003_020cc7c0(a, b, 0x100) != 0) {
                return;
            }
            data->unk_1D0 = 0;
            func_ov011_02129ed0(data, func_ov011_0212a540);
            return;
    }
}

void func_ov011_0212b318(BtlEnm010UG* data) {
    s32 r;

    if (data->unk_1C0 == 0) {
        data->unk_1C0     = data->unk_1C0 + 1;
        data->unk_1D8     = 0;
        data->unk_1D4     = 0;
        data->unk_1D0     = 0;
        data->actor.flags = data->actor.flags & ~0x10000000;
        func_ov011_02125750(0, &data->sprite, 0);
    }
    if (func_ov003_020c6bc8(data, 4) != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212b388);
}

void func_ov011_0212b388(BtlEnm010UG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
        func_ov011_02125750(0, &data->sprite, 0);
    }
    if (func_ov003_020c6c2c(data, 5) != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212b3ec(BtlEnm010UG* data) {
    s32 v;

    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_020cb520(data, 1);
        func_ov011_02125750(0, &data->sprite, 0);
        data->unk_1D8     = 0;
        data->unk_1D4     = 0;
        data->unk_1D0     = 0;
        data->actor.flags = data->actor.flags & ~0x10000000;
    }
    if (func_ov003_020c6b8c(data, 4) != 0) {
        return;
    }
    if (RNG_Next(0x64) < data_ov011_0212c35c[data->unk_080]) {
        v = data->actor.isFlipped;
        if ((v == 0 && ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x < data->actor.position.x) ||
            (v == 1 && ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x > data->actor.position.x))
        {
            func_ov011_02129ed0(data, func_ov011_0212aa20);
            return;
        }
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

void func_ov011_0212b4f4(BtlEnm010UG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(0, &data->sprite, 0);
        data->unk_1D8     = 0;
        data->unk_1D4     = 0;
        data->unk_1D0     = 0;
        data->actor.flags = data->actor.flags & ~0x10000000;
    }
    if (func_ov003_020c7070(data) == 0) {
        data->unk_1CC = 0;
    }
}

void func_ov011_0212b558(BtlEnm010UG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_020cb520(data, 1);
        func_ov011_02125750(0, &data->sprite, 0);
        data->unk_1D0     = 0;
        data->unk_1D4     = 0;
        data->unk_1D0     = 0;
        data->actor.flags = data->actor.flags & ~0x10000000;
    }
    if (func_ov003_020c72b4(data, 0, 4) != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

s32 func_ov011_0212b5d8(BtlEnm010UG* data, s32 arg1) {
    if (arg1 == 0) {
        func_ov011_02125750(0, &data->sprite, 1);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
        data->unk_1FE = func_ov011_0212bce0(data, 0x1800);
        data->unk_200 = 0;
    }
    if ((data->sprite.sprite.cellIndex - 1) % 2 == 0 && data->sprite.sprite.frameTimer == 1) {
        func_ov003_02087f00(0x1D6, func_ov003_020843b0(0, data->actor.position.x));
    }
    if (arg1 == -1) {
        data->unk_1E4 = 0;
        data->unk_1E0 = 0;
        data->unk_1DC = 0;
        data->unk_200 = 0;
        data->unk_1FE = 0;
        return 0;
    }
    if (data->unk_200 < data->unk_1FE) {
        data->unk_200 = data->unk_200 + 1;
        return 1;
    }
    data->unk_1E4 = 0;
    data->unk_1E0 = 0;
    data->unk_1DC = 0;
    data->unk_1D8 = 0;
    data->unk_1D4 = 0;
    data->unk_1D0 = 0;
    data->unk_200 = 0;
    data->unk_1FE = 0;
    return 0;
}

s32 func_ov011_0212b6e0(BtlEnm010UG* data, s32 arg1) {
    if (arg1 == 0) {
        data->unk_204 = 0;
        data->unk_202 = 0;
        func_ov011_02125750(0, &data->sprite, 7);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
        func_ov003_020cb520(data, 0);
    }
    switch (data->sprite.animTableIndex) {
        case 0:
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 0);
                data->unk_202 = func_ov011_0212bce0(data, 0x8000);
                func_ov003_02087f00(0x1E4, func_ov003_020843b0(0, data->actor.position.x));
            }
            goto out;
        case 1:
            if (data->unk_204 < data->unk_202) {
                data->unk_204 = data->unk_204 + 1;
                goto out;
            }
            CombatSprite_SetAnimFromTable(&data->sprite, 2, 1);
            data->unk_1D4 = 0;
            data->unk_1D0 = 0;
            goto out;
        case 2:
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                func_ov003_020cb520(data, 1);
                return 0;
            }
            goto out;
    }
out:
    return 1;
}

s32 func_ov011_0212b800(BtlEnm010UG* data, s32 arg1) {
    if (arg1 == 0) {
        func_ov011_02125750(0, &data->sprite, 3);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
    }
    if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
        func_ov011_02125750(0, &data->sprite, 0);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
        func_ov003_020c4c9c(data);
        if (data->actor.isFlipped == 0) {
            data->actor.position.x = data->actor.position.x + 0x10000;
        } else {
            data->actor.position.x = data->actor.position.x - 0x10000;
        }
        return 0;
    }
    return 1;
}

s32 func_ov011_0212b890(BtlEnm010UG* data, s16* p, s16* q, s32 arg3) {
    if (arg3 == 0) {
        func_ov011_02125750(0, &data->sprite, 2);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
    }
    switch (data->sprite.animTableIndex) {
        case 0:
            if (data->sprite.sprite.cellIndex == 4 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D3, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                *p = *p - 1;
                if (*p <= 0) {
                    CombatSprite_SetAnimFromTable(&data->sprite, 1, 0);
                }
            }
            goto out;
        case 1:
            *q = *q - 1;
            if (*q <= 0) {
                CombatSprite_SetAnimFromTable(&data->sprite, 3, 1);
            }
            goto out;
        case 3:
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                return 0;
            }
            goto out;
    }
out:
    return 1;
}

s32 func_ov011_0212b99c(void* arg0, void* arg1, void* arg2, s32 index) {
    BtlEnm010FnTable t;
    BtlEnm010Fn      f;

    t = data_ov011_0212c36c;
    f = t.f[index];
    return f(arg0, arg1, arg2);
}

void func_ov011_0212b9e4(void* arg0, Task* arg1, BtlEnm010Spawn* arg2) {
    BtlEnm010UG*    data;
    BtlEnm010Spawn* args;

    data = arg1->data;
    args = arg2;
    MI_CpuSet(data, 0, 0x214);
    func_ov011_021256c0(args->unk_04);
    func_ov003_020c3efc(data, args);
    func_ov003_020c4520(data);
    func_ov003_020c4b5c(data);
    data->unk_1C0 = 0;
    data->unk_1C2 = 0;
    data->unk_1C4 = 0;
    data->unk_1CC = 1;
    data->unk_1D8 = 0;
    data->unk_1D4 = 0;
    data->unk_1D0 = 0;
    data->unk_1E4 = 0;
    data->unk_1E0 = 0;
    data->unk_1DC = 0;
    data->unk_1F0 = 0;
    data->unk_1EC = 0;
    data->unk_1E8 = 0;
    data->unk_1F6 = 0;
    data->unk_1F8 = 0;
    data->unk_1FA = 0;
    data->unk_208 = -1;
    data->unk_20C = -1;
    func_ov011_0212bd90((void*)data, 0);
    func_ov011_02129ed0((void*)data, func_ov011_0212a10c);
    data->unk_206     = (data->unk_206 & 0xFE) | 1;
    data->actor.flags = data->actor.flags | 0x40000000;
    data->unk_18C     = data->unk_18C | 4;
}

s32 func_ov011_0212bac8(void* arg0, Task* arg1) {
    BtlEnm010UG* data;

    data = (BtlEnm010UG*)arg1->data;
    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 2:
            if (((u32)(data->unk_206 << 31) >> 31) == 0) {
                break;
            }
            func_ov003_02084694(&data->unk_144, 0);
            func_ov011_02129ed0(data, func_ov011_0212b3ec);
            break;
        case 3:
            func_ov003_02084694(&data->unk_144, 1);
            func_ov011_02129ed0(data, func_ov011_0212b4f4);
            break;
        case 4:
            func_ov003_02084694(&data->unk_144, 0);
            func_ov011_02129ed0(data, func_ov011_0212b318);
            break;
        case 5:
            func_ov003_02084694(&data->unk_144, 0);
            func_ov011_02129ed0(data, func_ov011_0212b388);
            break;
        case 6:
            func_ov011_02129ed0(data, func_ov011_0212b558);
            break;
    }
    if (data_ov003_020e71b8->unk3D874 == 2) {
        if (func_ov003_020c3bf0(data) == 0) {
            func_ov011_0212bd90(data, 0);
        }
    }
    EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, (u32*)&data->unk_208);
    if (data->unk_20C != -1) {
        data->unk_210 = data->unk_210 + 1;
    }
    if (data->unk_1C8 != NULL) {
        (*(void (**)(void*))(&data->unk_1C8))(data);
    }
    data->actor.position.x = data->actor.position.x + data->unk_1D0;
    data->actor.position.y = data->actor.position.y + data->unk_1D4;
    data->actor.position.z = data->actor.position.z + data->unk_1D8;
    data->unk_1D0          = data->unk_1D0 + data->unk_1E8;
    data->unk_1D4          = data->unk_1D4 + data->unk_1EC;
    data->unk_1D8          = data->unk_1D8 + data->unk_1F0;
    func_ov003_020c4668(data);
    return data->unk_1CC;
}

s32 func_ov011_0212bc84(void* arg0, Task* arg1) {
    BtlEnm010UG* p;

    p = arg1->data;
    if (p->unk_20C == -1 || p->unk_210 == 0) {
        func_ov003_020c48b0(p);
    } else {
        func_ov003_020c4748(p);
        func_ov003_0208810c(&p->unk_0E4, p);
    }
    return 1;
}

s32 func_ov011_0212bcc8(void* arg0, Task* arg1) {
    s32 r;

    func_ov003_020c492c(arg1->data);
    func_ov011_02125714();
    return 1;
}

s16 func_ov011_0212bce0(BtlEnm010UG* data, s32 arg1) {
    return func_ov003_020cb910(&data->unk_1D0, &data->unk_1D4, &data->unk_1D8, data->actor.position.x, data->actor.position.y,
                               data->actor.position.z, data->unk_1DC, data->unk_1E0, data->unk_1E4, arg1);
}

void func_ov011_0212bd3c(BtlEnm010UG* p, s32 i) {
    s32 lo;
    s32 hi;
    s32 r;

    lo            = data_ov011_0212c30c[i].unk_02;
    r             = RNG_Next(lo * 2 + 1);
    hi            = data_ov011_0212c30c[i].unk_00;
    p->unk_1F4[i] = hi + (r - lo);
}

void func_ov011_0212bd90(BtlEnm010UG* p, s32 i) {
    s16* q;

    if (p->unk_1F4[i] <= 0) {
        return;
    }
    q    = &p->unk_1F4[0];
    q[i] = q[i] - 1;
}

s32 func_ov011_0212bdbc(BtlEnm010UG* p, s32 i) {
    return p->unk_1F4[i] <= 0;
}
