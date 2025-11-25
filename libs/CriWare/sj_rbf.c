#include <CriWare/sj.h>

#define SJRBF_MAX_OBJ 160

typedef struct {
    /* 0x00 */ SJ_OBJ sj;
    /* 0x04 */ s32    used;
    /* 0x08 */ UUID*  uuid;
    /* 0x0C */ s32    unkC;
    /* 0x10 */ s32    unk10;
    /* 0x14 */ s32    unk14;
    /* 0x18 */ s32    unk18;
    /* 0x1C */ s8*    buf;
    /* 0x20 */ s32    bsize;
    /* 0x24 */ s32    xsize;
    /* 0x28 */ s32    unk28;
    /* 0x2C */ s32    unk2C;
    /* 0x30 */ s32    unk30;
    /* 0x34 */ s32    unk34;
    /* 0x38 */ void (*err_func)(void* obj, s32 ecode);
    /* 0x3C */ void* err_obj;
    /* 0x40 */ void (*unk40)(void* obj, SJCK* ck);
    /* 0x44 */ void* unk44;
} SJRBF_OBJ;

typedef SJRBF_OBJ* SJRBF;

static void  sjrbf_Init(void);
static void  sjrbf_Finish(void);
static SJRBF sjrbf_Create(s8* buf, s32 bsize, s32 xsize);
static void  sjrbf_Destroy(SJRBF sjrbf);
static UUID* sjrbf_GetUuid(SJRBF sjrbf);
static void  sjrbf_EntryErrFunc(SJRBF sjrbf, void (*func)(void* obj, s32 ecode), void* obj);
static void  sjrbf_Reset(SJRBF sjrbf);
static s32   sjrbf_GetNumData(SJRBF sjrbf, s32 id);
static void  sjrbf_GetChunk(SJRBF sjrbf, s32 id, s32 nbyte, SJCK* ck);
static void  sjrbf_PutChunk(SJRBF sjrbf, s32 id, SJCK* ck);
static void  sjrbf_UngetChunk(SJRBF sjrbf, s32 id, SJCK* ck);
static s32   sjrbf_IsGetChunk(SJRBF sjrbf, s32 id, s32 nbyte, s32* rbyte);
static void* sjrbf_GetBufPtr(SJRBF sjrbf);
static s32   sjrbf_GetBufSize(SJRBF sjrbf);
static s32   sjrbf_GetXtrSize(SJRBF sjrbf);

void  SJRBF_Reset(SJ sjrbf);
void  SJRBF_Destroy(SJ sjrbf);
UUID* SJRBF_GetUuid(SJ sjrbf);
void  SJRBF_GetChunk(SJ sjrbf, s32 id, s32 nbyte, SJCK* ck);
void  SJRBF_UngetChunk(SJ sjrbf, s32 id, SJCK* ck);
void  SJRBF_PutChunk(SJ sjrbf, s32 id, SJCK* ck);
s32   SJRBF_GetNumData(SJ sjrbf, s32 id);
s32   SJRBF_IsGetChunk(SJ sjrbf, s32 id, s32 nbyte, s32* rbyte);
void  SJRBF_EntryErrFunc(SJ sjrbf, void (*func)(void* obj, s32 ecode), void* obj);

_sj_vtable sjrbf_vtbl = {.QueryInterface = NULL,
                         .AddRef         = NULL,
                         .Release        = NULL,
                         .Destroy        = SJRBF_Destroy,
                         .GetUuid        = SJRBF_GetUuid,
                         .Reset          = SJRBF_Reset,
                         .GetChunk       = SJRBF_GetChunk,
                         .UngetChunk     = SJRBF_UngetChunk,
                         .PutChunk       = SJRBF_PutChunk,
                         .GetNumData     = SJRBF_GetNumData,
                         .IsGetChunk     = SJRBF_IsGetChunk,
                         .EntryErrFunc   = SJRBF_EntryErrFunc};

s32       sjrbf_init_cnt           = 0;
SJRBF_OBJ sjrbf_obj[SJRBF_MAX_OBJ] = {0};

const UUID sjrbf_uuid = {
    .data1 = 0x3B9A9E81, .data2 = 0x0DBB, .data3 = 0x11D2, .data4 = {0xA6, 0xBF, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}
};

void SJRBF_Error(void* obj, s32 ecode) {
    SJERR_CallErr("SJRBF Error");
}

void SJRBF_Log(char* ecode, char* edesc) {
    char errString[64];

    func_02021614(errString, sizeof(errString), ecode);
    func_02021638(errString, sizeof(errString), edesc);
    SJERR_CallErr(errString);
}

