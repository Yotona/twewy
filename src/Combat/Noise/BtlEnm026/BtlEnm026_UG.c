#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm026.h"
#include "Engine/Core/System.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"

#include <nitro/mi/cpumem.h>

extern s32   func_ov003_020c37f8(void*);
extern void  func_ov003_020c4ab4(void*, s32);
extern void  func_ov003_020c4cc4(void*, s32);
extern void  func_ov003_02084694(void*, s32);
extern void  func_ov003_020c48b0(void*);
extern void  func_ov003_020c492c(void*);
extern s32   func_ov003_0208a114(u16);
extern s32   func_ov003_0208a164(s32, void*, s32, s32, s32);
extern void  func_ov003_020ccf1c(void*, s32);
extern s32   func_ov003_020c42ec(void*);
extern void  func_ov003_020c4b5c(void*);
extern void  func_ov003_020a4390(s32, s32);
extern void  func_ov003_020c4fc8(void*);
extern void  func_ov003_020ccec0(void*, s32);
extern void* func_ov003_0208495c(void*);
extern void* func_ov003_02084984(void*);
extern s16   data_0205e4e0[];

/// An element of the `unk3D8BC` list.
typedef struct Ov003Node3D8BC {
    /* 0x000 */ u32   unk_000;
    /* 0x004 */ u8    unk_004[0x44 - 0x04];
    /* 0x044 */ s16   unk_044;
    /* 0x046 */ s16   unk_046;
    /* 0x048 */ u8    unk_048[0x108 - 0x48];
    /* 0x108 */ void* unk_108; // list link
} Ov003Node3D8BC;

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void        func_ov015_0212736c(BtlEnm026*);
void        func_ov015_02127594(BtlEnm026*);
s32         func_ov015_021275fc(TaskPool*, Task*, void*, s32);
extern char data_ov015_021284d4[44];

const TaskHandle Tsk_BtlEnm026_UG = {data_ov015_021284d4, func_ov015_021275fc, 0x1F4};

char data_ov015_021284d4[44] = "Tsk_BtlEnm026_UG";

void func_ov015_02126fd8(BtlEnm026* data) {
    BtlEnm026* boss = (BtlEnm026*)data_ov003_020e71b8->unk3D898;

    switch (data->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 0);
            func_ov003_020c4cc4(data, 0x27F);
            data->unk_1C2 = data->unk_1C2 + 1;
            return;

        case 1: {
            s32 angle;
            s16 sin;
            s16 cos;
            s32 mag;
            s32 tx;

            tx    = boss->actor.position.x + ((boss->actor.position.x > data->actor.position.x) ? -0x50000 : 0x50000);
            angle = FX_Atan2Idx(boss->actor.position.y - data->actor.position.y, tx - data->actor.position.x) >> 4;
            sin   = data_0205e4e0[angle * 2 + 1];
            cos   = data_0205e4e0[angle * 2];
            mag   = Mth_MulFixed((sin < 0) ? -sin : sin, 0x2000);
            mag += 0x1000;
            data->unk_1E8          = Mth_MulFixed(sin, mag);
            data->unk_1EC          = Mth_MulFixed(cos, mag);
            data->actor.position.x = data->actor.position.x + data->unk_1E8;
            data->actor.position.y = data->actor.position.y + data->unk_1EC;
            func_ov003_020c4ab4(data, (data->unk_1E8 > 0) ? 1 : 0);
            if (data->actor.position.x < boss->actor.position.x - 0x5A000 ||
                data->actor.position.x > boss->actor.position.x + 0x5A000 ||
                data->actor.position.y < boss->actor.position.y - 0xA000 ||
                data->actor.position.y > boss->actor.position.y + 0xA000)
            {
                return;
            }
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xA, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 2;
            return;
        }

        case 2: {
            s32 dx;

            func_ov003_020c4b5c(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 >= 0) {
                Sprite_Restart(&data->unk_084.sprite);
                return;
            }
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xB, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            dx            = boss->actor.position.x - data->actor.position.x;
            data->unk_1C0 = ((dx < 0) ? -dx : dx) / 0x8000 + 5;
            func_ov003_020c4cc4(data, 0x280);
            return;
        }

        case 3: {
            s32 dy;
            s32 h;

            data->actor.position.x += (data->unk_084.flags46 & 1) ? 0x8000 : -0x8000;
            dy = boss->actor.position.y - data->actor.position.y;
            if (dy > 0x800) {
                dy = 0x800;
            } else if (dy < -0x800) {
                dy = -0x800;
            }
            data->actor.position.y = data->actor.position.y + dy;
            h                      = func_ov003_0208a114(0xB8);
            if (func_ov003_0208a164(h, &data->actor.unk_04, data->actor.position.x, data->actor.position.y,
                                    data->actor.position.z) == 1)
            {
                data->unk_1F0 = 0;
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 >= 0) {
                return;
            }
            data->unk_1E8 = (data->unk_084.flags46 & 1) ? 0x8000 : -0x8000;
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xC, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            return;
        }

        case 4:
            func_ov003_020ccf1c(&data->unk_1E8, -0x400);
            data->actor.position.x = data->actor.position.x + data->unk_1E8;
            if (data->unk_1E8 != 0) {
                return;
            }
            data->unk_1F0 = data->unk_1F0 - 1;
            if (data->unk_1F0 <= 0) {
                func_ov015_02125a64(data, (s32)func_ov015_0212736c);
            } else {
                func_ov015_02125a64(data, (s32)func_ov015_02126fd8);
            }
            return;
    }
}

