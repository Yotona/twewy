#ifndef PRCMASTER_H
#define PRCMASTER_H

#include <types.h>

/** @brief Process context state object. */
typedef struct PrcCtx PrcCtx;

/** @brief Return value that controls process-step execution flow. */
typedef enum {
    /** @brief Keep running normally; continue next frame/update cycle. */
    PRC_STEP_CONTINUE = 0,
    /** @brief Pop the current frame from the context stack. */
    PRC_STEP_POP_FRAME = 1,
    /** @brief Run another step immediately in the same update. */
    PRC_STEP_REPEAT = 2,
} PrcStepResult;

/** @brief Per-step callback signature. */
typedef PrcStepResult (*PrcStepFn)(PrcCtx*, void*);

/** @brief A stack frame in a process context. */
typedef struct {
    /** @brief Called when the frame is entered. */
    /* 0x00 */ void (*enter)();
    /** @brief Table of step functions for this frame. */
    /* 0x04 */ PrcStepFn* stepTable;
    /** @brief Per-frame update callback. */
    /* 0x08 */ void (*update)();
    /** @brief Per-frame render callback. */
    /* 0x0C */ void (*render)();
    /** @brief Called when the frame is exited. */
    /* 0x10 */ void (*exit)();
    /** @brief Current index in #stepTable. */
    /* 0x14 */ u16 stepIndex;
    /** @brief TRUE after enter callback has run and before exit callback runs. */
    /* 0x18 */ BOOL entered;
} PrcFrame; // Size: 0x1C

/**
 * @brief Runtime state for one process context.
 *
 * Owns a stack of up to 15 frames and optional work memory used by callbacks.
 */
struct PrcCtx {
    /* 0x000 */ void*       work;
    /* 0x004 */ PrcFrame    frames[15];
    /* 0x1A8 */ u16         currFrameIndex; // Active frame stack index, -1 if inactive
    /* 0x1AA */ u16         lastObservedFrameIndex;
    /* 0x1AC */ u16         lastObservedStepIndex;
    /* 0x1AE */ u16         _padding_1AE;
    /* 0x1B0 */ const char* sequence;
    /* 0x1B4 */ BOOL        stepTblChangedThisUpdate;
    /* 0x1B8 */ s32         unk_1B8;
    /* 0x1BC */ s32         unk_1BC;
    /* 0x1C0 */ char        unk_1C0[0x1C8 - 0x1C0];
    /* 0x1C8 */ u16         unk_1C8;
    /* 0x1CA */ u16         unk_1CA;
    /* 0x1CC */ u16         unk_1CC;
    /* 0x1CE */ u16         unk_1CE;
    /* 0x1D0 */ s32         unk_1D0;
    /* 0x1D4 */ s32         unk_1D4;
    /* 0x1D8 */ s32         unk_1D8;
    /* 0x1DC */ s32         unk_1DC;
    /* 0x1E0 */ u16         delayCounter;
    /* 0x1E2 */ char        _padding_1E2[0x6];
};

/** @brief Container that manages multiple active process contexts. */
typedef struct {
    /* 0x0 */ u16      capacity;
    /* 0x4 */ PrcCtx** contexts;
} PrcMaster;

/**
 * @brief Initialize a process context and allocate zeroed work memory.
 * @param ctx Context to initialize.
 * @param sequence Memory-sequence tag used for tracking allocations.
 * @param size Work-object allocation size in bytes.
 */
void PrcCtx_Init(PrcCtx* ctx, const char* sequence, s32 size);

/**
 * @brief Replace the context work object.
 * @param ctx Context to modify.
 * @param work New work object pointer.
 *
 * If the context currently owns an allocated work object, it is freed first.
 */
void PrcCtx_SetWorkObject(PrcCtx* ctx, void* work);

/**
 * @brief Destroy a context and release owned work memory.
 * @param ctx Context to destroy.
 */
void PrcCtx_Destroy(PrcCtx* ctx);

/**
 * @brief Execute process steps for the current frame until not repeating.
 * @param ctx Context to run.
 * @return TRUE while the context should remain active; FALSE when complete.
 */
BOOL PrcCtx_RunSteps(PrcCtx* ctx);

/**
 * @brief Run the current frame update callback.
 * @param ctx Context to update.
 */
void PrcCtx_Update(PrcCtx* ctx);

/**
 * @brief Run the current frame render callback.
 * @param ctx Context to render.
 */
void PrcCtx_Render(PrcCtx* ctx);

