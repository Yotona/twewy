#include <cri/private/adx_b.h>
#include <cri/private/adx_bsc_tbl.h>

#define ADXB_MAX_OBJ 4
#define MIN(a, b)    ((a) <= (b) ? (a) : (b))

int skg_init_count                 = 0;
int skg_err_func                   = 0;
int skg_err_obj                    = 0;
void (*ahxsetextfunc)(int, short*) = NULL;
int pl2encodefunc                  = 0;
void (*pl2resetfunc)()             = NULL;
short    adxb_def_k0               = 0;
short    adxb_def_km               = 0;
short    adxb_def_ka               = 0;
ADXB_OBJ adxb_obj[ADXB_MAX_OBJ]    = {0};

int data_0206b87c;

// forward decls
void ADXB_Destroy(ADXB adxb);
int  adxb_get_key(ADXB adxb, unsigned char arg1, unsigned char arg2, int arg3, short* arg4, short* arg5, short* arg6);
void func_02012ed8(ADXB adxb);

static int SKG_Init(void) {
    skg_init_count++;
    return 0;
}

static int SKG_GenerateKey(char* arg0, int arg1, unsigned short* arg2, unsigned short* arg3, unsigned short* arg4) {
    short var_a0;
    short var_t0;
    short var_t2;
    int   i;

    if (skg_init_count == 0) {
        SKG_Init();
    }

    *arg2 = 0;
    *arg3 = 0;
    *arg4 = 0;

    if (arg0 == 0 && arg1 <= 0) {
        return 0;
    }

    var_t2 = skg_prim_tbl[0x100];
    for (i = 0; i < arg1; i++) {
        var_t2 = skg_prim_tbl[(var_t2 * skg_prim_tbl[arg0[i] + 0x80]) % 0x400];
    }

    var_t0 = skg_prim_tbl[0x200];
    for (i = 0; i < arg1; i++) {
        var_t0 = skg_prim_tbl[(var_t0 * skg_prim_tbl[arg0[i] + 0x80]) % 0x400];
    }

    var_a0 = skg_prim_tbl[0x300];
    for (i = 0; i < arg1; i++) {
        var_a0 = skg_prim_tbl[(var_a0 * skg_prim_tbl[arg0[i] + 0x80]) % 0x400];
    }

    *arg2 = var_t2;
    *arg3 = var_t0;
    *arg4 = var_a0;
    return 0;
}

void func_02012248(int arg0) {
    data_0206b87c = arg0;
}

int func_02012258(void) {
    return data_0206b87c;
}

void ADXB_Init() {
    ADXPD_Init();
    SKG_Init();
    __builtin__clear(adxb_obj, sizeof(adxb_obj));
    func_02012248(0);
}

void* adxb_DefGetWr(void* object, int* arg1, int* arg2, int* arg3) {
    ADXB adxb = (ADXB)object;

    *arg1 = adxb->curwpos;
    *arg2 = adxb->pcmbsize - adxb->curwpos;
    *arg3 = adxb->total_nsmpl - adxb->total_ndecsmpl;

    return adxb->pcmbuf;
}

void adxb_DefAddWr(void* object, int arg1, int arg2) {
    ADXB adxb = (ADXB)object;

    adxb->curwpos += arg2;
    adxb->total_ndecsmpl += arg2;
}

// adxb_clear. The 16 bytes at 0xC4 are unkC4. The target inlines this clear as
// four strb per iteration over four iterations; a memset() with a constant size
// is left as a real call by mwcc, so it has to be spelled out for the shape to
// come out.
void func_020122fc(ADXB adxb) {
    char* p = (char*)&adxb->unkC4;
    int   i;

    adxb->ainf_len    = 0;
    adxb->def_out_vol = 0;
    adxb->def_pan[0]  = -0x80;
    adxb->def_pan[1]  = -0x80;

    for (i = 0; i < 4; i++) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p += 4;
    }
}

ADXB ADXB_Create(int arg0, void* arg1, int arg2, int arg3) {
    ADXB  adxb;
    ADXB  chk_adxb;
    ADXPD adxpd;
    int   i;

    chk_adxb = &adxb_obj[0];

    for (i = 0; i < ADXB_MAX_OBJ; i++, chk_adxb++) {
        if (chk_adxb->used == 0) {
            break;
        }
    }

    if (i == ADXB_MAX_OBJ) {
        return NULL;
    }

    adxb = &adxb_obj[i];
    memset(adxb, 0, sizeof(ADXB_OBJ));
    adxb->used  = 1;
    adxpd       = ADXPD_Create();
    adxb->adxpd = adxpd;

    if (adxpd == NULL) {
        ADXB_Destroy(adxb);
        return NULL;
    }

    adxb->maxnch    = arg0;
    adxb->pcmbuf    = arg1;
    adxb->pcmbsize  = arg2;
    adxb->pcmbdist  = arg3;
    adxb->getwrfunc = adxb_DefGetWr;
    adxb->getwrobj  = adxb;
    adxb->addwrfunc = adxb_DefAddWr;
    adxb->addwrobj  = adxb;
    func_020122fc(adxb);
    return adxb;
}

