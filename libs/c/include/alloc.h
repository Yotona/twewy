#ifndef _C_ALLOC_H
#define _C_ALLOC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t size);

void free(void* ptr) __attribute__((nothrow));

#ifdef __cplusplus
}
#endif

#endif // _C_ALLOC_H