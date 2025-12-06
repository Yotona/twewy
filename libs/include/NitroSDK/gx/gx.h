#ifndef NITROSDK_GX_GX_H
#define NITROSDK_GX_GX_H

#include <registers/reg_dispcnt.h>
#include <types.h>

/// MARK: Typedefs

/**
 * @brief Graphical display mode, specifying the data feed to output
 */
typedef enum {
    GX_DISPMODE_OFF      = 0, // Blank white screen
    GX_DISPMODE_GRAPHICS = 1, // Normal BG and OBJ layers
    GX_DISPMODE_VRAM     = 2, // VRAM display (bitmap from block selected in REG_DISPCNT.vramBlock)
    GX_DISPMODE_MMEM     = 3, // Main memory display (bitmap DMA transfer from main RAM)
} GXDisplayMode;

typedef enum {
    GX_BGMODE_0 = 0, // BG0: Text/3D, BG1: Text, BG2: Text, BG3: Text
    GX_BGMODE_1 = 1, // BG0: Text/3D, BG1: Text, BG2: Text, BG3: Affine
    GX_BGMODE_2 = 2, // BG0: Text/3D, BG1: Text, BG2: Affine, BG3: Affine
    GX_BGMODE_3 = 3, // BG0: Text/3D, BG1: Text, BG2: Text, BG3: Extended
    GX_BGMODE_4 = 4, // BG0: Text/3D, BG1: Text, BG2: Affine, BG3: Extended
    GX_BGMODE_5 = 5, // BG0: Text/3D, BG1: Text, BG2: Extended, BG3: Extended
    GX_BGMODE_6 = 6, // BG0: 3D, BG1: N/A, BG2: Large, BG3: N/A
} GXBGMode;

typedef enum {
    GX2D3D_MODE_2D = 0,
    GX2D3D_MODE_3D = 1,
} GX2D3D;

typedef enum {
    GX_OBJTILEMODE_2D      = (0 << 4) | (0 << 20), // 2D mapping, max 32KB
    GX_OBJTILEMODE_1D_32K  = (1 << 4) | (0 << 20), // 1D mapping, max 32KB
    GX_OBJTILEMODE_1D_64K  = (1 << 4) | (1 << 20), // 1D mapping, max 64KB
    GX_OBJTILEMODE_1D_128K = (1 << 4) | (2 << 20), // 1D mapping, max 128KB
    GX_OBJTILEMODE_1D_256K = (1 << 4) | (3 << 20), // 1D mapping, max 256KB
} GXOBJTileMode;

typedef enum {
    GX_OBJBMPMODE_2D_128K = (0 << 5) | (0 << 22), // 2D mapping, max 128KB
    GX_OBJBMPMODE_2D_256K = (1 << 5) | (0 << 22), // 2D mapping, max 256KB
    GX_OBJBMPMODE_1D_128K = (2 << 5) | (0 << 22), // 1D mapping, max 128KB
    GX_OBJBMPMODE_1D_256K = (2 << 5) | (1 << 22), // 1D mapping, max 256KB
} GXOBJBmpMode;

extern u16 GX_LockID;

/// MARK: Functions

/**
 * @brief Initialize the graphics system.
 */
void GX_Init(void);

/**
 * @brief Enable/disable the HBlank interrupt.
 *
 * @param enabled TRUE to enable the interrupt, FALSE to disable.
 * @return The previous state of the HBlank interrupt (non-zero if enabled).
 */
s32 GX_HBlankIntr(BOOL enabled);

/**
 * @brief Enable/disable the VBlank interrupt.
 *
 * @param enabled TRUE to enable the interrupt, FALSE to disable.
 * @return The previous state of the VBlank interrupt (non-zero if enabled).
 */
s32 GX_VBlankIntr(BOOL enabled);

/**
 * @brief Stop display output.
 */
void GX_DispOff(void);

/**
 * @brief Start display output.
 */
void GX_DispOn(void);

/**
 * @brief Set the graphics mode for the main screen.
 *
 * @param displayMode Display mode
 * @param bgMode BG mode.
 * @param dimension Whether to display BG0 as 2D or 3D.
 */
