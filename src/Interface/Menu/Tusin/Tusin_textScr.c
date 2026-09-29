#include "Engine/Core/Memory.h"
#include "Interface/Menu/Tusin.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ TusinObject* tusin;
    /* 0x004 */ SysFont      fonts[11];
} Tusin_textScr; // Size: 0x558

typedef struct {
    /* 0x0 */ s32          dataType;
    /* 0x4 */ TusinObject* tusin;
} Tusin_textScr_Args;

static s32 Tusin_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Tusin_textScr = {"Tsk_Tusin_textScr", Tusin_textScr_RunTask, sizeof(Tusin_textScr)};

static void Tusin_textScr_InitFonts(Tusin_textScr* textScr) {
    Point positions[11] = {
        { 34,   8},
        { 64,  53},
        { 64,  68},
        { 64,  83},
        {162,  53},
        {162,  68},
        {162,  83},
        { 32,  96},
        { 88, 116},
        { 88, 145},
        { 88, 174},
    };
    s32 i;

    for (i = 0; i < 11; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void Tusin_textScr_Draw(Tusin_textScr* textScr) {
    TusinObject* tusin = textScr->tusin;
#ifdef REGION_USA
    SysCode text[100];
#else
    SysCode text[30];
#endif
    u16*     map      = tusin->resources[6].screenMap;
    u16*     charData = tusin->resources[6].charData;
    SysCode* msg;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    SysFont_SetMsg(&textScr->fonts[0], tusin->statusMsg);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[1], SYSMSG_MINGLE_ESPERS);
    SysFont_SetHAlign(&textScr->fonts[1], 0, 83);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[2], SYSMSG_MINGLE_CIVVIES);
    SysFont_SetHAlign(&textScr->fonts[2], 0, 83);
    SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[3], SYSMSG_MINGLE_ALIENS);
    SysFont_SetHAlign(&textScr->fonts[3], 0, 83);
    SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);

    if (tusin->espersMet != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[4], SYSMSG_COUNT_FMT);
        SysFont_Format(text, msg, tusin->espersMet);
        SysFont_SetHAlign(&textScr->fonts[4], 0, 30);
        SysFont_SetColor(&textScr->fonts[4], 12);
        SysFont_DrawToScreen(&textScr->fonts[4], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        SysFont_SetMsg(&textScr->fonts[4], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScr->fonts[4], 0, 30);
        SysFont_DrawCurrentToScreen(&textScr->fonts[4], map + 2, charData + 2, 0);
    }

    if (tusin->civviesMet != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[5], SYSMSG_COUNT_FMT);
        SysFont_Format(text, msg, tusin->civviesMet);
        SysFont_SetHAlign(&textScr->fonts[5], 0, 30);
        SysFont_SetColor(&textScr->fonts[5], 12);
        SysFont_DrawToScreen(&textScr->fonts[5], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        SysFont_SetMsg(&textScr->fonts[5], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScr->fonts[5], 0, 30);
        SysFont_DrawCurrentToScreen(&textScr->fonts[5], map + 2, charData + 2, 0);
    }

    if (tusin->aliensMet != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[6], SYSMSG_COUNT_FMT);
        SysFont_Format(text, msg, tusin->aliensMet);
        SysFont_SetHAlign(&textScr->fonts[6], 0, 30);
        SysFont_SetColor(&textScr->fonts[6], 12);
        SysFont_DrawToScreen(&textScr->fonts[6], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        SysFont_SetMsg(&textScr->fonts[6], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScr->fonts[6], 0, 30);
        SysFont_DrawCurrentToScreen(&textScr->fonts[6], map + 2, charData + 2, 0);
    }

#ifdef REGION_USA
    if (tusin->mingleRemaining == 1) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[7], SYSMSG_MINGLE_REMAINING_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScr->fonts[7], SYSMSG_MINGLE_REMAINING_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScr->fonts[7], SYSMSG_MINGLE_REMAINING);
#endif
    SysFont_Format(text, msg, tusin->mingleRemaining);
    SysFont_SetHAlign(&textScr->fonts[7], 0, 193);
    SysFont_DrawToScreen(&textScr->fonts[7], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    SysFont_SetMsg(&textScr->fonts[8], SYSMSG_MINGLE_POWER_LIGHT_HINT);
    SysFont_SetColor(&textScr->fonts[8], 8);
    SysFont_DrawCurrentToScreen(&textScr->fonts[8], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[9], SYSMSG_MINGLE_COUNT_HINT);
    SysFont_SetColor(&textScr->fonts[9], 8);
    SysFont_DrawCurrentToScreen(&textScr->fonts[9], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[10], SYSMSG_MINGLE_END_AND_SAVE);
    SysFont_SetColor(&textScr->fonts[10], 8);
    SysFont_DrawCurrentToScreen(&textScr->fonts[10], map + 2, charData + 2, 0);
}

static s32 Tusin_textScr_Init(TaskPool* pool, Task* task, void* args) {
    Tusin_textScr*      textScr  = task->data;
    Tusin_textScr_Args* initArgs = args;

    textScr->tusin = initArgs->tusin;
    Tusin_textScr_InitFonts(textScr);
    Tusin_textScr_Draw(textScr);
    return 1;
}

static s32 Tusin_textScr_Update(TaskPool* pool, Task* task, void* args) {
    Tusin_textScr* textScr = task->data;
    TusinObject*   tusin   = textScr->tusin;

    if (tusin->flags & 1) {
        Tusin_ReloadBgResource(&tusin->resources[6], DISPLAY_SUB, 2, 1, 15, 1);
        Tusin_textScr_Draw(textScr);
        tusin->flags &= ~1;
    }
    return 1;
}

static s32 Tusin_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Tusin_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    Tusin_textScr* textScr = task->data;

    for (s32 i = 0; i < 11; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 Tusin_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Tusin_textScr_Init,
        .update     = Tusin_textScr_Update,
        .render     = Tusin_textScr_Render,
        .cleanup    = Tusin_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Tusin_textScr_CreateTask(TaskPool* pool, s32 dataType, TusinObject* tusin) {
    Tusin_textScr_Args args;

    args.dataType = dataType;
    args.tusin    = tusin;

    return EasyTask_CreateTask(pool, &Tsk_Tusin_textScr, NULL, 0, NULL, &args);
}
