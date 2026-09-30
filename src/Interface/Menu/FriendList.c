#include "Interface/Menu/FriendList.h"
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
    /* 0x21620 */ s32           taskId_Partner;
    /* 0x21624 */ s32           taskId_TextScr;
    /* 0x21628 */ s32           taskId_NameD;
    /* 0x2162C */ s32           taskId_Sbar;
    /* 0x21630 */ s32           taskId_Item[3];
    /* 0x2163C */ s32           taskId_ItemCur;
    /* 0x21640 */ s32           taskId_NumFrdID;
    /* 0x21644 */ s32           taskId_NumDate;
    /* 0x21648 */ s32           taskId_MyShop;
    /* 0x2164C */ s32           taskId_HelpCur;
#ifdef REGION_USA
    /* 0x21650 */ s32 taskId_Pointer;
#endif
    /* 0x21654 */ s32              taskId_TextScrU;
    /* 0x21658 */ s32              taskId_BdgU[6];
    /* 0x21670 */ s32              taskId_ItemU[9];
    /* 0x21694 */ s32              taskId_NameU;
    /* 0x21698 */ s32              taskId_HelpCurU;
    /* 0x2169C */ s32              exitReady;
    /* 0x216A0 */ s16              timer;
    /* 0x216A2 */ u16              unk_216A2;
    /* 0x216A4 */ s16              repeatTimer; // Key repeat delay for the d-pad/ABXY cursor
    /* 0x216A8 */ FriendListObject friendList;
} FriendListState;                              // Size: 0x24038

void GX_LoadBgPltt(void* src, u32 offset, u32 size);
void GX_LoadObjPltt(void* src, u32 offset, u32 size);
void GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void GXs_LoadObjPltt(void* src, u32 offset, u32 size);
void func_0202b878(void);
BOOL func_02001b44(s32, s32, void*, s32);
s32  Inventory_IsHelpSeen(s32);
void Inventory_SetHelpSeen(s32);

extern void func_ov043_020aeee0();

// JP has no counterpart to USA's ov036, so every later overlay number is one lower there.
#ifdef REGION_USA
    #define FRIENDLIST_OVL_MENU       OVERLAY_43_ID
    #define FRIENDLIST_OVL_MENU_ENTRY ((void*)0x02084040) /* ProcessOverlay_MenuTop */
    #define FRIENDLIST_OVL_SHOP_ENTRY func_ov043_020aeee0
#else
    #define FRIENDLIST_OVL_MENU       OVERLAY_42_ID
    #define FRIENDLIST_OVL_MENU_ENTRY ((void*)0x020849C4)
    #define FRIENDLIST_OVL_SHOP_ENTRY func_ov042_020ace7c
extern void func_ov042_020ace7c();
#endif
extern u32 OVERLAY_42_ID;

void FriendList_Init(FriendListState* state);
void FriendList_Update(FriendListState* state);
void FriendList_Destroy(FriendListState* state);
void FriendList_StageFirstHelp(FriendListState* state);
void FriendList_StageMain(FriendListState* state);
void FriendList_StageShop(FriendListState* state);
void FriendList_StageCloseShop(FriendListState* state);
void FriendList_StageHelp(FriendListState* state);
void FriendList_StageCloseHelp(FriendListState* state);
void FriendList_StageFadeOut(FriendListState* state);
void FriendList_StageFadeIn(FriendListState* state);
void FriendList_RegisterVBlank(void);
void FriendList_DeregisterVBlank(void);

static const char* FriendList_SequenceName = "Seq_FriendList()";

static const OverlayProcess OvlProc_FriendList = {
    .init = (OverlayCB)FriendList_Init,
    .main = (OverlayCB)FriendList_Update,
    .exit = (OverlayCB)FriendList_Destroy,
};

