#ifndef SJ_H
#define SJ_H

#include <cri/cri_xpt.h>

typedef struct UUID {
    unsigned int   data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char  data4[8];
} UUID;

typedef struct SJCK {
    char* data;
    int   length;
} SJCK;

/**
 * @brief Stream Joint
 */
typedef struct SJ {
    struct _sj_vtable* vtable;
} SJ_OBJ;

typedef SJ_OBJ* SJ;

typedef struct _sj_vtable {
    /* 0x00 */ void (*QueryInterface)();
    /* 0x04 */ void (*AddRef)();
    /* 0x08 */ void (*Release)();
    /* 0x0C */ void (*Destroy)(SJ sj);
    /* 0x10 */ UUID* (*GetUuid)(SJ sj);
    /* 0x14 */ void (*Reset)(SJ sj);
    /* 0x18 */ void (*GetChunk)(SJ sj, int id, int nbyte, SJCK* ck);
    /* 0x1C */ void (*UngetChunk)(SJ sj, int id, SJCK* ck);
    /* 0x20 */ void (*PutChunk)(SJ sj, int id, SJCK* ck);
    /* 0x24 */ int (*GetNumData)(SJ sj, int id);
    /* 0x28 */ int (*IsGetChunk)(SJ sj, int id, int nbyte, int* rbyte);
    /* 0x2C */ void (*EntryErrFunc)(SJ sj, void (*func)(void* obj, int ecode), void* obj);
} _sj_vtable;

#define SJ_Destroy(sj)                 (*(sj)->vtable->Destroy)(sj)
#define SJ_Reset(sj)                   (*(sj)->vtable->Reset)(sj)
#define SJ_GetChunk(sj, id, nbyte, ck) (*(sj)->vtable->GetChunk)(sj, id, nbyte, ck)
#define SJ_UngetChunk(sj, id, ck)      (*(sj)->vtable->UngetChunk)(sj, id, ck)
#define SJ_PutChunk(sj, id, ck)        (*(sj)->vtable->PutChunk)(sj, id, ck)
#define SJ_GetNumData(sj, id)          (*(sj)->vtable->GetNumData)(sj, id)

#define SJ_ERR_PRM (-3)

// sj_crs.c
void SJCRS_Init(void);
void SJCRS_Finish(void);
void SJCRS_Lock(void);
void SJCRS_Unlock(void);

// sj_uni.c
void SJUNI_Init(void);
void SJUNI_Finish(void);

// sj_utl.c
void SJ_SplitChunk(SJCK* ck, int nbyte, SJCK* ck1, SJCK* ck2);

#endif // SJ_H
