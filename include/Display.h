#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * @file Display.h
 * @brief Display management system for Nintendo DS screens
 *
 * This module provides a high-level interface for configuring and managing
 * the Nintendo DS main and sub display engines, including background layers,
 * windows, blending, and various rendering effects.
 */

#include <NitroSDK/gx.h>

/// MARK: Defines

/**
 * Layer visibility flags for display control.
 * These can be combined with bitwise OR to enable multiple layers.
 */
enum {
    LAYER_NONE = 0,        ///< No layers visible
    LAYER_BG0  = (1 << 0), ///< Background layer 0
    LAYER_BG1  = (1 << 1), ///< Background layer 1
    LAYER_BG2  = (1 << 2), ///< Background layer 2
    LAYER_BG3  = (1 << 3), ///< Background layer 3
    LAYER_OBJ  = (1 << 4), ///< Object/sprite layer
};

/**
 * Display engine identifiers for main and sub screens.
 */
typedef enum {
    DISPLAY_MAIN = 0,
    DISPLAY_SUB  = 1,
} DisplayEngine;

/**
 * Background layer identifiers (0-3).
 */
typedef enum {
    DISPLAY_BG0 = 0, ///< Background layer 0
    DISPLAY_BG1 = 1, ///< Background layer 1
    DISPLAY_BG2 = 2, ///< Background layer 2
    DISPLAY_BG3 = 3, ///< Background layer 3
} DisplayBGLayer;

/**
 * Background rendering modes.
 */
typedef enum {
    DISPLAY_BGMODE_TEXT      = 0, ///< Text/tile mode
    DISPLAY_BGMODE_AFFINE    = 1, ///< Affine (rotation/scaling) mode
    DISPLAY_BGMODE_PLTT      = 2, ///< 256-color palette mode
    DISPLAY_BGMODE_BMP256    = 3, ///< 256-color bitmap mode
    DISPLAY_BGMODE_BMPDIRECT = 4, ///< Direct color bitmap mode
    DISPLAY_BGMODE_BMPLARGE  = 5, ///< Large bitmap mode (main engine only)
} DisplayBGMode;

/**
 * Display control settings for a single display engine.
 * Contains configuration for display mode, background mode, layers, and brightness.
 */
typedef struct {
    /* 0x00 */ GXDisplayMode dispMode;
    /* 0x04 */ GXBGMode      bgMode;
    /* 0x08 */ GX2D3D        dimension;
    /* 0x0C */ GXOBJTileMode objTileMode; // Size/mapping mode for OBJ tiles
    /* 0x10 */ GXOBJBmpMode  objBmpMode;  // Size/mapping mode for OBJ bitmaps
    /* 0x14 */ BOOL          displaying;  // Whether display is enabled
    /* 0x18 */ u32           layers;      // BG/OBJ layer visibility
    /* 0x1C */ u32           windows;
    /* 0x20 */ BOOL          hBlank;
    /* 0x24 */ u32           screenBase;
    /* 0x28 */ u32           charBase;
    /* 0x2C */ s32           brightness;
} DisplayControlSettings; // Size: 0x30

/**
 * Background layer configuration settings.
 * Stores mode-specific size parameters and rendering options for a single BG layer.
 */
typedef struct {
    /* 0x00 */ DisplayBGMode          bgMode;
    /* 0x04 */ GXBGScreenSizeText     screenSizeText;
    /* 0x08 */ GXBGScreenSizeAffine   screenSizeAffine;
    /* 0x0C */ GXBGScreenSizePltt     screenSizePltt;
    /* 0x10 */ GXBGScreenSizeBmp      screenSizeBmp256;
    /* 0x14 */ GXBGScreenSizeBmp      screenSizeBmpDirect;
    /* 0x18 */ GXBGScreenSizeBmpLarge screenSizeBmpLarge;
    /* 0x1C */ GXBGColors             colorMode;
    /* 0x20 */ u32                    screenBase;
    /* 0x24 */ u32                    screenBaseBmp;
    /* 0x28 */ u32                    charBase;
    /* 0x2C */ u32                    extPlttSlot;
    /* 0x30 */ GXBGOverflow           overflow;
    /* 0x34 */ u32                    priority;
    /* 0x38 */ BOOL                   mosaic;
} DisplayBGSettings; // Size: 0x3C

typedef struct {
    /* 0x00 */ s32 hOffset;
    /* 0x04 */ s32 vOffset;
} BgOffsets; // Size: 0x8

typedef struct {
    /* 0x00 */ u16 dx;
    /* 0x02 */ u16 dmx;
    /* 0x04 */ u16 dy;
    /* 0x06 */ u16 dmy;
    /* 0x08 */ u32 posX;
    /* 0x0C */ u32 posY;
    /* 0x10 */ u32 unk_10;
    /* 0x14 */ u32 unk_14;
    /* 0x18 */ u32 unk_18;
    /* 0x1C */ u32 unk_1C;
    /* 0x20 */ u32 unk_20;
    /* 0x24 */ u32 unk_24;
    /* 0x28 */ u32 unk_28;
} BgAffine; // Size: 0x2C

/**
 * Complete state for a single display engine.
 * Contains all background layer settings, offsets, affine parameters,
 * window configuration, and blending effects.
 */
