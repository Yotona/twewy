#ifndef ACSSBADX_H
#define ACSSBADX_H

#include <adxt.h>
#include <types.h>

/* Operation Status */
#define ACSSBADX_STAT_STOPPED 0 // Standby
#define ACSSBADX_STAT_LOADING 1 // Retrieving header information
#define ACSSBADX_STAT_PLAYING 2 // Decoding/Playing
#define ACSSBADX_STAT_ENDING  3 // Decoding/Playing ended
#define ACSSBADX_STAT_ERROR   4 // Error occurred

typedef struct {
    /* 0x0 */ struct _acssbadx_vtable* vtable;
    /* 0x4 */ ADXT*                    adxt;
    /* 0x8 */ s32                      unk_8;
} ACSSBADX; // Size: 0xC

typedef struct _acssbadx_vtable {
    BOOL (*CreateHandle)(ACSSBADX* badx, s32*, void* work, s32 workSize);
    void (*DestroyAdxt)(ACSSBADX* badx);
    void (*unkFunc)(ACSSBADX* badx, const char* filename);
    void (*StartFnameRange)(ACSSBADX* badx);
    void (*StartAfs)(ACSSBADX* badx, s32, s32);
    void (*StartMem2)(ACSSBADX* badx, s32, s32);
    void (*StartMemIdx)(ACSSBADX* badx, s32, s32);
    void (*Stop)(ACSSBADX* badx);
    void (*Pause)(ACSSBADX* badx);
    s32 (*GetStatPause)(ACSSBADX* badx);
    s32 (*GetStat)(ACSSBADX* badx);
    void (*SetOutVol)(ACSSBADX* badx, s16 vol);
    s32 (*GetOutVol)(ACSSBADX* badx);
    void (*SetOutVol2)(ACSSBADX* badx, s32, s16 vol);
    s32 (*GetOutVol2)(ACSSBADX* badx, s32);
    void (*SetLpFlg)(ACSSBADX* badx, s32 flag);
    ADXT* (*GetAdxt)(ACSSBADX* badx);
} _acssbadx_vtable;

ACSSBADX* ACSSBADX_Create(ACSSBADX* badx, u16 workSize);

BOOL ACSSBADX_CreateHndl(ACSSBADX* badx, s32* param_2, void* work, s32 workSize);

void ACSSBADX_DestroyAdxt(ACSSBADX* badx);

void func_02021004(ACSSBADX* badx, const char* filename);

void ACSSBADX_StartFnameRange(ACSSBADX* badx);

void ACSSBADX_StartAfs(ACSSBADX* badx, s32 param_1, s32 param_2);

void ACSSBADX_StartMem2(ACSSBADX* badx, s32 param_1, s32 param_2);

void ACSSBADX_StartMemIdx(ACSSBADX* badx, s32 param_1, s32 param_2);

void ACSSBADX_Stop(ACSSBADX* badx);

void ACSSBADX_Pause(ACSSBADX* badx);

s32 ACSSBADX_GetStatPause(ACSSBADX* badx);

s32 ACSSBADX_GetStat(ACSSBADX* badx);

void ACSSBADX_SetOutVol(ACSSBADX* badx, s16 vol);

s32 ACSSBADX_GetOutVol(ACSSBADX* badx);

void ACSSBADX_SetOutVol2(ACSSBADX* badx, s32 param_1, s16 vol);

s32 ACSSBADX_GetOutVol2(ACSSBADX* badx, s32 param_1);

void ACSSBADX_SetLpFlg(ACSSBADX* badx, s32 flag);

ADXT* ACSSBADX_GetAdxt(ACSSBADX* badx);

void func_02021168(ACSSBADX* badx);

#endif // ACSSBADX_H
