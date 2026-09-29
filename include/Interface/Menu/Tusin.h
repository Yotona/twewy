#ifndef INTERFACE_MENU_TUSIN_H
#define INTERFACE_MENU_TUSIN_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save/MainData.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ u16  pinID;
    /* 0x02 */ u8   brand;
    /* 0x03 */ u8   unk_03;
    /* 0x04 */ u32  pp[3]; // Battle, mingle, shutdown
    /* 0x10 */ u32  startPP[3];
    /* 0x1C */ u32  levelPP;
    /* 0x20 */ u32  totalPP;
    /* 0x24 */ u32  nextLevelPP;
    /* 0x28 */ u32  ppToNextLevel;
    /* 0x2C */ u8   level;
    /* 0x2D */ u8   maxLevel;
    /* 0x2E */ u8   slot;
    /* 0x2F */ u8   unk_2F;
    /* 0x30 */ u8   unk_30;
    /* 0x31 */ u8   unk_31;
    /* 0x32 */ u8   ppCurve;
    /* 0x33 */ u8   evolveChoice; // Index into evolvePinID, or 2 if the badge does not evolve
    /* 0x34 */ u8   evolveLevel[2];
    /* 0x36 */ u8   evolveCondition[2];
    /* 0x38 */ u16  evolvePinID[2];
    /* 0x3C */ u16  flags;
    /* 0x3E */ char unk_3E[0x40 - 0x3E];
} TusinBadge; // Size: 0x40

// The part of a player's profile that is sent to other players.
typedef struct {
    /* 0x000 */ Experience   experience;
    /* 0x00C */ MingleShop   shop;
    /* 0x03C */ MingleFriend friends[50];
} TusinProfile; // Size: 0x294

typedef struct {
#ifdef REGION_USA
    /* 0x000 */ TusinBadge badges[6];
#endif
    // 0x180-0x468 is the profile sent to other players.
    /* 0x180 */ u8             macAddress[6];
    /* 0x186 */ u16            nickName[11];
    /* 0x19C */ u16            message[27];
    /* 0x1D2 */ char           unk_1D2[0x1D4 - 0x1D2];
    /* 0x1D4 */ TusinProfile   profile;
    /* 0x468 */ u8             unk_468;
    /* 0x469 */ char           unk_469;
    /* 0x46A */ u16            flags;
    /* 0x46C */ u8             unk_46C;
    /* 0x46D */ u8             nextProcess;
    /* 0x46E */ u16            civviesMet;
    /* 0x470 */ u16            espersMet;
    /* 0x472 */ u16            aliensMet;
    /* 0x474 */ u16            totalMet;
    /* 0x476 */ u16            mingleRemaining;
    /* 0x478 */ s16            alienTimer;
    /* 0x47A */ u16            friendName[10];
    /* 0x48E */ u16            giftItemId;
    /* 0x490 */ u8             unk_490[50];
    /* 0x4C2 */ u8             unk_4C2[50];
    /* 0x4F4 */ u32            totalRakedMoney;
    /* 0x4F8 */ u8             btnPressed;
    /* 0x4F9 */ u8             btnFState;
    /* 0x4FA */ u8             btnLRPressed[2];
    /* 0x4FC */ u8             btnLRTimer;
    /* 0x4FD */ char           unk_4FD;
    /* 0x4FE */ u16            statusMsg;
    /* 0x500 */ u32            lastRakedMoney;
    /* 0x504 */ u32            money;
    /* 0x508 */ u16            alienKind;
    /* 0x50A */ u8             moneyCapLevel;
    /* 0x50B */ char           unk_50B;
    /* 0x50C */ MenuBgResource resources[8]; // [0]-[3]: main BG0-BG3, [4]-[7]: sub BG0-BG3
} TusinObject;                               // Size: 0x5EC (JP: 0x46C)

extern const BinIdentifier Tusin_BinIdentifiers[];

// TusinData.c
s32  Tusin_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height);
void Tusin_SetSpriteFrame(Sprite* sprite, s16 frame);
void Tusin_LoadPinData(RawPinData* pinData, u16 index);
void Tusin_LoadItemData(RawItemData* itemData, u16 index);
void Tusin_LoadFoodData(RawFoodData* foodData, u16 index);
void Tusin_LoadTreasureData(RawTreasureData* treasureData, u16 index);
u16  Tusin_GetRandomCivvyGift(void);
u16  Tusin_GetAlienGift(u16 alienKind);
void Tusin_AddBravery(u8 partner, u8 amount);
s32  Tusin_ClampMoney(TusinObject* tusin, u8 capLevel);
s32  Tusin_GetMoneyCapLevel(void);
#ifndef REGION_USA
s64 func_ov044_02085060(void);
s64 func_ov044_02085088(PackedDateTime* dateTime);
s64 func_ov044_02085104(s64 a, s64 b);
#endif
u8   Tusin_GetBadgeSlotCount(void);
u32  Tusin_GetBadgeTotalPP(TusinObject* tusin, u16 index);
u32  Tusin_GetBadgeLevelPP(TusinObject* tusin, u16 index, s32 ppCurve);
u32  Tusin_GetBadgeNextLevelPP(TusinObject* tusin, u16 index, s32 ppCurve);
void Tusin_WriteLastProfile(void);
void Tusin_LoadEquipment(TusinObject* tusin);
void Tusin_InitProfile(TusinObject* tusin);
u32  Tusin_CalcMinglePP(u16 civviesMet, u16 espersMet, u16 aliensMet);
void Tusin_ExportSurePins(TusinObject* tusin);
u16  Tusin_GetDominantPPType(u32 battlePP, u32 minglePP, u32 shutdownPP);
u8   Tusin_CheckBadgeEvolution(TusinObject* tusin, u16 index);
void Tusin_EvolveBadge(TusinObject* tusin, u16 index, u16 pinID);
void Tusin_AddBadgePP(TusinObject* tusin, u16 index, u32 pp);
void Tusin_CommitResults(TusinObject* tusin);
s32  Tusin_IsPointOnBtnF(s16 x, s16 y);
s32  Tusin_IsPointOnBtn(s16 x, s16 y);
s16  Tusin_GetBtnLRAtPoint(s16 x, s16 y);
void Tusin_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void Tusin_ReleaseBgResource(MenuBgResource* res, s32 engine);
void Tusin_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void Tusin_ClearBgResource(MenuBgResource* res);
void Tusin_LoadBackgrounds(TusinObject* tusin);
void Tusin_UpdateBackgrounds(TusinObject* tusin);
void Tusin_ReleaseBackgrounds(TusinObject* tusin);

s32 Tusin_textScr_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin);
s32 Tusin_btn_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin);
s32 Tusin_btnF_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin);
s32 Tusin_btnLR_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin);
s32 Tusin_num_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin);
#ifdef REGION_USA
s32 Tusin_beltU_CreateTask(TaskPool* pool, s32 dataType, s32 kind, TusinObject* tusin, u16 index);
#else
s32 Tusin_beltU_CreateTask(TaskPool* pool, s32 dataType, s32 kind, TusinObject* tusin);
#endif

#endif // INTERFACE_MENU_TUSIN_H
