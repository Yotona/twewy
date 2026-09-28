#include "Interface/Menu/MenuBadge.h"
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
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/fs/overlay.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>

typedef struct {
    /* 0x00000 */ MenuStateBase   base;
    /* 0x21618 */ s32             taskId_Icon;
    /* 0x2161C */ s32             taskId_Bdg[30];
    /* 0x21694 */ s32             taskId_BdgLV[30];
    /* 0x2170C */ s32             taskId_BdgBP[30];
    /* 0x21784 */ s32             taskId_BdgNum[30];
    /* 0x217FC */ s32             taskId_BdgSC[6];
    /* 0x21814 */ s32             taskId_BdgPRI[6];
    /* 0x2182C */ s32             taskId_SlotCover[6];
    /* 0x21844 */ s32             taskId_BdgCur;
    /* 0x21848 */ s32             taskId_Mov;
    /* 0x2184C */ s32             taskId_Shadow;
    /* 0x21850 */ s32             taskId_GbgBox;
    /* 0x21854 */ s32             taskId_Sbar;
    /* 0x21858 */ s32             taskId_Tab;
    /* 0x2185C */ s32             taskId_TabDeck;
    /* 0x21860 */ s32             taskId_TabBdgType;
    /* 0x21864 */ s32             taskId_NumMoney;
    /* 0x21868 */ s32             taskId_Window;
    /* 0x2186C */ s32             taskId_TextScr;
    /* 0x21870 */ s32             taskId_HelpCur;
    /* 0x21874 */ s32             taskId_StkmstIn;
    /* 0x21878 */ s32             taskId_Pointer;
    /* 0x2187C */ s32             taskId_NameU;
    /* 0x21880 */ s32             taskId_BdgU;
    /* 0x21884 */ s32             taskId_BrdLogoU;
    /* 0x21888 */ s32             taskId_TextScrU;
    /* 0x2188C */ s32             taskId_NumBdgIdU;
    /* 0x21890 */ s32             taskId_BpGaugeU;
    /* 0x21894 */ s32             taskId_HelpCurU;
    /* 0x21898 */ s32             taskId_MstStarU;
    /* 0x2189C */ s32             exitReady;
    /* 0x218A0 */ s16             timer;
    /* 0x218A2 */ u16             inputDelay;
    /* 0x218A4 */ MenuBadgeObject menuBadge;
} MenuBadgeState; // Size: 0x2CB18

static MenuBadgeState* data_ov043_020cd280;

void GX_LoadBgPltt(void* src, u32 offset, u32 size);
void GX_LoadObjPltt(void* src, u32 offset, u32 size);
void GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void GXs_LoadObjPltt(void* src, u32 offset, u32 size);
BOOL func_02001b44(s32, s32, s32*, s32);
void func_02023d00(s32);
s32  func_02023d1c(s32);
void func_0202b878(void);

extern void ProcessOverlay_MenuTop(void* state);
extern void func_ov027_020e860c(void);

void MenuBadge_RefreshBadgeInfo(MenuBadgeState* state);
void MenuBadge_RefreshList(MenuBadgeObject* menuBadge, u8 listMode, u16 listTop);
void MenuBadge_StageFadeIn(MenuBadgeState* state);
void MenuBadge_StageOpenHelp(MenuBadgeState* state);
void MenuBadge_StageMain(MenuBadgeState* state);
void MenuBadge_StageErrorMessage(MenuBadgeState* state);
void MenuBadge_StageBeginSell(MenuBadgeState* state);
void MenuBadge_StageSellConfirm(MenuBadgeState* state);
void MenuBadge_StageCloseSellWindow(MenuBadgeState* state);
void MenuBadge_StageSortWindow(MenuBadgeState* state);
void MenuBadge_StageCloseSortWindow(MenuBadgeState* state);
void MenuBadge_StageHelp(MenuBadgeState* state);
void MenuBadge_StageCloseHelp(MenuBadgeState* state);
void MenuBadge_StageFadeOut(MenuBadgeState* state);
void MenuBadge_Init(MenuBadgeState* state);
void MenuBadge_Update(MenuBadgeState* state);
void MenuBadge_Destroy(MenuBadgeState* state);
void MenuBadge_RegisterVBlank(void);
void MenuBadge_DeregisterVBlank(void);

static const char* data_ov043_020cb9c8 = "Seq_MenuBadge()";

static const OverlayProcess OvlProc_MenuBadge = {
    .init = (OverlayCB)MenuBadge_Init,
    .main = (OverlayCB)MenuBadge_Update,
    .exit = (OverlayCB)MenuBadge_Destroy,
};

const BinIdentifier MenuBadge_BinIdentifiers[15] = {
    [13] = {0x2B,              "Data/BadgeData.bin"},
    [8]  = {0x2B,           "Apl_Tak/Grp_Badge.bin"},
    [14] = {0x2B,        "Apl_Tak/BeBadge_Parm.bin"},
    [10] = {0x2B,        "Apl_Tak/Grp_MenuLuck.bin"},
    [11] = {0x2B,        "Apl_Tak/Grp_MenuIcon.bin"},
    [1]  = {0x2B,        "Apl_Tak/Grp_Menu_BGD.bin"},
    [4]  = {0x2B,        "Apl_Tak/Grp_Menu_BGU.bin"},
    [12] = {0x2B,       "Apl_Tak/Grp_BrandLogo.bin"},
    [9]  = {0x2B,      "Apl_Tak/Grp_DummyBadge.bin"},
    [6]  = {0x2B,    "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [7]  = {0x2B,    "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [3]  = {0x2B, "Apl_Tak/Grp_MenuBadge_BGU00.bin"},
    [5]  = {0x2B, "Apl_Tak/Grp_MenuBadge_OBU00.bin"},
    [0]  = {0x2B, "Apl_Tak/Grp_MenuBadge_BGD00.bin"},
    [2]  = {0x2B, "Apl_Tak/Grp_MenuBadge_OBD00.bin"},
};

// Position of each badge slot: 0-5 the current deck, 6-29 the visible list page.
const Point MenuBadge_SlotPositions[30] = {
    { 21,  39},
    { 55,  39},
    { 89,  39},
    {123,  39},
    {157,  39},
    {191,  39},
    { 19,  80},
    { 49,  80},
    { 79,  80},
    {109,  80},
    {139,  80},
    {169,  80},
    {199,  80},
    {229,  80},
    { 19, 116},
    { 49, 116},
    { 79, 116},
    {109, 116},
    {139, 116},
    {169, 116},
    {199, 116},
    {229, 116},
    { 19, 152},
    { 49, 152},
    { 79, 152},
    {109, 152},
    {139, 152},
    {169, 152},
    {199, 152},
    {229, 152},
};

void MenuBadge_DestroySlotTasks(MenuBadgeObject* menuBadge, s16 index, u8 listMode) {
    MenuBadgeState* state = data_ov043_020cd280;

    Sprite_Release(MenuBadge_bdg_GetTaskData(&state->base.taskPool, state->taskId_Bdg[index]));
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Bdg[index]);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgLV[index]);
    if (listMode == 0) {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgBP[index]);
    } else if (index < 6) {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgBP[index]);
    } else {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgNum[index]);
    }
    if (index < 6) {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgSC[index]);
    }
}

