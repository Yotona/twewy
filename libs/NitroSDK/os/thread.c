#include <NitroSDK/os/mutex.h>
#include <NitroSDK/os/thread.h>
#include <NitroSDK/util.h>
#include <bios.h>

OSThread** CurrentThreadPtr;
void*      ThreadDtorStack = NULL;
u32        IdleThreadStack[50];
BOOL       ThreadSchedulerInitialized = FALSE;
u32        ThreadSchedulerCount       = 0;

OSThreadSwitchCallback ThreadSwitchCallback = NULL;
s32                    ActiveThreadCount    = 0;
OSThreadInfo           ThreadInfo;

OSThread LaunchThread;
OSThread IdleThread;

#define STACK_BOTTOM_MAGIC       0xFDDB597D
#define STACK_TOP_MAGIC          0x7BF9DD5B
#define OS_GetCurrentThreadPtr() (*CurrentThreadPtr)

static void                   OS_TriggerThreadImmediate(void);
static OSThreadSwitchCallback OS_SetThreadSwitchCallback(OSThreadSwitchCallback callback);
static void                   OS_IdleThreadProc(void*);
static void                   OS_TerminateThread(OSThread* thread, void* args);
static void                   OS_DestroyThread(void* args);
static void                   OS_SetThreadDestructor(OSThread* thread, OSThreadDestructor destructor);
static void                   OS_ExitAndDestroyThread(void);
static u32                    OS_DecreaseSchedulerSuspension(void);
static u32                    OS_IncreaseSchedulerSuspension(void);
static void                   OS_TriggerThread(void);

static s32 OS_GetNextFreeThread(void) {
    return ++ActiveThreadCount;
}

static void OS_PushThreadToQueue(OSThreadQueue* queue, OSThread* thread) {
    OSThread* next = queue->head;

    while (next != NULL && next->priority <= thread->priority) {
        if (next == thread) {
            return;
        }
        next = next->list.next;
    }

    if (next == NULL) {
        OSThread* prev = queue->tail;

        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->list.next = thread;
        }

        thread->list.prev = prev;
        thread->list.next = NULL;
        queue->tail       = thread;
    } else {
        OSThread* prev = next->list.prev;

        if (prev == NULL) {
            queue->head = thread;
        } else {
            prev->list.next = thread;
        }

        thread->list.prev = prev;
        thread->list.next = next;
        next->list.prev   = thread;
    }
}

static OSThread* OS_PopThreadFromQueue(OSThreadQueue* queue) {
    OSThread* thread = queue->head;
    if (thread != NULL) {
        queue->head = thread->list.next;
        if (queue->head != NULL) {
            queue->head->list.prev = NULL;
        } else {
            queue->tail   = NULL;
            thread->queue = NULL;
        }
    }
    return thread;
}

static OSThread* OS_RemoveThreadFromQueue(OSThreadQueue* queue, OSThread* thread) {
    OSThread* scanThread = queue->head;

    while (scanThread != NULL) {
        OSThread* next = scanThread->list.next;

        if (scanThread == thread) {
            OSThread* prev = scanThread->list.prev;

            if (queue->head == scanThread) {
                queue->head = next;
            } else {
                prev->list.next = next;
            }

            if (queue->tail == scanThread) {
                queue->tail = prev;
            } else {
                next->list.prev = prev;
            }
            break;
        }

        scanThread = next;
    }

    return scanThread;
}

OSMutex* OS_RemoveMutexFromQueue(OSMutexQueue* queue) {
    OSMutex* mutex = queue->head;

    if (mutex != NULL) {
        OSMutex* next = mutex->list.next;

        queue->head = next;
        if (next != NULL) {
            next->list.prev = NULL;
        } else {
            queue->tail = NULL;
        }
    }
    return mutex;
}

static void OS_PushThreadToList(OSThread* thread) {
    OSThread* scanThread = ThreadInfo.list;
    OSThread* prev       = NULL;

    while (scanThread != NULL && scanThread->priority < thread->priority) {
        prev       = scanThread;
        scanThread = scanThread->next;
    }

    if (prev == NULL) {
        thread->next    = ThreadInfo.list;
        ThreadInfo.list = thread;
    } else {
        thread->next = prev->next;
        prev->next   = thread;
    }
}

static void OS_RemoveThreadFromList(OSThread* thread) {
    OSThread* scanThread = ThreadInfo.list;
    OSThread* prev       = NULL;

    while (scanThread != NULL && scanThread != thread) {
        prev       = scanThread;
        scanThread = scanThread->next;
    }

    if (prev == NULL) {
        ThreadInfo.list = thread->next;
    } else {
        prev->next = thread->next;
    }
}

