#include "Interface/Menu/Top.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ MenuTopObject* topMenu;
    /* 0x004 */ SysFont        fonts[8];
} MenuTop_textScr; // Size: 0x3E4

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
} MenuTop_textScr_Args;

static s32 MenuTop_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_textScr = {"Tsk_MenuTop_textScr", MenuTop_textScr_RunTask, sizeof(MenuTop_textScr)};

static void MenuTop_textScr_InitFonts(MenuTop_textScr* textScr) {
    for (s32 i = 0; i < 8; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetColor(&textScr->fonts[i], 8);
    }
}

static void MenuTop_textScr_DrawEntryNames(MenuTop_textScr* textScr) {
    MenuTopObject* topMenu    = textScr->topMenu;
    const Point    fontPos[8] = {
        { 34,  49},
        { 97,  49},
        {160,  49},
        {223,  49},
        { 34, 103},
        { 97, 103},
        {160, 103},
        {223, 103},
    };

    s32  i;
    u16* map      = topMenu->resources[5].screenMap;
    u16* charData = topMenu->resources[5].charData;
    if ((map == NULL) || (charData == NULL)) {
        OS_WaitForever();
    }

    for (i = 0; i < 8; i++) {
        SysFont_SetPos(&textScr->fonts[i], fontPos[i].x - 32, fontPos[i].y + 14);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
    }

    for (i = 0; i < 8; i++) {
        if (MenuTop_IsEntryAvailable(i) == 1) {
            SysFont_SetMsg(&textScr->fonts[i], SYSMSG_TOPMENU_ENTRY_NAMES_START + i);
            SysFont_SetHAlign(&textScr->fonts[i], 0, 64);
            SysFont_DrawCurrentToScreen(&textScr->fonts[i], map + 2, charData + 2, 0);
        }
    }
}

static s32 MenuTop_textScr_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScr*      textScr     = task->data;
    MenuTop_textScr_Args* textScrArgs = args;

    textScr->topMenu = textScrArgs->topMenu;
    MenuTop_textScr_InitFonts(textScr);
    MenuTop_textScr_DrawEntryNames(textScr);
    return 1;
}

static s32 MenuTop_textScr_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuTop_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuTop_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScr* textScr = task->data;

    for (s32 i = 0; i < 8; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 MenuTop_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_textScr_Init,
        .update     = MenuTop_textScr_Update,
        .render     = MenuTop_textScr_Render,
        .cleanup    = MenuTop_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_textScr_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu) {
    MenuTop_textScr_Args args;
    args.dataType = dataType;
    args.topMenu  = topMenu;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_textScr, NULL, 0, NULL, &args);
}