void MenuBadge_CreateSlotTasks(MenuBadgeObject* menuBadge, s32 index, u8 listMode) {
    MenuBadgeState* state = data_ov043_020cd280;
    u16             slot  = index;

    state->taskId_Bdg[index]   = MenuBadge_bdg_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
    state->taskId_BdgLV[index] = MenuBadge_bdgLV_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
    if (listMode == 0) {
        state->taskId_BdgBP[index] = MenuBadge_bdgBP_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
    } else if (index < 6) {
        state->taskId_BdgBP[index] = MenuBadge_bdgBP_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
    } else {
        state->taskId_BdgNum[index] =
            MenuBadge_bdgNum_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
    }
    if (index < 6) {
        state->taskId_BdgSC[index] = MenuBadge_bdgSC_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
    }
}

void MenuBadge_ClearSlot(MenuBadgeObject* menuBadge, u16 index, u8 listMode) {
    s16 slot = index;

    MenuBadge_DestroySlotTasks(menuBadge, slot, listMode);
    *menuBadge->slots[index] = MenuBadge_EmptyEntry;
    MenuBadge_CreateSlotTasks(menuBadge, slot, listMode);
}

void MenuBadge_RefreshSlot(MenuBadgeObject* menuBadge, u16 index, u8 listMode) {
    s16 slot = index;

    MenuBadge_DestroySlotTasks(menuBadge, slot, listMode);
    MenuBadge_CreateSlotTasks(menuBadge, slot, listMode);
}

void MenuBadge_InitState(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;
    u16              i;
    u16              k;
    u16              j;
    u16              l;

    state->exitReady = 0;
    state->timer     = 0;
    menuBadge->flags = 0;
    MenuBadge_InitBadgeData(menuBadge);
    menuBadge->dragSrc          = 0;
    menuBadge->dragDst          = 0;
    menuBadge->unk_AEF0         = 0;
    menuBadge->touchPos.x       = 0;
    menuBadge->touchPos.y       = 0;
    menuBadge->listTop          = 0;
    menuBadge->prevListTop      = 0;
    menuBadge->listMode         = 0;
    menuBadge->infoTab          = 0;
    menuBadge->buttonPressed[0] = 0;
    menuBadge->buttonPressed[1] = 0;
    menuBadge->buttonPressed[2] = 0;
    menuBadge->pressTimer       = 0;
    menuBadge->cursorSlot       = 0;
    menuBadge->cursorListIndex  = 0xFFFF;
    menuBadge->badgeVramPage    = 0;
    menuBadge->exitDest         = 0;
    menuBadge->sellCount        = 1;
    menuBadge->unk_AF22         = 0;
    menuBadge->windowCloseTimer = 0;
    for (i = 0; i < 2; i++) {
        menuBadge->sellButtonPressed[i] = 0;
    }
    for (j = 0; j < 2; j++) {
        menuBadge->sellArrowPressed[j] = 0;
    }
    for (k = 0; k < 30; k++) {
        menuBadge->slotVisible[k] = TRUE;
    }
    for (l = 0; l < 3; l++) {
        menuBadge->helpButtonPressed[l] = 0;
    }
    menuBadge->helpPage       = 0;
    menuBadge->windowMessage  = 0;
    menuBadge->excessPinCount = 0;
    state->inputDelay         = 0;
}

void MenuBadge_CreateTasks(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    EasyTask_CreateTask(&state->base.taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -0x10, 0x1000);
    state->taskId_Pointer = MenuBadge_pointer_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_Icon    = MenuBadge_icon_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_Tab     = MenuBadge_tab_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    for (u16 i = 0; i < 30; i++) {
        MenuBadge_CreateSlotTasks(menuBadge, (s16)i, menuBadge->listMode);
    }
    for (u16 i = 0; i < 6; i++) {
        state->taskId_BdgPRI[i] = MenuBadge_bdgPRI_CreateTask(&state->base.taskPool, state->base.dataType, i, menuBadge);
    }
    for (u16 i = 0; i < 6; i++) {
        if (i >= menuBadge->deckSlotCount) {
            state->taskId_SlotCover[i] =
                MenuBadge_slotCover_CreateTask(&state->base.taskPool, state->base.dataType, i, menuBadge);
        }
    }
    state->taskId_BdgCur     = MenuBadge_bdgCur_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_NameU      = MenuBadge_nameU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_GbgBox     = MenuBadge_gbgBox_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_Sbar       = MenuBadge_sbar_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_TabDeck    = MenuBadge_tabDeck_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_TabBdgType = MenuBadge_tabBdgType_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_NumMoney   = MenuBadge_numMoney_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_TextScr    = MenuBadge_textScr_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_BdgU       = MenuBadge_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_TextScrU   = MenuBadge_textScrU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_BrdLogoU   = MenuBadge_brdLogoU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_NumBdgIdU  = MenuBadge_numBdgIdU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_BpGaugeU   = MenuBadge_bpGaugeU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_MstStarU   = MenuBadge_mstStarU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
}

