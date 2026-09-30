#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Noise/Private/BtlEnm006.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

s16                       func_ov010_02126830(BtlEnm006* data, void* arg1);
extern s32                func_ov010_021269d0(TaskPool*, Task*, void*, s32);
extern s32                func_ov010_02126a20(BtlEnm006*, void*);
extern s32                func_ov010_02126a94(BtlEnm006*);
extern void               func_ov003_020c4878(void*);
extern void               func_ov003_020c48fc(void*);
extern s32                func_ov003_020c703c(void*);
extern void               func_ov003_020c427c(void*);
extern s32                func_ov003_020cba54(s32, s32, s32, s32, s32, s32);
extern s32                func_ov003_020c5bfc(void*);
extern s32                func_ov003_020c62c4(void*, s32);
extern s32                func_ov003_020c72b4(void*, s32, s32);
extern s32                func_ov003_020c4e0c(void*);
extern s32                func_ov003_020cc354(void*);
extern s32                func_ov003_020cc38c(void*, s32, s32, s32, s32, s32, s32);
extern s32                func_ov003_020c3efc(void*, void*);
extern void               func_ov003_02084694(void*, s32);
extern s32                func_ov003_020cb910(void*, void*, s32, s32, s32, s32, s32, s32, s32, void*);
extern s16                func_ov003_020843b0(s32, s32);
extern void               CombatSprite_SetPaletteSource(CombatSprite*, s32);
extern s32                func_ov003_020c4ab4(BtlEnm006*, s32);
extern s32                func_ov003_020c6230(void*);
extern s32                func_ov003_020c4c1c(void*);
extern void               func_ov003_020c4ee0(void*);
extern s32                func_ov003_020c5b2c(u16, void*, s32, s32, s32);
extern void               func_ov003_020cb578(BtlEnm006*, s32);
extern s32                func_ov003_020cba2c(s32, s32, s32, s32);
extern s32                func_ov003_020cb764(s32);
extern s32                func_ov003_020cb784(s32);
extern void               func_ov003_020c4c9c(BtlEnm006*);
extern s32                func_ov003_020cb744(s32);
extern CombatEnemyParams* func_ov003_020c3c88(BtlEnm006*);
extern s32                func_ov003_020c42ec(BtlEnm006*);
extern s32                func_ov003_020c4348(BtlEnm006*);

/// One corner of the hexagon the RG patrols, relative to the screen centre.
typedef struct Enm006Waypoint {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
    /* 0x08 */ u16 anim;   // movement animation for the leg that ends here
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ s32 unk_0C; // facing for that leg
} Enm006Waypoint;          // Size: 0x10

extern void func_ov010_021259e8(BtlEnm006*);
extern void func_ov010_02125c80(BtlEnm006*);
extern void func_ov010_02125b28(BtlEnm006*);
extern s32  func_ov010_021265ac(BtlEnm006*, s32);
extern s32  func_ov003_020cb7a4(s32);
extern void func_ov003_020c44ac(BtlEnm006*);
extern void func_ov003_020c4b1c(BtlEnm006*);
extern void func_ov003_020c4628(BtlEnm006*);
extern s32  func_ov003_020c3bf0(BtlEnm006*);
extern void func_ov010_02125de4(BtlEnm006*);
extern char data_ov010_021293a4[20];

const TaskHandle Tsk_BtlEnm006_RG = {data_ov010_021293a4, func_ov010_021269d0, 0x1FC};

/// Palette per variant (`unk_080`) and damage level (`unk_1E8`).
const u16 data_ov010_02129238[3][4] = {
    {19, 20, 21, 22},
    {19, 20, 21, 22},
    {19, 20, 21, 22},
};

const Enm006Waypoint data_ov010_02129250[6] = {
    { 0x60000,        0, 2, 0, 1},
    { 0x40000, -0x18000, 2, 0, 0},
    {-0x40000, -0x18000, 0, 0, 0},
    {-0x60000,        0, 1, 0, 0},
    {-0x40000,  0x18000, 1, 0, 1},
    { 0x40000,  0x18000, 0, 0, 1},
};

char data_ov010_021293a4[20] = "Tsk_BtlEnm006_RG";

