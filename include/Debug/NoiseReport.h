#ifndef DEBUG_NOISEREPORT_H
#define DEBUG_NOISEREPORT_H

#include "EasyTask.h"
#include "SpriteMgr.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ s32       unk_00;
    /* 0x04 */ TaskPool* pool;
    /* 0x08 */ char      unk_08[0x4];
    /* 0x0C */ s32       unk_0C;
    /* 0x10 */ char      unk_10[4];
    /* 0x14 */ s16       unk_14;
    /* 0x16 */ s16       unk_16;
    /* 0x18 */ char      unk_18[0x24];
} NoiseReport; // Size: 0x3C

typedef struct {
    /* 0x00000 */ char        unk_00000[0x11580];
    /* 0x11580 */ s32         unk_11580;
    /* 0x11584 */ MemPool     memPool;
    /* 0x11590 */ MemBlock    memBlock;
    /* 0x115B0 */ char        unk_115B0[0xFFE0];
    /* 0x21590 */ TaskPool    taskPool;
    /* 0x21610 */ NoiseReport unk_21610;
    /* 0x2164C */ u16         unk_2164C;
    /* 0x2164E */ char        unk_2164E[2];
} NoiseReportState; // Size: 0x21650

typedef struct {
    /* 0x000 */ s32    unk_000;
    /* 0x004 */ Sprite unk_004;
    /* 0x044 */ Sprite unk_044;
    /* 0x084 */ Sprite unk_084;
    /* 0x088 */ char   unk_088[0x100];
    /* 0x1C4 */ Sprite unk_1C4;
    /* 0x204 */ char   unk_204[0xC0];
    /* 0x2C4 */ Sprite unk_2C4;
    /* 0x304 */ char   unk_304[0xC0];
    /* 0x3C4 */ Sprite unk_3C4;
    /* 0x404 */ char   unk_404[0xC0];
    /* 0x4C4 */ Sprite unk_4C4;
    /* 0x504 */ char   unk_504[0x80];
    /* 0x584 */ Sprite unk_584;
    /* 0x5C4 */ char   unk_5C4[0x100];
    /* 0x6C4 */ Sprite unk_6C4;
    /* 0x704 */ Sprite unk_704;
    /* 0x744 */ Sprite unk_744;
    /* 0x784 */ Sprite unk_784;
    /* 0x7C4 */ s16    unk_7C4;
    /* 0x7C6 */ char   unk_7C6[2];
} NrepPreview; // Size: 0x7C8

typedef struct {
    /* 0x000 */ s32    unk_000;
    /* 0x004 */ Sprite unk_004;
    /* 0x044 */ Sprite unk_044;
    /* 0x084 */ Sprite unk_084;
    /* 0x0C4 */ Sprite unk_0C4;
    /* 0x104 */ Sprite unk_104;
    /* 0x144 */ Sprite unk_144;
    /* 0x184 */ Sprite unk_184;
    /* 0x1C4 */ Sprite unk_1C4;
    /* 0x204 */ Sprite unk_204;
} NrepSeal; // Size: 0x244

typedef void (*NoiseReportFunc)(NoiseReportState* state);

#endif // DEBUG_NOISEREPORT_H