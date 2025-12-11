#ifndef GX_BGCNT_H_
#define GX_BGCNT_H_

#include <registers/reg_bgcnt.h>

/// MARK: Typedefs

typedef enum {
    GX_BG_SIZE_TEXT_256x256 = 0,
    GX_BG_SIZE_TEXT_512x256 = 1,
    GX_BG_SIZE_TEXT_256x512 = 2,
    GX_BG_SIZE_TEXT_512x512 = 3,
} GXBGScreenSizeText;

typedef enum {
    GX_BG_SIZE_AFFINE_128x128   = 0,
    GX_BG_SIZE_AFFINE_256x256   = 1,
    GX_BG_SIZE_AFFINE_512x512   = 2,
    GX_BG_SIZE_AFFINE_1024x1024 = 3,
} GXBGScreenSizeAffine;

typedef enum {
    GX_BG_SIZE_PLTT_128x128   = 0,
    GX_BG_SIZE_PLTT_256x256   = 1,
    GX_BG_SIZE_PLTT_512x512   = 2,
    GX_BG_SIZE_PLTT_1024x1024 = 3,
} GXBGScreenSizePltt;

typedef enum {
    GX_BG_SIZE_BMP_128x128 = 0,
    GX_BG_SIZE_BMP_256x256 = 1,
    GX_BG_SIZE_BMP_512x256 = 2,
    GX_BG_SIZE_BMP_512x512 = 3,
} GXBGScreenSizeBmp;

typedef enum {
    GX_BG_SIZE_BMP_LARGE_512x1024 = 0,
    GX_BG_SIZE_BMP_LARGE_1024x512 = 1,
} GXBGScreenSizeBmpLarge;

typedef enum {
    GX_BG_COLORS_16  = 0,
    GX_BG_COLORS_256 = 1,
} GXBGColors;

typedef enum {
    GX_BG_OVERFLOW_TRANSPARENT = 0,
    GX_BG_OVERFLOW_WRAPAROUND  = 1,
} GXBGOverflow;

/// MARK: Main Engine Inlines

static inline void GX_SetBG0Priority(u32 priority) {
    REG_BG0CNT = (REG_BG0CNT & ~0x3) | priority;
}

static inline void GX_SetBG1Priority(u32 priority) {
    REG_BG1CNT = (REG_BG1CNT & ~0x3) | priority;
}

static inline void GX_SetBG2Priority(u32 priority) {
    REG_BG2CNT = (REG_BG2CNT & ~0x3) | priority;
}

static inline void GX_SetBG3Priority(u32 priority) {
    REG_BG3CNT = (REG_BG3CNT & ~0x3) | priority;
}

