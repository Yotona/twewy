#ifndef CRIWARE_ADX_B_H
#define CRIWARE_ADX_B_H

#include <cri/cri_xpt.h>
#include <cri/private/adx_xpnd.h>

typedef struct {
    char pad0[0x10];
} ADX_UNK;

// recvx's AdxDecPara: the decode request handed to ADXPD.
typedef struct {
    /* 0x00 */ int   ibuf;
    /* 0x04 */ int   niblk;
    /* 0x08 */ int   nch;
    /* 0x0C */ int   blksize;
    /* 0x10 */ int   blknsmpl;
    /* 0x14 */ void* pcmbuf;
    /* 0x18 */ int   pcmbsize;
    /* 0x1C */ int   pcmbdist;
    /* 0x20 */ int   wpos;
    /* 0x24 */ int   nroom;
    /* 0x28 */ int   lp_nsmpl;
} ADXB_DECPARA; // Size: 0x2C

// Field names follow recvx's ADX_BASIC. NITRO drops its 8-byte block at
// 0x48 (unk48/unk4A/unk4C), so everything from dp on sits 8 bytes lower.
typedef struct ADXB_OBJ {
    /* 0x00 */ short        used;
    /* 0x02 */ short        hdcdflag;
    /* 0x04 */ int          stat;
    /* 0x08 */ ADXPD        adxpd;
    /* 0x0C */ char         code;
    /* 0x0D */ char         bps;
    /* 0x0E */ char         nch;
    /* 0x0F */ char         blklen;
    /* 0x10 */ int          blknsmpl;
    /* 0x14 */ int          sfreq;
    /* 0x18 */ int          total_nsmpl;
    /* 0x1C */ short        cof;
    /* 0x1E */ char         pad1E[2];
    /* 0x20 */ int          lp_ins_nsmpl;
    /* 0x24 */ short        nloop;
    /* 0x26 */ short        lp_type;
    /* 0x28 */ int          lp_spos;
    /* 0x2C */ int          lp_sofst;
    /* 0x30 */ int          lp_epos;
    /* 0x34 */ int          lp_eofst;
    /* 0x38 */ int          maxnch;
    /* 0x3C */ void*        pcmbuf;
    /* 0x40 */ int          pcmbsize;
    /* 0x44 */ int          pcmbdist;
    /* 0x48 */ ADXB_DECPARA dp;
    /* 0x74 */ int          ndecsmpl;
    /* 0x78 */ void* (*getwrfunc)(void*, int*, int*, int*);
    /* 0x7C */ void* getwrobj;
    /* 0x80 */ void (*addwrfunc)(void*, int, int);
    /* 0x84 */ int     addwrobj;
    /* 0x88 */ int     total_ndecsmpl;
    /* 0x8C */ int     curwpos;
    /* 0x90 */ int     total_decsmpl;
    /* 0x94 */ int     total_decdtlen;
    /* 0x98 */ short   format;
    /* 0x9A */ short   unk9A;
    /* 0x9C */ short   unk9C;
    /* 0x9E */ short   unk9E;
    /* 0xA0 */ short   unkA0;
    /* 0xA2 */ short   unkA2;
    /* 0xA4 */ short   unkA4;
    /* 0xA6 */ short   unkA6;
    /* 0xA8 */ short   unkA8;
    /* 0xAA */ short   unkAA;
    /* 0xAC */ int     unkAC;
    /* 0xB0 */ int     unkB0;
    /* 0xB4 */ int     unkB4;
    /* 0xB8 */ int     unkB8;
    /* 0xBC */ int     unkBC;
    /* 0xC0 */ int     ainf_len;
    /* 0xC4 */ ADX_UNK unkC4;
    /* 0xD4 */ short   def_out_vol;
    /* 0xD6 */ short   def_pan[2];
    /* 0xDA */ char    padDA[2];
    /* 0xDC */ int     unkDC;
    /* 0xE0 */ int     unkE0;
    /* 0xE4 */ void (*unkE4)(void* obj, int nbyte, int pcm_nbyte); // decode-progress callback
    /* 0xE8 */ void* unkE8;                                        // its object
} ADXB_OBJ;                                                        // Size: 0xEC

typedef ADXB_OBJ* ADXB;

#endif // CRIWARE_ADX_B_H