#include <cri/private/adx_b.h>
#include <cri/private/adx_bsc_tbl.h>

#define ADXB_MAX_OBJ 4
#define MIN(a, b)    ((a) <= (b) ? (a) : (b))

int skg_init_count                 = 0;
int skg_err_func                   = 0;
int skg_err_obj                    = 0;
void (*ahxsetextfunc)(int, short*) = NULL;
int pl2encodefunc                  = 0;
void (*pl2resetfunc)()             = NULL;
short    adxb_def_k0               = 0;
short    adxb_def_km               = 0;
short    adxb_def_ka               = 0;
ADXB_OBJ adxb_obj[ADXB_MAX_OBJ]    = {0};

int data_0206b87c;

// forward decls
void ADXB_Destroy(ADXB adxb);
int  adxb_get_key(ADXB adxb, unsigned char arg1, unsigned char arg2, int arg3, short* arg4, short* arg5, short* arg6);
void func_02012ed8(ADXB adxb);

static int SKG_Init(void) {
    skg_init_count++;
    return 0;
}

static int SKG_GenerateKey(char* arg0, int arg1, unsigned short* arg2, unsigned short* arg3, unsigned short* arg4) {
    short var_a0;
    short var_t0;
    short var_t2;
    int   i;

    if (skg_init_count == 0) {
        SKG_Init();
    }

    *arg2 = 0;
    *arg3 = 0;
    *arg4 = 0;

    if (arg0 == 0 && arg1 <= 0) {
        return 0;
    }

    var_t2 = skg_prim_tbl[0x100];
    for (i = 0; i < arg1; i++) {
        var_t2 = skg_prim_tbl[(var_t2 * skg_prim_tbl[arg0[i] + 0x80]) % 0x400];
    }

    var_t0 = skg_prim_tbl[0x200];
    for (i = 0; i < arg1; i++) {
        var_t0 = skg_prim_tbl[(var_t0 * skg_prim_tbl[arg0[i] + 0x80]) % 0x400];
    }

    var_a0 = skg_prim_tbl[0x300];
    for (i = 0; i < arg1; i++) {
        var_a0 = skg_prim_tbl[(var_a0 * skg_prim_tbl[arg0[i] + 0x80]) % 0x400];
    }

    *arg2 = var_t2;
    *arg3 = var_t0;
    *arg4 = var_a0;
    return 0;
}

void func_02012248(int arg0) {
    data_0206b87c = arg0;
}

int func_02012258(void) {
    return data_0206b87c;
}

void ADXB_Init() {
    ADXPD_Init();
    SKG_Init();
    __builtin__clear(adxb_obj, sizeof(adxb_obj));
    func_02012248(0);
}

void* adxb_DefGetWr(void* object, int* arg1, int* arg2, int* arg3) {
    ADXB adxb = (ADXB)object;

    *arg1 = adxb->unk8C;
    *arg2 = adxb->unk40 - adxb->unk8C;
    *arg3 = adxb->total_samples - adxb->unk88;

    return adxb->pcm_buf;
}

void adxb_DefAddWr(void* object, int arg1, int arg2) {
    ADXB adxb = (ADXB)object;

    adxb->unk8C += arg2;
    adxb->unk88 += arg2;
}

// adxb_clear. The 16 bytes at 0xC4 are unkC4. The target inlines this clear as
// four strb per iteration over four iterations; a memset() with a constant size
// is left as a real call by mwcc, so it has to be spelled out for the shape to
// come out.
void func_020122fc(ADXB adxb) {
    char* p = (char*)&adxb->unkC4;
    int   i;

    adxb->ainf_len    = 0;
    adxb->def_out_vol = 0;
    adxb->def_pan[0]  = -0x80;
    adxb->def_pan[1]  = -0x80;

    for (i = 0; i < 4; i++) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p += 4;
    }
}

