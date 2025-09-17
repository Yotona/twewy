#ifndef BOOT_H
#define BOOT_H

#include <types.h>

typedef void (*IRQCallback)(void);

void func_02001254(void);

IRQCallback Interrupts_RegisterHBlankCallback(IRQCallback callback, BOOL enable);
IRQCallback Interrupts_SaveHBlankCallback(IRQCallback callback);
IRQCallback Interrupts_ResetHBlankCallback(void);
BOOL        Interrupts_IsHBlankInterrupted(void);

#endif // BOOT_H