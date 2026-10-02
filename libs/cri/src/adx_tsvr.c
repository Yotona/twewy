#include <cri/adxt.h>
#include <cri/private/adx_dcd.h>

void ADXERR_CallErrFunc1(const char*);
void ADXCRS_Enter();
void ADXCRS_Leave();

// SJCK_LEN_MAX is the "read to the very end" sentinel; the target builds it
// with `mvn r1, #0x80000000`.
#define SJCK_LEN_MAX 0x7FFFFFFF

// callees living in sibling TUs (address-named in the original)
// adx_sjd.c -- no public header declares these
int  ADXSJD_GetDecDtLen(ADXSJD* sjd); // GetDecDtLen
void ADXSJD_SetDecPos(ADXSJD* sjd, int pos);
// ADXSJD_EntryTrapFunc. Takes a real callback plus a user pointer, which is
// why unk_48/unk_4C are typed as pointers rather than unsigned int.
void ADXSJD_EntryTrapFunc(ADXSJD* sjd, void (*trap_func)(ADXT), void* trap_obj);
void ADXSJD_SetTrapNumSmpl(ADXSJD* sjd, int samples);
void ADXSJD_SetTrapCnt(ADXSJD* sjd, int cnt);
void ADXSJD_SetTrapDtLen(ADXSJD* sjd, int dt_len);
int  ADXSJD_GetNumChan(ADXSJD* sjd);     // GetNumChan
int  ADXSJD_GetBlkSmpl(ADXSJD* sjd);     // GetBlkSmpl
int  ADXSJD_GetLpStartPos(ADXSJD* sjd);  // GetCof
int  ADXSJD_GetLpStartOfst(ADXSJD* sjd); // GetAinfLen
int  ADXSJD_GetLpEndPos(ADXSJD* sjd);
int* ADXSJD_GetSpsdInfo(ADXSJD* sjd);
int  ADXSJD_TakeSnapshot(ADXSJD* sjd);
void ADXSJD_TermSupply(ADXSJD* sjd);
void ADXSJD_ExecHndl(ADXSJD* sjd);
int  ADXSJD_GetFormat(ADXSJD* sjd); // GetFormat
void ADXSJD_Start(ADXSJD* sjd);
void ADXSJD_Stop(ADXSJD* sjd);
void ADXSJD_SetMaxDecSmpl(ADXSJD* sjd, int n);
char ADXSJD_GetStat(ADXSJD* sjd);
int  ADXSJD_RestoreSnapshot(ADXSJD* sjd);
// adx_stmc.c
int  ADXSTM_GetStat(ADXSTM* stm); // ADXSTM_GetStat
void ADXSTM_Seek(ADXSTM* stm, int pos);
void ADXSTM_SetEos(ADXSTM* stm, int eos);
// lsc.c -- 4-byte stub
int LSC_GetStat(void* lsc);
// adx_rnanitro.c forwarder
int  ADXRNA_GetNumData(void* rna); // ADXRNA_GetNumData
int  ADXRNA_GetNumRoom(void* rna); // ADXRNA_GetNumRoom
void ADXRNA_SetPlaySw(void* rna, int sw);
void ADXRNA_SetTransSw(void* rna, int sw);
// adx_rnanitro.c forwarders
void ADXRNA_SetNumChan(void* rna, int n);
void ADXRNA_SetSfreq(void* rna, int n);
void ADXRNA_SetBitPerSmpl(void* rna, int n);
void ADXRNA_SetTotalNumSmpl(void* rna, int n);
void ADXRNA_SetStmHdInfo(void* rna, int* p);
// adx_bwav.c
int ADX_ScanInfoCodeWav(char* data, int len, short* ofst);
// adx_tlk.c
void adxt_start_stm(ADXT adxt, const char* filename, void* dir, int ofst, int range);
void ADXT_SetTranspose(ADXT adxt, int a, int b);
void ADXT_GetTranspose(ADXT adxt, int* a, int* b);
// adx_errs.c
void ADXERR_CallErrFunc2(char* msg, char* arg);
void ADXERR_ItoA2(int v, int base, char* str, int width);
// lsc.c
int ADXAMP_SetSfreq(void* amp, int sfreq);
int _s32_div_f(int a, int b);

