#include "Engine/IO/Str.h"
#include "Engine/Core/Memory.h"

#include <nitro/mi.h>
#include <stdarg.h>

// Lookup table of single-byte characters considered valid by Str_Validate
// (tab/CR/LF/space and printable ASCII 0x20-0x7E); all other single bytes are rejected.
static const BOOL charValidityTable[0x100] = {
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,
    TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
};

// Fallback string returned by Str_SPrintf() on allocation failure or overflow.
static char Str_Sprintf_ErrorMessage[] = "Str_SPrintf() failed";

s32 Str_Len(const char* str) {
    s32 len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

s32 Str_Copy(char* dst, const char* src) {
    s32 len = Str_Len(src);
    MI_CpuCopyU8(src, dst, len + 1);
    return len;
}

BOOL Str_Validate(const char* str, s32 maxLen) {
    BOOL nullTerminatorFound = FALSE;
    u32  pos                 = 0;

    while (TRUE) {
        char currChar = str[pos];

        if (currChar == '\0') {
            nullTerminatorFound = TRUE;
            break;
        }
        if ((maxLen != 0) && (pos >= maxLen)) {
            break;
        }
        if (((currChar >= 0x81) && (currChar <= 0x9F)) || ((currChar >= 0xE0) && (currChar <= 0xFC))) {
            char nextChar = str[pos + 1];
            if (((nextChar >= 0x40) && (nextChar <= 0x7E)) || ((nextChar >= 0x80) && (nextChar <= 0xFC))) {
                pos += 2;
                continue;
            }
            break;
        }
        if (charValidityTable[str[pos]] == 1) {
            pos += 1;
            continue;
        }
        break;
    }

    return nullTerminatorFound;
}

char* Str_SPrintf(const char* fmt, ...) {
    char* errorMsg = Str_Sprintf_ErrorMessage;

    char* buffer = Mem_PeekPool(&data_0206a9bc, 0x400);
    if (buffer == 0) {
        return errorMsg;
    }

    va_list args;
    va_start(args, fmt);
    func_020389f0(buffer, 0x400, fmt, args);
    va_end(args);

    u32 bufferLen = Str_Len(buffer) + 1;
    if (bufferLen > 0x400) {
        buffer = errorMsg;
    } else {
        Mem_AllocPool(&data_0206a9bc, bufferLen);
    }
    return buffer;
}