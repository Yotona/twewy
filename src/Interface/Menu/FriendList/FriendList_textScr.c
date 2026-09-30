#include "Engine/Core/Memory.h"
#include "Interface/Menu/FriendList.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ FriendListObject* friendList;
    /* 0x004 */ SysFont           fonts[7];
    /* 0x368 */ char              unk_368[0x4DC - 0x368];
} FriendList_textScr; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
} FriendList_textScr_Args;

static s32 FriendList_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static void FriendList_textScr_InitFonts(FriendList_textScr* textScr) {
    s32 i;

    for (i = 0; i < 7; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void FriendList_textScr_DrawList(FriendList_textScr* textScr) {
    FriendListObject* friendList = textScr->friendList;
    SysCode           fmt[4]     = {SYSFONT_CODE_FMT_STR, SYSFONT_CODE_LINEBREAK, SYSFONT_CODE_FMT_STR, SYSFONT_CODE_STR_END};
    Point             positions[7] = {
        {70,  34},
        {40,  48},
        {70,  82},
        {40,  96},
        {70, 130},
        {40, 144},
        { 0,   0},
    };
    SysCode  text[50];
    u16*     map      = friendList->resources[5].screenMap;
    u16*     charData = friendList->resources[5].charData;
    SysCode* name;
    SysCode* message1;
    SysCode* message2;
    s32      i;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 7; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    name = SysFont_GetSysCodeBuf_from_DsCode(friendList->friends[friendList->scroll].nickName, 10);
    SysFont_Format(text, name);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 116);
    SysFont_DrawToScreen(&textScr->fonts[0], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, name);

    message1 = SysFont_GetSysCodeBuf_from_DsCode(friendList->friends[friendList->scroll].message, 13);
    message2 = SysFont_GetSysCodeBuf_from_DsCode(&friendList->friends[friendList->scroll].message[13], 13);
    SysFont_Format(text, fmt, message1, message2);
    SysFont_SetHAlign(&textScr->fonts[1], 1, SYSFONT_NO_LIMIT);
    SysFont_DrawToScreen(&textScr->fonts[1], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, message1);
    Mem_Free(&gDebugHeap, message2);

    name = SysFont_GetSysCodeBuf_from_DsCode(friendList->friends[friendList->scroll + 1].nickName, 10);
    SysFont_Format(text, name);
    SysFont_SetHAlign(&textScr->fonts[2], 0, 116);
    SysFont_DrawToScreen(&textScr->fonts[2], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, name);

    message1 = SysFont_GetSysCodeBuf_from_DsCode(friendList->friends[friendList->scroll + 1].message, 13);
    message2 = SysFont_GetSysCodeBuf_from_DsCode(&friendList->friends[friendList->scroll + 1].message[13], 13);
    SysFont_Format(text, fmt, message1, message2);
    SysFont_SetHAlign(&textScr->fonts[3], 1, SYSFONT_NO_LIMIT);
    SysFont_DrawToScreen(&textScr->fonts[3], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, message1);
    Mem_Free(&gDebugHeap, message2);

    name = SysFont_GetSysCodeBuf_from_DsCode(friendList->friends[friendList->scroll + 2].nickName, 10);
    SysFont_Format(text, name);
    SysFont_SetHAlign(&textScr->fonts[4], 0, 116);
    SysFont_DrawToScreen(&textScr->fonts[4], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, name);

    message1 = SysFont_GetSysCodeBuf_from_DsCode(friendList->friends[friendList->scroll + 2].message, 13);
    message2 = SysFont_GetSysCodeBuf_from_DsCode(&friendList->friends[friendList->scroll + 2].message[13], 13);
    SysFont_Format(text, fmt, message1, message2);
    SysFont_SetHAlign(&textScr->fonts[5], 1, SYSFONT_NO_LIMIT);
    SysFont_DrawToScreen(&textScr->fonts[5], text, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, message1);
    Mem_Free(&gDebugHeap, message2);
}

static void FriendList_textScr_DrawShop(FriendList_textScr* textScr) {
    FriendListObject* friendList   = textScr->friendList;
    Point             positions[7] = {
        { 24,  18},
        { 51,  54},
        {202,  54},
        { 51,  94},
        {202,  94},
        { 51, 134},
        {202, 134},
    };
    SysCode fmt[3] = {SYSFONT_CODE_COLOR(12), SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};
    SysCode text[50];
    s32     i;
    u16*    map      = friendList->resources[5].screenMap;
    u16*    charData = friendList->resources[5].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (i = 0; i < 7; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i].x, positions[i].y);
    }

    SysFont_SetMsg(&textScr->fonts[0], SYSMSG_MINGLE_SHOP_SETUP);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 208);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[1], friendList->shopId + SYSMSG_SHOP_NAMES_START);
    SysFont_SetHAlign(&textScr->fonts[1], 0, 154);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);

    SysFont_Format(text, fmt, friendList->shopIndex + 1);
    SysFont_SetHAlign(&textScr->fonts[2], 0, 30);
    SysFont_DrawToScreen(&textScr->fonts[2], text, map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[3], friendList->clerkId + SYSMSG_SHOP_CLERK_NAMES_START);
    SysFont_SetHAlign(&textScr->fonts[3], 0, 154);
    SysFont_DrawCurrentToScreen(&textScr->fonts[3], map + 2, charData + 2, 0);

    SysFont_Format(text, fmt, friendList->clerkId + 1);
    SysFont_SetHAlign(&textScr->fonts[4], 0, 30);
    SysFont_DrawToScreen(&textScr->fonts[4], text, map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[5], friendList->musicId + SYSMSG_SHOP_BGM_NAMES_START);
    SysFont_SetHAlign(&textScr->fonts[5], 0, 154);
    SysFont_DrawCurrentToScreen(&textScr->fonts[5], map + 2, charData + 2, 0);

    SysFont_Format(text, fmt, friendList->musicId + 1);
    SysFont_SetHAlign(&textScr->fonts[6], 0, 30);
    SysFont_DrawToScreen(&textScr->fonts[6], text, map + 2, charData + 2, 0);
}

static s32 FriendList_textScr_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_textScr_Args* textScrArgs = args;
    FriendList_textScr*      textScr     = task->data;

    textScr->friendList = textScrArgs->friendList;
    FriendList_textScr_InitFonts(textScr);
    FriendList_textScr_DrawList(textScr);
    return 1;
}

static s32 FriendList_textScr_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_textScr* textScr    = task->data;
    FriendListObject*   friendList = textScr->friendList;

    if (friendList->flags & 1) {
        FriendList_ReloadBgResource(&friendList->resources[5], DISPLAY_MAIN, 1, 1, 15, 1);
        if (friendList->mode == 1) {
            FriendList_textScr_DrawShop(textScr);
        } else {
            FriendList_textScr_DrawList(textScr);
        }
        friendList->flags &= ~1;
    }
    return 1;
}

static s32 FriendList_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 FriendList_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_textScr* textScr = task->data;
    s32                 i;

    for (i = 0; i < 7; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 FriendList_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_textScr_Init,
        .update     = FriendList_textScr_Update,
        .render     = FriendList_textScr_Render,
        .cleanup    = FriendList_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_FriendList_textScr = {"Tsk_FriendList_textScr", FriendList_textScr_RunTask,
                                                  sizeof(FriendList_textScr)};

s32 FriendList_textScr_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_textScr_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_textScr, NULL, 0, NULL, &args);
}
