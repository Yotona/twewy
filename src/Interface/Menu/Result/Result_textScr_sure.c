#include "Interface/Menu/Result.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x00 */ ResultObject* owner;
    /* 0x04 */ SysFont       fonts[7];
} Result_textScr_sure; // Size: 0x368

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
} Result_textScr_sure_Args;

static s32 Result_textScr_sure_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_textScr_sure = {"Tsk_Result_textScr_sure", Result_textScr_sure_RunTask,
                                                   sizeof(Result_textScr_sure)};

static void Result_textScr_sure_InitFonts(Result_textScr_sure* textScr) {
    for (s32 i = 0; i < 7; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void Result_textScr_sure_Draw(Result_textScr_sure* textScr) {
    ResultObject* owner = textScr->owner;
#ifdef REGION_USA
    SysCode text[200];
#else
    SysCode text[50];
#endif
    s16 positions[7][2] = {
        { 34, 37},
        { 38, 66},
        { 38, 81},
        { 38, 96},
        {137, 66},
        {137, 81},
        {137, 96},
    };
    s32      i;
    u16*     map      = owner->bgResources[5].screenMap;
    u16*     charData = owner->bgResources[5].charData;
    SysCode* msg;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 7; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i][0], positions[i][1]);
    }

#ifdef REGION_USA
    if (owner->espersMet + owner->civviesMet + owner->aliensMet == 1) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[0], SYSMSG_RESULT_MINGLE_COUNT_SINGULAR);
    } else {
        msg = SysFont_GetMsgBuf(&textScr->fonts[0], SYSMSG_RESULT_MINGLE_COUNT_PLURAL);
    }
#else
    msg = SysFont_GetMsgBuf(&textScr->fonts[0], SYSMSG_RESULT_MINGLE_COUNT);
#endif

    SysFont_Format(text, msg, owner->espersMet + owner->civviesMet + owner->aliensMet);
    SysFont_SetColor(&textScr->fonts[0], 8);
    SysFont_DrawToScreen(&textScr->fonts[0], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    SysFont_SetMsg(&textScr->fonts[1], SYSMSG_MINGLE_ESPERS);
    SysFont_SetHAlign(&textScr->fonts[1], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[2], SYSMSG_MINGLE_CIVVIES);
    SysFont_SetHAlign(&textScr->fonts[2], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[2], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[3], SYSMSG_MINGLE_ALIENS);
    SysFont_SetHAlign(&textScr->fonts[3], 0, 84);
    SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);

    if (owner->espersMet != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[4], SYSMSG_COUNT_FMT);
        SysFont_Format(text, msg, owner->espersMet);
        SysFont_SetHAlign(&textScr->fonts[4], 0, 31);
        SysFont_SetColor(&textScr->fonts[4], 12);
        SysFont_DrawToScreen(&textScr->fonts[4], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        SysFont_SetMsg(&textScr->fonts[4], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScr->fonts[4], 0, 31);
        SysFont_DrawCurrentToScreen(&textScr->fonts[4], map + 2, charData + 2, 0);
    }

    if (owner->civviesMet != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[5], SYSMSG_COUNT_FMT);
        SysFont_Format(text, msg, owner->civviesMet);
        SysFont_SetHAlign(&textScr->fonts[5], 0, 31);
        SysFont_SetColor(&textScr->fonts[5], 12);
        SysFont_DrawToScreen(&textScr->fonts[5], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        SysFont_SetMsg(&textScr->fonts[5], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScr->fonts[5], 0, 31);
        SysFont_DrawCurrentToScreen(&textScr->fonts[5], map + 2, charData + 2, 0);
    }

    if (owner->aliensMet != 0) {
        msg = SysFont_GetMsgBuf(&textScr->fonts[6], SYSMSG_COUNT_FMT);
        SysFont_Format(text, msg, owner->aliensMet);
        SysFont_SetHAlign(&textScr->fonts[6], 0, 31);
        SysFont_SetColor(&textScr->fonts[6], 12);
        SysFont_DrawToScreen(&textScr->fonts[6], text, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, msg);
    } else {
        SysFont_SetMsg(&textScr->fonts[6], SYSMSG_COUNT_NONE);
        SysFont_SetHAlign(&textScr->fonts[6], 0, 31);
        SysFont_DrawCurrentToScreen(&textScr->fonts[6], map + 2, charData + 2, 0);
    }
}

static s32 Result_textScr_sure_Init(TaskPool* pool, Task* task, void* args) {
    Result_textScr_sure*      textScr     = task->data;
    Result_textScr_sure_Args* textScrArgs = args;

    textScr->owner = textScrArgs->owner;
    Result_textScr_sure_InitFonts(textScr);
    Result_textScr_sure_Draw(textScr);
    return 1;
}

static s32 Result_textScr_sure_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Result_textScr_sure_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Result_textScr_sure_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_textScr_sure* textScr = task->data;

    for (s32 i = 0; i < 7; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 Result_textScr_sure_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_textScr_sure_Init,
        .update     = Result_textScr_sure_Update,
        .render     = Result_textScr_sure_Render,
        .cleanup    = Result_textScr_sure_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_textScr_sure_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_textScr_sure_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_textScr_sure, NULL, 0, NULL, &args);
}
