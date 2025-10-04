#ifndef NITROSDK_OS_INTERRUPTS_H
#define NITROSDK_OS_INTERRUPTS_H

#include <registers.h>
#include <types.h>

/// MARK: Typedefs

typedef enum OS_IRQMask {
    IRQ_ALL         = -1,
    IRQ_VBLANK      = 1 << 0,  // LCD V-Blank
    IRQ_HBLANK      = 1 << 1,  // LCD H-Blank
    IRQ_VCOUNT      = 1 << 2,  // LCD V-Counter match
    IRQ_TIMER_0     = 1 << 3,  // Timer 0 Overflow
    IRQ_TIMER_1     = 1 << 4,  // Timer 1 Overflow
    IRQ_TIMER_2     = 1 << 5,  // Timer 2 Overflow
    IRQ_TIMER_3     = 1 << 6,  // Timer 3 Overflow
    IRQ_RTC         = 1 << 7,  // Real Time Clock
    IRQ_DMA_0       = 1 << 8,  // DMA 0
    IRQ_DMA_1       = 1 << 9,  // DMA 1
    IRQ_DMA_2       = 1 << 10, // DMA 2
    IRQ_DMA_3       = 1 << 11, // DMA 3
    IRQ_KEYPAD      = 1 << 12, // Keypad
    IRQ_GBA_SLOT    = 1 << 13, // GBA Cartridge slot
    IRQ_IPC_SYNC    = 1 << 16, // IPC Sync
    IRQ_IPC_SEND    = 1 << 17, // IPC Send FIFO Empty
    IRQ_IPC_RECEIVE = 1 << 18, // IPC Receive FIFO Not Empty
    IRQ_CARD_DATA   = 1 << 19, // Game Card Data Transfer
    IRQ_CARD_IREQ   = 1 << 20, // Game Card IREQ_MC
    IRQ_GEOMETRY    = 1 << 21, // Geometry Command FIFO
    IRQ_SCREEN_FOLD = 1 << 22, // Screen/System Fold
    IRQ_SPI         = 1 << 23, // SPI Data Transfer
    IRQ_WIFI        = 1 << 24, // WiFi
} OS_IRQMask;

typedef void (*OS_IRQCallback)(void);

typedef struct {
    /* 0x00 */ void (*func)(void*);
    /* 0x04 */ u32   enabled;
    /* 0x08 */ void* arg;
} OS_IRQCallbackInfo;

extern OS_IRQCallbackInfo IRQCallbackInfo[8];
extern OS_IRQCallback     IRQCallbacks[22];

/// MARK: Functions

/**
 * @brief Initializes the IRQ thread queue.
 */
void OS_InitIRQQueue(void);

/**
 * @brief Sets the IRQ callback for the given mask.
 * @param mask IRQ mask specifying which IRQs to set the callback for.
 * @param callback Function to call when the IRQ occurs.
 */
void OS_SetIRQCallback(u32 mask, OS_IRQCallback callback);

/**
 * @brief Gets the IRQ callback for the given mask.
 * @param mask IRQ mask specifying which IRQ to get the callback for.
 * @return The callback function associated with the specified IRQ, or NULL if none is set.
 */
OS_IRQCallback OS_GetIRQCallback(u32 mask);

/**
 * @brief Sets the DMA interrupt callback for the specified DMA channel.
 * @param dmaNumber DMA channel number (0-3).
 * @param callback Function to call when the DMA interrupt occurs.
 * @param arg Argument to pass to the callback function.
 */
void OS_SetDMAInterruptCallback(u32 dmaNumber, void (*callback)(void*), void* arg);

/**
 * @brief Sets the Timer interrupt callback for the specified timer.
 * @param timerNumber Timer number (0-3).
 * @param callback Function to call when the Timer interrupt occurs.
 * @param arg Argument to pass to the callback function.
 */
void OS_SetTimerInterruptCallback(u32 timerNumber, void (*callback)(void*), void* arg);

/**
 * @brief Sets the IRQ mask (enabled IRQs).
 * @param mask IRQ mask to set.
 * @return Previous IRQ mask value.
 */
OS_IRQMask OS_SetIRQMask(OS_IRQMask mask);

/**
 * @brief Enables interrupts specified by the mask.
 * @param mask IRQ mask to enable.
 * @return Previous IRQ mask value.
 */
OS_IRQMask OS_EnableInterrupts(OS_IRQMask mask);

/**
 * @brief Disables interrupts specified by the mask.
 * @param mask IRQ mask to disable.
 * @return Previous IRQ mask value.
 */
OS_IRQMask OS_DisableInterrupts(OS_IRQMask mask);

/**
 * @brief Resets IRQ request flags for the given mask.
 * @param mask IRQ mask to reset flags for.
 * @return Previous IRQ request flags value.
 */
OS_IRQMask OS_ResetIRQRequestFlags(OS_IRQMask mask);

/// MARK: Inlines

static inline BOOL OS_DisableIME(void) {
    u16 prev = REG_IME;
    REG_IME  = 0;
    return (BOOL)prev;
}

static inline BOOL OS_RestoreIME(BOOL enable) {
    u16 prev = REG_IME;
    REG_IME  = (vu16)enable;
    return (BOOL)prev;
}

#endif // NITROSDK_OS_INTERRUPTS_H