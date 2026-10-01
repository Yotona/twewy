#include <cri/adx_sjd.h>

typedef struct {
    /* 0x00 */ void (*unk00)(void*);
    /* 0x04 */ void* unk04;
    /* 0x08 */ void (*unk08)(void*);
    /* 0x0C */ volatile int init_cnt;
    /* 0x10 */ void*        unk10;
} ADXSJD_CTRL; // Size: 0x14

ADXSJD_CTRL data_0206c0b4              = {0};
ADXSJD      adxsjd_obj[ADXSJD_MAX_OBJ] = {0};

void  ADXB_SetAhxDecSmpl(ADXB adxb, int decsmpl);
void  ADXB_AhxTermSupply(ADXB adxb);
int   ADXB_GetStat(ADXB adxb);
void  ADXB_ExecHndl(ADXB adxb);
int   ADXB_GetNumChan(ADXB adxb);
void* ADXB_GetPcmBuf(ADXB adxb);
void* SJRBF_GetBufPtr(SJ sj);
int   func_02012258(void);
int   ADXB_DecodeHeader(ADXB adxb, unsigned short* header, int len);
void  func_02012748(ADXB adxb);
int   ADXB_GetSfreq(ADXB adxb);
int   ADXB_GetTotalNumSmpl(ADXB adxb);
int   ADXB_GetDecDtLen(ADXB adxb);
int   ADXB_GetDecNumSmpl(ADXB adxb);
void  ADXB_Reset(ADXB adxb);

void func_0201575c(ADXSJD* sjd);
void func_0201562c(ADXSJD* sjd);

int func_02015aa4(ADXSJD* sjd);

void* adxsjd_get_wr(void* obj, int* wpos, int* nroom, int* lp_nsmpl);

void ADXB_Init();

void ADXSJD_Init(void) {
    if (data_0206c0b4.init_cnt == 0) {
        ADXB_Init();
        __builtin__clear(adxsjd_obj, sizeof(adxsjd_obj));
    }

    data_0206c0b4.init_cnt++;
}

void ADXSJD_Finish(void) {
    if (--data_0206c0b4.init_cnt == 0) {
        __builtin__clear(adxsjd_obj, sizeof(adxsjd_obj));
    }
}

void ADXSJD_Clear(ADXSJD* sjd) {
    sjd->hdrlen         = 0;
    sjd->total_decsmpl  = 0;
    sjd->total_decdtlen = 0;
    sjd->decpos         = 0;
    sjd->maxdecsmpl     = 0x7FFFFFFF;
    sjd->dtrpsmpl       = -1;
    sjd->dtrpcnt        = 0;
    sjd->dtrpdtlen      = 0;
    sjd->empty_end      = 0;
    sjd->unk_A8         = 0;
    sjd->unk_AC         = 0;
}

ADXSJD* ADXSJD_Create(SJ sj, int maxChans, SJ* sjo) {
    ADXSJD* sjd;
    SJ      out;
    void*   buf_ptr;
    int     i;
    int     j;
    int     buf_size;
    int     xtr_size;

    out = sjo[0];

    for (i = 0; i < ADXSJD_MAX_OBJ; i++) {
        if (adxsjd_obj[i].used == FALSE) {
            break;
        }
    }
    if (i == ADXSJD_MAX_OBJ) {
        return NULL;
    }

    sjd      = &adxsjd_obj[i];
    buf_ptr  = SJRBF_GetBufPtr(out);
    buf_size = SJRBF_GetBufSize(out) / 2;
    xtr_size = SJRBF_GetXtrSize(out) / 2;

    sjd->adxb = ADXB_Create(maxChans, buf_ptr, buf_size, buf_size + xtr_size);
    if (sjd->adxb == NULL) {
        return NULL;
    }

    ADXB_EntryGetWrFunc(sjd->adxb, adxsjd_get_wr, sjd);

    sjd->sji    = sj;
    sjd->maxnch = maxChans;

    for (j = 0; j < maxChans; j++) {
        sjd->sjo[j] = sjo[j];
    }

    sjd->state = 0;
    ADXSJD_Clear(sjd);
    sjd->dtrpfunc = 0;
    sjd->dtrpobj  = 0;
    sjd->dfltfunc = 0;
    sjd->dfltobj  = 0;
    sjd->used     = TRUE;
    return sjd;
}

