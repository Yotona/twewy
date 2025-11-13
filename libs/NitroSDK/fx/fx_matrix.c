#include <NitroSDK/fx/fx_division.h>
#include <NitroSDK/fx/fx_matrix.h>

// clang-format off
asm void Mtx_Identity33(register Mtx33* matrix) {
    mov     r2, #0x1000
    str     r2, [matrix, #0x20]
    mov     r3, #0
    stmia   matrix!, {r2, r3}
    mov     r1, #0
    stmia   matrix!, {r1, r3}
    stmia   matrix!, {r2, r3}
    stmia   matrix!, {r1, r3}
    bx      lr
}
// clang-format on

void Mtx_Scale33(const Mtx33* src, Mtx33* dest, s32 scaleX, s32 scaleY, s32 scaleZ) {
    dest->_00 = (((u64)scaleX * src->_00) >> 12);
    dest->_01 = (((u64)scaleX * src->_01) >> 12);
    dest->_02 = (((u64)scaleX * src->_02) >> 12);

    dest->_10 = (((u64)scaleY * src->_10) >> 12);
    dest->_11 = (((u64)scaleY * src->_11) >> 12);
    dest->_12 = (((u64)scaleY * src->_12) >> 12);

    dest->_20 = (((u64)scaleZ * src->_20) >> 12);
    dest->_21 = (((u64)scaleZ * src->_21) >> 12);
    dest->_22 = (((u64)scaleZ * src->_22) >> 12);
}

// clang-format off
#pragma thumb on
asm void Mtx_YRot33(register Mtx33* matrix, register s32 sin, register s32 cos) {
    str     cos, [matrix, #0]
    str     cos, [matrix, #0x20]
    mov     r3, #0
    str     r3, [matrix, #0x4]
    str     r3, [matrix, #0xC]
    str     r3, [matrix, #0x14]
    str     r3, [matrix, #0x1C]
    neg     cos, sin
    mov     r3, #1
    lsl     r3, r3, #0xC
    str     sin, [matrix, #0x18]
    str     cos, [matrix, #0x8]
    str     r3, [matrix, #0x10]
    bx      lr
}
#pragma thumb off

asm void Mtx_Identity43(register Mtx43* matrix) {
    mov     r2, #0x1000
    mov     r3, #0
    stmia   matrix!, {r2, r3}
    mov     r1, #0
    stmia   matrix!, {r1, r3}
    stmia   matrix!, {r2, r3}
    stmia   matrix!, {r1, r3}
    stmia   matrix!, {r2, r3}
    stmia   matrix!, {r1, r3}
    bx      lr
}

asm void Mtx_Copy43To44(register const Mtx43* src, register Mtx44* dest) {
    stmdb   sp!, {r4}
    mov     r12, #0
    ldmia   src!, {r2-r4}
    stmia   dest!, {r2-r4, r12}
    ldmia   src!, {r2-r4}
    stmia   dest!, {r2-r4, r12}
    ldmia   src!, {r2-r4}
    stmia   dest!, {r2-r4, r12}
    mov     r12, #0x1000
    ldmia   src!, {r2-r4}
    stmia   dest!, {r2-r4, r12}
    ldmia   sp!, {r4}
    bx      lr
}
// clang-format on

void Mtx_Scale43(const Mtx43* src, Mtx43* dest, s32 scaleX, s32 scaleY, s32 scaleZ) {
    Mtx_Scale33((const Mtx33*)src, (Mtx33*)dest, scaleX, scaleY, scaleZ);
    dest->_30 = src->_30;
    dest->_31 = src->_31;
    dest->_32 = src->_32;
}

s32 MTX_Inverse43(const Mtx43* src, Mtx43* dest) {
    Mtx43  temp;
    Mtx43* mtx;

    if (src == dest) {
        mtx = &temp;
    } else {
        mtx = dest;
    }

    s32 subDet0 = ((s64)src->_11 * src->_22 - (s64)src->_12 * src->_21 + (s64)(0x1000L >> 1)) >> 12;
    s32 subDet1 = ((s64)src->_10 * src->_22 - (s64)src->_12 * src->_20 + (s64)(0x1000L >> 1)) >> 12;
    s32 subDet2 = ((s64)src->_10 * src->_21 - (s64)src->_11 * src->_20 + (s64)(0x1000L >> 1)) >> 12;
    s32 det     = ((s64)src->_00 * subDet0 - (s64)src->_01 * subDet1 + (s64)src->_02 * subDet2 + (s64)(0x1000L >> 1)) >> 12;
    if (det == 0) {
        return -1;
    }

    FX_InvAsync(det);

    s32 tmp01 = (((s64)src->_01 * src->_22 - (s64)src->_21 * src->_02) >> 12);
    s32 tmp02 = (((s64)src->_01 * src->_12 - (s64)src->_11 * src->_02) >> 12);
    s32 tmp11 = (((s64)src->_00 * src->_22 - (s64)src->_20 * src->_02) >> 12);
    s32 tmp12 = (((s64)src->_00 * src->_12 - (s64)src->_10 * src->_02) >> 12);

    det      = FX_GetDivResult();
    mtx->_00 = (s32)(((s64)det * subDet0) >> 12);
    mtx->_01 = -(s32)(((s64)det * tmp01) >> 12);
    mtx->_02 = (s32)(((s64)det * tmp02) >> 12);

    mtx->_10 = -(s32)(((s64)det * subDet1) >> 12);
    mtx->_11 = (s32)(((s64)det * tmp11) >> 12);
    mtx->_12 = -(s32)(((s64)det * tmp12) >> 12);

    mtx->_20 = (s32)(((s64)det * subDet2) >> 12);
    mtx->_21 = -(s32)(((s64)det * ((s32)(((s64)src->_00 * src->_21 - (s64)src->_20 * src->_01) >> 12))) >> 12);
    mtx->_22 = (s32)(((s64)det * ((s32)(((s64)src->_00 * src->_11 - (s64)src->_10 * src->_01) >> 12))) >> 12);

    mtx->_30 = -(s32)(((s64)mtx->_00 * src->_30 + (s64)mtx->_10 * src->_31 + (s64)mtx->_20 * src->_32) >> 12);
    mtx->_31 = -(s32)(((s64)mtx->_01 * src->_30 + (s64)mtx->_11 * src->_31 + (s64)mtx->_21 * src->_32) >> 12);
    mtx->_32 = -(s32)(((s64)mtx->_02 * src->_30 + (s64)mtx->_12 * src->_31 + (s64)mtx->_22 * src->_32) >> 12);

    if (mtx == &temp) {
        func_0203b370(&temp, dest);
    }
    return 0;
}

void Mtx_Concat43(const Mtx43* a, const Mtx43* b, Mtx43* dest) {
    Mtx43  temp;
    Mtx43* mtx;

    if (dest == b) {
        mtx = &temp;
    } else {
        mtx = dest;
    }

    s32 xa = a->_00;
    s32 ya = a->_01;
    s32 za = a->_02;

    mtx->_00 = (s32)(((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20) >> 12);
    mtx->_01 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12);

    s32 xb = b->_02;
    s32 yb = b->_12;
    s32 zb = b->_22;

    mtx->_02 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);

    xa = a->_10;
    ya = a->_11;
    za = a->_12;

    mtx->_12 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);
    mtx->_11 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12);

    xb = b->_00;
    yb = b->_10;
    zb = b->_20;

    mtx->_10 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);

    xa = a->_20;
    ya = a->_21;
    za = a->_22;

    mtx->_20 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);
    mtx->_21 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12);
    xb       = b->_02;
    yb       = b->_12;
    zb       = b->_22;

    mtx->_22 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12);

    xa = a->_30;
    ya = a->_31;
    za = a->_32;

    mtx->_32 = (s32)((((s64)xa * xb + (s64)ya * yb + (s64)za * zb) >> 12) + b->_32);
    mtx->_31 = (s32)((((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21) >> 12) + b->_31);
    mtx->_30 = (s32)((((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20) >> 12) + b->_30);

    if (mtx == &temp) {
        *dest = temp;
    }
}

void Mtx_LookAt(const Vec* camPosition, const Vec* upDirection, const Vec* targetPosition, Mtx43* matrix) {
    Vec forward, right, up;

    forward.x = camPosition->x - targetPosition->x;
    forward.y = camPosition->y - targetPosition->y;
    forward.z = camPosition->z - targetPosition->z;
    Vec_Normalize(&forward, &forward);
    Vec_CrossProduct(upDirection, &forward, &right);
    Vec_Normalize(&right, &right);
    Vec_CrossProduct(&forward, &right, &up);
    matrix->_00 = right.x;
    matrix->_01 = up.x;
    matrix->_02 = forward.x;
    matrix->_10 = right.y;
    matrix->_11 = up.y;
    matrix->_12 = forward.y;
    matrix->_20 = right.z;
    matrix->_21 = up.z;
    matrix->_22 = forward.z;
    matrix->_30 = -Vec_DotProduct(camPosition, &right);
    matrix->_31 = -Vec_DotProduct(camPosition, &up);
    matrix->_32 = -Vec_DotProduct(camPosition, &forward);
}

// clang-format off
asm void Mtx_Identity44(register Mtx44* matrix) {
    mov     r2, #0x1000
    mov     r3, #0
    stmia   matrix!, {r2, r3}
    mov     r1, #0
    stmia   matrix!, {r1, r3}
    stmia   matrix!, {r1, r2, r3}
    stmia   matrix!, {r1, r3}
    stmia   matrix!, {r1, r2, r3}
    stmia   matrix!, {r1, r3}
    stmia   matrix!, {r1, r2}
    bx      lr
}

asm void Mtx_Copy44To43(register const Mtx44* src, register Mtx43* dest) {
    ldmia   src!, {r2-r3, r12}
    add     src, src, #4
    stmia   dest!, {r2-r3, r12}
    ldmia   src!, {r2-r3, r12}
    add     src, src, #4
    stmia   dest!, {r2-r3, r12}
    ldmia   src!, {r2-r3, r12}
    add     src, src, #4
    stmia   dest!, {r2-r3, r12}
    ldmia   src!, {r2-r3, r12}
    add     src, src, #4
    stmia   dest!, {r2-r3, r12}
    bx      lr
}
// clang-format on

void Mtx_Concat44(const Mtx44* a, const Mtx44* b, Mtx44* dest) {
    Mtx44  temp;
    Mtx44* mtx;

    if (dest == b) {
        mtx = &temp;
    } else {
        mtx = dest;
    }
    s32 xa = a->_00;
    s32 ya = a->_01;
    s32 za = a->_02;
    s32 wa = a->_03;

    mtx->_00 = (s32)(((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20 + (s64)wa * b->_30) >> 12);
    mtx->_01 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    mtx->_03 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    s32 xb = b->_02;
    s32 yb = b->_12;
    s32 zb = b->_22;
    s32 wb = b->_32;

    mtx->_02 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);

    xa = a->_10;
    ya = a->_11;
    za = a->_12;
    wa = a->_13;

    mtx->_12 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);
    mtx->_11 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    mtx->_13 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    xb = b->_00;
    yb = b->_10;
    zb = b->_20;
    wb = b->_30;

    mtx->_10 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);

    xa = a->_20;
    ya = a->_21;
    za = a->_22;
    wa = a->_23;

    mtx->_20 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);
    mtx->_21 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    mtx->_23 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    xb = b->_02;
    yb = b->_12;
    zb = b->_22;
    wb = b->_32;

    mtx->_22 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);

    xa = a->_30;
    ya = a->_31;
    za = a->_32;
    wa = a->_33;

    mtx->_32 = (s32)(((s64)xa * xb + (s64)ya * yb + (s64)za * zb + (s64)wa * wb) >> 12);
    mtx->_31 = (s32)(((s64)xa * b->_01 + (s64)ya * b->_11 + (s64)za * b->_21 + (s64)wa * b->_31) >> 12);
    mtx->_30 = (s32)(((s64)xa * b->_00 + (s64)ya * b->_10 + (s64)za * b->_20 + (s64)wa * b->_30) >> 12);
    mtx->_33 = (s32)(((s64)xa * b->_03 + (s64)ya * b->_13 + (s64)za * b->_23 + (s64)wa * b->_33) >> 12);

    if (mtx == &temp) {
        *dest = temp;
    }
}

static inline s32 FX_Mul_32x64(s32 a, s64 b) {
    s64 temp = b * a + 0x80000000LL;
    return temp >> 32;
}

void Mtx_OrthoW(s32 top, s32 bottom, s32 left, s32 right, s32 near, s32 far, s32 scale, Mtx44* mtx) {
    FX_InvAsync(right - left);
    mtx->_01 = 0;
    mtx->_02 = 0;
    mtx->_03 = 0;
    mtx->_10 = 0;
    mtx->_12 = 0;
    mtx->_13 = 0;
    mtx->_20 = 0;
    mtx->_21 = 0;
    mtx->_23 = 0;
    mtx->_33 = scale;

    s64 leftRight = FX_GetDivResultFx64c();
    SetDiv_Numer64Denom32(0x1000LL << 32, top - bottom);
    if (scale != 0x1000L) {
        leftRight = (leftRight * scale) / 0x1000L;
    }
    mtx->_00 = FX_Mul_32x64(0x1000L * 2, leftRight);

    s64 topBottom = FX_GetDivResultFx64c();
    SetDiv_Numer64Denom32(0x1000LL << 32, near - far);
    if (scale != 0x1000L) {
        topBottom = (topBottom * scale) / 0x1000L;
    }
    mtx->_11 = FX_Mul_32x64(0x1000L * 2, topBottom);

    s64 nearFar = FX_GetDivResultFx64c();
    if (scale != 0x1000L) {
        nearFar = (nearFar * scale) / 0x1000L;
    }
    mtx->_22 = FX_Mul_32x64(0x1000L * 2, nearFar);
    mtx->_30 = FX_Mul_32x64(-right - left, leftRight);
    mtx->_31 = FX_Mul_32x64(-top - bottom, topBottom);
    mtx->_32 = FX_Mul_32x64(far + near, nearFar);
}