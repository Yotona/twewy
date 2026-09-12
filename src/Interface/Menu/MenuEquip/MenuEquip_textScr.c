#include "Interface/Menu/MenuEquip.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x00 */ MenuEquipObject* owner;
    /* 0x04 */ SysFont          fonts[2];
} MenuEquip_textScr; // Size: 0xFC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_textScr_Args;

static s32 MenuEquip_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_textScr = {"Tsk_MenuEquip_textScr", MenuEquip_textScr_RunTask,
                                                 sizeof(MenuEquip_textScr)};

static void MenuEquip_textScr_InitFonts(MenuEquip_textScr* textScr) {
    for (s32 i = 0; i < 2; i++) {
        SysFont_Init(&textScr->fonts[i]);
        SysFont_SetColor(&textScr->fonts[i], 14);
    }
}

static void MenuEquip_textScr_DrawItemName(MenuEquip_textScr* textScr) {
    MenuEquipObject* owner           = textScr->owner;
    s16              positions[2][2] = {
        { 6, 117},
        {10,  41},
    };
    u16* map      = owner->resources[5].screenMap;
    u16* charData = owner->resources[5].charData;
    u16  itemIndex;
    u8   category;
    u16  msgIndex;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (s32 i = 0; i < 2; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i][0], positions[i][1]);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
    }

    itemIndex = Inventory_GetCategorizedIndex(owner->cursorItem.itemId);
    category  = owner->cursorItem.category;
    if (itemIndex == 0xFFFF) {
        return;
    }

    if (category == 1) {
        msgIndex = itemIndex + SYSMSG_THREAD_NAMES_START;
    } else if (category == 2) {
        msgIndex = itemIndex + SYSMSG_FOOD_NAMES_START;
    } else {
        msgIndex = itemIndex + SYSMSG_SWAG_NAMES_START;
    }

    SysFont_SetMsg(&textScr->fonts[0], msgIndex);
    SysFont_SetHAlign(&textScr->fonts[0], 0, 119);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);
}

// Nonmatching: register allocation
static void MenuEquip_textScr_DrawTabName(MenuEquip_textScr* textScr) {
    MenuEquipObject* owner           = textScr->owner;
    s16              positions[2][2] = {
        {32, 43},
        {16, 60},
    };
    u16* map      = owner->resources[5].screenMap;
    u16* charData = owner->resources[5].charData;

    if (map == NULL || charData == NULL) {
        OS_WaitForever();
    }

    for (s32 i = 0; i < 2; i++) {
        SysFont_SetPos(&textScr->fonts[i], positions[i][0], positions[i][1]);
        SysFont_SetSpacing(&textScr->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScr->fonts[0], (s16)((owner->errorCode - 1) * 2 + SYSMSG_CANT_EAT_TITLE_START));
    SysFont_SetHAlign(&textScr->fonts[0], 1, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScr->fonts[0], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScr->fonts[1], (s16)((owner->errorCode - 1) * 2 + SYSMSG_CANT_EAT_TEXT_START));
    SysFont_SetHAlign(&textScr->fonts[1], 0, 224);
    SysFont_DrawCurrentToScreen(&textScr->fonts[1], map + 2, charData + 2, 0);
}

static s32 MenuEquip_textScr_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textScr*      textScr     = task->data;
    MenuEquip_textScr_Args* textScrArgs = args;

    textScr->owner = textScrArgs->owner;
    MenuEquip_textScr_InitFonts(textScr);
    MenuEquip_textScr_DrawItemName(textScr);
    return 1;
}

static s32 MenuEquip_textScr_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textScr* textScr = task->data;
    MenuEquipObject*   owner   = textScr->owner;

    if (owner->dirtyFlags & 0x40) {
        MenuEquip_ReloadBgResource(&owner->resources[5], 0, 1, 11, 15, 1);
        MenuEquip_textScr_DrawTabName(textScr);
        owner->dirtyFlags &= ~0x40;
    }
    return 1;
}

static s32 MenuEquip_textScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuEquip_textScr_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textScr* textScr = task->data;

    for (s32 i = 0; i < 2; i++) {
        SysFont_Destroy(&textScr->fonts[i]);
    }
    return 1;
}

static s32 MenuEquip_textScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_textScr_Init,
        .update     = MenuEquip_textScr_Update,
        .render     = MenuEquip_textScr_Render,
        .cleanup    = MenuEquip_textScr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_textScr_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_textScr_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_textScr, NULL, 0, NULL, &args);
}
