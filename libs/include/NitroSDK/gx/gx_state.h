/**
 * @file gx_state.h
 * @brief Header for GX state management, handling VRAM bank allocations for Nintendo DS graphics.
 *
 * This module provides functions to initialize and manage the allocation of VRAM banks for various
 * graphics components such as backgrounds, objects, textures, palettes, and clear images on both
 * the main and sub displays. It maintains a global state of current allocations and provides methods
 * to set, reset, disable, and query VRAM bank usage. The functions interact with hardware registers
 * to configure VRAM mappings for different graphics subsystems.
 */

#ifndef NITROSDK_GX_GX_STATE_H
#define NITROSDK_GX_GX_STATE_H

#include <types.h>

/**
 * @struct GXState
 * @brief Structure holding the current VRAM bank allocations for various graphics components.
 *
 * This structure tracks which VRAM banks are currently allocated for different purposes on the
 * Nintendo DS. Each field represents the VRAM banks assigned to a specific graphics subsystem.
 * The values are bitmasks using GXVRam enum values.
 */
typedef struct {
    /** @brief VRAM banks allocated for LCDC (direct CPU access). */
    /* 0x00 */ u16 lcdc;
    /** @brief VRAM banks allocated for background graphics. */
    /* 0x02 */ u16 bg;
    /** @brief VRAM banks allocated for object (sprite) graphics. */
    /* 0x04 */ u16 obj;
    /** @brief Unknown/unused field. */
    /* 0x06 */ u16 unk_06;
    /** @brief VRAM banks allocated for textures. */
    /* 0x08 */ u16 tex;
    /** @brief VRAM banks allocated for texture palettes. */
    /* 0x0A */ u16 texPltt;
    /** @brief VRAM banks allocated for clear image/color buffer. */
    /* 0x0C */ u16 clearImage;
    /** @brief VRAM banks allocated for background extended palettes. */
    /* 0x0E */ u16 bgExtPltt;
    /** @brief VRAM banks allocated for object extended palettes. */
    /* 0x10 */ u16 objExtPltt;
    /** @brief VRAM banks allocated for sub-display background graphics. */
    /* 0x12 */ u16 subBg;
    /** @brief VRAM banks allocated for sub-display object graphics. */
    /* 0x14 */ u16 subObj;
    /** @brief VRAM banks allocated for sub-display background extended palettes. */
    /* 0x16 */ u16 subBgExtPltt;
    /** @brief VRAM banks allocated for sub-display object extended palettes. */
    /* 0x18 */ u16 subObjExtPltt;
} GXState;

/**
 * @enum GXVRam
 * @brief Enumeration of VRAM bank identifiers and combinations.
 *
 * These values represent individual VRAM banks (A-I) as bitflags, allowing combinations
 * to be specified for allocating multiple banks at once. The Nintendo DS has 9 VRAM banks
 * of varying sizes used for different graphics purposes.
 */
