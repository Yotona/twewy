#include "Interface/Menu/MenuEquip.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/HBlank.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
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
    /* 0x00000 */ ResourceManager  unk_00000;
    /* 0x11580 */ ResourceManager* unk_11580;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ s32              unk_11588;
    /* 0x1158C */ Heap             heap;
    /* 0x11598 */ u8               heapBuffer[0x10000];
    /* 0x21598 */ TaskPool         taskPool;
    /* 0x21618 */ s32              taskId_Fade;
    /* 0x2161C */ s32              taskId_Icon;
    /* 0x21620 */ s32              taskId_Item[26];
    /* 0x21688 */ s32              taskId_ItemNum[26];
    /* 0x216F0 */ s32              taskId_ItemAbl[26];
    /* 0x21758 */ s32              taskId_ItemCur;
    /* 0x2175C */ s32              taskId_ItemMask[16];
    /* 0x2179C */ s32              taskId_Mov;
    /* 0x217A0 */ s32              taskId_Shadow;
    /* 0x217A4 */ s32              taskId_HelpCur;
    /* 0x217A8 */ s32              taskId_NameU;
    /* 0x217AC */ s32              taskId_Sbar;
    /* 0x217B0 */ s32              taskId_Sbar2;
    /* 0x217B4 */ s32              taskId_Tab;
    /* 0x217B8 */ s32              taskId_Partner;
    /* 0x217BC */ s32              taskId_PartnerHide;
    /* 0x217C0 */ s32              taskId_NumNek;
    /* 0x217C4 */ s32              taskId_NumPtr;
    /* 0x217C8 */ s32              taskId_NameD;
    /* 0x217CC */ s32              taskId_TextScr;
    /* 0x217D0 */ s32              taskId_Window;
    /* 0x217D4 */ s32              taskId_TextItem;
    /* 0x217D8 */ s32              taskId_Pointer;
    /* 0x217DC */ s32              taskId_ItemU;
    /* 0x217E0 */ s32              taskId_TextScrU;
    /* 0x217E4 */ s32              taskId_BrdLogoU;
    /* 0x217E8 */ s32              taskId_NumItemIdU;
    /* 0x217EC */ s32              taskId_HelpCurU;
    /* 0x217F0 */ s32              taskId_FoodGauge[2];
    /* 0x217F8 */ s32              unk_217F8;
    /* 0x217FC */ u16              unk_217FC;
    /* 0x217FE */ s16              unk_217FE;
    /* 0x21800 */ MenuEquipObject  menuEquip;
} MenuEquipState; // Size: 0x2F340

static MenuEquipState* data_ov043_020cd284;

void        GX_LoadBgPltt(void* src, u32 offset, u32 size);
void        GX_LoadObjPltt(void* src, u32 offset, u32 size);
void        GXs_LoadBgPltt(void* src, u32 offset, u32 size);
void        GXs_LoadObjPltt(void* src, u32 offset, u32 size);
BOOL        func_02001b44(s32, s32, s32*, s32);
void        func_02023d00(s32);
s32         func_02023d1c(s32);
void        func_0202b878(void);
extern void func_ov043_02084040(void* state);

void MenuEquip_StageFadeIn(MenuEquipState* state);
void MenuEquip_StageOpenHelp(MenuEquipState* state);
void MenuEquip_StageMain(MenuEquipState* state);
void MenuEquip_StageErrorMessage(MenuEquipState* state);
void MenuEquip_StageHelp(MenuEquipState* state);
void MenuEquip_StageCloseHelp(MenuEquipState* state);
void MenuEquip_StageFadeOut(MenuEquipState* state);
void MenuEquip_Init(MenuEquipState* state);
void MenuEquip_Update(MenuEquipState* state);
void MenuEquip_Destroy(MenuEquipState* state);
void MenuEquip_RegisterVBlank(void);
void MenuEquip_DeregisterVBlank(void);

static const char* data_ov043_020cc134 = "Seq_MenuEquip()";

static const OverlayProcess data_ov043_020c96e4 = {
    .init = MenuEquip_Init,
    .main = MenuEquip_Update,
    .exit = MenuEquip_Destroy,
};

// Position of each cursor slot: 0-4 Neku's equipment, 5-9 the partner's, 10-25 the item list.
const s16 data_ov043_020c96f0[26][2] = {
    {0x51, 0x29},
    {0x6E, 0x29},
    {0x51, 0x46},
    {0x6E, 0x46},
    {0x12, 0x5E},
    {0xD1, 0x29},
    {0xEE, 0x29},
    {0xD1, 0x46},
    {0xEE, 0x46},
    {0x92, 0x5E},
    {0x13, 0x8F},
    {0x31, 0x8F},
    {0x4F, 0x8F},
    {0x6D, 0x8F},
    {0x8B, 0x8F},
    {0xA9, 0x8F},
    {0xC7, 0x8F},
    {0xE5, 0x8F},
    {0x13, 0xAD},
    {0x31, 0xAD},
    {0x4F, 0xAD},
    {0x6D, 0xAD},
    {0x8B, 0xAD},
    {0xA9, 0xAD},
    {0xC7, 0xAD},
    {0xE5, 0xAD},
};

