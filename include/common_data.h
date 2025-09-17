#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <types.h>

extern s32 data_0206a9a4; // Global memory allocator handle

extern s32 data_0205c9b0;

extern s32 data_02066aac[16];
extern s16 data_02066aec;
extern s32 data_02066cec;
extern s16 data_02066eec;

extern s32 data_020670ec;
extern s32 data_020676ec;
extern s32 data_0206770c;

extern s32 data_02068778;
extern s32 data_02068798;

extern s32 data_0206a9b0;

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3C;
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    s32 unk_4C;
    s32 unk_50;
    s32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
} UnkStruct_0206aa80;
extern UnkStruct_0206aa80 data_0206aa80;

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
} UnkStruct_0206ab20;
extern UnkStruct_0206ab20 data_0206ab20;

typedef struct {
    /* 0x00 */ s32  unk_00;
    /* 0x04 */ s32  unk_04;
    /* 0x08 */ s32  unk_08;
    /* 0x0C */ s32  unk_0C;
    /* 0x10 */ s32  unk_10;
    /* 0x14 */ s32  unk_14;
    /* 0x18 */ s32  unk_18;
    /* 0x1C */ s32  unk_1C;
    /* 0x20 */ s32  unk_20;
    /* 0x24 */ s32  unk_24;
    /* 0x28 */ s32  unk_28;
    /* 0x2C */ s32  unk_2C;
    /* 0x30 */ s32  unk_30;
    /* 0x34 */ s32  unk_34;
    /* 0x38 */ s32  unk_38;
    /* 0x3C */ s32  unk_3C;
    /* 0x40 */ s32  unk_40;
    /* 0x44 */ s32  unk_44;
    /* 0x48 */ s32  unk_48;
    /* 0x4C */ s32  unk_4C;
    /* 0x50 */ s32  unk_50;
    /* 0x54 */ s32  unk_54;
    /* 0x58 */ s32  unk_58;
    /* 0x5C */ s32  unk_5C;
    /* 0x60 */ s32  unk_60;
    /* 0x64 */ s32  unk_64;
    /* 0x68 */ s32  unk_68;
    /* 0x6C */ s32  unk_6C;
    /* 0x70 */ s32  unk_70;
    /* 0x74 */ s32  unk_74;
    /* 0x78 */ char unk_78[0x34];
    /* 0xAC */ s32  unk_AC;
    /* 0xB0 */ s32  unk_B0;
    /* 0xB4 */ char unk_B4[0x34];
    /* 0xE8 */ s32  unk_E8;
    /* 0xEC */ s32  unk_EC;
} UnkStruct_0206ad04;
extern UnkStruct_0206ad04 data_0206ad04;

typedef struct {
    /* 0x0 */ s32 unk_00;
    /* 0x4 */ s32 unk_04;
} UnkStruct_0206b3cc;
extern UnkStruct_0206b3cc data_0206b3cc;

typedef struct {
    /* 0x00 */ s32   unk_00;
    /* 0x04 */ s32   unk_04;
    /* 0x08 */ s32   unk_08;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ void* unk_14;
    /* 0x18 */ s16   unk_18;
    /* 0x18 */ s16   unk_1A;
    /* 0x1C */ u16   unk_1C;
    /* 0x1E */ u16   unk_1E;
} UnkStruct_usedby_02025b68;
extern UnkStruct_usedby_02025b68 usedby_02025b68;

#endif // COMMON_DATA_H