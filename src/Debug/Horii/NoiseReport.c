#include "EasyTask.h"
#include "Memory.h"
#include "System.h"
#include "common_data.h"

extern s32 data_020672ec;

typedef struct {
    /* 0x00000 */ char     unk_00000[0x21590];
    /* 0x21590 */ TaskPool unk_21590;
    /* 0x21610 */ s32      unk_21610;
    /* 0x21614 */ char     unk_21614[0x38];
    /* 0x2164C */ u16      unk_2164C;
    /* 0x2164E */ char     unk_2164E[2];
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
    s32 idx = MainOvlDisp_GetRepeatCount();
    if (idx == 0x7fffffff) {
        func_ov028_020e8b68(state);
        return;
    }
    data_ov028_020ecf24[idx](state);
}

void func_ov028_020e8310(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 1024);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 1024);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 1024);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 1024);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
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
        DebugOvlDisp_Pop();
    }
}

void func_ov028_020e8830(NoiseReportState* state) {
    if (state->unk_2164C != 0) {
        DebugOvlDisp_Pop();
    }
}

void func_ov028_020e8850(void) {
    func_02026180(0, 0x10, 0x1000);
    if (func_0202623c() == FALSE) {
        DebugOvlDisp_Pop();
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
    EasyTask_DestroyPool(&state->unk_21590);
    func_0200cef0(NULL);
    func_ov028_020e87f4();
    Mem_Free(&gDebugHeap, state);
}