void ADXSJD_Destroy(ADXSJD* sjd) {
    if (sjd == NULL) {
        return;
    }

    ADXB adxb = sjd->adxb;
    if (adxb != NULL) {
        sjd->adxb = NULL;
        ADXB_Destroy(adxb);
    }
    ADXCRS_Lock();
    memset(sjd, 0, sizeof(ADXSJD));
    ADXCRS_Unlock();
}

char ADXSJD_GetStat(ADXSJD* sjd) {
    return sjd->state;
}

void ADXSJD_SetInSj(ADXSJD* sjd, SJ sj) {
    sjd->sji = sj;
    ADXB_SetAhxInSj(sjd->adxb, sj);
}

void ADXSJD_SetMaxDecSmpl(ADXSJD* sjd, int nsmpl) {
    sjd->maxdecsmpl = nsmpl;
    ADXB_SetAhxDecSmpl(sjd->adxb, nsmpl);
}

void ADXSJD_TermSupply(ADXSJD* sjd) {
    ADXB_AhxTermSupply(sjd->adxb);
}

void ADXSJD_Start(ADXSJD* sjd) {
    ADXSJD_Clear(sjd);
    sjd->state = 1;
}

void ADXSJD_Stop(ADXSJD* sjd) {
    ADXB_Stop(sjd->adxb);
    sjd->state = 0;
}

void adxsjd_decode_prep(ADXSJD* sjd) {
    ADXB  adxb = sjd->adxb;
    SJ    sji  = sjd->sji;
    SJCK  ck;
    SJCK  ck2;
    char* p;
    int   i;
    int   hdrlen;
    int   fmt;

    SJ_GetChunk(sji, 1, 0xC800, &ck);

    i = 0;

    if (ck.length > 0) {
        p = ck.data;

        for (;;) {
            if (*p != 0) {
                break;
            }

            i++;
            p++;

            if (i >= ck.length) {
                break;
            }
        }
    }

    if (i % 2 == 1) {
        SJ_UngetChunk(sji, 1, &ck);

        if (func_02012258() == 0) {
            ADXERR_CallErrFunc2("E04102501 adxsjd_decode_prep: ", "The data alignment is illegal.");
        }

        sjd->state = 4;
        return;
    }

    SJ_SplitChunk(&ck, i, &ck2, &ck);
    SJ_PutChunk(sji, 0, &ck2);

    if (ck.length < 0x10) {
        SJ_UngetChunk(sji, 1, &ck);
        return;
    }

    hdrlen = ADXB_DecodeHeader(adxb, (unsigned short*)ck.data, ck.length);

    if (hdrlen == 0 || hdrlen > ck.length) {
        SJ_UngetChunk(sji, 1, &ck);
        return;
    }

    if (hdrlen < 0) {
        if (adxb->unk9A != 0) {
            func_02012748(adxb);
            hdrlen = 0;
        } else {
            SJ_UngetChunk(sji, 1, &ck);

            if (func_02012258() == 0) {
                ADXERR_CallErrFunc2("E03010901 ADXB_DecodeHeader: ", "Can not decode this file format.");
            }

            sjd->state = 4;
            return;
        }
    }

    sjd->hdrlen = hdrlen;

    if (sjd->dfltfunc != NULL) {
        sjd->dfltfunc(sjd->dfltobj, ADXB_GetFormat(adxb), ADXB_GetNumChan(adxb), ADXB_GetSfreq(adxb),
                      ADXB_GetTotalNumSmpl(adxb));
    }

    if (ADXB_GetFormat(adxb) == 4) {
        sjd->empty_end = 1;
    }

    if (ADXB_GetFormat(adxb) == 2) {
        memcpy(sjd->spsdinfo, ck.data, (ck.length < 0x40) ? ck.length : 0x40);
    }

    fmt = ADXB_GetFormat(adxb);

    if (fmt == 10 || fmt == 11 || fmt == 12 || fmt == 20 || fmt == 15) {
        SJ_UngetChunk(sji, 1, &ck);
    } else {
        SJ_SplitChunk(&ck, hdrlen, &ck, &ck2);
        SJ_PutChunk(sji, 0, &ck);
        SJ_UngetChunk(sji, 1, &ck2);
    }

    sjd->state = 2;
}

