#ifndef ACSFDR_H
#define ACSFDR_H

#include <cri/cri_xpt.h>

/**
 * @brief Fader
 */
typedef struct {
    /* 0x00 */ char unk_00;
    /* 0x04 */ int  unk_04;
    /* 0x08 */ int  unk_08;
    /* 0x0C */ int  unk_0C;
    /* 0x10 */ int  unk_10;
    /* 0x14 */ int  unk_14;
    /* 0x18 */ char unk_18;
    /* 0x1C */ int  unk_1C;
    /* 0x20 */ int  unk_20;
    /* 0x24 */ int  unk_24;
    /* 0x28 */ int  unk_28;
    /* 0x2C */ char unk_2C[4];
    /* 0x30 */ char unk_30[4];
    /* 0x34 */ char unk_34[4];
    /* 0x38 */ char unk_38[4];
    /* 0x3C */ char unk_3C[4];
    /* 0x40 */ char unk_40[4];
} ACSFDR; // Size: 0x44

ACSFDR* ACSFDR_Create(ACSFDR* fdr, unsigned short workSize);

void func_020207d8(ACSFDR* fdr, int param_2, int param_3, int param_4, int param_5);

#endif // ACSFDR_H