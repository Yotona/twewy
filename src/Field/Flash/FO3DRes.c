#include "Engine/Core/Memory.h"
#include "Field/FO3D.h"

typedef struct {
    u8  unk_00[9];
    u8  unk_09;
    u16 unk_0a;
    u16 unk_0c;
    u16 dataOffset;
} FieldResourceHeader;

FieldResourceState* data_ov030_02109d98;

extern void* func_0202bd4c(void* resource);
extern void* func_0202bd5c(void* resource);
extern void* func_0202bd9c(void* resource, s32 index);
extern s32   func_ov030_020cd808(void* resource);

void* func_ov030_020cda74(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DRes_Alloc");
    return data;
}

void func_ov030_020cdaa8(void* data) {
    Mem_Free(&gDebugHeap, data);
}

FieldResourceState* func_ov030_020cdac0(void) {
    data_ov030_02109d98            = func_ov030_020cda74(0x20);
    data_ov030_02109d98->modelFile = NULL;
    data_ov030_02109d98->anmFile   = NULL;
    data_ov030_02109d98->resAnm    = NULL;
    data_ov030_02109d98->resMdl    = NULL;
    data_ov030_02109d98->resTex    = NULL;
    data_ov030_02109d98->unk_18    = 0;
    data_ov030_02109d98->name      = NULL;
    return data_ov030_02109d98;
}

void func_ov030_020cdb18(void) {
    func_ov030_020d21d4(data_ov030_02109d98->modelFile);
    func_ov030_020d21d4(data_ov030_02109d98->anmFile);
    func_ov030_020cdaa8(data_ov030_02109d98);
    data_ov030_02109d98 = NULL;
}

void func_ov030_020cdb5c(FieldResourceState* state) {
    data_ov030_02109d98 = state;
}

FieldResourceState* func_ov030_020cdb6c(void) {
    return data_ov030_02109d98;
}

u8 func_ov030_020cdb7c(void* resource) {
    u8 result = 0;

    if (resource != NULL) {
        FieldResourceHeader* header = func_0202bd4c(resource);
        if (header != NULL) {
            result = header->unk_09;
        }
    }
    return result;
}

s32 func_ov030_020cdba0(void* resource) {
    return func_ov030_020cd808(resource);
}

void func_ov030_020cdbac(void* resource, s32 index) {
    FieldResourceHeader* header;
    u8*                  table;

    if (data_ov030_02109d98 != NULL) {
        data_ov030_02109d98->modelFile = resource;
        header                         = func_0202bd4c(resource);
        table                          = (u8*)header + 8 + header->dataOffset;
        data_ov030_02109d98->resMdl    = (NNSG3dResMdl*)((u8*)header + *(u32*)(table + *(u16*)table * index + 4));
        data_ov030_02109d98->resTex    = func_0202bd5c(resource);
        header                         = func_0202bd4c(resource);
        table                          = (u8*)header + 8 + header->dataOffset;
        data_ov030_02109d98->name      = (const char*)(table + *(u16*)(table + 2) + index * 16);
    }
}

void func_ov030_020cdc40(void* resource) {
    if (data_ov030_02109d98 != NULL) {
        data_ov030_02109d98->modelFile = resource;
        data_ov030_02109d98->resTex    = func_0202bd5c(resource);
    }
}

void func_ov030_020cdc70(void* resource, s32 index) {
    if (data_ov030_02109d98 != NULL) {
        data_ov030_02109d98->anmFile = resource;
        data_ov030_02109d98->resAnm  = func_0202bd9c(resource, index);
    }
}

void* func_ov030_020cdca0(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DBase_Alloc");
    return data;
}

void func_ov030_020cdcd4(void* data) {
    Mem_Free(&gDebugHeap, data);
}
