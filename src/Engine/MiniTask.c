#include "Engine/MiniTask.h"

void* MiniTask_Alloc(MiniTaskPool* pool, s32 size) {
    return Mem_AllocHeapTail(&pool->heap, size);
}

BOOL MiniTask_Recycle(MiniTaskPool* pool, MiniTask* prev, MiniTask* task) {
    MiniTask* current = prev->next;

    if (current != NULL) {
        do {
            if (current == task) {
                prev->next = task->next;

                task->flags &= ~MINITASK_FLAG_INUSE;
                task->update(task, task->data, 0, 3);
                if (task->flags & MINITASK_FLAG_OWNDATA) {
                    Mem_Free(&pool->heap, task->data);
                }
                task->flags &= ~MINITASK_FLAG_OWNDATA;

                task->generation++;
                pool->lastFreedTask->next = task;
                pool->lastFreedTask       = task;
                task->next                = NULL;
                return TRUE;
            }

            prev    = current;
            current = current->next;
        } while (current != NULL);
    }
    return FALSE;
}

void MiniTask_Insert(MiniTaskPool* pool, MiniTask* task) {
    MiniTask* prev    = &pool->activeList;
    MiniTask* current = pool->activeList.next;

    if (current != NULL) {
        do {
            if (current->priority < task->priority) {
                task->next = prev->next;
                prev->next = task;
                break;
            }
            prev    = current;
            current = current->next;
        } while (current != NULL);
    }

    if (current == NULL) {
        task->next = NULL;
        prev->next = task;
    }
}

void MiniTask_ClearLists(MiniTaskPool* pool) {
    pool->freeList.next    = NULL;
    pool->activeList.next  = NULL;
    pool->pendingList.next = NULL;
}

BOOL MiniTask_InitPool(MiniTaskPool* pool, void* heapData, u32 heapSize, u32 count) {
    BOOL      ret = FALSE;
    MiniTask* task;
    u32       i;

    MiniTask_ClearLists(pool);
    pool->heapData = heapData;
    pool->heapSize = heapSize;
    Mem_InitializeHeap(&pool->heap, heapData, heapSize);
    pool->maxTasks = count;
    task           = MiniTask_Alloc(pool, count * sizeof(MiniTask));
    pool->tasks    = task;
    if (task != NULL) {
        pool->lastFreedTask = task;
        for (i = 0; i < count; i++) {
            task->flags            = 0;
            task->generation       = 0;
            task->childCount       = 0;
            task->parentGeneration = 0;
            task->id               = i;
            task->parentId         = 0xFFFF;
            task->priority         = 0;
            task->next             = pool->freeList.next;
            pool->freeList.next    = task;
            task++;
        }
        ret = TRUE;
    }
    return ret;
}

void MiniTask_CleanupAll(MiniTaskPool* pool) {
    MiniTask* current;
    MiniTask* next;

    current = pool->pendingList.next;
    while (current != NULL) {
        next = current->next;
        MiniTask_Recycle(pool, &pool->pendingList, current);
        current = next;
    }

    current = pool->activeList.next;
    while (current != NULL) {
        next = current->next;
        MiniTask_Recycle(pool, &pool->activeList, current);
        current = next;
    }
}

void MiniTask_UpdatePool(MiniTaskPool* pool) {
    MiniTask_ProcessPending(pool);
    MiniTask_UpdateActive(pool);
}

void MiniTask_ProcessPending(MiniTaskPool* pool) {
    MiniTask* iter = pool->activeList.next;
    MiniTask* current;

    while (iter != NULL) {
        MiniTask* nextPtr = iter->next;
        if (iter->flags & MINITASK_FLAG_DELETE) {
            MiniTask* parent = MiniTask_GetByHandle(pool, (iter->parentGeneration << 16) | iter->parentId);
            if (parent != NULL) {
                parent->childCount--;
            }
            MiniTask_Recycle(pool, &pool->activeList, iter);
        }
        iter = nextPtr;
    }

    current = pool->pendingList.next;
    while (current != NULL) {
        MiniTask* next = current->next;
        MiniTask_Insert(pool, current);
        current = next;
    }
    pool->pendingList.next = NULL;

    for (iter = pool->activeList.next; iter != NULL; iter = iter->next) {
        if (!(iter->flags & (MINITASK_FLAG_DELETE | MINITASK_FLAG_PENDING))) {
            if (iter->update(iter, iter->data, 0, 1) == 0) {
                iter->flags |= MINITASK_FLAG_DELETE;
            }
        }
    }
}

void MiniTask_UpdateActive(MiniTaskPool* pool) {
    MiniTask* iter;

    for (iter = pool->activeList.next; iter != NULL; iter = iter->next) {
        if (!(iter->flags & (MINITASK_FLAG_DELETE | MINITASK_FLAG_PENDING))) {
            iter->update(iter, iter->data, 0, 2);
        }
    }
}

MiniTask* MiniTask_GetById(MiniTaskPool* pool, u32 handle) {
    u16       id   = handle;
    MiniTask* task = NULL;

    if (id < pool->maxTasks) {
        task = &pool->tasks[id];
        if (!(task->flags & MINITASK_FLAG_INUSE) || (task->flags & MINITASK_FLAG_DELETE)) {
            task = NULL;
        }
    }
    return task;
}

MiniTask* MiniTask_GetByHandle(MiniTaskPool* pool, u32 handle) {
    u16       generation = (handle & 0xFFFF0000) >> 16;
    MiniTask* task       = MiniTask_GetById(pool, handle);

    if (task != NULL && task->generation != generation) {
        task = NULL;
    }
    return task;
}
