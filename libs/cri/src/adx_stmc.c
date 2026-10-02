#include <cri/adx_stmc.h>
#include <cri/sj.h>

ADXSTM* adxstm_Create(SJ sj, int offset);
void    adxstm_Destroy(ADXSTM* stm);
void    adxstm_ReleaseFileNw(ADXSTM* stm);
void    ADXSTM_ReleaseFile(ADXSTM* stm);
void    adxstm_ReleaseFile(ADXSTM* stm);
int     adxstm_GetStat(ADXSTM* stm);
int     adxstm_Seek(ADXSTM* stm, int param_2);
int     adxstm_Start(ADXSTM* stm);
void    ADXSTM_StopNw(ADXSTM* stm);
void    adxstm_StopNw(ADXSTM* stm);
void    ADXSTM_Stop(ADXSTM* stm);
void    adxstm_Stop(ADXSTM* stm);
void    adxstm_EntryEosFunc(ADXSTM* stm, int param_2, int param_3);
void    adxstm_SetEos(ADXSTM* stm, int param_2);
void    adxstm_ExecServer();
void    func_020168d0();
void    func_020168dc();
int     func_020168e8(int*);
int     adxstm_SetBufSize(ADXSTM* stm, int param_2, int param_3);
void    func_0201687c(void);

ADXSTMWork data_0206c398;

int data_0206c3a4 = 0;

int          adxstm_sj_internal_error_cnt = 0;
int          adxstmf_num_rtry             = 0;
volatile int adxstmf_execsvr_flag         = 0;
ADXSTM       adxstmf_obj[10];

int adxstmf_nrml_num  = 4;
int adxstmf_rtim_num  = 6;
int adxstmf_nrml_ofst = 6;
int adxstmf_rtim_ofst = 0;

int ADXSTM_Init(void) {
    if (++data_0206c3a4 == 1) {
        __builtin__clear(&adxstmf_obj, sizeof(adxstmf_obj));
    }

    return 1;
}

void ADXSTM_Finish(void) {
    ADXSTM* pAVar1;
    int     iVar2;
    ADXSTM* pAVar3;

    data_0206c3a4 += -1;
    if (data_0206c3a4 != 0) {
        return;
    }

    __builtin__clear(&adxstmf_obj, sizeof(adxstmf_obj));
}

// Nonmatching: one commutative add (the SJ_GetNumData sum) has its operands
// swapped; every other instruction matches. fnsct is ceil(fsize / 2048) spelled
// as divide + (remainder > 0), which is what the target's movle sequence is.
void ADXSTMF_SetupHandleMember(ADXSTM* stm, CVFSHandle* cvfs, int fofst, int fsize, SJ sj) {
    func_020168d0(); // ADXCRS_Lock

    stm->stat   = 1;
    stm->rdflg  = 0;
    stm->sj     = sj;
    stm->fp     = cvfs;
    stm->fofst  = fofst;
    stm->fsize  = fsize;
    stm->fnsct  = (fsize / 2048) + ((fsize % 2048 > 0) ? 1 : 0);
    stm->rdsct  = 0x200;
    stm->stpos  = 0;
    stm->unk_60 = 0xFFFFF;
    stm->esct   = stm->fnsct;

    if (stm->sj != NULL) {
        stm->minsize = stm->maxsize = stm->unk_44 = SJ_GetNumData(sj, 1) + SJ_GetNumData(sj, 0);
    }

    stm->pause = 0;
    stm->used  = 1;

    func_020168dc(); // ADXCRS_Unlock
}

static ADXSTM* ADXSTMF_CreateCvfsRt(int arg0, int offset, int file_len, SJ sj) {
    ADXSTM* stm = NULL;
    int     i;

    for (i = 0; i < adxstmf_rtim_num; i++) {
        stm = &adxstmf_obj[adxstmf_rtim_ofst + i];

        if (stm->used == 0) {
            break;
        }
    }

    if (i == adxstmf_rtim_num) {
        return NULL;
    }

    ADXSTMF_SetupHandleMember(stm, arg0, offset, file_len, sj);
    stm->unk_03 = 1;
    return stm;
}

