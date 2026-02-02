#ifndef EASY_TASK_H
#define EASY_TASK_H

/**
 * @file EasyTask.h
 * @brief Header file for the EasyTask module, providing task pool management functionality.
 *
 * This module implements a task system where tasks are managed in pools, allowing for
 * creation, updating, and deletion of tasks with priorities and parent-child relationships.
 */

#include "Memory.h"
#include <types.h>

struct TaskPool;

/**
 * @struct Task
 * @brief Structure representing a single task in the task pool.
 */
typedef struct Task {
    /* 0x00 */ struct Task* next;
    /* 0x04 */ struct {
        u16 inUse        : 1;
        u16 isActive     : 1;
        u16 isPending    : 1;
        u16 ownsData     : 1;
        u16 markedForDel : 1;
        u16 reserved     : 11;
    };
    /* 0x06 */ u16 childCount;
    /* 0x08 */ u16 id;
    /* 0x0A */ u16 parentId;
    /* 0x0C */ u16 generation;
    /* 0x0E */ u16 parentGeneration;
    /* 0x10 */ s32 priority;
    /* 0x14 */ s32 (*update)(struct TaskPool*, struct Task*, void*, s32);
    /* 0x18 */ void*       data;
    /* 0x1C */ const char* name;
} Task; // Size: 0x20

/**
 * @struct TaskHandle
 * @brief Structure defining a task type, including its name, update function, and data size.
 */
typedef struct TaskHandle {
    /* 0x00 */ const char* taskName;
    /* 0x04 */ s32 (*taskFunc)(struct TaskPool*, struct Task*, void*, s32);
    /* 0x08 */ u32 dataSize;
} TaskHandle;

/**
 * @struct TaskPool
 * @brief Structure representing the task pool, managing free, active, and pending tasks.
 */
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

typedef union {
    struct {
        s32 (*initialize)();
        s32 (*update)();
        s32 (*render)();
        s32 (*cleanup)();
    };
    s32 (*iter[4])();
} TaskStages;

/**
 * @brief Initializes the task pool with the given memory pool and maximum task count.
 * @param taskPool Pointer to the TaskPool to initialize.
 * @param memPool Pointer to the memory pool to use for allocations.
 * @param count Maximum number of tasks the pool can hold.
 * @param alloc Custom allocation function, or NULL to use default.
 * @param free Custom free function, or NULL to use default.
 * @return TRUE if initialization succeeded, FALSE otherwise.
 */
BOOL EasyTask_InitializePool(TaskPool* taskPool, MemPool* memPool, u32 count, void* (*alloc)(TaskPool*, s32, const char*),
                             void (*free)(TaskPool*, void*));

/**
 * @brief Cleans up all tasks in the pool, recycling them to the free list.
 * @param taskPool Pointer to the TaskPool to clean up.
 */
void EasyTask_CleanupAllTasks(TaskPool* taskPool);

/**
 * @brief Destroys the task pool, cleaning up all tasks and freeing the task array.
 * @param taskPool Pointer to the TaskPool to destroy.
 */
void EasyTask_DestroyPool(TaskPool* taskPool);

/**
 * @brief Updates the task pool by processing pending tasks and updating active tasks.
 * @param taskPool Pointer to the TaskPool to update.
 */
void EasyTask_UpdatePool(TaskPool* taskPool);

/**
 * @brief Processes pending tasks, moving them to active list and handling deletions.
 * @param taskPool Pointer to the TaskPool.
 */
void EasyTask_ProcessPendingTasks(TaskPool* taskPool);

/**
 * @brief Updates all active tasks by calling their update functions.
 * @param taskPool Pointer to the TaskPool.
 */
void EasyTask_UpdateActiveTasks(TaskPool* taskPool);

/**
 * @brief Creates a new task in the pool.
 * @param taskPool Pointer to the TaskPool.
 * @param taskHandle Pointer to the TaskHandle defining the task type.
 * @param data Pointer to data for the task, or NULL to allocate.
 * @param arg3 Priority of the task.
 * @param parentTask Pointer to parent task, or NULL.
 * @param param Parameter passed to the task's update function during initialization.
 * @return Task ID if successful, -1 otherwise.
 */
s32 EasyTask_CreateTask(TaskPool* taskPool, TaskHandle* taskHandle, void* data, s32 arg3, Task* parentTask, void* param);

/**
 * @brief Marks a task for deletion.
 * @param taskPool Pointer to the TaskPool.
 * @param arg1 Task ID to delete.
 */
void EasyTask_DeleteTask(TaskPool* taskPool, u32 arg1);

/**
 * @brief Validates if a task ID is valid, setting it to -1 if not.
 * @param taskPool Pointer to the TaskPool.
 * @param arg1 Pointer to the task ID to validate.
 * @return TRUE if valid, FALSE otherwise.
 */
BOOL EasyTask_ValidateTaskId(TaskPool* taskPool, u32* arg1);

/**
 * @brief Retrieves a task by its ID.
 * @param taskPool Pointer to the TaskPool.
 * @param arg1 Task ID.
 * @return Pointer to the Task if found, NULL otherwise.
 */
Task* EasyTask_GetTaskById(TaskPool* taskPool, u32 arg1);

/**
 * @brief Retrieves the data associated with a task.
 * @param taskPool Pointer to the TaskPool.
 * @param arg1 Task ID.
 * @return Pointer to the task data, or NULL if task not found.
 */
void* EasyTask_GetTaskData(TaskPool* taskPool, u32 arg1);

#endif // EASY_TASK_H