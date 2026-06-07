#include <pool_alloc.h>

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    return __sys_alloc(size);
}

void free(void* ptr) {
    if (ptr != NULL) {
        __sys_free(ptr);
    }
}