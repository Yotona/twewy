#include "Field/FieldRegion.h"

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
#include "Field/Flash/FlashGraphicsInit.h"
#include "Field/Flash/SeqFlash.h"
#include "Field/Flash/SeqFlashSel.h"
#include "Save.h"
#include "common_data.h"

extern void func_02010b50(TextObject* textObj);
extern void BgText_Init(void* textBg, UnkStruct_usedby_02025b68* params);
extern void BgText_Free(void* textBg);

void SeqFlashSel_Init(FlashSelectOverlayState* state);
void SeqFlashSel_Update(FlashSelectOverlayState* state);
void SeqFlashSel_Destroy(FlashSelectOverlayState* state);

char* SeqFlashSel_Sequence  = "Seq_FlashSel()";
char  SeqFlashSel_Title[16] = "Flash select";

const OverlayProcess OvlProc_FlashSel = {
    .init = SeqFlashSel_Init,
    .main = SeqFlashSel_Update,
    .exit = SeqFlashSel_Destroy,
};

void SeqFlashSel_InitText(FlashSelectOverlayState* state) {
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
    BgText_Init(&state->textBg, &params);
    Text_RenderToScreen(&state->text, 0, 0, SeqFlashSel_Title);
}

void SeqFlashSel_DrawText(FlashSelectOverlayState* state) {
    func_02010b50(&state->text);
    Text_RenderToScreen(&state->text, 0, 0, SeqFlashSel_Title);
    if (gSaveData.flashSequence >= 0 && gSaveData.flashSequence < FLASH_SEQUENCE_COUNT) {
        Text_RenderToScreen(&state->text, 8, 0x10,
                            Str_SPrintf("flash:%d %s", gSaveData.flashSequence, FlashSequences[gSaveData.flashSequence].name));
    } else {
        Text_RenderToScreen(&state->text, 8, 0x10, Str_SPrintf("flash:%d", gSaveData.flashSequence));
    }
}

void SeqFlashSel_ReleaseText(FlashSelectOverlayState* state) {
    BgText_Free(&state->textBg);
}

void SeqFlashSel_CreateScreen(FlashSelectOverlayState* state) {
    SeqFlashSel_InitText(state);
}

void SeqFlashSel_DestroyScreen(FlashSelectOverlayState* state) {
    SeqFlashSel_ReleaseText(state);
}

void SeqFlashSel_StepValue(s32* value, s16 count) {
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

void SeqFlashSel_HandleInput(FlashSelectOverlayState* state) {
    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) {
        OverlayTag tag;
        gSaveData.flashExitMode = 4;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, ProcessOverlay_Flash, NULL, PROCESS_STAGE_INIT);
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
    } else {
        SeqFlashSel_StepValue(&gSaveData.flashSequence, FLASH_SEQUENCE_COUNT);
    }
    SeqFlashSel_DrawText(state);
}

void SeqFlashSel_Init(FlashSelectOverlayState* state) {
    const char* sequence = SeqFlashSel_Sequence;

    state = Mem_AllocHeapTail(&gDebugHeap, sizeof(FlashSelectOverlayState));
    Mem_SetSequence(&gDebugHeap, state, sequence);
    MainOvlDisp_SetCbArg(state);
    state->dataSlot = DatMgr_AllocateSlot();
    if (gSaveData.flashSequence >= FLASH_SEQUENCE_COUNT) {
        gSaveData.flashSequence = 0;
    }
    FlashGfx_Init();
    g_DisplaySettings.controls[DISPLAY_MAIN].layers     = 0;
    state->previousResources                            = ResourceMgr_ReinitManagers(&state->resources);
    g_PaletteBuffers[DISPLAY_MAIN].bg[0]                = 0;
    g_PaletteBuffers[DISPLAY_SUB].bg[0]                 = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;
    SeqFlashSel_CreateScreen(state);
    MainOvlDisp_NextProcessStage();
}

void SeqFlashSel_Update(FlashSelectOverlayState* state) {
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    SeqFlashSel_HandleInput(state);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void SeqFlashSel_Destroy(FlashSelectOverlayState* state) {
    SeqFlashSel_DestroyScreen(state);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->dataSlot);
    Mem_Free(&gDebugHeap, state);
}

void ProcessOverlay_FlashSel(FlashSelectOverlayState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        SeqFlashSel_Destroy(state);
    } else {
        OvlProc_FlashSel.funcs[stage](state);
    }
}
