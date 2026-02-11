#ifndef DEBUG_MORI_GRPCHECK_H
#define DEBUG_MORI_GRPCHECK_H

#include "SpriteMgr.h"
#include "Text.h"

typedef enum {
    RGB_RED   = 0,
    RGB_GREEN = 1,
    RGB_BLUE  = 2,
    RGB_ALPHA = 3,
} RGBAChannel;

typedef struct {
    /* 0x00 */ const char* unk_00;
    /* 0x04 */ char        unk_04[0x14];
    /* 0x18 */ s16         unk_18;
    /* 0x1A */ char        unk_1A[0xA];
} GrpCheckUnk; // Size: 0x24

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ s32    unk_44;
    /* 0x48 */ s32    unk_48;
} GrpCheckBigUnk; // Size: 0x4C

typedef struct {
    /* 0x00 */ const char* unk_00;
    /* 0x04 */ const char* unk_04;
    /* 0x08 */ const char* unk_08;
    /* 0x0C */ s32         unk_0C;
    /* 0x10 */ char        unk_10[0x14];
    /* 0x24 */ s32         unk_24;
    /* 0x28 */ u32         unk_28;
    /* 0x2C */ char        unk_2C[0x4];
} SpriteConfig;

typedef struct {
    /* 0x00000 */ s32            unk_00000;
    /* 0x00004 */ s32            unk_00004;
    /* 0x00008 */ Data*          unk_00008;
    /* 0x0000C */ s32            unk_0000C;
    /* 0x00010 */ char           unk_00010[0x1157C];
    /* 0x1158C */ s32            unk_1158C;
    /* 0x11590 */ s32            bgR; // Background red color channel
    /* 0x11594 */ s32            bgG; // Background green color channel
    /* 0x11598 */ s32            bgB; // Background blue color channel
    /* 0x1159C */ s32            bgA; // Background alpha channel
    /* 0x115A0 */ char           unk_115A0[0xBC];
    /* 0x1165C */ s32            unk_1165C;
    /* 0x11660 */ s32            screenPosX;
    /* 0x11664 */ s32            screenPosY;
    /* 0x11668 */ s32            maxChr;
    /* 0x1166C */ s32            maxAtr;
    /* 0x11670 */ s32            maxLineChr;
    /* 0x11674 */ Sprite         unk_11674;
    /* 0x116B4 */ s32            unk_116B4;
    /* 0x116B8 */ BOOL           hasSprite; // Whether a sprite is currently loaded
    /* 0x116BC */ s32            unk_116BC;
    /* 0x116C0 */ GrpCheckBigUnk unk_116C0[16];
    /* 0x11B80 */ s32            unk_11B80;
    /* 0x11B84 */ BOOL           isAnimDisplayed; // Whether the animation is currently being displayed
    /* 0x11B88 */ s32            unk_11B88;
    /* 0x11B8C */ s32            unk_11B8C;
    /* 0x11B90 */ s16            unk_11B90;
    /* 0x11B92 */ char           unk_11B92[0x2];
    /* 0x11B94 */ BOOL           isPaused;     // Whether the animation is currently paused
    /* 0x11B98 */ BOOL           updateForced; // Whether the frame is being forcefully updated, ignoring pause for one frame
    /* 0x11B9C */ s32            unk_11B9C;
    /* 0x11BA0 */ char           unk_11BA0[0x4];
    /* 0x11BA4 */ s32            unk_11BA4;
    /* 0x11BA8 */ char           unk_11BA8[0x8];
    /* 0x11BB0 */ TextObject     textObj;
    /* 0x11CC4 */ char           unk_11CC4[0x2FC];
    /* 0x11FC0 */ s32            currentRow;         // Currently selected menu row
    /* 0x11FC4 */ BOOL           isVramLimitReached; // Whether the VRAM limit has been reached
} GrpCheckState;                                     // Size: 0x11FC8

#endif                                               // DEBUG_MORI_GRPCHECK_H