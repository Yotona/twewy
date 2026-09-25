#include "Combat/Noise/Private/BtlEnm026.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

extern s32   func_ov003_020c37f8(void*);
extern void  func_ov003_020c3efc(void*, void*);
extern void  func_ov003_020c427c(void*);
extern void  func_ov003_020c4ab4(void*, s32);
extern void  func_ov003_020c4628(void*);
extern void  func_ov003_020c4668(void*);
extern void  func_ov003_020c4cc4(void*, s32);
extern void  func_ov003_020ccea8(void*, void*);
extern s32   func_ov003_020c5bfc(void*);
extern s32   func_ov003_020ccfec(void*);
extern s32   func_ov003_020c72b4(void*, s32, s32);
extern void  func_ov003_02082750(void*, s32);
extern void  func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern s32   func_ov003_02082f2c(void*);
extern void  func_ov003_02084694(void*, s32);
extern s32   func_ov003_020c3c28(void);
extern void  func_ov003_020c48fc(void*);
extern void  func_ov003_020c4878(void*);
extern void  func_ov003_020c48b0(void*);
extern void  func_ov003_020c492c(void*);
extern s32   func_ov003_020ccedc(s32);
extern void  func_ov003_02087ed8(u16);
extern s32   func_ov003_0208a114(u16);
extern s32   func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32   func_ov003_0208a08c(s32, void*, s32);
extern void  func_ov003_02082cc4(void*);
extern void  func_ov003_02082b0c(void*);
extern void  func_ov003_02082b64(void*);
extern void  func_ov003_02082724(void*, s32, s32);
extern void  func_ov003_020ccf94(s32, void*, void*);
extern void  func_ov003_020c4af0(void*);
extern void  func_ov003_020ccf58(s32, void*);
extern s32   func_ov003_020ccfc8(void*, s32);
extern s32   func_ov003_020c6b8c(void*, s32);
extern s32   func_ov003_020c6bc8(void*, s32);
extern s32   func_ov003_020c6c2c(void*, s32);
extern s32   func_ov003_020c5e98(void*);
extern void  func_ov003_020c3cec(void*, void*, void*, void*);
extern void  func_ov003_020ccf1c(void*, s32);
extern void  func_ov003_020c4b1c(void*);
extern s32   func_ov003_020c42ec(void*);
extern void  func_ov003_020c5924(void*, void*);
extern void  func_ov003_02082d04(void*);
extern void  func_ov003_020c4b5c(void*);
extern void  func_ov003_020a4390(s32, s32);
extern void  func_ov003_02082f1c(void*, s32);
extern void  func_ov003_020c4fc8(void*);
extern void  func_ov003_020ccec0(void*, s32);
extern void* func_ov003_0208495c(void*);
extern void* func_ov003_02084984(void*);
extern s32   func_ov003_020cd010(void*);

extern s16 data_0205e4e0[];

/// Rounds a fixed-point value through a float, matching the original codegen.
#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

// MARK: Forward declarations

s32  func_ov015_021256c0(s32);
void func_ov015_021256f8(BtlEnm026*);
void func_ov015_021257f8(BtlEnm026*);
s32  func_ov015_021259f4(BtlEnm026*, s32, void*);
void func_ov015_02125a64(BtlEnm026*, s32);
void func_ov015_02125a8c(s32, BtlEnm026*, void*, s32);
s32  func_ov015_02125b08(BtlEnm026*);
s32  func_ov015_02125c94(BtlEnm026*);
void func_ov015_02125d0c(BtlEnm026*, s32, void*, void*);
void func_ov015_02125e5c(BtlEnm026*);
void func_ov015_02125f00(BtlEnm026*);
void func_ov015_02125f48(BtlEnm026*);
void func_ov015_02125f80(BtlEnm026*);
void func_ov015_02126070(BtlEnm026*);
void func_ov015_021260c4(BtlEnm026*);
void func_ov015_02126158(BtlEnm026*);
void func_ov015_02126184(BtlEnm026*);
void func_ov015_021261ac(BtlEnm026*);
s32  func_ov015_021261d8(BtlEnm026Icon*, Enm026IconSpawn*);
s32  func_ov015_02126464(BtlEnm026Icon*);
s32  func_ov015_02126660(BtlEnm026Icon*);
s32  func_ov015_0212683c(BtlEnm026Icon*);
s32  func_ov015_02126860(TaskPool*, Task*, void*, s32);
void func_ov015_021268b0(BtlEnm026*);
void func_ov015_0212699c(BtlEnm026*);
void func_ov015_02126db8(BtlEnm026*);
void func_ov015_02126ec4(BtlEnm026*);
s32  func_ov015_02126f44(TaskPool*, Task*, void*, s32);
void func_ov015_02126fd8(BtlEnm026*);
void func_ov015_0212736c(BtlEnm026*);
void func_ov015_02127594(BtlEnm026*);
s32  func_ov015_021275fc(TaskPool*, Task*, void*, s32);
void func_ov015_02127690(BtlEnm026*);
void func_ov015_021276c4();
void func_ov015_021276c8();
void func_ov015_02127758(BtlEnm026*);
void func_ov015_021277a4(BtlEnm026*);
void func_ov015_0212786c(BtlEnm026*);
void func_ov015_021278b0(BtlEnm026*);
void func_ov015_021278f8(BtlEnm026*);
void func_ov015_02127950(BtlEnm026*);
void func_ov015_02127974(BtlEnm026*);
void func_ov015_021279c0(BtlEnm026*);
void func_ov015_02127a14(BtlEnm026*);
void func_ov015_02127a60(BtlEnm026*);
void func_ov015_02127a74(BtlEnm026*);
void func_ov015_02127ac4(BtlEnm026*);
void func_ov015_02127bc0(BtlEnm026*);
void func_ov015_02127bd0(BtlEnm026*);
void func_ov015_02127c18(BtlEnm026*);
void func_ov015_02127c68(BtlEnm026*);
void func_ov015_02127ca0(BtlEnm026*);
void func_ov015_02127d64(void);
void func_ov015_02127da4(BtlEnm026*);
void func_ov015_02127e5c(BtlEnm026*);
void func_ov015_02127e7c(BtlEnm026*);
void func_ov015_02127f90(BtlEnm026*);

