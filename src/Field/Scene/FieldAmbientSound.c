#include "Save.h"
#include "SndMgr.h"

extern s32 func_ov030_020ac254(void);

SndMgrSeIdx data_ov030_02109d7c;

s32 func_ov030_020c2ccc(s32 arg0) {
    s32         value = arg0 - arg0 * func_ov030_020ac254() / 100;
    SndMgrSeIdx seIdx;

    if (value < 5) {
        seIdx = SEIDX_COUNT;
    } else if (value < 20) {
        seIdx = SEIDX_BG_CITY_AMBIS;
    } else if (value < 40) {
        seIdx = SEIDX_BG_CITY_AMBIM;
    } else {
        seIdx = SEIDX_BG_CITY_AMBIL;
    }
    if (gSaveData.fieldArea == 15 || gSaveData.fieldArea == 18) {
        return SEIDX_BG_CITY_TRAIN;
    }
    if (gSaveData.fieldArea == 41) {
        seIdx = SEIDX_BG_CITY_WATER_ROOM;
    }
    return seIdx;
}

void func_ov030_020c2d50(s32 arg0) {
    s32 sound = func_ov030_020c2ccc(arg0);

    if (data_ov030_02109d7c != sound) {
        if (sound == SEIDX_DISABLESOUNDEFFECTS) {
            SndMgr_StopPlayingSE(data_ov030_02109d7c);
        } else {
            if (data_ov030_02109d7c == SEIDX_BG_CITY_TRAIN) {
                SndMgr_StopPlayingSE(SEIDX_BG_CITY_TRAIN);
            }
            SndMgr_StartPlayingSE(sound);
        }
        data_ov030_02109d7c = sound;
    }
}

void func_ov030_020c2db4(void) {
    if (data_ov030_02109d7c != SEIDX_DISABLESOUNDEFFECTS) {
        SndMgr_StopPlayingSE(data_ov030_02109d7c);
    }
    data_ov030_02109d7c = SEIDX_DISABLESOUNDEFFECTS;
}

void func_ov030_020c2de8(void) {
    data_ov030_02109d7c = SEIDX_DISABLESOUNDEFFECTS;
}
