#ifndef NITROSDK_OS_THREAD_H
#define NITROSDK_OS_THREAD_H

#include <types.h>

typedef struct OSThread {
    /* 0x00 */ char unk_00[0x6C];
    /* 0x1C */ u32  id;
} OSThread;

typedef struct OSThreadQueue {
    /* 0x00 */ OSThread* head;
    /* 0x04 */ OSThread* tail;
} OSThreadQueue;

typedef struct OSThreadInfo {
    /* 0x00 */ char      unk_00[0x4];
    /* 0x10 */ OSThread* current;
} OSThreadInfo;

extern OSThreadInfo OS_ThreadInfo;

static inline OSThreadInfo* OS_GetThreadInfo(void) {
    return &OS_ThreadInfo;
}

static inline OSThread* OS_GetCurrentThread(void) {
    return OS_GetThreadInfo()->current;
}

static inline u32 OS_GetThreadId(OSThread* thread) {
    return thread->id;
}

#endif // NITROSDK_OS_THREAD_H