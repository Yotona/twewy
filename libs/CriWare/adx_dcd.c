#include <CriWare/private/adx_b.h>
#include <CriWare/private/adx_dcd.h>

// These macros do not yet match
#define BSWAP_U16(_val) ((u16)((((_val & 0xFF00) >> 8)) | ((_val << 8) & 0xFF00)))
#define BSWAP_S16(_val) ((s16)((((_val & 0xFF00) >> 8)) | ((_val << 8) & 0xFF00)))
#define BSWAP_U32(_val)                                                                                                      \
    ((u32)(((_val & 0xFF000000) >> 24) | ((_val << 24) & 0xFF000000) | (((_val << 8) & 0xFF0000) | ((_val >> 8) & 0xFF00))))
#define BSWAP_S32(_val)                                                                                                      \
    ((s32)(((_val & 0xFF000000) >> 24) | ((_val << 24) & 0xFF000000) | (((_val << 8) & 0xFF0000) | ((_val >> 8) & 0xFF00))))

s32 adx_decode_output_mono_flag = 0;

#define PI   3.14159265f
#define PI_2 (PI * 2)

// Nonmatching
void ADX_GetCoefficient(s32 highpass_frequency, s32 sample_rate, s16* coef1_ptr, s16* coef2_ptr) {
    f32 f21 = sqrtf(2) - cosf((PI_2 * highpass_frequency) / sample_rate);
    f32 f20 = sqrtf(2) - 1.0f;

    f32 r = (f21 - sqrtf((f21 + f20) * (f21 - f20))) / f20;

    *coef1_ptr = 2 * r * 4096;  // Q12(2r)
    *coef2_ptr = -r * r * 4096; // Q12(-r^2)
}

func_020130dc() {}

s32 ADX_DecodeInfo(ADXHeader* hdr, s32 arg1, s16* audio_offset, s8* encoding_type, s8* sample_bitdepth, s8* block_size,
                   s8* channel_count, s32* sample_rate, s32* total_samples, s32* samples_per_block) {
    if (arg1 < 16) {
        return -1;
    }

    if ((u16)(hdr->magic_0 << 8 | hdr->magic_1) != 0x8000) {
        return -2;
    }

    *audio_offset    = ((hdr->copyright_offset_0 << 8) | hdr->copyright_offset_1) + 4;
    *encoding_type   = hdr->encoding_type;
    *block_size      = hdr->block_size;
    *sample_bitdepth = hdr->sample_bitdepth;
    *channel_count   = hdr->channel_count;
    *sample_rate = (hdr->sample_rate_0 << 24) | (hdr->sample_rate_1 << 16) | (hdr->sample_rate_2 << 8) | hdr->sample_rate_3;
    *total_samples =
        (hdr->total_samples_0 << 24) | (hdr->total_samples_1 << 16) | (hdr->total_samples_2 << 8) | hdr->total_samples_3;

    if (*sample_bitdepth == 0) {
        *samples_per_block = 0;
    } else {
        *samples_per_block = (*block_size - 2) * 8 / *sample_bitdepth;
    }

    return 0;
}

// Nonmatching
s32 ADX_DecodeInfoExADPCM2(ADXHeader* hdr, s32 arg1, s16* high_pass_frequency) {
    u32 magic;

    if (arg1 < 0x12) {
        return -1;
    }

    if (BSWAP_U16(hdr->magic) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(hdr->copyright_offset) < 0xE) {
        return -1;
    }

    *high_pass_frequency = BSWAP_S16(hdr->high_pass_frequency);

    return 0;
}

// Nonmatching
s32 ADX_DecodeInfoExVer(ADXHeader* hdr, s32 arg1, s8* version, s8* flags) {
    if (arg1 < 0x14) {
        return -1;
    }

    if (BSWAP_U16(hdr->magic) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(hdr->copyright_offset) < 0x10) {
        return -1;
    }

    *version = hdr->version;
    *flags   = hdr->flags;

    return 0;
}

