#include "CriSndMgr.h"
#include "adxt.h"
#include "common_data.h"

const s32 data_0205e0ec = 1284;

const s32 CriSndMgr_AdxDataCount = 70;

extern s32 data_02071b00;

void CriSndMgr_DummyFunc(void) {
    return;
}

// Nonmatching: Regswap, data order differs
// Scratch: 5JLuF
void CriSndMgr_Init(void) {
    s32* volume = criSndMgr.volume;
    for (s32 idx = 0; idx < 70; idx++) {
        volume[idx] = CriSndMgr_AdxData[idx].volume;
    }

    criSndMgr.createFunc = CriSndMgr_Create;
    criSndMgr.execFunc   = CriSndMgr_Exec;
    func_02021728(&criSndMgr.createFunc, 2, &data_0205e0ec, CriSndMgr_DummyFunc);
    criSndMgr.unk_00 = func_02021960(&criSndMgr.createFunc, 2, 0);
    func_02021d74(criSndMgr.unk_00, 0, 0);
}

void func_02027330(void) {
    func_020218ec();
}

void func_0202733c(s32 adxIdx) {
    if (adxIdx < ARRAY_COUNT(criSndMgr.volume)) {
        func_020273a8(adxIdx, 0);
        CriSndMgr_SetAdxIdxVolume(adxIdx, criSndMgr.volume[adxIdx]);
        func_02021bc8(criSndMgr.unk_00, CriSndMgr_AdxData[adxIdx].adxFile);
    }
}

void func_02027388(s32 param_1) {
    if (param_1 < ARRAY_COUNT(criSndMgr.volume)) {
        func_02021c50(criSndMgr.unk_00);
    }
}

void func_020273a8(s32 param_1, s32 param_2) {
    if (param_1 < ARRAY_COUNT(criSndMgr.volume)) {
        func_02021cac(criSndMgr.unk_00, param_2);
    }
}

void CriSndMgr_SetAdxIdxVolume(s32 adxIdx, s32 adxVolume) {
    if (adxIdx < ARRAY_COUNT(criSndMgr.volume)) {
        criSndMgr.volume[adxIdx] = adxVolume;
        func_02021d10(criSndMgr.unk_00, adxVolume);
    }
}

s32 CriSndMgr_GetAdxIdxVolume(s32 adxIdx) {
    s32 volume = 0;
    if (adxIdx < ARRAY_COUNT(criSndMgr.volume)) {
        volume = criSndMgr.volume[adxIdx];
    }
    return volume;
}

s32 func_02027410(void) {
    func_02021e30(criSndMgr.unk_00);
}

void func_02027428(s32 param_1) {
    func_02021e90(criSndMgr.unk_00, param_1);
}

GameState* CriSndMgr_Create(s32 param_1) {
    GameState* state = func_02004618(&data_0206a9a4, param_1);
    func_020049a8(&data_0206a9a4, state, "CriSndMgr");
    func_0203b2d0(0, state, func_0200498c(&data_0206a9a4, state));
    return state;
}

void CriSndMgr_Exec(GameState* state) {
    func_020048b4(&data_0206a9a4, state);
}
