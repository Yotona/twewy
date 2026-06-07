#ifndef SAVE_FRIENDDATA_H
#define SAVE_FRIENDDATA_H

#include <nitro/types.h>

typedef struct {
    /* 0x0 */ u8   unk_0[6];
    /* 0x6 */ char unk_6[0xC - 0x6];
} UnkSmallFriendStruct; // Size: 0xC

typedef struct {
    /* 0x00 */ u8   unk_00[6];
    /* 0x06 */ char unk_06[0x6A - 0x06];
    /* 0x6A */ u16  unk_6A;
    /* 0x6C */ u16  unk_6C[6];
    /* 0x78 */ u16  unk_78[0x10];
} UnkLargeFriendStruct; // Size: 0x98

typedef struct GlobalFriendData {
    /* 0x0000 */ UnkLargeFriendStruct unk_0000[50];
    /* 0x1DB0 */ UnkSmallFriendStruct unk_1DB0[50];
} GlobalFriendData; // Size: 0x2008

#endif              // SAVE_FRIENDDATA_H
