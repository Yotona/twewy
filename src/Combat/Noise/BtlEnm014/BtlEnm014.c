#include "Combat/Noise/Private/BtlEnm014.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "Save.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

extern s32  func_ov003_02082f2c(void*);
extern void func_ov003_02082b0c(void*);
extern void func_ov003_02082b64(void*);
extern void func_ov003_02082cc4(void*);
extern void func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern void func_ov003_02084694(void*, s32);
extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c3c28(void);
extern void func_ov003_020c3efc(void*, void*);
extern void func_ov003_020c427c(void*);
extern s32  func_ov003_020c42ec(void*);
extern void func_ov003_020c4628(void*);
extern void func_ov003_020c4668(void*);
extern void func_ov003_020c4878(void*);
extern void func_ov003_020c48b0(void*);
extern void func_ov003_020c48fc(void*);
extern void func_ov003_020c492c(void*);
extern void func_ov003_020c4b1c(void*);
extern void func_ov003_020c4b5c(void*);
extern void func_ov003_020c4cc4(void*, s32);
extern s32  func_ov003_020c4e0c(void*);
extern void func_ov003_020c4ee0(void*);
extern s32  func_ov003_020c59a0(void*);
extern s32  func_ov003_020c5bfc(void*);
extern s32  func_ov003_020c5e98(void*);
extern s32  func_ov003_020c6230(void*);
extern s32  func_ov003_020c6bac(void*, s32);
extern s32  func_ov003_020c6bc8(void*, s32);
extern s32  func_ov003_020c6c2c(void*, s32);
extern s32  func_ov003_020c72b4(void*, s32, s32);
extern void func_ov003_020ccea8(void*, void*);
extern void func_ov003_020ccf1c(void*, s32);
extern void func_ov003_020ccf58(s32, void*);
extern s32  func_ov003_020ccfc8(void*, s32);
extern s32  func_ov003_020ccfec(void*);
extern s32  func_ov003_020cd11c(s32);

// The sprite animation setter is referenced by its ov000 linker symbol in the original binary.
extern void Mini108_VBlank(CombatSprite* cSprite, u16 animTableIndex, s32 arg2);

extern s16 data_0205e4e0[];

/// Rounds a fixed-point value through a float, matching the original codegen.
#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

/// Absolute value, spelled to match the original's compare/negate pair.
#define ABSVAL(value) ((value) < 0 ? -(value) : (value))

// MARK: Forward declarations

BinIdentifier*         func_ov012_021256c0(s32);
const SpriteAnimEntry* func_ov012_021256d0(void);
u16                    func_ov012_021256dc(s32);
void                   func_ov012_021256f0(s32, BtlEnm014*, s32);
void                   func_ov012_02125768(BtlEnm014*, void (*)(BtlEnm014*));
void                   func_ov012_02125790(s32, BtlEnm014*, s32, s32);
void                   func_ov012_021257c4(BtlEnm014*);
void                   func_ov012_021257e8(BtlEnm014*);
void                   func_ov012_02125810(BtlEnm014*);
void                   func_ov012_0212582c(BtlEnm014*);
void                   func_ov012_02125858(BtlEnm014*);
void                   func_ov012_021258e8(void);
void                   func_ov012_02125958(BtlEnm014*);
s32                    func_ov012_021259bc(BtlEnm014Eff*, Enm014Spawn*);
s32                    func_ov012_02125ab4(TaskPool*, Task*, void*, s32);
s32                    func_ov012_02125c48(BtlEnm014*);
void                   func_ov012_02125d84(BtlEnm014*);
void                   func_ov012_02125de4(BtlEnm014*);
void                   func_ov012_021265a4(BtlEnm014*);
void                   func_ov012_0212684c(BtlEnm014*);
s32                    func_ov012_02126898(BtlEnm014*);
s32                    func_ov012_02126968(TaskPool*, Task*, void*, s32);
s32                    func_ov012_02126a1c(BtlEnm014*);
s32                    func_ov012_02126a4c(BtlEnm014*);
s32                    func_ov012_02126ad8(BtlEnm014*);
void                   func_ov012_02126b74(BtlEnm014*);
void                   func_ov012_02126bb0(BtlEnm014*);
void                   func_ov012_02126bd8(BtlEnm014*);
s32                    func_ov012_02126c1c(BtlEnm014*);
void                   func_ov012_02126c74(BtlEnm014*);
void                   func_ov012_02126dac(BtlEnm014*);
void                   func_ov012_02126e2c(BtlEnm014*);
void                   func_ov012_02126ea0(BtlEnm014*);
void                   func_ov012_02127134(BtlEnm014*);
void                   func_ov012_02127378(BtlEnm014*);
void                   func_ov012_02127608(BtlEnm014*);
void                   func_ov012_02127634(BtlEnm014*);
void                   func_ov012_02127660(BtlEnm014*);
s32                    func_ov012_0212768c(BtlEnm014*);
s32                    func_ov012_021277b0(TaskPool*, Task*, void*, s32);

