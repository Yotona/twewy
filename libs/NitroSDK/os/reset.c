#include <NitroSDK/os/interrupt.h>
#include <bios.h>
#include <types.h>

void func_0203a850();

u16 data_0207e068 = FALSE;
u16 data_0207e06a = FALSE;

void func_0203a7fc(void) {
    if (data_0207e06a) {
        return;
    }
    data_0207e06a = TRUE;

    func_0203cc10();

    while (func_0203cd68(12, 1) == FALSE) {
        // intentionally empty
    }

    func_0203cd1c(12, func_0203a850);
}

void func_0203a850(u32 arg0, u32 arg1) {
    if ((u16)((arg1 & 0x7F00) >> 8) == 0x10) {
        data_0207e068 = TRUE;
    } else {
        func_0203aad0();
    }
}

void func_0203a884(u16 arg0) {
    while (func_0203cd8c(12, arg0 << 8, 0)) {
        // intentionally empty
    }
}

// Nonmatching: Data not loading into constant pool properly
void OS_SystemReset(u32 reset) {
    if (BIOS_IsDownloadPlay()) {
        func_0203aad0();
    }

    u16 val = func_02038968();
    func_02042344(val);

    func_0203ae1c(0);
    func_0203ae1c(1);
    func_0203ae1c(2);
    func_0203ae1c(3);

    OS_SetIRQMask(IRQ_IPC_RECEIVE);
    OS_ResetIRQRequestFlags(IRQ_ALL);

    BIOS_RESET = reset;

    func_0203a884(16);
    func_01ff81b4();
}