typedef enum {
    /** No VRAM banks allocated. */
    GX_VRAM_NONE = 0,
    /** VRAM bank A (128KB). */
    GX_VRAM_A = 1 << 0,
    /** VRAM bank B (128KB). */
    GX_VRAM_B = 1 << 1,
    /** VRAM bank C (128KB). */
    GX_VRAM_C = 1 << 2,
    /** VRAM bank D (128KB). */
    GX_VRAM_D = 1 << 3,
    /** VRAM bank E (64KB). */
    GX_VRAM_E = 1 << 4,
    /** VRAM bank F (16KB). */
    GX_VRAM_F = 1 << 5,
    /** VRAM bank G (16KB). */
    GX_VRAM_G = 1 << 6,
    /** VRAM bank H (32KB). */
    GX_VRAM_H = 1 << 7,
    /** VRAM bank I (16KB). */
    GX_VRAM_I = 1 << 8,

    /** Combination of banks A and B. */
    GX_VRAM_AB = GX_VRAM_A | GX_VRAM_B,
    /** Combination of banks A and C. */
    GX_VRAM_AC = GX_VRAM_A | GX_VRAM_C,
    /** Combination of banks A and D. */
    GX_VRAM_AD = GX_VRAM_A | GX_VRAM_D,
    /** Combination of banks B and C. */
    GX_VRAM_BC = GX_VRAM_B | GX_VRAM_C,
    /** Combination of banks B and D. */
    GX_VRAM_BD = GX_VRAM_B | GX_VRAM_D,
    /** Combination of banks C and D. */
    GX_VRAM_CD = GX_VRAM_C | GX_VRAM_D,
    /** Combination of banks E and F. */
    GX_VRAM_EF = GX_VRAM_E | GX_VRAM_F,
    /** Combination of banks E and G. */
    GX_VRAM_EG = GX_VRAM_E | GX_VRAM_G,
    /** Combination of banks F and G. */
    GX_VRAM_FG = GX_VRAM_F | GX_VRAM_G,
    /** Combination of banks H and I. */
    GX_VRAM_HI = GX_VRAM_H | GX_VRAM_I,

    /** Combination of banks A, B, and C. */
    GX_VRAM_ABC = GX_VRAM_A | GX_VRAM_B | GX_VRAM_C,
    /** Combination of banks A, B, and D. */
    GX_VRAM_ABD = GX_VRAM_A | GX_VRAM_B | GX_VRAM_D,
    /** Combination of banks A, C, and D. */
    GX_VRAM_ACD = GX_VRAM_A | GX_VRAM_C | GX_VRAM_D,
    /** Combination of banks B, C, and D. */
    GX_VRAM_BCD = GX_VRAM_B | GX_VRAM_C | GX_VRAM_D,
    /** Combination of banks E, F, and G. */
    GX_VRAM_EFG = GX_VRAM_E | GX_VRAM_F | GX_VRAM_G,

    /** Combination of banks A, B, C, and D. */
    GX_VRAM_ABCD = GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D,

    /** All VRAM banks A through I. */
    GX_VRAM_ALL = GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D | GX_VRAM_E | GX_VRAM_F | GX_VRAM_G | GX_VRAM_H | GX_VRAM_I,
} GXVRam;

/**
 * @brief Initialize the global GXState structure to zero, indicating no VRAM banks are allocated,
 * and initialize the hardware VRAM control registers to their default state.
 */
void GX_InitGxState(void);

/**
 * @brief Allocates VRAM banks for background graphics on the main display.
 *
 * Configures the specified VRAM banks for use by the 2D background graphics engine.
 * Updates hardware registers and the global state accordingly.
 *
 * @param bg Bitmask of VRAM banks to allocate for backgrounds (e.g., GX_VRAM_ABCD).
 */
void GX_SetBankForBg(GXVRam bg);

/**
 * @brief Allocates VRAM banks for object (sprite) graphics on the main display.
 *
 * Configures the specified VRAM banks for use by the 2D object graphics engine.
 * Updates hardware registers and the global state accordingly.
 *
 * @param obj Bitmask of VRAM banks to allocate for objects (e.g., GX_VRAM_AB).
 */
void GX_SetBankForObj(GXVRam obj);

/**
 * @brief Allocates VRAM banks for background extended palettes on the main display.
 *
 * Configures the specified VRAM banks for extended background color palettes.
 * Enables extended palette mode in the display control register if banks are allocated.
 *
 * @param bgExtPltt Bitmask of VRAM banks to allocate for background extended palettes (e.g., GX_VRAM_F).
 */
void GX_SetBankForBgExtPltt(GXVRam bgExtPltt);

/**
 * @brief Allocates VRAM banks for object extended palettes on the main display.
 *
 * Configures the specified VRAM banks for extended object color palettes.
 * Enables extended palette mode in the display control register if banks are allocated.
 *
 * @param objExtPltt Bitmask of VRAM banks to allocate for object extended palettes (e.g., GX_VRAM_G).
 */
void GX_SetBankForObjExtPltt(GXVRam objExtPltt);

/**
 * @brief Allocates VRAM banks for textures used by the 3D graphics engine.
 *
 * Configures the specified VRAM banks for texture storage. Enables 3D graphics mode
 * in the display control register and sets appropriate mapping modes.
 *
 * @param tex Bitmask of VRAM banks to allocate for textures (e.g., GX_VRAM_ABD).
 */
void GX_SetBankForTex(GXVRam tex);

/**
 * @brief Allocates VRAM banks for texture palettes used by the 3D graphics engine.
 *
 * Configures the specified VRAM banks for storing texture color palettes.
 *
 * @param texPltt Bitmask of VRAM banks to allocate for texture palettes (e.g., GX_VRAM_FG).
 */
void GX_SetBankForTexPltt(GXVRam texPltt);

/**
 * @brief Allocates VRAM banks for the clear image/color buffer used by the 3D graphics engine.
 *
 * Configures the specified VRAM banks for the 3D clear image buffer.
 * Enables clear image mode in the 3D display control register.
 *
 * @param clearImage Bitmask of VRAM banks to allocate for clear image (e.g., GX_VRAM_CD).
 */
