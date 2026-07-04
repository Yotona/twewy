#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[7];
    /* 0x1C0 */ BOOL        unk_1C0[7];
    /* 0x1DC */ ShopObject* unk_1DC;
} Shop_itemPrice; // Size: 0x1E0

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         unk_08;
    /* 0x0A */ u16         unk_0A;
    /* 0x0C */ u32         unk_0C;
    /* 0x10 */ u8          unk_10;
    /* 0x14 */ s32         unk_14;
} Shop_itemPrice_Args; // Size: 0x18

static SpriteFrameInfo* Shop_itemPrice_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_itemPrice_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_itemPrice = {"Tsk_Shop_itemPrice", Shop_itemPrice_RunTask, sizeof(Shop_itemPrice)};

static const SpriteAnimation data_ov043_020ca768 = {
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
    .unk_08     = Shop_itemPrice_GetFrameInfo,
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

static SpriteFrameInfo* Shop_itemPrice_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (arg2) {
        case 1:
            info->unk_00 = 1;
            return info;

        case 2: {
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;
            if (arg0->animData != NULL && arg0->frameDataTable != NULL && arg0->unk16 >= 0) {
                info->unk_04 = *((u16*)arg0->frameDataTable + (arg0->unk16 * 4 + 1));
                info->unk_08 = (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
            }
            info->unk_10 = 0x398000;
            return info;
        } break;
    }

    return NULL;
}

// Nonmatching
static void Shop_itemPrice_Load(Shop_itemPrice* arg0, Sprite* arg1, Shop_itemPrice_Args* args) {
    SpriteAnimation anim = data_ov043_020ca768;
    u32             digits[7];
    s16             x;
    s16             y;

    anim.dataType = args->dataType;

    if (args->unk_0A == 0xFFFF) {
        for (s32 i = 0; i < 7; i++) {
            arg0->unk_1C0[i] = FALSE;
            _Sprite_Load(&arg1[i], &anim);
        }
        return;
    }

    x = (s16)((args->unk_08 % 6) * 0x21 + 0x16);
    y = (s16)((args->unk_08 / 6) * 0x29 + 0x27);

    if (args->unk_14 == 1) {
        anim.unk_04 = x;
        anim.unk_06 = y;
        anim.unk_2A = (args->unk_10 == 1) ? 13 : 14;

        arg0->unk_1C0[0] = TRUE;
        for (s32 i = 1; i < 7; i++) {
            arg0->unk_1C0[i] = FALSE;
        }

        for (s32 i = 0; i < 7; i++) {
            _Sprite_Load(&arg1[i], &anim);
        }
        return;
    }

    for (s32 i = 0; i < 7; i++) {
        arg0->unk_1C0[i] = TRUE;
    }

    {
        u32 value = args->unk_0C;

        digits[0] = value / 1000000;
        value %= 1000000;
        digits[1] = value / 100000;
        value %= 100000;
        digits[2] = value / 10000;
        value %= 10000;
        digits[3] = value / 1000;
        value %= 1000;
        digits[4] = value / 100;
        value %= 100;
        digits[5] = value / 10;
        digits[6] = value % 10;
    }

    for (s32 i = 0; i < 6; i++) {
        if (digits[i] != 0) {
            break;
        }
        arg0->unk_1C0[i] = FALSE;
    }

    for (s32 i = 0; i < 7; i++) {
        anim.unk_2A = (u16)(digits[i] + 1);
        anim.unk_04 = (u16)(x - 0xC + (i * 4));
        anim.unk_06 = (u16)(y + 0x14);
        _Sprite_Load(&arg1[i], &anim);
    }
}

static s32 Shop_itemPrice_Init(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemPrice*      itemPrice = arg1->data;
    Shop_itemPrice_Args* args      = arg2;

    Shop_itemPrice_Load(itemPrice, itemPrice->unk_000, args);
    itemPrice->unk_1DC = args->shop;
    return 1;
}

static s32 Shop_itemPrice_Update(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemPrice* itemPrice = arg1->data;

    for (s32 i = 0; i < 7; i++) {
        Sprite_Update(&itemPrice->unk_000[i]);
    }
    return 1;
}

static s32 Shop_itemPrice_Render(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemPrice* itemPrice = arg1->data;

    for (s32 i = 0; i < 7; i++) {
        if (itemPrice->unk_1C0[i] != 0) {
            Sprite_RenderFrame(&itemPrice->unk_000[i]);
        }
    }
    return 1;
}

static s32 Shop_itemPrice_Destroy(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_itemPrice* itemPrice = arg1->data;

    for (s32 i = 0; i < 7; i++) {
        Sprite_Release(&itemPrice->unk_000[i]);
    }
    return 1;
}

static s32 Shop_itemPrice_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_itemPrice_Init,
        .update     = Shop_itemPrice_Update,
        .render     = Shop_itemPrice_Render,
        .cleanup    = Shop_itemPrice_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

// Nonmatching
s32 Shop_itemPrice_CreateTask(TaskPool* arg0, s32 dataType, s16 arg2, ShopObject* shop) {
    Shop_itemPrice_Args args;

    args.dataType = dataType;
    args.shop     = shop;

    args.unk_08 = shop->slots[arg2].cost;
    args.unk_0A = shop->unk_738[arg2]->unk_00;
    args.unk_0C = shop->unk_738[arg2]->unk_08;
    args.unk_10 = shop->unk_738[arg2]->unk_0C;
    args.unk_14 = shop->unk_738[arg2]->unk_10;
    return EasyTask_CreateTask(arg0, &Tsk_Shop_itemPrice, NULL, 0, NULL, &args);
}
