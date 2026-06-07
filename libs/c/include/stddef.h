#ifndef _C_STDDEF_H
#define _C_STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
    #define NULL 0
#endif

#ifndef nullptr
    #define nullptr 0
#endif

#define offsetof(ST, M) ((size_t)&(((ST*)0)->M))

typedef signed long   intptr_t;
typedef unsigned long uintptr_t;
typedef intptr_t      ptrdiff_t;

#if defined(__MWERKS__) || defined(__INTELLISENSE__)
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

#ifdef __cplusplus
}
#endif

#endif