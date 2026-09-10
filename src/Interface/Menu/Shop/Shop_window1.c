#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[3];
    /* 0x0C0 */ BOOL        unk_0C0;
    /* 0x0C4 */ ShopObject* unk_0C4;
    /* 0x0C8 */ SysFont     fonts[3];
    /* 0x23C */ char        unk_23C[0x23E - 0x23C];
    /* 0x23E */ SysCode     textBuf[(0x2DC - 0x23E) / 2];
    /* 0x2DC */ u16         unk_2DC;
    /* 0x2DE */ u16         unk_2DE;
} Shop_window1; // Size: 0x2E0

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ char        unk_8[4];
} Shop_window1_Args;

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);

SpriteFrameInfo* Shop_window1_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
void             Shop_window1_Load(Shop_window1* arg0, Sprite* arg1, Shop_window1_Args* arg2);
void             func_ov043_020ba2f0(Shop_window1* arg0);
void             func_ov043_020ba33c(Shop_window1* arg0);
void             func_ov043_020ba6d4(Shop_window1* arg0);
s32              Shop_window1_Init(TaskPool* arg0, Task* arg1, void* arg2);
s32              Shop_window1_Update(TaskPool* arg0, Task* arg1, void* arg2);
s32              Shop_window1_Render(TaskPool* arg0, Task* arg1, void* arg2);
s32              Shop_window1_Destroy(TaskPool* arg0, Task* arg1, void* arg2);
s32              Shop_window1_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_window1 = {"Tsk_Shop_window1", Shop_window1_RunTask, sizeof(Shop_window1)};

static const Point data_ov043_020caa48[3] = {
    {0, 0x06},
    {0, 0x15},
    {0, 0x24},
};

static const SpriteAnimation data_ov043_020caa64 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 1,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_window1_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1c0,
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

SpriteFrameInfo* Shop_window1_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* temp = &data_0206b408;
    switch (mode) {
        case 1: {
            temp->unk_00 = 1;
            return temp;
        } break;

        case 2: {
            temp         = &data_0206b408;
            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            temp->unk_10 = 0x3E8000;
            return temp;
        } break;

        default:
            return NULL;
    }
}

void Shop_window1_Load(Shop_window1* window, Sprite* sprites, Shop_window1_Args* arg2) {
    SpriteAnimation anim = data_ov043_020caa64;

    window->unk_0C0 = TRUE;
    window->unk_2DC = 2;

    anim.dataType = (u16)arg2->dataType;
    anim.unk_2A   = 6;
    anim.unk_04   = 0x80;
    anim.unk_06   = 0x60;
    _Sprite_Load(sprites, &anim);

    anim.unk_2A = 2;
    anim.unk_04 = 0x4B;
    anim.unk_06 = 0x75;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_04 = 0xB4;
    anim.unk_06 = 0x75;
    anim.unk_2A = 4;
    _Sprite_Load(&sprites[2], &anim);
}

void func_ov043_020ba2f0(Shop_window1* window) {
    for (u16 i = 0; i < 3; i++) {
        SysFont_Init(&window->fonts[i]);
        SysFont_SetSpacing(&window->fonts[i], 1, 0);
    }
}

