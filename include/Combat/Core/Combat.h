#ifndef COMBAT_CORE_COMBAT_H
#define COMBAT_CORE_COMBAT_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "SndMgrSeIdx.h"

/// One per screen engine; `BtlEnm015_Shake` jitters the offset at 0x44..0x4C.
typedef struct Ov003Camera {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ u8  unk_14[0x20 - 0x14];
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ u8  unk_28[0x44 - 0x28];
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ u8  unk_50[0x58 - 0x50];
} Ov003Camera; // Size: 0x58

typedef struct {
    /* 0x00000 */ TaskPool         unk_00000;
    /* 0x00080 */ u8               unk_00080[0x808C - 0x80];
    /* 0x0808C */ TaskPool         taskPool;
    /* 0x0810C */ char             unk_0810C[0x10118 - 0x810C];
    /* 0x10118 */ TaskPool         unk_10118;
    /* 0x10198 */ char             unk_10198[0x34230 - 0x10198];
    /* 0x34230 */ TaskPool         unk_34230;
    /* 0x342B0 */ char             unk_342B0[0x3D348 - 0x342B0];
    /* 0x3D348 */ PaletteResource* unk3D348[2];
    /* 0x3D350 */ Data*            unk3D350;
    /* 0x3D354 */ u8               unk_3D354[0x3D79C - 0x3D354];
    /* 0x3D79C */ u32              unk3D79C;
    /* 0x3D7A0 */ u8               unk_3D7A0[0x3D7C0 - 0x3D7A0];
    /* 0x3D7C0 */ Ov003Camera      unk3D7C0[2]; // [0] main engine, [1] sub engine
    /* 0x3D870 */ u8               unk_3D870[0x3D874 - 0x3D870];
    /* 0x3D874 */ u8               unk3D874;
    /* 0x3D875 */ u8               unk3D875;
    /* 0x3D876 */ char             unk_3D876[0x2];
    /* 0x3D878 */ u32              unk3D878;
    /* 0x3D87C */ s32              unk3D87C;
    /* 0x3D880 */ u8               unk_3D880[0x3D88E - 0x3D880];
    /* 0x3D88E */ u16              unk3D88E;
    /* 0x3D890 */ u8               unk_3D890[0x3D898 - 0x3D890];
    /* 0x3D898 */ void*            unk3D898;
    /* 0x3D89C */ void*            unk3D89C;
    /* 0x3D8A0 */ void*            unk3D8A0;
    /* 0x3D8A4 */ void*            unk3D8A4;
    /* 0x3D8A8 */ void*            unk3D8A8;
    /* 0x3D8AC */ void*            unk3D8AC;
    /* 0x3D8B0 */ u8               pad_3D8B4[0x4];
    /* 0x3D8B4 */ void*            unk3D8B4;
    /* 0x3D8B8 */ char             unk_3D8B8[0x3D8BC - 0x3D8B8];
    /* 0x3D8BC */ void*            unk3D8BC;
    /* 0x3D8C0 */ char             unk_3D8C0[0x3D8D4 - 0x3D8C0];
    /* 0x3D8D4 */ s16              unk3D8D4;
    /* 0x3D8D6 */ s16              unk3D8D6;
    /* 0x3D8D8 */ s16              unk3D8D8;
    /* 0x3D8DA */ s16              unk3D8DA;
    /* 0x3D8DC */ s16              unk3D8DC;
    /* 0x3D8DE */ s16              unk3D8DE;
    /* 0x3D8E0 */ s32              unk3D8E0;
    /* 0x3D8E4 */ u8               unk_3D8E4[0x8];
    /* 0x3D8EC */ u8               unk3D8EC;
    /* 0x3D8ED */ s8               unk3D8ED;
    /* 0x3D8EE */ s16              unk3D8EE;
    /* 0x3D8F0 */ s32              unk3D8F0;
    /* 0x3D8F4 */ s32              unk3D8F4;
    /* 0x3D8F8 */ s16              unk3D8F8;
} Ov003Global;

extern Ov003Global* data_ov003_020e71b8;

/// Per-enemy parameter record returned by `func_ov003_020c3c88(unit)`. The noise tasks scale
/// the halfwords at 0x1C..0x26 by their damage level to get per-state speeds/timers.
typedef struct CombatEnemyParams {
    /* 0x00 */ u8  unk_00[0x3];
    /* 0x03 */ u8  unk_03;
    /* 0x04 */ u8  unk_04;
    /* 0x05 */ u8  unk_05[0x10 - 0x5];
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ u8  unk_14[0x1C - 0x14];
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
} CombatEnemyParams;

// Used in TaskStage.render functions
void func_ov003_020c4878(void*);

void func_ov003_020c48fc(void*);

void func_ov003_02087f00(SndMgrSeIdx seIdx, s32 sePan);

#endif // COMBAT_CORE_COMBAT_H