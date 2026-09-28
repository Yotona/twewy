#ifndef INTERFACE_MENU_MENUBADGE_H
#define INTERFACE_MENU_MENUBADGE_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "SpriteMgr.h"

// A single pin as displayed/sorted in the badge menu. Filled from the save data,
// Apl_Tak/BadgeData.bin (RawPinData), and Apl_Tak/BeBadge_Parm.bin (Tin Pin Slammer stats).
typedef struct {
    /* 0x00 */ u16  pinId; // 0xFFFF = empty
    /* 0x02 */ u16  battlePP;
    /* 0x04 */ u16  minglePP;
    /* 0x06 */ u16  shutdownPP;
    /* 0x08 */ u16  levelPP;     // PP needed for the current level
    /* 0x0A */ u16  totalPP;     // battlePP + minglePP + shutdownPP
    /* 0x0C */ u16  nextLevelPP; // 0xFFFF once maxed
    /* 0x0E */ char unk_0E[0x10 - 0x0E];
    /* 0x10 */ u32  maxPP;
    /* 0x14 */ u8   level;
    /* 0x15 */ u8   deckSlot;  // slot it was loaded into, for pins in a deck
    /* 0x16 */ u8   unk_16;    // L/R toggles 0/1 (bdgSC icon); 2 = fixed, for "Touch the pin" pins
    /* 0x17 */ u8   count;     // stack size in the mastered list
    /* 0x18 */ u8   unk_18;    // PinFlags.bits.unk_07
    /* 0x19 */ char unk_19[0x1A - 0x19];
    /* 0x1A */ u16  iconIndex; // sprite pack index - 1
    /* 0x1C */ u16  psychId;   // 0xFFFF = none (abilityId is shown instead)
    /* 0x1E */ u8   unk_1E;
    /* 0x1F */ u8   brand;
    /* 0x20 */ u8   ppCurve;
    /* 0x21 */ u8   pinClass;     // PinClass, decides how many can share a deck
    /* 0x22 */ char unk_22[0x24 - 0x22];
    /* 0x24 */ u32  price;        // 10000000 = cannot be sold; sells for half
    /* 0x28 */ s16  priceGrowth;
    /* 0x2A */ u16  attack;       // 0 = none
    /* 0x2C */ s16  attackGrowth;
    /* 0x2E */ u16  durationType; // 1-2: lasts `duration` uses, 3+: lasts `duration` frames
    /* 0x30 */ u16  duration;
    /* 0x32 */ s16  durationGrowth;
    /* 0x34 */ u16  bootType; // 0: instant, 1: ready after `bootTime` frames
    /* 0x36 */ u16  bootTime;
    /* 0x38 */ s16  bootTimeGrowth;
    /* 0x3A */ u16  rebootType; // 1: reboots after `rebootTime` frames
    /* 0x3C */ u16  rebootTime;
    /* 0x3E */ s16  rebootTimeGrowth;
    /* 0x40 */ u8   abilityId; // 0xFF = none
    /* 0x41 */ u8   maxLevel;
    /* 0x42 */ u8   inputType; // "Touch the pin", "Slash Neku", ...; 0xFF = none
    /* 0x43 */ u8   tinPinWeight;
    /* 0x44 */ u8   tinPinSpin;
    /* 0x45 */ u8   tinPinKOLength; // seconds
    /* 0x46 */ u8   whammyStinger;
    /* 0x47 */ u8   whammyBomber;
    /* 0x48 */ u8   whammyHammer;
    /* 0x49 */ u8   whammyHand;
    /* 0x4A */ char unk_4A[0x4C - 0x4A];
} MenuBadgeEntry; // Size: 0x4C

// MenuBadgeObject.flags
#define MENUBADGE_FLAG_DRAGGING         0x0001 // a badge is being dragged
#define MENUBADGE_FLAG_DROP_VALID       0x0002 // the dragged badge is over a valid drop target
#define MENUBADGE_FLAG_SCROLL_JUMP      0x0008 // scroll track tapped: jump the knob there
#define MENUBADGE_FLAG_SCROLL_DRAG      0x0010 // scroll knob held
#define MENUBADGE_FLAG_QUICK_SELL       0x0020 // sell box toggled on: sell without confirming
#define MENUBADGE_FLAG_INFO_TAB_CHANGED 0x0040
#define MENUBADGE_FLAG_REDRAW_INFO      0x0080 // redraw the upper-screen info text
#define MENUBADGE_FLAG_REDRAW_WINDOW    0x0100 // redraw the open window's text
#define MENUBADGE_FLAG_MONEY_CAP_WARNED 0x0800
#define MENUBADGE_FLAG_EXITING          0x1000

