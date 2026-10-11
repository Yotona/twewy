#include "Field/Ov030Data.h"
#include "Field/TaskHelpers.h"
#include "Save.h"

// USA: 0x020b7c58-0x020b7d74: per-area map lookups.

extern const FieldSlope data_ov030_020e0fb4[3];

extern s32 func_ov030_020b7d74(s32 value);

void func_ov030_020b7c58(s32 unused, s32* out1, s32* out2, s32* out3) {
    *out1 = data_ov030_020e1d64[gSaveData.unk_2458].unk_08;
    *out2 = data_ov030_020e1d64[gSaveData.unk_2458].unk_0C;
    *out3 = *out2 - *out1;
}

s32 func_ov030_020b7c9c(s32 value, s32 unusedY) {
    s32 area = gSaveData.unk_2458;
    if (area == 0) {
        return func_ov030_020b7d74(value);
    }
    if (area == 1) {
        if (value < 0x118000) {
            return 0x78000;
        }
        if (value < 0x167000) {
            return 0x66000;
        }
        if (value < 0x1BE000) {
            return 0x75000;
        }
        return 0x86000;
    }
    return func_ov030_020b7b40() << 12;
}

s32 func_ov030_020b7d04(s32 value) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (value + 0x100000 < data_ov030_020e0fb4[i].limit) {
            break;
        }
    }
    return data_ov030_020e0fb4[i].base + (s32)(((s64)(value + 0x100000) * data_ov030_020e0fb4[i].slope + 0x800) >> 12) -
           0xC1000;
}