void* adxsjd_get_wr(void* obj, int* wpos, int* nroom, int* lp_nsmpl) {
    ADXSJD* sjd = obj;
    SJ      sjrbf;
    int     i;
    int     tmp;
    int     tmp2;

    sjrbf = sjd->sjo[0];

    for (i = 0; i < ADXB_GetNumChan(sjd->adxb); i++) {
        SJ_GetChunk(sjd->sjo[i], 0, 0x4000, &sjd->cko[i]);
    }

    *wpos = (unsigned int)(sjd->cko[0].data - SJRBF_GetBufPtr(sjrbf)) / 2;

    tmp  = (unsigned int)sjd->cko[0].length / 2;
    tmp2 = sjd->maxdecsmpl;
    if (tmp < tmp2) {
        tmp2 = tmp;
    }
    *nroom = tmp2;

    if (sjd->dtrpsmpl >= 0) {
        *lp_nsmpl = sjd->dtrpsmpl - sjd->dtrpcnt;
    } else {
        *lp_nsmpl = 0x1FFFFFFF;
    }

    return ADXB_GetPcmBuf(sjd->adxb);
}

#define BSWAP_U16_EX(x) ((unsigned short)(short)(((((short)(x)) >> 8) & 0xFF) | ((((short)(x)) << 8) & 0xFF00)))

