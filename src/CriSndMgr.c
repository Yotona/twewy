#include "CriSndMgr.h"
#include "adxt.h"
#include "common_data.h"

const s32 data_0205e0ec = 1284;

const s32 CriSndMgr_AdxDataCount = 70;

extern s32 data_02071b00;

void func_02027294(void) {
    return;
}

// Nonmatching: Regswap, data order differs
// Scratch: 5JLuF
void func_02027298(void) {
    s32* data = criSndMgr.unk_0C;
    for (s32 idx = 0; idx < 70; idx++) {
        data[idx] = CriSndMgr_AdxData[idx].unk_04;
    }

    criSndMgr.initFunc = func_02027444;
    criSndMgr.execFunc = func_02027494;
    func_02021728(&criSndMgr.initFunc, 2, &data_0205e0ec, func_02027294);
    criSndMgr.unk_00 = func_02021960(&criSndMgr.initFunc, 2, 0);
    func_02021d74(criSndMgr.unk_00, 0, 0);
}

void func_02027330(void) {
    func_020218ec();
}

void func_0202733c(s32 adxIdx) {
    if (adxIdx < ARRAY_COUNT(criSndMgr.unk_0C)) {
        func_020273a8(adxIdx, 0);
        func_020273c8(adxIdx, criSndMgr.unk_0C[adxIdx]);
        func_02021bc8(criSndMgr.unk_00, CriSndMgr_AdxData[adxIdx].adxFile);
    }
}

void func_02027388(s32 param_1) {
    if (param_1 < ARRAY_COUNT(criSndMgr.unk_0C)) {
        func_02021c50(criSndMgr.unk_00);
    }
}

void func_020273a8(s32 param_1, s32 param_2) {
    if (param_1 < ARRAY_COUNT(criSndMgr.unk_0C)) {
        func_02021cac(criSndMgr.unk_00, param_2);
    }
}

void func_020273c8(s32 param_1, s32 param_2) {
    if (param_1 < ARRAY_COUNT(criSndMgr.unk_0C)) {
        criSndMgr.unk_0C[param_1] = param_2;
        func_02021d10(criSndMgr.unk_00, param_2);
    }
}

s32 func_020273f4(s32 param_1) {
    s32 sVar1 = 0;
    if (param_1 < ARRAY_COUNT(criSndMgr.unk_0C)) {
        sVar1 = criSndMgr.unk_0C[param_1];
    }
    return sVar1;
}

s32 func_02027410(void) {
    func_02021e30(criSndMgr.unk_00);
}

void func_02027428(s32 param_1) {
    func_02021e90(criSndMgr.unk_00, param_1);
}

GameState* func_02027444(s32 param_1) {
    GameState* state = func_02004618(&data_0206a9a4, param_1);
    func_020049a8(&data_0206a9a4, state, "CriSndMgr");
    func_0203b2d0(0, state, func_0200498c(&data_0206a9a4, state));
    return state;
}

void func_02027494(GameState* param_1) {
    func_020048b4(&data_0206a9a4, param_1);
}