// MenuBadgeObject.windowMessage: the open window / message box, 0 = none
#define MENUBADGE_MSG_NONE          0
#define MENUBADGE_MSG_HELP          1
#define MENUBADGE_MSG_SELL_CONFIRM  2  // "Cash in this pin?"
#define MENUBADGE_MSG_ARRANGE       3  // sort window
#define MENUBADGE_MSG_CANNOT_SELL   4
#define MENUBADGE_MSG_CLASS_LIMIT   5  // + PinClass of the rejected pin
#define MENUBADGE_MSG_TOO_MANY_PINS 10 // excessPinCount over the 200 limit
#define MENUBADGE_MSG_MONEY_CAPPED  11

typedef struct {
    /* 0x0000 */ MenuBadgeEntry  decks[4][6];
    /* 0x0720 */ MenuBadgeEntry  stockpile[256];
    /* 0x5320 */ MenuBadgeEntry  mastered[304];
    /* 0xAD60 */ MenuBadgeEntry* slots[30]; // 0-5 the current deck, 6-29 the visible list page
    /* 0xADD8 */ MenuBadgeEntry  cursorBadge;
    /* 0xAE24 */ MenuBadgeEntry  dragBadge;
    /* 0xAE70 */ BOOL            slotVisible[30]; // FALSE while that slot's badge is being dragged
    /* 0xAEE8 */ u16             flags;           // MENUBADGE_FLAG_*
    /* 0xAEEA */ u8              deckSlotCount;   // unlocked deck slots, 2-6
    /* 0xAEEB */ u8              listMode;        // 0 = stockpile, 1 = mastered
    /* 0xAEEC */ s16             dragSrc;         // slot the drag started on, -1 = none
    /* 0xAEEE */ s16             dragDst;         // slot under the dragged badge; 40 = back to the list
    /* 0xAEF0 */ s16             unk_AEF0;
    /* 0xAEF2 */ Point           touchPos;
    /* 0xAEF6 */ u16             cursorSlot;       // 0-29
    /* 0xAEF8 */ u16             cursorListIndex;  // index into the current list, 0xFFFF = on the deck
    /* 0xAEFA */ u16             listTop;          // first list entry on the visible page
    /* 0xAEFC */ u16             prevListTop;
    /* 0xAEFE */ u8              infoTab;          // upper-screen page: 0 = effect, 1 = growth, 2 = Tin Pin
    /* 0xAEFF */ u8              buttonPressed[3]; // arrange, help, exit
    /* 0xAF02 */ u8              pressTimer;       // frames a pressed button stays highlighted
    /* 0xAF03 */ u8              badgeVramPage;    // double-buffers the upper-screen badge
    /* 0xAF04 */ u8              exitDest;         // 0 = Top menu, 1 = overlay 27
    /* 0xAF05 */ u8              moneyCapLevel;
    /* 0xAF06 */ char            unk_AF06[0xAF08 - 0xAF06];
    /* 0xAF08 */ s32             unk_AF08;
    /* 0xAF0C */ u32             money;
    /* 0xAF10 */ u16             sellCount;
    /* 0xAF12 */ u8              sellButtonPressed[2]; // confirm, cancel
    /* 0xAF14 */ char            unk_AF14[0xAF18 - 0xAF14];
    /* 0xAF18 */ u8              sellArrowPressed[2];  // count up, count down
    /* 0xAF1A */ char            unk_AF1A[0xAF1C - 0xAF1A];
    /* 0xAF1C */ u8              helpButtonPressed[3]; // previous, next, close
    /* 0xAF1F */ u8              helpPage;
    /* 0xAF20 */ u16             windowMessage;        // MENUBADGE_MSG_*; tasks pause while non-zero
    /* 0xAF22 */ s16             unk_AF22;
    /* 0xAF24 */ s16             windowCloseTimer;
    /* 0xAF26 */ u8              currentDeck;
    /* 0xAF27 */ u8              arrangeButtonPressed[4]; // by number, by psych, always, close
    /* 0xAF2B */ u8              autoArrangeBy[2];        // [0] by number, [1] by psych
    /* 0xAF2D */ u8              autoArrange;             // "Always arrange"
    /* 0xAF2E */ char            unk_AF2E[0xAF30 - 0xAF2E];
    /* 0xAF30 */ u16             excessPinCount;          // unmastered pins over the 200 limit
    /* 0xAF32 */ s16             masteredCounts[304];     // mastered copies owned, by pin ID
    /* 0xB192 */ char            unk_B192[0xB194 - 0xB192];
    /* 0xB194 */ MenuBgResource  resources[8];
} MenuBadgeObject; // Size: 0xB274

