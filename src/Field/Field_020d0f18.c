#include "Engine/Core/Memory.h"

typedef struct {
    void* unk_00;
    u32   unk_04;
    u32   unk_08;
    u32   unk_0c;
    u32   unk_10;
    u32   unk_14;
    u32   unk_18;
    u32   unk_1c;
    u32   unk_20[8];
    u32   unk_40;
    u32   unk_44;
    u32   unk_48;
    u32   unk_4c;
    u32   unk_50;
    u32   unk_54;
    u32   unk_58;
    u32   unk_5c;
    u32   unk_60;
    u32   unk_64;
    u32   unk_68;
    u32   unk_6c;
} FieldViewerState;

extern FieldViewerState* data_ov030_02109ddc;
extern void              func_ov030_020d2558(s32 size);
extern void*             func_ov030_020d2548(void);
extern void              func_ov030_020d2538(void* data);
extern void              func_ov030_020d2504(void);

char data_ov030_02108a0c[20] = "FO3DViewerImd_Alloc";

void* func_ov030_020d0f18(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, data_ov030_02108a0c);
    return data;
}

void func_ov030_020d0f4c(void* data) {
    Mem_Free(&gDebugHeap, data);
}

void* func_ov030_020d0f64(void) {
    data_ov030_02109ddc            = func_ov030_020d0f18(0x70);
    data_ov030_02109ddc->unk_00    = NULL;
    data_ov030_02109ddc->unk_04    = 0;
    data_ov030_02109ddc->unk_08    = 0;
    data_ov030_02109ddc->unk_0c    = 0;
    data_ov030_02109ddc->unk_10    = 0;
    data_ov030_02109ddc->unk_1c    = 0;
    data_ov030_02109ddc->unk_18    = data_ov030_02109ddc->unk_1c;
    data_ov030_02109ddc->unk_14    = data_ov030_02109ddc->unk_18;
    data_ov030_02109ddc->unk_20[0] = 0;
    data_ov030_02109ddc->unk_40    = 0;
    data_ov030_02109ddc->unk_48    = 0;
    data_ov030_02109ddc->unk_44    = data_ov030_02109ddc->unk_48;
    data_ov030_02109ddc->unk_50    = 0;
    data_ov030_02109ddc->unk_4c    = data_ov030_02109ddc->unk_50;
    data_ov030_02109ddc->unk_58    = 0;
    data_ov030_02109ddc->unk_54    = data_ov030_02109ddc->unk_58;
    data_ov030_02109ddc->unk_60    = 0;
    data_ov030_02109ddc->unk_5c    = data_ov030_02109ddc->unk_60;
    data_ov030_02109ddc->unk_6c    = 0;
    data_ov030_02109ddc->unk_68    = data_ov030_02109ddc->unk_6c;
    data_ov030_02109ddc->unk_64    = 0;
    func_ov030_020d2558(0x40);
    data_ov030_02109ddc->unk_20[2] = (u32)func_ov030_020d2548();
    return data_ov030_02109ddc;
}

void func_ov030_020d1060(void) {
    func_ov030_020d2538((void*)data_ov030_02109ddc->unk_20[2]);
    func_ov030_020d2504();
    func_ov030_020d0f4c(data_ov030_02109ddc);
    data_ov030_02109ddc = NULL;
}
