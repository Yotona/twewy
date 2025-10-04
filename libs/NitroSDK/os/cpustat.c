#include <types.h>

// clang-format off
asm u32 OS_EnableIRQ(void) {
    mrs r0, cpsr
    bic r1, r0, #0x80
    msr cpsr_c, r1
    and r0, r0, #0x80
    bx lr
}

asm u32 OS_DisableIRQ(void) {
    mrs r0, cpsr
    orr r1, r0, #0x80
    msr cpsr_c, r1
    and r0, r0, #0x80
    bx lr
}

asm u32 OS_RestoreIRQ(register u32 irq_flag) {
    mrs r1, cpsr
    bic r2, r1, #0x80
    orr r2, r2, irq_flag
    msr cpsr_c, r2
    and irq_flag, r1, #0x80
    bx lr
}

asm u32 OS_EnableFIQ(void) {
    mrs r0, cpsr
    bic r1, r0, #0xC0
    msr cpsr_c, r1
    and r0, r0, #0xC0
    bx lr
}

asm u32 OS_DisableFIQ(void) {
    mrs r0, cpsr
    orr r1, r0, #0xC0
    msr cpsr_c, r1
    and r0, r0, #0xC0
    bx lr
}

asm u32 OS_RestoreFIQ(register u32 fiq_flag) {
    mrs r1, cpsr
    bic r2, r1, #0xC0
    orr r2, r2, fiq_flag
    msr cpsr_c, r2
    and r0, r1, #0xC0
    bx lr
}

asm u32 OS_GetIRQFlag(void) {
    mrs r0, cpsr
    and r0, r0, #0x80
    bx lr
}

asm u32 OS_GetCPUMode(void) {
    mrs r0, cpsr
    and r0, r0, #0x1F
    bx lr
}

asm void OS_Delay(register u32 count) {
loop:
    subs count, count, #0x4
    bhs loop
    bx lr
}
// clang-format on

void OS_Wait(void) {
    WaitByLoop(1);
    func_0203823c(1, 1);
}
