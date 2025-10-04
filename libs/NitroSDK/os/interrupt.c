#include <NitroSDK/os/interrupt.h>
#include <NitroSDK/os/thread.h>
#include <bios.h>

extern OS_IRQCallbackInfo IRQCallbackInfo[8];
extern OS_IRQCallback     IRQCallbacks[22];
extern OSThreadQueue      IRQThreadQueue;

void OS_InitIRQQueue(void) {
    IRQThreadQueue.tail = NULL;
    IRQThreadQueue.head = NULL;
}

void OS_SetIRQCallback(u32 mask, OS_IRQCallback callback) {
    for (int shift = 0; shift < 22; shift++) {
        if (mask & TRUE) {
            OS_IRQCallbackInfo* info = NULL;

            if ((shift >= 8) && (shift <= 0xB)) {
                info = &IRQCallbackInfo[shift - 8];
            } else if ((shift >= 3) && (shift <= 6)) {
                info = &IRQCallbackInfo[shift + 1];
            } else {
                IRQCallbacks[shift] = callback;
            }

            if (info != NULL) {
                info->func    = (void (*)(void*))callback;
                info->arg     = NULL;
                info->enabled = TRUE;
            }
        }
        mask >>= 1;
    }
}

OS_IRQCallback OS_GetIRQCallback(u32 mask) {
    for (int shift = 0; shift < 22; shift++) {
        OS_IRQCallback* callback = &IRQCallbacks[shift];

        if (mask & TRUE) {
            if ((shift >= 8) && (shift <= 0xB)) {
                return (OS_IRQCallback)IRQCallbackInfo[shift - 8].func;
            } else if ((shift >= 3) && (shift <= 6)) {
                return (OS_IRQCallback)IRQCallbackInfo[shift + 1].func;
            } else {
                return *callback;
            }
        }
        mask >>= 1;
    }
    return NULL;
}

void OS_SetDMAInterruptCallback(u32 dmaNumber, void (*callback)(void*), void* arg) {
    OS_IRQMask mask                    = 1 << (8 + dmaNumber);
    IRQCallbackInfo[dmaNumber].func    = callback;
    IRQCallbackInfo[dmaNumber].arg     = arg;
    IRQCallbackInfo[dmaNumber].enabled = OS_EnableInterrupts(mask) & mask;
}

void OS_SetTimerInterruptCallback(u32 timerNumber, void (*callback)(void*), void* arg) {
    OS_IRQMask mask                       = 1 << (3 + timerNumber);
    IRQCallbackInfo[timerNumber + 4].func = callback;
    IRQCallbackInfo[timerNumber + 4].arg  = arg;
    OS_EnableInterrupts(mask);
    IRQCallbackInfo[timerNumber + 4].enabled = TRUE;
}

OS_IRQMask OS_SetIRQMask(OS_IRQMask mask) {
    BOOL ime = OS_DisableIME();

    OS_IRQMask prevIE = REG_IE;
    REG_IE            = mask;

    OS_RestoreIME(ime);
    return prevIE;
}

OS_IRQMask OS_EnableInterrupts(OS_IRQMask mask) {
    BOOL ime = OS_DisableIME();

    OS_IRQMask prevIE = REG_IE;
    REG_IE            = prevIE | mask;

    OS_RestoreIME(ime);
    return prevIE;
}

OS_IRQMask OS_DisableInterrupts(OS_IRQMask mask) {
    BOOL ime = OS_DisableIME();

    OS_IRQMask prevIE = REG_IE;
    REG_IE            = prevIE & ~mask;

    OS_RestoreIME(ime);
    return prevIE;
}

OS_IRQMask OS_ResetIRQRequestFlags(OS_IRQMask mask) {
    BOOL ime = OS_DisableIME();

    OS_IRQMask prevIF = REG_IF;
    REG_IF            = mask;

    OS_RestoreIME(ime);
    return prevIF;
}

void func_02038630(void) {
    BIOS_INTERRUPT_IRQSTACK_BOTTOM = 0xfddb597d;
    BIOS_INTERRUPT_IRQSTACK_TOP    = 0x7bf9dd5b;
}
