#ifndef EASY_TASK_H
#define EASY_TASK_H

#include "Memory.h"
#include <types.h>

struct TaskPool;

typedef struct Task {
    /* 0x00 */ struct Task* next;
    /* 0x04 */ u16          flags;
    /* 0x06 */ u16          childCount;
    /* 0x08 */ u16          id;
    /* 0x0A */ u16          parentId;
    /* 0x0C */ u16          generation;
    /* 0x0E */ u16          parentGeneration;
    /* 0x10 */ s32          priority;
    /* 0x14 */ s32 (*update)(struct TaskPool*, struct Task*, s32, s32);
    /* 0x18 */ void*       data;
    /* 0x1C */ const char* name;
} Task; // Size: 0x20

typedef struct TaskHandle {
    /* 0x00 */ const char* taskName;
    /* 0x04 */ s32 (*taskFunc)(struct TaskPool*, struct Task*, s32, s32);
    /* 0x08 */ u32 dataSize;
} TaskHandle;

typedef struct TaskPool {
    /* 0x00 */ MemPool* memPool;
    /* 0x04 */ u32      maxTasks;
    /* 0x08 */ void*    taskArray;
    /* 0x0C */ Task*    lastFreedTask;
    /* 0x10 */ void* (*alloc)(struct TaskPool*, s32, const char*);
    /* 0x14 */ void (*free)(struct TaskPool*, void*);
    /* 0x18 */ s32  unk_18;
    /* 0x1C */ s32  unk_1C;
    /* 0x20 */ Task freeList;    // Head of free task list
    /* 0x40 */ Task activeList;  // Head of active task list
    /* 0x60 */ Task pendingList; // Head of pending task list
} TaskPool;

BOOL EasyTask_InitializePool(TaskPool* taskPool, MemPool* memPool, u32 count, void* (*alloc)(TaskPool*, s32, const char*),
                             void (*free)(TaskPool*, void*));

void EasyTask_CleanupAllTasks(TaskPool* taskPool);

void EasyTask_DestroyPool(TaskPool* taskPool);

void EasyTask_UpdatePool(TaskPool* taskPool);

void EasyTask_ProcessPendingTasks(TaskPool* taskPool);

void EasyTask_UpdateActiveTasks(TaskPool* taskPool);

s32 EasyTask_CreateTask(TaskPool* taskPool, TaskHandle* taskHandle, void* data, s32 arg3, Task* parentTask, s32 param);

void func_0200f7a0(TaskPool* taskPool, u32 arg1);

BOOL func_0200f7bc(TaskPool* taskPool, s32* arg1);

Task* func_0200f82c(TaskPool* taskPool, s32 arg1);

void* EasyTask_GetTaskData(TaskPool* taskPool, u32 arg1);

#endif // EASY_TASK_H