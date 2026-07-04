#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[10];
    /* 0x280 */ BOOL        unk_280[10];
    /* 0x2A8 */ ShopObject* unk_2A8;
    /* 0x2AC */ u8          unk_2AC;
    /* 0x2AD */ char        unk_2AD[0x2B0 - 0x2AD];
} Shop_numMoneyBdg; // Size: 0x2B0

typedef struct {
    /* 0x0 */ s32         dataType;
    /* 0x4 */ ShopObject* shop;
} Shop_numMoneyBdg_Args;

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);

static SpriteFrameInfo* Shop_numMoneyBdg_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              Shop_numMoneyBdg_RunTask(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static const TaskHandle Tsk_Shop_numMoneyBdg = {"Tsk_Shop_numMoneyBdg", Shop_numMoneyBdg_RunTask, sizeof(Shop_numMoneyBdg)};

static const SpriteAnimation Shop_numMoneyBdg_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Shop_numMoneyBdg_GetFrameInfo,
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

static void Shop_numMoneyBdg_CalcDigits(Shop_numMoneyBdg* arg0, u32* arg1) {
    ShopObject* shop = arg0->unk_2A8;
    u32         temp;
    s32         i;

    if (shop->slots[shop->unk_848].unk_10 == 0) {
        temp    = shop->slots[shop->unk_848].cost;
        arg1[0] = temp / 1000000;
        temp %= 1000000;
        arg1[1] = temp / 100000;
        temp %= 100000;
        arg1[2] = temp / 10000;
        temp %= 10000;
        arg1[3] = temp / 1000;
        temp %= 1000;
        arg1[4] = temp / 100;
        temp %= 100;
        arg1[5] = temp / 10;
        arg1[6] = temp % 10;

        for (i = 0; i < 10; i++) {
            arg0->unk_280[i] = TRUE;
        }

        arg0->unk_2AC = 7;
        for (i = 0; i < 6; i++) {
            if (arg1[i] != 0) {
                break;
            }
            arg0->unk_280[i + 1] = FALSE;
            arg0->unk_2AC--;
        }

        if (shop->slots[shop->unk_848].cost < 1000000) {
            arg0->unk_280[8] = FALSE;
            if (shop->slots[shop->unk_848].cost < 1000) {
                arg0->unk_280[9] = FALSE;
            }
        }
    } else {
        for (i = 0; i < 10; i++) {
            arg0->unk_280[i] = FALSE;
        }

        for (i = 0; i < 7; i++) {
            arg1[i] = 0;
        }
    }
}

static void Shop_numMoneyBdg_UpdateDigits(Shop_numMoneyBdg* arg0) {
    u32 digits[7];

    Shop_numMoneyBdg_CalcDigits(arg0, digits);
    for (u16 i = 0; i < 7; i++) {
        func_ov043_020af42c(&arg0->unk_000[i + 1], (s16)(digits[i] + 0x12));
    }
}

static SpriteFrameInfo* Shop_numMoneyBdg_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
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

static void Shop_numMoneyBdg_Load(Shop_numMoneyBdg* arg0, Sprite* arg1, Shop_numMoneyBdg_Args* args) {
    SpriteAnimation anim = Shop_numMoneyBdg_Anim;

    u32   digits[7];
    Point positions[5] = {
        {-10,  0},
        { 10, 16},
        { 22, 32},
        { 38, 44},
        {  5, 27},
    };

    anim.dataType = args->dataType;

    Shop_numMoneyBdg_CalcDigits(arg0, digits);

    anim.unk_2A = 0x1C;
    anim.unk_04 = positions[0].x + 202;
    anim.unk_06 = 0x77;
    _Sprite_Load(&arg1[0], &anim);

    for (u16 i = 0; i < 7; i++) {
        anim.unk_2A = digits[i] + 18;
        anim.unk_04 = ((s16*)positions)[i + 1] + 202;
        anim.unk_06 = 0x77;
        _Sprite_Load(&arg1[i + 1], &anim);
    }

    anim.unk_2A = 0x44;
    anim.unk_04 = positions[4].x + 202;
    anim.unk_06 = 0x77;
    _Sprite_Load(&arg1[8], &anim);

    anim.unk_2A = 0x44;
    anim.unk_04 = positions[4].y + 202;
    anim.unk_06 = 0x77;
    _Sprite_Load(&arg1[9], &anim);
}

static s32 Shop_numMoneyBdg_Init(TaskPool* pool, Task* task, void* args) {
    Shop_numMoneyBdg*      money     = task->data;
    Shop_numMoneyBdg_Args* moneyArgs = args;

    money->unk_2A8 = moneyArgs->shop;
    Shop_numMoneyBdg_Load(money, money->unk_000, moneyArgs);
    return 1;
}

static s32 Shop_numMoneyBdg_Update(TaskPool* pool, Task* task, void* args) {
    Shop_numMoneyBdg* money = task->data;
    s32               i;

    Shop_numMoneyBdg_UpdateDigits(money);
    for (i = 0; i < 0xA; i++) {
        Sprite_Update(&money->unk_000[i]);
    }
    return 1;
}

static s32 Shop_numMoneyBdg_Render(TaskPool* pool, Task* task, void* args) {
    Shop_numMoneyBdg* money = task->data;
    s32               i;

    for (i = 0; i < 0xA; i++) {
        if (money->unk_280[i] != 0) {
            Sprite_RenderFrame(&money->unk_000[i]);
        }
    }
    return 1;
}

static s32 Shop_numMoneyBdg_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_numMoneyBdg* money = task->data;

    for (s32 i = 0; i < 10; i++) {
        Sprite_Release(&money->unk_000[i]);
    }
    return 1;
}

static s32 Shop_numMoneyBdg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_numMoneyBdg_Init,
        .update     = Shop_numMoneyBdg_Update,
        .render     = Shop_numMoneyBdg_Render,
        .cleanup    = Shop_numMoneyBdg_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_numMoneyBdg_CreateTask(TaskPool* pool, s32 dataType, ShopObject* shop) {
    Shop_numMoneyBdg_Args args;

    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_numMoneyBdg, NULL, 0, NULL, &args);
}
