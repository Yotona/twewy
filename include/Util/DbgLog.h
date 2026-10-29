#ifndef ENGINE_DBGLOG_H
#define ENGINE_DBGLOG_H

#include <nitro/types.h>

/** @brief Frees every logged string and empties the ring. */
void DbgLog_Reset(void);

/**
 * @brief Appends to the log. When the ring wraps, the log is dumped and reset.
 * @return TRUE when the ring wrapped and was not reset.
 */
BOOL DbgLog_Print(const char* str);

/** @brief Returns the n-th most recent entry */
const char* DbgLog_GetEntry(s32 index);

/** @brief Walks all entries */
void DbgLog_Dump(void);

#endif // ENGINE_DBGLOG_H
