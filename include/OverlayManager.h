#ifndef OVERLAY_MANAGER_H
#define OVERLAY_MANAGER_H

#include <types.h>

#define OVERLAY_ID_NONE     (-0x7FFFFFFF)
#define OVERLAY_ID_UNLOADED 0x80000000
extern s32 OVERLAY_0_ID;
extern u32 OVERLAY_2_ID;
extern u32 OVERLAY_3_ID;
extern u32 OVERLAY_30_ID;
extern u32 OVERLAY_31_ID;
extern u32 OVERLAY_37_ID;
extern u32 OVERLAY_39_ID;
extern s32 OVERLAY_40_ID;
extern u32 OVERLAY_43_ID;
extern u32 OVERLAY_44_ID;
extern u32 OVERLAY_46_ID;

/**
 * @brief Initialize the overlay manager state.
 *
 * Sets all overlay slots to the sentinel value indicating they are unloaded.
 */
void OvlMgr_Init(void);

/**
 * @brief Unload every overlay tracked by the manager.
 */
void OvlMgr_UnloadAllOverlays(void);

/**
 * @brief Unload a specific overlay slot, or all slots if a negative index is given.
 *
 * @param slot Overlay slot index. Pass a negative value to unload all slots.
 */
void OvlMgr_UnloadOverlay(s32 slot);

/**
 * @brief Load an overlay into a slot.
 *
 * If the requested overlay differs from the current one and is in range, the
 * previous overlay in the slot is unloaded, the new overlay is loaded, and the
 * slot is updated.
 *
 * @param slot Overlay slot to use.
 * @param newOverlay Overlay ID to load.
 * @return The overlay ID that was previously in the slot.
 */
u32 OvlMgr_LoadOverlay(s32 slot, u32 newOverlay);

/**
 * @brief Check whether an overlay ID is marked as loaded.
 *
 * Searches all managed slots for the given overlay ID.
 *
 * @param overlayID Overlay ID to query.
 * @return TRUE if the overlay is considered loaded; otherwise FALSE.
 */
BOOL OvlMgr_IsOverlayLoaded(u32 overlayID);

#endif