ADXB ADXB_Create(int arg0, void* arg1, int arg2, int arg3) {
    ADXB  adxb;
    ADXB  chk_adxb;
    ADXPD adxpd;
    int   i;

    chk_adxb = &adxb_obj[0];

    for (i = 0; i < ADXB_MAX_OBJ; i++, chk_adxb++) {
        if (chk_adxb->unk0 == 0) {
            break;
        }
    }

    if (i == ADXB_MAX_OBJ) {
        return NULL;
    }

    adxb = &adxb_obj[i];
    memset(adxb, 0, sizeof(ADXB_OBJ));
    adxb->unk0  = 1;
    adxpd       = ADXPD_Create();
    adxb->adxpd = adxpd;

    if (adxpd == NULL) {
        ADXB_Destroy(adxb);
        return NULL;
    }

    adxb->unk38   = arg0;
    adxb->pcm_buf = arg1;
    adxb->unk40   = arg2;
    adxb->unk44   = arg3;
    adxb->get_wr  = adxb_DefGetWr;
    adxb->object  = adxb;
    adxb->add_wr  = adxb_DefAddWr;
    adxb->unk84   = adxb;
    func_020122fc(adxb);
    return adxb;
}

void ADXB_Destroy(ADXB adxb) {
    if (adxb != NULL) {
        ADXPD adxpd = adxb->adxpd;
        adxb->adxpd = 0;
        ADXPD_Destroy(adxpd);
        memset(adxb, 0, sizeof(ADXB_OBJ));
        adxb->unk0 = 0;
    }
}

int ADXB_DecodeHeaderAdx(ADXB adxb, void* header, int len) {
    short sp10[2];
    short sp20[2];
    short sp36;
    short sp34;
    short sp32;
    short sp30;
    short audio_offset;
    char  version;
    char  flags;
    short sp44;
    short sp46;
    short sp48;

    adxb->unk2 = 1;

    if (ADX_DecodeInfo(header, len, &audio_offset, &adxb->encoding_type, &adxb->sample_bitdepth, &adxb->block_size,
                       &adxb->channel_count, &adxb->sample_rate, &adxb->total_samples, &adxb->samples_per_block) < 0)
    {
        return 0;
    }

    if (adxb->encoding_type > 4) {
        if (adxb->unkB4 == 0) {
            ADXERR_CallErrFunc2("E1060101 ADXB_DecodeHeaderAdx: ", "can't play AHX data by this handle");
            return -1;
        }

        adxb->sample_bitdepth   = 8;
        adxb->block_size        = adxb->channel_count * 0xC0;
        adxb->samples_per_block = 0x60;
        adxb->format            = 0xA;
        adxb->unk1C             = 0;
        adxb->loop_count        = 0;
        adxb->unk26             = 0;
        adxb->unk20             = NULL;
        adxb->unk28             = 0;
        adxb->unk2C             = 0;
        adxb->unk30             = 0;
        adxb->unk34             = 0;
        adxb->unk88             = 0;

        if (ADX_DecodeInfoExVer(header, len, &version, &flags) < 0) {
            return 0;
        }

        sp30 = 0;

        if (adxb_get_key(adxb, version, flags, adxb->total_samples, &sp32, &sp34, &sp36) < 0) {
            return -1;
        }

        if (ahxsetextfunc != NULL) {
            ahxsetextfunc(adxb->unkB4, &sp30);
        }
    } else {
        if (ADX_DecodeInfoExVer(header, len, &version, &flags) < 0) {
            return 0;
        }

        if (adxb_get_key(adxb, version, flags, adxb->total_samples, &sp44, &sp46, &sp48) < 0) {
            return -1;
        }

        ADXPD_SetExtPrm(adxb->adxpd, sp44, sp46, sp48);

        if (ADX_DecodeInfoExADPCM2(header, len, &adxb->unk1C) < 0) {
            return 0;
        }

        if (ADX_DecodeInfoExIdly(header, len, &sp10, &sp20) < 0) {
            return 0;
        }

        ADXPD_SetCoef(adxb->adxpd, adxb->sample_rate, adxb->unk1C);
        ADXPD_SetDly(adxb->adxpd, &sp10, &sp20);
        ADX_DecodeInfoExLoop(header, len, &adxb->unk20, &adxb->loop_count, &adxb->unk26, &adxb->unk28, &adxb->unk2C,
                             &adxb->unk30, &adxb->unk34);
        ADX_DecodeInfoAinf(header, len, &adxb->ainf_len, &adxb->unkC4, &adxb->def_out_vol, &adxb->def_pan);
        adxb->format = 0;
    }

    adxb->unk48.unk8  = adxb->channel_count;
    adxb->unk48.unkC  = adxb->block_size;
    adxb->unk48.unk10 = adxb->samples_per_block;
    adxb->unk48.unk14 = adxb->pcm_buf;
    adxb->unk48.unk18 = adxb->unk40;
    adxb->unk48.unk1C = adxb->unk44;
    adxb->unk8C       = 0;

    return audio_offset;
}

