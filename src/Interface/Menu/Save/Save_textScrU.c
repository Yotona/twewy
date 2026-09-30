#include "Engine/Core/Memory.h"
#include "Interface/Menu/Save.h"
#include "Util/SysFont.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x000 */ SaveMenuObject* save;
    /* 0x004 */ SysFont         fonts[22];
} Save_textScrU; // Size: 0xAAC

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ SaveMenuObject* save;
} Save_textScrU_Args;

static s32 Save_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

#ifdef REGION_USA
    #define CARD_GRADE_X 232
    #define CARD_TIME_X  96 // Moved to 142 when there is no time to show
#else
    #define CARD_GRADE_X 231
    #define CARD_TIME_X  142
#endif

// Defined late, after the help-page position template, so the two equal-size .rodata objects sort in
// the original order (handle first).
static const TaskHandle Tsk_Save_textScrU;

#ifdef REGION_USA
    #define CARD_POINTS_MAX 10000
    #define CARD_POINTS_1   7000
    #define CARD_POINTS_2   5000
    #define CARD_POINTS_3   3000
    #define CARD_POINTS_4   2000
    #define CARD_POINTS_5   1000
    #define CARD_POINTS_6   500
    #define CARD_POINTS_7   300
    #define CARD_POINTS_8   200
#else
    #define CARD_POINTS_MAX 0xFFFF
    #define CARD_POINTS_1   32000
    #define CARD_POINTS_2   16000
    #define CARD_POINTS_3   8000
    #define CARD_POINTS_4   4000
    #define CARD_POINTS_5   2000
    #define CARD_POINTS_6   1000
    #define CARD_POINTS_7   500
    #define CARD_POINTS_8   250
#endif

static s32 Save_textScrU_GetEsperRank(u32 points) {
    if (points == CARD_POINTS_MAX) {
        return 0;
    }
    if (points >= CARD_POINTS_1) {
        return 1;
    }
    if (points >= CARD_POINTS_2) {
        return 2;
    }
    if (points >= CARD_POINTS_3) {
        return 3;
    }
    if (points >= CARD_POINTS_4) {
        return 4;
    }
    if (points >= CARD_POINTS_5) {
        return 5;
    }
    if (points >= CARD_POINTS_6) {
        return 6;
    }
    if (points >= CARD_POINTS_7) {
        return 7;
    }
    if (points >= CARD_POINTS_8) {
        return 8;
    }
    if (points >= 100) {
        return 9;
    }
    return 10;
}

static s32 Save_textScrU_GetEsperRankGrade(u32 points) {
    if (points >= CARD_POINTS_MAX) {
        return 0;
    }
    if (points >= CARD_POINTS_1) {
        return 1;
    }
    if (points >= CARD_POINTS_2) {
        return 1;
    }
    if (points >= CARD_POINTS_3) {
        return 1;
    }
    if (points >= CARD_POINTS_4) {
        return 2;
    }
    if (points >= CARD_POINTS_5) {
        return 2;
    }
    if (points >= CARD_POINTS_6) {
        return 3;
    }
    if (points >= CARD_POINTS_7) {
        return 3;
    }
    if (points >= CARD_POINTS_8) {
        return 4;
    }
    return 5;
}

static s32 Save_textScrU_GetEsperPointsGrade(u32 points) {
#ifdef REGION_USA
    if (points >= 10000) {
        return 0;
    }
    if (points >= 5000) {
        return 1;
    }
    if (points >= 3000) {
        return 2;
    }
    if (points >= 2000) {
        return 3;
    }
    if (points >= 1000) {
        return 4;
    }
#else
    if (points >= 50000) {
        return 0;
    }
    if (points >= 40000) {
        return 1;
    }
    if (points >= 30000) {
        return 2;
    }
    if (points >= 20000) {
        return 3;
    }
    if (points >= 10000) {
        return 4;
    }
#endif
    return 5;
}

static u32 Save_textScrU_CalcNoiseReportPercent(u16 count, u32* percent) {
    u32 permille = (u32)(count * 1000) / 96;

    percent[0] = permille / 10;
    percent[1] = permille % 10;
    return permille;
}

static s32 Save_textScrU_GetNoiseReportGrade(u32 permille) {
    if (permille >= 1000) {
        return 0;
    }
    if (permille >= 800) {
        return 1;
    }
    if (permille >= 600) {
        return 2;
    }
    if (permille >= 400) {
        return 3;
    }
    if (permille >= 200) {
        return 4;
    }
    return 5;
}

