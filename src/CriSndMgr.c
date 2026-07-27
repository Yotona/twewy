#include "CriSndMgr.h"
#include "Engine/Core/Memory.h"
#include "common_data.h"
#include <CriWare/adxt.h>
#include <CriWare/criss.h>

CriSndMgr criSndMgr;

const s32 CriSndMgr_AdxDataCount = ADX_COUNT;

const s32 data_0205e0ec = 1284;

const AdxData CriSndMgr_AdxData[ADX_COUNT] = {
    [ADX_TITLE] = {   "title.adx",  -30},
      [ADX_F04] = {     "F04.adx",  -70},
      [ADX_B04] = {     "B04.adx",  -65},
    [ADX_F07] = {     "F07.adx",  -85},
      [ADX_B01] = {     "B01.adx",  -70},
      [ADX_F03] = {     "F03.adx",  -75},
    [ADX_B06] = {     "B06.adx",  -50},
      [ADX_F06] = {     "F06.adx",  -50},
      [ADX_BZ05] = {    "Bz05.adx",  -75},
    [ADX_BZ03] = {    "Bz03.adx",  -80},
      [ADX_BZ04] = {    "Bz04.adx",  -70},
      [ADX_BZ07] = {    "Bz07.adx",  -60},
    [ADX_BZ02] = {    "Bz02.adx",  -75},
      [ADX_BZ01] = {    "Bz01.adx",  -50},
      [ADX_BZ06] = {    "Bz06.adx",  -50},
    [ADX_E01] = {     "E01.adx",  -55},
      [ADX_B09] = {     "B09.adx",  -70},
      [ADX_B03] = {     "B03.adx",  -30},
    [ADX_F05] = {     "F05.adx",  -65},
      [ADX_S02] = {     "S02.adx",  -45},
      [ADX_B11] = {     "B11.adx",  -50},
    [ADX_LB01] = {    "LB01.adx",  -50},
      [ADX_E04] = {     "E04.adx",  -65},
      [ADX_E05] = {     "E05.adx",  -35},
    [ADX_E06] = {     "E06.adx",  -65},
      [ADX_B10] = {     "B10.adx",  -80},
      [ADX_S01] = {     "S01.adx",  -35},
    [ADX_F02] = {     "F02.adx",  -85},
      [ADX_S03] = {     "S03.adx",  -60},
      [ADX_B05] = {     "B05.adx",  -55},
    [ADX_E02] = {     "E02.adx",  -65},
      [ADX_F01] = {     "F01.adx",  -90},
      [ADX_B08] = {     "B08.adx",  -36},
    [ADX_B02] = {     "B02.adx",  -35},
      [ADX_ENDING] = {  "ending.adx",  -10},
      [ADX_E_01_04] = { "e_01_04.adx", -960},
    [ADX_E_01_02A] = {"e_01_02a.adx",  -15},
      [ADX_E_01_02B] = {"e_01_02b.adx",  -15},
      [ADX_E_01_02C] = {"e_01_02c.adx",  -15},
    [ADX_E_03_01] = { "e_03_01.adx",  -15},
      [ADX_B_07_03] = { "b_07_03.adx",  -15},
      [ADX_E_03_02] = { "e_03_02.adx",  -15},
    [ADX_E_04_01] = { "e_04_01.adx",  -15},
      [ADX_E_07_01] = { "e_07_01.adx",  -15},
      [ADX_R_01_01] = { "r_01_01.adx",    0},
    [ADX_R_02_01] = { "r_02_01.adx",    0},
      [ADX_R_02_02] = { "r_02_02.adx",  -15},
      [ADX_R_02_03] = { "r_02_03.adx",  -15},
    [ADX_R_04_01] = { "r_04_01.adx",    0},
      [ADX_B_07_09] = { "b_07_09.adx",  -15},
      [ADX_R_07_02] = { "r_07_02.adx",    0},
    [ADX_R_07_03] = { "r_07_03.adx",    0},
      [ADX_B_07_10] = { "b_07_10.adx",  -15},
      [ADX_E_01_03] = { "e_01_03.adx",  -15},
    [ADX_B_07_01] = { "b_07_01.adx",  -15},
      [ADX_B_07_02] = { "b_07_02.adx",  -15},
      [ADX_B_07_12] = { "b_07_12.adx",    0},
    [ADX_B_07_04] = { "b_07_04.adx",    0},
      [ADX_B_07_06] = { "b_07_06.adx",  -10},
      [ADX_B_07_07] = { "b_07_07.adx",    0},
    [ADX_B_07_08] = { "b_07_08.adx",  -15},
      [ADX_E_01_01] = { "e_01_01.adx",    0},
      [ADX_B_07_11] = { "b_07_11.adx",    0},
    [ADX_E_01_05] = { "e_01_05.adx",  -15},
      [ADX_E_01_06] = { "e_01_06.adx",  -15},
      [ADX_S_01_01] = { "s_01_01.adx",    0},
    [ADX_R_05_01] = { "r_05_01.adx",  -15},
      [ADX_B_02_01] = { "b_02_01.adx",  -15},
      [ADX_E_02_01] = { "e_02_01.adx",   -5},
    [ADX_R_07_01] = { "r_07_01.adx",  -10}
};