void func_02012748(ADXB adxb) {
    adxb->unk2              = 1;
    adxb->sample_rate       = 48000;
    adxb->channel_count     = 2;
    adxb->sample_bitdepth   = 16;
    adxb->total_samples     = 0x7fffffff;
    adxb->block_size        = 127;
    adxb->samples_per_block = 1024;
    adxb->format            = adxb->unk9A;
    adxb->unk48.unk8        = adxb->channel_count;
    adxb->unk48.unkC        = adxb->block_size;
    adxb->unk48.unk10       = adxb->samples_per_block;
    adxb->unk48.unk14       = adxb->pcm_buf;
    adxb->unk48.unk18       = adxb->unk40;
    adxb->unk48.unk1C       = adxb->unk44;
    adxb->unk8C             = 0;
    adxb->unk1C             = 0;
    adxb->loop_count        = 0;
    adxb->unk26             = 0;
    adxb->unk20             = NULL;
    adxb->unk28             = 0;
    adxb->unk2C             = 0;
    adxb->unk30             = 0;
    adxb->unk34             = 0;
    adxb->unk88             = 0;
}

int func_020127ec(ADXB adxb, unsigned short* header, int len) {
    func_020122fc(adxb);

    unsigned short temp = *header;
    if ((unsigned short)(short)((unsigned char)((int)temp >> 8) | ((temp << 8) & 0xFF00)) != 0x8000) {
        return -1;
    }
    return ADXB_DecodeHeaderAdx(adxb, header, len);
}

void ADXB_EntryGetWrFunc(ADXB adxb, void* (*get_wr)(void*, int*, int*, int*), void* object) {
    adxb->get_wr = get_wr;
    adxb->object = object;
}

void* ADXB_GetPcmBuf(ADXB adxb) {
    return adxb->pcm_buf;
}

int ADXB_GetFormat(ADXB adxb) {
    return adxb->format;
}

int ADXB_GetSfreq(ADXB adxb) {
    return adxb->sample_rate;
}

int ADXB_GetNumChan(ADXB adxb) {
    if (adxb == NULL) {
        ADXERR_CallErrFunc1("E2005042701 : NULL pointer is passed.");
        return -1;
    }

    return adxb->channel_count;
}

int ADXB_GetBitdepth(ADXB adxb) {
    return adxb->sample_bitdepth;
}

int ADXB_GetOutBps(ADXB adxb) {
    return 16;
}

int ADXB_GetBlkSmpl(ADXB adxb) {
    return adxb->samples_per_block;
}

int ADXB_GetTotalNumSmpl(ADXB adxb) {
    return adxb->total_samples;
}

int ADXB_GetNumLoop(ADXB adxb) {
    return adxb->loop_count;
}

int func_020128b8(ADXB adxb) {
    return adxb->unk28;
}

int func_020128c0(ADXB adxb) {
    if (adxb == NULL) {
        return 0;
    }
    return adxb->unk2C;
}

int func_020128d0(ADXB adxb) {
    return adxb->unk30;
}

int func_020128d8(ADXB adxb) {
    return adxb->unk34;
}

int ADXB_GetAinfLen(ADXB adxb) {
    return adxb->ainf_len;
}

short ADXB_GetDefOutVol(ADXB adxb) {
    return adxb->def_out_vol;
}

short ADXB_GetDefPan(ADXB adxb, int arg1) {
    return adxb->def_pan[arg1];
}

void func_020128fc(ADXB adxb) {
    ADXPD_GetDly(adxb->adxpd, &adxb->unkAC, &adxb->unkB0);
    ADXPD_GetExtPrm(adxb->adxpd, &adxb->unkA6, &adxb->unkA8, &adxb->unkAA);
}

void func_0201292c(ADXB adxb) {
    ADXPD_SetDly(adxb->adxpd, &adxb->unkAC, &adxb->unkB0);
    ADXPD_SetExtPrm(adxb->adxpd, adxb->unkA6, adxb->unkA8, adxb->unkAA);
}