void SJRBF_Init(void) {
    func_0201a534();
    SJCRS_Lock();
    sjrbf_Init();
    SJCRS_Unlock();
}

static void sjrbf_Init(void) {
    if (sjrbf_init_cnt == 0) {
        __builtin__clear(sjrbf_obj, sizeof(sjrbf_obj));
    }
    sjrbf_init_cnt++;
}

void SJRBF_Finish(void) {
    SJCRS_Lock();
    sjrbf_Finish();
    SJCRS_Unlock();
    func_0201a55c();
}

static void sjrbf_Finish(void) {
    if (--sjrbf_init_cnt == 0) {
        __builtin__clear(sjrbf_obj, sizeof(sjrbf_obj));
    }
}

SJRBF SJRBF_Create(s8* buf, s32 bsize, s32 xsize) {
    SJCRS_Lock();
    SJRBF sj = sjrbf_Create(buf, bsize, xsize);
    SJCRS_Unlock();
    return sj;
}

static SJRBF sjrbf_Create(s8* buf, s32 bsize, s32 xsize) {
    s32 i;
    for (i = 0; i < SJRBF_MAX_OBJ; i++) {
        if (sjrbf_obj[i].used == FALSE) {
            break;
        }
    }

    SJRBF sjrbf;
    if (i == SJRBF_MAX_OBJ) {
        sjrbf = NULL;
    } else {
        sjrbf            = &sjrbf_obj[i];
        sjrbf->used      = 1;
        sjrbf->sj.vtable = &sjrbf_vtbl;
        sjrbf->buf       = buf;
        sjrbf->bsize     = bsize;
        sjrbf->xsize     = xsize;
        sjrbf->uuid      = &sjrbf_uuid;
        sjrbf->err_func  = SJRBF_Error;
        sjrbf->err_obj   = sjrbf;
        sjrbf->unk40     = 0;
        sjrbf->unk44     = 0;
        sjrbf_Reset(sjrbf);
    }
    return sjrbf;
}

void SJRBF_Destroy(SJ sjrbf) {
    SJCRS_Lock();
    sjrbf_Destroy((SJRBF)sjrbf);
    SJCRS_Unlock();
}

static void sjrbf_Destroy(SJRBF sjrbf) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090201", " : NULL pointer is passed.");
        return;
    }

    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090202", " : Specified handle is invalid.");
        return;
    }

    memset(sjrbf, 0, sizeof(SJRBF_OBJ));
    sjrbf->used = FALSE;
}

UUID* SJRBF_GetUuid(SJ sjrbf) {
    SJCRS_Lock();
    UUID* uuid = sjrbf_GetUuid((SJRBF)sjrbf);
    SJCRS_Unlock();
    return uuid;
}

UUID* sjrbf_GetUuid(SJRBF sjrbf) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090203", " : NULL pointer is passed.");
        return NULL;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090204", " : Specified handle is invalid.");
        return NULL;
    }
    return sjrbf->uuid;
}

void SJRBF_EntryErrFunc(SJ sjrbf, void (*func)(void* obj, s32 ecode), void* obj) {
    SJCRS_Lock();
    sjrbf_EntryErrFunc((SJRBF)sjrbf, func, obj);
    SJCRS_Unlock();
}

void sjrbf_EntryErrFunc(SJRBF sjrbf, void (*func)(void* obj, s32 ecode), void* obj) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090205", " : NULL pointer is passed.");
        return;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090206", " : Specified handle is invalid.");
        return;
    }
    sjrbf->err_func = func;
    sjrbf->err_obj  = obj;
}

void SJRBF_Reset(SJ sjrbf) {
    SJCRS_Lock();
    sjrbf_Reset((SJRBF)sjrbf);
    SJCRS_Unlock();
}

static void sjrbf_Reset(SJRBF sjrbf) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090207", " : NULL pointer is passed.");
        return;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090208", " : Specified handle is invalid.");
        return;
    }

    sjrbf->unkC  = 0;
    sjrbf->unk10 = sjrbf->bsize;
    sjrbf->unk14 = 0;
    sjrbf->unk18 = 0;
    sjrbf->unk28 = 0;
    sjrbf->unk2C = 0;
    sjrbf->unk30 = 0;
    sjrbf->unk34 = 0;
}

s32 SJRBF_GetNumData(SJ sjrbf, s32 id) {
    SJCRS_Lock();
    s32 num = sjrbf_GetNumData((SJRBF)sjrbf, id);
    SJCRS_Unlock();
    return num;
}

