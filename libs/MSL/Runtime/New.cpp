#include <MSL/C++/new.hpp>
#include <MSL/Common/alloc.h>
#include <types.h>

#pragma overload extern void* operator new(size_t) throw(bad_alloc);
#pragma overload extern void* operator new(size_t, const nothrow_t&) throw();
#pragma overload extern void* operator new[](size_t) throw(bad_alloc);
#pragma overload extern void* operator new[](size_t, const nothrow_t&) throw();

#pragma overload extern void operator delete(void*) throw();
#pragma overload extern void operator delete(void*, const nothrow_t&) throw();
#pragma overload extern void operator delete[](void*) throw();
#pragma overload extern void operator delete[](void*, const nothrow_t&) throw();

typedef void (*new_handler)();

unsigned char __throws_bad_alloc = 1;
new_handler   __new_handler      = NULL;

namespace std {
    extern void __throw_bad_alloc() {
        if (__throws_bad_alloc) {
            throw std::bad_alloc();
        }
    }
} // namespace std

void* operator new(size_t size) throw(bad_alloc) {
    void* ptr;

    if (size == 0) {
        size = sizeof(int);
    }

    while (true) {
        if ((ptr = malloc(size)) != NULL) {
            break;
        }

        if (__new_handler != NULL) {
            __new_handler();
        } else {
            __throw_bad_alloc();
            break;
        }
    }
    return ptr;
}

void operator delete(void* ptr) throw() {
    if (ptr) {
        free(ptr);
    }
}

extern void* operator new[](size_t size) throw(bad_alloc) {
    return operator new(size);
}

extern void operator delete[](void* ptr) throw() {
    delete(ptr);
}

extern void operator delete(void* ptr, const std::nothrow_t&) throw() {
    delete(ptr);
}