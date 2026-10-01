#include <cri/adx_inis.h>
#include <cri/adxt.h>
#include <cri/lsc.h>

#include <cri/private/adx_tlk.h>

int   adxt_tlk2_unused1 = 0;
int   adxt_tlk2_unused0 = 0;
int   adxt_time_unit    = 0;
float adxt_diff_av      = 0.0f;
int   adxt_tlk2_unused2 = 0;

static void adxt_StartAfs(ADXT adxt, int partitionId, int fileId);
void        adxt_StartFname(ADXT adxt, const char* filename);
static void adxt_StartMem2(ADXT adxt, void* adxData, int dataLength);
static void adxt_StartMemIdx(ADXT adxt, void* acxData, int idx);
static void adxt_StartFnameRange(ADXT adxt, const char* filename, int sectOffset, int sectRange);

void ADXT_StartAfs(ADXT adxt, int partitionId, int fileId) {
    ADXCRS_Enter();
    adxt_StartAfs(adxt, partitionId, fileId);
    ADXCRS_Leave();
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
    ADXCRS_Enter();
    adxt_StartFnameRange(adxt, filename, sectOffset, sectRange);
    ADXCRS_Leave();
}

static void adxt_StartFnameRange(ADXT adxt, const char* filename, int sectOffset, int sectRange) {
    if (adxt == NULL || filename == NULL) {
        ADXERR_CallErrFunc1("E02080807 adxt_StartFnameRange: parameter error");
        return;
    }

    ADXT_Stop(adxt);
    CRICRW_Strcpy(adxt->workFilename, 0x100, filename);
    adxt->filename        = adxt->workFilename;
    adxt->directory       = NULL;
    adxt->offset          = sectOffset;
    adxt->range           = sectRange;
    adxt->stat            = ADXT_STAT_LOADING;
    adxt->streamStartFlag = 1;
    adxt->pmode           = ADXT_PLAYBACK_FILENAME;
    ADXT_SetLnkSw(adxt, 0);
}

void ADXT_StartFname(ADXT adxt, const char* filename) {
    ADXCRS_Enter();
    adxt_StartFname(adxt, filename);
    ADXCRS_Leave();
}

void adxt_StartFname(ADXT adxt, const char* filename) {
    adxt_StartFnameRange(adxt, filename, 0, 0xFFFFF);
}

void ADXT_StartMem2(ADXT adxt, void* adxData, int dataLength) {
    ADXCRS_Enter();
    adxt_StartMem2(adxt, adxData, dataLength);
    ADXCRS_Leave();
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
    ADXCRS_Enter();
    adxt_StartMemIdx(adxt, acxData, idx);
    ADXCRS_Leave();
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