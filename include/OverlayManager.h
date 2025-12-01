#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include <types.h>

// Function pointer type for overlay callbacks.
// The callbacks call either an overlay init, overlay main(? verify this), or overlay exit function from the overlay function
// pointer table.
typedef void (*OverlayCB)(void*);

typedef struct {
    /* 0x00 */ s32       id;
    /* 0x04 */ OverlayCB cb;
} OverlayTag;

typedef struct {
    /* 0x00 */ OverlayTag tag;
    /* 0x08 */ void*      gState;
    /* 0x0C */ s32 unk_0C; // Possibly the index to the overlay function pointer table? Why is it sometimes set to 0x7FFFFFFF?
} OverlayData;

typedef struct {
    /* 0x00 */ OverlayData* data;
    /* 0x04 */ OverlayCB    cb;         // TODO: Why is there a second cb here when OverlayData->tag also has one?
    /* 0x08 */ void*        gState;     // TODO: Why is there a second gState pointer here when OverlayData also has one?
    /* 0x0C */ s32          unk_0C;     // TODO: find more functions that set the value for this.
    /* 0x10 */ u32          unk_10;     // Number of overlays currently loaded?
    /* 0x14 */ OverlayData  unk_14[17]; // Stack of OverlayData for currently loaded overlays. TODO: Why 17?
} OverlayManager;

void func_02006f78(OverlayManager* overlayMgr, s32 param_2, OverlayCB func, void* param_4, s32 param_5);

void  func_02007174(OverlayTag* tag);
void  func_020071f4(OverlayTag* tag, s32 overlayId, void* callback, void* state, s32 param_5);
void* func_02007260(void* state);
s32   func_02007278(void);
void  func_0200728c(s32 param_1);
void  func_020072a4(void);
void  func_020072b8(void);
void  func_020072ec(void);
void  func_02007300(OverlayCB callback, void* state, s32 param_3);
s32   func_02007328(void);
s32   func_0200734c(OverlayCB callback, void* state, s32 param_3);
BOOL  func_0200737c(void);
void  func_02007390(void);

#endif // OVERLAYMANAGER_H
