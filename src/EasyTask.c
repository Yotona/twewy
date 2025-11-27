#include "EasyTask.h"

void  EasyTask_ProcessPendingTasks(TaskPool* taskPool);
void  EasyTask_UpdateActiveTasks(TaskPool* taskPool);
Task* EasyTask_GetTaskById(TaskPool* taskPool, u32 arg1);

static s32 EasyTask_DummyUpdate(TaskPool* taskPool, Task* task, s32 arg2, s32 arg3) {
    return 1;
}

static BOOL EasyTask_RecycleTask(TaskPool* taskPool, Task* prev, Task* task) {
    Task* current = prev->next;

    if (current != NULL) {
        do {
            if (current == task) {
                prev->next = task->next;

                task->inUse = FALSE;
                task->update(taskPool, task, 0, 3);
                if (task->ownsData == TRUE) {
                    taskPool->free(taskPool, task->data);
                }
                task->ownsData = FALSE;

                task->generation++;
                taskPool->lastFreedTask->next = task;
                taskPool->lastFreedTask       = task;
                task->next                    = NULL;
                return TRUE;
            }

            prev    = current;
            current = current->next;
        } while (current != NULL);
    }
    return FALSE;
}

// Nonmatching: Two instructions swapped
// Scratch: nG1sP
static Task* EasyTask_AllocateTask(TaskPool* taskPool, s32 priority) {
    static const char* taskName = "no name";

    Task* task = taskPool->freeList.next;
    if (task != NULL) {
        taskPool->freeList.next = task->next;

        task->inUse            = TRUE;
        task->isActive         = TRUE;
        task->isPending        = FALSE;
        task->ownsData         = FALSE;
        task->markedForDel     = FALSE;
        task->childCount       = 0;
        task->parentId         = 0xFFFF;
        task->parentGeneration = 0;
        task->priority         = priority;
        task->update           = EasyTask_DummyUpdate;
        task->data             = NULL;
        task->name             = taskName;
    }
    return task;
}

static void EasyTask_InsertTask(TaskPool* taskPool, Task* task) {
    Task* prev    = &taskPool->activeList;
    Task* current = taskPool->activeList.next;

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

    task->isActive = FALSE;
}

static void* EasyTask_Alloc(TaskPool* taskPool, s32 size, const char* sequence) {
    void* data = Mem_AllocHeapTail(taskPool->memPool, size);
    Mem_SetSequence(taskPool->memPool, data, sequence);
    return data;
}

static void EasyTask_Free(TaskPool* taskPool, void* data) {
    Mem_Free(taskPool->memPool, data);
}

BOOL EasyTask_InitializePool(TaskPool* taskPool, MemPool* memPool, u32 count, void* (*alloc)(TaskPool*, s32, const char*),
                             void (*free)(TaskPool*, void*)) {
    BOOL ret = FALSE;
    if (alloc == NULL) {
        alloc = EasyTask_Alloc;
    }
    if (free == NULL) {
        free = EasyTask_Free;
    }
    taskPool->memPool   = memPool;
    taskPool->alloc     = alloc;
    taskPool->free      = free;
    taskPool->maxTasks  = count;
    taskPool->taskArray = EasyTask_Alloc(taskPool, count * sizeof(Task), "EasyTask TCB");

    taskPool->freeList.next    = NULL;
    taskPool->activeList.next  = NULL;
    taskPool->pendingList.next = NULL;

    Task* taskData = (Task*)taskPool->taskArray;
    if (taskData != NULL) {
        taskPool->lastFreedTask = taskData;
        for (s32 i = 0; i < count; i++) {
            taskData->inUse            = FALSE;
            taskData->isActive         = FALSE;
            taskData->isPending        = FALSE;
            taskData->ownsData         = FALSE;
            taskData->markedForDel     = FALSE;
            taskData->childCount       = 0;
            taskData->id               = i;
            taskData->parentId         = 0xFFFF;
            taskData->generation       = 0;
            taskData->parentGeneration = 0;
            taskData->priority         = 0;
            taskData->next             = taskPool->freeList.next;
            taskPool->freeList.next    = taskData;
            taskData++;
        }
        ret = TRUE;
    }
    return ret;
}

void EasyTask_CleanupAllTasks(TaskPool* taskPool) {
    Task* current;
    Task* next;

    current = taskPool->pendingList.next;
    while (current != NULL) {
        next = current->next;
        EasyTask_RecycleTask(taskPool, &taskPool->pendingList, current);
        current = next;
    }

    current = taskPool->activeList.next;
    while (current != NULL) {
        next = current->next;
        EasyTask_RecycleTask(taskPool, &taskPool->activeList, current);
        current = next;
    }
}

