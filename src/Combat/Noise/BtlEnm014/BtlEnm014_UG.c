#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm014.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

extern void func_ov003_02084694(void*, s32);
extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32  func_ov003_020c42ec(void*);
extern void func_ov003_020c4668(void*);
extern void func_ov003_020c48b0(void*);
extern void func_ov003_020c492c(void*);
extern void func_ov003_020c4b5c(void*);
extern void func_ov003_020c4cc4(void*, s32);
extern s32  func_ov003_020c6bac(void*, s32);
extern s32  func_ov003_020c6bc8(void*, s32);
extern s32  func_ov003_020c6c2c(void*, s32);
extern void func_ov003_020ccea8(void*, void*);
extern void func_ov003_020ccf1c(void*, s32);
extern s32  func_ov003_020cd11c(s32);

extern s16 data_0205e4e0[];

#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

#define ABSVAL(value) ((value) < 0 ? -(value) : (value))

s32  func_ov012_02126a4c(BtlEnm014*);
s32  func_ov012_02126ad8(BtlEnm014*);
void func_ov012_02126dac(BtlEnm014*);
void func_ov012_02126e2c(BtlEnm014*);
void func_ov012_02126ea0(BtlEnm014*);
void func_ov012_02127134(BtlEnm014*);
void func_ov012_02127378(BtlEnm014*);
void func_ov012_02127634(BtlEnm014*);
s32  func_ov012_021277b0(TaskPool*, Task*, void*, s32);

const TaskHandle Tsk_BtlEnm014_UG = {"Tsk_BtlEnm014_UG", func_ov012_021277b0, 0x1F0};

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
    }
    if (data->unk_084.flags46 & 1) {
        if (data->actor.position.x > boss->actor.position.x) {
            goto hit;
        }
    }
    goto miss;

hit:
    return 1;

miss:
    return 0;
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
    cos           = data_0205e4e0[idx * 2];
    mag           = Mth_MulFixed(ABSVAL(sin) + 0x1000, 0x800);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 1, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 3, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xC, 1);
            data->unk_1D0 = 0;
            func_ov012_02126bb0(data);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0;
            return;

        case 1: {
            s32 d;
            s32 x;
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1C0 == 0x28) {
                func_ov003_020c4cc4(data, 0x219);
            }
            if (data->unk_1C0 > 0x28 && data->unk_1C0 < 0x32) {
                d = ROUND(func_ov003_020cd11c(0x82));
                d = (data->unk_084.flags46 & 1) ? d : -d;
                x = data->actor.position.x;
                if (data->unk_1D0 == 0) {
                    data->unk_1D0 = func_ov003_0208a164(func_ov003_0208a114(0x82), &data->actor.unk_04, x + d,
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xD, 1);
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
                goto dash;
            }
            func_ov012_02126bd8(data);
            return;

        dash:
            func_ov012_02125768(data, func_ov012_02126dac);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xE, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xF, 1);
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
