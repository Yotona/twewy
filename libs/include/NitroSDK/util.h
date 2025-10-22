#ifndef NITROSDK_UTIL_H
#define NITROSDK_UTIL_H

#include <types.h>

#define ALIGN_MASK(align)         ((align) - 1)
#define ALIGN_BYTE(target, align) (((u32)target + ALIGN_MASK(align)) & ~ALIGN_MASK(align))

/**
 * @brief Enter a critical section by disabling IRQs and saving the previous state
 */
#define ENTER_CRITICAL_SECTION() u32 prevIRQState = OS_DisableIRQ()

/**
 * @brief Leave a critical section by restoring the previous IRQ state
 */
#define LEAVE_CRITICAL_SECTION() OS_RestoreIRQ(prevIRQState)

#endif // NITROSDK_UTIL_H