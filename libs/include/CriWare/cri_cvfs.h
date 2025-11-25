#ifndef CVFS_H
#define CVFS_H

#include <types.h>

typedef struct {
    /* 0x00 */ void (*ExecServer)();
    /* 0x04 */ void (*EntryErrFunc)();
    /* 0x08 */ s32 (*GetFileSize)();
    /* 0x0C */ void (*unkC)();
    /* 0x10 */ void* (*Open)(char* device_name, void*, s32);
    /* 0x14 */ void (*Close)(void* fd);
    /* 0x18 */ s32 (*Seek)(void* fd, s32 offset, s32 whence);
    /* 0x1C */ s32 (*Tell)(void* fd);
    /* 0x20 */ s32 (*ReqRd)(void* fd, s32 len, void* buf);
    /* 0x24 */ void (*unk24)();
    /* 0x28 */ void (*StopTr)();
    /* 0x2C */ s32 (*GetStat)(void* fd);
    /* 0x30 */ void (*GetSctLen)();
    /* 0x34 */ void (*unk34)();
    /* 0x38 */ void (*GetNumTr)();
    /* 0x3C */ void (*unk3C)();
    /* 0x40 */ void (*IsExistFile)();
    /* 0x44 */ void (*unk44)();
    /* 0x48 */ void (*unk48)();
    /* 0x4C */ void (*unk4C)();
    /* 0x50 */ void (*unk50)();
    /* 0x54 */ void (*unk54)();
    /* 0x58 */ void (*unk58)();
    /* 0x5C */ void (*unk5C)();
    /* 0x60 */ s32 (*OptFn1)();
    /* 0x64 */ void (*unk64)();
} CVFSDevice;

typedef struct {
    CVFSDevice* device;
    void*       fd;
} CVFSHandle;

CVFSHandle* cvFsOpen(const char* fname, void* dir, s32 arg2);

s32 cvFsTell(CVFSHandle* hndl);

s32 cvFsSeek(CVFSHandle* handle, s32 offset, s32 whence);

s32 cvFsGetFileSize(const char* full_path);

void cvFsExecServer(void);

#endif // CVFS_H