s32 MenuBadge_InsertIntoStockpile(MenuBadgeObject* menuBadge, MenuBadgeEntry* entry) {
    for (u16 i = 0; i < 256; i++) {
        if (menuBadge->stockpile[i].pinId == 0xFFFF) {
            menuBadge->stockpile[i] = *entry;
            if (menuBadge->listMode == 0) {
                if ((i >= menuBadge->listTop) && (i < (menuBadge->listTop + 24))) {
                    u16 slot = (i - menuBadge->listTop) + 6;
                    if ((slot != menuBadge->dragSrc) && (slot != menuBadge->dragDst)) {
                        MenuBadge_DestroySlotTasks(menuBadge, (s16)slot, 0);
                        MenuBadge_CreateSlotTasks(menuBadge, (s16)slot, 0);
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 MenuBadge_StackIntoMastered(MenuBadgeObject* menuBadge, u16 pinId) {
    for (u16 i = 0; i < 304; i++) {
        if (pinId == menuBadge->mastered[i].pinId) {
            menuBadge->mastered[i].count++;
            if (menuBadge->mastered[i].count > 99) {
                menuBadge->mastered[i].count = 99;
            }
            if (menuBadge->listMode == 1) {
                if ((i >= menuBadge->listTop) && (i < (menuBadge->listTop + 24))) {
                    u16 slot = (i - menuBadge->listTop) + 6;
                    if ((slot != menuBadge->dragSrc) && (slot != menuBadge->dragDst)) {
                        MenuBadge_DestroySlotTasks(menuBadge, (s16)slot, 1);
                        MenuBadge_CreateSlotTasks(menuBadge, (s16)slot, 1);
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 MenuBadge_InsertIntoMastered(MenuBadgeObject* menuBadge, MenuBadgeEntry* entry) {
    for (u16 i = 0; i < 304; i++) {
        if (menuBadge->mastered[i].pinId == 0xFFFF) {
            menuBadge->mastered[i] = *entry;
            if (menuBadge->listMode == 1) {
                if ((i >= menuBadge->listTop) && (i < (menuBadge->listTop + 24))) {
                    u16 slot = (i - menuBadge->listTop) + 6;
                    if ((slot != menuBadge->dragSrc) && (slot != menuBadge->dragDst)) {
                        MenuBadge_DestroySlotTasks(menuBadge, (s16)slot, 1);
                        MenuBadge_CreateSlotTasks(menuBadge, (s16)slot, 1);
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 MenuBadge_AddToMastered(MenuBadgeObject* menuBadge, MenuBadgeEntry* entry) {
    if (MenuBadge_StackIntoMastered(menuBadge, entry->pinId) == 1) {
        return 1;
    }
    if (MenuBadge_InsertIntoMastered(menuBadge, entry) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuBadge_ReturnToMastered(MenuBadgeObject* menuBadge, MenuBadgeEntry* entry, u16 slot) {
    if (MenuBadge_StackIntoMastered(menuBadge, entry->pinId) != 1) {
        *menuBadge->slots[slot]       = *entry;
        menuBadge->slots[slot]->count = 2;
    }
    return 1;
}

s32 MenuBadge_StockpileSwapDeckSlots(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    *menuBadge->slots[src] = *menuBadge->slots[dst];
    *menuBadge->slots[dst] = menuBadge->dragBadge;
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_StockpileUnequip(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    MenuBadgeState* state = data_ov043_020cd280;

    if (MenuBadge_IsSlotMastered(menuBadge, src) == 0) {
        if (MenuBadge_InsertIntoStockpile(menuBadge, &menuBadge->dragBadge) == 0) {
            return 0;
        }
    } else {
        MenuBadge_AddToMastered(menuBadge, &menuBadge->dragBadge);
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_STOCK_MASTER_IN);
        state->taskId_StkmstIn =
            MenuBadge_stkmstIn_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge->touchPos, 1, menuBadge);
    }
    MenuBadge_ClearSlot(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_StockpileEquip(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    if (menuBadge->slots[dst]->pinId != 0xFFFF) {
        if (MenuBadge_IsSlotMastered(menuBadge, dst) == 0) {
            *menuBadge->slots[src] = *menuBadge->slots[dst];
            *menuBadge->slots[dst] = menuBadge->dragBadge;
        } else {
            MenuBadge_AddToMastered(menuBadge, menuBadge->slots[dst]);
            *menuBadge->slots[dst] = menuBadge->dragBadge;
            *menuBadge->slots[src] = MenuBadge_EmptyEntry;
            {
                MenuBadgeState* state = data_ov043_020cd280;
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_STOCK_MASTER_IN);
                state->taskId_StkmstIn = MenuBadge_stkmstIn_CreateTask(&state->base.taskPool, state->base.dataType,
                                                                       menuBadge->touchPos, 1, menuBadge);
            }
        }
    } else {
        *menuBadge->slots[src] = *menuBadge->slots[dst];
        *menuBadge->slots[dst] = menuBadge->dragBadge;
    }
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_StockpileMoveBadge(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    *menuBadge->slots[src] = *menuBadge->slots[dst];
    *menuBadge->slots[dst] = menuBadge->dragBadge;
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_MasteredSwapDeckSlots(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    *menuBadge->slots[src] = *menuBadge->slots[dst];
    *menuBadge->slots[dst] = menuBadge->dragBadge;
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_MasteredUnequip(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    MenuBadgeState* state = data_ov043_020cd280;

    if (MenuBadge_IsSlotMastered(menuBadge, src) == 0) {
        if (MenuBadge_InsertIntoStockpile(menuBadge, &menuBadge->dragBadge) == 0) {
            return 0;
        }
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_STOCK_MASTER_IN);
        state->taskId_StkmstIn =
            MenuBadge_stkmstIn_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge->touchPos, 0, menuBadge);
    } else {
        MenuBadge_AddToMastered(menuBadge, &menuBadge->dragBadge);
    }
    MenuBadge_ClearSlot(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_MasteredEquip(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    if (menuBadge->slots[dst]->pinId != 0xFFFF) {
        if (MenuBadge_IsSlotMastered(menuBadge, dst) == 0) {
            if (MenuBadge_InsertIntoStockpile(menuBadge, menuBadge->slots[dst]) == 0) {
                return 0;
            }
            {
                MenuBadgeState* state = data_ov043_020cd280;
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_STOCK_MASTER_IN);
                state->taskId_StkmstIn = MenuBadge_stkmstIn_CreateTask(&state->base.taskPool, state->base.dataType,
                                                                       menuBadge->touchPos, 0, menuBadge);
            }
        } else if (menuBadge->slots[src]->pinId == menuBadge->slots[dst]->pinId) {
            MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
            MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
            return 1;
        } else if (menuBadge->slots[src]->count == 1) {
            MenuBadge_ReturnToMastered(menuBadge, menuBadge->slots[dst], src);
        } else {
            MenuBadge_AddToMastered(menuBadge, menuBadge->slots[dst]);
        }
    }
    *menuBadge->slots[dst]       = menuBadge->dragBadge;
    menuBadge->slots[dst]->count = 1;
    menuBadge->slots[src]->count--;
    if (menuBadge->slots[src]->count == 0) {
        *menuBadge->slots[src] = MenuBadge_EmptyEntry;
    }
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_MasteredMoveBadge(MenuBadgeObject* menuBadge, s32 src, s32 dst) {
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_DestroySlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    *menuBadge->slots[src] = *menuBadge->slots[dst];
    *menuBadge->slots[dst] = menuBadge->dragBadge;
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    MenuBadge_CreateSlotTasks(menuBadge, menuBadge->dragDst, menuBadge->listMode);
    return 1;
}

s32 MenuBadge_DropBadge(MenuBadgeObject* menuBadge, u8 listMode, s32 src, s32 dst) {
    MenuBadgeState* state = data_ov043_020cd280;

    if (listMode == 0) {
        if (src < 6) {
            if (dst < 6) {
                MenuBadge_StockpileSwapDeckSlots(menuBadge, src, dst);
            } else {
                MenuBadge_StockpileUnequip(menuBadge, src, dst);
            }
        } else if (dst < 6) {
            if (MenuBadge_CanEquipToDeck(menuBadge, src, dst) == 1) {
                MenuBadge_StockpileEquip(menuBadge, src, dst);
            } else {
                return 0;
            }
        } else {
            MenuBadge_StockpileMoveBadge(menuBadge, src, dst);
        }
    } else {
        if (src < 6) {
            if (dst < 6) {
                MenuBadge_MasteredSwapDeckSlots(menuBadge, src, dst);
            } else {
                MenuBadge_MasteredUnequip(menuBadge, src, dst);
            }
        } else if (dst < 6) {
            if (MenuBadge_CanEquipToDeck(menuBadge, src, dst) == 1) {
                MenuBadge_MasteredEquip(menuBadge, src, dst);
            } else {
                return 0;
            }
        } else {
            MenuBadge_MasteredMoveBadge(menuBadge, src, dst);
        }
    }

    if ((src < 6) && (dst == 40)) {
        menuBadge->cursorSlot      = src;
        menuBadge->cursorListIndex = 0xFFFF;
        menuBadge->cursorBadge     = MenuBadge_EmptyEntry;
        MenuBadge_RefreshBadgeInfo(state);
    } else if (dst < 6) {
        menuBadge->cursorSlot      = dst;
        menuBadge->cursorListIndex = 0xFFFF;
    } else if (menuBadge->autoArrange != 1) {
        menuBadge->cursorSlot      = dst;
        menuBadge->cursorListIndex = menuBadge->listTop + (menuBadge->cursorSlot - 6);
    }
    if (menuBadge->autoArrange == 1) {
        if (menuBadge->autoArrangeBy[0] == 1) {
            MenuBadge_SortList(menuBadge, menuBadge->listMode, 0);
        } else {
            MenuBadge_SortList(menuBadge, menuBadge->listMode, 1);
        }
        MenuBadge_RefreshList(menuBadge, menuBadge->listMode, menuBadge->listTop);
    }
    return 1;
}

void MenuBadge_RefreshBadgeInfo(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgU);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BrdLogoU);
    state->taskId_BdgU     = MenuBadge_bdgU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_BrdLogoU = MenuBadge_brdLogoU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
}

void MenuBadge_SwitchDeck(MenuBadgeObject* menuBadge, u8 listMode, u8 deck) {
    for (s16 i = 0; i < 6; i++) {
        MenuBadge_DestroySlotTasks(menuBadge, i, listMode);
    }
    for (s16 i = 0; i < 6; i++) {
        menuBadge->slots[i] = &menuBadge->decks[deck][i];
    }
    for (s16 i = 0; i < 6; i++) {
        MenuBadge_CreateSlotTasks(menuBadge, i, listMode);
    }
}

void MenuBadge_RefreshList(MenuBadgeObject* menuBadge, u8 listMode, u16 listTop) {
    for (s16 i = 6; i < 30; i++) {
        MenuBadge_DestroySlotTasks(menuBadge, i, listMode);
    }
    if (listMode == 0) {
        for (s16 i = 0; i < 24; i++) {
            menuBadge->slots[6 + i] = &menuBadge->stockpile[listTop + i];
        }
    } else {
        for (s16 i = 0; i < 24; i++) {
            menuBadge->slots[6 + i] = &menuBadge->mastered[listTop + i];
        }
    }
    for (s16 i = 6; i < 30; i++) {
        MenuBadge_CreateSlotTasks(menuBadge, i, listMode);
    }
}

void MenuBadge_SwitchList(MenuBadgeObject* menuBadge, u8 listMode, s16 listTop) {
    MenuBadgeState* state = data_ov043_020cd280;
    s16             i;

    for (s16 i = 6; i < 30; i++) {
        MenuBadge_DestroySlotTasks(menuBadge, i, 1 - listMode);
    }
    if (listMode == 0) {
        for (i = 0; i < 24; i++) {
            menuBadge->slots[6 + i] = &menuBadge->stockpile[listTop + i];
        }
    } else {
        for (i = 0; i < 24; i++) {
            menuBadge->slots[6 + i] = &menuBadge->mastered[listTop + i];
        }
    }
    for (s16 i = 6; i < 30; i++) {
        MenuBadge_CreateSlotTasks(menuBadge, i, listMode);
    }
    menuBadge->listTop         = listTop;
    menuBadge->cursorSlot      = 6;
    menuBadge->cursorListIndex = 0;
    menuBadge->cursorBadge     = *menuBadge->slots[menuBadge->cursorSlot];
    MenuBadge_RefreshBadgeInfo(state);
}

void func_ov043_0208d194(MenuBadgeObject* menuBadge) {
    u16             slot  = menuBadge->cursorSlot;
    MenuBadgeState* state = data_ov043_020cd280;

    if (slot < 6) {
        if ((menuBadge->slots[slot]->unk_16 != 2) && (menuBadge->slots[slot]->pinId != 0xFFFF)) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            EasyTask_DeleteTask(&state->base.taskPool, state->taskId_BdgSC[slot]);
            menuBadge->slots[slot]->unk_16 = 1 - menuBadge->slots[slot]->unk_16;
            state->taskId_BdgSC[slot] =
                MenuBadge_bdgSC_CreateTask(&state->base.taskPool, state->base.dataType, slot, menuBadge);
        }
    }
}

void MenuBadge_DestroyDragTasks(MenuBadgeState* state) {
    Sprite_Release(MenuBadge_bdg_GetTaskData(&state->base.taskPool, state->taskId_Mov));
    Sprite_Release(MenuBadge_bdg_GetTaskData(&state->base.taskPool, state->taskId_Shadow));
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Mov);
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Shadow);
}

void MenuBadge_UpdateCursorBadge(MenuBadgeObject* menuBadge) {
    if (menuBadge->cursorListIndex == 0xFFFF) {
        menuBadge->cursorBadge = *menuBadge->slots[menuBadge->cursorSlot];
    } else if (menuBadge->listMode == 0) {
        menuBadge->cursorBadge = menuBadge->stockpile[menuBadge->cursorListIndex];
    } else {
        menuBadge->cursorBadge = menuBadge->mastered[menuBadge->cursorListIndex];
    }
}

void MenuBadge_UpdateButtonInput(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;
    u32              count;

    if (menuBadge->listMode == 0) {
        count = 256;
    } else {
        count = 304;
    }
    if (!(InputStatus.buttonState.currButtons & INPUT_ABXY) && !(InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->inputDelay = 0;
    }
    if (InputStatus.buttonState.pressedButtons & (INPUT_BUTTON_L | INPUT_BUTTON_R)) {
        func_ov043_0208d194(menuBadge);
        goto block_72;
    }
    if (state->inputDelay != 0) {
        state->inputDelay--;
        return;
    }
    if ((InputStatus.buttonState.pressedButtons & INPUT_ABXY) || (InputStatus.buttonState.pressedButtons & INPUT_DPAD)) {
        state->inputDelay = 20;
    } else if ((InputStatus.buttonState.currButtons & INPUT_ABXY) || (InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->inputDelay = 1;
    }
    if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_UP) || (InputStatus.buttonState.currButtons & INPUT_BUTTON_X)) {
        if (menuBadge->cursorListIndex != 0xFFFF) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if ((menuBadge->cursorListIndex >= menuBadge->listTop) && (menuBadge->cursorListIndex < (menuBadge->listTop + 24)))
            {
                if (menuBadge->cursorListIndex < 8) {
                    menuBadge->cursorSlot      = 0;
                    menuBadge->cursorListIndex = 0xFFFF;
                } else if ((menuBadge->cursorSlot >= 6) && (menuBadge->cursorSlot < 14)) {
                    menuBadge->listTop -= 8;
                    menuBadge->cursorListIndex -= 8;
                } else {
                    menuBadge->cursorSlot -= 8;
                    menuBadge->cursorListIndex -= 8;
                }
            } else {
                menuBadge->cursorSlot      = 6;
                menuBadge->cursorListIndex = menuBadge->listTop;
            }
        }
        MenuBadge_UpdateCursorBadge(menuBadge);
        MenuBadge_RefreshBadgeInfo(state);
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_DOWN) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_B))
    {
        if (menuBadge->cursorListIndex == 0xFFFF) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            menuBadge->listTop         = 0;
            menuBadge->cursorSlot      = 6;
            menuBadge->cursorListIndex = 0;
        } else if ((menuBadge->cursorListIndex >= menuBadge->listTop) &&
                   (menuBadge->cursorListIndex < (menuBadge->listTop + 24)))
        {
            if (menuBadge->cursorListIndex < (s32)(count - 8)) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                if ((menuBadge->cursorSlot >= 22) && (menuBadge->cursorSlot < 30)) {
                    menuBadge->listTop += 8;
                    menuBadge->cursorListIndex += 8;
                } else {
                    menuBadge->cursorSlot += 8;
                    menuBadge->cursorListIndex += 8;
                }
            }
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            menuBadge->cursorSlot      = 6;
            menuBadge->cursorListIndex = menuBadge->listTop;
        }
        MenuBadge_UpdateCursorBadge(menuBadge);
        MenuBadge_RefreshBadgeInfo(state);
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_LEFT) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_Y))
    {
        if (menuBadge->cursorListIndex == 0xFFFF) {
            if (menuBadge->cursorSlot != 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                menuBadge->cursorSlot--;
            }
            menuBadge->cursorListIndex = 0xFFFF;
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if ((menuBadge->cursorListIndex >= menuBadge->listTop) && (menuBadge->cursorListIndex < (menuBadge->listTop + 24)))
            {
                if (menuBadge->cursorListIndex == 0) {
                    menuBadge->listTop         = ((count >> 3) - 3) * 8;
                    menuBadge->cursorSlot      = 29;
                    menuBadge->cursorListIndex = count - 1;
                } else if (menuBadge->cursorSlot == 6) {
                    menuBadge->listTop -= 8;
                    menuBadge->cursorSlot = 13;
                    menuBadge->cursorListIndex--;
                } else {
                    menuBadge->cursorSlot--;
                    menuBadge->cursorListIndex--;
                }
            } else {
                menuBadge->cursorSlot      = 6;
                menuBadge->cursorListIndex = menuBadge->listTop;
            }
        }
        MenuBadge_UpdateCursorBadge(menuBadge);
        MenuBadge_RefreshBadgeInfo(state);
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_RIGHT) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_A))
    {
        if (menuBadge->cursorListIndex == 0xFFFF) {
            if (menuBadge->cursorSlot < 5) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                menuBadge->cursorSlot++;
            }
            menuBadge->cursorListIndex = 0xFFFF;
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if ((menuBadge->cursorListIndex >= menuBadge->listTop) && (menuBadge->cursorListIndex < (menuBadge->listTop + 24)))
            {
                if (menuBadge->cursorListIndex == (count - 1)) {
                    menuBadge->listTop         = 0;
                    menuBadge->cursorSlot      = 6;
                    menuBadge->cursorListIndex = 0;
                } else if (menuBadge->cursorSlot == 29) {
                    menuBadge->listTop += 8;
                    menuBadge->cursorSlot = 22;
                    menuBadge->cursorListIndex++;
                } else {
                    menuBadge->cursorSlot++;
                    menuBadge->cursorListIndex++;
                }
            } else {
                menuBadge->cursorSlot      = 6;
                menuBadge->cursorListIndex = menuBadge->listTop;
            }
        }
        MenuBadge_UpdateCursorBadge(menuBadge);
        MenuBadge_RefreshBadgeInfo(state);
    }
block_72:
    if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        menuBadge->currentDeck++;
        if (menuBadge->currentDeck >= 4) {
            menuBadge->currentDeck = 0;
        }
        MenuBadge_SwitchDeck(menuBadge, menuBadge->listMode, menuBadge->currentDeck);
        menuBadge->cursorBadge = *menuBadge->slots[menuBadge->cursorSlot];
        MenuBadge_RefreshBadgeInfo(state);
    }
}

void MenuBadge_StageFadeIn(MenuBadgeState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        if (func_02023d1c(4) == 0) {
            state->timer = 30;
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageOpenHelp, state, PROCESS_STAGE_INIT);
        } else {
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageMain, state, PROCESS_STAGE_INIT);
        }
    }
}

void MenuBadge_StageOpenHelp(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (state->timer > 0) {
        state->timer--;
        return;
    }

    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
    state->taskId_HelpCur    = MenuBadge_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    state->taskId_HelpCurU   = MenuBadge_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    menuBadge->windowMessage = 1;
    menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageHelp, state, PROCESS_STAGE_INIT);
}

void MenuBadge_StageMain(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;
    TouchCoord       touch;

    if (menuBadge->flags & MENUBADGE_FLAG_DRAGGING) {
        TouchInput_GetCoord(&touch);
        menuBadge->touchPos.x = touch.x;
        menuBadge->touchPos.y = touch.y;
        if (MenuBadge_IsPointOnSellBox(menuBadge->touchPos.x, menuBadge->touchPos.y) == 1) {
            menuBadge->flags &= ~MENUBADGE_FLAG_DROP_VALID;
            if (TouchInput_WasTouchReleased()) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CLICK02);
                menuBadge->flags &= ~MENUBADGE_FLAG_DRAGGING;
                EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Mov);
                EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Shadow);
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageBeginSell, state, PROCESS_STAGE_INIT);
                return;
            }
        }
        menuBadge->dragDst = MenuBadge_GetDropTargetAtPoint(menuBadge->touchPos.x, menuBadge->touchPos.y, menuBadge->dragSrc,
                                                            menuBadge->deckSlotCount);
        if (MenuBadge_CanDropOnSlot(menuBadge, menuBadge->dragSrc, menuBadge->dragDst) == 1) {
            menuBadge->flags |= MENUBADGE_FLAG_DROP_VALID;
            if (TouchInput_WasTouchReleased()) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CLICK02);
                MenuBadge_DestroyDragTasks(state);
                if (MenuBadge_DropBadge(menuBadge, menuBadge->listMode, menuBadge->dragSrc, menuBadge->dragDst) == 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_KINSHI);
                    menuBadge->windowMessage = menuBadge->slots[menuBadge->dragSrc]->pinClass + 5;
                    menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
                    state->taskId_Window =
                        MenuBadge_window2_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
                    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
                    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageErrorMessage, state, PROCESS_STAGE_INIT);
                }
                menuBadge->slotVisible[menuBadge->dragSrc] = TRUE;
                menuBadge->flags &= ~MENUBADGE_FLAG_DRAGGING;
                menuBadge->flags &= ~MENUBADGE_FLAG_DROP_VALID;
            }
        } else {
            menuBadge->flags &= ~MENUBADGE_FLAG_DROP_VALID;
            if (TouchInput_WasTouchReleased()) {
                MenuBadge_DestroyDragTasks(state);
                menuBadge->slotVisible[menuBadge->dragSrc] = TRUE;
                menuBadge->flags &= ~MENUBADGE_FLAG_DRAGGING;
            }
        }
    } else if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&touch);
        menuBadge->dragSrc = MenuBadge_GetSlotAtPoint(touch.x, touch.y, menuBadge->deckSlotCount);
        if (MenuBadge_CanDragSlot(menuBadge, menuBadge->dragSrc) == 1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CLICK01);
            menuBadge->touchPos.x = touch.x;
            menuBadge->touchPos.y = touch.y;
            menuBadge->flags |= MENUBADGE_FLAG_DRAGGING;
            menuBadge->cursorSlot  = menuBadge->dragSrc;
            menuBadge->cursorBadge = *menuBadge->slots[menuBadge->cursorSlot];
            menuBadge->dragBadge   = menuBadge->cursorBadge;
            if (menuBadge->dragSrc < 6) {
                menuBadge->cursorListIndex = 0xFFFF;
            } else {
                menuBadge->cursorListIndex = menuBadge->listTop + (menuBadge->dragSrc - 6);
            }
            menuBadge->slotVisible[menuBadge->dragSrc] = FALSE;
            state->taskId_Mov    = MenuBadge_mov_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
            state->taskId_Shadow = MenuBadge_shadow_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
            MenuBadge_RefreshBadgeInfo(state);
            return;
        }

        s16 listMode = MenuBadge_GetListTabAtPoint(touch.x, touch.y);
        if ((listMode != -1) && (listMode != menuBadge->listMode)) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            menuBadge->listMode = listMode;
            if (menuBadge->cursorListIndex == 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            }
            if (menuBadge->autoArrange == 1) {
                if (menuBadge->autoArrangeBy[0] == 1) {
                    MenuBadge_SortList(menuBadge, menuBadge->listMode, 0);
                } else {
                    MenuBadge_SortList(menuBadge, menuBadge->listMode, 1);
                }
            }
            MenuBadge_SwitchList(menuBadge, menuBadge->listMode, 0);
            MenuBadge_ReloadBgScreen(&menuBadge->resources[7], menuBadge->resources[7].data, DISPLAY_MAIN, 3,
                                     (menuBadge->listMode * 2) + 2);
            return;
        }

        s32 deck = MenuBadge_GetDeckTabAtPoint(touch.x, touch.y);
        if (deck != -1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            menuBadge->currentDeck = (s8)deck; // (s8): keeps the store and the u8 argument below as separate conversions
            MenuBadge_SwitchDeck(menuBadge, menuBadge->listMode, deck);
            menuBadge->cursorBadge = *menuBadge->slots[menuBadge->cursorSlot];
            MenuBadge_RefreshBadgeInfo(state);
        }

        s16 button = MenuBadge_GetButtonAtPoint(touch.x, touch.y);
        if (button != -1) {
            menuBadge->buttonPressed[button] = 1;
            menuBadge->pressTimer            = 12;
            if (button == 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                menuBadge->windowMessage = 3;
                menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
                state->taskId_Window = MenuBadge_window1_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageSortWindow, state, PROCESS_STAGE_INIT);
                return;
            }
            if (button == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
                state->taskId_HelpCur  = MenuBadge_helpCur_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
                state->taskId_HelpCurU = MenuBadge_helpCurU_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
                menuBadge->windowMessage = 1;
                menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageHelp, state, PROCESS_STAGE_INIT);
                return;
            }
            u32 total = MenuBadge_CountUnmasteredBadges(menuBadge);
            if (total > 200) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_KINSHI);
                menuBadge->excessPinCount = total - 200;
                menuBadge->windowMessage  = 10;
                menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
                state->taskId_Window = MenuBadge_window2_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageErrorMessage, state, PROCESS_STAGE_INIT);
                return;
            }
            menuBadge->flags |= MENUBADGE_FLAG_EXITING;
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
            SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
            if (gSaveData.unk_3130 == 3) {
                menuBadge->exitDest = 1;
            } else {
                menuBadge->exitDest = 0;
            }
            DebugOvlDisp_Pop();
            return;
        }
    } else if (TouchInput_IsTouchActive() == FALSE) {
        MenuBadge_UpdateButtonInput(state);
    }

    if (menuBadge->prevListTop != menuBadge->listTop) {
        MenuBadge_RefreshList(menuBadge, menuBadge->listMode, menuBadge->listTop);
        menuBadge->prevListTop = menuBadge->listTop;
    }
}

