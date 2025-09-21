#ifndef DEBUG_LAUNCHER_H
#define DEBUG_LAUNCHER_H

#include "OverlayManager.h"

typedef struct {
    /* 0x00 */ s32         unk_00;
    /* 0x04 */ const char* function;
    /* 0x08 */ const char* description;
    /* 0x0C */ u32         overlayId;
    /* 0x10 */ OverlayCB   overlayCB;
} DebugLauncherOption; // Size: 0x14

typedef struct {
    /* 0x00 */ s32                  unk_00;
    /* 0x04 */ const char*          creator;
    /* 0x08 */ const char*          description;
    /* 0x0C */ DebugLauncherOption* options;
} DebugLauncherCategory; // Size: 0x10

typedef struct {
    /* 0x00000 */ BOOL      active;
    /* 0x00004 */ s32       overlay;
    /* 0x00008 */ OverlayCB overlayCB;
    /* 0x0000C */ s16       currButtons;
    /* 0x0000E */ s16       pressedButtons;
    /* 0x00010 */ s16       holdButtons;
    /* 0x00012 */ s16       prevButtons;
    /* 0x00014 */ char      unk_00014[0x1163C];
    /* 0x11650 */ s32       unk_11650;
    /* 0x11654 */ char      unk_11654[0x4B00];
} DebugLauncherState; // Size: 0x16154

typedef void (*DebugLauncherFunc)(DebugLauncherState*);

#endif // DEBUG_LAUNCHER_H