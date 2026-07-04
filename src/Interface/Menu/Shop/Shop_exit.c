#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ s32         unk_40;
    /* 0x44 */ ShopObject* shop;
} Shop_exit;

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_exit_Args;

static SpriteFrameInfo* Shop_exit_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_exit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation data_ov043_020ca3e4 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 231,
    .unk_06     = 9,
    .unk_08     = Shop_exit_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1b0,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 3,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static const TaskHandle Tsk_Shop_exit = {"Tsk_Shop_exit", Shop_exit_RunTask, sizeof(Shop_exit)};

static SpriteFrameInfo* Shop_exit_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

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
            info = temp;
        } break;
    }

    return info;
}

static void Shop_exit_Load(Shop_exit* exit, Sprite* sprite, Shop_exit_Args* exitArgs) {
    SpriteAnimation anim = data_ov043_020ca3e4;
    anim.dataType        = exitArgs->dataType;
    anim.unk_2A          = 41;
    _Sprite_Load(sprite, &anim);
}

static s32 Shop_exit_Init(TaskPool* pool, Task* task, void* args) {
    Shop_exit*      exit     = task->data;
    Shop_exit_Args* exitArgs = args;

    exit->unk_40 = 1;
    exit->shop   = exitArgs->shop;
    Shop_exit_Load(exit, &exit->sprite, exitArgs);
    return 1;
}

static s32 Shop_exit_Update(TaskPool* pool, Task* task, void* args) {
    Shop_exit*  exit = task->data;
    ShopObject* shop = exit->shop;

    if (shop->unk_838 == 1) {
        func_ov043_020af42c(exit, 42);
        if (shop->unk_839 != 0) {
            shop->unk_839--;
        } else {
            shop->unk_838 = 0;
        }
    } else {
        func_ov043_020af42c(exit, 41);
    }
    Sprite_Update(&exit->sprite);
    return 1;
}

static s32 Shop_exit_Render(TaskPool* pool, Task* task, void* args) {
    Shop_exit* exit = task->data;

    Sprite_RenderFrame(&exit->sprite);
    return 1;
}

static s32 Shop_exit_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_exit* exit = task->data;

    Sprite_Release(&exit->sprite);
    return 1;
}

static s32 Shop_exit_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_exit_Init,
        .update     = Shop_exit_Update,
        .render     = Shop_exit_Render,
        .cleanup    = Shop_exit_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_exit_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_exit_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_exit, NULL, 0, NULL, &args);
}
