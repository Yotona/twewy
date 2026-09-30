#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Noise/Private/BtlEnm006.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

typedef struct Enm006SpriteAlt3 {
    u8             pad00[0xF6];
    /* 0xF6 */ u16 unk_F6;
} Enm006SpriteAlt3;

typedef struct Enm006SpriteAlt4 {
    u8             pad00[0xF6];
    /* 0xF6 */ u16 unk_F6;
    /* 0xF8 */ u16 unk_F8;
} Enm006SpriteAlt4;

void func_ov010_02127650(BtlEnm006* data);
void func_ov010_02127cc0(BtlEnm006* data);
s16  func_ov010_02128a6c(BtlEnm006* data, void* arg1);
typedef struct Enm006PickCtx {
    /* 0x00 */ s32        remaining; // counts down to the randomly chosen unit
    /* 0x04 */ BtlEnm006* picked;
} Enm006PickCtx;

BtlEnm006*                func_ov010_02128b48(void);
s32                       func_ov010_02128c3c(void* arg0, BtlEnm006* unit);
s32                       func_ov010_02128c6c(s32 arg0, BtlEnm006* unit, Enm006PickCtx* ctx);
extern s32                func_ov010_02128dbc(TaskPool*, Task*, void*, s32);
extern s32                func_ov010_02128e80(BtlEnm006*);
extern void               func_ov003_020c48b0(void*);
extern void               func_ov003_020c492c(void*);
extern void               func_ov003_020c427c(void*);
extern s32                func_ov003_020cba54(s32, s32, s32, s32, s32, s32);
extern s32                func_ov003_020cba14(s32, s32, s32, s32);
extern s32                func_ov003_020cb3c4(s32, s32);
extern s32                func_ov003_020c5bfc(void*);
extern s32                func_ov003_020c65cc(void*, s32);
extern s32                func_ov003_020c72b4(void*, s32, s32);
extern s32                func_ov003_020c7070(void*);
extern s32                func_ov003_020cc354(void*);
extern s32                func_ov003_020cc38c(void*, s32, s32, s32, s32, s32, s32);
extern s32                func_ov003_020c3efc(void*, void*);
extern void               func_ov003_020c4520(void*);
extern void               func_ov003_020c4b5c(void*);
extern void               func_ov003_020c4668(void*);
extern void               func_ov003_02084694(void*, s32);
extern s32                func_ov003_020cb910(void*, void*, s32, s32, s32, s32, s32, s32, s32, void*);
extern s32                func_ov003_020cb498(s32, s32, void*, void*);
extern s16                func_ov003_020843b0(s32, s32);
extern void               CombatSprite_SetPaletteSource(CombatSprite*, s32);
extern s32                func_ov003_020c4ab4(BtlEnm006*, s32);
extern s32                func_ov003_020c5b2c(u16, void*, s32, s32, s32);
extern s32                func_ov010_02128cbc(s32, BtlEnm006*, BtlEnm006*);
extern void               func_ov003_020c4c5c(BtlEnm006*);
extern void               func_ov003_020cb578(BtlEnm006*, s32);
extern s32                func_ov010_02128bcc(void*, void*);
extern s32                func_ov003_020cba2c(s32, s32, s32, s32);
extern s32                func_ov003_020cb764(s32);
extern void               func_ov003_020c4c9c(BtlEnm006*);
extern s32                func_ov003_020cb744(s32);
extern CombatEnemyParams* func_ov003_020c3c88(BtlEnm006*);
extern s32                func_ov003_020c42ec(BtlEnm006*);
extern s32                func_ov003_020c4348(BtlEnm006*);
extern s32                func_ov010_02128a08(BtlEnm006*, s32);
extern s32                func_ov003_020cc300(BtlEnm006*, s32);

typedef struct Enm006CmdTbl {
    u8            pad00[5];
    /* 0x05 */ u8 unk_05;
    u8            pad06[6];
} Enm006CmdTbl;

extern const Enm006CmdTbl* func_ov003_0208a114(u16);

extern void func_ov010_02127764(BtlEnm006*);
extern s32  func_ov003_020cb7a4(s32);
extern s32  func_ov003_020c3bf0(BtlEnm006*);
extern char data_ov010_021293e0[32];

const TaskHandle Tsk_BtlEnm006_UG = {data_ov010_021293e0, func_ov010_02128dbc, 0x200};

