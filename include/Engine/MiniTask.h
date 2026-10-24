#ifndef ENGINE_MINITASK_H
#define ENGINE_MINITASK_H

/**
 * @file MiniTask.h
 * @brief A small task pool with its own heap, used by debug scenes (Mini108).
 *
 * It mirrors EasyTask (free/active/pending lists, ids with generations, priorities), but the
 * pool owns its heap and task callbacks receive the task and its data instead of the pool.
 */

#include "Engine/Core/Memory.h"
#include <nitro/types.h>

#define MINITASK_FLAG_DELETE  0x10
#define MINITASK_FLAG_OWNDATA 0x20
#define MINITASK_FLAG_PENDING 0x40
#define MINITASK_FLAG_INUSE   0x80

typedef struct MiniTask {
    /* 0x00 */ struct MiniTask* next;
    /* 0x04 */ u16              flags;
    /* 0x06 */ u16              childCount;
    /* 0x08 */ u16              id;
    /* 0x0A */ u16              parentId;
    /* 0x0C */ u16              generation;
    /* 0x0E */ u16              parentGeneration;
    /* 0x10 */ s32              priority;
    /* 0x14 */ s32 (*update)(struct MiniTask* task, void* data, s32 arg2, s32 stage);
    /* 0x18 */ void* data;
    /* 0x1C */ u32   unk_1C;
} MiniTask; // Size: 0x20

typedef struct {
    /* 0x00 */ void*     heapData;
    /* 0x04 */ u32       heapSize;
    /* 0x08 */ u32       maxTasks;
    /* 0x0C */ MiniTask* tasks;
    /* 0x10 */ MiniTask* lastFreedTask;
    /* 0x14 */ MiniTask  freeList;
    /* 0x34 */ MiniTask  activeList;
    /* 0x54 */ MiniTask  pendingList;
    /* 0x74 */ Heap      heap;
} MiniTaskPool; // Size: 0x80

void*     MiniTask_Alloc(MiniTaskPool* pool, s32 size);
BOOL      MiniTask_Recycle(MiniTaskPool* pool, MiniTask* prev, MiniTask* task);
void      MiniTask_Insert(MiniTaskPool* pool, MiniTask* task);
void      MiniTask_ClearLists(MiniTaskPool* pool);
BOOL      MiniTask_InitPool(MiniTaskPool* pool, void* heapData, u32 heapSize, u32 count);
void      MiniTask_CleanupAll(MiniTaskPool* pool);
void      MiniTask_UpdatePool(MiniTaskPool* pool);
void      MiniTask_ProcessPending(MiniTaskPool* pool);
void      MiniTask_UpdateActive(MiniTaskPool* pool);
MiniTask* MiniTask_GetById(MiniTaskPool* pool, u32 handle);
MiniTask* MiniTask_GetByHandle(MiniTaskPool* pool, u32 handle);

#endif // ENGINE_MINITASK_H
