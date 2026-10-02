#include <cri/cri_cvfs.h>
#include <cri/cri_xpt.h>
#include <cri/private/svm.h>
#include <mem.h>
#include <nitro/fs/file.h>
#include <scanf.h>
#include <string.h>

#define NITROCI_MAX_OBJ     10
#define NITROCI_DEF_SCT_LEN 0x800

typedef struct {
    /* 0x00 */ signed char used;
    /* 0x01 */ signed char stat;
    /* 0x04 */ FS_File     file;
    /* 0x4C */ FS_File*    fp;
    /* 0x50 */ void*       buf;
    /* 0x54 */ int         sct_len;
    /* 0x58 */ int         fsize;
    /* 0x5C */ int         fsize_sct;
    /* 0x60 */ int         sct_pos;
    /* 0x64 */ int         num_tr;
    /* 0x68 */ int         req_sct;
    /* 0x6C */ int         unk6C;
    /* 0x70 */ int         req_size;
    /* 0x74 */ int         rd_req;
    /* 0x78 */ int         rd_busy;
    /* 0x7C */ int         unk7C;
    /* 0x80 */ int         index;
    /* 0x84 */ int         ofs;
} NITROCI_OBJ; // size: 0x88

#ifdef REGION_USA
const char nitroci_build_str[] = "\nNITROCI/NITRO Ver.1.02 Build:Sep 28 2007 13:14:04\n\0Append: MW4020\n";
#else
const char nitroci_build_str[] = "\nNITROCI/NITRO Ver.1.02 Build:Jun 22 2007 15:54:46\n\0Append: MW4020\n";
#endif

void  nitroCiExecServer(void);
void  nitroCiEntryErrFunc(void (*fn)(void* obj, const char* msg, void* arg), void* obj);
int   nitroCiGetFileSize(const char* fname);
void* nitroCiOpen(char* fname, void* param, int rw);
void  nitroCiClose(void* hndl);
int   nitroCiSeek(void* hndl, int ofs, int whence);
int   nitroCiTell(void* hndl);
int   nitroCiReqRd(void* hndl, int nsct, void* buf);
void  nitroCiStopTr(void* hndl);
int   nitroCiGetStat(void* hndl);
int   nitroCiGetSctLen(void* hndl);
void  nitroCiSetSctLen(void* hndl, int sct_len);
int   nitroCiGetNumTr(void* hndl);
int   nitroCiOptFn1(void* hndl, int cmd);

extern const char* nitroci_build_ptr;

CVFSDevice nitroci_vtbl = {
    nitroCiExecServer,
    nitroCiEntryErrFunc,
    nitroCiGetFileSize,
    NULL,
    nitroCiOpen,
    nitroCiClose,
    nitroCiSeek,
    nitroCiTell,
    nitroCiReqRd,
    NULL,
    nitroCiStopTr,
    nitroCiGetStat,
    nitroCiGetSctLen,
    nitroCiSetSctLen,
    nitroCiGetNumTr,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    nitroCiOptFn1,
    NULL,
};

struct {
    int unk0; // nonzero holds off starting queued reads
    void (*err_func)(void* obj, const char* msg, void* arg);
    void* err_obj;
#ifdef REGION_USA
    int unkC;
#endif
} nitroci_ctrl = {0};

char        nitroci_root_dir[0x200];
NITROCI_OBJ nitroci_obj[NITROCI_MAX_OBJ];

#define NITROCI_CALC_NSCT(size, sct_len) ((size) / (sct_len) + (((size) % (sct_len) > 0) ? 1 : 0))

int nitroci_opt_get_fsize(NITROCI_OBJ* obj);
int nitroci_opt_200(NITROCI_OBJ* obj);
int nitroci_opt_201(NITROCI_OBJ* obj);
int nitroci_get_file_size(const char* path);

void nitroci_call_errfn(void* arg, const char* msg) {
    if (nitroci_ctrl.err_func != 0) {
        nitroci_ctrl.err_func(nitroci_ctrl.err_obj, msg, arg);
    }
}

CVFSDevice* nitroCiGetInterface(void) {
    return &nitroci_vtbl;
}

void nitroCiInit(void* arg) {
    nitroci_build_ptr = nitroci_build_str;
    memset(nitroci_root_dir, 0, sizeof(nitroci_root_dir));
    memset(nitroci_obj, 0, sizeof(nitroci_obj));
}

// Defined after its only use, so it gets its own literal instead of being pooled with the table.
const char* nitroci_build_ptr = NULL;

