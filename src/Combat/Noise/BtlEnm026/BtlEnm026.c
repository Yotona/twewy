#include "Combat/Noise/Private/BtlEnm026.h"
#include "Combat/Core/Combat.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

typedef struct Enm026Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u16              unk_08;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u32              unk_0C;
} Enm026Variant; // Size: 0x10

typedef struct Enm026IdPair {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} Enm026IdPair;

typedef struct Enm026ParticleParams {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
} Enm026ParticleParams; // Size: 0x14

typedef struct Enm026StateFns {
    /* 0x00 */ void (*unk_00)();
    /* 0x04 */ void (*unk_04)();
} Enm026StateFns;

extern s32  func_ov003_020c37f8(void*);
extern void func_ov003_020c3efc(void*, void*);
extern void func_ov003_020c427c(void*);
extern void func_ov003_020c4628(void*);
extern void func_ov003_020c4668(void*);
extern void func_ov003_020c4cc4(void*, s32);
extern void func_ov003_020ccea8(void*, void*);
extern s32  func_ov003_020c5bfc(void*);
extern s32  func_ov003_020ccfec(void*);
extern s32  func_ov003_020c72b4(void*, s32, s32);
extern void func_ov003_02084694(void*, s32);
extern s32  func_ov003_020ccedc(s32);
extern s32  func_ov003_0208a08c(s32, void*, s32);
extern void func_ov003_020ccf94(s32, void*, void*);
extern void func_ov003_020c4af0(void*);
extern void func_ov003_020ccf58(s32, void*);
extern s32  func_ov003_020ccfc8(void*, s32);
extern s32  func_ov003_020c6b8c(void*, s32);
extern s32  func_ov003_020c6bc8(void*, s32);
extern s32  func_ov003_020c6c2c(void*, s32);
extern s32  func_ov003_020c5e98(void*);
extern void func_ov003_020c3cec(void*, void*, void*, void*);
extern void func_ov003_020ccec0(void*, s32);
extern s32  func_ov003_020cd010(void*);
s32         func_ov015_02125c94(BtlEnm026*);
void        func_ov015_02125e5c(BtlEnm026*);
void        func_ov015_02125f00(BtlEnm026*);
void        func_ov015_02126070(BtlEnm026*);
void        func_ov015_021260c4(BtlEnm026*);
void        func_ov015_02126158(BtlEnm026*);
void        func_ov015_02126184(BtlEnm026*);
void        func_ov015_021261ac(BtlEnm026*);

extern char data_ov015_021282e0[28];

extern char data_ov015_021282fc[28];
extern char data_ov015_02128318[28];
extern char data_ov015_02128334[28];
extern char data_ov015_02128350[28];
extern char data_ov015_0212836c[28];
extern char data_ov015_02128388[28];
extern char data_ov015_021283a4[28];
extern char data_ov015_021283c0[28];
extern char data_ov015_021283dc[20];

extern const BinIdentifier data_ov015_02128078;

extern const BinIdentifier   data_ov015_02128080;
extern const BinIdentifier   data_ov015_02128088;
extern const BinIdentifier   data_ov015_02128090;
extern const BinIdentifier   data_ov015_02128098;
extern const BinIdentifier   data_ov015_021280a0;
extern const BinIdentifier   data_ov015_021280a8;
extern const BinIdentifier   data_ov015_021280b0;
extern const BinIdentifier   data_ov015_021280b8;
extern const BinIdentifier   data_ov015_021280c0;
extern const SpriteAnimEntry data_ov015_021280c8[13];

const Enm026Variant data_ov015_02127fd8 = {&data_ov015_02128078, data_ov015_021280c8, 0, 0x0D, 0x40};

const Enm026Variant data_ov015_02127fe8 = {&data_ov015_02128088, data_ov015_021280c8, 0, 0x0D, 0x44};

const Enm026Variant data_ov015_02127ff8 = {&data_ov015_021280a0, data_ov015_021280c8, 0, 0x0D, 0x30};

const Enm026Variant data_ov015_02128008 = {&data_ov015_021280b0, data_ov015_021280c8, 0, 0x0D, 0x42};

const Enm026Variant data_ov015_02128018 = {&data_ov015_02128090, data_ov015_021280c8, 0, 0x0D, 0x30};

const Enm026Variant data_ov015_02128028 = {&data_ov015_02128080, data_ov015_021280c8, 0, 0x0D, 0x40};

const Enm026Variant data_ov015_02128038 = {&data_ov015_021280c0, data_ov015_021280c8, 0, 0x10, 0x44};

const Enm026Variant data_ov015_02128048 = {&data_ov015_02128098, data_ov015_021280c8, 0, 0x0D, 0x30};

const Enm026Variant data_ov015_02128058 = {&data_ov015_021280b8, data_ov015_021280c8, 0, 0x0D, 0x46};

const Enm026Variant data_ov015_02128068 = {&data_ov015_021280a8, data_ov015_021280c8, 0, 0x0D, 0x42};

const BinIdentifier data_ov015_02128078 = {3, data_ov015_021282e0};

