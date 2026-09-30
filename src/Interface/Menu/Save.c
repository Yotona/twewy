#include "Interface/Menu/Save.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"
#include "Util/SysFont.h"
#include "common_data.h"
#include <nitro/fs/overlay.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>

typedef struct {
    /* 0x00000 */ MenuStateBase     base;
    /* 0x21618 */ s32               taskId_Icon;
    /* 0x2161C */ s32               taskId_NameD;
    /* 0x21620 */ s32               taskId_Chara;
    /* 0x21624 */ s32               taskId_SaveBtn;
    /* 0x21628 */ s32               taskId_Partner;
    /* 0x2162C */ s32               taskId_TextScr;
    /* 0x21630 */ s32               taskId_HelpCur;
    /* 0x21634 */ s32               taskId_NameU;
    /* 0x21638 */ s32               taskId_TextScrU;
    /* 0x2163C */ s32               taskId_ItemU[9];
    /* 0x21660 */ s32               taskId_BdgU[6];
    /* 0x21678 */ s32               taskId_SlotCoverU[6];
    /* 0x21690 */ s32               taskId_HelpCurU;
    /* 0x21694 */ s32               exitReady;
    /* 0x21698 */ s16               timer;
    /* 0x2169A */ u16               unk_2169A;
    /* 0x2169C */ s16               unk_2169C;
    /* 0x216A0 */ GlobalFriendData* friendData;
    /* 0x216A4 */ SaveMenuObject    save;
} SaveMenuState; // Size: 0x21934

void GX_LoadBgPltt(void* src, u32 offset, u32 size);
void GX_LoadObjPltt(void* src, u32 offset, u32 size);
void GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void GXs_LoadObjPltt(void* src, u32 offset, u32 size);
void func_0202b878(void);
BOOL func_02001b44(s32, s32, void*, s32);
s32  Inventory_IsHelpSeen(s32);
void Inventory_SetHelpSeen(s32);

extern void ProcessOverlay_MenuTop();
extern u32  OVERLAY_42_ID;

// Where Save_Update goes for each nextProcess value. JP has no counterpart to USA's ov036, so every later
// overlay number is one lower there, and the entry points of the other overlays moved.
#ifdef REGION_USA
    #define SAVE_OVL_MENU    OVERLAY_43_ID
    #define SAVE_NEXT1_OVL   OVERLAY_30_ID
    #define SAVE_NEXT1_ENTRY ((void*)0x020B0FE8)
    #define SAVE_NEXT2_OVL   OVERLAY_37_ID
    #define SAVE_NEXT2_ENTRY ((void*)0x0208370C)
    #define SAVE_NEXT3_ENTRY ((void*)0x02086B0C)
    #define SAVE_NEXT4_ENTRY ((void*)0x02086B4C)
    #define SAVE_NEXT5_ENTRY ((void*)0x02086A8C)
#else
    #define SAVE_OVL_MENU    OVERLAY_42_ID
    #define SAVE_NEXT1_OVL   OVERLAY_30_ID
    #define SAVE_NEXT1_ENTRY ((void*)0x020B16F0)
    #define SAVE_NEXT2_OVL   OVERLAY_36_ID
    #define SAVE_NEXT2_ENTRY ((void*)0x02083FB8)
    #define SAVE_NEXT3_ENTRY ((void*)0x020873AC)
    #define SAVE_NEXT4_ENTRY ((void*)0x020873EC)
    #define SAVE_NEXT5_ENTRY ((void*)0x0208736C)
#endif

void Save_Init(SaveMenuState* state);
void Save_Update(SaveMenuState* state);
void Save_Destroy(SaveMenuState* state);
void Save_StageFirstHelp(SaveMenuState* state);
void Save_StageMain(SaveMenuState* state);
void Save_StageStartSave(SaveMenuState* state);
void Save_StageSaving(SaveMenuState* state);
void Save_StageSaved(SaveMenuState* state);
void Save_StageHelp(SaveMenuState* state);
void Save_StageCloseHelp(SaveMenuState* state);
void Save_StageFadeOut(SaveMenuState* state);
void Save_StageFadeIn(SaveMenuState* state);
void Save_RegisterVBlank(void);
void Save_DeregisterVBlank(void);

static const char* Save_SequenceName     = "Seq_Save()";
static char        Save_FriendDataName[] = "GlobalFriendData";

static const OverlayProcess OvlProc_Save = {
    .init = (OverlayCB)Save_Init,
    .main = (OverlayCB)Save_Update,
    .exit = (OverlayCB)Save_Destroy,
};

