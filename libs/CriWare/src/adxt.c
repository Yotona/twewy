#include <adxt.h>

static ADXT* adxt_Create(s32 maxChans, void* work, s32 worksize);
static ADXT* func_02016c90(void* work, s32 workSize);
static void  adxt_Destroy(ADXT* adxt);
static void  adxt_DestroyAll(void);
static void  adxt_StartMem2(ADXT* adxt, void* adxData, s32 dataLength);
static void  adxt_StartMemIdx(ADXT* adxt, void* acxData, s32 idx);
static void  adxt_StartAfs(ADXT* adxt, s32 partitionId, s32 fileId);
static void  adxt_SetLpFlg(ADXT* adxt, s32 flag);
static void  adxt_SetOutPan(ADXT* adxt, s32 channel, s32 pan);
static s32   adxt_GetStat(ADXT* adxt);
static s32   adxt_GetNumChan();
static void  adxt_SetOutVol(ADXT* adxt, s32 vol);
static s32   adxt_GetOutVol(ADXT* adxt);
static void  adxt_Pause(ADXT* adxt, s32 pauseState);
static s32   adxt_GetStatPause(ADXT* adxt);
static void  adxt_Stop(ADXT* adxt);
static void  adxt_ExecHndl(ADXT* adxt);

void func_02017da4(ADXT* adxt, const char* filename);

extern ADXT data_0206bd94[4];
extern void (*data_0206c7c8)(void);
extern s32 data_0206c7c0;
extern s32 data_0206bd80;
extern s32 data_02063ae8;
extern s32 data_0206bd7c;

void func_02012f88();
void func_02012f8c();
void func_0201433c(const char*);
void func_02049118(void*, int, int);

void func_020168f4(void) {
    func_02012f88();
}

void func_02016900(void) {
    func_02012f8c();
}

ADXT* ADXT_Create(s32 maxChans, void* work, s32 workSize) {
    ADXT* padxt;

    func_02012f88();
    padxt = adxt_Create(maxChans, work, workSize);
    func_02012f8c();
    return padxt;
}

static ADXT* adxt_Create(s32 maxChans, void* work, s32 workSize) {

    if (maxChans < 0 || work == NULL || workSize < 0) {
        func_0201433c("E02080804 adxt_Create: parameter error");
        return NULL;
    }

    func_0201433c("E03100801 adxt_Create: Too many handles.");

    func_0201433c("E06022201 adxt_Create: Work size is too short.");
    return NULL;
}

ADXT* func_02016c64(void* work, s32 workSize) {
    ADXT* adxt;

    func_02012f88();
    adxt = func_02016c90(work, workSize);
    func_02012f8c();
    return adxt;
}

ADXT* func_02016c90(void* work, s32 workSize) {
    ADXT* adxt = adxt_Create(1, work, workSize);
    if (adxt == NULL) {
        adxt = NULL;
    }
    return adxt;
}

void func_02016cb0(void) {
    if (data_0206c7c8 != NULL) {
        data_0206c7c8();
    }
}

void ADXT_Destroy(ADXT* adxt) {
    func_02012f88();
    adxt_Destroy(adxt);
    func_02012f8c();
}

void adxt_Destroy(ADXT* adxt) {
    SJ*   pSVar3;
    void* handle;
    int   channel;

    if (adxt == NULL) {
        func_0201433c("E02080805 adxt_Destroy: parameter error");
        return;
    }
    func_02016cb0();

    if (adxt->used == TRUE) {
        adxt_Stop(adxt);
    }

    handle = adxt->audioRenderer;
    if (handle != NULL) {
        adxt->audioRenderer = NULL;
        func_0201bed0(handle, 0);
    }

    handle = adxt->streamDecoder;
    if (handle != NULL) {
        adxt->streamDecoder = NULL;
        ADXSJD_Destroy(handle);
    }

    handle = adxt->streamController;
    if (handle != NULL) {
        adxt->streamController = NULL;
        func_02016104(handle, 0, 0);
        func_02015dc4(handle);
    }

    handle = adxt->loopStreamController;
    if (handle != NULL) {
        adxt->loopStreamController = NULL;
        func_020215e4(handle);
    }

    func_02012f70();

    handle = adxt->fileInputStream;
    if (handle != NULL) {
        adxt->fileInputStream = NULL;
        SJ_Destroy((SJ*)handle);
    }

    for (channel = 0; channel < adxt->maxChannels; channel++) {
        handle = adxt->outputStream[channel];
        if (handle != NULL) {
            adxt->outputStream[channel] = NULL;
            SJ_Destroy((SJ*)handle);
        }

        handle = adxt->ampInputStream[channel];
        if (handle != NULL) {
            adxt->ampInputStream[channel] = NULL;
            SJ_Destroy((SJ*)handle);
        }

        handle = adxt->ampOutputStream[channel];
        if (handle != NULL) {
            adxt->ampOutputStream[channel] = NULL;
            SJ_Destroy((SJ*)handle);
        }
    }

    handle = adxt->amp;
    if (handle != NULL) {
        adxt->amp = NULL;
        func_020215c0(handle);
    }

    func_02049118(adxt, 0, sizeof(ADXT));
    adxt->used = 0;
    func_02012f7c();
}