void adxsjd_decexec_start(ADXSJD* sjd) {
    ADXB  adxb = sjd->adxb;
    SJ    sji  = sjd->sji;
    SJCK  ck1;
    SJCK  ck2;
    SJCK* cki = &sjd->cki;
    short ofst;
    int   i;
    int   len;
    int   total;
    int   done;

    done = 0;

    // The loop callback only fires once the trap has been fully consumed.
    if (sjd->dtrpsmpl >= 0 && sjd->dtrpcnt >= sjd->dtrpsmpl) {
        if (sjd->dtrpfunc != NULL) {
            sjd->dtrpfunc(sjd->dtrpobj);
        }
    }

    if (sjd->empty_end == 1 && SJ_GetNumData(sji, 1) == 0) {
        sjd->state = 3;
        return;
    }

    SJ_GetChunk(sji, 1, 0x7FFFFFFF, cki);

    // A leading big-endian 0x8001 tag marks this chunk as an ADX sub-header
    // rather than PCM.
    if (ADXB_GetFormat(adxb) == 0 && cki->length >= 4 && BSWAP_U16_EX(*(short*)cki->data) == 0x8001) {
        sjd->state = 3;

        if (ADX_DecodeFooter(cki->data, cki->length, &ofst) == 0) {
            if (ofst > cki->length) {
                SJ_UngetChunk(sji, 1, cki);
                return;
            }

            SJ_SplitChunk(cki, ofst, cki, &ck1);
            SJ_PutChunk(sji, 0, cki);
            SJ_UngetChunk(sji, 1, &ck1);
        }

        if (sjd->lnkflg == 0) {
            return;
        }

        // Peel leading zero bytes off the stream one chunk at a time. The byte
        // test runs before the bound test, so an all-zero chunk makes no
        // progress and the loop only ends once a chunk is fully consumed.
        for (;;) {
            char* p;

            SJ_GetChunk(sji, 1, 0x7FFFFFFF, cki);

            len = cki->length;

            if (len == 0) {
                return;
            }

            i = 0;

            if (len > 0) {
                p = cki->data;

                // Same two-break shape as adxsjd_decode_prep's scan: a combined while
                // condition gets bottom-tested here, and the bound test has to
                // use the already-incremented i.
                for (;;) {
                    if (*p != 0) {
                        break;
                    }

                    i++;
                    p++;

                    if (i >= len) {
                        break;
                    }
                }
            }

            SJ_SplitChunk(cki, i, cki, &ck1);
            SJ_PutChunk(sji, 0, cki);
            SJ_UngetChunk(sji, 1, &ck1);

            if (i < len) {
                return;
            }
        }
    }

    total = ADXSJD_GetTotalNumSmpl(sjd);

    if (sjd->decpos >= total) {
        if (ADXB_GetFormat(adxb) == 1) {
            if (func_02015aa4(sjd) != 1) {
                done = 1;
            }
        } else {
            done = 1;
        }
    } else if (ADXB_GetFormat(adxb) == 10 && sjd->decpos + 0x240 >= total) {
        done = 1;
    }

    if (done != 0) {
        sjd->state = 3;
        SJ_UngetChunk(sji, 1, cki);
        return;
    }

    if (ADXSJD_GetBlkSmpl(sjd) > SJ_GetNumData(sjd->sjo[0], 0) / 2) {
        SJ_UngetChunk(sji, 1, cki);
        return;
    }

    if (func_02015aa4(sjd) != 1 && ADXB_GetFormat(adxb) == 1) {
        if (ADXB_GetFmtBps(adxb) == 0x10) {
            int nch  = ADXB_GetNumChan(sjd->adxb);
            int have = sjd->decpos + cki->length / nch / 2;

            // Trim the chunk down to the audio that is still outstanding.
            if (have > total) {
                SJ_SplitChunk(cki, nch * (total - have) * 2, cki, &ck2);
                SJ_UngetChunk(sji, 1, &ck2);
            }
        } else {
            ADXERR_CallErrFunc2("E07021901 adxsjd_decexec_start: ", "8 or 4bit WAV file can't playback continuously.");
        }
    }

    if (ADXB_GetFormat(adxb) == 10) {
        SJ_UngetChunk(sji, 1, cki);
    }

    ADXB_EntryData(adxb, cki->data, cki->length);
    ADXB_Start(adxb);
}

void adxsjd_decexec_end(ADXSJD* sjd) {
    ADXB adxb = sjd->adxb;
    SJ   sji  = sjd->sji;
    SJCK ck;
    SJCK ck2;
    int  total_nsmpl;
    int  dlen;
    int  ndecsmpl;
    int  i;

    total_nsmpl = ADXB_GetTotalNumSmpl(adxb);
    dlen        = ADXB_GetDecDtLen(adxb);
    ndecsmpl    = ADXB_GetDecNumSmpl(adxb);

    if (ADXB_GetFormat(adxb) != 1 || func_02015aa4(sjd) != 1) {
        if (ndecsmpl >= total_nsmpl - sjd->decpos) {
            ndecsmpl = total_nsmpl - sjd->decpos;
        }
    }

    SJ_SplitChunk(&sjd->cki, dlen, &ck, &ck2);
    SJ_PutChunk(sji, 0, &ck);
    SJ_UngetChunk(sji, 1, &ck2);

    for (i = 0; i < ADXB_GetNumChan(sjd->adxb); i++) {
        SJ_SplitChunk(&sjd->cko[i], ndecsmpl * 2, &ck, &ck2);

        if (sjd->unk_58 != NULL) {
            sjd->unk_58(sjd->unk_5C, i, ck.data, ck.length);
        }

        SJ_PutChunk(sjd->sjo[i], 1, &ck);
        SJ_UngetChunk(sjd->sjo[i], 0, &ck2);
    }

    sjd->total_decsmpl += ndecsmpl;
    sjd->total_decdtlen += dlen;
    sjd->decpos += ndecsmpl;
    sjd->dtrpcnt += ndecsmpl;
    sjd->dtrpdtlen += dlen;

    ADXB_Reset(adxb);
}

