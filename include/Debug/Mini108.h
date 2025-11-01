#ifndef MINI_108_H
#define MINI_108_H
#include "game.h"

typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_Mini108_tuple;
extern u32                     OVERLAY_0_ID;
extern UnkStruct_Mini108_tuple data_ov000_020831a8;
extern UnkStruct_Mini108_tuple data_ov000_020831b4;
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
extern char* data_ov000_020831e0;
extern u32   data_ov000_020831c0;
extern u32   data_ov000_02083210;
extern u32   data_02066a58;
extern char* data_ov000_0208322c;
extern u32   data_ov000_02083238;
extern u32   data_ov000_02083254;

void func_ov000_020825c0(void);
void func_ov000_02082ac0(u32* state);
void func_ov000_02082b18(u32* unk_r0);
void func_ov000_02082b1c(u32* unk_r0);
void func_ov000_020830b8();
void func_ov000_020830bc();

#endif // MINI_108_H