// clang-format off
const BinIdentifier FriendList_BinIdentifiers[18] = {
    [0]  = {45, "Apl_Tak/Grp_Friend_BGD00.bin"},
    [1]  = {45, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [2]  = {45, "Apl_Tak/Grp_Friend_OBD00.bin"},
    [3]  = {45, "Apl_Tak/Grp_Friend_OBD01.bin"},
    [4]  = {45, "Apl_Tak/Grp_Friend_OBD02.bin"},
    [5]  = {45, "Apl_Tak/Grp_Save_BGU00.bin"},
    [6]  = {45, "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [7]  = {45, "Apl_Tak/Grp_Save_OBU00.bin"},
    [8]  = {45, "Apl_Tak/Grp_Save_OBU01.bin"},
    [9]  = {45, "Apl_Tak/Grp_MenuIcon.bin"},
    [10] = {45, "Apl_Tak/Grp_Badge.bin"},
    [11] = {45, "Apl_Tak/Grp_Item.bin"},
    [12] = {45, "Apl_Tak/Grp_DummyBadge.bin"},
    [13] = {45, "Apl_Tak/Grp_DummyItem.bin"},
    [14] = {45, "Data/BadgeData.bin"},
    [15] = {45, "Apl_Tak/ItemData.bin"},
    [16] = {45, "Apl_Tak/FoodData.bin"},
    [17] = {45, "Apl_Tak/TreasureData.bin"},
};
// clang-format on

FriendListState* FriendList_State;

void FriendList_InitState(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;
    u16               i;

    state->exitReady        = 0;
    state->timer            = 0;
    state->unk_216A2        = 0;
    state->repeatTimer      = 0;
    friendList->flags       = 0;
    friendList->mode        = 0;
    friendList->nextProcess = 0;
    friendList->unk_2870    = 6;
    for (i = 0; i < 3; i++) {
        friendList->rowPressed[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        friendList->arrowPressed[i] = 0;
    }
    friendList->btnPressed     = 0;
    friendList->iconPressed[0] = 0;
    friendList->iconPressed[1] = 0;
    friendList->iconPressed[2] = 0;
    friendList->iconTimer      = 0;
    friendList->scroll         = 0;
    friendList->lastScroll     = 0;
    friendList->cursor         = 0;
    friendList->unk_28A1       = 0;
    for (i = 0; i < 3; i++) {
        friendList->helpButtonPressed[i] = 0;
    }
    friendList->helpPage = 0;
    friendList->helpOpen = 0;
    friendList->timer    = 0;
    FriendList_LoadFromSave(friendList);
}

void FriendList_CreateTasks(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;
    u16               i;

#ifdef REGION_USA
    state->taskId_Pointer = FriendList_pointer_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
#endif
    state->taskId_Icon     = FriendList_icon_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_NameD    = FriendList_nameD_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_Btn      = FriendList_btn_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_Sbar     = FriendList_sbar_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_TextScr  = FriendList_textScr_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_NumFrdID = FriendList_numFrdID_CreateTask(&state->base.taskPool, state->base.dataType, 0, friendList);
    state->taskId_NumDate  = FriendList_numDate_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_ItemCur  = FriendList_itemCur_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_MyShop   = FriendList_myShop_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    if (Inventory_GetOwnedCount(ITEM_STICKER_GAME_CLEARED) != 0 && friendList->partner != 0xFF) {
        state->taskId_Partner = FriendList_partner_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    }
    for (i = 0; i < 3; i++) {
        state->taskId_Item[i] = FriendList_item_CreateTask(&state->base.taskPool, state->base.dataType, i, friendList);
    }
    state->taskId_NameU    = FriendList_nameU_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_TextScrU = FriendList_textScrU_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    for (i = 0; i < 9; i++) {
        state->taskId_ItemU[i] = FriendList_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i, friendList);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_BdgU[i] = FriendList_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, i, friendList);
    }
}

void FriendList_ReloadItemTasks(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;
    s16               i;

    for (i = 0; i < 3; i++) {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Item[i]);
    }
#ifndef REGION_USA
    for (i = 0; i < 3; i++) {
        friendList->rowFriends[i] = &friendList->friends[friendList->scroll + i];
    }
#endif
    for (i = 0; i < 3; i++) {
        state->taskId_Item[i] = FriendList_item_CreateTask(&state->base.taskPool, state->base.dataType, i, friendList);
    }
}

