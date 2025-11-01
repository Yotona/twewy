#ifndef MINI_108_H
#define MINI_108_H
#include "game.h"

extern vu32 data_02066a58;

extern u32 OVERLAY_0_ID;

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
void func_ov000_02082ac0(u32* state);
void func_ov000_02082b18(u32* unk_r0);
void func_ov000_02082b1c(u32* unk_r0);
void func_ov000_020830b8();
void func_ov000_020830bc();

#endif // MINI_108_H