// Listed out of index order on purpose: mwcc derives the .data order of these literals from the
// initializer listing, and this is the listing that reproduces the ROM layout.
const BinIdentifier data_ov043_020c9758[15] = {
    [2] = {0x2B, "Apl_Tak/Grp_MenuEquip_OBD00.bin"},
      [1] = {0x2B,        "Apl_Tak/Grp_Menu_BGD.bin"},
    [3] = {0x2B, "Apl_Tak/Grp_MenuEquip_BGU00.bin"},
      [5] = {0x2B, "Apl_Tak/Grp_MenuEquip_OBU00.bin"},
    [14] = {0x2B,        "Apl_Tak/TreasureData.bin"},
      [0] = {0x2B, "Apl_Tak/Grp_MenuEquip_BGD00.bin"},
    [13] = {0x2B,            "Apl_Tak/FoodData.bin"},
      [9] = {0x2B,       "Apl_Tak/Grp_BrandLogo.bin"},
    [8] = {0x2B,        "Apl_Tak/Grp_MenuIcon.bin"},
      [7] = {0x2B,       "Apl_Tak/Grp_DummyItem.bin"},
    [10] = {0x2B,    "Apl_Tak/Grp_Menu_fontSCR.bin"},
      [11] = {0x2B,    "Apl_Tak/Grp_Menu_fontSCR.bin"},
    [12] = {0x2B,            "Apl_Tak/ItemData.bin"},
      [6] = {0x2B,            "Apl_Tak/Grp_Item.bin"},
    [4] = {0x2B,        "Apl_Tak/Grp_Menu_BGU.bin"},
};

void MenuEquip_InitState(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    state->unk_217F8      = 0;
    state->unk_217FE      = 0;
    menuEquip->dirtyFlags = 0;
    MenuEquip_LoadFromSave(menuEquip);
    menuEquip->touchPosX       = 0;
    menuEquip->touchPosY       = 0;
    menuEquip->listScroll      = 0;
    menuEquip->prevListScroll  = 0;
    menuEquip->iconPressed[0]  = 0;
    menuEquip->iconPressed[1]  = 0;
    menuEquip->pressTimer      = 0;
    menuEquip->cursorSlot      = 0;
    menuEquip->cursorListIndex = 0xFFFF;
    menuEquip->itemUBuffer     = 0;
    menuEquip->errorCode       = 0;
    menuEquip->unk_DA06        = 0;
    for (u16 i = 0; i < 3; i++) {
        menuEquip->helpPressed[i] = 0;
    }
    menuEquip->helpPage = 0;
    menuEquip->helpOpen = 0;
    for (u16 i = 0; i < 8; i++) {
        menuEquip->tabActive[i] = 0;
    }
    state->unk_217FC = 0;
}

void MenuEquip_CreateTasks(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    EasyTask_CreateTask(&state->taskPool, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, -0x10, 0x1000);
    state->taskId_Pointer = MenuEquip_pointer_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_Icon    = MenuEquip_icon_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_Tab     = MenuEquip_tab_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_NameD   = MenuEquip_nameD_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_NumNek  = MenuEquip_numNek_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    if ((func_02023010(686) != 0) && (menuEquip->activePartner != PARTNER_NONE)) {
        state->taskId_Partner = MenuEquip_partner_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    }
    if (menuEquip->activePartner != PARTNER_NONE) {
        state->taskId_NumPtr = MenuEquip_numPtr_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    } else {
        state->taskId_PartnerHide = MenuEquip_partnerHide_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    }
    for (u16 i = 0; i < 26; i++) {
        state->taskId_Item[i]    = MenuEquip_item_CreateTask(&state->taskPool, state->unk_11588, i, menuEquip);
        state->taskId_ItemNum[i] = MenuEquip_itemNum_CreateTask(&state->taskPool, state->unk_11588, i, menuEquip);
        state->taskId_ItemAbl[i] = MenuEquip_itemAbl_CreateTask(&state->taskPool, state->unk_11588, i, menuEquip);
    }
    state->taskId_ItemCur = MenuEquip_itemCur_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    for (u16 j = 0; j < 16; j++) {
        state->taskId_ItemMask[j] = MenuEquip_itemMask_CreateTask(&state->taskPool, state->unk_11588, j + 10, menuEquip);
    }

    state->taskId_FoodGauge[0] = MenuEquip_foodGauge_CreateTask(&state->taskPool, state->unk_11588, 0, menuEquip);
    state->taskId_FoodGauge[1] = MenuEquip_foodGauge_CreateTask(&state->taskPool, state->unk_11588, 1, menuEquip);
    state->taskId_Sbar         = MenuEquip_sbar_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_Sbar2        = MenuEquip_sbar2_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_Window       = MenuEquip_window_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_TextScr      = MenuEquip_textScr_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_TextItem     = MenuEquip_textItem_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_NameU        = MenuEquip_nameU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_TextScrU     = MenuEquip_textScrU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_BrdLogoU     = MenuEquip_brdLogoU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_ItemU        = MenuEquip_itemU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_NumItemIdU   = MenuEquip_numItemIdU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
}