void adxsjd_decexec_extra(ADXSJD* sjd) {
    ADXB adxb        = sjd->adxb;
    int  total_nsmpl = ADXB_GetTotalNumSmpl(adxb);
    int  dlen        = ADXB_GetDecDtLen(adxb);
    int  ndecsmpl    = ADXB_GetDecNumSmpl(adxb);

    total_nsmpl -= sjd->decpos;

    if (ndecsmpl >= total_nsmpl) {
        ndecsmpl = total_nsmpl;
    }
    sjd->total_decsmpl += ndecsmpl;
    sjd->total_decdtlen += dlen;
    sjd->decpos += ndecsmpl;
}

void adxsjd_decode_exec(ADXSJD* sjd) {
    ADXB adxb = sjd->adxb;

    if (ADXB_GetStat(adxb) == 0) {
        adxsjd_decexec_start(sjd);
    }

    ADXB_ExecHndl(adxb);

    if (ADXB_GetStat(adxb) == 3) {
        adxsjd_decexec_end(sjd);
    }

    if (adxb->format == 10 || adxb->format == 20 || adxb->format == 11 || adxb->format == 12 || adxb->format == 15) {
        adxsjd_decexec_extra(sjd);
    }
}

void ADXSJD_ExecHndl(ADXSJD* sjd) {
    if (0 < sjd->unk_A8) {
        ADXCRS_Lock();
        func_0201562c(sjd);
        ADXCRS_Unlock();
    }
    if (sjd->state == 2) {
        adxsjd_decode_exec(sjd);
    } else if (sjd->state == 1) {
        adxsjd_decode_prep(sjd);
    }
    if (sjd->unk_AC > 0) {
        ADXCRS_Lock();
        func_0201575c(sjd);
        ADXCRS_Unlock();
    }
}

void func_0201562c(ADXSJD* sjd) {
    SJCK ck;
    int  nbyte;
    int  nsmpl;
    int  i;

    nbyte = sjd->unk_A8 * 2;

    // Clamp against the shortest output channel.
    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 0, 0x7FFFFFFF, &ck);

        if (nbyte >= ck.length) {
            nbyte = ck.length;
        }

        SJ_UngetChunk(sjd->sjo[i], 0, &ck);
    }

    // Whole samples only, so drop any odd trailing byte.
    nsmpl = nbyte / 2;
    nbyte = nsmpl * 2;

    if (nbyte <= 0) {
        return;
    }

    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 0, nbyte, &ck);
        memset(ck.data, 0, nbyte);
        SJ_PutChunk(sjd->sjo[i], 1, &ck);
    }

    sjd->unk_A8 -= nsmpl;
}

void func_0201575c(ADXSJD* sjd) {
    SJCK ck;
    int  nbyte;
    int  nsmpl;
    int  i;

    nbyte = sjd->unk_AC * 2;

    // Clamp against the shortest output channel.
    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 1, 0x7FFFFFFF, &ck);

        if (nbyte >= ck.length) {
            nbyte = ck.length;
        }

        SJ_UngetChunk(sjd->sjo[i], 1, &ck);
    }

    // Whole samples only, so drop any odd trailing byte.
    nsmpl = nbyte / 2;
    nbyte = nsmpl * 2;

    if (nbyte <= 0) {
        return;
    }

    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 1, nbyte, &ck);
        SJ_PutChunk(sjd->sjo[i], 0, &ck);
    }

    sjd->unk_AC -= nsmpl;
}

