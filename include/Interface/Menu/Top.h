#ifndef INTERFACE_MENU_TOP_H
#define INTERFACE_MENU_TOP_H

#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

extern BinIdentifier data_ov043_020c7988;
extern BinIdentifier data_ov043_020c79a8;
extern BinIdentifier data_ov043_020c79d8;

typedef struct Point {
    s16 x;
    s16 y;
} Point;

typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ char unk_02[0x06 - 0x02];
    /* 0x06 */ u16  unk_06;
    /* 0x08 */ u16  unk_08;
    /* 0x0A */ char unk_0A[0x0C - 0x0A];
    /* 0x0C */ u32  unk_0C;
    /* 0x10 */ u16  unk_10;
    /* 0x12 */ u16  unk_12;
    /* 0x14 */ u16  unk_14;
    /* 0x16 */ char unk_16[0x18 - 0x16];
    /* 0x18 */ u32  unk_18;
    /* 0x1C */ u32  unk_1C;
    /* 0x20 */ u8   unk_20[4];
    /* 0x24 */ char unk_24[0x4E - 0x24];
    /* 0x4E */ u16  unk_4E;
    /* 0x50 */ char unk_50[0x52 - 0x50];
    /* 0x52 */ s16  unk_52;
    /* 0x54 */ s16  unk_54;
    /* 0x56 */ u16  unk_56;
    /* 0x58 */ char unk_58[0x5C - 0x58];
    /* 0x5C */ u8   unk_5C;
    /* 0x5D */ char unk_5D[0x60 - 0x5D];
    /* 0x60 */ u8   unk_60;
    /* 0x61 */ char unk_61[0x66 - 0x61];
    /* 0x66 */ s16  unk_66;
    /* 0x68 */ s16  unk_68;
    /* 0x6A */ char unk_6A[0x78 - 0x6A];
    /* 0x78 */ s32  unk_78;
    /* 0x7A */ s32  unk_7C;
} UnkStruct_TopMenu;

extern Point data_ov043_020c7914[];

#endif
