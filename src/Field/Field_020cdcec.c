#include "Engine/Core/Memory.h"

extern void* data_ov030_02109d9c;

char data_ov030_021089fc[16] = "FO3DObj_Alloc";

void func_ov030_020cdcec(void* state) {
    data_ov030_02109d9c = state;
}

void* func_ov030_020cde54(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, data_ov030_021089fc);
    return data;
}

void func_ov030_020cde88(void* data) {
    Mem_Free(&gDebugHeap, data);
}