void GX_SetGraphicsMode(GXDisplayMode displayMode, GXBGMode bgMode, GX2D3D dimension);

/**
 * @brief Set the graphics mode for the sub screen.
 *
 * @param bgMode BG mode.
 */
void GXs_SetGraphicsMode(GXBGMode bgMode);

/**
 * @brief Sets the master brightness for a display register.
 *
 * @param arg0 Pointer to the brightness register (e.g., &REG_MASTER_BRIGHT).
 * @param arg1 Brightness level (-31 to 31, where 0 is off, positive for brighter, negative for dimmer).
 *
 * This function adjusts the master brightness by setting the appropriate bits in the register.
 */
void GXx_SetMasterBrightness(u16* arg0, s32 arg1);

/**
 * @brief Gets the master brightness from a display register.
 *
 * @param arg0 Pointer to the brightness register (e.g., &REG_MASTER_BRIGHT).
 * @return The current brightness level (-31 to 31).
 *
 * This function reads the brightness value from the register and returns it as a signed integer.
 */
s32 GXx_GetMasterBrightness(vu16* arg0);

/// MARK: Inlines

/**
 * @brief Set visibility of BG/OBJ layers of main graphics engine
 *
 * @param layers Bits indicating which BG/OBJ layers to display
 */
static inline void GX_SetVisibleLayers(s32 layers) {
    REG_DISPCNT = (REG_DISPCNT & ~0x1F00) | (layers << 8);
}

/**
 * @brief Set window visibility of main graphics engine
 *
 * @param windows Bits indicating which windows to display
 */
static inline void GX_SetVisibleWindows(s32 windows) {
    REG_DISPCNT = (REG_DISPCNT & ~0xE000) | (windows << 13);
}

/**
 * @brief Set OBJ tile size and mode of main graphics engine
 *
 * @param mode Tile mode to set
 */
static inline void GX_SetOBJTileMode(GXOBJTileMode mode) {
    REG_DISPCNT = (REG_DISPCNT & ~(0x300000 | 0x10)) | mode;
}

/**
 * @brief Set OBJ bitmap size and mode of main graphics engine
 *
 * @param mode Bitmap mode to set
 */
static inline void GX_SetOBJBmpMode(GXOBJBmpMode mode) {
    REG_DISPCNT = (REG_DISPCNT & ~(0x400000 | 0x60)) | mode;
}

/**
 * @brief Sets the character base block for BG0-BG3.
 *
 * @param charBase The character base block number (0-3).
 *                 Determines where character/tile data is located in VRAM.
 */
static inline void GX_SetCharBase(u32 screenBase) {
    REG_DISPCNT = (REG_DISPCNT & ~0x07000000) | (screenBase << 24);
}

/**
 * @brief Sets the screen base block for BG0-BG3.
 *
 * @param screenBase The screen base block number (0-31).
 *                   Determines where screen/map data is located in VRAM.
 */
static inline void GX_SetScreenBase(u32 charBase) {
    REG_DISPCNT = (REG_DISPCNT & ~0x38000000) | (charBase << 27);
}

/**
 * @brief Set visibility of BG/OBJ layers of sub graphics engine
 *
 * @param layers Bits indicating which BG/OBJ layers to display
 */
static inline void GXs_SetVisibleLayers(s32 layers) {
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x1F00) | (layers << 8);
}

/**
 * @brief Set window visibility of sub graphics engine
 *
 * @param windows Bits indicating which windows to display
 */
static inline void GXs_SetVisibleWindows(s32 windows) {
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0xE000) | (windows << 13);
}

/**
 * @brief Set OBJ tile size and mode of sub graphics engine
 *
 * @param mode Tile mode to set
 */
static inline void GXs_SetOBJTileMode(GXOBJTileMode mode) {
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~(0x300000 | 0x10)) | mode;
}

/**
 * @brief Set OBJ bitmap size and mode of sub graphics engine
 *
 * @param mode Bitmap mode to set
 */
static inline void GXs_SetOBJBmpMode(GXOBJBmpMode mode) {
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x60) | mode;
}

#endif // NITROSDK_GX_GX_H