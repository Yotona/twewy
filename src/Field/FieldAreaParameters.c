#include "Field/Ov030Data.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include <nitro/fx/fx_matrix.h>

// USA 0x020b7a00-0x020b7c58: area parameters and projection helpers.
// Member meanings remain provisional; widths and offsets come from ARM loads.

// Anonymous arm9 BSS object at 0x02076a44 (size 0x100: ends exactly at the
// next symbol data_02076b44). Only +0xFC is touched directly; the Mtx44 at
// +0x08 has its own delinked symbol data_02076a4c (see the extern below).

extern void func_02027c94(s32);
extern void func_ov030_020d18cc(s32, s32);
extern void func_ov030_020d1a1c(s32, s32);
extern void func_ov030_020d20bc(void);
extern u16  func_ov030_020c5430(void);

// Sets up the field camera/projection for one frame from the camera origin
// (FieldEventManager::origin, passed by value). arg1 (eventMgr+0x2191C) is unused.
void func_ov030_020b7a00(void* arg1, FieldPos origin) {
    func_ov030_020d1a1c(gSaveData.fieldPos.x - origin.x, gSaveData.fieldPos.y - origin.y);
    Mtx_OrthoW(-origin.y, -0xC0000 - origin.y, origin.x, origin.x + 0x100000, -0x400000, 0x400000, 0x20000, &data_02076a4c);
    data_02076a44.unk_FC &= ~0x50;
    func_02028a44();
    func_ov030_020d18cc(origin.x + 0x80000, -(origin.y + 0x60000));
}

void func_ov030_020b7aac(s32* arg0) {
    func_ov030_020d20bc();
    func_02027c94(*arg0);
    func_0202688c();
}

void func_ov030_020b7ac8(void) {
    s32 i = 0;
    do {
        gSaveData.unk_2548[i] = data_ov030_020e1d64[i].unk_1C;
        i++;
    } while (i < 44);
}

s16 func_ov030_020b7b00(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_18;
}
s16 func_ov030_020b7b20(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_02;
}
s16 func_ov030_020b7b40(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_04;
}
s16 func_ov030_020b7b60(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_1A;
}
s32 func_ov030_020b7b80(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_00 / 2;
}
s32 func_ov030_020b7ba8(void) {
    s16 origin = func_ov030_020b7b40();
    s32 extent = func_ov030_020b7b60();
    return origin + extent / 2;
}
s16 func_ov030_020b7bc4(void) {
    s16 area   = gSaveData.unk_2458;
    s16 second = data_ov030_020e1d64[area].unk_18;
    s16 first  = data_ov030_020e1d64[area].unk_00;
    if (first <= second) {
        first = second;
    }
    return first;
}
s16 func_ov030_020b7bf8(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_10;
}
s32 func_ov030_020b7c18(void) {
    return data_ov030_020e1d64[gSaveData.unk_2458].unk_14;
}
u16 func_ov030_020b7c34(s32 area) {
    if (area >= 70) {
        return func_ov030_020c5430();
    }
    return data_ov030_020e1d64[area].unk_1E;
}
