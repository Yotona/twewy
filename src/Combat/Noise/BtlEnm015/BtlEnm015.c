#include "Combat/Noise/Private/BtlEnm015.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

extern s32   func_ov003_020c37f8(void*);
extern void* func_ov003_020c3c88(void*);
extern void  func_ov003_020c3efc(void*, void*);
extern void  func_ov003_020c427c(void*);
extern void  func_ov003_020c495c(void*);
extern void  func_ov003_020c4ab4(void*, s32);
extern s32   func_ov003_020c4e0c(void*);
extern s16   func_ov003_020843b0(s32, s32);
extern s16   func_ov003_020843ec(s32, s32, s32);
extern void  func_ov003_02084634(void*, s32, s32, s32);
extern void  func_ov003_020880e4(s32, void*, void*);
extern s32   func_ov003_02082f2c(void*);
extern void  func_ov003_02084694(void*, s32);
extern s32   func_ov003_020c3c28(void);
extern void  func_ov003_020c4628(void*);
extern void  func_ov003_020c4668(void*);
extern void  func_ov003_020c4748(void*);
extern void  func_ov003_020c4cc4(void*, s32);
extern void  func_ov003_020ccea8(void*, void*);
extern s32   func_ov003_020c5bfc(void*);
extern s32   func_ov003_020ccfec(void*);
extern s32   func_ov003_020c72b4(void*, s32, s32);
extern s32   func_ov003_020c4830(void*);
extern void  func_ov003_020827c0(void*, s32);
extern void  func_ov003_020831e4(void*, void*);
extern s32   func_ov003_02088130(void);
extern void  func_ov003_0208810c(void*, void*);
extern void  func_ov003_02082750(void*, s32);
extern void  func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);

// MARK: Forward declarations

s32 func_ov013_021260e8(TaskPool*, Task*, void*, s32);
s32 func_ov013_02126700(TaskPool*, Task*, void*, s32);
s32 func_ov013_02126a30(TaskPool*, Task*, void*, s32);
s32 func_ov013_02126dd8(TaskPool*, Task*, void*, s32);
s32 func_ov013_02126ff4(TaskPool*, Task*, void*, s32);
s32 func_ov013_02127370(TaskPool*, Task*, void*, s32);

s16  func_ov013_021258f0(BtlEnm015*);
s32  func_ov013_02125a04(BtlEnm015*, s32, s32, s32);
void func_ov013_02125b64(BtlEnm015*, s32);
void func_ov013_02125b8c(s32, BtlEnm015*, s32, s32);
s32  func_ov013_02125d24(BtlEnm015*);
void func_ov013_02125e90(BtlEnm015*);
void func_ov013_02125eb4(BtlEnm015*);
void func_ov013_02125ed0(BtlEnm015*);
void func_ov013_02125efc(BtlEnm015*);
void func_ov013_02125fd0(BtlEnm015Eff*, Enm015Spawn*);

// MARK: Data

char data_ov013_0212754c[] = "Apl_Hor/Grp_BtlEnm015.bin";
char data_ov013_02127568[] = "Apl_Hor/Grp_BtlEnm015b.bin";
char data_ov013_02127584[] = "Apl_Hor/Grp_BtlEnm015a.bin";

BinIdentifier data_ov013_02127484 = {3, data_ov013_0212754c};
BinIdentifier data_ov013_0212748c = {3, data_ov013_02127568};
BinIdentifier data_ov013_02127494 = {3, data_ov013_02127584};

SpriteAnimEntry data_ov013_02127454[2] = {
    {0x10, 0x12, 0x11, 0},
    {0x16, 0x18, 0x17, 0},
};

SpriteAnimEntry data_ov013_0212749c[8] = {
    {0x1, 0x3, 0x2, 0},
    {0x4, 0x6, 0x5, 0},
    {0x4, 0x6, 0x5, 1},
    {0x4, 0x6, 0x5, 2},
    {0x7, 0x9, 0x8, 0},
    {0x7, 0x9, 0x8, 1},
    {0x7, 0x9, 0x8, 2},
    {0xA, 0xC, 0xB, 0},
};

