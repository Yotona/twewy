#include "Player/Progress/ProgressFlags.h"
#include "Save.h"

extern s32 func_ov030_020b7b40();
extern s32 func_ov030_020b7b60();

void Progress_SetGate(ProgressGate gate) {
    s32 word_idx = gate / 32;
    s32 bit_idx  = gate % 32;

    if (word_idx < 4) {
        gSaveData.progressFlags[word_idx] |= 1 << bit_idx;
    }
}

void Progress_ClearGate(ProgressGate gate) {
    s32 word_idx = gate / 32;
    s32 bit_idx  = gate % 32;

    if (word_idx < 4) {
        gSaveData.progressFlags[word_idx] &= ~(1 << bit_idx);
    }
}

BOOL Progress_Check(ProgressGate gate) {
    s32 word_idx = gate / 32;
    s32 bit_idx  = gate % 32;

    if (word_idx >= 4) {
        return 0;
    }

    return (gSaveData.progressFlags[word_idx] & (1 << bit_idx)) != 0;
}

void func_ov030_020aebf0(s32 arg0) {
    gSaveData.progressFlags[0] |= arg0;
}

s32 func_ov030_020aec08(s32 arg0) {
    return gSaveData.progressFlags[0] & arg0;
}

void func_ov030_020aec1c(s32 arg0) {
    gSaveData.unk_246C |= (1 << arg0);
}

void func_ov030_020aec38(s32 arg0) {
    gSaveData.unk_246C &= ~(1 << arg0);
}

// Nonmatching: regswap
void func_ov030_020aec58(s32 arg0) {
    s32  word_idx = arg0 / 32;
    s32  bit_idx  = arg0 % 32;
    u32* ptr      = &gSaveData.unk_2474;
    if (word_idx > 1) {
        ptr = &gSaveData.unk_247C;
        word_idx -= 2;
    }

    s32 new_var = 1 << bit_idx;
    if (word_idx != 0) {
        ptr++;
    }
    *ptr |= new_var;
}

s32 func_ov030_020aeca4(s32 arg0) {
    s32 temp_r0 = (s32)(arg0 - (func_ov030_020b7b40() << 0xC)) / func_ov030_020b7b60();
    if ((gSaveData.unk_2458 == 0) && (arg0 < 0x8E000)) {
        return (s32)(((s64)(arg0 / 142) * 0xE66 + 0x800) >> 12);
    }
    return gSaveData.unk_248C + (s32)(((s64)gSaveData.unk_2494 * temp_r0 + 0x800) >> 12);
}
