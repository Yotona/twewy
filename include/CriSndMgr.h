#ifndef CRISNDMGR_H
#define CRISNDMGR_H

#include <CriWare/criss.h>
#include <types.h>

typedef struct {
    /* 0x0 */ const char* adxFile;
    /* 0x4 */ s32         volume;
} AdxData;

// Nonmatching: .data section order differs, .rodata matches
extern const AdxData CriSndMgr_AdxData[70];

extern const s32 CriSndMgr_AdxDataCount;

typedef struct {
    /* 0x00 */ CRISS* criss;
    /* 0x04 */ void* (*createFunc)(s32);
    /* 0x08 */ void (*deleteFunc)(void*);
    /* 0x0C */ s32 volume[70];
} CriSndMgr;
extern CriSndMgr criSndMgr;

void CriSndMgr_Init(void);

void func_02027330(void);

void func_0202733c(s32 adxIdx);

void CriSndMgr_Stop(s32 param_1);

void func_020273a8(s32 param_1, s32 param_2);

void CriSndMgr_SetAdxIdxVolume(s32 adxIdx, s32 adxVolume);

s32 CriSndMgr_GetAdxIdxVolume(s32 adxIdx);

void CriSndMgr_SetLpFlg(BOOL lpFlg);

void* CriSndMgr_Create(s32 size);

void CriSndMgr_Free(void* data);

#endif // CRISNDMGR_H