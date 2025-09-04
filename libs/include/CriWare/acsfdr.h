#ifndef ACSFDR_H
#define ACSFDR_H

#include <types.h>

/**
 * @brief Fader
 */
typedef struct {
    /* 0x00 */ s8  unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s8  unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s8  unk_2C[4];
    /* 0x30 */ s8  unk_30[4];
    /* 0x34 */ s8  unk_34[4];
    /* 0x38 */ s8  unk_38[4];
    /* 0x3C */ s8  unk_3C[4];
    /* 0x40 */ s8  unk_40[4];
} ACSFDR; // Size: 0x44

ACSFDR* ACSFDR_Create(ACSFDR* fdr, u16 workSize);

void func_020207d8(ACSFDR* fdr, s32 param_2, s32 param_3, s32 param_4, s32 param_5);

#endif // ACSFDR_H