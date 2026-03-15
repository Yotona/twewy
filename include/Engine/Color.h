#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H

#include <types.h>

#define RED_SHIFT   00
#define GREEN_SHIFT 05
#define BLUE_SHIFT  10
#define ALPHA_SHIFT 15

void Color_CopyRange(u16* dest, u16* src, s32 destOffset, u32 count, BOOL skipFirst);

static inline u16 Color_PackRGB(u32 red, u32 green, u32 blue, u32 alpha) {
    return (u16)((alpha << ALPHA_SHIFT) | (blue << BLUE_SHIFT) | (green << GREEN_SHIFT) | (red << RED_SHIFT));
}

static inline u32 Color_UnpackRed(u16 color) {
    return (color >> RED_SHIFT) & 0x1F;
}

static inline u32 Color_UnpackGreen(u16 color) {
    return (color >> GREEN_SHIFT) & 0x1F;
}

static inline u32 Color_UnpackBlue(u16 color) {
    return (color >> BLUE_SHIFT) & 0x1F;
}

static inline u32 Color_UnpackAlpha(u16 color) {
    return (color >> ALPHA_SHIFT) & 1;
}

/**
 * @brief Blends a color channel towards a target value by a specified strength.
 *
 * @param channel The original color channel value (0-31).
 * @param targetFixed The target value in fixed-point (12.4 format).
 * @param blendStrength The strength of the blend (0-32).
 * @return The blended color channel value (0-31).
 */
static inline u32 Color_BlendChannel(u32 channel, u32 targetFixed, u32 blendStrength) {
    return F2I(((blendStrength * (targetFixed - I2F(channel))) >> 5) + I2F(channel));
}

#endif // ENGINE_COLOR_H