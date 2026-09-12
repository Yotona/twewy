#include "Engine/Core/Memory.h"
#include "Interface/Menu/MenuEquip.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ MenuEquipObject* owner;
    /* 0x004 */ SysFont          fonts[10];
} MenuEquip_textScrU; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_textScrU_Args;

static s32 MenuEquip_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static void MenuEquip_textScrU_InitFonts(MenuEquip_textScrU* textScrU) {
    for (s32 i = 0; i < 10; i++) {
        SysFont_Init(&textScrU->fonts[i]);
        SysFont_SetColor(&textScrU->fonts[i], 14);
    }
}

static void MenuEquip_textScrU_DrawItemName(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                            u16 category) {
    u16 msgIndex;

    if (category == ITEM_CATEGORY_THREAD) {
        msgIndex = itemIndex + SYSMSG_THREAD_NAMES_START;
    } else if (category == ITEM_CATEGORY_FOOD) {
        msgIndex = itemIndex + SYSMSG_FOOD_NAMES_START;
    } else {
        msgIndex = itemIndex + SYSMSG_SWAG_NAMES_START;
    }

    SysFont_SetMsg(&textScrU->fonts[0], msgIndex);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);
}

static void MenuEquip_textScrU_DrawItemDesc(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                            u16 category) {
    u16 msgIndex;

    if (category == ITEM_CATEGORY_THREAD) {
        msgIndex = itemIndex + SYSMSG_THREAD_DESC_START;
    } else if (category == ITEM_CATEGORY_FOOD) {
        msgIndex = itemIndex + SYSMSG_FOOD_DESC_START;
    } else {
        msgIndex = itemIndex + SYSMSG_SWAG_DESC_START;
    }

    SysFont_SetMsg(&textScrU->fonts[6], msgIndex);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[6], map + 2, charData + 2, 0);
}

static void MenuEquip_textScrU_DrawItemEffect(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                              u16 category) {
    MenuEquipObject* owner = textScrU->owner;
    SysCode          buf[200];

    if (category == ITEM_CATEGORY_THREAD) {
        u16 msgIndex;
        if (owner->cursorItem.abilityUnlocked != 1) {
            msgIndex = SYSMSG_THREAD_ABILITY_LOCKED_HELP;
        } else {
            msgIndex = owner->cursorItem.abilityId + SYSMSG_THREAD_ABILITY_HELP_START;
        }
        SysFont_SetMsg(&textScrU->fonts[7], msgIndex);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[7], map + 2, charData + 2, 0);
    } else if (category == ITEM_CATEGORY_FOOD) {
        u16      effectArg1 = owner->cursorItem.syncGain;
        s16      effectArg2 = owner->cursorItem.foodEffectValue;
        SysCode* fmt =
            SysFont_GetMsgBuf(&textScrU->fonts[7], (s16)(owner->cursorItem.foodEffectMsg + SYSMSG_FOOD_EFFECT_START));

        SysFont_Format(buf, fmt, effectArg1, effectArg2);
        SysFont_DrawToScreen(&textScrU->fonts[7], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
    } else {
        SysFont_SetMsg(&textScrU->fonts[7], itemIndex + SYSMSG_SWAG_TIPS_START);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[7], map + 2, charData + 2, 0);
    }
}

