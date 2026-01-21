#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <types.h>

extern s32 data_02069804;

extern struct {
    /* 0x00 */ char unk_00[0xC];
    /* 0x0C */ s32  unk_0C;
} data_0206a890;

extern s32 data_0205c9b0;
typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
} UnkStruct_0206b408;
extern UnkStruct_0206b408 data_0206b408;

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

typedef struct {
    /* 0x0 */ s32 unk_00;
    /* 0x4 */ s32 unk_04;
    /* 0x8 */ s32 unk_08;
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

extern u16 data_02073710[];

typedef struct {
    /* 0x000 */ char unk_00[0x410];
    /* 0x410 */ s32  unk_410;
} UnkStruct_02074010;
extern UnkStruct_02074010 data_02074d10;

#endif // COMMON_DATA_H