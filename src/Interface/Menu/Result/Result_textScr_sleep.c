#include "Interface/Menu/Result.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x00 */ ResultObject* owner;
    /* 0x04 */ SysFont       fonts[4];
} Result_textScr_sleep; // Size: 0x1F4

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
} Result_textScr_sleep_Args;

static s32 Result_textScr_sleep_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_textScr_sleep = {"Tsk_Result_textScr_sleep", Result_textScr_sleep_RunTask,
                                                    sizeof(Result_textScr_sleep)};

static void Result_textScr_sleep_InitFonts(Result_textScr_sleep* textScr) {
    for (s32 i = 0; i < 4; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void Result_textScr_sleep_Draw(Result_textScr_sleep* textScr) {
    ResultObject* owner = textScr->owner;
#ifdef REGION_USA
    SysCode text[200];
#else
    SysCode text[50];
#endif
    s16 positions[4][2] = {
        { 34, 37},
        { 32, 66},
        {200, 66},
        { 70, 66},
    };
    s32      i;
    u16*     map      = owner->bgResources[5].screenMap;
    u16*     charData = owner->bgResources[5].charData;
    SysCode* msg;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 4; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i][0], positions[i][1]);
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_RESULT_SLEEP_PP_INFO);
    SysFont_SetColor(&textScr->fonts[0], 8);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

#ifdef REGION_USA
    if (owner->elapsedDays != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_RESULT_SLEEP_TOTAL_DHM);
        SysFont_Format(text, msg, owner->elapsedDays, owner->elapsedHours, owner->elapsedMinutes);
    } else {
        msg = SysFont_GetMsgBuf(&textScr->fonts[1], SYSMSG_RESULT_SLEEP_TOTAL_HMS);
        SysFont_Format(text, msg, owner->elapsedHours, owner->elapsedMinutes, owner->elapsedSeconds);
    }

    SysFont_SetHAlign(&textScr->fonts[1], 0, 191);
    SysFont_DrawToScreen(&textScr->fonts[1], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);
#else
    SysFont_SetMsg(&textScr->fonts[1], SYSMSG_RESULT_SLEEP_SINCE);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[2], SYSMSG_RESULT_SLEEP_ELAPSED);
    SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);

    if (owner->elapsedDays != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[3], SYSMSG_RESULT_SLEEP_DHM);
        SysFont_Format(text, msg, owner->elapsedDays, owner->elapsedHours, owner->elapsedMinutes);
        SysFont_SetHAlign(&textScr->fonts[3], 0, 130);
        SysFont_DrawToScreen(&textScr->fonts[3], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        msg = SysFont_GetMsgBuf(&textScr->fonts[3], SYSMSG_RESULT_SLEEP_HMS);
        SysFont_Format(text, msg, owner->elapsedHours, owner->elapsedMinutes, owner->elapsedSeconds);
        SysFont_SetHAlign(&textScr->fonts[3], 0, 130);
        SysFont_DrawToScreen(&textScr->fonts[3], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    }
#endif
}

static s32 Result_textScr_sleep_Init(TaskPool* pool, Task* task, void* args) {
    Result_textScr_sleep*      textScr     = task->data;
    Result_textScr_sleep_Args* textScrArgs = args;

    textScr->owner = textScrArgs->owner;
    Result_textScr_sleep_InitFonts(textScr);
    Result_textScr_sleep_Draw(textScr);
    return 1;
}

static s32 Result_textScr_sleep_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Result_textScr_sleep_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Result_textScr_sleep_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_textScr_sleep* textScr = task->data;

    for (s32 i = 0; i < 4; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 Result_textScr_sleep_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_textScr_sleep_Init,
        .update     = Result_textScr_sleep_Update,
        .render     = Result_textScr_sleep_Render,
        .cleanup    = Result_textScr_sleep_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_textScr_sleep_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_textScr_sleep_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_textScr_sleep, NULL, 0, NULL, &args);
}