void MenuEquip_DestroySlotTasks(MenuEquipObject* menuEquip, s16 index) {
    MenuEquipState* state = data_ov043_020cd284;

    Sprite_Release(MenuEquip_item_GetTaskData(&state->taskPool, state->taskId_Item[index]));
    if (index != 30) {
        EasyTask_DeleteTask(&state->taskPool, state->taskId_Item[index]);
        EasyTask_DeleteTask(&state->taskPool, state->taskId_ItemNum[index]);
        EasyTask_DeleteTask(&state->taskPool, state->taskId_ItemAbl[index]);
    }
}

void MenuEquip_CreateSlotTasks(MenuEquipObject* menuEquip, s16 index) {
    MenuEquipState* state = data_ov043_020cd284;

    if (index != 30) {
        state->taskId_Item[index]    = MenuEquip_item_CreateTask(&state->taskPool, state->unk_11588, index, menuEquip);
        state->taskId_ItemNum[index] = MenuEquip_itemNum_CreateTask(&state->taskPool, state->unk_11588, index, menuEquip);
        state->taskId_ItemAbl[index] = MenuEquip_itemAbl_CreateTask(&state->taskPool, state->unk_11588, index, menuEquip);
    }
}

void MenuEquip_RefreshItemText(MenuEquipObject* menuEquip) {
    MenuEquipState* state = data_ov043_020cd284;

    EasyTask_DeleteTask(&state->taskPool, state->taskId_TextItem);
    state->taskId_TextItem = MenuEquip_textItem_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
}

void MenuEquip_RefreshSlot(MenuEquipObject* menuEquip, s32 index) {
    s16 slot = index;

    MenuEquip_DestroySlotTasks(menuEquip, slot);
    MenuEquip_CreateSlotTasks(menuEquip, slot);
}

void MenuEquip_RefreshItemList(MenuEquipObject* menuEquip, u16 listTop, u8 tab) {
    for (s16 i = 10; i < 26; i++) {
        MenuEquip_DestroySlotTasks(menuEquip, i);
    }
    for (s16 i = 0; i < 16; i++) {
        menuEquip->slots[10 + i] = menuEquip->tabItems[tab][listTop + i];
    }
    for (s16 i = 10; i < 26; i++) {
        MenuEquip_CreateSlotTasks(menuEquip, i);
    }
}

void MenuEquip_SwitchPartnerEquipment(MenuEquipObject* menuEquip, u16 index) {
    MenuEquipState* state = data_ov043_020cd284;

    for (s16 i = 0; i < 5; i++) {
        MenuEquip_DestroySlotTasks(menuEquip, i + 5);
    }
    EasyTask_DeleteTask(&state->taskPool, state->taskId_FoodGauge[1]);
    for (s16 i = 0; i < 5; i++) {
        menuEquip->slots[5 + i] = &menuEquip->equipped[index + 1][i];
    }
    for (s16 i = 0; i < 5; i++) {
        MenuEquip_CreateSlotTasks(menuEquip, i + 5);
    }
    state->taskId_FoodGauge[1] = MenuEquip_foodGauge_CreateTask(&state->taskPool, state->unk_11588, 1, menuEquip);
}

void MenuEquip_RefreshFoodGauge(MenuEquipObject* menuEquip, s16 index) {
    MenuEquipState* state = data_ov043_020cd284;

    EasyTask_DeleteTask(&state->taskPool, state->taskId_FoodGauge[index]);
    state->taskId_FoodGauge[index] = MenuEquip_foodGauge_CreateTask(&state->taskPool, state->unk_11588, index, menuEquip);
}

s32 MenuEquip_ReturnItemToInventory(MenuEquipObject* menuEquip, MenuEquipItemEntry* item) {
    if (MenuEquip_StackIntoInventory(menuEquip, item->itemId) == 1) {
        return 1;
    }
    if (MenuEquip_InsertIntoInventory(menuEquip, item) == 1) {
        return 1;
    }
    return 0;
}

s16 MenuEquip_SwapSlots(MenuEquipObject* menuEquip, s32 src, s32 dst) {
    MenuEquip_SwapItemEntries(menuEquip->slots[src], menuEquip->slots[dst]);
    MenuEquip_RefreshSlot(menuEquip, src);
    MenuEquip_RefreshSlot(menuEquip, dst);
    return 0;
}

s16 MenuEquip_UnequipSlot(MenuEquipObject* menuEquip, s32 slot, s32 dst) {
    MenuEquip_ReturnItemToInventory(menuEquip, menuEquip->slots[slot]);
    *menuEquip->slots[slot] = data_ov043_020cc2d8;
    MenuEquip_RefreshSlot(menuEquip, slot);
    MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
    return 0;
}