void MenuBadge_StageErrorMessage(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (TouchInput_WasTouchPressed()) {
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Window);
        g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
        g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
        menuBadge->windowMessage                   = 0;
        menuBadge->slotVisible[menuBadge->dragSrc] = TRUE;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void MenuBadge_ClearSoldSlot(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    MenuBadge_ClearSlot(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
    menuBadge->cursorBadge = MenuBadge_EmptyEntry;
    MenuBadge_RefreshBadgeInfo(state);
}

void MenuBadge_SellBadge(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;
    u32              price     = MenuBadge_CalcSellPrice(menuBadge->cursorBadge.price, menuBadge->cursorBadge.priceGrowth,
                                                         menuBadge->cursorBadge.level);

    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    if (menuBadge->flags & MENUBADGE_FLAG_QUICK_SELL) {
        menuBadge->money += price;
        MenuBadge_ClampMoney(menuBadge, menuBadge->moneyCapLevel);
        if (MenuBadge_IsSlotMastered(menuBadge, menuBadge->dragSrc) == 0) {
            MenuBadge_ClearSoldSlot(state);
        } else {
            MenuBadge_SubtractMasteredCount(menuBadge, menuBadge->cursorBadge.pinId, 1);
            menuBadge->slots[menuBadge->cursorSlot]->count--;
            if (menuBadge->slots[menuBadge->cursorSlot]->count == 0) {
                MenuBadge_ClearSoldSlot(state);
            } else {
                MenuBadge_RefreshSlot(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
            }
        }
        menuBadge->slotVisible[menuBadge->dragSrc] = TRUE;
        menuBadge->windowMessage                   = 0;
        if (menuBadge->autoArrange == 1) {
            if (menuBadge->autoArrangeBy[0] == 1) {
                MenuBadge_SortList(menuBadge, menuBadge->listMode, 0);
            } else {
                MenuBadge_SortList(menuBadge, menuBadge->listMode, 1);
            }
            MenuBadge_RefreshList(menuBadge, menuBadge->listMode, menuBadge->listTop);
            MenuBadge_UpdateCursorBadge(menuBadge);
            menuBadge->badgeVramPage ^= 1;
            MenuBadge_RefreshBadgeInfo(state);
        }
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageMain, state, PROCESS_STAGE_INIT);
        return;
    }

    if (MenuBadge_IsSlotMastered(menuBadge, menuBadge->dragSrc) == 0) {
        menuBadge->money += price;
        MenuBadge_ClampMoney(menuBadge, menuBadge->moneyCapLevel);
        MenuBadge_ClearSoldSlot(state);
    } else {
        MenuBadge_SubtractMasteredCount(menuBadge, menuBadge->cursorBadge.pinId, menuBadge->sellCount);
        menuBadge->money += price * menuBadge->sellCount;
        MenuBadge_ClampMoney(menuBadge, menuBadge->moneyCapLevel);
        menuBadge->slots[menuBadge->cursorSlot]->count -= menuBadge->sellCount;
        if (menuBadge->slots[menuBadge->cursorSlot]->count == 0) {
            MenuBadge_ClearSoldSlot(state);
        } else {
            MenuBadge_RefreshSlot(menuBadge, menuBadge->dragSrc, menuBadge->listMode);
        }
    }
    menuBadge->windowCloseTimer = 20;
    if (menuBadge->autoArrange == 1) {
        if (menuBadge->autoArrangeBy[0] == 1) {
            MenuBadge_SortList(menuBadge, menuBadge->listMode, 0);
        } else {
            MenuBadge_SortList(menuBadge, menuBadge->listMode, 1);
        }
        MenuBadge_RefreshList(menuBadge, menuBadge->listMode, menuBadge->listTop);
        MenuBadge_UpdateCursorBadge(menuBadge);
        menuBadge->badgeVramPage ^= 1;
        MenuBadge_RefreshBadgeInfo(state);
    }
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageCloseSellWindow, state, PROCESS_STAGE_INIT);
}

void MenuBadge_StageBeginSell(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (MenuBadge_CalcSellPrice(menuBadge->cursorBadge.price, menuBadge->cursorBadge.priceGrowth,
                                menuBadge->cursorBadge.level) != 0)
    {
        if ((MenuBadge_ClampMoney(menuBadge, menuBadge->moneyCapLevel) == 1) &&
            !(menuBadge->flags & MENUBADGE_FLAG_MONEY_CAP_WARNED))
        {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_KINSHI);
            menuBadge->windowMessage = 11;
            menuBadge->flags |= MENUBADGE_FLAG_MONEY_CAP_WARNED;
            menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
            state->taskId_Window = MenuBadge_window2_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
            g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageErrorMessage, state, PROCESS_STAGE_INIT);
            return;
        }
        if (menuBadge->flags & MENUBADGE_FLAG_QUICK_SELL) {
            MenuBadge_SellBadge(state);
            return;
        }
        menuBadge->windowMessage = 2;
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
        menuBadge->sellCount = 1;
        state->taskId_Window = MenuBadge_window0_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
        g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageSellConfirm, state, PROCESS_STAGE_INIT);
        return;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_KINSHI);
    menuBadge->windowMessage = 4;
    menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
    state->taskId_Window = MenuBadge_window2_CreateTask(&state->base.taskPool, state->base.dataType, menuBadge);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageErrorMessage, state, PROCESS_STAGE_INIT);
}

