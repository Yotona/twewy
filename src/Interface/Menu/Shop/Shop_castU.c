#include "Interface/Menu/Shop.h"

extern void func_ov043_020af478(void*, s16);

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ BOOL        shouldRender;
    /* 0x44 */ ShopObject* shop;
    /* 0x48 */ u16         unk_48;
} Shop_castU; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ u16         unk_8;
} Shop_castU_Args;

static SpriteFrameInfo* Shop_castU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Shop_castU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_castU = {"Tsk_Shop_castU", Shop_castU_RunTask, sizeof(Shop_castU)};

static const SpriteAnimation Shop_castU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 204,
    .unk_06            = 96,
    .frameInfoCallback = Shop_castU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020ca1e8,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* Shop_castU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Shop_castU_Load(Shop_castU* cast, Sprite* sprite, Shop_castU_Args* castArgs) {
    ShopObject* shop = cast->shop;

    SpriteAnimation anim = Shop_castU_Anim;

    anim.dataType  = castArgs->dataType;
    anim.unk_20    = 1;
    anim.unk_1C    = 2;
    anim.unk_26    = 3;
    anim.unk_28    = 4;
    anim.unk_2A    = castArgs->unk_8 + 1;
    anim.packIndex = shop->unk_776 + 30;

    cast->shouldRender = TRUE;
    cast->unk_48       = castArgs->unk_8;

    _Sprite_Load(sprite, &anim);
}

static s32 Shop_castU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_castU*      cast     = task->data;
    Shop_castU_Args* castArgs = args;

    cast->shop = castArgs->shop;
    Shop_castU_Load(cast, &cast->sprite, castArgs);
    return 1;
}

static s32 Shop_castU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_castU* cast = task->data;
    ShopObject* shop = cast->shop;

    if (cast->unk_48 != shop->unk_7B6) {
        func_ov043_020af478(cast, shop->unk_7B6 + 1);
        cast->unk_48 = shop->unk_7B6;
    }
    Sprite_Update(&cast->sprite);
    return 1;
}

static s32 Shop_castU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_castU* cast = task->data;

    if (cast->shouldRender) {
        Sprite_RenderFrame(&cast->sprite);
    }
    return 1;
}

static s32 Shop_castU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_castU* cast = task->data;

    Sprite_Release(&cast->sprite);
    return 1;
}

static s32 Shop_castU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_castU_Init,
        .update     = Shop_castU_Update,
        .render     = Shop_castU_Render,
        .cleanup    = Shop_castU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_castU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_castU_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    args.unk_8    = shop->unk_7B6;
    return EasyTask_CreateTask(pool, &Tsk_Shop_castU, NULL, 0, NULL, &args);
}