SpriteAnimEntry data_ov013_021274dc[2] = {
    { 0xD,  0xF,  0xE, 0},
    {0x13, 0x15, 0x14, 0},
};

Enm015Variant data_ov013_02127444 = {&data_ov013_02127484, data_ov013_0212749c, 0, 0, 0x19, 0x2BC};
Enm015Variant data_ov013_02127474 = {&data_ov013_0212748c, data_ov013_0212749c, 0, 0, 0x19, 0x2BC};
Enm015Variant data_ov013_02127464 = {&data_ov013_02127494, data_ov013_0212749c, 0, 0, 0x19, 0x2C8};

Enm015Variant* data_ov013_02127540[3] = {
    &data_ov013_02127444,
    &data_ov013_02127474,
    &data_ov013_02127464,
};

char data_ov013_021275a0[] = "Tsk_BtlEnm015_Eff";
char data_ov013_021275b4[] = "Tsk_BtlEnm015_EffStamp";
char data_ov013_021275cc[] = "Tsk_BtlEnm015_EffStampSub";
char data_ov013_021275e8[] = "Tsk_BtlEnm015_RG";
char data_ov013_021275fc[] = "Tsk_BtlEnm015_Shake";
char data_ov013_02127610[] = "Tsk_BtlEnm015_UG";

const TaskHandle Tsk_BtlEnm015_Eff         = {data_ov013_021275a0, func_ov013_021260e8, 0x70};
const TaskHandle Tsk_BtlEnm015_EffStamp    = {data_ov013_021275b4, func_ov013_02126700, 0xC};
const TaskHandle Tsk_BtlEnm015_EffStampSub = {data_ov013_021275cc, func_ov013_02126a30, 0x74};
const TaskHandle Tsk_BtlEnm015_RG          = {data_ov013_021275e8, func_ov013_02126dd8, 0x1E8};
const TaskHandle Tsk_BtlEnm015_Shake       = {data_ov013_021275fc, func_ov013_02126ff4, 0x10};
const TaskHandle Tsk_BtlEnm015_UG          = {data_ov013_02127610, func_ov013_02127370, 0x1EC};

s32 data_ov013_02127640;
s32 data_ov013_02127644;

// MARK: Functions

BinIdentifier* func_ov013_021256c0(s32 index) {
    return (BinIdentifier*)((u8*)&data_ov013_02127484 + index * 8);
}

u16 func_ov013_021256d0(s32 index) {
    return data_ov013_02127540[index]->unk_0A;
}

SpriteAnimEntry* func_ov013_021256e4(void) {
    return data_ov013_02127454;
}

void func_ov013_021256f0(BtlEnm015* arg0, u16 arg1, void* arg2, void* arg3, void* arg4) {
    Enm015Spawn spawn;
    MI_CpuSet(&spawn, 0, sizeof(spawn));
    spawn.unk_00 = arg0;
    spawn.unk_04 = arg1;
    spawn.unk_08 = (s32)arg2;
    spawn.unk_0C = (s32)arg3;
    spawn.unk_10 = (s32)arg4;

    TaskPool* pool;
    if (func_ov003_020c37f8(&arg0->unk_084) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }

    if (arg1 != 0) {
        EasyTask_CreateTask(pool, &Tsk_BtlEnm015_Eff, NULL, 0, NULL, &spawn);
    } else {
        EasyTask_CreateTask(pool, &Tsk_BtlEnm015_EffStampSub, NULL, 0, NULL, &spawn);
    }
}