void MenuBadge_StageSellConfirm(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;
    TouchCoord       touch;

    if ((TouchInput_IsTouchActive() == FALSE) && !(InputStatus.buttonState.currButtons & INPUT_ABXY) &&
        !(InputStatus.buttonState.currButtons & INPUT_DPAD))
    {
        state->inputDelay = 0;
    }
    if (state->inputDelay != 0) {
        state->inputDelay--;
        return;
    }
    if (TouchInput_WasTouchPressed()) {
        state->inputDelay = 20;
    } else if (TouchInput_IsBeingHeld()) {
        state->inputDelay = 1;
    } else if ((InputStatus.buttonState.pressedButtons & INPUT_ABXY) || (InputStatus.buttonState.pressedButtons & INPUT_DPAD))
    {
        state->inputDelay = 20;
    } else if ((InputStatus.buttonState.currButtons & INPUT_ABXY) || (InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->inputDelay = 1;
    }

    if (TouchInput_IsTouchActive()) {
        TouchInput_GetCoord(&touch);
        if (TouchInput_WasTouchPressed()) {
            s16 button = MenuBadge_GetSellButtonAtPoint(touch.x, touch.y);
            if (button != -1) {
                menuBadge->sellButtonPressed[button] = 1;
                menuBadge->pressTimer                = 12;
                if (button == 0) {
                    MenuBadge_SellBadge(state);
                } else {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                    menuBadge->windowCloseTimer = 20;
                    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageCloseSellWindow, state, PROCESS_STAGE_INIT);
                }
            }
        }
        s16 arrow = MenuBadge_GetSellCountArrowAtPoint(touch.x, touch.y);
        if (arrow == -1) {
            return;
        }
        if (MenuBadge_IsSlotMastered(menuBadge, menuBadge->dragSrc) != 1) {
            return;
        }
        menuBadge->sellArrowPressed[arrow] = 1;
        menuBadge->pressTimer              = 12;
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
        if (arrow == 0) {
            if (menuBadge->sellCount < menuBadge->cursorBadge.count) {
                menuBadge->sellCount++;
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            } else if (TouchInput_WasTouchPressed()) {
                menuBadge->sellCount = 1;
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            }
        } else if (menuBadge->sellCount > 1) {
            menuBadge->sellCount--;
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
        } else if (TouchInput_WasTouchPressed()) {
            menuBadge->sellCount = menuBadge->cursorBadge.count;
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
        }
        return;
    }

    if (!(InputStatus.buttonState.currButtons & (INPUT_BUTTON_UP | INPUT_BUTTON_DOWN | INPUT_BUTTON_B | INPUT_BUTTON_X))) {
        return;
    }
    if (MenuBadge_IsSlotMastered(menuBadge, menuBadge->dragSrc) != 1) {
        return;
    }
    menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
    if (InputStatus.buttonState.currButtons & (INPUT_BUTTON_UP | INPUT_BUTTON_X)) {
        if (menuBadge->sellCount < menuBadge->cursorBadge.count) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            menuBadge->sellCount++;
        } else if (InputStatus.buttonState.pressedButtons & (INPUT_BUTTON_UP | INPUT_BUTTON_X)) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            menuBadge->sellCount = 1;
        }
    } else if (InputStatus.buttonState.currButtons & (INPUT_BUTTON_DOWN | INPUT_BUTTON_B)) {
        if (menuBadge->sellCount > 1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            menuBadge->sellCount--;
        } else if (InputStatus.buttonState.pressedButtons & (INPUT_BUTTON_DOWN | INPUT_BUTTON_B)) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
            menuBadge->sellCount = menuBadge->cursorBadge.count;
        }
    }
}

