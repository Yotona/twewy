#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"
#include "common_data.h"

#define SHOP_TEXTSCR_LAYOUT_SIZE 17
#define SHOP_TEXTSCR_LAYOUT_A    (0 * SHOP_TEXTSCR_LAYOUT_SIZE)
#define SHOP_TEXTSCR_LAYOUT_B    (1 * SHOP_TEXTSCR_LAYOUT_SIZE)
#define SHOP_TEXTSCR_LAYOUT_C    (2 * SHOP_TEXTSCR_LAYOUT_SIZE)
#define SHOP_TEXTSCR_LAYOUT_D    (3 * SHOP_TEXTSCR_LAYOUT_SIZE)

typedef struct {
    /* 0x00 */ void*   owner;
    /* 0x04 */ SysFont fonts[17];
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
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ u8  unk_10;
    /* 0x11 */ u8  unk_11[0x18 - 0x11];
} Shop_textScr_ThreadData; // Size: 0x18

typedef struct {
    /* 0x00 */ u8  unk_00[0x02];
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u8  unk_04[0x04];
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8  unk_0E[0x14 - 0x0E];
} Shop_textScr_FoodData; // Size: 0x14

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_textScr_Args; // Size: 0x8

extern void OS_WaitForever(void);
extern s32  func_02024434(s32);
extern void func_ov043_020af4c4(void*, s32);
extern void func_ov043_020af4f0(void*, s32);
extern void func_ov043_020af51c(void*, s32);
extern u16  func_ov043_020b0088(u16);
extern void func_ov043_020b288c(void*, s32, s32, s32, s32, s32);