static ADXSTM* ADXSTMF_CreateCvfs(int arg0, int offset, int file_len, SJ sj) {
    ADXSTM* stm = NULL;
    int     i;

    for (i = 0; i < adxstmf_nrml_num; i++) {
        stm = &adxstmf_obj[adxstmf_nrml_ofst + i];

        if (stm->used == 0) {
            break;
        }
    }

    if (i == adxstmf_nrml_num) {
        return NULL;
    }

    ADXSTMF_SetupHandleMember(stm, arg0, offset, file_len, sj);
    stm->unk_03 = 0;
    return stm;
}

ADXSTM* ADXSTM_Create(SJ sj, int offset) {
    func_020168f4();
    ADXSTM* stm = adxstm_Create(sj, offset);
    func_02016900();
    return stm;
}

ADXSTM* adxstm_Create(SJ sj, int offset) {
    ADXSTM* stm;

    if (offset < 0x100) {
        stm = ADXSTMF_CreateCvfsRt(0, 0, 0, sj);
    } else {
        stm = ADXSTMF_CreateCvfs(0, 0, 0, sj);
    }
    return stm;
}

void ADXSTM_Destroy(ADXSTM* stm) {
    func_020168f4();
    adxstm_Destroy(stm);
    func_02016900();
}

void adxstm_Destroy(ADXSTM* stm) {
    if (stm == NULL) {
        return;
    }
    ADXSTM_Stop(stm);
    ADXSTM_ReleaseFile(stm);
    stm->used = 0;
    memset(stm, 0, sizeof(ADXSTM));
}

void ADXSTM_BindFileNw(ADXSTM* stm, const char* fname, void* dir, int fofst, long long fsize) {
    func_020168f4();
    adxstm_BindFileNw(stm, fname, dir, fofst, fsize);
    func_02016900();
}

// NITRO takes a 64-bit byte size; the sector count is its 64-bit round-up.
void adxstm_BindFileNw(ADXSTM* stm, const char* fname, void* dir, int fofst, long long fsize) {
    func_020168d0();
    stm->fofst    = fofst;
    stm->fsize    = fsize;
    stm->fnsct    = (fsize + 0x7FF) / 0x800;
    stm->filename = fname;
    stm->dir      = dir;
    stm->unk_49   = 1;
    func_020168dc();
}

void ADXSTM_ReleaseFileNw(ADXSTM* stm) {
    func_020168f4();
    adxstm_ReleaseFileNw(stm);
    func_02016900();
}

void adxstm_ReleaseFileNw(ADXSTM* stm) {
    ADXSTM_StopNw(stm);
    func_020168d0();
    if (stm->unk_4D == 1) {
        stm->unk_4A = 1;
    }
    stm->unk_49 = 0;
    func_020168dc();
}

void ADXSTM_ReleaseFile(ADXSTM* stm) {
    func_020168f4();
    adxstm_ReleaseFile(stm);
    func_02016900();
}

void adxstm_ReleaseFile(ADXSTM* stm) {
    ADXSTM_Stop(stm);
    ADXSTM_ReleaseFileNw(stm);

    while (TRUE) {
        if (stm->unk_4D == 0) {
            break;
        }
        ADXT_ExecFsSvr(stm->unk_4D);
    }
}

int ADXSTM_GetStat(ADXSTM* stm) {
    int val;

    func_020168f4();
    val = adxstm_GetStat(stm);
    func_02016900();
    return val;
}

int adxstm_GetStat(ADXSTM* stm) {
    return stm->stat;
}

int ADXSTM_Seek(ADXSTM* stm, int param_2) {
    int val;

    func_020168f4();
    val = adxstm_Seek(stm, param_2);
    func_02016900();
    return val;
}

