// These functions all include ARM instructions (mcr/mrc) that interact with a coprocessor
// for data caching operations. Due to the presence of these instructions, they seem to all
// be written in assembly. Functions were named and documented by AI based on its
// interpretation of their behavior, and may not be entirely accurate.

#ifndef CACHING_H
#define CACHING_H

#include <types.h>

/**
 * @brief Enables data cache on the ARM9 processor.
 *
 * Reads the CP15 Control Register (c1, c0), sets the data cache enable bit (bit 2),
 * writes back to the control register, and returns the previous state.
 *
 * @return BOOL Previous state of the data cache enable bit (1 if enabled, 0 if disabled)
 */
BOOL DC_Enable(void);

/**
 * @brief Invalidates the entire data cache.
 *
 * Writes 0 to CP15 c7, c6, 0 to invalidate the entire data cache.
 * All data cache lines are marked as invalid. Subsequent reads will fetch from main memory.
 */
void DC_InvalidateAll(void);

/**
 * @brief Cleans the entire data cache.
 *
 * Iterates through all possible cache indices and sets to clean all cache lines.
 * This writes back any modified cache lines to main memory but keeps them valid in the cache.
 */
void DC_CleanAll(void);

/**
 * @brief Cleans and invalidates the entire data cache.
 *
 * Combines cleaning and invalidation operations for the entire data cache.
 * First drains the write buffer, then writes back and invalidates all cache lines.
 */
void DC_PurgeAll(void);

/**
 * @brief Invalidates a range of memory in the data cache.
 *
 * @param addr Start address (will be aligned to cache line boundary)
 * @param count Size of the memory region in bytes
 */
void DC_InvalidateRange(void* addr, u32 count);

/**
 * @brief Cleans a range of memory in the data cache.
 *
 * Writes back any modified data in the specified range to main memory.
 *
 * @param addr Start address (will be aligned to cache line boundary)
 * @param count Size of the memory region in bytes
 */
void DC_CleanRange(const void* addr, u32 count);

/**
 * @brief Cleans and invalidates a range of memory in the data cache.
 *
 * Combines cleaning and invalidation for a specific memory range.
 *
 * @param addr Start address (will be aligned to cache line boundary)
 * @param count Size of the memory region in bytes
 */
void DC_PurgeRange(const void* addr, u32 count);

/**
 * @brief Drains the data cache write buffer.
 *
 * Ensures all pending writes in the write buffer are completed.
 */
void DC_DrainWriteBuffer(void);

/**
 * @brief Enables instruction cache on the ARM9 processor.
 *
 * Reads the CP15 Control Register (c1, c0), sets the instruction cache enable bit (bit 12),
 * writes back to the control register, and returns the previous state.
 *
 * @return BOOL Previous state of the instruction cache enable bit (1 if enabled, 0 if disabled)
 */
BOOL IC_Enable(void);

/**
 * @brief Invalidates the entire instruction cache.
 *
 * Causes the processor to discard all cached instructions.
 * Next instruction fetches will come from main memory.
 */
void IC_InvalidateAll(void);

/**
 * @brief Invalidates a range of memory in the instruction cache.
 *
 * @param addr Start address (will be aligned to cache line boundary)
 * @param count Size of the memory region in bytes
 */
void IC_InvalidateRange(void* addr, u32 count);

#endif // CACHING_H