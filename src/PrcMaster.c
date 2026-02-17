#include "PrcMaster.h"
#include "Memory.h"
#include "System.h"

static void PrcCtx_Reset(PrcCtx* arg0, const char* sequence, void* work) {
    arg0->sequence = sequence;

    arg0->currFrameIndex         = -1;
    arg0->lastObservedFrameIndex = -1;
    arg0->lastObservedStepIndex  = -1;

    arg0->stepTblChangedThisUpdate = FALSE;

    arg0->work = work;

    arg0->unk_1BC = 0;
    arg0->unk_1C8 = 0;
    arg0->unk_1CA = 0;
    arg0->unk_1CC = 0;
    arg0->unk_1CE = 0;
    arg0->unk_1D0 = 0;
    arg0->unk_1D4 = 0;
    arg0->unk_1D8 = 0;
    arg0->unk_1DC = 0xFFFF;

    for (u16 i = 0; i < 15; i++) {
        arg0->frames[i].enter     = NULL;
        arg0->frames[i].stepTable = NULL;
        arg0->frames[i].update    = NULL;
        arg0->frames[i].render    = NULL;
        arg0->frames[i].exit      = NULL;
        arg0->frames[i].stepIndex = 0;
        arg0->frames[i].entered   = FALSE;
    }

    arg0->unk_1B8 = 0;
}

void PrcCtx_Init(PrcCtx* ctx, const char* sequence, s32 size) {
    PrcCtx_Reset(ctx, sequence, NULL);

    void* work = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, work, sequence);
    ctx->work    = work;
    ctx->unk_1B8 = 1;
    MI_CpuFillU32(0, ctx->work, size);
}

void PrcCtx_SetWorkObject(PrcCtx* ctx, void* work) {
    if (ctx->work != NULL) {
        Mem_Free(&gMainHeap, ctx->work);
    }
    ctx->work    = work;
    ctx->unk_1B8 = 0;
}

static void PrcCtx_CallEnter(PrcCtx* ctx, PrcFrame* frame, s32 arg2) {
    if (frame != NULL && frame->enter != NULL) {
        frame->entered = TRUE;
        frame->enter(ctx, ctx->work, arg2);
    }
}

static void PrcCtx_CallExit(PrcCtx* ctx, PrcFrame* frame) {
    if (frame != NULL && frame->entered && frame->exit != NULL) {
        frame->exit(ctx, ctx->work);
        frame->entered = FALSE;
    }
}

void PrcCtx_Destroy(PrcCtx* ctx) {
    PrcFrame* frame = &ctx->frames[ctx->currFrameIndex];

    PrcCtx_CallExit(ctx, frame);
    if (ctx->unk_1B8 != 0) {
        Mem_Free(&gMainHeap, ctx->work);
    }
    ctx->work = NULL;
}

BOOL PrcCtx_RunSteps(PrcCtx* ctx) {
    BOOL keepRunning = TRUE;

    if ((SysControl.buttonState.currButtons & INPUT_BUTTON_DEBUG) && !(SysControl.buttonState.pressedButtons & INPUT_BUTTON_L))
    {
        return TRUE;
    }
    if (ctx->currFrameIndex == 0xFFFF) {
        return FALSE;
    }

    PrcStepResult stepAction = PRC_STEP_CONTINUE;
    while (TRUE) {
        PrcFrame* frame = &ctx->frames[ctx->currFrameIndex];
        if ((ctx->lastObservedFrameIndex != ctx->currFrameIndex) || (ctx->lastObservedStepIndex != frame->stepIndex)) {
            ctx->lastObservedFrameIndex = ctx->currFrameIndex;
            ctx->lastObservedStepIndex  = frame->stepIndex;
        }
        ctx->stepTblChangedThisUpdate = FALSE;
        stepAction                    = frame->stepTable[frame->stepIndex](ctx, ctx->work);
        if (stepAction != PRC_STEP_REPEAT) {
            break;
        }
    }

    if (stepAction == PRC_STEP_POP_FRAME) {
        PrcCtx_CallExit(ctx, &ctx->frames[ctx->currFrameIndex]);
        if (ctx->currFrameIndex != 0) {
            ctx->currFrameIndex--;
        } else {
            keepRunning = FALSE;
        }
    }
    return keepRunning;
}

void PrcCtx_Update(PrcCtx* ctx) {
    if (!(SysControl.buttonState.currButtons & INPUT_BUTTON_DEBUG) || (SysControl.buttonState.pressedButtons & INPUT_BUTTON_L))
    {
        if (ctx->currFrameIndex != 0xFFFF && ctx->frames[ctx->currFrameIndex].update != NULL) {
            ctx->frames[ctx->currFrameIndex].update(ctx, ctx->work);
        }
    }
}

void PrcCtx_Render(PrcCtx* ctx) {
    if (!(SysControl.buttonState.currButtons & INPUT_BUTTON_DEBUG) || (SysControl.buttonState.pressedButtons & INPUT_BUTTON_L))
    {
        if (ctx->currFrameIndex != 0xFFFF && ctx->frames[ctx->currFrameIndex].render != NULL) {
            ctx->frames[ctx->currFrameIndex].render(ctx, ctx->work);
        }
    }
}

void PrcCtx_ReplaceStepTable(PrcCtx* ctx, PrcStepFn* stepTable) {
    if (ctx->currFrameIndex == 0xFFFF) {
        ctx->currFrameIndex = 0;
    }
    ctx->stepTblChangedThisUpdate = TRUE;

    PrcFrame* frame = &ctx->frames[ctx->currFrameIndex];

    PrcCtx_CallExit(ctx, frame);
    frame->enter     = NULL;
    frame->stepTable = stepTable;
    frame->update    = NULL;
    frame->render    = NULL;
    frame->exit      = NULL;
    frame->stepIndex = 0;
}