// MARK: Data

char data_ov015_021282e0[] = "Apl_Hor/Grp_BtlEnm026.bin";
char data_ov015_021282fc[] = "Apl_Hor/Grp_BtlEnm026a.bin";
char data_ov015_02128318[] = "Apl_Hor/Grp_BtlEnm026c.bin";
char data_ov015_02128334[] = "Apl_Hor/Grp_BtlEnm026d.bin";
char data_ov015_02128350[] = "Apl_Hor/Grp_BtlEnm026e.bin";
char data_ov015_0212836c[] = "Apl_Hor/Grp_BtlEnm026f.bin";
char data_ov015_02128388[] = "Apl_Hor/Grp_BtlEnm026g.bin";
char data_ov015_021283a4[] = "Apl_Hor/Grp_BtlEnm026h.bin";
char data_ov015_021283c0[] = "Apl_Hor/Grp_BtlEnm026i.bin";
char data_ov015_021283dc[] = "Apl_Hor/Grp_BtlEnm026b.bin";
char data_ov015_0212846c[] = "Tsk_BtlEnm026_Icon";
char data_ov015_02128480[] = "Apl_Hor/Grp_BtlEnm026_Bdg.bin";
char data_ov015_021284a0[] = "Apl_Hor/Grp_BtlEnm026_Icon.bin";
char data_ov015_021284c0[] = "Tsk_BtlEnm026_RG";
char data_ov015_021284d4[] = "Tsk_BtlEnm026_UG";

BinIdentifier data_ov015_02128078 = {3, data_ov015_021282e0};
BinIdentifier data_ov015_02128080 = {3, data_ov015_021282fc};
BinIdentifier data_ov015_02128088 = {3, data_ov015_021283dc};
BinIdentifier data_ov015_02128090 = {3, data_ov015_02128318};
BinIdentifier data_ov015_02128098 = {3, data_ov015_02128334};
BinIdentifier data_ov015_021280a0 = {3, data_ov015_02128350};
BinIdentifier data_ov015_021280a8 = {3, data_ov015_0212836c};
BinIdentifier data_ov015_021280b0 = {3, data_ov015_02128388};
BinIdentifier data_ov015_021280b8 = {3, data_ov015_021283a4};
BinIdentifier data_ov015_021280c0 = {3, data_ov015_021283c0};

SpriteAnimEntry data_ov015_021280c8[13] = {
    {0x1, 0x3, 0x2, 0},
    {0x1, 0x3, 0x2, 1},
    {0x4, 0x6, 0x5, 0},
    {0x4, 0x6, 0x5, 1},
    {0x7, 0x9, 0x8, 0},
    {0x7, 0x9, 0x8, 1},
    {0x7, 0x9, 0x8, 2},
    {0x7, 0x9, 0x8, 3},
    {0x7, 0x9, 0x8, 4},
    {0xA, 0xC, 0xB, 0},
    {0xD, 0xF, 0xE, 0},
    {0xD, 0xF, 0xE, 1},
    {0xD, 0xF, 0xE, 2},
};

Enm026IdPair data_ov015_02128130[13] = {
    {0x11, 0x00},
    {0x12, 0x04},
    {0x13, 0x01},
    {0x14, 0x0B},
    {0x15, 0x02},
    {0x16, 0x06},
    {0x17, 0x05},
    {0x18, 0x08},
    {0x19, 0x09},
    {0x1A, 0x0A},
    {0x1B, 0x07},
    {0x1C, 0x11},
    {0x1D, 0x03},
};

Enm026StateFns data_ov015_02128198[18] = {
    {               NULL, func_ov015_0212786c},
    {func_ov015_021278b0, func_ov015_021278f8},
    {func_ov015_02127690, func_ov015_021276c4},
    {               NULL,                NULL},
    {func_ov015_02127bc0,                NULL},
    {func_ov015_02127d64, func_ov015_02127da4},
    {func_ov015_02127bd0, func_ov015_02127c18},
    {func_ov015_02127e5c, func_ov015_02127e7c},
    {func_ov015_02127a74, func_ov015_02127ac4},
    {func_ov015_021279c0, func_ov015_02127a14},
    {func_ov015_02127758, func_ov015_021277a4},
    {func_ov015_02127c68, func_ov015_02127ca0},
    {               NULL, func_ov015_02127f90},
    {func_ov015_02127a60,                NULL},
    {               NULL, func_ov015_021276c8},
    {               NULL,                NULL},
    {               NULL,                NULL},
    {func_ov015_02127950, func_ov015_02127974},
};

SpriteAnimEntry data_ov015_02128238[13] = {
    {0x1, 0x3, 0x2, 0x0},
    {0x1, 0x3, 0x2, 0x1},
    {0x1, 0x3, 0x2, 0x2},
    {0x1, 0x3, 0x2, 0x3},
    {0x1, 0x3, 0x2, 0x4},
    {0x1, 0x3, 0x2, 0x5},
    {0x1, 0x3, 0x2, 0x6},
    {0x1, 0x3, 0x2, 0x7},
    {0x1, 0x3, 0x2, 0x8},
    {0x1, 0x3, 0x2, 0x9},
    {0x1, 0x3, 0x2, 0xA},
    {0x1, 0x3, 0x2, 0xB},
    {0x1, 0x3, 0x2, 0xC},
};

