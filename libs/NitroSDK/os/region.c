#include <NitroSDK/os/interrupt.h>
#include <NitroSDK/os/region.h>
#include <bios.h>

#define ALIGN(ptr, align) (((u32)ptr + align - 1) & ~(align - 1))

static BOOL RegionSystemInitialized = FALSE;
static BOOL ExtendedRegionEnabled   = FALSE;

void OS_RegionsInit(void) {
    if (RegionSystemInitialized) {
        return;
    }
    RegionSystemInitialized = TRUE;

    OS_InitializeRegion(REGION_MAIN_ARM9);
    OS_SetRegionBottom(REGION_EXTENDED, NULL);
    OS_SetRegionTop(REGION_EXTENDED, NULL);
    OS_InitializeRegion(REGION_ITCM);
    OS_InitializeRegion(REGION_DTCM);
    OS_InitializeRegion(REGION_SHARED);
    OS_InitializeRegion(REGION_WORK_ARM9);
}

void OS_ExtendedRegionInit(void) {
    OS_InitializeRegion(REGION_EXTENDED);

    if (ExtendedRegionEnabled == FALSE || (func_02039380() & 3) == 1) {
        func_0203a104(0x200002B);
        func_0203a10c(0x23E0021);
    }
}

void* OS_GetRegionTop(Region region) {
    return REGION_INFO.regionTop[region];
}

void* OS_GetRegionBottom(Region region) {
    return REGION_INFO.regionBottom[region];
}

// Nonmatching: missing unreachable portion in case 4
static void* OS_GetDefaultRegionTop(Region region) {
    switch (region) {
        case 0:
            return (void*)0x023E0000;
        case 2:
            if (ExtendedRegionEnabled == FALSE || (func_02039380() & 3) == 1) {
                return NULL;
            }
            return (void*)0x02700000;
        case 3:
            return (void*)0x02000000;
        case 4: // wrong
            return (void*)0x027E0780;
        case 5:
            return (void*)0x027FF680;
        case 6:
            return (void*)0x037F8000;
        default:
            return NULL;
    }
}

static void* OS_GetDefaultRegionBottom(Region region) {
    switch (region) {
        case 0:
            return (void*)0x02143A80;
        case 2:
            if (ExtendedRegionEnabled == FALSE || (func_02039380() & 3) == 1) {
                return NULL;
            }
            return (void*)0x023E0000;
        case 3:
            return (void*)0x01FF8580;
        case 4:
            return (void*)0x027E0080;
        case 5:
            return (void*)0x027FF000;
        case 6:
            return (void*)0x037F8000;
        case 1:
        default:
            return NULL;
    }
}

void OS_SetRegionTop(Region region, void* top) {
    REGION_INFO.regionTop[region] = top;
}

void OS_SetRegionBottom(Region region, void* bottom) {
    REGION_INFO.regionBottom[region] = bottom;
}

void* OS_AllocRegionMemory(Region region, u32 size, u32 align) {
    void* bottom = OS_GetRegionBottom(region);

    if (bottom == NULL) {
        return NULL;
    }

    void* alignedBottom = bottom = ALIGN(bottom, align);
    alignedBottom += size;
    alignedBottom = ALIGN(alignedBottom, align);

    if (alignedBottom > OS_GetRegionTop(region)) {
        return NULL;
    }
    OS_SetRegionBottom(region, alignedBottom);
    return bottom;
}