/// Palette per variant (`unk_080`) and damage level (`unk_1E8`).
const u16 data_ov010_021292f4[3][4] = {
    {19, 20, 21, 22},
    {19, 20, 21, 22},
    {19, 20, 21, 22},
};

char data_ov010_021293e0[32] = "Tsk_BtlEnm006_UG";

void func_ov010_02127460(BtlEnm006* data, void* arg1) {
    func_ov003_020c427c(data);
    data->unk_1C8 = arg1;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

void func_ov010_02127488(BtlEnm006* data) {
    if (data->unk_080 != 0) {
        if (RNG_Next(0x64) >= 0x5A) {
            goto big;
        }
    }
    if ((u32)RNG_Next(0x64) < (u32)func_ov010_02128b00()) {
        func_ov010_02127460(data, (void*)func_ov010_02127764);
    } else {
        func_ov010_02127460(data, (void*)func_ov010_02127650);
    }
    return;
big:
    func_ov010_02127460(data, (void*)func_ov010_02127cc0);
}

void func_ov010_02127500(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_188 = data->ext.ug.unk_1F6;
    }
    if (func_ov003_020c5bfc(data) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127764);
}

void func_ov010_02127550(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        CombatEnemyParams* g = func_ov003_020c3c88(data);
        data->unk_198        = ((-(data->unk_1E8 << 11) / 3 + 0x1000) * g->unk_1C) >> 12;
        data->unk_19C        = ((data->unk_1E8 << 11) / 3 + 0x1000) * g->unk_20 >> 12;
        data->unk_1C2        = func_ov003_020c42ec(data);
    }
    func_ov010_02128a08(data, data->unk_1C0);
    if (data->unk_1C0 < data->unk_1C2) {
        goto bump;
    }
    if (func_ov003_020c4348(data) != 0) {
        data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
        func_ov010_02127488(data);
        return;
    }
    data->unk_1C0 = 1;
    data->unk_1C2 = func_ov003_020c42ec(data);
    return;
bump:
    data->unk_1C0 = data->unk_1C0 + 1;
}