void func_ov013_021257a4(BtlEnm015* arg0, s32 arg1) {
    Enm015Spawn spawn;
    MI_CpuSet(&spawn, 0, sizeof(spawn));
    spawn.unk_14 = (arg1 != 0);
    spawn.unk_00 = arg0;
    spawn.unk_08 = arg0->actor.position.x;
    spawn.unk_0C = arg0->actor.position.y;

    TaskPool* pool;
    if (func_ov003_020c37f8(&arg0->unk_084) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(pool, &Tsk_BtlEnm015_EffStamp, NULL, 0, NULL, &spawn);
}

void func_ov013_02125838(BtlEnm015* arg0) {
    TaskPool* pool;

    if (func_ov003_020c37f8(&arg0->unk_084) != 0) {
        if (data_ov013_02127640 != -1) {
            return;
        }
        pool                = &data_ov003_020e71b8->taskPool;
        data_ov013_02127640 = EasyTask_CreateTask(pool, &Tsk_BtlEnm015_Shake, NULL, 0, NULL, arg0);
    } else {
        if (data_ov013_02127644 != -1) {
            return;
        }
        pool                = &data_ov003_020e71b8->unk_00000;
        data_ov013_02127644 = EasyTask_CreateTask(pool, &Tsk_BtlEnm015_Shake, NULL, 0, NULL, arg0);
    }
}

s16 func_ov013_021258f0(BtlEnm015* arg0) {
    s16 maxHp = arg0->actor.maxHp;
    s16 v     = arg0->unk_1D6;
    s32 diff  = maxHp - (v + 1);
    s32 ten   = maxHp / 10;

    if (diff <= 0) {
        return (s16)((RNG_Next(3) + 2) * 0x3C);
    }

    s32 ratio = diff / ten;
    if (ratio >= 8) {
        return (s16)((RNG_Next(7) + 0x11) * 0x3C);
    }
    if (ratio >= 6) {
        return (s16)((RNG_Next(5) + 0xA) * 0x3C);
    }
    if (ratio >= 4) {
        return (s16)((RNG_Next(5) + 6) * 0x3C);
    }
    if (ratio >= 1) {
        return (s16)((RNG_Next(3) + 4) * 0x3C);
    }
    return (s16)((RNG_Next(3) + 2) * 0x3C);
}

s32 func_ov013_02125a04(BtlEnm015* arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk_1D8--;
    arg0->unk_1DA--;

    if (RNG_Next(arg3) == 0) {
        return 0;
    }

    if (arg0->unk_1D8 < 0) {
        arg0->unk_1D8 = func_ov013_021258f0(arg0);

        if (func_ov003_020c37f8(&arg0->unk_084) != 0) {
            if (func_ov003_020c4e0c(arg0) == 0) {
                return 0;
            }
        } else {
            s16 x = func_ov003_020843b0(0, arg0->actor.position.x);
            if (x >= 0xDC || x <= 0x1E) {
                return 0;
            }
            if (func_ov003_020843ec(0, arg0->actor.position.y, arg0->actor.position.z) <= 0x32) {
                return 0;
            }
        }
        func_ov013_02125b64(arg0, arg1);
        return 1;
    }

    if (arg0->unk_1DA < 0) {
        if (func_ov003_020c37f8(&arg0->unk_084) != 0) {
            arg0->unk_1DA = (s16)(arg0->unk_19E + RNG_Next(arg0->unk_1A2));
            if (func_ov003_020c4e0c(arg0) == 0) {
                return 0;
            }
        } else {
            arg0->unk_1DA = (s16)(arg0->unk_198 + RNG_Next(arg0->unk_19C));
        }
        func_ov013_02125b64(arg0, arg2);
        return 1;
    }

    return 0;
}

void func_ov013_02125b64(BtlEnm015* arg0, s32 arg1) {
    func_ov003_020c427c(arg0);
    arg0->unk_1C4 = (void (*)(BtlEnm015*))arg1;
    arg0->unk_1C2 = 0;
    arg0->unk_1C0 = 0;
}

void func_ov013_02125b8c(s32 arg0, BtlEnm015* data, s32 arg2, s32 arg3) {
    data->unk_1CC = 1;
    data->unk_1C8 = arg3;
    func_ov003_020c3efc(data, (void*)arg2);
    data->actor.flags |= 0x10000000;

    Enm015Variant* variant = data_ov013_02127540[data->unk_080];
    void*          r4      = func_ov003_020c3c88(data);

    SpriteAnimationEx anim;
    CombatSprite_InitAnim(&anim.anim, arg0, variant->binIden);
    SpriteAnimEntry* entry = &variant->animTable[variant->unk_08];
    anim.anim.bits_10_11   = 1;
    anim.anim.unk_1C       = entry->charDataIndex;
    anim.anim.unk_1E       = (s16)(variant->unk_0C << 5);
    anim.anim.unk_26       = entry->frameDataIndex;
    anim.anim.unk_28       = entry->paletteDataIndex;
    anim.anim.unk_20       = variant->unk_0A;
    anim.anim.unk_22       = 2;
    anim.anim.unk_2A       = entry->animDataIndex + 1;
    anim.unk_2C            = variant->animTable;
    CombatSprite_Load(&data->unk_084, &anim);
    CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);

    func_ov003_020880e4(arg0, &data->unk_0E4, data);
    func_ov003_020c495c(data);
    if (arg0 == 0) {
        func_ov003_02084634(&data->unk_144, 1, ((u8*)r4)[3], ((u8*)r4)[4]);
    }
    func_ov003_020c4ab4(data, 0);

    data->unk_1D8 = (s16)((RNG_Next(7) + 0x11) * 0x3C);
    if (arg0 == 0) {
        data->unk_1DA = (s16)(data->unk_198 + RNG_Next(data->unk_19C));
    } else {
        data->unk_1DA = (s16)(data->unk_19E + RNG_Next(data->unk_1A2));
    }
    data_ov013_02127644 = -1;
    data_ov013_02127640 = -1;
}

