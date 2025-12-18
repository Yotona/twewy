#ifndef BOOT_H
#define BOOT_H

#include <types.h>

typedef void (*IRQCallback)(void);

IRQCallback Interrupts_RegisterHBlankCallback(IRQCallback callback, BOOL enable);
IRQCallback Interrupts_SaveHBlankCallback(IRQCallback callback);
IRQCallback Interrupts_ResetHBlankCallback(void);
BOOL        Interrupts_IsHBlankInterrupted(void);
IRQCallback Interrupts_RegisterVBlankCallback(IRQCallback callback, s32 param_2);

#endif // BOOT_H