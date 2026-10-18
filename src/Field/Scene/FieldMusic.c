#include "Field/Scene/FieldMusic.h"
#include "CriSndMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Field/FieldTypes.h"
#include "Field/Flash/FO3DViewer.h"
#include "FixedPoint.h"
#include "Save.h"
#include "SndMgr.h"
#include <nitro/fx/fx_vector.h>

extern s32 CriSndMgr_Play(void);

// Field BGM pool of a chapter group.
typedef struct {
    /* 0x0 */ u16 allMask; // every track of the pool
    /* 0x2 */ u16 count;
} Ov030BgmPool;

const Ov030BgmPool data_ov030_020e70fc[3] = {
    { 0x7, 3},
    {0x1F, 5},
    {0x7F, 7}
};
// Field BGM tracks.
const s32  data_ov030_020e7108[7] = {1, 2, 3, 4, 5, 6, 7};
extern s32 data_ov030_02107534[];
extern s32 data_ov030_02107540[];
extern s32 data_ov030_0210754c[];
extern s32 data_ov030_02107558[];
extern s32 data_ov030_02107560[];

s32 func_ov030_020c2e00(void) {
    if (gSaveData.fieldArea < 0x26) {
        return 1;
    }
    if (gSaveData.fieldArea > 0x2B) {
        return 1;
    }
    return 0;
}

// Nonmatching: retail reaches .count as (table + group * 4) + 2 from a reloaded
// table base; mwcc here folds the +2 into a second pool word (USA 94.17%; JP unvalidated).
void func_ov030_020c2e28(void) {
    s32 group;

    if (func_ov030_020c2e00() == 0) {
        return;
    }
    if (gSaveData.chapter <= 6) {
        group = 0;
    } else if (gSaveData.chapter <= 13) {
        group = 1;
    } else if (gSaveData.chapter <= 21) {
        group = 2;
    } else if (gSaveData.chapter <= 28) {
        group = 0;
    } else if (gSaveData.chapter <= 35) {
        group = 1;
    } else {
        group = 2;
    }

    u16 mask = data_ov030_020e70fc[group].allMask;
    if (mask == (mask & gSaveData.unk_3106)) {
        gSaveData.unk_3106 = 0;
    }

    s16 idx;
    s32 limit = data_ov030_020e70fc[group].count * 100;
    do {
        do {
            idx = RNG_Next(limit) / 100;
        } while (gSaveData.unk_3106 & (1 << idx));
    } while (idx == gSaveData.unk_3108);

    gSaveData.unk_3106 |= (1 << idx);
    CriSndMgr_PlayFile(data_ov030_020e7108[idx]);
    gSaveData.unk_3108 = idx;
}

s32 func_ov030_020c2f2c(s32 arg0) {
    if (arg0 > 12000) {
        return 0;
    }
    if (arg0 != 0) {
        return 1;
    }
    if (gSaveData.fieldArea == 18) {
        return 0;
    }
    if (gSaveData.fieldArea == 41) {
        return 0;
    }
    return 1;
}

void func_ov030_020c2f74(void) {
    s32 snd = CriSndMgr_Play();

    if (func_ov030_020c2e00() == 0) {
        CriSndMgr_Stop(ADX_E02);
        gSaveData.unk_3108 = -1;
    } else if (func_ov030_020c2f2c(snd) == 0) {
        func_ov030_020c2e28();
    }
}

void func_ov030_020c2fc0(s32 arg0) {
    if (arg0 == 70) {
        func_ov030_020c2e28();
    } else {
        for (s16 i = 0; (u32)i < 7; i++) {
            if (arg0 == data_ov030_020e7108[i]) {
                if (gSaveData.unk_3108 == i) {
                    return;
                }
                gSaveData.unk_3106 |= (1 << i);
                CriSndMgr_PlayFile(data_ov030_020e7108[i]);
                gSaveData.unk_3108 = i;
                return;
            }
        }
        CriSndMgr_PlayFile(arg0);
        gSaveData.unk_3108 = -1;
    }
}