// The owning overlay's id (JP has no ov036, so the menu overlay is one lower there).
#ifdef REGION_USA
    #define SAVE_BIN_ID 43
#else
    #define SAVE_BIN_ID 42
#endif

// clang-format off
// Listed out of index order: this order makes the heapsorted .data string literals land where the
// original has them (see mwcc-section-order-heapsort).
const BinIdentifier Save_BinIdentifiers[17] = {
    [13] = {SAVE_BIN_ID, "Data/BadgeData.bin"},
    [14] = {SAVE_BIN_ID, "Apl_Tak/ItemData.bin"},
    [15] = {SAVE_BIN_ID, "Apl_Tak/FoodData.bin"},
    [11] = {SAVE_BIN_ID, "Apl_Tak/Grp_Item.bin"},
    [9]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Badge.bin"},
    [0]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Menu_BGD.bin"},
    [8]  = {SAVE_BIN_ID, "Apl_Tak/Grp_MenuIcon.bin"},
    [16] = {SAVE_BIN_ID, "Apl_Tak/TreasureData.bin"},
    [12] = {SAVE_BIN_ID, "Apl_Tak/Grp_DummyItem.bin"},
    [7]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Save_OBU00.bin"},
    [1]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Save_BGD00.bin"},
    [3]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Save_OBD00.bin"},
    [4]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Save_OBD01.bin"},
    [5]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Save_BGU00.bin"},
    [10] = {SAVE_BIN_ID, "Apl_Tak/Grp_DummyBadge.bin"},
    [2]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [6]  = {SAVE_BIN_ID, "Apl_Tak/Grp_Menu_fontSCR.bin"},
};
// clang-format on

SaveMenuState* Save_State;

void Save_InitState(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;
    u16             i;

    state->exitReady     = 0;
    state->timer         = 0;
    state->unk_2169A     = 0;
    state->unk_2169C     = 0;
    save->flags          = 0;
    save->unk_16C        = 0;
    save->nextProcess    = 0;
    save->iconPressed[0] = 0;
    save->iconPressed[1] = 0;
    save->iconTimer      = 0;
    save->saving         = FALSE;
    for (i = 0; i < 3; i++) {
        save->helpButtonPressed[i] = 0;
    }
    save->helpPage    = 0;
    save->helpOpen    = 0;
    save->bufferIndex = 0;
    Save_LoadCard(save);
}

void Save_CreateTasks(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;
    u16             i;

    state->taskId_Icon    = Save_icon_CreateTask(&state->base.taskPool, state->base.dataType, save);
    state->taskId_NameD   = Save_nameD_CreateTask(&state->base.taskPool, state->base.dataType, save);
    state->taskId_Chara   = Save_chara_CreateTask(&state->base.taskPool, state->base.dataType, save);
    state->taskId_SaveBtn = Save_saveBtn_CreateTask(&state->base.taskPool, state->base.dataType, save);
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) != 0 && save->partner != 0xFF) {
        state->taskId_Partner = Save_partner_CreateTask(&state->base.taskPool, state->base.dataType, save);
    }
    state->taskId_TextScr  = Save_textScr_CreateTask(&state->base.taskPool, state->base.dataType, save);
    state->taskId_NameU    = Save_nameU_CreateTask(&state->base.taskPool, state->base.dataType, save);
    state->taskId_TextScrU = Save_textScrU_CreateTask(&state->base.taskPool, state->base.dataType, save);
    for (i = 0; i < 9; i++) {
        state->taskId_ItemU[i] = Save_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i, save);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_BdgU[i] = Save_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, i, save);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_SlotCoverU[i] = Save_slotCoverU_CreateTask(&state->base.taskPool, state->base.dataType, i, save);
    }
}