// MARK: Data

// The asset name strings and the variant/anim tables are referenced by the
// variant records declared first, so they are forward-declared here.
extern char data_ov012_02127990[];
extern char data_ov012_021279ac[];
extern char data_ov012_021279c8[];
extern char data_ov012_021279e4[];
extern char data_ov012_02127a00[];
extern char data_ov012_02127a14[];
extern char data_ov012_02127a28[];

BinIdentifier   data_ov012_02127894;
BinIdentifier   data_ov012_0212789c;
BinIdentifier   data_ov012_021278a4;
BinIdentifier   data_ov012_021278ac;
SpriteAnimEntry data_ov012_021278b4[8][2];

u16 data_ov012_0212783c[4] = {0x10, 0x16, 0x16, 0x16};

Enm014Variant data_ov012_02127844 = {&data_ov012_02127894, &data_ov012_021278b4[0][0], 0, 0x10, 0xD8, 0};

SpriteAnimEntry data_ov012_02127854[2] = {
    {   4,    6,    5, 0},
    {0x13, 0x15, 0x14, 0},
};

Enm014Variant data_ov012_02127864 = {&data_ov012_0212789c, &data_ov012_021278b4[0][0], 0, 0x16, 0xF0, 0};
Enm014Variant data_ov012_02127874 = {&data_ov012_021278ac, &data_ov012_021278b4[0][0], 0, 0x16, 0x104, 0};
Enm014Variant data_ov012_02127884 = {&data_ov012_021278a4, &data_ov012_021278b4[0][0], 0, 0x16, 0x104, 0};

BinIdentifier data_ov012_02127894 = {3, data_ov012_02127990};
BinIdentifier data_ov012_0212789c = {3, data_ov012_021279ac};
BinIdentifier data_ov012_021278a4 = {3, data_ov012_021279c8};
BinIdentifier data_ov012_021278ac = {3, data_ov012_021279e4};

SpriteAnimEntry data_ov012_021278b4[8][2] = {
    {         {1, 3, 2, 0},          {1, 3, 2, 1}},
    {         {7, 9, 8, 0},          {7, 9, 8, 1}},
    {   {0xA, 0xC, 0xB, 0},    {0xA, 0xC, 0xB, 1}},
    {   {0xA, 0xC, 0xB, 2},    {0xA, 0xC, 0xB, 3}},
    {   {0xA, 0xC, 0xB, 4},    {0xA, 0xC, 0xB, 0}},
    {   {0xA, 0xC, 0xB, 5},    {0xA, 0xC, 0xB, 2}},
    {   {0xD, 0xF, 0xE, 0},    {0xD, 0xF, 0xE, 1}},
    {{0x10, 0x12, 0x11, 0}, {0x10, 0x12, 0x11, 1}},
};

const TaskHandle Tsk_BtlEnm014_Eff = {data_ov012_02127a00, func_ov012_02125ab4, 0x7C};
const TaskHandle Tsk_BtlEnm014_RG  = {data_ov012_02127a14, func_ov012_02126968, 0x1E8};

s32 data_ov012_0212794c[4] = {0x64, 0x32, 0x32, 0x41};

const TaskHandle Tsk_BtlEnm014_UG = {data_ov012_02127a28, func_ov012_021277b0, 0x1F0};

Enm014Variant* data_ov012_02127980[4] = {
    &data_ov012_02127844,
    &data_ov012_02127864,
    &data_ov012_02127884,
    &data_ov012_02127874,
};

char data_ov012_02127990[] = "Apl_Hor/Grp_BtlEnm014.bin";
char data_ov012_021279ac[] = "Apl_Hor/Grp_BtlEnm014a.bin";
char data_ov012_021279c8[] = "Apl_Hor/Grp_BtlEnm014b.bin";
char data_ov012_021279e4[] = "Apl_Hor/Grp_BtlEnm014c.bin";

char data_ov012_02127a00[] = "Tsk_BtlEnm014_Eff";
char data_ov012_02127a14[] = "Tsk_BtlEnm014_RG";
char data_ov012_02127a28[] = "Tsk_BtlEnm014_UG";

// MARK: Functions

BinIdentifier* func_ov012_021256c0(s32 index) {
    return (BinIdentifier*)((u8*)&data_ov012_02127894 + index * 8);
}

const SpriteAnimEntry* func_ov012_021256d0(void) {
    return data_ov012_02127854;
}

u16 func_ov012_021256dc(s32 index) {
    return data_ov012_0212783c[index];
}

void func_ov012_021256f0(s32 engine, BtlEnm014* data, s32 arg2) {
    Enm014Spawn spawn;
    MI_CpuSet(&spawn, 0, sizeof(spawn));
    spawn.unk_00 = data;
    spawn.unk_04 = engine;
    spawn.unk_08 = (u16)arg2;

    TaskPool* pool;
    if (engine == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm014_Eff, NULL, 0, NULL, &spawn);
}