void func_ov010_02125910(BtlEnm006* data, void* arg1) {
    func_ov003_020c427c(data);
    data->unk_1C8 = arg1;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

void func_ov010_02125938(BtlEnm006* data) {
    if (func_ov003_020c4e0c(data) != 0) {
        if (RNG_Next(0x64) < 0x4B) {
            func_ov010_02125910(data, (void*)func_ov010_02125c80);
        } else {
            func_ov010_02125910(data, (void*)func_ov010_02125de4);
        }
    } else {
        func_ov010_02125910(data, (void*)func_ov010_021259e8);
    }
}

void func_ov010_02125998(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_188 = data->twin->unk_188;
    }
    if (func_ov003_020c5bfc(data) != 0) {
        return;
    }
    func_ov010_02125910(data, (void*)func_ov010_021259e8);
}

void func_ov010_021259e8(BtlEnm006* data) {
    s32 arrived;
    if (data->unk_188[1] != 0) {
        data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
        func_ov010_02125910(data, (void*)func_ov010_02125b28);
        return;
    }
    if (data->unk_1C0 == 0) {
        CombatEnemyParams* g = func_ov003_020c3c88(data);
        data->unk_19E        = ((-(data->unk_1E8 << 11) / 3 + 0x1000) * g->unk_22) >> 12;
        data->unk_1A2        = ((data->unk_1E8 << 11) / 3 + 0x1000) * g->unk_26 >> 12;
        data->unk_1C2        = func_ov003_020c42ec(data);
    }
    arrived = func_ov010_021265ac(data, data->unk_1C0);
    if (data->unk_1C0 >= data->unk_1C2 && arrived != 0 && data->actor.position.y == func_ov003_020cb7a4(1) >> 1) {
        if (func_ov003_020c4348(data) != 0) {
            func_ov010_02125938(data);
            return;
        }
        data->unk_1C0 = 1;
        data->unk_1C2 = func_ov003_020c42ec(data);
        return;
    }
    data->unk_1C0 = data->unk_1C0 + 1;
}

void func_ov010_02125b28(BtlEnm006* data) {
    CombatSprite* cs = &data->sprite;
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1C0++;
                CombatSprite_SetAnimFromTable(cs, 3, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
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
                data->unk_1C2 = 1;
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0++;
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                data->unk_1C0++;
                data->unk_18C &= ~1;
                data->actor.flags &= ~0x20;
                CombatSprite_SetAnimFromTable(cs, 4, 1);
                func_ov003_020cb578(data, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            func_ov010_02125910(data, (void*)func_ov010_021259e8);
            return;
    }
}

void func_ov010_02125c80(BtlEnm006* data) {
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
                CombatSprite_SetAnimFromTable(&data->sprite, 0xC, 0);
                if (data->actor.position.x < func_ov003_020cb744(1) >> 1) {
                    data->unk_1D0 = (func_ov003_020cb744(1) >> 1) + 0x60000;
                } else {
                    data->unk_1D0 = (func_ov003_020cb744(1) >> 1) - 0x60000;
                }
                data->unk_1D4 = data->actor.position.y;
                data->unk_1D8 = data->actor.position.z;
                data->unk_1C2 = func_ov010_02126830(data, (void*)0x4000);
                func_ov003_020c4c1c(data);
            }
            if (data->sprite.sprite.cellIndex == 1 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CF, func_ov003_020843b0(1, data->actor.position.x));
            }
            func_ov003_020c5b2c(0x51, data, data->actor.position.x, data->actor.position.y, data->actor.position.z);
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            func_ov010_02125910(data, (void*)func_ov010_021259e8);
            func_ov003_020c4ee0(data);
            return;
    }
}

