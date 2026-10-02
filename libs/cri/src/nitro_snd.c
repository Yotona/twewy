#include <cri/cri_xpt.h>
#include <cri/private/nitro_snd.h>
#include <mem.h>
#include <nitro/mi/cpumem.h>
#include <nitro/os/interrupt.h>

void func_02030458(NNSSndStrm* strm);
BOOL func_020304d0(NNSSndStrm* strm, int numChannels, u8* chNoList);
void func_02030530(NNSSndStrm* strm);
BOOL func_02030558(NNSSndStrm* strm, int format, void* buf, u32 bufSize, int timer, int interval,
                   void (*cb)(int status, int numChannels, void* buf[], u32 len, int format, void* arg), void* arg);
void func_020306f8(NNSSndStrm* strm);
void func_0203074c(NNSSndStrm* strm);
void func_02030768(NNSSndStrm* strm, int vol);
void func_020307d4(NNSSndStrm* strm, int vol, int frames);
void func_02030828(NNSSndStrm* strm, int chNo, int pan);

void func_0201dec0(int status, int numChannels, void* buf[], u32 len, int format, void* arg);
void func_0201e104(NITROSND_CH* ch, u16* freeLen, u16* dataLen);
void func_0201e138(NITROSND_CH* ch, u32 len);
void func_0201e170(NITROSND_CH* ch);
u16  func_0201e358(NITROSND_CH* ch, u16* len1, u16* len2);
s16* func_0201e3b0(NITROSND_CH* ch, s16* src, u32 len);

// Linear interpolation of two samples, and two samples packed into one word.
// PACK is a function rather than a macro: its s16 parameters force the sign
// extensions the target performs before packing.
#define AVG(a, b) ((s16)(((a) + (b)) / 2))
static inline u32 PACK(s16 lo, s16 hi) {
    return (u16)lo | ((u16)hi << 16);
}

int         adx_decode_output_mono_flag;
NITROSND_CH nitrosnd_ch[NITROSND_MAX_CH];

void func_0201dc48(void) {
    __builtin__clear(nitrosnd_ch, sizeof(nitrosnd_ch));
    func_0201dcb8(NULL, 1);
    func_0201dc94(2);
}

void func_0201dc90(void) {}

void func_0201dc94(s16 interval) {
    int i;

    for (i = 0; i < NITROSND_MAX_CH; i++) {
        nitrosnd_ch[i].interval = interval;
    }
}

void func_0201dcb8(NITROSND_CH* ch, s16 resample) {
    int i;

    if (ch == NULL) {
        for (i = 0; i < NITROSND_MAX_CH; i++) {
            nitrosnd_ch[i].resample = resample;
            nitrosnd_ch[i].ratio    = 8;
        }
    } else {
        ch->resample = resample;
    }
}

NITROSND_CH* func_0201dcf4(int idx, void* work, u8 chNo) {
    NITROSND_CH* ch;
    int          i;

    for (i = 0; i < NITROSND_MAX_CH; i++) {
        if (nitrosnd_ch[i].used == 0) {
            break;
        }
    }
    if (i == NITROSND_MAX_CH) {
        return NULL;
    }
    ch = &nitrosnd_ch[i];
    func_02030458(&ch->strm);
    if (func_020304d0(&ch->strm, 1, &chNo) == FALSE) {
        return NULL;
    }
    nitrosnd_ch[i].buf     = (u8*)work + idx * NITROSND_BUF_SIZE;
    nitrosnd_ch[i].timer   = 16;
    nitrosnd_ch[i].bufSize = NITROSND_BUF_SIZE;
    nitrosnd_ch[i].used    = 1;
    nitrosnd_ch[i].chNo    = chNo;
    return &nitrosnd_ch[i];
}

void func_0201ddd8(NITROSND_CH* ch) {
    if (ch != NULL && ch->used != 0) {
        func_0201e0a8(ch);
        func_02030530(&ch->strm);
        ch->cb   = NULL;
        ch->used = 0;
    }
}

BOOL func_0201de08(NITROSND_CH* ch) {
    BOOL ret;

    if (ch->flags & 1) {
        return FALSE;
    }
    MI_CpuFill(0, ch->buf, ch->bufSize);
    func_0201e170(ch);
    ret = func_02030558(&ch->strm, 1, ch->buf, ch->bufSize, ch->timer, ch->interval, func_0201dec0, ch);
    func_02030828(&ch->strm, 0, ch->pan);
    func_02030768(&ch->strm, ch->vol);
    func_020307d4(&ch->strm, 0, 0);
    ch->last = 0;
    ch->flags |= 1;
    return ret;
}

