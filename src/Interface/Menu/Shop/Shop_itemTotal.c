#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[2];
    /* 0x080 */ BOOL        unk_080[2];
    /* 0x088 */ ShopObject* shop;
} Shop_itemTotal; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         unk_08;
    /* 0x0A */ u16         unk_0A;
    /* 0x0C */ u16         unk_0C;
    /* 0x0E */ char        unk_0E[2];
} Shop_itemTotal_Args; // Size: 0x10

static SpriteFrameInfo* Shop_itemTotal_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Shop_itemTotal_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_itemTotal = {"Tsk_Shop_itemTotal", Shop_itemTotal_RunTask, sizeof(Shop_itemTotal)};

static const SpriteAnimation data_ov043_020ca840 = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Shop_itemTotal_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020ca1b8,
    .unk_18            = 0,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* Shop_itemTotal_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3B6000);
}

static void Shop_itemTotal_Load(Shop_itemTotal* taskData, Sprite* sprites, Shop_itemTotal_Args* args) {
    SpriteAnimation anim = data_ov043_020ca840;

    anim.dataType = (u16)args->dataType;
    anim.bits_7_9 = 5;

    if (args->unk_08 == 0xFFFF) {
        anim.posX            = 0;
        anim.posY            = 0;
        taskData->unk_080[0] = FALSE;
        taskData->unk_080[1] = FALSE;
        _Sprite_Load(&sprites[0], &anim);
        _Sprite_Load(&sprites[1], &anim);
    } else {
        anim.posX = 0xEB;
        anim.posY = 0x54;

        if (args->unk_0A == 0) {
            taskData->unk_080[0] = TRUE;
            taskData->unk_080[1] = TRUE;
            anim.binIden         = &data_ov043_020ca1f0;
            anim.unk_18          = 2;
            anim.packIndex       = args->unk_0C + 1;
            anim.animIndex       = 1;
            anim.unk_22          = 1;
            _Sprite_Load(&sprites[0], &anim);

            anim.binIden   = &data_ov043_020ca1b8;
            anim.unk_18    = 0;
            anim.packIndex = 1;
            anim.animIndex = 0x17;
            anim.unk_22    = 4;
            _Sprite_Load(&sprites[1], &anim);
            return;
        } else {

            taskData->unk_080[0] = TRUE;
            taskData->unk_080[1] = FALSE;
            anim.binIden         = &data_ov043_020ca1f8;
            anim.unk_18          = 2;
            anim.packIndex       = args->unk_0C + 1;
            anim.animIndex       = 1;
            anim.unk_22          = 1;
            _Sprite_Load(&sprites[0], &anim);

            anim.unk_22 = 4;
            _Sprite_Load(&sprites[1], &anim);
        }
    }
}

static s32 Shop_itemTotal_Init(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemTotal*      itemTotal = arg1->data;
    Shop_itemTotal_Args* args      = arg2;

    itemTotal->shop = args->shop;
    Shop_itemTotal_Load(itemTotal, itemTotal->unk_000, args);
    return 1;
}

static s32 Shop_itemTotal_Update(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemTotal* itemTotal = arg1->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&itemTotal->unk_000[i]);
    }
    return 1;
}

static s32 Shop_itemTotal_Render(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemTotal* itemTotal = arg1->data;

    for (s32 i = 0; i < 2; i++) {
        if (itemTotal->unk_080[i] != 0) {
            Sprite_RenderFrame(&itemTotal->unk_000[i]);
        }
    }
    return 1;
}

static s32 Shop_itemTotal_Destroy(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemTotal* itemTotal = arg1->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&itemTotal->unk_000[i]);
    }
    return 1;
}

static s32 Shop_itemTotal_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = {
        .initialize = Shop_itemTotal_Init,
        .update     = Shop_itemTotal_Update,
        .render     = Shop_itemTotal_Render,
        .cleanup    = Shop_itemTotal_Destroy,
    };

    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 Shop_itemTotal_CreateTask(TaskPool* pool, s32 dataType, u16 arg2, ShopObject* shop) {
    Shop_itemTotal_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    args.unk_08   = shop->slots[shop->unk_848].itemID;
    args.unk_0A   = shop->slots[shop->unk_848].unk_04;
    args.unk_0C   = shop->slots[shop->unk_848].unk_02;
    return EasyTask_CreateTask(pool, &Tsk_Shop_itemTotal, NULL, 0, NULL, &args);
}
