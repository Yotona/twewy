#ifndef ADX_XPND_H
#define ADX_XPND_H

#include <cri/cri_xpt.h>

// Field names follow the recvx decomp's ADX_XPDOBJ/ADXPDPRM (PS2).
typedef struct {
    /* 0x00 */ int nch;
    /* 0x04 */ int ibuf;
    /* 0x08 */ int nblk;
    /* 0x0C */ int obuf_l;
    /* 0x10 */ int obuf_r;
} ADXPDPRM;

typedef struct {
    /* 0x00 */ int      used;
    /* 0x04 */ int      xno;
    /* 0x08 */ int      mode;
    /* 0x0C */ int      stat;
    /* 0x10 */ int      ndecblk;
    /* 0x14 */ ADXPDPRM xprm;
    /* 0x28 */ short    dly[2][2]; // [channel][tap]
    /* 0x30 */ short    k[2];      // predictor coefficients (Q12)
                                   // Not in recvx: scale-key LCG for scrambled ADX (see ADX_DecodeMono4).
    /* 0x34 */ short key;
    /* 0x36 */ short key_mul;
    /* 0x38 */ short key_add;
    /* 0x3A */ short pad3A;
} ADXPD_OBJ;

typedef ADXPD_OBJ* ADXPD;

// Prototypes were absent here, which let ADXB_EvokeExpandSte() in adx_bsc.c call
// ADXPD_EntrySte() with 4 of its 5 arguments and silently leave ADXPD::unk24
// (the stereo inter-channel buffer, forwarded to ADX_DecodeSte4 by
// ADXPD_ExecHndl) unwritten. The declarations below make that a compile error.
// Signatures are taken verbatim from the definitions in adx_xpnd.c.
//
// ADXPD_SetCoef / ADXPD_SetDly / ADXPD_GetDly / ADXPD_SetExtPrm /
// ADXPD_GetExtPrm are deliberately still undeclared: existing call sites in
// adx_bsc.c pass `int*` where the definitions take `short*`/`unsigned short*` (e.g.
// `ADXPD_GetDly(adxb->adxpd, &adxb->unkAC, &adxb->unkB0)` with both fields
// declared `int`). Declaring them would surface those as new diagnostics, so
// they are left for a separate pass.
void  ADXPD_Init(void);
ADXPD ADXPD_Create(void);
void  ADXPD_Destroy(ADXPD adxpd);
int   ADXPD_GetStat(ADXPD adxpd);
int   ADXPD_EntryMono(ADXPD adxpd, int arg1, int arg2, int arg3, int arg4);
int   ADXPD_EntrySte(ADXPD adxpd, int arg1, int arg2, int arg3, int arg4);
void  ADXPD_Start(ADXPD adxpd);
void  ADXPD_Stop(ADXPD adxpd);
void  ADXPD_Reset(ADXPD adxpd);
int   ADXPD_GetNumBlk(ADXPD adxpd);
void  ADXPD_ExecHndl(ADXPD adxpd);

#endif // ADX_XPND_H