#ifndef NITROSDK_INCLUDE_DTOR_H
#define NITROSDK_INCLUDE_DTOR_H

#include <types.h>

typedef void (*DestructorFunc)(void*);

typedef struct DestructorChain {
    /* 0x00 */ struct DestructorChain* next;
    /* 0x04 */ DestructorFunc          dtor;
    /* 0x08 */ void*                   object;
} DestructorChain;

extern DestructorChain* __global_destructor_chain;

#endif // NITROSDK_INCLUDE_DTOR_H