void ADXSJD_ExecServer(void) {
    int i;

    if (data_0206c0b4.unk08 != 0) {
        data_0206c0b4.unk08(data_0206c0b4.unk04);
    }

    for (i = 0; i < ADXSJD_MAX_OBJ; i++) {
        if (adxsjd_obj[i].used == 1) {
            ADXSJD_ExecHndl(&adxsjd_obj[i]);
        }
    }

    if (data_0206c0b4.unk00 != 0) {
        data_0206c0b4.unk00(data_0206c0b4.unk10);
    }
}

int ADXSJD_GetDecDtLen(ADXSJD* sjd) {
    return sjd->total_decdtlen;
}

int ADXSJD_GetDecNumSmpl(ADXSJD* sjd) {
    return sjd->total_decsmpl;
}

void ADXSJD_SetDecPos(ADXSJD* sjd, int param_2) {
    sjd->decpos = param_2;
}

void ADXSJD_SetLnkSw(ADXSJD* sjd, int param_2) {
    sjd->lnkflg = param_2;
}

void ADXSJD_EntryTrapFunc(ADXSJD* sjd, int param_2, int param_3) {
    sjd->dtrpfunc = param_2;
    sjd->dtrpobj  = param_3;
}

void ADXSJD_SetTrapNumSmpl(ADXSJD* sjd, int param_2) {
    sjd->dtrpsmpl = param_2;
}

void ADXSJD_SetTrapCnt(ADXSJD* sjd, int param_2) {
    sjd->dtrpcnt = param_2;
}

void ADXSJD_SetTrapDtLen(ADXSJD* sjd, int param_2) {
    sjd->dtrpdtlen = param_2;
}

int ADXSJD_GetFormat(ADXSJD* sjd) {
    return ADXB_GetFormat(sjd->adxb);
}

int ADXSJD_GetSfreq(ADXSJD* sjd) {
    return ADXB_GetSfreq(sjd->adxb);
}

int ADXSJD_GetNumChan(ADXSJD* sjd) {
    return ADXB_GetNumChan(sjd->adxb);
}

int ADXSJD_GetOutBps(ADXSJD* sjd) {
    return ADXB_GetOutBps(sjd->adxb);
}

int ADXSJD_GetBlkSmpl(ADXSJD* sjd) {
    return ADXB_GetBlkSmpl(sjd->adxb);
}

int ADXSJD_GetTotalNumSmpl(ADXSJD* sjd) {
    return ADXB_GetTotalNumSmpl(sjd->adxb);
}

int ADXSJD_GetNumLoop(ADXSJD* sjd) {
    return ADXB_GetNumLoop(sjd->adxb);
}

int ADXSJD_GetLpStartPos(ADXSJD* sjd) {
    return ADXB_GetLpStartPos(sjd->adxb);
}

int ADXSJD_GetLpStartOfst(ADXSJD* sjd) {
    if (sjd != NULL) {
        return ADXB_GetLpStartOfst(sjd->adxb);
    }
    return 0;
}

int ADXSJD_GetLpEndPos(ADXSJD* sjd) {
    return ADXB_GetLpEndPos(sjd->adxb);
}

int ADXSJD_GetLpEndOfst(ADXSJD* sjd) {
    return ADXB_GetLpEndOfst(sjd->adxb);
}

int ADXSJD_GetDefOutVol(ADXSJD* sjd) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((unsigned int)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefOutVol(sjd->adxb);
    }
    return 0;
}

int ADXSJD_GetDefPan(ADXSJD* sjd, int chan) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((unsigned int)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefPan(sjd->adxb, chan);
    }
    return -0x80;
}

int* ADXSJD_GetSpsdInfo(ADXSJD* sjd) {
    return sjd->spsdinfo;
}

int ADXSJD_TakeSnapshot(ADXSJD* sjd) {
    return ADXB_TakeSnapshot(sjd->adxb);
}

int ADXSJD_RestoreSnapshot(ADXSJD* sjd) {
    return ADXB_RestoreSnapshot(sjd->adxb);
}

int func_02015aa4(ADXSJD* sjd) {
    return sjd->unk_B0;
}
