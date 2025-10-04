#ifndef NITROSDK_OS_ARENA_H
#define NITROSDK_OS_ARENA_H

#include <types.h>

/// MARK: Typedefs

typedef enum {
    REGION_MAIN_ARM9 = 0,
    REGION_MAIN_ARM7 = 1,
    REGION_EXTENDED  = 2,
    REGION_ITCM      = 3,
    REGION_DTCM      = 4,
    REGION_SHARED    = 5,
    REGION_WORK_ARM9 = 6,
    REGION_WORK_ARM7 = 7,
    REGION_UNKNOWN_8 = 8
} Region;

typedef struct {
    /* 0x00 */ void* regionBottom[9];
    /* 0x24 */ void* regionTop[9];
} RegionInfo;

#define REGION_INFO (*(RegionInfo*)0x027FFDA0)

/// MARK: Functions

/**
 * @brief Initialize each memory region
 */
void OS_RegionsInit(void);

/**
 * @brief Initialize the extended memory region
 */
void OS_ExtendedRegionInit(void);

/**
 * @brief Get the top address of a memory region
 * @param region Region ID
 * @return Top address of the specified region
 */
void* OS_GetRegionTop(Region region);

/**
 * @brief Get the bottom address of a memory region
 * @param region Region ID
 * @return Bottom address of the specified region
 */
void* OS_GetRegionBottom(Region region);

/**
 * @brief Get the default top address of a memory region
 */
void* OS_GetDefaultRegionTop(Region region);

/**
 * @brief Get the default bottom address of a memory region
 */
void* OS_GetDefaultRegionBottom(Region region);

/**
 * @brief Assign the top address of a memory region
 */
void OS_SetRegionTop(Region region, void* top);

/**
 * @brief Assign the bottom address of a memory region
 */
void OS_SetRegionBottom(Region region, void* bottom);

/**
 * @brief Allocate memory from a memory region
 * @param region Region ID
 * @param size Size to allocate
 * @param align Alignment of the allocated address
 * @return Allocated address, or NULL if allocation failed
 */
void* OS_AllocRegionMemory(Region region, u32 size, u32 align);

/// MARK: Inlines

static inline void OS_InitializeRegionTop(Region region) {
    OS_SetRegionTop(region, OS_GetDefaultRegionTop(region));
}

static inline void OS_InitializeRegionBottom(Region region) {
    OS_SetRegionBottom(region, OS_GetDefaultRegionBottom(region));
}

static inline void OS_InitializeRegion(Region region) {
    OS_InitializeRegionTop(region);
    OS_InitializeRegionBottom(region);
}

#endif // NITROSDK_OS_ARENA_H