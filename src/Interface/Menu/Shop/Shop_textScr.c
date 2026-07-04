#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include "common_data.h"

#define SHOP_TEXTSCR_LAYOUT_SIZE 17
#define SHOP_TEXTSCR_LAYOUT_A    (0 * SHOP_TEXTSCR_LAYOUT_SIZE)
#define SHOP_TEXTSCR_LAYOUT_B    (1 * SHOP_TEXTSCR_LAYOUT_SIZE)
#define SHOP_TEXTSCR_LAYOUT_C    (2 * SHOP_TEXTSCR_LAYOUT_SIZE)
#define SHOP_TEXTSCR_LAYOUT_D    (3 * SHOP_TEXTSCR_LAYOUT_SIZE)

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u16 unk_18;
    /* 0x1A */ u16 unk_1A;
} Shop_textScr_ConstText;

typedef struct {
    /* 0x00 */ void*         owner;
    /* 0x04 */ UnkOv31Struct unk_004[17];
} Shop_textScr; // Size: 0x840

typedef struct {
    /* 0x00 */ u8  unk_00[0x02];
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u8  unk_04;
    /* 0x05 */ u8  unk_05;
    /* 0x06 */ u8  unk_06[0x08];
    /* 0x0E */ u16 unk_0E;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ u8  unk_18[0x0C];
    /* 0x24 */ u8  unk_24;
    /* 0x25 */ u8  unk_25[0x0B];
    /* 0x30 */ u8  unk_30;
    /* 0x31 */ u8  unk_31[0x4F];
} Shop_textScr_PinData; // Size: 0x80

typedef struct {
    /* 0x00 */ u8  unk_00[0x02];
    /* 0x02 */ u8  unk_02;
    /* 0x03 */ u8  unk_03;
    /* 0x04 */ u8  unk_04[0x04];
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u8  unk_12[0x6E];
} Shop_textScr_ThreadData; // Size: 0x80

typedef struct {
    /* 0x00 */ u8  unk_00[0x02];
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u8  unk_04[0x04];
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8  unk_0E[0x32];
} Shop_textScr_FoodData; // Size: 0x40

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_textScr_Args; // Size: 0x8

extern void OS_WaitForever(void);
extern u16* func_02024434(s32);
extern void func_ov031_0210a5fc(u16*, const void*, ...);
extern s32  func_ov031_0210bde8(UnkOv31Struct*, u16*, s32, s32, u32);
extern s32  func_ov031_0210be18(UnkOv31Struct*, u32, u32, s32);
extern void func_ov031_0210ab54(UnkOv31Struct*, s32, s32);
extern void func_ov043_020af4c4(void*, s32);
extern void func_ov043_020af4f0(void*, s32);
extern void func_ov043_020af51c(void*, s32);
extern u8   func_ov043_020b0088(u16);
extern void func_ov043_020b288c(void*, s32, s32, s32, s32, s32);

