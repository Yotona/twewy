#include "Engine/EasyTask.h"
#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprites[2];
    /* 0x80 */ BOOL        shouldRender[2];
    /* 0x88 */ ShopObject* shop;
    /* 0x8C */ u16         itemIndex;
    /* 0x8E */ u16         unk_8E;
} Shop_itemAbl; // Size: 0x90

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         itemIndex;
} Shop_itemAbl_Args;

static SpriteFrameInfo* Shop_itemAbl_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_itemAbl_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_itemAbl = {"Tsk_Shop_itemAbl", Shop_itemAbl_RunTask, sizeof(Shop_itemAbl)};

static const SpriteAnimation Shop_itemAbl_Anim = {
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
    .unk_08     = Shop_itemAbl_GetFrameInfo,
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

static SpriteFrameInfo* Shop_itemAbl_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
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
            frameInfo->unk_10 = 0x3A2000;
            return frameInfo;

        default:
            return NULL;
    }
}

static void Shop_itemAbl_Load(Shop_itemAbl* abl, Sprite* arg1, Shop_itemAbl_Args* arg2) {
    ShopObject* shop = abl->shop;

    SpriteAnimation anim = Shop_itemAbl_Anim;

    abl->shouldRender[0] = TRUE;
    abl->shouldRender[1] = FALSE;
    abl->itemIndex       = arg2->itemIndex;

    anim.dataType = arg2->dataType;
    anim.unk_04   = ((arg2->itemIndex % 6) * 0x21) + 0x16;
    anim.unk_06   = ((arg2->itemIndex / 6) * 0x29) + 0x27;

    anim.unk_2A = 0x0F;
    _Sprite_Load(&arg1[0], &anim);

    anim.unk_2A = 0x10;
    _Sprite_Load(&arg1[1], &anim);

    if (shop->unk_738[abl->itemIndex]->unk_20 == 1) {
        abl->shouldRender[0] = TRUE;
    } else {
        abl->shouldRender[0] = FALSE;
    }
}

static s32 Shop_itemAbl_Init(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemAbl*      itemAbl  = arg1->data;
    Shop_itemAbl_Args* itemArgs = arg2;

    itemAbl->shop = itemArgs->shop;
    Shop_itemAbl_Load(itemAbl, itemAbl->sprites, itemArgs);
    return 1;
}

static s32 Shop_itemAbl_Update(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemAbl* itemAbl = arg1->data;

    if (itemAbl->shop->unk_738[itemAbl->itemIndex]->unk_20 == 1) {
        itemAbl->shouldRender[0] = TRUE;
    } else {
        itemAbl->shouldRender[0] = FALSE;
    }

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&itemAbl->sprites[i]);
    }
    return 1;
}

static s32 Shop_itemAbl_Render(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemAbl* itemAbl = arg1->data;

    for (s32 i = 0; i < 2; i++) {
        if (itemAbl->shouldRender[i]) {
            Sprite_RenderFrame(&itemAbl->sprites[i]);
        }
    }
    return 1;
}

static s32 Shop_itemAbl_Destroy(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemAbl* itemAbl = arg1->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&itemAbl->sprites[i]);
    }
    return 1;
}

static s32 Shop_itemAbl_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = {
        .initialize = Shop_itemAbl_Init,
        .update     = Shop_itemAbl_Update,
        .render     = Shop_itemAbl_Render,
        .cleanup    = Shop_itemAbl_Destroy,
    };

    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 Shop_itemAbl_CreateTask(TaskPool* pool, s32 dataType, s16 itemIndex, ShopObject* shop) {
    Shop_itemAbl_Args args;

    args.dataType  = dataType;
    args.itemIndex = itemIndex;
    args.shop      = shop;

    return EasyTask_CreateTask(pool, &Tsk_Shop_itemAbl, NULL, 0, NULL, &args);
}
