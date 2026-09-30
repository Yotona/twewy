#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm015.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm015Stamp {
    /* 0x00 */ BtlEnm015* unk_00;
    /* 0x04 */ s32        unk_04;
    /* 0x08 */ u16        unk_08;
} BtlEnm015Stamp; // Size: 0x0C

extern s32 func_ov003_020c37f8(void*);
extern s32 func_ov003_020c3c28(void);
extern s32 func_ov003_020ccedc(s32);
extern s32 func_ov003_020ccefc(s32);
extern s16 data_0205e4e0[];

/// Rounds a fixed-point value through a float, matching the original codegen.
#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

static inline s32 Mth_MulFixed(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

s32 func_ov013_02126700(TaskPool*, Task*, void*, s32);

char data_ov013_021275b4[24] = "Tsk_BtlEnm015_EffStamp";

const TaskHandle Tsk_BtlEnm015_EffStamp = {data_ov013_021275b4, func_ov013_02126700, 0xC};

s32 func_ov013_021261e8(s32 arg0, s32 arg1) {
    if (arg0 >= func_ov003_020ccedc(0) + 0x18000 || arg0 <= -0x18000) {
        return 1;
    }
    if (arg1 >= func_ov003_020ccefc(0) + 0x18000 || arg1 <= -0x18000) {
        return 1;
    }
    return 0;
}

s32 func_ov013_02126254(BtlEnm015Stamp* data) {
    s32 count = 0;
    s32 i;

    switch (data->unk_08) {
        case 0: {
            s32 t = data->unk_04;
            if (t % 60 == 0) {
                s32 q2 = t / 60 + 1;
                for (i = 0; i < 5; i++) {
                    s32 angle = i * 0x200;
                    s32 dy    = Mth_MulFixed(data_0205e4e0[angle * 2 + 1], ROUND(q2 * 0x28));
                    s16 tx    = data_0205e4e0[angle * 2];
                    s32 px    = data->unk_00->actor.position.x + dy;
                    s32 dx    = Mth_MulFixed(tx, ROUND(q2 * 0x28));
                    s32 py    = (data->unk_00->actor.position.y + dx) >> 1;
                    if (func_ov013_021261e8(px, py) != 0) {
                        count++;
                        if (count >= 5) {
                            return 0;
                        }
                    } else {
                        func_ov013_021256f0(data->unk_00, 0, px, py, 0);
                    }
                }
            }
            break;
        }

        case 1: {
            s32 t = data->unk_04;
            if (t % 60 == 0) {
                s32 q2 = t / 60 + 1;
                for (i = 0; i < 4; i++) {
                    s32 angle = i * 0x200 + 0x100;
                    s32 dy    = Mth_MulFixed(data_0205e4e0[angle * 2 + 1], ROUND(q2 * 0x28));
                    s16 tx    = data_0205e4e0[angle * 2];
                    s32 px    = data->unk_00->actor.position.x + dy;
                    s32 dx    = Mth_MulFixed(tx, ROUND(q2 * 0x28));
                    s32 py    = (data->unk_00->actor.position.y + dx) >> 1;
                    if (func_ov013_021261e8(px, py) != 0) {
                        count++;
                        if (count >= 4) {
                            return 0;
                        }
                    } else {
                        func_ov013_021256f0(data->unk_00, 0, px, py, 0);
                    }
                }
            }
            break;
        }

        default:
            return count;
    }

    data->unk_04++;
    return 1;
}

s32 func_ov013_021265b0(BtlEnm015Stamp* data) {
    s32        t     = data->unk_04;
    BtlEnm015* boss  = (BtlEnm015*)data_ov003_020e71b8->unk3D89C;
    BtlEnm015* owner = data->unk_00;

    if (t % 60 == 0) {
        s32 sec = t / 60;
        s32 dx;

        if (owner->actor.position.x < boss->actor.position.x) {
            dx = ROUND(0x28 * sec);
        } else {
            dx = ROUND(-(0x28 * sec));
        }

        func_ov013_021256f0(owner, 0, owner->actor.position.x + dx - 0x8000, owner->actor.position.y, 0);

        if (sec >= 5) {
            return 0;
        }
    }

    data->unk_04++;
    return 1;
}

s32 func_ov013_021266c8(BtlEnm015Stamp* data, Enm015Spawn* args) {
    MI_CpuSet(data, 0, sizeof(BtlEnm015Stamp));
    data->unk_00 = args->unk_00;
    data->unk_08 = args->unk_14;
    data->unk_04 = 0x3C;
    return 1;
}

s32 func_ov013_02126700(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm015Stamp* data  = task->data;
    BtlEnm015*      owner = data->unk_00;

    switch (stage) {
        case 0:
            func_ov013_021266c8(data, (Enm015Spawn*)args);
            break;
        case 1:
            if ((owner->actor.flags & 4) || func_ov003_020c3c28() != 0) {
                return 0;
            }
            if (func_ov003_020c37f8(&data->unk_00->unk_084) != 0) {
                return func_ov013_021265b0(data);
            }
            return func_ov013_02126254(data);
        case 2:
        case 3:
            break;
    }
    return 1;
}
