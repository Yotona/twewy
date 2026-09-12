#ifndef INTERFACE_MENU_MENUEQUIP_H
#define INTERFACE_MENU_MENUEQUIP_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "SpriteMgr.h"

// A single item entry as displayed/sorted in the equip lists
typedef struct {
    /* 0x00 */ u16 itemId;      // 0xFFFF = empty
    /* 0x02 */ u16 graphicIndex;
    /* 0x04 */ u8  category;    // ItemCategory
    /* 0x05 */ u8  subCategory; // Thread kind, matching list tabs 0-5
    /* 0x06 */ u16 capacityCost;
    /* 0x08 */ u16 braveryReq;
    /* 0x0A */ u16 attackBonus;
    /* 0x0C */ u16 defenseBonus;
    /* 0x0E */ u16 healthBonus;
    /* 0x10 */ u8  count;
    /* 0x11 */ u8  unk_11; // Round-trips to gSaveData.inventoryItems[].flags bit 4; never read
    /* 0x12 */ u8  abilityUnlocked;
    /* 0x13 */ u8  brand;
    /* 0x14 */ s32 unk_14; // Copied from Raw{Item,Food,Treasure}Data.unk_04; never read
    /* 0x18 */ u8  abilityId;
    /* 0x19 */ u8  unk_19; // Never written or read
    /* 0x1A */ u16 unk_1A; // Copied from RawItemData.unk_12; never read
    /* 0x1C */ u16 unk_1C; // Copied from RawItemData.unk_14; never read
    /* 0x1E */ u16 foodEffectMsg;
    /* 0x20 */ s16 foodEffectValue;
    /* 0x22 */ u16 syncGain;
    /* 0x24 */ u8  taste[4]; // Per character: Neku, then partners
} MenuEquipItemEntry;        // Size: 0x28

typedef struct {
    /* 0x00 */ s16 health;
    /* 0x02 */ s16 healthBonus;
    /* 0x04 */ u16 sync; // Tenths of a percent
    /* 0x06 */ s16 braveryBonus;
    /* 0x08 */ s16 bravery;
    /* 0x0A */ s16 attack;
    /* 0x0C */ s16 attackBonus;
    /* 0x0E */ s16 defense;
    /* 0x10 */ s16 defenseBonus;
    /* 0x12 */ u8  foodCapacityUsed;
    /* 0x13 */ u8  foodCapacityLeft;
} MenuEquipStats; // Size: 0x14

// Loaded background/palette resource, shared by all of MenuEquip's background layers.
// Same shape as DepartResources (see Depart.h).
typedef struct {
    /* 0x00 */ Data*            data;
    /* 0x04 */ BgResource*      screenResource;
    /* 0x08 */ BgResource*      charResource;
    /* 0x0C */ PaletteResource* paletteResource;
    /* 0x10 */ u16*             charData;
    /* 0x14 */ u16*             screenMap;
    /* 0x18 */ u8*              paletteData;
} MenuEquipResource; // Size: 0x1C

typedef struct {
    /* 0x0000 */ MenuEquipItemEntry  equipped[4][5];   // Per character (row 0 is Neku): 4 threads, then food
    /* 0x0320 */ MenuEquipItemEntry  inventory[472];   // Mirrors gSaveData.inventoryItems
    /* 0x4CE0 */ MenuEquipItemEntry* tabItems[9][472]; // Per-tab filtered views into inventory (row 8 = all items)
    /* 0x8F40 */ MenuEquipItemEntry* slots[26];        // One per cursor slot (see data_ov043_020c96f0): 0-4 Neku,
                                                       // 5-9 partner, 10-25 the visible list rows
    /* 0x8FA8 */ char               unk_8FA8[0xD968 - 0x8FA8];
    /* 0xD968 */ MenuEquipStats     stats[4];          // Row 0 is Neku, row N+1 is partner N
    /* 0xD9B8 */ MenuEquipItemEntry cursorItem;        // Copy of the entry under the cursor
    /* 0xD9E0 */ u16                dirtyFlags;        // Redraw requests: 0x1 dragging, 0x20 textScrU, 0x40 textScr,
                                                       // 0x80 numNek, 0x100 numPtr, 0x200 error window
    /* 0xD9E2 */ char              unk_D9E2[0xD9E4 - 0xD9E2];
    /* 0xD9E4 */ s16               dragSrcSlot;
    /* 0xD9E6 */ s16               dragDstSlot;
    /* 0xD9E8 */ s16               touchPosX;
    /* 0xD9EA */ s16               touchPosY;
    /* 0xD9EC */ u16               cursorSlot;      // Indexes slots[]
    /* 0xD9EE */ u16               cursorListIndex; // 0xFFFF while the cursor sits on an equip slot
    /* 0xD9F0 */ u16               listScroll;      // First visible list index
    /* 0xD9F2 */ u16               prevListScroll;  // Previous listScroll, for change detection
    /* 0xD9F4 */ u8                iconPressed[2];
    /* 0xD9F6 */ u8                pressTimer;
    /* 0xD9F7 */ char              unk_D9F7[0xD9F8 - 0xD9F7];
    /* 0xD9F8 */ s16               activePartner;
    /* 0xD9FA */ u16               partnerArtSet;
    /* 0xD9FC */ u8                tabActive[8];
    /* 0xDA04 */ u8                currentTab;
    /* 0xDA05 */ u8                itemUBuffer;
    /* 0xDA06 */ u8                unk_DA06; // Only ever written 0
    /* 0xDA07 */ char              unk_DA07;
    /* 0xDA08 */ u16               errorCode;
    /* 0xDA0A */ u8                helpPressed[3];
    /* 0xDA0D */ u8                helpPage;
    /* 0xDA0E */ u16               helpOpen;
    /* 0xDA10 */ MenuEquipResource resources[8]; // [0]/[5] passed to MenuEquip_ReloadBgResource, [7] to
                                                 // MenuEquip_ReleaseBgResource/MenuEquip_LoadBgResourceIndexed
    /* 0xDAF0 */ u16  pagedCount[9];             // Indexed by currentTab
    /* 0xDB02 */ u16  maxScrollRow[9];           // Indexed by currentTab
    /* 0xDB14 */ u16  scrollBarRange[9];         // Indexed by currentTab
    /* 0xDB26 */ u16  fitsOnePage[9];            // Indexed by currentTab
    /* 0xDB38 */ u8   swagAttackShiki;
    /* 0xDB39 */ u8   swagDefenseShiki;
    /* 0xDB3A */ u8   swagAttackJoshua;
    /* 0xDB3B */ u8   swagDefenseJoshua;
    /* 0xDB3C */ u8   swagAttackBeat;
    /* 0xDB3D */ u8   swagDefenseBeat;
    /* 0xDB3E */ char unk_DB3E[0x2];
} MenuEquipObject; // Size: 0xDB40