s16 MenuEquip_EquipThread(MenuEquipObject* menuEquip, u32 src, s32 dst, u16 activePartner) {
    if (menuEquip->slots[dst]->itemId != 0xFFFF) {
        MenuEquipItemEntry* item = menuEquip->slots[src];
        if (item->braveryReq > (u16)menuEquip->stats[activePartner].bravery) {
            return 4;
        }
        if (item->category != ITEM_CATEGORY_THREAD) {
            return 6;
        }
        if (MenuEquip_CanEquipSubCategory(menuEquip, activePartner, item->subCategory, menuEquip->slots[dst]->subCategory) ==
            0)
        {
            return 5;
        }
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);

        MenuEquipItemEntry replaced = *menuEquip->slots[dst];
        *menuEquip->slots[dst]      = *menuEquip->slots[src];

        menuEquip->slots[dst]->count = 1;
        menuEquip->slots[src]->count--;

        MenuEquip_ReturnItemToInventory(menuEquip, &replaced);
        MenuEquip_RefreshSlot(menuEquip, dst);
        MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
    } else {
        MenuEquipItemEntry* item = menuEquip->slots[src];

        if (item->braveryReq > (u16)menuEquip->stats[activePartner].bravery) {
            return 4;
        }
        if (item->category != ITEM_CATEGORY_THREAD) {
            return 6;
        }
        if (MenuEquip_CanEquipSubCategory(menuEquip, activePartner, item->subCategory, menuEquip->slots[dst]->subCategory) ==
            0)
        {
            return 5;
        }
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        *menuEquip->slots[dst]       = *menuEquip->slots[src];
        menuEquip->slots[dst]->count = 1;
        menuEquip->slots[src]->count--;
        MenuEquip_RefreshSlot(menuEquip, dst);
        MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
    }
    return 0;
}

s16 MenuEquip_EquipFood(MenuEquipObject* menuEquip, u32 src, s32 dst) {
    MenuEquipItemEntry* item = menuEquip->slots[src];

    if (item->category != ITEM_CATEGORY_FOOD) {
        return 1;
    }
    if (menuEquip->slots[dst]->itemId != 0xFFFF) {
        return 2;
    }
    if (dst == 4) {
        if (item->capacityCost > menuEquip->stats[0].foodCapacityLeft) {
            return 3;
        }
    } else if (item->capacityCost > menuEquip->stats[menuEquip->activePartner + 1].foodCapacityLeft) {
        return 3;
    }
    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
    *menuEquip->slots[dst]       = *menuEquip->slots[src];
    menuEquip->slots[dst]->count = 1;
    menuEquip->slots[src]->count--;
    MenuEquip_RefreshSlot(menuEquip, dst);
    MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
    if (dst == 4) {
        MenuEquip_PlayTasteVoice(0, menuEquip->cursorItem.taste[0]);
        MenuEquip_ApplySyncGain(menuEquip, 0);
        MenuEquip_RefreshFoodGauge(menuEquip, 0);
    } else {
        u8 character = menuEquip->activePartner + 1;
        MenuEquip_PlayTasteVoice(character, menuEquip->cursorItem.taste[character]);
        MenuEquip_ApplySyncGain(menuEquip, character);
        MenuEquip_RefreshFoodGauge(menuEquip, 1);
    }
    menuEquip->dirtyFlags |= 0x100;
    return 0;
}

s16 MenuEquip_MoveItem(MenuEquipObject* menuEquip, u16 src, u16 dst) {
    s16 result = 0;

    if (src <= 3) {
        if (dst <= 3) {
            result = MenuEquip_SwapSlots(menuEquip, src, dst);

            menuEquip->cursorSlot      = dst;
            menuEquip->cursorListIndex = 0xFFFF;
        } else if (dst == 30) {
            result = MenuEquip_UnequipSlot(menuEquip, src, dst);
        }
    } else if ((src >= 5) && (src <= 8)) {
        if ((dst >= 5) && (dst <= 8)) {
            result = MenuEquip_SwapSlots(menuEquip, src, dst);

            menuEquip->cursorSlot      = dst;
            menuEquip->cursorListIndex = 0xFFFF;
        } else if (dst == 30) {
            result = MenuEquip_UnequipSlot(menuEquip, src, dst);
        }
    } else if (dst <= 3) {
        result = MenuEquip_EquipThread(menuEquip, src, dst, 0);
    } else if ((dst >= 5) && (dst <= 8)) {
        result = MenuEquip_EquipThread(menuEquip, src, dst, menuEquip->activePartner + 1);
    } else if ((dst != 4) && (dst != 9)) {

    } else {
        result = MenuEquip_EquipFood(menuEquip, src, dst);
    }
    return result;
}

void MenuEquip_DestroyDragTasks(MenuEquipState* state) {
    Sprite_Release(MenuEquip_item_GetTaskData(&state->taskPool, state->taskId_Mov));
    Sprite_Release(MenuEquip_item_GetTaskData(&state->taskPool, state->taskId_Shadow));
    EasyTask_DeleteTask(&state->taskPool, state->taskId_Mov);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_Shadow);
}

void MenuEquip_RefreshItemInfo(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    EasyTask_DeleteTask(&state->taskPool, state->taskId_ItemU);
    EasyTask_DeleteTask(&state->taskPool, state->taskId_BrdLogoU);
    state->taskId_ItemU    = MenuEquip_itemU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_BrdLogoU = MenuEquip_brdLogoU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    menuEquip->dirtyFlags |= 0x20;
}

void MenuEquip_UpdateCursorItem(MenuEquipObject* menuEquip) {
    if (menuEquip->cursorListIndex == 0xFFFF) {
        menuEquip->cursorItem = *menuEquip->slots[menuEquip->cursorSlot];
    } else {
        menuEquip->cursorItem = *menuEquip->tabItems[menuEquip->currentTab][menuEquip->cursorListIndex];
    }
}

