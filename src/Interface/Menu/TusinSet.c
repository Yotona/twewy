#include "Interface/Menu/TusinSet.h"
#include "CriSndMgr.h"
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
#include "common_data.h"
#include <nitro/fs/overlay.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>

typedef struct {
    /* 0x00000 */ MenuStateBase base;
    /* 0x21618 */ s32           taskId_Icon;
    /* 0x2161C */ s32           taskId_Btn;
    /* 0x21620 */ s32           taskId_TextScr;
    /* 0x21624 */ s32           taskId_NameD;
    /* 0x21628 */ s32           taskId_Partner;
    /* 0x2162C */ s32           taskId_Tab;
    /* 0x21630 */ s32           taskId_Sbar;
#ifdef REGION_USA
    /* 0x21634 */ s32 taskId_Sbar2;
#endif
    /* 0x21638 */ s32 taskId_Item[16];
    /* 0x21678 */ s32 taskId_ItemCur;
    /* 0x2167C */ s32 taskId_HelpCur;
#ifdef REGION_USA
    /* 0x21680 */ s32 taskId_Pointer;
#endif
    /* 0x21684 */ s32 taskId_TextScrU;
    /* 0x21688 */ s32 taskId_ItemU[9];
    /* 0x216AC */ s32 taskId_BdgU[6];
    /* 0x216C4 */ s32 taskId_SlotCoverU[6];
    /* 0x216DC */ s32 taskId_NameU;
    /* 0x216E0 */ s32 taskId_HelpCurU;
    /* 0x216E4 */ s32 exitReady;
#ifdef REGION_USA
    /* 0x216E8 */ u16 repeatTimer; // Key repeat delay for the d-pad/ABXY cursor
#endif
    /* 0x216EA */ s16            timer;
    /* 0x216EC */ u16            unk_216EC;
    /* 0x216F0 */ TusinSetObject tusinSet;
} TusinSetState; // Size: 0x27A14 (JP: 0x279C0)

void GX_LoadBgPltt(void* src, u32 offset, u32 size);
void GX_LoadObjPltt(void* src, u32 offset, u32 size);
void GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void GXs_LoadObjPltt(void* src, u32 offset, u32 size);
void func_0202b878(void);
BOOL func_02001b44(s32, s32, void*, s32);
s32  Inventory_IsHelpSeen(s32);
void Inventory_SetHelpSeen(s32);
void TusinSet_itemU_ReleaseSprite(TaskPool* pool, s32 taskId);
void TusinSet_item_ReleaseSprite(TaskPool* pool, s32 taskId);

s32 TusinSet_pointer_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_icon_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_btn_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_textScr_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_nameD_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_tab_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_sbar_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_sbar2_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_itemCur_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_partner_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_item_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet);
s32 TusinSet_nameU_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_textScrU_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_itemU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet);
s32 TusinSet_bdgU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet);
s32 TusinSet_slotCoverU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet);
s32 TusinSet_helpCur_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);
s32 TusinSet_helpCurU_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet);

// JP has no counterpart to USA's ov036, so every later overlay number is one lower there.
#ifdef REGION_USA
    #define TUSINSET_OVL_SELF       45
    #define TUSINSET_OVL_MENU       OVERLAY_43_ID
    #define TUSINSET_OVL_MENU_ENTRY ((void*)0x02084040) /* ProcessOverlay_MenuTop */
    #define TUSINSET_OVL_TUSIN      OVERLAY_45_ID
#else
    #define TUSINSET_OVL_SELF       44
    #define TUSINSET_OVL_MENU       OVERLAY_42_ID
    #define TUSINSET_OVL_MENU_ENTRY ((void*)0x020849C4)
    #define TUSINSET_OVL_TUSIN      OVERLAY_44_ID
#endif
extern u32 OVERLAY_42_ID;

void ProcessOverlay_Tusin(void* state);

void TusinSet_Init(TusinSetState* state);
void TusinSet_Update(TusinSetState* state);
void TusinSet_Destroy(TusinSetState* state);
void TusinSet_StageFirstHelp(TusinSetState* state);
void TusinSet_StageMain(TusinSetState* state);
void TusinSet_StageHelp(TusinSetState* state);
void TusinSet_StageCloseHelp(TusinSetState* state);
void TusinSet_StageFadeOut(TusinSetState* state);
void TusinSet_StageFadeIn(TusinSetState* state);
void TusinSet_RegisterVBlank(void);
void TusinSet_DeregisterVBlank(void);

