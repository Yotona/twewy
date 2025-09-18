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
    /* 0x11588 */ char   unk_11588[0x1000C];
    /* 0x21594 */ s32    unk_21594;
    /* 0x21598 */ s32    unk_21598;
    /* 0x2159C */ s32    unk_2159C;
    /* 0x215A0 */ s32    unk_215A0;
    /* 0x215A4 */ char   unk_215A4[0x40C];
} GameState; // Size: 0x219B0

#endif       // GAME_H