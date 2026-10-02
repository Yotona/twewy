#include <cri/private/adx_rna.h>

void   NITRORNA_Init(void);
void   NITRORNA_Finish(void);
void   NITRORNA_EntryErrFunc(void (*func)(void* obj, const char* msg), void* obj);
ADXRNA NITRORNA_Create(void* sjo, int maxnch, int size);
void   NITRORNA_Destroy(ADXRNA rna);
void   func_0201ccd4(ADXRNA rna);
void   NITRORNA_SetTransSw(ADXRNA rna, int sw);
void   NITRORNA_SetPlaySw(ADXRNA rna, int sw);
void   NITRORNA_GetTime(ADXRNA rna, int* ncount, int* tscale);
int    NITRORNA_GetNumData(ADXRNA rna);
int    NITRORNA_GetNumRoom(ADXRNA rna);
void   NITRORNA_ExecServer(void);
void   NITRORNA_SetNumChan(ADXRNA rna, int nch);
void   NITRORNA_SetSfreq(ADXRNA rna, int sfreq);
void   NITRORNA_SetOutVol(ADXRNA rna, int vol);
void   NITRORNA_SetOutPan(ADXRNA rna, int ch, int pan);
void   NITRORNA_SetBitPerSmpl(ADXRNA rna, int bps);

void ADXRNA_Init(void) {
    NITRORNA_Init();
}

void ADXRNA_Finish(void) {
    NITRORNA_Finish();
}

void ADXRNA_EntryErrFunc(void (*func)(void* obj, const char* msg), void* obj) {
    NITRORNA_EntryErrFunc(func, obj);
}

ADXRNA ADXRNA_Create(void* sjo, int maxnch, int size) {
    return NITRORNA_Create(sjo, maxnch, size);
}

void ADXRNA_Destroy(ADXRNA rna) {
    func_0201ccd4(rna);
    NITRORNA_Destroy(rna);
}

void ADXRNA_SetTransSw(ADXRNA rna, int sw) {
    NITRORNA_SetTransSw(rna, sw);
}

void ADXRNA_SetPlaySw(ADXRNA rna, int sw) {
    NITRORNA_SetPlaySw(rna, sw);
}

void ADXRNA_GetTime(ADXRNA rna, int* ncount, int* tscale) {
    NITRORNA_GetTime(rna, ncount, tscale);
}

int ADXRNA_GetNumData(ADXRNA rna) {
    return NITRORNA_GetNumData(rna);
}

int ADXRNA_GetNumRoom(ADXRNA rna) {
    return NITRORNA_GetNumRoom(rna);
}

void ADXRNA_ExecServer(void) {
    NITRORNA_ExecServer();
}

void ADXRNA_SetNumChan(ADXRNA rna, int nch) {
    NITRORNA_SetNumChan(rna, nch);
}

void ADXRNA_SetSfreq(ADXRNA rna, int sfreq) {
    NITRORNA_SetSfreq(rna, sfreq);
}

void ADXRNA_SetOutVol(ADXRNA rna, int vol) {
    NITRORNA_SetOutVol(rna, vol);
}

void ADXRNA_SetOutPan(ADXRNA rna, int ch, int pan) {
    NITRORNA_SetOutPan(rna, ch, pan);
}

void ADXRNA_SetBitPerSmpl(ADXRNA rna, int bps) {
    NITRORNA_SetBitPerSmpl(rna, bps);
}

void ADXRNA_SetTotalNumSmpl(ADXRNA rna, int nsmpl) {
    return; // Do nothing
}

int ADXRNA_SetStmHdInfo(ADXRNA rna, void* info) {
    return 0;
}