void func_0201dec0(int status, int numChannels, void* buf[], u32 len, int format, void* arg) {
    NITROSND_CH* ch = arg;

    if (status == 1) {
        ch->cbTotal += len;
        if (ch->dataLen != 0) {
            if (len > ch->dataLen) {
                len = ch->dataLen;
            }
            ch->dataLen -= len;
            ch->played += len >> 1;
        }
        if (ch->lock == 0 && ch->cbTotal > ch->wrTotal) {
            ch->wpos    = ch->cbTotal % NITROSND_BUF_SIZE;
            ch->wrTotal = ch->cbTotal;
        }
    } else {
        ch->cbTotal += len;
        ch->wpos    = ch->cbTotal % NITROSND_BUF_SIZE;
        ch->wrTotal = ch->cbTotal;
    }
    ch->freeLen = NITROSND_BUF_SIZE - ch->dataLen;
}

int func_0201df8c(NITROSND_CH* ch, int rate) {
    if (ch == NULL) {
        return 0;
    }
    if (ch->resample != 0) {
        if (rate == NITROSND_UPSAMPLE_RATE / 8) {
            ch->ratio = 1;
            rate      = NITROSND_UPSAMPLE_RATE;
        } else if (rate == NITROSND_UPSAMPLE_RATE / 4) {
            ch->ratio = 2;
            rate      = NITROSND_UPSAMPLE_RATE;
        } else if (rate == NITROSND_UPSAMPLE_RATE * 3 / 8) {
            ch->ratio = 3;
            rate      = NITROSND_UPSAMPLE_RATE;
        } else if (rate == NITROSND_UPSAMPLE_RATE / 2) {
            ch->ratio = 4;
            rate      = NITROSND_UPSAMPLE_RATE;
        } else if (rate == NITROSND_UPSAMPLE_RATE * 3 / 4) {
            ch->ratio = 6;
            rate      = NITROSND_UPSAMPLE_RATE;
        } else {
            ch->ratio = 8;
        }
    }
    ch->timer = 0.5f + 523655.0f / rate;
    return rate;
}

u8* func_0201e06c(NITROSND_CH* ch) {
    if (ch == NULL) {
        return NULL;
    }
    return ch->buf;
}

u32 func_0201e07c(NITROSND_CH* ch) {
    if (ch == NULL) {
        return 0;
    }
    return ch->bufSize / 2;
}

void func_0201e090(NITROSND_CH* ch) {
    func_0201de08(ch);
    func_020306f8(&ch->strm);
}

void func_0201e0a8(NITROSND_CH* ch) {
    u16 freeLen;
    u16 dataLen;

    if (!(ch->flags & 1)) {
        return;
    }
    func_0203074c(&ch->strm);
    ch->flags &= ~1;
    func_0201e104(ch, &freeLen, &dataLen);
    if (dataLen != 0) {
        ch->played += dataLen / 2;
    }
}

void func_0201e104(NITROSND_CH* ch, u16* freeLen, u16* dataLen) {
    BOOL enabled = OS_DisableIME();

    *freeLen = ch->freeLen;
    *dataLen = ch->dataLen;
    OS_RestoreIME(enabled);
}

void func_0201e138(NITROSND_CH* ch, u32 len) {
    BOOL enabled = OS_DisableIME();

    ch->freeLen -= len;
    ch->dataLen += len;
    OS_RestoreIME(enabled);
}

void func_0201e170(NITROSND_CH* ch) {
    BOOL enabled = OS_DisableIME();

    ch->wpos    = 0;
    ch->freeLen = NITROSND_BUF_SIZE;
    ch->dataLen = 0;
    ch->cbTotal = 0;
    ch->wrTotal = 0;
    ch->lock    = 0;
    OS_RestoreIME(enabled);
}

