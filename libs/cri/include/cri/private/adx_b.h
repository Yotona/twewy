#ifndef CRIWARE_ADX_B_H
#define CRIWARE_ADX_B_H

#include <cri/cri_xpt.h>
#include <cri/private/adx_xpnd.h>

typedef struct {
    char pad0[0x10];
} ADX_UNK;

typedef struct {
    /* 0x00 */ int   unk0;
    /* 0x04 */ int   unk4;
    /* 0x08 */ int   unk8;
    /* 0x0C */ int   unkC;
    /* 0x10 */ int   unk10;
    /* 0x14 */ void* unk14;
    /* 0x18 */ int   unk18;
    /* 0x1C */ int   unk1C;
    /* 0x20 */ int   unk20;
    /* 0x24 */ int   unk24;
    /* 0x28 */ int   unk28;
} ADXB_UNK; // Size: 0x2C

typedef struct ADXB_OBJ {
    /* 0x00 */ short    unk0;
    /* 0x02 */ short    unk2;
    /* 0x04 */ int      stat;
    /* 0x08 */ ADXPD    adxpd;
    /* 0x0C */ char     encoding_type;
    /* 0x0D */ char     sample_bitdepth;
    /* 0x0E */ char     channel_count;
    /* 0x0F */ char     block_size;
    /* 0x10 */ int      samples_per_block;
    /* 0x14 */ int      sample_rate;
    /* 0x18 */ int      total_samples;
    /* 0x1C */ short    unk1C;
    /* 0x1E */ char     pad1E[2];
    /* 0x20 */ int      unk20; // lp_ins_nsmpl
    /* 0x24 */ short    loop_count;
    /* 0x26 */ short    unk26;
    /* 0x28 */ int      unk28;
    /* 0x2C */ int      unk2C;
    /* 0x30 */ int      unk30;
    /* 0x34 */ int      unk34;
    /* 0x38 */ int      unk38;
    /* 0x3C */ void*    pcm_buf;
    /* 0x40 */ int      unk40;
    /* 0x44 */ int      unk44;
    /* 0x48 */ ADXB_UNK unk48;
    /* 0x74 */ int      unk74;
    /* 0x78 */ void* (*get_wr)(void*, int*, int*, int*);
    /* 0x7C */ void* object;
    /* 0x80 */ void (*add_wr)(void*, int, int);
    /* 0x84 */ int     unk84;
    /* 0x88 */ int     unk88;
    /* 0x8C */ int     unk8C;
    /* 0x90 */ int     dec_num_sample;
    /* 0x94 */ int     dec_data_len;
    /* 0x98 */ short   format;
    /* 0x9A */ short   unk9A;
    /* 0x9C */ short   unk9C;
    /* 0x9E */ short   unk9E;
    /* 0xA0 */ short   unkA0;
    /* 0xA2 */ short   unkA2;
    /* 0xA4 */ short   unkA4;
    /* 0xA6 */ short   unkA6;
    /* 0xA8 */ short   unkA8;
    /* 0xAA */ short   unkAA;
    /* 0xAC */ int     unkAC;
    /* 0xB0 */ int     unkB0;
    /* 0xB4 */ int     unkB4;
    /* 0xB8 */ int     unkB8;
    /* 0xBC */ int     unkBC;
    /* 0xC0 */ int     ainf_len;
    /* 0xC4 */ ADX_UNK unkC4;
    /* 0xD4 */ short   def_out_vol;
    /* 0xD6 */ short   def_pan[2];
    /* 0xDA */ char    padDA[2];
    /* 0xDC */ int     unkDC;
    /* 0xE0 */ int     unkE0;
    /* 0xE4 */ int     unkE4;
    /* 0xE8 */ char    padE8[4];
} ADXB_OBJ; // Size: 0xEC

typedef ADXB_OBJ* ADXB;

#endif // CRIWARE_ADX_B_H