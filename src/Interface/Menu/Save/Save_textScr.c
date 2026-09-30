#include "Engine/Core/Memory.h"
#include "Interface/Menu/Save.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ SaveMenuObject* save;
    /* 0x004 */ SysFont         fonts[10];
} Save_textScr; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_textScr_Args;

static s32 Save_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_textScr = {"Tsk_Save_textScr", Save_textScr_RunTask, sizeof(Save_textScr)};

static void Save_textScr_InitFonts(Save_textScr* textScr) {
    s32 i;

    for (i = 0; i < 10; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void Save_textScr_DrawSaveInfo(Save_textScr* textScr, u16* map, u16* charData) {
    SaveMenuObject* save = textScr->save;
#ifdef REGION_USA
    SysCode  text[60];
    SysCode* fmt;
    SysCode* name;
    u32      year  = save->saveYear + 2000;
    u32      month = save->saveMonth;
    u32      day   = save->saveDay;
    u32      hour;
    u32      minute;
    u32      tens;
    u32      ones;
    u16      partnerMsg;
    u8       chapter;
#else
    SysCode  text[30];
    u8       chapter;
    u16      partnerMsg;
    SysCode* fmt;
    SysCode* name;
    u32      year  = save->saveYear + 2000;
    u32      month = save->saveMonth;
    u32      day   = save->saveDay;
    u32      hour;
    u32      minute;
#endif

#ifdef REGION_USA
    fmt  = SysFont_GetMsgBuf(&textScr->fonts[4], SYSMSG_SAVE_DATE_MONTH_FMT);
    name = SysFont_GetMsgBuf(&textScr->fonts[4], SYSMSG_SAVE_MONTH_NAMES_START + month - 1);
    SysFont_Format(text, fmt, name, day, year);
    SysFont_SetHAlign(&textScr->fonts[4], 0, 147);
    SysFont_DrawToScreen(&textScr->fonts[4], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, name);

    if (save->saveHour == 0 || save->saveHour == 12) {
        hour = 12;
    } else if (save->saveHour > 12 && save->saveHour < 24) {
        hour = save->saveHour - 12;
    } else {
        hour = save->saveHour;
    }
    minute = save->saveMinute;
    tens   = minute / 10;
    ones   = minute % 10;
    if (save->saveHour < 12) {
        fmt = SysFont_GetMsgBuf(&textScr->fonts[5], SYSMSG_SAVE_TIME_AM_FMT);
    } else {
        fmt = SysFont_GetMsgBuf(&textScr->fonts[5], SYSMSG_SAVE_TIME_PM_FMT);
    }
    SysFont_Format(text, fmt, hour, tens, ones);
#else
    fmt = SysFont_GetMsgBuf(&textScr->fonts[4], SYSMSG_SAVE_DATE_FMT);
    SysFont_Format(text, fmt, year, month / 10, month % 10, day / 10, day % 10);
    SysFont_SetHAlign(&textScr->fonts[4], 0, 147);
    SysFont_DrawToScreen(&textScr->fonts[4], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);

    hour   = save->saveHour;
    minute = save->saveMinute;
    fmt    = SysFont_GetMsgBuf(&textScr->fonts[5], SYSMSG_SAVE_TIME_FMT);
    SysFont_Format(text, fmt, hour / 10, hour % 10, minute / 10, minute % 10);
#endif
    SysFont_SetHAlign(&textScr->fonts[5], 0, 147);
    SysFont_DrawToScreen(&textScr->fonts[5], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);

    chapter = save->chapter;
    if (chapter <= 6) {
        partnerMsg = SYSMSG_PARTNER_SHIKI;
    } else if (chapter <= 13) {
        partnerMsg = SYSMSG_PARTNER_JOSHUA;
    } else if (chapter <= 20) {
        partnerMsg = SYSMSG_PARTNER_BEAT;
    }
    if (chapter <= 20) {
        fmt  = SysFont_GetMsgBuf(&textScr->fonts[6], SYSMSG_PARTNER_DAY_FMT);
        name = SysFont_GetMsgBuf(&textScr->fonts[6], partnerMsg);
        SysFont_Format(text, fmt, name, chapter % 7 + 1);
        SysFont_SetHAlign(&textScr->fonts[6], 0, 147);
        SysFont_DrawToScreen(&textScr->fonts[6], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, name);
    } else {
        SysFont_SetMsg(&textScr->fonts[6], SYSMSG_ANOTHER_DAY);
        SysFont_SetHAlign(&textScr->fonts[6], 0, 147);
        SysFont_DrawCurrentToScreen(&textScr->fonts[6], map + 2, charData + 2, 0);
    }

#ifdef REGION_USA
    {
        u8 area = save->area;

        if (func_0202366c(area, 1) == 1) {
            SysFont_SetMsg(&textScr->fonts[7], SYSMSG_AREA_NAMES_START + area);
        } else {
            SysFont_SetMsg(&textScr->fonts[7], SYSMSG_AREA_NAME_UNKNOWN);
        }
    }
#else
    SysFont_SetMsg(&textScr->fonts[7], SYSMSG_AREA_NAMES_START + save->area);
#endif
    SysFont_SetHAlign(&textScr->fonts[7], 0, 147);
    SysFont_DrawCurrentToScreen(&textScr->fonts[7], map + 2, charData + 2, 0);
}

static void Save_textScr_DrawNoSaveInfo(Save_textScr* textScr, u16* map, u16* charData) {
    u16 i;

    for (i = 4; i <= 7; i++) {
        SysFont_SetMsg(&textScr->fonts[i], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScr->fonts[i], 0, 147);
        SysFont_DrawCurrentToScreen(&textScr->fonts[i], map + 2, charData + 2, 0);
    }
}

static void Save_textScr_Draw(Save_textScr* textScr) {
    SaveMenuObject* save          = textScr->save;
    Point           positions[10] = {
        {10,  38},
        {10,  50},
        {10,  62},
        {10,  74},
        {96,  38},
        {96,  50},
        {96,  62},
        {96,  74},
        { 8, 163},
        {96,  27},
    };
#ifdef REGION_USA
    SysCode text[100];
#else
    SysCode text[30];
#endif
    SysCode* fmt;
    u16      hours;
    s32      i;
    u16*     map      = save->resources[5].screenMap;
    u16*     charData = save->resources[5].charData;
    u32      minutes;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 10; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_SAVE_DATE_LABEL);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScr->fonts[1], SYSMSG_SAVE_TIME_LABEL);
    SysFont_SetHAlign(&textScr->fonts[1], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScr->fonts[2], SYSMSG_SAVE_CHAPTER_LABEL);
    SysFont_SetHAlign(&textScr->fonts[2], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScr->fonts[3], SYSMSG_SAVE_AREA_LABEL);
    SysFont_SetHAlign(&textScr->fonts[3], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);

    if (save->message != 0) {
        SysFont_SetMsg(&textScr->fonts[8], save->message);
        SysFont_DrawCurrentToScreen(&textScr->fonts[8], map + 2, charData + 2, 0);
    }

    if (gSaveData.unk_1AB6 & 1) {
        Save_textScr_DrawSaveInfo(textScr, map, charData);
    } else {
        Save_textScr_DrawNoSaveInfo(textScr, map, charData);
    }

    if (!(save->flags & 4)) {
        return;
    }

    if (gSaveData.unk_1D94 >= 1000 * 216000) {
        hours   = 999;
        minutes = 59;
    } else {
        hours   = gSaveData.unk_1D94 / 216000;
        minutes = (u16)((gSaveData.unk_1D94 % 216000) / 3600);
        if (hours > 999) {
            hours = 999;
        }
        if (minutes > 59) {
            minutes = 59;
        }
    }
    fmt = SysFont_GetMsgBuf(&textScr->fonts[9], SYSMSG_SAVE_PLAY_TIME_FMT);
    SysFont_Format(text, fmt, hours, minutes);
    SysFont_SetHAlign(&textScr->fonts[9], 2, 147);
    SysFont_DrawToScreen(&textScr->fonts[9], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
}

static s32 Save_textScr_Init(TaskPool* pool, Task* task, void* args) {
    Save_textScr*      textScr     = task->data;
    Save_textScr_Args* textScrArgs = args;

    textScr->save = textScrArgs->save;
    Save_textScr_InitFonts(textScr);
    Save_textScr_Draw(textScr);
    return 1;
}

static s32 Save_textScr_Update(TaskPool* pool, Task* task, void* args) {
    Save_textScr*   textScr = task->data;
    SaveMenuObject* save    = textScr->save;

    if (save->flags & 1) {
        Save_ReloadBgResource(&save->resources[5], DISPLAY_MAIN, 1, 2, 15, 1);
        Save_textScr_Draw(textScr);
        save->flags &= ~1;
    }
    return 1;
}

static s32 Save_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Save_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_textScr* textScr = task->data;
    s32           i;

    for (i = 0; i < 10; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 Save_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_textScr_Init,
        .update     = Save_textScr_Update,
        .render     = Save_textScr_Render,
        .cleanup    = Save_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Save_textScr_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_textScr_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_textScr, NULL, 0, NULL, &args);
}