static void OS_TriggerThreadImmediate(void) {
    if (ThreadSchedulerCount <= 0) {
        OSThreadInfo* info = &ThreadInfo;

        if (info->irqDepth > 0 || func_0203a7c8() == 18) {
            info->isSchedulerWaiting = TRUE;
            return;
        }

        OSThread* currentThread = OS_GetCurrentThreadPtr();
        OSThread* nextThread    = OS_SelectThread();

        if (currentThread == nextThread || nextThread == NULL) {
            return;
        }

        if (currentThread->state != 2 && func_020392e8(currentThread)) {
            return;
        }

        if (ThreadSwitchCallback != NULL) {
            ThreadSwitchCallback(currentThread, nextThread);
        }

        if (info->callback != NULL) {
            info->callback(currentThread, nextThread);
        }

        OS_SetCurrentThread(nextThread);
        func_02039334(nextThread);
    }
}

void OS_InitThread(void) {
    if (ThreadSchedulerInitialized) {
        return;
    }
    ThreadSchedulerInitialized = TRUE;
    CurrentThreadPtr           = &ThreadInfo.current;

    LaunchThread.priority    = 0x10;
    LaunchThread.id          = 0;
    LaunchThread.stackBottom = 1;
    LaunchThread.next        = NULL;
    LaunchThread.profiler    = NULL;

    ThreadInfo.list = &LaunchThread;
    OS_SetCurrentThread(&LaunchThread);

    LaunchThread.stackTop    = BIOS_LAUNCHTHREAD_STACK_TOP;
    LaunchThread.stackBottom = BIOS_LAUNCHTHREAD_STACK_BOTTOM;
    LaunchThread.stackOffset = 0;

    *(u32*)(LaunchThread.stackBottom - 4) = STACK_BOTTOM_MAGIC;
    *(u32*)LaunchThread.stackTop          = STACK_TOP_MAGIC;

    LaunchThread.joinQueue.tail = NULL;
    LaunchThread.joinQueue.head = NULL;

    ThreadInfo.isSchedulerWaiting = FALSE;
    ThreadInfo.irqDepth           = 0;

    BIOS_MAIN_THREAD_INFO = &ThreadInfo;

    OS_SetThreadSwitchCallback(NULL);

    OS_CreateThread(&IdleThread, OS_IdleThreadProc, NULL, IdleThreadStack + 200 / sizeof(u32), 200, 0x1F);
    IdleThread.priority = 0x20;
    IdleThread.state    = 1;
}

void OS_CreateThread(OSThread* thread, void (*func)(void*), void* arg, void* stack, u32 stackSize, u32 priority) {
    ENTER_CRITICAL_SECTION();

    u32 freeThreadId = OS_GetNextFreeThread();

    thread->priority = priority;
    thread->id       = freeThreadId;
    thread->state    = 0;
    thread->profiler = NULL;

    OS_PushThreadToList(thread);
    thread->stackBottom = stack;
    thread->stackTop    = stack - stackSize;
    thread->stackOffset = 0;

    *(u32*)(thread->stackBottom - 4) = STACK_BOTTOM_MAGIC;
    *(u32*)thread->stackTop          = STACK_TOP_MAGIC;

    thread->joinQueue.tail = NULL;
    thread->joinQueue.head = NULL;

    func_0203927c(&thread->context, func, stack - 4);

    thread->context.r[0] = (u32)arg;
    thread->context.lr   = (u32)OS_ExitThread;

    func_0203b28c(0, stack - stackSize + 4, stackSize - 8);

    thread->mutex           = NULL;
    thread->mutexQueue.head = NULL;
    thread->mutexQueue.tail = NULL;

    OS_SetThreadDestructor(thread, NULL);

    thread->queue = NULL;

    thread->list.next = NULL;
    thread->list.prev = NULL;

    func_0203b28c(0, thread->unk_A4, 0xC);

    thread->unk_B0 = NULL;

    LEAVE_CRITICAL_SECTION();
}

void OS_ExitThread(void) {
    ENTER_CRITICAL_SECTION();

    OS_TerminateThread(OS_GetCurrentThread(), NULL);
}

static void OS_TerminateThread(OSThread* thread, void* args) {
    if (ThreadDtorStack != NULL) {
        func_0203927c(thread, OS_DestroyThread, ThreadDtorStack);
        thread->context.r[0] = (u32)args;
        thread->context.cpsr |= 0x80;
        thread->state = 1;
        func_02039334(thread);
    } else {
        OS_DestroyThread(args);
    }
}