void func_ov010_02127650(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        CombatSprite_SetAnimFromTable(&data->sprite, 0xC, 0);
        s32 mid = func_ov003_020cb744(0);
        if (data->actor.position.x < mid >> 1) {
            data->unk_1D0 = data->actor.position.x + 0x80000;
        } else {
            data->unk_1D0 = data->actor.position.x - 0x80000;
        }
        data->unk_1D4 = data->actor.position.y;
        data->unk_1D8 = data->actor.position.z;
        data->unk_1C2 = func_ov010_02128a6c(data, (void*)0x4000);
        if (data->unk_1DC < 0) {
            func_ov003_020c4ab4(data, 0);
        } else {
            func_ov003_020c4ab4(data, 1);
        }
    }
    if (data->sprite.sprite.cellIndex == 1 && data->sprite.sprite.frameTimer == 1) {
        func_ov003_02087f00(0x1CF, func_ov003_020843b0(0, data->actor.position.x));
    }
    func_ov003_020c5b2c(0x4C, data, data->actor.position.x, data->actor.position.y, data->actor.position.z);
    if (data->unk_1C0 < data->unk_1C2) {
        data->unk_1C0 = data->unk_1C0 + 1;
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

void func_ov010_02127764(BtlEnm006* data) {
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 3, 1);
                func_ov003_020c4c5c(data);
                if (data->actor.flags & 0x40000000) {
                    data->ext.ug.unk_1F5 |= 2;
                } else {
                    data->ext.ug.unk_1F5 &= ~2;
                    data->actor.flags |= 0x40000000;
                }
                func_ov003_02087f00(0x1CC, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_18C |= 1;
                data->actor.flags |= 0x20;
                data->unk_18C |= 1;
                func_ov003_020cb578(data, 0);
                data->unk_1C2 = 0x3C;
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2: {
            if (data->unk_1C0 == 0) {
                data->unk_1C0    = data->unk_1C0 + 1;
                BtlEnm006* other = func_ov010_02128b48();
                if (other != NULL) {
                    if (other->actor.unk_00 == 1) {
                        data->ext.ug.unk_1F4 = 1;
                    } else {
                        data->ext.ug.unk_1F4 = 0;
                    }
                }
                CombatSprite_SetAnimFromTable(&data->sprite, 0xD, 1);
                data->unk_18C &= ~1;
                if (data->actor.position.x < other->actor.position.x) {
                    func_ov003_020c4ab4(data, 1);
                } else {
                    func_ov003_020c4ab4(data, 0);
                }
                data->actor.position.y = other->actor.position.y;
                data->actor.position.z = 0;
                if (data->actor.position.y < 0x20000) {
                    data->actor.position.y = 0x20000;
                }
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = other->actor.position.x + 0x1C000;
                } else {
                    data->actor.position.x = other->actor.position.x - 0x1C000;
                }
                if (func_ov003_020cc300(data, 0) == 0) {
                    func_ov003_020c4c9c(data);
                    if (data->actor.isFlipped == 0) {
                        data->actor.position.x = other->actor.position.x + 0x1C000;
                    } else {
                        data->actor.position.x = other->actor.position.x - 0x1C000;
                    }
                }
            }
            if (data->sprite.sprite.cellIndex == 0xF && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CD, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 3;
            return;
        }
        case 3:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 0xE, 1);
                data->actor.flags &= ~0x20;
                if (data->ext.ug.unk_1F4 == 0) {
                    func_ov003_020cb578(data, 1);
                }
                func_ov003_020c4c9c(data);
                func_ov003_02087f00(0x1C9, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 6 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CA, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 9 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CB, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 4 && data->sprite.sprite.frameTimer == 1) {
                void* t = data_ov003_020e71b8->unk3D898;
                if (func_ov010_02128bcc(data, t) != 0) {
                    func_ov010_021270a8(data, t);
                }
                data->ext.ug.unk_1F6[0] = func_ov003_020cb498(0, 0x3C, (void*)func_ov010_02128cbc, data);
                data->ext.ug.unk_1F6[1] = data->ext.ug.unk_1F6[0];
            }
            if (data->sprite.sprite.cellIndex < 9) {
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = data->actor.position.x + 0x800;
                } else {
                    data->actor.position.x = data->actor.position.x - 0x800;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 4;
            return;
        case 4:
            if (data->unk_1C0 == 0) {
                data->unk_18C |= 1;
                data->actor.flags |= 0x20;
                func_ov003_020cb578(data, 0);
                data->unk_1C2 = 0x3C;
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 5;
            return;
        case 5:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                data->unk_18C &= ~1;
                data->actor.flags &= ~0x20;
                CombatSprite_SetAnimFromTable(&data->sprite, 4, 1);
                func_ov003_020cb578(data, 1);
                func_ov003_020c4c5c(data);
                data->actor.position.x = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                data->actor.position.y = RNG_Next((func_ov003_020cb7a4(0) >> 12) + 1) << 12;
                data->actor.position.z = 0;
                func_ov003_02087f00(0x1CC, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            if (((u32)data->ext.ug.unk_1F5 << 30) >> 31 == 0) {
                data->actor.flags &= ~0x40000000;
            }
            func_ov010_02127460(data, (void*)func_ov010_02127550);
            return;
    }
}

void func_ov010_02127cc0(BtlEnm006* data) {
    s32 n;
    s32 v;
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 3, 1);
                func_ov003_02087f00(0x1CC, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_18C |= 1;
                data->actor.flags |= 0x20;
                data->unk_18C |= 1;
                func_ov003_020cb578(data, 0);
                data->unk_1C2 = 0x3C;
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 0xF, 1);
                data->unk_18C &= ~1;
                if (data->actor.position.x < ((BtlEnm006*)data_ov003_020e71b8->unk3D898)->actor.position.x) {
                    func_ov003_020c4ab4(data, 0);
                } else {
                    func_ov003_020c4ab4(data, 1);
                }
                data->actor.position.y = ((BtlEnm006*)data_ov003_020e71b8->unk3D898)->actor.position.y;
                data->actor.position.z = 0;
                if (data->actor.position.y < 0x20000) {
                    data->actor.position.y = 0x20000;
                }
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = ((BtlEnm006*)data_ov003_020e71b8->unk3D898)->actor.position.x - 0x60000;
                } else {
                    data->actor.position.x = ((BtlEnm006*)data_ov003_020e71b8->unk3D898)->actor.position.x + 0x60000;
                }
                if (func_ov003_020cc300(data, 0) == 0) {
                    func_ov003_020c4c9c(data);
                    if (data->actor.isFlipped == 0) {
                        data->actor.position.x = ((BtlEnm006*)data_ov003_020e71b8->unk3D898)->actor.position.x - 0x60000;
                    } else {
                        data->actor.position.x = ((BtlEnm006*)data_ov003_020e71b8->unk3D898)->actor.position.x + 0x60000;
                    }
                }
            }
            if (data->sprite.sprite.cellIndex == 0xC && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CD, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 3;
            return;
        case 3:
            if (data->unk_1C0 == 0) {
                CombatSprite_SetAnimFromTable(&data->sprite, 0x10, 1);
                data->unk_18C &= ~1;
                data->actor.flags &= ~0x20;
                func_ov003_020cb578(data, 1);
            }
            if (data->sprite.sprite.cellIndex == 0xF && data->sprite.sprite.frameTimer == 1) {
                if (data->unk_080 == 2) {
                    func_ov010_02126c38(data);
                }
                func_ov003_02087f00(0x1D1, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (data->unk_080 == 1) {
                v = 0xF;
                n = 0x4E;
            } else {
                if (data->unk_080 == 2) {
                    v = 0xE;
                    n = 0x4F;
                }
            }
            if (data->sprite.sprite.cellIndex == v && data->sprite.sprite.frameTimer == 1) {
                s32 t = func_ov003_0208a114(n)->unk_05;
                s32 x;
                if (data->actor.isFlipped == 0) {
                    x = data->actor.position.x - 0x48000 + (s32)(t != 0 ? (f32)(t * 0x1000) + 0.5f : (f32)(t * 0x1000) - 0.5f);
                } else {
                    x = data->actor.position.x + 0x48000 - (s32)(t != 0 ? (f32)(t * 0x1000) + 0.5f : (f32)(t * 0x1000) - 0.5f);
                }
                func_ov003_020c5b2c(n, data, x, data->actor.position.y, data->actor.position.z);
            }
            if (data->unk_1C0 == 0x19) {
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = data->actor.position.x + 0x48000;
                } else {
                    data->actor.position.x = data->actor.position.x - 0x48000;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 4;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 4:
            if (data->unk_1C0 == 0) {
                data->unk_18C |= 1;
                data->actor.flags |= 0x20;
                func_ov003_020cb578(data, 0);
                data->unk_1C2 = 0x3C;
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 5;
            return;
        case 5:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(&data->sprite, 4, 1);
                data->unk_18C &= ~1;
                data->actor.flags &= ~0x20;
                CombatSprite_SetAnimFromTable(&data->sprite, 4, 1);
                func_ov003_020cb578(data, 1);
                func_ov003_020c4c5c(data);
                data->actor.position.x = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                data->actor.position.y = RNG_Next((func_ov003_020cb7a4(0) >> 12) + 1) << 12;
                data->actor.position.z = 0;
                func_ov003_02087f00(0x1CC, func_ov003_020843b0(0, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) == 0) {
                return;
            }
            func_ov010_02127460(data, (void*)func_ov010_02127550);
            return;
    }
}

void func_ov010_021282b8(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->actor.zVelocity = data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
    }
    if (func_ov003_020c65cc(data, 6) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

void func_ov010_02128314(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_1E4         = 0;
        data->unk_1E0         = 0;
        data->unk_1DC         = 0;
        data->actor.zVelocity = 0;
        data->actor.xVelocity = (s32)((((s64)data->actor.xVelocity * 0x2800) + 0x800) >> 12);
        if (((u32)data->ext.ug.unk_1F5 << 30) >> 31 == 0) {
            data->actor.flags &= ~0x40000000;
        }
        data->actor.flags &= ~0x4000;
    }
    if (func_ov003_020c65cc(data, 9) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

void func_ov010_021283c0(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
        if (((u32)data->ext.ug.unk_1F5 << 30) >> 31 == 0) {
            data->actor.flags &= ~0x40000000;
        }
    }
    if (func_ov003_020c72b4(data, 0, 9) != 0) {
        return;
    }
    func_ov010_02127460(data, (void*)func_ov010_02127550);
}

void func_ov010_02128434(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
    }
    if (func_ov003_020c7070(data) == 0) {
        data->unk_1CC = 0;
    }
}

void func_ov010_0212847c(s32* outX, s32* outY, BtlEnm006* data, s32 index) {
    s32 i = index;
    s32 lo;
    s32 hi;
    s32 midLo;
    s32 midHi;
    s32 rad;

    if (i < 0) {
        s32 d = i - 5;
        i     = i + 10 * ((d < 0 ? -d : d) / 6);
    }
    i %= 10;

    s32 v = data->actor.unk_70;
    rad   = (s32)(v > 0 ? (f32)(v * 0x1000) + 0.5f : (f32)(v * 0x1000) - 0.5f);

    lo    = func_ov003_020cb744(0);
    hi    = func_ov003_020cb7a4(0);
    midLo = lo >> 1;
    midHi = hi >> 1;

    switch (i) {
        case 0:
            *outX = rad;
            *outY = midHi;
            return;
        case 1:
            *outX = rad + 0x30000;
            *outY = midHi - 0x20000;
            return;
        case 2:
            *outX = midLo - 0x30000;
            *outY = midHi - 0x20000;
            return;
        case 3:
            *outX = midLo + 0x30000;
            *outY = midHi + 0x20000;
            return;
        case 4:
            *outX = lo - 0x31000 - rad;
            *outY = midHi + 0x20000;
            return;
        case 5:
            *outX = lo - 0x1000 - rad;
            *outY = midHi;
            return;
        case 6:
            *outX = lo - 0x31000 - rad;
            *outY = midHi - 0x20000;
            return;
        case 7:
            *outX = midLo + 0x30000;
            *outY = midHi - 0x20000;
            return;
        case 8:
            *outX = midLo - 0x30000;
            *outY = midHi + 0x20000;
            return;
        case 9:
            *outX = rad + 0x30000;
            *outY = midHi + 0x20000;
            return;
        default:
            return;
    }
}

s32 func_ov010_02128624(BtlEnm006* data) {
    s32 bestD = 0x7FFFFFFF;
    s32 bestI = -1;
    s32 x;
    s32 y;
    s32 i;
    for (i = 0; i < 10; i++) {
        func_ov010_0212847c(&x, &y, data, i);
        s32 flip = data->actor.isFlipped;
        if (flip == 0) {
            if (x > data->actor.position.x) {
                continue;
            }
        }
        if (flip == 1) {
            if (x < data->actor.position.x) {
                continue;
            }
        }
        s32 d = func_ov003_020cba2c(data->actor.position.x, data->actor.position.y, x, y);
        if (d < bestD) {
            bestD = d;
            bestI = i;
        }
    }
    if (bestI == -1) {
        if (data->actor.position.x < func_ov003_020cb764(1)) {
            bestI = 0;
        } else {
            bestI = 5;
        }
    }
    return bestI;
}

s32 func_ov010_021286e8(BtlEnm006* data, s32 x1, s32 y1, s16* outHalf, s32* outWord) {
    u16 sector[4];
    u16 a;
    u16 b;
    s32 angle;
    s32 i;

    angle     = func_ov003_020cba14(data->actor.position.x, data->actor.position.y, x1, y1);
    a         = 0x182D;
    b         = 0x67D2;
    sector[0] = a;
    sector[1] = b;
    sector[2] = a + 0x8000;
    sector[3] = b + 0x8000;

    for (i = 0; i < 4; i++) {
        u32 d = (u16)(sector[i] - angle);
        if (d < 0xAAA || d > 0xF6BE) {
            break;
        }
    }

    switch (i) {
        case 0:
            *outHalf = 1;
            *outWord = 1;
            return 1;
        case 1:
            *outHalf = 1;
            *outWord = 0;
            return 0;
        case 2:
            *outHalf = 2;
            *outWord = 0;
            return 0;
        case 3:
            *outHalf = 2;
            *outWord = 1;
            return 1;
    }
    *outHalf = 0;
    s32 v    = data->actor.position.x;
    if (v < x1) {
        v        = 1;
        *outWord = 1;
    } else {
        *outWord = 0;
    }
    return v;
}

void func_ov010_02128820(BtlEnm006* data, s32 arg1) {
    s16 h;
    s32 o1a;
    s32 o1b;
    s32 o2a;
    s32 o2b;
    s32 w;

    if (arg1 == 0) {
        data->unk_1EC = func_ov010_02128624(data);
        data->unk_1F0 = 0;
    }
    if (data->unk_1F0 != 0) {
        goto tail;
    }

    func_ov010_0212847c(&o1a, &o1b, data, data->unk_1EC);
    func_ov010_0212847c(&o2a, &o2b, data, data->unk_1EC + 1);

    if (arg1 == 0) {
        Vec toOwner;
        Vec edge;
        toOwner.x = data->actor.position.x - o1a;
        toOwner.y = data->actor.position.y - o1b;
        toOwner.z = 0;
        edge.x    = o2a - o1a;
        edge.y    = o2b - o1b;
        edge.z    = 0;
        if (Vec_DotProduct(&toOwner, &edge) >= 0) {
            data->ext.ug.unk_1F5 = (data->ext.ug.unk_1F5 & ~1) | 1;
        } else {
            data->ext.ug.unk_1F5 &= ~1;
        }
    }

    data->unk_1D0 = o1a;
    data->unk_1D4 = o1b;
    data->unk_1D8 = 0;
    func_ov010_021286e8(data, o1a, o1b, &h, &w);
    CombatSprite_SetAnimFromTable(&data->sprite, h, 0);
    func_ov003_020c4ab4(data, w);
    data->unk_1F2 = func_ov010_02128a6c(data, (void*)0x2800);

    if ((u32)(data->ext.ug.unk_1F5 << 31) >> 31 == 1) {
        data->unk_1EC -= 1;
    } else {
        data->unk_1EC += 1;
    }
    data->unk_1EC += 10;
    data->unk_1EC %= 10;

tail:
    if (data->unk_1F0 < data->unk_1F2) {
        if (func_ov010_02128ac8(data) >= 0x2800) {
            goto bump;
        }
    }
    data->unk_1DC = data->unk_1E0 = data->unk_1E4 = data->unk_1F0 = 0;
    return;
bump:
    data->unk_1F0 = data->unk_1F0 + 1;
}

s32 func_ov010_02128a08(BtlEnm006* data, s32 arg1) {
    if (arg1 == 0) {
        data->unk_1F0 = 0;
    }
    func_ov010_02128820(data, arg1);
    if ((data->sprite.sprite.cellIndex - 1) % 4 == 0) {
        if (data->sprite.sprite.frameTimer == 1) {
            func_ov003_02087f00(0x1CE, func_ov003_020843b0(0, data->actor.position.x));
        }
    }
    return 1;
}

s16 func_ov010_02128a6c(BtlEnm006* data, void* arg1) {
    return func_ov003_020cb910(&data->unk_1DC, &data->unk_1E0, &data->unk_1E4, data->actor.position.x, data->actor.position.y,
                               data->actor.position.z, data->unk_1D0, data->unk_1D4, data->unk_1D8, arg1);
}

s32 func_ov010_02128ac8(BtlEnm006* data) {
    return func_ov003_020cba54(data->actor.position.x, data->actor.position.y, data->actor.position.z, data->unk_1D0,
                               data->unk_1D4, data->unk_1D8);
}

s32 func_ov010_02128b00(void) {
    s32 lo = func_ov003_020cb3c4(0, 5);
    s32 hi = func_ov003_020cb3c4(0, 0x3C);
    s32 t  = ((hi - lo) << 11) + 0x1000;
    return ((u32)FX_Divide(t * 0x64, t + 0x3000) << 4) >> 16;
}

BtlEnm006* func_ov010_02128b48(void) {
    s32           hits = func_ov003_020cb498(0, 0x3C, (void*)func_ov010_02128c3c, 0);
    Enm006PickCtx ctx;
    if (hits != 0 && RNG_Next(0x64) >= 0x14) {
        goto pick;
    }
    return data_ov003_020e71b8->unk3D898;
pick:
    ctx.remaining = RNG_Next(hits);
    func_ov003_020cb498(0, 0x3C, (void*)func_ov010_02128c6c, &ctx);
    return ctx.picked;
}

s32 func_ov010_02128bcc(void* arg0, void* arg1) {
    BtlEnm006* data = arg0;
    if (func_ov003_020cc354(arg1) != 0) {
        return 0;
    }
    s32 flip = data->actor.isFlipped == 0 ? 0x8000 : -0x8000;
    return func_ov003_020cc38c(arg1, data->actor.position.x + flip, data->actor.position.y + 0x8000, data->actor.position.z,
                               0x18000, 0x20000, 0x20000);
}

s32 func_ov010_02128c3c(void* arg0, BtlEnm006* unit) {
    s32 v = unit->unk_07C;
    if (v < -1) {
        goto zero;
    }
    if (v == 5) {
        goto zero;
    }
    if (v < 0x3C) {
        goto one;
    }
zero:
    return 0;
one:
    return 1;
}

s32 func_ov010_02128c6c(s32 arg0, BtlEnm006* unit, Enm006PickCtx* ctx) {
    s32 v = unit->unk_07C;
    if (v < -1) {
        goto zero;
    }
    if (v == 5) {
        goto zero;
    }
    if (v < 0x3C) {
        goto body;
    }
zero:
    return 0;
body:
    s32 c = ctx->remaining;
    if (c < 0) {
        return 0;
    }
    if (c == 0) {
        ctx->picked = unit;
    }
    ctx->remaining = ctx->remaining - 1;
    return 0;
}

s32 func_ov010_02128cbc(s32 arg0, BtlEnm006* unit, BtlEnm006* data) {
    if (unit->unk_07C == 5) {
        return 0;
    }
    if (unit->actor.flags & 4) {
        return 0;
    }
    if (func_ov010_02128bcc(data, unit) == 0) {
        goto zero;
    }
    func_ov010_02127110(data, unit);
    return arg0 == 0;
zero:
    return 0;
}

void func_ov010_02128d20(BtlEnm006* data, u16 arg1) {
    data->unk_1E8 = data->unk_1E8 + arg1;
    if (data->unk_1E8 >= 4) {
        data->unk_1E8 = 3;
    } else {
        CombatSprite_SetPaletteSource(&data->sprite, data_ov010_021292f4[data->unk_080][data->unk_1E8]);
        data->sprite.basePaletteSource = data->sprite.sprite.unk3C;
        data->sprite.basePalette       = data->sprite.sprite.paletteData;
        data->actor.unk_08             = (s16)((((s32)(data->unk_1E8 << 12) / 3 + 0x1000) * 0x64) >> 12);
    }
    data->actor.flags |= 0x40000000;
}

s32 func_ov010_02128dbc(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_02128e0c(data, args);
        case 1:
            return func_ov010_02128e80(data);
        case 2:
            return func_ov010_02128fe8(data);
        case 3:
            return func_ov010_02128ff8(data);
    }
    return 1;
}

s32 func_ov010_02128e0c(BtlEnm006* data, void* arg1) {
    MI_CpuSet(data, 0, 0x200);
    func_ov003_020c3efc(data, arg1);
    func_ov003_020c4520(data);
    func_ov003_020c4b5c(data);
    func_ov003_02084694(&data->unk_144, 1);
    func_ov010_02127460(data, (void*)func_ov010_02127500);
    data->unk_1CC = 1;
    data->unk_1DC = 0;
    data->unk_1E0 = 0;
    data->unk_1E4 = 0;
    data->unk_1E8 = 0;
    return 1;
}

s32 func_ov010_02128e80(BtlEnm006* data) {
    data->actor.zVelocity = 0;
    if (data->unk_1C8 != (void*)func_ov010_02127500) {
        if (func_ov003_020c3bf0(data) != 0) {
            return 1;
        }
    }
    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 1:
            func_ov010_02127460(data, (void*)func_ov010_021282b8);
            data->actor.zVelocity = 0;
            break;
        case 2:
            func_ov010_02127460(data, (void*)func_ov010_02128314);
            break;
        case 3:
            func_ov010_02127460(data, (void*)func_ov010_02128434);
            break;
        case 6:
            data->actor.flags |= 1;
            if (data->actor.flags != 0) {
                break;
            }
            func_ov010_02127460(data, (void*)func_ov010_021283c0);
            break;
    }
    data->actor.position.x += data->unk_1DC;
    data->actor.position.y += data->unk_1E0;
    data->actor.position.z += data->unk_1E4;
    if (data->unk_1C8 != NULL) {
        ((void (*)(BtlEnm006*))data->unk_1C8)(data);
    }
    u16 flag = data->ext.ug.unk_1F6[0];
    if (flag != 0) {
        if ((data->unk_18C & 1) || data->sprite.animTableIndex == 0xD || data->sprite.animTableIndex == 0xF) {
            func_ov010_02128d20(data, flag);
            data->ext.ug.unk_1F6[0] = 0;
        }
    }
    data->actor.flags |= 0x80000000;
    func_ov003_020c4668(data);
    return data->unk_1CC;
}

s32 func_ov010_02128fe8(BtlEnm006* data) {
    func_ov003_020c48b0(data);
    return 1;
}

s32 func_ov010_02128ff8(BtlEnm006* data) {
    func_ov003_020c492c(data);
    return 1;
}