int adxstm_Seek(ADXSTM* stm, int param_2) {
    stm->stpos = param_2;
    if (param_2 > stm->fnsct) {
        stm->stpos = stm->fnsct;
    }
    return stm->stpos;
}

void adxstm_start_sub(ADXSTM* stm) {
    stm->tbyte  = 0;
    stm->errcnt = 0;

    stm->stat         = stm->fnsct == 0 ? 3 : 2;
    stm->rdflg        = 0;
    stm->reqck.data   = NULL;
    stm->reqck.length = 0;
    stm->unk_4B       = 1;
}

int ADXSTM_Start(ADXSTM* stm) {
    int sVar1;

    func_020168f4();
    sVar1 = adxstm_Start(stm);
    func_02016900();
    return sVar1;
}

int adxstm_Start(ADXSTM* stm) {
    func_020168d0();
    adxstm_start_sub(stm);
    stm->unk_60 = 0xfffff;
    func_020168dc();
    return 1;
}

void ADXSTM_StopNw(ADXSTM* stm) {
    func_020168f4();
    adxstm_StopNw(stm);
    func_02016900();
}

void adxstm_StopNw(ADXSTM* stm) {
    func_020168d0();

    if (stm->stat == 2 && stm->rdflg == 1) {
        stm->unk_4C = 1;
        if (stm->unk_4B == 1) {
            stm->unk_4B = 0;
        }
    } else {
        stm->stat = 1;
    }

    func_020168dc();
}

void ADXSTM_Stop(ADXSTM* stm) {
    func_020168f4();
    adxstm_Stop(stm);
    func_02016900();
}

void adxstm_Stop(ADXSTM* stm) {
    ADXSTM_StopNw(stm);

    while (TRUE) {
        if (stm->stat == 1) {
            if (stm->reqck.data == NULL) {
                break;
            }
        }
        ADXT_ExecFsSvr();
    }
}

void ADXSTM_EntryEosFunc(ADXSTM* stm, int param_2, int param_3) {
    func_020168f4();
    adxstm_EntryEosFunc(stm, param_2, param_3);
    func_02016900();
}

void adxstm_EntryEosFunc(ADXSTM* stm, int param_2, int param_3) {
    stm->eosfunc = param_2;
    stm->eosobj  = param_3;
}

void ADXSTM_SetEos(ADXSTM* stm, int param_2) {
    func_020168f4();
    adxstm_SetEos(stm, param_2);
    func_02016900();
}

void adxstm_SetEos(ADXSTM* stm, int param_2) {
    if (param_2 < 0) {
        param_2 = stm->fnsct;
    }
    stm->esct = param_2;
}

void adxstm_sj_internal_error(void) {
    adxstm_sj_internal_error_cnt++;
}