int adxb_get_key(ADXB adxb, unsigned char arg1, unsigned char arg2, int arg3, short* arg4, short* arg5, short* arg6) {
    char sp[16];

    if (arg1 < 4) {
        *arg4 = 0;
        *arg5 = 0;
        *arg6 = 0;
    } else {
        if (arg2 >= 0x10) {
            func_0202165c(sp, sizeof(sp), "%08X", arg3);
            SKG_GenerateKey(sp, 8, arg4, arg5, arg6);
        } else if (arg2 >= 8) {
            if ((adxb->unkA0 == 0) && (adxb->unkA2 == 0) && (adxb->unkA4 == 0)) {
                adxb->unkA0 = adxb_def_k0;
                adxb->unkA2 = adxb_def_km;
                adxb->unkA4 = adxb_def_ka;
            }

            *arg4 = adxb->unkA0;
            *arg5 = adxb->unkA2;
            *arg6 = adxb->unkA4;
        } else {
            *arg4 = 0;
            *arg5 = 0;
            *arg6 = 0;
        }
    }

    return 0;
}

int ADXB_GetStat(ADXB adxb) {
    return adxb->stat;
}

void ADXB_EntryData(ADXB adxb, int arg1, int arg2) {
    if (adxb->format == 0) {
        adxb->unk48.unk0 = arg1;
        adxb->unk48.unk4 = arg2 / adxb->block_size;
        adxb->unk74      = 0;
    } else {
        adxb->unk48.unk0 = arg1;
        adxb->unk48.unk4 = arg2 / ((adxb->sample_bitdepth / 8) * adxb->channel_count);
        adxb->unk74      = 0;
    }
    adxb->dec_num_sample = 0;
    adxb->dec_data_len   = 0;
    adxb->unkE0          = 0;
    adxb->unkDC          = 0;
}

void ADXB_Start(ADXB adxb) {
    if (adxb->stat == 0) {
        adxb->stat = 1;
    }
}

void ADXB_Stop(ADXB adxb) {
    ADXPD_Stop(adxb->adxpd);
    adxb->stat = 0;
}

void ADXB_Reset(ADXB adxb) {
    if (adxb->stat == 3) {
        ADXPD_Reset(adxb->adxpd);
        adxb->unk8C = 0;
        adxb->stat  = 0;
    }
}

int ADXB_GetDecDtLen(ADXB adxb) {
    return adxb->dec_data_len;
}

int ADXB_GetDecNumSmpl(ADXB adxb) {
    return adxb->dec_num_sample;
}

// Nonmatching: Wrong instruction order
void ADXB_EvokeExpandMono(ADXB arg0, int arg1) {
    ADXPD     temp_r4 = arg0->adxpd;
    ADXB_UNK* unk     = &arg0->unk48;

    ADXPD_EntryMono(temp_r4, unk->unk0, arg1, unk->unk14 + (unk->unk20 * 2), 0);
    ADXPD_Start(temp_r4);
}

void ADXB_EvokeExpandSte(ADXB arg0, int arg1) {
    ADXPD     temp_r4 = arg0->adxpd;
    ADXB_UNK* unk     = &arg0->unk48;
    int       a3;
    int       t0;

    // These are two distinct arguments to ADXPD_EntrySte: a3 goes to unk20 and
    // t0 to unk24 (the extra/inter-channel buffer that ADXPD_ExecHndl hands to
    // ADX_DecodeSte4). Passing only their sum left unk24 unwritten.
    a3 = unk->unk14 + (unk->unk20 * 2);
    t0 = a3 + (unk->unk1C * 2);

    ADXPD_EntrySte(temp_r4, unk->unk0, arg1 * 2, a3, t0);
    ADXPD_Start(temp_r4);
}