void ADXB_Destroy(ADXB adxb) {
    if (adxb != NULL) {
        ADXPD adxpd = adxb->adxpd;
        adxb->adxpd = 0;
        ADXPD_Destroy(adxpd);
        memset(adxb, 0, sizeof(ADXB_OBJ));
        adxb->used = 0;
    }
}

int ADXB_DecodeHeaderAdx(ADXB adxb, void* header, int len) {
    short sp10[2];
    short sp20[2];
    short sp36;
    short sp34;
    short sp32;
    short sp30;
    short audio_offset;
    char  version;
    char  flags;
    short sp44;
    short sp46;
    short sp48;

    adxb->hdcdflag = 1;

    if (ADX_DecodeInfo(header, len, &audio_offset, &adxb->code, &adxb->bps, &adxb->blklen, &adxb->nch, &adxb->sfreq,
                       &adxb->total_nsmpl, &adxb->blknsmpl) < 0)
    {
        return 0;
    }

    if (adxb->code > 4) {
        if (adxb->unkB4 == 0) {
            ADXERR_CallErrFunc2("E1060101 ADXB_DecodeHeaderAdx: ", "can't play AHX data by this handle");
            return -1;
        }

        adxb->bps            = 8;
        adxb->blklen         = adxb->nch * 0xC0;
        adxb->blknsmpl       = 0x60;
        adxb->format         = 0xA;
        adxb->cof            = 0;
        adxb->nloop          = 0;
        adxb->lp_type        = 0;
        adxb->lp_ins_nsmpl   = NULL;
        adxb->lp_spos        = 0;
        adxb->lp_sofst       = 0;
        adxb->lp_epos        = 0;
        adxb->lp_eofst       = 0;
        adxb->total_ndecsmpl = 0;

        if (ADX_DecodeInfoExVer(header, len, &version, &flags) < 0) {
            return 0;
        }

        sp30 = 0;

        if (adxb_get_key(adxb, version, flags, adxb->total_nsmpl, &sp32, &sp34, &sp36) < 0) {
            return -1;
        }

        if (ahxsetextfunc != NULL) {
            ahxsetextfunc(adxb->unkB4, &sp30);
        }
    } else {
        if (ADX_DecodeInfoExVer(header, len, &version, &flags) < 0) {
            return 0;
        }

        if (adxb_get_key(adxb, version, flags, adxb->total_nsmpl, &sp44, &sp46, &sp48) < 0) {
            return -1;
        }

        ADXPD_SetExtPrm(adxb->adxpd, sp44, sp46, sp48);

        if (ADX_DecodeInfoExADPCM2(header, len, &adxb->cof) < 0) {
            return 0;
        }

        if (ADX_DecodeInfoExIdly(header, len, &sp10, &sp20) < 0) {
            return 0;
        }

        ADXPD_SetCoef(adxb->adxpd, adxb->sfreq, adxb->cof);
        ADXPD_SetDly(adxb->adxpd, &sp10, &sp20);
        ADX_DecodeInfoExLoop(header, len, &adxb->lp_ins_nsmpl, &adxb->nloop, &adxb->lp_type, &adxb->lp_spos, &adxb->lp_sofst,
                             &adxb->lp_epos, &adxb->lp_eofst);
        ADX_DecodeInfoAinf(header, len, &adxb->ainf_len, &adxb->unkC4, &adxb->def_out_vol, &adxb->def_pan);
        adxb->format = 0;
    }

    adxb->dp.nch      = adxb->nch;
    adxb->dp.blksize  = adxb->blklen;
    adxb->dp.blknsmpl = adxb->blknsmpl;
    adxb->dp.pcmbuf   = adxb->pcmbuf;
    adxb->dp.pcmbsize = adxb->pcmbsize;
    adxb->dp.pcmbdist = adxb->pcmbdist;
    adxb->curwpos     = 0;

    return audio_offset;
}