BinIdentifier data_ov015_02128228 = {3, data_ov015_021284a0};
BinIdentifier data_ov015_02128230 = {3, data_ov015_02128480};

const TaskHandle Tsk_BtlEnm026_Icon = {data_ov015_0212846c, func_ov015_02126860, 0x134};
const TaskHandle Tsk_BtlEnm026_RG   = {data_ov015_021284c0, func_ov015_02126f44, 0x1F4};
const TaskHandle Tsk_BtlEnm026_UG   = {data_ov015_021284d4, func_ov015_021275fc, 0x1F4};

Enm026Variant data_ov015_02127fd8 = {&data_ov015_02128078, data_ov015_021280c8, 0, 0x0D, 0x40};
Enm026Variant data_ov015_02127fe8 = {&data_ov015_02128088, data_ov015_021280c8, 0, 0x0D, 0x44};
Enm026Variant data_ov015_02127ff8 = {&data_ov015_021280a0, data_ov015_021280c8, 0, 0x0D, 0x30};
Enm026Variant data_ov015_02128008 = {&data_ov015_021280b0, data_ov015_021280c8, 0, 0x0D, 0x42};
Enm026Variant data_ov015_02128018 = {&data_ov015_02128090, data_ov015_021280c8, 0, 0x0D, 0x30};
Enm026Variant data_ov015_02128028 = {&data_ov015_02128080, data_ov015_021280c8, 0, 0x0D, 0x40};
Enm026Variant data_ov015_02128038 = {&data_ov015_021280c0, data_ov015_021280c8, 0, 0x10, 0x44};
Enm026Variant data_ov015_02128048 = {&data_ov015_02128098, data_ov015_021280c8, 0, 0x0D, 0x30};
Enm026Variant data_ov015_02128058 = {&data_ov015_021280b8, data_ov015_021280c8, 0, 0x0D, 0x46};
Enm026Variant data_ov015_02128068 = {&data_ov015_021280a8, data_ov015_021280c8, 0, 0x0D, 0x42};

Enm026Variant* data_ov015_021283f8[11] = {
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8,
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8,
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8,
};

Enm026Variant* data_ov015_02128424[18] = {
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fe8, &data_ov015_02128058, &data_ov015_02128028,
    &data_ov015_02128018, &data_ov015_02128018, &data_ov015_02128008, &data_ov015_02128048, &data_ov015_02127ff8,
    &data_ov015_02128068, &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02128068, &data_ov015_02128068,
    &data_ov015_02128068, &data_ov015_02127fd8, &data_ov015_02128038,
};

Enm026State data_ov015_02128500;

// MARK: Functions

s32 func_ov015_021256c0(s32 arg0) {
    s32 i;
    for (i = 0; i < 13; i++) {
        if (arg0 == data_ov015_02128130[i].unk_00) {
            return data_ov015_02128130[i].unk_04;
        }
    }
    return 0;
}

void func_ov015_021256f8(BtlEnm026* data) {
    s32 engine = data->unk_084.sprite.bits_0_1;

    if (data_ov015_02128500.initialized == 0) {
        MI_CpuSet(&data_ov015_02128500, 0, 0x2C);
        data_ov015_02128500.variant     = func_ov015_021256c0((u16)data->unk_07C);
        data_ov015_02128500.flag08      = 1;
        data_ov015_02128500.initialized = 1;
    }
    CombatSprite_Release(&data->unk_084);
    func_ov003_020ccf94(engine, &data->unk_084, data_ov015_02128424[data_ov015_02128500.variant]);
    func_ov003_020c4af0(data);
    if (func_ov003_020c37f8(&data->unk_084) == 0) {
        data_ov015_02128500.counter0C = data_ov015_02128500.counter0C + 1;
    }
    data->unk_1D4 = data_ov015_02128500.counter0C;
    if (data_ov015_02128198[data_ov015_02128500.variant].unk_00 != NULL) {
        data_ov015_02128198[data_ov015_02128500.variant].unk_00(data);
    }
    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        data->unk_196 &= ~0x2;
    }
}

void func_ov015_021257f8(BtlEnm026* data) {
    TaskPool* pool;

    if (data_ov003_020e71b8->unk3D874 != 2) {
        return;
    }
    data_ov015_02128500.unk14 = data->actor.pendingCommand;
    if (data_ov015_02128500.flag08 != 0) {
        data->actor.flags |= 0x10000000;
    }
    if (func_ov015_02125c94(data) != 0) {
        data->unk_1CC = 0;
        if (data->unk_1D4 != 1) {
            if (data_ov015_02128500.variant == 6 || data_ov015_02128500.variant == 1) {
                data_ov015_02128500.result10 = data->unk_1CC - 1;
            }
        } else {
            data_ov015_02128500.result10 = data->unk_1CC - 1;
        }
    }
    if (data_ov015_02128198[data_ov015_02128500.variant].unk_04 != NULL) {
        data_ov015_02128198[data_ov015_02128500.variant].unk_04(data);
    }
    if (data_ov015_02128500.result10 == -1) {
        if (data_ov015_02128500.variant == 0xB) {
            if (func_ov003_020c37f8(&data->unk_084) == 0) {
                pool = &data_ov003_020e71b8->unk_00000;
            } else {
                pool = &data_ov003_020e71b8->taskPool;
            }
            EasyTask_DeleteTask(pool, data->unk_1E0);
        }
        data_ov015_02128500.flag08 = 1;
        {
            s32 vel = 0x6000;
            if (data->unk_1D0 <= 0) {
                vel = -vel;
            }
            data->unk_1D0 = vel;
        }
        func_ov003_020ccec0(data, 1);
        data_ov003_020e71b8->unk3D878 |= 0x200000;
        if (data_ov015_02128500.variant == 3) {
            *(u8*)((u8*)data_ov003_020e71b8 + 0x3D875) = 2;
            data_ov003_020e71b8->unk3D878 |= 0x20000000;
        }
    } else {
        if (func_ov003_020c37f8(&data->unk_084) == 0) {
            return;
        }
        if ((u32)(data->actor.pendingCommand - 1) > 1) {
            return;
        }
        if (data_ov015_02128500.variant != 0xA && data_ov015_02128500.variant != 7 && data_ov015_02128500.variant != 0xB &&
            data_ov015_02128500.variant != 3 && data_ov015_02128500.variant != 9 && data_ov015_02128500.variant != 8 &&
            data_ov015_02128500.variant != 5 && data_ov015_02128500.variant != 0x11)
        {
            func_ov003_0208a08c(0, data->unk_184, 0);
        }
    }
}

