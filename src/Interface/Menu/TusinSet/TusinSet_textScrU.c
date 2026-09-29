#include "Engine/Core/Memory.h"
#include "Interface/Menu/TusinSet.h"
#include "Util/SysFont.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x000 */ TusinSetObject* tusinSet;
    /* 0x004 */ SysFont         fonts[22];
} TusinSet_textScrU; // Size: 0xAAC

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_textScrU_Args;

static s32 TusinSet_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

#ifdef REGION_USA
    #define CARD_GRADE_X 232
    #define CARD_TIME_X  96 // Moved to 142 when there is no time to show
#else
    #define CARD_GRADE_X 231
    #define CARD_TIME_X  142
#endif

// Defined late, after the help-page position template, so the two equal-size .rodata objects sort in
// the original order.
static const TaskHandle Tsk_TusinSet_textScrU;

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

static s32 TusinSet_textScrU_GetEsperRank(u32 points) {
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

static s32 TusinSet_textScrU_GetEsperRankGrade(u32 points) {
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

static s32 TusinSet_textScrU_GetEsperPointsGrade(u32 points) {
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

static u32 TusinSet_textScrU_CalcNoiseReportPercent(u16 count, u32* percent) {
    u32 permille = (u32)(count * 1000) / 96;

    percent[0] = permille / 10;
    percent[1] = permille % 10;
    return permille;
}

static s32 TusinSet_textScrU_GetNoiseReportGrade(u32 permille) {
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

static u32 TusinSet_textScrU_CalcPinMasteryPercent(u16 count, u32* percent) {
    u32 permille = (u32)(count * 1000) / 304;

    percent[0] = permille / 10;
    percent[1] = permille % 10;
    return permille;
}

static s32 TusinSet_textScrU_GetPinMasteryGrade(u32 permille) {
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

static u32 TusinSet_textScrU_CalcItemCollectionPercent(u16 count, u32* percent) {
    u32 permille = (u32)(count * 1000) / 472;

    percent[0] = permille / 10;
    percent[1] = permille % 10;
    return permille;
}

static s32 TusinSet_textScrU_GetItemCollectionGrade(u32 permille) {
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

static s32 TusinSet_textScrU_SplitTime(u32 frames, u32* time) {
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

static s32 TusinSet_textScrU_GetTimeAttackGrade(u32 frames) {
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

static void TusinSet_textScrU_InitFonts(TusinSet_textScrU* textScrU) {
    s32 i;

    for (i = 0; i < 22; i++) {
        SysFont_Init(&textScrU->fonts[i]);
        SysFont_SetColor(&textScrU->fonts[i], 14);
    }
}

static void TusinSet_textScrU_DrawCard(TusinSet_textScrU* textScrU) {
    TusinSetObject* tusinSet      = textScrU->tusinSet;
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
    u16*     map      = tusinSet->resources[0].screenMap;
    u16*     charData = tusinSet->resources[0].charData;

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

    rank  = TusinSet_textScrU_GetEsperRank(tusinSet->esperPoints);
    grade = TusinSet_textScrU_GetEsperRankGrade(tusinSet->esperPoints);
    SysFont_SetMsg(&textScrU->fonts[1], SYSMSG_CARD_ESPER_RANK);
    SysFont_SetHAlign(&textScrU->fonts[1], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[1], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScrU->fonts[2], SYSMSG_CARD_ESPER_RANKS_START + rank);
    SysFont_SetHAlign(&textScrU->fonts[2], 2, 133);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
    SysFont_SetMsg(&textScrU->fonts[3], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[3], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[3], map + 2, charData + 2, 0);

    grade = TusinSet_textScrU_GetEsperPointsGrade(tusinSet->esperPoints);
    SysFont_SetMsg(&textScrU->fonts[4], SYSMSG_CARD_ESPER_POINTS);
    SysFont_SetHAlign(&textScrU->fonts[4], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[4], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (tusinSet->esperPoints == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_CARD_POINTS_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_CARD_POINTS_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_CARD_POINTS_FMT);
#endif
    SysFont_Format(text, msg, tusinSet->esperPoints);
    SysFont_SetHAlign(&textScrU->fonts[5], 2, 133);
    SysFont_DrawToScreen(&textScrU->fonts[5], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
    SysFont_SetMsg(&textScrU->fonts[6], SYSMSG_CARD_GRADES_START + grade);
    SysFont_SetHAlign(&textScrU->fonts[6], 0, 13);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[6], map + 2, charData + 2, 0);

    grade = TusinSet_textScrU_GetNoiseReportGrade(
        TusinSet_textScrU_CalcNoiseReportPercent(tusinSet->noiseReportCount, noisePercent));
    SysFont_SetMsg(&textScrU->fonts[7], SYSMSG_CARD_NOISE_REPORT);
    SysFont_SetHAlign(&textScrU->fonts[7], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[7], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (tusinSet->noiseReportCount == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_CARD_TYPES_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_CARD_TYPES_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_CARD_TYPES_FMT);
#endif
    SysFont_Format(text, msg, tusinSet->noiseReportCount);
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

    grade = TusinSet_textScrU_GetPinMasteryGrade(TusinSet_textScrU_CalcPinMasteryPercent(tusinSet->pinsMastered, pinPercent));
    SysFont_SetMsg(&textScrU->fonts[11], SYSMSG_CARD_PIN_MASTERY);
    SysFont_SetHAlign(&textScrU->fonts[11], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[11], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (tusinSet->pinsMastered == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_CARD_TYPES_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_CARD_TYPES_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[12], SYSMSG_CARD_TYPES_FMT);
#endif
    SysFont_Format(text, msg, tusinSet->pinsMastered);
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

    grade = TusinSet_textScrU_GetItemCollectionGrade(
        TusinSet_textScrU_CalcItemCollectionPercent(tusinSet->itemsCollected, itemPercent));
    SysFont_SetMsg(&textScrU->fonts[15], SYSMSG_CARD_ITEM_COLLECTION);
    SysFont_SetHAlign(&textScrU->fonts[15], 0, 86);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[15], map + 2, charData + 2, 0);
#ifdef REGION_USA
    if (tusinSet->itemsCollected == 1) {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[16], SYSMSG_CARD_TYPES_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScrU->fonts[16], SYSMSG_CARD_TYPES_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScrU->fonts[16], SYSMSG_CARD_TYPES_FMT);
#endif
    SysFont_Format(text, msg, tusinSet->itemsCollected);
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

    TusinSet_textScrU_SplitTime(tusinSet->timeAttackFrames, time);
    grade = TusinSet_textScrU_GetTimeAttackGrade(tusinSet->timeAttackFrames);
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

static void TusinSet_textScrU_DrawHelp(TusinSet_textScrU* textScrU) {
    TusinSetObject* tusinSet = textScrU->tusinSet;
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
    u16*     map      = tusinSet->resources[0].screenMap;
    u16*     charData = tusinSet->resources[0].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 3; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScrU->fonts[0], SYSMSG_MINGLEMENU_HELP_LABELS + tusinSet->helpPage);
    SysFont_SetHAlign(&textScrU->fonts[0], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);

    msg = SysFont_GetMsgBuf(&textScrU->fonts[1], SYSMSG_DIVIDED_U32S);
    SysFont_Format(text, msg, tusinSet->helpPage + 1, 7);
    SysFont_SetHAlign(&textScrU->fonts[1], 2, 220);
    SysFont_DrawToScreen(&textScrU->fonts[1], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    SysFont_SetMsg(&textScrU->fonts[2], SYSMSG_MINGLEMENU_HELP_TEXT + tusinSet->helpPage);
    SysFont_SetHAlign(&textScrU->fonts[2], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
}

static s32 TusinSet_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_textScrU_Args* textScrUArgs = args;
    TusinSet_textScrU*      textScrU     = task->data;

    textScrU->tusinSet = textScrUArgs->tusinSet;
    TusinSet_textScrU_InitFonts(textScrU);
    TusinSet_textScrU_DrawCard(textScrU);
    return 1;
}

static s32 TusinSet_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_textScrU* textScrU = task->data;
    TusinSetObject*    tusinSet = textScrU->tusinSet;

    if (tusinSet->flags & 2) {
        TusinSet_ReloadBgResource(&tusinSet->resources[0], DISPLAY_SUB, 0, 6, 15, 1);
        if (tusinSet->helpOpen == 0) {
            TusinSet_textScrU_DrawCard(textScrU);
        } else {
            TusinSet_textScrU_DrawHelp(textScrU);
        }
        tusinSet->flags &= ~2;
    }
    return 1;
}

static s32 TusinSet_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 TusinSet_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_textScrU* textScrU = task->data;
    s32                i;

    for (i = 0; i < 22; i++) {
        SysFont_Destroy(&textScrU->fonts[i]);
    }
    return 1;
}

static s32 TusinSet_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_textScrU_Init,
        .update     = TusinSet_textScrU_Update,
        .render     = TusinSet_textScrU_Render,
        .cleanup    = TusinSet_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_TusinSet_textScrU = {"Tsk_TusinSet_textScrU", TusinSet_textScrU_RunTask,
                                                 sizeof(TusinSet_textScrU)};

s32 TusinSet_textScrU_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_textScrU_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_textScrU, NULL, 0, NULL, &args);
}
