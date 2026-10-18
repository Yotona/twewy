#include "Field/Flash/FO3DObj.h"
#include "Engine/Core/Memory.h"
#include "Field/Flash/FO3D.h"
#include "Field/Flash/FO3DRes.h"
#include <nitro/fx.h>

Field3DObject* gFO3DObj;

void func_ov030_020cde0c(s32 value);
void func_ov030_020cde40(s32 value);

void FO3DObj_SetCurrent(Field3DObject* state) {
    gFO3DObj = state;
}

Field3DObject* FO3DObj_Create(void) {
    gFO3DObj = FO3DBase_Alloc(0x5C);

    gFO3DObj->unk_0C = 0;
    gFO3DObj->unk_10 = 0;
    gFO3DObj->unk_14 = 0;
    gFO3DObj->unk_18 = 0;
    gFO3DObj->unk_1C = 0;
    gFO3DObj->unk_50 = 0;
    FO3DObj_SetPos(0, 0, 0);
    FO3DObj_SetRotY(0);
    func_ov030_020cde0c(1);
    func_ov030_020cde40(1);
    return gFO3DObj;
}

void FO3DObj_Destroy(void) {
    FO3DBase_Free(gFO3DObj);
    gFO3DObj = NULL;
}

void FO3DObj_SetRotY(s16 rotY) {
    s32 idx = (rotY >> 4) * 2;
    Mtx_YRot33(&gFO3DObj->rotMtx, FX_SinCosTable_[idx], FX_SinCosTable_[idx + 1]);
    gFO3DObj->rotY = rotY;
}

void FO3DObj_SetPos(s32 x, s32 y, s32 z) {
    gFO3DObj->x = x;
    gFO3DObj->y = y;
    gFO3DObj->z = z;
}

void func_ov030_020cde0c(s32 value) {
    gFO3DObj->unk_48 = value;
}

void func_ov030_020cde20(s32 value) {
    gFO3DObj->unk_4C = value;
    gFO3DObj->unk_50 = 1;
}

void func_ov030_020cde40(s32 value) {
    gFO3DObj->unk_58 = value;
}

void* FO3DObj_Alloc(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DObj_Alloc");
    return data;
}

void FO3DObj_Free(void* data) {
    Mem_Free(&gDebugHeap, data);
}
