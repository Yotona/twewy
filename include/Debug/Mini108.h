#ifndef MINI_108_H
#define MINI_108_H
#include "DatMgr.h"
#include "OverlayDispatcher.h"
#include <types.h>

extern vu32 data_02066a58;

extern s32 OVERLAY_0_ID;

typedef struct {
    /* 0x0*/ u32      unk_00;
    /* 0x4*/ u16      currButtons;
    /* 0x6*/ u16      pressedButtons;
    /* 0x8*/ u16      holdButtons;
    /* 0xa */ u16     prevButtons;
    /* 0xC */ u32     unk_0C;
    /* 0x10*/ u32     unk_10;
    /* 0x14 */ u32    unk_14;
    /* 0x18 */ char   pad_18[0x28];
    /* 0x40 */ u32    unk_40;
    /* 0x44 */ char   pad_44[0x3C];
    /* 0x80 */ u32    unk_80;
    /* 0x84 */ char   pad_84[0x3AC];
    /* 0x430 */ u32   unk_430;
    /* 0x434 */ char  pad_434[0x418];
    /* 0x84C */ Data* unk_84C;
    /* 0x854 */ char  pad_87C[0x4];
    /* 0x8D0 */ u32   unk_8D0;
    /* 0x8D4 */ char  pad_8D4[0x3FFC];
    /* 0x48D0 */ u32  unk_48D0;
    /* 0x48D4 */ char pad_48D4[0x1157C];
    /* 0x15e50 */ u32 unk_15e50;
} Mini108State; // Size = 0x15e54

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    s16 unk_1E;
} UnkStruct_ov000_02082b1c;

void func_ov000_020825c0(void);
void func_ov000_02082ac0(Mini108State* state);
void func_ov000_02082b18(Mini108State* unk_r0);
void func_ov000_02082b1c(Mini108State* unk_r0);
void func_ov000_020830b8();
void func_ov000_020830bc();

#endif // MINI_108_H