void EasyTask_DestroyPool(TaskPool* taskPool) {
    EasyTask_CleanupAllTasks(taskPool);
    EasyTask_Free(taskPool, taskPool->taskArray);
}

void EasyTask_UpdatePool(TaskPool* taskPool) {
    EasyTask_ProcessPendingTasks(taskPool);
    EasyTask_UpdateActiveTasks(taskPool);
}

void EasyTask_ProcessPendingTasks(TaskPool* taskPool) {
    Task* iter = taskPool->activeList.next;
    while (iter != NULL) {
        Task* nextPtr = iter->next;
        if (iter->markedForDel == TRUE) {
            Task* temp = EasyTask_GetTaskById(taskPool, (iter->parentGeneration << 0x10) | iter->parentId);
            if (temp != NULL) {
                temp->childCount--;
            }
            EasyTask_RecycleTask(taskPool, &taskPool->activeList, iter);
        }
        iter = nextPtr;
    }

    Task* current = taskPool->pendingList.next;
    while (current != NULL) {
        Task* next = current->next;
        EasyTask_InsertTask(taskPool, current);
        current = next;
    }
    taskPool->pendingList.next = NULL;

    iter = taskPool->activeList.next;
    while (iter != NULL) {
        if (iter->isPending == FALSE && iter->markedForDel == FALSE) {
            if (iter->update(taskPool, iter, 0, 1) == 0) {
                iter->markedForDel = TRUE;
            }
        }
        iter = iter->next;
    }
}

void EasyTask_UpdateActiveTasks(TaskPool* taskPool) {
    Task* iter = taskPool->activeList.next;

    while (iter != NULL) {
        if (iter->isPending == FALSE && iter->markedForDel == FALSE) {
            iter->update(taskPool, iter, 0, 2);
        }
        iter = iter->next;
    };
}

s32 EasyTask_CreateTask(TaskPool* taskPool, TaskHandle* taskHandle, void* data, s32 arg3, Task* parentTask, s32 param) {
    Task* task = EasyTask_AllocateTask(taskPool, arg3);
    if (task == NULL) {
        return -1;
    }
    task->next                 = taskPool->pendingList.next;
    taskPool->pendingList.next = task;
    if (data == NULL && taskHandle->dataSize != 0) {
        task->ownsData  = TRUE;
        void* allocated = taskPool->alloc(taskPool, taskHandle->dataSize, taskHandle->taskName);
        task->data      = allocated;
        if (allocated == NULL) {
            EasyTask_RecycleTask(taskPool, &taskPool->pendingList, task);
            return -1;
        }
    } else {
        task->ownsData = FALSE;
        task->data     = data;
    }
    task->name                                = taskHandle->taskName;
    s32 (*update)(TaskPool*, Task*, s32, s32) = taskHandle->taskFunc;
    task->update                              = update;
    if (update(taskPool, task, param, 0) == 0) {
        EasyTask_RecycleTask(taskPool, &taskPool->pendingList, task);
        return -1;
    }
    if (parentTask != NULL) {
        task->parentId         = parentTask->id;
        task->parentGeneration = parentTask->generation;
        parentTask->childCount++;
    }
    return (task->generation << 0x10) | task->id;
}

void EasyTask_DeleteTask(TaskPool* taskPool, u32 arg1) {
    Task* task = EasyTask_GetTaskById(taskPool, arg1);
    if (task != NULL) {
        task->markedForDel = TRUE;
    }
}

BOOL EasyTask_ValidateTaskId(TaskPool* taskPool, u32* arg1) {
    BOOL success = TRUE;
    if (EasyTask_GetTaskById(taskPool, *arg1) == NULL) {
        *arg1   = -1;
        success = FALSE;
    }
    return success;
}

// Nonmatching: Missing a shift instruction
// Scratch: PPLQK
static void* EasyTask_GetTaskByIdUnchecked(TaskPool* taskPool, u32 arg1) {
    Task* task = NULL;
    if ((u16)arg1 < taskPool->maxTasks) {
        task = taskPool->taskArray + (arg1 << 5);
        if (task->inUse == FALSE || task->markedForDel == TRUE) {
            task = NULL;
        }
    }
    return task;
}

// Nonmatching: Missing shift instructions
// Scratch: U1mfd
Task* EasyTask_GetTaskById(TaskPool* taskPool, u32 arg1) {
    Task* task = EasyTask_GetTaskByIdUnchecked(taskPool, arg1);
    if ((task != NULL) && (task->generation != (arg1 >> 0x10))) {
        task = NULL;
    }
    return task;
}

void* EasyTask_GetTaskData(TaskPool* taskPool, u32 arg1) {
    void* data = NULL;

    Task* task = EasyTask_GetTaskById(taskPool, arg1);
    if (task != NULL) {
        data = task->data;
    }
    return data;
}
