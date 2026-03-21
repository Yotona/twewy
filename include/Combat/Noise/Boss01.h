#ifndef COMBAT_NOISE_BOSS01_H
#define COMBAT_NOISE_BOSS01_H

#include <types.h>

/// MARK: BtlObs_RG

typedef struct {
    /* 0x0 */ u16 unk_0;
    /* 0x2 */ u16 unk_2;
    /* 0x4 */ u16 unk_4;
    /* 0x6 */ u16 unk_6;
} Quad;

typedef struct {
    /* 0x00 */ char unk_00[0x2C];
    /* 0x2C */ s32  unk_2C;
    /* 0x30 */ s32  unk_30;
    /* 0x34 */ s32  unk_34;
    /* 0x38 */ char unk_38[0x44 - 0x38];
    /* 0x44 */ s32  unk_44;
    /* 0x48 */ s32  unk_48;
    /* 0x4C */ s32  unk_4C;
    /* 0x50 */ char unk_50[0x5C - 0x50];
    /* 0x5C */ u16  unk_5C;
    /* 0x5E */ char unk_5E[0x74 - 0x5E];
    /* 0x74 */ s16  unk_74;
    /* 0x76 */ s16  unk_76;
} UnkStruct_Boss01;

typedef struct Boss01 {
    /* 0x000 */ char           unk_000[0x2];
    /* 0x002 */ s16            unk_002;
    /* 0x004 */ char           unk_004[0x24 - 0x4];
    /* 0x024 */ s32            unk_24;
    /* 0x028 */ s32            unk_28;
    /* 0x02C */ s32            unk_2C;
    /* 0x030 */ s32            unk_30;
    /* 0x034 */ char           unk_34[0x54 - 0x34];
    /* 0x054 */ s32            unk_54;
    /* 0x058 */ char           unk_58[0x5C - 0x58];
    /* 0x05C */ u32            unk_5C;
    /* 0x060 */ char           unk_60[0x6C - 0x60];
    /* 0x06C */ s16            unk_6C;
    /* 0x06E */ s16            unk_6E;
    /* 0x070 */ s16            unk_70;
    /* 0x072 */ s16            unk_72;
    /* 0x074 */ s16            unk_74;
    /* 0x076 */ s16            unk_76;
    /* 0x078 */ char           unk_78[0x84 - 0x78];
    /* 0x084 */ s32            unk_84;
    /* 0x088 */ char           unk_88[0xCA - 0x88];
    /* 0x0CA */ u16            unk_CA;
    /* 0x0CC */ char           unk_CC[0x184 - 0xCC];
    /* 0x184 */ struct Boss01* unk_184;
    /* 0x188 */ struct Boss01* unk_188;
    /* 0x18C */ u16            unk_18C;
    /* 0x18E */ char           unk_18E[0x1A4 - 0x18E];
    /* 0x1A4 */ s16            unk_1A4;
    /* 0x1A6 */ s16            unk_1A6;
    /* 0x1A8 */ char           unk_1A8[0x1C0 - 0x1A8];
    /* 0x1C0 */ s16            unk_1C0;
    /* 0x1C2 */ s16            unk_1C2;
    /* 0x1C4 */ s16            unk_1C4;
    /* 0x1C6 */ s16            unk_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct Boss01*);
    /* 0x1CC */ void (*unk_1CC)(struct Boss01*);
    /* 0x1D0 */ s32               unk_1D0;
    /* 0x1D4 */ s32               unk_1D4;
    /* 0x1D8 */ s32               unk_1D8;
    /* 0x1DC */ s32               unk_1DC;
    /* 0x1E0 */ UnkStruct_Boss01* unk_1E0;
    /* 0x1E4 */ UnkStruct_Boss01* unk_1E4;
    /* 0x1E8 */ s32               unk_1E8;
    /* 0x1EC */ s32               unk_1EC;
    /* 0x1F0 */ s32               unk_1F0;
} Boss01;

#endif // COMBAT_NOISE_BOSS01_H