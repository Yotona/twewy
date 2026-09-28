#include "Engine/Core/Memory.h"
#include "Interface/Menu/MenuBadge.h"
#include "Util/SysFont.h"

// JP has no Tin Pin stat readout (fonts 15-21).
#ifdef REGION_USA
    #define MENUBADGE_TEXTSCRU_FONT_COUNT 22
#else
    #define MENUBADGE_TEXTSCRU_FONT_COUNT 15
#endif

typedef struct {
    /* 0x000 */ MenuBadgeObject* menuBadge;
    /* 0x004 */ SysFont          fonts[MENUBADGE_TEXTSCRU_FONT_COUNT];
} MenuBadge_textScrU; // Size: 0xAAC (JP: 0x748)

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
} MenuBadge_textScrU_Args;

extern s32 func_02024434(s32 pinId);

static s32 MenuBadge_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static u16 MenuBadge_textScrU_CalcLevelAttack(u16 base, s16 growth, u8 level) {
    return base + growth * (level - 1);
}

static u16 MenuBadge_textScrU_CalcLevelTiming(u16 base, s16 growth, u8 level) {
    return base + growth * (level - 1);
}

static void MenuBadge_textScrU_FramesToSeconds(u16 frames, u16* time) {
    if (frames >= 6040) {
        time[0] = 99;
        time[1] = 9;
    }

    u16 fraction = frames % 60;

    time[0]  = frames / 60;
    fraction = fraction * 100 / 60;
    time[1]  = fraction / 10;
}

static void MenuBadge_textScrU_InitFonts(MenuBadge_textScrU* textScrU) {
    for (s32 i = 0; i < MENUBADGE_TEXTSCRU_FONT_COUNT; i++) {
        SysFont_Init(&textScrU->fonts[i]);
        SysFont_SetColor(&textScrU->fonts[i], 14);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }
}

static void MenuBadge_textScrU_DrawName(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
    SysFont_SetMsg(&textScrU->fonts[0], menuBadge->cursorBadge.pinId + SYSMSG_PIN_NAMES_START);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);
}

static void MenuBadge_textScrU_DrawBrand(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
    u8 brand = menuBadge->cursorBadge.brand;

    if (brand <= 12) {
        SysFont_SetMsg(&textScrU->fonts[1], brand + SYSMSG_BRAND_NAMES_START);
        SysFont_SetHAlign(&textScrU->fonts[1], 1, 220);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[1], map + 2, charData + 2, 0);
    } else {
        SysFont_SetMsg(&textScrU->fonts[1], SYSMSG_BRAND_UNBRANDED);
        SysFont_SetHAlign(&textScrU->fonts[1], 1, 220);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[1], map + 2, charData + 2, 0);
    }
}

static void MenuBadge_textScrU_DrawLevel(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif
    u8      level    = menuBadge->cursorBadge.level;
    u8      maxLevel = menuBadge->cursorBadge.maxLevel;
    SysCode fmt[2]   = {SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};

    SysFont_Format(buf, fmt, level);
    SysFont_SetHAlign(&textScrU->fonts[2], 0, 14);
    SysFont_DrawToScreen(&textScrU->fonts[2], buf, map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScrU->fonts[3], SYSMSG_SLASH);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[3], map + 2, charData + 2, 0);

    SysFont_Format(buf, fmt, maxLevel);
    SysFont_SetHAlign(&textScrU->fonts[4], 0, 14);
    SysFont_DrawToScreen(&textScrU->fonts[4], buf, map + 2, charData + 2, 0);
}

static void MenuBadge_textScrU_DrawPsych(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
    u16 psychId   = menuBadge->cursorBadge.psychId;
    u8  abilityId = menuBadge->cursorBadge.abilityId;

    if (psychId != 0xFFFF) {
        SysFont_SetMsg(&textScrU->fonts[5], psychId + SYSMSG_PSYCH_NAMES_START);
        SysFont_SetHAlign(&textScrU->fonts[5], 0, 121);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[5], map + 2, charData + 2, 0);
    } else if (abilityId == 0xFF) {
        SysFont_SetMsg(&textScrU->fonts[5], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[5], 0, 121);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[5], map + 2, charData + 2, 0);
    } else {
        SysFont_SetMsg(&textScrU->fonts[5], abilityId + SYSMSG_THREAD_ABILITY_NAME_START);
        SysFont_SetHAlign(&textScrU->fonts[5], 0, 121);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[5], map + 2, charData + 2, 0);
    }
}