u32 func_0201e1ac(NITROSND_CH* ch, SJCK* ck, u16* outLen) {
    u16 len1;
    u16 len2;
    int mode;
    u16 nbyte;
    u16 mul;
    u16 ratio;
    u16 out;

    if (ck->length == 0) {
        *outLen = 0;
        return 0;
    }
    func_0201e358(ch, &len1, &len2);
    if (len1 == 0) {
        *outLen = 0;
        return 0;
    }
    ratio = ch->ratio;
    nbyte = ck->length;
    if (nbyte > 0xC00) {
        nbyte = 0xC00;
    }
    if (nbyte > len1) {
        nbyte = len1;
    }
    mode = ratio;
    if (mode != 8) {
        if (ratio == 3 || ratio == 6) {
            if (nbyte < 6) {
                *outLen = 0;
                return nbyte;
            }
            nbyte = nbyte / 6 * 6;
            mul   = 0;
            out   = nbyte / ratio * 8;
        } else {
            mul = 8 / ratio;
            out = nbyte * mul;
        }
    } else {
        out = nbyte;
        mul = 1;
    }
    if (out > len1) {
        if (ratio == 3 || ratio == 6) {
            nbyte   = ratio * len1 / 8;
            *outLen = nbyte / ratio * 8;
        } else {
            nbyte -= (out - len1) / mul;
            *outLen = nbyte * mul;
        }
    } else {
        *outLen = out;
    }
    return nbyte;
}

u32 func_0201e328(NITROSND_CH* ch, s16* src, u32 len) {
    func_0201e138(ch, len);
    func_0201e3b0(ch, src, len);
    return len;
}

u16 func_0201e358(NITROSND_CH* ch, u16* len1, u16* len2) {
    u16 freeLen;
    u16 dataLen;
    u16 wpos;
    int end;
    int wrap;

    func_0201e104(ch, &freeLen, &dataLen);
    wpos = ch->wpos;
    end  = wpos + freeLen;
    if (end > NITROSND_BUF_SIZE) {
        *len1 = NITROSND_BUF_SIZE - wpos;
        wrap  = end - NITROSND_BUF_SIZE;
    } else {
        *len1 = freeLen;
        wrap  = 0;
    }
    *len2 = wrap;
    return freeLen;
}

