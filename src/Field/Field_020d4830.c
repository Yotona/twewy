#include "Engine/Core/System.h"
#include "Save.h"
#include "SndMgr.h"

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