static void MenuBadge_textScrU_DrawNextLevelPP(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData,
                                               u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif

    if (menuBadge->cursorBadge.level != menuBadge->cursorBadge.maxLevel) {
        SysCode fmt[2] = {SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};

        SysFont_Format(buf, fmt, (u16)(menuBadge->cursorBadge.nextLevelPP - menuBadge->cursorBadge.totalPP));
        SysFont_SetHAlign(&textScrU->fonts[6], 0, 71);
        SysFont_DrawToScreen(&textScrU->fonts[6], buf, map + 2, charData + 2, 0);
    } else {
        SysFont_SetMsg(&textScrU->fonts[6], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[6], 0, 71);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[6], map + 2, charData + 2, 0);
    }
}

static void MenuBadge_textScrU_DrawAttack(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif
    u8  level    = menuBadge->cursorBadge.level;
    u16 attack   = menuBadge->cursorBadge.attack;
    s16 growth   = menuBadge->cursorBadge.attackGrowth;
    s32 recovery = func_02024434(menuBadge->cursorBadge.pinId);

    if (recovery != 0) {
        SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[7], SYSMSG_PIN_RECOVERY_FMT);

        SysFont_Format(buf, fmt, recovery);
        SysFont_SetHAlign(&textScrU->fonts[7], 0, 90);
        SysFont_DrawToScreen(&textScrU->fonts[7], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
    } else if (attack == 0) {
        SysFont_SetMsg(&textScrU->fonts[7], SYSMSG_PIN_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[7], 0, 90);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[7], map + 2, charData + 2, 0);
    } else {
        SysCode  fmt[5] = {SYSFONT_CODE_FMT_STR, 0, SYSFONT_CODE_COLOR(12), SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};
        SysCode* label  = SysFont_GetMsgBuf(&textScrU->fonts[7], SYSMSG_STAT_ATTACK);

        SysFont_Format(buf, fmt, label, MenuBadge_textScrU_CalcLevelAttack(attack, growth, level));
        SysFont_SetHAlign(&textScrU->fonts[7], 0, 90);
        SysFont_DrawToScreen(&textScrU->fonts[7], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, label);
    }
}

static void MenuBadge_textScrU_DrawClass(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
    SysFont_SetMsg(&textScrU->fonts[8], menuBadge->cursorBadge.pinClass + SYSMSG_PIN_CLASS_NAMES_START);
    SysFont_SetHAlign(&textScrU->fonts[8], 0, 61);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[8], map + 2, charData + 2, 0);
}

static void MenuBadge_textScrU_DrawDuration(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData,
                                            u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif
    u16      time[3];
    u16      type     = menuBadge->cursorBadge.durationType;
    u8       level    = menuBadge->cursorBadge.level;
    u16      duration = menuBadge->cursorBadge.duration;
    s16      growth   = menuBadge->cursorBadge.durationGrowth;
    SysCode* fmt;

    if (type == 0) {
        SysFont_SetMsg(&textScrU->fonts[9], SYSMSG_PIN_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[9], 0, 90);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[9], map + 2, charData + 2, 0);
    } else if (type == 1 || type == 2) {
#ifdef REGION_USA
        u16 uses = MenuBadge_textScrU_CalcLevelTiming(duration, growth, level);

        if (uses == 1) {
            fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_PIN_DURATION_USE_FMT);
        } else {
            fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_PIN_DURATION_USES_FMT);
        }
        SysFont_Format(buf, fmt, uses);
#else
        fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_PIN_DURATION_USES_FMT);
        SysFont_Format(buf, fmt, MenuBadge_textScrU_CalcLevelTiming(duration, growth, level));