static const char* TusinSet_SequenceName = "Seq_TusinSet()";

static const OverlayProcess OvlProc_TusinSet = {
    .init = (OverlayCB)TusinSet_Init,
    .main = (OverlayCB)TusinSet_Update,
    .exit = (OverlayCB)TusinSet_Destroy,
};

const Point TusinSet_SlotPositions[16] = {
    { 19, 111},
    { 49, 111},
    { 79, 111},
    {109, 111},
    {139, 111},
    {169, 111},
    {199, 111},
    {229, 111},
    { 19, 141},
    { 49, 141},
    { 79, 141},
    {109, 141},
    {139, 141},
    {169, 141},
    {199, 141},
    {229, 141},
};

// clang-format off
const BinIdentifier TusinSet_BinIdentifiers[17] = {
    [0]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Tuset_BGD00.bin"},
    [1]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [2]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Tuset_OBD00.bin"},
    [3]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Tuset_OBD01.bin"},
    [4]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Tuset_OBD02.bin"},
    [5]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Save_BGU00.bin"},
    [6]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [7]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Save_OBU00.bin"},
    [8]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_MenuIcon.bin"},
    [9]  = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Badge.bin"},
    [10] = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_Item.bin"},
    [11] = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_DummyBadge.bin"},
    [12] = {TUSINSET_OVL_SELF, "Apl_Tak/Grp_DummyItem.bin"},
    [13] = {TUSINSET_OVL_SELF, "Data/BadgeData.bin"},
    [15] = {TUSINSET_OVL_SELF, "Apl_Tak/FoodData.bin"},
    [14] = {TUSINSET_OVL_SELF, "Apl_Tak/ItemData.bin"},
    [16] = {TUSINSET_OVL_SELF, "Apl_Tak/TreasureData.bin"},
};
// clang-format on

TusinSetItem TusinSet_EmptyItem = {.itemId = 0xFFFF};

TusinSetState* TusinSet_State;

void TusinSet_InitState(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;
    u16             i;

    state->exitReady         = 0;
    state->timer             = 0;
    state->unk_216EC         = 0;
    tusinSet->flags          = 0;
    tusinSet->unk_61C8       = 0;
    tusinSet->nextProcess    = 0;
    tusinSet->badgeSlots     = 6;
    tusinSet->iconPressed[0] = 0;
    tusinSet->iconPressed[1] = 0;
    tusinSet->iconTimer      = 0;
    tusinSet->unk_61CE       = 0;
    tusinSet->partner        = 0;
    tusinSet->unk_61D2       = 0;
    tusinSet->unk_61D3       = 0;
#ifndef REGION_USA
    tusinSet->scroll   = 0;
    tusinSet->unk_61D6 = 0;
    tusinSet->cursor   = 0;
#endif
    tusinSet->unk_61EC = 0;
    for (i = 0; i < 3; i++) {
        tusinSet->helpButtonPressed[i] = 0;
    }
    tusinSet->helpPage = 0;
    tusinSet->helpOpen = 0;
    for (i = 0; i < 8; i++) {
        tusinSet->tabSelected[i] = 0;
    }
#ifdef REGION_USA
    state->repeatTimer = 0;
#endif
    TusinSet_LoadFromSave(tusinSet);
}