void ADXT_DestroyAll(void) {
    func_02012f88();
    adxt_DestroyAll();
    func_02012f8c();
}

void adxt_DestroyAll(void) {
    s32 idx;

    for (idx = 0; idx < 4; idx++) {
        if (data_0206bd94[idx].used == TRUE) {
            adxt_Destroy(&data_0206bd94[idx]);
        }
    }
}

void func_02016ec4(ADXT* adxt, SJ* sj) {
    s32 idx;

    for (idx = 0; idx < adxt->maxChannels; idx++) {
        SJ_Reset(adxt->outputStream[idx]);
    }

    func_02014b28(adxt->streamDecoder, sj);
    adxt->inputStream = sj;
    func_02014b60(adxt->streamDecoder);
    adxt->stat                = 1;
    adxt->loopCounter         = 0;
    adxt->readyFlag           = 0;
    adxt->loopPlaybackEndSect = 0x7FFFFFFF;
    adxt->trapSample          = -1;
    adxt->startTimeOffset     = 0;
    adxt->decodeOffset        = 0;
    adxt->vsyncCount          = data_0206bd80;
    adxt->loopDecodeLength    = 0;
    if (adxt->playbackMode == ADXT_PLAYBACK_SLFILE) {
        ADXT_SetLpFlg(adxt, 0);
    }
    if (adxt->amp != NULL) {
        func_020215b4();
    }
}

void func_02016f84(ADXT* adxt, s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5) {
    func_0201688c(adxt->streamController, adxt->inputBufMinSectors << 0xb, adxt->inputBufMaxSectors << 0xb);
    func_0201613c(adxt->streamController, data_02063ae8);
    func_02016104(adxt->streamController, 0, 0);
    func_02015f88(adxt->streamController, 0);
    func_02016058(adxt->streamController);
    func_02015ec8(adxt->streamController);
    func_02015e10(adxt->streamController, param_1, param_2, param_3, param_4 << 0xb, param_5 >> 0x1f << 0xb | param_5 >> 0x15);
    func_02016008(adxt->streamController);
    func_02016ec4(adxt, adxt->fileInputStream);
}

void func_02017040(ADXT* adxt) {
    SJ* sj;

    func_02012f70();
    func_0201bee8(adxt->audioRenderer, 0);
    func_0201bef4(adxt->audioRenderer, 0);
    func_02014b78(adxt->streamDecoder);
    if (adxt->playbackMode == ADXT_PLAYBACK_MEM) {
        sj = adxt->inputStream;
        if (sj != NULL) {
            adxt->inputStream = NULL;
            SJ_Destroy(sj);
        }
    }

    if (adxt->amp != NULL) {
        func_020215b8();
    }
    adxt->inputStream     = NULL;
    adxt->stat            = 0;
    adxt->streamStartFlag = 0;
    func_02012f7c();
}

void ADXT_Stop(ADXT* adxt) {
    func_02012f88();
    adxt_Stop(adxt);
    func_02012f8c();
}

void adxt_Stop(ADXT* adxt) {
    SJ* sj;

    if (adxt == NULL) {
        func_0201433c("E02080813 adxt_Stop: parameter error");
        return;
    }
    if (adxt->streamController != NULL) {
        func_02015ec8(adxt->streamController);
    }
    if (adxt->playbackMode == ADXT_PLAYBACK_SLFILE) {
        func_020215f4(adxt->loopStreamController);
        sj = adxt->inputStream;
        if (adxt->inputStream != NULL) {
            SJ_Reset(adxt->inputStream);
        }
    }
    func_02017040(adxt);
}