void Save_ReloadCardTasks(SaveMenuObject* save, u16 partner) {
    SaveMenuState* state = Save_State;
    s16            i;
    s32            screenIndex;

    save->bufferIndex = 1 - save->bufferIndex;
    for (i = 0; i < 9; i++) {
        Save_itemU_ReleaseSprite(&state->base.taskPool, state->taskId_ItemU[i]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_ItemU[i]);
    }
    for (i = 0; i < 6; i++) {
        Save_bdgU_ReleaseSprite(&state->base.taskPool, state->taskId_BdgU[i]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgU[i]);
    }
    for (i = 0; i < 6; i++) {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_SlotCoverU[i]);
    }

    if (save->partner == 0xFF) {
        for (i = 0; i < 4; i++) {
            save->slots[i + 4] = &save->threads[i + 4];
        }
    } else {
        for (i = 0; i < 4; i++) {
            save->slots[i + 4] = &save->threads[i + (save->partner + 1) * 4];
        }
    }

    for (i = 0; i < 9; i++) {
        state->taskId_ItemU[i] = Save_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i, save);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_BdgU[i] = Save_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, i, save);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_SlotCoverU[i] = Save_slotCoverU_CreateTask(&state->base.taskPool, state->base.dataType, i, save);
    }

    if (save->partner == 0xFF) {
        Save_ReleaseBgResource(&save->resources[3], DISPLAY_SUB);
        Save_LoadBgResourceIndexed(&save->resources[3], DISPLAY_SUB, 3, 5, 1, 13, 8, 0);
    } else {
        if (save->partner == 0) {
            screenIndex = 0;
        } else if (save->partner == 1) {
            screenIndex = 4;
        } else {
            screenIndex = 5;
        }
        Save_ReleaseBgResource(&save->resources[3], DISPLAY_SUB);
        Save_LoadBgResourceIndexed(&save->resources[3], DISPLAY_SUB, 3, 5, 1, 13, screenIndex, save->partner);
    }
}

void Save_ChangePartner(SaveMenuObject* save, u16 partner) {
    SaveMenuState* state = Save_State;
    s16            i;

    for (i = 0; i < 4; i++) {
        Save_itemU_ReleaseSprite(&state->base.taskPool, state->taskId_ItemU[i + 4]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_ItemU[i + 4]);
    }
    for (i = 0; i < 4; i++) {
        save->slots[i + 4] = &save->threads[partner * 4 + 4 + i];
    }
    for (i = 0; i < 4; i++) {
        state->taskId_ItemU[i + 4] = Save_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i + 4, save);
    }
}

void Save_StageFadeIn(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading()) {
        return;
    }
    save->message = SYSMSG_SAVE_PROMPT;
    save->flags |= 1;
    if (gSaveData.unk_1AB4 & 0x80) {
        gSaveData.unk_1AB4 &= ~0x80;
        save->nextProcess = 2;
    } else if (gSaveData.unk_1AB4 & 2) {
        save->nextProcess = 1;
    } else {
        save->nextProcess = 0;
    }
    if (Inventory_IsHelpSeen(7) == 0) {
        state->timer = 30;
        DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageFirstHelp, state, PROCESS_STAGE_INIT);
    } else {
        DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void Save_StageFirstHelp(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    if (state->timer > 0) {
        state->timer--;
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG0;
    state->taskId_HelpCur  = Save_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, save);
    state->taskId_HelpCurU = Save_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, save);
    save->helpOpen         = 1;
    save->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageHelp, state, PROCESS_STAGE_INIT);
}

void Save_StageMain(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;
    TouchCoord      coord;
    s16             index;

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);

        if (Save_IsPointOnSaveBtn(coord.x, coord.y) == 1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            save->saving  = TRUE;
            save->message = SYSMSG_SAVE_SAVING;
            save->flags |= 1;
            save->flags |= 0x100;
            DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageStartSave, state, PROCESS_STAGE_INIT);
            return;
        }

        index = Save_GetIconAtPoint(coord.x, coord.y);
        if (index != -1) {
            save->iconPressed[index] = 1;
            save->iconTimer          = 12;
            if (index == 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG0;
                state->taskId_HelpCur  = Save_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, save);
                state->taskId_HelpCurU = Save_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, save);
                save->helpOpen         = 1;
                save->flags |= 2;
                DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageHelp, state, PROCESS_STAGE_INIT);
            } else {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
                save->flags |= 0x1000;
                DebugOvlDisp_Pop();
                return;
            }
        }
    }

    if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_L) && (InputStatus.buttonState.currButtons & INPUT_BUTTON_R)) {
        if (save->flags & 4) {
            return;
        }
        save->flags |= 4;
        save->flags |= 1;
    } else {
        if (!(save->flags & 4)) {
            return;
        }
        save->flags &= ~4;
        save->flags |= 1;
    }
}

void Save_StageStartSave(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    gSaveData.unk_1AB6 |= 1;
#ifdef REGION_USA
    Save_WriteSnapshot(save, save->prevChapter);
#else
    Save_WriteSnapshot(save);
#endif
    Savefile_ResetIOPipeline();
    FriendData_Set(state->friendData);
    DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageSaving, state, PROCESS_STAGE_INIT);
}

