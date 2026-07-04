#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[4];
    /* 0x100 */ BOOL        unk_100[4];
    /* 0x110 */ ShopObject* shop;
    /* 0x114 */ u8          unk_114;
    /* 0x115 */ char        unk_115[0x118 - 0x115];
} Shop_itemTotalNum; // Size: 0x118

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_itemTotalNum_Args;

static SpriteFrameInfo* Shop_itemTotalNum_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_itemTotalNum_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_itemTotalNum = {"Tsk_Shop_itemTotalNum", Shop_itemTotalNum_RunTask,
                                                 sizeof(Shop_itemTotalNum)};

static const SpriteAnimation Shop_itemTotalNum_Anim = {
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
    .unk_08     = Shop_itemTotalNum_GetFrameInfo,
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

static void Shop_itemTotalNum_CalcDigits(Shop_itemTotalNum* arg0, s32 arg1, u16* arg2) {
    s32 value = arg1;
    u16 rem;

    arg2[0] = value / 100;
    rem     = value % 100;
    arg2[1] = rem / 10;
    arg2[2] = rem % 10;

    for (s32 i = 0; i < 4; i++) {
        arg0->unk_100[i] = TRUE;
    }

    arg0->unk_114 = 3;

    for (s32 i = 0; i < 2; i++) {
        if (arg2[i] != 0) {
            return;
        }
        arg0->unk_100[i + 1] = FALSE;
        arg0->unk_114--;
    }
}

static SpriteFrameInfo* Shop_itemTotalNum_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (arg2) {
        case 1:
            info->unk_00 = 1;
            return info;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;
            if (arg0->animData != NULL && arg0->frameDataTable != NULL && arg0->unk16 >= 0) {
                temp->unk_04 = *((u16*)arg0->frameDataTable + (arg0->unk16 * 4 + 1));
                temp->unk_08 = (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
            }
            temp->unk_10 = 0x398000;
            return temp;
        } break;
    }

    return NULL;
}

static void Shop_itemTotalNum_Load(Shop_itemTotalNum* totalNum, Sprite* arg1, Shop_itemTotalNum_Args* arg2) {
    ShopObject* shop = totalNum->shop;

    SpriteAnimation anim = Shop_itemTotalNum_Anim;
    u16             digits[3];

    anim.dataType = arg2->dataType;

    Shop_itemTotalNum_CalcDigits(totalNum, func_02023010(shop->slots[shop->unk_848].unk_00), digits);

    anim.unk_2A = 0x0C;
    anim.unk_04 = ((3 - totalNum->unk_114) * 4) + 0xEF;
    anim.unk_06 = 0x67;
    _Sprite_Load(&arg1[0], &anim);

    for (s32 i = 0, posX = 0xEF; i < 3; i++, posX += 4) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = posX;
        anim.unk_06 = 0x67;
        _Sprite_Load(&arg1[i + 1], &anim);
    }
}

static s32 Shop_itemTotalNum_Init(TaskPool* pool, Task* task, void* args) {
    Shop_itemTotalNum*      totalNum = task->data;
    Shop_itemTotalNum_Args* taskArgs = args;

    totalNum->shop = taskArgs->shop;
    Shop_itemTotalNum_Load(totalNum, totalNum->unk_000, taskArgs);
    return 1;
}

static s32 Shop_itemTotalNum_Update(TaskPool* pool, Task* task, void* args) {
    Shop_itemTotalNum* totalNum = task->data;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Update(&totalNum->unk_000[i]);
    }
    return 1;
}

static s32 Shop_itemTotalNum_Render(TaskPool* pool, Task* task, void* args) {
    Shop_itemTotalNum* totalNum = task->data;

    for (s32 i = 0; i < 4; i++) {
        if (totalNum->unk_100[i] != 0) {
            Sprite_RenderFrame(&totalNum->unk_000[i]);
        }
    }
    return 1;
}

static s32 Shop_itemTotalNum_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_itemTotalNum* totalNum = task->data;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Release(&totalNum->unk_000[i]);
    }
    return 1;
}

static s32 Shop_itemTotalNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_itemTotalNum_Init,
        .update     = Shop_itemTotalNum_Update,
        .render     = Shop_itemTotalNum_Render,
        .cleanup    = Shop_itemTotalNum_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Shop_itemTotalNum_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_itemTotalNum_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_itemTotalNum, NULL, 0, NULL, &args);
}
