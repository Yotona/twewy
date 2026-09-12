#include "Interface/Menu/Top.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "Util/SysFont.h"

extern s32 func_020243d4(u32 arg0);

typedef struct {
    /* 0x000 */ UnkStruct_TopMenu* topMenu;
    /* 0x004 */ SysFont            fonts[10];
} MenuTop_textScrU; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32                dataType;
    /* 0x4 */ UnkStruct_TopMenu* topMenu;
} MenuTop_textScrU_Args;

static s32 MenuTop_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_textScrU = {"Tsk_MenuTop_textScrU", MenuTop_textScrU_RunTask, 0x4DC};

static const Point MenuTop_textScrU_HelpFontPos[3] = {
    {24, 11},
    {24, 10},
    {14, 27},
};

static void MenuTop_textScrU_InitFonts(MenuTop_textScrU* textScrU) {
    for (s32 i = 0; i < 10; i++) {
        SysFont_Init(&textScrU->fonts[i]);
        SysFont_SetColor(&textScrU->fonts[i], 14);
    }
}

static void MenuTop_textScrU_DrawTrendPage(MenuTop_textScrU* textScrU) {
    UnkStruct_TopMenu* topMenu     = textScrU->topMenu;
    const Point        fontPos[10] = {
        {28,  91},
        {28, 116},
        {28, 141},
        {28, 166},
        {28, 103},
        {28, 128},
        {28, 153},
        {28, 178},
        { 8,   4},
        { 8,   4}
    };

    SysCode text[180];

    s32  i;
    u16* map      = topMenu->unk_7C;
    u16* charData = topMenu->unk_78;
    if ((map == NULL) || (charData == NULL)) {
        OS_WaitForever();
    }

    for (i = 0; i < 10; i++) {
        SysFont_SetPos(&textScrU->fonts[i], fontPos[i].x, fontPos[i].y);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    if (topMenu->unk_4E < 21) {
        SysFont_SetMsg(&textScrU->fonts[0], topMenu->unk_20[0] + SYSMSG_BRAND_NAMES_START);
        SysFont_SetMsg(&textScrU->fonts[1], topMenu->unk_20[1] + SYSMSG_BRAND_NAMES_START);
        SysFont_SetMsg(&textScrU->fonts[2], topMenu->unk_20[2] + SYSMSG_BRAND_NAMES_START);
        SysFont_SetMsg(&textScrU->fonts[3], topMenu->unk_2C + SYSMSG_BRAND_NAMES_START);
        SysFont_SetMsg(&textScrU->fonts[4], SYSMSG_BRAND_ATTACK_DOUBLED);
        SysFont_SetMsg(&textScrU->fonts[5], SYSMSG_BRAND_ATTACK_UP_50);
        SysFont_SetMsg(&textScrU->fonts[6], SYSMSG_BRAND_ATTACK_UP_20);
        SysFont_SetMsg(&textScrU->fonts[7], SYSMSG_BRAND_ATTACK_HALVED);

        for (i = 0; i < 8; i++) {
            SysFont_SetColor(&textScrU->fonts[i], 14);
            SysFont_SetHAlign(&textScrU->fonts[i], 0, 92);
            SysFont_DrawCurrentToScreen(&textScrU->fonts[i], map + 2, charData + 2, 0);
        }
    } else if ((topMenu->unk_4E >= 22) && (topMenu->unk_4E <= 34)) {
        SysFont_SetPos(&textScrU->fonts[0], 7, 24);

        SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[0], SYSMSG_BRAND_AREA_PROTECTED_FMT);
        SysCode* areaName =
            SysFont_GetMsgBuf(&textScrU->fonts[0], func_020243d4(topMenu->unk_4E & 0xFF) + SYSMSG_BRAND_NAMES_START);

        SysFont_Format(text, fmt, areaName);
        SysFont_SetColor(&textScrU->fonts[0], 14);
        SysFont_SetHAlign(&textScrU->fonts[0], 0, 112);
        SysFont_SetVAlign(&textScrU->fonts[0], 0, 160);
        SysFont_DrawToScreen(&textScrU->fonts[0], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, areaName);
    } else {
        SysFont_SetPos(&textScrU->fonts[0], 7, 24);
        SysFont_SetMsg(&textScrU->fonts[0], SYSMSG_BRAND_AREA_UNAFFECTED);
        SysFont_SetColor(&textScrU->fonts[0], 14);
        SysFont_SetHAlign(&textScrU->fonts[0], 0, 112);
        SysFont_SetVAlign(&textScrU->fonts[0], 0, 160);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);
    }

    u8  day = gSaveData.unk_1AB0;
    u16 partnerMsg;
    if (day <= 6) {
        partnerMsg = SYSMSG_PARTNER_SHIKI;
    } else if (day <= 13) {
        partnerMsg = SYSMSG_PARTNER_JOSHUA;
    } else if (day <= 20) {
        partnerMsg = SYSMSG_PARTNER_BEAT;
    }

    if (day <= 20) {
        SysCode* fmt     = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_PARTNER_DAY_FMT);
        SysCode* partner = SysFont_GetMsgBuf(&textScrU->fonts[8], partnerMsg);

        SysFont_Format(text, fmt, partner, day % 7 + 1);
        SysFont_SetColor(&textScrU->fonts[8], 8);
        SysFont_SetHAlign(&textScrU->fonts[8], 1, 240);
        SysFont_DrawToScreen(&textScrU->fonts[8], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, partner);
