#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/Input.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/ResourceMgr.h"
#include "common_data.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00000 */ TaskPool        taskPool;
    /* 0x00080 */ Heap            heap;
    /* 0x0008C */ u8              memBuffer[0x4000];
    /* 0x0408C */ ResourceManager resources;
} LocaTitleSeq; // Size: 0x1560C

extern void FO3D_InitDisplay(void);
extern void FO3D_ExitDisplay(void);
extern void FO3D_FadeInStep(void);
extern void FO3D_FadeOutStep(void);
extern s32  LocaTitle_CreateTask(TaskPool* pool, s32 dataType, s32 packIndex);
extern void func_02001b44(s32 arg0, s32 arg1, void* arg2, s32 arg3);

s32 data_ov030_02109de8;

typedef struct {
    /* 0x0 */ s32         spawnNext;
    /* 0x4 */ const char* sequence;
    /* 0x8 */ s32         flip;
    /* 0xC */ s32         packIndex;
} LocaTitleSeqState; // Size: 0x10

static LocaTitleSeqState LocaTitleSeq_State = {1, "Seq_LocaTitle(void *)", -16, 0x28};

void func_ov030_020d2a80(LocaTitleSeq* seq) {
    if (LocaTitleSeq_State.spawnNext != 0) {
        data_ov030_02109de8          = LocaTitle_CreateTask(&seq->taskPool, 1, LocaTitleSeq_State.packIndex);
        LocaTitleSeq_State.spawnNext = 0;
        if (LocaTitleSeq_State.flip == 0) {
            LocaTitleSeq_State.flip = -16;
        } else {
            LocaTitleSeq_State.flip = 0;
        }
    }
    if (EasyTask_ValidateTaskId(&seq->taskPool, &data_ov030_02109de8) == FALSE) {
        LocaTitleSeq_State.packIndex--;
        if (LocaTitleSeq_State.packIndex < 0) {
            LocaTitleSeq_State.packIndex = 0x29;
        }
        LocaTitleSeq_State.spawnNext = 1;
    }
}

void LocaTitleSeq_Main(LocaTitleSeq* seq) {
    func_ov030_020d2a80(seq);
    if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        DebugOvlDisp_Pop();
    }
}

void LocaTitleSeq_Init(LocaTitleSeq* seq) {
    if (seq == NULL) {
        const char* sequence = LocaTitleSeq_State.sequence;

        seq = Mem_AllocHeapTail(&gDebugHeap, sizeof(LocaTitleSeq));
        Mem_SetSequence(&gDebugHeap, seq, sequence);
        MainOvlDisp_SetCbArg(seq);
    }
    MI_CpuFill(0, seq, Mem_GetBlockSize(&gDebugHeap, seq));
    FO3D_InitDisplay();
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = -16;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = -16;
    ResourceMgr_ReinitManagers(&seq->resources);
    Mem_InitializeHeap(&seq->heap, seq->memBuffer, sizeof(seq->memBuffer));
    EasyTask_InitializePool(&seq->taskPool, &seq->heap, 0x40, NULL, NULL);
    EasyTask_CreateTask(&seq->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)FO3D_FadeOutStep, seq, 0);
    DebugOvlDisp_Push((OverlayCB)LocaTitleSeq_Main, seq, 0);
    DebugOvlDisp_Push((OverlayCB)FO3D_FadeInStep, seq, 0);
    MainOvlDisp_NextProcessStage();
}

void LocaTitleSeq_Update(LocaTitleSeq* seq) {
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    DebugOvlDisp_Run();
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
    }
    EasyTask_UpdatePool(&seq->taskPool);
    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[2], NULL);
    func_02001b44(2, 0, &data_020672ec, 0x400);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void LocaTitleSeq_Destroy(LocaTitleSeq* seq) {
    if (seq != NULL) {
        FO3D_ExitDisplay();
        ResourceMgr_ReinitManagers(NULL);
        Mem_Free(&gDebugHeap, seq);
        MainOvlDisp_SetCbArg(NULL);
    }
}

static const OverlayProcess OvlProc_LocaTitleSeq = {
    .init = LocaTitleSeq_Init,
    .main = LocaTitleSeq_Update,
    .exit = LocaTitleSeq_Destroy,
};

void ProcessOverlay_LocaTitleSeq(void* object) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        LocaTitleSeq_Destroy(object);
    } else {
        OvlProc_LocaTitleSeq.funcs[stage](object);
    }
}
