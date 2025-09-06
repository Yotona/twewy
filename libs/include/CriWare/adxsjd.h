#ifndef ADXSJD_H
#define ADXSJD_H

#include <CriWare/private/adx_b.h>
#include <CriWare/sj.h>
#include <types.h>

/**
 * @brief Stream Joint Decoder
 */
typedef struct {
    /* 0x00 */ s8    used;   // Decoder in use
    /* 0x01 */ s8    state;
    /* 0x02 */ s8    maxnch; // Max channels
    /* 0x03 */ s8    unk_03;
    /* 0x04 */ ADXB* adxb;
    /* 0x08 */ SJ*   sji;
    /* 0x0C */ SJ*   sjo[2];
    /* 0x14 */ char  unk_14[0x18];
    /* 0x2C */ u32   unk_2C;
    /* 0x30 */ u32   unk_30;
    /* 0x34 */ s32   unk_34;
    /* 0x38 */ s32   unk_38;
    /* 0x3C */ u32   unk_3C;
    /* 0x40 */ u32   unk_40;
    /* 0x44 */ u32   unk_44;
    /* 0x48 */ u32   unk_48;
    /* 0x4C */ u32   unk_4C;
    /* 0x50 */ u32   unk_50;
    /* 0x54 */ u32   unk_54;
    /* 0x58 */ u32   unk_58;
    /* 0x5C */ u32   unk_5C;
    /* 0x60 */ s32   unk_60;
    /* 0x64 */ char  unk_64[0x3C];
    /* 0xA0 */ s32   unk_A0;
    /* 0xA4 */ s32   unk_A4;
    /* 0xA8 */ s32   unk_A8;
    /* 0xAC */ s32   unk_AC;
    /* 0xB0 */ s32   unk_B0;
} ADXSJD; // Size: 0xB4

void ADXSJD_Destroy(ADXSJD* sjd);

s8 ADXSJD_GetStat(ADXSJD* sjd);

void ADXSJD_SetInSj(ADXSJD* sjd, SJ* sj);

#endif // ADXSJD_H
