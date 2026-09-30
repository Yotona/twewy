#ifndef ACSSBADX_H
#define ACSSBADX_H

#include <cri/adxt.h>
#include <cri/cri_xpt.h>

/* Operation Status */
#define ACSSBADX_STAT_STOPPED 0 // Standby
#define ACSSBADX_STAT_LOADING 1 // Retrieving header information
#define ACSSBADX_STAT_PLAYING 2 // Decoding/Playing
#define ACSSBADX_STAT_ENDING  3 // Decoding/Playing ended
#define ACSSBADX_STAT_ERROR   4 // Error occurred

typedef struct {
    /* 0x0 */ struct _acssbadx_vtable* vtable;
    /* 0x4 */ ADXT                     adxt;
    /* 0x8 */ int                      unk_8;
} ACSSBADX; // Size: 0xC

typedef struct _acssbadx_vtable {
    int (*CreateHandle)(ACSSBADX* badx, int*, void* work, int workSize);
    void (*DestroyAdxt)(ACSSBADX* badx);
    void (*unkFunc)(ACSSBADX* badx, const char* filename);
    void (*StartFnameRange)(ACSSBADX* badx);
    void (*StartAfs)(ACSSBADX* badx, int, int);
    void (*StartMem2)(ACSSBADX* badx, int, int);
    void (*StartMemIdx)(ACSSBADX* badx, int, int);
    void (*Stop)(ACSSBADX* badx);
    void (*Pause)(ACSSBADX* badx);
    int (*GetStatPause)(ACSSBADX* badx);
    int (*GetStat)(ACSSBADX* badx);
    void (*SetOutVol)(ACSSBADX* badx, short vol);
    int (*GetOutVol)(ACSSBADX* badx);
    void (*SetOutVol2)(ACSSBADX* badx, int, short vol);
    int (*GetOutVol2)(ACSSBADX* badx, int);
    void (*SetLpFlg)(ACSSBADX* badx, int flag);
    ADXT (*GetAdxt)(ACSSBADX* badx);
} _acssbadx_vtable;

ACSSBADX* ACSSBADX_Create(ACSSBADX* badx, unsigned short workSize);

int ACSSBADX_CreateHndl(ACSSBADX* badx, int* param_2, void* work, int workSize);

void ACSSBADX_DestroyAdxt(ACSSBADX* badx);

void func_02021004(ACSSBADX* badx, const char* filename);

void ACSSBADX_StartFnameRange(ACSSBADX* badx);

void ACSSBADX_StartAfs(ACSSBADX* badx, int param_1, int param_2);

void ACSSBADX_StartMem2(ACSSBADX* badx, int param_1, int param_2);

void ACSSBADX_StartMemIdx(ACSSBADX* badx, int param_1, int param_2);

void ACSSBADX_Stop(ACSSBADX* badx);

void ACSSBADX_Pause(ACSSBADX* badx);

int ACSSBADX_GetStatPause(ACSSBADX* badx);

int ACSSBADX_GetStat(ACSSBADX* badx);

void ACSSBADX_SetOutVol(ACSSBADX* badx, short vol);

int ACSSBADX_GetOutVol(ACSSBADX* badx);

void ACSSBADX_SetOutVol2(ACSSBADX* badx, int param_1, short vol);

int ACSSBADX_GetOutVol2(ACSSBADX* badx, int param_1);

void ACSSBADX_SetLpFlg(ACSSBADX* badx, int flag);

ADXT ACSSBADX_GetAdxt(ACSSBADX* badx);

void func_02021168(ACSSBADX* badx);

#endif // ACSSBADX_H
