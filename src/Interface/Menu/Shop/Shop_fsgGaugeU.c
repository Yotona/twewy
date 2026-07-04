#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprites[3];
    /* 0xC0 */ char        unk_C0[0xCC - 0xC0];
    /* 0xCC */ ShopObject* shop;
    /* 0xD0 */ char        unk_D0[0xF0 - 0xD0];
} Shop_fsgGaugeU; // Size: 0xF0

typedef struct {
    /* 0x0 */ s32         unk_0;
    /* 0x4 */ ShopObject* shop;
} Shop_fsgGaugeU_Args;

static s32 Shop_fsgGaugeU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_fsgGaugeU = {"Tsk_Shop_fsgGaugeU", Shop_fsgGaugeU_RunTask, 0xF0};

static void func_ov043_020bbcc8(Shop_fsgGaugeU* gauge, s32 arg1, u32 arg2) {
    // Not yet implemented
}

SpriteFrameInfo* Shop_fsgGaugeU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void Shop_fsgGaugeU_Load(Shop_fsgGaugeU* gauge, Sprite* sprite, Shop_fsgGaugeU_Args* gaugeArgs) {
    // Not yet implemented
}

static s32 Shop_fsgGaugeU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_fsgGaugeU*      gauge     = task->data;
    Shop_fsgGaugeU_Args* gaugeArgs = args;

    gauge->shop = gaugeArgs->shop;
    Shop_fsgGaugeU_Load(gauge, gauge->sprites, gaugeArgs);
    return 1;
}

static s32 Shop_fsgGaugeU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_fsgGaugeU* gauge = task->data;
    ShopObject*     shop  = gauge->shop;

    func_ov043_020bbcc8(gauge, 1, shop->unk_788);
    func_ov043_020bbcc8(gauge, 2, shop->unk_78A);

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&gauge->sprites[i]);
    }
    return 1;
}

static s32 Shop_fsgGaugeU_Render(TaskPool* pool, Task* task, void* args) {
    // Not yet implemented
}

static s32 Shop_fsgGaugeU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_fsgGaugeU* gauge = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&gauge->sprites[i]);
    }
    return 1;
}

static s32 Shop_fsgGaugeU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_fsgGaugeU_Init,
        .update     = Shop_fsgGaugeU_Update,
        .render     = Shop_fsgGaugeU_Render,
        .cleanup    = Shop_fsgGaugeU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_fsgGaugeU_CreateTask(TaskPool* pool, s32 arg1, ShopObject* shop) {
    Shop_fsgGaugeU_Args args;
    args.unk_0 = arg1;
    args.shop  = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_fsgGaugeU, NULL, 0, NULL, &args);
}
