#include "Engine/Core/Memory.h"
#include "Field/FO3D.h"

FieldViewerState* data_ov030_02109ddc;

void* func_ov030_020d0f18(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DViewerImd_Alloc");
    return data;
}

void func_ov030_020d0f4c(void* data) {
    Mem_Free(&gDebugHeap, data);
}

FieldViewerState* func_ov030_020d0f64(void) {
    data_ov030_02109ddc                        = func_ov030_020d0f18(sizeof(FieldViewerState));
    data_ov030_02109ddc->scene                 = 0;
    data_ov030_02109ddc->resources             = NULL;
    data_ov030_02109ddc->models                = NULL;
    data_ov030_02109ddc->modelCount            = 0;
    data_ov030_02109ddc->resourceCount         = 0;
    data_ov030_02109ddc->unk_1C                = 0;
    data_ov030_02109ddc->pos.y                 = data_ov030_02109ddc->unk_1C;
    data_ov030_02109ddc->pos.x                 = data_ov030_02109ddc->pos.y;
    data_ov030_02109ddc->unk_20                = 0;
    data_ov030_02109ddc->extraCount            = 0;
    data_ov030_02109ddc->extraResources[1]     = NULL;
    data_ov030_02109ddc->extraResources[0]     = data_ov030_02109ddc->extraResources[1];
    data_ov030_02109ddc->extraModels[1]        = NULL;
    data_ov030_02109ddc->extraModels[0]        = data_ov030_02109ddc->extraModels[1];
    data_ov030_02109ddc->extraModelCount[1]    = 0;
    data_ov030_02109ddc->extraModelCount[0]    = data_ov030_02109ddc->extraModelCount[1];
    data_ov030_02109ddc->extraResourceCount[1] = 0;
    data_ov030_02109ddc->extraResourceCount[0] = data_ov030_02109ddc->extraResourceCount[1];
    data_ov030_02109ddc->extraModelFiles[1]    = NULL;
    data_ov030_02109ddc->extraModelFiles[0]    = data_ov030_02109ddc->extraModelFiles[1];
    data_ov030_02109ddc->modelFile             = NULL;
    func_ov030_020d2558(0x40);
    data_ov030_02109ddc->ptrList = func_ov030_020d2548();
    return data_ov030_02109ddc;
}

void func_ov030_020d1060(void) {
    func_ov030_020d2538(data_ov030_02109ddc->ptrList);
    func_ov030_020d2504();
    func_ov030_020d0f4c(data_ov030_02109ddc);
    data_ov030_02109ddc = NULL;
}
