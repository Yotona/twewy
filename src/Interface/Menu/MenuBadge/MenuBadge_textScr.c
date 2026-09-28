#include "Engine/Core/Memory.h"
#include "Interface/Menu/MenuBadge.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ MenuBadgeObject* menuBadge;
    /* 0x004 */ SysFont          fonts[5];
} MenuBadge_textScr; // Size: 0x270

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_textScr_Args;

static s32 MenuBadge_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static void MenuBadge_textScr_InitFonts(MenuBadge_textScr* textScr) {
    for (s32 i = 0; i < 5; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetColor(&textScr->fonts[i], 14);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
    }
}

#ifdef REGION_USA
static void MenuBadge_textScr_DrawPrice(MenuBadge_textScr* textScr, u16* map, u16* charData, u32 price) {
    SysCode  buf[60];
    SysCode* fmt;

    if (price < 1000) {
        fmt = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_SHOP_PRICE_FMT);
        SysFont_Format(buf, fmt, price);
    } else if (price < 1000000) {
        u32 remainder = price;
        remainder %= 1000;
        u32 hundreds = remainder / 100;
        remainder %= 100;
        u32 tens = remainder / 10;
        remainder %= 10;

        fmt = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_SHOP_PRICE_THOUSANDS_FMT);
        SysFont_Format(buf, fmt, price / 1000, hundreds, tens, remainder);
    } else {
        u32 thousands;
        u32 hundreds;
        u32 tens;
        u32 remainder = price;
        u32 hundredThousands;
        u32 tenThousands;

        remainder %= 1000000;
        hundredThousands = remainder / 100000;
        remainder %= 100000;
        tenThousands = remainder / 10000;
        remainder %= 10000;
        thousands = remainder / 1000;
        remainder %= 1000;
        hundreds = remainder / 100;
        remainder %= 100;
        tens = remainder / 10;
        remainder %= 10;

        fmt = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_SHOP_PRICE_MILLIONS_FMT);
        SysFont_Format(buf, fmt, price / 1000000, hundredThousands, tenThousands, thousands, hundreds, tens, remainder);
    }

    SysFont_SetHAlign(&textScr->fonts[1], 0, 256);
    SysFont_DrawToScreen(&textScr->fonts[1], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
}
#endif

static void MenuBadge_textScr_DrawSellConfirm(MenuBadge_textScr* textScr, u16* map, u16* charData) {
    MenuBadgeObject* menuBadge = textScr->menuBadge;
#ifdef REGION_USA
    SysCode buf[60];
#else
    SysCode buf[30];
#endif
    SysCode countFmt[3]  = {SYSFONT_CODE_COLOR(12), SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};
    Point   positions[5] = {
        {  0, 62},
        {  0, 78},
        {  0, 93},
        {209, 77},
        {  0,  0},
    };
    u16 pinId;
    u32 price;

    for (u16 i = 0; i < 5; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    pinId = menuBadge->cursorBadge.pinId;
    price = MenuBadge_CalcSellPrice(menuBadge->cursorBadge.price, menuBadge->cursorBadge.priceGrowth,
                                    menuBadge->cursorBadge.level);

    SysFont_SetMsg(&textScr->fonts[0], pinId + SYSMSG_PIN_NAMES_START);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 256);
    SysFont_SetVAlign(&textScr->fonts[0], 3, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[2], SYSMSG_PIN_SELL_CONFIRM);
    SysFont_SetHAlign(&textScr->fonts[2], 0, 256);
    SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);

#ifdef REGION_USA
    if (MenuBadge_IsSlotMastered(menuBadge, menuBadge->dragSrc) == FALSE) {
        MenuBadge_textScr_DrawPrice(textScr, map, charData, price);
        return;
    }

    u16 count = menuBadge->sellCount;
    MenuBadge_textScr_DrawPrice(textScr, map, charData, price * count);
