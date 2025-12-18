#ifndef SYSSTATE_H
#define SYSSTATE_H

#include "Input.h"
#include <types.h>

extern volatile struct {
    u32 vblank   : 1; // In VBlank period of the frame
    u32 reset    : 1; // The game is soft-resetting
    u32 unk_02   : 1;
    u32 unk_03   : 1;
    u32 unk_04   : 1;
    u32 unk_05   : 1;
    u32 unk_06   : 1;
    u32 unk_07   : 1;
    u32 unk_08   : 1;
    u32 unk_09   : 1;
    u32 unk_10   : 1;
    u32 reserved : 21;
} SystemStatusFlags;

typedef struct {
    /* 0x00 */ InputButtons buttonState;
    /* 0x08 */ u32          loadedOverlays[6];
    /* 0x20 */ u32          frameCount;
    /* 0x24 */ void (*vBlankCallback)(void);
    /* 0x28 */ void (*hBlankCallback)(void);
} SystemControl;
extern SystemControl SysControl;

#endif // SYSSTATE_H