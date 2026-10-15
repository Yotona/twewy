#include "Engine/Core/Memory.h"
#include "Field/FO3D.h"
#include <nitro/fx.h>

Field3DObject* data_ov030_02109d9c;

void func_ov030_020cde0c(s32 arg0);
void func_ov030_020cde40(s32 arg0);

void func_ov030_020cdcec(Field3DObject* state) {
    data_ov030_02109d9c = state;
}

Field3DObject* func_ov030_020cdcfc(void) {
    data_ov030_02109d9c = func_ov030_020cdca0(0x5C);

    data_ov030_02109d9c->unk_0C = 0;
    data_ov030_02109d9c->unk_10 = 0;
    data_ov030_02109d9c->unk_14 = 0;
    data_ov030_02109d9c->unk_18 = 0;
    data_ov030_02109d9c->unk_1C = 0;
    data_ov030_02109d9c->unk_50 = 0;
    func_ov030_020cdde8(0, 0, 0);
    func_ov030_020cdd98(0);
    func_ov030_020cde0c(1);
    func_ov030_020cde40(1);
    return data_ov030_02109d9c;
}

void func_ov030_020cdd74(void) {
    func_ov030_020cdcd4(data_ov030_02109d9c);
    data_ov030_02109d9c = NULL;
}

void func_ov030_020cdd98(s16 arg0) {
    s32 temp_r2 = (arg0 >> 4) * 2;
    Mtx_YRot33(&data_ov030_02109d9c->unk_20, FX_SinCosTable_[temp_r2], FX_SinCosTable_[temp_r2 + 1]);
    data_ov030_02109d9c->unk_44 = arg0;
}

void func_ov030_020cdde8(s32 arg0, s32 arg1, s32 arg2) {
    data_ov030_02109d9c->unk_00 = arg0;
    data_ov030_02109d9c->unk_04 = arg1;
    data_ov030_02109d9c->unk_08 = arg2;
}

void func_ov030_020cde0c(s32 arg0) {
    data_ov030_02109d9c->unk_48 = arg0;
}

void func_ov030_020cde20(s32 arg0) {
    data_ov030_02109d9c->unk_4C = arg0;
    data_ov030_02109d9c->unk_50 = 1;
}

void func_ov030_020cde40(s32 arg0) {
    data_ov030_02109d9c->unk_58 = arg0;
}

void* func_ov030_020cde54(s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FO3DObj_Alloc");
    return data;
}

void func_ov030_020cde88(void* data) {
    Mem_Free(&gDebugHeap, data);
}
