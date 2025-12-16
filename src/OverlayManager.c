#include "OverlayManager.h"
#include "System.h"
#include <NitroSDK/fs/overlay.h>

void OvlMgr_UnloadOverlay(s32 idx);

void OvlMgr_Init(void) {
    for (u32 i = 0; i < 6; i++) {
        SysControl.loadedOverlays[i] = 0x80000001;
    }
}

void OvlMgr_UnloadAllOverlays(void) {
    OvlMgr_UnloadOverlay(-1);
}

// Nonmatching: Unresolvable overlay ID constant
// Scratch: D0ztp
void OvlMgr_UnloadOverlay(s32 slot) {
    u32 currentOverlayId;

    if (slot < 0) {
        for (u32 i = 0; i < 6; i++) {
            currentOverlayId = SysControl.loadedOverlays[i];
            if (currentOverlayId < 48) {
                FS_UnloadOverlay(0, currentOverlayId);
            }
            SysControl.loadedOverlays[i] = 0x80000000;
        }
    } else {
        currentOverlayId = SysControl.loadedOverlays[slot];
        if (currentOverlayId < 0x80000000) {
            FS_UnloadOverlay(0, currentOverlayId);
        }
        SysControl.loadedOverlays[slot] = 0x80000001;
    }
}

// Nonmatching: Unresolvable overlay ID constant
// Scratch: vak4k
u32 OvlMgr_LoadOverlay(s32 slot, u32 newOverlay) {
    u32 currOverlay = SysControl.loadedOverlays[slot];
    if ((newOverlay < 48) && (newOverlay != currOverlay)) {
        if (currOverlay < 48) {
            FS_UnloadOverlay(0, currOverlay);
        }
        FS_LoadOverlay(0, newOverlay);
        SysControl.loadedOverlays[slot] = newOverlay;
    }
    return currOverlay;
}

BOOL OvlMgr_IsOverlayLoaded(u32 overlayID) {
    if (overlayID >= 0x80000000) {
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
