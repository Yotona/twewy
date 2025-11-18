#include <types.h>

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    return func_02054604(size);
}

void free(void* ptr) {
    if (ptr != NULL) {
        func_0205461c(ptr);
    }
}