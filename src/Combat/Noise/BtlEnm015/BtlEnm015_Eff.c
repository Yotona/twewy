#include "Combat/Noise/Private/BtlEnm015.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm015Eff {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          unk_60;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ BtlEnm015*   unk_6C;
} BtlEnm015Eff; // Size: 0x70

extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c3c28(void);
extern void func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);

s32 func_ov013_021260e8(TaskPool*, Task*, void*, s32);

const TaskHandle Tsk_BtlEnm015_Eff = {"Tsk_BtlEnm015_Eff", func_ov013_021260e8, 0x70};

void func_ov013_02125fd0(BtlEnm015Eff* data, Enm015Spawn* args) {
    BtlEnm015* owner = args->unk_00;
    s32        flag  = 0;

    MI_CpuSet(data, 0, sizeof(BtlEnm015Eff));
    data->unk_6C = args->unk_00;
    data->unk_60 = args->unk_08;
    data->unk_64 = args->unk_0C;
    data->unk_68 = args->unk_10;

    if (args->unk_04 == 1) {
        flag = 0x118;
    }
    s32 engine = owner->unk_084.sprite.bits_0_1;

    SpriteAnimationEx anim;
    CombatSprite_InitAnim(&anim.anim, engine, func_ov013_021256c0(owner->unk_080));
    s16              unk20 = func_ov013_021256d0(owner->unk_080);
    SpriteAnimEntry* table = func_ov013_021256e4();
    SpriteAnimEntry* entry = &table[args->unk_04];
    anim.anim.bits_10_11   = 1;
    anim.anim.unk_1C       = entry->charDataIndex;
    anim.anim.unk_1E       = (s16)(flag << 5);
    anim.anim.unk_26       = entry->frameDataIndex;
    anim.anim.unk_28       = entry->paletteDataIndex;
    anim.anim.unk_20       = unk20;
    anim.anim.unk_22       = 2;
    anim.anim.animIndex    = entry->animDataIndex + 1;
    anim.unk_2C            = table;
    CombatSprite_Load(&data->sprite, &anim);
    CombatSprite_SetAnimFromTable(&data->sprite, args->unk_04, 1);
    CombatSprite_SetFlip(&data->sprite, data->unk_6C->unk_084.flags46 & 1);
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
            func_ov003_02082730(&data->sprite, 0x7FFFFFFF - data->unk_64);
            CombatSprite_Render(&data->sprite);
        } break;
        case 3:
            CombatSprite_Release(&data->sprite);
            break;
    }
    return 1;
}