void nitroci_make_path(char* path, const char* fname) {
    if (fname == NULL) {
        strcpy(path, nitroci_root_dir);
    } else if (fname[0] == '\0') {
        strcpy(path, nitroci_root_dir);
    } else if (fname[0] == '\\') {
        path[0] = nitroci_root_dir[0];
        path[1] = nitroci_root_dir[1];
        path[2] = '\0';
    } else if (fname[1] == ':') {
        path[0] = '\0';
    } else {
        strcpy(path, nitroci_root_dir);
    }
    if (fname != NULL) {
        strcat(path, fname);
    }
}

void nitroci_exec_hndl(NITROCI_OBJ* obj) {
    int size;

    if (obj->stat != 2) {
        return;
    }
    if (obj->rd_req == 1 && nitroci_ctrl.unk0 == 0) {
        size = FS_FileReadAsync(obj->fp, obj->buf, obj->req_size);
        if (obj->req_size >= 0 && size >= 0) {
            obj->req_size = size;
            obj->rd_req   = 0;
            obj->rd_busy  = 1;
        }
    }
    if (obj->rd_busy != 1) {
        return;
    }
    if (FS_IsFileBusy(obj->fp)) {
        return;
    }
    SVM_Lock();
    obj->rd_busy = 0;
    obj->stat    = 1;
    obj->sct_pos += obj->req_size / obj->sct_len;
    obj->num_tr  = obj->req_size;
    obj->req_sct = 0;
    SVM_Unlock();
}

void nitroCiExecServer(void) {
    NITROCI_OBJ* obj;
    int          i;

    for (i = 0; i < NITROCI_MAX_OBJ; i++) {
        obj = &nitroci_obj[i];
        if (obj->used == 1) {
            nitroci_exec_hndl(obj);
        }
    }
}

void nitroCiEntryErrFunc(void (*fn)(void* obj, const char* msg, void* arg), void* obj) {
    nitroci_ctrl.err_func = fn;
    nitroci_ctrl.err_obj  = obj;
}

int nitroCiGetFileSize(const char* fname) {
    char path[0x200];
    int  size;

    if (fname == NULL) {
        nitroci_call_errfn(NULL, "E0092901:fname is null.(nitroCiGetFileSize)");
        return 0;
    }
    nitroci_make_path(path, fname);
    size = nitroci_get_file_size(path);
    if (size < 0) {
        size = 0x7FFFF800;
    }
    return size;
}

int nitroCiOptFn1(void* hndl, int cmd) {
    switch (cmd) {
        case 300:
            return nitroci_opt_get_fsize(hndl);
        case 200:
            return nitroci_opt_200(hndl);
        case 201:
            return nitroci_opt_201(hndl);
        default:
            return -1;
    }
}

int nitroci_opt_get_fsize(NITROCI_OBJ* obj) {
    return obj->fsize;
}

int nitroci_parse_fs_name(const char* fname, FS_File** fp, int* ofs, int* size) {
    if (memcmp("NitroFs", fname, 7) == 0 && sscanf(fname, "NitroFs%08x.%08x.%08x", fp, ofs, size) == 3) {
        if (*size == 0) {
            *size = 0x7FFFFFFF;
        }
        return 1;
    }
    *fp   = NULL;
    *ofs  = 0;
    *size = 0;
    return 0;
}

void* nitroCiOpen(char* fname, void* param, int rw) {
    char         path[0x200];
    FS_File*     fp;
    int          ofs;
    int          size;
    NITROCI_OBJ* obj;
    NITROCI_OBJ* cur;
    int          i;
    int          fsize;

    if (fname == NULL) {
        nitroci_call_errfn(NULL, "E0092908:fname is null.(nitroCiOpen)");
        return NULL;
    }
    if (rw != 0) {
        nitroci_call_errfn(NULL, "E0092909:rw is illigal.(nitroCiOpen)");
        return NULL;
    }
    cur = nitroci_obj;
    for (i = 0; i < NITROCI_MAX_OBJ; i++, cur++) {
        obj = cur;
        if (cur->used == 0) {
            break;
        }
    }
    if (i == NITROCI_MAX_OBJ) {
        nitroci_call_errfn(NULL, "E0092910:not enough handle resource.(nitroCiOpen)");
        return NULL;
    }
    obj->index = i;
    nitroci_make_path(path, fname);
    if (nitroci_parse_fs_name(fname, &fp, &ofs, &size) == 1) {
        obj->fp  = fp;
        obj->ofs = ofs;
        fsize    = size;
    } else {
        FS_FileInit(&obj->file);
        obj->fp = &obj->file;
        if (FS_FileOpen(&obj->file, path) == FALSE) {
            nitroci_call_errfn(NULL, "E0092920:can't open (nitroCiOpen)");
            return NULL;
        }
        obj->ofs = 0;
        fsize    = obj->fp->endPosition - obj->fp->startPosition;
    }
    obj->fsize     = fsize;
    obj->sct_len   = NITROCI_DEF_SCT_LEN;
    obj->fsize_sct = NITROCI_CALC_NSCT(obj->fsize, obj->sct_len);
    obj->sct_pos   = 0;
    obj->buf       = NULL;
    obj->req_sct   = 0;
    obj->num_tr    = 0;
    obj->stat      = 0;
    obj->used      = 1;
    obj->rd_req    = 0;
    obj->rd_busy   = 0;
    return obj;
}

