#include "Otosu/OtosuGame.h"

void OtuStageDispatch_Init(OtuStageDispatch* dispatch, Heap* heap) {
    dispatch->heap       = heap;
    dispatch->stage      = NULL;
    dispatch->next       = NULL;
    dispatch->pending    = FALSE;
    dispatch->stageIndex = 0;
    dispatch->stageBlock = NULL;
}

void OtuStageDispatch_SetNext(OtuStageDispatch* dispatch, const OtuSceneStage* next) {
    dispatch->next    = next;
    dispatch->pending = TRUE;
}

void OtuStageDispatch_NextStep(OtuStageDispatch* dispatch) {
    dispatch->stageIndex = dispatch->stageIndex + 1;
}

void OtuStageDispatch_Enter(OtuStageDispatch* dispatch, void* scene) {
    const OtuSceneStage* stage;
    OtuStageHandler      enter;

    if (dispatch->pending == FALSE) {
        return;
    }

    dispatch->stage      = dispatch->next;
    dispatch->stageIndex = 0;
    dispatch->next       = NULL;
    dispatch->pending    = FALSE;

    stage = dispatch->stage;
    if (stage->blockSize != 0) {
        dispatch->stageBlock = Mem_AllocHeapTail(dispatch->heap, stage->blockSize);
    }

    stage = dispatch->stage;
    if (stage != NULL && (enter = stage->enter) != NULL) {
        enter(scene);
    }
}

void OtuStageDispatch_Run(OtuStageDispatch* dispatch, void* scene) {
    const OtuSceneStage* stage = dispatch->stage;

    if (stage == NULL) {
        return;
    }

    stage->steps[dispatch->stageIndex](scene);
}

void OtuStageDispatch_Exit(OtuStageDispatch* dispatch, void* scene) {
    const OtuSceneStage* stage;
    void*                block;
    OtuStageHandler      exit;

    if (dispatch->pending == FALSE) {
        return;
    }

    stage = dispatch->stage;
    if (stage != NULL && (exit = stage->exit) != NULL) {
        exit(scene);
    }

    block = dispatch->stageBlock;
    if (block == NULL) {
        return;
    }

    Mem_Free(dispatch->heap, block);
    dispatch->stageBlock = NULL;
}

s32 OtuStageDispatch_IsFinished(OtuStageDispatch* dispatch) {
    if (dispatch->next == NULL && dispatch->pending != FALSE) {
        return 1;
    }
    return 0;
}

const OtuSceneStage* OtuStageDispatch_GetStage(OtuStageDispatch* dispatch) {
    return dispatch->stage;
}

void* OtuStageDispatch_GetBlock(OtuStageDispatch* dispatch) {
    return dispatch->stageBlock;
}