void TusinSet_CreateTasks(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;
    u16             i;

    EasyTask_CreateTask(&state->base.taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -16, 0x1000);
#ifdef REGION_USA
    state->taskId_Pointer = TusinSet_pointer_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
#endif
    state->taskId_Icon    = TusinSet_icon_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_Btn     = TusinSet_btn_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_TextScr = TusinSet_textScr_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_NameD   = TusinSet_nameD_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_Tab     = TusinSet_tab_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_Sbar    = TusinSet_sbar_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
#ifdef REGION_USA
    state->taskId_Sbar2 = TusinSet_sbar2_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
#endif
    state->taskId_ItemCur = TusinSet_itemCur_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    if (func_02023010(0x2AE) != 0 && tusinSet->partner != 0xFF) {
        state->taskId_Partner = TusinSet_partner_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    }
    for (i = 0; i < 16; i++) {
        state->taskId_Item[i] = TusinSet_item_CreateTask(&state->base.taskPool, state->base.dataType, i, tusinSet);
    }
    state->taskId_NameU    = TusinSet_nameU_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_TextScrU = TusinSet_textScrU_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    for (i = 0; i < 9; i++) {
        state->taskId_ItemU[i] = TusinSet_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i, tusinSet);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_BdgU[i] = TusinSet_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, i, tusinSet);
    }
    for (i = 0; i < 6; i++) {
        if (i >= tusinSet->badgeSlots) {
            state->taskId_SlotCoverU[i] =
                TusinSet_slotCoverU_CreateTask(&state->base.taskPool, state->base.dataType, i, tusinSet);
        }
    }
}

void TusinSet_ChangePartner(TusinSetObject* tusinSet, u16 partner) {
    TusinSetState* state = TusinSet_State;
    s16            i;

    for (i = 0; i < 4; i++) {
        TusinSet_itemU_ReleaseSprite(&state->base.taskPool, state->taskId_ItemU[i + 4]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_ItemU[i + 4]);
    }
    for (i = 0; i < 4; i++) {
        tusinSet->slots[i + 4] = &tusinSet->threads[partner * 4 + 4 + i];
    }
    for (i = 0; i < 4; i++) {
        state->taskId_ItemU[i + 4] = TusinSet_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i + 4, tusinSet);
    }
}

void TusinSet_ReloadItemTasks(TusinSetObject* tusinSet, u16 scroll, u8 tab) {
    TusinSetState* state = TusinSet_State;
    s16            i;

    for (i = 0; i < 16; i++) {
        TusinSet_item_ReleaseSprite(&state->base.taskPool, state->taskId_Item[i]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Item[i]);
    }
    for (i = 0; i < 16; i++) {
        tusinSet->visible[i] = tusinSet->lists[tab][scroll + i];
    }
    for (i = 0; i < 16; i++) {
        state->taskId_Item[i] = TusinSet_item_CreateTask(&state->base.taskPool, state->base.dataType, i, tusinSet);
    }
}

void TusinSet_SelectCursorItem(TusinSetObject* tusinSet) {
    tusinSet->selected               = *tusinSet->lists[tusinSet->tab][tusinSet->cursor];
    tusinSet->slots[8]->itemId       = tusinSet->selected.itemId;
    tusinSet->slots[8]->graphicIndex = tusinSet->selected.graphicIndex;
}

void TusinSet_ReloadGiftIcon(TusinSetObject* tusinSet) {
    TusinSetState* state = TusinSet_State;

    TusinSet_itemU_ReleaseSprite(&state->base.taskPool, state->taskId_ItemU[8]);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_ItemU[8]);
    state->taskId_ItemU[8] = TusinSet_itemU_CreateTask(&state->base.taskPool, state->base.dataType, 8, tusinSet);
}

BOOL TusinSet_IsAnyTabSelected(TusinSetObject* tusinSet) {
    for (u16 i = 0; i < 8; i++) {
        if (tusinSet->tabSelected[i] == 1) {
            return TRUE;
        }
    }
    return FALSE;
}

void TusinSet_ClearTabSelection(TusinSetObject* tusinSet) {
    for (u16 i = 0; i < 8; i++) {
        tusinSet->tabSelected[i] = 0;
    }
}