#ifdef REGION_USA
void MenuEquip_RefreshCursorInfo(void) {
    MenuEquipState* state = data_ov043_020cd284;

    MenuEquip_UpdateCursorItem(&state->menuEquip);
    MenuEquip_RefreshItemInfo(state);
}
#endif

s32 MenuEquip_IsAnyTabActive(MenuEquipObject* menuEquip) {
    for (u16 i = 0; i < 8; i++) {
        if (menuEquip->tabActive[i] == 1) {
            return 1;
        }
    }
    return 0;
}

void MenuEquip_ClearActiveTabs(MenuEquipObject* menuEquip) {
    for (u16 i = 0; i < 8; i++) {
        menuEquip->tabActive[i] = 0;
    }
}

void MenuEquip_UpdateButtonInput(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    if (!(InputStatus.buttonState.currButtons & INPUT_ABXY) && !(InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->unk_217FC = 0;
    }
    if (InputStatus.buttonState.pressedButtons & (INPUT_BUTTON_L | INPUT_BUTTON_R)) {
        if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_L) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if (menuEquip->currentTab == 0) {
                menuEquip->currentTab = 8;
            } else {
                menuEquip->currentTab -= 1;
            }
            MenuEquip_ClearActiveTabs(menuEquip);
            if (menuEquip->currentTab != 8) {
                menuEquip->tabActive[menuEquip->currentTab] = 1;
            }
            menuEquip->listScroll      = 0;
            menuEquip->cursorListIndex = 0;
            menuEquip->cursorSlot      = 10;
            MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
            MenuEquip_UpdateCursorItem(menuEquip);
            MenuEquip_RefreshItemInfo(state);
            MenuEquip_RefreshItemText(menuEquip);
        } else if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_R) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if (menuEquip->currentTab == 8) {
                menuEquip->currentTab = 0;
            } else {
                menuEquip->currentTab += 1;
            }
            MenuEquip_ClearActiveTabs(menuEquip);
            if (menuEquip->currentTab != 8) {
                menuEquip->tabActive[menuEquip->currentTab] = 1;
            }
            menuEquip->listScroll      = 0;
            menuEquip->cursorListIndex = 0;
            menuEquip->cursorSlot      = 10;
            MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
            MenuEquip_UpdateCursorItem(menuEquip);
            MenuEquip_RefreshItemInfo(state);
            MenuEquip_RefreshItemText(menuEquip);
        }
        goto block_82;
    }
    if (state->unk_217FC != 0) {
        state->unk_217FC--;
        return;
    }
    if ((InputStatus.buttonState.pressedButtons & INPUT_ABXY) || (InputStatus.buttonState.pressedButtons & INPUT_DPAD)) {
        state->unk_217FC = 20;
        if (menuEquip->cursorListIndex == 0xFFFF) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            menuEquip->listScroll      = 0;
            menuEquip->cursorSlot      = 10;
            menuEquip->cursorListIndex = 0;
            MenuEquip_UpdateCursorItem(menuEquip);
            MenuEquip_RefreshItemInfo(state);
            MenuEquip_RefreshItemText(menuEquip);
            return;
        }
        goto block_27;
    }
    if ((InputStatus.buttonState.currButtons & INPUT_ABXY) || (InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        state->unk_217FC = 1;
    }
block_27:
    if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_UP) || (InputStatus.buttonState.currButtons & INPUT_BUTTON_X)) {
        if (menuEquip->cursorListIndex != 0xFFFF) {
            if ((menuEquip->cursorListIndex >= menuEquip->listScroll) &&
                (menuEquip->cursorListIndex < (menuEquip->listScroll + 16)))
            {
                if (menuEquip->cursorListIndex >= 8) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    if ((menuEquip->cursorSlot >= 10) && (menuEquip->cursorSlot < 18)) {
                        menuEquip->listScroll -= 8;
                        menuEquip->cursorListIndex -= 8;
                    } else {
                        menuEquip->cursorSlot -= 8;
                        menuEquip->cursorListIndex -= 8;
                    }
                }
            } else {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                menuEquip->cursorSlot      = 10;
                menuEquip->cursorListIndex = menuEquip->listScroll;
            }
        }
        MenuEquip_UpdateCursorItem(menuEquip);
        MenuEquip_RefreshItemInfo(state);
        MenuEquip_RefreshItemText(menuEquip);
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_DOWN) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_B))
    {
        if (menuEquip->cursorListIndex == 0xFFFF) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            menuEquip->listScroll      = 0;
            menuEquip->cursorSlot      = 10;
            menuEquip->cursorListIndex = 0;
        } else {
            if ((menuEquip->cursorListIndex >= menuEquip->listScroll) &&
                (menuEquip->cursorListIndex < (menuEquip->listScroll + 16)))
            {
                if (menuEquip->cursorListIndex < (menuEquip->pagedCount[menuEquip->currentTab] - 8)) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    if ((menuEquip->cursorSlot >= 18) && (menuEquip->cursorSlot < 26)) {
                        menuEquip->listScroll += 8;
                        menuEquip->cursorListIndex += 8;
                    } else {
                        menuEquip->cursorSlot += 8;
                        menuEquip->cursorListIndex += 8;
                    }
                }
            } else {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                menuEquip->cursorSlot      = 10;
                menuEquip->cursorListIndex = menuEquip->listScroll;
            }
        }
        MenuEquip_UpdateCursorItem(menuEquip);
        MenuEquip_RefreshItemInfo(state);
        MenuEquip_RefreshItemText(menuEquip);
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_LEFT) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_Y))
    {
        if (menuEquip->cursorListIndex == 0xFFFF) {
            if (menuEquip->cursorSlot != 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                menuEquip->cursorSlot--;
            }
            menuEquip->cursorListIndex = 0xFFFF;
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if ((menuEquip->cursorListIndex >= menuEquip->listScroll) &&
                (menuEquip->cursorListIndex < (menuEquip->listScroll + 16)))
            {
                if (menuEquip->cursorListIndex == 0) {
                    menuEquip->listScroll      = menuEquip->maxScrollRow[menuEquip->currentTab] * 8;
                    menuEquip->cursorSlot      = 25;
                    menuEquip->cursorListIndex = menuEquip->pagedCount[menuEquip->currentTab] - 1;
                } else {
                    if (menuEquip->cursorSlot == 10) {
                        menuEquip->listScroll -= 8;
                        menuEquip->cursorSlot = 17;
                        menuEquip->cursorListIndex--;
                    } else {
                        menuEquip->cursorSlot--;
                        menuEquip->cursorListIndex--;
                    }
                }
            } else {
                menuEquip->cursorSlot      = 10;
                menuEquip->cursorListIndex = menuEquip->listScroll;
            }
        }
        MenuEquip_UpdateCursorItem(menuEquip);
        MenuEquip_RefreshItemInfo(state);
        MenuEquip_RefreshItemText(menuEquip);
    } else if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_RIGHT) ||
               (InputStatus.buttonState.currButtons & INPUT_BUTTON_A))
    {
        if (menuEquip->cursorListIndex == 0xFFFF) {
            if (menuEquip->cursorSlot < 9) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                menuEquip->cursorSlot++;
            }
            menuEquip->cursorListIndex = 0xFFFF;
        } else {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if ((menuEquip->cursorListIndex >= menuEquip->listScroll) &&
                (menuEquip->cursorListIndex < (menuEquip->listScroll + 16)))
            {
                if (menuEquip->cursorListIndex == (menuEquip->pagedCount[menuEquip->currentTab] - 1)) {
                    menuEquip->listScroll      = 0;
                    menuEquip->cursorSlot      = 10;
                    menuEquip->cursorListIndex = 0;
                } else {
                    if (menuEquip->cursorSlot == 25) {
                        menuEquip->listScroll += 8;
                        menuEquip->cursorSlot = 18;
                        menuEquip->cursorListIndex++;
                    } else {
                        menuEquip->cursorSlot++;
                        menuEquip->cursorListIndex++;
                    }
                }
            } else {
                menuEquip->cursorSlot      = 10;
                menuEquip->cursorListIndex = menuEquip->listScroll;
            }
        }
        MenuEquip_UpdateCursorItem(menuEquip);
        MenuEquip_RefreshItemInfo(state);
        MenuEquip_RefreshItemText(menuEquip);
    }