// Nonmatching: register allocation
static void MenuEquip_textScrU_DrawThreadStats(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                               u16 category) {
    MenuEquipObject* owner = textScrU->owner;
    SysCode          buf[60];
    SysCode*         label;
    SysCode*         fmt;
    s16              statBonus1 = owner->cursorItem.attackBonus;
    s16              statBonus2 = owner->cursorItem.defenseBonus;
    s16              statBonus3 = owner->cursorItem.healthBonus;

    SysFont_SetMsg(&textScrU->fonts[1], owner->cursorItem.brand + SYSMSG_BRAND_NAMES_START);
    SysFont_SetHAlign(&textScrU->fonts[1], 1, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[1], map + 2, charData + 2, 0);

    if (statBonus1 == 0) {
        SysFont_SetMsg(&textScrU->fonts[2], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[2], 0, 93);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
    } else {
        label = SysFont_GetMsgBuf(&textScrU->fonts[2], SYSMSG_STAT_ATTACK);
        if (statBonus1 > 0) {
            fmt = SysFont_GetMsgBuf(&textScrU->fonts[2], SYSMSG_STAT_BONUS_FMT);
        } else {
            fmt        = SysFont_GetMsgBuf(&textScrU->fonts[2], SYSMSG_STAT_PENALTY_FMT);
            statBonus1 = -statBonus1;
        }
        SysFont_Format(buf, fmt, label, statBonus1);
        SysFont_SetHAlign(&textScrU->fonts[2], 0, 93);
        SysFont_DrawToScreen(&textScrU->fonts[2], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, label);
    }

    if (statBonus2 == 0) {
        SysFont_SetMsg(&textScrU->fonts[4], SYSMSG_STAT_NONE);
        SysFont_SetHAlign(&textScrU->fonts[4], 0, 93);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[4], map + 2, charData + 2, 0);
    } else {
        label = SysFont_GetMsgBuf(&textScrU->fonts[4], SYSMSG_STAT_DEFENSE);
        if (statBonus2 > 0) {
            fmt = SysFont_GetMsgBuf(&textScrU->fonts[4], SYSMSG_STAT_BONUS_FMT);
        } else {
            fmt        = SysFont_GetMsgBuf(&textScrU->fonts[4], SYSMSG_STAT_PENALTY_FMT);
            statBonus2 = -statBonus2;
        }
        SysFont_Format(buf, fmt, label, statBonus2);
        SysFont_SetHAlign(&textScrU->fonts[4], 0, 93);
        SysFont_DrawToScreen(&textScrU->fonts[4], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, label);
    }

    if (statBonus3 == 0) {
        SysFont_SetMsg(&textScrU->fonts[5], SYSMSG_STAT_NONE);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[5], map + 2, charData + 2, 0);
        SysFont_SetHAlign(&textScrU->fonts[5], 0, 93);
    } else {
        label = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_STAT_HP);
        if (statBonus3 > 0) {
            fmt = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_STAT_BONUS_FMT);
        } else {
            fmt        = SysFont_GetMsgBuf(&textScrU->fonts[5], SYSMSG_STAT_PENALTY_FMT);
            statBonus3 = -statBonus3;
        }
        SysFont_Format(buf, fmt, label, statBonus3);
        SysFont_SetHAlign(&textScrU->fonts[5], 0, 93);
        SysFont_DrawToScreen(&textScrU->fonts[5], buf, map + 2, charData + 2, 0);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, label);
    }
}

static void MenuEquip_textScrU_ClearThreadStats(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                                u16 category) {
    SysFont_SetMsg(&textScrU->fonts[1], SYSMSG_STAT_NONE);
    SysFont_SetHAlign(&textScrU->fonts[1], 1, SYSFONT_NO_LIMIT);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[1], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScrU->fonts[2], SYSMSG_STAT_NONE);
    SysFont_SetHAlign(&textScrU->fonts[2], 0, 93);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScrU->fonts[3], SYSMSG_STAT_NONE);
    SysFont_SetHAlign(&textScrU->fonts[3], 0, 93);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[3], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScrU->fonts[4], SYSMSG_STAT_NONE);
    SysFont_SetHAlign(&textScrU->fonts[4], 0, 93);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[4], map + 2, charData + 2, 0);

    SysFont_SetMsg(&textScrU->fonts[5], SYSMSG_STAT_NONE);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[5], map + 2, charData + 2, 0);
    SysFont_SetHAlign(&textScrU->fonts[5], 0, 93);
}

static void MenuEquip_textScrU_DrawThreadValue(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                               u16 category) {
    MenuEquipObject* owner = textScrU->owner;
    SysCode          buf[60];
    SysCode          fmt[5] = {SYSFONT_CODE_FMT_STR, 0, SYSFONT_CODE_COLOR(12), SYSFONT_CODE_FMT_U32, SYSFONT_CODE_STR_END};
    u16              value  = owner->cursorItem.braveryReq;
    SysCode*         label  = SysFont_GetMsgBuf(&textScrU->fonts[3], SYSMSG_BRAVERY_REQ);

    SysFont_Format(buf, fmt, label, value);
    SysFont_SetHAlign(&textScrU->fonts[3], 0, 93);
    SysFont_DrawToScreen(&textScrU->fonts[3], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, label);
}