typedef struct {
    /* 0x000 */ DisplayBGSettings bgSettings[4];
    /* 0x0F0 */ BgOffsets         bgOffsets[4];
    /* 0x110 */ BgAffine          bgAffines[4];

    /* 0x1C0 */ u32  window0;              // Layers contained in window 0
    /* 0x1C4 */ BOOL window0Effects;       // Brightness/blending effects for window 0
    /* 0x1C8 */ u32  windowOutside;        // Layers contained outside windows
    /* 0x1CC */ BOOL windowOutsideEffects; // Brightness/blending effects outside windows
    /* 0x1D0 */ s32  window0Top;
    /* 0x1D4 */ s32  window0Bottom;
    /* 0x1D8 */ s32  window0Right;
    /* 0x1DC */ s32  window0Left;

    /* 0x1E0 */ u32  window1;          // Layers contained in window 1
    /* 0x1E4 */ BOOL window1Effects;   // Brightness/blending effects for window 1
    /* 0x1E8 */ u32  windowObj;        // Layers contained in OBJ window
    /* 0x1EC */ u32  windowObjEffects; // Brightness/blending effects for OBJ window
    /* 0x1F0 */ s32  window1Top;
    /* 0x1F4 */ s32  window1Bottom;
    /* 0x1F8 */ s32  window1Right;
    /* 0x1FC */ s32  window1Left;

    /* 0x200 */ u16 mosaicBGWidth;
    /* 0x202 */ u16 mosaicBGHeight;
    /* 0x204 */ u16 mosaicOBJWidth;
    /* 0x206 */ u16 mosaicOBJHeight;

    /* 0x208 */ u32 blendMode;
    /* 0x20C */ u32 blendLayer0;
    /* 0x210 */ u32 blendLayer1;
    /* 0x214 */ u32 blendCoeff0;
    /* 0x218 */ u32 blendCoeff1;
    /* 0x21C */ u32 blendBrightness;
} DisplayEngineState; // Size: 0x220

/**
 * Global display settings structure.
 * Contains control settings and engine state for both main and sub displays.
 */
typedef struct {
    /* 0x000 */ s32                    unk_000;
    /* 0x004 */ DisplayControlSettings controls[2];
    /* 0x064 */ DisplayEngineState     engineState[2];
} DisplaySettings;

extern s32             data_0206aa78;
extern s32             data_0206aa7c;
extern DisplaySettings g_DisplaySettings;

/// MARK: Functions

/**
 * @brief Initialize the display system with default control settings and engine states for both main and sub displays.
 */
void Display_Init(void);

/**
 * @brief Commit all pending display settings for both main and sub display engines to hardware.
 */
void Display_Commit(void);

/**
 * @brief Commit all pending display settings, timing to be synchronized with VBlank.
 */
void Display_CommitSynced(void);

/// MARK: Inlines

static inline DisplayControlSettings* Display_GetMainControls(void) {
    return &g_DisplaySettings.controls[DISPLAY_MAIN];
}

static inline DisplayControlSettings* Display_GetSubControls(void) {
    return &g_DisplaySettings.controls[DISPLAY_SUB];
}

static inline DisplayBGSettings* Display_GetBGSettings(DisplayEngine engine, DisplayBGLayer layer) {
    return &g_DisplaySettings.engineState[engine].bgSettings[layer];
}

static inline DisplayBGSettings* Display_GetBG0Settings(DisplayEngine engine) {
    return &g_DisplaySettings.engineState[engine].bgSettings[DISPLAY_BG0];
}

static inline DisplayBGSettings* Display_GetBG1Settings(DisplayEngine engine) {
    return &g_DisplaySettings.engineState[engine].bgSettings[DISPLAY_BG1];
}

static inline DisplayBGSettings* Display_GetBG2Settings(DisplayEngine engine) {
    return &g_DisplaySettings.engineState[engine].bgSettings[DISPLAY_BG2];
}

static inline DisplayBGSettings* Display_GetBG3Settings(DisplayEngine engine) {
    return &g_DisplaySettings.engineState[engine].bgSettings[DISPLAY_BG3];
}

static inline void Display_SetMainBrightness(u32 brightness) {
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = brightness;
}

static inline void Display_SetSubBrightness(u32 brightness) {
    g_DisplaySettings.controls[DISPLAY_SUB].brightness = brightness;
}

static inline void Display_InitMainBG1(DisplayBGMode bgMode, GXBGScreenSizeText screenSizeText, GXBGColors colorMode,
                                       u32 screenBase, u32 charBase, u32 extPlttSlot, u32 regMask) {
    DisplayBGSettings* bg1Settings = Display_GetBG1Settings(DISPLAY_MAIN);
    bg1Settings->bgMode            = bgMode;
    bg1Settings->screenSizeText    = screenSizeText;
    bg1Settings->colorMode         = colorMode;
    bg1Settings->screenBase        = screenBase;
    bg1Settings->charBase          = charBase;
    bg1Settings->extPlttSlot       = extPlttSlot;
    REG_BG1CNT                     = (REG_BG1CNT & 0x43) | regMask;
}

static inline void Display_InitSubBG1(DisplayBGMode bgMode, GXBGScreenSizeText screenSizeText, GXBGColors colorMode,
                                      u32 screenBase, u32 charBase, u32 extPlttSlot, u32 regMask) {
    DisplayBGSettings* bg1Settings = Display_GetBG1Settings(DISPLAY_SUB);
    bg1Settings->bgMode            = bgMode;
    bg1Settings->screenSizeText    = screenSizeText;
    bg1Settings->colorMode         = colorMode;
    bg1Settings->screenBase        = screenBase;
    bg1Settings->charBase          = charBase;
    bg1Settings->extPlttSlot       = extPlttSlot;
    REG_BG1CNT_SUB                 = (REG_BG1CNT_SUB & 0x43) | regMask;
}

#endif // DISPLAY_H