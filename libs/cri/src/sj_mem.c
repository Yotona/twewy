#include <cri/sj.h>

typedef struct {
    /* 0x00 */ SJ_OBJ sj;
    /* 0x04 */ int    used;
    /* 0x08 */ UUID*  uuid;
    /* 0x0C */ int    unkC;
    /* 0x10 */ int    unk10;
    /* 0x14 */ char*  buf_ptr;
    /* 0x18 */ int    bsize;
    /* 0x1C */ void (*err_func)(void* obj, int ecode);
    /* 0x20 */ void* err_obj;
} SJMEM_OBJ;

typedef SJMEM_OBJ* SJMEM;

#define MIN(a, b)     ((a) < (b) ? (a) : (b))
#define MAX(a, b)     ((a) < (b) ? (b) : (a))
#define SJMEM_MAX_OBJ 20

// forward decls
static void  sjmem_Init(void);
static void  sjmem_Finish(void);
static SJMEM sjmem_Create(char* data, int bsize);
static void  sjmem_Destroy(SJMEM sjmem);
static UUID* sjmem_GetUuid(SJMEM sjmem);
static void  sjmem_EntryErrFunc(SJMEM sjmem, void (*func)(void* obj, int ecode), void* obj);
static void  sjmem_Reset(SJMEM sjmem);
static int   sjmem_GetNumData(SJMEM sjmem, int id);
static void  sjmem_GetChunk(SJMEM sjmem, int id, int nbyte, SJCK* ck);
static void  sjmem_PutChunk(SJMEM sjmem, int id, SJCK* ck);
static void  sjmem_UngetChunk(SJMEM sjmem, int id, SJCK* ck);
static int   sjmem_IsGetChunk(SJMEM sjmem, int id, int nbyte, int* rbyte);

void  SJMEM_Reset(SJ sjmem);
void  SJMEM_Destroy(SJ sjmem);
UUID* SJMEM_GetUuid(SJ sjmem);
void  SJMEM_Reset(SJ sjmem);
void  SJMEM_GetChunk(SJ sjmem, int id, int nbyte, SJCK* ck);
void  SJMEM_UngetChunk(SJ sjmem, int id, SJCK* ck);
void  SJMEM_PutChunk(SJ sjmem, int id, SJCK* ck);
int   SJMEM_GetNumData(SJ sjmem, int id);
int   SJMEM_IsGetChunk(SJ sjmem, int id, int nbyte, int* rbyte);
void  SJMEM_EntryErrFunc(SJ sjmem, void (*func)(void* obj, int ecode), void* obj);

// data
_sj_vtable sjmem_vtbl = {.QueryInterface = NULL,
                         .AddRef         = NULL,
                         .Release        = NULL,
                         .Destroy        = SJMEM_Destroy,
                         .GetUuid        = SJMEM_GetUuid,
                         .Reset          = SJMEM_Reset,
                         .GetChunk       = SJMEM_GetChunk,
                         .UngetChunk     = SJMEM_UngetChunk,
                         .PutChunk       = SJMEM_PutChunk,
                         .GetNumData     = SJMEM_GetNumData,
                         .IsGetChunk     = SJMEM_IsGetChunk,
                         .EntryErrFunc   = SJMEM_EntryErrFunc};

int       sjmem_init_cnt           = 0;
SJMEM_OBJ sjmem_obj[SJMEM_MAX_OBJ] = {0};

const UUID sjmem_uuid = {
    .data1 = 0xDD9EEE41, .data2 = 0x1679, .data3 = 0x11D2, .data4 = {0x93, 0x6C, 0x00, 0x60, 0x08, 0x94, 0x48, 0xBC}
};

void SJMEM_Error(void* obj, int ecode) {
    SJERR_CallErr("SJMEM Error");
}

void SJMEM_Log(char* ecode, char* edesc) {
    char errString[64];

    func_02021614(errString, sizeof(errString), ecode);
    func_02021638(errString, sizeof(errString), edesc);
    SJERR_CallErr(errString);
}

