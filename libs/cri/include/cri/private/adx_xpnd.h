#ifndef ADX_XPND_H
#define ADX_XPND_H

#include <cri/cri_xpt.h>

typedef struct {
    /* 0x00 */ short unk0;
    /* 0x02 */ short unk2;
    /* 0x04 */ short unk4;
    /* 0x06 */ short unk6;
} ADXPD_OBJ_SUB;

typedef struct {
    /* 0x00 */ int           used;
    /* 0x04 */ int           unk4;
    /* 0x08 */ int           mode;
    /* 0x0C */ int           stat;
    /* 0x10 */ int           num_blk;
    /* 0x14 */ int           unk14;
    /* 0x18 */ int           unk18;
    /* 0x1C */ int           unk1C;
    /* 0x20 */ int           unk20;
    /* 0x24 */ int           unk24;
    /* 0x28 */ ADXPD_OBJ_SUB unk28;
    /* 0x30 */ short         unk30;
    /* 0x32 */ short         unk32;
    /* 0x34 */ short         unk34;
    /* 0x36 */ short         unk36;
    /* 0x38 */ short         unk38;
    /* 0x3A */ short         unk3A;
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