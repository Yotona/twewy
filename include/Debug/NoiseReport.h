#ifndef DEBUG_NOISEREPORT_H
#define DEBUG_NOISEREPORT_H

#include "EasyTask.h"
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
    /* 0x000 */ s32                unk_000;
    /* 0x004 */ UnkStruct_0200e998 unk_004;
    /* 0x044 */ UnkStruct_0200e998 unk_044;
    /* 0x084 */ UnkStruct_0200e998 unk_084;
    /* 0x088 */ char               unk_088[0x100];
    /* 0x1C4 */ UnkStruct_0200e998 unk_1C4;
    /* 0x1C8 */ char               unk_1C8[0xC0];
    /* 0x2C4 */ UnkStruct_0200e998 unk_2C4;
    /* 0x2C8 */ char               unk_2C8[0xC0];
    /* 0x3C4 */ UnkStruct_0200e998 unk_3C4;
    /* 0x3C8 */ char               unk_3C8[0xC0];
    /* 0x4C4 */ UnkStruct_0200e998 unk_4C4;
    /* 0x4C8 */ char               unk_4C8[0x80];
    /* 0x584 */ UnkStruct_0200e998 unk_584;
    /* 0x588 */ char               unk_588[0x100];
    /* 0x6C4 */ UnkStruct_0200e998 unk_6C4;
    /* 0x704 */ UnkStruct_0200e998 unk_704;
    /* 0x744 */ UnkStruct_0200e998 unk_744;
    /* 0x784 */ UnkStruct_0200e998 unk_784;
    /* 0x7C4 */ s16                unk_7C4;
    /* 0x7C6 */ char               unk_7C6[2];
} NrepPreview; // Size: 0x7C8

typedef struct {
    /* 0x000 */ s32                unk_000;
    /* 0x004 */ UnkStruct_0200e998 unk_004;
    /* 0x044 */ UnkStruct_0200e998 unk_044;
    /* 0x084 */ UnkStruct_0200e998 unk_084;
    /* 0x0C4 */ UnkStruct_0200e998 unk_0C4;
    /* 0x104 */ UnkStruct_0200e998 unk_104;
    /* 0x144 */ UnkStruct_0200e998 unk_144;
    /* 0x184 */ UnkStruct_0200e998 unk_184;
    /* 0x1C4 */ UnkStruct_0200e998 unk_1C4;
    /* 0x204 */ UnkStruct_0200e998 unk_204;
} NrepSeal; // Size: 0x244

typedef void (*NoiseReportFunc)(NoiseReportState* state);

#endif // DEBUG_NOISEREPORT_H