void TusinSet_HandleButtons(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;

#ifdef REGION_USA
    if (!(InputStatus.buttonState.currButtons & INPUT_ABXY) && !(InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->repeatTimer = 0;
    }

    if (InputStatus.buttonState.pressedButtons & (INPUT_BUTTON_L | INPUT_BUTTON_R)) {
#endif
        if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_L) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if (tusinSet->tab == 0) {
                tusinSet->tab = 8;
            } else {
                tusinSet->tab--;
            }
            TusinSet_ClearTabSelection(tusinSet);
            if (tusinSet->tab != 8) {
                tusinSet->tabSelected[tusinSet->tab] = 1;
            }
            tusinSet->scroll = 0;
            tusinSet->cursor = 0;
            TusinSet_ReloadItemTasks(tusinSet, tusinSet->scroll, tusinSet->tab);
            TusinSet_SelectCursorItem(tusinSet);
            TusinSet_ReloadGiftIcon(tusinSet);
            tusinSet->flags |= 1;
        } else if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_R) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if (tusinSet->tab == 8) {
                tusinSet->tab = 0;
            } else {
                tusinSet->tab++;
            }
            TusinSet_ClearTabSelection(tusinSet);
            if (tusinSet->tab != 8) {
                tusinSet->tabSelected[tusinSet->tab] = 1;
            }
            tusinSet->scroll = 0;
            tusinSet->cursor = 0;
            TusinSet_ReloadItemTasks(tusinSet, tusinSet->scroll, tusinSet->tab);
            TusinSet_SelectCursorItem(tusinSet);
            TusinSet_ReloadGiftIcon(tusinSet);
            tusinSet->flags |= 1;
        }
#ifdef REGION_USA
        return;
    }

    if (state->repeatTimer != 0) {
        state->repeatTimer--;
        return;
    }

    if ((InputStatus.buttonState.pressedButtons & INPUT_ABXY) || (InputStatus.buttonState.pressedButtons & INPUT_DPAD)) {
        state->repeatTimer = 20;
    } else if ((InputStatus.buttonState.currButtons & INPUT_ABXY) || (InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->repeatTimer = 1;
    }

    if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_UP) || (InputStatus.buttonState.currButtons & INPUT_BUTTON_X)) {
        if (tusinSet->cursor >= tusinSet->scroll && tusinSet->cursor < tusinSet->scroll + 16) {
            if (tusinSet->cursor >= 8) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                if (tusinSet->cursor >= tusinSet->scroll && tusinSet->cursor < tusinSet->scroll + 8) {
                    tusinSet->scroll -= 8;
                    tusinSet->cursor -= 8;
                } else {
                    tusinSet->cursor -= 8;
                }
            }
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            tusinSet->cursor = tusinSet->scroll;
        }
        TusinSet_SelectCursorItem(tusinSet);
        TusinSet_ReloadGiftIcon(tusinSet);
        tusinSet->flags |= 1;
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_DOWN) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_B))
    {
        if (tusinSet->cursor >= tusinSet->scroll && tusinSet->cursor < tusinSet->scroll + 16) {
            if (tusinSet->cursor < tusinSet->listSizes[tusinSet->tab] - 8) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                if (tusinSet->cursor >= tusinSet->scroll + 8 && tusinSet->cursor < tusinSet->scroll + 16) {
                    tusinSet->scroll += 8;
                    tusinSet->cursor += 8;
                } else {
                    tusinSet->cursor += 8;
                }
            }
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            tusinSet->cursor = tusinSet->scroll;
        }
        TusinSet_SelectCursorItem(tusinSet);
        TusinSet_ReloadGiftIcon(tusinSet);
        tusinSet->flags |= 1;
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_LEFT) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_Y))
    {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        if (tusinSet->cursor >= tusinSet->scroll && tusinSet->cursor < tusinSet->scroll + 16) {
            if (tusinSet->cursor == 0) {
                tusinSet->scroll = tusinSet->maxScrollRow[tusinSet->tab] * 8;
                tusinSet->cursor = tusinSet->listSizes[tusinSet->tab] - 1;
            } else if (tusinSet->cursor != tusinSet->scroll) {
                tusinSet->cursor--;
            } else {
                tusinSet->scroll -= 8;
                tusinSet->cursor--;
            }
        } else {
            tusinSet->cursor = tusinSet->scroll;
        }
        TusinSet_SelectCursorItem(tusinSet);
        TusinSet_ReloadGiftIcon(tusinSet);
        tusinSet->flags |= 1;
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_RIGHT) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_A))
    {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        if (tusinSet->cursor >= tusinSet->scroll && tusinSet->cursor < tusinSet->scroll + 16) {
            if (tusinSet->cursor == tusinSet->listSizes[tusinSet->tab] - 1) {
                tusinSet->scroll = 0;
                tusinSet->cursor = 0;
            } else if (tusinSet->cursor != tusinSet->scroll + 15) {
                tusinSet->cursor++;
            } else {
                tusinSet->scroll += 8;
                tusinSet->cursor++;
            }
        } else {
            tusinSet->cursor = tusinSet->scroll;
        }
        TusinSet_SelectCursorItem(tusinSet);
        TusinSet_ReloadGiftIcon(tusinSet);
        tusinSet->flags |= 1;
    }
