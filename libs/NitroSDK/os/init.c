#include <registers.h>

// clang-format off
static asm void func_020398b0(void) {
    mov     r12, #0x4000000
    ldr     r1, [r12, #0x208]
    str     r12, [r12, #0x208]
loop:
    ldrh    r0, [r12, #0x6]
    cmp     r0, #0x0
    bne     loop
    str     r1, [r12, #0x208]
    bx      lr
}
// clang-format on

void OS_InitAllSystems(void) {
    OS_RegionsInit();
    func_0203cc10();
    func_02038664();
    OS_ExtendedRegionInit();
    OS_InitIRQQueue();
    func_02038630();
    func_0203a114();
    func_0203babc();
    OS_VAlarmSystemInit();
    func_0203aa14();
    func_02038cb4();
    func_0203a7fc();
    func_0204799c();
    func_02043134();
    func_02040ac0();
    func_020398b0();
}
