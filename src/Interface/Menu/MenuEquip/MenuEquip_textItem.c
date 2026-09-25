#include "Interface/Menu/MenuEquip.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ Sprite           sprite;
    /* 0x040 */ BOOL             visible;
    /* 0x044 */ MenuEquipObject* owner;
    /* 0x048 */ SysFont          font;
    /* 0x0C4 */ char             unk_0C4[0x114 - 0x0C4];
    /* 0x114 */ u16              drawDelay;
} MenuEquip_textItem; // Size: 0x118

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ s32              unk_8;
} MenuEquip_textItem_Args;

static SpriteFrameInfo* MenuEquip_textItem_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_textItem_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_textItem = {"Tsk_MenuEquip_textItem", MenuEquip_textItem_RunTask,
                                                  sizeof(MenuEquip_textItem)};

static const SpriteAnimation MenuEquip_textItem_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_textItem_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_textItem_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3E8000);
}

static void MenuEquip_textItem_Load(MenuEquip_textItem* textItem, Sprite* sprite, MenuEquip_textItem_Args* args) {
    SpriteAnimation anim = MenuEquip_textItem_Anim;

    textItem->visible   = TRUE;
    textItem->drawDelay = 2;

    anim.dataType = args->dataType;
    anim.unk_2A   = 0x2C;
    anim.unk_04   = 0x42;
    anim.unk_06   = 0x79;
    _Sprite_Load(sprite, &anim);
}

static void MenuEquip_textItem_InitFont(MenuEquip_textItem* textItem) {
    SysFont_Init(&textItem->font);
}

static void MenuEquip_textItem_SetItemName(MenuEquip_textItem* textItem) {
    MenuEquipObject* owner = textItem->owner;

    SysFont_SetPos(&textItem->font, 0, 3);
    SysFont_SetHAlign(&textItem->font, 0, 122);
    SysFont_SetSpacing(&textItem->font, TRUE, 0);

    u16          itemId    = owner->cursorItem.itemId;
    u16          itemIndex = Inventory_GetCategorizedIndex(itemId);
    ItemCategory category  = Inventory_GetCategory(itemId);

    if (itemId == 0xFFFF) {
        SysFont_SetMsg(&textItem->font, SYSMSG_THREAD_NAMES_START);
        textItem->visible = FALSE;
        return;
    }

    s32 msgBase;
    if (category == ITEM_CATEGORY_PIN) {
        msgBase = SYSMSG_PIN_NAMES_START;
    } else if (category == ITEM_CATEGORY_THREAD) {
        msgBase = SYSMSG_THREAD_NAMES_START;
    } else if (category == ITEM_CATEGORY_FOOD) {
        msgBase = SYSMSG_FOOD_NAMES_START;
    } else {
        msgBase = SYSMSG_SWAG_NAMES_START;
    }

    SysFont_SetMsg(&textItem->font, msgBase + itemIndex);
    textItem->visible = TRUE;
}

static void MenuEquip_textItem_DrawTextDelayed(MenuEquip_textItem* textItem) {
    if (textItem->drawDelay != 0) {
        textItem->drawDelay--;
        if (textItem->drawDelay == 0) {
            SysFont_DrawCurrentToSprite(&textItem->font, &textItem->sprite, TRUE);
        }
    }
}

static s32 MenuEquip_textItem_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textItem*      textItem     = task->data;
    MenuEquip_textItem_Args* textItemArgs = args;

    textItem->owner = textItemArgs->owner;
    MenuEquip_textItem_Load(textItem, &textItem->sprite, textItemArgs);
    MenuEquip_textItem_InitFont(textItem);
    MenuEquip_textItem_SetItemName(textItem);
    return 1;
}

static s32 MenuEquip_textItem_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textItem* textItem = task->data;

    Sprite_Update(&textItem->sprite);
    return 1;
}

static s32 MenuEquip_textItem_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textItem* textItem = task->data;

    if (textItem->visible != FALSE) {
        Sprite_RenderFrame(&textItem->sprite);
    }
    MenuEquip_textItem_DrawTextDelayed(textItem);
    return 1;
}

static s32 MenuEquip_textItem_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_textItem* textItem = task->data;

    Sprite_Release(&textItem->sprite);
    SysFont_Destroy(&textItem->font);
    return 1;
}

static s32 MenuEquip_textItem_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_textItem_Init,
        .update     = MenuEquip_textItem_Update,
        .render     = MenuEquip_textItem_Render,
        .cleanup    = MenuEquip_textItem_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_textItem_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_textItem_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_textItem, NULL, 0, NULL, &args);
}
