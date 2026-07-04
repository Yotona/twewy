#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x00 */ Sprite      sprites[2];
    /* 0x80 */ BOOL        shouldRender[2];
    /* 0x88 */ ShopObject* shop;
} Shop_mov; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         unk_08;
    /* 0x0A */ u16         unk_0A;
    /* 0x0C */ u16         unk_0C;
} Shop_mov_Args;

static SpriteFrameInfo* Shop_mov_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_mov_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_mov = {"Tsk_Shop_mov", Shop_mov_RunTask, sizeof(Shop_mov)};

static const SpriteAnimation data_ov043_020ca918 = {
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
    .unk_08     = Shop_mov_GetFrameInfo,
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

static SpriteFrameInfo* Shop_mov_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* info = &data_0206b408;
    switch (arg2) {
        case 1:
            info->unk_00 = 1;
            return info;

        case 2:
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (arg0->animData != NULL && arg0->frameDataTable != NULL && arg0->unk16 >= 0) {
                info->unk_04 = *((u16*)arg0->frameDataTable + (arg0->unk16 * 4 + 1));
                info->unk_08 = (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
            }

            info->unk_10 = 0x3E8000;
            return info;

        default:
            return NULL;
    }
}

static void Shop_mov_Load(Shop_mov* mov, Sprite* arg1, Shop_mov_Args* args) {
    ShopObject* shop = mov->shop;

    SpriteAnimation anim = data_ov043_020ca918;

    if (args->unk_08 == 0xFFFF) {
        OS_WaitForever();
        return;
    }

    anim.dataType = args->dataType;
    anim.bits_7_9 = 6;
    anim.unk_04   = shop->unk_840;
    anim.unk_06   = shop->unk_842;

    if (args->unk_0A == 0) {
        mov->shouldRender[0] = TRUE;
        mov->shouldRender[1] = TRUE;

        anim.binIden   = &data_ov043_020ca1f0;
        anim.unk_18    = 2;
        anim.packIndex = args->unk_0C + 1;
        anim.unk_2A    = 1;
        anim.unk_22    = 1;
        _Sprite_Load(arg1, &anim);

        anim.binIden   = &data_ov043_020ca1b8;
        anim.unk_18    = 0;
        anim.packIndex = 1;
        anim.unk_2A    = 0x17;
        anim.unk_22    = 4;
        _Sprite_Load(&arg1[1], &anim);
    } else {
        mov->shouldRender[0] = TRUE;
        mov->shouldRender[1] = FALSE;

        anim.binIden   = &data_ov043_020ca1f8;
        anim.unk_18    = 2;
        anim.packIndex = args->unk_0C + 1;
        anim.unk_2A    = 1;
        anim.unk_22    = 1;
        _Sprite_Load(arg1, &anim);

        anim.unk_22 = 4;
        _Sprite_Load(&arg1[1], &anim);
    }
}

static s32 Shop_mov_Init(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_mov*      mov  = arg1->data;
    Shop_mov_Args* args = arg2;

    mov->shop = args->shop;
    Shop_mov_Load(mov, mov->sprites, args);
    return 1;
}

static s32 Shop_mov_Update(TaskPool* pool, Task* task, void* args) {
    Shop_mov*   mov  = task->data;
    ShopObject* shop = mov->shop;

    for (s32 i = 0; i < 2; i++) {
        mov->sprites[i].posX = shop->unk_840;
        mov->sprites[i].posY = shop->unk_842;
        Sprite_Update(&mov->sprites[i]);
    }

    return 1;
}

static s32 Shop_mov_Render(TaskPool* pool, Task* task, void* args) {
    Shop_mov* mov = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (mov->shouldRender[i] != 0) {
            Sprite_RenderFrame(&mov->sprites[i]);
        }
    }

    return 1;
}

static s32 Shop_mov_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_mov* mov = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&mov->sprites[i]);
    }

    return 1;
}

static s32 Shop_mov_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = {
        .initialize = Shop_mov_Init,
        .update     = Shop_mov_Update,
        .render     = Shop_mov_Render,
        .cleanup    = Shop_mov_Destroy,
    };
    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 Shop_mov_CreateTask(TaskPool* arg0, s32 dataType, s32 arg2, ShopObject* shop) {
    Shop_mov_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    args.unk_08   = shop->unk_738[arg2]->unk_00;
    args.unk_0A   = shop->unk_738[arg2]->unk_04;
    args.unk_0C   = shop->unk_738[arg2]->unk_02;

    return EasyTask_CreateTask(arg0, &Tsk_Shop_mov, NULL, 0, NULL, &args);
}