const BinIdentifier data_ov015_02128080 = {3, data_ov015_021282fc};

const BinIdentifier data_ov015_02128088 = {3, data_ov015_021283dc};

const BinIdentifier data_ov015_02128090 = {3, data_ov015_02128318};

const BinIdentifier data_ov015_02128098 = {3, data_ov015_02128334};

const BinIdentifier data_ov015_021280a0 = {3, data_ov015_02128350};

const BinIdentifier data_ov015_021280a8 = {3, data_ov015_0212836c};

const BinIdentifier data_ov015_021280b0 = {3, data_ov015_02128388};

const BinIdentifier data_ov015_021280b8 = {3, data_ov015_021283a4};

const BinIdentifier data_ov015_021280c0 = {3, data_ov015_021283c0};

const SpriteAnimEntry data_ov015_021280c8[13] = {
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

const Enm026IdPair data_ov015_02128130[13] = {
    {0x11,  0x0},
    {0x12,  0x4},
    {0x13,  0x1},
    {0x14,  0xB},
    {0x15,  0x2},
    {0x16,  0x6},
    {0x17,  0x5},
    {0x18,  0x8},
    {0x19,  0x9},
    {0x1A,  0xA},
    {0x1B,  0x7},
    {0x1C, 0x11},
    {0x1D,  0x3},
};

const Enm026StateFns data_ov015_02128198[18] = {
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

char data_ov015_021282e0[28] = "Apl_Hor/Grp_BtlEnm026.bin";

char data_ov015_021282fc[28] = "Apl_Hor/Grp_BtlEnm026a.bin";

char data_ov015_02128318[28] = "Apl_Hor/Grp_BtlEnm026c.bin";

char data_ov015_02128334[28] = "Apl_Hor/Grp_BtlEnm026d.bin";

char data_ov015_02128350[28] = "Apl_Hor/Grp_BtlEnm026e.bin";

char data_ov015_0212836c[28] = "Apl_Hor/Grp_BtlEnm026f.bin";

char data_ov015_02128388[28] = "Apl_Hor/Grp_BtlEnm026g.bin";

char data_ov015_021283a4[28] = "Apl_Hor/Grp_BtlEnm026h.bin";

char data_ov015_021283c0[28] = "Apl_Hor/Grp_BtlEnm026i.bin";

char data_ov015_021283dc[20] = "Apl_Hor/Grp_BtlEnm02";

char data_ov015_021283f0[8] = "6b.bin";

const Enm026Variant* data_ov015_021283f8[11] = {
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8,
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8,
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fd8,
};

const Enm026Variant* data_ov015_02128424[18] = {
    &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02127fe8, &data_ov015_02128058, &data_ov015_02128028,
    &data_ov015_02128018, &data_ov015_02128018, &data_ov015_02128008, &data_ov015_02128048, &data_ov015_02127ff8,
    &data_ov015_02128068, &data_ov015_02127fd8, &data_ov015_02127fd8, &data_ov015_02128068, &data_ov015_02128068,
    &data_ov015_02128068, &data_ov015_02127fd8, &data_ov015_02128038,
};

Enm026State data_ov015_02128500;

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
            data_ov003_020e71b8->unk3D875 = 2;
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
        if (data->unk_084.sprite.cellIndex == 1 && data->unk_084.sprite.frameTimer == 1 && data->unk_1D4 == 1) {
            func_ov003_020c4cc4(data, 0x27B);
        }
    } else if (data->unk_084.animTableIndex == 3) {
        if (data->unk_084.sprite.cellIndex == 4 && data->unk_084.sprite.frameTimer == 1 && data->unk_1D4 == 1) {
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
    blockA.unk_00 = blockB.unk_00 = data->unk_07C;
    blockA.unk_06 &= ~0x1;
    blockB.unk_06 = (blockB.unk_06 & ~0x1) | (blockA.unk_06 & 0x1);
    blockA.unk_04 = data->unk_080;
    blockB.unk_04 = data->unk_080;
    blockA.unk_10 = 0;
    blockB.unk_10 = 0;
    if (count <= 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        blockB.unk_08 = RNG_Next(data_ov003_020e71b8->unk3D7C0[0].unk_0C >> 12) << 12;
        blockB.unk_0C = RNG_Next(data_ov003_020e71b8->unk3D7C0[0].unk_10 >> 12) << 12;
        blockA.unk_08 = RNG_Next(data_ov003_020e71b8->unk3D7C0[1].unk_0C >> 12) << 12;
        blockA.unk_0C = RNG_Next(data_ov003_020e71b8->unk3D7C0[1].unk_10 >> 12) << 12;
        func_ov003_020c3cec(handleA, handleB, (handleA != 0) ? &blockB : NULL, (handleB != 0) ? &blockA : NULL);
    }
}

void func_ov015_02125e5c(BtlEnm026* data) {
    s32 ret;

    if (func_ov003_020c37f8(&data->unk_084) != 0) {
        ret = func_ov003_020ccfc8(data, 4);
    } else if (data_ov015_02128500.variant != 7) {
        if (data->unk_1C0 == 0) {
            CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);
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

        case 12:
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
