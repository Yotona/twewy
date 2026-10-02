#include <cri/cri_cvfs.h>
#include <cri/cri_xpt.h>
#include <cri/private/svm.h>
#include <mem.h>
#include <string.h>

#define MFCI_MAX_OBJ     10
#define MFCI_DEF_SCT_LEN 0x800

#define MFCI_STAT_STOP  0
#define MFCI_STAT_END   1
#define MFCI_STAT_TRANS 2

typedef struct {
    /* 0x00 */ signed char used;
    /* 0x01 */ signed char stat;
    /* 0x04 */ int         sct_len;
    /* 0x08 */ int         fsize;
    /* 0x0C */ int         fsize_sct;
    /* 0x10 */ int         sct_pos;
    /* 0x14 */ int         num_tr;
    /* 0x18 */ int         req_sct;
    /* 0x1C */ char        fname[0x14];
    /* 0x30 */ int         ofs;
    /* 0x34 */ int         req_size;
} MFCI_OBJ; // size: 0x38

char*         CRICRW_Strcpy(char* dst, int size, const char* src);
unsigned long strtoul(const char* str, char** end, int base);

void  mfCiExecServer(void);
void  mfCiEntryErrFunc(void (*fn)(void* obj, const char* msg, void* arg), void* obj);
int   mfCiGetFileSize(const char* fname);
void* mfCiOpen(char* fname, void* param, int rw);
void  mfCiClose(void* hndl);
int   mfCiSeek(void* hndl, int ofs, int whence);
int   mfCiTell(void* hndl);
int   mfCiReqRd(void* hndl, int nsct, void* buf);
void  mfCiStopTr(void* hndl);
int   mfCiGetStat(void* hndl);
int   mfCiGetSctLen(void* hndl);
void  mfCiSetSctLen(void* hndl, int sct_len);
int   mfCiGetNumTr(void* hndl);
int   mfCiOptFn1(void* hndl, int cmd);

#ifdef REGION_USA
char* volatile const mfci_build = "\nMFCI/NITRO Ver.1.21 Build:Sep 28 2007 13:14:08\n";
#else
char* volatile const mfci_build = "\nMFCI/NITRO Ver.1.21 Build:Jun 22 2007 15:54:50\n";
#endif

CVFSDevice mfci_vtbl = {
    mfCiExecServer,
    mfCiEntryErrFunc,
    mfCiGetFileSize,
    NULL,
    mfCiOpen,
    mfCiClose,
    mfCiSeek,
    mfCiTell,
    mfCiReqRd,
    NULL,
    mfCiStopTr,
    mfCiGetStat,
    mfCiGetSctLen,
    mfCiSetSctLen,
    mfCiGetNumTr,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    mfCiOptFn1,
    NULL,
};

struct {
    void* obj;
    void (*fn)(void* obj, const char* msg, void* arg);
} mfci_ctrl = {0, 0};

MFCI_OBJ mfci_obj[MFCI_MAX_OBJ];

void mfci_lock(void) {
    SVM_Lock();
}

void mfci_unlock(void) {
    SVM_Unlock();
}

CVFSDevice* mfCiGetInterface(void) {
    mfci_build;
    return &mfci_vtbl;
}

void mfci_call_errfn(void* arg, const char* msg) {
    if (mfci_ctrl.fn != 0) {
        mfci_ctrl.fn(mfci_ctrl.obj, msg, arg);
    }
}

unsigned long mfci_strtoul(const char* str, char** end, int base) {
    return strtoul(str, end, base);
}

unsigned long mfci_str_to_uint(const char* str, char** end, int base) {
    unsigned long val = 0;
    int           digit;

    while (TRUE) {
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        } else if (*str >= 'a' && *str <= 'f') {
            digit = *str - 'a' + 10;
        } else if (*str >= 'A' && *str <= 'F') {
            digit = *str - 'A' + 10;
        } else {
            break;
        }
        val = val * base + digit;
        str++;
    }
    *end = (char*)str;
    return val;
}

void* mfci_str_to_uint_ptr(const char* fname, int* fsize) {
    char* end;
    void* adr;

    if (strlen(fname) >= 18) {
        mfci_call_errfn(NULL, NULL);
    }
    end = (char*)fname;
    adr = (void*)mfci_str_to_uint(fname, &end, 16);
    if (*end != '\0') {
        end++;
    }
    if (fsize != NULL) {
        *fsize = mfci_strtoul(end, &end, 16);
    }
    return adr;
}

void mfci_exec_hndl(MFCI_OBJ* obj) {}

void mfCiExecServer(void) {
    int       i;
    MFCI_OBJ* obj = mfci_obj;

    for (i = 0; i < MFCI_MAX_OBJ; i++, obj++) {
        if (obj->used != 0) {
            mfci_exec_hndl(obj);
        }
    }
}

void mfCiEntryErrFunc(void (*fn)(void* obj, const char* msg, void* arg), void* obj) {
    mfci_ctrl.fn  = fn;
    mfci_ctrl.obj = obj;
}

int mfCiGetFileSize(const char* fname) {
    int fsize;

    mfci_str_to_uint_ptr(fname, &fsize);
    return fsize;
}

MFCI_OBJ* mfci_get_free_hn(void) {
    MFCI_OBJ* obj = NULL;
    int       i;

    for (i = 0; i < MFCI_MAX_OBJ; i++) {
        if (mfci_obj[i].used == 0) {
            obj = &mfci_obj[i];
            break;
        }
    }
    return obj;
}

void mfci_clear_obj(MFCI_OBJ* obj) {
    memset(obj, 0, sizeof(MFCI_OBJ));
}

