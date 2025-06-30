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
    /* 0x0068 */ s32        seIdxVolume[1388];
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

void func_02026a94(void);

s32 func_02026aa4(s32 idx);

void func_02026ae0(s32 param_1, s32 seqArc, s32 se);

void func_02026b20(s32 seIdx);

void func_02026b9c(s32 seIdx);

void func_02026c04(void);

s32 SndMgr_GetSeIdxVolume(s32 seIdx);

void func_02026cc4(s32 seIdx, s32 seIdxVolume);

void func_02026d0c(s32 seIdx, s32 sePan);

void func_02026dd8(void);

BOOL func_02026e00(s32);

BOOL func_02026e28(s32 se);

void func_02026e44(s32);

/**
 * @brief Converts a pan value to a signed 8-bit range.
 *
 * - If sePan is less than 0, returns -0x80 (-128).
 * - If sePan is greater than or equal to 0x100 (256), returns 0x7f (127).
 * - Otherwise, returns sePan offset by -0x80.
 *
 * @param sePan The input pan value to convert.
 * @return The signed 8-bit pan value in the range [-128, 127].
 */
s32 SndMgr_ConvertPanToSigned(s32 sePan);

/**
 * @brief Converts a pan value to a corresponding appropriate volume adjustment for sound playback.
 *
 * @param sePan The pan value to convert.
 * @return The calculated volume value corresponding to the given pan.
 */
s32 SndMgr_ConvertPanToVolume(s32 sePan);

u32 func_02027040(s32 param_1);

void func_020270e4(void);

BOOL func_02027124(s32 seIdx);

void func_020271b8(s32 seIdx, s32 sePitch);

void func_02027200(u32 param_1);

void func_02027220(s32 param_1);

#endif // SNDMGR_H