void func_ov015_0212736c(BtlEnm026* data) {
    if (data_ov015_02128500.variant == 0x11 && data->unk_1D8 == 1) {
        s32 count;

        data->unk_1D8 = 0;
        count         = RNG_Next(3) + 3;
        data->unk_1F0 = count;
        func_ov015_02125a64(data, (s32)func_ov015_02126fd8);
        return;
    }
    if (data_ov015_02128500.flag08 != 0 || data_ov015_02128500.result10 == -1) {
        if (data->unk_1D4 == 1 && data->unk_1E4 == 0) {
            func_ov003_020c4cc4(data, 0x27A);
            data->unk_1E4 = 1;
        }
        func_ov015_02125a64(data, (s32)func_ov015_02127594);
        return;
    }
    if (data_ov015_02128500.variant == 0xB) {
        return;
    }
    switch (data->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = func_ov003_020c42ec(data);
            break;

        case 1: {
            s32 r;
            s16 sin;
            s16 cos;

            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 >= 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 0);
            r             = RNG_Next(0xFFFF);
            sin           = data_0205e4e0[(r >> 4) * 2 + 1];
            cos           = data_0205e4e0[(r >> 4) * 2];
            data->unk_1E8 = Mth_MulFixed(sin, data->unk_1D0);
            data->unk_1EC = Mth_MulFixed(cos, data->unk_1D0) >> 1;
            func_ov003_020c4ab4(data, (data->unk_1E8 > 0) ? 1 : 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = RNG_Next(data->unk_198);
            break;
        }

        case 2:
            data->actor.position.x = data->actor.position.x + data->unk_1E8;
            data->actor.position.y = data->actor.position.y + data->unk_1EC;
            data->unk_1C0          = data->unk_1C0 - 1;
            if (data->unk_1C0 < 0) {
                data->unk_1C2 = 0;
            }
            break;
    }
}

void func_ov015_02127594(BtlEnm026* data) {
    if (data_ov015_02128500.result10 != -1) {
        CombatSprite_SetAnimFromTable(&data->unk_084, 2, 0);
    } else {
        CombatSprite_SetAnimFromTable(&data->unk_084, 3, 0);
    }
    data->actor.position.x += data->unk_1D0;
    func_ov003_020c4ab4(data, (data->unk_1D0 > 0) ? 1 : 0);
}

s32 func_ov015_021275fc(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm026* data = task->data;
    s32        ret  = 1;

    switch (stage) {
        case 0:
            MI_CpuSet(data, 0, 0x1F4);
            func_ov015_02125a8c(0, data, args, (s32)func_ov015_0212736c);
            break;

        case 1:
            ret = func_ov015_02125b08(data);
            break;

        case 2:
            func_ov003_020c48b0(data);
            break;

        case 3:
            data_ov015_02128500.initialized = 0;
            func_ov003_020c492c(data);
            break;
    }
    return ret;
}

void func_ov015_02127690(BtlEnm026* data) {
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        if (data->unk_1D4 == 1) {
            data_ov015_02128500.unk_28 = data->unk_080;
        }
    }
}

void func_ov015_021276c4() {}

void func_ov015_021276c8(void) {
    if (data_ov015_02128500.unk_24 == 0) {
        Ov003Node3D8BC* node = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC);
        node                 = func_ov003_02084984(&node->unk_108);
        while (node != 0) {
            if (node->unk_044 < node->unk_046) {
                u32 id = node->unk_000;
                if (id != 4) {
                    data_ov015_02128500.unk_24 = (u32)node;
                    return;
                }
            }
            node = func_ov003_02084984(&node->unk_108);
        }
        return;
    }
    func_ov003_020a4390(data_ov015_02128500.unk_24, 4);
    data_ov015_02128500.unk_24 = 0;
}

void func_ov015_02127758(BtlEnm026* data) {
    data_ov015_02128500.flag08 = 0;
    data_ov015_02128500.unk_1E = 5;
    data_ov015_02128500.unk_20 = 0;
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    func_ov003_020ccec0(data, 1);
    data->actor.unk_62 = 0;
}

