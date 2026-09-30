#ifndef INTERFACE_SAVE_H
#define INTERFACE_SAVE_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save/FriendData.h"
#include "Save/MainData.h"
#include "SpriteMgr.h"

// An equipped thread (or the gift item) as shown on the player's card.
typedef struct {
    /* 0x0 */ u16  itemId;
    /* 0x2 */ u16  graphicIndex;
    /* 0x4 */ u8   unk_4;
    /* 0x5 */ u8   abilityUnlocked;
    /* 0x6 */ char unk_6[0xE - 0x6];
} SaveMenuItem; // Size: 0xE

// An equipped pin as shown on the player's card.
typedef struct {
    /* 0x0 */ u16 pinId;
    /* 0x2 */ u16 iconIndex;
    /* 0x4 */ u8  unk_4;
    /* 0x5 */ u8  unk_5;
    /* 0x6 */ u8  level;
    /* 0x7 */ u8  maxLevel;
    /* 0x8 */ u16 levelPP;
    /* 0xA */ u16 totalPP;
    /* 0xC */ u16 nextLevelPP;
} SaveMenuPin; // Size: 0xE

typedef struct {
    /* 0x000 */ SaveMenuItem  threads[16]; // [0]-[3]: player, [4]-[15]: partners
    /* 0x0E0 */ SaveMenuItem  gift;
    /* 0x0F0 */ SaveMenuItem* slots[9];    // [0]-[3]: player, [4]-[7]: partner, [8]: gift
    /* 0x114 */ SaveMenuPin   pins[6];
    /* 0x168 */ u8            badgeSlots;
    /* 0x16A */ u16           flags;
    /* 0x16C */ u8            unk_16C;
    /* 0x16D */ u8            nextProcess;
    /* 0x16E */ u8            iconPressed[2];
    /* 0x170 */ u8            iconTimer;
    /* 0x172 */ s16           partner;
    /* 0x174 */ u8            saving;
    /* 0x176 */ u16           message;
    /* 0x178 */ u32           saveYear;
    /* 0x17C */ u32           saveMonth;
    /* 0x180 */ u32           saveDay;
    /* 0x184 */ u32           saveHour;
    /* 0x188 */ u32           saveMinute;
    /* 0x18C */ u8            chapter;
    /* 0x18D */ u8            area;
    /* 0x190 */ u32           esperPoints;
    /* 0x194 */ u16           noiseReportCount;
    /* 0x196 */ u16           pinsMastered;
    /* 0x198 */ u16           itemsCollected;
    /* 0x19C */ u32           timeAttackFrames;
    /* 0x1A0 */ u8            bufferIndex;
    /* 0x1A1 */ u8            helpButtonPressed[3];
    /* 0x1A4 */ u8            helpPage;
    /* 0x1A6 */ u16           helpOpen;
    /* 0x1A8 */ s32           loaded;
#ifdef REGION_USA
    // Chapter being saved; the day-end save has already advanced gSaveData.chapter
    /* 0x1AC */ u8 prevChapter;
#endif
    /* 0x1B0 */ MenuBgResource resources[8]; // [0]-[3]: sub BG0-BG3, [4]-[7]: main BG0-BG3
} SaveMenuObject;                            // Size: 0x290 (JP: 0x28C)

extern const BinIdentifier Save_BinIdentifiers[];

// SaveData.c
s32  Save_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height);
void Save_SetSpriteFrame(Sprite* sprite, s16 frame);
void Save_LoadPinData(RawPinData* buffer);
void Save_LoadItemData(RawItemData* buffer);
void Save_LoadFoodData(RawFoodData* buffer);
void Save_LoadTreasureData(RawTreasureData* buffer);
u8   Save_GetBadgeSlotCount(void);
u16  Save_SumPinPP(u16 battlePP, u16 minglePP, u16 shutdownPP);
u16  Save_GetPinLevelPP(u8 level, s32 ppCurve);
u16  Save_GetPinNextLevelPP(u8 level, u8 maxLevel, s32 ppCurve);
void Save_LoadFromSave(SaveMenuObject* save);
void Save_LoadCard(SaveMenuObject* save);
#ifdef REGION_USA
void Save_WriteSnapshot(SaveMenuObject* save, u8 chapter);
#else
void Save_WriteSnapshot(SaveMenuObject* save);
#endif
s16  Save_GetIconAtPoint(s16 x, s16 y);
s32  Save_IsPointOnSaveBtn(s16 x, s16 y);
s16  Save_GetPartnerAtPoint(s16 x, s16 y);
s16  Save_GetHelpBtnAtPoint(s16 x, s16 y);
void Save_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void Save_LoadBgResourceIndexed(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                s32 screenIndex, s32 palIndex);
void Save_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex);
void Save_ReleaseBgResource(MenuBgResource* res, s32 engine);
void Save_ReleaseBgScreen(MenuBgResource* res, s32 engine);
void Save_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void Save_ClearBgResource(MenuBgResource* res);
void Save_LoadBackgrounds(SaveMenuObject* save);
void Save_UpdateBackgrounds(SaveMenuObject* save);
void Save_ReleaseBackgrounds(SaveMenuObject* save);

// Save.c
void Save_ChangePartner(SaveMenuObject* save, u16 partner);

// Tasks
s32  Save_icon_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_nameD_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_nameU_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_chara_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_saveBtn_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_partner_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_textScr_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_helpCur_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_textScrU_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);
s32  Save_itemU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, SaveMenuObject* save);
void Save_itemU_ReleaseSprite(TaskPool* pool, s32 taskId);
s32  Save_bdgU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, SaveMenuObject* save);
void Save_bdgU_ReleaseSprite(TaskPool* pool, s32 taskId);
s32  Save_slotCoverU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, SaveMenuObject* save);
s32  Save_helpCurU_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save);

#endif // INTERFACE_SAVE_H
