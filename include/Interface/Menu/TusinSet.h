#ifndef INTERFACE_MENU_TUSINSET_H
#define INTERFACE_MENU_TUSINSET_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save/MainData.h"
#include "SpriteMgr.h"

// An item as shown on the mingle mode setup screen.
typedef struct {
    /* 0x0 */ u16  itemId;
    /* 0x2 */ u16  graphicIndex;
    /* 0x4 */ u8   unk_4;
    /* 0x5 */ u8   abilityUnlocked;
    /* 0x6 */ char unk_6[0x8 - 0x6];
    /* 0x8 */ u8   category;
    /* 0x9 */ u8   subCategory;
    /* 0xA */ char unk_A[0x10 - 0xA];
} TusinSetItem; // Size: 0x10

// An equipped pin as shown on the mingle mode setup screen.
typedef struct {
    /* 0x0 */ u16  pinId;
    /* 0x2 */ u16  iconIndex;
    /* 0x4 */ u8   unk_4;
    /* 0x5 */ u8   unk_5;
    /* 0x6 */ u8   level;
    /* 0x7 */ u8   maxLevel;
    /* 0x8 */ char unk_8[0xA - 0x8];
    /* 0xA */ u16  levelPP;
    /* 0xC */ u16  totalPP;
    /* 0xE */ u16  nextLevelPP;
} TusinSetPin; // Size: 0x10

typedef struct {
    /* 0x0000 */ TusinSetItem   threads[16]; // [0]-[3]: player, [4]-[15]: partners
    /* 0x0100 */ TusinSetItem   gift;
    /* 0x0110 */ TusinSetItem*  slots[9];    // [0]-[3]: player, [4]-[7]: partner, [8]: gift
    /* 0x0134 */ TusinSetPin    pins[6];
    /* 0x0194 */ TusinSetItem   inventory[472];
    /* 0x1F14 */ TusinSetItem*  lists[9][472]; // Inventory filtered per tab
    /* 0x6174 */ TusinSetItem*  visible[16];
    /* 0x61B4 */ TusinSetItem   selected;
    /* 0x61C4 */ u8             badgeSlots;
    /* 0x61C5 */ char           unk_61C5;
    /* 0x61C6 */ u16            flags;
    /* 0x61C8 */ u8             unk_61C8;
    /* 0x61C9 */ u8             nextProcess;
    /* 0x61CA */ u8             iconPressed[2];
    /* 0x61CC */ u8             iconTimer;
    /* 0x61CD */ char           unk_61CD;
    /* 0x61CE */ u16            unk_61CE;
    /* 0x61D0 */ s16            partner;
    /* 0x61D2 */ u8             unk_61D2;
    /* 0x61D3 */ u8             unk_61D3;
    /* 0x61D4 */ u16            scroll;
    /* 0x61D6 */ u16            unk_61D6;
    /* 0x61D8 */ u16            cursor;
    /* 0x61DA */ s16            unk_61DA;
    /* 0x61DC */ u32            esperPoints;
    /* 0x61E0 */ u16            noiseReportCount;
    /* 0x61E2 */ u16            pinsMastered;
    /* 0x61E4 */ u16            itemsCollected;
    /* 0x61E6 */ char           unk_61E6[0x61E8 - 0x61E6];
    /* 0x61E8 */ u32            timeAttackFrames;
    /* 0x61EC */ u8             unk_61EC;
    /* 0x61ED */ u8             tabSelected[8];
    /* 0x61F5 */ u8             tab;
    /* 0x61F6 */ u8             helpButtonPressed[3];
    /* 0x61F9 */ u8             helpPage;
    /* 0x61FA */ u16            helpOpen;
    /* 0x61FC */ MenuBgResource resources[8]; // [0]-[3]: sub BG0-BG3, [4]-[7]: main BG0-BG3
#ifdef REGION_USA
    /* 0x62DC */ u16 listSizes[9];
    /* 0x62EE */ u16 maxScrollRow[9];
    /* 0x6300 */ u16 scrollBarRange[9];
    /* 0x6312 */ u16 fitsOnePage[9];
#endif
} TusinSetObject; // Size: 0x6324 (JP: 0x62DC)

extern const BinIdentifier TusinSet_BinIdentifiers[];
extern const Point         TusinSet_SlotPositions[16];
extern TusinSetItem        TusinSet_EmptyItem;

// TusinSetData.c
s32  TusinSet_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height);
void TusinSet_SetSpriteFrame(Sprite* sprite, s16 frame);
void TusinSet_LoadPinData(RawPinData* buffer);
void TusinSet_LoadItemData(RawItemData* buffer);
void TusinSet_LoadFoodData(RawFoodData* buffer);
void TusinSet_LoadTreasureData(RawTreasureData* buffer);
u8   TusinSet_GetBadgeSlotCount(void);
u16  TusinSet_SumPinPP(u16 battlePP, u16 minglePP, u16 shutdownPP);
u16  TusinSet_GetPinLevelPP(u8 level, s32 ppCurve);
u16  TusinSet_GetPinNextLevelPP(u8 level, u8 maxLevel, s32 ppCurve);
#ifdef REGION_USA
u16 TusinSet_BuildListBySubCategory(TusinSetObject* tusinSet, s32 tab, s32 category, s32 subCategory);
u16 TusinSet_BuildListByCategory(TusinSetObject* tusinSet, s32 tab, s32 category);
u16 TusinSet_RoundUpToPages(u16 count);
#else
void TusinSet_BuildListBySubCategory(TusinSetObject* tusinSet, s32 tab, s32 category, s32 subCategory);
void TusinSet_BuildListByCategory(TusinSetObject* tusinSet, s32 tab, s32 category);
#endif
void TusinSet_BuildLists(TusinSetObject* tusinSet);
s32  TusinSet_CompareItemIds(u16* a, u16* b);
void TusinSet_SortInventory(TusinSetObject* tusinSet);
void TusinSet_LoadFromSave(TusinSetObject* tusinSet);
void TusinSet_WriteToSave(TusinSetObject* tusinSet);
s16  TusinSet_GetIconAtPoint(s16 x, s16 y);
s32  TusinSet_IsPointOnBtn(s16 x, s16 y);
s16  TusinSet_GetTabAtPoint(s16 x, s16 y);
s16  TusinSet_GetPartnerAtPoint(s16 x, s16 y);
s16  TusinSet_GetSlotAtPoint(s16 x, s16 y);
s32  TusinSet_IsPointOnSbar(s16 x, s16 y);
s32  TusinSet_IsPointOnSbarKnob(s16 x, s16 y, s16 knobX, s16 knobY);
s16  TusinSet_GetSbarArrowAtPoint(s16 x, s16 y);
s16  TusinSet_GetHelpBtnAtPoint(s16 x, s16 y);
void TusinSet_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void TusinSet_LoadBgResourceIndexed(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                    s32 screenIndex, s32 palIndex);
void TusinSet_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex);
void TusinSet_ReleaseBgResource(MenuBgResource* res, s32 engine);
void TusinSet_ReleaseBgScreen(MenuBgResource* res, s32 engine);
void TusinSet_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void TusinSet_ClearBgResource(MenuBgResource* res);
void TusinSet_LoadBackgrounds(TusinSetObject* tusinSet);
void TusinSet_UpdateBackgrounds(TusinSetObject* tusinSet);
void TusinSet_ReleaseBackgrounds(TusinSetObject* tusinSet);

// TusinSet.c
void TusinSet_ChangePartner(TusinSetObject* tusinSet, u16 partner);

#endif // INTERFACE_MENU_TUSINSET_H