void GX_SetBankForClearImage(GXVRam clearImage);

/**
 * @brief Allocates VRAM banks for direct LCDC (CPU) access.
 *
 * Configures the specified VRAM banks to be directly accessible by the CPU.
 * This mode allows direct memory access to VRAM for custom graphics operations.
 *
 * @param lcdc Bitmask of VRAM banks to allocate for LCDC access.
 */
void GX_SetBankForLcdc(GXVRam lcdc);

/**
 * @brief Allocates VRAM banks for background graphics on the sub-display.
 *
 * Configures the specified VRAM banks for use by the sub-display's 2D background graphics engine.
 *
 * @param subBg Bitmask of VRAM banks to allocate for sub-display backgrounds (e.g., GX_VRAM_C).
 */
void GX_SetBankForSubBg(GXVRam subBg);

/**
 * @brief Allocates VRAM banks for object graphics on the sub-display.
 *
 * Configures the specified VRAM banks for use by the sub-display's 2D object graphics engine.
 *
 * @param subObj Bitmask of VRAM banks to allocate for sub-display objects (e.g., GX_VRAM_D).
 */
void GX_SetBankForSubObj(GXVRam subObj);

/**
 * @brief Allocates VRAM banks for background extended palettes on the sub-display.
 *
 * Configures the specified VRAM banks for extended background color palettes on the sub-display.
 * Enables extended palette mode for the sub-display.
 *
 * @param subBgExtPltt Bitmask of VRAM banks to allocate for sub-display background extended palettes (e.g., GX_VRAM_H).
 */
void GX_SetBankForSubBgExtPltt(GXVRam subBgExtPltt);

/**
 * @brief Allocates VRAM banks for object extended palettes on the sub-display.
 *
 * Configures the specified VRAM banks for extended object color palettes on the sub-display.
 * Enables extended palette mode for the sub-display.
 *
 * @param subObjExtPltt Bitmask of VRAM banks to allocate for sub-display object extended palettes (e.g., GX_VRAM_I).
 */
void GX_SetBankForSubObjExtPltt(GXVRam subObjExtPltt);

/**
 * @brief Resets VRAM bank allocation for background extended palettes on the main display.
 *
 * Deallocates any banks previously allocated for background extended palettes and disables
 * extended palette mode. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for background extended palettes.
 */
GXVRam GX_ResetBankForBgExtPltt(void);

/**
 * @brief Resets VRAM bank allocation for object extended palettes on the main display.
 *
 * Deallocates any banks previously allocated for object extended palettes and disables
 * extended palette mode. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for object extended palettes.
 */
GXVRam GX_ResetBankForObjExtPltt(void);

/**
 * @brief Resets VRAM bank allocation for textures.
 *
 * Deallocates any banks previously allocated for textures. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for textures.
 */
GXVRam GX_ResetBankForTex(void);

/**
 * @brief Resets VRAM bank allocation for texture palettes.
 *
 * Deallocates any banks previously allocated for texture palettes. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for texture palettes.
 */
GXVRam GX_ResetBankForTexPltt(void);

/**
 * @brief Resets VRAM bank allocation for clear image.
 *
 * Deallocates any banks previously allocated for the clear image buffer. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for clear image.
 */
GXVRam GX_ResetBankForClearImage(void);

/**
 * @brief Resets VRAM bank allocation for sub-display backgrounds.
 *
 * Deallocates any banks previously allocated for sub-display backgrounds. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for sub-display backgrounds.
 */
GXVRam GX_ResetBankForSubBg(void);

/**
 * @brief Resets VRAM bank allocation for sub-display objects.
 *
 * Deallocates any banks previously allocated for sub-display objects. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for sub-display objects.
 */
GXVRam GX_ResetBankForSubObj(void);

/**
 * @brief Resets VRAM bank allocation for sub-display background extended palettes.
 *
 * Deallocates any banks previously allocated for sub-display background extended palettes and disables
 * extended palette mode for the sub-display. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for sub-display background extended palettes.
 */
GXVRam GX_ResetBankForSubBgExtPltt(void);

/**
 * @brief Resets VRAM bank allocation for sub-display object extended palettes.
 *
 * Deallocates any banks previously allocated for sub-display object extended palettes and disables
 * extended palette mode for the sub-display. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for sub-display object extended palettes.
 */
GXVRam GX_ResetBankForSubObjExtPltt(void);