void func_ov015_021277a4(BtlEnm026* data) {
    if (func_ov003_020c37f8(&data->unk_084) == 0 && data_ov015_02128500.flag08 == 0) {
        if (data_ov015_02128500.unk_20 != 0) {
            if (data_ov003_020e71b8->unk3D8F8 != 0) {
                return;
            }
            data_ov015_02128500.unk_20 = 0;
            data_ov015_02128500.unk_1E = data_ov015_02128500.unk_1E - 1;
            if (data_ov015_02128500.unk_1E > 0) {
                return;
            }
            if (data->actor.currentHp != 0) {
                if (data->unk_184->actor.currentHp != 0) {
                    data_ov015_02128500.result10 = -1;
                }
            }
            return;
        }
        if (data_ov003_020e71b8->unk3D8F8 != 0) {
            data_ov015_02128500.unk_20 = 1;
        }
        return;
    }
    func_ov003_020ccec0(data, 1);
    data->actor.unk_62 = 0;
}

void func_ov015_0212786c(BtlEnm026* data) {
    BtlEnm026* twin;

    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    twin = data->unk_184;
    if (data_ov015_02128500.unk14 == 2) {
        func_ov015_02125a64(twin->unk_188, (s32)func_ov015_02125e5c);
    }
}

void func_ov015_021278b0(BtlEnm026* data) {
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    if (data->unk_1D4 != 1) {
        return;
    }
    func_ov015_02125d0c(data, 2, &Tsk_BtlEnm026_UG, &Tsk_BtlEnm026_RG);
}

void func_ov015_021278f8(BtlEnm026* data) {
    if (data->actor.pendingCommand == 3 && (data->actor.flags & 0x2000)) {
        data_ov015_02128500.counter1C = data_ov015_02128500.counter1C + 1;
        data->unk_1D4                 = (data_ov015_02128500.counter1C > 2) ? 1 : 3;
        return;
    }
    data->unk_1D4 = 3;
}

void func_ov015_02127950(BtlEnm026* data) {
    data_ov015_02128500.flag08 = 0;
    data->unk_196 &= ~0x2;
}

void func_ov015_02127974(BtlEnm026* data) {
    if ((u32)(data_ov015_02128500.unk14 - 1) > 1) {
        return;
    }
    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        data->unk_196 |= 0x2;
    }
    data->unk_1D8 = 1;
}

void func_ov015_021279c0(BtlEnm026* data) {
    if (func_ov003_020c37f8(&data->unk_084) == 0 && data->unk_1D4 == 1) {
        data->actor.flags |= 0x80000020;
        data->unk_18C |= 0x1;
        return;
    }
    data->unk_196 &= ~0x2;
}

void func_ov015_02127a14(BtlEnm026* data) {
    BtlEnm026* twin;

    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    twin = data->unk_184->unk_188;
    twin->unk_18C |= 0x1;
    if (data->unk_084.animTableIndex != 0) {
        twin->unk_18C &= ~0x1;
    }
}

void func_ov015_02127a60(BtlEnm026* data) {
    data->unk_196 |= 0x200;
}

void func_ov015_02127a74(BtlEnm026* data) {
    func_ov003_02084694(&data->unk_144, 1);
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    if (data->unk_1D4 != 1) {
        return;
    }
    func_ov015_02125d0c(data, 5, &Tsk_BtlEnm026_UG, NULL);
}

void func_ov015_02127ac4(BtlEnm026* data) {
    BtlEnm026* twin;
    BtlEnm026* node;

    if (data->unk_1D4 != 1) {
        if (func_ov015_02125c94(data) != 0) {
            data->unk_1CC = 0;
        }
    }
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    twin = data->unk_184->unk_188;
    CombatSprite_SetPaletteMode(&twin->unk_084, 0);
    if ((u32)(data_ov015_02128500.unk14 - 1) <= 1) {
        CombatSprite_SetPaletteMode(&twin->unk_084, 2);
    }
    if (data->unk_1C4 != (void (*)(BtlEnm026*))data->unk_1C8) {
        return;
    }
    if (RNG_Next(data->unk_19E) == 0) {
        func_ov015_02125a64(data, (s32)func_ov015_02125f48);
    }
    if (data->unk_1D4 != 1 || data_ov015_02128500.unk14 != 3) {
        return;
    }
    node = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    while (node != 0) {
        func_ov003_020c4fc8(node);
        node = func_ov003_02084984(&node->unk_178);
    }
}

void func_ov015_02127bc0(BtlEnm026* data) {
    data->unk_1D0 = data->unk_1D0 * 2;
}

