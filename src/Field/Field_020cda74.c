#include "Engine/Core/Memory.h"

typedef struct {
    void* unk_00;
    void* unk_04;
    u32   unk_08;
    u32   unk_0c;
    void* unk_10;
    void* unk_14;
    u32   unk_18;
    void* unk_1c;
} FieldResourceState;

typedef struct {
    u8  unk_00[9];
    u8  unk_09;
    u16 unk_0a;
    u16 unk_0c;
    u16 dataOffset;
} FieldResourceHeader;

extern FieldResourceState* data_ov030_02109d98;
extern void                func_ov030_020d21d4(void* resource);
extern void*               func_0202bd4c(void* resource);
extern void*               func_0202bd5c(void* resource);
extern void*               func_0202bd9c(void* resource, s32 index);
extern s32                 func_ov030_020cd808(void* resource);

char data_ov030_021089dc[16] = "FO3DRes_Alloc";
char data_ov030_021089ec[16] = "FO3DBase_Alloc";

void* func_ov030_020cda74(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, data_ov030_021089dc);
    return data;
}

void func_ov030_020cdaa8(void* data) {
    Mem_Free(&gDebugHeap, data);
}

void* func_ov030_020cdac0(void) {
    data_ov030_02109d98         = func_ov030_020cda74(0x20);
    data_ov030_02109d98->unk_00 = NULL;
    data_ov030_02109d98->unk_04 = NULL;
    data_ov030_02109d98->unk_08 = 0;
    data_ov030_02109d98->unk_10 = 0;
    data_ov030_02109d98->unk_14 = 0;
    data_ov030_02109d98->unk_18 = 0;
    data_ov030_02109d98->unk_1c = 0;
    return data_ov030_02109d98;
}

void func_ov030_020cdb18(void) {
    func_ov030_020d21d4(data_ov030_02109d98->unk_00);
    func_ov030_020d21d4(data_ov030_02109d98->unk_04);
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

    if (data_ov030_02109d98 == NULL) {
        return;
    }
    data_ov030_02109d98->unk_00 = resource;
    header                      = func_0202bd4c(resource);
    table                       = (u8*)header + 8 + header->dataOffset;
    data_ov030_02109d98->unk_10 = (u8*)header + *(u32*)(table + *(u16*)table * index + 4);
    data_ov030_02109d98->unk_14 = func_0202bd5c(resource);
    header                      = func_0202bd4c(resource);
    table                       = (u8*)header + 8 + header->dataOffset;
    data_ov030_02109d98->unk_1c = table + *(u16*)(table + 2) + index * 16;
}

void func_ov030_020cdc40(void* resource) {
    if (data_ov030_02109d98 == NULL) {
        return;
    }
    data_ov030_02109d98->unk_00 = resource;
    data_ov030_02109d98->unk_14 = func_0202bd5c(resource);
}

void func_ov030_020cdc70(void* resource, s32 index) {
    if (data_ov030_02109d98 == NULL) {
        return;
    }
    data_ov030_02109d98->unk_04 = resource;
    data_ov030_02109d98->unk_08 = (u32)func_0202bd9c(resource, index);
}

void* func_ov030_020cdca0(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, data_ov030_021089ec);
    return data;
}

void func_ov030_020cdcd4(void* data) {
    Mem_Free(&gDebugHeap, data);
}