s32 ADXT_GetStat(ADXT* adxt) {
    s32 stat;

    func_02012f88();
    stat = adxt_GetStat(adxt);
    func_02012f8c();
    return stat;
}

s32 adxt_GetStat(ADXT* adxt) {
    if (adxt == NULL) {
        func_0201433c("E02080814 adxt_GetStat: parameter error");
        return -1;
    }
    return adxt->stat;
}

void func_02017188() {
    /* NYI */
}

void adxt_GetTime(ADXT* adxt, s32* count, s32* frequency) {
    if (adxt == NULL || count == NULL || frequency == NULL) {
        func_0201433c("E02080815 adxt_GetTime: parameter error");
        return;
    }
    /* NYI */
}

s32 ADXT_Play(ADXT* adxt) {
    s32 val;

    func_02012f88();
    val = func_020174c8(adxt);
    func_02012f8c();
    return val;
}

s32 func_020174c8(ADXT* adxt) {
    u32 uVar1;
    s32 local_c;
    s32 local_10;

    adxt_GetTime(adxt, &local_c, &local_10);
    uVar1 = func_02056078(100.0f, func_02055b38(func_02055fe8(local_c), func_02055fe8(local_10)));
    return func_02055f74(uVar1);
}

s32 ADXT_GetNumChan(ADXT* adxt) {
    s32 numChan;

    func_02012f88();
    numChan = adxt_GetNumChan(adxt);
    func_02012f8c();
    return numChan;
}

s32 adxt_GetNumChan(ADXT* adxt) {
    if (adxt == NULL) {
        func_0201433c("E02080820 adxt_GetNumChan: parameter error");
        return -1;
    }
    if (adxt->stat >= ADXT_STAT_PREPPING) {
        return func_02015948(adxt->streamDecoder);
    }
    return 0;
}

void ADXT_SetOutPan(ADXT* adxt, s32 channel, s32 pan) {
    func_02012f88();
    adxt_SetOutPan(adxt, channel, pan);
    func_02012f8c();
}

