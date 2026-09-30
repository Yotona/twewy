#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm015.h"

#include <nitro/mi/cpumem.h>

extern void func_ov003_020c4cc4(void*, s32);

extern s32  func_ov003_020ccedc(s32);
extern void func_ov003_020c492c(void*);
extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32  func_ov003_0208a08c(s32, void*, s32);
s32         func_ov013_02127370(TaskPool*, Task*, void*, s32);
void        func_ov013_0212710c(BtlEnm015*);
void        func_ov013_02127230(BtlEnm015*);

char data_ov013_02127610[48] = "Tsk_BtlEnm015_UG";

const TaskHandle Tsk_BtlEnm015_UG = {data_ov013_02127610, func_ov013_02127370, 0x1EC};

void func_ov013_021270a0(BtlEnm015* arg0) {
    switch (arg0->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 0, 0);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1:
            func_ov013_02125a04(arg0, (s32)func_ov013_02127230, (s32)func_ov013_0212710c, arg0->unk_19C);
            break;
    }
}

void func_ov013_0212710c(BtlEnm015* arg0) {
    BtlEnm015* boss = (BtlEnm015*)data_ov003_020e71b8->unk3D898;

    switch (arg0->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 7, 1);
            func_ov003_020c4cc4(arg0, 0x220);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1:
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_084.sprite) == 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 8, 1);
            func_ov003_020c4cc4(arg0, 0x226);
            func_ov013_021257a4(arg0, arg0->unk_1E8 & 1);
            arg0->unk_1E8 = arg0->unk_1E8 + 1;
            func_ov003_0208a08c(0, boss, 0);
            func_ov013_02125838(arg0);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 2:
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_084.sprite) == 0) {
                return;
            }
            func_ov013_02125b64(arg0, (s32)func_ov013_021270a0);
            break;
    }
}

void func_ov013_02127230(BtlEnm015* arg0) {
    BtlEnm015* boss = (BtlEnm015*)data_ov003_020e71b8->unk3D898;

    switch (arg0->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&arg0->unk_084, 9, 1);
            arg0->unk_1DC = boss->actor.position.x;
            arg0->unk_1E0 = boss->actor.position.y;
            arg0->unk_1E4 = 0;
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1: {
            s32 h;

            if (arg0->unk_1C0 == 0) {
                func_ov003_020c4cc4(arg0, 0x220);
            }
            if (arg0->unk_1C0 == 0x2B) {
                func_ov013_021256f0(arg0, 1, arg0->unk_1DC, arg0->unk_1E0, arg0->unk_1E4);
            }
            if (arg0->unk_1C0 == 0x2C) {
                func_ov003_020c4cc4(arg0, 0x224);
                h = func_ov003_0208a114(0x89);
                if (func_ov003_0208a164(h, &arg0->actor.unk_04, arg0->unk_1DC, arg0->unk_1E0, arg0->unk_1E4) == 1) {
                    func_ov003_020c4cc4(arg0, 0x225);
                }
            }
            arg0->unk_1C0 = arg0->unk_1C0 + 1;
            if (SpriteMgr_IsAnimationFinished(&arg0->unk_084.sprite) == 0) {
                return;
            }
            func_ov013_02125b64(arg0, (s32)func_ov013_021270a0);
            break;
        }
    }
}

s32 func_ov013_02127370(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm015* data = task->data;
    s32        ret  = 1;

    switch (stage) {
        case 0:
            MI_CpuSet(data, 0, sizeof(BtlEnm015));
            func_ov013_02125b8c(0, data, (s32)args, (s32)func_ov013_021270a0);
            func_ov013_02125b64(data, (s32)func_ov013_02125e90);
            break;

        case 1:
            ret = func_ov013_02125d24(data);
            if (data->unk_1C4 != func_ov013_02125ed0) {
                data->unk_148          = func_ov003_020ccedc(0) >> 1;
                data->unk_14C          = 0x19000;
                data->unk_150          = 0;
                data->actor.position.x = data->unk_148;
                data->actor.position.y = 0x19000;
                data->actor.position.z = 0;
            }
            break;

        case 2:
            func_ov013_02125efc(data);
            break;

        case 3:
            func_ov003_020c492c(data);
            break;
    }
    return ret;
}