extern int volatile adxt_vsync_cnt; // adx_inis.c
void memset(void*, int, int);

#define ADXSJD_STAT_PLAYING 3
#define ADXSJD_STAT_DECINFO 2

void adxt_trap_entry(ADXT adxt);

struct {
    int f_7fc; // callback: (ADXT, sfreq, num_chan, total_smpl)
    int dbg_rna_ndata;
    int dbg_ndt;
    int dbg_nch;
} data_0206c7fc = {0, 0, 0, 0};

#define adxt_dbg_rna_ndata data_0206c7fc.dbg_rna_ndata
#define adxt_dbg_ndt       data_0206c7fc.dbg_ndt
#define adxt_dbg_nch       data_0206c7fc.dbg_nch

#define ADXT_CB_FUNC ((void (*)(ADXT adxt, int sfreq, int num_chan, int total_smpl))data_0206c7fc.f_7fc)

char data_02063edc[] = "E8101201 adxt_trap_entry: not enough data";
char data_02063f08[] = "E9081001 adxt_stat_decinfo: can't play this number of channels";
char data_02063f48[] = "E02080842 adxt_ExecHndl: parameter error";

void adxt_ExecHndl(ADXT adxt);
void adxt_stat_decinfo(ADXT adxt);
void adxt_stat_prep(ADXT adxt);
void adxt_stat_playing(ADXT adxt);
void adxt_stat_decend(ADXT adxt);
void adxt_stat_playend(ADXT adxt);
void ADXT_ExecRdErrChk(ADXT adxt);
void ADXT_ExecRdCompChk(ADXT adxt);

// Loop-start trap: snapshot the decoder at the loop start, then re-arm the trap
// for one loop body (lp_epos - lp_spos samples) with adxt_trap_entry as the
// callback that rewinds to lp_spos/lp_sofst each time it fires.
void adxt_trap_entry_lps(ADXT adxt) {
    ADXSJD* sjd      = adxt->sjd;
    int     lp_spos  = ADXSJD_GetLpStartPos(sjd);
    int     lp_sofst = ADXSJD_GetLpStartOfst(sjd);
    int     lp_epos  = ADXSJD_GetLpEndPos(sjd);
    int     trp;

    ADXSJD_TakeSnapshot(sjd);
    ADXSJD_SetTrapCnt(sjd, 0);

    trp            = lp_epos - lp_spos;
    adxt->trpnsmpl = trp;

    ADXSJD_SetTrapNumSmpl(sjd, trp);
    ADXSJD_SetTrapDtLen(sjd, lp_sofst);
    ADXSJD_SetDecPos(sjd, lp_spos);
    ADXSJD_EntryTrapFunc(sjd, adxt_trap_entry, adxt);
}

// Loop-end trap (recvx adxt_trap_entry): skip lp_skiplen bytes, rewind the
// decoder to the loop start and, for in-memory playback, re-prime the input
// stream from lp_sofst. NITRO adds the ADXSJD snapshot restore.
void adxt_trap_entry(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    SJ      sji = adxt->sji;
    int     lp_spos;
    int     lp_sofst;
    int     lp_epos;
    int     trp;
    SJCK    ck;

    lp_spos  = ADXSJD_GetLpStartPos(sjd);
    lp_sofst = ADXSJD_GetLpStartOfst(sjd);
    lp_epos  = ADXSJD_GetLpEndPos(sjd);

    if ((unsigned char)(char)(adxt->pmode - ADXT_PLAYBACK_MEM) <= 1 && adxt->lpflg == 0) {
        ADXSJD_SetTrapNumSmpl(adxt->sjd, -1);
        return;
    }

    SJ_GetChunk(sji, 1, adxt->lp_skiplen, &ck);

    if (ck.length < adxt->lp_skiplen) {
        ADXERR_CallErrFunc1(data_02063edc);
    }

    SJ_PutChunk(sji, 0, &ck);

    ADXSJD_SetTrapCnt(sjd, 0);

    trp            = lp_epos - lp_spos;
    adxt->trpnsmpl = trp;

    ADXSJD_SetTrapNumSmpl(sjd, trp);
    ADXSJD_SetTrapDtLen(sjd, lp_sofst);
    ADXSJD_SetDecPos(sjd, lp_spos);

    if (adxt->pmode == ADXT_PLAYBACK_MEM) {
        SJ_Reset(sji);
        SJ_GetChunk(sji, 1, lp_sofst, &ck);
        SJ_PutChunk(sji, 0, &ck);
    }

    ADXSJD_RestoreSnapshot(sjd);
    adxt->lpcnt++;
}

