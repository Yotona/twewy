#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include "game.h"
#include <types.h>

typedef void (*OverlayCB)(GameState*);

typedef struct {
    /* 0x00 */ s32       id;
    /* 0x04 */ OverlayCB cb;
} OverlayTag;

typedef struct {
    /* 0x00 */ OverlayTag tag;
    /* 0x08 */ GameState* gState;
    /* 0x0C */ s32        unk_0C;
} OverlayData;

typedef struct {
    /* 0x00 */ OverlayData* data;
    /* 0x04 */ OverlayCB    cb;
    /* 0x08 */ GameState*   gState;
    /* 0x0C */ s32          unk_0C;
    /* 0x10 */ u32          unk_10;
    /* 0x14 */ OverlayData  unk_14[17];
} Overlay;

void       func_02007174(OverlayTag* tag);
void       func_020071f4(OverlayTag* tag, s32 overlayId, void* callback, GameState* state, s32 param_5);
GameState* func_02007260(void* state);
s32        func_02007278(void);
void       func_020072a4(void);
void       func_020072b8(void);
void       func_020072ec(void);
void       func_02007300(OverlayCB callback, GameState* state, s32 param_3);
s32        func_02007328(void);
s32        func_0200734c(OverlayCB callback, GameState* state, s32 param_3);
BOOL       func_0200737c(void);
void       func_02007390(void);

#endif // OVERLAYMANAGER_H
