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
    GX_BG_COLORS_16  = 0,
    GX_BG_COLORS_256 = 1,
} GXBGColors;

typedef enum {
    GX_BG_OVERFLOW_TRANSPARENT = 0,
    GX_BG_OVERFLOW_WRAPAROUND  = 1,
} GXBGOverflow;

/// MARK: Inlines

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

static inline void GX_SetBG3Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                    u32 extPlttSlot) {
    REG_BG3CNT = (REG_BG3CNT & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                 (extPlttSlot << 13);
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

static inline void GXs_SetBG3Control(GXBGScreenSizeText screenSize, GXBGColors colorMode, u32 screenBase, u32 charBase,
                                     u32 extPlttSlot) {
    REG_BG3CNT_SUB = (REG_BG3CNT_SUB & 0x43) | (screenSize << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) |
                     (extPlttSlot << 13);
}

#endif // GX_BGCNT_H_