void FriendList_SelectFriend(FriendListObject* friendList, u16 cursor, u16 partner) {
    FriendListState* state = FriendList_State;
    s16              i;

    friendList->unk_28A1 = 1 - friendList->unk_28A1;
    for (i = 0; i < 9; i++) {
        FriendList_itemU_ReleaseSprite(&state->base.taskPool, state->taskId_ItemU[i]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_ItemU[i]);
    }
    for (i = 0; i < 6; i++) {
        FriendList_bdgU_ReleaseSprite(&state->base.taskPool, state->taskId_BdgU[i]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgU[i]);
    }

    for (i = 0; i < 16; i++) {
        friendList->threads[i].itemId       = friendList->friends[cursor].shop.unk_10[i];
        friendList->threads[i].graphicIndex = friendList->friends[cursor].itemIcons[i];
    }
    friendList->gift.itemId       = friendList->friends[cursor].shop.giftItemId;
    friendList->gift.graphicIndex = friendList->friends[cursor].giftIcon;
    for (i = 0; i < 6; i++) {
        friendList->pins[i].itemId       = friendList->friends[cursor].shop.unk_04[i];
        friendList->pins[i].graphicIndex = friendList->friends[cursor].pinIcons[i];
    }
    friendList->esperPoints      = friendList->friends[cursor].experience.current;
    friendList->noiseReportCount = friendList->friends[cursor].experience.unk_0_0;
    friendList->pinsMastered     = friendList->friends[cursor].experience.pinCount;
    friendList->itemsCollected   = friendList->friends[cursor].experience.itemCount;
    friendList->timeAttackFrames = friendList->friends[cursor].experience.unk_8;

    for (i = 0; i < 4; i++) {
        friendList->slots[i] = &friendList->threads[i];
    }
    for (i = 0; i < 4; i++) {
        friendList->slots[i + 4] = &friendList->threads[partner * 4 + 4 + i];
    }
    friendList->slots[8] = &friendList->gift;

    for (i = 0; i < 9; i++) {
        state->taskId_ItemU[i] = FriendList_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i, friendList);
    }
    for (i = 0; i < 6; i++) {
        state->taskId_BdgU[i] = FriendList_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, i, friendList);
    }
}

void FriendList_ChangePartner(FriendListObject* friendList, u16 partner) {
    FriendListState* state = FriendList_State;
    s16              i;

    for (i = 0; i < 4; i++) {
        FriendList_itemU_ReleaseSprite(&state->base.taskPool, state->taskId_ItemU[i + 4]);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_ItemU[i + 4]);
    }
    for (i = 0; i < 4; i++) {
        friendList->slots[i + 4] = &friendList->threads[partner * 4 + 4 + i];
    }
    for (i = 0; i < 4; i++) {
        state->taskId_ItemU[i + 4] =
            FriendList_itemU_CreateTask(&state->base.taskPool, state->base.dataType, i + 4, friendList);
    }
    friendList->unk_28A1 = 1 - friendList->unk_28A1;
}

#ifdef REGION_USA
// JP has no d-pad/button navigation on this screen.
void FriendList_HandleButtons(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    if (!(InputStatus.buttonState.currButtons & INPUT_ABXY) && !(InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->repeatTimer = 0;
    }

    if (state->repeatTimer > 0) {
        state->repeatTimer--;
        return;
    }

    if ((InputStatus.buttonState.pressedButtons & INPUT_ABXY) || (InputStatus.buttonState.pressedButtons & INPUT_DPAD)) {
        state->repeatTimer = 20;
    } else if ((InputStatus.buttonState.currButtons & INPUT_ABXY) || (InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->repeatTimer = 1;
    }

    if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_UP) || (InputStatus.buttonState.currButtons & INPUT_BUTTON_X)) {
        if (friendList->cursor >= friendList->scroll && friendList->cursor < friendList->scroll + 3) {
            if (friendList->cursor != 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                if (friendList->cursor != friendList->scroll) {
                    friendList->cursor--;
                } else {
                    friendList->scroll--;
                    friendList->cursor--;
                }
            }
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            friendList->cursor = friendList->scroll;
        }
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_NumDate);
        state->taskId_NumDate = FriendList_numDate_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
        FriendList_SelectFriend(friendList, friendList->cursor, friendList->partner);
        friendList->flags |= 2;
        friendList->helpOpen = 0;
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_DOWN) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_B))
    {
        if (friendList->cursor >= friendList->scroll && friendList->cursor < friendList->scroll + 3) {
            if (friendList->cursor != 49) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                if (friendList->cursor != friendList->scroll + 2) {
                    friendList->cursor++;
                } else {
                    friendList->scroll++;
                    friendList->cursor++;
                }
            }
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            friendList->cursor = friendList->scroll;
        }
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_NumDate);
        state->taskId_NumDate = FriendList_numDate_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
        FriendList_SelectFriend(friendList, friendList->cursor, friendList->partner);
        friendList->flags |= 2;
        friendList->helpOpen = 0;
    }
}
#endif