s32 func_ov013_02125d24(BtlEnm015* arg0) {
    switch (CombatActor_PopPendingCommand(&arg0->actor)) {
        case 0:
            break;
        case 1: {
            arg0->unk_1D6 = (s16)(arg0->unk_1D6 + (s16)(arg0->unk_1D4 - arg0->actor.currentHp));
        } break;
        case 2:
            break;
        case 3: {
            func_ov003_02084694(&arg0->unk_144, 1);
            if (arg0->unk_18C & 0x10) {
                arg0->unk_18C |= 0x20;
            } else {
                func_ov013_02125b64(arg0, (s32)func_ov013_02125eb4);
            }
        } break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            func_ov013_02125b64(arg0, (s32)func_ov013_02125ed0);
            break;
    }

    arg0->unk_1D4 = arg0->actor.currentHp;
    if (arg0->unk_1C4 != NULL) {
        arg0->unk_1C4(arg0);
    }
    if (func_ov003_020c37f8(&arg0->unk_084) != 0) {
        func_ov003_020c4628(arg0);
    } else {
        func_ov003_020c4668(arg0);
    }
    if (data_ov003_020e71b8->unk3D874 == 2 && func_ov003_020c3c28() == 0 && arg0->unk_084.animTableIndex == 0) {
        u16 diff;
        if (SpriteMgr_IsFrameFinished(&arg0->unk_084.sprite)) {
            diff = arg0->unk_084.sprite.currentFrame - arg0->unk_084.sprite.loopFrame;
        } else {
            diff = arg0->unk_084.sprite.currentFrame - arg0->unk_084.sprite.loopFrame + 1;
        }
        if (diff == 4) {
            func_ov003_020c4cc4(arg0, 0x223);
        }
    }
    func_ov003_020ccea8(arg0, &arg0->unk_084);
    return arg0->unk_1CC;
}

void func_ov013_02125e90(BtlEnm015* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov013_02125b64(arg0, arg0->unk_1C8);
}

void func_ov013_02125eb4(BtlEnm015* arg0) {
    if (func_ov003_020ccfec(arg0) == 0) {
        arg0->unk_1CC = 0;
    }
}

