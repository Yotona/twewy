#include "OverlayManager.h"
#include "cache.h"
#include "common_data.h"
#include "game.h"

extern s32 OVERLAY_40_ID;

extern s32 DAT_0206aae0;
extern s32 DAT_0206b3d0;

char* data_ov041_02083020 = "Seq_STSample()";

typedef void (*UnkFuncPtr)(GameState*);

void func_ov041_02082ff0(void);

u32 func_ov041_020824a0(GameState* state, int param_2, u32 param_3) {
    /* Not yet implemented */
}

void func_ov041_02082600(GameState* state, s32 param_2, u16* param_3, u32 param_4) {
    /* Not yet implemented */
}

void func_ov041_02082730(GameState* state) {
    UnkStruct_usedby_02025b68 local_30;

    local_30.unk_00 = 1;
    local_30.unk_04 = 0;
    local_30.unk_08 = state->unk_11584;
    local_30.unk_0C = &data_0205c9b0;
    local_30.unk_10 = 0;
    local_30.unk_14 = NULL;
    local_30.unk_18 = 0;
    local_30.unk_1A = 0;
    local_30.unk_1C = 0x20;
    local_30.unk_1E = 0x18;
    func_02025b68(&state->unk_21594, &local_30);
    func_02010b18(&state->unk_215A0, 0, 0, "���ꂿ�����T���v��"); // "StreetPass Sample"
}

void func_ov041_020827b8(GameState* state) {
    func_02025e30(&state->unk_21594);
}

/* Nonmatching */
void func_ov041_020827cc(GameState* state) {
    int iVar1;
    s8  abStack_5c[4];
    u16 auStack_58[11];
    u16 local_42;
    u16 auStack_40[27];
    u16 local_a;

    func_ov041_02082730(state);
    func_ov040_0209dfdc(&state->unk_219B0, func_ov041_020824a0, state, func_ov041_02082600, state, &state->unk_21F30, 0x152);
    state->unk_21E60 = 0;
    state->unk_22084 = 0;
    state->unk_22088 = 0;
    state->unk_21E68 = 0;
    state->unk_21F2C = 0;
    state->unk_21E64 = 0;
    func_0203a96c(&state->unk_21F30);
    func_0203a988(abStack_5c);
    func_0203b3c0(&state->unk_21F36, 0, 0x16);
    func_ov040_0209da10(&state->unk_21F36, auStack_58, local_42);
    func_0203b3c0(&state->unk_21F4C, 0, 0x36);
    func_ov040_0209da10(&state->unk_21F4C, auStack_40, local_a);

    for (iVar1 = 0; iVar1 < 0x100; iVar1++) {
        state->unk_21F82[iVar1] = (s8)iVar1;
    }

    func_0203b3c0(&state->unk_21E6C, 0xff, 0xc0);
    func_0203b3c0(&state->unk_2208C, 0, 0x152);
}

void func_ov041_020828f0(GameState* state) {
    func_ov041_020827b8(state);
    func_ov040_0209e028(&state->unk_219B0);
}

/* Nonmatching */
void func_ov041_0208290c(GameState* state) {
    s32        idx;
    u8*        puVar2;
    OverlayTag tag;

    switch (data_02066a24.unk_02) {
        case 1:
            if (state->unk_21E60 == 0) {
                func_ov040_0209e888(&state->unk_219B0);
            }
            break;
        case 2:
            if (state->unk_21E60 != 0) {
                func_ov040_0209e900(&state->unk_219B0);
            }
            break;
        case 0x400:
            break;
    }

    if ((state->unk_21E60 != 0) && (state->unk_21E64 >= 10)) {
        func_ov040_0209e900(&state->unk_219B0);
    }

    if (state->unk_22084 > state->unk_22088) {
        puVar2 = &state->unk_0004C[state->unk_22088 * 0x141];
        for (idx = 0; idx < 0x100; idx++) {
            if ((idx & 0xff) != (u32)(u8)puVar2[0x220de]) {
                break;
            }
            puVar2++;
        }
        func_02010b18(&state->unk_215A0, 8, (state->unk_22088 + 1) * 8, "�f�[�^��M"); // "Data Received"
        state->unk_22088++;
    }

    state->unk_21E60 = func_ov040_0209e03c(&state->unk_219B0);
    if (state->unk_21E60 != 0) {
        return;
    }
    if ((data_02066a24.unk_02 & 4) != 0) {
        func_02007174(&tag);
    }
}

/* Nonmatching */
void func_ov041_02082a5c(GameState* param) {
    int        iVar1;
    GameState* state;
    GameState* sVar2;

    func_0200669c(3, OVERLAY_40_ID);
    iVar1 = data_ov041_02083020;
    state = (GameState*)func_02004618(&data_0206a9b0, 0x22B1C);
    func_020049a8(&data_0206a9b0, state, iVar1);
    sVar2 = func_02007260(state);
    func_02008e80();
    state->unk_11584 = sVar2;
    func_ov041_02082ff0();
    state->unk_11580     = func_0200cef0(state);
    data_02066aec        = 0;
    data_0206aa80.unk_30 = 0;
    data_0206aa80.unk_60 = 0;
    data_02066eec        = 0;
    func_ov041_020827cc(state);
    func_020072a4();
}

void func_ov041_02082b08(GameState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_ov041_0208290c(state);
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

void func_ov041_02082b88(GameState* state) {
    func_ov041_020828f0(state);
    func_0200cef0(NULL);
    func_02008ebc(state->unk_11584);
    func_020048b4(&data_0206a9b0, state);
    func_02006618(3);
}

void func_ov041_02082bc4(GameState* state) {
    static const UnkFuncPtr funcs[] = {
        func_ov041_02082a5c,
        func_ov041_02082b08,
        func_ov041_02082b88,
    };
    s32 idx;

    idx = func_02007278();
    if (idx == 0x7FFFFFFF) {
        func_ov041_02082b88(state);
        return;
    }
    funcs[idx](state);
}

/* Nonmatching */
void func_ov041_02082c04(void) {
    /* Not yet implemented */
}

void func_ov041_02082f1c(s32 param_1, s32 param_2, s32 param_3, s32 param_4) {
    s32 val = data_02066a20 << 0x1f;
    if ((u32)val >> 0x1f) {
        func_ov040_0209e91c();
        func_02006380();
        func_020019ac();
        DC_PurgeRange(&data_0206770c, 0x400);
        func_02037264(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        func_020372b8(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        func_02037108(&data_02066aec, 0, 0x200);
        func_020371b4(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        func_0203715c(&data_02066eec, 0, 0x200);
        func_0203720c(&data_020670ec, 0, 0x200);
    }
}

void func_ov041_02082ff0(void) {
    func_ov041_02082c04();
    func_020015a8(func_ov041_02082f1c, 1);
}
