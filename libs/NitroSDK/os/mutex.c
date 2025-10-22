#include <NitroSDK/os/mutex.h>
#include <NitroSDK/util.h>

static void OS_PushThreadMutex(OSThread* thread, OSMutex* mutex);
static void OS_RemoveThreadMutex(OSThread* thread, OSMutex* mutex);

void OS_InitMutex(OSMutex* mutex) {
    OS_InitThreadQueue(&mutex->queue);
    mutex->thread = NULL;
    mutex->count  = 0;
}

void OS_LockMutex(OSMutex* mutex) {
    ENTER_CRITICAL_SECTION();

    OSThread* currentThread = OS_GetCurrentThread();

    while (TRUE) {
        OSThread* owner = mutex->thread;

        if (owner == NULL) {
            mutex->thread = currentThread;
            mutex->count++;
            OS_PushThreadMutex(currentThread, mutex);
            break;
        } else if (owner == currentThread) {
            mutex->count++;
            break;
        } else {
            currentThread->mutex = mutex;
            OS_PauseThread(&mutex->queue);
            currentThread->mutex = NULL;
        }
    }

    LEAVE_CRITICAL_SECTION();
}

void OS_UnlockMutex(OSMutex* mutex) {
    ENTER_CRITICAL_SECTION();

    OSThread* currentThread = OS_GetCurrentThread();

    if (mutex->thread == currentThread) {
        if (--mutex->count == 0) {
            OS_RemoveThreadMutex(currentThread, mutex);
            mutex->thread = NULL;
            OS_UnpauseThread(&mutex->queue);
        }
    }

    LEAVE_CRITICAL_SECTION();
}

void OS_UnlockAllQueuedThreadMutex(OSThread* thread) {
    while (thread->mutexQueue.head != NULL) {
        OSMutex* mutex = OS_RemoveMutexFromQueue(&thread->mutexQueue);
        mutex->count   = 0;
        mutex->thread  = NULL;
        OS_UnpauseThread(&mutex->queue);
    }
}

BOOL OS_TryLockMutex(OSMutex* mutex) {
    ENTER_CRITICAL_SECTION();

    BOOL couldLock = FALSE;

    OSThread* currentThread = OS_GetCurrentThread();

    if (mutex->thread == NULL) {
        mutex->thread = currentThread;
        mutex->count++;
        OS_PushThreadMutex(currentThread, mutex);
        couldLock = TRUE;
    } else if (mutex->thread == currentThread) {
        mutex->count++;
        couldLock = TRUE;
    } else {
        couldLock = FALSE;
    }

    LEAVE_CRITICAL_SECTION();
    return couldLock;
}

static void OS_PushThreadMutex(OSThread* thread, OSMutex* mutex) {
    OSMutex* tail = thread->mutexQueue.tail;

    if (tail == NULL) {
        thread->mutexQueue.head = mutex;
    } else {
        tail->list.next = mutex;
    }
    mutex->list.prev        = tail;
    mutex->list.next        = NULL;
    thread->mutexQueue.tail = mutex;
}

static void OS_RemoveThreadMutex(OSThread* thread, OSMutex* mutex) {
    OSMutex* next = mutex->list.next;
    OSMutex* prev = mutex->list.prev;

    if (next == NULL) {
        thread->mutexQueue.tail = prev;
    } else {
        next->list.prev = prev;
    }

    if (prev == NULL) {
        thread->mutexQueue.head = next;
    } else {
        prev->list.next = next;
    }
}
