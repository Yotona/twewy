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
    /* 0x0C */ s32        repeatCount;
} OverlayData;

typedef struct {
    /* 0x00 */ OverlayData* data;
    /* 0x04 */ OverlayCB    cb;
    /* 0x08 */ void*        gState;
    /* 0x0C */ s32          unk_0C;
    /* 0x10 */ u32          stackDepth;
    /* 0x14 */ OverlayData  stack[17];
} OverlayDispatcher;

/**
 * @brief Initialize the main overlay dispatcher.
 */
void MainOvlDisp_Init(void);

/**
 * @brief Push a new overlay dispatch request onto the main overlay dispatcher's stack.
 *
 * @param overlayId The ID of the overlay the request is for.
 * @param callback The function to be called from the overlay.
 */
void MainOvlDisp_Push(s32 overlayId, OverlayCB callback, void* state, s32 repeatCount);

/**
 * @brief Pop the top overlay dispatch request from the main overlay dispatcher's stack.
 *
 * @param tag Pointer to an OverlayTag structure to receive the popped overlay tag.
 */
void MainOvlDisp_Pop(OverlayTag* tag);

/**
 * @brief Replace the top overlay dispatch request on the main overlay dispatcher's stack.
 *
 * @param tag Pointer to an OverlayTag structure to receive the replaced overlay tag.
 * @param overlayId The ID of the new overlay.
 * @param callback The function to be called from the new overlay.
 */
void MainOvlDisp_ReplaceTop(OverlayTag* tag, s32 overlayId, void* callback, void* state, s32 param_5);

void* MainOvlDisp_SetState(void* state);

/**
 * @brief Get the count of times the current overlay dispatcher has repeated a request
 *
 * @return The repeat count
 */
s32 MainOvlDisp_GetRepeatCount(void);

/**
 * @brief Set the count of times the current overlay dispatcher has repeated a request
 *
 * @param repeatCount The new repeat count
 */
void MainOvlDisp_SetRepeatCount(s32 repeatCount);

/**
 * @brief Increment the count of how many times main overlay dispatcher's current overlay request has been repeated
 */
void MainOvlDisp_IncrementRepeatCount(void);

/**
 * @brief Run the main overlay dispatcher's current overlay request
 */
void MainOvlDisp_Run(void);

void DebugOvlDisp_Init(void);

void DebugOvlDisp_Push(OverlayCB callback, void* state, s32 param_3);

s32 DebugOvlDisp_Pop(void);

s32 DebugOvlDisp_ReplaceTop(OverlayCB callback, void* state, s32 param_3);

BOOL DebugOvlDisp_IsStackAtBase(void);

void DebugOvlDisp_Run(void);

/**
 * @brief Initialize an overlay dispatcher that appears to be unused.
 */
void OvlDisp_InitUnused1(void);

/**
 * @brief Initialize an overlay dispatcher that appears to be unused.
 */
void OvlDisp_InitUnused2(void);

#endif // OVERLAYMANAGER_H
