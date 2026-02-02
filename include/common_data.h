#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <types.h>

extern s32 data_02069804;

extern struct {
    /* 0x00 */ char unk_00[0xC];
    /* 0x0C */ s32  unk_0C;
} data_0206a890;

extern s32 data_0205c9b0;

extern s16 data_02066aec;
extern s32 data_02066cec;
extern s16 data_02066eec;

extern s32 data_020670ec;
extern s32 data_020672ec;
extern s32 data_020676ec;
extern s32 data_0206770c;

extern s32 data_02068778;
extern s32 data_02068798;

extern s32 data_0205a128;

extern s32 data_0206b3cc[3];

typedef struct {
    /* 0x0 */ char pad_00[0x28];
    /* 0x28 */ u16 unk_28;
} UnkStruct_02074E10;
extern UnkStruct_02074E10 data_02074e10;

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

extern u16 data_02073710[];

typedef struct {
    /* 0x000 */ char unk_00[0x410];
    /* 0x410 */ s32  unk_410;
} UnkStruct_02074010;
extern UnkStruct_02074010 data_02074d10;

#endif // COMMON_DATA_H