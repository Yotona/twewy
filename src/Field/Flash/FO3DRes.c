#include "Engine/Core/Memory.h"
#include "Field/FO3D.h"

typedef struct {
    u8  header[9];
    u8  numEntry;
    u16 sizeDictBlk;
    u16 dummy;
    u16 dataOffset;
} FieldResourceHeader;

FieldResourceState* gFO3DRes;

extern void* func_0202bd4c(void* resource);
extern void* func_0202bd5c(void* resource);
extern void* func_0202bd9c(void* resource, s32 index);
extern s32   FO3D_LoadToVram(void* resource);

void* FO3DRes_Alloc(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DRes_Alloc");
    return data;
}

void FO3DRes_Free(void* data) {
    Mem_Free(&gDebugHeap, data);
}

FieldResourceState* FO3DRes_Create(void) {
    gFO3DRes            = FO3DRes_Alloc(0x20);
    gFO3DRes->modelFile = NULL;
    gFO3DRes->anmFile   = NULL;
    gFO3DRes->resAnm    = NULL;
    gFO3DRes->resMdl    = NULL;
    gFO3DRes->resTex    = NULL;
    gFO3DRes->unk_18    = 0;
    gFO3DRes->name      = NULL;
    return gFO3DRes;
}

void FO3DRes_Destroy(void) {
    FOFileIO_Release(gFO3DRes->modelFile);
    FOFileIO_Release(gFO3DRes->anmFile);
    FO3DRes_Free(gFO3DRes);
    gFO3DRes = NULL;
}

void FO3DRes_SetCurrent(FieldResourceState* state) {
    gFO3DRes = state;
}

FieldResourceState* FO3DRes_GetCurrent(void) {
    return gFO3DRes;
}

u8 FO3DRes_GetModelCount(void* resource) {
    u8 result = 0;

    if (resource != NULL) {
        FieldResourceHeader* header = func_0202bd4c(resource);
        if (header != NULL) {
            result = header->numEntry;
        }
    }
    return result;
}

s32 FO3DRes_LoadToVram(void* resource) {
    return FO3D_LoadToVram(resource);
}

void FO3DRes_SetModel(void* resource, s32 index) {
    FieldResourceHeader* header;
    u8*                  table;

    if (gFO3DRes != NULL) {
        gFO3DRes->modelFile = resource;
        header              = func_0202bd4c(resource);
        table               = (u8*)header + 8 + header->dataOffset;
        gFO3DRes->resMdl    = (NNSG3dResMdl*)((u8*)header + *(u32*)(table + *(u16*)table * index + 4));
        gFO3DRes->resTex    = func_0202bd5c(resource);
        header              = func_0202bd4c(resource);
        table               = (u8*)header + 8 + header->dataOffset;
        gFO3DRes->name      = (const char*)(table + *(u16*)(table + 2) + index * 16);
    }
}

void FO3DRes_SetTexture(void* resource) {
    if (gFO3DRes != NULL) {
        gFO3DRes->modelFile = resource;
        gFO3DRes->resTex    = func_0202bd5c(resource);
    }
}

void FO3DRes_SetAnim(void* resource, s32 index) {
    if (gFO3DRes != NULL) {
        gFO3DRes->anmFile = resource;
        gFO3DRes->resAnm  = func_0202bd9c(resource, index);
    }
}

void* FO3DBase_Alloc(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DBase_Alloc");
    return data;
}

void FO3DBase_Free(void* data) {
    Mem_Free(&gDebugHeap, data);
}
