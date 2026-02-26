#ifndef CELLTEXT_H
#define CELLTEXT_H

#include "EasyTask.h"

typedef struct {
    /* 0x00 */ s32   unk_00;
    /* 0x04 */ s32   unk_04;
    /* 0x08 */ s32   unk_08;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ void* unk_10;
    /* 0x14 */ s32   unk_14;
    /* 0x18 */ u16   unk_18;
    /* 0x1A */ u16   unk_1A;
    /* 0x1C */ s32   unk_1C;
    /* 0x20 */ s32   unk_20;
    /* 0x24 */ s32   unk_24;
} CellTextArgs; // Size: 0x28

typedef struct {
    /* 0x000 */ s32   unk_000;
    /* 0x004 */ void* unk_004;
    /* 0x008 */ void* unk_008;
    /* 0x00C */ void* unk_00C;
    /* 0x010 */ s32   unk_010;
    /* 0x014 */ char  unk_014[0x4C - 0x14];
    /* 0x04C */ s16   unk_04C;
    /* 0x04E */ char  unk_04E[0x428 - 0x4E];
    /* 0x428 */ s32   unk_428;
    /* 0x42C */ s32   unk_42C;
} CellText; // Size: 0x430

TaskHandle data_0205ca88;

#endif // CELLTEXT_H