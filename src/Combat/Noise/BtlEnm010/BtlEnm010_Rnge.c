#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm010Rnge {
    /* 0x00 */ CombatSprite    sprite;
    /* 0x60 */ BtlEnm010Owner* unk_060;
    /* 0x64 */ s32             unk_064;
    /* 0x68 */ s32             unk_068;
} BtlEnm010Rnge;

extern char data_ov011_0212cc10[20];
s32         func_ov011_02127ce0(TaskPool* arg0, Task* arg1, void* arg2, s32 index);

extern s32 func_ov003_020c37f8(void* p);
extern s32 func_ov003_020c3c28(void);
extern s32 func_ov003_020cc354(void* p);
extern s16 func_ov003_020843b0(s32 a, s32 b);
extern s32 func_ov003_020843ec(s32 a, s32 b, s32 c);
extern s32 func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32 func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32 func_ov003_020c3c28(void);
extern s32 func_ov003_020843ec(s32 a0, s32 a1, s32 a2);

const TaskHandle data_ov011_0212c1d4 = {(const char*)data_ov011_0212cc10, func_ov011_02127ce0, 108};

char data_ov011_0212cc10[20] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D,
                                0x30, 0x31, 0x30, 0x5F, 0x52, 0x6E, 0x67, 0x65, 0x00, 0x00};

s32 func_ov011_02127c84(void* p) {
    BtlEnm010Owner* data;
    TaskPool*       pool;

    data = p;
    if (func_ov003_020c37f8(&data->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    return EasyTask_CreateTask(pool, &data_ov011_0212c1d4, 0, 0, 0, (void*)&data);
}

s32 func_ov011_02127ce0(TaskPool* arg0, Task* arg1, void* arg2, s32 index) {
    BtlEnm010Rnge*   data;
    BtlEnm010Owner** spawn;
    s32              r;
    s32              snd;
    s32              pan;
    s32              bias;
    s32              y;

    data  = arg1->data;
    spawn = arg2;
    r     = 1;
    switch (index) {
        case 0:
            MI_CpuSet(data, 0, 0x6C);
            func_ov011_021258b4((*spawn)->sprite.sprite.bits_0_1, &data->sprite, 0);
            CombatSprite_SetAnimFromTable(&data->sprite, 0, r);
            data->unk_060 = *spawn;
            if ((*spawn)->actor.isFlipped == 1) {
                CombatSprite_SetFlip(&data->sprite, r);
            } else {
                CombatSprite_SetFlip(&data->sprite, 0);
            }
            break;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 0;
            }
            if (func_ov003_020cc354(data->unk_060) != 0 || (data->unk_060->actor.flags & 0x200) != 0 ||
                (data->unk_060->actor.flags & 0x4000) != 0)
            {
                return 0;
            }
            CombatSprite_Update(&data->sprite);
            if (func_ov003_020c37f8(&data->sprite) != 0) {
                snd = 0x5B;
                pan = func_ov003_020843b0(r, data->unk_060->actor.position.x);
            } else {
                snd = 0x53;
                pan = func_ov003_020843b0(0, data->unk_060->actor.position.x);
            }
            bias = 0x80000;
            if (data->unk_060->actor.isFlipped == 0) {
                bias = -bias;
            }
            if (data->sprite.sprite.cellIndex == 2 && data->sprite.sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1DB, pan);
            }
            if (data->sprite.sprite.cellIndex >= 4 && data->sprite.sprite.cellIndex <= 6) {
                if (func_ov003_020c5b2c((u16)snd, data->unk_060->actor.position.x + bias, data->unk_060->actor.position.y,
                                        data->unk_060->actor.position.z, data->unk_060->actor.position.z) == 1)
                {
                    func_ov003_02087f00(0x1DC, pan);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&data->sprite.sprite) != 0) {
                r = 0;
            }
            break;
        case 2:
            if (data->sprite.sprite.bits_0_1) {
                pan = func_ov003_020843b0(0, data->unk_060->actor.position.x);
                y   = func_ov003_020843ec(0, data->unk_060->actor.position.y, data->unk_060->actor.position.z);
            } else {
                pan = func_ov003_020843b0(r, data->unk_060->actor.position.x);
                y   = func_ov003_020843ec(r, data->unk_060->actor.position.y, data->unk_060->actor.position.z);
            }
            if (data->unk_060->actor.isFlipped == 0) {
                pan = (s16)(pan - 0x20);
            } else {
                pan = (s16)(pan + 0x20);
            }
            CombatSprite_SetPosition(&data->sprite, pan, y);
            func_ov003_02082730(&data->sprite, 0x7FFFFFFF - (data->unk_060->actor.position.y + 0x20000));
            CombatSprite_Render(&data->sprite);
            break;
        case 3:
            CombatSprite_Release(&data->sprite);
            break;
    }
    return r;
}