static s32 func_ov043_020b6864(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b688c(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b68e8(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b68f0(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b6920(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static Point data_ov043_020cc95c[17] = {
    {0x2B, 0x71},
    { 0x6, 0x83},
    {0x75, 0x8F},
    {0x94, 0x9B},
    {0x94, 0xA7},
    {   0,    0},
    {   0,    0},
    {   0,    0},
    {   0,    0},
    {   0,    0},
    {0x32, 0x8D},
    {0x84, 0x8D},
    {0xD6, 0x8D},
    {0x84, 0x99},
    {0xD6, 0x99},
    {0x84, 0xA5},
    {0xD6, 0xA5},
};

static const Shop_textScr_ConstText data_ov043_020ca5f4;

static const TaskHandle data_ov043_020ca610 = {"Tsk_Shop_textScr", func_ov043_020b6920, 0x840};

u16 func_ov043_020b5198(u16 arg0, s16 arg1, s32 arg2) {
    return (u16)((arg1 * (arg2 - 1)) + arg0);
}

u16 func_ov043_020b51ac(u16 arg0, s16 arg1, s32 arg2) {
    return (u16)((arg1 * (arg2 - 1)) + arg0);
}

void func_ov043_020b51c0(u16 arg0, u16* arg1) {
    if (arg0 >= 6040) {
        arg1[0] = 99;
        arg1[1] = 9;
    } else {
        arg1[0] = arg0 / 60;
        arg1[1] = (arg0 % 60) / 10;
    }
}

void func_ov043_020b525c(Shop_textScr* textScr) {
    for (s32 i = 0; i < 17; i++) {
        func_ov031_0210aa94(&textScr->unk_004[i]);
        func_ov031_0210ab34(&textScr->unk_004[i], 0xE);
        func_ov031_0210ab54(&textScr->unk_004[i], 1, 0);
    }
}

void func_ov043_020b52ac(Shop_textScr* textScr, s32 arg1, s32 arg2) {
    ShopObject* shop = textScr->owner;
    u16         buffer[64];

    for (u16 i = 0; i < 17; i++) {
        func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[i].x, data_ov043_020cc95c[i].y);
    }

    for (u16 i = 0; i < 3; i++) {
        s32           slotIndex       = shop->unk_848 + i;
        ShopItemSlot* slot            = (ShopItemSlot*)((u8*)shop + (slotIndex * sizeof(ShopItemSlot)));
        u16           itemID          = slot->itemID;
        const u16     categoryMsgBase = 0x27C1;
        const u16     foodMsgBase     = 0x2A6D;
        const u16     threadMsgBase   = 0x2C14;
        const u16     swagMsgBase     = 0x34BE;

        if (itemID == 0xFFFF || slot->validFlag == 0) {
            continue;
        }

        s32 itemIndex       = Inventory_GetCategorizedIndex(itemID);
        u32 category        = Inventory_GetCategory(itemID);
        u8  maxOwned        = func_ov043_020b0088(itemID);
        u16 categoryMessage = categoryMsgBase;

        if (category == ITEM_CATEGORY_THREAD) {
            categoryMessage = threadMsgBase;
        } else if (category == ITEM_CATEGORY_FOOD) {
            categoryMessage = foodMsgBase;
        } else if (category == ITEM_CATEGORY_SWAG) {
            categoryMessage = swagMsgBase;
        }

        u16* nameText     = func_ov031_0210b698(&textScr->unk_004[i * 3 + 1], 0x340B);
        u32* categoryText = func_ov031_0210b698(&textScr->unk_004[i * 3 + 1], (categoryMessage + itemIndex));

        func_ov031_0210a5fc(buffer, nameText, categoryText);
        func_ov031_0210ab3c(&textScr->unk_004[i * 3 + 1], 1, 0xFFFF);
        func_ov031_0210bde8(&textScr->unk_004[i * 3 + 1], buffer, arg2 + 4, arg1 + 4, 0);
        Mem_Free(&gDebugHeap, nameText);
        Mem_Free(&gDebugHeap, categoryText);

        u16* countText = func_ov031_0210b698(&textScr->unk_004[i * 3 + 2], 0x340C);

        func_ov031_0210a5fc(buffer, countText, slot->ownedCount);
        func_ov031_0210ab3c(&textScr->unk_004[i * 3 + 2], 1, 0xFFFF);
        func_ov031_0210bde8(&textScr->unk_004[i * 3 + 2], buffer, arg2 + 4, arg1 + 4, 0);
        Mem_Free(&gDebugHeap, countText);

        if (slot->ownedCount > maxOwned) {
            countText = func_ov031_0210b698(&textScr->unk_004[i * 3 + 3], 0x340E);
        } else {
            countText = func_ov031_0210b698(&textScr->unk_004[i * 3 + 3], 0x340D);
        }
        func_ov031_0210a5fc(buffer, countText, maxOwned);
        func_ov031_0210ab3c(&textScr->unk_004[i * 3 + 3], 1, 0xFFFF);
        func_ov031_0210bde8(&textScr->unk_004[i * 3 + 3], buffer, arg2 + 4, arg1 + 4, 0);
        Mem_Free(&gDebugHeap, countText);
    }
}

void func_ov043_020b55a4(Shop_textScr* textScr, s32 arg1, s32 arg2, s32 arg3) {
    ShopObject*          shop = textScr->owner;
    Shop_textScr_PinData raw;
    u16                  buffer[64];

    func_ov043_020af4c4(&raw, arg3);

    switch (shop->unk_854) {
        case 0: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_A + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_A + i].y);
            }

            if (raw.unk_05 <= 0x0C) {
                func_ov031_0210b630(&textScr->unk_004[1], (u16)(raw.unk_05 + 0x2A21));
                func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x6B);
                func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);
            } else {
                func_ov031_0210b630(&textScr->unk_004[1], 0x2A2F);
                func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x6B);
                func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);
            }

            if (raw.unk_02 != 0xFFFF) {
                func_ov031_0210b630(&textScr->unk_004[2], (u16)(raw.unk_02 + 0x28F1));
                func_ov031_0210ab3c(&textScr->unk_004[2], 0, 0x84);
                func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
            } else if (raw.unk_24 == 0xFF) {
                func_ov031_0210b630(&textScr->unk_004[2], 0x3339);
                func_ov031_0210ab3c(&textScr->unk_004[2], 0, 0x84);
                func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
            } else {
                func_ov031_0210b630(&textScr->unk_004[2], (u16)(raw.unk_24 + 0x22C3));
                func_ov031_0210ab3c(&textScr->unk_004[2], 0, 0x84);
                func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
            }

            {
                u16* specialName = func_02024434(arg3);
                if (specialName != NULL) {
                    u16* fmt = func_ov031_0210b698(&textScr->unk_004[3], 0x3427);
                    func_ov031_0210a5fc(buffer, fmt, specialName);
                    func_ov031_0210ab3c(&textScr->unk_004[3], 0, 0x65);
                    func_ov031_0210bde8(&textScr->unk_004[3], buffer, arg2 + 4, arg1 + 4, 0);
                    Mem_Free(&gDebugHeap, fmt);
                } else if (raw.unk_0E == 0) {
                    func_ov031_0210b630(&textScr->unk_004[3], 0x3357);
                    func_ov031_0210ab3c(&textScr->unk_004[3], 0, 0x65);
                    func_ov031_0210be18(&textScr->unk_004[3], arg2 + 4, arg1 + 4, 0);
                } else {
                    u16 templateVals[5];

                    templateVals[0] = data_ov043_020ca5f4.unk_12;
                    templateVals[1] = data_ov043_020ca5f4.unk_14;
                    templateVals[2] = data_ov043_020ca5f4.unk_16;
                    templateVals[3] = data_ov043_020ca5f4.unk_18;
                    templateVals[4] = data_ov043_020ca5f4.unk_1A;

                    u16* fmt = func_ov031_0210b698(&textScr->unk_004[3], 0x3322);

                    func_ov031_0210a5fc(buffer, templateVals, fmt, func_ov043_020b5198(raw.unk_0E, raw.unk_10, 1));
                    func_ov031_0210ab3c(&textScr->unk_004[3], 0, 0x65);
                    func_ov031_0210bde8(&textScr->unk_004[3], buffer, arg2 + 4, arg1 + 4, 0);
                    Mem_Free(&gDebugHeap, fmt);
                }
            }

            if (raw.unk_12 == 0) {
                func_ov031_0210b630(&textScr->unk_004[4], 0x3357);
                func_ov031_0210ab3c(&textScr->unk_004[4], 0, 0x65);
                func_ov031_0210be18(&textScr->unk_004[4], arg2 + 4, arg1 + 4, 0);
            } else if ((u16)(raw.unk_12 - 1) <= 1) {
                u16  total = func_ov043_020b51ac(raw.unk_14, raw.unk_16, 1);
                u16* fmt;

                if (total == 1) {
                    fmt = func_ov031_0210b698(&textScr->unk_004[4], 0x335A);
                } else {
                    fmt = func_ov031_0210b698(&textScr->unk_004[4], 0x3359);
                }

                func_ov031_0210a5fc(buffer, fmt, total);
                func_ov031_0210ab3c(&textScr->unk_004[4], 0, 0x65);
                func_ov031_0210bde8(&textScr->unk_004[4], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, fmt);
            } else {
                u16* fmt      = func_ov031_0210b698(&textScr->unk_004[4], 0x3358);
                u16  money[2] = {0};

                func_ov043_020b51c0((u32)func_ov043_020b51ac(raw.unk_14, raw.unk_16, 1), money);
                func_ov031_0210a5fc(buffer, fmt, money[0], money[1]);
                func_ov031_0210ab3c(&textScr->unk_004[4], 0, 0x65);
                func_ov031_0210bde8(&textScr->unk_004[4], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, fmt);
            }
        } break;

        case 1: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            if (raw.unk_30 != 0xFF) {
                func_ov031_0210b630(&textScr->unk_004[1], (u16)(raw.unk_30 + 0x333F));
            } else {
                func_ov031_0210b630(&textScr->unk_004[1], 0x3339);
            }
            func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
            func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);

            func_ov031_0210b630(&textScr->unk_004[2], (u16)(arg3 + 0x2430));
            func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
            func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
        } break;

        case 2: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            func_ov031_0210b630(&textScr->unk_004[2], (u16)(arg3 + 0x2560));
            func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
            func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
        } break;

        case 3: {
            u16 templateVals[2];

            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            templateVals[0] = data_ov043_020ca5f4.unk_00;
            templateVals[1] = data_ov043_020ca5f4.unk_02;

            func_ov031_0210b630(&textScr->unk_004[2], 0x2690);
            func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
            func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);

            for (u16 i = 0; i < 7; i++) {
                ShopItemSlot* slot = (ShopItemSlot*)((u8*)shop + ((shop->unk_848 + i) * sizeof(ShopItemSlot)));
                func_ov031_0210a5fc(buffer, templateVals, slot->unk_24);
                func_ov031_0210ab3c(&textScr->unk_004[i + 10], 0, 0x14);
                func_ov031_0210ab34(&textScr->unk_004[i + 10], 0xC);
                func_ov031_0210bde8(&textScr->unk_004[i + 10], buffer, arg2 + 4, arg1 + 4, 0);
            }
        } break;

        case 4:
            func_ov043_020b52ac(textScr, arg1, arg2);
            break;
    }

    func_ov031_0210b630(&textScr->unk_004[0], (u16)(arg3 + 0x27C1));
    func_ov031_0210ab3c(&textScr->unk_004[0], 0, 0x72);
    func_ov031_0210be18(&textScr->unk_004[0], arg2 + 4, arg1 + 4, 0);
}

