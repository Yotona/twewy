#ifndef MINI_108_H
#define MINI_108_H

#include "DatMgr.h"
#include "Input.h"
#include "OverlayDispatcher.h"
#include <types.h>

extern vu32 data_02066a58;

typedef struct {
    /* 0x00000 */ u32          unk_00;
    /* 0x00004 */ InputButtons buttonState;
    /* 0x0000C */ u32          unk_0C;
    /* 0x00010*/ u32           unk_10;
    /* 0x00014 */ u32          unk_14;
    /* 0x00018 */ char         pad_18[0x28];
    /* 0x00040 */ u32          unk_40;
    /* 0x00044 */ char         pad_44[0x3C];
    /* 0x00080 */ u32          unk_80;
    /* 0x00084 */ char         pad_84[0x3AC];
    /* 0x00430 */ u32          unk_430;
    /* 0x00434 */ char         pad_434[0x418];
    /* 0x0084C */ Data*        unk_84C;
    /* 0x00854 */ char         pad_87C[0x4];
    /* 0x008D0 */ u32          unk_8D0;
    /* 0x008D4 */ char         pad_8D4[0x3FFC];
    /* 0x048D0 */ u32          unk_48D0;
    /* 0x048D4 */ char         pad_48D4[0x1157C];
    /* 0x15e50 */ u32          unk_15e50;
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