#endif
}

void TusinSet_StageFadeIn(TusinSetState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading()) {
        return;
    }
    if (Inventory_IsHelpSeen(6) == 0) {
        state->timer = 30;
        DebugOvlDisp_ReplaceTop((OverlayCB)TusinSet_StageFirstHelp, state, PROCESS_STAGE_INIT);
    } else {
        DebugOvlDisp_ReplaceTop((OverlayCB)TusinSet_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void TusinSet_StageFirstHelp(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;

    if (state->timer > 0) {
        state->timer--;
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1;
    state->taskId_HelpCur  = TusinSet_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    state->taskId_HelpCurU = TusinSet_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
    tusinSet->helpOpen     = 1;
    tusinSet->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)TusinSet_StageHelp, state, PROCESS_STAGE_INIT);
}

void TusinSet_StageMain(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;
    TouchCoord      coord;
    s16             index;

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);

        index = TusinSet_GetSlotAtPoint(coord.x, coord.y);
        if (index != -1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            tusinSet->cursor = tusinSet->scroll + index;
            TusinSet_SelectCursorItem(tusinSet);
            TusinSet_ReloadGiftIcon(tusinSet);
            tusinSet->flags |= 1;
            return;
        }

        index = TusinSet_GetTabAtPoint(coord.x, coord.y);
        if (index != -1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if (tusinSet->tabSelected[index] == 1) {
                tusinSet->tabSelected[index] = 0;
                tusinSet->tab                = 8;
            } else if (TusinSet_IsAnyTabSelected(tusinSet) == TRUE) {
                TusinSet_ClearTabSelection(tusinSet);
                tusinSet->tabSelected[index] = 1;
                tusinSet->tab                = index;
            } else {
                tusinSet->tabSelected[index] = 1;
                tusinSet->tab                = index;
            }
            tusinSet->scroll = 0;
            tusinSet->cursor = 0;
            TusinSet_ReloadItemTasks(tusinSet, tusinSet->scroll, tusinSet->tab);
            TusinSet_SelectCursorItem(tusinSet);
            TusinSet_ReloadGiftIcon(tusinSet);
            tusinSet->flags |= 1;
            return;
        }

        index = TusinSet_GetIconAtPoint(coord.x, coord.y);
        if (index != -1) {
            tusinSet->iconPressed[index] = 1;
            tusinSet->iconTimer          = 12;
            if (index == 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1;
                state->taskId_HelpCur  = TusinSet_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
                state->taskId_HelpCurU = TusinSet_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, tusinSet);
                tusinSet->helpOpen     = 1;
                tusinSet->flags |= 2;
                DebugOvlDisp_ReplaceTop((OverlayCB)TusinSet_StageHelp, state, PROCESS_STAGE_INIT);
            } else {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
                tusinSet->flags |= 0x1000;
                tusinSet->nextProcess = 0;
                DebugOvlDisp_Pop();
            }
        }
    } else if (!TouchInput_IsTouchActive()) {
        TusinSet_HandleButtons(state);
    }

    if (tusinSet->unk_61D6 != tusinSet->scroll) {
        TusinSet_ReloadItemTasks(tusinSet, tusinSet->scroll, tusinSet->tab);
        tusinSet->unk_61D6 = tusinSet->scroll;
    }
}

void TusinSet_StageHelp(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;
    TouchCoord      coord;
    s16             index;

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);
        index = TusinSet_GetHelpBtnAtPoint(coord.x, coord.y);
        if (index != -1) {
            if (tusinSet->iconTimer != 0) {
                return;
            }
            tusinSet->helpButtonPressed[index] = 1;
            tusinSet->iconTimer                = 12;
            if (index == 0) {
                if (tusinSet->helpPage != 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    tusinSet->helpPage--;
                    tusinSet->flags |= 2;
                }
            } else if (index == 1) {
                if (tusinSet->helpPage < 6) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    tusinSet->helpPage++;
                    tusinSet->flags |= 2;
                }
            } else if (Inventory_IsHelpSeen(6) == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                DebugOvlDisp_ReplaceTop((OverlayCB)TusinSet_StageCloseHelp, state, PROCESS_STAGE_INIT);
            }
        }
    }

