#ifndef ADX_XPND_H
#define ADX_XPND_H

#include <types.h>

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
} ADXPD_OBJ_SUB;

typedef struct {
    /* 0x00 */ s32           used;
    /* 0x04 */ s32           unk4;
    /* 0x08 */ s32           mode;
    /* 0x0C */ s32           stat;
    /* 0x10 */ s32           num_blk;
    /* 0x14 */ s32           unk14;
    /* 0x18 */ s32           unk18;
    /* 0x1C */ s32           unk1C;
    /* 0x20 */ s32           unk20;
    /* 0x24 */ s32           unk24;
    /* 0x28 */ ADXPD_OBJ_SUB unk28;
    /* 0x30 */ s16           unk30;
    /* 0x32 */ s16           unk32;
    /* 0x34 */ s16           unk34;
    /* 0x36 */ s16           unk36;
    /* 0x38 */ s16           unk38;
    /* 0x3A */ s16           unk3A;
} ADXPD_OBJ;

typedef ADXPD_OBJ* ADXPD;

#endif // ADX_XPND_H