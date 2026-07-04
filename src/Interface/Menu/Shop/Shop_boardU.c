#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ u16         unk_8;
} Shop_boardU_Args;

typedef struct {
    /* 0x00 */ Sprite      sprite;
    /* 0x40 */ BOOL        shouldRender;
    /* 0x44 */ ShopObject* shop;
} Shop_boardU; // Size: 0x48

static SpriteFrameInfo* Shop_boardU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_boardU_RunTask(TaskPool*, Task* task, void* args, s32 stage);

static const SpriteAnimation data_ov043_020cad38 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_boardU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca200,
    .unk_18     = 2,
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

static const TaskHandle Tsk_Shop_boardU = {"Tsk_Shop_boardU", Shop_boardU_RunTask, sizeof(Shop_boardU)};

static SpriteFrameInfo* Shop_boardU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

static void func_ov043_020bca50(Shop_boardU* board, Sprite* sprite, Shop_boardU_Args* args, s32 packIndex) {
    // Not yet implemented
}

static void Shop_boardU_Load(Shop_boardU* board, Sprite* sprite, Shop_boardU_Args* args) {
    SpriteAnimation anim = data_ov043_020cad38;
    u32             val;

    anim.dataType = args->dataType;
    anim.unk_04   = 17;
    anim.unk_06   = 19;

    if (args->unk_8 == 0xFFFF) {
        board->shouldRender = FALSE;
        val                 = 1;
    } else {
        ItemCategory category = Inventory_GetCategory(args->unk_8);
        if (category == ITEM_CATEGORY_THREAD) {
            RawItemData itemData;
            func_ov043_020af4f0(&itemData, Inventory_GetCategorizedIndex(args->unk_8));
            val = itemData.unk_00;
        } else if (category == ITEM_CATEGORY_FOOD) {
            RawFoodData foodData;
            func_ov043_020af51c(&foodData, Inventory_GetCategorizedIndex(args->unk_8));
            val = foodData.unk_00;
        } else {
            RawTreasureData treasureData;
            func_ov043_020af548(&treasureData, Inventory_GetCategorizedIndex(args->unk_8));
            val = treasureData.unk_00;
        }
        board->shouldRender = TRUE;
    }

    _Sprite_Load(sprite, &anim);
    func_ov043_020bca50(board, sprite, args, val + 1 & 0xFFFF);
}

static s32 Shop_boardU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_boardU*      board     = task->data;
    Shop_boardU_Args* boardArgs = args;

    board->shop = boardArgs->shop;
    Shop_boardU_Load(board, &board->sprite, boardArgs);
    return 1;
}

static s32 Shop_boardU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_boardU* board = task->data;

    Sprite_Update(&board->sprite);
    return 1;
}

static s32 Shop_boardU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_boardU* board = task->data;

    if (board->shouldRender) {
        Sprite_RenderFrame(&board->sprite);
    }
    return 1;
}

static s32 Shop_boardU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_boardU* board = task->data;

    Sprite_Release(&board->sprite);
    return 1;
}

static s32 Shop_boardU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_boardU_Init,
        .update     = Shop_boardU_Update,
        .render     = Shop_boardU_Render,
        .cleanup    = Shop_boardU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_boardU_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_boardU_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    args.unk_8    = shop->unk_76E;
    return EasyTask_CreateTask(pool, &Tsk_Shop_boardU, NULL, 0, NULL, &args);
}
