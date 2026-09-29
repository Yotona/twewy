#include "Engine/Core/Memory.h"
#include "Interface/Menu/TusinSet.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ TusinSetObject* tusinSet;
    /* 0x004 */ SysFont         fonts[5];
} TusinSet_textScr; // Size: 0x270

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_textScr_Args;

static s32 TusinSet_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_textScr = {"Tsk_TusinSet_textScr", TusinSet_textScr_RunTask, sizeof(TusinSet_textScr)};

static void TusinSet_textScr_InitFonts(TusinSet_textScr* textScr) {
    Point positions[5] = {
        {10,  27},
        {60,  43},
        {60,  58},
        { 6,  86},
        { 6, 163},
    };
    s32 i;

    for (i = 0; i < 5; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void TusinSet_textScr_Draw(TusinSet_textScr* textScr) {
    TusinSetObject* tusinSet = textScr->tusinSet;
#ifdef REGION_USA
    SysCode nameText[100];
    SysCode commentText[100];
#else
    SysCode nameText[50];
    SysCode commentText[50];
#endif
    u16*         map      = tusinSet->resources[6].screenMap;
    u16*         charData = tusinSet->resources[6].charData;
    SysCode*     msg;
    u16          index;
    ItemCategory category;
    u16          msgIndex;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_MINGLE_REVIEW_INFO);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

    msg = SysFont_GetOwnerName();
    SysFont_Format(nameText, msg);
    SysFont_DrawToScreen(&textScr->fonts[1], nameText, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    msg = SysFont_GetUserDSCommentBufReturn();
    SysFont_Format(commentText, msg);
    SysFont_DrawToScreen(&textScr->fonts[2], commentText, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, msg);

    index    = Inventory_GetCategorizedIndex(tusinSet->selected.itemId);
    category = Inventory_GetCategory(tusinSet->selected.itemId);
    if (index != 0xFFFF) {
        if (category == 1) {
            msgIndex = SYSMSG_THREAD_NAMES_START + index;
        } else if (category == 2) {
            msgIndex = SYSMSG_FOOD_NAMES_START + index;
        } else {
            msgIndex = SYSMSG_SWAG_NAMES_START + index;
        }
        SysFont_SetMsg(&textScr->fonts[3], msgIndex);
        SysFont_SetHAlign(&textScr->fonts[3], 0, 117);
        SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);
    }

    SysFont_SetMsg(&textScr->fonts[4], SYSMSG_MINGLE_SEND_NOTICE);
    SysFont_DrawCurrentToScreen(&textScr->fonts[4], map + 2, charData + 2, 0);
}

static s32 TusinSet_textScr_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_textScr_Args* textScrArgs = args;
    TusinSet_textScr*      textScr     = task->data;

    textScr->tusinSet = textScrArgs->tusinSet;
    TusinSet_textScr_InitFonts(textScr);
    TusinSet_textScr_Draw(textScr);
    return 1;
}

static s32 TusinSet_textScr_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_textScr* textScr  = task->data;
    TusinSetObject*   tusinSet = textScr->tusinSet;

    if (tusinSet->flags & 1) {
        TusinSet_ReloadBgResource(&tusinSet->resources[6], DISPLAY_MAIN, 2, 1, 15, 1);
        TusinSet_textScr_Draw(textScr);
        tusinSet->flags &= ~1;
    }
    return 1;
}

static s32 TusinSet_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 TusinSet_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_textScr* textScr = task->data;
    s32               i;

    for (i = 0; i < 5; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 TusinSet_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_textScr_Init,
        .update     = TusinSet_textScr_Update,
        .render     = TusinSet_textScr_Render,
        .cleanup    = TusinSet_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_textScr_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_textScr_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_textScr, NULL, 0, NULL, &args);
}