static void OS_DestroyThread(void* args) {
    OSThread*          thread     = OS_GetCurrentThread();
    OSThreadDestructor destructor = thread->destructor;

    if (destructor != NULL) {
        thread->destructor = NULL;
        destructor(args);
        ENTER_CRITICAL_SECTION();
    }

    OS_ExitAndDestroyThread();
}

static void OS_ExitAndDestroyThread(void) {
    OSThread* thread = OS_GetCurrentThreadPtr();

    OS_IncreaseSchedulerSuspension();

    OS_UnlockAllQueuedThreadMutex(thread);
    if (thread->queue != NULL) {
        OS_RemoveThreadFromQueue(thread->queue, thread);
    }
    OS_RemoveThreadFromList(thread);
    thread->state = 2;
    OS_UnpauseThread(&thread->joinQueue);
    OS_DecreaseSchedulerSuspension();
    OS_TriggerThread();
    func_0203aad0();
}

void OS_PauseThread(OSThreadQueue* queue) {
    ENTER_CRITICAL_SECTION();

    OSThread* currentThread = OS_GetCurrentThreadPtr();

    if (queue != NULL) {
        currentThread->queue = queue;
        OS_PushThreadToQueue(queue, currentThread);
    }

    currentThread->state = 0;
    OS_TriggerThreadImmediate();

    LEAVE_CRITICAL_SECTION();
}

void OS_UnpauseThread(OSThreadQueue* queue) {
    ENTER_CRITICAL_SECTION();

    if (queue->head != NULL) {
        while (queue->head != NULL) {
            OSThread* thread  = OS_PopThreadFromQueue(queue);
            thread->state     = 1;
            thread->queue     = NULL;
            thread->list.next = NULL;
            thread->list.prev = NULL;
        }

        queue->tail = NULL;
        queue->head = NULL;
        OS_TriggerThreadImmediate();
    }

    LEAVE_CRITICAL_SECTION();
}

void OS_ResumeThreadImmediate(OSThread* thread) {
    ENTER_CRITICAL_SECTION();

    thread->state = 1;
    OS_TriggerThreadImmediate();

    LEAVE_CRITICAL_SECTION();
}

OSThread* OS_SelectThread(void) {
    OSThread* thread = ThreadInfo.list;

    while (thread != NULL && thread->state != 1) {
        thread = thread->next;
    }
    return thread;
}

static void OS_TriggerThread(void) {
    ENTER_CRITICAL_SECTION();

    OS_TriggerThreadImmediate();

    LEAVE_CRITICAL_SECTION();
}

BOOL OS_SetThreadPriority(OSThread* thread, u32 priority) {
    OSThread* scanThread = ThreadInfo.list;
    OSThread* prev       = NULL;

    ENTER_CRITICAL_SECTION();

    while (scanThread != NULL && scanThread != thread) {
        prev       = scanThread;
        scanThread = scanThread->next;
    }

    if (scanThread == NULL || scanThread == &IdleThread) {
        LEAVE_CRITICAL_SECTION();
        return FALSE;
    }

    if (scanThread->priority != priority) {
        if (prev == NULL) {
            ThreadInfo.list = thread->next;
        } else {
            prev->next = thread->next;
        }
        thread->priority = priority;
        OS_PushThreadToList(thread);
        OS_TriggerThreadImmediate();
    }

    LEAVE_CRITICAL_SECTION();
    return TRUE;
}

OSThreadSwitchCallback OS_SetThreadSwitchCallback(OSThreadSwitchCallback callback) {
    ENTER_CRITICAL_SECTION();

    OSThreadSwitchCallback oldCallback = ThreadSwitchCallback;
    ThreadSwitchCallback               = callback;

    LEAVE_CRITICAL_SECTION();
    return oldCallback;
}

static void OS_IdleThreadProc(void*) {
    OS_EnableIRQ();

    while (TRUE) {
        func_0203aae0();
    }
}

static u32 OS_IncreaseSchedulerSuspension(void) {
    ENTER_CRITICAL_SECTION();
    u32 count;

    if (ThreadSchedulerCount < -1) {
        count = ThreadSchedulerCount;
        ThreadSchedulerCount++;
    }

    LEAVE_CRITICAL_SECTION();
    return count;
}

static u32 OS_DecreaseSchedulerSuspension(void) {
    ENTER_CRITICAL_SECTION();
    u32 count = 0;

    if (ThreadSchedulerCount > 0) {
        count = ThreadSchedulerCount;
        ThreadSchedulerCount--;
    }

    LEAVE_CRITICAL_SECTION();
    return count;
}

static void OS_SetThreadDestructor(OSThread* thread, OSThreadDestructor destructor) {
    thread->destructor = destructor;
}
