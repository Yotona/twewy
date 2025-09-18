#include <NitroSDK/os/cache.h>

// clang-format off
asm BOOL DC_Enable(void) {
    mrc     p15, 0, r1, c1, c0, 0
    and     r0, r1, 4
    mov     r0, r0, lsr 2
    orr     r1, r1, 4
    mcr     p15, 0, r1, c1, c0, 0
    bx      lr
}

asm void DC_InvalidateAll(void) {
    mov     r0, 0
    mcr     p15, 0, r0, c7, c6, 0
    bx      lr
}

asm void DC_CleanAll(void) {
    mov     r1, 0
outerLoop:
    mov     r0, 0
innerLoop:
    orr     r2, r1, r0
    mcr     p15, 0, r2, c7, c10, 2
    add     r0, r0, 0x20
    cmp     r0, 0x400
    blt     innerLoop
    add     r1, r1, 0x40000000
    cmp     r1, 0
    bne     outerLoop
    bx      lr
}

asm void DC_PurgeAll(void) {
    mov     r12, 0
    mov     r1, 0
@1:
    mov     r0, 0
@2:
    orr     r2, r1, r0
    mcr     p15, 0, r12, c7, c10, 4
    mcr     p15, 0, r2, c7, c14, 2
    add     r0, r0, 0x20
    cmp     r0, 0x400
    blt     @2
    add     r1, r1, 0x40000000
    cmp     r1, #0
    bne     @1
    bx      lr
}

asm void DC_InvalidateRange(register void* addr, register u32 count) {
    add     r1, r1, r0
    bic     r0, r0, 0x1F
loop:
    mcr     p15, 0, r0, c7, c6, 1
    add     r0, r0, 0x20
    cmp     r0, r1
    blt     loop
    bx      lr
}

asm void DC_CleanRange(register const void * addr, register u32 count) {
    add     r1, r1, r0
    bic     r0, r0, 0x1F
loop:
    mcr     p15, 0, r0, c7, c10, 1
    add     r0, r0, 0x20
    cmp     r0, r1
    blt     loop
    bx      lr
}

asm void DC_PurgeRange(register const void * addr, register u32 count) {
    mov     r12, 0
    add     r1, r1, r0
    bic     r0, r0, 0x1F
loop:
    mcr     p15, 0, r12, c7, c10, 4
    mcr     p15, 0, r0, c7, c14, 1
    add     r0, r0, 0x20
    cmp     r0, r1
    blt     loop
    bx      lr
}

asm void DC_DrainWriteBuffer(void) {
    mov     r0, 0
    mcr     p15, 0, r0, c7, c10, 4
    bx      lr
}

asm BOOL IC_Enable(void) {
    mrc     p15, 0, r1, c1, c0, 0
    and     r0, r1, 0x1000
    mov     r0, r0, lsr 12
    orr     r1, r1, 0x1000
    mcr     p15, 0, r1, c1, c0, 0
    bx      lr
}

asm void IC_InvalidateAll(void) {
    mov r0, 0
    mcr p15, 0, r0, c7, c5, 0
    bx lr
}

asm void IC_InvalidateRange (register void* addr, register u32 count)
{
    add r1, r1, r0
    bic r0, r0, 0x20 - 1
loop:
    mcr p15, 0, r0, c7, c5, 1
    add r0, r0, 0x20
    cmp r0, r1
    blt loop
    bx lr
}
// clang-format on
