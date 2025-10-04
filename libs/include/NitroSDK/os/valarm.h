#ifndef NITROSDK_OS_VCNT_H
#define NITROSDK_OS_VCNT_H

#include <types.h>

typedef struct VAlarm {
    /* 0x00 */ void (*callback)(void*);
    /* 0x04 */ void*          arg;
    /* 0x08 */ u32            unk_08; // tag
    /* 0x0C */ u32            unk_0C; // frame
    /* 0x10 */ s16            unk_10; // fire
    /* 0x12 */ s16            unk_12; // delay
    /* 0x14 */ struct VAlarm* prev;
    /* 0x18 */ struct VAlarm* next;
    /* 0x1C */ s32            unk_1C; // period
    /* 0x20 */ s32            unk_20; // finish
    /* 0x24 */ s32            unk_24; // canceled
} VAlarm;

void OS_VAlarmSystemInit(void);
void OS_VAlarmSystemHalt(void);
BOOL OS_IsVAlarmSystemActive(void);
void OS_StopAllVAlarms(void);

#endif // NITROSDK_OS_VCNT_H