#ifndef ACSVHL_H
#define ACSVHL_H

#include <CriWare/acsfdr.h>
#include <types.h>

typedef struct ACSSVR ACSSVR;

typedef struct {
    /* 0x00 */ ACSSND** snd;
    /* 0x04 */ ACSFDR*  unk_04;
    /* 0x08 */ ACSFDR   fdr;
    /* 0x4C */ s32      unk_4C;
    /* 0x50 */ s8       unk_50;
    /* 0x51 */ char     unk_51[0x3];
    /* 0x54 */ s32      unk_54;
    /* 0x58 */ s32      unk_58;
    /* 0x5C */ s32      unk_5C;
} ACSVHL; // Size: 0x60

void func_02021488(ACSVHL* vhl);
void func_020214d0(ACSVHL* vhl, s8 param_2);
s32  func_020214d8(ACSVHL* vhl);
s32  func_02021504(ACSVHL* vhl);

#endif // ACSVHL_H