extern s32 data_02071b00;

void CriSndMgr_DummyFunc(void) {
    // Maybe fake, but its a no-op that forces mwcc to emit .rodata in the correct order so whatever
    (void)&CriSndMgr_AdxDataCount;
    return;
}

// Nonmatching: Regswap
// Scratch: jKJJV
void CriSndMgr_Init(void) {
    s32*           volume;
    const AdxData* data = CriSndMgr_AdxData;

    volume = criSndMgr.volume;
    for (s32 idx = 0; idx < ADX_COUNT; idx++) {
        volume[idx] = data[idx].volume;
    }

    criSndMgr.createFunc = CriSndMgr_Create;
    criSndMgr.deleteFunc = CriSndMgr_Free;
    func_02021728(&criSndMgr.createFunc, 2, &data_0205e0ec, CriSndMgr_DummyFunc);
    criSndMgr.criss = func_02021960(&criSndMgr.createFunc, 2, 0);
    criSsPly_SetFadeTime(criSndMgr.criss, 0, 0);
}

void func_02027330(void) {
    func_020218ec();
}

void CriSndMgr_PlayFile(CriSndMgrAdxId adxIdx) {
    if (adxIdx < ARRAY_LEN(criSndMgr.volume)) {
        CriSndMgr_Pause(adxIdx, 0);
        CriSndMgr_SetAdxIdxVolume(adxIdx, criSndMgr.volume[adxIdx]);
        func_02021bc8(criSndMgr.criss, CriSndMgr_AdxData[adxIdx].adxFile);
    }
}

void CriSndMgr_Stop(CriSndMgrAdxId adxIdx) {
    if (adxIdx < ARRAY_LEN(criSndMgr.volume)) {
        criSsPly_Stop(criSndMgr.criss);
    }
}

void CriSndMgr_Pause(CriSndMgrAdxId adxIdx, s32 pauseFlag) {
    if (adxIdx < ARRAY_LEN(criSndMgr.volume)) {
        criSsPly_Pause(criSndMgr.criss, pauseFlag);
    }
}

void CriSndMgr_SetAdxIdxVolume(CriSndMgrAdxId adxIdx, s32 adxVolume) {
    if (adxIdx < ARRAY_LEN(criSndMgr.volume)) {
        criSndMgr.volume[adxIdx] = adxVolume;
        criSsPly_SetVolume(criSndMgr.criss, adxVolume);
    }
}

s32 CriSndMgr_GetAdxIdxVolume(CriSndMgrAdxId adxIdx) {
    s32 volume = 0;
    if (adxIdx < ARRAY_LEN(criSndMgr.volume)) {
        volume = criSndMgr.volume[adxIdx];
    }
    return volume;
}

s32 CriSndMgr_Play(void) {
    criSsPly_Play(criSndMgr.criss);
}

void CriSndMgr_SetLpFlg(BOOL lpFlg) {
    criSsPly_SetLpFlg(criSndMgr.criss, lpFlg);
}

void* CriSndMgr_Create(s32 size) {
    void* mgr = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, mgr, "CriSndMgr");
    MI_CpuFill(0, mgr, Mem_GetBlockSize(&gMainHeap, mgr));
    return mgr;
}

void CriSndMgr_Free(void* data) {
    Mem_Free(&gMainHeap, data);
}
