#include "Otosu/OtosuGame.h"
#include <nitro/fx/fx_vector.h>

void OtuVec_Copy(Vec2p* src, Vec2p* dst) {
    dst->x = src->x;
    dst->y = src->y;
}

void OtuVec_Add(Vec2p* a, Vec2p* b, Vec2p* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
}

void OtuVec_Sub(Vec2p* a, Vec2p* b, Vec2p* out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
}

void OtuVec_Scale(s32 angle, Vec2p* a, Vec2p* out) {
    out->x = (s32)(((s64)angle * a->x) >> 12);
    out->y = (s32)(((s64)angle * a->y) >> 12);
}

void OtuVec_ScaleAdd(s32 angle, Vec2p* a, Vec2p* b, Vec2p* out) {
    out->x = b->x + (s32)(((s64)angle * a->x) >> 12);
    out->y = b->y + (s32)(((s64)angle * a->y) >> 12);
}

s32 OtuVec_Dot(Vec2p* a, Vec2p* b) {
    return (s32)(((s64)a->x * b->x + (s64)a->y * b->y + 0x800) >> 12);
}

s32 OtuVec_Cross(Vec2p* a, Vec2p* b) {
    return (s32)(((s64)a->x * b->y - (s64)b->x * a->y + 0x800) >> 12);
}

// Nonmatching: register allocation
s32 OtuVec_Distance(Vec2p* a, Vec2p* b) {
    s32 dx  = a->x - b->x;
    s32 dy  = a->y - b->y;
    s64 sum = (s64)dx * dx + (s64)dy * dy;

    REG_SQRT_CNT   = 1;
    REG_SQRT_PARAM = (u64)sum << 2;

    while (REG_SQRT_CNT & 0x8000) {
    }

    return ((s32)REG_SQRT_RESULT + 1) >> 1;
}

s32 OtuVec_Length(Vec2p* v) {
    Vec u;
    s32 x = v->x;
    s32 y = v->y;

    u.x = x;
    u.y = y;
    u.z = 0;

    return Vec_Magnitude(&u);
}

void OtuVec_Normalize(Vec2p* src, Vec2p* dest) {
    Vec u;
    Vec out;
    s32 x = src->x;
    s32 y = src->y;

    u.x = x;
    u.y = y;
    u.z = 0;

    Vec_Normalize(&u, &out);

    x       = out.x;
    y       = out.y;
    dest->x = x;
    dest->y = y;
}

s32 OtuFrameCursor_Init(OtuFrameCursor* cursor, s32 base, OtuFrameSlot* slots, s16 count) {
    cursor->slots     = slots;
    cursor->count     = count;
    cursor->base      = base;
    cursor->index     = 0;
    cursor->remaining = cursor->slots[cursor->index].duration;

    return cursor->base + cursor->slots[cursor->index].offset * 2;
}

s32 OtuFrameCursor_Step(OtuFrameCursor* cursor) {
    if (cursor->remaining <= 0) {
        cursor->index++;
        if (cursor->index >= cursor->count) {
            cursor->index = 0;
        }
        cursor->remaining = cursor->slots[cursor->index].duration;
    }
    cursor->remaining--;

    return cursor->base + cursor->slots[cursor->index].offset * 2;
}