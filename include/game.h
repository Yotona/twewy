/**
 * This file is intended to define and declare components related to a
 * large monolithic structure. The exact purpose and composition of
 * this structure have not yet been identified or documented.
 * Initial suspicion suggests that this structure may be used for
 * managing an overall "game state" functionality.
 *
 * Further analysis and decompilation efforts are required to determine
 * the functionality and details of the elements that will reside here.
 */
#ifndef GAME_H
#define GAME_H

#include "BinMgr.h"
#include <types.h>

typedef struct {
    /* 0x00000 */ BinMgr binMgr;
    /* 0x0004C */ char   unk_0004C[0x11534];
    /* 0x11580 */ s32    unk_11580;
    /* 0x11584 */ s32    unk_11584;
    ///* 0x11588 */ char   unk_11588[0x1000C];
    /* 0x11588 */ char unk_11588[0x494];
    /* 0x11A1C */ s16  unk_11A1C;
    /* 0x11A1E */ s16  pad_11A1E;
    /* 0x11A20 */ s16  unk_11A20;
    /* 0x11A22 */ s16  pad_11A22;
    /* 0x11A24 */ s32  unk_11A24;
    /* 0x11A28 */ s32  unk_11A28;
    /* 0x11A2C */ s32  unk_11A2C;
    /* 0x11A30 */ s32  unk_11A30;
    /* 0x11A34 */ s32  unk_11A34;
    /* 0x11A38 */ s16  unk_11A38;
    /* 0x11A3A */ s16  pad_11A3A;
    /* 0x11A3C */ s32  unk_11A3C;
    /* 0x11A40 */ s32  unk_11A40;
    /* 0x11A44 */ s32  unk_11A44;
    /* 0x11A48 */ s32  unk_11A48;
    /* 0x11A4C */ char unk_11A4C[0xFB48];
    /* 0x21594 */ s32  unk_21594;
    /* 0x21598 */ s32  unk_21598;
    /* 0x2159C */ s32  unk_2159C;
    /* 0x215A0 */ s32  unk_215A0;
    /* 0x215A4 */ char unk_215A4[0x40C];
} GameState; // Size: 0x219B0

#endif       // GAME_H