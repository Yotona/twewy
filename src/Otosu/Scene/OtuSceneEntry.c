#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Otosu/OtosuGame.h"
#include "Otosu/OtosuMenu.h"
#include "Save.h"

static void OtuScene_ProcInitEditor(void);
static void OtuScene_ProcInitSinglePlayer(void);
static void OtuScene_ProcInitMultiplayer(void);
static void OtuScene_ProcMainSinglePlayer(TinPinSlammer_Scene* scene);
static void OtuScene_ProcMainMultiplayer(TinPinSlammer_Scene* scene);
static void OtuScene_ProcExitSinglePlayer(TinPinSlammer_Scene* scene);
static void OtuScene_ProcExitMultiplayer(TinPinSlammer_Scene* scene);

u8  OtuScene_SyncStep;
s32 OtuScene_InputStalled;

static const char* OtuScene_SequenceNames[3] = {"Seq_Otosu()", "Seq_Otosu()", "Seq_Otosu()"};

static const OtuStageHandler OtuScene_SinglePlayerProcs[3] = {
    OtuScene_ProcInitSinglePlayer,
    OtuScene_ProcMainSinglePlayer,
    OtuScene_ProcExitSinglePlayer,
};

static const OtuStageHandler OtuScene_MultiplayerProcs[3] = {
    OtuScene_ProcInitMultiplayer,
    OtuScene_ProcMainMultiplayer,
    OtuScene_ProcExitMultiplayer,
};

const OtuStageHandler OtuScene_EditorProcs[3] = {
    OtuScene_ProcInitEditor,
    OtuScene_ProcMainSinglePlayer,
    OtuScene_ProcExitSinglePlayer,
};

static void OtuScene_ProcInitEditor(void) {
    const char*          sequenceName = OtuScene_SequenceNames[2];
    TinPinSlammer_Scene* scene        = Mem_AllocHeapTail(&gDebugHeap, sizeof(TinPinSlammer_Scene));

    Mem_SetSequence(&gDebugHeap, scene, sequenceName);
    MainOvlDisp_SetCbArg(scene);
    Mem_InitializeHeap(&scene->heap, &scene->heapBuffer, 0x30000);

    scene->spareDataType = DatMgr_AllocateSlot();
    scene->dataType      = DatMgr_AllocateSlot();

    OtuScene_InputStalled = 0;
    OtuScene_SyncStep     = 0;
    scene->linkLost       = 0;
    scene->linkTimeout    = 0;
    scene->linkOpen       = 0;

    OtuScene_InitDisplay();
    scene->prevResMgr = ResourceMgr_ReinitManagers(&scene->resMgr);
    EasyTask_InitializePool(&scene->pool1, &scene->heap, 8, NULL, NULL);
    EasyTask_InitializePool(&scene->pool2, &scene->heap, 0x180, NULL, NULL);
    func_0200d8f0();

    data_02066aec                            = 0;
    data_02066eec                            = 0;
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;

    OtuScene_Init(scene);
    OtuScene_InitSinglePlayer(scene);

    scene->matchIndex = 0;

    OtuScene_LoadPlayerDeck(scene);
    OtuStageDispatch_SetNext(&scene->stage, &OtuScene_EditorStage);
    MainOvlDisp_NextProcessStage();
}

