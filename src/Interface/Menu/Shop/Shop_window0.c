#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ Sprite      sprite;
    /* 0x040 */ BOOL        shouldRender;
    /* 0x044 */ ShopObject* shop;
    /* 0x048 */ SysFont     fonts[2];
    /* 0x140 */ char        unk_140[0x142 - 0x140];
    /* 0x142 */ SysCode     textBuf[199];
    /* 0x2D0 */ u16         unk_2D0;
    /* 0x2D2 */ u16         unk_2D2;
} Shop_window0; // Size: 0x2D4

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ char        unk_8[4];
} Shop_window0_Args;

SpriteFrameInfo* Shop_window0_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
s32              Shop_window0_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Shop_window0_Anim = {
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
    .unk_08     = Shop_window0_GetFrameInfo,
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

SpriteFrameInfo* Shop_window0_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;
            data_0206b408.unk_0C = 0;
            data_0206b408.unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info->unk_10 = 0x3E8000;
            return info;
        } break;
    }

    return NULL;
}

void Shop_window0_Load(Shop_window0* window, Sprite* sprite, Shop_window0_Args* windowArgs) {
    SpriteAnimation anim = Shop_window0_Anim;

    window->shouldRender = TRUE;
    window->unk_2D0      = 2;
    window->unk_2D2      = 2;

    anim.dataType = windowArgs->dataType;
    anim.unk_04   = 0x80;
    anim.unk_06   = 0x98;
    anim.unk_2A   = 1;
    _Sprite_Load(sprite, &anim);
}

void Shop_window0_InitFonts(Shop_window0* window) {
    SysFont_Init(&window->fonts[0]);
    SysFont_Init(&window->fonts[1]);
    SysFont_SetSpacing(&window->fonts[0], TRUE, 0);
    SysFont_SetSpacing(&window->fonts[1], TRUE, 0);
}

void Shop_window0_SetupLevelUpText(Shop_window0* window) {
    ShopObject* shop       = window->shop;
    const Point fontPos[2] = {
        {24, 27},
        { 0, 42}
    };

    for (u16 i = 0; i < 2; i++) {
        SysFont_SetPos(&window->fonts[i], fontPos[i].x, fontPos[i].y);
    }

    SysFont_SetMsg(&window->fonts[0], 12345); // "Friendship level up!"
    SysFont_SetHAlign(&window->fonts[0], 1, SYSFONT_NO_LIMIT);

    if (shop->unk_84E == 0xFFFF) {
        SysCode* fmt = SysFont_GetMsgBuf(&window->fonts[1], 12346); // "<c8>You've grown on the seller!<cE>\nThe"...
        SysFont_Format(window->textBuf, fmt, shop->unk_7B5);
        SysFont_SetHAlign(&window->fonts[1], 0, 240);
        SysFont_SetVAlign(&window->fonts[1], 0, 46);
        Mem_Free(&gDebugHeap, fmt);
        return;
    }

    s32          itemIndex = Inventory_GetCategorizedIndex(shop->unk_84E);
    ItemCategory category  = Inventory_GetCategory(shop->unk_84E);
    s32          nameBase;

    if (category == ITEM_CATEGORY_PIN) {
        nameBase = 10177; // "Ice Blow"...
    } else if (category == ITEM_CATEGORY_THREAD) {
        nameBase = 11284; // "M Cap"...
    } else if (category == ITEM_CATEGORY_FOOD) {
        nameBase = 10861; // "Hamburger"...
    } else {
        nameBase = 13502; // "Colorful Thread"...
    }

    SysCode* fmt      = SysFont_GetMsgBuf(&window->fonts[1], 12347); // "<c8>You've grown on the seller!<cE>\nNow"...
    SysCode* itemName = SysFont_GetMsgBuf(&window->fonts[1], nameBase + itemIndex);
    SysFont_Format(window->textBuf, fmt, itemName);
    SysFont_SetHAlign(&window->fonts[1], 0, 228);
    SysFont_SetVAlign(&window->fonts[1], 0, 46);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, itemName);
}

void Shop_window0_SetupStyleAbilityText(Shop_window0* window) {
    ShopObject* shop       = window->shop;
    const Point fontPos[2] = {
        {24, 27},
        { 0, 42}
    };

    for (u16 i = 0; i < 2; i++) {
        SysFont_SetPos(&window->fonts[i], fontPos[i].x, fontPos[i].y);
    }

    SysFont_SetMsg(&window->fonts[0], 12348); // "New ability unlocked!"
    SysFont_SetHAlign(&window->fonts[0], 1, SYSFONT_NO_LIMIT);

    SysCode*     fmt       = SysFont_GetMsgBuf(&window->fonts[1], 12349); // "<c8>The seller digs your style!<cE>\nYou"...
    ItemCategory category  = Inventory_GetCategory(shop->unk_84C);
    s32          itemIndex = Inventory_GetCategorizedIndex(shop->unk_84C);
    SysCode*     itemName;

    if (category == ITEM_CATEGORY_PIN) {
        itemName = SysFont_GetMsgBuf(&window->fonts[1], itemIndex + 10177); // "Ice Blow"...
    } else if (category == ITEM_CATEGORY_THREAD) {
        itemName = SysFont_GetMsgBuf(&window->fonts[1], itemIndex + 11284); // "M Cap"...
    } else {
        OS_WaitForever();
    }

    SysFont_Format(window->textBuf, fmt, itemName);
    SysFont_SetHAlign(&window->fonts[1], 0, 228);
    SysFont_SetVAlign(&window->fonts[1], 0, 46);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, itemName);
}