extern const BinIdentifier MenuBadge_BinIdentifiers[15];
extern const Point         MenuBadge_SlotPositions[30];
extern MenuBadgeEntry      MenuBadge_EmptyEntry;

s32  MenuBadge_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height);
void MenuBadge_SetSpriteFrame(Sprite* sprite, s16 frame);
void MenuBadge_SetSpriteFrameFromPack(Sprite* sprite, s16 frame, s32 animIndex, s32 frameDataIndex);
s32  MenuBadge_IsSlotMastered(MenuBadgeObject* owner, u16 index);
u16  MenuBadge_GetFreePaletteSlot(void);
void MenuBadge_SortList(MenuBadgeObject* menuBadge, u8 listMode, s32 sortMode);
u32  MenuBadge_CalcSellPrice(u32 basePrice, s16 levelPrice, u8 level);
s32  MenuBadge_ClampMoney(MenuBadgeObject* owner, u8 capLevel);
s32  MenuBadge_CanEquipToDeck(MenuBadgeObject* menuBadge, u16 src, u16 dst);
u32  MenuBadge_CountUnmasteredBadges(MenuBadgeObject* menuBadge);
void MenuBadge_SubtractMasteredCount(MenuBadgeObject* menuBadge, u16 pinId, u16 count);
void MenuBadge_InitBadgeData(MenuBadgeObject* menuBadge);
void MenuBadge_WriteBackToSave(MenuBadgeObject* menuBadge);
s16  MenuBadge_GetSlotAtPoint(s16 x, s16 y, u8 slotCount);
s16  MenuBadge_GetDropTargetAtPoint(s16 x, s16 y, u16 src, u8 slotCount);
s16  MenuBadge_GetButtonAtPoint(s16 x, s16 y);
s16  MenuBadge_GetListTabAtPoint(s16 x, s16 y);
s32  MenuBadge_IsPointOnScrollBar(s16 x, s16 y);
s32  MenuBadge_IsPointOnScrollKnob(s16 x, s16 y, s16 targetX, s16 targetY);
s32  MenuBadge_GetScrollArrowAtPoint(s16 x, s16 y);
s32  MenuBadge_IsPointOnSellBox(s16 x, s16 y);
u32  MenuBadge_GetTabAtPoint(s16 x, s16 y);
s32  MenuBadge_CanDragSlot(MenuBadgeObject* menuBadge, s16 slot);
s32  MenuBadge_CanDropOnSlot(MenuBadgeObject* menuBadge, s16 src, s16 dst);
s16  MenuBadge_GetSellButtonAtPoint(s16 x, s16 y);
s16  MenuBadge_GetSortButtonAtPoint(s16 x, s16 y);
s16  MenuBadge_GetSellCountArrowAtPoint(s16 x, s16 y);
s16  MenuBadge_GetHelpButtonAtPoint(s16 x, s16 y);
s16  MenuBadge_GetDeckTabAtPoint(s16 x, s16 y);
void MenuBadge_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void MenuBadge_ReloadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex);
void MenuBadge_LoadBackgrounds(MenuBadgeObject* menuBadge);
void MenuBadge_UpdateBackgrounds(MenuBadgeObject* menuBadge);
void MenuBadge_ReleaseBackgrounds(MenuBadgeObject* menuBadge);

s32   MenuBadge_bdg_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
void* MenuBadge_bdg_GetTaskData(TaskPool* pool, u32 taskId);
s32   MenuBadge_bdgBP_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
s32   MenuBadge_bdgCur_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_bdgLV_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
s32   MenuBadge_bdgNum_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
s32   MenuBadge_bdgPRI_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
s32   MenuBadge_bdgSC_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
s32   MenuBadge_bdgU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_bpGaugeU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_brdLogoU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_gbgBox_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_helpCur_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_helpCurU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_icon_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_mov_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_mstStarU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_nameU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_numBdgIdU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_numMoney_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_pointer_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_sbar_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_shadow_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_slotCover_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuBadgeObject* menuBadge);
s32   MenuBadge_stkmstIn_CreateTask(TaskPool* pool, s32 dataType, Point pos, s16 frame, MenuBadgeObject* menuBadge);
s32   MenuBadge_tab_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_tabBdgType_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_tabDeck_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_textScr_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_textScrU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_window0_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_window1_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);
s32   MenuBadge_window2_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge);

#endif // INTERFACE_MENU_MENUBADGE_H
