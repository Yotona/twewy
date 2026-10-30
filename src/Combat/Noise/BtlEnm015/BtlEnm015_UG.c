#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm015.h"

#include "Combat/Core/CombatApi.h"
#include <nitro/mi/cpumem.h>
s32  func_ov013_02127370(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov013_0212710c(BtlEnm015*);
void func_ov013_02127230(BtlEnm015*);

char data_ov013_02127610[48] = "Tsk_BtlEnm015_UG";

const TaskHandle Tsk_BtlEnm015_UG = {data_ov013_02127610, func_ov013_02127370, 0x1EC};

void func_ov013_021270a0(BtlEnm015* arg0) {
    switch (arg0->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&arg0->base.sprite, 0, 0);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1:
            func_ov013_02125a04(arg0, (s32)func_ov013_02127230, (s32)func_ov013_0212710c, arg0->base.unk_19C);
            break;
    }
}

void func_ov013_0212710c(BtlEnm015* arg0) {
    BtlEnm015* boss = (BtlEnm015*)data_ov003_020e71b8->unk3D898;

    switch (arg0->unk_1C2) {
        case 0:
            CombatSprite_SetAnimFromTable(&arg0->base.sprite, 7, 1);
            func_ov003_020c4cc4(&arg0->base, 0x220);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1:
            if (SpriteMgr_IsAnimationFinished(&arg0->base.sprite.sprite) == 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(&arg0->base.sprite, 8, 1);
            func_ov003_020c4cc4(&arg0->base, 0x226);
            func_ov013_021257a4(arg0, arg0->unk_1E8 & 1);
            arg0->unk_1E8 = arg0->unk_1E8 + 1;
            func_ov003_0208a08c(0, &boss->base, 0);
            func_ov013_02125838(arg0);
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 2:
            if (SpriteMgr_IsAnimationFinished(&arg0->base.sprite.sprite) == 0) {
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
            CombatSprite_SetAnimFromTable(&arg0->base.sprite, 9, 1);
            arg0->unk_1DC = boss->base.actor.position.x;
            arg0->unk_1E0 = boss->base.actor.position.y;
            arg0->unk_1E4 = 0;
            arg0->unk_1C2 = arg0->unk_1C2 + 1;
            arg0->unk_1C0 = 0;
            break;

        case 1: {
            s32 h;

            if (arg0->unk_1C0 == 0) {
                func_ov003_020c4cc4(&arg0->base, 0x220);
            }
            if (arg0->unk_1C0 == 0x2B) {
                func_ov013_021256f0(arg0, 1, arg0->unk_1DC, arg0->unk_1E0, arg0->unk_1E4);
            }
            if (arg0->unk_1C0 == 0x2C) {
                func_ov003_020c4cc4(&arg0->base, 0x224);
                h = func_ov003_0208a114(0x89);
                if (func_ov003_0208a164(h, &arg0->base.actor.baseArgs, arg0->unk_1DC, arg0->unk_1E0, arg0->unk_1E4) == 1) {
                    func_ov003_020c4cc4(&arg0->base, 0x225);
                }
            }
            arg0->unk_1C0 = arg0->unk_1C0 + 1;
            if (SpriteMgr_IsAnimationFinished(&arg0->base.sprite.sprite) == 0) {
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
                data->base.collider.x       = func_ov003_020ccedc(0) >> 1;
                data->base.collider.y       = 0x19000;
                data->base.collider.z       = 0;
                data->base.actor.position.x = data->base.collider.x;
                data->base.actor.position.y = 0x19000;
                data->base.actor.position.z = 0;
            }
            break;

        case 2:
            func_ov013_02125efc(data);
            break;

        case 3:
            func_ov003_020c492c(&data->base);
            break;
    }
    return ret;
}