void adxstmf_stat_exec(ADXSTM* stm) {
    int         iVar2;
    _sj_vtable* p_Var3;
    int         iVar6;
    int         iVar7;
    SJCK        SStack_18;
    SJCK        SStack_20;
    SJCK        SStack_28;

    SJ  sj   = stm->sj;
    int stat = cvFsGetStat(stm->fp);

    func_020168d0();
    if (stm->rdflg == 1) {
        if (stat == 1) {
            stm->rdflg = 0;
            func_020168dc();
            iVar2 = stm->reqsct;
            SJ_SplitChunk(&stm->reqck, iVar2 << 0xb, &SStack_18, &SStack_20);
            SJ_PutChunk(sj, 1, &SStack_18);
            SJ_UngetChunk(sj, 0, &SStack_20);
            stm->stpos += stm->reqsct;
            stm->tbyte += iVar2 * 0x800;
            stm->reqck.data   = NULL;
            stm->reqck.length = 0;

            iVar2 = stm->fnsct;
            if (stm->stpos == stm->esct && stm->eosfunc != NULL) {
                stm->eosfunc(stm->eosobj);
            }
            if (stm->stpos >= iVar2) {
                stm->stat = 3;
            } else if (stm->unk_60 <= stm->tbyte >> 0xb && stm->unk_60 < 0xfffff) {
                stm->stat = 3;
            }
            stm->errcnt = 0;
        } else if (stat == 3) {
            stm->rdflg = 0;
            func_020168dc();
            SJ_UngetChunk(sj, 0, &stm->reqck);
            stm->reqck.data   = NULL;
            stm->reqck.length = 0;

            if (0 <= adxstmf_num_rtry && stm->errcnt >= adxstmf_num_rtry) {
                stm->stat = 4;
            } else if (stm->errcnt < 0x7fffffff) {
                stm->errcnt++;
            }

        } else {
            func_020168dc();
        }
    } else {
        func_020168dc();
    }
    if (stm->stat == 4) {
        return;
    }
    func_020168d0();
    if (stm->rdflg == 0) {

        stm->rdflg          = 1;
        (stm->reqck).data   = NULL;
        (stm->reqck).length = 0;
        func_020168dc();
        if (stm->pause == 1 || stm->unk_4C == 1) {
            stm->rdflg = 0;
            return;
        }
        p_Var3 = stm->fnsct;
        if (p_Var3 == NULL) {
            stm->rdflg  = 0;
            stm->reqsct = 0;
            stm->stat   = 3;
            return;
        }
        if (sj == NULL || (p_Var3 = sj->vtable) == NULL) {
            stm->rdflg = 0;
            adxstm_sj_internal_error();
            return;
        }
        if (stm->unk_44 - (*p_Var3->GetNumData)(sj, 0) >= stm->minsize) {
            stm->rdflg = 0;
            return;
        }

        SJ_GetChunk(sj, 0, stm->maxsize, &SStack_28);

        iVar2 = SStack_28.length / 0x800;
        iVar7 = stm->esct - stm->stpos;
        if (iVar2 >= iVar7) {
            iVar2 = iVar7;
        }

        iVar7 = stm->fnsct - stm->stpos;
        if (iVar2 >= iVar7) {
            iVar2 = iVar7;
        }
        if (iVar2 >= stm->rdsct) {
            iVar2 = stm->rdsct;
        }
        cvFsSeek(stm->fp, stm->fofst + stm->stpos, 0);
        if (stm->unk_60 != 0xfffff) {
            iVar6 = stm->unk_60 - ((int)stm->tbyte / 0x800);
            if (iVar2 >= iVar6) {
                iVar2 = iVar6;
            }
        }
        stm->reqsct       = cvFsReqRd(stm->fp, iVar2, SStack_28.data);
        stm->reqck.data   = SStack_28.data;
        stm->reqck.length = SStack_28.length;
        if (0 < stm->reqsct) {
            return;
        }
        SJ_UngetChunk(sj, 0, &stm->reqck);
        stm->reqck.data   = NULL;
        stm->reqck.length = 0;
        stm->rdflg        = 0;

        if (cvFsGetStat(stm->fp) != 3) {
            return;
        }

        if (0 <= adxstmf_num_rtry && stm->errcnt >= adxstmf_num_rtry) {
            stm->stat = 4;
            return;
        }
        if (stm->errcnt < 0x7fffffff) {
            stm->errcnt++;
        }
        return;
    }
    func_020168dc();
    return;
}

