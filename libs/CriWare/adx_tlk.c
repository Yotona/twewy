#include <CriWare/adx_inis.h>
#include <CriWare/adxt.h>
#include <CriWare/lsc.h>

static ADXT adxt_Create(s32 maxChans, void* work, s32 worksize);
static ADXT func_02016c90(void* work, s32 workSize);
static void adxt_Destroy(ADXT adxt);
static void adxt_DestroyAll(void);
static void adxt_SetLpFlg(ADXT adxt, s32 flag);
static void adxt_SetOutPan(ADXT adxt, s32 channel, s32 pan);
static s32  adxt_GetStat(ADXT adxt);
static s32  adxt_GetNumChan();
static void adxt_SetOutVol(ADXT adxt, s32 vol);
static s32  adxt_GetOutVol(ADXT adxt);
static void adxt_Pause(ADXT adxt, s32 pauseState);
static s32  adxt_GetStatPause(ADXT adxt);
static void adxt_Stop(ADXT adxt);
static void adxt_ExecHndl(ADXT adxt);
void        ADXT_SetExtraInfo(ADXT adxt, s8 flag);
static void adxt_SetLnkSw(ADXT adxt, s32 sw);

extern s32 data_0206c7c0;
s32        data_02063ae8 = 25;
extern s32 data_0206bd7c;

extern s32 adxt_def_svrfreq;
extern f32 adxt_diff_av = 0.0f;

void (*ahxdetachfunc)(ADXT) = NULL;
s32 adxt_time_unit          = 0;
s32 adxt_time_mode          = 0;
s32 adxt_time_adjust_sw     = 1;

void func_02012f88();
void func_02012f8c();
void ADXERR_CallErrFunc1(const char*);
void memset(void*, int, int);

void func_020168f4(void) {
    func_02012f88();
}

void func_02016900(void) {
    func_02012f8c();
}

ADXT ADXT_Create(s32 maxChans, void* work, s32 workSize) {
    ADXT padxt;

    func_02012f88();
    padxt = adxt_Create(maxChans, work, workSize);
    func_02012f8c();
    return padxt;
}

static ADXT adxt_Create(s32 maxChans, void* work, s32 workSize) {
    ADXT adxt;

    s32 aligned_work = ((u32)work + 0x3F) & ~0x3F;
    s32 size         = workSize - (aligned_work - (u32)work);

    if (maxChans < 0 || work == NULL || workSize < 0) {
        ADXERR_CallErrFunc1("E02080804 adxt_Create: parameter error");
        return NULL;
    }

    s32 idx;
    for (idx = 0; idx < 4; idx++) {
        if (adxt_obj[idx].used == FALSE) {
            break;
        }
    }

    if (idx == 4) {
        ADXERR_CallErrFunc1("E03100801 adxt_Create: Too many handles.");
        return NULL;
    }

    adxt = &adxt_obj[idx];
    memset(adxt, 0, sizeof(ADX_TALK));

    adxt->maxnch = maxChans;

    s32 ibufSize = size - ADXT_CALC_OBUFSIZE(maxChans) - 0x124;

    adxt->ibuf    = (s8*)(aligned_work + ADXT_CALC_OBUFSIZE(maxChans));
    adxt->ibuflen = ibufSize / 0x800 * 0x800;

    if (adxt->ibuflen < 0) {
        ADXERR_CallErrFunc1("E06022201 adxt_Create: Work size is too short.");
        return NULL;
    }

    adxt->ibufxlen     = 36;
    adxt->workFilename = adxt->ibuf + (adxt->ibuflen + 0x24);
    adxt->obuf         = aligned_work;
    adxt->obufsize     = 2048;
    adxt->obufdist     = 2144;

    adxt->sji = NULL;

    adxt->sjf = SJRBF_Create(adxt->ibuf, adxt->ibuflen, adxt->ibufxlen);
    if (adxt->sjf == NULL) {
        adxt_Destroy(adxt);
        return NULL;
    }

    adxt->stm = ADXSTM_Create(adxt->sjf, 0);
    if (adxt->stm == NULL) {
        adxt_Destroy(adxt);
        return NULL;
    }

    for (s32 ix = 0; ix < maxChans; ix++) {
        adxt->sjo[ix] =
            SJRBF_Create(adxt->obuf + adxt->obufdist * ix, adxt->obufsize * 2, (adxt->obufdist - adxt->obufsize) * 2);
        if (adxt->sjo[ix] == NULL) {
            adxt_Destroy(adxt);
            return NULL;
        }
    }

    adxt->sjd = ADXSJD_Create(adxt->sjf, maxChans, adxt->sjo);
    if (adxt->sjd == NULL) {
        adxt_Destroy(adxt);
        return NULL;
    }

    adxt->rna = ADXRNA_Create(adxt->sjo, maxChans, (maxChans * 0x10C0) + size);
    if (adxt->rna == NULL) {
        adxt_Destroy(adxt);
        return NULL;
    }

    adxt->lsc = LSC_Create(adxt->sjf);
    if (adxt->lsc == NULL) {
        adxt_Destroy(adxt);
        return NULL;
    }

    LSC_SetStmHndl(adxt->lsc, adxt->stm);
    ADXCRS_Lock();

    adxt->svrfreq = adxt_def_svrfreq;
    adxt->maxsct  = adxt->ibuflen / 0x800;
    adxt->minsct  = adxt->maxsct * 0.85f;
    adxt->outvol  = 0;
    for (s32 i = 0; i < maxChans; i++) {
        adxt->outpan[i] = -0x80;
    }
    adxt->outbalance = 0;
    adxt->lpflg      = 1;
    adxt->trp        = 0;
    adxt->wpos       = 0;
    adxt->mofst      = 0;
    adxt->ercode     = 0;
    adxt->edecpos    = 0;
    adxt->edeccnt    = 0;
    adxt->eshrtcnt   = 0;
    adxt->autorcvr   = 1;
    adxt->pause_flag = 0;
    adxt->time_ofst  = 0;

    adxt_SetLnkSw(adxt, 0);
    ADXT_SetExtraInfo(adxt, 1);
    adxt->used = 1;
    ADXCRS_Unlock();
    return adxt;
}

