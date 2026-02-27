#ifndef CRISNDMGR_H
#define CRISNDMGR_H

#include <CriWare/criss.h>
#include <types.h>

typedef struct {
    /* 0x0 */ const char* adxFile;
    /* 0x4 */ s32         volume;
} AdxData;

typedef enum {
    ADX_TITLE,
    ADX_F01,
    ADX_F02,
    ADX_F07,
    ADX_F03,
    ADX_F04,
    ADX_F05,
    ADX_F06,
    ADX_BZ01,
    ADX_BZ02,
    ADX_BZ03,
    ADX_BZ04,
    ADX_BZ05,
    ADX_BZ06,
    ADX_BZ07,
    ADX_B01,
    ADX_B02,
    ADX_B03,
    ADX_B04,
    ADX_B05,
    ADX_B06,
    ADX_LB01,
    ADX_B08,
    ADX_B09,
    ADX_B10,
    ADX_B11,
    ADX_E01,
    ADX_E02,
    ADX_E04,
    ADX_E05,
    ADX_E06,
    ADX_S01,
    ADX_S02,
    ADX_S03,
    ADX_ENDING,
    ADX_E_01_01,
    ADX_E_01_02A,
    ADX_E_01_02B,
    ADX_E_01_02C,
    ADX_E_01_03,
    ADX_E_01_04,
    ADX_E_01_05,
    ADX_E_01_06,
    ADX_E_02_01,
    ADX_E_03_01,
    ADX_E_03_02,
    ADX_E_04_01,
    ADX_E_07_01,
    ADX_R_01_01,
    ADX_R_02_01,
    ADX_R_02_02,
    ADX_R_02_03,
    ADX_R_04_01,
    ADX_R_05_01,
    ADX_R_07_01,
    ADX_R_07_02,
    ADX_R_07_03,
    ADX_B_02_01,
    ADX_B_07_01,
    ADX_B_07_02,
    ADX_B_07_03,
    ADX_B_07_04,
    ADX_B_07_06,
    ADX_B_07_07,
    ADX_B_07_08,
    ADX_B_07_09,
    ADX_B_07_10,
    ADX_B_07_11,
    ADX_B_07_12,
    ADX_S_01_01,
    ADX_COUNT
} CriSndMgrAdxId;

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

void CriSndMgr_PlayFile(CriSndMgrAdxId adxIdx);

void CriSndMgr_Stop(CriSndMgrAdxId adxIdx);

void CriSndMgr_Pause(CriSndMgrAdxId adxIdx, s32 pauseFlag);

void CriSndMgr_SetAdxIdxVolume(CriSndMgrAdxId adxIdx, s32 adxVolume);

s32 CriSndMgr_GetAdxIdxVolume(CriSndMgrAdxId adxIdx);

void CriSndMgr_SetLpFlg(BOOL lpFlg);

void* CriSndMgr_Create(s32 size);

void CriSndMgr_Free(void* data);

#endif // CRISNDMGR_H