#include <cri/private/adx_b.h>
#include <cri/private/adx_dcd.h>
#include <math.h>

// The swaps shift first and mask second. BSWAP_U16 round-trips through short,
// which is what emits the extra lsl/asr #16 pair ahead of the unsigned compare.
#define BSWAP_S16(_val) ((short)((((_val) >> 8) & 0xFF) | (((_val) << 8) & 0xFF00)))
#define BSWAP_U16(_val) ((unsigned short)BSWAP_S16(_val))
#define BSWAP_S32(_val)                                                                                         \
    ((int)(((((_val) >> 24) & 0xFF) | (((_val) >> 8) & 0xFF00)) | (((_val) << 8) & 0xFF0000) | ((_val) << 24)))

extern int adx_decode_output_mono_flag;

// Sign-extended 4-bit ADPCM nibble. The high nibble gets the same value from an
// arithmetic shift; only the low nibble goes through the table.
const int adx_nibble_tbl[16] = {0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1};
// Added to every decoded sample (and, in ADX_DecodeSte4AsMono, only to the
// second mixed sample of each pair). Neither PS2 reference has it.
int adx_dcd_bias = 32;

#define ADX_CLAMP(_v)                      \
    if ((_v) > 0x7FFF || (_v) < -0x8000) { \
        if ((_v) < -0x8000) {              \
            (_v) = -0x8000;                \
        } else if ((_v) > 0x7FFF) {        \
            (_v) = 0x7FFF;                 \
        }                                  \
    }

#define sqrtf(_x) ((float)sqrt(_x))
#define cosf(_x)  ((float)cos(_x))

#define PI   3.14159265f
#define PI_2 (PI * 2)

void ADX_GetCoefficient(int highpass_frequency, int sample_rate, short* coef1_ptr, short* coef2_ptr) {
    float f21 = sqrtf(2) - cosf((PI_2 * highpass_frequency) / sample_rate);
    float f20 = sqrtf(2) - 1.0f;

    float r = (f21 - sqrtf((f21 + f20) * (f21 - f20))) / f20;

    *coef1_ptr = 2 * r * 4096;  // Q12(2r)
    *coef2_ptr = -r * r * 4096; // Q12(-r^2)
}

int ADX_ScanInfoCode(char* ibuf, int ibuflen, short* dlen) {
    int ptr;
    int minptr = 0x7FFFFFFF;

    for (ptr = 0; ptr < ibuflen - 1; ptr += 2) {
        if (*(unsigned short*)&ibuf[ptr] == 0x80) {
            minptr = (ptr < minptr) ? ptr : minptr;
            break;
        }
    }

    if (minptr != 0x7FFFFFFF) {
        *dlen = minptr;
        return 0;
    } else {
        *dlen = 0;
        return -1;
    }
}

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

int ADX_DecodeInfoExLoop(char* ibuf, int ibuflen, int* lp_ins_nsmpl, short* nloop, short* lp_type, int* lp_spos, int* lp_sofst,
                         int* lp_epos, int* lp_eofst) {
    unsigned char ver;
    char          rev;
    int           err;
    int           lp_inf_len;
    int           ptr;

    *nloop = 0;
    err    = ADX_DecodeInfoExVer((ADXHeader*)ibuf, ibuflen, &ver, &rev);
    if (err != 0) {
        return err;
    }

    lp_inf_len = 24;
    lp_inf_len += 24;
    if (ver == 4) {
        lp_inf_len += 12;
    }

    if (ibuflen < lp_inf_len) {
        return -1;
    }

    if (BSWAP_U16(((unsigned short*)ibuf)[0]) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(((short*)ibuf)[1]) < lp_inf_len - 4) {
        return -1;
    }

    ptr = 20;
    if (ver == 4) {
        ptr += 12;
    }
    ibuf += ptr;

    *lp_ins_nsmpl = BSWAP_S16(*(short*)&ibuf[0]);
    *nloop        = BSWAP_S16(*(short*)&ibuf[2]);
    if (*nloop != 1) {
        return -2;
    }

    *lp_type  = BSWAP_S16(*(short*)&ibuf[6]);
    *lp_spos  = BSWAP_S32(*(int*)&ibuf[8]);
    *lp_sofst = BSWAP_S32(*(int*)&ibuf[12]);
    *lp_epos  = BSWAP_S32(*(int*)&ibuf[16]);
    *lp_eofst = BSWAP_S32(*(int*)&ibuf[20]);

    return 0;
}