ADXT func_02016c64(void* work, s32 workSize) {
    ADXT adxt;

    func_02012f88();
    adxt = func_02016c90(work, workSize);
    func_02012f8c();
    return adxt;
}

ADXT func_02016c90(void* work, s32 workSize) {
    ADXT adxt = adxt_Create(1, work, workSize);
    if (adxt == NULL) {
        adxt = NULL;
    }
    return adxt;
}

void adxt_detach_ahx(ADXT adxt) {
    if (ahxdetachfunc != NULL) {
        ahxdetachfunc(adxt);
    }
}

void ADXT_Destroy(ADXT adxt) {
    func_02012f88();
    adxt_Destroy(adxt);
    func_02012f8c();
}

void adxt_Destroy(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080805 adxt_Destroy: parameter error");
        return;
    }

    adxt_detach_ahx(adxt);

    if (adxt->used == TRUE) {
        adxt_Stop(adxt);
    }

    ADXRNA rna = adxt->rna;
    if (rna != NULL) {
        adxt->rna = NULL;
        ADXRNA_Destroy(rna);
    }

    ADXSJD* sjd = adxt->sjd;
    if (sjd != NULL) {
        adxt->sjd = NULL;
        ADXSJD_Destroy(sjd);
    }

    ADXSTM* stm = adxt->stm;
    if (stm != NULL) {
        adxt->stm = NULL;
        ADXSTM_EntryEosFunc(stm, 0, 0);
        ADXSTM_Destroy(stm);
    }

    LSC lsc = adxt->lsc;
    if (lsc != NULL) {
        adxt->lsc = NULL;
        LSC_Destroy(lsc);
    }

    ADXCRS_Lock();

    SJ* sjf = adxt->sjf;
    if (sjf != NULL) {
        adxt->sjf = NULL;
        SJ_Destroy(sjf);
    }

    for (s32 channel = 0; channel < adxt->maxnch; channel++) {
        SJ* sj = adxt->sjo[channel];
        if (sj != NULL) {
            adxt->sjo[channel] = NULL;
            SJ_Destroy(sj);
        }

        SJ* ampsji = adxt->ampsji[channel];
        if (ampsji != NULL) {
            adxt->ampsji[channel] = NULL;
            SJ_Destroy(ampsji);
        }

        SJ* ampsjo = adxt->ampsjo[channel];
        if (ampsjo != NULL) {
            adxt->ampsjo[channel] = NULL;
            SJ_Destroy(ampsjo);
        }
    }

    void* amp = adxt->amp;
    if (amp != NULL) {
        adxt->amp = NULL;
        ADXAMP_Destroy(amp);
    }

    memset(adxt, 0, sizeof(ADX_TALK));
    adxt->used = FALSE;
    ADXCRS_Unlock();
}

void ADXT_DestroyAll(void) {
    func_02012f88();
    adxt_DestroyAll();
    func_02012f8c();
}