block_82:
    if ((InputStatus.buttonState.currButtons & INPUT_ABXY) || (InputStatus.buttonState.currButtons & INPUT_DPAD)) {
        if (MenuEquip_IsCdItem(menuEquip->cursorItem.itemId) == 1) {
            MenuEquip_PlayCdTrack(menuEquip->cursorItem.itemId);
        }
    }
}

void MenuEquip_StageFadeIn(MenuEquipState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        if (func_02023d1c(3) == 0) {
            state->unk_217FE = 30;
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageOpenHelp, state, PROCESS_STAGE_INIT);
        } else {
            DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageMain, state, PROCESS_STAGE_INIT);
        }
    }
}

void MenuEquip_StageOpenHelp(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    if (state->unk_217FE > 0) {
        state->unk_217FE--;
        return;
    }

    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
    state->taskId_HelpCur  = MenuEquip_helpCur_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    state->taskId_HelpCurU = MenuEquip_helpCurU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
    menuEquip->helpOpen    = 1;
    menuEquip->dirtyFlags |= 0x20;
    DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageHelp, state, PROCESS_STAGE_INIT);
}

void MenuEquip_StageMain(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;
    TouchCoord       touch;

    if (menuEquip->dirtyFlags & 1) {
        TouchInput_GetCoord(&touch);
        menuEquip->touchPosX   = touch.x;
        menuEquip->touchPosY   = touch.y;
        menuEquip->dragDstSlot = MenuEquip_GetDropTargetAtPoint(menuEquip->touchPosX, menuEquip->touchPosY);

        if (MenuEquip_CanDropOnSlot(menuEquip, menuEquip->dragSrcSlot, menuEquip->dragDstSlot) == 1) {
            menuEquip->dirtyFlags |= 2;
            if (TouchInput_WasTouchReleased()) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CLICK02);
                MenuEquip_DestroyDragTasks(state);
                s16 result = MenuEquip_MoveItem(menuEquip, menuEquip->dragSrcSlot, menuEquip->dragDstSlot);
                if (result == 0) {
                    MenuEquip_UpdateStatBonuses(menuEquip);
                    menuEquip->dirtyFlags |= 0x180;
                } else {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_KINSHI);
                    menuEquip->errorCode = result;
                    menuEquip->dirtyFlags |= 0x200;
                    menuEquip->dirtyFlags |= 0x40;
                    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG1 | LAYER_BG2;
                    DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageErrorMessage, state, PROCESS_STAGE_INIT);
                }
                menuEquip->dirtyFlags &= ~1;
                menuEquip->dirtyFlags &= ~2;
            }
        } else {
            menuEquip->dirtyFlags &= ~2;
            if (TouchInput_WasTouchReleased()) {
                MenuEquip_DestroyDragTasks(state);
                menuEquip->dirtyFlags &= ~1;
            }
        }
    } else if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&touch);
        menuEquip->dragSrcSlot = MenuEquip_GetSlotAtPoint(touch.x, touch.y);
        if (MenuEquip_CanDragSlot(menuEquip, menuEquip->dragSrcSlot) == 1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CLICK01);
            menuEquip->touchPosX = touch.x;
            menuEquip->touchPosY = touch.y;
            menuEquip->dirtyFlags |= 1;
            menuEquip->cursorSlot = menuEquip->dragSrcSlot;
            menuEquip->cursorItem = *menuEquip->slots[menuEquip->cursorSlot];
            if (menuEquip->dragSrcSlot < 10) {
                menuEquip->cursorListIndex = 0xFFFF;
            } else {
                menuEquip->cursorListIndex = menuEquip->listScroll + (menuEquip->dragSrcSlot - 10);
            }

            if ((menuEquip->cursorSlot == 4) || (menuEquip->cursorSlot == 9) ||
                (menuEquip->slots[menuEquip->cursorSlot]->count == 0))
            {
                menuEquip->dirtyFlags &= ~1;
            } else {
                state->taskId_Mov    = MenuEquip_mov_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
                state->taskId_Shadow = MenuEquip_shadow_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
            }
            MenuEquip_RefreshItemInfo(state);
            MenuEquip_RefreshItemText(menuEquip);
            if (MenuEquip_IsCdItem(menuEquip->slots[menuEquip->cursorSlot]->itemId) != 1) {
                return;
            }
            MenuEquip_PlayCdTrack(menuEquip->slots[menuEquip->cursorSlot]->itemId);
            return;
        }

        s16 tab = MenuEquip_GetTabAtPoint(touch.x, touch.y);
        if (tab != -1) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            if (menuEquip->tabActive[tab] == 1) {
                menuEquip->tabActive[tab] = 0;
                menuEquip->currentTab     = 8;
            } else if (MenuEquip_IsAnyTabActive(menuEquip) == 1) {
                MenuEquip_ClearActiveTabs(menuEquip);
                menuEquip->tabActive[tab] = 1;
                menuEquip->currentTab     = tab;
            } else {
                menuEquip->tabActive[tab] = 1;
                menuEquip->currentTab     = tab;
            }
            menuEquip->listScroll      = 0;
            menuEquip->cursorListIndex = 0;
            menuEquip->cursorSlot      = 10;
            MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
            MenuEquip_UpdateCursorItem(menuEquip);
            MenuEquip_RefreshItemInfo(state);
            MenuEquip_RefreshItemText(menuEquip);
            return;
        }

        s16 button = MenuEquip_GetButtonAtPoint(touch.x, touch.y);
        if (button != -1) {
            menuEquip->iconPressed[button] = 1;
            menuEquip->pressTimer          = 12;
            if (button == 0) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= LAYER_BG1 | LAYER_BG2;
                g_DisplaySettings.controls[DISPLAY_MAIN].layers |= LAYER_BG2;
                state->taskId_HelpCur  = MenuEquip_helpCur_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
                state->taskId_HelpCurU = MenuEquip_helpCurU_CreateTask(&state->taskPool, state->unk_11588, menuEquip);
                menuEquip->helpOpen    = 1;
                menuEquip->dirtyFlags |= 0x20;
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageHelp, state, PROCESS_STAGE_INIT);
            } else {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                SndMgr_StartPlayingSE(SEIDX_MENU_MEXIT);
                menuEquip->dirtyFlags |= 0x1000;
                menuEquip->unk_DA06 = 0;
                DebugOvlDisp_Pop();
            }
            return;
        }
    } else if (TouchInput_IsTouchActive() == FALSE) {
        MenuEquip_UpdateButtonInput(state);
    }

    if (menuEquip->prevListScroll != menuEquip->listScroll) {
        MenuEquip_RefreshItemList(menuEquip, menuEquip->listScroll, menuEquip->currentTab);
        menuEquip->prevListScroll = menuEquip->listScroll;
    }
}

