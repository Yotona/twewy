#ifndef RUNTIME_MWEXCEPTION_H
#define RUNTIME_MWEXCEPTION_H

#include <types.h>

typedef void (*DestructorFunc)(void*);

typedef struct DestructorChain {
    /* 0x00 */ struct DestructorChain* next;
    /* 0x04 */ DestructorFunc          dtor;
    /* 0x08 */ void*                   object;
} DestructorChain;

extern DestructorChain* __global_destructor_chain;

/**
 * @brief Call all static initializers
 */
extern void __call_static_initializers(void);

/**
 * @brief Destroy all constructed global objects
 */
extern void __destroy_global_chain(void);

#define DTOR_CALL(dtor, object) (((void (*)(void*, u32))dtor)(object, -1))

#endif // RUNTIME_MWEXCEPTION_H