// Stream end-of-file callback: stop at EOF when not looping, otherwise seek the
// stream back to the loop start sector.
void adxt_eos_entry(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    ADXSTM* stm = adxt->stm;
    int     lsofst;

    if (stm == NULL || sjd == NULL) {
        return;
    }

    lsofst = ADXSJD_GetLpStartOfst(sjd);

    if (adxt->pmode == ADXT_PLAYBACK_SLFILE) {
        ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
        return;
    }

    if (adxt->lpflg == 0) {
        if (ADXSJD_GetDecDtLen(adxt->sjd) >= (int)adxt->loopDecodeLength) {
            ADXSJD_SetTrapNumSmpl(adxt->sjd, -1);
        }

        ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
    } else {
        ADXSTM_Seek(stm, lsofst / 2048);
    }
}

// Mono sources have no meaningful channel-1 pan, so only channel 0 is applied.
void adxt_set_outpan(ADXT adxt) {
    int num_chan = ADXSJD_GetNumChan(adxt->sjd);

    if (num_chan == 1) {
        ADXT_SetOutPan(adxt, 0, adxt->outpan[0]);
    } else {
        ADXT_SetOutPan(adxt, 0, adxt->outpan[0]);
        ADXT_SetOutPan(adxt, 1, adxt->outpan[1]);
    }
}

void adxt_nlp_trap_entry(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    SJ      sji = adxt->sji;
    SJCK    ckA;
    SJCK    ckB;
    SJCK    ckC;
    SJCK    ckD;
    short   ofst[2];
    short   ofst0;
    short   ofst1;
    int     zero;
    int     skip;
    int     ret0;
    int     ret1;

    if (adxt->lnkflg == 0) {
        return;
    }

    ofst[0] = 0;
    ofst[1] = 0;
    zero    = 0;
    skip    = zero;

    ADXCRS_Enter();

    SJ_GetChunk(sji, 1, SJCK_LEN_MAX, &ckA);
    SJ_GetChunk(sji, 1, SJCK_LEN_MAX, &ckC);

    // A non-zero here means the header scan already consumed the link, so
    // unwind and bail. (Opposite polarity to the two decoders below.)
    if (ADXSJD_GetFormat(sjd) == 0 && ADX_DecodeFooter(ckA.data, ckA.length, &ofst[1]) != 0) {
        ADXT_SetLnkSw(adxt, 0);
        SJ_UngetChunk(sji, 1, &ckC);
        SJ_UngetChunk(sji, 1, &ckA);
        ADXCRS_Leave();
        return;
    }

    skip += ofst[1];

    if (ADXSJD_GetFormat(sjd) == 1) {
        ret0 = ADX_ScanInfoCodeWav(ckA.data + skip, ckA.length - skip, &ofst[1]);
        if (ret0 != 0) {
            ret1 = ADX_ScanInfoCodeWav(ckC.data, ckC.length, &ofst[0]);
        }
    } else {
        ret0 = ADX_ScanInfoCode(ckA.data + skip, ckA.length - skip, &ofst[1]);
        if (ret0 != 0) {
            ret1 = ADX_ScanInfoCode(ckC.data, ckC.length, &ofst[0]);
        }
    }

    ofst1 = ofst[1];
    ofst0 = ofst[0];

    if (ret0 != 0 && ret1 != 0) {
        SJ_UngetChunk(sji, 1, &ckC);
        SJ_UngetChunk(sji, 1, &ckA);
        ADXT_SetLnkSw(adxt, 0);
        ADXCRS_Leave();
        return;
    }

    if (ret0 == 0) {
        SJ_UngetChunk(sji, 1, &ckC);
        SJ_SplitChunk(&ckA, skip + ofst1, &ckA, &ckB);
        SJ_PutChunk(sji, 0, &ckA);
        SJ_UngetChunk(sji, 1, &ckB);
    } else {
        SJ_PutChunk(sji, 0, &ckA);
        SJ_SplitChunk(&ckC, zero + ofst0, &ckC, &ckD);
        SJ_PutChunk(sji, 0, &ckC);
        SJ_UngetChunk(sji, 1, &ckD);
    }

    ADXCRS_Leave();

    adxt->decofst += ADXSJD_GetDecNumSmpl(sjd);
    ADXSJD_Stop(sjd);
    ADXSJD_Start(sjd);
    ADXSJD_ExecHndl(sjd);

    if (ADXSJD_GetStat(sjd) != ADXSJD_STAT_DECINFO) {
        ADXT_SetLnkSw(adxt, 0);
        return;
    }

    ADXSJD_SetMaxDecSmpl(sjd, adxt->maxdecsmpl);
    ADXSJD_SetTrapNumSmpl(sjd, ADXSJD_GetTotalNumSmpl(sjd));
    ADXSJD_SetTrapDtLen(sjd, 0);
    ADXSJD_SetTrapCnt(sjd, 0);
}

