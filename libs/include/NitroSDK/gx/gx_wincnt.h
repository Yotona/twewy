#ifndef GX_WINCNT_H_
#define GX_WINCNT_H_

#include <registers.h>

static inline void GX_SetWindow0Inside(u32 window, BOOL effects) {
    u32 temp = (REG_WININ & ~0x3F) | window;
    if (effects != FALSE) {
        temp |= 0x20;
    }
    REG_WININ = (u16)temp;
}

static inline void GX_SetWindow1Inside(u32 window, BOOL effects) {
    u32 temp = (REG_WININ & ~0x3F00) | (window << 8);
    if (effects != FALSE) {
        temp |= (0x20 << 8);
    }
    REG_WININ = (u16)temp;
}

static inline void GX_SetWindowObjInside(u32 window, BOOL effects) {
    u32 temp = (REG_WINOUT & ~0x3F00) | (window << 8);
    if (effects != FALSE) {
        temp |= (0x20 << 8);
    }
    REG_WININ = (u16)temp;
}

static inline void GX_SetWindowOutside(u32 window, BOOL effects) {
    u32 temp = (REG_WINOUT & ~0x3F) | window;
    if (effects != FALSE) {
        temp |= 0x20;
    }
    REG_WINOUT = (u16)temp;
}

static inline void GX_SetWindow0Position(u32 x1, u32 y1, u32 x2, u32 y2) {
    REG_WIN0H = (u16)(((x1 << 8) & 0xFF00) | (x2 & 0xFF));
    REG_WIN0V = (u16)(((y1 << 8) & 0xFF00) | (y2 & 0xFF));
}

static inline void GX_SetWindow1Position(u32 x1, u32 y1, u32 x2, u32 y2) {
    REG_WIN1H = (u16)(((x1 << 8) & 0xFF00) | (x2 & 0xFF));
    REG_WIN1V = (u16)(((y1 << 8) & 0xFF00) | (y2 & 0xFF));
}

static inline void GXs_SetWindow0Inside(u32 window, BOOL effects) {
    u32 temp = (REG_WININ_SUB & ~0x3F) | window;
    if (effects != FALSE) {
        temp |= 0x20;
    }
    REG_WININ_SUB = (u16)temp;
}

static inline void GXs_SetWindow1Inside(u32 window, BOOL effects) {
    u32 temp = (REG_WININ_SUB & ~0x3F00) | (window << 8);
    if (effects != FALSE) {
        temp |= (0x20 << 8);
    }
    REG_WININ_SUB = (u16)temp;
}

static inline void GXs_SetWindowObjInside(u32 window, BOOL effects) {
    u32 temp = (REG_WINOUT_SUB & ~0x3F00) | (window << 8);
    if (effects != FALSE) {
        temp |= (0x20 << 8);
    }
    REG_WININ_SUB = (u16)temp;
}

static inline void GXs_SetWindowOutside(u32 window, BOOL effects) {
    u32 temp = (REG_WINOUT_SUB & ~0x3F) | window;
    if (effects != FALSE) {
        temp |= 0x20;
    }
    REG_WINOUT_SUB = (u16)temp;
}

static inline void GXs_SetWindow0Position(u32 x1, u32 y1, u32 x2, u32 y2) {
    REG_WIN0H_SUB = (u16)(((x1 << 8) & 0xFF00) | (x2 & 0xFF));
    REG_WIN0V_SUB = (u16)(((y1 << 8) & 0xFF00) | (y2 & 0xFF));
}

static inline void GXs_SetWindow1Position(u32 x1, u32 y1, u32 x2, u32 y2) {
    REG_WIN1H_SUB = (u16)(((x1 << 8) & 0xFF00) | (x2 & 0xFF));
    REG_WIN1V_SUB = (u16)(((y1 << 8) & 0xFF00) | (y2 & 0xFF));
}

#endif // GX_WINCNT_H_