s32 func_ov015_021259f4(BtlEnm026* owner, s32 kind, void* params) {
    TaskPool*       pool;
    Enm026IconSpawn spawn;

    if (func_ov003_020c37f8(&owner->unk_084) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    spawn.unk_00 = owner;
    spawn.unk_04 = kind;
    spawn.unk_08 = params;
    return EasyTask_CreateTask(pool, &Tsk_BtlEnm026_Icon, NULL, 0, NULL, &spawn);
}

void func_ov015_02125a64(BtlEnm026* data, s32 stateFn) {
    func_ov003_020c427c(data);
    data->unk_1C4 = (void (*)(BtlEnm026*))stateFn;
    data->unk_1C2 = 0;
    data->unk_1C0 = 0;
}

void func_ov015_02125a8c(s32 engine, BtlEnm026* data, void* args, s32 stateFn) {
    func_ov003_020c3efc(data, args);
    func_ov003_020ccf58(engine, data);
    data->unk_188 = data;
    func_ov015_02125a64(data, (s32)func_ov015_02126070);
    data->unk_1E0 = -1;
    data->unk_1CC = 1;
    data->unk_1C8 = stateFn;
    data->unk_1D0 = (data->unk_084.flags46 & 1) ? 0xC00 : -0xC00;
    func_ov015_021256f8(data);
    func_ov003_02084694(&data->unk_144, 1);
}

s32 func_ov015_02125b08(BtlEnm026* data) {
    func_ov015_021257f8(data);

    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 0:
            break;
        case 1:
            func_ov015_02125a64(data, (s32)func_ov015_02125e5c);
            break;
        case 3:
            func_ov003_02084694(&data->unk_144, 1);
            if (data->unk_18C & 0x10) {
                data->unk_18C |= 0x20;
            } else {
                func_ov015_02125a64(data, (s32)func_ov015_021260c4);
            }
            break;
        case 4:
            func_ov015_02125a64(data, (s32)func_ov015_02126158);
            break;
        case 5:
            func_ov015_02125a64(data, (s32)func_ov015_02126184);
            break;
        case 2:
            func_ov015_02125a64(data, (s32)func_ov015_02125f00);
            break;
        case 6:
            func_ov015_02125a64(data, (s32)func_ov015_021261ac);
            break;
    }

    if (data->unk_1C4 != NULL) {
        data->unk_1C4(data);
    }
    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        func_ov003_020c4628(data);
    } else {
        func_ov003_020c4668(data);
    }
    func_ov003_020ccea8(data, &data->unk_084);

    if (data->unk_084.animTableIndex == 2) {
        if (data->unk_084.sprite.unk16 == 1 && data->unk_084.sprite.frameTimer == 1 && data->unk_1D4 == 1) {
            func_ov003_020c4cc4(data, 0x27B);
        }
    } else if (data->unk_084.animTableIndex == 3) {
        if (data->unk_084.sprite.unk16 == 4 && data->unk_084.sprite.frameTimer == 1 && data->unk_1D4 == 1) {
            func_ov003_020c4cc4(data, 0x27C);
        }
    }
    return data->unk_1CC;
}

s32 func_ov015_02125c94(BtlEnm026* data) {
    s32 engine = (func_ov003_020c37f8(&data->unk_084) != 0);
    if (data_ov015_02128500.variant == 0x11) {
        return 0;
    }
    if (data->unk_1D0 > 0) {
        return data->actor.position.x > (func_ov003_020ccedc(engine) + 0xF000);
    }
    return data->actor.position.x < -0xF000;
}

void func_ov015_02125d0c(BtlEnm026* data, s32 count, void* handleA, void* handleB) {
    Enm026ParticleParams blockB;
    Enm026ParticleParams blockA;
    s32                  i;

    MI_CpuSet(&blockB, 0, sizeof(blockB));
    MI_CpuSet(&blockA, 0, sizeof(blockA));
    blockA.unk_00 = data->unk_07C;
    blockB.unk_00 = data->unk_07C;
    blockA.unk_06 &= ~0x1;
    blockA.unk_04 = data->unk_080;
    blockB.unk_04 = data->unk_080;
    blockB.unk_06 = (blockB.unk_06 & ~0x1) | (blockA.unk_06 & 0x1);
    blockA.unk_10 = 0;
    blockB.unk_10 = 0;
    if (count <= 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        blockB.unk_08 = RNG_Next(data_ov003_020e71b8->unk3D7CC >> 12) << 12;
        blockB.unk_0C = RNG_Next(data_ov003_020e71b8->unk3D7D0 >> 12) << 12;
        blockA.unk_08 = RNG_Next(data_ov003_020e71b8->unk3D824 >> 12) << 12;
        blockA.unk_0C = RNG_Next(data_ov003_020e71b8->unk3D828 >> 12) << 12;
        func_ov003_020c3cec(handleA, handleB, (handleA != 0) ? &blockB : NULL, (handleB != 0) ? &blockA : NULL);
    }
}

