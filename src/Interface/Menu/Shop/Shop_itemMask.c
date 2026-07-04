#include "Interface/Menu/Shop.h"
#include "common_data.h"

extern s32 func_ov043_020b1a70(ShopObject* shop, s16 itemIndex);

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ BOOL        shouldRender;
    /* 0x44 */ ShopObject* shop;
    /* 0x48 */ s16         itemIndex;
    /* 0x4A */ u16         itemFrame;
} Shop_itemMask; // Size: 0x4C

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         itemIndex;
    /* 0x0A */ u16         itemFrame;
} Shop_itemMask_Args;

static SpriteFrameInfo* Shop_itemMask_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_itemMask_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_itemMask = {"Tsk_Shop_itemMask", Shop_itemMask_RunTask, sizeof(Shop_itemMask)};

static const SpriteAnimation Shop_itemMask_Anim = {
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
    .unk_08     = Shop_itemMask_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1b8,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* Shop_itemMask_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* frameInfo = &data_0206b408;

    switch (mode) {
        case 1:
            frameInfo->unk_00 = 1;
            return frameInfo;

        case 2:
            frameInfo         = &data_0206b408;
            frameInfo->unk_04 = 0;
            frameInfo->unk_08 = 0;
            frameInfo->unk_0C = 0;
            frameInfo->unk_10 = -1;
            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                frameInfo->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                frameInfo->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            frameInfo->unk_10 = 0x3B6000;
            return frameInfo;

        default:
            return NULL;
    }
}

static void Shop_itemMask_Load(Shop_itemMask* itemMask, Sprite* sprite, Shop_itemMask_Args* args) {
    ShopObject* shop = itemMask->shop;

    SpriteAnimation anim = Shop_itemMask_Anim;

    anim.dataType = args->dataType;
    anim.unk_2A   = 19;

    anim.unk_04 = (args->itemIndex % 6) * 0x21 + 0x16;
    anim.unk_06 = (args->itemIndex / 6) * 0x29 + 0x27;

    if (args->itemFrame == 0xFFFF) {
        itemMask->shouldRender = FALSE;
    } else if (func_ov043_020b1a70(shop, args->itemIndex) == 1) {
        itemMask->shouldRender = FALSE;
    } else {
        itemMask->shouldRender = TRUE;
    }

    itemMask->itemIndex = args->itemIndex;

    anim.unk_02.raw = (anim.unk_02.raw & ~0xC) | 4;

    data_0206a890.unk_0C = 20;
    _Sprite_Load(sprite, &anim);
}

static s32 Shop_itemMask_Init(TaskPool* pool, Task* task, void* args) {
    Shop_itemMask*      itemMask     = task->data;
    Shop_itemMask_Args* itemMaskArgs = args;

    itemMask->shop = itemMaskArgs->shop;
    Shop_itemMask_Load(itemMask, &itemMask->sprite, itemMaskArgs);
    return 1;
}

static s32 Shop_itemMask_Update(TaskPool* pool, Task* task, void* args) {
    Shop_itemMask* itemMask = task->data;

    if (func_ov043_020b1a70(itemMask->shop, itemMask->itemIndex) == 1) {
        itemMask->shouldRender = FALSE;
    } else {
        itemMask->shouldRender = TRUE;
    }
    Sprite_Update(&itemMask->sprite);
    return 1;
}

static s32 Shop_itemMask_Render(TaskPool* pool, Task* task, void* args) {
    Shop_itemMask* itemMask = task->data;

    if (itemMask->shouldRender != FALSE) {
        Sprite_RenderFrame(&itemMask->sprite);
    }
    return 1;
}

static s32 Shop_itemMask_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_itemMask* itemMask = task->data;

    Sprite_Release(&itemMask->sprite);
    return 1;
}

static s32 Shop_itemMask_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_itemMask_Init,
        .update     = Shop_itemMask_Update,
        .render     = Shop_itemMask_Render,
        .cleanup    = Shop_itemMask_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_itemMask_CreateTask(TaskPool* pool, s32 dataType, s16 itemIndex, ShopObject* shop) {
    Shop_itemMask_Args args;

    args.dataType  = dataType;
    args.shop      = shop;
    args.itemIndex = itemIndex;
    args.itemFrame = shop->unk_738[itemIndex]->unk_00;
    return EasyTask_CreateTask(pool, &Tsk_Shop_itemMask, NULL, 0, NULL, &args);
}