void ADXSTMF_ExecHndl(ADXSTM* stm) {
    void*     handle;
    long long fsize;
    int       fnsct;
    int       bVar8;

    if (stm->rdflg == 0) {
        if ((stm->unk_4C == 1) && (stm->unk_4C = 0, stm->unk_4B == 0)) {
            stm->stat = 1;
        }
        if (stm->unk_4A == 1) {
            handle = stm->fp;
            if (handle != NULL) {
                stm->fp = 0;
                cvFsClose(handle);
            }
            func_020168d0();
            stm->unk_4A = 0;
            stm->unk_4D = 0;
            func_020168dc();
        }
        func_020168d0();
        bVar8 = FALSE;
        if (stm->unk_49 == 1) {
            if (stm->unk_4D == 0) {
                stm->unk_4D = 1;
                func_020168dc();
                bVar8 = TRUE;
                if (stm->fp == NULL) {
                    stm->fp = cvFsOpen(stm->filename, stm->dir, 0);
                    if (stm->fp == NULL) {
                        ADXERR_CallErrFunc2("E02110501 adxstmf_stat_exec: can\'t open ", stm->filename);
                        stm->stat   = 4;
                        stm->unk_4D = 0;
                        stm->unk_49 = 0;
                        return;
                    }
                }
            }
            if (stm->unk_4D == 1) {
                if (bVar8 == FALSE) {
                    func_020168dc();
                }
                if (stm->unk_49 == 1 && stm->unk_4A == 1) {
                    return;
                }
                if (stm->dir == 0) {
                    fsize = cvFsGetFileSizeByHndl(stm->fp);
                    if (fsize < 0) {
                        fsize = cvFsGetFileSize(stm->filename);
                    }
                    fnsct = (fsize + 0x7FF) / 0x800;
                } else {
                    cvFsSeek(stm->fp, 0, 2);
                    fnsct = cvFsTell(stm->fp);
                    fsize = fnsct * 0x800;
                    cvFsSeek(stm->fp, 0, 0);
                }
                // 0x7FFFF800 is the "size unknown" placeholder BindFileNw was given.
                if (stm->fsize == 0x7FFFF800) {
                    stm->fsize = fsize;
                    stm->fnsct = fnsct;
                }
                if (stm->fofst > fnsct) {
                    stm->fofst = fnsct;
                }
                if (stm->fnsct + stm->fofst > fnsct) {
                    stm->fnsct = fnsct - stm->fofst;
                    stm->fsize = (long long)stm->fnsct << 11;
                }
                ADXSTM_Seek(stm, 0);
                stm->unk_49 = 0;
                if (cvFsGetStat(stm->fp) == 3) {
                    ADXERR_CallErrFunc2("E05072801 adxstmf_stat_exec: can\'t open ", stm->filename);
                    handle = stm->fp;
                    if (handle != NULL) {
                        stm->fp = NULL;
                        cvFsClose(handle);
                    }
                    stm->stat   = 4;
                    stm->unk_4D = 0;
                    stm->unk_49 = 0;
                    return;
                }
            }
        } else {
            func_020168dc();
        }
        if (stm->unk_4B == 1) {
            stm->unk_4B = 0;
        }
    }
    if (stm->stat == 2 && stm->unk_4D == 1 && stm->unk_49 == 0) {
        adxstmf_stat_exec(stm);
    }
}

void ADXSTM_ExecServer() {
    func_020168f4();
    adxstm_ExecServer();
    func_02016900();
}

void adxstm_ExecServer(void) {
    int idx;

    if (func_020168e8(&adxstmf_execsvr_flag) == 0) {
        return;
    }

    for (int idx = 0; idx < 10; idx++) {
        if (adxstmf_obj[idx].used == 1) {
            ADXSTMF_ExecHndl(&adxstmf_obj[idx]);
        }
    }

    adxstmf_execsvr_flag = 0;
}

void func_02016868(void) {
    func_020168f4();
    func_0201687c();
    func_02016900();
}

void func_0201687c(void) {
    cvFsExecServer();
}

void func_02016888(void) {
    return;
}

int ADXSTM_SetBufSize(ADXSTM* stm, int param_2, int param_3) {
    int sVar1;

    func_020168f4();
    sVar1 = adxstm_SetBufSize(stm, param_2, param_3);
    func_02016900();
    return sVar1;
}

int adxstm_SetBufSize(ADXSTM* stm, int param_2, int param_3) {
    stm->minsize = param_2;
    stm->maxsize = param_3;
    return 1;
}

void func_020168d0(void) {
    ADXCRS_Lock();
}

void func_020168dc(void) {
    ADXCRS_Unlock();
}

int func_020168e8(int*) {
    return SVM_TestAndSet();
}
