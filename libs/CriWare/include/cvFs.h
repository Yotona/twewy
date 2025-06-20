#ifndef CVFS_H
#define CVFS_H

#include "types.h"

typedef struct CvFileHandle {

} CvFileHandle;

s32 cvFsGetFileSize(char* filename);

CvFileHandle* cvFsOpen(const char* filename, const char* mode, s32);

#endif // CVFS_H