void func_02012748(ADXB adxb) {
    adxb->hdcdflag       = 1;
    adxb->sfreq          = 48000;
    adxb->nch            = 2;
    adxb->bps            = 16;
    adxb->total_nsmpl    = 0x7fffffff;
    adxb->blklen         = 127;
    adxb->blknsmpl       = 1024;
    adxb->format         = adxb->unk9A;
    adxb->dp.nch         = adxb->nch;
    adxb->dp.blksize     = adxb->blklen;
    adxb->dp.blknsmpl    = adxb->blknsmpl;
    adxb->dp.pcmbuf      = adxb->pcmbuf;
    adxb->dp.pcmbsize    = adxb->pcmbsize;
    adxb->dp.pcmbdist    = adxb->pcmbdist;
    adxb->curwpos        = 0;
    adxb->cof            = 0;
    adxb->nloop          = 0;
    adxb->lp_type        = 0;
    adxb->lp_ins_nsmpl   = NULL;
    adxb->lp_spos        = 0;
    adxb->lp_sofst       = 0;
    adxb->lp_epos        = 0;
    adxb->lp_eofst       = 0;
    adxb->total_ndecsmpl = 0;
}

int ADXB_DecodeHeader(ADXB adxb, unsigned short* header, int len) {
    func_020122fc(adxb);

    unsigned short temp = *header;
    if ((unsigned short)(short)((unsigned char)((int)temp >> 8) | ((temp << 8) & 0xFF00)) != 0x8000) {
        return -1;
    }
    return ADXB_DecodeHeaderAdx(adxb, header, len);
}

void ADXB_EntryGetWrFunc(ADXB adxb, void* (*get_wr)(void*, int*, int*, int*), void* object) {
    adxb->getwrfunc = get_wr;
    adxb->getwrobj  = object;
}

void* ADXB_GetPcmBuf(ADXB adxb) {
    return adxb->pcmbuf;
}

int ADXB_GetFormat(ADXB adxb) {
    return adxb->format;
}

int ADXB_GetSfreq(ADXB adxb) {
    return adxb->sfreq;
}

int ADXB_GetNumChan(ADXB adxb) {
    if (adxb == NULL) {
        ADXERR_CallErrFunc1("E2005042701 : NULL pointer is passed.");
        return -1;
    }

    return adxb->nch;
}

int ADXB_GetFmtBps(ADXB adxb) {
    return adxb->bps;
}

int ADXB_GetOutBps(ADXB adxb) {
    return 16;
}

int ADXB_GetBlkSmpl(ADXB adxb) {
    return adxb->blknsmpl;
}

int ADXB_GetTotalNumSmpl(ADXB adxb) {
    return adxb->total_nsmpl;
}

int ADXB_GetNumLoop(ADXB adxb) {
    return adxb->nloop;
}

int ADXB_GetLpStartPos(ADXB adxb) {
    return adxb->lp_spos;
}

int ADXB_GetLpStartOfst(ADXB adxb) {
    if (adxb == NULL) {
        return 0;
    }
    return adxb->lp_sofst;
}

int ADXB_GetLpEndPos(ADXB adxb) {
    return adxb->lp_epos;
}

int ADXB_GetLpEndOfst(ADXB adxb) {
    return adxb->lp_eofst;
}

int ADXB_GetAinfLen(ADXB adxb) {
    return adxb->ainf_len;
}

short ADXB_GetDefOutVol(ADXB adxb) {
    return adxb->def_out_vol;
}

short ADXB_GetDefPan(ADXB adxb, int arg1) {
    return adxb->def_pan[arg1];
}

void ADXB_TakeSnapshot(ADXB adxb) {
    ADXPD_GetDly(adxb->adxpd, &adxb->unkAC, &adxb->unkB0);
    ADXPD_GetExtPrm(adxb->adxpd, &adxb->unkA6, &adxb->unkA8, &adxb->unkAA);
}

void ADXB_RestoreSnapshot(ADXB adxb) {
    ADXPD_SetDly(adxb->adxpd, &adxb->unkAC, &adxb->unkB0);
    ADXPD_SetExtPrm(adxb->adxpd, adxb->unkA6, adxb->unkA8, adxb->unkAA);
}

