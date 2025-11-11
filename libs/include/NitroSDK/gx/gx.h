#ifndef NITROSDK_GX_GX_H
#define NITROSDK_GX_GX_H

#include <types.h>

/**
 * @brief Graphical display mode, specifying the data feed to output
 */
typedef enum {
    GX_DISPMODE_OFF      = 0, // Blank white screen
    GX_DISPMODE_GRAPHICS = 1, // Normal BG and OBJ layers
    GX_DISPMODE_VRAM     = 2, // VRAM display (bitmap from block selected in REG_DISPCNT bits 18-19)
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

extern u16 GX_LockID;

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

#endif // NITROSDK_GX_GX_H