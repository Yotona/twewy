#include "Interface/Menu/Shop.h"

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);

typedef struct {
    /* 0x000 */ Sprite      sprites[8];
    /* 0x200 */ BOOL        shouldRender[8];
    /* 0x220 */ ShopObject* shop;
} Shop_nameD; // Size: 0x224

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_nameD_Args;

SpriteFrameInfo* func_ov043_020b2e70(Sprite* sprite, s32 arg1, s32 mode);
s32              func_ov043_020b31f0(TaskPool* pool, Task* task, void* args, s32 stage);
s32              func_ov043_020b3238(TaskPool* pool, s32 dataType, ShopObject* shop);

static const TaskHandle data_ov043_020ca360 = {"Tsk_Shop_nameD", func_ov043_020b31f0, sizeof(Shop_nameD)};

static const Point data_ov043_020ca37c[8] = {
    { 8, 159},
    { 8, 171},
    { 8, 183},
    {51, 183},
    {80,  19},
    {75, 171},
    {75, 183},
    {75, 195},
};

static const SpriteAnimation data_ov043_020ca39c = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = func_ov043_020b2e70,
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

// Nonmatching
void func_ov043_020b2b48(Shop_nameD* name) {
    ShopObject* shop = name->shop;

    u16 state = shop->unk_738[shop->unk_848]->unk_04;

    if (state == 0) {
        if (shop->unk_854 == 0) {
            func_ov043_020af42c(&name->sprites[4], 15);
            func_ov043_020af42c(&name->sprites[6], 16);
            func_ov043_020af42c(&name->sprites[7], 17);
            name->shouldRender[2] = FALSE;
            name->shouldRender[4] = TRUE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = TRUE;
            name->shouldRender[7] = TRUE;
        } else if (shop->unk_854 == 1) {
            func_ov043_020af42c(&name->sprites[2], 5);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else if (shop->unk_854 == 2) {
            func_ov043_020af42c(&name->sprites[2], 6);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else if (shop->unk_854 == 3) {
            func_ov043_020af42c(&name->sprites[2], 9);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else {
            func_ov043_020af42c(&name->sprites[2], 10);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        }
    } else if (state == 1) {
        if (shop->unk_854 == 0) {
            func_ov043_020af42c(&name->sprites[4], 64);
            func_ov043_020af42c(&name->sprites[5], 12);
            func_ov043_020af42c(&name->sprites[6], 13);
            func_ov043_020af42c(&name->sprites[7], 14);
            name->shouldRender[2] = FALSE;
            name->shouldRender[4] = TRUE;
            name->shouldRender[5] = TRUE;
            name->shouldRender[6] = TRUE;
            name->shouldRender[7] = TRUE;
        } else if (shop->unk_854 == 1) {
            func_ov043_020af42c(&name->sprites[2], 4);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else if (shop->unk_854 == 2) {
            func_ov043_020af42c(&name->sprites[2], 8);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else {
            func_ov043_020af42c(&name->sprites[2], 10);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        }
    } else if (state == 2) {
        if (shop->unk_854 == 0) {
            func_ov043_020af42c(&name->sprites[2], 4);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else if (shop->unk_854 == 1) {
            func_ov043_020af42c(&name->sprites[2], 7);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        } else {
            func_ov043_020af42c(&name->sprites[2], 0xA);
            name->shouldRender[2] = TRUE;
            name->shouldRender[4] = FALSE;
            name->shouldRender[5] = FALSE;
            name->shouldRender[6] = FALSE;
            name->shouldRender[7] = FALSE;
        }
    } else if (shop->unk_854 == 0) {
        func_ov043_020af42c(&name->sprites[2], 4);
        name->shouldRender[2] = TRUE;
        name->shouldRender[4] = FALSE;
        name->shouldRender[5] = FALSE;
        name->shouldRender[6] = FALSE;
        name->shouldRender[7] = FALSE;
    } else {
        func_ov043_020af42c(&name->sprites[2], 0xA);
        name->shouldRender[2] = TRUE;
        name->shouldRender[4] = FALSE;
        name->shouldRender[5] = FALSE;
        name->shouldRender[6] = FALSE;
        name->shouldRender[7] = FALSE;
    }
}

SpriteFrameInfo* func_ov043_020b2e70(Sprite* sprite, s32 arg1, s32 mode) {
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

// Nonmatching
void func_ov043_020b2f0c(Shop_nameD* name, Sprite* sprites, Shop_nameD_Args* args) {
    ShopObject* shop = name->shop;

    SpriteAnimation anim          = data_ov043_020ca39c;
    s16             frameIndex[8] = {1, 1, 4, 1, 1, 1, 1, 1};

    u16 state = shop->unk_738[shop->unk_848]->unk_04;

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 8; i++) {
        name->shouldRender[i] = TRUE;
    }

    if (state <= 1) {
        name->shouldRender[2] = FALSE;
    }

    if (state == 0) {
        name->shouldRender[5] = FALSE;
    } else if (state >= 2) {
        for (s16 i = 4; i < 8; i++) {
            frameIndex[i]         = 1;
            name->shouldRender[i] = FALSE;
        }
    }

    for (s16 i = 0; i < 8; i++) {
        anim.unk_2A = frameIndex[i];
        anim.unk_04 = data_ov043_020ca37c[i].x;
        anim.unk_06 = data_ov043_020ca37c[i].y;
        _Sprite_Load(&name->sprites[i], &anim);
    }
}

s32 func_ov043_020b3118(TaskPool* pool, Task* task, void* args) {
    Shop_nameD*      name     = task->data;
    Shop_nameD_Args* taskArgs = args;

    name->shop = taskArgs->shop;
    func_ov043_020b2f0c(name, name->sprites, taskArgs);
    return 1;
}

s32 func_ov043_020b3138(TaskPool* pool, Task* task, void* args) {
    Shop_nameD* name = task->data;

    func_ov043_020b2b48(name);
    for (s16 i = 0; i < 8; i++) {
        Sprite_Update(&name->sprites[i]);
    }
    return 1;
}

s32 func_ov043_020b3174(TaskPool* pool, Task* task, void* args) {
    Shop_nameD* name = task->data;

    for (s16 i = 0; i < 8; i++) {
        if (name->shouldRender[i]) {
            Sprite_RenderFrame(&name->sprites[i]);
        }
    }
    return 1;
}

s32 func_ov043_020b31bc(TaskPool* pool, Task* task, void* args) {
    Shop_nameD* name = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Release(&name->sprites[i]);
    }
    return 1;
}

s32 func_ov043_020b31f0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = func_ov043_020b3118,
        .update     = func_ov043_020b3138,
        .render     = func_ov043_020b3174,
        .cleanup    = func_ov043_020b31bc,
    };

    return stages.iter[stage](pool, task, args);
}

s32 func_ov043_020b3238(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_nameD_Args args;

    args.dataType = dataType;
    args.shop     = shop;

    return EasyTask_CreateTask(pool, &data_ov043_020ca360, NULL, 0, NULL, &args);
}
