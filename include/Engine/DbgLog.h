#ifndef ENGINE_DBGLOG_H
#define ENGINE_DBGLOG_H

/**
 * @file DbgLog.h
 * @brief Debug message log: a 256-entry ring of strings copied into a memory pool.
 */

#include <nitro/types.h>

/** @brief Frees every logged string and empties the ring. */
void DbgLog_Reset(void);

/**
 * @brief Appends a copy of `str` to the log.
 *
 * When the ring wraps, the log is dumped and reset, unless bit 5 of the system flags
 * word is set; then the oldest entries are kept being overwritten and TRUE is returned.
 *
 * @return TRUE when the ring wrapped and was not reset.
 */
BOOL DbgLog_Print(const char* str);

/** @brief Returns the `index`-th most recent entry (0 = newest). */
const char* DbgLog_GetEntry(s32 index);

/** @brief Walks all entries; the output itself was compiled out of the retail game. */
void DbgLog_Dump(void);

#endif // ENGINE_DBGLOG_H
