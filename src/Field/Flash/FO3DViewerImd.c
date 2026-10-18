#include "Field/Flash/FO3DViewerImd.h"
#include "Engine/Core/Memory.h"
#include "Field/Flash/FO3D.h"
#include "Field/Flash/FOPtrList.h"

FieldViewerState* gFO3DViewer;

void* FO3DViewerImd_Alloc(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DViewerImd_Alloc");
    return data;
}

void FO3DViewerImd_Free(void* data) {
    Mem_Free(&gDebugHeap, data);
}

FieldViewerState* FO3DViewerImd_Create(void) {
    gFO3DViewer                        = FO3DViewerImd_Alloc(sizeof(FieldViewerState));
    gFO3DViewer->scene                 = 0;
    gFO3DViewer->resources             = NULL;
    gFO3DViewer->models                = NULL;
    gFO3DViewer->modelCount            = 0;
    gFO3DViewer->resourceCount         = 0;
    gFO3DViewer->unk_1C                = 0;
    gFO3DViewer->pos.y                 = gFO3DViewer->unk_1C;
    gFO3DViewer->pos.x                 = gFO3DViewer->pos.y;
    gFO3DViewer->unk_20                = 0;
    gFO3DViewer->extraCount            = 0;
    gFO3DViewer->extraResources[1]     = NULL;
    gFO3DViewer->extraResources[0]     = gFO3DViewer->extraResources[1];
    gFO3DViewer->extraModels[1]        = NULL;
    gFO3DViewer->extraModels[0]        = gFO3DViewer->extraModels[1];
    gFO3DViewer->extraModelCount[1]    = 0;
    gFO3DViewer->extraModelCount[0]    = gFO3DViewer->extraModelCount[1];
    gFO3DViewer->extraResourceCount[1] = 0;
    gFO3DViewer->extraResourceCount[0] = gFO3DViewer->extraResourceCount[1];
    gFO3DViewer->extraModelFiles[1]    = NULL;
    gFO3DViewer->extraModelFiles[0]    = gFO3DViewer->extraModelFiles[1];
    gFO3DViewer->modelFile             = NULL;
    FOPtrList_Init(0x40);
    gFO3DViewer->ptrList = FOPtrList_GetCurrent();
    return gFO3DViewer;
}

void FO3DViewerImd_Destroy(void) {
    FOPtrList_SetCurrent(gFO3DViewer->ptrList);
    FOPtrList_Destroy();
    FO3DViewerImd_Free(gFO3DViewer);
    gFO3DViewer = NULL;
}
