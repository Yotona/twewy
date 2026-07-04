#include "Engine/Core/OamMgr.h"
#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ Sprite      sprites[3];
    /* 0xC0 */ s32         shouldRender[3];
    /* 0xCC */ ShopObject* shop;
    /* 0xD0 */ s32         affineScaleX;
    /* 0xD4 */ s32         affineScaleY;
    /* 0xD8 */ u16         itemState;
    /* 0xDA */ u16         animState;
    /* 0xDC */ u16         animTimer;
    /* 0xDE */ u16         arg_0E;
} Shop_item2; // Size: 0xE0

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
    /* 0x08 */ u16         categorizedIndex;
    /* 0x0A */ u16         category;
    /* 0x0C */ u16         iconPackIndex;
    /* 0x0E */ u16         arg_0E;
} Shop_item2_Args; // Size: 0x10

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);
extern void func_ov043_020af4c4(RawPinData* pinData, u16 categorizedIndex);
extern void func_ov043_020af4f0(RawItemData* itemData, u16 categorizedIndex);
extern void func_ov043_020af51c(RawFoodData* foodData, u16 categorizedIndex);
extern void func_ov043_020af548(RawTreasureData* treasureData, u16 categorizedIndex);

static SpriteFrameInfo* Shop_item2_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_item2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern BinIdentifier data_ov043_020ca198[];
extern BinIdentifier data_ov043_020ca258;

static const TaskHandle Tsk_Shop_item2 = {"Tsk_Shop_item2", Shop_item2_RunTask, sizeof(Shop_item2)};

static const SpriteAnimation data_ov043_020ca690 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_item2_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca258,
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

static SpriteFrameInfo* Shop_item2_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

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

void func_ov043_020b6e80(Shop_item2* item, Sprite* sprite, Shop_item2_Args* args, s32 resourceIndex, u16 packIndex) {
    s32            dataType = args->dataType;
    BinIdentifier* binIden  = &data_ov043_020ca198[resourceIndex];

    Data* data;
    if (BinMgr_FindById((s32)binIden) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, binIden, packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, binIden, packIndex, 0);
    }

    u8*   src        = (u8*)Data_GetPackEntryData(data, 1) + 4;
    void* paletteSrc = Data_GetPackEntryData(data, 4);
    u8*   dest       = (u8*)sprite->unk34 + 4;
    MI_CpuCopyU8(paletteSrc, sprite->unk3C, 0x20);

    s32 outer = 0;
    while (outer < 4) {
        s32 copyCount;
        s32 x;
        s32 y = 0;
        while (y < 4) {
            x = 0;
            while (x < 8) {
                copyCount = 0;
                while (copyCount < 4) {
                    *dest = *(src + outer * 0x80 + y * 4 + x * 0x10 + copyCount);
                    dest++;
                    copyCount++;
                }
                x++;
            }
            y++;
        }
        outer++;
    }

    DatMgr_ReleaseData(data);
}

void Shop_item2_Load(Shop_item2* item, Sprite* sprites, Shop_item2_Args* args) {
    SpriteAnimation anim = data_ov043_020ca690;

    anim.dataType  = args->dataType;
    anim.binIden   = &data_ov043_020ca1b0;
    anim.unk_18    = 0;
    anim.packIndex = 1;
    anim.unk_2A    = 66;
    anim.unk_22    = 3;
    anim.unk_04    = 0x80;
    anim.unk_06    = 0x40;
    anim.unk_02.raw |= 2;

    _Sprite_Load(&sprites[0], &anim);

    if (args->category == ITEM_CATEGORY_PIN) {
        item->shouldRender[0] = FALSE;
        item->shouldRender[1] = TRUE;
        item->shouldRender[2] = TRUE;

        anim.binIden   = &data_ov043_020ca258;
        anim.unk_18    = 2;
        anim.packIndex = 2;
        anim.unk_2A    = 1;
        anim.unk_22    = 1;
        anim.unk_02.raw |= 2;
        anim.unk_04 = 0x7C;
        anim.unk_06 = 0x3C;
        _Sprite_Load(&sprites[1], &anim);

        func_ov043_020b6e80(item, &sprites[1], args, 11, args->iconPackIndex + 1);
    } else {
        item->shouldRender[0] = FALSE;
        item->shouldRender[1] = TRUE;
        item->shouldRender[2] = FALSE;

        anim.binIden   = &data_ov043_020ca258;
        anim.unk_18    = 2;
        anim.packIndex = 2;
        anim.unk_2A    = 1;
        anim.unk_22    = 1;
        anim.unk_02.raw |= 2;
        anim.unk_04 = 0x80;
        anim.unk_06 = 0x40;
        _Sprite_Load(&sprites[1], &anim);

        func_ov043_020b6e80(item, &sprites[1], args, 12, args->iconPackIndex + 1);
    }

    anim.binIden   = &data_ov043_020ca1b0;
    anim.unk_18    = 0;
    anim.packIndex = 1;
    anim.unk_2A    = 0x46;
    anim.unk_22    = 4;
    anim.unk_02.raw |= 2;
    anim.unk_04 = 0x80;
    anim.unk_06 = 0x40;
    _Sprite_Load(&sprites[2], &anim);

    item->affineScaleX = 0x2000;
    item->affineScaleY = 0x19A;
    item->itemState    = args->category;
    item->animState    = 0;
    item->animTimer    = 9;
    item->arg_0E       = args->arg_0E;
}

