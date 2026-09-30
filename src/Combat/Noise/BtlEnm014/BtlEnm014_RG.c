#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm014.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32  func_ov003_020c42ec(void*);
extern void func_ov003_020c4628(void*);
extern void func_ov003_020c4878(void*);
extern void func_ov003_020c48fc(void*);
extern void func_ov003_020c4b1c(void*);
extern void func_ov003_020c4cc4(void*, s32);
extern s32  func_ov003_020c4e0c(void*);
extern void func_ov003_020c4ee0(void*);
extern s32  func_ov003_020c5e98(void*);
extern s32  func_ov003_020c6230(void*);
extern void func_ov003_020ccea8(void*, void*);
extern void func_ov003_020ccf1c(void*, s32);
extern s32  func_ov003_020cd11c(s32);

#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

#define ABSVAL(value) ((value) < 0 ? -(value) : (value))

void func_ov012_02125de4(BtlEnm014*);
void func_ov012_021265a4(BtlEnm014*);
void func_ov012_0212684c(BtlEnm014*);
s32  func_ov012_02126968(TaskPool*, Task*, void*, s32);

const TaskHandle Tsk_BtlEnm014_RG = {"Tsk_BtlEnm014_RG", func_ov012_02126968, 0x1E8};

const s32 data_ov012_0212794c[4] = {0x64, 0x32, 0x32, 0x41};

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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 1);
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
            s32 bx;
            s32 px;
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
            bx = boss->actor.position.x;
            px = data->actor.position.x;
            if (ABSVAL(px - bx) < 0x3C000) {
                CombatSprite_SetAnimFromTable(&data->unk_084, 0xC, 1);
                data->unk_1D0 = 0;
                data->unk_1C2 = 4;
                data->unk_1C0 = 0;
                return;
            }
            vel           = bx - px;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            CombatSprite_SetAnimFromTable(&data->unk_084, 3, 1);
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = 0x1E;
            return;
        }

        case 3: {
            s32 vel;
            s32 bx;
            s32 px;
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
            bx = boss->actor.position.x;
            px = data->actor.position.x;
            if (ABSVAL(px - bx) < 0x3C000) {
                CombatSprite_SetAnimFromTable(&data->unk_084, 0xC, 1);
                data->unk_1D0 = 0;
                data->unk_1C2 = 4;
                data->unk_1C0 = 0;
                return;
            }
            vel           = bx - px;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xD, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 1);
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
            vel           = data->unk_1AC - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            CombatSprite_SetAnimFromTable(&data->unk_084, 3, 1);
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
            vel           = data->unk_1AC - data->actor.position.x;
            data->unk_1DC = vel;
            data->unk_1D8 = vel;
            vel           = data->unk_1DC;
            if (vel > 0x800) {
                vel = 0x800;
            } else if (vel < -0x800) {
                vel = -0x800;
            }
            data->unk_1D8 = vel;
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 1);
            data->unk_1C2 = data->unk_1C2 - 1;
            data->unk_1C0 = 0x1E;
            return;
        }
    }
}

void func_ov012_021265a4(BtlEnm014* data) {
    switch (data->unk_1C2) {
        case 0: {
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1E4 = 0;
            if (data->unk_080 == 2) {
                s32 hp100 = data->actor.currentHp * 0x64;
                s32 pct;
                s32 threshold = 0x14;
                pct           = hp100 / data->actor.maxHp;
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xE, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xF, 1);
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
