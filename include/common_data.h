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

typedef struct {
    /* 0x00 */ char unk_00[0xB4];
    /* 0xB4 */ u16  unk_B4;
    /* 0xB6 */ u16  unk_B6;
} Data_02073710;
extern Data_02073710 data_02073710;

typedef struct {
    /* 0x000 */ char unk_00[0x40A];
    /* 0x40A */ u8   unk_40A;
    /* 0x40B */ u8   unk_40B;
    /* 0x40C */ u8   unk_40C;
    /* 0x40D */ char unk_40D[0x410 - 0x40D];
    /* 0x410 */ s32  unk_410;
    /* 0x414 */ char unk_414[0x41C - 0x414];
    /* 0x41C */ s8   unk_41C;
} UnkStruct_02074010;
extern UnkStruct_02074010 data_02074d10;

typedef struct {
    /* 0x00 */ s32         unk0;
    /* 0x04 */ const void* unk4;
    /* 0x08 */ void*       unk8;
    /* 0x0C */ void*       unkC;
    /* 0x10 */ void*       unk10;
    /* 0x14 */ void*       unk_14;
    /* 0x18 */ void*       unk18;
    /* 0x1C */ s32         unk1C;
    /* 0x20 */ s32         unk20[5];
    /* 0x34 */ s32         unk34[5];
    /* 0x48 */ s32         unk48;
    /* 0x4C */ u16         unk4C;
    /* 0x4E */ u16         unk4E;
    /* 0x50 */ s32         unk50;
    /* 0x54 */ u16         unk54;
    /* 0x56 */ s16         unk56;
    /* 0x58 */ s16         unk58;
    /* 0x5A */ s8          unk5A;
    /* 0x5B */ u8          unk5B;
    /* 0x5C */ s16         unk_5C;
    /* 0x5E */ s16         unk5E;
    /* 0x60 */ s16         unk60;
    /* 0x64 */ s32         unk64;
    /* 0x68 */ s32         unk68;
    /* 0x6C */ s32         unk6C;
    /* 0x70 */ s32         unk_70;
    /* 0x74 */ s32         unk74;
    /* 0x78 */ s32         unk78;
} UnkOv31Struct; // Size: 0x7C
extern void func_ov031_0210aa94(UnkOv31Struct*);
extern void func_ov031_0210aabc(UnkOv31Struct*);
extern void func_ov031_0210ab34(UnkOv31Struct*, s32);

#endif // COMMON_DATA_H