static s32 func_ov043_020b6864(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b688c(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b68e8(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b68f0(TaskPool* arg0, Task* arg1, void* arg2);
static s32 func_ov043_020b6920(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static Point data_ov043_020cc95c[68] = {
    // SHOP_TEXTSCR_LAYOUT_A
    { 43, 113},
    {  6, 131},
    {117, 143},
    {148, 155},
    {148, 167},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    { 50, 141},
    {132, 141},
    {214, 141},
    {132, 153},
    {214, 153},
    {132, 165},
    {214, 165},
    // SHOP_TEXTSCR_LAYOUT_B
    { 43, 113},
    { 99, 129},
    {  9, 141},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    { 50, 141},
    {132, 141},
    {214, 141},
    {132, 153},
    {214, 153},
    {132, 165},
    {214, 165},
    // SHOP_TEXTSCR_LAYOUT_C
    { 43, 113},
    {  9, 141},
    {130, 141},
    {160, 141},
    {  9, 154},
    {130, 154},
    {160, 154},
    {  9, 167},
    {130, 167},
    {160, 167},
    { 50, 141},
    {132, 141},
    {214, 141},
    {132, 153},
    {214, 153},
    {132, 165},
    {214, 165},
    // SHOP_TEXTSCR_LAYOUT_D
    { 43, 113},
    {  6, 131},
    {148, 131},
    {148, 143},
    {148, 155},
    {148, 167},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    {  0,   0},
    { 50, 141},
    {132, 141},
    {214, 141},
    {132, 153},
    {214, 153},
    {132, 165},
    {214, 165},
};

static const TaskHandle data_ov043_020ca610 = {"Tsk_Shop_textScr", func_ov043_020b6920, 0x840};

u16 func_ov043_020b5198(u16 arg0, s16 arg1, s32 arg2) {
    return (u16)((arg1 * (arg2 - 1)) + arg0);
}

u16 func_ov043_020b51ac(u16 arg0, s16 arg1, s32 arg2) {
    return (u16)((arg1 * (arg2 - 1)) + arg0);
}

// Nonmatching: regswap
// Splits a duration in frames into whole seconds and tenths of a second.
// The clamp writes are dead: the original falls through and overwrites them.
void func_ov043_020b51c0(u16 frames, u16* out) {
    if (frames >= 6040) {
        out[0] = 99;
        out[1] = 9;
    }

    u16 remainder  = frames % 60;
    u16 hundredths = remainder * 100 / 60;

    out[0] = frames / 60;
    out[1] = hundredths / 10;
}

void func_ov043_020b525c(Shop_textScr* textScr) {
    for (s32 i = 0; i < 17; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetColor(&textScr->fonts[i], 14);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
    }
}

// Nonmatching: regswap
// Lists the items (and counts) that a trade-in slot asks for, one row of three fonts per entry.
void func_ov043_020b52ac(Shop_textScr* textScr, u16* charData, u16* map) {
    ShopObject* shop = textScr->owner;
    SysCode     buffer[100];

    for (u16 i = 0; i < 17; i++) {
        SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_C + i].x,
                       data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_C + i].y);
    }

    for (u16 i = 0; i < 3; i++) {
        u16          itemID    = shop->slots[shop->unk_848].tradeInItemIDs[i];
        s32          itemIndex = Inventory_GetCategorizedIndex(itemID);
        ItemCategory category  = Inventory_GetCategory(itemID);
        u8           required  = shop->slots[shop->unk_848].tradeInCounts[i];
        u8           owned     = func_ov043_020b0088(itemID);

        if (required == 0) {
            continue;
        }

        u16 nameBase;
        if (category == ITEM_CATEGORY_PIN) {
            nameBase = 10177; // "Ice Blow"...
        } else if (category == ITEM_CATEGORY_THREAD) {
            nameBase = 11284; // "M Cap"...
        } else if (category == ITEM_CATEGORY_FOOD) {
            nameBase = 10861; // "Hamburger"...
        } else {
            nameBase = 13502; // "Colorful Thread"...
        }

        s32 row = i * 3;

        u16      nameRow = row + 1;
        SysCode* fmt     = SysFont_GetMsgBuf(&textScr->fonts[nameRow], 13323); // "<c6><str><cE>"
        SysCode* name    = SysFont_GetMsgBuf(&textScr->fonts[nameRow], nameBase + itemIndex);

        SysFont_Format(buffer, fmt, name);
        SysFont_SetHAlign(&textScr->fonts[nameRow], 1, SYSFONT_NO_LIMIT);
        SysFont_DrawToScreen(&textScr->fonts[nameRow], buffer, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, name);

        u16      requiredRow = row + 2;
        SysCode* requiredFmt = SysFont_GetMsgBuf(&textScr->fonts[requiredRow], 13324); // "? <u32>"

        SysFont_Format(buffer, requiredFmt, required);
        SysFont_SetHAlign(&textScr->fonts[requiredRow], 1, SYSFONT_NO_LIMIT);
        SysFont_DrawToScreen(&textScr->fonts[requiredRow], buffer, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, requiredFmt);

        u16      ownedRow = (i + 1) * 3;
        SysCode* ownedFmt;

        if (required > owned) {
            ownedFmt = SysFont_GetMsgBuf(&textScr->fonts[ownedRow], 13326); // ". . . <cC>You own: <u32>"
        } else {
            ownedFmt = SysFont_GetMsgBuf(&textScr->fonts[ownedRow], 13325); // ". . . You own: <u32>"
        }
        SysFont_Format(buffer, ownedFmt, owned);
        SysFont_SetHAlign(&textScr->fonts[ownedRow], 1, SYSFONT_NO_LIMIT);
        SysFont_DrawToScreen(&textScr->fonts[ownedRow], buffer, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, ownedFmt);
    }
}

// Nonmatching: stack layout
void func_ov043_020b55a4(Shop_textScr* textScr, u16* charData, u16* map, s32 itemIndex) {
    ShopObject*          shop = textScr->owner;
    Shop_textScr_PinData raw;

    func_ov043_020af4c4(&raw, itemIndex);

    switch (shop->unk_854) {
        case 0: {
            SysCode buffer[40];

            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_A + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_A + i].y);
            }

            if (raw.unk_05 <= 0x0C) {
                SysFont_SetMsg(&textScr->fonts[1], raw.unk_05 + 10785); // "Mus Rattus"...
                SysFont_SetHAlign(&textScr->fonts[1], 0, 107);
                SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);
            } else {
                SysFont_SetMsg(&textScr->fonts[1], 10799); // "Unbranded"
                SysFont_SetHAlign(&textScr->fonts[1], 0, 107);
                SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);
            }

            if (raw.unk_02 != 0xFFFF) {
                SysFont_SetMsg(&textScr->fonts[2], raw.unk_02 + 10481); // "<ic_ne>Spark Core<ic_ci>"...
                SysFont_SetHAlign(&textScr->fonts[2], 0, 132);
                SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
            } else if (raw.unk_24 == 0xFF) {
                SysFont_SetMsg(&textScr->fonts[2], 13113); // "â€•â€??"
                SysFont_SetHAlign(&textScr->fonts[2], 0, 132);
                SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
            } else {
                SysFont_SetMsg(&textScr->fonts[2], raw.unk_24 + 8899); // "<ic_ab>Mother Lode"...
                SysFont_SetHAlign(&textScr->fonts[2], 0, 132);
                SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
            }

            {
                s32 recoveryPercent = func_02024434(itemIndex);
                if (recoveryPercent != 0) {
                    SysCode* fmt = SysFont_GetMsgBuf(&textScr->fonts[3], 13351); // "Recovery <cC><u32><cE>%"
                    SysFont_Format(buffer, fmt, recoveryPercent);
                    SysFont_SetHAlign(&textScr->fonts[3], 0, 101);
                    SysFont_DrawToScreen(&textScr->fonts[3], buffer, map + 2, charData + 2, 0);
                    Mem_Free(&gDebugHeap, fmt);
                } else if (raw.unk_0E == 0) {
                    SysFont_SetMsg(&textScr->fonts[3], 13143); // "â€•â€??"
                    SysFont_SetHAlign(&textScr->fonts[3], 0, 101);
                    SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);
                } else {
                    SysCode fmt[5] = {SYSFONT_CODE_FMT_STR, SYSFONT_GLYPH_SPACE, SYSFONT_CODE_COLOR(12), SYSFONT_CODE_FMT_U32,
                                      SYSFONT_CODE_STR_END};                       // "<str> <cC><u32>"

                    SysCode* label = SysFont_GetMsgBuf(&textScr->fonts[3], 13090); // "Attack"

                    SysFont_Format(buffer, fmt, label, func_ov043_020b5198(raw.unk_0E, raw.unk_10, 1));
                    SysFont_SetHAlign(&textScr->fonts[3], 0, 101);
                    SysFont_DrawToScreen(&textScr->fonts[3], buffer, map + 2, charData + 2, 0);
                    Mem_Free(&gDebugHeap, label);
                }
            }

            if (raw.unk_12 == 0) {
                SysFont_SetMsg(&textScr->fonts[4], 13143); // "â€•â€??"
                SysFont_SetHAlign(&textScr->fonts[4], 0, 101);
                SysFont_DrawCurrentToScreen(&textScr->fonts[4], map + 2, charData + 2, 0);
            } else if ((u16)(raw.unk_12 - 1) <= 1) {
                u16      total = func_ov043_020b51ac(raw.unk_14, raw.unk_16, 1);
                SysCode* fmt;

                if (total == 1) {
                    fmt = SysFont_GetMsgBuf(&textScr->fonts[4], 13146); // "Lasts <cC><u32><cE> use"
                } else {
                    fmt = SysFont_GetMsgBuf(&textScr->fonts[4], 13145); // "Lasts <cC><u32><cE> uses"
                }

                SysFont_Format(buffer, fmt, total);
                SysFont_SetHAlign(&textScr->fonts[4], 0, 101);
                SysFont_DrawToScreen(&textScr->fonts[4], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, fmt);
            } else {
                SysCode* fmt         = SysFont_GetMsgBuf(&textScr->fonts[4], 13144); // "Lasts <cC><u32>.<u32><cE>s"
                u16      duration[2] = {0};

                func_ov043_020b51c0((u32)func_ov043_020b51ac(raw.unk_14, raw.unk_16, 1), duration);
                SysFont_Format(buffer, fmt, duration[0], duration[1]);
                SysFont_SetHAlign(&textScr->fonts[4], 0, 101);
                SysFont_DrawToScreen(&textScr->fonts[4], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, fmt);
            }
        } break;

        case 1: {
            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            if (raw.unk_30 != 0xFF) {
                SysFont_SetMsg(&textScr->fonts[1], raw.unk_30 + 13119); // "Touch the pin"...
            } else {
                SysFont_SetMsg(&textScr->fonts[1], 13113);              // "â€•â€??"
            }
            SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
            SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

            SysFont_SetMsg(&textScr->fonts[2], itemIndex + 9264); // "<cC>Slash vertically up on empty space<c"...
            SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
            SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
        } break;

        case 2: {
            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            SysFont_SetMsg(&textScr->fonts[2], itemIndex + 9568); // "Growth Bonus:   Power:<cC> Y    <cE>Uses"...
            SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
            SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
        } break;

        case 3: {
            SysCode buffer[20];
            SysCode fmt[2] = {SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END}; // "<u32>"

            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            SysFont_SetMsg(&textScr->fonts[2], 9872); // "Weight:                     Spin:       "...
            SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
            SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);

            for (u16 i = 0; i < 7; i++) {
                ShopItemSlot* slot = &shop->slots[shop->unk_848 + i];
                SysFont_Format(buffer, fmt, slot->unk_24);
                SysFont_SetHAlign(&textScr->fonts[i + 10], 0, 20);
                SysFont_SetColor(&textScr->fonts[i + 10], 12);
                SysFont_DrawToScreen(&textScr->fonts[i + 10], buffer, map + 2, charData + 2, 0);
            }
        } break;

        case 4:
            func_ov043_020b52ac(textScr, charData, map);
            break;
    }

    SysFont_SetMsg(&textScr->fonts[0], itemIndex + 10177); // "Ice Blow"...
    SysFont_SetHAlign(&textScr->fonts[0], 0, 114);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

