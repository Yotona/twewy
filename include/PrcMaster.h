#ifndef PRCMASTER_H
#define PRCMASTER_H

#include <types.h>

typedef struct PrcCtx PrcCtx;

typedef s32 (*PrcStepFn)(PrcCtx*, void*);

typedef enum {
    PRC_STEP_CONTINUE  = 0,
    PRC_STEP_POP_FRAME = 1,
    PRC_STEP_REPEAT    = 2,
} PrcStepResult;

typedef struct {
    /* 0x00 */ void (*enter)(PrcCtx*, void*, s32);
    /* 0x04 */ PrcStepFn* stepTable;
    /* 0x08 */ void (*update)(PrcCtx*, void*);
    /* 0x0C */ void (*render)(PrcCtx*, void*);
    /* 0x10 */ void (*exit)(PrcCtx*, void*);
    /* 0x14 */ u16  stepIndex;
    /* 0x18 */ BOOL entered;
} PrcFrame; // Size: 0x1C

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

typedef struct {
    /* 0x0 */ u16      capacity;
    /* 0x4 */ PrcCtx** contexts;
} PrcMaster;

// void PrcCtx_Init(PrcCtx* ctx, const char* sequence, s32 size);

// void PrcCtx_SetWorkObject(PrcCtx* ctx, void* work);

// void PrcCtx_Destroy(PrcCtx* ctx);

// BOOL PrcCtx_RunSteps(PrcCtx* ctx);

// void PrcCtx_Update(PrcCtx* ctx);

// void PrcCtx_Render(PrcCtx* ctx);

// void PrcCtx_ReplaceStepTable(PrcCtx* ctx, PrcStepFn* stepTable);

// void PrcCtx_ReplaceFrame(PrcCtx* ctx, PrcFrame* newFrame, void* arg2);

// void PrcCtx_ReplaceCurrentUpdateCallback(PrcCtx* ctx, void (*update)(PrcCtx*, void*));

// void PrcCtx_PushStepTable(PrcCtx* ctx, PrcStepFn* stepTable);

// void PrcCtx_AdvanceStep(PrcCtx* ctx);

PrcStepFn* PrcCtx_GetStepTable(PrcCtx* ctx);

void PrcMaster_Init(PrcMaster* master, u16 count);

u16 PrcMaster_RegisterContext(PrcMaster* master, PrcCtx* ctx);

BOOL PrcMaster_RunAllCtxSteps(PrcMaster* master);

void PrcMaster_UpdateAllContexts(PrcMaster* master);

void PrcMaster_RenderAllContexts(PrcMaster* master);

void PrcMaster_UnregisterContextOrPanic(PrcMaster* master, PrcCtx* ctx);

void PrcMaster_UnregisterContext(PrcMaster* master, PrcCtx* ctx);

void PrcMaster_Destroy(PrcMaster* master);

// PrcStepResult PrcStep_StartDelay(PrcCtx* ctx);

// PrcStepResult PrcStep_TickDelay(PrcCtx* ctx);

// PrcStepResult PrcStep_Advance(PrcCtx* ctx);

// PrcStepResult PrcStep_PopFrame(PrcCtx* ctx);

// PrcStepResult PrcStep_Continue(PrcCtx* ctx);

#endif // PRCMASTER_H