void Save_StageSaving(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;
    u16             errorFlags;

    if (Savefile_RunAlternatePipelineStep() != 1) {
        return;
    }
    errorFlags = Savefile_GetWriteErrorFlags();
    if (errorFlags == 0) {
        Save_LoadFromSave(save);
        save->helpOpen = 0;
        save->flags |= 2;
        Save_ReloadCardTasks(save, save->partner);
        save->message = SYSMSG_SAVE_COMPLETE;
        save->flags |= 1;
        save->saving = FALSE;
        state->timer = 120;
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        save->flags |= 1;
        DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageSaved, state, PROCESS_STAGE_INIT);
    } else if (errorFlags & 2) {
        save->nextProcess = 3;
        DebugOvlDisp_Pop();
    } else if (errorFlags & 4) {
        save->nextProcess = 4;
        DebugOvlDisp_Pop();
    } else {
        save->nextProcess = 5;
        DebugOvlDisp_Pop();
    }
}

void Save_StageSaved(SaveMenuState* state) {
    if (state->timer > 0) {
        state->timer--;
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
    SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
    DebugOvlDisp_Pop();
}

void Save_StageHelp(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;
    TouchCoord      coord;
    s16             index;

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);
        index = Save_GetHelpBtnAtPoint(coord.x, coord.y);
        if (index != -1) {
            if (save->iconTimer != 0) {
                return;
            }
            save->helpButtonPressed[index] = 1;
            save->iconTimer                = 12;
            if (index == 0) {
                if (save->helpPage != 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    save->helpPage--;
                    save->flags |= 2;
                }
            } else if (index == 1) {
                if (save->helpPage < 3) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    save->helpPage++;
                    save->flags |= 2;
                }
            } else if (Inventory_IsHelpSeen(7) == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageCloseHelp, state, PROCESS_STAGE_INIT);
            }
        }
    }

#ifdef REGION_USA
    if (Inventory_IsHelpSeen(7) == 0) {
#else
    // JP only marks the help as read once its last page has been reached.
    if (Inventory_IsHelpSeen(7) == 0 && save->helpPage == 3) {
#endif
        Inventory_SetHelpSeen(7);
    }
}

void Save_StageCloseHelp(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    if (save->helpButtonPressed[2] != 0) {
        return;
    }
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG1;
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG0;
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCur);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCurU);
    save->helpPage = 0;
    save->helpOpen = 0;
    save->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)Save_StageMain, state, PROCESS_STAGE_INIT);
}

void Save_StageFadeOut(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    if (save->nextProcess == 3 || save->nextProcess == 4 || save->nextProcess == 5) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, -16, 0x1000);
        if (EasyFade_IsFading()) {
            return;
        }
        DebugOvlDisp_Pop();
        return;
    }

    if ((gSaveData.unk_1AB4 & 2) || (gSaveData.unk_1AB4 & 0x80)) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, -16, 0x1000);
    } else {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
    }
    if (EasyFade_IsFading()) {
        return;
    }
    if (gSaveData.unk_1AB4 & 2) {
        gSaveData.unk_1AB4 &= ~2;
    }
#ifdef REGION_USA
    if (gSaveData.unk_1AB4 & 0x80) {
        gSaveData.unk_1AB4 &= ~0x80;
    }
#endif
    DebugOvlDisp_Pop();
}

void Save_Init(SaveMenuState* state) {
    SaveMenuObject*   save;
    GlobalFriendData* friendData;
    s32               result;

    if (state == NULL) {
        const char* sequence = Save_SequenceName;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(SaveMenuState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        Save_State = state;
        MainOvlDisp_SetCbArg(state);
    }
    save                      = &state->save;
    state->base.spareDataType = DatMgr_AllocateSlot();
    state->base.dataType      = DatMgr_AllocateSlot();
    Save_RegisterVBlank();
    state->base.prevResMgr = ResourceMgr_ReinitManagers(&state->base.resMgr);
    TouchInput_Init();
    Mem_InitializeHeap(&state->base.heap, state->base.heapBuffer, sizeof(state->base.heapBuffer));
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    EasyTask_InitializePool(&state->base.taskPool, &state->base.heap, 0x200, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    EasyTask_CreateTask(&state->base.taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -16, 0x1000);
    friendData = Mem_AllocHeapTail(&gMainHeap, sizeof(GlobalFriendData));
    Mem_SetSequence(&gMainHeap, friendData, Save_FriendDataName);
    state->friendData = friendData;
    result            = Savefile_LoadFriendImage(friendData);
    if (result == 0) {
        Save_InitState(state);
        Save_LoadBackgrounds(save);
        Save_CreateTasks(state);
        save->loaded = TRUE;
    } else if (result == 2) {
        save->nextProcess = 3;
        save->loaded      = FALSE;
    } else {
        save->nextProcess = 5;
        save->loaded      = FALSE;
    }
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)Save_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)Save_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void Save_Update(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    Save_UpdateBackgrounds(save);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->base.taskPool);
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->exitReady = 1;
    }
    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_EXTENDED], NULL);

    if (state->exitReady == 0) {
        return;
    }

    gSaveData.unk_1AB4 |= 1;
    switch (save->nextProcess) {
        case 0: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&SAVE_OVL_MENU, ProcessOverlay_MenuTop, NULL, PROCESS_STAGE_INIT);
        } break;
        case 1: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&SAVE_NEXT1_OVL, SAVE_NEXT1_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 2: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&SAVE_NEXT2_OVL, SAVE_NEXT2_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 3: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, SAVE_NEXT3_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 4: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, SAVE_NEXT4_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 5: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, SAVE_NEXT5_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        default: {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        } break;
    }
}

