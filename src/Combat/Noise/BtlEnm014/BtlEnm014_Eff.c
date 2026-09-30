#include "Combat/Noise/Private/BtlEnm014.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm014Eff {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          unk_60;
    /* 0x62 */ s16          unk_62;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ s32          unk_6C;
    /* 0x70 */ BtlEnm014*   unk_70;
    /* 0x74 */ s32          unk_74;
    /* 0x78 */ u16          unk_78;
    /* 0x7A */ u16          unk_7A;
} BtlEnm014Eff; // Size: 0x7C

extern void func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c3c28(void);
extern s32  func_ov003_020c59a0(void*);

s32 func_ov012_02125ab4(TaskPool*, Task*, void*, s32);

const TaskHandle Tsk_BtlEnm014_Eff = {"Tsk_BtlEnm014_Eff", func_ov012_02125ab4, 0x7C};

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
    CombatSprite_SetAnimFromTable(&data->sprite, args->unk_08, 1);
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
