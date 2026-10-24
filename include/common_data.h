#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <nitro/types.h>

extern struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} data_0206a890;

typedef struct TextFontFile {
    /* 0x00 */ u32         flags;
    /* 0x04 */ const char* path;
} TextFontFile;

typedef struct TextFontInfo {
    /* 0x00 */ const TextFontFile* file;
    /* 0x04 */ u32                 unk_04;
    /* 0x08 */ u32                 unk_08;
    /* 0x0C */ u16                 unk_0C;
    /* 0x0E */ u16                 unk_0E;
    /* 0x10 */ u16                 glyphWidth;
    /* 0x12 */ u16                 glyphHeight;
    /* 0x14 */ u32                 unk_14;
    /* 0x18 */ u32                 unk_18;
    /* 0x1C */ u32                 unk_1C;
    /* 0x20 */ u32                 lineHeight;
} TextFontInfo; // Size: 0x24

extern const TextFontInfo data_0205c9b0[6];

extern u16 data_02066aec;
extern s32 data_02066cec;
extern u16 data_02066eec;

extern s32 data_020670ec;
extern s32 data_020672ec;

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
    union {
        struct {
            /* 0x18 */ u16 unk_18;
            /* 0x1A */ u16 unk_1A;
        };
        /* 0x18 */ u16 tilePos[2];
    };
    union {
        struct {
            /* 0x1C */ u16 unk_1C;
            /* 0x1E */ u16 unk_1E;
        };
        /* 0x1C */ u16 tileSize[2];
    };
} UnkStruct_usedby_02025b68;
extern UnkStruct_usedby_02025b68 usedby_02025b68;

typedef struct {
    /* 0x000 */ char unk_00[0x40A];
    /* 0x40A */ u8   unk_40A;
    /* 0x40B */ u8   unk_40B;
    /* 0x40C */ u8   unk_40C;
    /* 0x40D */ char unk_40D[0x410 - 0x40D];
    /* 0x410 */ s32  unk_410;
    /* 0x414 */ char unk_414[0x41C - 0x414];
    /* 0x41C */ s8   unk_41C;
} UnkStruct_02074D10;
extern UnkStruct_02074D10 data_02074d10;

#endif // COMMON_DATA_H