void ADXB_EvokeDecode(ADXB adxb) {
    ADXB_UNK* unk = &adxb->unk48;

    int var_a3;
    int temp_t0;
    int temp_a1_2;
    int temp_t7;
    int var_t4;
    int var_t3;
    int var_t1;
    int temp_lo;
    int temp_lo_3;
    int temp_lo_2;

    temp_lo = unk->unk4 / unk->unk8;

    var_t1  = unk->unk10;
    temp_t7 = unk->unk18;
    temp_t0 = unk->unk20;
    var_a3  = unk->unk28;

    var_t4 = unk->unk24;

    temp_lo_2 = (var_a3 + var_t1 - 1) / var_t1;
    temp_a1_2 = var_t1 - (var_a3 + var_t1 - 1) % var_t1 - 1;
    var_t3    = ((var_t1 + (unk->unk18 - temp_t0)) - 1) / var_t1;

    temp_lo_3 = var_t3 * var_t1;

    if (temp_lo_2 < var_t3) {
        if ((temp_t0 + (temp_lo_3)-temp_a1_2) < unk->unk18) {
            var_t3 += 1;
        }
    }

    if (var_a3 < var_t4) {
        var_t4 += temp_a1_2;
    }

    temp_lo = MIN(temp_lo, var_t4 / var_t1);
    temp_lo = MIN(temp_lo, temp_lo_2);
    temp_lo = MIN(temp_lo, var_t3);

    if (unk->unk8 == 2) {
        ADXB_EvokeExpandSte(adxb, temp_lo);
    } else {
        ADXB_EvokeExpandMono(adxb, temp_lo);
    }
}

void memcpy2(void* dest, const void* src, int count) {
    short*       _dest = dest;
    const short* _src  = src;

    while (count > 0) {
        *_dest++ = *_src++;
        count -= 1;
    }
}

void ADXB_CopyExtraBufSte(void* arg0, int arg1, int arg2, int arg3) {
    memcpy2(arg0, arg0 + (arg1 * 2), arg3);
    memcpy2(arg0 + (arg2 * 2), arg0 + ((arg2 + arg1) * 2), arg3);
}

void ADXB_CopyExtraBufMono(void* arg0, int arg1, int arg2, int arg3) {
    memcpy2(arg0, arg0 + (arg1 * 2), arg3);
}

void ADXB_EndDecode(ADXB adxb) {
    int       s1, s2, sp0, s3, s0, _s0, _s1, v0, v1, temp_div;
    int       s7;
    ADXB_UNK* s5 = &adxb->unk48;
    void*     s8 = s5->unk14;
    int       s4;
    int       tmp1, tmp3;

    s3  = s5->unkC;
    s1  = s5->unk10;
    s4  = s5->unk20;
    sp0 = adxb->unk44;
    s7  = adxb->unk40;

    temp_div = s5->unk28 + s1 - 1;
    s0       = temp_div % s1;
    _s0      = s1 - s0 - 1;
    s2       = temp_div / s1;

    v0 = ADXPD_GetNumBlk(adxb->adxpd);

    tmp1                 = v0 * s1;
    s1                   = tmp1;
    s2                   = s2 * s5->unk8;
    tmp3                 = v0 * s3;
    s3                   = tmp3;
    s1                   = s1 / s5->unk8;
    adxb->dec_num_sample = (v0 < s2) ? s1 : s1 - _s0;
    s4 += adxb->dec_num_sample;
    adxb->dec_data_len = s3;

    if (s4 >= s7) {
        s4 -= s7;

        if ((s5->unk8 == 2)) {
            ADXB_CopyExtraBufSte(s8, s7, sp0, s4);
        } else {
            ADXB_CopyExtraBufMono(s8, s7, sp0, s4);
        }
    }
}

void ADXB_ExecOneAdx(ADXB adxb) {
    if (adxb->stat == 1) {
        if (ADXPD_GetStat(adxb->adxpd) == 0) {
            adxb->get_wr(adxb->object, &adxb->unk48.unk20, &adxb->unk48.unk24, &adxb->unk48.unk28);
            ADXB_EvokeDecode(adxb);
            adxb->stat = 2;
        }
    }

    if (adxb->stat == 2) {
        ADXPD_ExecHndl(adxb->adxpd);

        if (ADXPD_GetStat(adxb->adxpd) == 3) {
            ADXB_EndDecode(adxb);
            ADXPD_Reset(adxb->adxpd);
            adxb->add_wr(adxb->unk84, adxb->dec_data_len, adxb->dec_num_sample);
            adxb->stat = 3;
        }
    }
}

void ADXB_ExecHndl(ADXB adxb) {
    if (adxb->format == 0) {
        ADXB_ExecOneAdx(adxb);
    } else if (adxb->format == 10) {
        ADXB_ExecOneAhx(adxb);
    }

    if (adxb->unkE4 != 0) {
        func_02012ed8(adxb);
    }
}

void func_02012ed8(ADXB adxb) {
    // not yet implemented
}