void FriendList_StageFadeIn(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading()) {
        return;
    }
    if (friendList->nextProcess == 2 || friendList->nextProcess == 4) {
        DebugOvlDisp_Pop();
    } else if (Inventory_IsHelpSeen(1) == 0) {
        state->timer = 30;
        DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageFirstHelp, state, PROCESS_STAGE_INIT);
    } else {
        DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void FriendList_StageFirstHelp(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    if (state->timer > 0) {
        state->timer--;
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    friendList->mode                                                   = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0            = 4;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1            = 0x3B;
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
    state->taskId_HelpCur  = FriendList_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    state->taskId_HelpCurU = FriendList_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
    friendList->helpOpen   = 1;
    friendList->flags |= 2;
    DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageHelp, state, PROCESS_STAGE_INIT);
}

void FriendList_StageMain(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;
    TouchCoord        coord;
    s16               index;

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);

        index = FriendList_GetRowAtPoint(coord.x, coord.y);
        if (index != -1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            friendList->cursor = friendList->scroll + index;
            EasyTask_DeleteTask(&state->base.taskPool, state->taskId_NumDate);
            state->taskId_NumDate = FriendList_numDate_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
            FriendList_SelectFriend(friendList, friendList->cursor, friendList->partner);
            friendList->flags |= 2;
            friendList->helpOpen = 0;
        }

        index = FriendList_GetIconAtPoint(coord.x, coord.y);
        if (index != -1) {
            friendList->iconPressed[index] = 1;
            friendList->iconTimer          = 12;
            if (index == 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
                g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 0;
                g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 1;
                g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0            = 4;
                g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1            = 0x39;
                friendList->mode                                                   = 1;
                friendList->flags |= 1;
                DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageShop, state, PROCESS_STAGE_INIT);
            } else if (index == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                friendList->mode                                                   = 2;
                g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
                g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 0;
                g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0            = 4;
                g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1            = 0x3B;
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
                state->taskId_HelpCur = FriendList_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
                state->taskId_HelpCurU =
                    FriendList_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, friendList);
                friendList->helpOpen = 1;
                friendList->flags |= 2;
                DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageHelp, state, PROCESS_STAGE_INIT);
            } else {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
                friendList->flags |= 0x1000;
                friendList->nextProcess = 0;
                DebugOvlDisp_Pop();
                return;
            }
        }

        index = FriendList_GetRowBtnAtPoint(coord.x, coord.y);
        if (index != -1 && FriendList_IsSellable(friendList, friendList->cursor) == TRUE &&
            index == friendList->cursor - friendList->scroll)
        {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
            SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
            friendList->rowPressed[index] = 1;
            friendList->iconTimer         = 12;
            friendList->btnPressed        = 0;
            friendList->flags |= 0x1000;
            friendList->nextProcess = 1;
            gSaveData.unk_1AB4 |= 4;
            DebugOvlDisp_Pop();
            return;
        }
    }
#ifdef REGION_USA
    else if (!TouchInput_IsTouchActive())
    {
        FriendList_HandleButtons(state);
    }
#endif

    if (friendList->lastScroll != friendList->scroll) {
        FriendList_ReloadItemTasks(state);
        friendList->flags |= 1;
        friendList->lastScroll = friendList->scroll;
    }
}

