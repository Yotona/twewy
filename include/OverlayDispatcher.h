#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include "OverlayManager.h"
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
    /* 0x08 */ void*      cbArg;
    /* 0x0C */ s32        repeatCount;
} OverlayData;

typedef struct {
    /* 0x00 */ OverlayData* data;
    /* 0x04 */ OverlayCB    cb;
    /* 0x08 */ void*        cbArg;
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
 * @param cbArg The argument to pass to the callback function.
 * @param repeatCount The number of times the request has been repeated so far.
 */
void MainOvlDisp_Push(u32 overlayId, OverlayCB callback, void* cbArg, s32 repeatCount);

/**
 * @brief Pop the top overlay dispatch request from the main overlay dispatcher's stack. The popped request is executed.
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
 * @param cbArg The argument to pass to the new callback function.
 * @param repeatCount The number of times the new request has been repeated so far.
 */
void MainOvlDisp_ReplaceTop(OverlayTag* tag, s32 overlayId, void* callback, void* cbArg, s32 repeatCount);

/**
 * @brief Set the callback argument for the main overlay dispatcher.
 *
 * @param arg The new callback argument.
 * @return The previous callback argument.
 */
void* MainOvlDisp_SetCbArg(void* cbArg);

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

/**
 * @brief Initialize the auxiliary overlay dispatcher.
 */
void DebugOvlDisp_Init(void);

/**
 * @brief Push a new overlay dispatch request onto the auxiliary overlay dispatcher's stack.
 *
 * @param callback The function to be called from the overlay.
 * @param cbArg The argument to pass to the callback function.
 * @param repeatCount The number of times the request has been repeated so far.
 */
void DebugOvlDisp_Push(OverlayCB callback, void* cbArg, s32 repeatCount);

/**
 * @brief Pop the top overlay dispatch request from the auxiliary overlay dispatcher's stack. The popped request is not
 * executed.
 *
 * @return The callback ID of the popped overlay tag.
 */
OverlayCB DebugOvlDisp_Pop(void);

/**
 * @brief Replace the top overlay dispatch request on the auxiliary overlay dispatcher's stack.
 *
 * @param callback The function to be called from the new overlay.
 * @param cbArg The argument to pass to the new callback function.
 * @param repeatCount The number of times the new request has been repeated so far.
 * @return The callback of the replaced overlay tag.
 */
OverlayCB DebugOvlDisp_ReplaceTop(OverlayCB callback, void* cbArg, s32 repeatCount);

/**
 * @brief Check if the auxiliary overlay dispatcher's stack is at its base (only one overlay request present).
 *
 * @return TRUE if the stack is at its base, FALSE otherwise.
 */
BOOL DebugOvlDisp_IsStackAtBase(void);

/**
 * @brief Run the auxiliary overlay dispatcher's current overlay request
 */
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
