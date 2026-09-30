#include <cri/private/adx_b.h>
#include <cri/private/adx_dcd.h>

// These macros do not yet match
#define BSWAP_U16(_val) ((unsigned short)((((_val & 0xFF00) >> 8)) | ((_val << 8) & 0xFF00)))
#define BSWAP_S16(_val) ((short)((((_val & 0xFF00) >> 8)) | ((_val << 8) & 0xFF00)))
#define BSWAP_U32(_val)                                                         \
    ((unsigned int)(((_val & 0xFF000000) >> 24) | ((_val << 24) & 0xFF000000) | \
                    (((_val << 8) & 0xFF0000) | ((_val >> 8) & 0xFF00))))
#define BSWAP_S32(_val)                                                                                                      \
    ((int)(((_val & 0xFF000000) >> 24) | ((_val << 24) & 0xFF000000) | (((_val << 8) & 0xFF0000) | ((_val >> 8) & 0xFF00))))

int adx_decode_output_mono_flag = 0;

#define PI   3.14159265f
#define PI_2 (PI * 2)

// Nonmatching
void ADX_GetCoefficient(int highpass_frequency, int sample_rate, short* coef1_ptr, short* coef2_ptr) {
    float f21 = sqrtf(2) - cosf((PI_2 * highpass_frequency) / sample_rate);
    float f20 = sqrtf(2) - 1.0f;

    float r = (f21 - sqrtf((f21 + f20) * (f21 - f20))) / f20;

    *coef1_ptr = 2 * r * 4096;  // Q12(2r)
    *coef2_ptr = -r * r * 4096; // Q12(-r^2)
}

func_020130dc() {}

int ADX_DecodeInfo(ADXHeader* hdr, int arg1, short* audio_offset, char* encoding_type, char* sample_bitdepth, char* block_size,
                   char* channel_count, int* sample_rate, int* total_samples, int* samples_per_block) {
    if (arg1 < 16) {
        return -1;
    }

    if ((unsigned short)(hdr->magic_0 << 8 | hdr->magic_1) != 0x8000) {
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
int ADX_DecodeInfoExADPCM2(ADXHeader* hdr, int arg1, short* high_pass_frequency) {
    unsigned int magic;

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
int ADX_DecodeInfoExVer(ADXHeader* hdr, int arg1, char* version, char* flags) {
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
int ADX_DecodeInfoExIdly(ADXHeader* hdr, int arg1, short* arg2, short* arg3) {
    unsigned char version;
    char          flags;
    unsigned int  magic;

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
int ADX_DecodeInfoExLoop(ADXHeader* hdr, int arg1, int* arg2, short* arg3, short* arg4, int* arg5, int* arg6, int* arg7,
                         int* arg8) {
    unsigned char   version;
    char            flags;
    int             err;
    unsigned int    magic;
    unsigned short* p;

    int temp_t5;
    int temp_a2;

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

    if (BSWAP_U16(((unsigned short*)hdr)[0]) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(((short*)hdr)[1]) < (temp_a2 - 4)) {
        return -1;
    }

    temp_t5 = (version != 4) ? 0x14 : 0x20;

    *arg2 = BSWAP_S16(((short*)(hdr + temp_t5))[0]);
    temp_t5 += 2;
    *arg3 = BSWAP_S16(((short*)(hdr + temp_t5))[0]);
    temp_t5 += 2;

    if (*arg3 != 1) {
        return -2;
    }

    temp_t5 += 2;

    *arg4 = BSWAP_S16(((short*)(hdr + temp_t5))[0]);
    temp_t5 += 2;

    *arg5 = BSWAP_S32(((int*)(hdr + temp_t5))[0]);
    temp_t5 += 4;

    *arg6 = BSWAP_S32(((int*)(hdr + temp_t5))[0]);
    temp_t5 += 4;

    *arg7 = BSWAP_S32(((int*)(hdr + temp_t5))[0]);
    temp_t5 += 4;

    *arg8 = BSWAP_S32(((int*)(hdr + temp_t5))[0]);

    return 0;
}

// Nonmatching
int ADX_DecodeInfoAinf(unsigned char* hdr, int arg1, int* arg2, ADX_UNK* arg3, short* arg4, short* arg5) {
    unsigned char version;
    char          flags;
    int           temp_a2;
    int           temp_t1;
    int           err;
    unsigned int  magic;

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

    if (BSWAP_U16(((unsigned short*)hdr)[0]) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(((short*)hdr)[1]) < (temp_a2 - 4)) {
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

    *arg2 = BSWAP_S32(((int*)(hdr + temp_t1))[0]);
    temp_t1 += 4;

    memcpy(arg3, hdr + temp_t1, sizeof(ADX_UNK));
    temp_t1 += sizeof(ADX_UNK);

    *arg4 = BSWAP_S16(((short*)(hdr + temp_t1))[0]);
    temp_t1 += 4;

    arg5[0] = BSWAP_S16(((short*)(hdr + temp_t1))[0]);
    temp_t1 += 2;

    arg5[1] = BSWAP_S16(((short*)(hdr + temp_t1))[0]);
    temp_t1 += 2;

    return 0;
}

func_02013868() {}

ADX_DecodeMono4() {}

func_02013a90() {}

func_02013ea4() {}

ADX_DecodeSte4() {}
