#ifndef SNDMGR_H
#define SNDMGR_H

#include "game.h"
#include <types.h>

extern struct {
    /* 0x0 */ s8  seqArc;
    /* 0x1 */ u8  unk_01;
    /* 0x2 */ s16 se;
} data_0205cb3c[1388];

extern struct {
    /* 0x0000 */ GameState* state;
    /* 0x0004 */ s32        unk_0004;
    /* 0x0008 */ char       unk_0008[0x60];
    /* 0x0068 */ s32        unk_0068[1388];
    /* 0x1614 */ struct {
        /* 0x000 */ s32 unk_00;
        /* 0x004 */ s32 unk_04;
    } unk_1618[5];
    /* 0x1640 */ char unk_1640[0x90];
    /* 0x16D0 */ s32  unk_16D0;
    /* 0x16D4 */ s16  unk_16D4[3];
    /* 0x16DA */ s8   unk_16DA;
} data_02075214;

void SndMgr_Init(void);

void func_02026ae0(s32 param_1, s32 seqArc, s32 se);

void func_02026b20(s32 seIdx);

void func_02026b9c(s32 seIdx);

u32 func_02027040(s32 param_1);

BOOL func_02026e00(s32);

void func_02026e44(s32);

s32 func_02026db8(s32);

s32 func_02026d9c(s32);

void func_02026c04(void);

void func_02026dd8(void);

void func_020273a8(s32 param_1, s32 param_2);

void func_02027330(void);

void func_02027298(void);

void func_020273c8(s32 param_1, s32 param_2);

void func_02027220(s32 param_1);

#endif // SNDMGR_H