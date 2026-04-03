#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Core/System.h"
#include <NitroSDK/fs/overlay.h>

void OvlMgr_UnloadOverlay(s32 idx);

void OvlMgr_Init(void) {
    for (u32 i = 0; i < 6; i++) {
        SysControl.loadedOverlays[i] = OVERLAY_ID_NONE;
    }
}

void OvlMgr_UnloadAllOverlays(void) {
    OvlMgr_UnloadOverlay(-1);
}

void OvlMgr_UnloadOverlay(s32 slot) {
    if (slot < 0) {
        u32 maxOverlay = (u32)__OVERLAY_COUNT;

        for (u32 i = 0; i < 6; i++) {
            if (SysControl.loadedOverlays[i] < maxOverlay) {
                FS_UnloadOverlay(0, SysControl.loadedOverlays[i]);
            }
            SysControl.loadedOverlays[i] = OVERLAY_ID_UNLOADED;
        }
    } else {
        if (SysControl.loadedOverlays[slot] < OVERLAY_ID_UNLOADED) {
            FS_UnloadOverlay(0, SysControl.loadedOverlays[slot]);
        }
        SysControl.loadedOverlays[slot] = OVERLAY_ID_NONE;
    }
}

u32 OvlMgr_LoadOverlay(s32 slot, u32 newOverlay) {
    u32 currOverlay = SysControl.loadedOverlays[slot];
    if ((newOverlay < (u32)__OVERLAY_COUNT) && (newOverlay != currOverlay)) {
        if (currOverlay < (u32)__OVERLAY_COUNT) {
            FS_UnloadOverlay(0, currOverlay);
        }
        FS_LoadOverlay(0, newOverlay);
        SysControl.loadedOverlays[slot] = newOverlay;
    }
    return currOverlay;
}

BOOL OvlMgr_IsOverlayLoaded(u32 overlayID) {
    if (overlayID >= OVERLAY_ID_UNLOADED) {
        return TRUE;
    }

    BOOL found = FALSE;
    for (u32 i = 0; i < 6; i++) {
        if (SysControl.loadedOverlays[i] == overlayID) {
            found = TRUE;
            break;
        }
    }
    return found;
}