void func_ov043_020b5d18(Shop_textScr* textScr, s32 arg1, s32 arg2, s32 arg3) {
    ShopObject*             shop = textScr->owner;
    Shop_textScr_ThreadData raw;
    u16                     buffer[64];

    func_ov043_020af4f0(&raw, arg3);

    switch (shop->unk_854) {
        case 0: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_D + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_D + i].y);
            }

            func_ov031_0210b630(&textScr->unk_004[1], (u16)(raw.unk_02 + 0x2A21));
            func_ov031_0210ab3c(&textScr->unk_004[1], 0, 107);
            func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);

            {
                u16  templateVals[4];
                u16* fmt;

                templateVals[0] = data_ov043_020ca5f4.unk_0A;
                templateVals[1] = data_ov043_020ca5f4.unk_0C;
                templateVals[2] = data_ov043_020ca5f4.unk_0E;
                templateVals[3] = data_ov043_020ca5f4.unk_10;

                fmt = func_ov031_0210b698(&textScr->unk_004[2], 0x3328);
                func_ov031_0210a5fc(buffer, templateVals, fmt, raw.unk_08);
                func_ov031_0210ab3c(&textScr->unk_004[2], 0, 101);
                func_ov031_0210bde8(&textScr->unk_004[2], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, fmt);
            }

            if (raw.unk_0C == 0) {
                func_ov031_0210b630(&textScr->unk_004[3], 0x3339);
                func_ov031_0210ab3c(&textScr->unk_004[3], 0, 101);
                func_ov031_0210be18(&textScr->unk_004[3], arg2 + 4, arg1 + 4, 0);
            } else {
                u16* textFmt = func_ov031_0210b698(&textScr->unk_004[3], 0x3322);
                u16* valueFmt;
                s16  value = raw.unk_0C;

                if (value > 0) {
                    valueFmt = func_ov031_0210b698(&textScr->unk_004[3], 0x333B);
                } else {
                    valueFmt = func_ov031_0210b698(&textScr->unk_004[3], 0x333C);
                    value    = -value;
                }

                func_ov031_0210a5fc(buffer, valueFmt, textFmt, value);
                func_ov031_0210ab3c(&textScr->unk_004[3], 0, 101);
                func_ov031_0210bde8(&textScr->unk_004[3], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, valueFmt);
                Mem_Free(&gDebugHeap, textFmt);
            }

            if (raw.unk_0A == 0) {
                func_ov031_0210b630(&textScr->unk_004[4], 0x3339);
                func_ov031_0210ab3c(&textScr->unk_004[4], 0, 101);
                func_ov031_0210be18(&textScr->unk_004[4], arg2 + 4, arg1 + 4, 0);
            } else {
                s32  value = raw.unk_0A;
                u16* valueFmt;
                u16* textFmt = func_ov031_0210b698(&textScr->unk_004[4], 0x3323);

                if (value > 0) {
                    valueFmt = func_ov031_0210b698(&textScr->unk_004[4], 0x333B);
                } else {
                    valueFmt = func_ov031_0210b698(&textScr->unk_004[4], 0x333C);
                    value    = -value;
                }

                func_ov031_0210a5fc(buffer, valueFmt, textFmt, value);
                func_ov031_0210ab3c(&textScr->unk_004[4], 0, 0x65);
                func_ov031_0210bde8(&textScr->unk_004[4], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, valueFmt);
                Mem_Free(&gDebugHeap, textFmt);
            }

            if (raw.unk_0E == 0) {
                func_ov031_0210b630(&textScr->unk_004[5], 0x3339);
                func_ov031_0210be18(&textScr->unk_004[5], arg2 + 4, arg1 + 4, 0);
                func_ov031_0210ab3c(&textScr->unk_004[5], 0, 0x65);
            } else {
                s32  value = raw.unk_0E;
                u16* valueFmt;
                u16* textFmt = func_ov031_0210b698(&textScr->unk_004[5], 0x3324);

                if (value > 0) {
                    valueFmt = func_ov031_0210b698(&textScr->unk_004[5], 0x333B);
                } else {
                    valueFmt = func_ov031_0210b698(&textScr->unk_004[5], 0x333C);
                    value    = -value;
                }

                func_ov031_0210a5fc(buffer, valueFmt, textFmt, (u16)value);
                func_ov031_0210ab3c(&textScr->unk_004[5], 0, 0x65);
                func_ov031_0210bde8(&textScr->unk_004[5], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, valueFmt);
                Mem_Free(&gDebugHeap, textFmt);
            }
        } break;

        case 1: {
            u16 templateVals[3];

            templateVals[0] = data_ov043_020ca5f4.unk_04;
            templateVals[1] = data_ov043_020ca5f4.unk_06;
            templateVals[2] = data_ov043_020ca5f4.unk_08;

            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            {
                u16* fmtA = func_ov031_0210b698(&textScr->unk_004[1], 0x335F);
                u16* fmtB = func_ov031_0210b698(&textScr->unk_004[1], (u16)(raw.unk_03 + 0x3360));

                func_ov031_0210a5fc(buffer, templateVals, fmtA, fmtB);
                func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
                func_ov031_0210bde8(&textScr->unk_004[1], buffer, arg2 + 4, arg1 + 4, 0);
                Mem_Free(&gDebugHeap, fmtA);
                Mem_Free(&gDebugHeap, fmtB);
            }

            func_ov031_0210b630(&textScr->unk_004[2], (u16)(arg3 + 0x2AFC));
            func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
            func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
        } break;

        case 2: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            ShopItemSlot* slot = (ShopItemSlot*)((u8*)shop + (shop->unk_848 * sizeof(ShopItemSlot)));
            if (slot->validFlag == 1) {
                func_ov031_0210b630(&textScr->unk_004[1], (u16)(raw.unk_10 + 0x22C3));
                func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
                func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);

                func_ov031_0210b630(&textScr->unk_004[2], (u16)(raw.unk_10 + 0x219B));
                func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
                func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
            } else {
                func_ov031_0210b630(&textScr->unk_004[1], 0x33E0);
                func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
                func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);

                func_ov031_0210b630(&textScr->unk_004[2], 0x33E1);
                func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
                func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
            }
        } break;

        case 4:
            func_ov043_020b52ac(textScr, arg1, arg2);
            break;
    }

    func_ov031_0210b630(&textScr->unk_004[0], (u16)(arg3 + 0x2C14));
    func_ov031_0210ab3c(&textScr->unk_004[0], 0, 0x72);
    func_ov031_0210be18(&textScr->unk_004[0], arg2 + 4, arg1 + 4, 0);
}

