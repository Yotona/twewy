#ifndef MSL_COMMON_STRTOUL_H
#define MSL_COMMON_STRTOUL_H

#include <types.h>

extern u64 __strtoull(int base, int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned,
                      int* negative, int* overflow);

#endif // MSL_COMMON_STRTOUL_H