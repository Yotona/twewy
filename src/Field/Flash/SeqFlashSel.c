#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/Str.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Text.h"
#include "Field/FlashPlayer.h"
#include "Field/FlashRegion.h"
#include "Save.h"
#include "common_data.h"

typedef struct {
    /* 0x0 */ s32         count;
    /* 0x4 */ const void* movies;
    /* 0x8 */ const char* name;
} FlashSequence; // Size: 0xC

#define FLASH_SEQUENCE_COUNT 83

typedef struct {
    /* 0x00000 */ ResourceManager  resources;
    /* 0x11580 */ ResourceManager* previousResources;
    /* 0x11584 */ s32              dataSlot;
    /* 0x11588 */ char             unk_11588[0x1000C];
    /* 0x21594 */ s32              textBg;
    /* 0x21598 */ char             unk_21598[0x8];
    /* 0x215A0 */ TextObject       text;
    /* 0x216B4 */ char             unk_216B4[0x219B0 - 0x216B4];
} FlashSelectOverlayState; // Size: 0x219B0

extern const FlashSequence data_ov030_020fda64[];

extern void func_02010b50(TextObject* textObj);
extern void func_02025b68(void* textBg, UnkStruct_usedby_02025b68* params);
extern void func_02025e30(void* textBg);
extern void func_ov030_020d4f74(void* state);

void func_ov030_020d51fc(FlashSelectOverlayState* state);
void func_ov030_020d52bc(FlashSelectOverlayState* state);
void func_ov030_020d533c(FlashSelectOverlayState* state);

char* data_ov030_02109bc4     = "Seq_FlashSel()";
char  data_ov030_02109bd8[16] = "Flash select";

const OverlayProcess OvlProc_FlashSel = {
    .init = func_ov030_020d51fc,
    .main = func_ov030_020d52bc,
    .exit = func_ov030_020d533c,
};

void func_ov030_020d4fb4(FlashSelectOverlayState* state) {
    UnkStruct_usedby_02025b68 params;

    params.unk_00 = 1;
    params.unk_04 = 0;
    params.unk_08 = state->dataSlot;
    params.unk_0C = (s32)&data_0205c9b0;
    params.unk_10 = 0;
    params.unk_14 = NULL;
    params.unk_18 = 0;
    params.unk_1A = 0;
    params.unk_1C = 0x20;
    params.unk_1E = 0x18;
    func_02025b68(&state->textBg, &params);
    Text_RenderToScreen(&state->text, 0, 0, data_ov030_02109bd8);
}

void func_ov030_020d503c(FlashSelectOverlayState* state) {
    func_02010b50(&state->text);
    Text_RenderToScreen(&state->text, 0, 0, data_ov030_02109bd8);
    if (gSaveData.unk_3124 >= 0 && gSaveData.unk_3124 < FLASH_SEQUENCE_COUNT) {
        Text_RenderToScreen(&state->text, 8, 0x10,
                            Str_SPrintf("flash:%d %s", gSaveData.unk_3124, data_ov030_020fda64[gSaveData.unk_3124].name));
    } else {
        Text_RenderToScreen(&state->text, 8, 0x10, Str_SPrintf("flash:%d", gSaveData.unk_3124));
    }
}

void func_ov030_020d50ec(FlashSelectOverlayState* state) {
    func_02025e30(&state->textBg);
}

void func_ov030_020d5100(FlashSelectOverlayState* state) {
    func_ov030_020d4fb4(state);
}

void func_ov030_020d510c(FlashSelectOverlayState* state) {
    func_ov030_020d50ec(state);
}

void func_ov030_020d5118(s32* value, s16 count) {
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        *value += 1;
        if (*value >= count) {
            *value = 0;
        }
    } else if (SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) {
        *value -= 1;
        if (*value < 0) {
            *value = (s16)(count - 1);
        }
    }
}

void func_ov030_020d5174(FlashSelectOverlayState* state) {
    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) {
        OverlayTag tag;
        gSaveData.unk_3128 = 4;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020d4f74, NULL, PROCESS_STAGE_INIT);
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
    } else {
        func_ov030_020d5118(&gSaveData.unk_3124, FLASH_SEQUENCE_COUNT);
    }
    func_ov030_020d503c(state);
}

// Nonmatching
void func_ov030_020d51fc(FlashSelectOverlayState* state) {
    const char* sequence = data_ov030_02109bc4;

    state = Mem_AllocHeapTail(&gDebugHeap, sizeof(FlashSelectOverlayState));
    Mem_SetSequence(&gDebugHeap, state, sequence);
    MainOvlDisp_SetCbArg(state);
    state->dataSlot = DatMgr_AllocateSlot();
    if (gSaveData.unk_3124 >= FLASH_SEQUENCE_COUNT) {
        gSaveData.unk_3124 = 0;
    }
    func_ov030_020d57c8();
    g_DisplaySettings.controls[DISPLAY_MAIN].layers     = 0;
    state->previousResources                            = ResourceMgr_ReinitManagers(&state->resources);
    data_02066aec                                       = 0;
    data_02066eec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;
    func_ov030_020d5100(state);
    MainOvlDisp_NextProcessStage();
}

void func_ov030_020d52bc(FlashSelectOverlayState* state) {
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    func_ov030_020d5174(state);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void func_ov030_020d533c(FlashSelectOverlayState* state) {
    func_ov030_020d510c(state);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->dataSlot);
    Mem_Free(&gDebugHeap, state);
}

void func_ov030_020d5370(FlashSelectOverlayState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov030_020d533c(state);
    } else {
        OvlProc_FlashSel.funcs[stage](state);
    }
}
