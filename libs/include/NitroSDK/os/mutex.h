#ifndef NITROSDK_OS_MUTEX_H
#define NITROSDK_OS_MUTEX_H

typedef struct OSMutex {
    /* 0x00 */ char unk_00[0x18];
} OSMutex;

void OS_LockMutex(OSMutex* mutex);
void OS_UnlockMutex(OSMutex* mutex);
int  OS_TryLockMutex(OSMutex* mutex);

#endif // NITROSDK_OS_MUTEX_H