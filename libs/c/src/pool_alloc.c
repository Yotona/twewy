#include <pool_alloc.h>

void* __sys_alloc(size_t size) {
    return OS_Malloc(0, -1, size);
}

void __sys_free(void* ptr) {
    OS_Free(0, -1, ptr);
}