// Nonmatching: register allocation only (ptr lands in r0, the target reuses ver's r1)
int ADX_DecodeInfoAinf(unsigned char* ibuf, int ibuflen, int* ainf_len, ADX_UNK* dinf, short* def_vol, short* def_pan) {
    unsigned char ver;
    char          rev;
    int           err;
    int           hdr_len;
    int           ptr;
    short         nloop;
    unsigned int  magic;

    *ainf_len = 0;
    err       = ADX_DecodeInfoExVer((ADXHeader*)ibuf, ibuflen, &ver, &rev);
    if (err != 0) {
        return err;
    }

    hdr_len = 24;
    hdr_len += 36;
    if (ver == 4) {
        hdr_len += 12;
    }

    if (ibuflen < hdr_len) {
        return -1;
    }

    if (BSWAP_U16(((unsigned short*)ibuf)[0]) != 0x8000) {
        return -2;
    }

    if (BSWAP_S16(((short*)ibuf)[1]) < hdr_len - 4) {
        return -1;
    }

    ptr = 20;
    if (ver == 4) {
        ptr += 12;
    }

    nloop = BSWAP_S16(*(short*)(ibuf + ptr + 2));
    ptr += 4;
    if (nloop != 0) {
        ptr += 20;
    }

    magic = (ibuf + ptr)[0] << 24 | (ibuf + ptr)[1] << 16 | (ibuf + ptr)[2] << 8 | (ibuf + ptr)[3];
    if (magic != 0x41494E46) {
        return -2;
    }

    *ainf_len  = BSWAP_S32(*(int*)(ibuf + ptr + 4));
    *dinf      = *(ADX_UNK*)(ibuf + ptr + 8);
    *def_vol   = BSWAP_S16(*(short*)(ibuf + ptr + 24));
    def_pan[0] = BSWAP_S16(*(short*)(ibuf + ptr + 28));
    def_pan[1] = BSWAP_S16(*(short*)(ibuf + ptr + 30));

    return 0;
}

int ADX_DecodeFooter(char* ibuf, int ibuflen, short* dlen) {
    if (ibuflen < 16) {
        return -1;
    }

    if (BSWAP_U16(((unsigned short*)ibuf)[0]) != 0x8001) {
        return -2;
    }

    *dlen = BSWAP_S16(((short*)ibuf)[1]);
    *dlen += 4;

    return 0;
}

// 4-bit ADX ADPCM, 18-byte blocks (2-byte scale + 32 nibbles). The scale is
// XOR-scrambled: key/km/ka are the state, multiplier and increment of the
// 15-bit LCG that ADXPD_SetExtPrm installs.
int ADX_DecodeMono4(char* ibuf, int nblk, short* obuf, short* dly, short k0, short k1, short* key, short km, short ka) {
    int   blkno;
    int   sno;
    int   d0 = dly[0];
    int   d1 = dly[1];
    int   dt;
    int   qsig;
    short code;
    short scale;

    for (blkno = 0; blkno < nblk; blkno++) {
        code = BSWAP_S16(*(short*)ibuf);
        if (code & 0x8000) {
            return blkno;
        }
        ibuf += 2;

        scale = ((code ^ *key) & 0x1FFF) + 1;
        *key  = *key * km + ka;
        *key &= 0x7FFF;

        for (sno = 0; sno < 32; sno += 2) {
            qsig = *ibuf++;

            dt = (qsig >> 4) * scale + ((k0 * d0 + k1 * d1) >> 12);
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            d1      = d0;
            d0      = dt;
            *obuf++ = dt;

            qsig = adx_nibble_tbl[qsig & 0xF];
            dt   = qsig * scale + ((k0 * d0 + k1 * d1) >> 12);
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            d1      = d0;
            d0      = dt;
            *obuf++ = dt;
        }
    }

    dly[0] = d0;
    dly[1] = d1;
    return nblk;
}