void MenuBadge_StageCloseSellWindow(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (menuBadge->windowCloseTimer > 0) {
        menuBadge->windowCloseTimer--;
        return;
    }
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Window);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
    menuBadge->windowMessage                   = 0;
    menuBadge->slotVisible[menuBadge->dragSrc] = TRUE;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageMain, state, PROCESS_STAGE_INIT);
}

void MenuBadge_StageSortWindow(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;
    TouchCoord       touch;
    s16              button;

    if (!TouchInput_WasTouchPressed()) {
        return;
    }
    TouchInput_GetCoord(&touch);
    button = MenuBadge_GetSortButtonAtPoint(touch.x, touch.y);
    if (button == -1) {
        return;
    }
    menuBadge->arrangeButtonPressed[button] = 1;
    menuBadge->pressTimer                   = 12;
    if (button == 0) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
        if (menuBadge->autoArrange == 1) {
            menuBadge->autoArrangeBy[0] = 1;
            menuBadge->autoArrangeBy[1] = 0;
        }
        MenuBadge_SortList(menuBadge, menuBadge->listMode, 0);
        MenuBadge_RefreshList(menuBadge, menuBadge->listMode, menuBadge->listTop);
        MenuBadge_UpdateCursorBadge(menuBadge);
        MenuBadge_RefreshBadgeInfo(state);
        menuBadge->windowCloseTimer = 20;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageCloseSortWindow, state, PROCESS_STAGE_INIT);
    } else if (button == 1) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
        if (menuBadge->autoArrange == 1) {
            menuBadge->autoArrangeBy[0] = 0;
            menuBadge->autoArrangeBy[1] = 1;
        }
        MenuBadge_SortList(menuBadge, menuBadge->listMode, 1);
        MenuBadge_RefreshList(menuBadge, menuBadge->listMode, menuBadge->listTop);
        MenuBadge_UpdateCursorBadge(menuBadge);
        MenuBadge_RefreshBadgeInfo(state);
        menuBadge->windowCloseTimer = 20;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageCloseSortWindow, state, PROCESS_STAGE_INIT);
    } else if (button == 2) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_WINDOW;
        if (menuBadge->autoArrange == 1) {
            menuBadge->autoArrangeBy[0] = 0;
            menuBadge->autoArrangeBy[1] = 0;
            menuBadge->autoArrange      = 0;
        } else {
            menuBadge->autoArrange = 1;
        }
    } else {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
        if ((menuBadge->autoArrange == 1) && (menuBadge->autoArrangeBy[0] == 0) && (menuBadge->autoArrangeBy[1] == 0)) {
            menuBadge->autoArrange = 0;
        }
        menuBadge->windowCloseTimer = 20;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageCloseSortWindow, state, PROCESS_STAGE_INIT);
    }
}

