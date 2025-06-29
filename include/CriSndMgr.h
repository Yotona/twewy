#ifndef CRISNDMGR_H
#define CRISNDMGR_H

#include "game.h"
#include <types.h>

typedef struct {
    /* 0x0 */ const char* adxFile;
    /* 0x4 */ s32         unk_04;
} AdxData;

// Nonmatching: .data section order differs, .rodata matches
extern const AdxData CriSndMgr_AdxData[70] = {
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

extern const s32 CriSndMgr_AdxDataCount;

struct CriSndMgr {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ GameState* (*initFunc)(s32);
    /* 0x08 */ void (*execFunc)(GameState*);
    /* 0x0C */ s32 unk_0C[70];
} criSndMgr;

void func_020273a8(s32 param_1, s32 param_2);

void func_020273c8(s32 param_1, s32 param_2);

GameState* func_02027444(s32 param_1);

void func_02027494(GameState* param_1);

#endif // CRISNDMGR_H