#endif
        SysFont_SetHAlign(&textScrU->fonts[9], 0, 90);
        SysFont_DrawToScreen(&textScrU->fonts[9], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
    } else {
        fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_PIN_DURATION_TIME_FMT);
        MenuBadge_textScrU_FramesToSeconds(MenuBadge_textScrU_CalcLevelTiming(duration, growth, level), time);
        SysFont_Format(buf, fmt, time[0], time[1]);
        SysFont_SetHAlign(&textScrU->fonts[9], 0, 90);
        SysFont_DrawToScreen(&textScrU->fonts[9], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
    }
}

static void MenuBadge_textScrU_DrawSellPrice(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData,
                                             u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif
    SysCode fmt[2] = {SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};
    u32     price  = MenuBadge_CalcSellPrice(menuBadge->cursorBadge.price, menuBadge->cursorBadge.priceGrowth,
                                             menuBadge->cursorBadge.level);

#ifdef REGION_USA
    SysCode* priceFmt;

    if (price < 1000) {
        priceFmt = SysFont_GetMsgBuf(&textScrU->fonts[10], SYSMSG_PIN_PRICE_FMT);
        SysFont_Format(buf, priceFmt, price);
    } else if (price < 1000000) {
        u32 remainder = price;
        remainder %= 1000;
        u32 hundreds = remainder / 100;
        remainder %= 100;
        u32 tens = remainder / 10;
        remainder %= 10;

        priceFmt = SysFont_GetMsgBuf(&textScrU->fonts[10], SYSMSG_PIN_PRICE_THOUSANDS_FMT);
        SysFont_Format(buf, priceFmt, price / 1000, hundreds, tens, remainder);
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

        priceFmt = SysFont_GetMsgBuf(&textScrU->fonts[10], SYSMSG_PIN_PRICE_MILLIONS_FMT);
        SysFont_Format(buf, priceFmt, price / 1000000, hundredThousands, tenThousands, thousands, hundreds, tens, remainder);
    }

    SysFont_SetHAlign(&textScrU->fonts[10], 0, 61);
    SysFont_DrawToScreen(&textScrU->fonts[10], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, priceFmt);
#else
    SysFont_Format(buf, fmt, price);
    SysFont_SetHAlign(&textScrU->fonts[10], 0, 61);
    SysFont_DrawToScreen(&textScrU->fonts[10], buf, map + 2, charData + 2, 0);
#endif
}

static void MenuBadge_textScrU_DrawBootTime(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData,
                                            u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif
    u16 time[3];
    u16 type     = menuBadge->cursorBadge.bootType;
    u8  level    = menuBadge->cursorBadge.level;
    u16 bootTime = menuBadge->cursorBadge.bootTime;
    s16 growth   = menuBadge->cursorBadge.bootTimeGrowth;

    if (type == 0) {
        SysFont_SetMsg(&textScrU->fonts[11], SYSMSG_PIN_BOOT_INSTANT);
        SysFont_SetHAlign(&textScrU->fonts[11], 0, 196);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[11], map + 2, charData + 2, 0);
    } else if (type == 1) {
        SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[11], SYSMSG_PIN_BOOT_TIME_FMT);

        MenuBadge_textScrU_FramesToSeconds(MenuBadge_textScrU_CalcLevelTiming(bootTime, growth, level), time);
        SysFont_Format(buf, fmt, time[0], time[1]);
        SysFont_SetHAlign(&textScrU->fonts[11], 0, 196);
        SysFont_DrawToScreen(&textScrU->fonts[11], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
    }
#ifdef REGION_USA
    else
    {
        SysFont_SetMsg(&textScrU->fonts[11], SYSMSG_AREA_NAME_UNKNOWN);
        SysFont_SetHAlign(&textScrU->fonts[11], 0, 196);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[11], map + 2, charData + 2, 0);
    }
#endif
}

