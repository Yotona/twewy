#ifndef ENGINE_RESOURCES_PALETTEMGR_H
#define ENGINE_RESOURCES_PALETTEMGR_H

#include "Display.h"

/**
 * @brief Palette slot allocation metadata.
 */
typedef struct {
    /* 0x0 */ s16 index; /**< Slot start index, or -1 when free. */
    /* 0x2 */ u16 flags; /**< Bitmask describing ownership/use of the slot. */
    /* 0x4 */ u32 pad;   /**< Reserved/padding. */
} PalSlot;               // size 0x08

/**
 * @brief Runtime record for one allocated palette resource.
 */
typedef struct PaletteResource {
    /* 0x00 */ struct PaletteResource* next;     /**< Next node in linked list. */
    /* 0x04 */ u16                     flags;    /**< Resource state/category flags. */
    /* 0x06 */ u16                     refCount; /**< Reference count for shared resources. */
    /* 0x08 */ u32                     unk_08;   /**< Unknown field (preserved). */
    /* 0x0C */ struct {
        u32 slotType   : 3;                      /**< Slot class (0-6). */
        u32 chunkSize  : 5;                      /**< Size unit per palette chunk. */
        u32 chunkCount : 5;                      /**< Number of chunks (contiguous mode). */
        u32 colorParam : 6;                      /**< Processing parameter for color effects. */
        u32 _pad0C     : 13;                     /**< Reserved bits. */
    };
    /* 0x10 */ s16   palIndex;                   /**< Palette index within the selected slot type. */
    /* 0x12 */ u16   unk_12;                     /**< Allocation mask (or 0xFFFF for contiguous). */
    /* 0x14 */ s16   colorBias;                  /**< Signed brightness bias used by processors. */
    /* 0x16 */ u16   processMode;                /**< Current palette processing mode. */
    /* 0x18 */ void* sourcePalette;              /**< Source palette pointer. */
    /* 0x1C */ void* blendPalette;               /**< Optional secondary/blend palette pointer. */
} PaletteResource;                               // size 0x20

/**
 * @brief Palette manager state for one display engine.
 */
typedef struct PaletteMgr {
    DisplayEngine    engine;       /**< Target display engine. */
    void*            unk_04;       /**< Base pointer table for BG/OBJ destination buffers. */
    u32              unk_08;       /**< VRAM base for main BG ext palette bank A. */
    u32              unk_0C;       /**< VRAM base for main BG ext palette bank B. */
    u32              unk_10;       /**< VRAM base for sub/alt BG ext palette bank A. */
    u32              unk_14;       /**< VRAM base for sub/alt BG ext palette bank B. */
    void*            unk_18;       /**< Base pointer table for OBJ destination buffers. */
    void*            unk_1C;       /**< VRAM/indirect destination for OBJ palettes. */
    PaletteResource* activeList;   /**< Singly-linked list of active resources. */
    u8               _pad24[0x1C]; /**< Reserved/padding. */
    PaletteResource* freeList;     /**< Singly-linked list of free resources. */
    u8               _pad44[0x1C]; /**< Reserved/padding. */
    PaletteResource  pool[168];    /**< Fixed pool backing all palette resources. */
    PalSlot          slots[7][16]; /**< Slot trackers per slot type. */
} PaletteMgr;

/**
 * @brief Direct palette processing function type.
 */
typedef void (*PalLoadFunc)(PaletteResource*, void*, s32, u32);

/**
 * @brief Indirect (buffered/channel) palette processing function type.
 */
typedef void (*PalLoadIndirectFunc)(PaletteResource*, void*, u32, s32, s32);

/**
 * @brief Global palette manager table indexed by DisplayEngine.
 */
extern PaletteMgr* g_PaletteManagers[3];

/**
 * @brief Initialize a palette manager instance for a display engine.
 *
 * @param mgr    Pointer to manager storage.
 * @param engine Display engine to bind this manager to.
 */
void PaletteMgr_Init(PaletteMgr* mgr, DisplayEngine engine);

/**
 * @brief Acquire a resource using contiguous slot allocation.
 *
 * Reuses an existing active resource when source/count match and reference
 * counting permits. Otherwise, allocates a new resource with the specified source and count.
 *
 * @param mgr      Palette manager.
 * @param source   Source palette pointer (may be NULL).
 * @param slotType Slot class (0-6).
 * @param count    Number of chunks to allocate.
 * @return         Allocated/reused resource, or NULL on failure.
 */