void func_ov015_02125e5c(BtlEnm026* data) {
    s32 ret;

    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        ret = func_ov003_020ccfc8(data, 4);
    } else if (data_ov015_02128500.variant != 7) {
        if (data->unk_1C0 == 0) {
            Mini108_VBlank(&data->unk_084, 0, 0);
        }
        data->unk_1C0 = data->unk_1C0 + 1;
        ret           = data->unk_1C0 < 0xF;
    } else {
        ret = func_ov003_020ccfc8(data, 4);
    }
    if (ret != 0) {
        return;
    }
    func_ov015_02125a64(data, data->unk_1C8);
}

void func_ov015_02125f00(BtlEnm026* data) {
    s32 ret;

    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        ret = func_ov003_020ccfc8(data, 4);
    } else {
        ret = func_ov003_020c6b8c(data, 7);
    }
    if (ret != 0) {
        return;
    }
    func_ov015_02125a64(data, data->unk_1C8);
}

void func_ov015_02125f48(BtlEnm026* data) {
    if (func_ov003_020c5e98(data) != 0) {
        return;
    }
    func_ov015_02125a64(data, (data->unk_18C & 0x20) ? (s32)func_ov015_021260c4 : data->unk_1C8);
}

void func_ov015_02125f80(BtlEnm026* data) {
    if (data->unk_1A8 != 3 || data->unk_1E0 != -1) {
        return;
    }
    switch (data_ov015_02128500.variant) {
        case 2:
            if (func_ov003_020c37f8(&data->unk_084) == 0) {
                data->unk_1E0 = func_ov015_021259f4(data, 0, NULL);
            }
            break;

        case 5:
            data->unk_1E0 = func_ov015_021259f4(data, 1, &data_ov015_02128500.seconds1A);
            break;

        case 6:
            data->unk_1E0 = func_ov015_021259f4(data, 1, &data->unk_1D4);
            break;

        case 10:
            data->unk_1E0 = func_ov015_021259f4(data, 1, &data_ov015_02128500.unk_1E);
            break;

        case 11:
            data->unk_1E0 = func_ov015_021259f4(data, 2, NULL);
            break;
    }
}

void func_ov015_02126070(BtlEnm026* data) {
    s32 r4 = func_ov003_020c5bfc(data);
    func_ov015_02125f80(data);
    if (r4 != 0) {
        return;
    }
    if (data_ov015_02128500.variant == 9) {
        func_ov003_020c4cc4(data, 0x281);
    }
    func_ov015_02125a64(data, data->unk_1C8);
}

void func_ov015_021260c4(BtlEnm026* data) {
    s32 ret;

    data->actor.flags |= 0x4;
    if (data_ov015_02128500.variant == 6) {
        if (data_ov015_02128500.result10 != -1 && data->unk_1D4 == 3) {
            ret = func_ov003_020ccfec(data);
        } else {
            ret = func_ov003_020cd010(data);
        }
    } else {
        if (data_ov015_02128500.result10 != -1 && data->unk_1D4 == 1) {
            ret = func_ov003_020ccfec(data);
        } else {
            ret = func_ov003_020cd010(data);
        }
    }
    if (ret == 0) {
        data->unk_1CC = 0;
    }
}

void func_ov015_02126158(BtlEnm026* data) {
    if (func_ov003_020c6bc8(data, 7) != 0) {
        return;
    }
    func_ov015_02125a64(data, (s32)func_ov015_02126184);
}

void func_ov015_02126184(BtlEnm026* data) {
    if (func_ov003_020c6c2c(data, 7) != 0) {
        return;
    }
    func_ov015_02125a64(data, data->unk_1C8);
}

void func_ov015_021261ac(BtlEnm026* data) {
    if (func_ov003_020c72b4(data, 0, 7) != 0) {
        return;
    }
    func_ov015_02125a64(data, data->unk_1C8);
}

s32 func_ov015_021261d8(BtlEnm026Icon* data, Enm026IconSpawn* args) {
    BtlEnm026* owner = args->unk_00;
    s32        r5    = 0;

    MI_CpuSet(data, 0, sizeof(BtlEnm026Icon));
    data->unk_120 = args->unk_00;
    data->unk_124 = args->unk_04;
    data->unk_128 = args->unk_08;

    switch (args->unk_04) {
        case 0: {
            SpriteAnimationEx anim;
            CombatSprite_InitAnim(&anim.anim, r5, &data_ov015_02128230);
            r5                   = 0x11;
            anim.anim.unk_18     = 2;
            anim.anim.unk_26     = 2;
            anim.anim.packIndex  = data_ov015_02128500.unk_28 + 1;
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_28     = 3;
            anim.anim.unk_1C     = 1;
            anim.anim.unk_20     = 4;
            anim.anim.unk_2A     = 1;
            anim.unk_2C          = 0;
            CombatSprite_Load(&data->unk_60, &anim);
            CombatSprite_LoadFromTable(owner->unk_084.sprite.bits_0_1, &data->unk_C0, &data_ov015_02128228,
                                       data_ov015_02128238, 3, 4, 0x10);
            break;
        }

        case 1:
            CombatSprite_LoadFromTable(owner->unk_084.sprite.bits_0_1, &data->unk_60, &data_ov015_02128228,
                                       data_ov015_02128238, 3, 4, 2);
            r5 = 0x10;
            Mini108_VBlank(&data->unk_60, 3, 0);
            CombatSprite_LoadFromTable(owner->unk_084.sprite.bits_0_1, &data->unk_C0, &data_ov015_02128228,
                                       data_ov015_02128238, 3, 4, 2);
            Mini108_VBlank(&data->unk_C0, 3, 0);
            break;

        case 2:
            r5 = 4;
            CombatSprite_LoadFromTable(owner->unk_084.sprite.bits_0_1, &data->unk_60, &data_ov015_02128228,
                                       data_ov015_02128238, 3, 4, 0);
            CombatSprite_LoadFromTable(owner->unk_084.sprite.bits_0_1, &data->unk_C0, &data_ov015_02128228,
                                       data_ov015_02128238, 3, 4, 0);
            break;
    }

    CombatSprite_LoadFromTable(owner->unk_084.sprite.bits_0_1, &data->unk_00, &data_ov015_02128228, data_ov015_02128238,
                               data->unk_124, 4, r5);
    Mini108_VBlank(&data->unk_00, data->unk_124, data->unk_124 == 2);
    if (data->unk_124 != 2) {
        CombatSprite_SetFlip(&data->unk_00, owner->unk_084.flags46 & 1);
    }
    return 1;
}

