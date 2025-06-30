#include "CriSndMgr.h"
#include "adxt.h"
#include "common_data.h"

CriSndMgr criSndMgr;

const s32 data_0205e0ec = 1284;

const s32 CriSndMgr_AdxDataCount = 70;

const AdxData CriSndMgr_AdxData[70] = {
    {   "title.adx",  -30},
    {     "F01.adx",  -90},
    {     "F02.adx",  -85},
    {     "F07.adx",  -85},
    {     "F03.adx",  -75},
    {     "F04.adx",  -70},
    {     "F05.adx",  -65},
    {     "F06.adx",  -50},
    {    "Bz01.adx",  -50},
    {    "Bz02.adx",  -75},
    {    "Bz03.adx",  -80},
    {    "Bz04.adx",  -70},
    {    "Bz05.adx",  -75},
    {    "Bz06.adx",  -50},
    {    "Bz07.adx",  -60},
    {     "B01.adx",  -70},
    {     "B02.adx",  -35},
    {     "B03.adx",  -30},
    {     "B04.adx",  -65},
    {     "B05.adx",  -55},
    {     "B06.adx",  -50},
    {    "LB01.adx",  -50},
    {     "B08.adx",  -36},
    {     "B09.adx",  -70},
    {     "B10.adx",  -80},
    {     "B11.adx",  -50},
    {     "E01.adx",  -55},
    {     "E02.adx",  -65},
    {     "E04.adx",  -65},
    {     "E05.adx",  -35},
    {     "E06.adx",  -65},
    {     "S01.adx",  -35},
    {     "S02.adx",  -45},
    {     "S03.adx",  -60},
    {  "ending.adx",  -10},
    { "e_01_01.adx",    0},
    {"e_01_02a.adx",  -15},
    {"e_01_02b.adx",  -15},
    {"e_01_02c.adx",  -15},
    { "e_01_03.adx",  -15},
    { "e_01_04.adx", -960},
    { "e_01_05.adx",  -15},
    { "e_01_06.adx",  -15},
    { "e_02_01.adx",   -5},
    { "e_03_01.adx",  -15},
    { "e_03_02.adx",  -15},
    { "e_04_01.adx",  -15},
    { "e_07_01.adx",  -15},
    { "r_01_01.adx",    0},
    { "r_02_01.adx",    0},
    { "r_02_02.adx",  -15},
    { "r_02_03.adx",  -15},
    { "r_04_01.adx",    0},
    { "r_05_01.adx",  -15},
    { "r_07_01.adx",  -10},
    { "r_07_02.adx",    0},
    { "r_07_03.adx",    0},
    { "b_02_01.adx",  -15},
    { "b_07_01.adx",  -15},
    { "b_07_02.adx",  -15},
    { "b_07_03.adx",  -15},
    { "b_07_04.adx",    0},
    { "b_07_06.adx",  -10},
    { "b_07_07.adx",    0},
    { "b_07_08.adx",  -15},
    { "b_07_09.adx",  -15},
    { "b_07_10.adx",  -15},
    { "b_07_11.adx",    0},
    { "b_07_12.adx",    0},
    { "s_01_01.adx",    0}
};

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
