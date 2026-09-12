#include "Interface/Menu/MenuEquip.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
    /* 0x48 */ u16              index;
} MenuEquip_itemMask; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              index;
} MenuEquip_itemMask_Args;

static SpriteFrameInfo* MenuEquip_itemMask_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_itemMask_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_itemMask = {"Tsk_MenuEquip_itemMask", MenuEquip_itemMask_RunTask,
                                                  sizeof(MenuEquip_itemMask)};

static const SpriteAnimation MenuEquip_itemMask_Anim = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuEquip_itemMask_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[2],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 4,
    .unk_1E     = 0,
    .unk_20     = 0xA,
    .unk_22     = 6,
    .unk_24     = 0,
    .unk_26     = 5,
    .unk_28     = 6,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuEquip_itemMask_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1:
            info->unk_00 = 1;
            return info;

        case 2:
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;
            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info->unk_10 = 0x3B6000;
            return info;

        default:
            return NULL;
    }
}

static void MenuEquip_itemMask_Load(MenuEquip_itemMask* itemMask, Sprite* sprite, MenuEquip_itemMask_Args* args) {
    MenuEquipObject*    owner = itemMask->owner;
    SpriteAnimation     anim  = MenuEquip_itemMask_Anim;
    MenuEquipItemEntry* item;

    anim.dataType = args->dataType;
    {
        const u16* pos = (const u16*)data_ov043_020c96f0[args->index];

        anim.unk_2A = 0x3D;
        anim.unk_04 = pos[0];
        anim.unk_06 = pos[1];
    }

    item = owner->slots[args->index];
    if (item->itemId != 0xFFFF && item->count == 0) {
        itemMask->visible = TRUE;
    } else {
        itemMask->visible = FALSE;
    }

    anim.unk_02.unk_02   = 1;
    data_0206a890.unk_0C = 0xA;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_itemMask_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemMask*      itemMask     = task->data;
    MenuEquip_itemMask_Args* itemMaskArgs = args;

    itemMask->owner = itemMaskArgs->owner;
    itemMask->index = itemMaskArgs->index;
    MenuEquip_itemMask_Load(itemMask, &itemMask->sprite, itemMaskArgs);
    return 1;
}

static s32 MenuEquip_itemMask_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemMask* itemMask = task->data;
    MenuEquipItemEntry* item     = itemMask->owner->slots[itemMask->index];

    if (item->itemId != 0xFFFF && item->count == 0) {
        itemMask->visible = TRUE;
    } else {
        itemMask->visible = FALSE;
    }

    Sprite_Update(&itemMask->sprite);
    return 1;
}

static s32 MenuEquip_itemMask_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemMask* itemMask = task->data;

    if (itemMask->visible != 0) {
        Sprite_RenderFrame(&itemMask->sprite);
    }
    return 1;
}

static s32 MenuEquip_itemMask_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemMask* itemMask = task->data;

    Sprite_Release(&itemMask->sprite);
    return 1;
}

static s32 MenuEquip_itemMask_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_itemMask_Init,
        .update     = MenuEquip_itemMask_Update,
        .render     = MenuEquip_itemMask_Render,
        .cleanup    = MenuEquip_itemMask_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_itemMask_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* owner) {
    MenuEquip_itemMask_Args args;

    args.dataType = dataType;
    args.index    = index;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_itemMask, NULL, 0, NULL, &args);
}