int adxb_get_key(ADXB adxb, unsigned char arg1, unsigned char arg2, int arg3, short* arg4, short* arg5, short* arg6) {
    char sp[16];

    if (arg1 < 4) {
        *arg4 = 0;
        *arg5 = 0;
        *arg6 = 0;
    } else {
        if (arg2 >= 0x10) {
            CRICRW_Sprintf(sp, sizeof(sp), "%08X", arg3);
            SKG_GenerateKey(sp, 8, arg4, arg5, arg6);
        } else if (arg2 >= 8) {
            if ((adxb->unkA0 == 0) && (adxb->unkA2 == 0) && (adxb->unkA4 == 0)) {
                adxb->unkA0 = adxb_def_k0;
                adxb->unkA2 = adxb_def_km;
                adxb->unkA4 = adxb_def_ka;
            }

            *arg4 = adxb->unkA0;
            *arg5 = adxb->unkA2;
            *arg6 = adxb->unkA4;
        } else {
            *arg4 = 0;
            *arg5 = 0;
            *arg6 = 0;
        }
    }

    return 0;
}

int ADXB_GetStat(ADXB adxb) {
    return adxb->stat;
}

void ADXB_EntryData(ADXB adxb, int arg1, int arg2) {
    if (adxb->format == 0) {
        adxb->dp.ibuf  = arg1;
        adxb->dp.niblk = arg2 / adxb->blklen;
        adxb->ndecsmpl = 0;
    } else {
        adxb->dp.ibuf  = arg1;
        adxb->dp.niblk = arg2 / ((adxb->bps / 8) * adxb->nch);
        adxb->ndecsmpl = 0;
    }
    adxb->total_decsmpl  = 0;
    adxb->total_decdtlen = 0;
    adxb->unkE0          = 0;
    adxb->unkDC          = 0;
}

void ADXB_Start(ADXB adxb) {
    if (adxb->stat == 0) {
        adxb->stat = 1;
    }
}

void ADXB_Stop(ADXB adxb) {
    ADXPD_Stop(adxb->adxpd);
    adxb->stat = 0;
}

void ADXB_Reset(ADXB adxb) {
    if (adxb->stat == 3) {
        ADXPD_Reset(adxb->adxpd);
        adxb->curwpos = 0;
        adxb->stat    = 0;
    }
}

int ADXB_GetDecDtLen(ADXB adxb) {
    return adxb->total_decdtlen;
}

int ADXB_GetDecNumSmpl(ADXB adxb) {
    return adxb->total_decsmpl;
}

// Nonmatching: Wrong instruction order
void ADXB_EvokeExpandMono(ADXB arg0, int arg1) {
    ADXPD         temp_r4 = arg0->adxpd;
    ADXB_DECPARA* unk     = &arg0->dp;

    ADXPD_EntryMono(temp_r4, unk->ibuf, arg1, unk->pcmbuf + (unk->wpos * 2), 0);
    ADXPD_Start(temp_r4);
}

void ADXB_EvokeExpandSte(ADXB arg0, int arg1) {
    ADXPD         temp_r4 = arg0->adxpd;
    ADXB_DECPARA* unk     = &arg0->dp;
    int           a3;
    int           t0;

    // These are two distinct arguments to ADXPD_EntrySte: a3 goes to unk20 and
    // t0 to unk24 (the extra/inter-channel buffer that ADXPD_ExecHndl hands to
    // ADX_DecodeSte4). Passing only their sum left unk24 unwritten.
    a3 = unk->pcmbuf + (unk->wpos * 2);
    t0 = a3 + (unk->pcmbdist * 2);

    ADXPD_EntrySte(temp_r4, unk->ibuf, arg1 * 2, a3, t0);
    ADXPD_Start(temp_r4);
}

void ADXB_EvokeDecode(ADXB adxb) {
    ADXB_DECPARA* unk = &adxb->dp;

    int var_a3;
    int temp_t0;
    int temp_a1_2;
    int temp_t7;
    int var_t4;
    int var_t3;
    int var_t1;
    int temp_lo;
    int temp_lo_3;
    int temp_lo_2;

    temp_lo = unk->niblk / unk->nch;

    var_t1  = unk->blknsmpl;
    temp_t7 = unk->pcmbsize;
    temp_t0 = unk->wpos;
    var_a3  = unk->lp_nsmpl;

    var_t4 = unk->nroom;

    temp_lo_2 = (var_a3 + var_t1 - 1) / var_t1;
    temp_a1_2 = var_t1 - (var_a3 + var_t1 - 1) % var_t1 - 1;
    var_t3    = ((var_t1 + (unk->pcmbsize - temp_t0)) - 1) / var_t1;

    temp_lo_3 = var_t3 * var_t1;

    if (temp_lo_2 < var_t3) {
        if ((temp_t0 + (temp_lo_3)-temp_a1_2) < unk->pcmbsize) {
            var_t3 += 1;
        }
    }

    if (var_a3 < var_t4) {
        var_t4 += temp_a1_2;
    }

    temp_lo = MIN(temp_lo, var_t4 / var_t1);
    temp_lo = MIN(temp_lo, temp_lo_2);
    temp_lo = MIN(temp_lo, var_t3);

    if (unk->nch == 2) {
        ADXB_EvokeExpandSte(adxb, temp_lo);
    } else {
        ADXB_EvokeExpandMono(adxb, temp_lo);
    }
}