void MenuEquip_StageErrorMessage(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    if (TouchInput_WasTouchPressed()) {
        menuEquip->dirtyFlags &= ~0x200;
        g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG1;
        g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void MenuEquip_StageHelp(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    if (TouchInput_WasTouchPressed()) {
        TouchCoord touch;
        TouchInput_GetCoord(&touch);
        s16 button = MenuEquip_GetHelpButtonAtPoint(touch.x, touch.y);
        if (button != -1) {
            if (menuEquip->pressTimer != 0) {
                return;
            }
            menuEquip->helpPressed[button] = 1;
            menuEquip->pressTimer          = 12;
            if (button == 0) {
                if (menuEquip->helpPage != 0) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    menuEquip->helpPage--;
                    menuEquip->dirtyFlags |= 0x20;
                }
            } else if (button == 1) {
                if (menuEquip->helpPage < 8) {
                    SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
                    menuEquip->helpPage++;
                    menuEquip->dirtyFlags |= 0x20;
                }
            } else if (func_02023d1c(3) == 1) {
                SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_RETURN);
                DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageCloseHelp, state, PROCESS_STAGE_INIT);
            }
        }
    }
    if (func_02023d1c(3) == 0) {
        func_02023d00(3);
    }
}

void MenuEquip_StageCloseHelp(MenuEquipState* state) {
    MenuEquipObject* menuEquip = &state->menuEquip;

    if (menuEquip->helpPressed[2] == 0) {
        g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG1;
        g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~LAYER_BG2;
        g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~LAYER_BG2;
        EasyTask_DeleteTask(&state->taskPool, state->taskId_HelpCur);
        EasyTask_DeleteTask(&state->taskPool, state->taskId_HelpCurU);
        menuEquip->helpPage = 0;
        menuEquip->helpOpen = 0;
        menuEquip->dirtyFlags |= 0x20;
        DebugOvlDisp_ReplaceTop((OverlayCB)MenuEquip_StageMain, state, PROCESS_STAGE_INIT);
    }
}

