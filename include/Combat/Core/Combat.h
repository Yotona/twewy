#ifndef COMBAT_CORE_COMBAT_H
#define COMBAT_CORE_COMBAT_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "SndMgrSeIdx.h"

typedef struct {
    /* 0x00000 */ TaskPool unk_00000;
    /* 0x00080 */ u8       unk_00080[0x808C - 0x80];
    /* 0x0808C */ TaskPool taskPool;
    /* 0x0810C */ char     unk_0810C[0x10118 - 0x810C];
    /* 0x10118 */ TaskPool unk_10118;
    /* 0x10198 */ char     unk_10198[0x3D350 - 0x10198];
    /* 0x3D350 */ Data*    unk3D350;
    /* 0x3D354 */ u8       unk_3D354[0x3D7C2 - 0x3D354];
    /* 0x3D7C2 */ s16      unk3D7C2;
    /* 0x3D7C4 */ s32      unk3D7C4;
    /* 0x3D7C8 */ char     unk_3D7C8[0x4];
    /* 0x3D7CC */ s32      unk3D7CC;
    /* 0x3D7D0 */ s32      unk3D7D0;
    /* 0x3D7D4 */ u8       unk_3D7D4[0x3D81A - 0x3D7D4];
    /* 0x3D81A */ u16      unk3D81A;
    /* 0x3D81C */ char     unk_3D81C[0x8];
    /* 0x3D824 */ s32      unk3D824;
    /* 0x3D828 */ s32      unk3D828;
    /* 0x3D82C */ char     unk_3D82C[0x3D838 - 0x3D82C];
    /* 0x3D838 */ s32      unk3D838;
    /* 0x3D83C */ s32      unk3D83C;
    /* 0x3D840 */ char     unk_3D840[0x3D874 - 0x3D840];
    /* 0x3D874 */ u8       unk3D874;
    /* 0x3D875 */ u8       unk3D875;
    /* 0x3D876 */ char     unk_3D876[0x2];
    /* 0x3D878 */ u32      unk3D878;
    /* 0x3D87C */ u8       pad_3D898[0x1C];
    /* 0x3D898 */ void*    unk3D898;
    /* 0x3D89C */ void*    unk3D89C;
    /* 0x3D8A0 */ void*    unk3D8A0;
    /* 0x3D8A4 */ u8       pad_3D8AC[0x8];
    /* 0x3D8AC */ void*    unk3D8AC;
    /* 0x3D8B0 */ u8       pad_3D8B4[0x4];
    /* 0x3D8B4 */ void*    unk3D8B4;
    /* 0x3D8B8 */ char     unk_3D8B8[0x3D8D8 - 0x3D8B8];
    /* 0x3D8D8 */ s16      unk3D8D8;
    /* 0x3D8DA */ s16      unk3D8DA;
    /* 0x3D8DC */ s16      unk3D8DC;
    /* 0x3D8DE */ s16      unk3D8DE;
} Ov003Global;

extern Ov003Global* data_ov003_020e71b8;

// Used in TaskStage.render functions
void func_ov003_020c4878(void*);

// Used in TaskStage.destroy functions
void func_ov003_02082cc4(void*);
void func_ov003_020c48fc(void*);

void func_ov003_02087f00(SndMgrSeIdx seIdx, s32 (*)(s32, s32));

#endif // COMBAT_CORE_COMBAT_H