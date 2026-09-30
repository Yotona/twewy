#ifndef ADX_XPND_H
#define ADX_XPND_H

#include <nitro/types.h>

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

// Prototypes were absent here, which let ADXB_EvokeExpandSte() in adx_bsc.c call
// ADXPD_EntrySte() with 4 of its 5 arguments and silently leave ADXPD::unk24
// (the stereo inter-channel buffer, forwarded to ADX_DecodeSte4 by
// ADXPD_ExecHndl) unwritten. The declarations below make that a compile error.
// Signatures are taken verbatim from the definitions in adx_xpnd.c.
//
// ADXPD_SetCoef / ADXPD_SetDly / ADXPD_GetDly / ADXPD_SetExtPrm /
// ADXPD_GetExtPrm are deliberately still undeclared: existing call sites in
// adx_bsc.c pass `s32*` where the definitions take `s16*`/`u16*` (e.g.
// `ADXPD_GetDly(adxb->adxpd, &adxb->unkAC, &adxb->unkB0)` with both fields
// declared `s32`). Declaring them would surface those as new diagnostics, so
// they are left for a separate pass.
void  ADXPD_Init(void);
ADXPD ADXPD_Create(void);
void  ADXPD_Destroy(ADXPD adxpd);
s32   ADXPD_GetStat(ADXPD adxpd);
s32   ADXPD_EntryMono(ADXPD adxpd, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32   ADXPD_EntrySte(ADXPD adxpd, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void  ADXPD_Start(ADXPD adxpd);
void  ADXPD_Stop(ADXPD adxpd);
void  ADXPD_Reset(ADXPD adxpd);
s32   ADXPD_GetNumBlk(ADXPD adxpd);
void  ADXPD_ExecHndl(ADXPD adxpd);

#endif // ADX_XPND_H