#include "Combat/Noise/Private/Boss01.h"

typedef struct {
    /* 0x00 */ Ov016SpriteParams        params;
    /* 0x08 */ const void*              unk8;
    /* 0x0C */ const Ov016SpriteParams* unkC;
    /* 0x10 */ u16                      unk10;
    /* 0x12 */ u16                      unk12;
    /* 0x14 */ u16                      unk14;
    /* 0x16 */ u16                      unk16;
} Ov016AnimConfig;

const BinIdentifier data_ov016_02128f88 = {3, "Apl_Mor/Grp_Boss01b.bin"};
const BinIdentifier data_ov016_02128f90 = {3, "Apl_Mor/Grp_Boss01c.bin"};
const BinIdentifier data_ov016_02128f98 = {3, "Apl_Mor/Grp_Boss01s.bin"};

const Ov016AnimConfig data_ov016_02128f70 = {
    {1, 3, 2, 0},
    &data_ov016_02128f88, (const Ov016SpriteParams*)&data_ov016_02128f70, 0, 4, 0xD0, 0
};

// Nonmatching: Significant variation, needs rewritten
void func_ov016_021256c0(SpriteAnimationEx* arg0, s32 arg1, s32 arg2) {
    if (arg1 != 2) {
        CombatSprite_InitAnim(&arg0->anim, arg1, &data_ov016_02128f90);
    } else {
        CombatSprite_InitAnim(&arg0->anim, arg1, &data_ov016_02128f88);
    }

    const Ov016SpriteParams* temp_r6 = data_ov016_02128f70.unkC;
    arg0->anim.bits_10_11            = 0;
    arg0->anim.unk_1C                = temp_r6->unk0;
    arg0->anim.unk_26                = temp_r6->unk4;
    arg0->anim.unk_28                = temp_r6->unk2;
    arg0->anim.unk_20                = 4;
    arg0->anim.unk_2A                = (temp_r6->unk6 + 1);
    arg0->unk_2C                     = 0;
    arg0->anim.unk_04                = 0x80;
    arg0->anim.unk_06                = 0x60;

    u16 temp_r2 = arg0->anim.unk_02.raw;

    u16 var_r1;
    if (arg2 != 1) {
        var_r1 = ((u32)(temp_r2 << 0x16) >> 0x1B) & ~8;
    } else {
        var_r1 = ((u32)(temp_r2 << 0x16) >> 0x1B) | 8;
    }
    arg0->anim.unk_02.raw = (temp_r2 & ~0x3E0) | ((u32)(var_r1 << 0x1B) >> 0x16);
}
