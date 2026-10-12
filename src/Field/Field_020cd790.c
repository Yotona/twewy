#include "Engine/Core/Memory.h"

typedef struct {
    const void* callbacks;
    u32         state0;
    u32         state1;
    u32         state2;
} FieldAllocatorState;

typedef struct {
    void* (*allocate)(Heap*, s32);
    void (*free)(Heap*, void*);
    u32 parameters[9];
} FieldAllocatorCallbacks;

extern FieldAllocatorState data_ov030_02109d88;

void* func_ov030_020cd790(Heap* heap, s32 size);
void  func_ov030_020cd7c0(Heap* heap, void* data);

char                          data_ov030_021089c8[20] = "FOAllocatorAlloc";
const FieldAllocatorCallbacks data_ov030_020ed198     = {
    func_ov030_020cd790, func_ov030_020cd7c0, {0x7C0003E0, 31, 16, 16, 32767, 0, 10, 0x7FFF0000, 5}
};

void* func_ov030_020cd790(Heap* heap, s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, data_ov030_021089c8);
    return data;
}

void func_ov030_020cd7c0(Heap* heap, void* data) {
    Mem_Free(&gDebugHeap, data);
}

void func_ov030_020cd7d4(void) {
    data_ov030_02109d88.callbacks = &data_ov030_020ed198;
    data_ov030_02109d88.state0    = 0;
    data_ov030_02109d88.state1    = 0;
    data_ov030_02109d88.state2    = 0;
}

FieldAllocatorState* func_ov030_020cd7fc(void) {
    return &data_ov030_02109d88;
}