void MenuBadge_StageCloseSortWindow(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (menuBadge->windowCloseTimer > 0) {
        menuBadge->windowCloseTimer--;
        return;
    }
    EasyTask_DeleteTask(&state->base.taskPool, state->taskId_Window);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
    menuBadge->windowMessage = 0;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageMain, state, PROCESS_STAGE_INIT);
}

void MenuBadge_HelpPrevPage(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (menuBadge->helpPage != 0) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        menuBadge->helpPage--;
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
    }
}

void MenuBadge_HelpNextPage(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (menuBadge->helpPage < 10) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
        menuBadge->helpPage++;
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
    }
}

void MenuBadge_ExitHelp(MenuBadgeState* state) {
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageCloseHelp, state, PROCESS_STAGE_INIT);
}

void MenuBadge_StageHelp(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (TouchInput_WasTouchPressed()) {
        TouchCoord touch;
        TouchInput_GetCoord(&touch);
        s16 button = MenuBadge_GetHelpButtonAtPoint(touch.x, touch.y);
        if (button != -1) {
            if (menuBadge->pressTimer != 0) {
                return;
            }
            menuBadge->helpButtonPressed[button] = 1;
            menuBadge->pressTimer                = 12;
            if (button == 0) {
                MenuBadge_HelpPrevPage(state);
            } else if (button == 1) {
                MenuBadge_HelpNextPage(state);
            } else if (func_02023d1c(4) == 1) {
                MenuBadge_ExitHelp(state);
            }
        }
    }
    if (func_02023d1c(4) == 0) {
        func_02023d00(4);
    }
}

