/**
 * @file bios.h
 * @brief Contains definitions and declarations for BIOS functions and structures.
 */

#ifndef NITROSDK_BIOS_H
#define NITROSDK_BIOS_H

#include "types.h"

#define BIOS_FRAME_COUNTER *(u32*)0x027FFC3C
#define BIOS_BOOT_MODE     *(u16*)0x027FFC40 // 1 = Normal Boot, 2 = Booted through DS Download Play via WiFi

#endif // NITROSDK_BIOS_H