#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_item; // Size: 0x48

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ MenuEquipObject* owner;
    /* 0x08 */ u16              index;
    /* 0x0A */ u16              itemId;
    /* 0x0C */ u16              graphicIndex;
    /* 0x0E */ u8               unk_0E;
    /* 0x10 */ u16              unk_10;
} MenuEquip_item_Args;

static SpriteFrameInfo* MenuEquip_item_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_item = {"Tsk_MenuEquip_item", MenuEquip_item_RunTask, sizeof(MenuEquip_item)};

static const SpriteAnimation MenuEquip_item_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_item_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[6],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_item_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x384000);
}

// Nonmatching: r1/r2 regswap
static void MenuEquip_item_Load(MenuEquip_item* item, Sprite* sprite, MenuEquip_item_Args* args) {
    SpriteAnimation anim = MenuEquip_item_Anim;

    anim.dataType = args->dataType;

    const u16* pos = (const u16*)data_ov043_020c96f0[args->index];

    anim.unk_04   = pos[0];
    anim.unk_06   = pos[1];
    anim.bits_7_9 = MenuEquip_GetFreePaletteSlot();

    if (args->itemId == 0xFFFF) {
        anim.packIndex = 1;
        item->visible  = FALSE;
    } else {
        anim.packIndex = args->graphicIndex + 1;
        item->visible  = TRUE;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_item_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_item*      item     = task->data;
    MenuEquip_item_Args* itemArgs = args;

    item->owner = itemArgs->owner;
    MenuEquip_item_Load(item, &item->sprite, itemArgs);
    return 1;
}

static s32 MenuEquip_item_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_item* item = task->data;

    Sprite_Update(&item->sprite);
    return 1;
}

static s32 MenuEquip_item_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_item* item = task->data;

    if (item->visible != 0) {
        Sprite_RenderFrame(&item->sprite);
    }
    return 1;
}

static s32 MenuEquip_item_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_item* item = task->data;

    Sprite_Release(&item->sprite);
    return 1;
}

static s32 MenuEquip_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_item_Init,
        .update     = MenuEquip_item_Update,
        .render     = MenuEquip_item_Render,
        .cleanup    = MenuEquip_item_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_item_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* owner) {
    MenuEquip_item_Args args;

    args.index        = index;
    args.owner        = owner;
    args.dataType     = dataType;
    args.itemId       = owner->slots[index]->itemId;
    args.graphicIndex = owner->slots[index]->graphicIndex;
    args.unk_0E       = owner->slots[index]->category;
    args.unk_10       = owner->slots[index]->count;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_item, NULL, 0, NULL, &args);
}

void* MenuEquip_item_GetTaskData(TaskPool* pool, u32 taskId) {
    return EasyTask_GetTaskData(pool, taskId);
}
