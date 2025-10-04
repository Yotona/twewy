#ifndef NITROSDK_UTIL_H
#define NITROSDK_UTIL_H

#include <types.h>

/**
 * @brief Enter a critical section by disabling IRQs and saving the previous state
 */
#define CRITICAL_SECTION_ENTER() u32 prevIRQState = OS_DisableIRQ()

/**
 * @brief Leave a critical section by restoring the previous IRQ state
 */
#define CRITICAL_SECTION_LEAVE() OS_RestoreIRQ(prevIRQState)

#endif // NITROSDK_UTIL_H