void mfci_init_obj(MFCI_OBJ* obj) {
    obj->sct_len   = MFCI_DEF_SCT_LEN;
    obj->fsize     = mfCiGetFileSize(obj->fname);
    obj->fsize_sct = (obj->fsize + (obj->sct_len - 1)) / obj->sct_len;
    obj->sct_pos   = 0;
    obj->req_sct   = 0;
    obj->num_tr    = 0;
    obj->stat      = MFCI_STAT_STOP;
    obj->used      = 1;
}

void* mfCiOpen(char* fname, void* param, int rw) {
    MFCI_OBJ* obj;

    if (fname == NULL) {
        mfci_call_errfn(NULL, "E01100301:fname is null.(mfCiOpen)");
        return NULL;
    }
    if (rw != 0) {
        mfci_call_errfn(NULL, "E01100302:rw is illigal.(mfCiOpen)");
        return NULL;
    }
    obj = mfci_get_free_hn();
    if (obj == NULL) {
        mfci_call_errfn(NULL, "E01100303:not enough handle resource.(mfCiOpen)");
        return NULL;
    }
    CRICRW_Strcpy(obj->fname, 18, fname);
    mfci_init_obj(obj);
    return obj;
}

void mfCiClose(void* hndl) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        return;
    }
    mfCiStopTr(obj);
    if (obj->used == 1) {
        obj->used = 0;
        mfci_clear_obj(obj);
    }
}

int mfCiSeek(void* hndl, int ofs, int whence) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E01100305:handl is null.");
        return 0;
    }
    mfci_lock();
    if (whence == 0) {
        obj->sct_pos = ofs;
    } else if (whence == 2) {
        obj->sct_pos = obj->fsize_sct + ofs;
    } else if (whence == 1) {
        obj->sct_pos = obj->sct_pos + ofs;
    }
    obj->sct_pos = (obj->sct_pos < obj->fsize_sct) ? obj->sct_pos : obj->fsize_sct;
    obj->sct_pos = (obj->sct_pos > 0) ? obj->sct_pos : 0;
    mfci_unlock();
    return obj->sct_pos;
}

int mfCiTell(void* hndl) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E01100306:handl is null.");
        return 0;
    }
    return obj->sct_pos;
}

int mfCiReqRd(void* hndl, int nsct, void* buf) {
    MFCI_OBJ* obj = hndl;
    int       fsize;
    char*     adr;
    int       len;
    int       sct_len;
    int       ofs;
    int       size;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E01100307:handl is null.");
        return 0;
    }
    if (nsct < 0) {
        mfci_call_errfn(obj, "E01100308:nsct < 0.(mfCiReqRd)");
        return 0;
    }
    if (buf == NULL) {
        mfci_call_errfn(obj, "E01100309:buf is null.(mfCiReqRd)");
        return 0;
    }
    if (nsct == 0) {
        obj->stat = MFCI_STAT_END;
        return 0;
    }
    if (obj->stat == MFCI_STAT_TRANS) {
        return 0;
    }

    mfci_lock();
    obj->num_tr = 0;
    if (nsct >= obj->fsize_sct - obj->sct_pos) {
        nsct = obj->fsize_sct - obj->sct_pos;
    }
    obj->req_sct = nsct;
    sct_len      = obj->sct_len;
    ofs          = obj->sct_pos * sct_len;
    size         = obj->req_sct * sct_len;
    if (size == 0) {
        obj->stat = MFCI_STAT_END;
        mfci_unlock();
        return 0;
    }
    obj->ofs      = ofs;
    obj->req_size = size;
    obj->stat     = MFCI_STAT_TRANS;
    adr           = mfci_str_to_uint_ptr(obj->fname, &fsize);
    len           = fsize - obj->ofs;
    if (obj->req_size <= len) {
        len = obj->req_size;
    }
    mfci_unlock();

    memcpy(buf, adr + obj->ofs, obj->req_size);
    memset((char*)buf + len, 0, obj->req_size - len);

    mfci_lock();
    obj->num_tr  = obj->req_sct * obj->sct_len;
    obj->sct_pos = obj->sct_pos + obj->req_sct;
    obj->stat    = MFCI_STAT_END;
    mfci_unlock();
    return obj->req_sct;
}

void mfCiStopTr(void* hndl) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E0092912:handl is null.");
        return;
    }
    mfci_lock();
    obj->stat = MFCI_STAT_STOP;
    mfci_unlock();
}

int mfCiGetStat(void* hndl) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    return obj->stat;
}

int mfCiGetSctLen(void* hndl) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E0040301:handl is null.");
        return 0;
    }
    return obj->sct_len;
}

void mfCiSetSctLen(void* hndl, int sct_len) {
    MFCI_OBJ* obj = hndl;
    int       pos;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E0040302:handl is null.");
        return;
    }
    pos            = obj->sct_pos * obj->sct_len;
    obj->sct_len   = sct_len;
    obj->fsize_sct = (obj->fsize + (obj->sct_len - 1)) / obj->sct_len;
    obj->sct_pos   = pos / obj->sct_len;
    obj->num_tr    = obj->req_sct * sct_len;
}

int mfCiGetNumTr(void* hndl) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        mfci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    return obj->num_tr;
}

int mfCiOptFn1(void* hndl, int cmd) {
    MFCI_OBJ* obj = hndl;

    if (obj == NULL) {
        return 0;
    }
    switch (cmd) {
        case 300:
            return obj->fsize;
        case 301:
            return 0;
        case 302:
            return obj->fsize;
        case 200:
            return 0;
        case 201:
            return mfCiGetNumTr(obj);
        case 202:
            return 0;
        case 203:
            return mfCiGetFileSize((const char*)obj);
        case 204:
            return 0;
        case 205:
            return mfCiGetFileSize((const char*)obj);
        case 299:
            return 0;
        case 600:
            return 1;
        default:
            return -1;
    }
}
