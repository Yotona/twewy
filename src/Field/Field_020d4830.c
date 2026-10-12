#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Save.h"
#include "SndMgr.h"
#include "common_data.h"

s32 func_ov030_020d483c(void) {
    s32 value = gSaveData.unk_3124;

    if (value == 0x2f) {
        return TRUE;
    }
    if (value == 0x30) {
        return TRUE;
    }
    return FALSE;
}

s32 func_ov030_020d4864(void) {
    return gSaveData.unk_3124 == 0x31;
}

extern void func_020218ec(void);

void func_ov030_020d49dc(void) {
    SndMgr_Finalize();
    if (SystemStatusFlags.unk_03) {
        func_020218ec();
    }
    SystemStatusFlags.unk_03 = FALSE;
}

extern char data_ov030_02109bc8[16];
char*       data_ov030_02109bc4     = data_ov030_02109bc8;
char        data_ov030_02109bc8[16] = "Seq_FlashSel()\0";
char        data_ov030_02109bd8[16] = "Flash select";
char        data_ov030_02109be8[12] = "flash:%d %s";
char        data_ov030_02109bf4[12] = "flash:%d";

typedef struct {
    ResourceManager  resources;
    ResourceManager* previousResources;
    s32              dataSlot;
} FlashSelectOverlayState;

extern void func_ov030_020d57c8(void);
extern void func_ov030_020d5100(FlashSelectOverlayState* state);

// Nonmatching: store/address-load scheduling of the final zero initialisation
void func_ov030_020d51fc(void) {
    FlashSelectOverlayState* state;
    const char*              sequence = data_ov030_02109bc4;

    state = Mem_AllocHeapTail(&gDebugHeap, 0x219B0);
    Mem_SetSequence(&gDebugHeap, state, sequence);
    MainOvlDisp_SetCbArg(state);
    state->dataSlot = DatMgr_AllocateSlot();
    if (gSaveData.unk_3124 >= 0x53) {
        gSaveData.unk_3124 = 0;
    }
    func_ov030_020d57c8();
    g_DisplaySettings.controls[0].layers     = 0;
    state->previousResources                 = ResourceMgr_ReinitManagers(&state->resources);
    data_02066aec                            = 0;
    data_02066eec                            = 0;
    g_DisplaySettings.controls[0].brightness = 0;
    g_DisplaySettings.controls[1].brightness = 0;
    func_ov030_020d5100(state);
    MainOvlDisp_NextProcessStage();
}
