#include "Interface/Menu/Shop.h"

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);

typedef struct {
    /* 0x00 */ Sprite      sprites[2];
    /* 0x80 */ BOOL        shouldRender[2];
    /* 0x88 */ ShopObject* shop;
} Shop_numLvU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ u16         unk_8;
} Shop_numLvU_Args;

static SpriteFrameInfo* Shop_numLvU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_numLvU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_numLvU = {"Tsk_Shop_numLvU", Shop_numLvU_RunTask, sizeof(Shop_numLvU)};

static const SpriteAnimation data_ov043_020cab60 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_numLvU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1d8,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static void func_ov043_020bae58(Shop_numLvU* num) {
    u32 value = num->shop->unk_778;
    u32 digit[2];
    s16 offsetX;
    u16 i;

    digit[0] = value / 10;
    digit[1] = value % 10;

    for (i = 0; i < 2; i++) {
        num->shouldRender[i] = TRUE;
    }

    if (digit[0] == 0) {
        num->shouldRender[0] = FALSE;
        num->shouldRender[1] = TRUE;
        offsetX              = -3;
    } else {
        num->shouldRender[0] = TRUE;
        num->shouldRender[1] = TRUE;
        offsetX              = 0;
    }

    for (i = 0; i < 2; i++) {
        num->sprites[i].posX = offsetX + (i * 6 + 31);
        num->sprites[i].posY = 183;
        func_ov043_020af42c(&num->sprites[i], digit[i] + 10);
    }
}

static SpriteFrameInfo* Shop_numLvU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void Shop_numLvU_Load(Shop_numLvU* num, Sprite* sprites, Shop_numLvU_Args* args) {
    SpriteAnimation anim = data_ov043_020cab60;
    anim.dataType        = args->dataType;

    u32 digit[2];
    s32 offsetX;
    u32 value = args->unk_8;

    digit[0] = value / 10;
    digit[1] = value % 10;

    for (u16 i = 0; i < 2; i++) {
        num->shouldRender[i] = TRUE;
    }

    if (digit[0] == 0) {
        num->shouldRender[0] = FALSE;
        num->shouldRender[1] = TRUE;
        offsetX              = -3;
    } else {
        num->shouldRender[0] = TRUE;
        num->shouldRender[1] = TRUE;
        offsetX              = 0;
    }

    for (u16 i = 0; i < 2; i++) {
        anim.unk_2A = digit[i] + 10;
        anim.unk_04 = offsetX + (i * 6 + 0x1F);
        anim.unk_06 = 183;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 Shop_numLvU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_numLvU*      num     = task->data;
    Shop_numLvU_Args* numArgs = args;

    num->shop = numArgs->shop;
    Shop_numLvU_Load(num, num->sprites, numArgs);
    return 1;
}

static s32 Shop_numLvU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_numLvU* num = task->data;

    func_ov043_020bae58(num);
    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&num->sprites[i]);
    }
    return 1;
}

static s32 Shop_numLvU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_numLvU* num = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (num->shouldRender[i]) {
            Sprite_RenderFrame(&num->sprites[i]);
        }
    }
    return 1;
}

static s32 Shop_numLvU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_numLvU* num = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&num->sprites[i]);
    }
    return 1;
}

static s32 Shop_numLvU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_numLvU_Init,
        .update     = Shop_numLvU_Update,
        .render     = Shop_numLvU_Render,
        .cleanup    = Shop_numLvU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_numLvU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_numLvU_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    args.unk_8    = shop->unk_778;

    return EasyTask_CreateTask(pool, &Tsk_Shop_numLvU, NULL, 0, NULL, &args);
}
