#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm026.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

extern void func_ov003_020c4ab4(void*, s32);
extern void func_ov003_020c4cc4(void*, s32);
extern void func_ov003_020c48fc(void*);
extern void func_ov003_020c4878(void*);
extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern void func_ov003_020ccf1c(void*, s32);
extern void func_ov003_020c4b1c(void*);
extern s32  func_ov003_020c42ec(void*);
extern void func_ov003_020c5924(void*, void*);
extern s16  data_0205e4e0[];

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void        func_ov015_0212699c(BtlEnm026*);
void        func_ov015_02126ec4(BtlEnm026*);
s32         func_ov015_02126f44(TaskPool*, Task*, void*, s32);
extern char data_ov015_021284c0[20];

const TaskHandle Tsk_BtlEnm026_RG = {data_ov015_021284c0, func_ov015_02126f44, 0x1F4};

char data_ov015_021284c0[20] = "Tsk_BtlEnm026_RG";

void func_ov015_021268b0(BtlEnm026* data) {
    switch (data->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);
            data->actor.flags &= ~0x10000000;
            func_ov003_020c4b1c(data);
            data->unk_196 &= ~0x2;
            data->unk_1C2 = data->unk_1C2 + 1;
            data->unk_1C0 = func_ov003_020c42ec(data);
            /* fallthrough */
        case 1:
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
            break;
    }
}

void func_ov015_0212699c(BtlEnm026* data) {
    BtlEnm026* boss = (BtlEnm026*)data_ov003_020e71b8->unk3D89C;

    switch (data->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xA, 1);
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
                Sprite_Restart(&data->unk_084.sprite);
                return;
            }
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xB, 0);
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
            data->unk_1E8 = (data->unk_084.flags46 & 1) ? 0x8000 : -0x8000;
            CombatSprite_SetAnimFromTable(&data->unk_084, 0xC, 1);
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
            CombatSprite_SetAnimFromTable(&data->unk_084, 2, 0);
            data->unk_1C2 = data->unk_1C2 + 1;
            return;
        }

        case 4: {
            s32 angle     = FX_Atan2Idx(data->unk_1B0 - data->actor.position.y, data->unk_1AC - data->actor.position.x) >> 4;
            s16 sin       = data_0205e4e0[angle * 2 + 1];
            s16 cos       = data_0205e4e0[angle * 2];
            s32 mag       = Mth_MulFixed(((sin < 0) ? -sin : sin) + 0x1000, 0x2000);
            data->unk_1E8 = Mth_MulFixed(sin, mag);
            data->unk_1EC = Mth_MulFixed(cos, mag);
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
                CombatSprite_SetAnimFromTable(&data->unk_084, 0, 0);
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
    if (data_ov015_02128500.result10 != -1) {
        CombatSprite_SetAnimFromTable(&data->unk_084, 2, 0);
    } else {
        CombatSprite_SetAnimFromTable(&data->unk_084, 3, 0);
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