void func_ov015_02127bd0(BtlEnm026* data) {
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        return;
    }
    if (data->unk_1D4 != 1) {
        return;
    }
    func_ov015_02125d0c(data, 2, &Tsk_BtlEnm026_UG, &Tsk_BtlEnm026_RG);
}

void func_ov015_02127c18(BtlEnm026* data) {
    if (data->actor.pendingCommand != 3) {
        return;
    }
    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        return;
    }
    if (data->unk_1D4 == data_ov015_02128500.counter1C + 1) {
        data_ov015_02128500.counter1C = data_ov015_02128500.counter1C + 1;
    } else {
        data_ov015_02128500.result10 = -1;
    }
}

void func_ov015_02127c68(BtlEnm026* data) {
    data_ov015_02128500.flag08 = 0;
    CombatSprite_SetAnimFromTable(&data->unk_084, 1, 0);
    data->unk_196 &= ~0x2;
}

void func_ov015_02127ca0(BtlEnm026* data) {
    s32 flag;

    data->actor.pendingCommand = 0;
    if ((u32)data_ov015_02128500.unk14 - 1 <= 2) {
        data_ov015_02128500.result10 = -1;
    }
    flag = SystemStatusFlags.unk_10 != 0;
    if (flag == 0) {
        return;
    }
    if (data_ov003_020e71b8->unk3D874 != 2) {
        return;
    }
    if (data_ov015_02128500.result10 == -1) {
        return;
    }
    CombatActor_SetPendingCommand(&data->actor, 3);
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        data->actor.flags |= 0x2000;
    }
    if (SndMgr_IsSEPlaying(0x279) == 0) {
        return;
    }
    SndMgr_StopPlayingSE(0x279);
}

void func_ov015_02127d64(void) {
    data_ov015_02128500.timer18   = 0x258;
    data_ov015_02128500.seconds1A = data_ov015_02128500.timer18 / 60;
    data_ov015_02128500.flag08    = 0;
}

void func_ov015_02127da4(BtlEnm026* data) {
    s32 q;

    if (func_ov003_020c37f8(&data->unk_084) != 0 || data->unk_1D4 != 1) {
        return;
    }
    if (data_ov003_020e71b8->unk3D874 != 2) {
        return;
    }
    if (data->actor.flags & 4) {
        return;
    }
    data_ov015_02128500.timer18 = data_ov015_02128500.timer18 - 1;
    {
        s32 t = data_ov015_02128500.timer18;
        if (t > 0) {
            q = t / 60;
        } else {
            q = 0;
        }
    }
    data_ov015_02128500.seconds1A = q + 1;
    if (data_ov015_02128500.timer18 >= 0) {
        return;
    }
    data_ov015_02128500.timer18   = 0;
    data_ov015_02128500.seconds1A = 0;
    data_ov015_02128500.result10  = -1;
}

void func_ov015_02127e5c(BtlEnm026* data) {
    data->actor.flags |= 0x40;
    data->unk_196 &= ~0x2;
}

void func_ov015_02127e7c(BtlEnm026* data) {
    BtlEnm026* twin = data->unk_184->unk_188;

    data->unk_1DC = data->unk_1DC - 1;
    if (data->unk_1DC == 0) {
        func_ov003_020ccf1c(&data->unk_1D0, 0x100);
        func_ov003_020ccf1c(&twin->unk_1D0, 0x100);
        data->actor.flags |= 0x40;
        twin->actor.flags |= 0x40;
    } else if (data->unk_1DC < 0) {
        data->unk_1DC = 0;
    }

    if (data_ov015_02128500.unk14 == 7) {
        if (twin->actor.flags & 0x40) {
            twin->actor.flags &= ~0x40;
            data->unk_1DC = 0xF;
            return;
        }
    }
    if (data_ov015_02128500.unk14 != 1 && data_ov015_02128500.unk14 != 2 && data_ov015_02128500.unk14 != 7) {
        return;
    }
    if (twin->unk_1DC > 0) {
        func_ov015_02125a64(data, (s32)func_ov015_02125e5c);
        func_ov015_02125a64(twin, (s32)func_ov015_02125e5c);
        twin->unk_1D8 = 0;
        data->unk_1D8 = 0;
        twin->unk_1DC = 0;
        data->unk_1DC = 0;
        data->actor.flags |= 0x40;
        twin->actor.flags |= 0x40;
    }
}

void func_ov015_02127f90(BtlEnm026* data) {
    if (data_ov015_02128500.unk14 == 3) {
        data_ov015_02128500.result10 = -1;
    }
    if (data_ov003_020e71b8->unk3D79C == 0) {
        return;
    }
    if (data_ov003_020e71b8->unk3D874 == 2) {
        data->actor.pendingCommand = 3;
    }
}
