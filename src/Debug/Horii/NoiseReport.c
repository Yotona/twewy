#include "Memory.h"
#include "System.h"
#include "common_data.h"
#include "game.h"

extern s32 data_020672ec;

typedef struct {
    /* 0x00000 */ char unk_00000[0x21590];
    /* 0x21590 */ s32  unk_21590;
    /* 0x21594 */ char unk_21594[0x7C];
    /* 0x21610 */ s32  unk_21610;
    /* 0x21614 */ char unk_21614[0x38];
    /* 0x2164C */ u16  unk_2164C;
    /* 0x2164E */ char unk_2164E[2];
} NoiseReportState; // Size: 0x21650

typedef void (*NoiseReportFunc)(NoiseReportState* state);

void func_ov028_020e8878(NoiseReportState*);
void func_ov028_020e89d0(NoiseReportState*);
void func_ov028_020e8b68(NoiseReportState*);

const NoiseReportFunc data_ov028_020ecf24[3] = {
    func_ov028_020e8878,
    func_ov028_020e89d0,
    func_ov028_020e8b68,
};

void func_ov028_020e82d0(NoiseReportState* state) {
    s32 idx = func_02007278();
    if (idx == 0x7fffffff) {
        func_ov028_020e8b68(state);
        return;
    }
    data_ov028_020ecf24[idx](state);
}

void func_ov028_020e8310(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
        func_02006380();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 1024);
        func_02037264(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 1024);
        func_020372b8(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 1024);
        func_02037108(&data_02066aec, 0, 0x200);
        func_020371b4(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 1024);
        func_0203715c(&data_02066eec, 0, 0x200);
        func_0203720c(&data_020670ec, 0, 0x200);
        func_02001b44(2, NULL, &data_020672ec, 0x400);
    }
}

void func_ov028_020e83f8(void) {
    /* Not yet implemented */
}

void func_ov028_020e87f4(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov028_020e8808(void) {
    func_02026180(0, 0, 0x1000);
    if (func_0202623c() == FALSE) {
        func_02007328();
    }
}

void func_ov028_020e8830(NoiseReportState* state) {
    if (state->unk_2164C != 0) {
        func_02007328();
    }
}

void func_ov028_020e8850(void) {
    func_02026180(0, 0x10, 0x1000);
    if (func_0202623c() == FALSE) {
        func_02007328();
    }
}

void func_ov028_020e8878(NoiseReportState* state) {
    /* Not yet implemented */
}

void func_ov028_020e89d0(NoiseReportState* state) {
    /* Not yet implemented */
}

void func_ov028_020e8b68(NoiseReportState* state) {
    func_ov028_020e7f00(&state->unk_21590, &state->unk_21610);
    func_0200f4f8(&state->unk_21590);
    func_0200cef0(NULL);
    func_ov028_020e87f4();
    Mem_Free(&gDebugHeap, state);
}
