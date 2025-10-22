#include <NitroSDK/os/interrupt.h>
#include <NitroSDK/os/valarm.h>
#include <NitroSDK/util.h>

static struct {
    VAlarm* head;
    VAlarm* tail;
} VAlarmQueue = {NULL, NULL};

static u16 VAlarmSystemActive = FALSE;

static s32 data_0207e05c = 0;
static s32 data_0207e058 = 0;

void OS_VAlarmSystemInit(void) {
    if (VAlarmSystemActive == FALSE) {
        VAlarmSystemActive = TRUE;

        VAlarmQueue.head = NULL;
        VAlarmQueue.tail = NULL;

        OS_DisableInterrupts(IRQ_VCOUNT);

        data_0207e05c = 0;
        data_0207e058 = 0;
    }
}

void OS_VAlarmSystemHalt(void) {
    ENTER_CRITICAL_SECTION();

    if (VAlarmSystemActive != 0) {
        VAlarmSystemActive = 0;
    }

    LEAVE_CRITICAL_SECTION();
}

BOOL OS_IsVAlarmSystemActive(void) {
    return VAlarmSystemActive;
}

static void OS_RemoveVAlarmFromQueue(VAlarm* alarm) {
    if (alarm == NULL) {
        return;
    }

    VAlarm* prev = alarm->prev;
    VAlarm* next = alarm->next;

    if (next != NULL) {
        next->prev = prev;
    } else {
        VAlarmQueue.tail = prev;
    }

    if (prev != NULL) {
        prev->next = next;
    } else {
        VAlarmQueue.head = next;
    }
}

static void OS_StopVAlarm(VAlarm* alarm) {
    ENTER_CRITICAL_SECTION();

    alarm->unk_24 = 1;

    if (alarm->callback == NULL) {
        LEAVE_CRITICAL_SECTION();
        return;
    }

    OS_RemoveVAlarmFromQueue(alarm);
    alarm->callback = NULL;

    LEAVE_CRITICAL_SECTION();
}

void OS_StopAllVAlarms(void) {
    ENTER_CRITICAL_SECTION();

    VAlarm* current;
    VAlarm* next;

    for (current = VAlarmQueue.head, next = (current != NULL) ? current->next : NULL; current != NULL;
         current = next, next = (current != NULL) ? current->next : NULL)
    {
        OS_StopVAlarm(current);
    }

    LEAVE_CRITICAL_SECTION();
}
