#include <MSL/Common/mem_funcs.h>
#include <MSL/Common/string.h>

void* memcpy(void* dst, const void* src, s32 n) {
    const char* p = (char*)src;
    char*       q = (char*)dst;

    for (n++; --n;) {
        *q++ = *p++;
    }
    return dst;
}

void* memset(void* dest, s32 val, s32 n) {
    __fill_mem(dest, val, n);
    return dest;
}

void* memchr(const void* src, s32 val, s32 n) {
    const u8* p;

    u32 v = (val & 0xff);

    for (p = (u8*)src, n++; --n;) {
        if (*p++ == v) {
            return ((void*)(p - 1));
        }
    }

    return NULL;
}

s32 memcmp(const void* src1, const void* src2, s32 n) {
    const u8* p1;
    const u8* p2;

    for (p1 = (const u8*)src1, p2 = (const u8*)src2, n++; --n;) {
        if (*p1++ != *p2++) {
            return ((*--p1 < *--p2) ? -1 : +1);
        }
    }

    return 0;
}