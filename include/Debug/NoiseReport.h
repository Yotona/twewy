#ifndef DEBUG_NOISEREPORT_H
#define DEBUG_NOISEREPORT_H

#include "EasyTask.h"

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
    /* 0x00 */ char unk_00[0x40];
} UnkStruct_020eb9dc; // Size: 0x40

typedef struct {
    /* 0x000 */ s32  unk_000;
    /* 0x004 */ s32  unk_004;
    /* 0x008 */ char unk_008[0x6];
    /* 0x00E */ u16  unk_00E;
    /* 0x010 */ s16  unk_010;
    /* 0x012 */ s16  unk_012;
    /* 0x014 */ char unk_014[0x18];
    /* 0x02C */ s32  unk_02C;
    /* 0x030 */ char unk_030[0x14];
    /* 0x044 */ s32  unk_044;
    /* 0x048 */ char unk_048[0x6];
    /* 0x04E */ u16  unk_04E;
    /* 0x050 */ s16  unk_050;
    /* 0x052 */ s16  unk_052;
    /* 0x054 */ char unk_054[0x18];
    /* 0x06C */ s32  unk_06C;
    /* 0x070 */ char unk_070[0x14];
    /* 0x084 */ s32  unk_084;
    /* 0x088 */ char unk_088[0x13C];
    /* 0x1C4 */ u32  unk_1C4;
    /* 0x1C8 */ char unk_1C8[0xFC];
    /* 0x2C4 */ u32  unk_2C4;
    /* 0x2C8 */ char unk_2C8[0xFC];
    /* 0x3C4 */ u32  unk_3C4;
    /* 0x3C8 */ char unk_3C8[0xFC];
    /* 0x4C4 */ u32  unk_4C4;
    /* 0x4C8 */ char unk_4C8[0xBC];
    /* 0x584 */ u32  unk_584;
    /* 0x588 */ char unk_588[0x13C];
    /* 0x6C4 */ u32  unk_6C4;
    /* 0x6C8 */ char unk_6C8[0x3C];
    /* 0x704 */ u32  unk_704;
    /* 0x708 */ char unk_708[0x3C];
    /* 0x744 */ u32  unk_744;
    /* 0x748 */ char unk_748[0x3C];
    /* 0x784 */ u32  unk_784;
    /* 0x788 */ char unk_788[0x3C];
    /* 0x7C4 */ s16  unk_7C4;
    /* 0x7C6 */ char unk_7C6[2];
} NrepPreview; // Size: 0x7C8

typedef struct {
    /* 0x000 */ s32  unk_000;
    /* 0x004 */ s32  unk_004;
    /* 0x008 */ char unk_008[0x3C];
    /* 0x044 */ s32  unk_044;
    /* 0x048 */ char unk_048[0x3C];
    /* 0x084 */ s32  unk_084;
    /* 0x088 */ char unk_088[0x3C];
    /* 0x0C4 */ s32  unk_0C4;
    /* 0x0C8 */ char unk_0C8[0x3C];
    /* 0x104 */ s32  unk_104;
    /* 0x108 */ char unk_108[0x3C];
    /* 0x144 */ s32  unk_144;
    /* 0x148 */ char unk_148[0x3C];
    /* 0x184 */ s32  unk_184;
    /* 0x188 */ char unk_188[0x3C];
    /* 0x1C4 */ s32  unk_1C4;
    /* 0x1C8 */ char unk_1C8[0x3C];
    /* 0x204 */ s32  unk_204;
    /* 0x208 */ char unk_208[0x3C];
} NrepSeal; // Size: 0x244

typedef void (*NoiseReportFunc)(NoiseReportState* state);

#endif // DEBUG_NOISEREPORT_H