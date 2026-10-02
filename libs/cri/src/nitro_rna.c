#include <cri/adxt.h>
#include <cri/cri_xpt.h>
#include <cri/private/adx_rna.h>
#include <cri/private/nitro_snd.h>
#include <cri/sj.h>
#include <mem.h>

#define NITRORNA_MAX_OBJ 4
#define NITRORNA_MAX_CH  2
#define NITRORNA_NO_CH   0xFF

typedef struct {
    /* 0x00 */ s8           used;
    /* 0x01 */ s8           sw;    // bit 0: transfer, bit 1: play
    /* 0x02 */ s8           nch;   // SND channels allocated
    /* 0x03 */ s8           nchIn; // input channels
    /* 0x04 */ s8           mono;  // play one mono input on both channels
    /* 0x08 */ int          unk08;
    /* 0x0C */ s16          sfreq;
    /* 0x0E */ s16          bps;
    /* 0x10 */ SJ           sji[NITRORNA_MAX_CH];
    /* 0x18 */ SJ           sjo[NITRORNA_MAX_CH];
    /* 0x20 */ int          unk20[NITRORNA_MAX_CH];
    /* 0x28 */ s16          vol;
    /* 0x2A */ s16          pan[NITRORNA_MAX_CH];
    /* 0x30 */ NITROSND_CH* ch[NITRORNA_MAX_CH];
} NITRORNA_OBJ; // size: 0x38

typedef struct {
    /* 0x00 */ NITRORNA_OBJ obj[NITRORNA_MAX_OBJ];
    /* 0xE0 */ int          nch; // entries in chNo
    /* 0xE4 */ u8           chNo[8];
    /* 0xEC */ u8           chUsed[8];
    /* 0xF4 */ void (*allocCh)(u8* chNo);
} NITRORNA_WORK; // size: 0xF8

// A mono input played on both channels of a stereo handle.
#define NITRORNA_IS_DUAL_MONO(rna) ((rna)->mono == 1 && (rna)->nchIn == 1 && (rna)->nch == 2)

SJ   SJRBF_Create(void* buf, int bsize, int xsize);
void RNACRS_Lock(void);
void RNACRS_Unlock(void);
void RNAERR_Init(void);
void RNAERR_Finish(void);
void RNAERR_EntryErrFunc(void (*func)(void* obj, const char* msg), void* obj);
void RNAERR_CallErrFunc(const char* msg);

void NITRORNA_Destroy(ADXRNA hndl);
void NITRORNA_SetTransSw(ADXRNA hndl, int sw);
void NITRORNA_SetPlaySw(ADXRNA hndl, int sw);
int  func_0201cd84(NITRORNA_OBJ* rna);
int  func_0201ce58(NITRORNA_OBJ* rna);
void func_0201cedc(NITRORNA_OBJ* rna);
void func_0201d178(NITRORNA_OBJ* rna);
void NITRORNA_ExecHndl(NITRORNA_OBJ* rna);
void NITRORNA_SetNumChan(ADXRNA hndl, int nch);
void NITRORNA_SetSfreq(ADXRNA hndl, int sfreq);
void NITRORNA_SetOutVol(ADXRNA hndl, int vol);
void NITRORNA_SetOutPan(ADXRNA hndl, int ch, int pan);
void NITRORNA_SetBitPerSmpl(ADXRNA hndl, int bps);

#ifdef REGION_USA
const char nitrorna_build_str[] = "\nNITRORNA/NITRO Ver.0.98 Build:Sep 28 2007 13:14:07\n\0Append: MW4020\n";
#else
const char nitrorna_build_str[] = "\nNITRORNA/NITRO Ver.0.98 Build:Jun 22 2007 15:54:50\n\0Append: MW4020\n";
#endif

extern const char* nitrorna_build_ptr;

NITRORNA_WORK nitrorna_work;
int           nitrorna_init_cnt;

void NITRORNA_EntryErrFunc(void (*func)(void* obj, const char* msg), void* obj) {
    RNAERR_EntryErrFunc(func, obj);
}

void NITRORNA_Init(void) {
    nitrorna_build_ptr = nitrorna_build_str;
    if (nitrorna_init_cnt == 0) {
        RNAERR_Init();
        __builtin__clear(&nitrorna_work, sizeof(nitrorna_work));
        func_0201dc48();
        nitrorna_work.nch     = 2;
        nitrorna_work.chNo[0] = 4;
        nitrorna_work.chNo[1] = 5;
    }
    nitrorna_init_cnt++;
}

