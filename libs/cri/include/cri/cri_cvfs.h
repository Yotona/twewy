#ifndef CVFS_H
#define CVFS_H

#include <cri/cri_xpt.h>

typedef struct {
    /* 0x00 */ void (*ExecServer)();
    /* 0x04 */ void (*EntryErrFunc)();
    /* 0x08 */ int (*GetFileSize)();
    /* 0x0C */ void (*unkC)();
    /* 0x10 */ void* (*Open)(char* device_name, void*, int);
    /* 0x14 */ void (*Close)(void* fd);
    /* 0x18 */ int (*Seek)(void* fd, int offset, int whence);
    /* 0x1C */ int (*Tell)(void* fd);
    /* 0x20 */ int (*ReqRd)(void* fd, int len, void* buf);
    /* 0x24 */ void (*unk24)();
    /* 0x28 */ void (*StopTr)();
    /* 0x2C */ int (*GetStat)(void* fd);
    /* 0x30 */ int (*GetSctLen)();
    /* 0x34 */ void (*SetSctLen)();
    /* 0x38 */ int (*GetNumTr)();
    /* 0x3C */ void (*unk3C)();
    /* 0x40 */ void (*IsExistFile)();
    /* 0x44 */ void (*unk44)();
    /* 0x48 */ void (*unk48)();
    /* 0x4C */ void (*unk4C)();
    /* 0x50 */ void (*unk50)();
    /* 0x54 */ void (*unk54)();
    /* 0x58 */ void (*unk58)();
    /* 0x5C */ void (*unk5C)();
    /* 0x60 */ int (*OptFn1)();
    /* 0x64 */ void (*unk64)();
} CVFSDevice;

typedef struct {
    CVFSDevice* device;
    void*       fd;
} CVFSHandle;

CVFSHandle* cvFsOpen(const char* fname, void* dir, int arg2);

int cvFsTell(CVFSHandle* hndl);

int cvFsSeek(CVFSHandle* handle, int offset, int whence);

int cvFsGetFileSize(const char* full_path);

void cvFsExecServer(void);

#endif // CVFS_H
