#ifndef CVFS_H
#define CVFS_H

#include "types.h"

typedef struct CvFileHandleVTable {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ void (*close)(s32);
    /* 0x18 */ s32 (*seek)(s32);
    /* 0x1C */ s32 (*tell)(s32);
} CvFileHandleVTable;

typedef struct CvFileHandle {
    /* 0x00 */ const CvFileHandleVTable* vtable;
    /* 0x04 */ s32                       unk_04;
} CvFileHandle;

void cvFsCallUsrErrFn(char* param_1, const char* error, s32 param_3);

BOOL cvFsTell(CvFileHandle* hndl);

void cvFsError(const char* error);

s32 cvFsGetFileSize(char* filename);

CvFileHandle* cvFsOpen(const char* filename, const char* mode, s32);

#endif // CVFS_H
