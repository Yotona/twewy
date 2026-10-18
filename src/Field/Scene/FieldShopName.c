#include "Field/Scene/FieldShopName.h"
#include "Util/SysFontMsgs.h"
#include <nitro/types.h>

typedef struct {
    u16 message;
    u8  unk_02;
    u8  unk_03;
    u32 unk_04;
} Field_020c5404_Entry;

static const Field_020c5404_Entry data_ov030_020e7380[] = {
    { SYSMSG_SHOP_NAMES_START + 0,  0, 0, 1},
    { SYSMSG_SHOP_NAMES_START + 5,  5, 0, 1},
    {SYSMSG_SHOP_NAMES_START + 10, 10, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 11, 11, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 12, 12, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 13, 13, 0, 1},
    {SYSMSG_SHOP_NAMES_START + 18, 18, 0, 1},
    {SYSMSG_SHOP_NAMES_START + 23, 23, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 24, 24, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 25, 25, 0, 1},
    {SYSMSG_SHOP_NAMES_START + 30, 30, 0, 1},
    {SYSMSG_SHOP_NAMES_START + 35, 35, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 36, 36, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 37, 37, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 40, 40, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 39, 39, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 38, 38, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 41, 41, 0, 0},
    {SYSMSG_SHOP_NAMES_START + 42, 42, 0, 1},
};

s32 func_ov030_020c5404(s32 index) {
    if (index < 70 || index >= 89) {
        return 0;
    }
    return data_ov030_020e7380[index - 70].unk_02;
}

u16 func_ov030_020c5430(s32 index) {
    if (index < 70 || index >= 89) {
        return 0;
    }
    return data_ov030_020e7380[index - 70].message;
}

s32 func_ov030_020c5460(s32 index) {
    if (index < 70 || index >= 89) {
        return 0;
    }
    return data_ov030_020e7380[index - 70].unk_04;
}