void func_ov030_020c304c(FieldPos origin) {
    if (F2I(FO3DViewer_GetAnimFrame(data_ov030_02107534)) == 1) {
        SndMgr_StartPlayingSE(SEIDX_BG_CITY_TRAIN_NO010LTOR);
    }
    if (F2I(FO3DViewer_GetAnimFrame(data_ov030_02107540)) == 420) {
        SndMgr_StartPlayingSE(SEIDX_BG_CITY_TRAIN_NO010RTOL);
    }
}

void func_ov030_020c309c(FieldPos origin) {
    if (F2I(FO3DViewer_GetAnimFrame(data_ov030_0210754c)) == 2901) {
        SndMgr_StartPlayingSE(SEIDX_BG_CITY_TRAIN_EA001LTOR);
    }
    if (F2I(FO3DViewer_GetAnimFrame(data_ov030_0210754c)) == 981) {
        SndMgr_StartPlayingSE(SEIDX_BG_CITY_TRAIN_EA001RTOL);
    }
}

void func_ov030_020c30ec(FieldPos pos, s32* arg2, s32* arg3) {
    s32 se;
    s32 se2;
    s32 off;

    if (gSaveData.fieldArea == 15) {
        se  = SEIDX_BG_CITY_CARPASS_FAST;
        se2 = SEIDX_BG_CITY_CARPASS_SLOW;
        off = 0;
    } else if (gSaveData.fieldArea == 19) {
        se  = SEIDX_BG_CITY_CAR_SLOW;
        se2 = SEIDX_BG_CITY_CAR_SLOW;
        off = 0xE8000;
    } else {
        return;
    }

    Vec coordA;
    if (FO3DViewer_GetNodePos(data_ov030_02107558, &coordA) != 0) {
        if (pos.x <= coordA.x && coordA.x <= pos.x + 0x100000) {
            if (*arg2 == 0) {
                SndMgr_PlaySEWithPan(se, F2I(coordA.x - pos.x));
            }
            *arg2 = 1;
        } else {
            *arg2 = 0;
        }
    }

    Vec coordB;
    if (FO3DViewer_GetNodePos(data_ov030_02107560, &coordB) != 0) {
        if (pos.x <= coordB.x + off && coordB.x <= pos.x + 0x100000) {
            if (*arg3 == 0) {
                SndMgr_PlaySEWithPan(se2, F2I(coordB.x - pos.x));
            } else if (gSaveData.fieldArea == 19) {
                SndMgr_UpdateSEPan(se2, F2I(coordB.x - pos.x));
            }
            *arg3 = 1;
        } else {
            *arg3 = 0;
            if (gSaveData.fieldArea == 19) {
                SndMgr_StopPlayingSE(se2);
            }
        }
    }
}

void func_ov030_020c3280(FieldPos pos, s16* arg2) {
    if (gSaveData.fieldArea == 39) {
        if (*arg2 != 0) {
            (*arg2)--;
            SndMgr_UpdateSEPan(SEIDX_BG_CITY_WATER_SEWER, F2I(0x1EC000 - pos.x));
        } else {
            *arg2 = 0x1D;
            SndMgr_PlaySEWithPan(SEIDX_BG_CITY_WATER_SEWER, F2I(0x1EC000 - pos.x));
        }
    }
}

void func_ov030_020c3304(FieldPos origin) {
    if (gSaveData.fieldArea == 14) {
        func_ov030_020c309c(origin);
    } else if (gSaveData.fieldArea == 13) {
        func_ov030_020c304c(origin);
    }
}

void func_ov030_020c3360(void) {
    SndMgr_StopPlayingSE(SEIDX_BG_CITY_WATER_SEWER);
    SndMgr_StopPlayingSE(SEIDX_BG_CITY_CAR_SLOW);
}