static u32 Save_textScrU_CalcPinMasteryPercent(u16 count, u32* percent) {
    u32 permille = (u32)(count * 1000) / 304;

    percent[0] = permille / 10;
    percent[1] = permille % 10;
    return permille;
}

static s32 Save_textScrU_GetPinMasteryGrade(u32 permille) {
    if (permille >= 1000) {
        return 0;
    }
    if (permille >= 800) {
        return 1;
    }
    if (permille >= 600) {
        return 2;
    }
    if (permille >= 400) {
        return 3;
    }
    if (permille >= 200) {
        return 4;
    }
    return 5;
}

static u32 Save_textScrU_CalcItemCollectionPercent(u16 count, u32* percent) {
    u32 permille = (u32)(count * 1000) / 472;

    percent[0] = permille / 10;
    percent[1] = permille % 10;
    return permille;
}

static s32 Save_textScrU_GetItemCollectionGrade(u32 permille) {
    if (permille >= 1000) {
        return 0;
    }
    if (permille >= 800) {
        return 1;
    }
    if (permille >= 600) {
        return 2;
    }
    if (permille >= 400) {
        return 3;
    }
    if (permille >= 200) {
        return 4;
    }
    return 5;
}

static s32 Save_textScrU_SplitTime(u32 frames, u32* time) {
    u32 minutes;
    u32 seconds;
    u32 hundredths;

    if (frames >= 360000) {
        minutes    = 99;
        hundredths = 99;
        seconds    = 59;
    } else {
        minutes    = frames / 3600;
        seconds    = (frames % 3600) / 60;
        hundredths = ((frames % 3600) % 60) * 100 / 60;
    }

    if (minutes > 99) {
        minutes = 99;
    }
#ifdef REGION_USA
    time[0] = minutes;
    time[1] = seconds / 10;
    time[2] = seconds % 10;
    time[3] = hundredths / 10;
    time[4] = hundredths % 10;
#else
    time[0] = minutes / 10;
    time[1] = minutes % 10;
    time[2] = seconds / 10;
    time[3] = seconds % 10;
    time[4] = hundredths / 10;
    time[5] = hundredths % 10;
#endif
    return 0;
}

static s32 Save_textScrU_GetTimeAttackGrade(u32 frames) {
    s32 ratio;

    if (frames == 0) {
        return -1;
    }

    ratio = FX_Divide(frames << 12, 70800 << 12);
    if (ratio < 0x1000) {
        return 0;
    }
    if (ratio < 0x1400) {
        return 1;
    }
    if (ratio < 0x1800) {
        return 2;
    }
    if (ratio < 0x1C00) {
        return 3;
    }
    if (ratio < 0x2000) {
        return 4;
    }
    return 5;
}

static void Save_textScrU_InitFonts(Save_textScrU* textScrU) {
    s32 i;

    for (i = 0; i < 22; i++) {
        SysFont_Init(&textScrU->fonts[i]);
        SysFont_SetColor(&textScrU->fonts[i], 14);
    }
}

