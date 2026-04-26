#ifndef BOOT_H
#define BOOT_H

#include <types.h>

typedef void (*IRQCallback)(void);

void Interrupts_Init(void);

IRQCallback Interrupts_RegisterHBlankCallback(IRQCallback callback, BOOL enable);
IRQCallback Interrupts_SaveHBlankCallback(IRQCallback callback);
IRQCallback Interrupts_ResetHBlankCallback(void);
BOOL        Interrupts_IsHBlankInterrupted(void);
IRQCallback Interrupts_RegisterVBlankCallback(IRQCallback callback, s32 param_2);

IRQCallback Interrupts_SaveVBlankCallback(IRQCallback callback);
BOOL        Interrupts_IsVBlankInterrupted(void);

#endif // BOOT_H