// Nonmatching: regswap
void func_ov043_020b5d18(Shop_textScr* textScr, u16* charData, u16* map, s32 itemIndex) {
    ShopObject*             shop = textScr->owner;
    Shop_textScr_ThreadData raw;

    func_ov043_020af4f0(&raw, itemIndex);

    switch (shop->unk_854) {
        case 0: {
            SysCode buffer[200];

            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_D + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_D + i].y);
            }

            SysFont_SetMsg(&textScr->fonts[1], raw.unk_02 + 10785); // "Mus Rattus"...
            SysFont_SetHAlign(&textScr->fonts[1], 0, 107);
            SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

            {
                SysCode fmt[4] = {SYSFONT_CODE_FMT_STR, SYSFONT_CODE_COLOR(12), SYSFONT_CODE_FMT_U32,
                                  SYSFONT_CODE_STR_END};                       // "<str><cC><u32>"

                SysCode* label = SysFont_GetMsgBuf(&textScr->fonts[2], 13096); // "Bravery Req."
                SysFont_Format(buffer, fmt, label, raw.unk_08);
                SysFont_SetHAlign(&textScr->fonts[2], 0, 101);
                SysFont_DrawToScreen(&textScr->fonts[2], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, label);
            }

            if (raw.unk_0C == 0) {
                SysFont_SetMsg(&textScr->fonts[3], 13113); // "â€•â€??"
                SysFont_SetHAlign(&textScr->fonts[3], 0, 101);
                SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);
            } else {
                SysCode* fmt;
                SysCode* label = SysFont_GetMsgBuf(&textScr->fonts[3], 13090); // "Attack"

                if (raw.unk_0C > 0) {
                    fmt = SysFont_GetMsgBuf(&textScr->fonts[3], 13115);        // "<str> <c6>+<s32>"
                } else {
                    fmt        = SysFont_GetMsgBuf(&textScr->fonts[3], 13116); // "<str> <cC>-<s32>"
                    raw.unk_0C = -raw.unk_0C;
                }

                SysFont_Format(buffer, fmt, label, raw.unk_0C);
                SysFont_SetHAlign(&textScr->fonts[3], 0, 101);
                SysFont_DrawToScreen(&textScr->fonts[3], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, fmt);
                Mem_Free(&gDebugHeap, label);
            }

            if (raw.unk_0A == 0) {
                SysFont_SetMsg(&textScr->fonts[4], 13113); // "â€•â€??"
                SysFont_SetHAlign(&textScr->fonts[4], 0, 101);
                SysFont_DrawCurrentToScreen(&textScr->fonts[4], map + 2, charData + 2, 0);
            } else {
                SysCode* fmt;
                SysCode* label = SysFont_GetMsgBuf(&textScr->fonts[4], 13091); // "Defense"

                if (raw.unk_0A > 0) {
                    fmt = SysFont_GetMsgBuf(&textScr->fonts[4], 13115);        // "<str> <c6>+<s32>"
                } else {
                    fmt        = SysFont_GetMsgBuf(&textScr->fonts[4], 13116); // "<str> <cC>-<s32>"
                    raw.unk_0A = -raw.unk_0A;
                }

                SysFont_Format(buffer, fmt, label, raw.unk_0A);
                SysFont_SetHAlign(&textScr->fonts[4], 0, 101);
                SysFont_DrawToScreen(&textScr->fonts[4], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, fmt);
                Mem_Free(&gDebugHeap, label);
            }

            if (raw.unk_0E == 0) {
                SysFont_SetMsg(&textScr->fonts[5], 13113); // "â€•â€??"
                SysFont_DrawCurrentToScreen(&textScr->fonts[5], map + 2, charData + 2, 0);
                SysFont_SetHAlign(&textScr->fonts[5], 0, 101);
            } else {
                SysCode* fmt;
                SysCode* label = SysFont_GetMsgBuf(&textScr->fonts[5], 13092); // "HP"

                if (raw.unk_0E > 0) {
                    fmt = SysFont_GetMsgBuf(&textScr->fonts[5], 13115);        // "<str> <c6>+<s32>"
                } else {
                    fmt        = SysFont_GetMsgBuf(&textScr->fonts[5], 13116); // "<str> <cC>-<s32>"
                    raw.unk_0E = -raw.unk_0E;
                }

                SysFont_Format(buffer, fmt, label, raw.unk_0E);
                SysFont_SetHAlign(&textScr->fonts[5], 0, 101);
                SysFont_DrawToScreen(&textScr->fonts[5], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, fmt);
                Mem_Free(&gDebugHeap, label);
            }
        } break;

        case 1: {
            SysCode buffer[40];
            SysCode fmt[3] = {SYSFONT_CODE_FMT_STR, SYSFONT_CODE_FMT_STR, SYSFONT_CODE_STR_END}; // "<str><str>"

            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            {
                SysCode* threadLabel = SysFont_GetMsgBuf(&textScr->fonts[1], 13151);              // "Threads: "
                SysCode* slotName    = SysFont_GetMsgBuf(&textScr->fonts[1], raw.unk_03 + 13152); // "Headwear"...

                SysFont_Format(buffer, fmt, threadLabel, slotName);
                SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
                SysFont_DrawToScreen(&textScr->fonts[1], buffer, map + 2, charData + 2, 0);
                Mem_Free(&gDebugHeap, threadLabel);
                Mem_Free(&gDebugHeap, slotName);
            }

            SysFont_SetMsg(&textScr->fonts[2], itemIndex + 11004); // "This baseball cap bears an "M" patch.\nA"...
            SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
            SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
        } break;

        case 2: {
            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            ShopItemSlot* slot = &shop->slots[shop->unk_848];
            if (slot->validFlag == 1) {
                SysFont_SetMsg(&textScr->fonts[1], raw.unk_10 + 8899); // "<ic_ab>Mother Lode"...
                SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
                SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

                SysFont_SetMsg(&textScr->fonts[2], raw.unk_10 + 8603); // "Increases your <cC>drop rate<cE> by 1."...
                SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
                SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
            } else {
                SysFont_SetMsg(&textScr->fonts[1], 13280); // "Ability Locked"
                SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
                SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

                SysFont_SetMsg(&textScr->fonts[2], 13281); // "You haven't acquired this item's ability"...
                SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
                SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
            }
        } break;

        case 4:
            func_ov043_020b52ac(textScr, charData, map);
            break;
    }

    SysFont_SetMsg(&textScr->fonts[0], itemIndex + 11284); // "M Cap"...
    SysFont_SetHAlign(&textScr->fonts[0], 0, 114);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

