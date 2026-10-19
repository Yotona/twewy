#ifndef FIELD_EVENT_EVENTFLAGS_H
#define FIELD_EVENT_EVENTFLAGS_H

#include <nitro/types.h>

typedef enum {
    GATE_GAME_CLEARED = 0, // The main storyline has been completed.
    GATE_UNKNOWN01    = 1,

    GATE_UNKNOWN47 = 47,
    GATE_UNKNOWN48 = 48,
    GATE_UNKNOWN49 = 49,
    GATE_UNKNOWN50 = 50,
    GATE_UNKNOWN51 = 51,
} EventGate;

void FldGate_Set(EventGate gate);
void FldGate_Clear(EventGate gate);
BOOL FldGate_Get(EventGate gate);
void func_ov030_020aebf0(s32 arg0);
s32  func_ov030_020aec08(s32 arg0);
void func_ov030_020aec1c(s32 arg0);
void func_ov030_020aec38(s32 arg0);
void func_ov030_020aec58(s32 arg0);
s32  func_ov030_020aeca4(s32 arg0);

#endif // FIELD_EVENT_EVENTFLAGS_H