s32 func_ov015_02126464(BtlEnm026Icon* data) {
    BtlEnm026* owner = data->unk_120;

    if (data->unk_124 != 2) {
        CombatSprite_SetFlip(&data->unk_00, owner->unk_084.flags46 & 1);
    }
    if (owner->actor.flags & 4) {
        return 0;
    }
    switch (data->unk_00.animTableIndex) {
        case 0:
            CombatSprite_Update(&data->unk_60);
            break;

        case 1: {
            s16 hp = *(s16*)data->unk_128;
            if (hp > 0x63) {
                hp = 0x63;
            }
            if (data->unk_130 == 0) {
                if (*(u8*)((u8*)data_ov003_020e71b8 + 0x3D874) == 2) {
                    if (hp == 0) {
                        if (SndMgr_IsSEPlaying(0x27E) == 0) {
                            func_ov003_02087ed8(0x27E);
                        }
                        data->unk_130 = 1;
                    } else if (data->unk_12C != hp) {
                        if (SndMgr_IsSEPlaying(0x27D) == 0) {
                            func_ov003_02087ed8(0x27D);
                        }
                    }
                }
            }
            data->unk_12C = hp;
            Mini108_VBlank(&data->unk_60, hp % 10 + 3, 0);
            {
                s32 tens;
                if (hp < 0xA) {
                    tens = 0;
                } else {
                    tens = hp / 10;
                }
                Mini108_VBlank(&data->unk_C0, tens + 3, 0);
                if (tens > 0) {
                    CombatSprite_Update(&data->unk_C0);
                }
            }
            CombatSprite_Update(&data->unk_60);
            break;
        }

        case 2:
            if (SpriteMgr_IsAnimationFinished(&data->unk_00.sprite) == 0) {
                break;
            }
            func_ov003_02082d04(&data->unk_00);
            if (SndMgr_IsSEPlaying(0x279) == 0) {
                if (data_ov015_02128500.flag08 == 0) {
                    if (*(u8*)((u8*)data_ov003_020e71b8 + 0x3D874) == 2) {
                        func_ov003_02087ed8(0x279);
                    }
                }
            }
            break;
    }
    CombatSprite_Update(&data->unk_00);
    return 1;
}

s32 func_ov015_02126660(BtlEnm026Icon* data) {
    BtlEnm026* owner  = data->unk_120;
    s32        engine = (func_ov003_020c37f8(data) != 0);
    s32        r5;
    s32        r6;
    s16        sx;
    s16        sy;

    if (func_ov003_020c37f8(data) != 0) {
        if (owner->unk_18C & 0x10) {
            return 1;
        }
    }
    r5 = owner->actor.position.x + ((owner->unk_084.flags46 & 1) ? -0xF000 : 0xF000);
    r6 = owner->actor.position.z - 0x19000;

    switch (data->unk_00.animTableIndex) {
        case 0:
            func_ov003_02084348(engine, &sx, &sy, r5, owner->actor.position.y, r6);
            CombatSprite_SetPosition(&data->unk_60, sx, sy);
            func_ov003_02082730(&data->unk_60, 0x7FFFFFFD - owner->actor.position.y);
            CombatSprite_Render(&data->unk_60);
            break;

        case 1:
            if (data->unk_C0.animTableIndex == 3) {
                func_ov003_02084348(engine, &sx, &sy, r5, owner->actor.position.y, r6);
                CombatSprite_SetPosition(&data->unk_60, sx, sy);
                func_ov003_02082730(&data->unk_60, 0x7FFFFFFD - owner->actor.position.y);
                CombatSprite_Render(&data->unk_60);
            } else {
                func_ov003_02084348(engine, &sx, &sy, r5 - 0x4000, owner->actor.position.y, r6);
                CombatSprite_SetPosition(&data->unk_C0, sx, sy);
                func_ov003_02082730(&data->unk_C0, 0x7FFFFFFD - owner->actor.position.y);
                CombatSprite_Render(&data->unk_C0);
                func_ov003_02084348(engine, &sx, &sy, r5 + 0x4000, owner->actor.position.y, r6);
                CombatSprite_SetPosition(&data->unk_60, sx, sy);
                func_ov003_02082730(&data->unk_60, 0x7FFFFFFD - owner->actor.position.y);
                CombatSprite_Render(&data->unk_60);
            }
            break;
    }

    func_ov003_02084348(engine, &sx, &sy, r5, owner->actor.position.y, r6);
    CombatSprite_SetPosition(&data->unk_00, sx, sy);
    func_ov003_02082730(&data->unk_00, 0x7FFFFFFE - owner->actor.position.y);
    CombatSprite_Render(&data->unk_00);
    return 1;
}

s32 func_ov015_0212683c(BtlEnm026Icon* data) {
    CombatSprite_Release(&data->unk_00);
    CombatSprite_Release(&data->unk_60);
    CombatSprite_Release(&data->unk_C0);
    return 1;
}

