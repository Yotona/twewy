#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprites[2];
    /* 0x80 */ s32         shouldRender[2];
    /* 0x88 */ ShopObject* shop;
} Shop_item; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         itemIndex;
    /* 0x0A */ u16         itemFrame;
    /* 0x0C */ u16         badgeFrame;
    /* 0x0E */ u16         isUnlocked;
} Shop_item_Args;

static SpriteFrameInfo* Shop_item_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern BinIdentifier data_ov043_020ca1f0;
extern BinIdentifier data_ov043_020ca1f8;

static const TaskHandle Tsk_Shop_item = {"Tsk_Shop_item", Shop_item_RunTask, sizeof(Shop_item)};

static const SpriteAnimation data_ov043_020ca648 = {
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
    .unk_08     = Shop_item_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1b8,
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

// Nonmatching
static SpriteFrameInfo* Shop_item_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            temp->unk_10 = 0x352000;
            return temp;
        } break;

        default:
            return NULL;
    }
}

static void Shop_item_Load(Shop_item* item, Sprite* sprites, Shop_item_Args* args) {
    SpriteAnimation anim = data_ov043_020ca648;

    anim.dataType = args->dataType;

    anim.unk_04 = (s16)(((args->itemIndex % 6) * 33) + 22);
    anim.unk_06 = (s16)(((args->itemIndex / 6) * 41) + 39);

    if (args->itemFrame == 0xFFFF) {
        item->shouldRender[0] = FALSE;
        item->shouldRender[1] = FALSE;
        _Sprite_Load(&sprites[0], &anim);
        _Sprite_Load(&sprites[1], &anim);
        return;
    }

    if (args->isUnlocked == FALSE) {
        item->shouldRender[0] = TRUE;
        item->shouldRender[1] = TRUE;

        anim.binIden  = &data_ov043_020ca1f0;
        anim.unk_18   = 2;
        anim.bits_7_9 = 6;

        anim.packIndex = args->badgeFrame + 1;
        anim.unk_2A    = 1;
        anim.unk_22    = 1;
        _Sprite_Load(&sprites[0], &anim);

        anim.binIden  = &data_ov043_020ca1b8;
        anim.unk_18   = 0;
        anim.bits_7_9 = 5;

        anim.packIndex = 1;
        anim.unk_2A    = 23;
        anim.unk_22    = 4;
        _Sprite_Load(&sprites[1], &anim);
    } else {
        item->shouldRender[0] = TRUE;
        item->shouldRender[1] = FALSE;

        anim.binIden   = &data_ov043_020ca1f8;
        anim.unk_18    = 2;
        anim.bits_7_9  = 6;
        anim.packIndex = args->badgeFrame + 1;
        anim.unk_2A    = 1;
        anim.unk_22    = 1;
        _Sprite_Load(&sprites[0], &anim);

        anim.binIden   = &data_ov043_020ca1b8;
        anim.unk_18    = 0;
        anim.bits_7_9  = 5;
        anim.packIndex = 1;
        anim.unk_2A    = 1;
        anim.unk_22    = 4;
        _Sprite_Load(&sprites[1], &anim);
    }
}

static s32 Shop_item_Init(TaskPool* pool, Task* task, void* args) {
    Shop_item*      item     = task->data;
    Shop_item_Args* itemArgs = args;

    item->shop = itemArgs->shop;
    Shop_item_Load(item, item->sprites, itemArgs);
    return 1;
}

static s32 Shop_item_Update(TaskPool* pool, Task* task, void* args) {
    Shop_item* item = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&item->sprites[i]);
    }
    return 1;
}

static s32 Shop_item_Render(TaskPool* pool, Task* task, void* args) {
    Shop_item* item = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (item->shouldRender[i] != 0) {
            Sprite_RenderFrame(&item->sprites[i]);
        }
    }
    return 1;
}

static s32 Shop_item_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_item* item = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&item->sprites[i]);
    }
    return 1;
}

static s32 Shop_item_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_item_Init,
        .update     = Shop_item_Update,
        .render     = Shop_item_Render,
        .cleanup    = Shop_item_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

// Nonmatching
s32 Shop_item_CreateTask(TaskPool* pool, s32 dataType, u16 itemIndex, ShopObject* shop) {
    Shop_item_Args args;
    args.dataType  = dataType;
    args.itemIndex = itemIndex;
    args.shop      = shop;

    args.itemFrame  = shop->unk_738[itemIndex]->unk_00;
    args.badgeFrame = shop->unk_738[itemIndex]->unk_02;
    args.isUnlocked = shop->unk_738[itemIndex]->unk_04;
    return EasyTask_CreateTask(pool, &Tsk_Shop_item, NULL, 0, NULL, &args);
}

void func_ov043_020b6dc8(TaskPool* pool, u32 taskId) {
    Shop_item* item = EasyTask_GetTaskData(pool, taskId);

    Sprite_Release(&item->sprites[0]);
    Sprite_Release(&item->sprites[1]);
}
