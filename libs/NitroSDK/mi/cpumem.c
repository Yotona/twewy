#include <NitroSDK/mi/cpumem.h>

// clang-format off
asm void MI_CpuFillU16(register u16 value, register u16* dest, register u32 count) {
    mov     r3, #0x0
@1:
    cmp     r3, r2
    strlth    r0, [r1, r3]
    addlt     r3, r3, #0x2
    blt       @1
    bx      lr
}

asm void MI_CpuCopyU16(register const u16* src, register u16* dest, register u32 count) {
    mov     ip, #0x0
@1:
    cmp     ip, r2
    ldrlth    r3, [r0, ip]
    strlth    r3, [r1, ip]
    addlt     ip, ip, #0x2
    blt       @1
    bx      lr
}

asm void MI_CpuFillU32(register u32 value, register u32* dest, register u32 count) {
    add     ip, r1, r2
@1:
    cmp     r1, ip
    stmltia     r1!, {r0}
    blt       @1
    bx      lr
}

asm void MI_CpuCopyU32(register const u32* src, register u32* dest, register u32 count) {
    add     ip, r1, r2
@1:
    cmp     r1, ip
    ldmltia     r0!, {r2}
    stmltia     r1!, {r2}
    blt       @1
    bx      lr
}

asm void MI_CpuFillFromSrc(register const u32* src, register u32* dest, register u32 count) {
    add     ip, r0, r2
@1:
    cmp     r0, ip
    ldmltia     r0!, {r2}
    strlt     r2, [r1, 0x0]
    blt       @1
    bx      lr
}

asm void MI_CpuFill(register u32 value, register void* dest, register u32 count) {
    stmdb   sp!, {r4, r5, r6, r7, r8, r9}
    add     r9, r1, r2
    mov     ip, r2, lsr #0x5
    add     ip, r1, ip, lsl #0x5
    mov     r2, r0
    mov     r3, r2
    mov     r4, r2
    mov     r5, r2
    mov     r6, r2
    mov     r7, r2
    mov     r8, r2
@1:
    cmp     r1, ip
    stmltia     r1!, {r0, r2, r3, r4, r5, r6, r7, r8}
    blt       @1
@2:
    cmp     r1, r9
    stmltia     r1!, {r0}
    blt       @2
    ldmia   sp!, {r4, r5, r6, r7, r8, r9}
    bx      lr
}

asm void MI_CpuCopy(register const u32* src, register u32* dest, register u32 count) {
    stmdb   sp!, {r4, r5, r6, r7, r8, r9, r10}
    add     r10, r1, r2
    mov     ip, r2, lsr #0x5
    add     ip, r1, ip, lsl #0x5
@1:
    cmp     r1, ip
    ldmltia     r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
    stmltia     r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
    blt       @1
@2:
    cmp     r1, r10
    ldmltia     r0!, {r2}
    stmltia     r1!, {r2}
    blt       @2
    ldmia   sp!, {r4, r5, r6, r7, r8, r9, r10}
    bx      lr
}

asm void MI_CpuCopy36(register const u32* src, register u32* dest) {
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    bx      lr
}

asm void MI_CpuCopy48(register const u32* src, register u32* dest) {
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    bx      lr
}

asm void MI_CpuCopy64(register const u32* src, register u32* dest) {
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0!, {r2, r3, ip}
    stmia     r1!, {r2, r3, ip}
    ldmia     r0, {r0, r2, r3, ip}
    stmia     r1!, {r0, r2, r3, ip}
    bx      lr
}

