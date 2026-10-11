#include "Field/TaskHelpers.h"
// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020b3020-0x020b3128; 6 functions.
// Research draft: docs/decomp/ov030/Field_020b3020.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/FldPlayer.h"

extern s32 data_ov030_020df5c4[];

s32 func_ov030_020b3020(Task* arg0) {
    Ov030Player* obj = arg0->data;

    return obj->unk9E * obj->unkC8;
}

void func_ov030_020b3034(Task* arg0) {
    Ov030Player* obj = arg0->data;

    obj->unkA8 &= ~1;
    func_ov030_020b21c0(obj, func_ov030_020b24b0);
}

s32 func_ov030_020b3058(Task* arg0) {
    Ov030Player* obj = arg0->data;
    s32          right;
    s32          half = obj->unk9C;
    s32          left;
    s32          top;
    s32          cx = obj->screen.x;
    s32          cy = obj->screen.y;
    TouchCoord   coord;
    s32          y = func_ov030_020b3020(arg0);

    left  = (cx - (half << 12)) >> 12;
    top   = (cy - (y + 0x18000)) >> 12;
    right = (cx + (half << 12)) >> 12;

    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    TouchInput_GetCoord(&coord);
    if (left < coord.x && coord.x < right && top < coord.y && coord.y < (cy >> 12)) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020b30d0(Task* arg0) {
    if (((Ov030Player*)arg0->data)->unkE8 == -1) {
        return 0;
    }
    return func_ov030_020b3058(arg0);
}

void func_ov030_020b30f4(Task* arg0) {
    func_ov030_020b21c0(arg0->data, func_ov030_020b2418);
}

s32 func_ov030_020b310c(s32 arg0) {
    return data_ov030_020df5c4[((arg0 >> 12) & 1) + (arg0 >> 13)];
}