void adxt_DestroyAll(void) {
    s32 idx;

    for (idx = 0; idx < 4; idx++) {
        if (adxt_obj[idx].used == TRUE) {
            adxt_Destroy(&adxt_obj[idx]);
        }
    }
}

void adxt_start_sj(ADXT adxt, SJ* sj) {
    s32 idx;

    for (idx = 0; idx < adxt->maxnch; idx++) {
        SJ_Reset(adxt->sjo[idx]);
    }

    ADXSJD_SetInSj(adxt->sjd, sj);
    adxt->sji = sj;
    ADXSJD_Start(adxt->sjd);
    adxt->stat             = 1;
    adxt->lpcnt            = 0;
    adxt->readyFlag        = 0;
    adxt->lesct            = 0x7FFFFFFF;
    adxt->trpnsmpl         = -1;
    adxt->tvofst           = 0;
    adxt->decofst          = 0;
    adxt->svcnt            = adxt_vsync_cnt;
    adxt->loopDecodeLength = 0;

    if (adxt->pmode == ADXT_PLAYBACK_SLFILE) {
        ADXT_SetLpFlg(adxt, 0);
    }
    if (adxt->amp != NULL) {
        ADXAMP_Start(adxt->amp);
    }
}

// Nonmatching: Bit logic differences
void adxt_start_stm(ADXT adxt, const char* filename, void* dir, s32 arg3, s32 arg4) {
    ADXSTM_SetBufSize(adxt->stm, adxt->minsct << 11, adxt->maxsct << 11);
    ADXSTM_SetEos(adxt->stm, data_02063ae8);
    ADXSTM_EntryEosFunc(adxt->stm, NULL, NULL);
    ADXSTM_Seek(adxt->stm, 0);
    ADXSTM_StopNw(adxt->stm);
    ADXSTM_ReleaseFileNw(adxt->stm);
    ADXSTM_BindFileNw(adxt->stm, filename, dir, arg3, arg4 << 11, ((arg4 >> 0x1F) << 0xB) | ((u32)arg4 >> 0x15));
    ADXSTM_Start(adxt->stm);
    adxt_start_sj(adxt, adxt->sjf);
}

void ADXT_StopWithoutLsc(ADXT adxt) {
    SJ* sj;

    ADXCRS_Lock();
    ADXRNA_SetTransSw(adxt->rna, 0);
    ADXRNA_SetPlaySw(adxt->rna, 0);
    ADXSJD_Stop(adxt->sjd);
    if (adxt->pmode == ADXT_PLAYBACK_MEM) {
        sj = adxt->sji;
        if (sj != NULL) {
            adxt->sji = NULL;
            SJ_Destroy(sj);
        }
    }

    if (adxt->amp != NULL) {
        ADXAMP_Stop(adxt->amp);
    }

    adxt->sji             = NULL;
    adxt->stat            = 0;
    adxt->streamStartFlag = 0;
    ADXCRS_Unlock();
}

void ADXT_Stop(ADXT adxt) {
    func_02012f88();
    adxt_Stop(adxt);
    func_02012f8c();
}

void adxt_Stop(ADXT adxt) {
    SJ* sj;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080813 adxt_Stop: parameter error");
        return;
    }

    if (adxt->stm != NULL) {
        ADXSTM_ReleaseFileNw(adxt->stm);
    }

    if (adxt->pmode == ADXT_PLAYBACK_SLFILE) {
        LSC_Stop(adxt->lsc);
        sj = adxt->sji;
        if (adxt->sji != NULL) {
            SJ_Reset(adxt->sji);
        }
    }

    ADXT_StopWithoutLsc(adxt);
}

s32 ADXT_GetStat(ADXT adxt) {
    s32 stat;

    func_02012f88();
    stat = adxt_GetStat(adxt);
    func_02012f8c();
    return stat;
}

s32 adxt_GetStat(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080814 adxt_GetStat: parameter error");
        return -1;
    }
    return adxt->stat;
}

void ADXT_GetTimeSfreq(ADXT adxt, s32* count, s32* frequency) {
    s8 stat = adxt->stat;

    if ((u32)(u8)(s8)(stat - 3) <= 1) {
        func_0201bf00(adxt->rna, count, frequency);
    } else if (stat == ADXT_STAT_PLAYEND) {
        *count     = ADXSJD_GetTotalNumSmpl(adxt->sjd);
        *frequency = ADXSJD_GetSfreq(adxt->sjd);
        *count *= (16 / ADXSJD_GetOutBps(adxt->sjd));
    } else {
        *count     = 0;
        *frequency = 1;
    }
    *count += adxt->time_ofst;
}

