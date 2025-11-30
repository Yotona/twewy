#ifndef DEBUG_STSAMPLE_H
#define DEBUG_STSAMPLE_H

#include <types.h>

typedef struct {
    /* 0x00000 */ char pad_00000[0x4C];
    /* 0x0004C */ char unk_0004C[0x11534];
    /* 0x11580 */ s32  unk_11580;
    /* 0x11584 */ s32  unk_11584;
    /* 0x11588 */ char pad_11588[0x1000C];
    /* 0x21594 */ s32  unk_21594;
    /* 0x21598 */ char pad_21598[0x8];
    /* 0x215A0 */ s32  unk_215A0;
    /* 0x215A4 */ char pad_215A4[0x40C];
    /* 0x219B0 */ s32  unk_219B0;
    /* 0x219B4 */ char unk_219B4[0x4AC];
    /* 0x21E60 */ s32  unk_21E60;
    /* 0x21E64 */ s32  unk_21E64;
    /* 0x21E68 */ s32  unk_21E68;
    /* 0x21E6C */ s32  unk_21E6C;
    /* 0x21E68 */ char unk_21E70[0xBC];
    /* 0x21F2C */ s32  unk_21F2C;
    /* 0x21F30 */ s16  unk_21F30;
    /* 0x21F32 */ s16  unk_21F32;
    /* 0x21F34 */ s16  unk_21F34;
    /* 0x21F36 */ s16  unk_21F36;
    /* 0x21F38 */ s32  unk_21F38;
    /* 0x21F3C */ s32  unk_21F3C;
    /* 0x21F40 */ s32  unk_21F40;
    /* 0x21F44 */ s32  unk_21F44;
    /* 0x21F48 */ s32  unk_21F48;
    /* 0x21F4C */ s32  unk_21F4C;
    /* 0x21F50 */ char unk_21F50[0x32];
    /* 0x21F82 */ s8   unk_21F82[0x100];
    /* 0x22084 */ s32  unk_22084;
    /* 0x22088 */ s32  unk_22088;
    /* 0x2208C */ s32  unk_2208C;
    /* 0x22090 */ char unk_22090[0xA8C];
} STSampleState; // Size: 0x22B1C

#endif           // DEBUG_STSAMPLE_H