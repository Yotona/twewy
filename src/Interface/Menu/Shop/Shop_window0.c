#include "Interface/Menu/Shop.h"
#include "Player/Inventory.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ Sprite        sprite;
    /* 0x040 */ BOOL          shouldRender;
    /* 0x044 */ ShopObject*   shop;
    /* 0x048 */ UnkOv31Struct unk_048[2];
    /* 0x140 */ char          unk_140[0x142 - 0x140];
    /* 0x142 */ u16           unk_142;
    /* 0x144 */ char          unk_144[0x2D0 - 0x144];
    /* 0x2D0 */ u16           unk_2D0;
    /* 0x2D2 */ u16           unk_2D2;
} Shop_window0; // Size: 0x2D4

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_window0_Args;

SpriteFrameInfo* Shop_window0_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Shop_window0_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const Point data_ov043_020ca9d4[8] = {
    {0x18, 0x1B},
    {   0, 0x2A},
    {0x18, 0x1B},
    {   0, 0x2A},
    {0x18, 0x1B},
    {   0, 0x2A},
    {0x18, 0x1B},
    {   0, 0x2A}
};

static const SpriteAnimation data_ov043_020caa10 = {
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

static const TaskHandle Tsk_Shop_window0 = {"Tsk_Shop_window0", Shop_window0_RunTask, sizeof(Shop_window0)};

SpriteFrameInfo* Shop_window0_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    // Not yet implemented
}

void Shop_window0_Load(Shop_window0* window, Sprite* sprite, Shop_window0_Args* windowArgs) {
    SpriteAnimation anim = data_ov043_020caa10;

    window->shouldRender = TRUE;
    window->unk_2D0      = 2;
    window->unk_2D2      = 2;

    anim.dataType = windowArgs->dataType;
    anim.unk_04   = 0x80;
    anim.unk_06   = 0x98;
    anim.unk_2A   = 1;
    _Sprite_Load(sprite, &anim);
}

void func_ov043_020b99bc(Shop_window0* window) {
    func_ov031_0210aa94(&window->unk_048[0]);
    func_ov031_0210aa94(&window->unk_048[1]);
    func_ov031_0210ab54(&window->unk_048[0], 1, 0);
    func_ov031_0210ab54(&window->unk_048[1], 1, 0);
}

// Nonmatching
void func_ov043_020b99f8(Shop_window0* window) {
    ShopObject* shop    = window->shop;
    const Point temp[2] = {data_ov043_020ca9d4[0], data_ov043_020ca9d4[1]};

    for (u16 i = 0; i < 2; i++) {
        func_ov031_0210ab28(&window->unk_048[i], temp[i].x, temp[i].y);
    }

    func_ov031_0210b630(&window->unk_048[0], 0x3039);
    func_ov031_0210ab3c(&window->unk_048[0], 1, 0xFFFF);

    if (shop->unk_84E == 0xFFFF) {
        void* temp_r0 = func_ov031_0210b698(&window->unk_048[1], 0x303A);
        func_ov031_0210a5fc(&window->unk_142, temp_r0, shop->unk_7B5);
        func_ov031_0210ab3c(&window->unk_048[1], 0, 0xF0);
        func_ov031_0210ab48(&window->unk_048[1], 0, 0x2E);
        Mem_Free(&gDebugHeap, temp_r0);
        return;
    }

    s32          index    = Inventory_GetCategorizedIndex(shop->unk_84E);
    ItemCategory category = Inventory_GetCategory(shop->unk_84E);
    s32          var_r5;

    if (category == ITEM_CATEGORY_PIN) {
        var_r5 = 0x27C1;
    } else if (category == ITEM_CATEGORY_THREAD) {
        var_r5 = 0x2C14;
    } else if (category == ITEM_CATEGORY_FOOD) {
        var_r5 = 0x2A6D;
    } else {
        var_r5 = 0x34BE;
    }

    void* temp_r5   = func_ov031_0210b698(&window->unk_048[1], 0x303B);
    void* temp_r0_3 = func_ov031_0210b698(&window->unk_048[1], var_r5 + index);
    func_ov031_0210a5fc(&window->unk_142, temp_r5, temp_r0_3);
    func_ov031_0210ab3c(&window->unk_048[1], 0, 228);
    func_ov031_0210ab48(&window->unk_048[1], 0, 46);
    Mem_Free(&gDebugHeap, temp_r5);
    Mem_Free(&gDebugHeap, temp_r0_3);
}