void func_ov010_02125de4(BtlEnm006* data) {
    CombatSprite* cs = &data->sprite;
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                if ((u32)RNG_Next(0x64) < 0x1E) {
                    data->ext.rg.unk_1F4 = (data->ext.rg.unk_1F4 & ~3) | 1;
                } else {
                    data->ext.rg.unk_1F4 = data->ext.rg.unk_1F4 & ~3;
                }
                data->ext.rg.unk_1F8 = -1;
            }
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(cs, 3, 1);
                func_ov003_020c4c1c(data);
                func_ov003_02087f00(0x1CC, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                data->unk_18C |= 1;
                data->actor.flags |= 0x20;
                data->unk_18C |= 1;
                func_ov003_020cb578(data, 0);
                s32 v = (u32)((s32)data->ext.rg.unk_1F4 << 30) >> 30;
                switch (v) {
                    case 0:
                        data->unk_1C2 = 0x3C;
                        break;
                    case 1:
                        data->unk_1C2 = 0x78;
                        break;
                    case 2:
                        data->unk_1C2 = 0x1E;
                        break;
                }
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 3;
            return;
        case 3:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(cs, 0xD, 1);
                data->unk_18C &= ~1;
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = data_ov003_020e71b8->unk3D7C0[1].unk_20 + 0x10000;
                } else {
                    data->actor.position.x = data_ov003_020e71b8->unk3D7C0[1].unk_20 - 0x10000;
                }
                data->actor.position.y = data_ov003_020e71b8->unk3D7C0[1].unk_24;
                data->actor.position.z = 0;
                if (data->actor.isFlipped == 1) {
                    data->actor.position.x = data->actor.position.x - 0xB000;
                } else {
                    data->actor.position.x = data->actor.position.x + 0xB000;
                }
            }
            if (data->sprite.sprite.cellIndex == 0xF && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CD, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 4;
            return;
        case 4:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                CombatSprite_SetAnimFromTable(cs, 0xE, 1);
                data->actor.flags &= ~0x20;
                func_ov003_020cb578(data, 1);
                func_ov003_020c4c9c(data);
                func_ov003_02087f00(0x1C9, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 6 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CA, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 9 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1CB, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (data->sprite.sprite.cellIndex == 4 && data->sprite.sprite.frameTimer == 1) {
                void* t = data_ov003_020e71b8->unk3D89C;
                if (func_ov010_021268c4(data, t) != 0) {
                    data->ext.rg.unk_1F8 = func_ov010_021270a8(data, t);
                }
            }
            if (data->sprite.sprite.cellIndex < 9) {
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = data->actor.position.x + 0x800;
                } else {
                    data->actor.position.x = data->actor.position.x - 0x800;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            if ((u32)((s32)data->ext.rg.unk_1F4 << 30) >> 30 == 1 && data->ext.rg.unk_1F8 == -1) {
                data->ext.rg.unk_1F4 = (data->ext.rg.unk_1F4 & ~3) | 2;
                data->unk_1C4        = 2;
                return;
            }
            data->unk_1C4 = 5;
            return;
        case 5:
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
            data->unk_1C4 = 6;
            return;
        case 6:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                data->unk_18C &= ~1;
                data->actor.flags &= ~0x20;
                CombatSprite_SetAnimFromTable(cs, 4, 1);
                func_ov003_020cb578(data, 1);
                if (data->actor.isFlipped == 0) {
                    data->actor.position.x = func_ov003_020cb764(1) + 0x60000;
                } else {
                    data->actor.position.x = func_ov003_020cb764(1) - 0x60000;
                }
                data->actor.position.y = func_ov003_020cb784(1);
                data->actor.position.z = 0;
                func_ov003_020c4b1c(data);
                func_ov003_02087f00(0x1CC, func_ov003_020843b0(1, data->actor.position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&cs->sprite) == 0) {
                return;
            }
            func_ov010_02125910(data, (void*)func_ov010_021259e8);
            func_ov003_020c4ee0(data);
            return;
    }
}

void func_ov010_0212636c(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
    }
    if (func_ov003_020c62c4(data, 6) != 0) {
        return;
    }
    func_ov010_02125910(data, (void*)func_ov010_021259e8);
}

void func_ov010_021263c4(BtlEnm006* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0++;
        data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
    }
    if (func_ov003_020c72b4(data, 0, 9) != 0) {
        return;
    }
    func_ov010_02125910(data, (void*)func_ov010_021259e8);
}

s32 func_ov010_02126420(BtlEnm006* data) {
    s32 result = func_ov003_020c703c(data);
    if (result == 0) {
        data->unk_1CC = result = 0;
    }
    return result;
}