// Nonmatching
s32 ADX_DecodeInfoExIdly(ADXHeader* hdr, s32 arg1, s16* arg2, s16* arg3) {
    u8  version;
    s8  flags;
    u32 magic;

    if (ADX_DecodeInfoExVer(hdr, arg1, &version, &flags) != 0) {
        return -1;
    }

    if (version >= 4) {
        if (arg1 < 0x20) {
            return -1;
        }

        if (BSWAP_U16(hdr->magic) != 0x8000) {
            return -2;
        }

        if (BSWAP_S16(hdr->copyright_offset) < 0x1C) {
            return -1;
        }

        arg2[0] = BSWAP_S16(hdr->unk18);
        arg3[0] = BSWAP_S16(hdr->unk1A);
        arg2[1] = BSWAP_S16(hdr->unk1C);
        arg3[1] = BSWAP_S16(hdr->unk1E);
    } else {
        arg3[1] = 0;
        arg2[1] = 0;
        arg3[0] = 0;
        arg2[0] = 0;
    }

    return 0;
}

// Nonmatching
s32 ADX_DecodeInfoExLoop(ADXHeader* hdr, s32 arg1, s32* arg2, s16* arg3, s16* arg4, s32* arg5, s32* arg6, s32* arg7,
                         s32* arg8) {
    u8   version;
    s8   flags;
    s32  err;
    u32  magic;
    u16* p;

    s32 temp_t5;
    s32 temp_a2;

    *arg3 = 0;
    err   = ADX_DecodeInfoExVer(hdr, arg1, &version, &flags);

    if (err != 0) {
        return err;
    }

    temp_a2 = 0x30;

    if (version == 4) {
        temp_a2 = 0x3C;
    }

    if (arg1 < temp_a2) {
        return -1;
    }

    if (BSWAP_U16(((u16*)hdr)[0]) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(((s16*)hdr)[1]) < (temp_a2 - 4)) {
        return -1;
    }

    temp_t5 = (version != 4) ? 0x14 : 0x20;

    *arg2 = BSWAP_S16(((s16*)(hdr + temp_t5))[0]);
    temp_t5 += 2;
    *arg3 = BSWAP_S16(((s16*)(hdr + temp_t5))[0]);
    temp_t5 += 2;

    if (*arg3 != 1) {
        return -2;
    }

    temp_t5 += 2;

    *arg4 = BSWAP_S16(((s16*)(hdr + temp_t5))[0]);
    temp_t5 += 2;

    *arg5 = BSWAP_S32(((s32*)(hdr + temp_t5))[0]);
    temp_t5 += 4;

    *arg6 = BSWAP_S32(((s32*)(hdr + temp_t5))[0]);
    temp_t5 += 4;

    *arg7 = BSWAP_S32(((s32*)(hdr + temp_t5))[0]);
    temp_t5 += 4;

    *arg8 = BSWAP_S32(((s32*)(hdr + temp_t5))[0]);

    return 0;
}

// Nonmatching
s32 ADX_DecodeInfoAinf(u8* hdr, s32 arg1, s32* arg2, ADX_UNK* arg3, s16* arg4, s16* arg5) {
    u8  version;
    s8  flags;
    s32 temp_a2;
    s32 temp_t1;
    s32 err;
    u32 magic;

    *arg2 = 0;
    err   = ADX_DecodeInfoExVer((ADXHeader*)hdr, arg1, &version, &flags);

    if (err != 0) {
        return err;
    }

    temp_a2 = 0x3C;

    if (version == 4) {
        temp_a2 = 0x48;
    }

    if (arg1 < temp_a2) {
        return -1;
    }

    if (BSWAP_U16(((u16*)hdr)[0]) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(((s16*)hdr)[1]) < (temp_a2 - 4)) {
        return -1;
    }

    temp_t1 = (version != 4) ? 0x14 : 0x20;
    temp_t1 += 4;

    magic = hdr[temp_t1 + 0] << 24 | hdr[temp_t1 + 1] << 16 | hdr[temp_t1 + 2] << 8 | hdr[temp_t1 + 3];
    temp_t1 += 4;

    // 'AINF' magic
    if (magic != 0x41494E46) {
        return -2;
    }

    *arg2 = BSWAP_S32(((s32*)(hdr + temp_t1))[0]);
    temp_t1 += 4;

    memcpy(arg3, hdr + temp_t1, sizeof(ADX_UNK));
    temp_t1 += sizeof(ADX_UNK);

    *arg4 = BSWAP_S16(((s16*)(hdr + temp_t1))[0]);
    temp_t1 += 4;

    arg5[0] = BSWAP_S16(((s16*)(hdr + temp_t1))[0]);
    temp_t1 += 2;

    arg5[1] = BSWAP_S16(((s16*)(hdr + temp_t1))[0]);
    temp_t1 += 2;

    return 0;
}

func_02013868() {}

ADX_DecodeMono4() {}

func_02013a90() {}

func_02013ea4() {}

ADX_DecodeSte4() {}
