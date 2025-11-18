#ifndef MSL_COMMON_ALLOC_H
#define MSL_COMMON_ALLOC_H

#include <types.h>

extern "C" {
void* malloc(size_t size);

void free(void* ptr) __attribute__((nothrow));
}

#endif // MSL_COMMON_ALLOC_H