void adxt_GetTime(ADXT adxt, s32* count, s32* frequency) {
    s32 loc_count;
    s32 loc_freq;
    s32 temp_time_mode;

    if (adxt == NULL || count == NULL || frequency == NULL) {
        ADXERR_CallErrFunc1("E02080815 adxt_GetTime: parameter error");
        return;
    }

    if (adxt_time_mode == 0) {
        ADXT_GetTimeSfreq(adxt, count, frequency);
        return;
    }

    adxt_diff_av = 0.0f;

    if ((u32)(u8)(s8)(adxt->stat - 3) <= 1) {
        if (adxt->pause_flag == 0) {
            *count = ((adxt_vsync_cnt - adxt->svcnt) * 100) + adxt->tvofst;
        } else {
            *count = adxt->tvofst;
        }

        ADXT_GetTimeSfreq(adxt, &loc_count, &loc_freq);
        adxt_diff_av = (((f32)loc_count / loc_freq) - ((f32)*count / adxt_time_unit)) * 1000.0f;

        if (adxt_diff_av > 60.0f || adxt_diff_av < -60.0f) {
            func_0201bf00(adxt->rna, &loc_count, &loc_freq);

            adxt->tvofst = (f32)loc_count / loc_freq * adxt_time_unit;
            adxt->svcnt  = adxt_vsync_cnt;
        }
    } else if (adxt->stat == ADXT_STAT_PLAYEND) {
        loc_count = ADXSJD_GetTotalNumSmpl(adxt->sjd);
        loc_freq  = ADXSJD_GetSfreq(adxt->sjd);
        loc_count *= (16 / ADXSJD_GetOutBps(adxt->sjd));
        *count = (f32)loc_count / loc_freq * adxt->time_ofst;
        *count += adxt->tvofst + 1;
    } else {
        *count = 0;
    }

    *count += adxt->time_ofst;
    *frequency = adxt_time_unit;
}

s32 ADXT_Play(ADXT adxt) {
    s32 val;

    func_02012f88();
    val = func_020174c8(adxt);
    func_02012f8c();
    return val;
}

s32 func_020174c8(ADXT adxt) {
    s32 local_c;
    s32 local_10;

    adxt_GetTime(adxt, &local_c, &local_10);
    return 100.0f * ((f32)local_c / (f32)local_10);
}

s32 ADXT_GetNumChan(ADXT adxt) {
    s32 numChan;

    func_02012f88();
    numChan = adxt_GetNumChan(adxt);
    func_02012f8c();
    return numChan;
}

s32 adxt_GetNumChan(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080820 adxt_GetNumChan: parameter error");
        return -1;
    }
    if (adxt->stat >= ADXT_STAT_PREPPING) {
        return func_02015948(adxt->sjd);
    }
    return 0;
}

void ADXT_SetOutPan(ADXT adxt, s32 channel, s32 pan) {
    func_02012f88();
    adxt_SetOutPan(adxt, channel, pan);
    func_02012f8c();
}

void adxt_SetOutPan(ADXT adxt, s32 channel, s32 pan) {
    int iVar1;
    int iVar2;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080825 adxt_SetOutPan: Null pointer is passed");
        return;
    }

    if (channel != 0 && channel != 1) {
        ADXERR_CallErrFunc1("E07041801 adxt_SetOutPan: parameter error");
        return;
    }

    if (pan != -128) {
        if (pan < -15) {
            pan = -15;
        } else if (pan > 15) {
            pan = 15;
        }
    }

    if (adxt->extraInfoFlag == TRUE) {
        iVar1 = func_02015a2c(adxt->sjd, channel);
        if (iVar1 == -128) {
            iVar1 = 0;
        }
    } else {
        iVar1 = 0;
    }

    if (data_0206bd7c == 0) {
        if (pan == -128) {
            iVar2 = func_02015948((int)adxt->sjd);
            if (iVar2 == 2) {
                if (channel == 0) {
                    iVar2 = -15;
                } else {
                    iVar2 = 15;
                }
            } else {
                iVar2 = 0;
            }
            iVar2 += iVar1;
        } else {
            iVar2 = pan + iVar1;
        }
    } else {
        iVar2 = 0;
    }
    adxt->outpan[channel] = pan;
    if (channel < adxt->maxnch) {
        func_0201bf54((int)adxt->rna, channel, iVar2);
    } else {
        ADXERR_CallErrFunc1("E8101208 adxt_SetOutPan: parameter error");
    }
}