// Nonmatching: the final blend-register pool loads are scheduled one store earlier
void FriendList_StageShop(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;
    TouchCoord        coord;
    s32               changed;
    s16               index;

    if (!TouchInput_IsTouchActive()) {
        state->repeatTimer = 0;
    }
    if (state->repeatTimer > 0) {
        state->repeatTimer--;
        return;
    }
    if (TouchInput_WasTouchPressed()) {
        state->repeatTimer = 20;
    } else if (TouchInput_IsBeingHeld()) {
        state->repeatTimer = 1;
    }
    if (!TouchInput_IsTouchActive()) {
        return;
    }

    TouchInput_GetCoord(&coord);
    index = FriendList_GetArrowAtPoint(coord.x, coord.y);
    if (index != -1) {
        friendList->arrowPressed[index] = 1;
        friendList->iconTimer           = 12;
        changed                         = 0;
        switch (index) {
            case 0:
                if (friendList->shopIndex < 34) {
                    changed = 1;
                    friendList->shopIndex++;
                } else if (TouchInput_WasTouchPressed()) {
                    friendList->shopIndex = 0;
                    changed               = 1;
                }
                friendList->shopId = FriendList_ShopIds[friendList->shopIndex];
                break;
            case 1:
                if (friendList->clerkId < 34) {
                    changed = 1;
                    friendList->clerkId++;
                } else if (TouchInput_WasTouchPressed()) {
                    friendList->clerkId = 0;
                    changed             = 1;
                }
                break;
            case 2:
                if (friendList->musicId < 2) {
                    changed = 1;
                    friendList->musicId++;
                } else if (TouchInput_WasTouchPressed()) {
                    friendList->musicId = 0;
                    changed             = 1;
                }
                break;
            case 3:
                if (friendList->shopIndex > 0) {
                    changed = 1;
                    friendList->shopIndex--;
                } else if (TouchInput_WasTouchPressed()) {
                    friendList->shopIndex = 34;
                    changed               = 1;
                }
                friendList->shopId = FriendList_ShopIds[friendList->shopIndex];
                break;
            case 4:
                if (friendList->clerkId > 0) {
                    changed = 1;
                    friendList->clerkId--;
                } else if (TouchInput_WasTouchPressed()) {
                    friendList->clerkId = 34;
                    changed             = 1;
                }
                break;
            case 5:
                if (friendList->musicId > 0) {
                    changed = 1;
                    friendList->musicId--;
                } else if (TouchInput_WasTouchPressed()) {
                    friendList->musicId = 2;
                    changed             = 1;
                }
                break;
        }
        if (changed) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
        }
        friendList->flags |= 1;
    }

    if (!TouchInput_WasTouchPressed()) {
        return;
    }
    if (FriendList_IsPointOnBtn(coord.x, coord.y) != 1) {
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    friendList->iconTimer                                   = 12;
    friendList->timer                                       = 20;
    friendList->btnPressed                                  = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 4;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x39;
    DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageCloseShop, state, PROCESS_STAGE_INIT);
}

void FriendList_StageCloseShop(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    if (friendList->timer != 0) {
        friendList->timer--;
        return;
    }
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
    friendList->mode = 0;
    friendList->flags |= 1;
    DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageMain, state, PROCESS_STAGE_INIT);
}

void FriendList_StageHelp(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;
    TouchCoord        coord;
    s16               index;

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);
        index = FriendList_GetHelpBtnAtPoint(coord.x, coord.y);
        if (index != -1) {
            if (friendList->iconTimer != 0) {
                return;
            }
            friendList->helpButtonPressed[index] = 1;
            friendList->iconTimer                = 12;
            if (index == 0) {
                if (friendList->helpPage != 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    friendList->helpPage--;
                    friendList->flags |= 2;
                }
            } else if (index == 1) {
                if (friendList->helpPage < 2) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    friendList->helpPage++;
                    friendList->flags |= 2;
                }
            } else if (Inventory_IsHelpSeen(1) == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageCloseHelp, state, PROCESS_STAGE_INIT);
            }
        }
    }

#ifdef REGION_USA
    if (Inventory_IsHelpSeen(1) == 0) {
#else
    // JP only marks the help as read once its last page has been reached.
    if (Inventory_IsHelpSeen(1) == 0 && friendList->helpPage == 2) {
#endif
        Inventory_SetHelpSeen(1);
    }
}

void FriendList_StageCloseHelp(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    if (friendList->helpButtonPressed[2] != 0) {
        return;
    }
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG1;
    g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCur);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCurU);
    friendList->helpPage = 0;
    friendList->helpOpen = 0;
    friendList->flags |= 2;
    friendList->mode = 0;
    DebugOvlDisp_ReplaceTop((OverlayCB)FriendList_StageMain, state, PROCESS_STAGE_INIT);
}