static void MenuEquip_textScrU_DrawThreadAttribute(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                                   u16 category) {
    MenuEquipObject* owner = textScrU->owner;
    SysCode          buf[60];
    SysCode          fmt[3]         = {SYSFONT_CODE_FMT_STR, SYSFONT_CODE_FMT_STR, SYSFONT_CODE_STR_END};
    u8               attributeIndex = owner->cursorItem.subCategory;
    SysCode*         label          = SysFont_GetMsgBuf(&textScrU->fonts[8], SYSMSG_THREAD_ATTRIBUTE_LABEL);
    SysCode*         name = SysFont_GetMsgBuf(&textScrU->fonts[8], attributeIndex + SYSMSG_THREAD_ATTRIBUTE_NAMES_START);

    SysFont_Format(buf, fmt, label, name);
    SysFont_SetHAlign(&textScrU->fonts[8], 0, 149);
    SysFont_DrawToScreen(&textScrU->fonts[8], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, label);
    Mem_Free(&gDebugHeap, name);
}

static void MenuEquip_textScrU_DrawFoodLabel(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                             u16 category) {
    SysFont_SetMsg(&textScrU->fonts[8], SYSMSG_FOOD_ITEM_LABEL);
    SysFont_SetHAlign(&textScrU->fonts[8], 0, 149);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[8], map + 2, charData + 2, 0);
}

static void MenuEquip_textScrU_DrawFoodCount(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                             u16 category) {
    SysCode  buf[60];
    u16      count = textScrU->owner->cursorItem.capacityCost;
    SysCode* fmt;

#ifdef REGION_USA
    if (count == 1) {
        fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_FOOD_DIGEST_BYTE);
    } else {
        fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_FOOD_DIGEST_BYTES);
    }
#else
    fmt = SysFont_GetMsgBuf(&textScrU->fonts[9], SYSMSG_FOOD_DIGEST_BYTES);
#endif
    SysFont_Format(buf, fmt, count);
    SysFont_SetHAlign(&textScrU->fonts[9], 0, 149);
    SysFont_DrawToScreen(&textScrU->fonts[9], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);
}

static void MenuEquip_textScrU_DrawOtherLabel(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                              u16 category) {
    SysFont_SetMsg(&textScrU->fonts[8], SYSMSG_SWAG_LABEL);
    SysFont_SetHAlign(&textScrU->fonts[8], 0, 149);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[8], map + 2, charData + 2, 0);
}

static void MenuEquip_textScrU_DrawOtherInfo(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                             u16 category) {
    SysFont_SetMsg(&textScrU->fonts[9], SYSMSG_SWAG_TIPS_LABEL);
    SysFont_SetHAlign(&textScrU->fonts[9], 0, 149);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[9], map + 2, charData + 2, 0);
}

static void MenuEquip_textScrU_DrawThreadAbility(MenuEquip_textScrU* textScrU, u16* charData, u16* map, u16 itemIndex,
                                                 u16 category) {
    MenuEquipObject* owner    = textScrU->owner;
    u8               msgIndex = owner->cursorItem.abilityId;

    if (owner->cursorItem.abilityUnlocked == 1) {
        SysFont_SetMsg(&textScrU->fonts[9], msgIndex + SYSMSG_THREAD_ABILITY_NAME_START);
        SysFont_SetHAlign(&textScrU->fonts[9], 0, 149);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[9], map + 2, charData + 2, 0);
    } else {
        SysFont_SetMsg(&textScrU->fonts[9], SYSMSG_THREAD_ABILITY_LOCKED);
        SysFont_SetHAlign(&textScrU->fonts[9], 0, 149);
        SysFont_DrawCurrentToScreen(&textScrU->fonts[9], map + 2, charData + 2, 0);
    }
}

