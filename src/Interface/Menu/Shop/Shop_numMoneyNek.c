#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[10];
    /* 0x280 */ BOOL        unk_280[10];
    /* 0x2A8 */ ShopObject* unk_2A8;
    /* 0x2AC */ u8          unk_2AC;
    /* 0x2AD */ char        unk_2AD[0x2B0 - 0x2AD];
} Shop_numMoneyNek; // Size: 0x2B0

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ u32         unk_8;
} Shop_numMoneyNek_Args;

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);

SpriteFrameInfo* func_ov043_020b3684(Sprite* arg0, s32 arg1, s32 arg2);
s32              func_ov043_020b394c(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle data_ov043_020ca410 = {"Tsk_Shop_numMoneyNek", func_ov043_020b394c, 0x2B0};

static const Point data_ov043_020ca42c[5] = {
    {-10,  0},
    { 10, 16},
    { 22, 32},
    { 38, 44},
    {  5, 27},
};
static const SpriteAnimation data_ov043_020ca440 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = func_ov043_020b3684,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1b0,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 3,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

// Nonmatching
void func_ov043_020b34a0(Shop_numMoneyNek* arg0, u32 arg1, u32* arg2) {
    u16 i;

    arg2[0] = arg1 / 1000000;
    arg1 %= 1000000;
    arg2[1] = arg1 / 100000;
    arg1 %= 100000;
    arg2[2] = arg1 / 10000;
    arg1 %= 10000;
    arg2[3] = arg1 / 1000;
    arg1 %= 1000;
    arg2[4] = arg1 / 100;
    arg1 %= 100;
    arg2[5] = arg1 / 10;
    arg2[6] = arg1 % 10;

    for (i = 0; i < 10; i++) {
        arg0->unk_280[i] = TRUE;
    }

    arg0->unk_2AC = 7;
    for (i = 0; i < 6; i++) {
        if (arg2[i] != 0) {
            break;
        }
        arg0->unk_280[i + 1] = FALSE;
        arg0->unk_2AC--;
    }

    if (arg2[0] == 0) {
        arg0->unk_280[8] = FALSE;
        if (arg2[0] == 0 && arg2[1] == 0 && arg2[2] == 0 && arg2[3] == 0) {
            arg0->unk_280[9] = FALSE;
        }
    }
}

void func_ov043_020b3628(Shop_numMoneyNek* arg0) {
    u32 digits[7];
    u16 i;

    func_ov043_020b34a0(arg0, *(u32*)((u8*)arg0->unk_2A8 + 0x858), digits);
    for (i = 0; i < 7; i++) {
        func_ov043_020af42c(&arg0->unk_000[i + 1], (s16)(digits[i] + 0x1D));
    }
}

SpriteFrameInfo* func_ov043_020b3684(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* info = NULL;

    switch (arg2) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;
            if (arg0->animData != NULL && arg0->frameDataTable != NULL && arg0->unk16 >= 0) {
                temp->unk_04 = *((u16*)arg0->frameDataTable + (arg0->unk16 * 4 + 1));
                temp->unk_08 = (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
            }
            info = temp;
        } break;

        default:
            break;
    }

    return info;
}

// Nonmatching
void func_ov043_020b3720(Shop_numMoneyNek* arg0, Sprite* arg1, Shop_numMoneyNek_Args* arg2) {
    SpriteAnimation anim = data_ov043_020ca440;
    u32             digits[7];
    const s16*      positions = &data_ov043_020ca42c[0].y;
    u16             i;

    anim.dataType = arg2->dataType;
    func_ov043_020b34a0(arg0, arg2->unk_8, digits);

    anim.unk_2A = 0x27;
    anim.unk_04 = data_ov043_020ca42c[0].x + 0x93;
    anim.unk_06 = 9;
    _Sprite_Load(&arg1[0], &anim);

    for (i = 0; i < 7; i++) {
        anim.unk_2A = (s16)(digits[i] + 0x1D);
        anim.unk_04 = positions[i] + 0x93;
        anim.unk_06 = 9;
        _Sprite_Load(&arg1[i + 1], &anim);
    }

    anim.unk_2A = 0x45;
    anim.unk_04 = data_ov043_020ca42c[4].x + 0x93;
    anim.unk_06 = 9;
    _Sprite_Load(&arg1[8], &anim);

    anim.unk_2A = 0x45;
    anim.unk_04 = data_ov043_020ca42c[4].y + 0x93;
    anim.unk_06 = 9;
    _Sprite_Load(&arg1[9], &anim);
}

s32 func_ov043_020b388c(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_numMoneyNek*      temp_r0 = arg1->data;
    Shop_numMoneyNek_Args* args    = arg2;

    temp_r0->unk_2A8 = args->shop;
    func_ov043_020b3720(temp_r0, temp_r0->unk_000, args);
    return 1;
}

s32 func_ov043_020b38ac(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_numMoneyNek* var_r5 = arg1->data;
    s32               var_r4;

    func_ov043_020b3628(var_r5);
    var_r4 = 0;
    do {
        Sprite_Update(&var_r5->unk_000[var_r4]);
        var_r4 += 1;
    } while (var_r4 < 0xA);
    return 1;
}

s32 func_ov043_020b38e0(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_numMoneyNek* temp_r4 = arg1->data;
    s32               var_r5;

    var_r5 = 0;
    do {
        if (temp_r4->unk_280[var_r5] != 0) {
            Sprite_RenderFrame(&temp_r4->unk_000[var_r5]);
        }
        var_r5 += 1;
    } while (var_r5 < 0xA);
    return 1;
}

s32 func_ov043_020b3920(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_numMoneyNek* var_r5 = arg1->data;
    s32               var_r4;

    var_r4 = 0;
    do {
        Sprite_Release(&var_r5->unk_000[var_r4]);
        var_r4 += 1;
    } while (var_r4 < 0xA);
    return 1;
}

s32 func_ov043_020b394c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = func_ov043_020b388c,
        .update     = func_ov043_020b38ac,
        .render     = func_ov043_020b38e0,
        .cleanup    = func_ov043_020b3920,
    };
    return stages.iter[stage](pool, task, args);
}

s32 func_ov043_020b3994(TaskPool* arg0, s32 arg1, void* arg2) {
    Shop_numMoneyNek_Args args;
    ShopObject*           shop = arg2;

    args.dataType = arg1;
    args.shop     = shop;
    args.unk_8    = *(u32*)((u8*)shop + 0x858);
    return EasyTask_CreateTask(arg0, &data_ov043_020ca410, NULL, 0, NULL, &args);
}