void adxt_stat_decinfo(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    int     num_chan;
    int     sfreq;
    int     num_smpl;
    int     num_loop;
    int     blk_smpl;
    int     lp_end_ofst;
    int     stat;
    int     a;
    int     b;
    char    num_chan_str[32];

    a = 0;
    b = 0;

    if ((unsigned char)adxt->pmode <= 1 && adxt->streamStartFlag == 1) {
        if (ADXSTM_GetStat(adxt->stm) == 2) {
            return;
        }

        if (adxt->sjf != NULL) {
            SJ_Reset(adxt->sjf);
        }

        adxt_start_stm(adxt, adxt->filename, adxt->directory, adxt->offset, adxt->range);
        adxt->streamStartFlag = 0;
    }

    stat = ADXSJD_GetStat(sjd);

    if (stat == ADXSJD_STAT_DECINFO) {
        num_chan = ADXSJD_GetNumChan(sjd);

        if (num_chan > adxt->maxnch) {
            ADXERR_ItoA2(num_chan, adxt->maxnch, num_chan_str, 16);
            ADXERR_CallErrFunc2(data_02063f08, num_chan_str);
            ADXT_Stop(adxt);
            return;
        }

        sfreq    = ADXSJD_GetSfreq(sjd);
        num_loop = ADXSJD_GetNumLoop(sjd);

        // AHX (format 10) decodes one block per server tick; everything else
        // gets 3 (looping) or 1.5 ticks' worth, rounded to a whole stereo block.
        if (ADXSJD_GetFormat(sjd) == 10) {
            adxt->maxdecsmpl = sfreq / adxt->svrfreq;
            blk_smpl         = ADXSJD_GetBlkSmpl(sjd);
        } else {
            if (num_loop > 0) {
                adxt->maxdecsmpl = (sfreq / adxt->svrfreq) * 3;
            } else {
                adxt->maxdecsmpl = ((sfreq / adxt->svrfreq) * 3) / 2;
            }
            blk_smpl = ADXSJD_GetBlkSmpl(sjd) * 2;
        }

        adxt->maxdecsmpl = ((adxt->maxdecsmpl + blk_smpl) / blk_smpl) * blk_smpl;
        ADXSJD_SetMaxDecSmpl(sjd, adxt->maxdecsmpl);

        if (num_loop > 0) {
            if (adxt->pmode == ADXT_PLAYBACK_MEM) {
                adxt->lp_skiplen = 0;
            } else {
                lp_end_ofst      = ADXSJD_GetLpEndOfst(sjd);
                adxt->lp_skiplen = 2048 - (lp_end_ofst % 2048);
                lp_end_ofst += 2047;
                adxt->lp_skiplen %= 2048;
                adxt->lesct = lp_end_ofst / 2048;
                ADXSTM_SetEos(adxt->stm, adxt->lesct);
                ADXSTM_EntryEosFunc(adxt->stm, (int)adxt_eos_entry, (int)adxt);
            }

            ADXSJD_GetLpEndPos(sjd);
            adxt->trpnsmpl = ADXSJD_GetLpStartPos(sjd);
            ADXSJD_SetTrapNumSmpl(sjd, adxt->trpnsmpl);
            ADXSJD_SetTrapDtLen(sjd, 0);
            ADXSJD_SetTrapCnt(sjd, 0);
            ADXSJD_EntryTrapFunc(sjd, adxt_trap_entry_lps, adxt);
        } else {
            if (adxt->stm != NULL) {
                ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
            }

            ADXSJD_SetTrapNumSmpl(sjd, ADXSJD_GetTotalNumSmpl(sjd));
            ADXSJD_SetTrapDtLen(sjd, 0);
            ADXSJD_SetTrapCnt(sjd, 0);
            ADXSJD_EntryTrapFunc(sjd, adxt_nlp_trap_entry, adxt);
        }

        sfreq    = ADXSJD_GetSfreq(sjd);
        num_chan = ADXSJD_GetNumChan(sjd);
        num_smpl = ADXSJD_GetTotalNumSmpl(sjd);

        ADXRNA_SetBitPerSmpl(adxt->rna, ADXSJD_GetOutBps(sjd));
        ADXRNA_SetSfreq(adxt->rna, sfreq);
        ADXRNA_SetNumChan(adxt->rna, num_chan);
        ADXRNA_SetTotalNumSmpl(adxt->rna, num_smpl);

        ADXT_SetOutVol(adxt, adxt->outvol);

        ADXT_GetTranspose(adxt, &a, &b);

        if (a != 0 || b != 0) {
            ADXT_SetTranspose(adxt, a, b);
        }

        adxt_set_outpan(adxt);

        if (adxt->amp != NULL) {
            ADXAMP_SetSfreq(adxt->amp, sfreq);
        }

        if (ADXSJD_GetFormat(sjd) == 2) {
            ADXRNA_SetStmHdInfo(adxt->rna, ADXSJD_GetSpsdInfo(sjd));
        }
        ADXRNA_SetTransSw(adxt->rna, 1);

        if (data_0206c7fc.f_7fc != 0) {
            ADXT_CB_FUNC(adxt, sfreq, num_chan, num_smpl);
        }

        adxt->stat = ADXT_STAT_PREPPING;
    } else if (stat == 4) {
        adxt->stat = ADXT_STAT_ERROR;
    }
}