static void OtuScene_ProcInitSinglePlayer(void) {
    const char*          sequenceName = OtuScene_SequenceNames[0];
    TinPinSlammer_Scene* scene        = Mem_AllocHeapTail(&gDebugHeap, sizeof(TinPinSlammer_Scene));

    Mem_SetSequence(&gDebugHeap, scene, sequenceName);

    MainOvlDisp_SetCbArg(scene);
    Mem_InitializeHeap(&scene->heap, &scene->heapBuffer, 0x30000);

    scene->spareDataType = DatMgr_AllocateSlot();
    scene->dataType      = DatMgr_AllocateSlot();

    OtuScene_InputStalled = 0;
    OtuScene_SyncStep     = 0;
    scene->linkLost       = 0;
    scene->linkTimeout    = 0;
    scene->linkOpen       = 0;

    OtuScene_InitDisplay();
    scene->prevResMgr = ResourceMgr_ReinitManagers(&scene->resMgr);
    EasyTask_InitializePool(&scene->pool1, &scene->heap, 8, NULL, NULL);
    EasyTask_InitializePool(&scene->pool2, &scene->heap, 0x180, NULL, NULL);
    func_0200d8f0();

    data_02066aec                            = 0;
    data_02066eec                            = 0;
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;

    OtuScene_Init(scene);
    OtuScene_InitSinglePlayer(scene);

    scene->matchIndex = gSaveData.unk_341C;

    OtuScene_LoadPlayerDeck(scene);
    OtuStageDispatch_SetNext(&scene->stage, &OtuScene_SinglePlayerBoard);
    MainOvlDisp_NextProcessStage();
}

// Nonmatching: the two halfword clears are scheduled differently
static void OtuScene_ProcInitMultiplayer(void) {
    const char*          sequenceName = OtuScene_SequenceNames[1];
    TinPinSlammer_Scene* scene        = Mem_AllocHeapTail(&gDebugHeap, sizeof(TinPinSlammer_Scene));
    OtuPinTune           swap;
    u16                  linkStatus;
    BOOL                 wirelessStatus;

    Mem_SetSequence(&gDebugHeap, scene, sequenceName);

    MainOvlDisp_SetCbArg(scene);
    Mem_InitializeHeap(&scene->heap, &scene->heapBuffer, 0x30000);

    scene->spareDataType = DatMgr_AllocateSlot();
    scene->dataType      = DatMgr_AllocateSlot();

    OtuScene_InputStalled = 0;
    OtuScene_SyncStep     = 0;
    scene->linkLost       = 0;
    scene->linkTimeout    = 0;
    scene->linkOpen       = 0;

    OtuScene_InitDisplayWireless();
    scene->prevResMgr = ResourceMgr_ReinitManagers(&scene->resMgr);
    EasyTask_InitializePool(&scene->pool1, &scene->heap, 8, NULL, NULL);
    EasyTask_InitializePool(&scene->pool2, &scene->heap, 0x180, NULL, NULL);
    func_0200d8f0();

    data_02066aec                            = 0;
    data_02066eec                            = 0;
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;

    OtuScene_Init(scene);
    OtuScene_InitMultiplayer(scene);

    linkStatus               = SystemStatusFlags.unk_06;
    SystemStatusFlags.unk_06 = 0;
    scene->linkStatus        = linkStatus;
    wirelessStatus           = SystemStatusFlags.unk_07 != 0;
    SystemStatusFlags.unk_07 = 0;
    scene->wirelessStatus    = wirelessStatus;

    OtuScene_LoadPlayerDeck(scene);

    scene->boardIndex  = gSaveData.otosuBoard;
    scene->playerCount = gSaveData.otosuPlayerCount;

    swap            = OtuPinTunes[10];
    OtuPinTunes[10] = OtuPinTunes[11];
    OtuPinTunes[11] = swap;

    if (gSaveData.otosuGameKey == 0) {
        OtuStageDispatch_SetNext(&scene->stage, &OtuScene_HostBoard);
    } else {
        MI_CpuCopyU8(gSaveData.otosuParentBssid, scene->parentBssid, sizeof(scene->parentBssid));
        OtuStageDispatch_SetNext(&scene->stage, &OtuScene_GuestBoard);
    }

    MainOvlDisp_NextProcessStage();
}

