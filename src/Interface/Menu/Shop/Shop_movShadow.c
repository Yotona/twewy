#include "Interface/Menu/Shop.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ s32         unk_40;
    /* 0x44 */ ShopObject* shop;
} Shop_movShadow; // Size: 0x48

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_movShadow_Args;

static SpriteFrameInfo* Shop_movShadow_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_movShadow_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_movShadow = {"Tsk_Shop_movShadow", Shop_movShadow_RunTask, sizeof(Shop_movShadow)};

static const SpriteAnimation data_ov043_020ca960 = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_movShadow_GetFrameInfo,
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

static SpriteFrameInfo* Shop_movShadow_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* temp = &data_0206b408;

    switch (arg2) {
        case 1:
            temp->unk_00 = 1;
            return temp;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;
            if (arg0->animData != 0 && arg0->frameDataTable != 0) {
                if ((s32)arg0->unk16 >= 0) {
                    temp->unk_04 = *((u16*)arg0->frameDataTable + (((arg0->unk16 * 4) + 1)));
                    temp->unk_08 =
                        (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
                }
            }
            temp->unk_10 = 0x3D4000;
            return temp;
        }

        default:
            return NULL;
    }
}

// Nonmatching
static void Shop_movShadow_Load(Shop_movShadow* shadow, Sprite* sprite, Shop_movShadow_Args* args) {
    ShopObject*     shop = shadow->shop;
    SpriteAnimation anim = data_ov043_020ca960;

    anim.unk_2A   = 36;
    anim.dataType = args->dataType;
    anim.unk_04   = shop->unk_840 + 4;
    anim.unk_06   = shop->unk_842 + 6;

    data_0206a890.unk_0C = 20;
    _Sprite_Load(sprite, &anim);
    shadow->unk_40 = 1;
}

static s32 Shop_movShadow_Init(TaskPool* pool, Task* task, void* args) {
    Shop_movShadow*      shadow     = task->data;
    Shop_movShadow_Args* shadowArgs = args;

    shadow->shop = shadowArgs->shop;
    Shop_movShadow_Load(shadow, &shadow->sprite, shadowArgs);
    return 1;
}

static s32 Shop_movShadow_Update(TaskPool* pool, Task* task, void* arg2) {
    Shop_movShadow* shadow = task->data;
    ShopObject*     shop   = shadow->shop;

    shadow->sprite.posX = shop->unk_840 + 4;
    shadow->sprite.posY = shop->unk_842 + 6;
    Sprite_Update(&shadow->sprite);
    return 1;
}

static s32 Shop_movShadow_Render(TaskPool* pool, Task* task, void* arg2) {
    Shop_movShadow* shadow = task->data;

    if (shadow->unk_40 != 0) {
        Sprite_RenderFrame(&shadow->sprite);
    }
    return 1;
}

static s32 Shop_movShadow_Destroy(TaskPool* pool, Task* task, void* arg2) {
    Shop_movShadow* shadow = task->data;

    Sprite_Release(&shadow->sprite);
    return 1;
}

static s32 Shop_movShadow_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    TaskStages stages = {
        .initialize = Shop_movShadow_Init,
        .update     = Shop_movShadow_Update,
        .render     = Shop_movShadow_Render,
        .cleanup    = Shop_movShadow_Destroy,
    };
    return stages.iter[arg3](pool, task, arg2);
}

s32 Shop_movShadow_CreateTask(TaskPool* pool, s32 arg1, s32 arg2) {
    Shop_movShadow_Args args;

    args.dataType = arg1;
    args.shop     = (ShopObject*)arg2;
    return EasyTask_CreateTask(pool, &Tsk_Shop_movShadow, NULL, 0, NULL, &args);
}
