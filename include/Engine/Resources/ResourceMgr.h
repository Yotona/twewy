#ifndef ENGINE_RESOURCES_RESOURCEMGR_H
#define ENGINE_RESOURCES_RESOURCEMGR_H

#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/ObjResMgr.h"
#include "Engine/Resources/PaletteMgr.h"

/**
 * @brief Common header shared by background, object, and palette resources.
 */
typedef struct {
    /* 0x00 */ void* next;
    /* 0x04 */ u16   flags;
    /* 0x06 */ u16   refCount;
} Resource;

/**
 * @brief Aggregate of all per-engine resource managers used by one scene/context.
 */
typedef struct {
    /* 0x0000 */ BgResMgr   mainBgMgr;
    /* 0x1C80 */ BgResMgr   subBgMgr;
    /* 0x3900 */ ObjResMgr  objResMgr0;
    /* 0x69A0 */ ObjResMgr  objResMgr1;
    /* 0x9A40 */ ObjResMgr  objResMgr2;
    /* 0xCAE0 */ PaletteMgr palMgr0;
    /* 0xE3C0 */ PaletteMgr palMgr1;
    /* 0xFCA0 */ PaletteMgr palMgr2;
} ResourceManager; // Size: 0x11580

/**
 * @brief Reinitializes global manager pointers using the provided aggregate manager.
 *
 * This always performs initialization calls for all background/object/palette managers,
 * either with the corresponding fields from @p mgr or with @c NULL.
 *
 * @param mgr Resource manager aggregate to activate, or @c NULL to clear managers.
 * @return Previously active aggregate manager pointer.
 */
ResourceManager* ResourceMgr_ReinitManagers(ResourceManager* mgr);

/**
 * @brief Sets the active aggregate manager and updates global manager tables.
 *
 * If @p mgr matches the current active manager, this function returns immediately.
 * When @p mgr is @c NULL, all global manager slots are reset through init calls.
 *
 * @param mgr Resource manager aggregate to activate, or @c NULL to clear managers.
 * @return Previously active aggregate manager pointer.
 */
ResourceManager* ResourceMgr_SetActiveManagers(ResourceManager* mgr);

/**
 * @brief Releases a resource through the appropriate manager based on encoded flags.
 *
 * The display engine is selected from bits in @c resource->flags, and the resource type
 * (BG char/screen, OBJ, palette) is selected from the low three bits.
 *
 * @param resource Resource instance to release.
 */
void ResourceMgr_ReleaseResource(Resource* resource);

#endif // ENGINE_RESOURCES_RESOURCEMGR_H