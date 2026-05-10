#include "Player/Progress/ProgressFlags.h"
#include "Save.h"

extern s32 func_ov030_020b7b40();
extern s32 func_ov030_020b7b60();

void Progress_SetGate(ProgressGate gate) {
    s32 word_idx = gate / 32;
    s32 bit_idx  = gate % 32;

    if (word_idx < 4) {
        data_02071cf0.unk_20.progressFlags[word_idx] |= 1 << bit_idx;
    }
}

void Progress_ClearGate(ProgressGate gate) {
    s32 word_idx = gate / 32;
    s32 bit_idx  = gate % 32;

    if (word_idx < 4) {
        data_02071cf0.unk_20.progressFlags[word_idx] &= ~(1 << bit_idx);
    }
}

BOOL Progress_Check(ProgressGate gate) {
    s32 word_idx = gate / 32;
    s32 bit_idx  = gate % 32;

    if (word_idx >= 4) {
        return 0;
    }

    return (data_02071cf0.unk_20.progressFlags[word_idx] & (1 << bit_idx)) != 0;
}

void func_ov030_020aebf0(s32 arg0) {
    data_02071cf0.unk_20.progressFlags[0] |= arg0;
}

s32 func_ov030_020aec08(s32 arg0) {
    return data_02071cf0.unk_20.progressFlags[0] & arg0;
}

void func_ov030_020aec1c(s32 arg0) {
    data_02071cf0.unk_20.unk_246C |= (1 << arg0);
}

void func_ov030_020aec38(s32 arg0) {
    data_02071cf0.unk_20.unk_246C &= ~(1 << arg0);
}

void func_ov030_020aec58(s32 arg0) {
    s32  word_idx = arg0 / 32;
    s32  bit_idx  = arg0 % 32;
    u32* ptr      = &data_02071cf0.unk_20.unk_2474;

    if (word_idx > 1) {
        ptr = &data_02071cf0.unk_20.unk_247C;
        word_idx -= 2;
    }
    if (word_idx != 0) {
        ptr++;
    }
    *ptr |= (1 << (bit_idx));
}

s32 func_ov030_020aeca4(s32 arg0) {
    s32 temp_r0 = (s32)(arg0 - (func_ov030_020b7b40() << 0xC)) / func_ov030_020b7b60();
    if ((data_02071cf0.unk_20.unk_2458 == 0) && (arg0 < 0x8E000)) {
        return (s32)(((s64)(arg0 / 142) * 0xE66 + 0x800) >> 12);
    }
    return data_02071cf0.unk_20.unk_248C + (s32)(((s64)data_02071cf0.unk_20.unk_2494 * temp_r0 + 0x800) >> 12);
}
