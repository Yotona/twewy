#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ BOOL             visible;
    /* 0x44 */ MenuEquipObject* owner;
} MenuEquip_itemAbl; // Size: 0x48

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              index;
    /* 0xA */ u16              itemId;
    /* 0xC */ u8               abilityFlag;
} MenuEquip_itemAbl_Args;

static SpriteFrameInfo* MenuEquip_itemAbl_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_itemAbl_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_itemAbl = {"Tsk_MenuEquip_itemAbl", MenuEquip_itemAbl_RunTask,
                                                 sizeof(MenuEquip_itemAbl)};

static const SpriteAnimation MenuEquip_itemAbl_Anim = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuEquip_itemAbl_GetFrameInfo,
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

static SpriteFrameInfo* MenuEquip_itemAbl_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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
            info->unk_10 = 0x3A2000;
            return info;

        default:
            return NULL;
    }
}

static void MenuEquip_itemAbl_Load(MenuEquip_itemAbl* itemAbl, Sprite* sprite, MenuEquip_itemAbl_Args* args) {
    SpriteAnimation anim = MenuEquip_itemAbl_Anim;

    anim.dataType = args->dataType;

    if (args->itemId == 0xFFFF) {
        itemAbl->visible = FALSE;
    } else if (args->abilityFlag == 1) {
        itemAbl->visible = TRUE;
    } else {
        itemAbl->visible = FALSE;
    }

    anim.unk_2A = 0x39;
    anim.unk_04 = data_ov043_020c96f0[args->index][0] - 0xB;
    anim.unk_06 = data_ov043_020c96f0[args->index][1] + 0xB;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuEquip_itemAbl_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemAbl*      itemAbl     = task->data;
    MenuEquip_itemAbl_Args* itemAblArgs = args;

    itemAbl->owner = itemAblArgs->owner;
    MenuEquip_itemAbl_Load(itemAbl, &itemAbl->sprite, itemAblArgs);
    return 1;
}

static s32 MenuEquip_itemAbl_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemAbl* itemAbl = task->data;

    Sprite_Update(&itemAbl->sprite);
    return 1;
}

static s32 MenuEquip_itemAbl_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemAbl* itemAbl = task->data;

    if (itemAbl->visible != 0) {
        Sprite_RenderFrame(&itemAbl->sprite);
    }
    return 1;
}

static s32 MenuEquip_itemAbl_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemAbl* itemAbl = task->data;

    Sprite_Release(&itemAbl->sprite);
    return 1;
}

static s32 MenuEquip_itemAbl_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_itemAbl_Init,
        .update     = MenuEquip_itemAbl_Update,
        .render     = MenuEquip_itemAbl_Render,
        .cleanup    = MenuEquip_itemAbl_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_itemAbl_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* owner) {
    MenuEquip_itemAbl_Args args;

    args.index       = index;
    args.owner       = owner;
    args.dataType    = dataType;
    args.itemId      = owner->slots[index]->itemId;
    args.abilityFlag = owner->slots[index]->abilityUnlocked;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_itemAbl, NULL, 0, NULL, &args);
}
