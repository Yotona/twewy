#ifndef SYSSTATE_H
#define SYSSTATE_H

#include "Input.h"
#include <types.h>

extern vu32 SystemStatusFlags;

typedef enum {
    SYSFLAG_NONE       = 0,
    SYSFLAG_UNKNOWN_0  = (1 << 0),
    SYSFLAG_RESET      = (1 << 1),
    SYSFLAG_UNKNOWN_2  = (1 << 2),
    SYSFLAG_UNKNOWN_3  = (1 << 3),
    SYSFLAG_UNKNOWN_4  = (1 << 4),
    SYSFLAG_UNKNOWN_5  = (1 << 5),
    SYSFLAG_UNKNOWN_6  = (1 << 6),
    SYSFLAG_UNKNOWN_7  = (1 << 7),
    SYSFLAG_UNKNOWN_8  = (1 << 8),
    SYSFLAG_UNKNOWN_9  = (1 << 9),
    SYSFLAG_UNKNOWN_10 = (1 << 10)
} SysFlag;

typedef struct {
    /* 0x00 */ u16  currButtons;    // Current frame state
    /* 0x02 */ u16  pressedButtons; // Newly pressed this frame
    /* 0x04 */ u16  holdButtons;    // Still being held this frame
    /* 0x06 */ u16  prevButtons;    // Previous frame state
    /* 0x08 */ char unk_08[0x18];
    /* 0x20 */ u32  unk_20;
    /* 0x24 */ void (*vBlankCallback)(void);
    /* 0x28 */ void (*hBlankCallback)(void);
} SystemControl;
extern SystemControl SysControl;

#define SYSFLAG_BIT_INDEX(f)          \
    ((f) == SYSFLAG_UNKNOWN_0    ? 0  \
     : (f) == SYSFLAG_RESET      ? 1  \
     : (f) == SYSFLAG_UNKNOWN_2  ? 2  \
     : (f) == SYSFLAG_UNKNOWN_3  ? 3  \
     : (f) == SYSFLAG_UNKNOWN_4  ? 4  \
     : (f) == SYSFLAG_UNKNOWN_5  ? 5  \
     : (f) == SYSFLAG_UNKNOWN_6  ? 6  \
     : (f) == SYSFLAG_UNKNOWN_7  ? 7  \
     : (f) == SYSFLAG_UNKNOWN_8  ? 8  \
     : (f) == SYSFLAG_UNKNOWN_9  ? 9  \
     : (f) == SYSFLAG_UNKNOWN_10 ? 10 \
                                 : 0)

// Returns 1 if the specified flag is set, 0 if cleared
#define System_CheckFlag(flag) (u32)(((SystemStatusFlags) << (31 - SYSFLAG_BIT_INDEX((flag)))) >> 31)

// Sets the specified flag bit(s) to 1, leaving other bits unchanged
static inline void System_SetFlag(SysFlag flag) {
    SystemStatusFlags |= flag;
}

// Clears the specified flag bit(s) to 0, leaving other bits unchanged
static inline void System_ClearFlag(SysFlag flag) {
    SystemStatusFlags &= ~flag;
}

// Forces the specified flag bit(s) to 1 via clear-then-set operation
static inline void System_ForceFlag(SysFlag flag) {
    SystemStatusFlags = SystemStatusFlags & ~flag | flag;
}

#endif // SYSSTATE_H