void MenuBadge_StageCloseHelp(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    if (menuBadge->helpButtonPressed[2] == 0) {
        g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG1;
        g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG2;
        g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCur);
        EasyTask_DeleteTask(&state->base.taskPool, state->taskId_HelpCurU);
        menuBadge->helpPage      = 0;
        menuBadge->windowMessage = 0;
        menuBadge->flags |= MENUBADGE_FLAG_REDRAW_INFO;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuBadge_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void MenuBadge_StageFadeOut(MenuBadgeState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void MenuBadge_Init(MenuBadgeState* state) {
    if (state == NULL) {
        const char* sequence = data_ov043_020cb9c8;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(MenuBadgeState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        data_ov043_020cd280 = state;
        MainOvlDisp_SetCbArg(state);
    }
    state->base.spareDataType = DatMgr_AllocateSlot();
    state->base.dataType      = DatMgr_AllocateSlot();
    MenuBadge_RegisterVBlank();
    state->base.prevResMgr = ResourceMgr_ReinitManagers(&state->base.resMgr);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
    TouchInput_Init();
    Mem_InitializeHeap(&state->base.heap, state->base.heapBuffer, sizeof(state->base.heapBuffer));
    EasyTask_InitializePool(&state->base.taskPool, &state->base.heap, 0x100, NULL, NULL);
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    data_02066aec = 0;
    data_02066eec = 0;
    MenuBadge_InitState(state);
    MenuBadge_LoadBackgrounds(&state->menuBadge);
    MenuBadge_CreateTasks(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)MenuBadge_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)MenuBadge_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
}

void MenuBadge_Update(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    MenuBadge_UpdateBackgrounds(menuBadge);
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

    if (state->exitReady != 0) {
        if (menuBadge->exitDest == 1) {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_27_ID, func_ov027_020e860c, NULL, PROCESS_STAGE_INIT);
        } else {
            OverlayTag tag;
            MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_43_ID, ProcessOverlay_MenuTop, NULL, PROCESS_STAGE_INIT);
        }
    }
}

void MenuBadge_Destroy(MenuBadgeState* state) {
    MenuBadgeObject* menuBadge = &state->menuBadge;

    MenuBadge_WriteBackToSave(menuBadge);
    MenuBadge_ReleaseBackgrounds(menuBadge);
    EasyTask_DestroyPool(&state->base.taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->base.spareDataType);
    DatMgr_ClearSlot(state->base.dataType);
    MenuBadge_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
    Mem_ValidateSequences(&gMainHeap);
    Mem_ValidateSequences(&gDebugHeap);
}

void ProcessOverlay_MenuBadge(MenuBadgeState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        MenuBadge_Destroy(state);
    } else {
        OvlProc_MenuBadge.funcs[stage](state);
    }
}

void MenuBadge_InitDisplay(void) {
    Interrupts_Init();
    HBlank_Init();
    GX_Init();
    func_0202b878();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(GX_VRAM_ALL);
    GX_SetBankForTex(GX_VRAM_A);
    GX_SetBankForTexPltt(GX_VRAM_G);
    GX_SetBankForBg(GX_VRAM_D);
    GX_SetBankForObj(GX_VRAM_B);
    GX_SetBankForBgExtPltt(GX_VRAM_NONE);
    GX_SetBankForObjExtPltt(GX_VRAM_NONE);
    GX_SetBankForSubBg(GX_VRAM_C);
    GX_SetBankForSubObj(GX_VRAM_I);
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
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension &= 0xFF;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);
    Display_InitMainBG2(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 1, 3, 1, 0x10C);
    Display_InitMainBG3(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 2, 3, 1, 0x20C);

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].priority = 2;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].objBmpMode  = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode  = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].objBmpMode   = GX_OBJBMPMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= 0xFF;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG3 | LAYER_OBJ);
    data_0206aa78                                       = 0x300010;
    data_0206aa7c                                       = 0x400040;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 16;

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
    g_DisplaySettings.controls[DISPLAY_SUB].brightness = 16;
    Display_Commit();
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

void MenuBadge_VBlank(void) {
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

void MenuBadge_RegisterVBlank(void) {
    MenuBadge_InitDisplay();
    Interrupts_RegisterVBlankCallback(MenuBadge_VBlank, 1);
}

void MenuBadge_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}