/**
 * @brief Replace the current frame step table.
 * @param ctx Context to modify.
 * @param stepTable New step table.
 */
void PrcCtx_ReplaceStepTable(PrcCtx* ctx, PrcStepFn* stepTable);

/**
 * @brief Replace the current frame callbacks and step table.
 * @param ctx Context to modify.
 * @param newFrame Source frame definition.
 * @param enterArg Argument passed to the enter callback.
 */
void PrcCtx_ReplaceFrame(PrcCtx* ctx, PrcFrame* newFrame, void* enterArg);

/**
 * @brief Replace only the current frame update callback.
 * @param ctx Context to modify.
 * @param update New update callback.
 */
void PrcCtx_ReplaceCurrentUpdateCallback(PrcCtx* ctx, void (*update)(PrcCtx*, void*));

/**
 * @brief Push a new frame that uses the given step table.
 * @param ctx Context to modify.
 * @param stepTable Step table for the pushed frame.
 */
void PrcCtx_PushStepTable(PrcCtx* ctx, PrcStepFn* stepTable);

/**
 * @brief Advance to the next step in the current frame.
 * @param ctx Context to modify.
 */
void PrcCtx_AdvanceStep(PrcCtx* ctx);

/**
 * @brief Get the current frame's step table.
 * @param ctx Context to query.
 * @return Pointer to current step table.
 */
PrcStepFn* PrcCtx_GetStepTable(PrcCtx* ctx);

/**
 * @brief Initialize a process-context manager.
 * @param master Manager instance to initialize.
 * @param count Maximum number of context slots.
 */
void PrcMaster_Init(PrcMaster* master, u16 count);

/**
 * @brief Register a context into the first free slot.
 * @param master Manager that owns the slot array.
 * @param ctx Context to register.
 * @return Slot index on success, or 0xFFFF if full.
 */
u16 PrcMaster_RegisterContext(PrcMaster* master, PrcCtx* ctx);

/**
 * @brief Run step execution on all registered contexts.
 * @param master Manager to process.
 * @return TRUE if at least one context remains active after running.
 */
BOOL PrcMaster_RunAllCtxSteps(PrcMaster* master);

/**
 * @brief Run update callbacks on all registered contexts.
 * @param master Manager to process.
 */
void PrcMaster_UpdateAllContexts(PrcMaster* master);

/**
 * @brief Run render callbacks on all registered contexts.
 * @param master Manager to process.
 */
void PrcMaster_RenderAllContexts(PrcMaster* master);

/**
 * @brief Unregister a context or halt if it was not found.
 * @param master Manager to modify.
 * @param ctx Context to remove.
 */
void PrcMaster_UnregisterContextOrPanic(PrcMaster* master, PrcCtx* ctx);

/**
 * @brief Unregister a context if found.
 * @param master Manager to modify.
 * @param ctx Context to remove.
 */
void PrcMaster_UnregisterContext(PrcMaster* master, PrcCtx* ctx);

/**
 * @brief Destroy manager and all remaining registered contexts.
 * @param master Manager to destroy.
 */
void PrcMaster_Destroy(PrcMaster* master);

/**
 * @brief Step helper: initialize delay counter and advance immediately.
 * @param ctx Context to modify.
 * @return Always PRC_STEP_REPEAT.
 */
PrcStepResult PrcStep_StartDelay(PrcCtx* ctx, void*);

/**
 * @brief Step helper: tick delay counter until complete.
 * @param ctx Context to modify.
 * @return PRC_STEP_CONTINUE while delaying, else PRC_STEP_REPEAT after advancing.
 */
PrcStepResult PrcStep_TickDelay(PrcCtx* ctx, void*);

/**
 * @brief Step helper: advance to next step.
 * @param ctx Context to modify.
 * @return Always PRC_STEP_CONTINUE.
 */
PrcStepResult PrcStep_Advance(PrcCtx* ctx, void*);

/**
 * @brief Step helper: request popping the current frame.
 * @param ctx Context (unused by behavior).
 * @return Always PRC_STEP_POP_FRAME.
 */
PrcStepResult PrcStep_PopFrame(PrcCtx* ctx, void*);

/**
 * @brief Step helper: continue without side effects.
 * @param ctx Context (unused by behavior).
 * @return Always PRC_STEP_CONTINUE.
 */
PrcStepResult PrcStep_Continue(PrcCtx* ctx, void*);

#endif // PRCMASTER_H