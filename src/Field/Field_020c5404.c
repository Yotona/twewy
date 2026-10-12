#include <nitro/types.h>

typedef struct {
    u16 message;
    u8  unk_02;
    u8  unk_03;
    u32 unk_04;
} Field_020c5404_Entry;

static const Field_020c5404_Entry data_ov030_020e7380[] = {
    {0x2fb8,  0, 0, 1},
    {0x2fbd,  5, 0, 1},
    {0x2fc2, 10, 0, 0},
    {0x2fc3, 11, 0, 0},
    {0x2fc4, 12, 0, 0},
    {0x2fc5, 13, 0, 1},
    {0x2fca, 18, 0, 1},
    {0x2fcf, 23, 0, 0},
    {0x2fd0, 24, 0, 0},
    {0x2fd1, 25, 0, 1},
    {0x2fd6, 30, 0, 1},
    {0x2fdb, 35, 0, 0},
    {0x2fdc, 36, 0, 0},
    {0x2fdd, 37, 0, 0},
    {0x2fe0, 40, 0, 0},
    {0x2fdf, 39, 0, 0},
    {0x2fde, 38, 0, 0},
    {0x2fe1, 41, 0, 0},
    {0x2fe2, 42, 0, 1},
};

s32 func_ov030_020c5404(s32 index) {
    if (index < 0x46 || index >= 0x59) {
        return 0;
    }
    return data_ov030_020e7380[index - 0x46].unk_02;
}

s32 func_ov030_020c5430(s32 index) {
    if (index < 0x46 || index >= 0x59) {
        return 0;
    }
    return data_ov030_020e7380[index - 0x46].message;
}

s32 func_ov030_020c5460(s32 index) {
    if (index < 0x46 || index >= 0x59) {
        return 0;
    }
    return data_ov030_020e7380[index - 0x46].unk_04;
}