static void MenuBadge_textScrU_DrawRebootTime(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData,
                                              u16* map) {
#ifdef REGION_USA
    SysCode buf[40];
#else
    SysCode buf[20];
#endif
    u16 time[3];
    u16 type       = menuBadge->cursorBadge.rebootType;
    u8  level      = menuBadge->cursorBadge.level;
    u16 rebootTime = menuBadge->cursorBadge.rebootTime;
    s16 growth     = menuBadge->cursorBadge.rebootTimeGrowth;

    if (type == 0) {
        SysFont_SetMsg(&textScrU->fonts[12], SYSMSG_PIN_REBOOT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[12], 0, 196);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[12], map + 2, charData + 2, 0);
    } else if (type == 1) {
        SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_PIN_REBOOT_TIME_FMT);

        MenuBadge_textScrU_FramesToSeconds(MenuBadge_textScrU_CalcLevelTiming(rebootTime, growth, level), time);
        SysFont_Format(buf, fmt, time[0], time[1]);
        SysFont_SetHAlign(&textScrU->fonts[12], 0, 196);
        SysFont_DrawToScreen(&textScrU->fonts[12], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
    }
}

static void MenuBadge_textScrU_DrawInputType(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData,
                                             u16* map) {
    u8 inputType = menuBadge->cursorBadge.inputType;

    if (menuBadge->infoTab != 0) {
        return;
    }

    if (inputType != 0xFF) {
        SysFont_SetMsg(&textScrU->fonts[13], inputType + SYSMSG_PIN_INPUT_TYPES_START);
        SysFont_SetHAlign(&textScrU->fonts[13], 0, 135);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[13], map + 2, charData + 2, 0);
    } else {
        SysFont_SetMsg(&textScrU->fonts[13], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[13], 0, 135);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[13], map + 2, charData + 2, 0);
    }
}

static void MenuBadge_textScrU_DrawInfoTab(MenuBadge_textScrU* textScrU, MenuBadgeObject* menuBadge, u16* charData, u16* map) {
    u8  infoTab = menuBadge->infoTab;
    u16 pinId   = menuBadge->cursorBadge.pinId;

    if (infoTab == 0) {
        SysFont_SetMsg(&textScrU->fonts[14], pinId + SYSMSG_PIN_EFFECT_HELP_START);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[14], map + 2, charData + 2, 0);
    } else if (infoTab == 1) {
        SysFont_SetMsg(&textScrU->fonts[14], pinId + SYSMSG_PIN_GROWTH_HELP_START);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[14], map + 2, charData + 2, 0);
    } else {
#ifdef REGION_USA
        SysCode buf[20];
        SysCode fmt[2] = {SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};

        SysFont_SetMsg(&textScrU->fonts[14], SYSMSG_PIN_TINPIN_HELP_START);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[14], map + 2, charData + 2, 0);

        // Weight, spin, KO length and the four whammy stats, in field order.
        for (u16 i = 0; i < 7; i++) {
            SysFont_Format(buf, fmt, (&menuBadge->cursorBadge.tinPinWeight)[i]);
            SysFont_SetHAlign(&textScrU->fonts[15 + i], 0, 20);
            SysFont_SetColor(&textScrU->fonts[15 + i], 12);
            SysFont_DrawToScreen(&textScrU->fonts[15 + i], buf, map + 2, charData + 2, 0);
        }
#else
        SysFont_SetMsg(&textScrU->fonts[14], pinId + SYSMSG_PIN_TINPIN_HELP_START);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[14], map + 2, charData + 2, 0);
#endif
    }
}