#ifdef REGION_USA
    if (Inventory_IsHelpSeen(6) == 0) {
#else
    // JP only marks the help as read once its last page has been reached.
    if (Inventory_IsHelpSeen(6) == 0 && tusinSet->helpPage == 6) {
#endif
        Inventory_SetHelpSeen(6);
    }
}

void TusinSet_StageCloseHelp(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;

    if (tusinSet->helpButtonPressed[2] != 0) {
        return;
    }
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG1;
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCur);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCurU);
    tusinSet->helpPage = 0;
    tusinSet->helpOpen = 0;
    tusinSet->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)TusinSet_StageMain, state, PROCESS_STAGE_INIT);
}

void TusinSet_StageFadeOut(TusinSetState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
    if (EasyFade_IsFading()) {
        return;
    }
    DebugOvlDisp_Pop();
}

void TusinSet_Init(TusinSetState* state) {
    if (state == NULL) {
        const char* sequence = TusinSet_SequenceName;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(TusinSetState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        TusinSet_State = state;
        MainOvlDisp_SetCbArg(state);
    }
    state->base.spareDataType = DatMgr_AllocateSlot();
    state->base.dataType      = DatMgr_AllocateSlot();
    TusinSet_RegisterVBlank();
    state->base.prevResMgr = ResourceMgr_ReinitManagers(&state->base.resMgr);
    TouchInput_Init();
    Mem_InitializeHeap(&state->base.heap, state->base.heapBuffer, sizeof(state->base.heapBuffer));
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    EasyTask_InitializePool(&state->base.taskPool, &state->base.heap, 0x200, NULL, NULL);
    data_02066aec = 0;
    data_02066eec = 0;
    TusinSet_InitState(state);
    TusinSet_LoadBackgrounds(&state->tusinSet);
    TusinSet_CreateTasks(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)TusinSet_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)TusinSet_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
    CriSndMgr_PlayFile(gSaveData.bgmFile);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void TusinSet_Update(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    TusinSet_UpdateBackgrounds(tusinSet);
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

    switch (tusinSet->nextProcess) {
        case 0: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&TUSINSET_OVL_MENU, TUSINSET_OVL_MENU_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 1: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&TUSINSET_OVL_TUSIN, ProcessOverlay_Tusin, NULL, PROCESS_STAGE_INIT);
        } break;
        default: {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        } break;
    }
}

void TusinSet_Destroy(TusinSetState* state) {
    TusinSetObject* tusinSet = &state->tusinSet;

    CriSndMgr_Stop(0);
    TusinSet_WriteToSave(tusinSet);
    TusinSet_ReleaseBackgrounds(tusinSet);
    EasyTask_DestroyPool(&state->base.taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->base.spareDataType);
    DatMgr_ClearSlot(state->base.dataType);
    TusinSet_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void ProcessOverlay_TusinSet(TusinSetState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        TusinSet_Destroy(state);
    } else {
        OvlProc_TusinSet.funcs[stage](state);
    }
}

// Nonmatching: BG-setting constants materialised in a different order (target keeps 2 in r7)
void TusinSet_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    do {
    } while (REG_VCOUNT < (s16)0xC0);
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
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
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 1, 0x10C);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 1, 1, 0x204);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_32K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    data_0206aa78                                        = 0x300010;
    data_0206aa7c                                        = 0x400040;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG2 | LAYER_BG3 | LAYER_OBJ);

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
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x3D;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 6;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 10;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode    = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0  = 4;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1  = 0x38;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0  = 6;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1  = 10;

    OamMgr_InitExtended();
}

void TusinSet_VBlank(void) {
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

void TusinSet_RegisterVBlank(void) {
    TusinSet_InitDisplay();
    Interrupts_RegisterVBlankCallback(TusinSet_VBlank, 1);
}

void TusinSet_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