#ifdef REGION_USA
    } else {
        SysFont_SetMsg(&textScrU->fonts[8], SYSMSG_ANOTHER_DAY);
        SysFont_SetColor(&textScrU->fonts[8], 8);
        SysFont_SetHAlign(&textScrU->fonts[8], 1, 240);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[8], map + 2, charData + 2, 0);
#endif
    }

    if (func_0202366c(topMenu->unk_4E & 0xFF, 1) == 1) {
        SysFont_SetMsg(&textScrU->fonts[9], topMenu->unk_4E + SYSMSG_AREA_NAMES_START);
    } else {
        SysFont_SetMsg(&textScrU->fonts[9], SYSMSG_AREA_NAME_UNKNOWN);
    }

    SysFont_SetColor(&textScrU->fonts[9], 8);
    SysFont_SetHAlign(&textScrU->fonts[9], 2, 240);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[9], map + 2, charData + 2, 0);
}

static void MenuTop_textScrU_DrawEmptyPage(MenuTop_textScrU* textScrU) {
    return;
}

static void MenuTop_textScrU_DrawHelpPage(MenuTop_textScrU* textScrU) {
    UnkStruct_TopMenu* topMenu = textScrU->topMenu;
    Point              fontPos[3];
    fontPos = MenuTop_textScrU_HelpFontPos;
    u16 i;

    u16* map      = topMenu->unk_7C;
    u16* charData = topMenu->unk_78;
    if ((map == NULL) || (charData == NULL)) {
        OS_WaitForever();
    }
    for (i = 0; i < 3; i++) {
        SysFont_SetPos(&textScrU->fonts[i], fontPos[i].x, fontPos[i].y);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScrU->fonts[0], topMenu->unk_60 + SYSMSG_TOPMENU_HELP_LABELS);
    SysFont_SetHAlign(&textScrU->fonts[0], 1, 224);
    SysFont_SetVAlign(&textScrU->fonts[0], 3, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);
    SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[1], SYSMSG_DIVIDED_U32S);

    SysCode text[180];
    SysFont_Format(text, fmt, topMenu->unk_60 + 1, 7);
    SysFont_SetHAlign(&textScrU->fonts[1], 2, 220);
    SysFont_DrawToScreen(&textScrU->fonts[1], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
    SysFont_SetMsg(&textScrU->fonts[2], topMenu->unk_60 + SYSMSG_TOPMENU_HELP_TEXT);
    SysFont_SetHAlign(&textScrU->fonts[2], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
}

static s32 MenuTop_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScrU*      textScrU     = task->data;
    MenuTop_textScrU_Args* textScrUArgs = args;

    textScrU->topMenu = textScrUArgs->topMenu;
    MenuTop_textScrU_InitFonts(textScrU);
    MenuTop_textScrU_DrawTrendPage(textScrU);
    return 1;
}

static s32 MenuTop_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScrU*  textScrU = task->data;
    UnkStruct_TopMenu* topMenu  = textScrU->topMenu;

    if (topMenu->unk_00 & 2) {
        topMenu->unk_00 &= ~2;
        func_ov043_02085670(&topMenu->unk_68, 0, 0, 9, 15, 1);

        if (topMenu->unk_56 == 0) {
            MenuTop_textScrU_DrawTrendPage(textScrU);
        } else if (topMenu->unk_56 == 1) {
            MenuTop_textScrU_DrawEmptyPage(textScrU);
        } else {
            MenuTop_textScrU_DrawHelpPage(textScrU);
        }
    }
    return 1;
}

static s32 MenuTop_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuTop_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScrU* textScrU = task->data;

    for (s32 i = 0; i < 10; i++) {
        SysFont_Destroy(&textScrU->fonts[i]);
    }

    return 1;
}

static s32 MenuTop_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_textScrU_Init,
        .update     = MenuTop_textScrU_Update,
        .render     = MenuTop_textScrU_Render,
        .cleanup    = MenuTop_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void MenuTop_textScrU_CreateTask(TaskPool* pool, s32 dataType, UnkStruct_TopMenu* topMenu) {
    MenuTop_textScrU_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    EasyTask_CreateTask(pool, &Tsk_MenuTop_textScrU, NULL, 0, NULL, &args);
}