void func_ov012_02125768(BtlEnm014* data, void (*fn)(BtlEnm014*)) {
    func_ov003_020c427c(data);
    data->unk_1C4 = fn;
    data->unk_1C2 = 0;
    data->unk_1C0 = 0;
}

void func_ov012_02125790(s32 engine, BtlEnm014* data, s32 arg2, s32 arg3) {
    data->unk_1CC = 1;
    data->unk_1C8 = arg3;
    func_ov003_020c3efc(data, (void*)arg2);
    func_ov003_020ccf58(engine, data);
}

void func_ov012_021257c4(BtlEnm014* data) {
    if (func_ov003_020c5bfc(data) != 0) {
        return;
    }
    func_ov012_02125768(data, (void (*)(BtlEnm014*))data->unk_1C8);
}

void func_ov012_021257e8(BtlEnm014* data) {
    if (func_ov003_020ccfc8(data, 4) != 0) {
        return;
    }
    func_ov012_02125768(data, (void (*)(BtlEnm014*))data->unk_1C8);
}

void func_ov012_02125810(BtlEnm014* data) {
    if (func_ov003_020ccfec(data) == 0) {
        data->unk_1CC = 0;
    }
}

void func_ov012_0212582c(BtlEnm014* data) {
    if (func_ov003_020c72b4(data, 0, 7) != 0) {
        return;
    }
    func_ov012_02125768(data, (void (*)(BtlEnm014*))data->unk_1C8);
}

void func_ov012_02125858(BtlEnm014* data) {
    BtlEnm014* boss;

    if (data->unk_084.sprite.bits_0_1 == 0) {
        boss = data_ov003_020e71b8->unk3D898;
    } else {
        boss = data_ov003_020e71b8->unk3D89C;
    }
    data->actor.unk_62 = 100;
    if (data->unk_084.flags46 & 1) {
        if (data->actor.position.x < boss->actor.position.x) {
            goto hit;
        }
    }
    if (!(data->unk_084.flags46 & 1)) {
        if (data->actor.position.x > boss->actor.position.x) {
            goto hit;
        }
    }
    return;

hit:
    data->actor.unk_62 = 50;
    if (data->unk_080 == 3) {
        data->actor.unk_62 = 0;
    }
}

void func_ov012_021258e8(void) {
    if (*(u16*)((u8*)data_ov003_020e71b8 + 0x3D88E) != 0x90) {
        return;
    }
    if (*(u32*)((u8*)&gSaveData + 0x3188) <= 0x654) {
        return;
    }
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 |= 0x200000;
    data_ov003_020e71b8->unk3D878 |= 0x20000000;
}

void func_ov012_02125958(BtlEnm014* data) {
    if (data->unk_1D4 == 0 && data->actor.unk_5A != 0 && data->actor.unk_62 != 0x64) {
        func_ov012_021256f0(data->unk_084.sprite.bits_0_1, data, 0);
        func_ov003_020c4cc4(data, 0x21E);
    }
    data->unk_1D4 = data->actor.unk_5A;
}

s32 func_ov012_021259bc(BtlEnm014Eff* data, Enm014Spawn* args) {
    u16 unk_06;

    MI_CpuSet(data, 0, sizeof(BtlEnm014Eff));
    data->unk_70 = args->unk_00;
    data->unk_74 = args->unk_04;
    data->unk_78 = args->unk_08;
    data->unk_64 = data->unk_70->actor.position.x;
    data->unk_68 = data->unk_70->actor.position.y;
    data->unk_60 = 0;

    if (args->unk_08 == 1) {
        unk_06 = (args->unk_00->unk_080 == 3) ? 0xC8 : 0xB0;
    } else {
        unk_06 = 0x50;
    }

    CombatSprite_LoadFromTable(args->unk_04, &data->sprite, func_ov012_021256c0(args->unk_00->unk_080), func_ov012_021256d0(),
                               args->unk_08, func_ov012_021256dc(args->unk_00->unk_080), unk_06);
    Mini108_VBlank(&data->sprite, args->unk_08, 1);
    CombatSprite_SetFlip(&data->sprite, data->unk_70->unk_084.flags46 & 1);
    return 1;
}