void SJMEM_Init(void) {
    func_0201a534();
    SJCRS_Lock();
    sjmem_Init();
    SJCRS_Unlock();
}

static void sjmem_Init(void) {
    if (sjmem_init_cnt == 0) {
        __builtin__clear(sjmem_obj, sizeof(sjmem_obj));
    }

    sjmem_init_cnt++;
}

void SJMEM_Finish(void) {
    SJCRS_Lock();
    sjmem_Finish();
    SJCRS_Unlock();
    func_0201a55c();
}

static void sjmem_Finish(void) {
    if (--sjmem_init_cnt == 0) {
        __builtin__clear(sjmem_obj, sizeof(sjmem_obj));
    }
}

SJ SJMEM_Create(char* data, int bsize) {
    SJCRS_Lock();
    SJ sj = (SJ)sjmem_Create(data, bsize);
    SJCRS_Unlock();
    return sj;
}

static SJMEM sjmem_Create(char* data, int bsize) {
    SJMEM sjmem;
    int   i;

    for (i = 0; i < SJMEM_MAX_OBJ; i++) {
        if (sjmem_obj[i].used == 0) {
            break;
        }
    }

    if (i == SJMEM_MAX_OBJ) {
        return NULL;
    }

    sjmem            = &sjmem_obj[i];
    sjmem->used      = 1;
    sjmem->sj.vtable = &sjmem_vtbl;
    sjmem->buf_ptr   = data;
    sjmem->bsize     = bsize;
    sjmem->uuid      = &sjmem_uuid;
    sjmem->err_func  = SJMEM_Error;
    sjmem->err_obj   = sjmem;
    sjmem_Reset(sjmem);
    return sjmem;
}

void SJMEM_Destroy(SJ sjmem) {
    SJCRS_Lock();
    sjmem_Destroy((SJMEM)sjmem);
    SJCRS_Unlock();
}

static void sjmem_Destroy(SJMEM sjmem) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090231", " : NULL pointer is passed.");
        return;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090232", " : Specified handle is invalid.");
        return;
    }
    memset(sjmem, 0, sizeof(SJMEM_OBJ));
    sjmem->used = 0;
}

UUID* SJMEM_GetUuid(SJ sjmem) {
    SJCRS_Lock();
    UUID* uuid = sjmem_GetUuid((SJMEM)sjmem);
    SJCRS_Unlock();
    return uuid;
}

static UUID* sjmem_GetUuid(SJMEM sjmem) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090233", " : NULL pointer is passed.");
        return NULL;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090234", " : Specified handle is invalid.");
        return NULL;
    }
    return sjmem->uuid;
}

void SJMEM_EntryErrFunc(SJ sjmem, void (*func)(void* obj, int ecode), void* obj) {
    SJCRS_Lock();
    sjmem_EntryErrFunc((SJMEM)sjmem, func, obj);
    SJCRS_Unlock();
}

static void sjmem_EntryErrFunc(SJMEM sjmem, void (*func)(void* obj, int ecode), void* obj) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090235", " : NULL pointer is passed.");
        return;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090236", " : Specified handle is invalid.");
        return;
    }
    sjmem->err_func = func;
    sjmem->err_obj  = obj;
}

void SJMEM_Reset(SJ sjmem) {
    SJCRS_Lock();
    sjmem_Reset((SJMEM)sjmem);
    SJCRS_Unlock();
}

static void sjmem_Reset(SJMEM sjmem) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090237", " : NULL pointer is passed.");
        return;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090238", " : Specified handle is invalid.");
        return;
    }
    sjmem->unkC  = sjmem->bsize;
    sjmem->unk10 = 0;
}

int SJMEM_GetNumData(SJ sjmem, int id) {
    SJCRS_Lock();
    int numData = sjmem_GetNumData((SJMEM)sjmem, id);
    SJCRS_Unlock();
    return numData;
}

static int sjmem_GetNumData(SJMEM sjmem, int id) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090239", " : NULL pointer is passed.");
        return 0;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090240", " : Specified handle is invalid.");
        return 0;
    }
    if (id == 1) {
        return sjmem->unkC;
    }
    if (id == 0) {
        return 0;
    }
    if (sjmem->err_func != NULL) {
        sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
    }
    return 0;
}