int ADX_DecodeSte4AsMono(char* ibuf, int nblk, short* obuf_l, short* dly_l, short* obuf_r, short* dly_r, short k0, short k1,
                         short* key, short km, short ka) {
    int   sno;
    int   blkno;
    int   dt;
    int   qsig_l;
    int   qsig_r;
    int   nblk2 = nblk / 2;
    int   d0_l  = dly_l[0];
    int   d1_l  = dly_l[1];
    int   d0_r  = dly_r[0];
    int   d1_r  = dly_r[1];
    short code;
    short scale_l;
    short scale_r;

    for (blkno = 0; blkno < nblk2; blkno++) {
        code = BSWAP_S16(*(short*)ibuf);
        if (code & 0x8000) {
            return blkno * 2;
        }
        scale_l = ((code ^ *key) & 0x1FFF) + 1;
        *key    = *key * km + ka;
        *key &= 0x7FFF;

        code = BSWAP_S16(*(short*)(ibuf + 18));
        if (code & 0x8000) {
            return blkno * 2;
        }
        scale_r = ((code ^ *key) & 0x1FFF) + 1;
        ibuf += 2;
        *key = *key * km + ka;
        *key &= 0x7FFF;

        for (sno = 0; sno < 32; sno += 2) {
            qsig_l = ibuf[0];
            qsig_r = ibuf[18];
            ibuf++;

            dt = (qsig_l >> 4) * scale_l + ((k0 * d0_l + k1 * d1_l) >> 12);
            ADX_CLAMP(dt);
            d1_l = d0_l;
            d0_l = dt;

            dt = (qsig_r >> 4) * scale_r + ((k0 * d0_r + k1 * d1_r) >> 12);
            ADX_CLAMP(dt);
            d1_r = d0_r;
            d0_r = dt;

            dt = (d0_l + d0_r) * 7 / 10;
            ADX_CLAMP(dt);
            *obuf_l++ = *obuf_r++ = dt;

            qsig_l = adx_nibble_tbl[qsig_l & 0xF];
            qsig_r = adx_nibble_tbl[qsig_r & 0xF];

            dt = qsig_l * scale_l + ((k0 * d0_l + k1 * d1_l) >> 12);
            ADX_CLAMP(dt);
            d1_l = d0_l;
            d0_l = dt;

            dt = qsig_r * scale_r + ((k0 * d0_r + k1 * d1_r) >> 12);
            ADX_CLAMP(dt);
            d1_r = d0_r;
            d0_r = dt;

            // Only the second sample of each pair gets the bias in the original.
            dt = (d0_l + d0_r) * 7 / 10;
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            *obuf_l++ = *obuf_r++ = dt;
        }
        ibuf += 18;
    }

    dly_l[0] = d0_l;
    dly_l[1] = d1_l;
    dly_r[0] = d0_r;
    dly_r[1] = d1_r;
    return nblk;
}

int ADX_DecodeSte4AsSte(char* ibuf, int nblk, short* obuf_l, short* dly_l, short* obuf_r, short* dly_r, short k0, short k1,
                        short* key, short km, short ka) {
    int   sno;
    int   blkno;
    int   dt;
    int   qsig_l;
    int   nblk2 = nblk / 2;
    int   qsig_r;
    int   d0_l = dly_l[0];
    int   d1_l = dly_l[1];
    int   d0_r = dly_r[0];
    int   d1_r = dly_r[1];
    short code;
    short scale_l;
    short scale_r;

    for (blkno = 0; blkno < nblk2; blkno++) {
        code = BSWAP_S16(*(short*)ibuf);
        if (code & 0x8000) {
            return blkno * 2;
        }
        scale_l = ((code ^ *key) & 0x1FFF) + 1;
        *key    = *key * km + ka;
        *key &= 0x7FFF;

        code = BSWAP_S16(*(short*)(ibuf + 18));
        if (code & 0x8000) {
            return blkno * 2;
        }
        scale_r = ((code ^ *key) & 0x1FFF) + 1;
        ibuf += 2;
        *key = *key * km + ka;
        *key &= 0x7FFF;

        for (sno = 0; sno < 32; sno += 2) {
            qsig_l = ibuf[0];
            qsig_r = ibuf[18];
            ibuf++;

            dt = (qsig_l >> 4) * scale_l + ((k0 * d0_l + k1 * d1_l) >> 12);
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            d1_l = d0_l;
            d0_l = dt;

            dt = (qsig_r >> 4) * scale_r + ((k0 * d0_r + k1 * d1_r) >> 12);
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            d1_r = d0_r;
            d0_r = dt;

            *obuf_l++ = d0_l;
            *obuf_r++ = d0_r;

            qsig_l = adx_nibble_tbl[qsig_l & 0xF];
            qsig_r = adx_nibble_tbl[qsig_r & 0xF];

            dt = qsig_l * scale_l + ((k0 * d0_l + k1 * d1_l) >> 12);
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            d1_l = d0_l;
            d0_l = dt;

            dt = qsig_r * scale_r + ((k0 * d0_r + k1 * d1_r) >> 12);
            dt += adx_dcd_bias;
            ADX_CLAMP(dt);
            d1_r = d0_r;
            d0_r = dt;

            *obuf_l++ = d0_l;
            *obuf_r++ = d0_r;
        }
        ibuf += 18;
    }

    dly_l[0] = d0_l;
    dly_l[1] = d1_l;
    dly_r[0] = d0_r;
    dly_r[1] = d1_r;
    return nblk;
}

int ADX_DecodeSte4(char* ibuf, int nblk, short* obuf_l, short* dly_l, short* obuf_r, short* dly_r, short k0, short k1,
                   short* key, short km, short ka) {
    if (adx_decode_output_mono_flag == 0) {
        return ADX_DecodeSte4AsSte(ibuf, nblk, obuf_l, dly_l, obuf_r, dly_r, k0, k1, key, km, ka);
    }
    return ADX_DecodeSte4AsMono(ibuf, nblk, obuf_l, dly_l, obuf_r, dly_r, k0, k1, key, km, ka);
}