static void MenuBadge_textScrU_DrawInfoPage(MenuBadge_textScrU* textScrU) {
    MenuBadgeObject* menuBadge                                = textScrU->menuBadge;
    Point            positions[MENUBADGE_TEXTSCRU_FONT_COUNT] = {
#ifdef REGION_USA
        {  9,  17},
         {  9,  41},
         { 37,  56},
         { 52,  56},
         { 57,  56},
         {126,  56},
         { 44,  68},
         { 51,  86},
        {186,  86},
         { 51,  98},
         {186,  98},
         { 51, 110},
         { 51, 122},
         {112, 139},
         {  9, 151},
         { 50, 151},
        {132, 151},
         {214, 151},
         {132, 163},
         {214, 163},
         {132, 175},
         {214, 175},
#else
        {10, 17},  {10, 41}, {22, 56},  {56, 56},  {64, 56},  {126, 56}, {44, 68},  {51, 86},
        {186, 86}, {51, 98}, {186, 98}, {51, 110}, {51, 122}, {99, 139}, {10, 151},
#endif
    };
    s32  i;
    u16* map      = menuBadge->resources[0].screenMap;
    u16* charData = menuBadge->resources[0].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    if (menuBadge->cursorBadge.pinId == 0xFFFF) {
        return;
    }

    for (i = 0; i < MENUBADGE_TEXTSCRU_FONT_COUNT; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i].x, positions[i].y);
    }

    MenuBadge_textScrU_DrawName(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawBrand(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawLevel(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawPsych(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawNextLevelPP(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawAttack(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawClass(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawDuration(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawSellPrice(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawBootTime(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawRebootTime(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawInputType(textScrU, menuBadge, charData, map);
    MenuBadge_textScrU_DrawInfoTab(textScrU, menuBadge, charData, map);
}

static void MenuBadge_textScrU_DrawHelpPage(MenuBadge_textScrU* textScrU) {
    MenuBadgeObject* menuBadge    = textScrU->menuBadge;
    Point            positions[3] = {
        {24, 11},
        {24, 10},
        {14, 27},
    };
    u16  i;
    u16* map      = menuBadge->resources[0].screenMap;
    u16* charData = menuBadge->resources[0].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 3; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScrU->fonts[0], menuBadge->helpPage + SYSMSG_BADGEMENU_HELP_LABELS);
    SysFont_SetHAlign(&textScrU->fonts[0], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);

    SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[1], SYSMSG_DIVIDED_U32S);
#ifdef REGION_USA
    SysCode buf[60];
#else
    SysCode buf[30];
#endif

    SysFont_Format(buf, fmt, menuBadge->helpPage + 1, 11);
    SysFont_SetHAlign(&textScrU->fonts[1], 2, 220);
    SysFont_DrawToScreen(&textScrU->fonts[1], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);

    SysFont_SetMsg(&textScrU->fonts[2], menuBadge->helpPage + SYSMSG_BADGEMENU_HELP_TEXT);
    SysFont_SetHAlign(&textScrU->fonts[2], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
}

static s32 MenuBadge_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_textScrU*      textScrU     = task->data;
    MenuBadge_textScrU_Args* textScrUArgs = args;

    textScrU->menuBadge = textScrUArgs->menuBadge;
    MenuBadge_textScrU_InitFonts(textScrU);
    MenuBadge_textScrU_DrawInfoPage(textScrU);
    return 1;
}

static s32 MenuBadge_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_textScrU* textScrU  = task->data;
    MenuBadgeObject*    menuBadge = textScrU->menuBadge;

    if (menuBadge->flags & MENUBADGE_FLAG_REDRAW_INFO) {
        MenuBadge_ReloadBgResource(&menuBadge->resources[0], DISPLAY_SUB, 0, 7, 15, 1);
        if (menuBadge->windowMessage == MENUBADGE_MSG_HELP) {
            MenuBadge_textScrU_DrawHelpPage(textScrU);
        } else {
            MenuBadge_textScrU_DrawInfoPage(textScrU);
        }
        menuBadge->flags &= ~MENUBADGE_FLAG_REDRAW_INFO;
    }
    return 1;
}

static s32 MenuBadge_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuBadge_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuBadge_textScrU* textScrU = task->data;

    for (s32 i = 0; i < MENUBADGE_TEXTSCRU_FONT_COUNT; i++) {
        SysFont_Destroy(&textScrU->fonts[i]);
    }
    return 1;
}

static s32 MenuBadge_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_textScrU_Init,
        .update     = MenuBadge_textScrU_Update,
        .render     = MenuBadge_textScrU_Render,
        .cleanup    = MenuBadge_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_MenuBadge_textScrU = {"Tsk_MenuBadge_textScrU", MenuBadge_textScrU_RunTask,
                                                  sizeof(MenuBadge_textScrU)};

s32 MenuBadge_textScrU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* menuBadge) {
    MenuBadge_textScrU_Args args;

    args.dataType  = dataType;
    args.menuBadge = menuBadge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_textScrU, NULL, 0, NULL, &args);
}