PaletteResource* PaletteMgr_AcquireContiguous(PaletteMgr* mgr, void* source, u16 slotType, u32 count);

/**
 * @brief Acquire a resource using masked slot allocation.
 *
 * Reuses an existing active resource when source/mask match and reference
 * counting permits. Otherwise, allocates a new resource with the specified source and mask.
 *
 * @param mgr      Palette manager.
 * @param source   Source palette pointer (may be NULL).
 * @param slotType Slot class (0-6).
 * @param mask     16-bit slot mask for masked allocation.
 * @return         Allocated/reused resource, or NULL on failure.
 */
PaletteResource* PaletteMgr_AcquireMasked(PaletteMgr* mgr, void* source, u16 slotType, s16 mask);

/**
 * @brief Allocate palette data in VRAM.
 *
 * @param mgr           Pointer to the palette manager
 * @param sourcePalette Pointer to the palette data to load
 * @param slotType      Type of palette slot to allocate (0-6)
 * @param start         Starting index for allocation (if applicable)
 * @param count         Number of palette entries to allocate
 * @return              Pointer to the allocated PaletteResource, or NULL on failure
 */
PaletteResource* PaletteMgr_AllocPalette(PaletteMgr* mgr, void* sourcePalette, u16 slotType, s16 start, s32 count);

/**
 * @brief Release a palette resource, freeing its slot and allowing reuse.
 *
 * @param mgr      Pointer to the palette manager.
 * @param resource Pointer to the resource to release.
 * @return         TRUE if the resource was successfully released, FALSE otherwise.
 */
BOOL PaletteMgr_ReleaseResource(PaletteMgr* mgr, PaletteResource* resource);

/**
 * @brief Flush palette resource data to palette memory/transfer channels.
 *
 * - If @p mgr is NULL, the function returns immediately.
 *
 * - If @p resource is non-NULL, that resource is forcibly marked dirty, then
 *   processed immediately (single-resource path).
 *
 * - If @p resource is NULL, the function walks the active resource list and
 *   processes only entries already marked dirty (batch path).
 *
 * @param mgr      Pointer to the palette manager.
 * @param resource Pointer to one resource to force-flush, or NULL to flush all
 *                 currently dirty active resources.
 */
void PaletteMgr_Flush(PaletteMgr* mgr, PaletteResource* resource);

/**
 * @brief Set or replace the source palette pointer for a resource.
 *
 * Marks the resource dirty when the source changes to non-NULL.
 *
 * @param mgr           Pointer to the palette manager.
 * @param resource      Pointer to the target resource.
 * @param sourcePalette New source palette pointer.
 */
void PaletteMgr_SetSource(PaletteMgr* mgr, PaletteResource* resource, void* sourcePalette);

/**
 * @brief Configure processing mode for one resource or a filtered set.
 *
 * Mode 1 uses blend processing; other modes use non-blend processing.
 *
 * @param mgr        Pointer to the palette manager.
 * @param resource   Specific resource, or NULL to apply to filtered active resources.
 * @param filter     Filter mask selecting applicable resource classes (0 uses default mask).
 * @param mode       Processing mode index.
 * @param colorParam Mode-dependent color parameter.
 * @param blend      Optional blend palette pointer for blend mode.
 */
void PaletteMgr_SetProcess(PaletteMgr* mgr, PaletteResource* resource, s32 filter, s32 mode, s32 colorParam, void* blend);

/**
 * @brief Set brightness bias for one resource or a filtered set.
 *
 * @param mgr       Palette manager.
 * @param resource  Specific resource, or NULL to apply to filtered active resources.
 * @param filter    Filter mask selecting applicable resource classes (0 uses default mask).
 * @param colorBias Signed bias value.
 */
void PaletteMgr_SetColorBias(PaletteMgr* mgr, PaletteResource* resource, s32 filter, s16 colorBias);

/**
 * @brief Effectively does nothing. Only contains a call to an empty function and an empty loop.
 */
void PaletteMgr_UnknownLoop(PaletteMgr* mgr);

#endif // ENGINE_RESOURCES_PALETTEMGR_H