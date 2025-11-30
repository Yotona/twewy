#ifndef DEBUG_SOUNDTEST_H
#define DEBUG_SOUNDTEST_H

#include <types.h>

#define MENU_ROW_ADX     0
#define MENU_ROW_SEQARC  1
#define MENU_ROW_SE      2
#define MENU_ROW_SEIDX   3
#define MENU_ROW_SEVOL   4
#define MENU_ROW_SEPAN   5
#define MENU_ROW_ADXVOL  6
#define MENU_ROW_ADXLOOP 7
#define MENU_ROW_SEPITCH 8
#define MENU_ROW_NOISE   9

typedef struct {
    /* 0x00000 */ char pad_00000[0x11580];
    /* 0x11580 */ s32  unk_11580;
    /* 0x11584 */ s32  unk_11584;
    /* 0x11588 */ char pad_11588[0x1000C];
    /* 0x21594 */ s32  unk_21594;
    /* 0x21598 */ char pad_21598[0x8];
    /* 0x215A0 */ s32  unk_215A0;
    /* 0x215A4 */ char pad_215A4[0x40C];
    /* 0x219B0 */ s32  menuCurrentRow; // Index of selected menu row
    /* 0x219B4 */ s32  unk_219B4;
    /* 0x219B8 */ s32  unk_219B8;
    /* 0x219BC */ s32  adxIdx;
    /* 0x219C0 */ s32  seqArc;
    /* 0x219C4 */ s32  se;
    /* 0x219C8 */ s32  seIdx;
    /* 0x219CC */ s32  unk_219CC;
    /* 0x219D0 */ s32  unk_219D0;
    /* 0x219D4 */ s32  seIdxVolume;
    /* 0x219D8 */ s32  adxVolume;
    /* 0x219DC */ s32  sePan;
    /* 0x219E0 */ s32  sePitch;
    /* 0x219E4 */ BOOL adxLoopEnabled;
    /* 0x219E8 */ s32  unk_219E8;
    /* 0x219EC */ BOOL noiseNoWaveLoad;
} SoundTestState; // Size: 0x219F0

#endif            // DEBUG_SOUNDTEST_H