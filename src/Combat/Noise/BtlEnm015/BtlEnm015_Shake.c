#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm015.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm015Shake {
    /* 0x00 */ BtlEnm015*   unk_00;
    /* 0x04 */ Ov003Camera* unk_04;
    /* 0x08 */ s32          unk_08;
    /* 0x0C */ u16          unk_0C;
} BtlEnm015Shake; // Size: 0x10

extern s32 func_ov003_020c37f8(void*);
extern s32 func_ov003_020c3c28(void);
s32        func_ov013_02126ff4(TaskPool*, Task*, void*, s32);

const TaskHandle Tsk_BtlEnm015_Shake = {"Tsk_BtlEnm015_Shake", func_ov013_02126ff4, 0x10};

s32 func_ov013_02126ef0(BtlEnm015Shake* data) {
    data->unk_0C = (RNG_Next(3) + 1 + data->unk_0C) % 4;

    switch (data->unk_0C) {
        case 0:
            data->unk_04->unk_48 = -data->unk_08;
            break;

        case 1:
            data->unk_04->unk_48 = data->unk_08;
            break;

        case 2:
            data->unk_04->unk_48 = data->unk_08;
            break;

        case 3:
            data->unk_04->unk_48 = -data->unk_08;
            break;
    }

    data->unk_08 = data->unk_08 - 0x800;
    return data->unk_08 > 0;
}

s32 func_ov013_02126f8c(BtlEnm015Shake* data, BtlEnm015* args) {
    MI_CpuSet(data, 0, sizeof(BtlEnm015Shake));
    data->unk_00 = args;

    if (func_ov003_020c37f8(&args->unk_084) != 0) {
        data->unk_04 = &data_ov003_020e71b8->unk3D7C0[1];
    } else {
        data->unk_04 = &data_ov003_020e71b8->unk3D7C0[0];
    }

    data->unk_08 = 0x5000;
    return 1;
}

s32 func_ov013_02126ff4(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlEnm015Shake* data  = task->data;
    BtlEnm015*      owner = data->unk_00;

    switch (stage) {
        case 0:
            func_ov013_02126f8c(data, (BtlEnm015*)args);
            break;
        case 1:
            if ((owner->actor.flags & 4) || func_ov003_020c3c28() != 0) {
                return 0;
            }
            return func_ov013_02126ef0(data);
        case 2:
            break;
        case 3:
            *((func_ov003_020c37f8(&owner->unk_084) != 0) ? &data_ov013_02127640 : &data_ov013_02127644) = -1;
            data->unk_04->unk_44                                                                         = 0;
            data->unk_04->unk_48                                                                         = 0;
            data->unk_04->unk_4C                                                                         = 0;
            break;
    }
    return 1;
}
