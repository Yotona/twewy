#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ s32         unk_40;
    /* 0x44 */ ShopObject* shop;
} Shop_brdLogo; // Size: 0x48

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ char        unk_08[4];
} Shop_brdLogo_Args;

static SpriteFrameInfo* Shop_brdLogo_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_brdLogo_ShouldRender(Shop_brdLogo* brdLogo);
static void             Shop_brdLogo_Load(Shop_brdLogo* brdLogo, Sprite* sprite, Shop_brdLogo_Args* args);
static s32              Shop_brdLogo_Init(TaskPool* pool, Task* task, void* args);
static s32              Shop_brdLogo_Update(TaskPool* pool, Task* task, void* args);
static s32              Shop_brdLogo_Render(TaskPool* pool, Task* task, void* args);
static s32              Shop_brdLogo_Destroy(TaskPool* pool, Task* task, void* args);
static s32              Shop_brdLogo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_brdLogo = {"Tsk_Shop_brdLogo", Shop_brdLogo_RunTask, 0x48};

static const SpriteAnimation data_ov043_020ca9a8 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 60,
    .unk_06     = 160,
    .unk_08     = Shop_brdLogo_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca208,
    .unk_18     = 2,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* Shop_brdLogo_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

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
            info = temp;
        } break;

        default:
            break;
    }

    return info;
}

BOOL Shop_brdLogo_ShouldRender(Shop_brdLogo* brdLogo) {
    ShopObject* shop   = brdLogo->shop;
    u16         itemId = *((u16*)((u8*)shop + shop->unk_848 * 0x2C + 6));

    if ((u32)(u8)itemId <= 0xC && shop->unk_854 == 0) {
        return TRUE;
    }
    return FALSE;
}

void Shop_brdLogo_Load(Shop_brdLogo* brdLogo, Sprite* sprite, Shop_brdLogo_Args* args) {
    ShopObject* shop = brdLogo->shop;

    SpriteAnimation anim = data_ov043_020ca9a8;
    anim.dataType        = args->dataType;

    u8 itemId = *((u16*)((u8*)shop + shop->unk_848 * 0x2C + 6));
    if (itemId <= 12) {
        anim.packIndex = itemId + 1;

        if (shop->unk_854 == 0) {
            brdLogo->unk_40 = 1;
        } else {
            brdLogo->unk_40 = 0;
        }
    } else {
        anim.packIndex  = 1;
        brdLogo->unk_40 = 0;
    }

    _Sprite_Load(sprite, &anim);
}

s32 Shop_brdLogo_Init(TaskPool* pool, Task* task, void* args) {
    Shop_brdLogo*      brdLogo     = task->data;
    Shop_brdLogo_Args* brdLogoArgs = args;

    brdLogo->shop = brdLogoArgs->shop;
    Shop_brdLogo_Load(brdLogo, &brdLogo->sprite, brdLogoArgs);
    return 1;
}

s32 Shop_brdLogo_Update(TaskPool* pool, Task* task, void* args) {
    Shop_brdLogo* brdLogo = task->data;

    if (Shop_brdLogo_ShouldRender(brdLogo) == 1) {
        brdLogo->unk_40 = 1;
    } else {
        brdLogo->unk_40 = 0;
    }
    Sprite_Update(&brdLogo->sprite);
    return 1;
}

s32 Shop_brdLogo_Render(TaskPool* pool, Task* task, void* args) {
    Shop_brdLogo* brdLogo = task->data;

    if (brdLogo->unk_40 != 0) {
        Sprite_RenderAltPalette(&brdLogo->sprite, NULL, NULL, 1);
    }
    return 1;
}

s32 Shop_brdLogo_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_brdLogo* brdLogo = task->data;

    Sprite_Release(&brdLogo->sprite);
    return 1;
}

s32 Shop_brdLogo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_brdLogo_Init,
        .update     = Shop_brdLogo_Update,
        .render     = Shop_brdLogo_Render,
        .cleanup    = Shop_brdLogo_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_brdLogo_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_brdLogo_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_brdLogo, NULL, 0, NULL, &args);
}
