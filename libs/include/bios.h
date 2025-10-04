/**
 * @file bios.h
 * @brief Contains definitions and declarations for BIOS functions and structures.
 */

#ifndef BIOS_H
#define BIOS_H

#include "types.h"

/// Interrupts
// Nonmatching: Every usage seems to be an offset read from dtcm base

#define BIOS_INTERRUPT_IRQSTACK_TOP    *(vu32*)0x027E3780
#define BIOS_INTERRUPT_IRQSTACK_BOTTOM *(vu32*)0x027E3F7C
#define BIOS_INTERRUPT_IRQCHECK        *(vu32*)0x027E3FF8

/// System Control/Status

#define BIOS_RESET         *(u32*)0x027FFC20
#define BIOS_FRAME_COUNTER *(u32*)0x027FFC3C
#define BIOS_BOOT_MODE     *(u16*)0x027FFC40 // 1 = Normal Boot, 2 = Booted through DS Download Play via WiFi

/**
 * @brief Checks if the system was booted through DS Download Play via WiFi.
 * @return TRUE if booted through Download Play, FALSE otherwise.
 */
static inline BOOL BIOS_IsDownloadPlay(void) {
    return BIOS_BOOT_MODE == 2;
}

/// System Buttons/Sensors

#define BIOS_EXTRA_BUTTONS *(vu16*)0x027FFFA8

#endif // BIOS_H