#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm015.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm015StampSub {
    /* 0x00 */ BtlEnm015*   unk_00;
    /* 0x04 */ CombatSprite sprite;
    /* 0x64 */ s16          unk_64;
    /* 0x66 */ s16          unk_66;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ s32          unk_6C;
    /* 0x70 */ s32          unk_70;
} BtlEnm015StampSub; // Size: 0x74

extern s32  func_ov003_020c37f8(void*);
extern s32  func_ov003_020c3c28(void);
extern void func_ov003_020c4cc4(void*, s32);
extern void func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern void func_ov003_02087ed8(u16);
extern s32  func_ov003_0208a114(u16);
extern s32  func_ov003_0208a164(s32, void*, s32, s32, s32);
extern s32  func_ov003_0208a08c(s32, void*, s32);
s32         func_ov013_02126a30(TaskPool*, Task*, void*, s32);

const TaskHandle Tsk_BtlEnm015_EffStampSub = {"Tsk_BtlEnm015_EffStampSub", func_ov013_02126a30, 0x74};

s32 func_ov013_02126788(BtlEnm015StampSub* data) {
    s32          engine = func_ov003_020c37f8(&data->unk_00->unk_084) != 0;
    CombatActor* base;
    s32          se;

    if (engine != 0) {
        base = data_ov003_020e71b8->unk3D89C;
    } else {
        base = data_ov003_020e71b8->unk3D898;
    }
    se = (engine != 0) ? 0x8A : 0x88;

    switch (data->unk_64) {
        case 0: {
            s32 h;

            data->unk_70 += 0xF000;
            if (data->unk_70 >= 0) {
                s32 r;

                func_ov013_02125838(data->unk_00);
                if (SndMgr_IsSEPlaying(0x227) == 0) {
                    func_ov003_02087ed8(0x227);
                }
                h = func_ov003_0208a114(se);
                r = func_ov003_0208a164(h, &data->unk_00->actor.unk_04, data->unk_68, data->unk_6C, data->unk_70);
                if (r == 1) {
                    func_ov003_020c4cc4(data->unk_00, 0x228);
                } else if (r != 2) {
                    if (func_ov003_0208a08c(engine, base, 0) != 0) {
                        base->unk_10 = 1;
                    }
                }
                data->unk_64 = data->unk_64 + 1;
                data->unk_66 = 0x3C;
            }
            break;
        }

        case 1:
            data->unk_66 = data->unk_66 - 1;
            if (data->unk_66 < 0) {
                return 0;
            }
            break;
    }

    CombatSprite_Update(&data->sprite);
    return 1;
}

void func_ov013_021268d8(BtlEnm015StampSub* data) {
    s16 x;
    s16 y;

    func_ov003_02084348(func_ov003_020c37f8(&data->unk_00->unk_084) != 0, &x, &y, data->unk_68, data->unk_6C, 0);
    CombatSprite_SetPosition(&data->sprite, x, y);
    func_ov003_02082730(&data->sprite, 0x7FFFFFFF - data->unk_6C);
    CombatSprite_Render(&data->sprite);
}

void func_ov013_02126950(BtlEnm015StampSub* data) {
    CombatSprite_Release(&data->sprite);
}

void func_ov013_02126960(BtlEnm015StampSub* data, Enm015Spawn* args) {
    BtlEnm015* owner = args->unk_00;

    MI_CpuSet(data, 0, sizeof(BtlEnm015StampSub));
    data->unk_00 = args->unk_00;
    data->unk_68 = args->unk_08;
    data->unk_6C = args->unk_0C;
    data->unk_70 = 0;

    u16* table  = (u16*)func_ov013_021256e4();
    s32  engine = owner->unk_084.sprite.bits_0_1;

    SpriteAnimationEx anim;
    CombatSprite_InitAnim(&anim.anim, engine, func_ov013_021256c0(owner->unk_080));
    u16 animData         = table[3];
    anim.anim.unk_20     = func_ov013_021256d0(owner->unk_080);
    anim.unk_2C          = 0;
    anim.anim.unk_26     = table[2];
    anim.anim.unk_28     = table[1];
    anim.anim.unk_1C     = table[0];
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_22     = 2;
    anim.anim.animIndex  = animData + 1;
    CombatSprite_Load(&data->sprite, &anim);
}

s32 func_ov013_02126a30(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm015StampSub* data  = task->data;
    BtlEnm015*         owner = data->unk_00;

    switch (stage) {
        case 0:
            func_ov013_02126960(data, (Enm015Spawn*)args);
            break;
        case 1:
            if ((owner->actor.flags & 4) || func_ov003_020c3c28() != 0) {
                return 0;
            }
            return func_ov013_02126788(data);
        case 2:
            func_ov013_021268d8(data);
            break;
        case 3:
            func_ov013_02126950(data);
            break;
    }
    return 1;
}
