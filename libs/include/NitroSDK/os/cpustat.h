#ifndef NITROSDK_OS_CPUSTAT_H_
#define NITROSDK_OS_CPUSTAT_H_

#include <types.h>

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

#ifdef __cplusplus
}
#endif

#endif // NITROSDK_OS_CPUSTAT_H_