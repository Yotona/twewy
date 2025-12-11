#ifndef GX_EFFECTS_H_
#define GX_EFFECTS_H_

#include <registers.h>

/// MARK: Main Engine

static inline void GX_SetBG0Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG0CNT |= 0x40;
    } else {
        REG_BG0CNT &= ~0x40;
    }
}

static inline void GX_SetBG1Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG1CNT |= 0x40;
    } else {
        REG_BG1CNT &= ~0x40;
    }
}

static inline void GX_SetBG2Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG2CNT |= 0x40;
    } else {
        REG_BG2CNT &= ~0x40;
    }
}

static inline void GX_SetBG3Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG3CNT |= 0x40;
    } else {
        REG_BG3CNT &= ~0x40;
    }
}

static inline void GX_SetMosaicSizeBG(u16 sizeX, u16 sizeY) {
    REG_MOSAIC_BG = (u8)(sizeX | (sizeY << 4));
}

static inline void GX_SetMosaicSizeOBJ(u16 sizeX, u16 sizeY) {
    REG_MOSAIC_OBJ = (u8)(sizeX | (sizeY << 4));
}

/// MARK: Sub Engine

static inline void GXs_SetBG0Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG0CNT_SUB |= 0x40;
    } else {
        REG_BG0CNT_SUB &= ~0x40;
    }
}

static inline void GXs_SetBG1Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG1CNT_SUB |= 0x40;
    } else {
        REG_BG1CNT_SUB &= ~0x40;
    }
}

static inline void GXs_SetBG2Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG2CNT_SUB |= 0x40;
    } else {
        REG_BG2CNT_SUB &= ~0x40;
    }
}

static inline void GXs_SetBG3Mosaic(BOOL enabled) {
    if (enabled != FALSE) {
        REG_BG3CNT_SUB |= 0x40;
    } else {
        REG_BG3CNT_SUB &= ~0x40;
    }
}

static inline void GXs_SetMosaicSizeBG(u16 sizeX, u16 sizeY) {
    REG_MOSAIC_BG_SUB = (u8)(sizeX | (sizeY << 4));
}

static inline void GXs_SetMosaicSizeOBJ(u16 sizeX, u16 sizeY) {
    REG_MOSAIC_OBJ_SUB = (u8)(sizeX | (sizeY << 4));
}

#endif // GX_EFFECTS_H_