void PrcCtx_ReplaceFrame(PrcCtx* ctx, PrcFrame* newFrame, void* arg2) {
    if (ctx->currFrameIndex == 0xFFFF) {
        ctx->currFrameIndex = 0;
    }

    PrcFrame* currFrame = &ctx->frames[ctx->currFrameIndex];

    PrcCtx_CallExit(ctx, currFrame);
    PrcCtx_ReplaceStepTable(ctx, newFrame->stepTable);

    currFrame->enter  = newFrame->enter;
    currFrame->update = newFrame->update;
    currFrame->render = newFrame->render;
    currFrame->exit   = newFrame->exit;

    PrcCtx_CallEnter(ctx, currFrame, arg2);
}

void PrcCtx_ReplaceCurrentUpdateCallback(PrcCtx* ctx, void (*update)(PrcCtx*, void*)) {
    if (ctx->currFrameIndex == 0xFFFF) {
        ctx->currFrameIndex = 0;
    }
    ctx->frames[ctx->currFrameIndex].update = update;
}

void PrcCtx_PushStepTable(PrcCtx* ctx, PrcStepFn* stepTable) {
    ctx->currFrameIndex++;
    if (ctx->currFrameIndex >= 15) {
        OS_WaitForever();
    }

    ctx->stepTblChangedThisUpdate = TRUE;

    PrcFrame* frame  = &ctx->frames[ctx->currFrameIndex];
    frame->enter     = NULL;
    frame->stepTable = stepTable;
    frame->exit      = NULL;
    frame->stepIndex = 0;
}

void PrcCtx_AdvanceStep(PrcCtx* ctx) {
    PrcFrame* frame = &ctx->frames[ctx->currFrameIndex];
    if (ctx->stepTblChangedThisUpdate) {
        OS_WaitForever();
    }
    frame->stepIndex++;
}

PrcStepFn* PrcCtx_GetStepTable(PrcCtx* ctx) {
    return ctx->frames[ctx->currFrameIndex].stepTable;
}

void PrcMaster_Init(PrcMaster* prc, u16 count) {
    prc->capacity = count;

    void* data = Mem_AllocHeapTail(&gMainHeap, prc->capacity * 4);
    Mem_SetSequence(&gMainHeap, data, "PrcMaster");
    prc->contexts = data;
    MI_CpuFill(0, data, prc->capacity * 4);
}

u16 PrcMaster_RegisterContext(PrcMaster* master, PrcCtx* ctx) {
    u16 i;
    u16 result = 0xFFFF;

    for (i = 0; i < master->capacity; i++) {
        if (master->contexts[i] == NULL) {
            master->contexts[i] = ctx;
            result              = i;
            break;
        }
    }
    return result;
}

BOOL PrcMaster_RunAllCtxSteps(PrcMaster* master) {
    u16  i;
    BOOL keepRunning = FALSE;

    for (i = 0; i < master->capacity; i++) {
        if (master->contexts[i] != NULL) {
            if (PrcCtx_RunSteps(master->contexts[i]) == FALSE) {
                master->contexts[i] = NULL;
            } else {
                keepRunning = TRUE;
            }
        }
    }

    return keepRunning;
}

void PrcMaster_UpdateAllContexts(PrcMaster* master) {
    for (u16 i = 0; i < master->capacity; i++) {
        if (master->contexts[i] != NULL) {
            PrcCtx_Update(master->contexts[i]);
        }
    }
}

void PrcMaster_RenderAllContexts(PrcMaster* master) {
    for (u16 i = 0; i < master->capacity; i++) {
        if (master->contexts[i] != NULL) {
            PrcCtx_Render(master->contexts[i]);
        }
    }
}

void PrcMaster_UnregisterContextOrPanic(PrcMaster* master, PrcCtx* ctx) {
    for (u16 i = 0; i < master->capacity; i++) {
        if (master->contexts[i] == ctx) {
            PrcCtx_Destroy(master->contexts[i]);
            master->contexts[i] = NULL;
            return;
        }
    }
    OS_WaitForever();
}

void PrcMaster_UnregisterContext(PrcMaster* master, PrcCtx* ctx) {
    for (u16 i = 0; i < master->capacity; i++) {
        if (master->contexts[i] == ctx) {
            PrcCtx_Destroy(master->contexts[i]);
            master->contexts[i] = NULL;
            return;
        }
    }
}

void PrcMaster_Destroy(PrcMaster* master) {
    for (u16 i = 0; i < master->capacity; i++) {
        if (master->contexts[i] != NULL) {
            PrcCtx_Destroy(master->contexts[i]);
            master->contexts[i] = NULL;
        }
    }
    Mem_Free(&gMainHeap, master->contexts);
    master->contexts = NULL;
}

PrcStepResult PrcStep_StartDelay(PrcCtx* ctx) {
    ctx->delayCounter = 10;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

PrcStepResult PrcStep_TickDelay(PrcCtx* ctx) {
    if (ctx->delayCounter != 0) {
        ctx->delayCounter--;
    } else {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_REPEAT;
    }
    return PRC_STEP_CONTINUE;
}

PrcStepResult PrcStep_Advance(PrcCtx* ctx) {
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_CONTINUE;
}

PrcStepResult PrcStep_PopFrame(PrcCtx* ctx) {
    return PRC_STEP_POP_FRAME;
}

PrcStepResult PrcStep_Continue(PrcCtx* ctx) {
    return PRC_STEP_CONTINUE;
}
