#ifndef _NITRO_OS_CPUSTAT_H_
#define _NITRO_OS_CPUSTAT_H_

#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enable IRQ interrupts
 *
 * @return Previous IRQ flag
 */
u32 OS_EnableIRQ(void);

/**
 * @brief Disable IRQ interrupts
 *
 * @return Previous IRQ flag
 */
u32 OS_DisableIRQ(void);

/**
 * @brief Restore IRQ interrupts to a previous state
 *
 * @param irq_flag Previous IRQ flag to restore
 * @return Current IRQ flag before restoration
 */
u32 OS_RestoreIRQ(u32 irq_flag);

/**
 * @brief Enable FIQ interrupts
 *
 * @return Previous FIQ flag
 */
u32 OS_EnableFIQ(void);

/**
 * @brief Disable FIQ interrupts
 *
 * @return Previous FIQ flag
 */
u32 OS_DisableFIQ(void);

/**
 * @brief Restore FIQ interrupts to a previous state
 *
 * @param fiq_flag Previous FIQ flag to restore
 * @return Current FIQ flag before restoration
 */
u32 OS_RestoreFIQ(u32 fiq_flag);

/**
 * @brief Get the current IRQ flag
 *
 * @return Current IRQ flag
 */
u32 OS_GetIRQFlag(void);

/**
 * @brief Get the current CPU mode
 *
 * @return Current CPU mode
 */
u32 OS_GetCPUMode(void);

/**
 * @brief Delay execution for a specified count
 *
 * @param count Number of cycles to delay
 */
void OS_Delay(u32 count);

/**
 * @brief Wait indefinitely (idle)
 */
void OS_Wait(void);

/**
 * @brief Enter a critical section by disabling IRQs and saving the previous state
 */
#define ENTER_CRITICAL_SECTION() u32 prevIRQState = OS_DisableIRQ()

/**
 * @brief Leave a critical section by restoring the previous IRQ state
 */
#define LEAVE_CRITICAL_SECTION() OS_RestoreIRQ(prevIRQState)

#ifdef __cplusplus
}
#endif

#endif // _NITRO_OS_CPUSTAT_H_