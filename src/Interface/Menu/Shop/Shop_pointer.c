#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ BOOL        shouldRender;
    /* 0x44 */ ShopObject* shop;
} Shop_pointer; // Size: 0x48

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_pointer_Args;

SpriteFrameInfo* Shop_pointer_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Shop_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_pointer = {"Tsk_Shop_pointer", Shop_pointer_RunTask, 0x48};

static const SpriteAnimation data_ov043_020caaac = {

    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_pointer_GetFrameInfo,
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

SpriteFrameInfo* Shop_pointer_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

void Shop_pointer_Load(Shop_pointer* pointer, Shop_pointer_Args* pointerArgs) {
    SpriteAnimation anim = data_ov043_020caaac;

    anim.dataType = pointerArgs->dataType;
    anim.unk_2A   = 67;
    _Sprite_Load(&pointer->sprite, &anim);
}

s32 Shop_pointer_Init(TaskPool* pool, Task* task, void* args) {
    Shop_pointer*      pointer     = task->data;
    Shop_pointer_Args* pointerArgs = args;

    pointer->shop         = pointerArgs->shop;
    pointer->shouldRender = FALSE;
    Shop_pointer_Load(pointer, pointerArgs);
    return 1;
}

s32 Shop_pointer_Update(TaskPool* pool, Task* task, void* args) {
    Shop_pointer* pointer = task->data;
    TouchCoord    coord;

    if (TouchInput_IsTouchActive() == FALSE) {
        pointer->shouldRender = 0;
    } else {
        TouchInput_GetCoord(&coord);
        pointer->sprite.posX  = coord.x;
        pointer->sprite.posY  = coord.y;
        pointer->shouldRender = 1;
    }

    Sprite_Update(&pointer->sprite);
    return 1;
}

s32 Shop_pointer_Render(TaskPool* pool, Task* task, void* args) {
    Shop_pointer* pointer = task->data;

    if (pointer->shouldRender == TRUE) {
        Sprite_RenderFrame(&pointer->sprite);
    }
    return 1;
}

s32 Shop_pointer_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_pointer* pointer = task->data;

    Sprite_Release(&pointer->sprite);
    return 1;
}

s32 Shop_pointer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_pointer_Init,
        .update     = Shop_pointer_Update,
        .render     = Shop_pointer_Render,
        .cleanup    = Shop_pointer_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_pointer_CreateTask(TaskPool* arg0, s32 dataType, ShopObject* shop) {
    Shop_pointer_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(arg0, &Tsk_Shop_pointer, NULL, 0, NULL, &args);
}
