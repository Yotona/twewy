#ifndef MSL_COMMON_STRTOLD_H
#define MSL_COMMON_STRTOLD_H

extern long double __strtold(int max_width, int (*ReadProc)(void*, int, int), void* ReadProcArg, int* chars_scanned,
                             int* overflow);

#endif // MSL_COMMON_STRTOLD_H