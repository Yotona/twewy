#ifndef ADXSTM_H
#define ADXSTM_H

#include <cri/cri_cvfs.h>
#include <cri/cri_xpt.h>
#include <cri/sj.h>

/**
 * @brief Stream Controller
 */
// Field names follow recvx's ADXSTM_FILE where the layouts coincide, else 3s.
typedef struct {
    /* 0x00 */ char         used;
    /* 0x01 */ char         stat;
    /* 0x02 */ char         rdflg;
    /* 0x03 */ char         unk_03;
    /* 0x04 */ SJ           sj;
    /* 0x08 */ CVFSHandle*  fp;
    /* 0x0C */ int          fofst;
    /* 0x10 */ long long    fsize; // bytes; 64-bit on NITRO
    /* 0x18 */ int          fnsct;
    /* 0x1C */ int          maxsize;
    /* 0x20 */ int          minsize;
    /* 0x24 */ int          reqsct;
    /* 0x28 */ SJCK         reqck;
    /* 0x30 */ int          rdsct;
    /* 0x34 */ int          esct;
    /* 0x38 */ unsigned int tbyte;
    /* 0x3C */ void (*eosfunc)(void*);
    /* 0x40 */ void*        eosobj;
    /* 0x44 */ int          unk_44;
    /* 0x48 */ char         pause;
    /* 0x49 */ char         unk_49;
    /* 0x4A */ char         unk_4A;
    /* 0x4B */ char         unk_4B;
    /* 0x4C */ char         unk_4C;
    /* 0x4D */ char         unk_4D;
    /* 0x4E */ char         unk_4E[0x2];
    /* 0x50 */ int          errcnt;
    /* 0x54 */ char*        filename;
    /* 0x58 */ void*        dir;
    /* 0x5C */ int          stpos;
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

void ADXSTM_BindFileNw(ADXSTM* stm, const char* fname, void* dir, int fofst, long long fsize);
void adxstm_BindFileNw(ADXSTM* stm, const char* fname, void* dir, int fofst, long long fsize);

int ADXSTM_SetBufSize(ADXSTM* stm, int param_2, int param_3);

void func_020168d0(void);

void func_020168dc(void);

#endif // ADXSTM_H