void MenuEquip_StageFadeOut(MenuEquipState* state) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
    if (EasyFade_IsFading() == FALSE) {
        DebugOvlDisp_Pop();
    }
}

void MenuEquip_Init(MenuEquipState* state) {
    if (state == NULL) {
        const char* sequence = data_ov043_020cc134;
        state                = Mem_AllocHeapTail(&gDebugHeap, sizeof(MenuEquipState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        data_ov043_020cd284 = state;
        MainOvlDisp_SetCbArg(state);
    }
    state->unk_11584 = DatMgr_AllocateSlot();
    state->unk_11588 = DatMgr_AllocateSlot();
    MenuEquip_RegisterVBlank();
    state->unk_11580 = ResourceMgr_ReinitManagers(&state->unk_00000);
    TouchInput_Init();
    Mem_InitializeHeap(&state->heap, state->heapBuffer, sizeof(state->heapBuffer));
    EasyTask_InitializePool(&state->taskPool, &state->heap, 0x100, NULL, NULL);
    FS_LoadOverlay(0, (u32)&OVERLAY_31_ID);
    data_02066aec = 0;
    data_02066eec = 0;
    MenuEquip_InitState(state);
    MenuEquip_LoadBackgrounds(&state->menuEquip);
    MenuEquip_CreateTasks(state);
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)MenuEquip_StageFadeOut, state, PROCESS_STAGE_INIT);
    DebugOvlDisp_Push((OverlayCB)MenuEquip_StageFadeIn, state, PROCESS_STAGE_INIT);
    MainOvlDisp_NextProcessStage();
}

void MenuEquip_Update(MenuEquipState* state) {
    TouchInput_Update();
    OamMgr_Reset3DState();
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    MenuEquip_UpdateBackgrounds(&state->menuEquip);
    DebugOvlDisp_Run();
    EasyTask_UpdatePool(&state->taskPool);
    if (DebugOvlDisp_IsStackAtBase() == TRUE) {
        state->unk_217F8 = 1;
    }
    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_EXTENDED], NULL);

    if (state->unk_217F8 != 0) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_43_ID, func_ov043_02084040, NULL, PROCESS_STAGE_INIT);
    }
}

void MenuEquip_Destroy(MenuEquipState* state) {
    MenuEquip_WriteBackToSave(&state->menuEquip);
    MenuEquip_ReleaseBackgrounds(&state->menuEquip);
    EasyTask_DestroyPool(&state->taskPool);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    DatMgr_ClearSlot(state->unk_11588);
    MenuEquip_DeregisterVBlank();
    FS_UnloadOverlay(0, (u32)&OVERLAY_31_ID);
    Mem_Free(&gDebugHeap, state);
}

void ProcessOverlay_MenuEquip(MenuEquipState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        MenuEquip_Destroy(state);
    } else {
        data_ov043_020c96e4.funcs[stage](state);
    }
}

void MenuEquip_InitDisplay(void) {
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
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG3 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_MAIN].layers &= 0xFF;
    data_0206aa78                                       = 0x300010;
    data_0206aa7c                                       = 0x400040;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 16;

    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode &= 0xFF;
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

void MenuEquip_VBlank(void) {
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

void MenuEquip_RegisterVBlank(void) {
    MenuEquip_InitDisplay();
    Interrupts_RegisterVBlankCallback(MenuEquip_VBlank, 1);
}

void MenuEquip_DeregisterVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
