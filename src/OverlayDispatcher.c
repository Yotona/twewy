#include "OverlayDispatcher.h"
#include "OverlayManager.h"

static OverlayDispatcher MainOverlayDispatcher;
static OverlayDispatcher DebugOverlayDispatcher;
static OverlayDispatcher UnusedDispatcher2;
static OverlayDispatcher UnusedDispatcher1;
s32*                     data_0206af24;

static void OvlDisp_Push(OverlayDispatcher* dispatcher, s32 overlayId, OverlayCB callback, void* arg, s32 repeatCount);

/**
 * @brief A dummy callback that does nothing.
 */
static void OvlDisp_DummyCb(void*) {
    return;
}

/**
 * @brief Initializes an OverlayDispatcher instance.
 *
 * @param dispatcher Pointer to the OverlayDispatcher instance to initialize.
 */
static void OvlDisp_Init(OverlayDispatcher* dispatcher) {
    dispatcher->stackDepth = 0;
    OvlDisp_Push(dispatcher, OVERLAY_ID_NONE, OvlDisp_DummyCb, NULL, 0);
}

static void OvlDisp_Push(OverlayDispatcher* dispatcher, s32 overlayId, OverlayCB callback, void* arg, s32 repeatCount) {
    OverlayData* currentData = &dispatcher->stack[dispatcher->stackDepth];
    currentData->tag.id      = overlayId;
    currentData->tag.cb      = callback;
    currentData->cbArg       = arg;
    currentData->repeatCount = repeatCount;

    currentData       = &dispatcher->stack[dispatcher->stackDepth];
    dispatcher->data  = currentData;
    dispatcher->cb    = currentData->tag.cb;
    dispatcher->cbArg = currentData->cbArg;
    dispatcher->stackDepth++;
}

// Nonmatching: Some data movement differences
// Scratch: 48XL1
static void OvlDisp_Pop(OverlayTag* tag, OverlayDispatcher* dispatcher) {
    s32        currentIndex = dispatcher->stackDepth - 1;
    OverlayTag currTag      = dispatcher->stack[currentIndex].tag;

    dispatcher->stackDepth--;
    dispatcher->data  = &dispatcher->stack[currentIndex - 1];
    dispatcher->cb    = dispatcher->stack[currentIndex - 1].tag.cb;
    dispatcher->cbArg = dispatcher->stack[currentIndex - 1].cbArg;

    if (dispatcher->stackDepth == 0) {
        OvlDisp_Push(dispatcher, OVERLAY_ID_NONE, OvlDisp_DummyCb, NULL, 0);
    }

    *tag = currTag;
}

static void OvlDisp_ReplaceTop(OverlayTag* tag, OverlayDispatcher* dispatcher, s32 param_3, OverlayCB cb, void* state,
                               s32 param_6) {
    OverlayTag local;

    OvlDisp_Pop(&local, dispatcher);
    OvlDisp_Push(dispatcher, param_3, cb, state, param_6);
    *tag = local;
}

static BOOL OvlDisp_IsStackAtBase(OverlayDispatcher* dispatcher) {
    BOOL ret = FALSE;
    if (dispatcher->stackDepth <= 1) {
        ret = TRUE;
    }
    return ret;
}

static void* OvlDisp_ReplaceArg(OverlayDispatcher* dispatcher, void* cbArg) {
    void* prevArg           = dispatcher->cbArg;
    dispatcher->data->cbArg = cbArg;
    dispatcher->cbArg       = cbArg;
    return prevArg;
}

static s32 OvlDisp_GetRepeatCount(OverlayDispatcher* dispatcher) {
    return dispatcher->data->repeatCount;
}

static void OvlDisp_SetRepeatCount(OverlayDispatcher* dispatcher, s32 repeatCount) {
    dispatcher->data->repeatCount = repeatCount;
}

static void OvlDisp_IncrementRepeatCount(OverlayDispatcher* dispatcher) {
    dispatcher->data->repeatCount++;
}

static void OvlDisp_Run(OverlayDispatcher* dispatcher) {
    do {
        dispatcher->unk_0C = 0;
        dispatcher->cb(dispatcher->cbArg);
    } while (dispatcher->unk_0C == 1);
}

