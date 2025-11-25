#ifndef ADXSTM_H
#define ADXSTM_H

#include <CriWare/cri_cvfs.h>
#include <CriWare/sj.h>
#include <types.h>

/**
 * @brief Stream Controller
 */
typedef struct {
    /* 0x00 */ s8          unk_00;
    /* 0x01 */ s8          unk_01;
    /* 0x02 */ s8          unk_02;
    /* 0x03 */ s8          unk_03;
    /* 0x04 */ SJ          sj;
    /* 0x08 */ CVFSHandle* fileHndl;
    /* 0x0C */ s32         unk_0C;
    /* 0x10 */ u32         file_len;
    /* 0x14 */ u32         unk_14;
    /* 0x18 */ s32         unk_18;
    /* 0x1C */ s32         unk_1C;
    /* 0x20 */ s32         unk_20;
    /* 0x24 */ s32         unk_24;
    /* 0x28 */ SJCK        unk_28;
    /* 0x30 */ s32         req_rd_size;
    /* 0x34 */ s32         unk_34;
    /* 0x38 */ u32         unk_38;
    /* 0x3C */ s32 (*unk_3C)(s32);
    /* 0x40 */ s32   unk_40;
    /* 0x44 */ s32   unk_44;
    /* 0x48 */ s8    unk_48;
    /* 0x49 */ s8    unk_49;
    /* 0x4A */ s8    unk_4A;
    /* 0x4B */ s8    unk_4B;
    /* 0x4C */ s8    unk_4C;
    /* 0x4D */ s8    unk_4D;
    /* 0x4E */ char  unk_4E[0x2];
    /* 0x50 */ s32   unk_50;
    /* 0x54 */ char* filename;
    /* 0x58 */ s32   unk_58;
    /* 0x5C */ s32   unk_5C;
    /* 0x60 */ u32   unk_60;
} ADXSTM;

typedef struct {
    /* 0x00 */ s8  unk_00;
    /* 0x01 */ s8  unk_01;
    /* 0x02 */ s8  unk_02;
    /* 0x03 */ s8  unk_03;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
} ADXSTMWork; // Size: 0x18

void adxstm_BindFileNw(ADXSTM* stm, const char* filename, void* dir, s32, s32, s32);

s32 ADXSTM_SetBufSize(ADXSTM* stm, s32 param_2, s32 param_3);

void func_020168d0(void);

void func_020168dc(void);

#endif // ADXSTM_H