static s32 sjrbf_GetNumData(SJRBF sjrbf, s32 id) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090209", " : NULL pointer is passed.");
        return 0;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090210", " : Specified handle is invalid.");
        return 0;
    }

    if (id == 1) {
        return sjrbf->unkC;
    } else if (id == 0) {
        return sjrbf->unk10;
    } else {
        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }

        return 0;
    }
}

void SJRBF_GetChunk(SJ sjrbf, s32 id, s32 nbyte, SJCK* ck) {
    SJCRS_Lock();
    sjrbf_GetChunk((SJRBF)sjrbf, id, nbyte, ck);
    SJCRS_Unlock();
}

static void sjrbf_GetChunk(SJRBF sjrbf, s32 id, s32 nbyte, SJCK* ck) {
    s32 iVar1, iVar3;

    if (sjrbf == NULL) {
        SJRBF_Log("E2004090211", " : NULL pointer is passed.");
        return;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090212", " : Specified handle is invalid.");
        return;
    }
    iVar3 = sjrbf->bsize;
    if (iVar3 == 0) {
        SJRBF_Log("E2004090219", " : Illegal buffer size.");
        return;
    }

    if (id == 0) {
        iVar1 = sjrbf->xsize + (iVar3 - sjrbf->unk14);
        iVar3 = sjrbf->unk10;
        if (iVar3 >= iVar1) {
            iVar3 = iVar1;
        }
        ck->length = iVar3;
        if (iVar3 >= nbyte) {
            iVar3 = nbyte;
        }
        ck->length   = iVar3;
        ck->data     = sjrbf->buf + sjrbf->unk14;
        sjrbf->unk14 = (sjrbf->unk14 + ck->length) % sjrbf->bsize;
        sjrbf->unk10 -= ck->length;
        sjrbf->unk28 += ck->length;
    } else if (id == 1) {
        iVar1 = sjrbf->xsize + (iVar3 - sjrbf->unk18);
        iVar3 = sjrbf->unkC;
        if (sjrbf->unkC >= iVar1) {
            iVar3 = iVar1;
        }
        ck->length = iVar3;
        if (iVar3 >= nbyte) {
            iVar3 = nbyte;
        }
        ck->length   = iVar3;
        ck->data     = sjrbf->buf + sjrbf->unk18;
        sjrbf->unk18 = (sjrbf->unk18 + ck->length) % sjrbf->bsize;
        sjrbf->unkC -= ck->length;
        sjrbf->unk30 += ck->length;
    } else {
        ck->length = 0;
        ck->data   = NULL;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }
}

void SJRBF_PutChunk(SJ sjrbf, s32 id, SJCK* ck) {
    sjrbf_PutChunk((SJRBF)sjrbf, id, ck);
}

static void sjrbf_PutChunk(SJRBF sjrbf, s32 id, SJCK* ck) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090213", " : NULL pointer is passed.");
        return;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090214", " : Specified handle is invalid.");
        return;
    }

    if ((ck->length <= 0) || (ck->data == NULL)) {
        return;
    }

    if (id == 1) {
        if (sjrbf->unk40 != NULL) {
            sjrbf->unk40(sjrbf->unk44, ck);
        }

        s32 offset = ck->data - sjrbf->buf;
        if (offset < sjrbf->xsize) {
            s32 available = sjrbf->xsize - offset;
            s32 copy_size = (ck->length < available) ? ck->length : available;
            memcpy(sjrbf->bsize + offset + sjrbf->buf, ck->data, copy_size);
        }

        s8* dest       = sjrbf->buf;
        s32 length     = ck->length;
        s32 end_offset = (ck->data - sjrbf->buf) + length;
        if (end_offset > sjrbf->bsize) {
            s32 overlap   = end_offset - sjrbf->bsize;
            s32 copy_size = (length < overlap) ? length : overlap;
            memcpy(dest, dest + (end_offset - copy_size), copy_size);
        }

        SJCRS_Lock();
        sjrbf->unkC += ck->length;
        sjrbf->unk34 += ck->length;
        SJCRS_Unlock();
    } else if (id == 0) {
        SJCRS_Lock();
        sjrbf->unk10 += ck->length;
        sjrbf->unk2C += ck->length;
        SJCRS_Unlock();
    } else {
        SJCRS_Lock();
        ck->length = 0;
        ck->data   = NULL;
        SJCRS_Unlock();

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }
}

