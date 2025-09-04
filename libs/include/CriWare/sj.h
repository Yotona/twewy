#ifndef SJ_H
#define SJ_H

#include <types.h>

typedef struct UUID {
    u32 data1;
    u16 data2;
    u16 data3;
    u8  data4[8];
} UUID;

typedef struct SJCK {
    s8* data;
    s32 length;
} SJCK;

/**
 * @brief Stream Joint
 */
typedef struct SJ {
    struct _sj_vtable* vtable;
} SJ;

typedef struct _sj_vtable {
    /* 0x00 */ void (*QueryInterface)();
    /* 0x04 */ void (*AddRef)();
    /* 0x08 */ void (*Release)();
    /* 0x0C */ void (*Destroy)(SJ* sj);
    /* 0x10 */ UUID* (*GetUuid)(SJ* sj);
    /* 0x14 */ void (*Reset)(SJ* sj);
    /* 0x18 */ void (*GetChunk)(SJ* sj, s32 id, s32 nbyte, SJCK* ck);
    /* 0x1C */ void (*UngetChunk)(SJ* sj, s32 id, SJCK* ck);
    /* 0x20 */ void (*PutChunk)(SJ* sj, s32 id, SJCK* ck);
    /* 0x24 */ s32 (*GetNumData)(SJ* sj, s32 id);
    /* 0x28 */ s32 (*IsGetChunk)(SJ* sj, s32 id, s32 nbyte, s32* rbyte);
    /* 0x2C */ void (*EntryErrFunc)(SJ* sj, void (*func)(void* obj, s32 ecode), void* obj);
} _sj_vtable;

#define SJ_Destroy(sj)                 (*(sj)->vtable->Destroy)(sj)
#define SJ_Reset(sj)                   (*(sj)->vtable->Reset)(sj)
#define SJ_GetChunk(sj, id, nbyte, ck) (*(sj)->vtable->GetChunk)(sj, id, nbyte, ck)
#define SJ_UngetChunk(sj, id, ck)      (*(sj)->vtable->UngetChunk)(sj, id, ck)
#define SJ_PutChunk(sj, id, ck)        (*(sj)->vtable->PutChunk)(sj, id, ck)
#define SJ_GetNumData(sj, id)          (*(sj)->vtable->GetNumData)(sj, id)

#endif // SJ_H