// Nonmatching: regswap
static void MenuEquip_textScrU_DrawItemPage(MenuEquip_textScrU* textScrU) {
    MenuEquipObject* owner            = textScrU->owner;
    s16              positions[10][2] = {
        {  9,  17},
        {  9,  41},
        { 31,  56},
        {154,  56},
        { 31,  68},
        {154,  68},
        {  9,  97},
        {  9, 151},
        { 99,  85},
        { 99, 139},
    };

    u16* charData = owner->resources[0].charData;
    u16* map      = owner->resources[0].screenMap;
    if (owner->resources[0].screenMap == NULL || owner->resources[0].charData == NULL) {
        OS_WaitForever();
    }

    u16 itemIndex = Inventory_GetCategorizedIndex(owner->cursorItem.itemId);
    u8  category  = owner->cursorItem.category;

    if (itemIndex == 0xFFFF) {
        return;
    }

    for (s32 i = 0; i < 10; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i][0], positions[i][1]);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    MenuEquip_textScrU_DrawItemName(textScrU, charData, map, itemIndex, category);
    MenuEquip_textScrU_DrawItemDesc(textScrU, charData, map, itemIndex, category);
    MenuEquip_textScrU_DrawItemEffect(textScrU, charData, map, itemIndex, category);

    if (category == ITEM_CATEGORY_THREAD) {
        MenuEquip_textScrU_DrawThreadStats(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawThreadAttribute(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawThreadValue(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawThreadAbility(textScrU, charData, map, itemIndex, category);
    } else if (category == ITEM_CATEGORY_FOOD) {
        MenuEquip_textScrU_ClearThreadStats(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawFoodLabel(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawFoodCount(textScrU, charData, map, itemIndex, category);
    } else {
        MenuEquip_textScrU_ClearThreadStats(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawOtherLabel(textScrU, charData, map, itemIndex, category);
        MenuEquip_textScrU_DrawOtherInfo(textScrU, charData, map, itemIndex, category);
    }
}

static void MenuEquip_textScrU_DrawHelpPage(MenuEquip_textScrU* textScrU) {
    MenuEquipObject* owner = textScrU->owner;

    s16 positions[3][2] = {
        {24, 11},
        {24, 10},
        {14, 27},
    };

    u16* charData = owner->resources[0].charData;
    u16* map      = owner->resources[0].screenMap;
    if (owner->resources[0].screenMap == NULL || owner->resources[0].charData == NULL) {
        OS_WaitForever();
    }

    for (u16 i = 0; i < 3; i++) {
        SysFont_SetPos(&textScrU->fonts[i], positions[i][0], positions[i][1]);
        SysFont_SetSpacing(&textScrU->fonts[i], TRUE, 0);
    }

    SysFont_SetMsg(&textScrU->fonts[0], owner->helpPage + SYSMSG_EQUIPMENU_HELP_LABELS);
    SysFont_SetHAlign(&textScrU->fonts[0], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[0], map + 2, charData + 2, 0);

    SysCode* fmt = SysFont_GetMsgBuf(&textScrU->fonts[1], SYSMSG_DIVIDED_U32S);
    SysCode  buf[60];

    SysFont_Format(buf, fmt, owner->helpPage + 1, 9);
    SysFont_SetHAlign(&textScrU->fonts[1], 2, 220);
    SysFont_DrawToScreen(&textScrU->fonts[1], buf, map + 2, charData + 2, 0);
    Mem_Free(&gDebugHeap, fmt);

    SysFont_SetMsg(&textScrU->fonts[2], owner->helpPage + SYSMSG_EQUIPMENU_HELP_TEXT);
    SysFont_SetHAlign(&textScrU->fonts[2], 1, 224);
    SysFont_DrawCurrentToScreen(&textScrU->fonts[2], map + 2, charData + 2, 0);
}

static s32 MenuEquip_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textScrU*      textScrU     = task->data;
    MenuEquip_textScrU_Args* textScrUArgs = args;

    textScrU->owner = textScrUArgs->owner;
    MenuEquip_textScrU_InitFonts(textScrU);
    MenuEquip_textScrU_DrawItemPage(textScrU);
    return 1;
}

static s32 MenuEquip_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textScrU* textScrU = task->data;
    MenuEquipObject*    owner    = textScrU->owner;

    if (owner->dirtyFlags & 0x20) {
        MenuEquip_ReloadBgResource(&owner->resources[0], 1, 0, 10, 15, 1);
        if (owner->helpOpen == 0) {
            MenuEquip_textScrU_DrawItemPage(textScrU);
        } else {
            MenuEquip_textScrU_DrawHelpPage(textScrU);
        }
        owner->dirtyFlags &= ~0x20;
    }
    return 1;
}

static s32 MenuEquip_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 MenuEquip_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textScrU* textScrU = task->data;

    for (s32 i = 0; i < 10; i++) {
        SysFont_Destroy(&textScrU->fonts[i]);
    }
    return 1;
}

static s32 MenuEquip_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_textScrU_Init,
        .update     = MenuEquip_textScrU_Update,
        .render     = MenuEquip_textScrU_Render,
        .cleanup    = MenuEquip_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_MenuEquip_textScrU = {"Tsk_MenuEquip_textScrU", MenuEquip_textScrU_RunTask,
                                                  sizeof(MenuEquip_textScrU)};

s32 MenuEquip_textScrU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_textScrU_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_textScrU, NULL, 0, NULL, &args);
}