void NITRORNA_Finish(void) {
    int i;

    if (--nitrorna_init_cnt != 0) {
        return;
    }
    for (i = 0; i < NITRORNA_MAX_OBJ; i++) {
        if (nitrorna_work.obj[i].used == 1) {
            NITRORNA_Destroy(&nitrorna_work.obj[i]);
        }
    }
    func_0201dc90();
    RNAERR_Finish();
}

void func_0201c920(u8* chNo, int nch) {
    if (nch > 8) {
        return;
    }
    nitrorna_work.nch = nch;
    if (nch <= 0) {
        return;
    }
    if (chNo == NULL) {
        return;
    }
    memcpy(nitrorna_work.chNo, chNo, nch);
}

ADXRNA NITRORNA_Create(SJ* sji, int nch, void* work) {
    NITRORNA_OBJ* rna;
    NITROSND_CH*  ch;
    int           i;
    int           j;
    u8            chNo;

    if (nch <= 0) {
        return NULL;
    }
    if (sji == NULL) {
        return NULL;
    }
    for (j = 0; j < nch; j++) {
        if (sji[j] == NULL) {
            return NULL;
        }
    }
    for (i = 0; i < NITRORNA_MAX_OBJ; i++) {
        if (nitrorna_work.obj[i].used == 0) {
            break;
        }
    }
    if (i == NITRORNA_MAX_OBJ) {
        RNAERR_CallErrFunc("E04051004:Not enough RNA handle.\n");
        return NULL;
    }
    rna        = &nitrorna_work.obj[i];
    rna->nchIn = nch;
    rna->nch   = rna->nchIn;
    for (i = 0; i < rna->nch; i++) {
        rna->sji[i] = sji[i];
    }
    rna->vol = 0;
    for (i = 0; i < nch; i++) {
        chNo = NITRORNA_NO_CH;
        if (nitrorna_work.allocCh != NULL) {
            nitrorna_work.allocCh(&chNo);
        } else {
            for (j = 0; j < nitrorna_work.nch; j++) {
                if (nitrorna_work.chUsed[j] == 0) {
                    nitrorna_work.chUsed[j] = 1;
                    chNo                    = nitrorna_work.chNo[j];
                    break;
                }
            }
        }
        if (chNo == NITRORNA_NO_CH) {
            NITRORNA_Destroy(rna);
            return NULL;
        }
        ch = func_0201dcf4(i, work, chNo);
        if (ch == NULL) {
            NITRORNA_Destroy(rna);
            return NULL;
        }
        rna->ch[i]  = ch;
        rna->sjo[i] = SJRBF_Create(func_0201e06c(ch), func_0201e07c(rna->ch[i]) * 2, 0);
        if (rna->sjo[i] == NULL) {
            RNAERR_CallErrFunc("E04042701:Can't create SJ.\n");
            NITRORNA_Destroy(rna);
            return NULL;
        }
        rna->unk20[i] = 0;
    }
    NITRORNA_SetNumChan(rna, rna->nchIn);
    NITRORNA_SetSfreq(rna, NITROSND_UPSAMPLE_RATE);
    NITRORNA_SetBitPerSmpl(rna, 16);
    NITRORNA_SetOutVol(rna, 0);
    if (rna->nch == 2) {
        NITRORNA_SetOutPan(rna, 0, -15);
        NITRORNA_SetOutPan(rna, 1, 15);
    } else {
        NITRORNA_SetOutPan(rna, 0, 0);
    }
    rna->sw   = 0;
    rna->used = 1;
    rna->mono = 0;
    return rna;
}

void NITRORNA_Destroy(ADXRNA hndl) {
    NITRORNA_OBJ* rna = hndl;
    int           i;
    int           j;

    if (rna == NULL) {
        return;
    }
    NITRORNA_SetPlaySw(rna, 0);
    NITRORNA_SetTransSw(rna, 0);
    for (i = 0; i < rna->nch; i++) {
        if (rna->sjo[i] != NULL) {
            SJ_Destroy(rna->sjo[i]);
        }
        for (j = 0; j < nitrorna_work.nch; j++) {
            if (nitrorna_work.chNo[j] == rna->ch[i]->chNo) {
                nitrorna_work.chUsed[j] = 0;
                break;
            }
        }
        func_0201ddd8(rna->ch[i]);
    }
    rna->used = 0;
}

void func_0201ccd4(ADXRNA hndl) {}