s32 func_ov012_02125ab4(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm014Eff* data = task->data;

    switch (stage) {
        case 0:
            func_ov012_021259bc(data, (Enm014Spawn*)args);
            break;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 0;
            }
            if (data->sprite.animTableIndex == 1) {
                if (func_ov003_020c59a0(data->unk_70) != 0) {
                    return 0;
                }
                data->unk_60 = data->unk_60 + 1;
                if (data->unk_60 < 0x31) {
                    data->unk_64 = data->unk_70->actor.position.x;
                    data->unk_68 = data->unk_70->actor.position.y;
                }
                data->unk_6C = 0;
            } else {
                if (data->unk_70->actor.flags & 4) {
                    return 0;
                }
                CombatSprite_SetFlip(&data->sprite, data->unk_70->unk_084.flags46 & 1);
                data->unk_64 = data->unk_70->actor.position.x;
                data->unk_68 = data->unk_70->actor.position.y;
                data->unk_6C = data->unk_70->actor.position.z;
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                return 0;
            }
            CombatSprite_Update(&data->sprite);
            break;
        case 2: {
            s16 x;
            s16 y;
            func_ov003_02084348(func_ov003_020c37f8(&data->sprite) != 0, &x, &y, data->unk_64, data->unk_68, data->unk_6C);
            CombatSprite_SetPosition(&data->sprite, x, y);
            func_ov003_02082730(&data->sprite, 0x7FFFFFFF - data->unk_68);
            CombatSprite_Render(&data->sprite);
        } break;
        case 3:
            CombatSprite_Release(&data->sprite);
            break;
    }
    return 1;
}

s32 func_ov012_02125c48(BtlEnm014* data) {
    s32 flag = (RNG_Next(data->unk_1A0) == 0);

    data->unk_1E0.half.lo = data->unk_1E0.half.lo - 1;
    data->unk_1E0.half.hi = data->unk_1E0.half.hi - 1;

    if (func_ov003_020c4e0c(data) != 0 && data->unk_1E0.half.hi < 0 && flag != 0) {
        data->unk_1E0.half.hi = data->unk_19E + RNG_Next(data->unk_1A2);
        if (RNG_Next(0x64) < data_ov012_0212794c[data->unk_080]) {
            func_ov012_02125768(data, func_ov012_02125de4);
        } else {
            func_ov012_02125768(data, func_ov012_021265a4);
        }
        return 1;
    }

    if (data->unk_1E0.half.lo < 0 && flag != 0) {
        data->unk_1E0.half.lo = func_ov003_020c42ec(data);
        if ((u16)(data->unk_080 + 0xFFFE) <= 1) {
            data->unk_1E0.half.lo = func_ov003_020c42ec(data);
        }
        func_ov012_02125768(data, func_ov012_0212684c);
        return 1;
    }
    return 0;
}

void func_ov012_02125d84(BtlEnm014* data) {
    func_ov003_020c4b1c(data);
    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 0, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            break;

        case 1:
            func_ov012_02125c48(data);
            break;
    }
}