#else
    SysCode* fmt;

    if (MenuBadge_IsSlotMastered(menuBadge, menuBadge->dragSrc) == FALSE) {
        fmt = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_SHOP_PRICE_FMT);
        SysFont_SetHAlign(&textScr->fonts[1], 0, 256);
        SysFont_Format(buf, fmt, price);
        SysFont_DrawToScreen(&textScr->fonts[1], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        return;
    }

    u16 count = menuBadge->sellCount;
    fmt       = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_SHOP_PRICE_FMT);
    SysFont_SetHAlign(&textScr->fonts[1], 0, 256);
    SysFont_Format(buf, fmt, price * count);
    SysFont_DrawToScreen(&textScr->fonts[1], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
#endif

    SysFont_Format(buf, countFmt, count);
    SysFont_SetHAlign(&textScr->fonts[3], 0, 30);
    SysFont_DrawToScreen(&textScr->fonts[3], buf, map + 2, charData + 2, 0);
}

static void MenuBadge_textScr_DrawArrange(MenuBadge_textScr* textScr, u16* map, u16* charData) {
    MenuBadgeObject* menuBadge    = textScr->menuBadge;
    Point            positions[5] = {
        {16,  40},
        {88,  67},
        {88,  90},
        {88, 119},
        { 0,   0},
    };

    for (u16 i = 0; i < 5; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_PIN_ARRANGE_TITLE);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 224);
    SysFont_SetVAlign(&textScr->fonts[0], 3, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[1], menuBadge->autoArrangeBy[0] + SYSMSG_PIN_ARRANGE_BY_NUMBER);
    SysFont_SetHAlign(&textScr->fonts[1], 2, 136);
    SysFont_SetVAlign(&textScr->fonts[1], 3, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[2], menuBadge->autoArrangeBy[1] + SYSMSG_PIN_ARRANGE_BY_PSYCH);
    SysFont_SetHAlign(&textScr->fonts[2], 2, 136);
    SysFont_SetVAlign(&textScr->fonts[2], 3, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[3], SYSMSG_PIN_ALWAYS_ARRANGE);
    SysFont_SetHAlign(&textScr->fonts[3], 2, 136);
    SysFont_SetVAlign(&textScr->fonts[3], 3, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);
}

static void MenuBadge_textScr_DrawCannotSell(MenuBadge_textScr* textScr, u16* map, u16* charData) {
    Point positions[5] = {
        {0, 56},
        {0,  0},
        {0,  0},
        {0,  0},
        {0,  0},
    };

    for (u16 i = 0; i < 5; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_PIN_CANNOT_SELL);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 256);
    SysFont_SetVAlign(&textScr->fonts[0], 0, 80);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

