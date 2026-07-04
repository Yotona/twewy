#ifndef INTERFACE_MENU_SHOP_H
#define INTERFACE_MENU_SHOP_H

#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct Point {
    s16 x;
    s16 y;
} Point;

typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ u16  unk_02;
    /* 0x04 */ u16  unk_04;
    /* 0x06 */ u16  unk_06;
    /* 0x08 */ u32  unk_08;
    /* 0x0C */ u8   unk_0C;
    /* 0x0D */ u8   unk_0D;
    /* 0x0E */ u16  unk_0E;
    /* 0x10 */ s32  unk_10;
    /* 0x14 */ char unk_14[0x20 - 0x14];
    /* 0x20 */ u32  unk_20;
} ShopItemTableEntry;

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u32 cost;
    /* 0x0C */ u8  unk_0C[0x04];
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 itemID;
    /* 0x18 */ u8  unk_18[0x04];
    /* 0x1C */ u8  ownedCount;
    /* 0x1D */ u8  unk_1D[0x03];
    /* 0x20 */ u8  validFlag;
    /* 0x21 */ u8  unk_21[0x03];
    /* 0x24 */ u16 unk_24;
    /* 0x26 */ u8  unk_26[0x06];
} ShopItemSlot; // Size: 0x2C

typedef struct {
    /* 0x000 */ ShopItemSlot        slots[42];
    /* 0x738 */ ShopItemTableEntry* unk_738[12];
    /* 0x768 */ char                unk_768[0x76E - 0x768];
    /* 0x76E */ u16                 unk_76E;
    /* 0x770 */ char                unk_770[0x776 - 0x770];
    /* 0x776 */ u16                 unk_776;
    /* 0x778 */ u16                 unk_778;
    /* 0x77A */ char                unk_77A[0x788 - 0x77A];
    /* 0x788 */ u16                 unk_788;
    /* 0x78A */ u16                 unk_78A;
    /* 0x78C */ char                unk_78C[0x7B5 - 0x78C];
    /* 0x7B5 */ u8                  unk_7B5;
    /* 0x7B6 */ u8                  unk_7B6;
    /* 0x7B7 */ char                unk_7B7[0x834 - 0x7B7];
    /* 0x834 */ u16                 unk_834;
    /* 0x836 */ u8                  unk_836;
    /* 0x837 */ u8                  unk_837;
    /* 0x838 */ u8                  unk_838;
    /* 0x839 */ u8                  unk_839;
    /* 0x83A */ char                unk_83A[0x840 - 0x83A];
    /* 0x840 */ s16                 unk_840;
    /* 0x842 */ s16                 unk_842;
    /* 0x844 */ u16                 unk_844;
    /* 0x846 */ u16                 unk_846;
    /* 0x848 */ u16                 unk_848;
    /* 0x84A */ u16                 unk_84A;
    /* 0x84C */ u16                 unk_84C;
    /* 0x84E */ u16                 unk_84E;
    /* 0x850 */ u16                 unk_850;
    /* 0x852 */ char                unk_852[0x854 - 0x852];
    /* 0x854 */ u8                  unk_854;
    /* 0x855 */ char                unk_855[0x85E - 0x855];
    /* 0x85E */ u16                 unk_85E;
    /* 0x860 */ char                unk_860[0x864 - 0x860];
    /* 0x864 */ u8                  unk_864[3];
    /* 0x867 */ char                unk_867[0x870 - 0x867];
    /* 0x870 */ u16                 unk_870;
    /* 0x872 */ u16                 unk_872;
    /* 0x874 */ char                unk_874[0x910 - 0x874];
    /* 0x910 */ u32                 unk_910;
    /* 0x914 */ u32                 unk_914;
    /* 0x918 */ char                unk_918[0x938 - 0x918];
    /* 0x938 */ u32                 unk_938;
} ShopObject;

extern BinIdentifier data_ov043_020ca1b0;
extern BinIdentifier data_ov043_020ca1b8;
extern BinIdentifier data_ov043_020ca1c0;
extern BinIdentifier data_ov043_020ca1d8;
extern BinIdentifier data_ov043_020ca1e0;
extern BinIdentifier data_ov043_020ca1e8;
extern BinIdentifier data_ov043_020ca1f0;
extern BinIdentifier data_ov043_020ca1f8;
extern BinIdentifier data_ov043_020ca200;
extern BinIdentifier data_ov043_020ca208;

s32 func_ov043_020b3238(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 func_ov043_020b6968(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_itemCur_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_itemAbl_CreateTask(TaskPool* pool, s32 dataType, s16 itemIndex, ShopObject* shop);
s32 Shop_itemStar_CreateTask(TaskPool* pool, s32 dataType, s16 itemIndex, ShopObject* shop);
s32 Shop_brdLogo_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_numLvU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_item_CreateTask(TaskPool* pool, s32 dataType, u16 itemIndex, ShopObject* shop);
s32 func_ov043_020b4570(TaskPool* arg0, s32 dataType, ShopObject* shop);
s32 func_ov043_020b4ac4(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_numAblU_CreateTask(TaskPool* pool, s32 arg1, ShopObject* shop);
s32 Shop_numFsgU_CreateTask(TaskPool* arg0, s32 arg1, ShopObject* shop);
s32 Shop_fsgGaugeU_CreateTask(TaskPool* pool, s32 arg1, ShopObject* shop);
s32 Shop_textScrU_CreateTask(TaskPool* pool, s32 arg1, void* arg2);

s32 Shop_window1_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_windowU_CreateTask(TaskPool* pool, s32 dataType, s16 arg2, s32 arg3);

s32 Shop_castU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop);
s32 Shop_boardU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop);

#endif // INTERFACE_MENU_SHOP_H