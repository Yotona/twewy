#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ s32         shouldRender;
    /* 0x44 */ ShopObject* shop;
    /* 0x48 */ u16         itemIndex;
    /* 0x4A */ u16         unk_4A;
} Shop_itemStar; // Size: 0x4C

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         itemIndex;
} Shop_itemStar_Args;

typedef struct {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ u32  unk_10;
    /* 0x14 */ u16  unk_14;
} Shop_itemStar_ItemInfo;

static SpriteFrameInfo* Shop_itemStar_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_itemStar_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_itemStar = {"Tsk_Shop_itemStar", Shop_itemStar_RunTask, sizeof(Shop_itemStar)};

static const SpriteAnimation Shop_itemStar_Anim = {
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
    .unk_08     = Shop_itemStar_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1b8,
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

SpriteFrameInfo* Shop_itemStar_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* frameInfo = &data_0206b408;
    switch (arg2) {
        case 1:
            frameInfo->unk_00 = 1;
            return frameInfo;

        case 2:
            frameInfo         = &data_0206b408;
            frameInfo->unk_04 = 0;
            frameInfo->unk_08 = 0;
            frameInfo->unk_0C = 0;
            frameInfo->unk_10 = -1;
            if (arg0->animData != NULL && arg0->frameDataTable != NULL && arg0->unk16 >= 0) {
                frameInfo->unk_04 = *((u16*)arg0->frameDataTable + (arg0->unk16 * 4 + 1));
                frameInfo->unk_08 =
                    (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
            }
            frameInfo->unk_10 = 0x3AC000;
            return frameInfo;

        default:
            return NULL;
    }
}

static void Shop_itemStar_Load(Shop_itemStar* itemStar, Sprite* sprite, Shop_itemStar_Args* args) {
    ShopObject* shop = itemStar->shop;

    SpriteAnimation anim = Shop_itemStar_Anim;

    itemStar->itemIndex = args->itemIndex;

    anim.dataType = args->dataType;
    anim.unk_04   = ((args->itemIndex % 6) * 33) + 29;
    anim.unk_06   = ((args->itemIndex / 6) * 41) + 46;
    anim.unk_2A   = 0x11;

    _Sprite_Load(sprite, &anim);

    Shop_itemStar_ItemInfo* itemInfo = (Shop_itemStar_ItemInfo*)shop->unk_738[itemStar->itemIndex];

    u32 val = itemInfo->unk_14;
    if (itemInfo->unk_10 == 1 && func_02023d88(val) == 1) {
        itemStar->shouldRender = TRUE;
    } else {
        itemStar->shouldRender = FALSE;
    }
}

static s32 Shop_itemStar_Init(TaskPool* pool, Task* task, void* args) {
    Shop_itemStar*      itemStar = task->data;
    Shop_itemStar_Args* itemArgs = args;

    itemStar->shop = itemArgs->shop;
    Shop_itemStar_Load(itemStar, &itemStar->sprite, itemArgs);
    return 1;
}

static s32 Shop_itemStar_Update(TaskPool* pool, Task* task, void* args) {
    Shop_itemStar* itemStar = task->data;
    ShopObject*    shop     = itemStar->shop;

    Shop_itemStar_ItemInfo* itemInfo = (Shop_itemStar_ItemInfo*)shop->unk_738[itemStar->itemIndex];

    u32 val = itemInfo->unk_14;
    if (itemInfo->unk_10 == 1 && func_02023d88(val) == 1) {
        itemStar->shouldRender = 1;
    } else {
        itemStar->shouldRender = 0;
    }
    Sprite_Update(&itemStar->sprite);
    return 1;
}

static s32 Shop_itemStar_Render(TaskPool* pool, Task* task, void* args) {
    Shop_itemStar* itemStar = task->data;

    if (itemStar->shouldRender) {
        Sprite_RenderFrame(&itemStar->sprite);
    }
    return 1;
}

static s32 Shop_itemStar_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_itemStar* itemStar = task->data;

    Sprite_Release(&itemStar->sprite);
    return 1;
}

static s32 Shop_itemStar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_itemStar_Init,
        .update     = Shop_itemStar_Update,
        .render     = Shop_itemStar_Render,
        .cleanup    = Shop_itemStar_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_itemStar_CreateTask(TaskPool* pool, s32 dataType, s16 itemIndex, ShopObject* shop) {
    Shop_itemStar_Args args;

    args.dataType  = dataType;
    args.shop      = shop;
    args.itemIndex = itemIndex;

    return EasyTask_CreateTask(pool, &Tsk_Shop_itemStar, NULL, 0, NULL, &args);
}
