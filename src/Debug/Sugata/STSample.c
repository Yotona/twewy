#include "Debug/STSample.h"
#include "Display.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "OverlayManager.h"
#include "System.h"
#include "common_data.h"
#include <NitroSDK/os/cache.h>

extern s32 DAT_0206aae0;
extern s32 DAT_0206b3d0;

char* data_ov041_02083020 = "Seq_STSample()";

void func_ov041_02082ff0(void);

u32 func_ov041_020824a0(STSampleState* state, int param_2, u32 param_3) {
    /* Not yet implemented */
}

void func_ov041_02082600(STSampleState* state, s32 param_2, u16* param_3, u32 param_4) {
    /* Not yet implemented */
}

void func_ov041_02082730(STSampleState* state) {
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
    Text_RenderToScreen(&state->unk_215A0, 0, 0, "‚·‚ê‚¿‚ª‚¢ƒTƒ“ƒvƒ‹"); // "StreetPass Sample"
}

void func_ov041_020827b8(STSampleState* state) {
    func_02025e30(&state->unk_21594);
}

/* Nonmatching */
void func_ov041_020827cc(STSampleState* state) {
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
    GetDsFirmwareUserSettings(abStack_5c);
    MI_CpuSet(&state->unk_21F36, 0, 0x16);
    func_ov040_0209da10(&state->unk_21F36, auStack_58, local_42);
    MI_CpuSet(&state->unk_21F4C, 0, 0x36);
    func_ov040_0209da10(&state->unk_21F4C, auStack_40, local_a);

    for (s32 idx = 0; idx < 0x100; idx++) {
        state->unk_21F82[idx] = idx;
    }

    MI_CpuSet(&state->unk_21E6C, 0xff, 0xc0);
    MI_CpuSet(&state->unk_2208C, 0, 0x152);
}

void func_ov041_020828f0(STSampleState* state) {
    func_ov041_020827b8(state);
    func_ov040_0209e028(&state->unk_219B0);
}

/* Nonmatching */
void STSample_ControlMenu(STSampleState* state) {
    s32        idx;
    u8*        puVar2;
    OverlayTag tag;

    switch (SysControl.buttonState.pressedButtons) {
        case INPUT_BUTTON_A:
            if (state->unk_21E60 == 0) {
                func_ov040_0209e888(&state->unk_219B0);
            }
            break;
        case INPUT_BUTTON_B:
            if (state->unk_21E60 != 0) {
                func_ov040_0209e900(&state->unk_219B0);
            }
            break;
        case INPUT_BUTTON_X:
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
        Text_RenderToScreen(&state->unk_215A0, 8, (state->unk_22088 + 1) * 8, "ƒf[ƒ^ŽóM"); // "Data Received"
        state->unk_22088++;
    }

    state->unk_21E60 = func_ov040_0209e03c(&state->unk_219B0);
    if (state->unk_21E60 != 0) {
        return;
    }

    // Return to main debug menu
    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        MainOvlDisp_Pop(&tag);
    }
}

/* Nonmatching */
void func_ov041_02082a5c(STSampleState* param) {
    int            iVar1;
    STSampleState* state;
    void*          sVar2;

    OvlMgr_LoadOverlay(3, OVERLAY_40_ID);
    iVar1 = data_ov041_02083020;
    state = Mem_AllocHeapTail(&gDebugHeap, sizeof(STSampleState));
    Mem_SetSequence(&gDebugHeap, state, iVar1);
    sVar2 = MainOvlDisp_SetCbArg(&state);
    DatMgr_AllocateSlot();
    state->unk_11584 = sVar2;
    func_ov041_02082ff0();
    state->unk_11580                                    = func_0200cef0(state);
    data_02066aec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;
    data_02066eec                                       = 0;
    func_ov041_020827cc(state);
    MainOvlDisp_NextProcessStage();
}

void func_ov041_02082b08(STSampleState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    STSample_ControlMenu(state);
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

void func_ov041_02082b88(STSampleState* state) {
    func_ov041_020828f0(state);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    Mem_Free(&gDebugHeap, state);
    OvlMgr_UnloadOverlay(3);
}

static const OverlayProcess OvlProc_STSample = {
    .init = func_ov041_02082a5c,
    .main = func_ov041_02082b08,
    .exit = func_ov041_02082b88,
};

void func_ov041_02082bc4(STSampleState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov041_02082b88(state);
        return;
    }
    OvlProc_STSample.funcs[stage](state);
}

/* Nonmatching */
void func_ov041_02082c04(void) {
    /* Not yet implemented */
}

void STSample_InterruptCallback(void) {
    if (SystemStatusFlags.vblank != FALSE) {
        func_ov040_0209e91c();
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void func_ov041_02082ff0(void) {
    func_ov041_02082c04();
    Interrupts_RegisterVBlankCallback(STSample_InterruptCallback, 1);
}