void func_ov043_020b63fc(Shop_textScr* textScr, u16* charData, u16* map, s32 itemIndex) {
    ShopObject* shop = textScr->owner;

    Shop_textScr_FoodData raw;

    func_ov043_020af51c(&raw, itemIndex);

    switch (shop->unk_854) {
        case 0: {
            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            SysFont_SetMsg(&textScr->fonts[1], 13158); // "Food Item"
            SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
            SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

            SysFont_SetMsg(&textScr->fonts[2], itemIndex + 10819); // "A regular hamburger.\nNothing special, b"...
            SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
            SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
        } break;

        case 1: {
            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            SysCode* fmt;
            if (raw.unk_02 == 1) {
                fmt = SysFont_GetMsgBuf(&textScr->fonts[1], 13299); // "Goes down in <u32> byte"
            } else {
                fmt = SysFont_GetMsgBuf(&textScr->fonts[1], 13298); // "Goes down in <u32> bytes"
            }

            SysCode buffer[200];
            SysFont_Format(buffer, fmt, raw.unk_02);
            SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
            SysFont_DrawToScreen(&textScr->fonts[1], buffer, map + 2, charData + 2, 0);
            Mem_Free(&gDebugHeap, fmt);

            s16      msgIndex = raw.unk_08 + 13292;
            SysCode* text = SysFont_GetMsgBuf(&textScr->fonts[2], msgIndex); // "Eat this to <cC>boost your sync rate rou"...
            SysFont_Format(buffer, text, raw.unk_0C, raw.unk_0A);
            SysFont_DrawToScreen(&textScr->fonts[2], buffer, map + 2, charData + 2, 0);
            Mem_Free(&gDebugHeap, text);
        } break;

        case 4:
            func_ov043_020b52ac(textScr, charData, map);
            break;
    }

    SysFont_SetMsg(&textScr->fonts[0], itemIndex + 10861); // "Hamburger"...
    SysFont_SetHAlign(&textScr->fonts[0], 0, 114);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

void func_ov043_020b6668(Shop_textScr* textScr, u16* charData, u16* map, s32 itemIndex) {
    ShopObject* shop = textScr->owner;

    switch (shop->unk_854) {
        case 0: {
            for (u16 i = 0; i < 17; i++) {
                SysFont_SetPos(&textScr->fonts[i], data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].x,
                               data_ov043_020cc95c[SHOP_TEXTSCR_LAYOUT_B + i].y);
            }

            SysFont_SetMsg(&textScr->fonts[1], 13159); // "Swag"
            SysFont_SetHAlign(&textScr->fonts[1], 0, 149);
            SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

            SysFont_SetMsg(&textScr->fonts[2], itemIndex + 13352); // "Awe-inspiring thread, hand-dyed in Nishi"...
            SysFont_SetHAlign(&textScr->fonts[2], 1, SYSFONT_NO_LIMIT);
            SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
        } break;

        case 4:
            func_ov043_020b52ac(textScr, charData, map);
            break;

        default:
            break;
    }

    SysFont_SetMsg(&textScr->fonts[0], itemIndex + 13502); // "Colorful Thread"...
    SysFont_SetHAlign(&textScr->fonts[0], 0, 114);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

void func_ov043_020b678c(Shop_textScr* textScr) {
    ShopObject* shop = textScr->owner;

    u16* map      = shop->unk_914;
    u16* charData = shop->unk_910;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    u16          itemID        = shop->slots[shop->unk_848].itemID;
    u16          categoryIndex = Inventory_GetCategorizedIndex(itemID);
    ItemCategory category      = Inventory_GetCategory(itemID);

    if (itemID == 0xFFFF) {
        return;
    }

    switch (category) {
        case ITEM_CATEGORY_PIN:
            func_ov043_020b55a4(textScr, charData, map, categoryIndex);
            break;
        case ITEM_CATEGORY_THREAD:
            func_ov043_020b5d18(textScr, charData, map, categoryIndex);
            break;
        case ITEM_CATEGORY_FOOD:
            func_ov043_020b63fc(textScr, charData, map, categoryIndex);
            break;
        case ITEM_CATEGORY_SWAG:
            func_ov043_020b6668(textScr, charData, map, categoryIndex);
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
        SysFont_Destroy(&textScr->fonts[i]);
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