void func_ov012_02125de4(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D89C;

    switch (data->unk_1C2) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;

        case 1: {
            s32 vel;
            Mini108_VBlank(&data->unk_084, 2, 1);
            vel           = boss->actor.position.x - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            data->unk_084.flags46 &= ~1;
            if (data->unk_1D8 > 0) {
                data->unk_084.flags46 |= 1;
            }
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x1E;
            return;
        }

        case 2: {
            s32 vel;
            data->actor.position.x = data->actor.position.x + data->unk_1D8;
            func_ov003_020ccf1c(&data->unk_1D8, 0xFFFFFFD7);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (data->unk_1C0 == 0x1E) {
                func_ov003_020c4cc4(data, 0x218);
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 > 0) {
                return;
            }
            if (ABSVAL(data->actor.position.x - boss->actor.position.x) < 0x3C000) {
                Mini108_VBlank(&data->unk_084, 0xC, 1);
                data->unk_1D0 = 0;
                data->unk_1C2 = 4;
                data->unk_1C0 = 0;
                return;
            }
            vel           = boss->actor.position.x - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            Mini108_VBlank(&data->unk_084, 3, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x1E;
            return;
        }

        case 3: {
            s32 vel;
            data->actor.position.x = data->actor.position.x + data->unk_1D8;
            func_ov003_020ccf1c(&data->unk_1D8, 0xFFFFFFD7);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (data->unk_1C0 == 0x1E) {
                func_ov003_020c4cc4(data, 0x218);
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 > 0) {
                return;
            }
            vel           = boss->actor.position.x - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            Mini108_VBlank(&data->unk_084, 2, 1);
            data->unk_1C2 = data->unk_1C2 - 1;
            data->unk_1C0 = 0x1E;
            return;
        }

        case 4: {
            s32 d;
            s32 h;
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1C0 == 0x28) {
                func_ov003_020c4cc4(data, 0x219);
            }
            if (data->unk_1C0 > 0x28 && data->unk_1C0 < 0x32 && data->unk_1D0 == 0) {
                d             = ROUND(func_ov003_020cd11c(0x85));
                d             = (data->unk_084.flags46 & 1) ? d : -d;
                h             = func_ov003_0208a114(0x85);
                data->unk_1D0 = func_ov003_0208a164(h, &data->actor.unk_04, data->actor.position.x + d, data->actor.position.y,
                                                    data->actor.position.z);
                if (data->unk_1D0 == 1) {
                    func_ov003_020c4cc4(data, 0x21A);
                    boss->actor.zVelocity = Mth_MulFixed(boss->actor.zVelocity, 0x1800);
                    boss->actor.yVelocity = Mth_MulFixed(boss->actor.yVelocity, 0x1800);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            Mini108_VBlank(&data->unk_084, 0xD, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;
        }

        case 5: {
            s32 vel;
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            func_ov003_020c4ee0(data);
            Mini108_VBlank(&data->unk_084, 2, 1);
            vel           = data->unk_1AC - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            data->unk_084.flags46 &= ~1;
            if (data->unk_1D8 > 0) {
                data->unk_084.flags46 |= 1;
            }
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x1E;
            return;
        }

        case 6: {
            s32 vel;
            data->actor.position.x = data->actor.position.x + data->unk_1D8;
            if (data->unk_084.flags46 & 1) {
                if (data->actor.position.x >= data->unk_1AC) {
                    func_ov003_020c4b1c(data);
                    data->unk_1AC = data->actor.position.x;
                    func_ov012_02125768(data, func_ov012_02125d84);
                    return;
                }
            } else if (data->actor.position.x <= data->unk_1AC) {
                func_ov003_020c4b1c(data);
                data->unk_1AC = data->actor.position.x;
                func_ov012_02125768(data, func_ov012_02125d84);
                return;
            }
            func_ov003_020ccf1c(&data->unk_1D8, 0xFFFFFFD7);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (data->unk_1C0 == 0x1E) {
                func_ov003_020c4cc4(data, 0x218);
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 > 0) {
                return;
            }
            vel           = boss->actor.position.x - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            Mini108_VBlank(&data->unk_084, 3, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x1E;
            return;
        }

        case 7: {
            s32 vel;
            data->actor.position.x = data->actor.position.x + data->unk_1D8;
            if (data->unk_084.flags46 & 1) {
                if (data->actor.position.x >= data->unk_1AC) {
                    func_ov003_020c4b1c(data);
                    data->unk_1AC = data->actor.position.x;
                    func_ov012_02125768(data, func_ov012_02125d84);
                    return;
                }
            } else if (data->actor.position.x <= data->unk_1AC) {
                func_ov003_020c4b1c(data);
                data->unk_1AC = data->actor.position.x;
                func_ov012_02125768(data, func_ov012_02125d84);
                return;
            }
            func_ov003_020ccf1c(&data->unk_1D8, 0xFFFFFFD7);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (data->unk_1C0 == 0x1E) {
                func_ov003_020c4cc4(data, 0x218);
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 > 0) {
                return;
            }
            vel           = boss->actor.position.x - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            Mini108_VBlank(&data->unk_084, 2, 1);
            data->unk_1C2 = data->unk_1C2 - 1;
            data->unk_1C0 = 0x1E;
            return;
        }
    }
}

void func_ov012_021265a4(BtlEnm014* data) {
    switch (data->unk_1C2) {
        case 0: {
            s32 threshold = 0x14;
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1E4 = 0;
            if (data->unk_080 == 2) {
                s32 pct = data->actor.currentHp * 100 / data->actor.maxHp;
                if (pct <= 0xA) {
                    threshold = 0x64;
                } else if (pct <= 0x28) {
                    threshold = 0x3C;
                } else if (pct <= 0x46) {
                    threshold = 0x28;
                }
                if (RNG_Next(0x64) < threshold) {
                    data->unk_1E4 = 1;
                }
            }
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;
        }

        case 1:
            Mini108_VBlank(&data->unk_084, 0xE, 1);
            data->unk_1D0 = 0;
            func_ov012_021256f0(1, data, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;

        case 2: {
            s32 se;
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1C0 == 0x31) {
                func_ov003_020c4cc4(data, 0x21B);
                func_ov003_020c4cc4(data, (data->unk_080 == 3) ? 0x21D : 0x21C);
            }
            if (data->unk_1C0 > 0x31 && data->unk_1C0 < 0x3C && data->unk_1D0 == 0) {
                s32 d;
                s32 h;
                se            = (data->unk_080 == 3) ? 0x87 : 0x86;
                d             = ROUND(func_ov003_020cd11c(se));
                d             = (data->unk_084.flags46 & 1) ? d : -d;
                h             = func_ov003_0208a114(se);
                data->unk_1D0 = func_ov003_0208a164(h, &data->actor.unk_04, data->actor.position.x + d, data->actor.position.y,
                                                    data->actor.position.z);
            }
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            Mini108_VBlank(&data->unk_084, 0xF, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;
        }

        case 3:
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (data->unk_1E4 > 0) {
                data->unk_1E4 = data->unk_1E4 - 1;
                data->unk_1C2 = 1;
                data->unk_1C0 = 0;
                return;
            }
            func_ov003_020c4ee0(data);
            func_ov012_02125768(data, func_ov012_02125d84);
            return;
    }
}

void func_ov012_0212684c(BtlEnm014* data) {
    if (func_ov003_020c5e98(data) != 0) {
        return;
    }
    if (data->unk_18C & 0x20) {
        func_ov012_02125768(data, func_ov012_02125810);
    } else {
        func_ov012_02125768(data, func_ov012_02125d84);
    }
}

s32 func_ov012_02126898(BtlEnm014* data) {
    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 0:
            break;
        case 1:
        case 2:
            if (data->actor.periodicEffectMode == 4) {
                func_ov012_02125768(data, func_ov012_021257e8);
            }
            break;
        case 3:
            if (data->unk_18C & 0x10) {
                data->unk_18C |= 0x20;
            } else {
                func_ov012_02125768(data, func_ov012_02125810);
            }
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            func_ov012_02125768(data, func_ov012_0212582c);
            break;
    }

    func_ov012_02125958(data);
    func_ov012_02125858(data);
    if (data->unk_1C4 != NULL) {
        data->unk_1C4(data);
    }
    func_ov003_020c4628(data);
    func_ov003_020ccea8(data, &data->unk_084);
    return data->unk_1CC;
}

s32 func_ov012_02126968(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm014* data = task->data;

    switch (stage) {
        case 0:
            MI_CpuSet(data, 0, 0x1E8);
            func_ov012_02125790(1, data, (s32)args, (s32)func_ov012_02125d84);
            data->unk_1E0.half.lo = func_ov003_020c42ec(data);
            data->unk_1E0.half.hi = data->unk_19E + RNG_Next(data->unk_1A2);
            func_ov012_02125768(data, func_ov012_021257c4);
            break;
        case 1:
            return func_ov012_02126898(data);
        case 2:
            func_ov003_020c4878(data);
            break;
        case 3:
            func_ov003_020c48fc(data);
            break;
    }
    return 1;
}

s32 func_ov012_02126a1c(BtlEnm014* data) {
    if (func_ov012_02126ad8(data) != 0) {
        return 1;
    }
    if (func_ov012_02126a4c(data) != 0) {
        return 1;
    }
    return 0;
}

s32 func_ov012_02126a4c(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;
    s32        lo   = data->actor.position.x - 0x3C000;

    if (data->unk_084.flags46 & 1) {
        lo = data->actor.position.x;
    }
    if (!(boss->actor.position.x - boss->actor.unk_70 >= lo + 0x3C000 || boss->actor.position.x + boss->actor.unk_70 <= lo ||
          boss->actor.position.y >= data->actor.position.y + 0xF000 ||
          boss->actor.position.y <= data->actor.position.y - 0xF000))
    {
        func_ov012_02125768(data, func_ov012_02127134);
        return 1;
    }
    return 0;
}

s32 func_ov012_02126ad8(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;
    s32        lo;

    if (data->unk_080 == 0) {
        return 0;
    }
    lo = data->actor.position.x - 0x50000;
    if (data->unk_084.flags46 & 1) {
        lo = data->actor.position.x;
    }
    if (!(boss->actor.position.x - boss->actor.unk_70 >= lo + 0x50000 || boss->actor.position.x + boss->actor.unk_70 <= lo ||
          boss->actor.position.y >= data->actor.position.y + 0xA000 ||
          boss->actor.position.y <= data->actor.position.y - 0xA000))
    {
        func_ov012_02125768(data, func_ov012_02127378);
        return 1;
    }
    return 0;
}

void func_ov012_02126b74(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;
    s32        d    = boss->actor.position.x - data->unk_1E4;

    if (d < 0) {
        d = -d;
    }
    if (d > data->unk_1E0.word) {
        data->unk_1E0.word = d;
    }
    data->unk_1E4 = boss->actor.position.x;
}

void func_ov012_02126bb0(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;

    data->unk_1E0.word = 0;
    data->unk_1E4      = boss->actor.position.x;
}

void func_ov012_02126bd8(BtlEnm014* data) {
    if (data->unk_080 == 2) {
        data->unk_1E0.word = 0;
    }
    func_ov012_02125768(data, (data->unk_1E0.word > 0x6000) ? func_ov012_02126e2c : func_ov012_02126dac);
    func_ov012_02126bb0(data);
}

s32 func_ov012_02126c1c(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;

    if (!(data->unk_084.flags46 & 1)) {
        if (data->actor.position.x < boss->actor.position.x) {
            goto hit;
        }
    } else if (data->actor.position.x > boss->actor.position.x) {
        goto hit;
    }
    return 0;

hit:
    return 1;
}

void func_ov012_02126c74(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;
    s32        dx   = boss->actor.position.x - data->actor.position.x;
    s32        aim;
    s32        idx;
    s32        sin;
    s32        mag;
    s32        cos;

    if (boss->actor.position.x > data->actor.position.x) {
        if (dx > 0x28000) {
            aim = dx - 0x28000;
        } else {
            aim = -1;
        }
    } else {
        if (dx < -0x28000) {
            aim = dx + 0x28000;
        } else {
            aim = 1;
        }
    }

    idx           = FX_Atan2Idx(boss->actor.position.y - data->actor.position.y, aim) >> 4;
    sin           = data_0205e4e0[idx * 2 + 1];
    mag           = Mth_MulFixed(ABSVAL(sin) + 0x1000, 0x800);
    cos           = data_0205e4e0[idx * 2];
    data->unk_1D8 = Mth_MulFixed(sin, mag);
    data->unk_1DC = Mth_MulFixed(cos, mag);

    if (data->unk_1D8 != 0) {
        return;
    }
    data->unk_1D8 = ROUND(aim) >> 4;
}

void func_ov012_02126dac(BtlEnm014* data) {
    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 0, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = func_ov003_020c42ec(data);
            break;
        case 1:
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 < 0) {
                func_ov012_02125768(data, func_ov012_02126ea0);
            }
            break;
    }
}

void func_ov012_02126e2c(BtlEnm014* data) {
    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 1, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            break;
        case 1:
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            func_ov012_02125768(data, func_ov012_02126dac);
            break;
    }
}