void NITRORNA_SetTransSw(ADXRNA hndl, int sw) {
    NITRORNA_OBJ* rna = hndl;
    int           i;
    int           n;

    if (rna == NULL) {
        return;
    }
    if (sw == func_0201cd84(rna)) {
        return;
    }
    if (sw == 1) {
        RNACRS_Lock();
        if (NITRORNA_IS_DUAL_MONO(rna)) {
            n = 2;
        } else {
            n = rna->nchIn;
        }
        for (i = 0; i < n; i++) {
            func_0201e0a8(rna->ch[i]);
            rna->ch[i]->played = 0;
            func_0201de08(rna->ch[i]);
        }
        rna->sw |= 1;
        RNACRS_Unlock();
    } else if (sw == 0) {
        rna->sw ^= 1;
    }
}

int func_0201cd84(NITRORNA_OBJ* rna) {
    if (rna == NULL) {
        return -1;
    }
    return (rna->sw & 1) ? 1 : 0;
}

void NITRORNA_SetPlaySw(ADXRNA hndl, int sw) {
    NITRORNA_OBJ* rna = hndl;
    int           i;
    int           n;

    if (rna == NULL) {
        return;
    }
    if (sw == func_0201ce58(rna)) {
        return;
    }
    if (NITRORNA_IS_DUAL_MONO(rna)) {
        n = 2;
    } else {
        n = rna->nchIn;
    }
    if (sw == 1) {
        for (i = 0; i < n; i++) {
            func_0201e090(rna->ch[i]);
        }
        rna->sw |= 2;
    } else if (sw == 0) {
        for (i = 0; i < n; i++) {
            func_0201e0a8(rna->ch[i]);
        }
        rna->sw ^= 2;
    }
}

int func_0201ce58(NITRORNA_OBJ* rna) {
    if (rna == NULL) {
        return -1;
    }
    return (rna->sw & 2) ? 1 : 0;
}

void NITRORNA_GetTime(ADXRNA hndl, int* ncount, int* tscale) {
    NITRORNA_OBJ* rna = hndl;

    if (rna == NULL) {
        return;
    }
    *ncount = func_0201e9ec(rna->ch[0]);
    *tscale = rna->sfreq;
}

int NITRORNA_GetNumData(ADXRNA hndl) {
    NITRORNA_OBJ* rna = hndl;

    if (rna == NULL) {
        return -1;
    }
    return func_0201e9fc(rna->ch[0]);
}

int NITRORNA_GetNumRoom(ADXRNA hndl) {
    NITRORNA_OBJ* rna = hndl;

    if (rna == NULL) {
        return -1;
    }
    return func_0201ea0c(rna->ch[0]);
}

void func_0201cedc(NITRORNA_OBJ* rna) {
    SJCK ck[NITRORNA_MAX_CH];
    SJCK rest[NITRORNA_MAX_CH];
    u16  len;
    u16  len0;
    u16  len1;
    u32  n0;
    u32  n;
    int  nbyte0;
    int  nbyte1;
    u16  i;

    if (rna->nchIn == 1) {
        nbyte0 = SJ_GetNumData(rna->sji[0], 1);
        SJ_GetChunk(rna->sji[0], 1, nbyte0, &ck[0]);
#ifdef REGION_USA
        if (rna->nch == 2 && rna->mono == 1) {
            n0 = func_0201e1ac(rna->ch[0], &ck[0], &len0);
            n  = func_0201e1ac(rna->ch[1], &ck[0], &len1);
            if (n0 > n) {
                len = len1;
            } else {
                n   = n0;
                len = len0;
            }
            if (len != 0) {
                func_0201e328(rna->ch[0], (s16*)ck[0].data, len);
                func_0201e328(rna->ch[1], (s16*)ck[0].data, len);
            }
        } else {
            n = func_0201e1ac(rna->ch[0], &ck[0], &len);
            if (len != 0) {
                func_0201e328(rna->ch[0], (s16*)ck[0].data, len);
            }
        }
#else
        if (rna->nch == 1) {
            n = func_0201e1ac(rna->ch[0], &ck[0], &len);
            if (len != 0) {
                func_0201e328(rna->ch[0], (s16*)ck[0].data, len);
            }
        } else {
            n0 = func_0201e1ac(rna->ch[0], &ck[0], &len0);
            n  = func_0201e1ac(rna->ch[1], &ck[0], &len1);
            if (n0 > n) {
                len = len1;
            } else {
                n   = n0;
                len = len0;
            }
            if (len != 0) {
                func_0201e328(rna->ch[0], (s16*)ck[0].data, len);
                func_0201e328(rna->ch[1], (s16*)ck[0].data, len);
            }
        }
#endif
    } else {
        nbyte0 = SJ_GetNumData(rna->sji[0], 1);
        nbyte1 = SJ_GetNumData(rna->sji[1], 1);
        SJ_GetChunk(rna->sji[0], 1, nbyte0, &ck[0]);
        SJ_GetChunk(rna->sji[1], 1, nbyte1, &ck[1]);
        n0 = func_0201e1ac(rna->ch[0], &ck[0], &len0);
        n  = func_0201e1ac(rna->ch[1], &ck[1], &len1);
        if (n0 > n) {
            len = len1;
        } else {
            n   = n0;
            len = len0;
        }
        if (len != 0) {
            func_0201e328(rna->ch[0], (s16*)ck[0].data, len);
            func_0201e328(rna->ch[1], (s16*)ck[1].data, len);
        }
    }
    for (i = 0; i < rna->nchIn; i++) {
        if (n == ck[i].length) {
            SJ_PutChunk(rna->sji[i], 0, &ck[i]);
        } else if (n == 0) {
            SJ_UngetChunk(rna->sji[i], 1, &ck[i]);
        } else {
            SJ_SplitChunk(&ck[i], n, &ck[i], &rest[i]);
            SJ_PutChunk(rna->sji[i], 0, &ck[i]);
            SJ_UngetChunk(rna->sji[i], 1, &rest[i]);
        }
    }
}

