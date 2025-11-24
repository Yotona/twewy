#ifndef CRIWARE_ADX_B_H
#define CRIWARE_ADX_B_H

#include <CriWare/private/adx_xpnd.h>
#include <types.h>

typedef struct {
    char pad0[0x10];
} ADX_UNK;

typedef struct {
    /* 0x00 */ s32   unk0;
    /* 0x04 */ s32   unk4;
    /* 0x08 */ s32   unk8;
    /* 0x0C */ s32   unkC;
    /* 0x10 */ s32   unk10;
    /* 0x14 */ void* unk14;
    /* 0x18 */ s32   unk18;
    /* 0x1C */ s32   unk1C;
    /* 0x20 */ s32   unk20;
    /* 0x24 */ s32   unk24;
    /* 0x28 */ s32   unk28;
} ADXB_UNK; // Size: 0x2C

typedef struct ADXB_OBJ {
    /* 0x00 */ s16              unk0;
    /* 0x02 */ s16              unk2;
    /* 0x04 */ s32              stat;
    /* 0x08 */ ADXPD            adxpd;
    /* 0x0C */ s8               encoding_type;
    /* 0x0D */ s8               sample_bitdepth;
    /* 0x0E */ s8               channel_count;
    /* 0x0F */ s8               block_size;
    /* 0x10 */ s32              samples_per_block;
    /* 0x14 */ s32              sample_rate;
    /* 0x18 */ s32              total_samples;
    /* 0x1C */ s16              unk1C;
    /* 0x1E */ char             pad1E[2];
    /* 0x20 */ struct ADXB_OBJ* unk20;
    /* 0x24 */ s16              loop_count;
    /* 0x26 */ s16              unk26;
    /* 0x28 */ s32              unk28;
    /* 0x2C */ s32              unk2C;
    /* 0x30 */ s32              unk30;
    /* 0x34 */ s32              unk34;
    /* 0x38 */ s32              unk38;
    /* 0x3C */ void*            pcm_buf;
    /* 0x40 */ s32              unk40;
    /* 0x44 */ s32              unk44;
    /* 0x48 */ ADXB_UNK         unk48;
    /* 0x74 */ s32              unk74;
    /* 0x78 */ void* (*get_wr)(void*, s32*, s32*, s32*);
    /* 0x7C */ void* object;
    /* 0x80 */ void (*add_wr)(void*, s32, s32);
    /* 0x84 */ s32     unk84;
    /* 0x88 */ s32     unk88;
    /* 0x8C */ s32     unk8C;
    /* 0x90 */ s32     dec_num_sample;
    /* 0x94 */ s32     dec_data_len;
    /* 0x98 */ s16     format;
    /* 0x9A */ s16     unk9A;
    /* 0x9C */ s16     unk9C;
    /* 0x9E */ s16     unk9E;
    /* 0xA0 */ s16     unkA0;
    /* 0xA2 */ s16     unkA2;
    /* 0xA4 */ s16     unkA4;
    /* 0xA6 */ s16     unkA6;
    /* 0xA8 */ s16     unkA8;
    /* 0xAA */ s16     unkAA;
    /* 0xAC */ s32     unkAC;
    /* 0xB0 */ s32     unkB0;
    /* 0xB4 */ s32     unkB4;
    /* 0xB8 */ s32     unkB8;
    /* 0xBC */ s32     unkBC;
    /* 0xC0 */ s32     ainf_len;
    /* 0xC4 */ ADX_UNK unkC4;
    /* 0xD4 */ s16     def_out_vol;
    /* 0xD6 */ s16     def_pan[2];
    /* 0xDA */ char    padDA[2];
    /* 0xDC */ s32     unkDC;
    /* 0xE0 */ s32     unkE0;
    /* 0xE4 */ s32     unkE4;
    /* 0xE8 */ char    padE8[4];
} ADXB_OBJ; // Size: 0xEC

typedef ADXB_OBJ* ADXB;

#endif // CRIWARE_ADX_B_H