asm void MI_CpuSet(register void* dest, register u8 value, register u32 count) {
    cmp     r2, #0x0
    bxeq    lr
    tst     r0, #0x1
    beq     aligned
    ldrh    ip, [r0, #-0x1]
    and     ip, ip, #0xff
    orr     r3, ip, r1, lsl #0x8
    strh    r3, [r0, #-0x1]
    add     r0, r0, #0x1
    subs    r2, r2, #0x1
    bxeq    lr
aligned:
    cmp     r2, #0x2
    blo     final
    orr     r1, r1, r1, lsl #0x8
    tst     r0, #0x2
    beq     word_aligned
    strh    r1, [r0], #0x2
    subs    r2, r2, #0x2
    bxeq    lr
word_aligned:
    orr     r1, r1, r1, lsl #0x10
    bics    r3, r2, #0x3
    beq     check_half
    sub     r2, r2, r3
    add     ip, r3, r0
loop:
    str     r1, [r0], #0x4
    cmp     r0, ip
    blo     loop
check_half:
    tst     r2, #0x2
    strneh    r1, [r0], #0x2
final:
    tst     r2, #0x1
    bxeq    lr
    ldrh    r3, [r0, #0x0]
    and     r3, r3, #0xff00
    and     r1, r1, #0xff
    orr     r1, r1, r3
    strh    r1, [r0, #0x0]
    bx      lr
}

asm void MI_CpuCopyU8(register const void* src, register void* dest, register u32 count) {
    cmp     r2, #0x0
    bxeq    lr
    tst     r1, #0x1
    beq     aligned
    ldrh    ip, [r1, #-0x1]
    and     ip, ip, #0xff
    tst     r0, #0x1
    ldrneh    r3, [r0, #-0x1]
    movne     r3, r3, lsr #0x8
    ldreqh    r3, [r0, #0x0]
    orr     r3, ip, r3, lsl #0x8
    strh    r3, [r1, #-0x1]
    add     r0, r0, #0x1
    add     r1, r1, #0x1
    subs    r2, r2, #0x1
    bxeq    lr
aligned:
    eor     ip, r1, r0
    tst     ip, #0x1
    beq     check_half_align
    bic     r0, r0, #0x1
    ldrh    ip, [r0], #0x2
    mov     r3, ip, lsr #0x8
    subs    r2, r2, #0x2
    blo     final_byte
loop1:
    ldrh    ip, [r0], #0x2
    orr     ip, r3, ip, lsl #0x8
    strh    ip, [r1], #0x2
    mov     r3, ip, lsr #0x10
    subs    r2, r2, #0x2
    bhs     loop1
final_byte:
    tst     r2, #0x1
    bxeq    lr
    ldrh    ip, [r1, #0x0]
    and     ip, ip, #0xff00
    orr     ip, ip, r3
    strh    ip, [r1, #0x0]
    bx      lr
check_half_align:
    tst     ip, #0x2
    beq     check_word_align
    bics    r3, r2, #0x1
    beq     final
    sub     r2, r2, r3
    add     ip, r3, r1
loop2:
    ldrh    r3, [r0], #0x2
    strh    r3, [r1], #0x2
    cmp     r1, ip
    blo     loop2
    b       final
check_word_align:
    cmp     r2, #0x2
    blo     final
    tst     r1, #0x2
    beq     word_aligned
    ldrh    r3, [r0], #0x2
    strh    r3, [r1], #0x2
    subs    r2, r2, #0x2
    bxeq    lr
word_aligned:
    bics    r3, r2, #0x3
    beq     check_half
    sub     r2, r2, r3
    add     ip, r3, r1
loop3:
    ldr     r3, [r0], #0x4
    str     r3, [r1], #0x4
    cmp     r1, ip
    blo     loop3
check_half:
    tst     r2, #0x2
    ldrneh    r3, [r0], #0x2
    strneh    r3, [r1], #0x2
final:
    tst     r2, #0x1
    bxeq    lr
    ldrh    r2, [r1, #0x0]
    ldrh    r0, [r0, #0x0]
    and     r2, r2, #0xff00
    and     r0, r0, #0xff
    orr     r0, r2, r0
    strh    r0, [r1, #0x0]
    bx      lr
}

#pragma thumb on
asm void MI_CpuClear24(register u32* dest) {
    mov    r1, #0x0
    mov    r2, #0x0
    mov    r3, #0x0
    stmia     r0!, {r1, r2, r3}
    stmia     r0!, {r1, r2, r3}
    stmia     r0!, {r1, r2, r3}
    bx      lr
}
#pragma thumb off

asm u32 MI_CpuSwap(register u32 value, register u32* addr) {
    swp     r0, r0, [r1]
    bx      lr
}
// clang-format on