void Shop_window0_SetupBrowseAbilityText(Shop_window0* window) {
    ShopObject* shop       = window->shop;
    const Point fontPos[2] = {
        {24, 27},
        { 0, 42}
    };

    for (u16 i = 0; i < 2; i++) {
        SysFont_SetPos(&window->fonts[i], fontPos[i].x, fontPos[i].y);
    }

    SysFont_SetMsg(&window->fonts[0], 12348); // "New ability unlocked!"
    SysFont_SetHAlign(&window->fonts[0], 1, SYSFONT_NO_LIMIT);

    SysCode*     fmt       = SysFont_GetMsgBuf(&window->fonts[1], 12350); // "<c8>The seller notices you browsing<cE> "...
    ItemCategory category  = Inventory_GetCategory(shop->unk_84C);
    s32          itemIndex = Inventory_GetCategorizedIndex(shop->unk_84C);
    SysCode*     itemName;

    if (category == ITEM_CATEGORY_PIN) {
        itemName = SysFont_GetMsgBuf(&window->fonts[1], itemIndex + 10177); // "Ice Blow"...
    } else if (category == ITEM_CATEGORY_THREAD) {
        itemName = SysFont_GetMsgBuf(&window->fonts[1], itemIndex + 11284); // "M Cap"...
    } else {
        OS_WaitForever();
    }

    SysFont_Format(window->textBuf, fmt, itemName);
    SysFont_SetHAlign(&window->fonts[1], 0, 228);
    SysFont_SetVAlign(&window->fonts[1], 0, 46);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, itemName);
}

void Shop_window0_SetupBagFullText(Shop_window0* window) {
    const Point fontPos[2] = {
        {24, 27},
        { 0, 42}
    };

    for (u16 i = 0; i < 2; i++) {
        SysFont_SetPos(&window->fonts[i], fontPos[i].x, fontPos[i].y);
    }

    window->unk_2D2 = 1;
    SysFont_SetMsg(&window->fonts[0], 13349); // "You <cC>can't carry<cE> any more!"
    SysFont_SetHAlign(&window->fonts[0], 1, SYSFONT_NO_LIMIT);
    SysFont_SetMsg(&window->fonts[1], 13350); // "One more item to lug around and you\nmig"...
    SysFont_SetHAlign(&window->fonts[1], 0, 228);
    SysFont_SetVAlign(&window->fonts[1], 0, 46);
}

void Shop_window0_DrawTextDelayed(Shop_window0* window) {
    if (window->unk_2D0 == 0) {
        return;
    }

    window->unk_2D0--;
    if (window->unk_2D0 == 0) {
        if (window->unk_2D2 == 1) {
            SysFont_DrawCurrentToSprite(&window->fonts[0], &window->sprite, TRUE);
            SysFont_DrawCurrentToSprite(&window->fonts[1], &window->sprite, TRUE);
        } else {
            SysFont_DrawCurrentToSprite(&window->fonts[0], &window->sprite, TRUE);
            SysFont_DrawToSprite(&window->fonts[1], window->textBuf, &window->sprite, TRUE);
        }
    }
}

s32 Shop_window0_Init(TaskPool* pool, Task* task, void* args) {
    Shop_window0*      window     = task->data;
    Shop_window0_Args* windowArgs = args;

    window->shop = windowArgs->shop;
    Shop_window0_Load(window, &window->sprite, windowArgs);
    Shop_window0_InitFonts(window);

    switch (window->shop->unk_85E) {
        case 1:
            Shop_window0_SetupLevelUpText(window);
            break;
        case 2:
            Shop_window0_SetupStyleAbilityText(window);
            break;
        case 3:
            Shop_window0_SetupBrowseAbilityText(window);
            break;
        case 4:
            Shop_window0_SetupBagFullText(window);
            break;
    }
    return 1;
}

s32 Shop_window0_Update(TaskPool* pool, Task* task, void* args) {
    Shop_window0* window = task->data;

    Sprite_Update(&window->sprite);
    return 1;
}

s32 Shop_window0_Render(TaskPool* pool, Task* task, void* args) {
    Shop_window0* window = task->data;

    if (window->shouldRender) {
        Sprite_RenderFrame(&window->sprite);
    }
    Shop_window0_DrawTextDelayed(window);
    return 1;
}

s32 Shop_window0_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_window0* window = task->data;

    Sprite_Release(&window->sprite);
    SysFont_Destroy(&window->fonts[0]);
    SysFont_Destroy(&window->fonts[1]);
    return 1;
}

s32 Shop_window0_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_window0_Init,
        .update     = Shop_window0_Update,
        .render     = Shop_window0_Render,
        .cleanup    = Shop_window0_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Shop_window0 = {"Tsk_Shop_window0", Shop_window0_RunTask, sizeof(Shop_window0)};

s32 Shop_window0_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_window0_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_window0, NULL, 0, NULL, &args);
}