static inline void GX_SetBG0Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                    u32 extPlttSlot) {
    REG_BG0CNT = (REG_BG0CNT & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                 (extPlttSlot << 13);
}

static inline void GX_SetBG1Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                    u32 extPlttSlot) {
    REG_BG1CNT = (REG_BG1CNT & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                 (extPlttSlot << 13);
}

static inline void GX_SetBG2ControlText(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase) {
    REG_BG2CNT = (REG_BG2CNT & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2);
}

static inline void GX_SetBG2ControlAffine(GXBGScreenSizeAffine screenSize, GXBGOverflow overflow, u32 screenBase,
                                          u32 charBase) {
    REG_BG2CNT = (REG_BG2CNT & 0x43) | (screenSize << 14) | (screenBase << 8) | (charBase << 2) | (overflow << 13);
}

static inline void GX_SetBG2ControlPltt(GXBGScreenSizePltt screenSize, GXBGOverflow overflow, u32 screenBase, u32 charBase) {
    REG_BG2CNT = (REG_BG2CNT & 0x43) | (screenSize << 14) | (charBase << 2) | (screenBase << 8) | (overflow << 13);
}

static inline void GX_SetBG2ControlBmp256(GXBGScreenSizeBmp screenSize, GXBGOverflow overflow, u32 screenBase) {
    REG_BG2CNT = (REG_BG2CNT & 0x43) | (screenSize << 14) | 128 | (screenBase << 8) | (overflow << 13);
}

static inline void GX_SetBG2ControlBmpDirect(GXBGScreenSizeBmp screenSize, GXBGOverflow overflow, u32 screenBase) {
    REG_BG2CNT = (REG_BG2CNT & 0x43) | (screenSize << 14) | 132 | (screenBase << 8) | (overflow << 13);
}

static inline void GX_SetBGControlBmpLarge(GXBGScreenSizeBmpLarge screenSize, GXBGOverflow overflow) {
    REG_BG2CNT = (REG_BG2CNT & 0x43) | (screenSize << 14) | (overflow << 13);
}

static inline void GX_SetBG3Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                    u32 extPlttSlot) {
    REG_BG3CNT = (REG_BG3CNT & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                 (extPlttSlot << 13);
}

static inline void GX_SetBG0Offset(s32 hOffset, s32 vOffset) {
    REG_BG0HOFS = ((hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000));
}

static inline void GX_SetBG1Offset(s32 hOffset, s32 vOffset) {
    REG_BG1HOFS = ((hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000));
}

static inline void GX_SetBG2Offset(s32 hOffset, s32 vOffset) {
    REG_BG2HOFS = ((hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000));
}

static inline void GX_SetBG3Offset(s32 hOffset, s32 vOffset) {
    REG_BG3HOFS = ((hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000));
}

/// MARK: Sub Engine Inlines

static inline void GXs_SetBG0Priority(u32 priority) {
    REG_BG0CNT_SUB = (REG_BG0CNT_SUB & ~0x3) | priority;
}

static inline void GXs_SetBG1Priority(u32 priority) {
    REG_BG1CNT_SUB = (REG_BG1CNT_SUB & ~0x3) | priority;
}

static inline void GXs_SetBG2Priority(u32 priority) {
    REG_BG2CNT_SUB = (REG_BG2CNT_SUB & ~0x3) | priority;
}

static inline void GXs_SetBG3Priority(u32 priority) {
    REG_BG3CNT_SUB = (REG_BG3CNT_SUB & ~0x3) | priority;
}

static inline void GXs_SetBG0Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                     u32 extPlttSlot) {
    REG_BG0CNT_SUB = (REG_BG0CNT_SUB & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                     (extPlttSlot << 13);
}

static inline void GXs_SetBG1Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                     u32 extPlttSlot) {
    REG_BG1CNT_SUB = (REG_BG1CNT_SUB & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                     (extPlttSlot << 13);
}

static inline void GXs_SetBG2ControlText(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase) {
    REG_BG2CNT_SUB = (REG_BG2CNT_SUB & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2);
}

static inline void GXs_SetBG2ControlAffine(GXBGScreenSizeAffine screenSize, GXBGOverflow overflow, u32 screenBase,
                                           u32 charBase) {
    REG_BG2CNT_SUB = (REG_BG2CNT_SUB & 0x43) | (screenSize << 14) | (screenBase << 8) | (charBase << 2) | (overflow << 13);
}

static inline void GXs_SetBG2ControlPltt(GXBGScreenSizePltt screenSize, GXBGOverflow overflow, u32 screenBase, u32 charBase) {
    REG_BG2CNT_SUB = (REG_BG2CNT_SUB & 0x43) | (screenSize << 14) | (charBase << 2) | (screenBase << 8) | (overflow << 13);
}

static inline void GXs_SetBG2ControlBmp256(GXBGScreenSizeBmp screenSize, GXBGOverflow overflow, u32 screenBase) {
    REG_BG2CNT_SUB = (REG_BG2CNT_SUB & 0x43) | (screenSize << 14) | 128 | (screenBase << 8) | (overflow << 13);
}

static inline void GXs_SetBG2ControlBmpDirect(GXBGScreenSizeBmp screenSize, GXBGOverflow overflow, u32 screenBase) {
    REG_BG2CNT_SUB = (REG_BG2CNT_SUB & 0x43) | (screenSize << 14) | 132 | (screenBase << 8) | (overflow << 13);
}

static inline void GXs_SetBG3Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                     u32 extPlttSlot) {
    REG_BG3CNT_SUB = (REG_BG3CNT_SUB & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                     (extPlttSlot << 13);
}

static inline void GXs_SetBG0Offset(s32 hOffset, s32 vOffset) {
    REG_BG0HOFS_SUB = (hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000);
}

static inline void GXs_SetBG1Offset(s32 hOffset, s32 vOffset) {
    REG_BG1HOFS_SUB = (hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000);
}

static inline void GXs_SetBG2Offset(s32 hOffset, s32 vOffset) {
    REG_BG2HOFS_SUB = (hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000);
}

static inline void GXs_SetBG3Offset(s32 hOffset, s32 vOffset) {
    REG_BG3HOFS_SUB = (hOffset & 0x1FF) | ((vOffset << 16) & 0x1FF0000);
}

#endif // GX_BGCNT_H_