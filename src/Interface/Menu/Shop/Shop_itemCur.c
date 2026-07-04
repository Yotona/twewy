#include "Engine/EasyTask.h"
#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ BOOL        shouldRender;
    /* 0x44 */ ShopObject* shop;
    /* 0x48 */ u16         unk_48;
    /* 0x4A */ u16         unk_4A;
} Shop_itemCur; // Size: 0x4C

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_itemCur_Args;

static SpriteFrameInfo* Shop_itemCur_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_itemCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_itemCur = {"Tsk_Shop_itemCur", Shop_itemCur_RunTask, sizeof(Shop_itemCur)};

static const SpriteAnimation Shop_itemCur_Anim = {
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
    .unk_08     = Shop_itemCur_GetFrameInfo,
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

static SpriteFrameInfo* Shop_itemCur_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
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
            frameInfo->unk_10 = 0x3C0000;
            return frameInfo;

        default:
            return NULL;
    }
}

static void Shop_itemCur_Load(Shop_itemCur* arg0, Sprite* arg1, Shop_itemCur_Args* arg2) {
    SpriteAnimation anim = Shop_itemCur_Anim;

    arg0->shouldRender = TRUE;
    arg0->unk_48       = 0;

    anim.dataType = arg2->dataType;
    anim.unk_2A   = 0x12;
    anim.unk_04   = 0x16;
    anim.unk_06   = 0x27;

    _Sprite_Load(arg1, &anim);
}

static s32 Shop_itemCur_Init(TaskPool* pool, Task* task, void* args) {
    Shop_itemCur*      itemCur  = task->data;
    Shop_itemCur_Args* itemArgs = args;

    itemCur->shop = itemArgs->shop;
    Shop_itemCur_Load(itemCur, &itemCur->sprite, itemArgs);
    return 1;
}

static s32 Shop_itemCur_Update(TaskPool* pool, Task* task, void* args) {
    Shop_itemCur* itemCur = task->data;
    u16           start   = itemCur->shop->unk_850;
    u16           cursor  = itemCur->shop->unk_848;

    if (cursor >= start && cursor < start + 0xC) {
        u16 index = cursor - start;

        itemCur->sprite.posX  = ((index % 6) * 0x21) + 0x16;
        itemCur->sprite.posY  = ((index / 6) * 0x29) + 0x27;
        itemCur->shouldRender = TRUE;
    } else {
        itemCur->shouldRender = FALSE;
    }

    Sprite_Update(&itemCur->sprite);
    return 1;
}

static s32 Shop_itemCur_Render(TaskPool* pool, Task* task, void* args) {
    Shop_itemCur* itemCur = task->data;

    if (itemCur->shouldRender) {
        Sprite_RenderFrame(&itemCur->sprite);
    }
    return 1;
}

static s32 Shop_itemCur_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_itemCur* itemCur = task->data;

    Sprite_Release(&itemCur->sprite);
    return 1;
}

static s32 Shop_itemCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_itemCur_Init,
        .update     = Shop_itemCur_Update,
        .render     = Shop_itemCur_Render,
        .cleanup    = Shop_itemCur_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Shop_itemCur_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_itemCur_Args args;

    args.dataType = dataType;
    args.shop     = shop;

    return EasyTask_CreateTask(pool, &Tsk_Shop_itemCur, NULL, 0, NULL, &args);
}
