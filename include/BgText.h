#ifndef BGTEXT_H
#define BGTEXT_H

#include <nitro/types.h>

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

extern const TextFontInfo data_0205c9b0[6];

#endif // BGTEXT_H
