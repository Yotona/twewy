#include "Engine/EasyTask.h"
#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ s32         shouldRender;
    /* 0x44 */ ShopObject* shop;
} Shop_itemPlate; // Size: 0x48

typedef struct {
    /* 0x00 */ s32         unk_00;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         unk_08;
    /* 0x0A */ u16         unk_0A;
    /* 0x0C */ u8          unk_0C;
    /* 0x0E */ s16         unk_0E;
    /* 0x10 */ s32         unk_10;
} Shop_itemPlate_Args;

static SpriteFrameInfo* Shop_itemPlate_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_itemPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_itemPlate = {"Tsk_Shop_itemPlate", Shop_itemPlate_RunTask, sizeof(Shop_itemPlate)};

static const SpriteAnimation Shop_itemPlate_Anim = {
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
    .unk_08     = Shop_itemPlate_GetFrameInfo,
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

static SpriteFrameInfo* Shop_itemPlate_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* frameInfo = &data_0206b408;

    switch (mode) {
        case 1:
            frameInfo->unk_00 = 1;
            return frameInfo;

        case 2: {
            frameInfo = &data_0206b408;

            frameInfo->unk_04 = 0;
            frameInfo->unk_08 = 0;
            frameInfo->unk_0C = 0;
            frameInfo->unk_10 = -1;

            if ((sprite->animData != NULL) && (sprite->frameDataTable != NULL) && (sprite->unk16 >= 0)) {
                frameInfo->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                frameInfo->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            frameInfo->unk_10 = 0x320000;
            return frameInfo;
        }

        default:
            return NULL;
    }
}

static void Shop_itemPlate_Load(Shop_itemPlate* itemPlate, Sprite* sprite, Shop_itemPlate_Args* args) {
    SpriteAnimation anim = Shop_itemPlate_Anim;

    anim.dataType = args->unk_00;
    anim.unk_04   = ((args->unk_08 % 6) * 33) + 22;
    anim.unk_06   = ((args->unk_08 / 6) * 41) + 39;

    if (args->unk_0A == 0xFFFF) {
        itemPlate->shouldRender = FALSE;
    } else {
        itemPlate->shouldRender = TRUE;
        if (args->unk_10 == 0) {
            anim.unk_2A = 0x14;
        } else if (args->unk_0C == 1) {
            anim.unk_2A = 0x15;
        } else {
            anim.unk_2A = 0x16;
        }
    }

    _Sprite_Load(sprite, &anim);
}

static s32 Shop_itemPlate_Init(TaskPool* pool, Task* task, void* args) {
    Shop_itemPlate*      itemPlate = task->data;
    Shop_itemPlate_Args* itemArgs  = args;

    itemPlate->shop = itemArgs->shop;
    Shop_itemPlate_Load(itemPlate, &itemPlate->sprite, itemArgs);
    return 1;
}

static s32 Shop_itemPlate_Update(TaskPool* pool, Task* task, void* args) {
    Shop_itemPlate* itemPlate = task->data;

    Sprite_Update(&itemPlate->sprite);
    return 1;
}

static s32 Shop_itemPlate_Render(TaskPool* pool, Task* task, void* args) {
    Shop_itemPlate* itemPlate = task->data;

    if (itemPlate->shouldRender) {
        Sprite_RenderFrame(&itemPlate->sprite);
    }
    return 1;
}

static s32 Shop_itemPlate_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_itemPlate* itemPlate = task->data;

    Sprite_Release(&itemPlate->sprite);
    return 1;
}

static s32 Shop_itemPlate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_itemPlate_Init,
        .update     = Shop_itemPlate_Update,
        .render     = Shop_itemPlate_Render,
        .cleanup    = Shop_itemPlate_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_itemPlate_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, ShopObject* shop) {
    Shop_itemPlate_Args args;

    args.unk_00 = arg1;
    args.shop   = shop;
    args.unk_08 = arg2;
    args.unk_0A = shop->unk_738[arg2]->unk_00;
    args.unk_0C = shop->unk_738[arg2]->unk_0C;
    args.unk_10 = shop->unk_738[arg2]->unk_10;

    return EasyTask_CreateTask(pool, &Tsk_Shop_itemPlate, NULL, 0, NULL, &args);
}