void SJRBF_UngetChunk(SJ sj, s32 id, SJCK* ck) {
    SJCRS_Lock();
    sjrbf_UngetChunk((SJRBF)sj, id, ck);
    SJCRS_Unlock();
}

static void sjrbf_UngetChunk(SJRBF sjrbf, s32 id, SJCK* ck) {
    s32 a;
    s32 b;
    s32 iVar3;

    if (sjrbf == NULL) {
        SJRBF_Log("E2004090215", " : NULL pointer is passed.");
        return;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090216", " : Specified handle is invalid.");
        return;
    }
    iVar3 = sjrbf->bsize;
    if (iVar3 == 0) {
        SJRBF_Log("E2004090220", " : Illegal buffer size.");
        return;
    }

    if ((ck->length <= 0) || (ck->data == NULL)) {
        return;
    }

    if (id == 0) {
        a = (sjrbf->unk14 + sjrbf->bsize - ck->length) % sjrbf->bsize;
        b = (s32)(ck->data - sjrbf->buf) % sjrbf->bsize;

        if (a == b) {
            sjrbf->unk14 = a;
            sjrbf->unk10 += ck->length;
        } else {
            if (sjrbf->err_func != NULL) {
                sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
            }
        }

        sjrbf->unk28 -= ck->length;
    } else if (id == 1) {
        a = (sjrbf->unk18 + sjrbf->bsize - ck->length) % sjrbf->bsize;
        b = (s32)(ck->data - sjrbf->buf) % sjrbf->bsize;

        if (a == b) {
            sjrbf->unk18 = a;
            sjrbf->unkC += ck->length;
        } else {
            if (sjrbf->err_func != NULL) {
                sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
            }
        }

        sjrbf->unk30 -= ck->length;
    } else {
        ck->length = 0;
        ck->data   = NULL;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }
}

s32 SJRBF_IsGetChunk(SJ sj, s32 id, s32 nbyte, s32* rbyte) {
    SJCRS_Lock();
    s32 result = sjrbf_IsGetChunk((SJRBF)sj, id, nbyte, rbyte);
    SJCRS_Unlock();
    return result;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static s32 sjrbf_IsGetChunk(SJRBF sjrbf, s32 id, s32 nbyte, s32* rbyte) {
    s32 unk;

    if (sjrbf == NULL) {
        SJRBF_Log("E2004090217", " : NULL pointer is passed.");
        return 0;
    }

    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090218", " : Specified handle is invalid.");
        return 0;
    }

    if (id == 0) {
        unk = MIN(sjrbf->unk10, sjrbf->bsize - sjrbf->unk14 + sjrbf->xsize);
        unk = MIN(unk, nbyte);
    } else if (id == 1) {
        unk = MIN(sjrbf->unkC, sjrbf->bsize - sjrbf->unk18 + sjrbf->xsize);
        unk = MIN(unk, nbyte);
    } else {
        unk = 0;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }

    *rbyte = unk;
    return unk == nbyte;
}

void* SJRBF_GetBufPtr(SJ sj) {
    SJCRS_Lock();
    void* buf = sjrbf_GetBufPtr((SJRBF)sj);
    SJCRS_Unlock();
    return buf;
}

void* sjrbf_GetBufPtr(SJRBF sjrbf) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090221", " : NULL pointer is passed.");
        return NULL;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090222", " : Specified handle is invalid.");
        return NULL;
    }
    return sjrbf->buf;
}

s32 SJRBF_GetBufSize(SJ sj) {
    SJCRS_Lock();
    s32 size = sjrbf_GetBufSize((SJRBF)sj);
    SJCRS_Unlock();
    return size;
}

static s32 sjrbf_GetBufSize(SJRBF sjrbf) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090223", " : NULL pointer is passed.");
        return 0;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090224", " : Specified handle is invalid.");
        return 0;
    }
    return sjrbf->bsize;
}

s32 SJRBF_GetXtrSize(SJ sj) {
    SJCRS_Lock();
    s32 size = sjrbf_GetXtrSize((SJRBF)sj);
    SJCRS_Unlock();
    return size;
}

static s32 sjrbf_GetXtrSize(SJRBF sjrbf) {
    if (sjrbf == NULL) {
        SJRBF_Log("E2004090225", " : NULL pointer is passed.");
        return 0;
    }
    if (sjrbf->used == 0) {
        SJRBF_Log("E2004090226", " : Specified handle is invalid.");
        return 0;
    }
    return sjrbf->xsize;
}
