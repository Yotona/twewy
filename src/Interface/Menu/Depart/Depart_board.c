#include "Interface/Menu/Depart.h"
#include "Player/Inventory.h"

extern BinIdentifier data_ov043_020cadb0;

typedef struct {
    /* 0x00 */ Sprite        sprites[2];
    /* 0x80 */ s32           unk_80[2];
    /* 0x88 */ DepartObject* depart;
} DepartBoard; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32           dataType;
    /* 0x04 */ DepartObject* depart;
    /* 0x08 */ u16           unk_08;
    /* 0x0A */ u16           itemID;
    /* 0x0C */ u16           unk_0C;
} DepartBoardArgs;

static SpriteFrameInfo* func_ov043_020be7dc(Sprite* sprite, s32 arg1, s32 mode);
static s32              DepartBoard_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle data_ov043_020cae7c = {"Tsk_Depart_board", DepartBoard_RunTask, sizeof(DepartBoard)};

static const SpriteAnimation data_ov043_020cae98 = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = func_ov043_020be7dc,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020cadb0,
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

SpriteFrameInfo* func_ov043_020be7dc(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info->unk_10 = 0x352000;
            return info;
        } break;
    }

    return NULL;
}

void func_ov043_020be888(DepartBoard* board, Sprite* sprite, DepartBoardArgs* args) {
    DepartObject* depart = board->depart;

    SpriteAnimation anim = data_ov043_020cae98;

    anim.dataType  = args->dataType;
    anim.packIndex = args->unk_08;
    anim.unk_04    = (args->unk_08 * 37) + data_ov043_020ccd00[depart->unk_0C].x - 54;
    anim.unk_06    = (args->unk_08 * 10) + data_ov043_020ccd00[depart->unk_0C].y;

    board->unk_80[0] = 1;
    board->unk_80[1] = 0;

    _Sprite_Load(&sprite[0], &anim);
    _Sprite_Load(&sprite[1], &anim);
}

s32 DepartBoard_Init(TaskPool* pool, Task* task, void* args) {
    DepartBoard*     board     = task->data;
    DepartBoardArgs* boardArgs = args;

    board->depart = boardArgs->depart;
    func_ov043_020be888(board, board->sprites, boardArgs);
    return 1;
}

s32 DepartBoard_Update(TaskPool* pool, Task* task, void* args) {
    DepartBoard* board = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&board->sprites[i]);
    }
    return 1;
}

s32 DepartBoard_Render(TaskPool* pool, Task* task, void* args) {
    DepartBoard* board = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (board->unk_80[i] != 0) {
            Sprite_RenderFrame(&board->sprites[i]);
        }
    }
    return 1;
}

s32 DepartBoard_Release(TaskPool* pool, Task* task, void* args) {
    DepartBoard* board = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&board->sprites[i]);
    }
    return 1;
}

s32 DepartBoard_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov043_020cae88 = {
        .initialize = DepartBoard_Init,
        .update     = DepartBoard_Update,
        .render     = DepartBoard_Render,
        .cleanup    = DepartBoard_Release,
    };
    return data_ov043_020cae88.iter[stage](pool, task, args);
}

s32 DepartBoard_CreateTask(TaskPool* pool, s32 dataType, u16 arg2, DepartObject* depart) {
    DepartBoardArgs args;

    args.dataType = dataType;
    args.depart   = depart;
    args.unk_08   = arg2;
    args.itemID   = depart->unk_10[arg2].itemID;

    ItemCategory category = Inventory_GetCategory(args.itemID);

    if (category == ITEM_CATEGORY_THREAD) {
        RawItemData itemData;
        func_ov043_020bd984(&itemData, Inventory_GetCategorizedIndex(args.itemID));
        args.unk_0C = itemData.unk_00;
    } else if (category == ITEM_CATEGORY_FOOD) {
        RawFoodData foodData;
        func_ov043_020bd9b0(&foodData, Inventory_GetCategorizedIndex(args.itemID));
        args.unk_0C = foodData.unk_00;
    } else {
        RawTreasureData treasureData;
        func_ov043_020bd9dc(&treasureData, Inventory_GetCategorizedIndex(args.itemID));
        args.unk_0C = treasureData.unk_00;
    }
    EasyTask_CreateTask(pool, &data_ov043_020cae7c, NULL, 0, NULL, &args);
}
