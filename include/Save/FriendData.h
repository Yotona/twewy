#ifndef SAVE_FRIENDDATA_H
#define SAVE_FRIENDDATA_H

#include "Save/MainData.h"
#include <nitro/types.h>

typedef struct {
    /* 0x0 */ u8   unk_0[6];
    /* 0x6 */ char unk_6[0xC - 0x6];
} UnkSmallFriendStruct; // Size: 0xC

// A player met in mingle mode.
typedef struct {
    /* 0x00 */ u8             unk_00[6]; // MAC address
    /* 0x06 */ u16            nickName[11];
    /* 0x1C */ u16            message[27];
    /* 0x52 */ u8             timesMet;
    /* 0x53 */ PackedDateTime lastMet;
    /* 0x59 */ char           unk_59[0x5C - 0x59];
    /* 0x5C */ Experience     experience;
    /* 0x68 */ MingleShop     shop;
} UnkLargeFriendStruct; // Size: 0x98

typedef struct GlobalFriendData {
    /* 0x0000 */ UnkLargeFriendStruct unk_0000[50];
    /* 0x1DB0 */ UnkSmallFriendStruct unk_1DB0[50];
} GlobalFriendData; // Size: 0x2008

#endif              // SAVE_FRIENDDATA_H
