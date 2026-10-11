// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020c2e00-0x020c3380; 11 functions.
// Research draft: docs/decomp/ov030/Field_020c2e00.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "CriSndMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Field/FieldTypes.h"
#include "Save.h"
#include "SndMgr.h"
#include <nitro/fx/fx_vector.h>

extern s32 CriSndMgr_Play(void);
extern s32 func_ov030_020d1a8c(void* name, Vec* pos);
extern s32 func_ov030_020d1ba0(void* name);
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
    if (gSaveData.unk_2458 < 0x26) {
        return 1;
    }
    if (gSaveData.unk_2458 > 0x2B) {
        return 1;
    }
    return 0;
}

// Nonmatching: retail reaches .count as (table + group * 4) + 2 from a reloaded
// table base; mwcc here folds the +2 into a second pool word (USA 94.17%; JP unvalidated).
void func_ov030_020c2e28(void) {
    s32 group;
    u16 mask;
    s16 idx;
    s32 limit;

    if (func_ov030_020c2e00() == 0) {
        return;
    }
    if (gSaveData.chapter <= 6) {
        group = 0;
    } else if (gSaveData.chapter <= 0xD) {
        group = 1;
    } else if (gSaveData.chapter <= 0x15) {
        group = 2;
    } else if (gSaveData.chapter <= 0x1C) {
        group = 0;
    } else if (gSaveData.chapter <= 0x23) {
        group = 1;
    } else {
        group = 2;
    }
    mask = data_ov030_020e70fc[group].allMask;
    if (mask == (mask & (u16)gSaveData.unk_3106)) {
        gSaveData.unk_3106 = 0;
    }
    limit = data_ov030_020e70fc[group].count * 100;
    do {
        do {
            s32 rnd = RNG_Next(limit);

            idx = rnd / 100;
        } while ((u16)gSaveData.unk_3106 & (1 << idx));
    } while (idx == gSaveData.unk_3108);
    gSaveData.unk_3106 = (u16)gSaveData.unk_3106 | (1 << idx);
    CriSndMgr_PlayFile(data_ov030_020e7108[idx]);
    gSaveData.unk_3108 = idx;
}

s32 func_ov030_020c2f2c(s32 arg0) {
    if (arg0 > 0x2EE0) {
        return 0;
    }
    if (arg0 != 0) {
        return 1;
    }
    if (gSaveData.unk_2458 == 18) {
        return 0;
    }
    if (gSaveData.unk_2458 == 41) {
        return 0;
    }
    return 1;
}

void func_ov030_020c2f74(void) {
    s32 snd = CriSndMgr_Play();

    if (func_ov030_020c2e00() == 0) {
        CriSndMgr_Stop(0x1B);
        gSaveData.unk_3108 = -1;
        return;
    }
    if (func_ov030_020c2f2c(snd) != 0) {
        return;
    }
    func_ov030_020c2e28();
}

void func_ov030_020c2fc0(s32 arg0) {
    s16 i;

    if (arg0 == 0x46) {
        func_ov030_020c2e28();
        return;
    }
    for (i = 0; (u32)i < 7; i++) {
        if (arg0 == data_ov030_020e7108[i]) {
            if (gSaveData.unk_3108 == i) {
                return;
            }
            gSaveData.unk_3106 = (u16)gSaveData.unk_3106 | (1 << i);
            CriSndMgr_PlayFile(data_ov030_020e7108[i]);
            gSaveData.unk_3108 = i;
            return;
        }
    }
    CriSndMgr_PlayFile(arg0);
    gSaveData.unk_3108 = -1;
}

void func_ov030_020c304c(FieldPos origin) {
    if ((func_ov030_020d1ba0(data_ov030_02107534) >> 12) == 1) {
        SndMgr_StartPlayingSE(0x10B);
    }
    if ((func_ov030_020d1ba0(data_ov030_02107540) >> 12) != 0x1A4) {
        return;
    }
    SndMgr_StartPlayingSE(0x10A);
}

void func_ov030_020c309c(FieldPos origin) {
    if ((func_ov030_020d1ba0(data_ov030_0210754c) >> 12) == 0xB55) {
        SndMgr_StartPlayingSE(0x109);
    }
    if ((func_ov030_020d1ba0(data_ov030_0210754c) >> 12) != 0x3D5) {
        return;
    }
    SndMgr_StartPlayingSE(0x108);
}

// The ambient sound emitters below take the camera origin by value and pan
// their sound effects by the emitter's offset from it.
void func_ov030_020c30ec(FieldPos pos, s32* arg2, s32* arg3) {
    s32 se;
    s32 se2;
    s32 off;
    Vec coordA;
    Vec coordB;

    if (gSaveData.unk_2458 == 15) {
        se  = 0x10D;
        se2 = 0x10C;
        off = 0;
    } else if (gSaveData.unk_2458 == 19) {
        se  = 0x10E;
        se2 = 0x10E;
        off = 0xE8000;
    } else {
        return;
    }
    if (func_ov030_020d1a8c(data_ov030_02107558, &coordA) != 0) {
        if (pos.x <= coordA.x && coordA.x <= pos.x + 0x100000) {
            if (*arg2 == 0) {
                SndMgr_PlaySEWithPan(se, (coordA.x - pos.x) >> 12);
            }
            *arg2 = 1;
        } else {
            *arg2 = 0;
        }
    }
    if (func_ov030_020d1a8c(data_ov030_02107560, &coordB) == 0) {
        return;
    }
    if (pos.x <= coordB.x + off && coordB.x <= pos.x + 0x100000) {
        if (*arg3 == 0) {
            SndMgr_PlaySEWithPan(se2, (coordB.x - pos.x) >> 12);
        } else if (gSaveData.unk_2458 == 0x13) {
            SndMgr_UpdateSEPan(se2, (coordB.x - pos.x) >> 12);
        }
        *arg3 = 1;
        return;
    }
    *arg3 = 0;
    if (gSaveData.unk_2458 != 0x13) {
        return;
    }
    SndMgr_StopPlayingSE(se2);
}

void func_ov030_020c3280(FieldPos pos, s16* arg2) {
    s16 temp;

    if (gSaveData.unk_2458 != 0x27) {
        return;
    }
    temp = *arg2;
    if (temp != 0) {
        *arg2 = temp - 1;
        SndMgr_UpdateSEPan(0x10F, (0x1EC000 - pos.x) >> 12);
    } else {
        *arg2 = 0x1D;
        SndMgr_PlaySEWithPan(0x10F, (0x1EC000 - pos.x) >> 12);
    }
}

void func_ov030_020c3304(FieldPos origin) {
    if (gSaveData.unk_2458 == 14) {
        func_ov030_020c309c(origin);
        return;
    }
    if (gSaveData.unk_2458 == 13) {
        func_ov030_020c304c(origin);
        return;
    }
}

void func_ov030_020c3360(void) {
    SndMgr_StopPlayingSE(0x10F);
    SndMgr_StopPlayingSE(0x10E);
}