void FriendList_StageFadeOut(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    if (friendList->nextProcess == 2 || friendList->nextProcess == 4) {
        EasyFade_FadeBothDisplays(FADER_LINEAR, -16, 0x1000);
    } else {
        EasyFade_FadeBothDisplays(FADER_LINEAR, 16, 0x1000);
    }
    if (EasyFade_IsFading()) {
        return;
    }
    DebugOvlDisp_Pop();
}

void FriendList_Init(FriendListState* state) {
    FriendListObject* friendList;
    GlobalFriendData* friendData;
    s32               result;

    if (state == NULL) {
        const char* sequence = FriendList_SequenceName;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(FriendListState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        FriendList_State = state;
        MainOvlDisp_SetCbArg(state);
    }
    friendList                = &state->friendList;
    state->base.spareDataType = DatMgr_AllocateSlot();
    state->base.dataType      = DatMgr_AllocateSlot();
    FriendList_RegisterVBlank();
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
    Mem_SetSequence(&gMainHeap, friendData, "GlobalFriendData");
    friendList->friendData = friendData;
    result                 = Savefile_LoadFriendImage(friendData);
    if (result == 0) {
        FriendList_InitState(state);
        FriendList_LoadBackgrounds(friendList);
        FriendList_CreateTasks(state);
    } else if (result == 2) {
        friendList->nextProcess = 2;
    } else {
        friendList->nextProcess = 4;
    }
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)FriendList_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)FriendList_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
    CriSndMgr_PlayFile(gSaveData.bgmFile);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void FriendList_Update(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    FriendList_UpdateBackgrounds(friendList);
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

    switch (friendList->nextProcess) {
        case 0: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&FRIENDLIST_OVL_MENU, FRIENDLIST_OVL_MENU_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 1: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&FRIENDLIST_OVL_MENU, FRIENDLIST_OVL_SHOP_ENTRY, NULL, PROCESS_STAGE_INIT);
        } break;
        case 2: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, (void*)0x02086B0C, NULL, PROCESS_STAGE_INIT);
        } break;
        case 3: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, (void*)0x02086B4C, NULL, PROCESS_STAGE_INIT);
        } break;
        case 4: {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_2_ID, (void*)0x02086A8C, NULL, PROCESS_STAGE_INIT);
        } break;
        default: {
            OverlayTag tag;
            MainOvlDisp_Pop(&tag);
        } break;
    }
}

void FriendList_Destroy(FriendListState* state) {
    FriendListObject* friendList = &state->friendList;

    CriSndMgr_Stop(0);
    if (friendList->nextProcess != 2 && friendList->nextProcess != 4) {
        if (gSaveData.unk_1AB4 & 4) {
            FriendList_WriteToSaveForShop(friendList);
        } else {
            FriendList_WriteToSave(friendList);
        }
        FriendList_ReleaseBackgrounds(friendList);
    }
    Mem_Free(&gMainHeap, friendList->friendData);
    gSaveData.unk_1AB4 |= 1;
    EasyTask_DestroyPool(&state->base.taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->base.spareDataType);
    DatMgr_ClearSlot(state->base.dataType);
    FriendList_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
}

void ProcessOverlay_FriendList(FriendListState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        FriendList_Destroy(state);
    } else {
        OvlProc_FriendList.funcs[stage](state);
    }
}

// Nonmatching: BG-setting constants materialised in a different order (target keeps 2 in r7), as in
// TusinSet_InitDisplay
void FriendList_InitDisplay(void) {
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
    GX_SetBankForBg(GX_VRAM_EFG);
    GX_SetBankForObj(GX_VRAM_B);
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
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x20C);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    data_0206aa78                                        = 0x300010;
    data_0206aa7c                                        = 0x400040;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG3 | LAYER_OBJ);

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
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 4;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x39;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 6;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 10;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendMode    = 1;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0  = 4;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1  = 0x38;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0  = 6;
    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1  = 10;

    OamMgr_InitExtended();
}

void FriendList_VBlank(void) {
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

void FriendList_RegisterVBlank(void) {
    FriendList_InitDisplay();
    Interrupts_RegisterVBlankCallback(FriendList_VBlank, 1);
}

void FriendList_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

// Shop ids selectable for the player's own Mingle Mode shop.
u16 FriendList_ShopIds[35] = {1,  2,  3,  4,  6,  7,  8,  10, 12, 14, 15, 16, 17, 19, 20, 21, 22, 23,
                              24, 26, 27, 28, 31, 32, 33, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45};
