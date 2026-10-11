#include "Field/Ov030Data.h"
#include "Save.h"

// USA: 0x020b7d74-0x020b7dd8: floor height of area 0.

extern const FieldSlope data_ov030_020e14f8[6];

s32 func_ov030_020b7d74(s32 value) {
    u32 i;

    for (i = 0; i < 5; i++) {
        if (value < data_ov030_020e14f8[i].limit) {
            break;
        }
    }
    return data_ov030_020e14f8[i].base + (s32)(((s64)value * data_ov030_020e14f8[i].slope + 0x800) >> 12);
}