void func_ov043_020b63fc(Shop_textScr* textScr, s32 arg1, s32 arg2, s32 arg3) {
    ShopObject* shop = textScr->owner;

    Shop_textScr_FoodData raw;

    func_ov043_020af51c(&raw, arg3);

    switch (shop->unk_854) {
        case 0: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            func_ov031_0210b630(&textScr->unk_004[1], 0x3366);
            func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
            func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);

            func_ov031_0210b630(&textScr->unk_004[2], (u16)(arg3 + 0x2A43));
            func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
            func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
        } break;

        case 1: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            u16* fmt;
            if (raw.unk_02 == 1) {
                fmt = func_ov031_0210b698(&textScr->unk_004[1], 0x33F3);
            } else {
                fmt = func_ov031_0210b698(&textScr->unk_004[1], 0x33F2);
            }

            u16 buffer[64];
            func_ov031_0210a5fc(buffer, fmt, raw.unk_02);
            func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
            func_ov031_0210bde8(&textScr->unk_004[1], buffer, arg2 + 4, arg1 + 4, 0);
            Mem_Free(&gDebugHeap, fmt);

            u16* text = func_ov031_0210b698(&textScr->unk_004[2], raw.unk_08 + 0x33EC);
            func_ov031_0210a5fc(buffer, text, raw.unk_0C, raw.unk_0A);
            func_ov031_0210bde8(&textScr->unk_004[2], buffer, arg2 + 4, arg1 + 4, 0);
            Mem_Free(&gDebugHeap, text);
        } break;

        case 4:
            func_ov043_020b52ac(textScr, arg1, arg2);
            break;
    }

    func_ov031_0210b630(&textScr->unk_004[0], (u16)(arg3 + 0x2A6D));
    func_ov031_0210ab3c(&textScr->unk_004[0], 0, 0x72);
    func_ov031_0210be18(&textScr->unk_004[0], arg2 + 4, arg1 + 4, 0);
}