static void OtuScene_ProcMainSinglePlayer(TinPinSlammer_Scene* scene) {
    OverlayTag menuTag;
    OverlayTag resultTag;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    OamMgr_ResetCommandQueues(&g_OamMgr[1]);

    EasyTask_ProcessPendingTasks(&scene->pool1);
    if (scene->playing != 0) {
        EasyTask_ProcessPendingTasks(&scene->pool2);
    }

    OtuScene_RunStages(scene);

    EasyTask_UpdateActiveTasks(&scene->pool1);
    EasyTask_UpdateActiveTasks(&scene->pool2);

    if (scene->done != 0) {
        if (gSaveData.unk_341C != 0x1F) {
            MainOvlDisp_ReplaceTop(&menuTag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_SinglePlayerRanking, NULL, 0);
        } else {
            MainOvlDisp_ReplaceTop(&resultTag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_RoleSelection, NULL, 0);
        }
    }

    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[0]);
    OamMgr_FlushCommands(&g_OamMgr[1]);
    PaletteMgr_Flush(g_PaletteManagers[0], NULL);
    PaletteMgr_Flush(g_PaletteManagers[1], NULL);
    PaletteMgr_Flush(g_PaletteManagers[2], NULL);
    func_0200d90c();
}

static void OtuScene_ProcMainMultiplayer(TinPinSlammer_Scene* scene) {
    OverlayTag linkTag;
    OverlayTag resultTag;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    OamMgr_ResetCommandQueues(&g_OamMgr[1]);

    if (OtuScene_InputStalled == 0) {
        EasyTask_ProcessPendingTasks(&scene->pool1);
        if (scene->playing != 0) {
            EasyTask_ProcessPendingTasks(&scene->pool2);
        }

        OtuScene_RunStagesWireless(scene);
    }

    EasyTask_UpdateActiveTasks(&scene->pool1);
    EasyTask_UpdateActiveTasks(&scene->pool2);

    if (scene->done != 0) {
        if (scene->linkError == 0) {
            MainOvlDisp_ReplaceTop(&linkTag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_MultiplayerRanking, NULL, 0);
        } else {
            MainOvlDisp_ReplaceTop(&resultTag, &OVERLAY_2_ID, ProcessOverlay_OtosuMenu_ConnectionError, NULL, 0);
        }
    }

    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[0]);
    OamMgr_FlushCommands(&g_OamMgr[1]);
    PaletteMgr_Flush(g_PaletteManagers[0], NULL);
    PaletteMgr_Flush(g_PaletteManagers[1], NULL);
    PaletteMgr_Flush(g_PaletteManagers[2], NULL);
    func_0200d90c();
}

static void OtuScene_Destroy(TinPinSlammer_Scene* scene) {
    OtuScene_ClearVBlank();
    OtuScene_DeleteFade(scene);
    EasyTask_DestroyPool(&scene->pool1);
    EasyTask_DestroyPool(&scene->pool2);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(scene->spareDataType);
    DatMgr_ClearSlot(scene->dataType);
    Mem_Free(&gDebugHeap, scene);
}

static void OtuScene_ProcExitSinglePlayer(TinPinSlammer_Scene* scene) {
    OtuScene_Destroy(scene);
}

static void OtuScene_ProcExitMultiplayer(TinPinSlammer_Scene* scene) {
    SystemStatusFlags;
    SystemStatusFlags.unk_06 = scene->linkStatus;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = scene->wirelessStatus != 0;

    OtuScene_Destroy(scene);
}

void ProcessOverlay_OtosuGame_SinglePlayer(TinPinSlammer_Scene* scene) {
    s32 stage = MainOvlDisp_GetProcessStage();

    if (stage == 0x7FFFFFFF) {
        OtuScene_ProcExitSinglePlayer(scene);
        return;
    }

    OtuScene_SinglePlayerProcs[stage](scene);
}

void ProcessOverlay_OtosuGame_Multiplayer(TinPinSlammer_Scene* scene) {
    s32 stage = MainOvlDisp_GetProcessStage();

    if (stage == 0x7FFFFFFF) {
        OtuScene_ProcExitMultiplayer(scene);
        return;
    }

    OtuScene_MultiplayerProcs[stage](scene);
}
