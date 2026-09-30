#ifndef ACSVHL_H
#define ACSVHL_H

#include <cri/acsfdr.h>
#include <cri/cri_xpt.h>

typedef struct ACSSVR ACSSVR;

typedef struct {
    /* 0x00 */ ACSSND** snd;
    /* 0x04 */ ACSFDR*  unk_04;
    /* 0x08 */ ACSFDR   fdr;
    /* 0x4C */ int      unk_4C;
    /* 0x50 */ char     unk_50;
    /* 0x51 */ char     unk_51[0x3];
    /* 0x54 */ int      unk_54;
    /* 0x58 */ int      unk_58;
    /* 0x5C */ int      unk_5C;
} ACSVHL; // Size: 0x60

void func_02021488(ACSVHL* vhl);
void func_020214d0(ACSVHL* vhl, char param_2);
int  func_020214d8(ACSVHL* vhl);
int  func_02021504(ACSVHL* vhl);

#endif // ACSVHL_H