/**
 * @brief Disables VRAM bank allocation for backgrounds on the main display.
 *
 * Deallocates any banks previously allocated for backgrounds and resets the corresponding
 * hardware registers to disabled state. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for backgrounds.
 */
GXVRam GX_DisableBankForBg(void);

/**
 * @brief Disables VRAM bank allocation for objects on the main display.
 *
 * Deallocates any banks previously allocated for objects and resets the corresponding
 * hardware registers to disabled state. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for objects.
 */
GXVRam GX_DisableBankForObj(void);

/**
 * @brief Disables VRAM bank allocation for object extended palettes on the main display.
 *
 * Deallocates any banks previously allocated for object extended palettes, disables
 * extended palette mode, and resets hardware registers. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for object extended palettes.
 */
GXVRam GX_DisableBankForObjExtPltt(void);

/**
 * @brief Disables VRAM bank allocation for textures.
 *
 * Deallocates any banks previously allocated for textures and resets the corresponding
 * hardware registers to disabled state. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for textures.
 */
GXVRam GX_DisableBankForTex(void);

/**
 * @brief Disables VRAM bank allocation for texture palettes.
 *
 * Deallocates any banks previously allocated for texture palettes and resets the corresponding
 * hardware registers to disabled state. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for texture palettes.
 */
GXVRam GX_DisableBankForTexPltt(void);

/**
 * @brief Disables VRAM bank allocation for LCDC access.
 *
 * Deallocates any banks previously allocated for LCDC access and resets the corresponding
 * hardware registers to disabled state. Returns the previously allocated banks.
 *
 * @return Bitmask of VRAM banks that were previously allocated for LCDC access.
 */
GXVRam GX_DisableBankForLcdc(void);

/**
 * @brief Gets the current VRAM bank allocation for backgrounds on the main display.
 *
 * @return Bitmask of VRAM banks currently allocated for backgrounds.
 */
GXVRam GX_GetBankForBg(void);

/**
 * @brief Gets the current VRAM bank allocation for objects on the main display.
 *
 * @return Bitmask of VRAM banks currently allocated for objects.
 */
GXVRam GX_GetBankForObj(void);

/**
 * @brief Gets the current VRAM bank allocation for background extended palettes on the main display.
 *
 * @return Bitmask of VRAM banks currently allocated for background extended palettes.
 */
GXVRam GX_GetBankForBgExtPltt(void);

/**
 * @brief Gets the current VRAM bank allocation for object extended palettes on the main display.
 *
 * @return Bitmask of VRAM banks currently allocated for object extended palettes.
 */
GXVRam GX_GetBankForObjExtPltt(void);

/**
 * @brief Gets the current VRAM bank allocation for textures.
 *
 * @return Bitmask of VRAM banks currently allocated for textures.
 */
GXVRam GX_GetBankForTex(void);

/**
 * @brief Gets the current VRAM bank allocation for sub-display backgrounds.
 *
 * @return Bitmask of VRAM banks currently allocated for sub-display backgrounds.
 */
GXVRam GX_GetBankForSubBg(void);

/**
 * @brief Gets the current VRAM bank allocation for sub-display objects.
 *
 * @return Bitmask of VRAM banks currently allocated for sub-display objects.
 */
GXVRam GX_GetBankForSubObj(void);

/**
 * @brief Gets the current VRAM bank allocation for sub-display background extended palettes.
 *
 * @return Bitmask of VRAM banks currently allocated for sub-display background extended palettes.
 */
GXVRam GX_GetBankForSubBgExtPltt(void);

/**
 * @brief Gets the current VRAM bank allocation for sub-display object extended palettes.
 *
 * @return Bitmask of VRAM banks currently allocated for sub-display object extended palettes.
 */
GXVRam GX_GetBankForSubObjExtPltt(void);

/**
 * @brief Gets the total size in bytes of VRAM allocated for backgrounds on the main display.
 *
 * Calculates the combined size of all VRAM banks currently allocated for backgrounds.
 *
 * @return Total size in bytes of background VRAM allocation.
 */
u32 GX_GetSizeOfBg(void);

/**
 * @brief Gets the total size in bytes of VRAM allocated for backgrounds on the sub-display.
 *
 * Calculates the combined size of all VRAM banks currently allocated for sub-display backgrounds.
 *
 * @return Total size in bytes of sub-display background VRAM allocation.
 */
u32 GX_GetSizeOfSubBg(void);

#endif // NITROSDK_GX_GX_STATE_H