void adxt_SetOutPan(ADXT* adxt, s32 channel, s32 pan) {
    int iVar1;
    int iVar2;

    if (adxt == NULL) {
        func_0201433c("E02080825 adxt_SetOutPan: Null pointer is passed");
        return;
    }

    if (channel != 0 && channel != 1) {
        func_0201433c("E07041801 adxt_SetOutPan: parameter error");
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
        iVar1 = func_02015a2c(adxt->streamDecoder, channel);
        if (iVar1 == -128) {
            iVar1 = 0;
        }
    } else {
        iVar1 = 0;
    }

    if (data_0206bd7c == 0) {
        if (pan == -128) {
            iVar2 = func_02015948((int)adxt->streamDecoder);
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
    adxt->outPan[channel] = pan;
    if (channel < adxt->maxChannels) {
        func_0201bf54((int)adxt->audioRenderer, channel, iVar2);
    } else {
        func_0201433c("E8101208 adxt_SetOutPan: parameter error");
    }
}

void ADXT_SetOutVol(ADXT* adxt, s32 vol) {
    func_02012f88();
    adxt_SetOutVol(adxt, vol);
    func_02012f8c();
}

void adxt_SetOutVol(ADXT* adxt, s32 vol) {
    s32 amp;

    if (adxt == NULL) {
        func_0201433c("E02080823 adxt_SetOutVol: parameter error");
        return;
    }
    adxt->outVol = vol;
    if (adxt->extraInfoFlag == 1) {
        amp = func_020159e4(adxt->streamDecoder);
    } else {
        amp = 0;
    }
    func_0201bf48(adxt->audioRenderer, adxt->outVol + amp);
}

s32 ADXT_GetOutVol(ADXT* adxt) {
    s32 outVol;

    func_02012f88();
    outVol = adxt_GetOutVol(adxt);
    func_02012f8c();
    return outVol;
}

s32 adxt_GetOutVol(ADXT* adxt) {
    if (adxt == NULL) {
        func_0201433c("E02080824 adxt_GetOutVol: parameter error");
        return 0;
    }
    return adxt->outVol;
}

void ADXT_SetExtraInfo(ADXT* adxt, s8 flag) {
    adxt->extraInfoFlag = flag;
}

void func_02017788(ADXT* adxt) {
    func_02012f88();
    func_020177a4(adxt);
    func_02012f8c();
}

void func_020177a4(ADXT* adxt) {
    /* NYI */
}

void func_020177b8() {
    func_02012f88();
    func_020177cc();
    func_02012f8c();
}

void func_020177cc() {
    /* NYI */
}

void ADXT_SetLpFlg(ADXT* adxt, s32 flag) {
    func_02012f88();
    adxt_SetLpFlg(adxt, flag);
    func_02012f8c();
}

void adxt_SetLpFlg(ADXT* adxt, s32 flag) {
    if (adxt == NULL) {
        func_0201433c("E02080828 adxt_SetLpFlg: parameter error");
        return;
    }
    /* NYI */
}

void ADXT_Pause(ADXT* adxt, s32 pauseState) {
    func_02012f88();
    adxt_Pause(adxt, pauseState);
    func_02012f8c();
}

void adxt_Pause(ADXT* adxt, s32 pauseState) {
    if (adxt == NULL) {
        func_0201433c("E02080846 adxt_Pause: parameter error");
        return;
    }
    /* NYI */
}

s32 ADXT_GetStatPause(ADXT* adxt) {
    s32 statPause;

    func_02012f88();
    statPause = adxt_GetStatPause(adxt);
    func_02012f8c();
    return statPause;
}

s32 adxt_GetStatPause(ADXT* adxt) {
    if (adxt == NULL) {
        func_0201433c("E02080847 adxt_GetStatPause: parameter error");
        return 0;
    }
    return adxt->pauseFlag;
}

void func_02017b20(ADXT* adxt, s32 param_1, s32 param_2) {
    func_02012f88();
    func_02017b78(adxt, param_1, param_2);
    func_02012f8c();
}

void func_02017b4c(ADXT* adxt, s32 param_1, s32 param_2) {
    func_02012f88();
    func_02017b7c(adxt, param_1, param_2);
    func_02012f8c();
}

void func_02017b78(ADXT* adxt, s32 param_1, s32 param_2) {
    return;
}

void func_02017b7c(ADXT* adxt, s32 param_1, s32 param_2) {
    return;
}

void func_02017b80(ADXT* adxt, s32 param_1) {
    func_02012f88();
    func_02017ba4(adxt, param_1);
    func_02012f8c();
}

void func_02017ba4(ADXT* adxt, s32 param_1) {
    adxt->playbackFlag = param_1;
    if (adxt->streamDecoder != NULL) {
        func_020158fc(adxt->streamDecoder, param_1);
    }
}

void ADXT_StartAfs(ADXT* adxt, s32 partitionId, s32 fileId) {
    func_02012f88();
    adxt_StartAfs(adxt, partitionId, fileId);
    func_02012f8c();
}

void adxt_StartAfs(ADXT* adxt, s32 partitionId, s32 fileId) {
    if (adxt == NULL) {
        func_0201433c("E02080811 adxt_StartAfs: parameter error");
        return;
    }
    func_02014398("E8101202 adxt_StartAfs: can\'t open ");
    /* NYI */
}

void ADXT_StartFnameRange(ADXT* adxt, const char* filename, s32 sectOffset, s32 sectRange) {
    /* NYI */
}

void adxt_StartFnameRange(ADXT* adxt, const char* filename, s32 sectOffset, s32 sectRange) {
    if (adxt == NULL || filename == NULL) {
        func_0201433c("E02080807 adxt_StartFnameRange: parameter error");
        return;
    }
    /* NYI */
}

void func_02017d80(ADXT* adxt, const char* filename) {
    func_02012f88();
    func_02017da4(adxt, filename);
    func_02012f8c();
}

void func_02017da4(ADXT* adxt, const char* filename) {
    adxt_StartFnameRange(adxt, filename, 0, 0xFFFFF);
}

void ADXT_StartMem2(ADXT* adxt, void* adxData, s32 dataLength) {
    func_02012f88();
    adxt_StartMem2(adxt, adxData, dataLength);
    func_02012f8c();
}

void adxt_StartMem2(ADXT* adxt, void* adxData, s32 dataLength) {
    void* puVar1;

    if (adxt == NULL || adxData == NULL || dataLength < 0) {
        func_0201433c("E02080809 adxt_StartMem2: parameter error");
        return;
    }

    ADXT_Stop(adxt);
    func_02012f70();
    puVar1 = func_02019df0(adxData, dataLength);
    if (puVar1 == NULL) {
        func_02012f7c();
        func_0201433c("E8101207: can\'t create sj (adxt_StartMem)");
        return;
    }
    adxt->playbackMode = ADXT_PLAYBACK_MEM;
    func_02016ec4(adxt, puVar1);
    func_02017b80(adxt, 0);
    func_02012f7c();
}

void ADXT_StartMemIdx(ADXT* adxt, void* acxData, s32 idx) {
    func_02012f88();
    adxt_StartMemIdx(adxt, acxData, idx);
    func_02012f8c();
}

void adxt_StartMemIdx(ADXT* adxt, void* acxData, s32 idx) {
    if ((adxt == NULL || acxData == NULL) || (idx < 0)) {
        func_0201433c("E02080810 adxt_StartMemIdx: parameter error");
        return;
    }
    func_0201433c("E8101207: can\'t create sj (adxt_StartMemIdx)");
    /* NYI */
}

void func_02017f80() {
    /* NYI */
}

void func_02018004() {
    func_0201433c("E8101201 adxt_trap_entry: not enough data");
    /* NYI */
}

void func_02018168() {
    /* NYI */
}

void func_020181f4(ADXT* adxt) {
    if (func_02015948(adxt->streamDecoder) == 1) {
        ADXT_SetOutPan(adxt, 0, adxt->outPan[0]);
    } else {
        ADXT_SetOutPan(adxt, 0, adxt->outPan[0]);
        ADXT_SetOutPan(adxt, 1, adxt->outPan[1]);
    }
}

void func_02018238() {
    /* NYI */
}

void func_0201854c() {
    func_02014398("E9081001 adxt_stat_decinfo: can\'t play this number of channels");
    /* NYI */
}

void func_02018910() {
    /* NYI */
}

void func_02018a64() {
    /* NYI */
}

void func_02018b30() {
    /* NYI */
}

void func_02018b74() {
    return;
}

void func_02018b78(ADXT* adxt) {
    int iVar1;

    if (adxt->streamController != NULL && func_02015f5c(adxt->streamController) == 4) {
        adxt->errorCode = ADXT_ERR_BUFF;
        adxt->stat      = ADXT_STAT_ERROR;
    }
    if (adxt->loopStreamController == 0) {
        return;
    }
    if (func_020215ec() == 3) {
        adxt->errorCode = ADXT_ERR_BUFF;
        adxt->stat      = ADXT_STAT_ERROR;
    }
}

void func_02018bd4(ADXT* adxt) {
    if (adxt->streamController == NULL) {
        return;
    }

    if (ADXT_GetStat(adxt) != 0) {
        switch (adxt->playbackMode) {
            case ADXT_PLAYBACK_FILENAME:
            case ADXT_PLAYBACK_AFS:
                if (func_02015f5c(adxt->streamController) == 3) {
                    func_02014b50(adxt->streamDecoder);
                }
                return;
            case ADXT_PLAYBACK_MEM:
                func_02014b50(adxt->streamDecoder);
                return;
            case ADXT_PLAYBACK_STREAM:
            case ADXT_PLAYBACK_SLFILE:
            default:
                return;
        }
    }
}

void func_02018c40(ADXT* adxt) {
    func_02012f88();
    adxt_ExecHndl(adxt);
    func_02012f8c();
}

void adxt_ExecHndl(ADXT* adxt) {
    if (adxt == NULL) {
        func_0201433c("E02080842 adxt_ExecHndl: parameter error");
        return;
    }

    if (adxt->stat == ADXT_STAT_PLAYING) {
        func_02018a64();
    } else if (adxt->stat == ADXT_STAT_LOADING) {
        func_0201854c();
    } else if (adxt->stat == ADXT_STAT_PREPPING) {
        func_02018910();
    } else if (adxt->stat == ADXT_STAT_DECEND) {
        func_02018b30();
    } else if (adxt->stat == ADXT_STAT_PLAYEND) {
        func_02018b74();
    }
    func_02018bd4(adxt);
    func_02018b78(adxt);
}