s32 func_ov015_02126860(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm026Icon* data = task->data;

    switch (stage) {
        case 0:
            return func_ov015_021261d8(data, (Enm026IconSpawn*)args);
        case 1:
            return func_ov015_02126464(data);
        case 2:
            return func_ov015_02126660(data);
        case 3:
            return func_ov015_0212683c(data);
    }
    return 1;
}

void func_ov015_021268b0(BtlEnm026* data) {
    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 0, 0);
            data->actor.flags &= ~0x10000000;
            func_ov003_020c4b1c(data);
            data->unk_196 &= ~0x2;
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = func_ov003_020c42ec(data);
            /* fallthrough */
        case 1:
            break;
    }

    data->unk_1C0 = data->unk_1C0 - 1;
    if (data->unk_1C0 < 0) {
        func_ov015_02125a64(data, (s32)func_ov015_02125f48);
    }
    if (data->unk_1D8 == 1) {
        s16 count;

        data->unk_196 |= 0x2;
        count         = RNG_Next(3) + 3;
        data->unk_1F0 = count;
        data->unk_1D8 = 0;
        func_ov015_02125a64(data, (s32)func_ov015_0212699c);
    }
}

void func_ov015_0212699c(BtlEnm026* data) {
    BtlEnm026* boss = (BtlEnm026*)data_ov003_020e71b8->unk3D89C;

    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 0xA, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 2;
            return;

        case 1: {
            s32 dx;

            func_ov003_020c4b1c(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 >= 0) {
                func_ov003_02082d04(&data->unk_084);
                return;
            }
            Mini108_VBlank(&data->unk_084, 0xB, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            dx            = boss->actor.position.x - data->actor.position.x;
            data->unk_1C0 = ((dx < 0) ? -dx : dx) / 0x8000 + 5;
            func_ov003_020c4cc4(data, 0x280);
            if (boss->actor.position.z < -0x1E000) {
                data->actor.zVelocity = -0x1E000 + 0x19000;
            }
            return;
        }

        case 2: {
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
            h                      = func_ov003_0208a114(0xB9);
            if (func_ov003_0208a164(h, &data->actor.unk_04, data->actor.position.x, data->actor.position.y,
                                    data->actor.position.z) == 1)
            {
                data->unk_1F0 = 0;
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 >= 0) {
                return;
            }
            if (data->actor.position.z < 0) {
                return;
            }
            data->unk_1E8 = (data->unk_084.flags46 & 1) ? -0x8000 : 0x8000;
            Mini108_VBlank(&data->unk_084, 0xC, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            return;
        }

        case 3: {
            s32 target[2];

            func_ov003_020ccf1c(&data->unk_1E8, -0x400);
            data->actor.position.x = data->actor.position.x + data->unk_1E8;
            if (data->unk_1E8 != 0) {
                return;
            }
            func_ov003_020c5924(data, target);
            data->unk_1AC = target[0];
            data->unk_1B0 = target[1];
            Mini108_VBlank(&data->unk_084, 2, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            return;
        }

        case 4: {
            s32 angle = FX_Atan2Idx(data->unk_1B0 - data->actor.position.y, data->unk_1AC - data->actor.position.x) >> 4;
            s16 sin   = data_0205e4e0[angle * 2 + 1];
            s16 cos   = data_0205e4e0[angle * 2];
            s32 mag   = Mth_MulFixed((sin < 0) ? -sin : sin, 0x2000);

            mag += 0x1000;
            data->unk_1E8          = Mth_MulFixed(sin, mag);
            data->unk_1EC          = Mth_MulFixed(cos, mag);
            data->actor.position.x = data->actor.position.x + data->unk_1E8;
            if (data->unk_1E8 > 0) {
                if (data->actor.position.x > data->unk_1AC) {
                    data->actor.position.x = data->unk_1AC;
                }
            } else if (data->actor.position.x < data->unk_1AC) {
                data->actor.position.x = data->unk_1AC;
            }
            data->actor.position.y = data->actor.position.y + data->unk_1EC;
            if (data->unk_1EC > 0) {
                if (data->actor.position.y > data->unk_1B0) {
                    data->actor.position.y = data->unk_1B0;
                }
            } else if (data->actor.position.y < data->unk_1B0) {
                data->actor.position.y = data->unk_1B0;
            }
            func_ov003_020c4ab4(data, (data->unk_1E8 > 0) ? 1 : 0);
            if (data->unk_1AC != data->actor.position.x || data->unk_1B0 != data->actor.position.y) {
                return;
            }
            data->unk_1F0 = data->unk_1F0 - 1;
            func_ov003_020c4b1c(data);
            if (data->unk_1F0 <= 0) {
                func_ov015_02125a64(data, (s32)func_ov015_021268b0);
            } else {
                func_ov015_02125a64(data, (s32)func_ov015_0212699c);
            }
            return;
        }
    }
}

void func_ov015_02126db8(BtlEnm026* data) {
    if (data_ov015_02128500.variant == 0x11) {
        func_ov015_02125a64(data, (s32)func_ov015_021268b0);
        return;
    }
    if ((data_ov015_02128500.flag08 != 0 && data_ov015_02128500.variant == 3) || data_ov015_02128500.result10 == -1) {
        if (data->unk_1D4 == 1 && data->unk_1E4 == 0) {
            func_ov003_020c4cc4(data, 0x27A);
            data->unk_1E4 = 1;
        }
        func_ov015_02125a64(data, (s32)func_ov015_02126ec4);
    } else {
        if (data_ov015_02128500.variant == 0xB) {
            return;
        }
        switch (data->unk_1C2) {
            case 0:
                Mini108_VBlank(&data->unk_084, 0, 0);
                data->unk_1C2 = data->unk_1C2 + 1;
                data->unk_1C0 = 0;
                break;

            case 1:
                if (RNG_Next(data->unk_19E) != 0) {
                    return;
                }
                func_ov015_02125a64(data, (s32)func_ov015_02125f48);
                break;
        }
    }
}

void func_ov015_02126ec4(BtlEnm026* data) {
    if (data_ov015_02128500.result10 == -1) {
        Mini108_VBlank(&data->unk_084, 3, 0);
    } else {
        Mini108_VBlank(&data->unk_084, 2, 0);
    }
    data->actor.position.x += data->unk_1D0 >> 1;
    func_ov003_020c4ab4(data, (data->unk_1D0 > 0) ? 1 : 0);
    data->unk_1AC = data->actor.position.x;
    data->unk_1B0 = data->actor.position.y;
    data->unk_1B4 = data->actor.position.z;
}

s32 func_ov015_02126f44(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm026* data = task->data;
    s32        ret  = 1;

    switch (stage) {
        case 0:
            MI_CpuSet(data, 0, 0x1F4);
            func_ov015_02125a8c(1, data, args, (s32)func_ov015_02126db8);
            break;

        case 1:
            ret = func_ov015_02125b08(data);
            break;

        case 2:
            func_ov003_020c4878(data);
            break;

        case 3:
            data_ov015_02128500.initialized = 0;
            func_ov003_020c48fc(data);
            break;
    }
    return ret;
}

void func_ov015_02126fd8(BtlEnm026* data) {
    BtlEnm026* boss = (BtlEnm026*)data_ov003_020e71b8->unk3D898;

    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 2, 0);
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
            Mini108_VBlank(&data->unk_084, 0xA, 1);
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
                func_ov003_02082d04(&data->unk_084);
                return;
            }
            Mini108_VBlank(&data->unk_084, 0xB, 0);
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
            data->unk_1E8 = (data->unk_084.flags46 & 1) ? -0x8000 : 0x8000;
            Mini108_VBlank(&data->unk_084, 0xC, 1);
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
        s16 count;

        data->unk_1D8 = 0;
        count         = RNG_Next(3) + 3;
        data->unk_1F0 = count;
        func_ov015_02125a64(data, (s32)func_ov015_02126fd8);
        return;
    }
    if (data_ov015_02128500.flag08 != 0 || data_ov015_02128500.result10 != -1) {
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
            Mini108_VBlank(&data->unk_084, 0, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = func_ov003_020c42ec(data);
            break;

        case 1: {
            u32 r;
            s16 sin;
            s16 cos;

            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 >= 0) {
                return;
            }
            Mini108_VBlank(&data->unk_084, 2, 0);
            r             = RNG_Next(0xFFFF);
            sin           = data_0205e4e0[(r >> 4) * 2 + 1];
            cos           = data_0205e4e0[(r >> 4) * 2];
            data->unk_1E8 = Mth_MulFixed(sin, data->unk_1D0);
            data->unk_1EC = Mth_MulFixed(cos, data->unk_1D0) >> 1;
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
    if (data_ov015_02128500.result10 == -1) {
        Mini108_VBlank(&data->unk_084, 3, 0);
    } else {
        Mini108_VBlank(&data->unk_084, 2, 0);
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
        void* node = func_ov003_0208495c((u8*)data_ov003_020e71b8 + 0x3D8BC);
        node       = func_ov003_02084984((u8*)node + 0x108);
        while (node != 0) {
            if (*(s16*)((u8*)node + 0x44) < *(s16*)((u8*)node + 0x46)) {
                u32 id = *(u32*)((u8*)node + 0x0);
                if (id != 4) {
                    data_ov015_02128500.unk_24 = (u32)node;
                    return;
                }
            }
            node = func_ov003_02084984((u8*)node + 0x108);
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
            if (*(s16*)((u8*)data_ov003_020e71b8 + 0x3D8F8) != 0) {
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
        if (*(s16*)((u8*)data_ov003_020e71b8 + 0x3D8F8) != 0) {
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
    if (data_ov015_02128500.unk14 != 2) {
        return;
    }
    func_ov015_02125a64(twin->unk_188, (s32)func_ov015_02125e5c);
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
        *(u32*)&twin->unk_18C &= ~0x1;
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
    void*      node;

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
    if (data->unk_1C4 == (void (*)(BtlEnm026*))data->unk_1C8) {
        if (RNG_Next(data->unk_19E) == 0) {
            func_ov015_02125a64(data, (s32)func_ov015_02125f48);
        }
    }
    if (data->unk_1D4 != 1 || data_ov015_02128500.unk14 != 3) {
        return;
    }
    node = func_ov003_0208495c((u8*)data_ov003_020e71b8 + 0x3D8A4);
    while (node != 0) {
        func_ov003_020c4fc8(node);
        node = func_ov003_02084984((u8*)node + 0x178);
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
    Mini108_VBlank(&data->unk_084, 1, 0);
    data->unk_196 &= ~0x2;
}

void func_ov015_02127ca0(BtlEnm026* data) {
    data->actor.pendingCommand = 0;
    if ((u32)data_ov015_02128500.unk14 - 1 <= 2) {
        data_ov015_02128500.result10 = -1;
    }
    if (SystemStatusFlags.unk_10 == 0) {
        return;
    }
    if (*(u8*)((u8*)data_ov003_020e71b8 + 0x3D874) != 2) {
        return;
    }
    if (data_ov015_02128500.result10 == -1) {
        return;
    }
    func_ov003_02082f1c(data, 3);
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
    if (*(u8*)((u8*)data_ov003_020e71b8 + 0x3D874) != 2) {
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
    if (*(u32*)((u8*)data_ov003_020e71b8 + 0x3D79C) == 0) {
        return;
    }
    if (*(u8*)((u8*)data_ov003_020e71b8 + 0x3D874) == 2) {
        data->actor.pendingCommand = 3;
    }
}
