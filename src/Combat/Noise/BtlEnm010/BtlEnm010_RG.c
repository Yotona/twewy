#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm010RGAim {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ s16 unk_02;
} BtlEnm010RGAim;

typedef struct BtlEnm010RGEntry {
    /* 0x00 */ void (*func[4])(void*, s32, s32);
} BtlEnm010RGEntry;

typedef struct Ov011Rec_PPW {
    void* w0;
    void* w1;
    u32   w2;
} Ov011Rec_PPW;

extern char data_ov011_0212cbfc[20];
void        func_ov011_0212681c(BtlEnm010RG* data);
void        func_ov011_02126aec(BtlEnm010RG* data);
void        func_ov011_02126b2c(BtlEnm010RG* data);
void        func_ov011_02126e80(BtlEnm010RG* data);
void        func_ov011_02127240(BtlEnm010RG* data);
void        func_ov011_02127370(BtlEnm010RG* data);
void        func_ov011_02127474(BtlEnm010RG* data);
s32         func_ov011_02127758(void* p, s32 arg1);
s32         func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3);
void        func_ov011_021278d4(void* p, s32 arg1, s32 arg2, s32 index);
s32         func_ov011_0212791c(void* arg0, Task* arg1, s32 arg2);
s32         func_ov011_02127a64(void* arg0, Task* arg1);
s32         func_ov011_02127b98(void* arg0, Task* arg1);
s32         func_ov011_02127bdc(void* arg0, Task* arg1);
s32         func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1);
s32         func_ov011_02127c4c(BtlEnm010RG* data);

extern s16  func_ov003_020843b0(s32 a, s32 b);
extern void func_ov003_020c427c(void* p, void* arg1);
extern void func_ov011_02126b2c(BtlEnm010RG* data);
extern void func_ov011_02126bf8(BtlEnm010RG* data);
extern void func_ov011_02126e80(BtlEnm010RG* data);
extern void func_ov011_02126fb0(BtlEnm010RG* data);
extern void func_ov011_02127240(BtlEnm010RG* data);
extern void func_ov011_02127370(BtlEnm010RG* data);
extern void func_ov011_02127474(BtlEnm010RG* data);
extern s32  func_ov003_020c5bfc(void);
extern s32  func_ov003_020c42ec(void* p);
extern void func_ov011_02126aec(BtlEnm010RG* data);
extern void func_ov011_0212681c(BtlEnm010RG* data);
extern s32  func_ov003_020cb744(s32 arg);
extern s32  func_ov003_020cb7a4(s32 arg);
extern s32  func_ov003_020c4ab4(BtlEnm010RG* data, s32 arg1);
extern s32  func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1);
extern s32  func_ov003_020c4c9c(void* p);
extern s32  func_ov003_020cb520(void* p, s32 arg1);
extern s32  func_ov003_020cb594(void* p, s32 arg1);
extern s32  func_ov011_02127758(void* p, s32 arg1);
extern s32  func_ov003_020c6230(void* p);
extern s32  func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3);
extern s32  func_ov003_020c703c(void* p);
extern s32  func_ov003_020c62c4(void* p, s32 arg1);
extern s32  func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);
extern s32  func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32  func_ov003_020c3efc(void* p, void* arg1);
extern s32  func_ov003_020c44ac(void* p);
extern s32  func_ov003_020c4b1c(void* p);
extern void func_ov003_020c4748(void* p);
extern s32  func_ov003_0208810c(void* p, void* arg1);
extern s32  func_ov003_020cba54(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern s32  func_ov003_020cb910(void* a0, void* a1, void* a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
extern s32  func_ov003_020c4628(void* p);
extern s32  func_ov003_020c4c9c(void* p);
extern s32  func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);
extern s32  func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32  func_ov003_020cb594(void* p, s32 arg1);
extern s32  func_ov003_020cb744(s32 arg0);
extern s32  func_ov003_020cb7a4(s32 arg0);
extern s32  func_ov003_020c42ec(void* p);

const Ov011Rec_PPW data_ov011_0212c130 = {data_ov011_0212cbfc, func_ov011_021278d4, 0x00000208};

const BtlEnm010RGAim data_ov011_0212c13c[3] = {
    {0x0000, 10},
    {0x0400, 10},
    {0xFC00, 10}
};

const s32 data_ov011_0212c148[4] = {0, 1, 0, 3};

const BtlEnm010RGEntry data_ov011_0212c158 = {
    {(void (*)(void*, s32, s32))func_ov011_0212791c, (void (*)(void*, s32, s32))func_ov011_02127a64,
     (void (*)(void*, s32, s32))func_ov011_02127b98, (void (*)(void*, s32, s32))func_ov011_02127bdc}
};

const s32 data_ov011_0212c168[4] = {1, 2, 0, 3};

const s32 data_ov011_0212c178[5] = {7, 4, 8, 4, 9};

const s32 data_ov011_0212c18c[5] = {2, 4, 1, 2, 0};

const s32 data_ov011_0212c1a0[6] = {2, 1, 5, 4, 2, 0};

const s32 data_ov011_0212c1b8[7] = {0, 2, 1, 4, 3, 5, 9};

char data_ov011_0212cbfc[20] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D,
                                0x30, 0x31, 0x30, 0x5F, 0x52, 0x47, 0x00, 0x00, 0x00, 0x00};

