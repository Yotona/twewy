#include <MSL/C++/exception.hpp>
#include <MSL/Runtime/ExceptionHandler.h>
#include <types.h>

void DestroyArrayRange(u8* first, u8* last, u32 element_size, void (*destructor)(void* thisptr)) {
    try {
        while (last > first) {
            last -= element_size;
            destructor(last);
        }
    } catch (...) {
        std::terminate();
    }
}

extern "C" void* __cxa_vec_new(u32 element_count, u32 element_size, u32 padding_size, void (*constructor)(void* thisptr),
                               void (*destructor)(void* thisptr)) {
    u8* ptr;

    if ((ptr = (u8*)::operator new[](element_count * element_size + padding_size)) == NULL) {
        return NULL;
    }

    if (padding_size != 0) {
        *(u32*)(ptr + padding_size - sizeof(u32)) = element_count;
        if (padding_size >= 2 * sizeof(u32)) {
            *(u32*)(ptr + padding_size - 2 * sizeof(u32)) = element_size;
        }
    }

    if (constructor != NULL) {
        u8* arrayptr = ptr + padding_size;

        try {
            for (; element_count > 0; --element_count) {
                constructor(arrayptr);
                arrayptr += element_size;
            }
        } catch (...) {
            if (destructor != NULL) {
                DestroyArrayRange(ptr + padding_size, arrayptr, element_size, destructor);
            }
            ::operator delete[](ptr);
            throw;
        }
    }
    return ptr + padding_size;
}

extern "C" void __cxa_vec_dtor(void* array_address, u32 element_count, u32 element_size, void (*destructor)(void* thisptr)) {
    u8* ptr;

    if (destructor == NULL) {
        return;
    }

    ptr = (u8*)array_address + element_count * element_size;

    try {
        for (; element_count > 0; --element_count) {
            ptr -= element_size;
            destructor(ptr);
        }
    } catch (...) {
        try {
            for (--element_count; element_count > 0; --element_count) {
                ptr -= element_size;
                destructor(ptr);
            }
        } catch (...) {
            std::terminate();
        }
        throw;
    }
}

extern "C" void __cxa_vec_delete(void* array_address, u32 element_size, u32 padding_size, void (*destructor)(void* thisptr)) {
    if (array_address != NULL) {
        if (destructor != NULL) {
            u32 element_count = *(u32*)((u8*)array_address - sizeof(u32));
            __cxa_vec_dtor(array_address, element_count, element_size, destructor);
        }
        ::operator delete[]((u8*)array_address - padding_size);
    }
}