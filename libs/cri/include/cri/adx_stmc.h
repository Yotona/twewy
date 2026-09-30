#ifndef ADXSTM_H
#define ADXSTM_H

#include <cri/cri_cvfs.h>
#include <cri/cri_xpt.h>
#include <cri/sj.h>

/**
 * @brief Stream Controller
 */
typedef struct {
    /* 0x00 */ char         unk_00;
    /* 0x01 */ char         unk_01;
    /* 0x02 */ char         unk_02;
    /* 0x03 */ char         unk_03;
    /* 0x04 */ SJ           sj;
    /* 0x08 */ CVFSHandle*  fileHndl;
    /* 0x0C */ int          unk_0C;
    /* 0x10 */ unsigned int file_len;
    /* 0x14 */ unsigned int unk_14;
    /* 0x18 */ int          unk_18;
    /* 0x1C */ int          unk_1C;
    /* 0x20 */ int          unk_20;
    /* 0x24 */ int          unk_24;
    /* 0x28 */ SJCK         unk_28;
    /* 0x30 */ int          req_rd_size;
    /* 0x34 */ int          unk_34;
    /* 0x38 */ unsigned int unk_38;
    /* 0x3C */ int (*unk_3C)(int);
    /* 0x40 */ int          unk_40;
    /* 0x44 */ int          unk_44;
    /* 0x48 */ char         unk_48;
    /* 0x49 */ char         unk_49;
    /* 0x4A */ char         unk_4A;
    /* 0x4B */ char         unk_4B;
    /* 0x4C */ char         unk_4C;
    /* 0x4D */ char         unk_4D;
    /* 0x4E */ char         unk_4E[0x2];
    /* 0x50 */ int          unk_50;
    /* 0x54 */ char*        filename;
    /* 0x58 */ int          unk_58;
    /* 0x5C */ int          unk_5C;
    /* 0x60 */ unsigned int unk_60;
} ADXSTM;

typedef struct {
    /* 0x00 */ char unk_00;
    /* 0x01 */ char unk_01;
    /* 0x02 */ char unk_02;
    /* 0x03 */ char unk_03;
    /* 0x04 */ int  unk_04;
    /* 0x08 */ int  unk_08;
    /* 0x0C */ int  unk_0C;
    /* 0x10 */ int  unk_10;
    /* 0x14 */ int  unk_14;
} ADXSTMWork; // Size: 0x18

void adxstm_BindFileNw(ADXSTM* stm, const char* filename, void* dir, int, int, int);

int ADXSTM_SetBufSize(ADXSTM* stm, int param_2, int param_3);

void func_020168d0(void);

void func_020168dc(void);

#endif // ADXSTM_H