s32 func_ov010_0212643c(BtlEnm006* data) {
    s32 bestD = 0x7FFFFFFF;
    s32 best  = 0x7FFFFFFF - 0x80000000;
    s32 cx    = func_ov003_020cb744(1);
    s32 cy    = func_ov003_020cb7a4(1);
    s32 i;

    for (i = 0; i < 6; i++) {
        if (data->actor.isFlipped == 0 && data_ov010_02129250[i].x + (cx >> 1) > data->actor.position.x) {
            continue;
        }
        if (data->actor.isFlipped == 1 && data_ov010_02129250[i].x + (cx >> 1) < data->actor.position.x) {
            continue;
        }
        s32 px = data_ov010_02129250[i].x;
        s32 py = data_ov010_02129250[i].y;
        s32 d  = func_ov003_020cba2c(data->actor.position.x, data->actor.position.y, px + (cx >> 1), py + (cy >> 1));
        if (d < bestD) {
            bestD = d;
            best  = i;
        }
    }
    if (best == 0x7FFFFFFF - 0x80000000) {
        best = (data->actor.position.x < (cx >> 1)) ? 3 : 0;
    }

    s32 next = best + 1;
    next %= 6;
    Vec toOwner;
    Vec edge;
    toOwner.x = data->actor.position.x - (data_ov010_02129250[best].x + (cx >> 1));
    toOwner.y = data->actor.position.y - (data_ov010_02129250[best].y + (cy >> 1));
    toOwner.z = 0;
    edge.x    = data_ov010_02129250[next].x - data_ov010_02129250[best].x;
    edge.y    = data_ov010_02129250[next].y - data_ov010_02129250[best].y;
    edge.z    = 0;
    if (Vec_DotProduct(&toOwner, &edge) >= 0) {
        data->ext.rg.unk_1F4 |= 4;
    } else {
        data->ext.rg.unk_1F4 &= ~4;
    }
    return best;
}

s32 func_ov010_021265ac(BtlEnm006* data, s32 arg1) {
    if (arg1 == 0) {
        data->unk_1F0 = 0;
        data->unk_1EC = func_ov010_0212643c(data);
        if (((u32)data->ext.rg.unk_1F4 << 29) >> 31 == 1) {
            data->unk_1EC = data->unk_1EC + 1;
        } else {
            data->unk_1EC = data->unk_1EC - 1;
        }
        data->unk_1EC += 6;
        data->unk_1EC %= 6;
    }
    if (data->unk_1F0 == 0) {
        u16 v;
        if (((u32)data->ext.rg.unk_1F4 << 29) >> 31 == 1) {
            data->unk_1EC = data->unk_1EC - 1;
        } else {
            data->unk_1EC = data->unk_1EC + 1;
        }
        data->unk_1EC += 6;
        data->unk_1EC %= 6;
        data->unk_1D0 = data_ov010_02129250[data->unk_1EC].x + (func_ov003_020cb744(1) >> 1);
        data->unk_1D4 = data_ov010_02129250[data->unk_1EC].y + (func_ov003_020cb7a4(1) >> 1);
        data->unk_1D8 = 0;
        if (arg1 == 0) {
            v = 0;
            if (data->unk_1D0 < data->actor.position.x) {
                arg1 = 0;
            } else {
                arg1 = 1;
            }
        } else {
            if (((u32)data->ext.rg.unk_1F4 << 29) >> 31 != 1) {
                goto reflected;
            }
            {
                s32 idx = (data->unk_1EC + 1) % 6;
                v       = data_ov010_02129250[idx].anim;
                arg1    = data_ov010_02129250[idx].unk_0C ^ 1;
                if (v == 2) {
                    v = 1;
                    goto restart;
                }
                if (v == 1) {
                    v = 2;
                }
            }
            goto restart;
        reflected: {
            s32 idx = data->unk_1EC;
            v       = data_ov010_02129250[idx].anim;
            arg1    = data_ov010_02129250[idx].unk_0C;
        }
        restart:;
        }
        CombatSprite_SetAnimFromTable(&data->sprite, v, 0);
        func_ov003_020c4ab4(data, arg1);
    }
    func_ov010_02126830(data, (void*)0x2000);
    if ((data->sprite.sprite.cellIndex - 1) % 4 == 0) {
        if (data->sprite.sprite.frameTimer == 1) {
            func_ov003_02087f00(0x1CE, func_ov003_020843b0(1, data->actor.position.x));
        }
    }
    if (func_ov010_0212688c(data) >= 0x2000) {
        goto bump;
    }
    data->unk_1AC          = data->actor.position.x;
    data->unk_1B0          = data->actor.position.y;
    data->unk_1B4          = data->actor.position.z;
    data->actor.position.x = data->unk_1D0;
    data->actor.position.y = data->unk_1D4;
    data->actor.position.z = data->unk_1D8;
    data->unk_1DC = data->unk_1E0 = data->unk_1E4 = 0;
    data->unk_1F0                                 = 0;
    return 1;
bump:
    data->unk_1F0 = data->unk_1F0 + 1;
    return 0;
}