static void MenuBadge_textScr_DrawClassLimit(MenuBadge_textScr* textScr, u16* map, u16* charData, s32 pinClass) {
    Point positions[5] = {
        {0, 56},
        {0,  0},
        {0,  0},
        {0,  0},
        {0,  0},
    };

    for (u16 i = 0; i < 5; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    SysFont_SetMsg(&textScr->fonts[0], pinClass + SYSMSG_PIN_CLASS_LIMIT_START);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 256);
    SysFont_SetVAlign(&textScr->fonts[0], 0, 80);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

static void MenuBadge_textScr_DrawTooManyPins(MenuBadge_textScr* textScr, u16* map, u16* charData) {
    MenuBadgeObject* menuBadge = textScr->menuBadge;
#ifdef REGION_USA
    SysCode buf[200];
#else
    SysCode buf[100];
#endif
    Point positions[5] = {
        {0, 56},
        {0,  0},
        {0,  0},
        {0,  0},
        {0,  0},
    };
    SysCode* fmt;

    for (u16 i = 0; i < 5; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

#ifdef REGION_USA
    if (menuBadge->excessPinCount == 1) {
        fmt = SysFont_GetMsgBuf(&textScr->fonts[0], SYSMSG_PIN_STOCKPILE_FULL_SINGULAR);
    } else {
        fmt = SysFont_GetMsgBuf(&textScr->fonts[0], SYSMSG_PIN_STOCKPILE_FULL);
    }
#else
    fmt = SysFont_GetMsgBuf(&textScr->fonts[0], SYSMSG_PIN_STOCKPILE_FULL);
#endif
    SysFont_Format(buf, fmt, menuBadge->excessPinCount);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 256);
    SysFont_SetVAlign(&textScr->fonts[0], 0, 80);
    SysFont_DrawToScreen(&textScr->fonts[0], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
}

#ifdef REGION_USA
static void MenuBadge_textScr_DrawMoneyCapped(MenuBadge_textScr* textScr, u16* map, u16* charData) {
    Point positions[5] = {
        {0, 56},
        {0,  0},
        {0,  0},
        {0,  0},
        {0,  0},
    };

    for (u16 i = 0; i < 5; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_PIN_WALLET_FULL);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 256);
    SysFont_SetVAlign(&textScr->fonts[0], 0, 80);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}
#endif

static void MenuBadge_textScr_DrawWindow(MenuBadge_textScr* textScr) {
    MenuBadgeObject* menuBadge = textScr->menuBadge;
    u16*             map       = menuBadge->resources[5].screenMap;
    u16*             charData  = menuBadge->resources[5].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    switch (menuBadge->windowMessage) {
        case MENUBADGE_MSG_SELL_CONFIRM:
            MenuBadge_textScr_DrawSellConfirm(textScr, map, charData);
            break;
        case MENUBADGE_MSG_ARRANGE:
            MenuBadge_textScr_DrawArrange(textScr, map, charData);
            break;
        case MENUBADGE_MSG_CANNOT_SELL:
            MenuBadge_textScr_DrawCannotSell(textScr, map, charData);
            break;
        case MENUBADGE_MSG_CLASS_LIMIT + 0:
            MenuBadge_textScr_DrawClassLimit(textScr, map, charData, 0);
            break;
        case MENUBADGE_MSG_CLASS_LIMIT + 1:
            MenuBadge_textScr_DrawClassLimit(textScr, map, charData, 1);
            break;
        case MENUBADGE_MSG_CLASS_LIMIT + 2:
            MenuBadge_textScr_DrawClassLimit(textScr, map, charData, 2);
            break;
        case MENUBADGE_MSG_CLASS_LIMIT + 3:
            MenuBadge_textScr_DrawClassLimit(textScr, map, charData, 3);
            break;
        case MENUBADGE_MSG_CLASS_LIMIT + 4:
            MenuBadge_textScr_DrawClassLimit(textScr, map, charData, 4);
            break;
        case MENUBADGE_MSG_TOO_MANY_PINS:
            MenuBadge_textScr_DrawTooManyPins(textScr, map, charData);
            break;
#ifdef REGION_USA
        case MENUBADGE_MSG_MONEY_CAPPED:
            MenuBadge_textScr_DrawMoneyCapped(textScr, map, charData);
            break;
#endif
    }
}

static s32 MenuBadge_textScr_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_textScr*      textScr     = task->data;
    MenuBadge_textScr_Args* textScrArgs = args;

    textScr->menuBadge = textScrArgs->menuBadge;
    MenuBadge_textScr_InitFonts(textScr);
    MenuBadge_textScr_DrawWindow(textScr);
    return 1;
}

static s32 MenuBadge_textScr_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_textScr* textScr   = task->data;
    MenuBadgeObject*   menuBadge = textScr->menuBadge;

    if (menuBadge->flags & MENUBADGE_FLAG_REDRAW_WINDOW) {
        MenuBadge_ReloadBgResource(&menuBadge->resources[5], DISPLAY_MAIN, 1, 6, 15, 1);
        MenuBadge_textScr_DrawWindow(textScr);
        menuBadge->flags &= ~MENUBADGE_FLAG_REDRAW_WINDOW;
    }
    return 1;
}

static s32 MenuBadge_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuBadge_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuBadge_textScr* textScr = task->data;

    for (s32 i = 0; i < 5; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 MenuBadge_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_textScr_Init,
        .update     = MenuBadge_textScr_Update,
        .render     = MenuBadge_textScr_Render,
        .cleanup    = MenuBadge_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_MenuBadge_textScr = {"Tsk_MenuBadge_textScr", MenuBadge_textScr_RunTask,
                                                 sizeof(MenuBadge_textScr)};

s32 MenuBadge_textScr_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_textScr_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_textScr, NULL, 0, NULL, &args);
}