void SJMEM_GetChunk(SJ sjmem, int id, int nbyte, SJCK* ck) {
    SJCRS_Lock();
    sjmem_GetChunk((SJMEM)sjmem, id, nbyte, ck);
    SJCRS_Unlock();
}

static void sjmem_GetChunk(SJMEM sjmem, int id, int nbyte, SJCK* ck) {
    int len;

    if (sjmem == NULL) {
        SJMEM_Log("E2004090241", " : NULL pointer is passed.");
        return;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090242", " : Specified handle is invalid.");
        return;
    }

    if (id == 0) {
        ck->length = 0;
        ck->data   = NULL;
    } else if (id == 1) {
        len        = sjmem->unkC;
        len        = MIN(len, nbyte);
        ck->length = len;
        ck->data   = sjmem->buf_ptr + sjmem->unk10;
        sjmem->unk10 += ck->length;
        sjmem->unkC -= ck->length;
    } else {
        ck->length = 0;
        ck->data   = NULL;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }
}

void SJMEM_PutChunk(SJ sjmem, int id, SJCK* ck) {
    SJCRS_Lock();
    sjmem_PutChunk((SJMEM)sjmem, id, ck);
    SJCRS_Unlock();
}

static void sjmem_PutChunk(SJMEM sjmem, int id, SJCK* ck) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090243", " : NULL pointer is passed.");
        return;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090244", " : Specified handle is invalid.");
        return;
    }
    if ((ck->length <= 0) || (ck->data == NULL)) {
        return;
    }

    if (id == 0) {
        // Do nothing
    } else if (id == 1) {
        // Do nothing
    } else {
        ck->length = 0;
        ck->data   = NULL;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }
}

void SJMEM_UngetChunk(SJ sjmem, int id, SJCK* ck) {
    SJCRS_Lock();
    sjmem_UngetChunk((SJMEM)sjmem, id, ck);
    SJCRS_Unlock();
}

static void sjmem_UngetChunk(SJMEM sjmem, int id, SJCK* ck) {
    if (sjmem == NULL) {
        SJMEM_Log("E2004090245", " : NULL pointer is passed.");
        return;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090246", " : Specified handle is invalid.");
        return;
    }
    if ((ck->length <= 0) || (ck->data == NULL)) {
        return;
    }

    if (id == 0) {
        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    } else if (id == 1) {
        int iVar3 = sjmem->unk10 - ck->length;
        if (iVar3 <= 0) {
            iVar3 = 0;
        }
        sjmem->unk10 = iVar3;

        int iVar1 = sjmem->unkC + ck->length;
        int iVar4 = sjmem->bsize;
        if (sjmem->bsize >= iVar1) {
            iVar4 = iVar1;
        }
        sjmem->unkC = iVar4;

        if (iVar3 != (ck->data - sjmem->buf_ptr)) {
            if (sjmem->err_func != NULL) {
                sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
            }
        }
    } else {
        ck->length = 0;
        ck->data   = NULL;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }
}

int SJMEM_IsGetChunk(SJ sjmem, int id, int nbyte, int* rbyte) {
    SJCRS_Lock();
    int result = sjmem_IsGetChunk((SJMEM)sjmem, id, nbyte, rbyte);
    SJCRS_Unlock();
    return result;
}

static int sjmem_IsGetChunk(SJMEM sjmem, int id, int nbyte, int* rbyte) {
    int unk;

    if (sjmem == NULL) {
        SJMEM_Log("E2004090247", " : NULL pointer is passed.");
        return 0;
    }
    if (sjmem->used == 0) {
        SJMEM_Log("E2004090248", " : Specified handle is invalid.");
        return 0;
    }

    if (id == 0) {
        unk = 0;
    } else if (id == 1) {
        unk = MIN(sjmem->unkC, nbyte);
    } else {
        unk = 0;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }

    *rbyte = unk;
    return unk == nbyte;
}