void Save_Destroy(SaveMenuState* state) {
    SaveMenuObject* save = &state->save;

    if (save->loaded == TRUE) {
        Save_ReleaseBackgrounds(save);
    }
    Mem_Free(&gMainHeap, state->friendData);
    EasyTask_DestroyPool(&state->base.taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->base.spareDataType);
    DatMgr_ClearSlot(state->base.dataType);
    Save_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
}

void ProcessOverlay_Save(SaveMenuState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        Save_Destroy(state);
    } else {
        OvlProc_Save.funcs[stage](state);
    }
}

// Nonmatching: the main-layer mask 0xE is materialised early and held in an extra callee-saved register, and
// the target writes the layers word twice (0xE, then 0x1E) where this merges it into one store
void Save_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    do {
    } while (REG_VCOUNT < (s16)0xC0);
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    if (gSaveData.unk_1AB4 & 2) {
        g_DisplaySettings.controls[DISPLAY_MAIN].brightness = -16;
        g_DisplaySettings.controls[DISPLAY_SUB].brightness  = -16;
        Display_Commit();
    }
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_A);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_B);
    GX_SetBankForObj(GX_VRAM_E);
    GX_SetBankForBgExtPltt(GX_VRAM_NONE);
    GX_SetBankForObjExtPltt(GX_VRAM_NONE);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_D);
    GX_SetBankForSubBgExtPltt(GX_VRAM_NONE);
    GX_SetBankForSubObjExtPltt(GX_VRAM_NONE);
    MI_CpuFill(0, (void*)0x06800000, 0xA4000);
    MI_CpuFill(0, (void*)0x06000000, 0x80000);
    MI_CpuFill(0, (void*)0x06200000, 0x20000);
    MI_CpuFill(0, (void*)0x06400000, 0x40000);
    MI_CpuFill(0, (void*)0x06600000, 0x20000);
    REG_POWER_CNT &= ~0x8000;
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension &= 0xFF;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);

    Display_InitMainBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 0, 0x10C);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 1, 0x204);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 3, 5, 1, 0x314);
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic   = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic   = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic   = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic   = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode               = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode                = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode                = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode                 = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= 0xFF;
    Display_SetMainLayers(LAYER_BG1 | LAYER_BG2 | LAYER_BG3);
    data_0206aa78 = 0x300010;
    data_0206aa7c = 0x400040;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_OBJ;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitSubBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 0, 0x10C);
    Display_InitSubBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x20C);
    Display_InitSubBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 3, 3, 1, 0x30C);

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    Display_SetSubLayers(LAYER_BG0 | LAYER_BG3 | LAYER_OBJ);
    OamMgr_Init3DSpritePipeline();
    OamMgr_Swap3DBuffers();

    g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode   = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 6;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 10;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode    = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0  = 4;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1  = 0x38;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0  = 6;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1  = 10;

    OamMgr_InitEngine(0, DISPLAY_MAIN);
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    DC_PurgeRange(g_OamMgr[DISPLAY_MAIN].oam, 0x400);
    GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_SUB);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    DC_PurgeRange(g_OamMgr[DISPLAY_SUB].oam, 0x400);
    GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
    OamMgr_InitEngine(0, DISPLAY_EXTENDED);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
}

void Save_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        OamMgr_Commit();
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
}

void Save_RegisterVBlank(void) {
    Save_InitDisplay();
    Interrupts_RegisterVBlankCallback(Save_VBlank, 1);
}

void Save_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
