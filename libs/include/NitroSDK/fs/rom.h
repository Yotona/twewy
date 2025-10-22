#ifndef NITROSDK_FS_ROM_H
#define NITROSDK_FS_ROM_H

#include <bios.h>
#include <types.h>

extern CartridgeRegion Arm7OverlayTable;
extern CartridgeRegion Arm9OverlayTable;

extern FS_Record Rom_DefaultRecord;

/**
 * @brief Loads the ROM file allocation and name tables into memory if sufficient space is provided
 *
 * @param buf Pointer to memory where the tables will be loaded
 * @param size Size of the memory buffer. If insufficient, only returns required size
 *
 * @return The size of memory used by FAT and FNT
 */
u32 FS_RomLoadDefaultTables(void* buf, u32 size);

#endif // NITROSDK_FS_ROM_H