void nitroCiClose(void* hndl) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        return;
    }
    if (obj->fp == &obj->file) {
        FS_FileClose(obj->fp);
    }
    obj->used = 0;
    memset(obj, 0, sizeof(NITROCI_OBJ));
}

int nitroCiSeek(void* hndl, int ofs, int whence) {
    NITROCI_OBJ* obj = hndl;
    int          pos = 0;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    SVM_Lock();
    if (whence == 0) {
        pos = ofs;
    } else if (whence == 2) {
        pos = obj->fsize_sct + ofs;
    } else if (whence == 1) {
        pos = obj->sct_pos + ofs;
    }
    if (pos >= obj->fsize_sct) {
        pos = obj->fsize_sct;
    }
    if (pos <= 0) {
        pos = 0;
    }
    obj->sct_pos = pos;
    SVM_Unlock();
    FS_FileSeek(obj->fp, obj->sct_pos * obj->sct_len + obj->ofs, 0);
    return obj->sct_pos;
}

int nitroCiTell(void* hndl) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    return obj->sct_pos;
}

int nitroCiReqRd(void* hndl, int nsct, void* buf) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    if (nsct < 0) {
        nitroci_call_errfn(obj, "E0092913:nsct < 0.(nitroCiReqRd)");
        return 0;
    }
    if (buf == NULL) {
        nitroci_call_errfn(obj, "E0092914:buf is null.(nitroCiReqRd)");
        return 0;
    }
    if (nsct >= 0x100000) {
        nitroci_call_errfn(obj, "E0092915:nsct >= 0x100000 .(nitroCiReqRd)");
        return 0;
    }
    if (obj->rd_req == 1 || obj->rd_busy == 1) {
        return 0;
    }
    if (nsct == 0) {
        obj->stat = 1;
        return 0;
    }
    SVM_Lock();
    obj->req_sct  = nsct;
    obj->req_size = obj->req_sct * obj->sct_len;
    obj->rd_req   = 1;
    obj->rd_busy  = 0;
    obj->buf      = buf;
    obj->stat     = 2;
    SVM_Unlock();
    return obj->req_sct;
}

void nitroCiStopTr(void* hndl) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0092912:handl is null.");
        return;
    }
    SVM_Lock();
    obj->stat = 0;
    SVM_Unlock();
}

int nitroCiGetStat(void* hndl) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    return obj->stat;
}

int nitroCiGetSctLen(void* hndl) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0040301:handl is null.");
        return 0;
    }
    return obj->sct_len;
}

void nitroCiSetSctLen(void* hndl, int sct_len) {
    NITROCI_OBJ* obj = hndl;
    int          pos;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0040302:handl is null.");
        return;
    }
    if (sct_len % 32 != 0) {
        nitroci_call_errfn(NULL, "E0040303:invalidate size.");
        return;
    }
    pos            = obj->sct_pos * obj->sct_len;
    obj->sct_len   = sct_len;
    obj->fsize_sct = NITROCI_CALC_NSCT(obj->fsize, obj->sct_len);
    obj->sct_pos   = pos / obj->sct_len;
    obj->num_tr    = obj->req_sct * obj->sct_len;
}

int nitroCiGetNumTr(void* hndl) {
    NITROCI_OBJ* obj = hndl;

    if (obj == NULL) {
        nitroci_call_errfn(NULL, "E0092912:handl is null.");
        return 0;
    }
    return obj->num_tr;
}

int nitroci_opt_200(NITROCI_OBJ* obj) {
    return 0;
}

int nitroci_opt_201(NITROCI_OBJ* obj) {
    return obj->num_tr;
}

int nitroci_get_file_size(const char* path) {
    FS_File  file;
    FS_File* fp;
    int      ofs;
    int      size;
    int      ret;

    if (nitroci_parse_fs_name(path, &fp, &ofs, &size) == 1) {
        ret = size;
    } else {
        FS_FileInit(&file);
        if (FS_FileOpen(&file, path) == FALSE) {
            return 0;
        }
        ret = file.endPosition - file.startPosition;
        FS_FileClose(&file);
    }
    return ret;
}