void adxt_stat_prep(ADXT adxt) {
    void*   rna = adxt->rna;
    ADXSJD* sjd = adxt->sjd;
    int     num_data;
    int     num_room;
    int     num_chan;
    int     limit;
    int     size;
    int     i;
    SJCK    ck;

    num_data = ADXRNA_GetNumData(rna);
    num_room = ADXRNA_GetNumRoom(rna);

    limit = adxt->maxdecsmpl;
    if (limit >= 0x800) {
        limit = 0x800;
    }

    if (num_data >= limit || num_room <= ADXSJD_GetBlkSmpl(sjd) || ADXSJD_GetStat(sjd) == ADXSJD_STAT_PLAYING) {
        if (adxt->waitFlag == 0) {
            if (adxt->pause_flag == 0) {
                ADXRNA_SetPlaySw(rna, 1);
                adxt->tvofst = 0;
                adxt->svcnt  = adxt_vsync_cnt;
            }

            adxt->stat = ADXT_STAT_PLAYING;
        }

        adxt->readyFlag = 1;
    }

    if (ADXSJD_GetStat(sjd) != ADXSJD_STAT_PLAYING) {
        return;
    }

    num_chan = ADXT_GetNumChan(adxt);
    size     = (adxt->maxdecsmpl * num_chan) << 1;

    for (i = 0; i < num_chan; i++) {
        SJ sj = adxt->sjo[i];

        SJ_GetChunk(sj, i, size, &ck);
        memset(ck.data, i, ck.length);
        SJ_PutChunk(sj, 1, &ck);
    }
}

