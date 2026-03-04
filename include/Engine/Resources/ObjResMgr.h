#ifndef ENGINE_RESOURCES_OBJRESMGR_H
#define ENGINE_RESOURCES_OBJRESMGR_H

#include "Display.h"

/**
 * @brief Single object resource entry managed by ObjResMgr.
 *
 * Tracks allocation metadata, reference counting, transfer behavior, and
 * backing source data for one VRAM-resident object resource.
 */
typedef struct ObjResource {
    /* 0x00 */ struct ObjResource* next;
    /* 0x04 */ u16                 flags;
    /* 0x06 */ u16                 refCount;
    /* 0x08 */ struct {
        u16 mode          : 1;
        u16 skipTransfer  : 1;
        u16 unk_02        : 1;
        u16 hasSourceData : 1;
        u16 _reserved     : 12;
    };
    /* 0x0A */ u16   unk_0A;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ s16   bitmapIndex;
    /* 0x12 */ s16   unk_12;
    /* 0x14 */ s32   vramOffset;
    /* 0x18 */ void* unk_18;
    /* 0x1C */ s32   unk_1C;
} ObjResource; // size 0x20

/**
 * @brief Bitmap allocator state for object resource VRAM.
 *
 * Uses a signed-size run list stored in @ref slots to represent used/free
 * regions for allocation from either start or end.
 */
typedef struct ObjResBitmap {
    u32 mode;          // 0x00
    u8  _pad04[0x1C];  // 0x04-0x1F
    s16 slots[0x1010]; // 0x20
} ObjResBitmap;        // size 0x2040

/**
 * @brief Object resource manager for one display engine domain.
 *
 * Owns allocation state, active/free resource lists, and an internal pool of
 * @ref ObjResource nodes.
 */
typedef struct ObjResMgr {
    DisplayEngine engine;       // 0x00
    s32           unk_04;       // 0x04
    u32           vramBase;     // 0x08
    u8            _pad0C[0x14]; // 0x0C-0x1F
    ObjResource*  activeList;   // 0x20
    u8            _pad24[0x1C]; // 0x24-0x3F
    ObjResource*  freeList;     // 0x40
    u8            _pad44[0x1C]; // 0x44-0x5F
    ObjResource   pool[0x80];   // 0x60
    ObjResBitmap  bitmap;       // 0x1060
} ObjResMgr;                    // Size: 0x30A0

/**
 * @brief Global object resource managers, indexed by display engine.
 */
extern ObjResMgr* g_ObjResourceManagers[3];

/**
 * @brief Initialize an object resource manager for a display engine.
 *
 * Configures VRAM base, allocation granularity, free list population, and
 * bitmap allocator state based on current VRAM bank settings.
 *
 * @param mgr Manager instance to initialize.
 * @param engine Target display engine this manager services.
 */
void ObjResMgr_Init(ObjResMgr* mgr, DisplayEngine engine);

/**
 * @brief Allocate or reuse an object resource entry.
 *
 * When possible, reuses an already active resource that matches source data
 * and mode, increasing its reference count. Otherwise allocates VRAM and
 * creates a new active resource entry.
 *
 * @param mgr Manager instance.
 * @param data Source data pointer (format-dependent header/content).
 * @param size Payload size in bytes; if 0, size is inferred from @p data.
 * @param mode Allocation mode (front/back allocator path).
 * @return Pointer to allocated/reused resource, or NULL on failure.
 */
ObjResource* ObjResMgr_AllocResource(ObjResMgr* mgr, void* data, u32 size, u32 mode);

/**
 * @brief Release a resource reference and free it when count reaches zero.
 *
 * @param mgr Manager instance.
 * @param resource Resource to release.
 * @return TRUE if the resource was fully released and returned to free pool,
 *         FALSE otherwise.
 */
BOOL ObjResMgr_ReleaseResource(ObjResMgr* mgr, ObjResource* resource);

/**
 * @brief Transfer source data into resource VRAM space.
 *
 * Performs format-dependent copy/decode flow and stores transfer provenance in
 * the resource metadata.
 *
 * @param mgr Manager instance.
 * @param resource Destination resource entry.
 * @param data Source data pointer.
 * @param mode Transfer mode selector.
 */
void ObjResMgr_LoadToVram(ObjResMgr* mgr, ObjResource* resource, void* data, s32 mode);

#endif // ENGINE_RESOURCES_OBJRESMGR_H