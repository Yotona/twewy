#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm015.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

extern void func_ov003_020c4cc4(void*, s32);

extern s32  func_ov003_020ccedc(s32);
extern s32  func_ov003_020ccefc(s32);
extern s32  func_ov003_020c6230(void*);
extern void func_ov003_020c4ee0(void*);
extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32  func_ov003_0208a08c(s32, void*, s32);
s32         func_ov013_02126dd8(TaskPool*, Task*, void*, s32);
void        func_ov013_02126b30(BtlEnm015*);
void        func_ov013_02126c64(BtlEnm015*);

const TaskHandle Tsk_BtlEnm015_RG = {"Tsk_BtlEnm015_RG", func_ov013_02126dd8, 0x1E8};

void func_ov013_02126ab0(BtlEnm015* arg0) {
    switch (arg0->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 0, 0);
            func_ov003_020c4ee0(arg0);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = RNG_Next(0x3C) + 0x3C;
            break;

        case 1:
            func_ov013_02125a04(arg0, (s32)func_ov013_02126c64, (s32)func_ov013_02126b30, arg0->unk_1A2);
            break;
    }
}

void func_ov013_02126b30(BtlEnm015* arg0) {
    BtlEnm015* boss = (BtlEnm015*)data_ov003_020e71b8->unk3D89C;

    switch (arg0->unk_1C2) {
        case 0:
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1:
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 7, 1);
            func_ov003_020c4cc4(arg0, 0x220);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 2:
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_084.sprite) == 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 8, 1);
            func_ov003_020c4cc4(arg0, 0x226);
            func_ov013_021257a4(arg0, 0);
            func_ov003_0208a08c(1, boss, 0);
            func_ov013_02125838(arg0);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 3:
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_084.sprite) == 0) {
                return;
            }
            func_ov013_02125b64(arg0, (s32)func_ov013_02126ab0);
            break;
    }
}

void func_ov013_02126c64(BtlEnm015* arg0) {
    BtlEnm015* boss = (BtlEnm015*)data_ov003_020e71b8->unk3D89C;

    switch (arg0->unk_1C2) {
        case 0:
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1:
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 9, 1);
            arg0->unk_1DC = boss->actor.position.x;
            arg0->unk_1E0 = boss->actor.position.y;
            arg0->unk_1E4 = boss->actor.position.z;
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 2: {
            s32 h;

            if (arg0->unk_1C0 == 0) {
                func_ov003_020c4cc4(arg0, 0x220);
            }
            if (arg0->unk_1C0 == 0x2B) {
                func_ov013_021256f0(arg0, 1, arg0->unk_1DC, arg0->unk_1E0, arg0->unk_1E4);
            }
            if (arg0->unk_1C0 == 0x2C) {
                func_ov003_020c4cc4(arg0, 0x224);
                h = func_ov003_0208a114(0x8B);
                if (func_ov003_0208a164(h, &arg0->actor.unk_04, arg0->unk_1DC, arg0->unk_1E0, arg0->unk_1E4) == 1) {
                    func_ov003_020c4cc4(arg0, 0x225);
                }
            }
            arg0->unk_1C0 = arg0->unk_1C0 + 1;
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_084.sprite) == 0) {
                return;
            }
            func_ov013_02125b64(arg0, (s32)func_ov013_02126ab0);
            break;
        }
    }
}

s32 func_ov013_02126dd8(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm015* data = task->data;
    s32        ret  = 1;

    switch (stage) {
        case 0: {
            BtlEnm015* boss;
            s32        x;

            MI_CpuSet(data, 0, 0x1E8);
            func_ov013_02125b8c(1, data, (s32)args, (s32)func_ov013_02126ab0);
            func_ov013_02125b64(data, (s32)func_ov013_02125e90);
            data->unk_1AC                           = func_ov003_020ccedc(1) >> 2;
            data->actor.position.x                  = data->unk_1AC;
            data->unk_1B0                           = (func_ov003_020ccefc(1) >> 1) + 1;
            data->actor.position.y                  = data->unk_1B0;
            data->unk_1B4                           = 0;
            data->actor.position.z                  = 0;
            boss                                    = (BtlEnm015*)data_ov003_020e71b8->unk3D89C;
            x                                       = (func_ov003_020ccedc(1) >> 1) + 0x40000;
            boss->actor.position.x                  = x;
            data_ov003_020e71b8->unk3D7C0[1].unk_20 = x;
            data_ov003_020e71b8->unk3D878 |= 0x40000000;
            break;
        }

        case 1:
            ret = func_ov013_02125d24(data);
            break;

        case 2:
            func_ov013_02125efc(data);
            break;

        case 3:
            func_ov003_020c48fc(data);
            break;
    }
    return ret;
}