s16 func_ov010_02126830(BtlEnm006* data, void* arg1) {
    return func_ov003_020cb910(&data->unk_1DC, &data->unk_1E0, &data->unk_1E4, data->actor.position.x, data->actor.position.y,
                               data->actor.position.z, data->unk_1D0, data->unk_1D4, data->unk_1D8, arg1);
}

s32 func_ov010_0212688c(BtlEnm006* data) {
    return func_ov003_020cba54(data->actor.position.x, data->actor.position.y, data->actor.position.z, data->unk_1D0,
                               data->unk_1D4, data->unk_1D8);
}

s32 func_ov010_021268c4(void* arg0, void* arg1) {
    BtlEnm006* data = arg0;
    if (func_ov003_020cc354(arg1) != 0) {
        return 0;
    }
    s32 flip = data->actor.isFlipped == 0 ? 0x8000 : -0x8000;
    return func_ov003_020cc38c(arg1, data->actor.position.x + flip, data->actor.position.y + 0x8000, data->actor.position.z,
                               0x10000, 0x10000, 0x20000);
}

void func_ov010_02126934(BtlEnm006* data, u16 arg1) {
    data->unk_1E8 = data->unk_1E8 + arg1;
    if (data->unk_1E8 >= 4) {
        data->unk_1E8 = 3;
    } else {
        CombatSprite_SetPaletteSource(&data->sprite, data_ov010_02129238[data->unk_080][data->unk_1E8]);
        data->sprite.basePaletteSource = data->sprite.sprite.unk3C;
        data->sprite.basePalette       = data->sprite.sprite.paletteData;
        data->actor.unk_08             = (s16)((((s32)(data->unk_1E8 << 12) / 3 + 0x1000) * 0x64) >> 12);
    }
    data->actor.flags |= 0x40000000;
}

s32 func_ov010_021269d0(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* data = task->data;
    switch (stage) {
        case 0:
            return func_ov010_02126a20(data, args);
        case 1:
            return func_ov010_02126a94(data);
        case 2:
            return func_ov010_02126c18(data);
        case 3:
            return func_ov010_02126c28(data);
    }
    return 1;
}

s32 func_ov010_02126a20(BtlEnm006* data, void* arg1) {
    MI_CpuSet(data, 0, 0x1FC);
    func_ov003_020c3efc(data, arg1);
    func_ov003_020c44ac(data);
    func_ov003_020c4b1c(data);
    data->actor.flags |= 0x80000000;
    func_ov010_02125910(data, (void*)func_ov010_02125998);
    data->unk_1CC = 1;
    data->unk_1DC = 0;
    data->unk_1E0 = 0;
    data->unk_1E4 = 0;
    data->unk_1E8 = 0;
    return 1;
}

s32 func_ov010_02126a94(BtlEnm006* data) {
    s32 flag = 0;
    if (data->unk_1C8 != (void*)func_ov010_02125998) {
        if (func_ov003_020c3bf0(data) != 0) {
            return 1;
        }
    }
    if (data->unk_1C8 == (void*)func_ov010_02125de4) {
        flag = 1;
    }
    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 1:
        case 2:
            if (flag == 0) {
                func_ov003_02084694(&data->unk_144, 0);
                func_ov010_02125910(data, (void*)func_ov010_0212636c);
            }
            break;
        case 3:
            if (data->unk_18C & 0x10) {
                data->unk_18C |= 0x20;
            } else {
                func_ov010_02125910(data, (void*)func_ov010_02126420);
            }
            break;
        case 6:
            data->actor.flags |= 1;
            if (data->actor.flags != 0) {
                break;
            }
            func_ov010_02125910(data, (void*)func_ov010_021263c4);
            break;
    }
    data->actor.position.x += data->unk_1DC;
    data->actor.position.y += data->unk_1E0;
    data->actor.position.z += data->unk_1E4;
    if (data->unk_1C8 != NULL) {
        ((void (*)(BtlEnm006*))data->unk_1C8)(data);
    }
    u16* p    = data->unk_188;
    u16  step = p[1];
    if (step != 0) {
        if ((data->unk_18C & 1) || data->sprite.animTableIndex == 0xD) {
            func_ov010_02126934(data, step);
            p[1] = 0;
        }
    }
    data->actor.flags |= 0x80000000;
    func_ov003_020c4628(data);
    return data->unk_1CC;
}

s32 func_ov010_02126c18(BtlEnm006* data) {
    func_ov003_020c4878(data);
    return 1;
}

s32 func_ov010_02126c28(BtlEnm006* data) {
    func_ov003_020c48fc(data);
    return 1;
}
