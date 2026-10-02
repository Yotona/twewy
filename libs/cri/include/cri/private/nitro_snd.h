#ifndef CRIWARE_NITRO_SND_H
#define CRIWARE_NITRO_SND_H

#include <cri/sj.h>
#include <nitro/types.h>

#define NITROSND_MAX_CH   8
#define NITROSND_BUF_SIZE 0x1800

// Sample rate the stream is set up for when a source is upsampled to it.
#define NITROSND_UPSAMPLE_RATE 0x7FD8

typedef struct {
    /* 0x00 */ u8 data[0x5C];
} NNSSndStrm;

typedef void (*NITROSND_CB)(int chNo, void* buf, u32 len);

typedef struct {
    /* 0x00 */ s16         used;
    /* 0x02 */ s16         chNo;
    /* 0x04 */ u8*         buf;
    /* 0x08 */ int         timer;
    /* 0x0C */ u32         cbTotal;
    /* 0x10 */ u32         wrTotal;
    /* 0x14 */ int         played;
    /* 0x18 */ u16         bufSize;
    /* 0x1A */ u16         lock;
    /* 0x1C */ u16         wpos;
    /* 0x1E */ u16         dataLen;
    /* 0x20 */ u16         freeLen;
    /* 0x22 */ u16         pan;
    /* 0x24 */ s16         vol;
    /* 0x26 */ s16         flags;
    /* 0x28 */ s16         resample;
    /* 0x2A */ u16         ratio;
    /* 0x2C */ s16         last;
    /* 0x2E */ s16         interval;
    /* 0x30 */ NITROSND_CB cb;
    /* 0x34 */ NNSSndStrm  strm;
} NITROSND_CH; // size: 0x90

void         func_0201dc48(void);
void         func_0201dc90(void);
void         func_0201dc94(s16 interval);
void         func_0201dcb8(NITROSND_CH* ch, s16 resample);
NITROSND_CH* func_0201dcf4(int idx, void* work, u8 chNo);
void         func_0201ddd8(NITROSND_CH* ch);
BOOL         func_0201de08(NITROSND_CH* ch);
int          func_0201df8c(NITROSND_CH* ch, int rate);
u8*          func_0201e06c(NITROSND_CH* ch);
u32          func_0201e07c(NITROSND_CH* ch);
void         func_0201e090(NITROSND_CH* ch);
void         func_0201e0a8(NITROSND_CH* ch);
u32          func_0201e1ac(NITROSND_CH* ch, SJCK* ck, u16* outLen);
u32          func_0201e328(NITROSND_CH* ch, s16* src, u32 len);
u16          func_0201e948(NITROSND_CH* ch);
int          func_0201e9ec(NITROSND_CH* ch);
u16          func_0201e9fc(NITROSND_CH* ch);
u32          func_0201ea0c(NITROSND_CH* ch);
s16          func_0201ea20(NITROSND_CH* ch, s16 vol);
void         func_0201ea50(NITROSND_CH* ch, int pan);

#endif // CRIWARE_NITRO_SND_H
