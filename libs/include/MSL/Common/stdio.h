#ifndef MSL_COMMON_STDIO_API_H
#define MSL_COMMON_STDIO_API_H

#include <types.h>

typedef struct {
    char* CharStr;
    s32   MaxCharCount;
    s32   CharsWritten;
} __OutStrCtrl;

typedef struct {
    char* NextChar;
    int   NullCharDetected;
} __InStrCtrl;

int __StringRead(void*, int, int);

enum __ReadProcActions {
    __GetAChar,
    __UngetAChar,
    __TestForError
};

#endif // MSL_COMMON_STDIO_API_H