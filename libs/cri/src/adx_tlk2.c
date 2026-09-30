#include <cri/adx_inis.h>
#include <cri/adxt.h>
#include <cri/lsc.h>

#include <cri/private/adx_tlk.h>

static void adxt_StartAfs(ADXT adxt, int partitionId, int fileId);
void        func_02017da4(ADXT adxt, const char* filename);
static void adxt_StartMem2(ADXT adxt, void* adxData, int dataLength);
static void adxt_StartMemIdx(ADXT adxt, void* acxData, int idx);
static void adxt_StartFnameRange(ADXT adxt, const char* filename, int sectOffset, int sectRange);

void ADXT_StartAfs(ADXT adxt, int partitionId, int fileId) {
    func_02012f88();
    adxt_StartAfs(adxt, partitionId, fileId);
    func_02012f8c();
}

void adxt_StartAfs(ADXT adxt, int partitionId, int fileId) {
    char  error[16];
    int   ofst;
    int   fnsct;
    void* dir;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080811 adxt_StartAfs: parameter error");
        return;
    }

    ADXT_Stop(adxt);

    if (ADXF_GetFnameRangeEx(partitionId, fileId, adxt->workFilename, &dir, &ofst, &fnsct) != 0) {
        return;
    }

    if (adxt->stm == NULL) {
        ADXERR_ItoA2(partitionId, fileId, &error, 16);
        ADXERR_CallErrFunc2("E8101202 adxt_StartAfs: can\'t open ", error);
        adxt->ercode = ADXT_ERR_BUFF;
        adxt->stat   = ADXT_STAT_ERROR;
        return;
    }

    adxt->filename        = adxt->workFilename;
    adxt->directory       = dir;
    adxt->offset          = ofst;
    adxt->range           = fnsct;
    adxt->stat            = ADXT_STAT_LOADING;
    adxt->streamStartFlag = 1;
    adxt->pmode           = ADXT_PLAYBACK_AFS;
    ADXT_SetLnkSw(adxt, 0);
}

void ADXT_StartFnameRange(ADXT adxt, const char* filename, int sectOffset, int sectRange) {
    func_02012f88();
    adxt_StartFnameRange(adxt, filename, sectOffset, sectRange);
    func_02012f8c();
}

static void adxt_StartFnameRange(ADXT adxt, const char* filename, int sectOffset, int sectRange) {
    if (adxt == NULL || filename == NULL) {
        ADXERR_CallErrFunc1("E02080807 adxt_StartFnameRange: parameter error");
        return;
    }

    ADXT_Stop(adxt);
    func_02021614(adxt->workFilename, 0x100, filename);
    adxt->filename        = adxt->workFilename;
    adxt->directory       = NULL;
    adxt->offset          = sectOffset;
    adxt->range           = sectRange;
    adxt->stat            = ADXT_STAT_LOADING;
    adxt->streamStartFlag = 1;
    adxt->pmode           = ADXT_PLAYBACK_FILENAME;
    ADXT_SetLnkSw(adxt, 0);
}

void func_02017d80(ADXT adxt, const char* filename) {
    func_02012f88();
    func_02017da4(adxt, filename);
    func_02012f8c();
}

void func_02017da4(ADXT adxt, const char* filename) {
    adxt_StartFnameRange(adxt, filename, 0, 0xFFFFF);
}

void ADXT_StartMem2(ADXT adxt, void* adxData, int dataLength) {
    func_02012f88();
    adxt_StartMem2(adxt, adxData, dataLength);
    func_02012f8c();
}

void adxt_StartMem2(ADXT adxt, void* adxData, int dataLength) {
    if (adxt == NULL || adxData == NULL || dataLength < 0) {
        ADXERR_CallErrFunc1("E02080809 adxt_StartMem2: parameter error");
        return;
    }

    ADXT_Stop(adxt);
    ADXCRS_Lock();
    SJ sj = SJMEM_Create(adxData, dataLength);
    if (sj == NULL) {
        ADXCRS_Unlock();
        ADXERR_CallErrFunc1("E8101207: can\'t create sj (adxt_StartMem)");
        return;
    }
    adxt->pmode = ADXT_PLAYBACK_MEM;
    adxt_start_sj(adxt, sj);
    ADXT_SetLnkSw(adxt, 0);
    ADXCRS_Unlock();
}

void ADXT_StartMemIdx(ADXT adxt, void* acxData, int idx) {
    func_02012f88();
    adxt_StartMemIdx(adxt, acxData, idx);
    func_02012f8c();
}

void adxt_StartMemIdx(ADXT adxt, void* acxData, int idx) {
    if (adxt == NULL || acxData == NULL || idx < 0) {
        ADXERR_CallErrFunc1("E02080810 adxt_StartMemIdx: parameter error");
        return;
    }
    ADXT_Stop(adxt);

    int dataVal = *(unsigned int*)(acxData + 4);
    if (idx >= (int)(((dataVal << 8) & 0xFF0000) | ((unsigned char)(dataVal >> 0x18) | ((dataVal >> 8) & 0xFF00)) |
                     (dataVal << 0x18)))
    {
        return;
    }
    if (idx < 0) {
        return;
    }
    ADXCRS_Lock();

    int   temp2   = *(int*)((acxData + (idx * 8)) + 8);
    void* temp_r0 = SJMEM_Create(
        acxData + (((temp2 << 8) & 0xFF0000) | ((unsigned char)(temp2 >> 0x18) | ((temp2 >> 8) & 0xFF00)) | (temp2 << 0x18)),
        0x40000000);

    if (temp_r0 == NULL) {
        ADXCRS_Unlock();
        ADXERR_CallErrFunc1("E8101207: can\'t create sj (adxt_StartMemIdx)");
        return;
    }

    adxt->pmode = ADXT_PLAYBACK_MEM;
    adxt_start_sj(adxt, temp_r0);
    ADXT_SetLnkSw(adxt, 0);
    ADXCRS_Unlock();
}