void func_0201d178(NITRORNA_OBJ* rna) {
    int i;
    int n;

    if (NITRORNA_IS_DUAL_MONO(rna)) {
        n = 2;
    } else {
        n = rna->nchIn;
    }
    for (i = 0; i < n; i++) {
        func_0201e948(rna->ch[i]);
    }
}

void NITRORNA_ExecHndl(NITRORNA_OBJ* rna) {
    if (func_0201cd84(rna) == 1) {
        func_0201cedc(rna);
    } else if (func_0201ce58(rna) == 1) {
        func_0201d178(rna);
    }
}

void NITRORNA_ExecServer(void) {
    int i;

    for (i = 0; i < NITRORNA_MAX_OBJ; i++) {
        if (nitrorna_work.obj[i].used == 1) {
            NITRORNA_ExecHndl(&nitrorna_work.obj[i]);
        }
    }
}

void NITRORNA_SetNumChan(ADXRNA hndl, int nch) {
    NITRORNA_OBJ* rna = hndl;

    if (rna != NULL) {
        rna->nchIn = nch;
    }
}

void NITRORNA_SetSfreq(ADXRNA hndl, int sfreq) {
    NITRORNA_OBJ* rna = hndl;
    int           i;

    if (rna == NULL) {
        return;
    }
    if (sfreq != NITROSND_UPSAMPLE_RATE / 8 && sfreq != NITROSND_UPSAMPLE_RATE / 4 && sfreq != NITROSND_UPSAMPLE_RATE / 2 &&
        sfreq != NITROSND_UPSAMPLE_RATE && sfreq != NITROSND_UPSAMPLE_RATE * 3 / 8 && sfreq != NITROSND_UPSAMPLE_RATE * 3 / 4)
    {
        RNAERR_CallErrFunc("E06110200:Inaccurate frequency.\n");
    }
    for (i = 0; i < rna->nch; i++) {
        rna->sfreq = func_0201df8c(rna->ch[i], sfreq);
    }
}

void NITRORNA_SetOutVol(ADXRNA hndl, int vol) {
    NITRORNA_OBJ* rna = hndl;
    int           i;
    s16           v;

    if (rna == NULL) {
        return;
    }
    if (vol >= 0) {
        vol = 0;
    }
    v = vol;
    if (v <= -999) {
        v = -999;
    }
    rna->vol = v;
    for (i = 0; i < rna->nch; i++) {
        func_0201ea20(rna->ch[i], rna->vol);
    }
}

void NITRORNA_SetOutPan(ADXRNA hndl, int ch, int pan) {
    NITRORNA_OBJ* rna = hndl;
    s16           p;

    if (rna == NULL) {
        return;
    }
    if (ch >= rna->nch) {
        return;
    }
    if (pan >= 15) {
        pan = 15;
    }
    p = pan;
    if (p <= -15) {
        p = -15;
    }
    rna->pan[ch] = p;
    if (NITRORNA_IS_DUAL_MONO(rna)) {
        func_0201ea50(rna->ch[0], -15);
        func_0201ea50(rna->ch[1], 15);
    } else {
        func_0201ea50(rna->ch[ch], rna->pan[ch]);
    }
}

void NITRORNA_SetBitPerSmpl(ADXRNA hndl, int bps) {
    NITRORNA_OBJ* rna = hndl;

    if (rna != NULL) {
        rna->bps = bps;
    }
}

void func_0201d3c0(ADXT adxt, int mono) {
    ((NITRORNA_OBJ*)adxt->rna)->mono = mono;
}

const char* nitrorna_build_ptr = NULL;
