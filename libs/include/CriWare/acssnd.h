#ifndef ACSSND_H
#define ACSSND_H

#include <CriWare/acsfdr.h>
#include <types.h>

typedef struct {
    /* 0x00 */ char           unk_00[0x0C];
    /* 0x0C */ struct ACSSND* unk_0C;
    /* 0x10 */ char           unk_10[0x10];
    /* 0x20 */ ACSFDR*        fdrHndl;
    /* 0x24 */ char           unk_24[0x34];
    /* 0x58 */ s32            unk_58;
    /* 0x5C */ char           unk_5C[0x4C];
    /* 0xA8 */ s32            numAdxt;
    /* 0xAC */ s8             unk_AC;
    /* 0xB0 */ s32            unk_B0;
    /* 0xB4 */ s32            unk_B4;
    /* 0xB8 */ s32            unk_B8;
    /* 0xBC */ s32            unk_BC;
    /* 0xC0 */ s32            unk_C0;
    /* 0xC4 */ char           unk_C4[0x8];
    /* 0xCC */ s32            unk_CC;
    /* 0xD0 */ s32            unk_D0;
    /* 0xD4 */ char           unk_D4[0xC];
} ACSSND; // Size: 0xE0

ACSSND* ACSSND_Create(ACSSND* work, u16 workSize);

#endif // ACSSND_H