extern const s16           data_ov043_020c96f0[][2];
extern const BinIdentifier data_ov043_020c9758[15];
extern MenuEquipItemEntry  data_ov043_020cc2d8; // Empty slot

// MenuEquip.c
void MenuEquip_SwitchPartnerEquipment(MenuEquipObject* menuEquip, u16 index);
#ifdef REGION_USA
void MenuEquip_RefreshCursorInfo(void);
#endif

// MenuEquipData.c
void MenuEquip_SetSpriteFrame(Sprite* sprite, s16 frame);
void MenuEquip_SetSpriteFrameFromPack(Sprite* sprite, s16 frame, s32 animIndex, s32 frameDataIndex);
void MenuEquip_SwapItemEntries(MenuEquipItemEntry* a, MenuEquipItemEntry* b);
u16  MenuEquip_GetFreePaletteSlot(void);
s32  MenuEquip_IsCdItem(u32 itemId);
void MenuEquip_PlayCdTrack(u16 itemId);
void MenuEquip_ApplySyncGain(MenuEquipObject* menuEquip, u8 character);
s32  MenuEquip_CanEquipSubCategory(MenuEquipObject* menuEquip, u16 character, u8 subCategory, u8 replacedSubCategory);
void MenuEquip_UpdateStatBonuses(MenuEquipObject* menuEquip);
void MenuEquip_LoadFromSave(MenuEquipObject* menuEquip);
void MenuEquip_WriteBackToSave(MenuEquipObject* menuEquip);
s16  MenuEquip_GetSlotAtPoint(s16 x, s16 y);
s16  MenuEquip_GetDropTargetAtPoint(s16 x, s16 y);
s16  MenuEquip_GetButtonAtPoint(s16 x, s16 y);
s16  MenuEquip_GetTabAtPoint(s16 x, s16 y);
s16  MenuEquip_GetPartnerTabAtPoint(s16 x, s16 y);
s32  MenuEquip_IsPointOnScrollBar(s16 x, s16 y);
s32  MenuEquip_IsPointOnScrollKnob(s16 x, s16 y, s16 targetX, s16 targetY);
s32  MenuEquip_GetScrollArrowAtPoint(s16 x, s16 y);
s32  MenuEquip_CanDragSlot(MenuEquipObject* menuEquip, s32 slot);
s32  MenuEquip_CanDropOnSlot(MenuEquipObject* menuEquip, s32 src, s32 dst);
s32  MenuEquip_StackIntoInventory(MenuEquipObject* menuEquip, u16 itemId);
s32  MenuEquip_InsertIntoInventory(MenuEquipObject* menuEquip, MenuEquipItemEntry* item);
void MenuEquip_PlayTasteVoice(u8 character, u32 taste);
s16  MenuEquip_GetHelpButtonAtPoint(s16 x, s16 y);
void MenuEquip_LoadBgResourceIndexed(MenuEquipResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, s32 palCount,
                                     s32 screenIndex, s32 palIndex);
void MenuEquip_ReleaseBgResource(MenuEquipResource* res, s32 engine);
void MenuEquip_ReloadBgResource(MenuEquipResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, s32 palCount);
void MenuEquip_LoadBackgrounds(MenuEquipObject* menuEquip);
void MenuEquip_UpdateBackgrounds(MenuEquipObject* menuEquip);
void MenuEquip_ReleaseBackgrounds(MenuEquipObject* menuEquip);

s32   MenuEquip_brdLogoU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_foodGauge_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuEquipObject* menuEquip);
s32   MenuEquip_helpCur_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_helpCurU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_icon_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_item_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* menuEquip);
void* MenuEquip_item_GetTaskData(TaskPool* pool, u32 taskId);
s32   MenuEquip_itemAbl_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* menuEquip);
s32   MenuEquip_itemCur_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_itemMask_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* menuEquip);
s32   MenuEquip_itemNum_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* menuEquip);
s32   MenuEquip_itemU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_mov_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_nameD_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_nameU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_numItemIdU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_numNek_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_numPtr_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_partner_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_partnerHide_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_pointer_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_sbar_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_sbar2_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_shadow_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_tab_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_textItem_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_textScr_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_textScrU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);
s32   MenuEquip_window_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* menuEquip);

#endif // INTERFACE_MENU_MENUEQUIP_H
