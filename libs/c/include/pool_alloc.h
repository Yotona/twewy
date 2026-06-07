#ifndef _C_POOL_ALLOC_H
#define _C_POOL_ALLOC_H

#include <stddef.h>

void* __sys_alloc(size_t size);
void  __sys_free(void* ptr);

#endif /* _C_POOL_ALLOC_H */
