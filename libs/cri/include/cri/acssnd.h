#ifndef ACSSND_H
#define ACSSND_H

#include <cri/acsfdr.h>
#include <cri/cri_xpt.h>

typedef struct {
    /* 0x00 */ char           unk_00[0x0C];
    /* 0x0C */ struct ACSSND* unk_0C;
    /* 0x10 */ char           unk_10[0x10];
    /* 0x20 */ ACSFDR*        fdrHndl;
    /* 0x24 */ char           unk_24[0x34];
    /* 0x58 */ int            unk_58;
    /* 0x5C */ char           unk_5C[0x4C];
    /* 0xA8 */ int            numAdxt;
    /* 0xAC */ char           unk_AC;
    /* 0xB0 */ int            unk_B0;
    /* 0xB4 */ int            unk_B4;
    /* 0xB8 */ int            unk_B8;
    /* 0xBC */ int            unk_BC;
    /* 0xC0 */ int            unk_C0;
    /* 0xC4 */ char           unk_C4[0x8];
    /* 0xCC */ int            unk_CC;
    /* 0xD0 */ int            unk_D0;
    /* 0xD4 */ char           unk_D4[0xC];
} ACSSND; // Size: 0xE0

ACSSND* ACSSND_Create(ACSSND* work, unsigned short workSize);

#endif // ACSSND_H
