#include "EasyTask.h"

void  EasyTask_ProcessPendingTasks(TaskPool* taskPool);
void  EasyTask_UpdateActiveTasks(TaskPool* taskPool);
Task* func_0200f82c(TaskPool* taskPool, s32 arg1);

static s32 EasyTask_DummyUpdate(TaskPool* taskPool, Task* task, s32 arg2, s32 arg3) {
    return 1;
}

static BOOL EasyTask_RecycleTask(TaskPool* taskPool, Task* prev, Task* task) {
    Task* current = prev->next;

    if (current != NULL) {
        do {
            if (current == task) {
                prev->next = task->next;

                task->flags &= ~1;
                task->update(taskPool, task, 0, 3);
                if (((u32)(task->flags << 0x1c) >> 0x1f) == 1) {
                    taskPool->free(taskPool, task->data);
                }
                task->flags &= ~8;

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

        task->flags = (task->flags & ~1) | 1;
        task->flags = (task->flags | 2) & ~4;
        task->flags &= ~8;
        task->flags &= ~0x10;

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

    task->flags &= ~2;
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
            taskData->flags &= ~1;
            taskData->flags &= ~2;
            taskData->flags &= ~4;
            taskData->flags &= ~8;
            taskData->flags &= ~0x10;
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

// Nonmatching: Some flipped registers
// Scratch: wbMqj
void EasyTask_ProcessPendingTasks(TaskPool* taskPool) {
    Task* iter = taskPool->activeList.next;
    while (iter != NULL) {
        Task* nextPtr = iter->next;
        if (((u32)(iter->flags << 0x1B) >> 0x1F) == 1) {
            Task* temp = func_0200f82c(taskPool, (iter->parentGeneration << 0x10) | iter->parentId);
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
        if (((u32)(iter->flags << 0x1D) >> 0x1F) == 0 && ((u32)(iter->flags << 0x1B) >> 0x1F) == 0) {
            if (iter->update(taskPool, iter, 0, 1) == 0) {
                iter->flags |= (1 << 4);
            }
        }
        iter = iter->next;
    }
}

// Nonmatching: Registers swapped
// Scratch: NYhjL
void EasyTask_UpdateActiveTasks(TaskPool* taskPool) {
    Task* iter = taskPool->activeList.next;

    while (iter != NULL) {
        if (((u32)(iter->flags << 0x1D) >> 0x1F) == 0 && ((u32)(iter->flags << 0x1B) >> 0x1F) == 0) {
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
        task->flags |= 8;
        void* allocated = taskPool->alloc(taskPool, taskHandle->dataSize, taskHandle->taskName);
        task->data      = allocated;
        if (allocated == NULL) {
            EasyTask_RecycleTask(taskPool, &taskPool->pendingList, task);
            return -1;
        }
    } else {
        task->flags &= ~8;
        task->data = data;
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

void func_0200f7a0(TaskPool* taskPool, u32 arg1) {
    Task* task = func_0200f82c(taskPool, arg1);
    if (task != NULL) {
        task->flags |= 0x10;
    }
}

BOOL func_0200f7bc(TaskPool* taskPool, s32* arg1) {
    BOOL success = TRUE;
    if (func_0200f82c(taskPool, *arg1) == 0) {
        *arg1   = -1;
        success = FALSE;
    }
    return success;
}

static void* func_0200f7e8(TaskPool* taskPool, u16 arg1) {
    u16   temp_r0;
    Task* var_r2;

    var_r2 = NULL;
    if (taskPool->maxTasks > arg1) {
        var_r2  = taskPool->taskArray + (arg1 << 5);
        temp_r0 = var_r2->flags;
        if ((((u32)(temp_r0 << 0x1F) >> 0x1F) != 0) && (((u32)(temp_r0 << 0x1B) >> 0x1F) != 1)) {

        } else {
            var_r2 = NULL;
        }
    }
    return var_r2;
}

Task* func_0200f82c(TaskPool* taskPool, s32 arg1) {
    Task* task = func_0200f7e8(taskPool, arg1);
    if ((task != NULL) && (task->generation != (arg1 >> 0x10))) {
        task = NULL;
    }
    return task;
}

void* EasyTask_GetTaskData(TaskPool* taskPool, u32 arg1) {
    void* data = NULL;

    Task* task = func_0200f82c(taskPool, arg1);
    if (task != NULL) {
        data = task->data;
    }
    return data;
}