void MainOvlDisp_Init(void) {
    OvlDisp_Init(&MainOverlayDispatcher);
}

void MainOvlDisp_Push(u32 overlayId, OverlayCB callback, void* cbArg, s32 repeatCount) {
    OvlDisp_Push(&MainOverlayDispatcher, overlayId, callback, cbArg, repeatCount);
}

// Nonmatching: Some data movement differences
// Scratch: rz8Fc
void MainOvlDisp_Pop(OverlayTag* tag) {
    OverlayDispatcher* dispatcher = &MainOverlayDispatcher;

    if (MainOvlDisp_GetRepeatCount() != 0x7FFFFFFF) {
        MainOvlDisp_SetRepeatCount(0x7FFFFFFF);
        MainOvlDisp_Run();
    }

    s32          idx          = dispatcher->stackDepth - 1;
    OverlayData* currentData  = &dispatcher->stack[idx];
    OverlayData* previousData = &dispatcher->stack[idx - 1];
    OverlayTag   currTag      = currentData->tag;

    dispatcher->stackDepth = idx;
    dispatcher->data       = previousData;
    dispatcher->cb         = previousData->tag.cb;
    dispatcher->cbArg      = previousData->cbArg;

    *tag = currTag;
}

void MainOvlDisp_ReplaceTop(OverlayTag* tag, s32 overlayId, void* callback, void* cbArg, s32 repeatCount) {
    OverlayTag local;

    MainOvlDisp_Pop(&local);
    MainOvlDisp_Push(overlayId, callback, cbArg, repeatCount);
    *tag = local;
}

/* Nonmatching: opcode reordering? maybe? */
u32 func_02007240(void) {
    OvlMgr_LoadOverlay(1, *(data_0206af24 + 0xDC));
}

void* MainOvlDisp_SetCbArg(void* arg) {
    OvlDisp_ReplaceArg(&MainOverlayDispatcher, arg);
}

s32 MainOvlDisp_GetRepeatCount(void) {
    return OvlDisp_GetRepeatCount(&MainOverlayDispatcher);
}

void MainOvlDisp_SetRepeatCount(s32 repeatCount) {
    OvlDisp_SetRepeatCount(&MainOverlayDispatcher, repeatCount);
}

void MainOvlDisp_IncrementRepeatCount(void) {
    OvlDisp_IncrementRepeatCount(&MainOverlayDispatcher);
}

void MainOvlDisp_Run(void) {
    OverlayDispatcher* dispatcher = &MainOverlayDispatcher;

    if (OvlMgr_IsOverlayLoaded(dispatcher->data->tag.id) == TRUE) {
        OvlDisp_Run(dispatcher);
    }
}

void DebugOvlDisp_Init(void) {
    OvlDisp_Init(&DebugOverlayDispatcher);
}

void DebugOvlDisp_Push(OverlayCB callback, void* cbArg, s32 repeatCount) {
    OvlDisp_Push(&DebugOverlayDispatcher, 0, callback, cbArg, repeatCount);
}

OverlayCB DebugOvlDisp_Pop(void) {
    OverlayTag tag;

    OvlDisp_Pop(&tag, &DebugOverlayDispatcher);
    return tag.cb;
}

OverlayCB DebugOvlDisp_ReplaceTop(OverlayCB callback, void* cbArg, s32 repeatCount) {
    OverlayTag tag;

    OvlDisp_ReplaceTop(&tag, &DebugOverlayDispatcher, 0, callback, cbArg, repeatCount);
    return tag.cb;
}

BOOL DebugOvlDisp_IsStackAtBase(void) {
    return OvlDisp_IsStackAtBase(&DebugOverlayDispatcher);
}

void DebugOvlDisp_Run(void) {
    OvlDisp_Run(&DebugOverlayDispatcher);
}

void OvlDisp_InitUnused2(void) {
    OvlDisp_Init(&UnusedDispatcher2);
}

void OvlDisp_InitUnused1(void) {
    OvlDisp_Init(&UnusedDispatcher1);
}