// Nonmatching
void func_ov043_020ba33c(Shop_window1* window) {
    ShopObject* shop = window->unk_0C4;

    for (u16 i = 0; i < 3; i++) {
        SysFont_SetPos(&window->fonts[i], data_ov043_020caa48[i].x, data_ov043_020caa48[i].y);
        SysFont_SetHAlign(&window->fonts[i], 0, 0xE0);
    }

    {
        u8* itemEntry = (u8*)shop + (shop->unk_848 * 0x2C);
        u16 itemID    = *(u16*)itemEntry;
        u16 itemIndex = Inventory_GetCategorizedIndex(itemID);
        u32 itemCost  = *(u32*)(itemEntry + 0x08);
        u16 itemState = *(u16*)(itemEntry + 0x10);
        s32 msgBase;

        switch (Inventory_GetCategory(itemID)) {
            case ITEM_CATEGORY_PIN:
                msgBase = 0x27C1;
                break;

            case ITEM_CATEGORY_THREAD:
                msgBase = 0x2C14;
                break;

            case ITEM_CATEGORY_FOOD:
                msgBase = 0x2A6D;
                break;

            default:
                msgBase = 0x34BE;
                break;
        }

        SysFont_SetMsg(&window->fonts[0], (u16)(msgBase + itemIndex));

        if (itemState == 0) {
            void* text;

            if (itemCost < 0x3E8U) {
                text = SysFont_GetMsgBuf(&window->fonts[1], 0x3413);
                SysFont_Format(window->textBuf, text, itemCost);
            } else if (itemCost < 0xF4240U) {
                u32 thousands = itemCost / 1000;
                u32 rem1000   = itemCost % 1000;
                u32 hundreds  = rem1000 / 100;
                u32 rem100    = rem1000 % 100;
                u32 tens      = rem100 / 10;
                u32 ones      = rem100 % 10;

                text = SysFont_GetMsgBuf(&window->fonts[1], 0x3414);
                SysFont_Format(window->textBuf, text, thousands, hundreds, tens, ones);
            } else {
                u32 millions         = itemCost / 1000000;
                u32 rem1000000       = itemCost % 1000000;
                u32 hundredThousands = rem1000000 / 100000;
                u32 rem100000        = rem1000000 % 100000;
                u32 tenThousands     = rem100000 / 10000;
                u32 rem10000         = rem100000 % 10000;
                u32 thousands        = rem10000 / 1000;
                u32 rem1000          = rem10000 % 1000;
                u32 hundreds         = rem1000 / 100;
                u32 rem100           = rem1000 % 100;
                u32 tens             = rem100 / 10;
                u32 ones             = rem100 % 10;

                text = SysFont_GetMsgBuf(&window->fonts[1], 0x3415);
                SysFont_Format(window->textBuf, text, millions, hundredThousands, tenThousands, thousands, hundreds, tens,
                               ones);
            }

            SysFont_SetMsgPtr(&window->fonts[1], window->textBuf);
            Mem_Free(&gDebugHeap, text);
            SysFont_SetMsg(&window->fonts[2], 0x3416);
            return;
        }
    }

    SysFont_SetMsg(&window->fonts[1], 0x3423);
    SysFont_SetMsg(&window->fonts[2], 0x3424);
}

void func_ov043_020ba6d4(Shop_window1* window) {
    if (window->unk_2DC == 0) {
        return;
    }

    window->unk_2DC--;
    if (window->unk_2DC != 0) {
        return;
    }

    SysFont_DrawCurrentToSprite(&window->fonts[0], window->unk_000, 1);
    SysFont_DrawCurrentToSprite(&window->fonts[1], window->unk_000, 1);
    SysFont_DrawCurrentToSprite(&window->fonts[2], window->unk_000, 1);
}

s32 Shop_window1_Init(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_window1*      window = arg1->data;
    Shop_window1_Args* args   = arg2;

    window->unk_0C4 = args->shop;
    Shop_window1_Load(window, window->unk_000, args);
    func_ov043_020ba2f0(window);
    func_ov043_020ba33c(window);
    return 1;
}

s32 Shop_window1_Update(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_window1* window = arg1->data;
    ShopObject*   shop   = window->unk_0C4;

    for (u16 i = 0; i < 2; i++) {
        if (shop->unk_864[i] == 1) {
            func_ov043_020af42c(&window->unk_000[i + 1], (s16)(i * 2 + 3));
            if (shop->unk_839 != 0) {
                shop->unk_839--;
            } else {
                shop->unk_864[i] = 0;
            }
        } else {
            func_ov043_020af42c(&window->unk_000[i + 1], (s16)(i * 2 + 2));
        }
    }

    for (u16 i = 0; i < 3; i++) {
        Sprite_Update(&window->unk_000[i]);
    }
    return 1;
}

s32 Shop_window1_Render(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_window1* window = arg1->data;

    for (u16 i = 0; i < 3; i++) {
        if (window->unk_0C0 != 0) {
            Sprite_RenderFrame(&window->unk_000[i]);
        }
    }
    func_ov043_020ba6d4(window);
    return 1;
}

s32 Shop_window1_Destroy(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_window1* window = arg1->data;

    for (u16 i = 0; i < 3; i++) {
        Sprite_Release(&window->unk_000[i]);
    }
    SysFont_Destroy(&window->fonts[0]);
    SysFont_Destroy(&window->fonts[1]);
    SysFont_Destroy(&window->fonts[2]);
    return 1;
}

s32 Shop_window1_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_window1_Init,
        .update     = Shop_window1_Update,
        .render     = Shop_window1_Render,
        .cleanup    = Shop_window1_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_window1_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_window1_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_window1, NULL, 0, NULL, &args);
}