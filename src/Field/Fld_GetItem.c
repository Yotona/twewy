#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Field/FieldTasks.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"
#include "SpriteMgr.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ u16    unk_44;
} Fld_GetItem; // Size: 0x48

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ u16 unk_4;
} Fld_GetItem_Args;

SpriteFrameInfo* func_ov030_020c7d20(Sprite* sprite, s32 arg1, s32 mode);
s32              Fld_GetItem_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_GetItem = {"Tsk_Fld_GetItem", Fld_GetItem_RunTask, sizeof(Fld_GetItem)};

extern s32 data_ov030_020ec9b8;

static const BinIdentifier data_ov030_020ec9f4[7] = {
    {30,      "Apl_Tak/Grp_Item.bin"},
    {30,      "Apl_Tak/ItemData.bin"},
    {30,      "Apl_Tak/FoodData.bin"},
    {30,  "Apl_Tak/TreasureData.bin"},
    {30,        "Data/BadgeData.bin"},
    {30, "Apl_Tak/Grp_DummyItem.bin"},
    {30,     "Apl_Tak/Grp_Badge.bin"},
};

static const SpriteAnimation data_ov030_020ec9c8 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 3,
    .unk_04     = 0x80,
    .unk_06     = 0x51,
    .unk_08     = func_ov030_020c7d20,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov030_020ec9f4[5],
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

void Fld_GetItem_LoadThreadData(RawItemData* itemData, s32 arg1) {
    Data_LoadToBuffer(1, *itemData, &data_ov030_020ec9f4[1], arg1);
}

void Fld_GetItem_LoadFoodData(RawFoodData* foodData, s32 arg1) {
    Data_LoadToBuffer(1, *foodData, &data_ov030_020ec9f4[2], arg1);
}

void Fld_GetItem_LoadTreasureData(RawTreasureData* treasureData, s32 arg1) {
    Data_LoadToBuffer(1, *treasureData, &data_ov030_020ec9f4[3], arg1);
}

void Fld_GetItem_LoadPinData(RawPinData* pinData, s32 arg1) {
    Data_LoadToBuffer(1, *pinData, &data_ov030_020ec9f4[4], arg1);
}

s16 func_ov030_020c7b24(u16 itemID, s32* arg1) {
    ItemCategory category = Inventory_GetCategory(itemID);
    s32          itemIdx  = Inventory_GetCategorizedIndex(itemID);

    *arg1 = 0;

    u16 var_r1;
    if (category == ITEM_CATEGORY_THREAD) {
        RawItemData itemData;
        Fld_GetItem_LoadThreadData(&itemData, itemIdx);
        var_r1 = itemData.unk_00;
    } else if (category == ITEM_CATEGORY_FOOD) {
        RawFoodData foodData;
        Fld_GetItem_LoadFoodData(&foodData, itemIdx);
        var_r1 = foodData.unk_00;
    } else if (category == ITEM_CATEGORY_SWAG) {
        RawTreasureData treasureData;
        Fld_GetItem_LoadTreasureData(&treasureData, itemIdx);
        var_r1 = treasureData.unk_00;
    } else {
        RawPinData pinData;
        Fld_GetItem_LoadPinData(&pinData, itemIdx);
        var_r1 = pinData.unk_00;
        *arg1  = 1;
    }

    return var_r1 + 1;
}

// Nonmatching: Regswap
void func_ov030_020c7bc0(Sprite* arg0, Fld_GetItem_Args* arg1, s32 arg2, s32 arg3) {
    s32 var_r2 = 0;
    if (arg3 != 0) {
        var_r2 = 6;
    }
    s32            temp_r5 = arg1->unk_0;
    BinIdentifier* temp_r6 = &data_ov030_020ec9f4[var_r2];

    Data* sp8;
    if (BinMgr_FindById((s32)temp_r6) == NULL) {
        sp8 = DatMgr_LoadPackEntry(temp_r5, NULL, 0, temp_r6, arg2, 0);
    } else {
        sp8 = DatMgr_LoadPackEntryDirect(temp_r5, temp_r6, arg2, 0);
    }

    void* spC = Data_GetPackEntryData(sp8, 1) + 4;

    void* var_r0 = Data_GetPackEntryData(sp8, 4);
    u8*   var_r4 = arg0->unk34 + 4;

    MI_CpuCopyU8(var_r0, arg0->unk3C, 0x20);

    s32 zeroInner = 0;
    s32 zeroCols  = 0;
    s32 block     = 0;
    u8* blockBase;
    s32 col;
    s32 row;
    s32 blockOffset = block;
    s32 rowOffset;
    s32 copyCount;
    s32 colOffset;
    u8* rowBase;
    u8* src;

    do {
        row       = 0;
        rowOffset = row;
        blockBase = spC + blockOffset;
        do {
            col       = zeroCols;
            colOffset = zeroCols;
            rowBase   = blockBase + rowOffset;
            do {
                copyCount = zeroInner;
                src       = rowBase + colOffset;
                do {
                    *var_r4 = *src;
                    var_r4 += 1;
                    src += 1;
                    copyCount += 1;
                } while (copyCount < 4);
                colOffset += 0x10;
                col += 1;
            } while (col < 8);
            row += 1;
            rowOffset += 4;
        } while (row < 4);
        block += 1;
        blockOffset += 0x80;
    } while (block < 4);
    DatMgr_ReleaseData(sp8);
}

SpriteFrameInfo* func_ov030_020c7d20(Sprite* sprite, s32 arg1, s32 mode) {
    // Not yet implemented
}

void func_ov030_020c7e00(Sprite* sprite, Fld_GetItem_Args* args, void* arg2) {
    s32             temp;
    SpriteAnimation anim = data_ov030_020ec9c8;

    anim.dataType = args->unk_0;
    anim.unk_10   = arg2;

    s16 temp_r4_3 = func_ov030_020c7b24(args->unk_4, &temp);
    if (temp != 0) {
        anim.unk_04 -= 4;
        anim.unk_06 -= 4;
    }
    _Sprite_Load(sprite, &anim);
    func_ov030_020c7bc0(sprite, args, (u16)temp_r4_3, temp);
}

s32 Fld_GetItem_Init(TaskPool* pool, Task* task, void* args) {
    Fld_GetItem* item = task->data;

    func_ov030_020c7e00(&item->sprite, args, item);
    item->unk_44 = 0x14;
    item->unk_40 = 0;
    return 1;
}

s32 Fld_GetItem_Update(TaskPool* pool, Task* task, void* args) {
    Fld_GetItem* item = task->data;

    if (item->unk_44 != 0) {
        func_02026590(&item->unk_40, 0x2000, item->unk_44);
        item->unk_44--;
    }
    Sprite_Update(&item->sprite);
    return 1;
}

s32 Fld_GetItem_Render(TaskPool* pool, Task* task, void* args) {
    Fld_GetItem* item = task->data;

    Sprite_RenderFrame(&item->sprite);
    return 1;
}

s32 Fld_GetItem_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_GetItem* item = task->data;

    Sprite_Destroy(&item->sprite);
    return 1;
}

s32 Fld_GetItem_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_GetItem_Init,
        .update     = Fld_GetItem_Update,
        .render     = Fld_GetItem_Render,
        .cleanup    = Fld_GetItem_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_GetItem_CreateTask(TaskPool* pool, s32 arg1, s16 arg2) {
    Fld_GetItem_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &Tsk_Fld_GetItem, NULL, 0, NULL, &args);
}
