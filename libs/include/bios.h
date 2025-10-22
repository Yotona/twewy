/**
 * @file bios.h
 * @brief Contains definitions and declarations for BIOS functions and structures.
 */

#ifndef BIOS_H
#define BIOS_H

#include "types.h"

#define BIOS_LAUNCHTHREAD_STACK_BOTTOM *(u32*)0x027E0780
#define BIOS_LAUNCHTHREAD_STACK_TOP    *(u32*)0x027E3780

/// Interrupts
// Nonmatching: Every usage seems to be an offset read from dtcm base

#define BIOS_INTERRUPT_IRQSTACK_TOP    *(vu32*)0x027E3780
#define BIOS_INTERRUPT_IRQSTACK_BOTTOM *(vu32*)0x027E3F7C
#define BIOS_INTERRUPT_IRQCHECK        *(vu32*)0x027E3FF8

/// System Control/Status

#define BIOS_RESET         *(u32*)0x027FFC20
#define BIOS_FRAME_COUNTER *(u32*)0x027FFC3C
#define BIOS_BOOT_MODE     *(u16*)0x027FFC40 // 1 = Normal Boot, 2 = Booted through DS Download Play via WiFi

#define BIOS_MAIN_THREAD_INFO *(u32*)0x027FFFA0

/**
 * @brief Checks if the system was booted through DS Download Play via WiFi.
 * @return TRUE if booted through Download Play, FALSE otherwise.
 */
static inline BOOL BIOS_IsDownloadPlay(void) {
    return BIOS_BOOT_MODE == 2;
}

/// Input

#define BIOS_EXTRA_BUTTONS *(vu16*)0x027FFFA8

/// Cartridge Header

typedef struct {
    /* 0x0 */ u32 offset;
    /* 0x4 */ u32 size;
} CartridgeRegion;

#define CARTHEADER_FNT      0x027FFE40
#define CARTHEADER_FAT      0x027FFE48
#define CARTHEADER_ARM9_OVL 0x027FFE50
#define CARTHEADER_ARM7_OVL 0x027FFE58

static inline const CartridgeRegion* Cart_GetFileNameTable(void) {
    return (const CartridgeRegion*)((const u8*)CARTHEADER_FNT);
}

static inline const CartridgeRegion* Cart_GetFileAllocTable(void) {
    return (const CartridgeRegion*)((const u8*)CARTHEADER_FAT);
}

static inline const CartridgeRegion* Cart_GetArm9OverlayTable(void) {
    return (const CartridgeRegion*)((const u8*)CARTHEADER_ARM9_OVL);
}

static inline const CartridgeRegion* Cart_GetArm7OverlayTable(void) {
    return (const CartridgeRegion*)((const u8*)CARTHEADER_ARM7_OVL);
}

#endif // BIOS_H