void memcpy2(void* dest, const void* src, int count) {
    short*       _dest = dest;
    const short* _src  = src;

    while (count > 0) {
        *_dest++ = *_src++;
        count -= 1;
    }
}

void ADXB_CopyExtraBufSte(void* arg0, int arg1, int arg2, int arg3) {
    memcpy2(arg0, arg0 + (arg1 * 2), arg3);
    memcpy2(arg0 + (arg2 * 2), arg0 + ((arg2 + arg1) * 2), arg3);
}

void ADXB_CopyExtraBufMono(void* arg0, int arg1, int arg2, int arg3) {
    memcpy2(arg0, arg0 + (arg1 * 2), arg3);
}

void ADXB_EndDecode(ADXB adxb) {
    int           s1, s2, sp0, s3, s0, _s0, _s1, v0, v1, temp_div;
    int           s7;
    ADXB_DECPARA* s5 = &adxb->dp;
    void*         s8 = s5->pcmbuf;
    int           s4;
    int           tmp1, tmp3;

    s3  = s5->blksize;
    s1  = s5->blknsmpl;
    s4  = s5->wpos;
    sp0 = adxb->pcmbdist;
    s7  = adxb->pcmbsize;

    temp_div = s5->lp_nsmpl + s1 - 1;
    s0       = temp_div % s1;
    _s0      = s1 - s0 - 1;
    s2       = temp_div / s1;

    v0 = ADXPD_GetNumBlk(adxb->adxpd);

    tmp1                = v0 * s1;
    s1                  = tmp1;
    s2                  = s2 * s5->nch;
    tmp3                = v0 * s3;
    s3                  = tmp3;
    s1                  = s1 / s5->nch;
    adxb->total_decsmpl = (v0 < s2) ? s1 : s1 - _s0;
    s4 += adxb->total_decsmpl;
    adxb->total_decdtlen = s3;

    if (s4 >= s7) {
        s4 -= s7;

        if ((s5->nch == 2)) {
            ADXB_CopyExtraBufSte(s8, s7, sp0, s4);
        } else {
            ADXB_CopyExtraBufMono(s8, s7, sp0, s4);
        }
    }
}

void ADXB_ExecOneAdx(ADXB adxb) {
    if (adxb->stat == 1) {
        if (ADXPD_GetStat(adxb->adxpd) == 0) {
            adxb->getwrfunc(adxb->getwrobj, &adxb->dp.wpos, &adxb->dp.nroom, &adxb->dp.lp_nsmpl);
            ADXB_EvokeDecode(adxb);
            adxb->stat = 2;
        }
    }

    if (adxb->stat == 2) {
        ADXPD_ExecHndl(adxb->adxpd);

        if (ADXPD_GetStat(adxb->adxpd) == 3) {
            ADXB_EndDecode(adxb);
            ADXPD_Reset(adxb->adxpd);
            adxb->addwrfunc(adxb->addwrobj, adxb->total_decdtlen, adxb->total_decsmpl);
            adxb->stat = 3;
        }
    }
}

void ADXB_ExecHndl(ADXB adxb) {
    if (adxb->format == 0) {
        ADXB_ExecOneAdx(adxb);
    } else if (adxb->format == 10) {
        ADXB_ExecOneAhx(adxb);
    }

    if (adxb->unkE4 != 0) {
        func_02012ed8(adxb);
    }
}

// NITRO-only: report how many input bytes were consumed since the last call
// (the counter wraps at 0x7FFFFFFF) and how many PCM bytes the last decode made.
void func_02012ed8(ADXB adxb) {
    int dtlen = adxb->unkDC;
    int total = adxb->total_decdtlen;
    int nsmpl = adxb->total_decsmpl;
    int nbyte = total - dtlen;

    if (nbyte < 0) {
        nbyte = (0x7FFFFFFF - dtlen) + total;
    }

    adxb->unkE4(adxb->unkE8, nbyte, adxb->nch * (nsmpl * 2));
    adxb->unkDC = adxb->total_decdtlen;
}
