#ifndef NITROSDK_GX_GX_STATE_H
#define NITROSDK_GX_GX_STATE_H

#include <types.h>

typedef struct {
    /* 0x00 */ u16 lcdc;
    /* 0x02 */ u16 bg;
    /* 0x04 */ u16 obj;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u16 tex;
    /* 0x0A */ u16 texPltt;
    /* 0x0C */ u16 clearImage;
    /* 0x0E */ u16 bgExtPltt;
    /* 0x10 */ u16 objExtPltt;
    /* 0x12 */ u16 subBg;
    /* 0x14 */ u16 subObj;
    /* 0x16 */ u16 subBgExtPltt;
    /* 0x18 */ u16 subObjExtPltt;
} GXState;

typedef enum {
    GX_VRAM_NONE = 0,
    GX_VRAM_A    = 1 << 0,
    GX_VRAM_B    = 1 << 1,
    GX_VRAM_C    = 1 << 2,
    GX_VRAM_D    = 1 << 3,
    GX_VRAM_E    = 1 << 4,
    GX_VRAM_F    = 1 << 5,
    GX_VRAM_G    = 1 << 6,
    GX_VRAM_H    = 1 << 7,
    GX_VRAM_I    = 1 << 8,

    GX_VRAM_AB = GX_VRAM_A | GX_VRAM_B,
    GX_VRAM_AC = GX_VRAM_A | GX_VRAM_C,
    GX_VRAM_AD = GX_VRAM_A | GX_VRAM_D,
    GX_VRAM_BC = GX_VRAM_B | GX_VRAM_C,
    GX_VRAM_BD = GX_VRAM_B | GX_VRAM_D,
    GX_VRAM_CD = GX_VRAM_C | GX_VRAM_D,
    GX_VRAM_EF = GX_VRAM_E | GX_VRAM_F,
    GX_VRAM_EG = GX_VRAM_E | GX_VRAM_G,
    GX_VRAM_FG = GX_VRAM_F | GX_VRAM_G,
    GX_VRAM_HI = GX_VRAM_H | GX_VRAM_I,

    GX_VRAM_ABC = GX_VRAM_A | GX_VRAM_B | GX_VRAM_C,
    GX_VRAM_ABD = GX_VRAM_A | GX_VRAM_B | GX_VRAM_D,
    GX_VRAM_ACD = GX_VRAM_A | GX_VRAM_C | GX_VRAM_D,
    GX_VRAM_BCD = GX_VRAM_B | GX_VRAM_C | GX_VRAM_D,
    GX_VRAM_EFG = GX_VRAM_E | GX_VRAM_F | GX_VRAM_G,

    GX_VRAM_ABCD = GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D,

    GX_VRAM_ALL = GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D | GX_VRAM_E | GX_VRAM_F | GX_VRAM_G | GX_VRAM_H | GX_VRAM_I,
} GXVRam;

void GX_InitGxState(void);

void GX_SetBankForBg(GXVRam bg);

void GX_SetBankForObj(GXVRam obj);

void GX_SetBankForBgExtPltt(GXVRam bgExtPltt);

void GX_SetBankForObjExtPltt(GXVRam objExtPltt);

void GX_SetBankForTex(GXVRam tex);

void GX_SetBankForTexPltt(GXVRam texPltt);

void GX_SetBankForClearImage(GXVRam clearImage);

void GX_SetBankForLcdc(GXVRam lcdc);

void GX_SetBankForSubBg(GXVRam subBg);

void GX_SetBankForSubObj(GXVRam subObj);

void GX_SetBankForSubBgExtPltt(GXVRam subBgExtPltt);

void GX_SetBankForSubObjExtPltt(GXVRam subObjExtPltt);

GXVRam GX_ResetBankForBgExtPltt(void);

GXVRam GX_ResetBankForObjExtPltt(void);

GXVRam GX_ResetBankForTex(void);

GXVRam GX_ResetBankForTexPltt(void);

GXVRam GX_ResetBankForClearImage(void);

GXVRam GX_ResetBankForSubBg(void);

GXVRam GX_ResetBankForSubObj(void);

GXVRam GX_ResetBankForSubBgExtPltt(void);

GXVRam GX_ResetBankForSubObjExtPltt(void);

GXVRam GX_DisableBankForBg(void);

GXVRam GX_DisableBankForObj(void);

GXVRam GX_DisableBankForObjExtPltt(void);

GXVRam GX_DisableBankForTex(void);

GXVRam GX_DisableBankForTexPltt(void);

GXVRam GX_DisableBankForLcdc(void);

GXVRam GX_GetBankForBg(void);

GXVRam GX_GetBankForObj(void);

GXVRam GX_GetBankForBgExtPltt(void);

GXVRam GX_GetBankForObjExtPltt(void);

GXVRam GX_GetBankForTex(void);

GXVRam GX_GetBankForSubBg(void);

GXVRam GX_GetBankForSubObj(void);

GXVRam GX_GetBankForSubBgExtPltt(void);

GXVRam GX_GetBankForSubObjExtPltt(void);

u32 GX_GetSizeOfBg(void);

u32 GX_GetSizeOfSubBg(void);

#endif // NITROSDK_GX_GX_STATE_H