void func_ov011_021265d4(BtlEnm010RG* data, void (*func)(struct BtlEnm010RG*)) {
    func_ov003_020c427c(data, (void*)func);
    data->unk_1C8 = func;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

void func_ov011_021265fc(BtlEnm010RG* data) {
    s32   i;
    void* func;

    i    = 0;
    func = NULL;
    switch (data->unk_080) {
        case 0:
            i = data_ov011_0212c18c[(u32)data->unk_1F4 % 5];
            break;
        case 1:
            i = data_ov011_0212c1a0[(u32)data->unk_1F4 % 6];
            break;
        case 2:
            i = data_ov011_0212c178[(u32)data->unk_1F4 % 5];
            break;
        case 3:
            i = data_ov011_0212c1b8[(u32)data->unk_1F4 % 7];
            break;
    }
    data->unk_1F4 = data->unk_1F4 + 1;
    switch (i) {
        case 0:
            func = func_ov011_02126b2c;
            break;
        case 1:
            func          = func_ov011_02126bf8;
            data->unk_1F6 = (data->unk_1F6 & ~1) | 1;
            data->unk_1F6 = data->unk_1F6 & ~2;
            break;
        case 2:
            func          = func_ov011_02126bf8;
            data->unk_1F6 = data->unk_1F6 & ~1;
            data->unk_1F6 = data->unk_1F6 | 2;
            break;
        case 3:
            func          = func_ov011_02126bf8;
            data->unk_1F6 = (data->unk_1F6 & ~1) | 1;
            data->unk_1F6 = data->unk_1F6 | 2;
            break;
        case 4:
            func = func_ov011_02126e80;
            break;
        case 5:
            func = func_ov011_02126fb0;
            break;
        case 7:
            func = func_ov011_02127240;
            break;
        case 8:
            func = func_ov011_02127370;
            break;
        case 9:
            func = func_ov011_02127474;
            break;
    }
    func_ov011_021265d4(data, func);
}

void func_ov011_021267f4(BtlEnm010RG* data) {
    if (func_ov003_020c5bfc() != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

void func_ov011_0212681c(BtlEnm010RG* data) {
    if (data->actor.isFlipped == 0) {
        func_ov011_021265d4(data, func_ov011_02126aec);
        return;
    }
    if (data->unk_1C0 == 0) {
        func_ov011_02125750(1, &data->sprite, 0);
        CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
        data->unk_1C2 = func_ov003_020c42ec(data);
    }
    if (data->unk_1FC == -1 && data->unk_1C0 >= data->unk_1C2) {
        func_ov011_021265fc(data);
        return;
    }
    data->unk_1C0 = data->unk_1C0 + 1;
}

void func_ov011_021268c4(BtlEnm010RG* data) {
    CombatSprite* sp;
    s32           t;

    switch (data->unk_1C4) {
        case 0:
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                sp = &data->sprite;
                func_ov011_02125750(1, sp, 1);
                CombatSprite_SetAnimFromTable(sp, 0, 0);
                data->unk_1DC = (func_ov003_020cb744(1) >> 1) - 0x40000;
                data->unk_1E0 = func_ov003_020cb7a4(1) >> 1;
                data->unk_1E4 = 0;
                func_ov011_02127bf0(data, 0x1800);
                func_ov003_020c4ab4(data, data->unk_1D0 >= 0 ? 1 : 0);
            }
            sp = &data->sprite;
            t  = sp->sprite.cellIndex;
            if (t == 1 && sp->sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D6, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1D4 = 0;
            data->unk_1D0 = 0;
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

void func_ov011_02126a04(BtlEnm010RG* data) {
    CombatSprite* sp;
    s32           v;

    sp = &data->sprite;
    if (data->unk_1C0 == 0) {
        func_ov011_02125750(1, sp, 6);
        CombatSprite_SetAnimFromTable(sp, 0, 1);
        func_ov003_020c4c9c(data);
        v                      = 0 - 0x40000;
        data->actor.position.z = v;
        if (data->actor.isFlipped == 0) {
            data->unk_1D0 = v + 0x38000;
        } else {
            data->unk_1D0 = 0x8000;
        }
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_02087f00(0x1E3, func_ov003_020843b0(1, data->actor.position.x));
    }
    if (sp->sprite.cellIndex == 3 && sp->sprite.frameTimer == 1) {
        data->unk_1D8 = 0x2800;
    }
    if (data->actor.position.z < 0) {
        return;
    }
    data->unk_1D8 = 0;
    data->unk_1D0 = 0;
    func_ov003_020cb520(data, 1);
    func_ov003_020cb594(data, 1);
    func_ov011_021265d4(data, func_ov011_0212681c);
}

void func_ov011_02126aec(BtlEnm010RG* data) {
    s32 r;

    r             = func_ov011_02127758(data, data->unk_1C0);
    data->unk_1C0 = data->unk_1C0 + 1;
    if (r != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

void func_ov011_02126b2c(BtlEnm010RG* data) {
    s32 r;

    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x1E;
                data->unk_1FA = 0xBD;
            }
            r             = func_ov011_021277c8(data, (s16*)&data->unk_1F8, &data->unk_1FA, data->unk_1C0);
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1F8 == 0 && data->unk_1FA == 0xBD) {
                data->unk_1FC = func_ov011_02127c84(data);
            }
            if (r != 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

void func_ov011_02126bf8(BtlEnm010RG* data) {
    s32 bit1;
    s32 n;
    s32 r;
    s32 slot;
    s32 count;
    s32 i;
    s32 flags[2];
    s32 bias;
    s32 angle;

    bit1  = (data->unk_1F6 << 30) >> 31;
    count = data_ov011_0212c168[data->unk_080];
    if (bit1 != 0 && ((data->unk_1F6 << 31) >> 31) != 0) {
        count = 2;
    }
    flags[0] = bit1;
    flags[1] = (data->unk_1F6 << 31) >> 31;
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x14;
                data->unk_1FA = 0x1E;
            }
            r             = func_ov011_021277c8(data, (s16*)&data->unk_1F8, &data->unk_1FA, data->unk_1C0);
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1F8 == 0 && data->unk_1FA % 5 == 0) {
                slot = (0x1E - data->unk_1FA) / 5;
                if (slot == 0) {
                    if (data->actor.isFlipped == 0) {
                        data->unk_1D0 = 0x2000;
                        data->unk_1E8 = 5 - 0x338;
                    } else {
                        data->unk_1D0 = 0 - 0x2000;
                        data->unk_1E8 = 0x338 - 5;
                    }
                }
                for (i = 0; i < 2; i++) {
                    if (flags[i] != 0 && slot < count) {
                        if (data->actor.isFlipped == 0) {
                            if (i == 0) {
                                angle = 0x8000;
                            }
                            bias = 0 - 0x40000;
                            if (i != 0) {
                                angle = 0x8C00;
                            }
                        } else {
                            if (i == 0) {
                                angle = 0;
                            }
                            bias = 0x40000;
                            if (i != 0) {
                                angle = 0xF400;
                            }
                        }
                        data->unk_1FC = func_ov011_02127f6c(data, bias, 0, (0 - 0x40000) + 0x30000, 1,
                                                            (u16)(angle + data_ov011_0212c13c[slot].unk_00), 0, 0x333,
                                                            data_ov011_0212c13c[slot].unk_02);
                    }
                }
            }
            if (data->unk_1D0 > 0) {
                if (data->unk_1D0 + data->unk_1E8 < 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            } else if (data->unk_1D0 < 0) {
                if (data->unk_1D0 + data->unk_1E8 > 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            }
            if (r != 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_021268c4);
            return;
    }
}

void func_ov011_02126e80(BtlEnm010RG* data) {
    s32 bias;

    bias = (data->actor.isFlipped == 0) ? 0 - 0x60000 : 0x60000;
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(1, &data->sprite, 3);
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 1);
                func_ov003_02087f00(0x1D7, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 6 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D8, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex >= 5 && data->sprite.sprite.cellIndex <= 7) {
                func_ov003_020c5b2c(0x5D, (s32)(u32)data, data->actor.position.x + bias, data->actor.position.y,
                                    data->actor.position.z);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

void func_ov011_02126fb0(BtlEnm010RG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(1, &data->sprite, 9);
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
                data->unk_1FC = func_ov011_02125b98(data, 0);
                data->unk_1C2 = 0x4B;
            }
            if (data->unk_1C0 >= 0x28) {
                s32 y = data->unk_1C0 - 0x28;

                if (y % 15 == 0) {
                    s32 q = y / 15;

                    if (q > 0 && q < data_ov011_0212c148[data->unk_080]) {
                        data->unk_1FC = func_ov011_02125b98(data, q);
                    }
                }
            }
            if (data->unk_1C0 >= 0x3C) {
                s32 y = data->unk_1C0 - 0x3C;

                if (y % 15 == 0) {
                    s32 q = y / 15;

                    if (q < data_ov011_0212c148[data->unk_080]) {
                        data->unk_1D0 = (data->actor.isFlipped == 0) ? 0x4000 : 0 - 0x4000;
                        data->unk_1E8 = 0;
                    }
                    if (q == data_ov011_0212c148[data->unk_080] - 1) {
                        CombatSprite_SetAnimFromTable(&data->sprite, 3, 1);
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
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                func_ov011_021265d4(data, func_ov011_021268c4);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

void func_ov011_02127240(BtlEnm010RG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(1, &data->sprite, 8);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
                func_ov003_02087f00(0x1E0, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->unk_1C0 == 0x1C) {
                data->unk_1FC = func_ov011_02128c44(data);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 2;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

void func_ov011_02127370(BtlEnm010RG* data) {
    s32 bias;

    bias = (data->actor.isFlipped == 0) ? 0 - 0x80000 : 0x80000;
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(1, &data->sprite, 4);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
                func_ov003_02087f00(0x1DF, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex >= 3 && data->sprite.sprite.cellIndex <= 4) {
                func_ov003_020c5b2c(0x60, (s32)(u32)data, data->actor.position.x + bias, data->actor.position.y,
                                    data->actor.position.z);
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

void func_ov011_02127474(BtlEnm010RG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(1, &data->sprite, 5);
                CombatSprite_SetAnimFromTable(&data->sprite, 0, 1);
                func_ov003_020cb520(data, 0);
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov003_02087f00(0x1E2, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&data->sprite, 1, 0);
                data->unk_1DC = func_ov003_020cb744(1) + 0xC0000;
                data->unk_1E0 = data->actor.position.y;
                data->unk_1E4 = 0 - 0x20000;
                func_ov011_02127bf0(data, 0x8000);
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov003_02087f00(0x1E3, func_ov003_020843b0(1, data->actor.position.x));
            }
            func_ov003_020c5b2c(0x61, (s32)(u32)data, data->actor.position.x, data->actor.position.y, data->actor.position.z);
            if (func_ov011_02127c4c(data) >= 0x8000) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_02126a04);
            return;
    }
}

void func_ov011_02127628(BtlEnm010RG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_020cb520(data, 1);
        func_ov011_02125750(1, &data->sprite, 0);
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
    }
    if (func_ov003_020c62c4(data, 1) != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

void func_ov011_02127698(BtlEnm010RG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(1, &data->sprite, 0);
    }
    if (func_ov003_020c703c(data) == 0) {
        data->unk_1CC = 0;
    }
}

void func_ov011_021276e0(BtlEnm010RG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(1, &data->sprite, 0);
        func_ov003_020cb520(data, 1);
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
    }
    if (func_ov003_020c72b4(data, 0, 4) != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

s32 func_ov011_02127758(void* p, s32 arg1) {
    BtlEnm010RG*  data;
    CombatSprite* sp;

    data = (BtlEnm010RG*)p;
    sp   = &data->sprite;
    if (arg1 == 0) {
        func_ov011_02125750(1, sp, 3);
        CombatSprite_SetAnimFromTable(sp, 0, 1);
    }
    if (SpriteMgr_IsAnimationFinished(&sp->sprite) == 0) {
        return 1;
    }
    func_ov011_02125750(1, sp, 0);
    CombatSprite_SetAnimFromTable(sp, 0, 0);
    func_ov003_020c4c9c(data);
    return 0;
}

s32 func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3) {
    BtlEnm010RG*  data;
    CombatSprite* sp;

    data = (BtlEnm010RG*)p;
    sp   = &data->sprite;
    if (arg3 == 0) {
        func_ov011_02125750(1, sp, 2);
        CombatSprite_SetAnimFromTable(sp, 0, 1);
    }
    switch (sp->animTableIndex) {
        case 0:
            if (sp->sprite.cellIndex == 4 && sp->sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D3, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&sp->sprite) != 0) {
                *arg1 = *arg1 - 1;
                if (*arg1 <= 0) {
                    CombatSprite_SetAnimFromTable(sp, 1, 0);
                }
            }
            break;
        case 1:
            *arg2 = *arg2 - 1;
            if (*arg2 <= 0) {
                CombatSprite_SetAnimFromTable(sp, 3, 1);
            }
            break;
        case 3:
            if (SpriteMgr_IsAnimationFinished(&sp->sprite) != 0) {
                return 0;
            }
            break;
    }
    return 1;
}

void func_ov011_021278d4(void* p, s32 arg1, s32 arg2, s32 index) {
    BtlEnm010RGEntry t;

    t = data_ov011_0212c158;
    t.func[index](p, arg1, arg2);
}

s32 func_ov011_0212791c(void* arg0, Task* arg1, s32 arg2) {
    BtlEnm010RG* data;
    CombatActor* t;
    s32          v;

    data = arg1->data;
    t    = data_ov003_020e71b8->unk3D89C;
    MI_CpuSet(data, 0, 0x208);
    t->position.x                           = (func_ov003_020cb744(1) >> 1) + 0x40000;
    t->position.y                           = func_ov003_020cb7a4(1) >> 1;
    t->position.z                           = 0;
    data_ov003_020e71b8->unk3D878           = data_ov003_020e71b8->unk3D878 | 0x40000000;
    data_ov003_020e71b8->unk3D7C0[1].unk_20 = (func_ov003_020cb744(1) >> 1) + 0x40000;
    data_ov003_020e71b8->unk3D7C0[1].unk_24 = func_ov003_020cb7a4(1) >> 1;
    func_ov003_020c3efc(data, (void*)(u32)arg2);
    func_ov003_020c44ac(data);
    data->unk_1CC = 1;
    v             = data->unk_1CC - 2;
    data->unk_1FC = v;
    data->unk_200 = v;
    func_ov011_021265d4(data, func_ov011_021267f4);
    data->unk_1AC          = (func_ov003_020cb744(1) >> 1) - 0x40000;
    data->actor.position.x = data->unk_1AC;
    data->unk_1B0          = func_ov003_020cb7a4(1) >> 1;
    data->actor.position.y = data->unk_1B0;
    data->unk_1B4          = 0;
    data->actor.position.z = 0;
    func_ov003_020c4b1c(data);
    data->unk_1F4 = 0;
    data->actor.flags |= 1 << 30;
    return 1;
}

s32 func_ov011_02127a64(void* arg0, Task* arg1) {
    BtlEnm010RG* data;

    data = arg1->data;
    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 2:
            func_ov011_021265d4(data, func_ov011_02127628);
            break;
        case 3:
            if (data->unk_18C & 0x10) {
                data->unk_18C = data->unk_18C | 0x20;
            } else {
                func_ov011_021265d4(data, func_ov011_02127698);
            }
            break;
        case 6:
            func_ov011_021265d4(data, func_ov011_021276e0);
            break;
    }
    EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, (u32*)&data->unk_1FC);
    if (data->unk_200 != -1) {
        data->unk_204 = data->unk_204 + 1;
    }
    if (data->unk_1C8 != NULL) {
        data->unk_1C8(data);
    }
    data->actor.position.x = data->actor.position.x + data->unk_1D0;
    data->actor.position.y = data->actor.position.y + data->unk_1D4;
    data->actor.position.z = data->actor.position.z + data->unk_1D8;
    data->unk_1D0          = data->unk_1D0 + data->unk_1E8;
    data->unk_1D4          = data->unk_1D4 + data->unk_1EC;
    data->unk_1D8          = data->unk_1D8 + data->unk_1F0;
    func_ov003_020c4628(data);
    return data->unk_1CC;
}

s32 func_ov011_02127b98(void* arg0, Task* arg1) {
    BtlEnm010RG* data;

    data = arg1->data;
    if (data->unk_200 == -1 || data->unk_204 == 0) {
        func_ov003_020c4878(data);
    } else {
        func_ov003_020c4748(data);
        func_ov003_0208810c(&data->unk_0E4, data);
    }
    return 1;
}

s32 func_ov011_02127bdc(void* arg0, Task* arg1) {
    func_ov003_020c48fc(arg1->data);
    return 1;
}

s32 func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1) {
    s32 r;

    r = func_ov003_020cb910(&data->unk_1D0, &data->unk_1D4, &data->unk_1D8, data->actor.position.x, data->actor.position.y,
                            data->actor.position.z, data->unk_1DC, data->unk_1E0, data->unk_1E4, arg1);
    data->unk_1C2 = r;
    return r;
}

s32 func_ov011_02127c4c(BtlEnm010RG* data) {
    func_ov003_020cba54(data->actor.position.x, data->actor.position.y, data->actor.position.z, data->unk_1DC, data->unk_1E0,
                        data->unk_1E4);
}
