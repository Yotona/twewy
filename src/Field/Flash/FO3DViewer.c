#include "Engine/Core/Memory.h"

char data_ov030_0210909c[16] = "FOFileIO_Alloc";

void* func_ov030_020d2188(s32 size) {
    void* data = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, data, data_ov030_0210909c);
    return data;
}

void func_ov030_020d21bc(void* data) {
    Mem_Free(&gMainHeap, data);
}

void func_ov030_020d21d4(void* data) {
    func_ov030_020d21bc(data);
}
