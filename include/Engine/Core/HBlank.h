#ifndef ENGINE_CORE_HBLANK_H
#define ENGINE_CORE_HBLANK_H

#include "Engine/Core/Interrupts.h"

/**
 * @brief Executes per-scanline HBlank register writes for all active entries.
 *
 * Reads the current scanline and copies one value from each active source table
 * into its corresponding destination register.
 */
void HBlank_RunScanlineWrites(void);

/**
 * @brief Initializes the HBlank manager state.
 *
 * Clears the null sink, resets callback state, and marks all entries inactive.
 */
void HBlank_Init(void);

/**
 * @brief Installs HBlank management callbacks.
 *
 * Saves the current VBlank/HBlank callbacks and installs the internal VBlank
 * dispatcher plus the provided HBlank callback.
 *
 * @param cb HBlank callback to run each HBlank interval.
 */
void HBlank_InstallCallbacks(IRQCallback cb);

/**
 * @brief Restores callbacks that were active before installation.
 *
 * Also disables HBlank interrupt generation managed by this module.
 */
void HBlank_RestoreCallbacks(void);

/**
 * @brief Enables HBlank interrupt delivery.
 */
void HBlank_EnableInterrupt(void);

/**
 * @brief Allocates a free HBlank entry and sets its pending configuration.
 *
 * @param dest Destination register address (stored as a 32-bit value).
 * @param src Source scanline table address (stored as a 32-bit value).
 * @param mode Transfer mode (0 = u16, 1 = s32/u32).
 * @return Entry index in range [0, 9] on success, otherwise 0xFFFFFFFF.
 */
u32 HBlank_AllocEntry(void* dest, void* src, s32 mode);

/**
 * @brief Resets one HBlank entry to the inactive null-sink state.
 *
 * @param idx Index of the entry to reset (0-9).
 */
void HBlank_ResetEntry(u32 idx);

#endif // ENGINE_CORE_HBLANK_H