void func_ov012_02126ea0(BtlEnm014* data) {
    if (func_ov012_02126a1c(data) != 0) {
        return;
    }
    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 2, 1);
            func_ov012_02126c74(data);
            func_ov003_020c4b5c(data);
            func_ov012_02126bb0(data);
            data->unk_1E8 = 0;
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x13;
            break;

        case 1:
            data->actor.position.x = data->actor.position.x + data->unk_1D8;
            data->actor.position.y = data->actor.position.y + data->unk_1DC;
            func_ov003_020ccf1c(&data->unk_1D8, -(ABSVAL(data->unk_1D8) >> 4));
            func_ov003_020ccf1c(&data->unk_1DC, -(ABSVAL(data->unk_1DC) >> 4));
            func_ov012_02126b74(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            data->unk_1D8 = 0;
            data->unk_1DC = 0;
            data->unk_1E8 = func_ov012_02126c1c(data);
            if (data->unk_1C0 == 0x13) {
                func_ov003_020c4cc4(data, 0x218);
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 > 0) {
                return;
            }
            func_ov012_02126c74(data);
            Mini108_VBlank(&data->unk_084, 3, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x13;
            if (data->unk_1E8 != 0) {
                data->unk_1D8 = 0;
            }
            break;

        case 2:
            data->actor.position.x = data->actor.position.x + data->unk_1D8;
            data->actor.position.y = data->actor.position.y + data->unk_1DC;
            func_ov003_020ccf1c(&data->unk_1D8, -(ABSVAL(data->unk_1D8) >> 4));
            func_ov003_020ccf1c(&data->unk_1DC, -(ABSVAL(data->unk_1DC) >> 4));
            func_ov012_02126b74(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            data->unk_1D8 = 0;
            data->unk_1DC = 0;
            if (data->unk_1C0 == 0x13) {
                func_ov003_020c4cc4(data, 0x218);
            }
            data->unk_1C0 = data->unk_1C0 - 1;
            if (data->unk_1C0 > 0) {
                return;
            }
            if (func_ov012_02126c1c(data) != 0 || data->unk_1E8 != 0) {
                func_ov012_02126bd8(data);
                return;
            }
            Mini108_VBlank(&data->unk_084, 2, 1);
            func_ov012_02126c74(data);
            data->unk_1C2 = data->unk_1C2 - 1;
            data->unk_1C0 = 0x13;
            break;
    }
}

void func_ov012_02127134(BtlEnm014* data) {
    BtlEnm014* boss = data_ov003_020e71b8->unk3D898;

    switch (data->unk_1C2) {
        case 0:
            Mini108_VBlank(&data->unk_084, 0xC, 1);
            data->unk_1D0 = 0;
            func_ov012_02126bb0(data);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;

        case 1: {
            s32 d;
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1C0 == 0x28) {
                func_ov003_020c4cc4(data, 0x219);
            }
            if (data->unk_1C0 > 0x28 && data->unk_1C0 < 0x32) {
                d = ROUND(func_ov003_020cd11c(0x82));
                d = (data->unk_084.flags46 & 1) ? d : -d;
                if (data->unk_1D0 == 0) {
                    data->unk_1D0 =
                        func_ov003_0208a164(func_ov003_0208a114(0x82), &data->actor.unk_04, data->actor.position.x + d,
                                            data->actor.position.y, data->actor.position.z);
                    if (data->unk_1D0 == 1) {
                        func_ov003_020c4cc4(data, 0x21A);
                        boss->actor.zVelocity = Mth_MulFixed(boss->actor.zVelocity, 0x2000);
                        boss->actor.yVelocity = Mth_MulFixed(boss->actor.yVelocity, 0x1800);
                    }
                }
            }
            func_ov012_02126b74(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            Mini108_VBlank(&data->unk_084, 0xD, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;
        }

        case 2:
            func_ov012_02126b74(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (func_ov012_02126c1c(data) != 0 && data->unk_1D0 == 1) {
                func_ov012_02125768(data, func_ov012_02126dac);
                return;
            }
            func_ov012_02126bd8(data);
            return;
    }
}

void func_ov012_02127378(BtlEnm014* data) {
    switch (data->unk_1C2) {
        case 0:
            data->unk_1EC = 0;
            if ((u16)(data->unk_080 + 0xFFFE) <= 1) {
                data->unk_1EC = RNG_Next(2);
            }
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            /* fallthrough */

        case 1:
            Mini108_VBlank(&data->unk_084, 0xE, 1);
            data->unk_1D0 = 0;
            func_ov012_02126bb0(data);
            func_ov012_021256f0(0, data, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;

        case 2: {
            s32 se;
            data->unk_1C0 = data->unk_1C0 + 1;
            func_ov012_02126b74(data);
            if (data->unk_1C0 == 0x31) {
                func_ov003_020c4cc4(data, 0x21B);
                func_ov003_020c4cc4(data, (data->unk_080 == 3) ? 0x21D : 0x21C);
            }
            if (data->unk_1C0 > 0x31 && data->unk_1C0 < 0x3C && data->unk_1D0 == 0) {
                s32 d;
                s32 h;
                se            = (data->unk_080 == 3) ? 0x84 : 0x83;
                d             = ROUND(func_ov003_020cd11c(se));
                d             = (data->unk_084.flags46 & 1) ? d : -d;
                h             = func_ov003_0208a114(se);
                data->unk_1D0 = func_ov003_0208a164(h, &data->actor.unk_04, data->actor.position.x + d, data->actor.position.y,
                                                    data->actor.position.z);
            }
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            Mini108_VBlank(&data->unk_084, 0xF, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;
        }

        case 3:
            func_ov012_02126b74(data);
            if (SpriteMgr_IsAnimationFinished(&data->unk_084.sprite) == 0) {
                return;
            }
            if (data->unk_1EC > 0) {
                func_ov003_020c4b5c(data);
                data->unk_1EC = data->unk_1EC - 1;
                data->unk_1C2 = 1;
                return;
            }
            if (func_ov012_02126c1c(data) != 0) {
                func_ov012_02126bd8(data);
                return;
            }
            func_ov012_02125768(data, func_ov012_02126dac);
            return;
    }
}

void func_ov012_02127608(BtlEnm014* data) {
    if (func_ov003_020c6bc8(data, 7) != 0) {
        return;
    }
    func_ov012_02125768(data, func_ov012_02127634);
}

void func_ov012_02127634(BtlEnm014* data) {
    if (func_ov003_020c6c2c(data, 7) != 0) {
        return;
    }
    func_ov012_02125768(data, func_ov012_02126dac);
}

void func_ov012_02127660(BtlEnm014* data) {
    if (func_ov003_020c6bac(data, 9) != 0) {
        return;
    }
    func_ov012_02125768(data, func_ov012_02126dac);
}

s32 func_ov012_0212768c(BtlEnm014* data) {
    switch (CombatActor_PopPendingCommand(&data->actor)) {
        case 0:
            break;
        case 1:
            if (data->actor.periodicEffectMode == 4) {
                func_ov012_02125768(data, func_ov012_021257e8);
            }
            if (data->actor.zVelocity < 0) {
                data->actor.zVelocity = 0;
            }
            break;
        case 3:
            func_ov003_02084694(&data->unk_144, 1);
            func_ov012_02125768(data, func_ov012_02125810);
            break;
        case 4:
            func_ov012_02125768(data, func_ov012_02127608);
            break;
        case 5:
            func_ov012_02125768(data, func_ov012_02127634);
            break;
        case 2:
            func_ov012_02125768(data, func_ov012_02127660);
            break;
        case 6:
            func_ov012_02125768(data, func_ov012_0212582c);
            break;
    }

    if (data->unk_1C4 != func_ov012_02127660) {
        func_ov012_02125958(data);
    }
    func_ov012_02125858(data);
    if (data->unk_1C4 != NULL) {
        data->unk_1C4(data);
    }
    func_ov003_020c4668(data);
    func_ov003_020ccea8(data, &data->unk_084);
    func_ov012_021258e8();
    return data->unk_1CC;
}

s32 func_ov012_021277b0(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm014* data = task->data;

    switch (stage) {
        case 0:
            MI_CpuSet(data, 0, 0x1F0);
            func_ov012_02125790(0, data, (s32)args, (s32)func_ov012_02126dac);
            func_ov012_02125768(data, func_ov012_021257c4);
            break;
        case 1:
            return func_ov012_0212768c(data);
        case 2:
            func_ov003_020c48b0(data);
            break;
        case 3:
            func_ov003_020c492c(data);
            break;
    }
    return 1;
}