static s32 Shop_item2_Init(TaskPool* pool, Task* task, void* args) {
    Shop_item2*      item     = task->data;
    Shop_item2_Args* itemArgs = args;

    item->shop = itemArgs->shop;
    Shop_item2_Load(item, item->sprites, itemArgs);
    return 1;
}

static s32 Shop_item2_Update(TaskPool* pool, Task* task, void* args) {
    Shop_item2* item = task->data;

    switch (item->animState) {
        case 0:
            if (item->affineScaleY < 0x2000) {
                item->affineScaleY += 0x333;
                if (item->affineScaleY >= 0x2000) {
                    item->affineScaleY = 0x2000;
                    if (item->arg_0E != 1) {
                        item->animState = 1;
                    } else {
                        item->shouldRender[0] = TRUE;
                        func_ov043_020af42c(&item->sprites[0], 0x41);
                        item->animState = 2;
                    }
                }
            }
            break;

        case 1:
            break;

        case 2:
            if (item->animTimer != 0) {
                item->animTimer--;
            } else {
                func_ov043_020af42c(&item->sprites[0], 0x42);
                item->animState = 1;
            }
            break;
    }

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&item->sprites[i]);
    }
    return 1;
}

// Nonmatching
static s32 Shop_item2_Render(TaskPool* pool, Task* task, void* args) {
    Shop_item2* item   = task->data;
    u32         affine = (u16)OamMgr_AllocAffineGroup(g_OamMgr, 0, item->affineScaleX, item->affineScaleY, 0) << 0x1B;

    for (s32 i = 0; i < 3; i++) {
        Sprite* sprite = &item->sprites[i];

        sprite->unk_0A.raw = (sprite->unk_0A.raw & ~1) | 1;
        sprite->unk_0A.raw = (sprite->unk_0A.raw & ~0x3E0) | (affine >> 0x16);

        if (item->shouldRender[i] != 0) {
            Sprite_RenderFrame(sprite);
        }
    }
    return 1;
}

static s32 Shop_item2_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_item2* item = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&item->sprites[i]);
    }
    return 1;
}

static s32 Shop_item2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_item2_Init,
        .update     = Shop_item2_Update,
        .render     = Shop_item2_Render,
        .cleanup    = Shop_item2_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_item2_CreateTask(TaskPool* pool, s32 dataType, u16 itemID, s16 arg3, ShopObject* shop) {
    Shop_item2_Args args;

    args.dataType         = dataType;
    args.shop             = shop;
    args.categorizedIndex = Inventory_GetCategorizedIndex(itemID);
    args.category         = Inventory_GetCategory(itemID);
    args.arg_0E           = arg3;

    switch (args.category) {
        case ITEM_CATEGORY_PIN: {
            RawPinData pinData;
            func_ov043_020af4c4(&pinData, args.categorizedIndex);
            args.iconPackIndex = pinData.unk_00;
            break;
        }

        case ITEM_CATEGORY_THREAD: {
            RawItemData itemData;
            func_ov043_020af4f0(&itemData, args.categorizedIndex);
            args.iconPackIndex = itemData.unk_00;
            break;
        }

        case ITEM_CATEGORY_FOOD: {
            RawFoodData foodData;
            func_ov043_020af51c(&foodData, args.categorizedIndex);
            args.iconPackIndex = foodData.unk_00;
            break;
        }

        case ITEM_CATEGORY_SWAG: {
            RawTreasureData treasureData;
            func_ov043_020af548(&treasureData, args.categorizedIndex);
            args.iconPackIndex = treasureData.unk_00;
            break;
        }
    }

    return EasyTask_CreateTask(pool, &Tsk_Shop_item2, NULL, 0, NULL, &args);
}