// Nonmatching: logic matches; the x8 branch needs one register too many, so
// len and start both spill (the target keeps len in fp).
s16* func_0201e3b0(NITROSND_CH* ch, s16* src, u32 len) {
    int  start;
    u16  wpos;
    s16* dst;
    int  i;
    int  n;
    s32  w;
    s16  prev;
    s16  lo;
    s16  hi;
    s16  m1;
    s16  m2;
    s16  q1;
    s16  q2;
    s16  q3;
    s16  q4;

    start = ch->wpos;
    wpos  = start + len;
    dst   = (s16*)(ch->buf + start);
    if (wpos >= NITROSND_BUF_SIZE) {
        wpos -= NITROSND_BUF_SIZE;
    }
    ch->lock = 1;
    i        = 0;
    ch->wrTotal += len;
    ch->wpos = wpos;
    ch->lock = 0;

    if (ch->ratio != 8) {
        if (ch->ratio == 6) {
            // 3 samples -> 4
            n = (int)(len >> 1) / 4;
            for (; i < n; i++) {
                lo       = src[0];
                m1       = src[1];
                hi       = src[2];
                dst[0]   = lo;
                dst[1]   = (lo + m1 * 2) / 3;
                dst[2]   = (hi + m1 * 2) / 3;
                dst[3]   = hi;
                ch->last = hi;
                src += 3;
                dst += 4;
            }
        } else if (ch->ratio == 3) {
            // 3 samples -> 8
            n = (int)(len >> 1) / 8;
            for (; i < n; i++) {
                lo       = src[0];
                dst[0]   = AVG(ch->last, lo);
                q1       = src[1];
                hi       = src[2];
                m1       = (lo + q1 * 2) / 3;
                dst[1]   = lo;
                dst[2]   = AVG(lo, m1);
                dst[3]   = m1;
                m2       = (hi + q1 * 2) / 3;
                dst[4]   = AVG(m1, m2);
                dst[5]   = m2;
                dst[6]   = AVG(hi, m2);
                dst[7]   = hi;
                ch->last = hi;
                src += 3;
                dst += 8;
            }
        } else if (ch->ratio == 4) {
            // x2
            n    = len >> 2;
            prev = ch->last;
            while (n > 0) {
                w  = *(s32*)src;
                lo = w;
                src += 2;
                n -= 2;
                m1             = AVG(prev, lo);
                prev           = w >> 16;
                m2             = AVG(prev, lo);
                ((u32*)dst)[0] = PACK(m1, lo);
                ((u32*)dst)[1] = PACK(m2, prev);
                dst += 4;
            }
            ch->last = prev;
        } else if (ch->ratio == 2) {
            // x4
            n    = len >> 3;
            prev = ch->last;
            while (n > 0) {
                w  = *(s32*)src;
                lo = w;
                hi = w >> 16;
                src += 2;
                n -= 2;
                m1             = AVG(prev, lo);
                m2             = AVG(lo, hi);
                ((u32*)dst)[0] = PACK(AVG(prev, m1), m1);
                ((u32*)dst)[1] = PACK(AVG(m1, lo), lo);
                ((u32*)dst)[2] = PACK(AVG(lo, m2), m2);
                ((u32*)dst)[3] = PACK(AVG(m2, hi), hi);
                prev           = hi;
                dst += 8;
            }
            ch->last = prev;
        } else if (ch->ratio == 1) {
            // x8
            n    = (int)(len >> 1) / 8;
            prev = ch->last;
            for (; i < n; i++) {
                w  = *(s32*)src;
                lo = w;
                hi = w >> 16;
                src += 2;
                m1             = AVG(prev, lo);
                q1             = AVG(prev, m1);
                ((u32*)dst)[0] = PACK(AVG(prev, q1), q1);
                m2             = AVG(lo, hi);
                ((u32*)dst)[1] = PACK(AVG(q1, m1), m1);
                q2             = AVG(m1, lo);
                ((u32*)dst)[2] = PACK(AVG(m1, q2), q2);
                q3             = AVG(lo, m2);
                ((u32*)dst)[3] = PACK(AVG(q2, lo), lo);
                ((u32*)dst)[4] = PACK(AVG(lo, q3), q3);
                q4             = AVG(m2, hi);
                ((u32*)dst)[5] = PACK(AVG(q3, m2), m2);
                ((u32*)dst)[6] = PACK(AVG(m2, q4), q4);
                ((u32*)dst)[7] = PACK(AVG(q4, hi), hi);
                prev           = hi;
                dst += 16;
            }
            ch->last = prev;
        }
    } else {
        MI_CpuCopyU8(src, dst, len);
        src += len / 2;
    }
    if (ch->cb != NULL) {
        ch->cb(ch->chNo, ch->buf + start, len);
    }
    return src;
}

u16 func_0201e948(NITROSND_CH* ch) {
    u16 freeLen;
    u16 dataLen;
    u16 len;
    u16 wpos;
    int end;
    u16 len1;
    u16 len2;

    if (ch == NULL) {
        return 0;
    }
    func_0201e104(ch, &freeLen, &dataLen);
    len = freeLen;
    if (len == 0) {
        return 0;
    }
    wpos = ch->wpos;
    end  = wpos + len;
    if (end <= NITROSND_BUF_SIZE) {
        len1 = len;
        len2 = 0;
    } else {
        len1 = NITROSND_BUF_SIZE - wpos;
        len2 = end - NITROSND_BUF_SIZE;
    }
    MI_CpuSet(ch->buf + wpos, 0, len1);
    if (len2 != 0) {
        MI_CpuSet(ch->buf, 0, len2);
    }
    ch->last = 0;
    return len;
}

int func_0201e9ec(NITROSND_CH* ch) {
    if (ch == NULL) {
        return 0;
    }
    return ch->played;
}

u16 func_0201e9fc(NITROSND_CH* ch) {
    if (ch == NULL) {
        return 0;
    }
    return ch->dataLen;
}

u32 func_0201ea0c(NITROSND_CH* ch) {
    if (ch == NULL) {
        return 0;
    }
    return ch->freeLen / 2;
}

s16 func_0201ea20(NITROSND_CH* ch, s16 vol) {
    ch->vol = vol;
    func_02030768(&ch->strm, vol);
    func_020307d4(&ch->strm, 0, 0);
    return vol;
}

void func_0201ea50(NITROSND_CH* ch, int pan) {
    int p;

    if (pan == 0) {
        p = 64;
    } else if (pan == -15) {
        p = 0;
    } else if (pan == 15) {
        p = 127;
    } else {
        p = (pan + 15) * 128 / 30;
    }
    ch->pan = p;
    func_02030828(&ch->strm, 0, p);
}