void func_ov013_02125ed0(BtlEnm015* arg0) {
    if (func_ov003_020c72b4(arg0, 0, 5) != 0) {
        return;
    }
    func_ov013_02125b64(arg0, arg0->unk_1C8);
}

void func_ov013_02125efc(BtlEnm015* arg0) {
    if (func_ov003_020c4830(arg0) != 0) {
        return;
    }
    func_ov003_020c4ab4(arg0, 0);
    func_ov003_020c4748(arg0);

    s32 x                  = arg0->actor.position.x;
    s32 y                  = arg0->actor.position.y;
    arg0->actor.position.x = x + 0xC000;
    arg0->actor.position.y = y + 0xA000;
    if (arg0->actor.flags & 1) {
        arg0->actor.flags &= ~1;
    }

    if (*(s32*)((u8*)data_ov003_020e71b8 + 0x3D87C) != 0 && arg0->unk_084.paletteMode == 1) {
        func_ov003_020827c0(&arg0->unk_084, 0);
        func_ov003_020831e4(arg0, &arg0->unk_084);
        func_ov003_020827c0(&arg0->unk_084, 1);
    } else {
        func_ov003_020831e4(arg0, &arg0->unk_084);
    }

    arg0->actor.position.x = x;
    arg0->actor.position.y = y;
    if (func_ov003_02088130() != 0) {
        func_ov003_0208810c(&arg0->unk_0E4, arg0);
    }
}

void func_ov013_02125fd0(BtlEnm015Eff* data, Enm015Spawn* args) {
    s32        flag  = 0;
    BtlEnm015* owner = args->unk_00;

    MI_CpuSet(data, 0, sizeof(BtlEnm015Eff));
    data->unk_6C = owner;
    data->unk_60 = args->unk_08;
    data->unk_64 = args->unk_0C;
    data->unk_68 = args->unk_10;

    u16 idx = args->unk_04;
    if (idx == 1) {
        flag = 0x118;
    }
    s32 engine = owner->unk_084.sprite.bits_0_1;

    SpriteAnimationEx anim;
    CombatSprite_InitAnim(&anim.anim, engine, func_ov013_021256c0(owner->unk_080));
    s16              unk20 = func_ov013_021256d0(owner->unk_080);
    SpriteAnimEntry* table = func_ov013_021256e4();
    SpriteAnimEntry* entry = &table[idx];
    anim.anim.bits_10_11   = 1;
    anim.anim.unk_1C       = entry->charDataIndex;
    anim.anim.unk_1E       = (s16)(flag << 5);
    anim.anim.unk_26       = entry->frameDataIndex;
    anim.anim.unk_28       = entry->paletteDataIndex;
    anim.anim.unk_20       = unk20;
    anim.anim.unk_22       = 2;
    anim.anim.unk_2A       = entry->animDataIndex + 1;
    anim.unk_2C            = table;
    CombatSprite_Load(&data->sprite, &anim);
    CombatSprite_SetAnimFromTable(&data->sprite, idx, 1);
    func_ov003_02082750(data, owner->unk_084.flags46 & 1);
}

s32 func_ov013_021260e8(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm015Eff* data  = task->data;
    BtlEnm015*    owner = data->unk_6C;

    switch (stage) {
        case 0:
            func_ov013_02125fd0(data, (Enm015Spawn*)args);
            break;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 0;
            }
            if (owner->actor.flags & 4) {
                return 0;
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                return 0;
            }
            CombatSprite_Update(&data->sprite);
            break;
        case 2: {
            s16 x;
            s16 y;
            func_ov003_02084348(func_ov003_020c37f8(data) ? 1 : 0, &x, &y, data->unk_60, data->unk_64, data->unk_68);
            CombatSprite_SetPosition(&data->sprite, x, y);
            func_ov003_02082730(&data->sprite, 0x80000000 - data->unk_64);
            CombatSprite_Render(&data->sprite);
        } break;
        case 3:
            CombatSprite_Release(&data->sprite);
            break;
    }
    return 1;
}