void ADXT_SetOutVol(ADXT adxt, s32 vol) {
    func_02012f88();
    adxt_SetOutVol(adxt, vol);
    func_02012f8c();
}

void adxt_SetOutVol(ADXT adxt, s32 vol) {
    s32 amp;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080823 adxt_SetOutVol: parameter error");
        return;
    }
    adxt->outvol = vol;
    if (adxt->extraInfoFlag == 1) {
        amp = ADXSJD_GetDefOutVol(adxt->sjd);
    } else {
        amp = 0;
    }
    ADXRNA_SetOutVol(adxt->rna, adxt->outvol + amp);
}

s32 ADXT_GetOutVol(ADXT adxt) {
    s32 outVol;

    func_02012f88();
    outVol = adxt_GetOutVol(adxt);
    func_02012f8c();
    return outVol;
}

s32 adxt_GetOutVol(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080824 adxt_GetOutVol: parameter error");
        return 0;
    }
    return adxt->outvol;
}

void ADXT_SetExtraInfo(ADXT adxt, s8 flag) {
    adxt->extraInfoFlag = flag;
}

void func_02017788(s32 freq) {
    func_02012f88();
    ADXT_SetDefSvrFreq(freq);
    func_02012f8c();
}

// Nonmatching: Data differences
void ADXT_SetDefSvrFreq(s32 freq) {
    adxt_def_svrfreq = freq;
}

void func_020177b8() {
    func_02012f88();
    ADXT_ExecServer();
    func_02012f8c();
}

void ADXT_ExecServer() {
    /* NYI */
}

void ADXT_SetLpFlg(ADXT adxt, s32 flag) {
    func_02012f88();
    adxt_SetLpFlg(adxt, flag);
    func_02012f8c();
}

void adxt_SetLpFlg(ADXT adxt, s32 flag) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080828 adxt_SetLpFlg: parameter error");
        return;
    }
    /* NYI */
}

void ADXT_Pause(ADXT adxt, s32 pauseState) {
    func_02012f88();
    adxt_Pause(adxt, pauseState);
    func_02012f8c();
}

// Nonmatching: Data differences
void adxt_Pause(ADXT adxt, s32 pauseState) {
    s32 count;
    s32 frequency;
    s32 time_mode_temp;
    s8  stat;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080846 adxt_Pause: parameter error");
        return;
    }

    stat = adxt->stat;

    if (pauseState == adxt->pause_flag) {
        return;
    }

    ADXCRS_Lock();
    adxt->pause_flag = pauseState;

    if ((u32)(stat - 3) <= 1) {
        if (pauseState == TRUE) {
            ADXRNA_SetPlaySw(adxt->rna, FALSE);
        } else {
            ADXRNA_SetPlaySw(adxt->rna, TRUE);
            adxt->svcnt = adxt_vsync_cnt;
        }

        time_mode_temp = adxt_time_mode;
        adxt_time_mode = 0;
        adxt_GetTime(adxt, &count, &frequency);
        adxt_time_mode = time_mode_temp;
        adxt->tvofst   = adxt_time_unit * ((f32)count / (f32)frequency);
    }
    ADXCRS_Unlock();
}

s32 ADXT_GetStatPause(ADXT adxt) {
    s32 statPause;

    func_02012f88();
    statPause = adxt_GetStatPause(adxt);
    func_02012f8c();
    return statPause;
}

s32 adxt_GetStatPause(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080847 adxt_GetStatPause: parameter error");
        return 0;
    }
    return adxt->pause_flag;
}

void func_02017b20(ADXT adxt, s32 param_1, s32 param_2) {
    func_02012f88();
    func_02017b78(adxt, param_1, param_2);
    func_02012f8c();
}

void func_02017b4c(ADXT adxt, s32 param_1, s32 param_2) {
    func_02012f88();
    func_02017b7c(adxt, param_1, param_2);
    func_02012f8c();
}

void func_02017b78(ADXT adxt, s32 param_1, s32 param_2) {
    return;
}

void func_02017b7c(ADXT adxt, s32 param_1, s32 param_2) {
    return;
}

void ADXT_SetLnkSw(ADXT adxt, s32 param_1) {
    func_02012f88();
    adxt_SetLnkSw(adxt, param_1);
    func_02012f8c();
}

static void adxt_SetLnkSw(ADXT adxt, s32 sw) {
    adxt->playbackFlag = sw;
    if (adxt->sjd != NULL) {
        ADXSJD_SetLnkSw(adxt->sjd, sw);
    }
}
