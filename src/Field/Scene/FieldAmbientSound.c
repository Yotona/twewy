// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020c2ccc-0x020c2e00; 4 functions.
// Research draft: docs/decomp/ov030/Field_020c2ccc.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Save.h"
#include "SndMgr.h"

extern s32 func_ov030_020ac254(void);
extern s32 data_ov030_02109d7c;

s32 func_ov030_020c2ccc(s32 arg0) {
    s32 value = arg0 - arg0 * func_ov030_020ac254() / 100;
    s32 result;

    if (value < 5) {
        result = 0x56C;
    } else if (value < 0x14) {
        result = 0x104;
    } else if (value < 0x28) {
        result = 0x105;
    } else {
        result = 0x106;
    }
    if (gSaveData.unk_2458 == 15 || gSaveData.unk_2458 == 18) {
        return 0x107;
    }
    if (gSaveData.unk_2458 == 41) {
        result = 0x110;
    }
    return result;
}

void func_ov030_020c2d50(s32 arg0) {
    s32 sound = func_ov030_020c2ccc(arg0);

    if (data_ov030_02109d7c == sound) {
        return;
    }
    if (sound == 0x56C) {
        SndMgr_StopPlayingSE(data_ov030_02109d7c);
    } else {
        if (data_ov030_02109d7c == 0x107) {
            SndMgr_StopPlayingSE(0x107);
        }
        SndMgr_StartPlayingSE(sound);
    }
    data_ov030_02109d7c = sound;
}

void func_ov030_020c2db4(void) {
    if (data_ov030_02109d7c != 0x56C) {
        SndMgr_StopPlayingSE(data_ov030_02109d7c);
    }
    data_ov030_02109d7c = 0x56C;
}

void func_ov030_020c2de8(void) {
    data_ov030_02109d7c = 0x56C;
}