static void Save_textScrU_DrawCard(Save_textScrU* textScrU) {
    SaveMenuObject* save          = textScrU->save;
    Point           positions[22] = {
        {         133,  3},
        {           8, 15},
        {          96, 15},
        {CARD_GRADE_X, 15},
        {           8, 27},
        {          96, 27},
        {CARD_GRADE_X, 27},
        {           8, 39},
        {          96, 39},
        {         170, 39},
        {CARD_GRADE_X, 39},
        {           8, 51},
        {          96, 51},
        {         170, 51},
        {CARD_GRADE_X, 51},
        {           8, 63},
        {          96, 63},
        {         170, 63},
        {CARD_GRADE_X, 63},
        {           8, 75},
        { CARD_TIME_X, 75},
        {CARD_GRADE_X, 75},
    };
#ifdef REGION_USA
    SysCode text[60];
#else
    SysCode text[30];
#endif
    u32      time[6];
    u32      noisePercent[2];
    u32      pinPercent[2];
    u32      itemPercent[2];
    SysCode* msg;
    s32      rank;
    s32      grade;
    s32      i;
    u16*     map      = save->resources[0].screenMap;
    u16*     charData = save->resources[0].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 22; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    msg = SysFont_GetOwnerName();
    SysFont_Format(text, msg);
    SysFont_SetHAlign(&textScrU->fonts[0], 0, 116);
    SysFont_DrawToScreen(&textScrU->fonts[0], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    rank  = Save_textScrU_GetEsperRank(save->esperPoints);
    grade = Save_textScrU_GetEsperRankGrade(save->esperPoints);
    SysFont_SetMsg(&textScrU->fonts[1], SYSMSG_CARD_ESPER_RANK);
    SysFont_SetHAlign(&textScrU->fonts[1], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[1], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScrU->fonts[2], SYSMSG_CARD_ESPER_RANKS_START + rank);
    SysFont_SetHAlign(&textScrU->fonts[2], 2, 133);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScrU->fonts[3], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[3], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[3], map + 2, charData + 2, 0);

    grade = Save_textScrU_GetEsperPointsGrade(save->esperPoints);
    SysFont_SetMsg(&textScrU->fonts[4], SYSMSG_CARD_ESPER_POINTS);
    SysFont_SetHAlign(&textScrU->fonts[4], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[4], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (save->esperPoints == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_CARD_POINTS_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_CARD_POINTS_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_CARD_POINTS_FMT);
#endif
    SysFont_Format(text, msg, save->esperPoints);
    SysFont_SetHAlign(&textScrU->fonts[5], 2, 133);
    SysFont_DrawToScreen(&textScrU->fonts[5], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    SysFont_SetMsg(&textScrU->fonts[6], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[6], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[6], map + 2, charData + 2, 0);

    grade = Save_textScrU_GetNoiseReportGrade(Save_textScrU_CalcNoiseReportPercent(save->noiseReportCount, noisePercent));
    SysFont_SetMsg(&textScrU->fonts[7], SYSMSG_CARD_NOISE_REPORT);
    SysFont_SetHAlign(&textScrU->fonts[7], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[7], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (save->noiseReportCount == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_CARD_TYPES_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_CARD_TYPES_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_CARD_TYPES_FMT);
#endif
    SysFont_Format(text, msg, save->noiseReportCount);
    SysFont_SetHAlign(&textScrU->fonts[8], 2, 74);
    SysFont_DrawToScreen(&textScrU->fonts[8], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    msg = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_CARD_PERCENT_FMT);
    SysFont_Format(text, msg, noisePercent[0], noisePercent[1]);
    SysFont_SetHAlign(&textScrU->fonts[9], 2, 59);
    SysFont_DrawToScreen(&textScrU->fonts[9], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    SysFont_SetMsg(&textScrU->fonts[10], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[10], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[10], map + 2, charData + 2, 0);

    grade = Save_textScrU_GetPinMasteryGrade(Save_textScrU_CalcPinMasteryPercent(save->pinsMastered, pinPercent));
    SysFont_SetMsg(&textScrU->fonts[11], SYSMSG_CARD_PIN_MASTERY);
    SysFont_SetHAlign(&textScrU->fonts[11], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[11], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (save->pinsMastered == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_CARD_TYPES_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_CARD_TYPES_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_CARD_TYPES_FMT);
#endif
    SysFont_Format(text, msg, save->pinsMastered);
    SysFont_SetHAlign(&textScrU->fonts[12], 2, 74);
    SysFont_DrawToScreen(&textScrU->fonts[12], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    msg = SysFont_GetMsgBuf(&textScrU->fonts[13], SYSMSG_CARD_PERCENT_FMT);
    SysFont_Format(text, msg, pinPercent[0], pinPercent[1]);
    SysFont_SetHAlign(&textScrU->fonts[13], 2, 59);
    SysFont_DrawToScreen(&textScrU->fonts[13], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    SysFont_SetMsg(&textScrU->fonts[14], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[14], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[14], map + 2, charData + 2, 0);

    grade = Save_textScrU_GetItemCollectionGrade(Save_textScrU_CalcItemCollectionPercent(save->itemsCollected, itemPercent));
    SysFont_SetMsg(&textScrU->fonts[15], SYSMSG_CARD_ITEM_COLLECTION);
    SysFont_SetHAlign(&textScrU->fonts[15], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[15], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (save->itemsCollected == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[16], SYSMSG_CARD_TYPES_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[16], SYSMSG_CARD_TYPES_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[16], SYSMSG_CARD_TYPES_FMT);
#endif
    SysFont_Format(text, msg, save->itemsCollected);
    SysFont_SetHAlign(&textScrU->fonts[16], 2, 74);
    SysFont_DrawToScreen(&textScrU->fonts[16], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    msg = SysFont_GetMsgBuf(&textScrU->fonts[17], SYSMSG_CARD_PERCENT_FMT);
    SysFont_Format(text, msg, itemPercent[0], itemPercent[1]);
    SysFont_SetHAlign(&textScrU->fonts[17], 2, 59);
    SysFont_DrawToScreen(&textScrU->fonts[17], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    SysFont_SetMsg(&textScrU->fonts[18], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[18], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[18], map + 2, charData + 2, 0);

    Save_textScrU_SplitTime(save->timeAttackFrames, time);
    grade = Save_textScrU_GetTimeAttackGrade(save->timeAttackFrames);
    SysFont_SetMsg(&textScrU->fonts[19], SYSMSG_CARD_TIME_ATTACK);
#ifdef REGION_USA
    SysFont_SetHAlign(&textScrU->fonts[19], 0, 86);
#else
    SysFont_SetHAlign(&textScrU->fonts[19], 0, 132);
#endif
    SysFont_DrawCurrentToScreen(&textScrU->fonts[19], map + 2, charData + 2, 0);
    if (grade == -1) {
#ifdef REGION_USA
        SysFont_SetPos(&textScrU->fonts[20], 142, 75);
#endif
        SysFont_SetMsg(&textScrU->fonts[20], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[20], 0, 87);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[20], map + 2, charData + 2, 0);
        SysFont_SetMsg(&textScrU->fonts[21], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[21], 0, 13);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[21], map + 2, charData + 2, 0);
        return;
    }
    msg = SysFont_GetMsgBuf(&textScrU->fonts[20], SYSMSG_CARD_TIME_FMT);
#ifdef REGION_USA
    SysFont_Format(text, msg, time[0], time[1], time[2], time[3], time[4]);
    SysFont_SetHAlign(&textScrU->fonts[20], 2, 133);
#else
    SysFont_Format(text, msg, time[0], time[1], time[2], time[3], time[4], time[5]);
    SysFont_SetHAlign(&textScrU->fonts[20], 0, 87);
#endif
    SysFont_DrawToScreen(&textScrU->fonts[20], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    SysFont_SetMsg(&textScrU->fonts[21], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[21], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[21], map + 2, charData + 2, 0);
}

static void Save_textScrU_DrawHelp(Save_textScrU* textScrU) {
    SaveMenuObject* save = textScrU->save;
#ifdef REGION_USA
    SysCode text[60];
#else
    SysCode text[30];
#endif
    Point positions[3] = {
        {24, 11},
        {24, 10},
        {14, 27},
    };
    SysCode* msg;
    u16      i;
    u16*     map      = save->resources[0].screenMap;
    u16*     charData = save->resources[0].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 3; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScrU->fonts[0], SYSMSG_SAVEMENU_HELP_LABELS + save->helpPage);
    SysFont_SetHAlign(&textScrU->fonts[0], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);

    msg = SysFont_GetMsgBuf(&textScrU->fonts[1], SYSMSG_DIVIDED_U32S);
    SysFont_Format(text, msg, save->helpPage + 1, 4);
    SysFont_SetHAlign(&textScrU->fonts[1], 2, 220);
    SysFont_DrawToScreen(&textScrU->fonts[1], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    SysFont_SetMsg(&textScrU->fonts[2], SYSMSG_SAVEMENU_HELP_TEXT + save->helpPage);
    SysFont_SetHAlign(&textScrU->fonts[2], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
}

static s32 Save_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    Save_textScrU_Args* textScrUArgs = args;
    Save_textScrU*      textScrU     = task->data;

    textScrU->save = textScrUArgs->save;
    Save_textScrU_InitFonts(textScrU);
    Save_textScrU_DrawCard(textScrU);
    return 1;
}

static s32 Save_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    Save_textScrU*  textScrU = task->data;
    SaveMenuObject* save     = textScrU->save;

    if (save->flags & 2) {
        Save_ReloadBgResource(&save->resources[0], DISPLAY_SUB, 0, 6, 15, 1);
        if (save->helpOpen == 0) {
            Save_textScrU_DrawCard(textScrU);
        } else {
            Save_textScrU_DrawHelp(textScrU);
        }
        save->flags &= ~2;
    }
    return 1;
}

static s32 Save_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Save_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    Save_textScrU* textScrU = task->data;
    s32            i;

    for (i = 0; i < 22; i++) {
        SysFont_Destroy(&textScrU->fonts[i]);
    }
    return 1;
}

static s32 Save_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Save_textScrU_Init,
        .update     = Save_textScrU_Update,
        .render     = Save_textScrU_Render,
        .cleanup    = Save_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Save_textScrU = {"Tsk_Save_textScrU", Save_textScrU_RunTask, sizeof(Save_textScrU)};

s32 Save_textScrU_CreateTask(TaskPool* pool, s32 dataType, SaveMenuObject* save) {
    Save_textScrU_Args args;

    args.dataType = dataType;
    args.save     = save;

    return EasyTask_CreateTask(pool, &Tsk_Save_textScrU, NULL, 0, NULL, &args);
}