void func_ov043_020b6668(Shop_textScr* textScr, s32 arg1, s32 arg2, s32 arg3) {
    ShopObject* shop = textScr->owner;

    switch (shop->unk_854) {
        case 0: {
            for (u16 i = 0; i < 17; i++) {
                func_ov031_0210ab28(&textScr->unk_004[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                                    data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            func_ov031_0210b630(&textScr->unk_004[1], 0x3367);
            func_ov031_0210ab3c(&textScr->unk_004[1], 0, 0x95);
            func_ov031_0210be18(&textScr->unk_004[1], arg2 + 4, arg1 + 4, 0);

            func_ov031_0210b630(&textScr->unk_004[2], (u16)(arg3 + 0x3428));
            func_ov031_0210ab3c(&textScr->unk_004[2], 1, 0xFFFF);
            func_ov031_0210be18(&textScr->unk_004[2], arg2 + 4, arg1 + 4, 0);
        } break;

        case 4:
            func_ov043_020b52ac(textScr, arg1, arg2);
            break;

        default:
            break;
    }

    func_ov031_0210b630(&textScr->unk_004[0], (u16)(arg3 + 0x34BE));
    func_ov031_0210ab3c(&textScr->unk_004[0], 0, 0x72);
    func_ov031_0210be18(&textScr->unk_004[0], arg2 + 4, arg1 + 4, 0);
}

// Nonmatching
void func_ov043_020b678c(Shop_textScr* textScr) {
    ShopObject* shop = textScr->owner;

    s32 arg2 = shop->unk_914;
    s32 arg1 = shop->unk_910;

    if (arg2 == 0 || arg1 == 0) {
        OS_WaitForever();
    }

    ShopItemSlot* selectedSlot  = (ShopItemSlot*)((u8*)shop + (shop->unk_848 * sizeof(ShopItemSlot)));
    u16           itemID        = selectedSlot->itemID;
    u16           categoryIndex = Inventory_GetCategorizedIndex(itemID);
    ItemCategory  category      = Inventory_GetCategory(itemID);

    if (itemID == 0xFFFF) {
        return;
    }

    switch (category) {
        case ITEM_CATEGORY_PIN:
            func_ov043_020b55a4(textScr, arg1, arg2, categoryIndex);
            break;
        case ITEM_CATEGORY_THREAD:
            func_ov043_020b5d18(textScr, arg1, arg2, categoryIndex);
            break;
        case ITEM_CATEGORY_FOOD:
            func_ov043_020b63fc(textScr, arg1, arg2, categoryIndex);
            break;
        case ITEM_CATEGORY_SWAG:
            func_ov043_020b6668(textScr, arg1, arg2, categoryIndex);
            break;
        default:
            break;
    }
}

static s32 func_ov043_020b6864(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_textScr*      textScr = arg1->data;
    Shop_textScr_Args* args    = arg2;

    textScr->owner = args->shop;
    func_ov043_020b525c(textScr);
    func_ov043_020b678c(textScr);
    return 1;
}

static s32 func_ov043_020b688c(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_textScr* textScr = arg1->data;
    ShopObject*   shop    = textScr->owner;

    if ((shop->unk_834 & 0x40) != 0) {
        shop->unk_834 = (u16)(shop->unk_834 & ~0x40);
        func_ov043_020b288c((u8*)shop + 0x900, 0, 1, 0, 0xF, 1);
        func_ov043_020b678c(textScr);
    }

    return 1;
}

static s32 func_ov043_020b68e8(TaskPool* arg0, Task* arg1, void* arg2) {
    return 1;
}

static s32 func_ov043_020b68f0(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_textScr* textScr = arg1->data;

    for (s32 i = 0; i < 17; i++) {
        func_ov031_0210aabc(&textScr->unk_004[i]);
    }

    return 1;
}

static s32 func_ov043_020b6920(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = func_ov043_020b6864,
        .update     = func_ov043_020b688c,
        .render     = func_ov043_020b68e8,
        .cleanup    = func_ov043_020b68f0,
    };
    return stages.iter[stage](pool, task, args);
}

s32 func_ov043_020b6968(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_textScr_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &data_ov043_020ca610, NULL, 0, NULL, &args);
}