void adxt_stat_playing(ADXT adxt) {
    int num_chan;
    int i;

    if (adxt->lpflg == 0 && adxt->loopDecodeLength != 0 && ADXSJD_GetDecDtLen(adxt->sjd) >= (int)adxt->loopDecodeLength) {
        ADXSJD_SetTrapNumSmpl(adxt->sjd, -1);
    }

    if (ADXSJD_GetStat(adxt->sjd) != ADXSJD_STAT_PLAYING) {
        return;
    }

    num_chan     = ADXSJD_GetNumChan(adxt->sjd);
    adxt_dbg_nch = num_chan;

    for (i = 0; i < num_chan; i++) {
        int nbyte = SJ_GetNumData(adxt->sjo[i], 1);

        adxt_dbg_ndt = nbyte;

        if (nbyte >= 0x40) {
            break;
        }
    }

    if (i != num_chan) {
        return;
    }

    ADXRNA_SetTransSw(adxt->rna, 0);
    adxt->stat = ADXT_STAT_DECEND;
}

void adxt_stat_decend(ADXT adxt) {
    adxt_dbg_rna_ndata = ADXRNA_GetNumData(adxt->rna);

    if (ADXRNA_GetNumData(adxt->rna) <= 0) {
        ADXRNA_SetPlaySw(adxt->rna, 0);
        adxt->stat = ADXT_STAT_PLAYEND;
    }
}

void adxt_stat_playend(ADXT adxt) {
    return; // do nothing
}

void ADXT_ExecRdErrChk(ADXT adxt) {
    if (adxt->stm != NULL && ADXSTM_GetStat(adxt->stm) == 4) {
        adxt->ercode = -1;
        adxt->stat   = ADXT_STAT_ERROR;
    }

    if (adxt->lsc != NULL && LSC_GetStat(adxt->lsc) == 3) {
        adxt->ercode = -1;
        adxt->stat   = ADXT_STAT_ERROR;
    }
}

void ADXT_ExecRdCompChk(ADXT adxt) {
    if (adxt->stm == NULL || ADXT_GetStat(adxt) == 0) {
        return;
    }

    switch (adxt->pmode) {
        case ADXT_PLAYBACK_FILENAME:
        case ADXT_PLAYBACK_AFS:
            if (ADXSTM_GetStat(adxt->stm) == 3) {
                ADXSJD_TermSupply(adxt->sjd);
            }
            break;
        case ADXT_PLAYBACK_MEM:
            ADXSJD_TermSupply(adxt->sjd);
            break;
        case ADXT_PLAYBACK_STREAM:
        case ADXT_PLAYBACK_SLFILE:
        default:
            break;
    }
}

void ADXT_ExecHndl(ADXT adxt) {
    ADXCRS_Enter();
    adxt_ExecHndl(adxt);
    ADXCRS_Leave();
}

void adxt_ExecHndl(ADXT adxt) {
    char stat;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1(data_02063f48);
        return;
    }

    stat = adxt->stat;

    if (stat == ADXT_STAT_PLAYING) {
        adxt_stat_playing(adxt);
    } else if (stat == ADXT_STAT_LOADING) {
        adxt_stat_decinfo(adxt);
    } else if (stat == ADXT_STAT_PREPPING) {
        adxt_stat_prep(adxt);
    } else if (stat == ADXT_STAT_DECEND) {
        adxt_stat_decend(adxt);
    } else if (stat == ADXT_STAT_PLAYEND) {
        adxt_stat_playend(adxt);
    }

    ADXT_ExecRdCompChk(adxt);
    ADXT_ExecRdErrChk(adxt);
}