// Nonmatching
void func_ov043_020b9bcc(Shop_window0* window) {
    ShopObject* shop    = window->shop;
    const Point temp[2] = {data_ov043_020ca9d4[0], data_ov043_020ca9d4[1]};

    for (u16 i = 0; i < 2; i++) {
        func_ov031_0210ab28(&window->unk_048[i], temp[i].x, temp[i].y);
    }

    func_ov031_0210b630(&window->unk_048[0], 0x303C);
    func_ov031_0210ab3c(&window->unk_048[0], 1, 0xFFFF);

    void* temp_r4 = func_ov031_0210b698(&window->unk_048[1], 0x303D);
    s32   temp_r5 = Inventory_GetCategory(shop->unk_84C);
    s32   temp_r0 = Inventory_GetCategorizedIndex(shop->unk_84C);

    void* var_r8;

    if (temp_r5 == 0) {
        var_r8 = func_ov031_0210b698(&window->unk_048[1], temp_r0 + 0x27C1);
    } else if (temp_r5 == 1) {
        var_r8 = func_ov031_0210b698(&window->unk_048[1], temp_r0 + 0x2C14);
    } else {
        OS_WaitForever();
    }
    func_ov031_0210a5fc(&window->unk_142, temp_r4, var_r8);
    func_ov031_0210ab3c(&window->unk_048[1], 0, 228);
    func_ov031_0210ab48(&window->unk_048[1], 0, 46);
    Mem_Free(&gDebugHeap, temp_r4);
    Mem_Free(&gDebugHeap, var_r8);
}

// Nonmatching
void func_ov043_020b9d3c(Shop_window0* window) {
    ShopObject* shop    = window->shop;
    const Point temp[2] = {data_ov043_020ca9d4[0], data_ov043_020ca9d4[1]};

    for (u16 i = 0; i < 2; i++) {
        func_ov031_0210ab28(&window->unk_048[i], temp[i].x, temp[i].y);
    }

    func_ov031_0210b630(&window->unk_048[0], 0x303C);
    func_ov031_0210ab3c(&window->unk_048[0], 1, 0xFFFF);

    void* temp_r4 = func_ov031_0210b698(&window->unk_048[1], 0x303E);
    s32   temp_r5 = Inventory_GetCategory(shop->unk_84C);
    s32   temp_r0 = Inventory_GetCategorizedIndex(shop->unk_84C);

    void* var_r8;

    if (temp_r5 == 0) {
        var_r8 = func_ov031_0210b698(&window->unk_048[1], temp_r0 + 0x27C1);
    } else if (temp_r5 == 1) {
        var_r8 = func_ov031_0210b698(&window->unk_048[1], temp_r0 + 0x2C14);
    } else {
        OS_WaitForever();
    }
    func_ov031_0210a5fc(&window->unk_142, temp_r4, var_r8);
    func_ov031_0210ab3c(&window->unk_048[1], 0, 228);
    func_ov031_0210ab48(&window->unk_048[1], 0, 46);
    Mem_Free(&gDebugHeap, temp_r4);
    Mem_Free(&gDebugHeap, var_r8);
}

// Nonmatching
void func_ov043_020b9eac(Shop_window0* window) {
    const Point temp[2] = {data_ov043_020ca9d4[6], data_ov043_020ca9d4[7]};

    for (u16 i = 0; i < 2; i++) {
        func_ov031_0210ab28(&window->unk_048[i], temp[i].x, temp[i].y);
    }

    window->unk_2D2 = 1;
    func_ov031_0210b630(&window->unk_048[0], 0x3425);
    func_ov031_0210ab3c(&window->unk_048[0], 1, 0xFFFF);
    func_ov031_0210b630(&window->unk_048[1], 0x3426);
    func_ov031_0210ab3c(&window->unk_048[1], 0, 228);
    func_ov031_0210ab48(&window->unk_048[1], 0, 46);
}

void func_ov043_020b9f80(Shop_window0* window) {
    if (window->unk_2D0 == 0) {
        return;
    }

    window->unk_2D0--;
    if (window->unk_2D0 == 0) {
        if (window->unk_2D2 == 1) {
            func_ov031_0210bed8(&window->unk_048[0], window, 1);
            func_ov031_0210bed8(&window->unk_048[1], window, 1);
        } else {
            func_ov031_0210bed8(&window->unk_048[0], window, 1);
            func_ov031_0210beb0(&window->unk_048[1], &window->unk_142, window, 1);
        }
    }
}

s32 Shop_window0_Init(TaskPool* pool, Task* task, void* args) {
    Shop_window0*      window     = task->data;
    Shop_window0_Args* windowArgs = args;

    window->shop = windowArgs->shop;
    Shop_window0_Load(window, &window->sprite, windowArgs);
    func_ov043_020b99bc(window);

    switch (window->shop->unk_85E) {
        case 1:
            func_ov043_020b99f8(window);
            break;
        case 2:
            func_ov043_020b9bcc(window);
            break;
        case 3:
            func_ov043_020b9d3c(window);
            break;
        case 4:
            func_ov043_020b9eac(window);
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
    func_ov043_020b9f80(window);
    return 1;
}

s32 Shop_window0_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_window0* window = task->data;

    Sprite_Release(&window->sprite);
    func_ov031_0210aabc(&window->unk_048[0]);
    func_ov031_0210aabc(&window->unk_048[1]);
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

s32 Shop_window0_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_window0_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_window0, NULL, 0, NULL, &args);
}
