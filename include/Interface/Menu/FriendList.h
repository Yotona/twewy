#ifndef INTERFACE_MENU_FRIENDLIST_H
#define INTERFACE_MENU_FRIENDLIST_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "Save/FriendData.h"
#include "Save/MainData.h"
#include "SpriteMgr.h"

// An item or pin as shown on the friend list screen.
typedef struct {
    /* 0x0 */ u16 itemId;
    /* 0x2 */ u16 graphicIndex;
} FriendListIcon; // Size: 0x4

// A registered friend, copied out of the saved GlobalFriendData.
typedef struct {
    /* 0x00 */ u8             macAddress[6];
    /* 0x06 */ u16            nickName[11];
    /* 0x1C */ u16            message[27];
    /* 0x52 */ u8             unk_52;
    /* 0x53 */ u8             timesMet;
    /* 0x54 */ PackedDateTime lastMet;
    /* 0x5A */ char           unk_5A[0x5C - 0x5A];
    /* 0x5C */ Experience     experience;
    /* 0x68 */ MingleShop     shop;
    /* 0x98 */ u16            pinIcons[6];
    /* 0xA4 */ u16            itemIcons[16];
    /* 0xC4 */ u16            giftIcon;
    /* 0xC8 */ BOOL           hasSellable; // Any of the friend's pins/items has a sale price
} FriendListFriend;                        // Size: 0xCC

typedef struct {
    /* 0x0000 */ FriendListIcon   threads[16]; // Selected friend's shop items
    /* 0x0040 */ FriendListIcon   gift;
    /* 0x0044 */ FriendListIcon*  slots[9];    // [0]-[3]: player, [4]-[7]: partner, [8]: gift
    /* 0x0068 */ FriendListIcon   pins[6];
    /* 0x0080 */ FriendListIcon*  pinSlots[6];
    /* 0x0098 */ FriendListFriend friends[50];
#ifndef REGION_USA
    /* 0x2870 */ FriendListFriend* rowFriends[3]; // JP only: the friends shown in the three list rows
#endif
    /* 0x2870 */ u8                unk_2870;
    /* 0x2871 */ char              unk_2871;
    /* 0x2872 */ u16               flags;
    /* 0x2874 */ u8                mode; // 0: list, 1: my shop, 2: help
    /* 0x2875 */ u8                nextProcess;
    /* 0x2876 */ u8                rowPressed[3];
    /* 0x2879 */ u8                btnPressed;
    /* 0x287A */ u8                arrowPressed[6];
    /* 0x2880 */ u8                iconPressed[3];
    /* 0x2883 */ u8                iconTimer;
    /* 0x2884 */ s16               partner;
    /* 0x2886 */ u16               scroll;
    /* 0x2888 */ u16               lastScroll;
    /* 0x288A */ u16               cursor;
    /* 0x288C */ u32               esperPoints;
    /* 0x2890 */ u16               noiseReportCount;
    /* 0x2892 */ u16               pinsMastered;
    /* 0x2894 */ u16               itemsCollected;
    /* 0x2896 */ char              unk_2896[0x2898 - 0x2896];
    /* 0x2898 */ u32               timeAttackFrames;
    /* 0x289C */ s8                shopIndex;
    /* 0x289D */ s8                shopId;
    /* 0x289E */ s8                clerkId;
    /* 0x289F */ s8                musicId;
    /* 0x28A0 */ u8                badgeSlots;
    /* 0x28A1 */ u8                unk_28A1;
    /* 0x28A2 */ u8                helpButtonPressed[3];
    /* 0x28A5 */ u8                helpPage;
    /* 0x28A6 */ u16               helpOpen;
    /* 0x28A8 */ u16               timer;
    /* 0x28AC */ GlobalFriendData* friendData;
    /* 0x28B0 */ MenuBgResource    resources[8]; // [0]-[3]: sub BG0-BG3, [4]-[7]: main BG0-BG3
} FriendListObject;                              // Size: 0x2990 (JP: 0x299C)

extern const BinIdentifier FriendList_BinIdentifiers[];
extern u16                 FriendList_ShopIds[35];

// FriendListData.c
s32  FriendList_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height);
void FriendList_SetSpriteFrame(Sprite* sprite, s16 frame);
void FriendList_LoadPinData(RawPinData* buffer);
void FriendList_LoadItemData(RawItemData* buffer);
void FriendList_LoadFoodData(RawFoodData* buffer);
void FriendList_LoadTreasureData(RawTreasureData* buffer);
BOOL FriendList_IsMacAddressSet(u8* macAddress);
BOOL FriendList_CheckSellable(FriendListObject* friendList, u16 index, RawPinData* pinData, RawItemData* itemData);
s8   FriendList_GetShopIndex(s8 shopId);
void FriendList_LoadFromSave(FriendListObject* friendList);
void FriendList_WriteToSave(FriendListObject* friendList);
void FriendList_WriteToSaveForShop(FriendListObject* friendList);
s16  FriendList_GetIconAtPoint(s16 x, s16 y);
s16  FriendList_GetPartnerAtPoint(s16 x, s16 y);
s16  FriendList_GetRowAtPoint(s16 x, s16 y);
s16  FriendList_GetRowBtnAtPoint(s16 x, s16 y);
s16  FriendList_IsSellable(FriendListObject* friendList, u16 index);
s32  FriendList_IsPointOnBtn(s16 x, s16 y);
s16  FriendList_GetArrowAtPoint(s16 x, s16 y);
s32  FriendList_IsPointOnSbar(s16 x, s16 y);
s32  FriendList_IsPointOnSbarKnob(s16 x, s16 y, s16 knobX, s16 knobY);
s16  FriendList_GetSbarArrowAtPoint(s16 x, s16 y);
s16  FriendList_GetHelpBtnAtPoint(s16 x, s16 y);
void FriendList_LoadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void FriendList_LoadBgResourceIndexed(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                      s32 screenIndex, s32 palIndex);
void FriendList_LoadBgScreen(MenuBgResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex);
void FriendList_ReleaseBgResource(MenuBgResource* res, s32 engine);
void FriendList_ReleaseBgScreen(MenuBgResource* res, s32 engine);
void FriendList_ReloadBgResource(MenuBgResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void FriendList_ClearBgResource(MenuBgResource* res);
void FriendList_LoadBackgrounds(FriendListObject* friendList);
void FriendList_UpdateBackgrounds(FriendListObject* friendList);
void FriendList_ReleaseBackgrounds(FriendListObject* friendList);

// FriendList.c
void FriendList_ChangePartner(FriendListObject* friendList, u16 partner);

// Tasks
s32  FriendList_textScr_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_btn_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_icon_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_partner_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_sbar_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_numFrdID_CreateTask(TaskPool* pool, s32 dataType, s32 index, FriendListObject* friendList);
s32  FriendList_numDate_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_nameD_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_item_CreateTask(TaskPool* pool, s32 dataType, u16 slot, FriendListObject* friendList);
s32  FriendList_itemCur_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_myShop_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_helpCur_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_pointer_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_textScrU_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_bdgU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, FriendListObject* friendList);
void FriendList_bdgU_ReleaseSprite(TaskPool* pool, s32 taskId);
s32  FriendList_itemU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, FriendListObject* friendList);
void FriendList_itemU_ReleaseSprite(TaskPool* pool, s32 taskId);
s32  FriendList_nameU_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);
s32  FriendList_helpCurU_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList);

#endif // INTERFACE_MENU_FRIENDLIST_H
