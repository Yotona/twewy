#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[24];
    /* 0x600 */ BOOL             shouldRender[24];
    /* 0x660 */ MenuEquipObject* owner;
    /* 0x664 */ u16              index;
    /* 0x666 */ u16              unk_666;
    /* 0x668 */ u16              unk_668;
    /* 0x66A */ u16              unk_66A;
} MenuEquip_foodGauge; // Size: 0x66C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              index;
    /* 0xA */ u16              unk_A;
    /* 0xC */ u16              unk_C;
} MenuEquip_foodGauge_Args;

static SpriteFrameInfo* MenuEquip_foodGauge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_foodGauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_foodGauge = {"Tsk_MenuEquip_foodGauge", MenuEquip_foodGauge_RunTask,
                                                   sizeof(MenuEquip_foodGauge)};

static const SpriteAnimation MenuEquip_foodGauge_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_foodGauge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 1,
    .unk_1C            = 4,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 5,
    .unk_28            = 6,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_foodGauge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3D4000);
}

static void MenuEquip_foodGauge_Load(MenuEquip_foodGauge* foodGauge, Sprite* sprites, MenuEquip_foodGauge_Args* args) {
    SpriteAnimation anim            = MenuEquip_foodGauge_Anim;
    s16             positions[2][2] = {
        { 33, 103},
        {161, 103},
    };

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 24; i++) {
        anim.unk_04 = (s16)((i % 6) * 5) + positions[args->index][0] + 2;
        anim.unk_06 = positions[args->index][1] - (s16)((i / 6) * 5) + 2;
        if (i < foodGauge->unk_666) {
            if (i < 12) {
                anim.unk_2A = 0x3C;
            } else if (i < 18) {
                anim.unk_2A = 0x3B;
            } else {
                anim.unk_2A = 0x3A;
            }
            foodGauge->shouldRender[i] = TRUE;
        } else if (i > foodGauge->unk_66A - 1) {
            anim.unk_2A                = 0x3E;
            foodGauge->shouldRender[i] = TRUE;
        } else {
            anim.unk_2A                = 0x3C;
            foodGauge->shouldRender[i] = FALSE;
        }
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuEquip_foodGauge_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_foodGauge*      foodGauge     = task->data;
    MenuEquip_foodGauge_Args* foodGaugeArgs = args;

    foodGauge->owner   = foodGaugeArgs->owner;
    foodGauge->index   = foodGaugeArgs->index;
    foodGauge->unk_668 = foodGauge->unk_666 = foodGaugeArgs->unk_A;
    foodGauge->unk_66A                      = foodGaugeArgs->unk_C;
    MenuEquip_foodGauge_Load(foodGauge, foodGauge->sprites, foodGaugeArgs);
    return 1;
}

static s32 MenuEquip_foodGauge_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_foodGauge* foodGauge = task->data;

    for (s16 i = 0; i < 24; i++) {
        Sprite_Update(&foodGauge->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_foodGauge_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_foodGauge* foodGauge = task->data;

    for (s16 i = 0; i < 24; i++) {
        if (foodGauge->shouldRender[i] != 0) {
            Sprite_RenderFrame(&foodGauge->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_foodGauge_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_foodGauge* foodGauge = task->data;

    for (s16 i = 0; i < 24; i++) {
        Sprite_Release(&foodGauge->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_foodGauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_foodGauge_Init,
        .update     = MenuEquip_foodGauge_Update,
        .render     = MenuEquip_foodGauge_Render,
        .cleanup    = MenuEquip_foodGauge_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_foodGauge_CreateTask(TaskPool* pool, s32 dataType, s16 index, MenuEquipObject* owner) {
    MenuEquip_foodGauge_Args args;

    args.dataType = dataType;
    args.owner    = owner;
    args.index    = index;
    args.unk_A    = owner->slots[index * 5 + 4]->capacityCost;

    if (index == 0) {
        args.unk_C = owner->stats[0].foodCapacityLeft;
    } else if (owner->activePartner == 0xFF) {
        args.unk_C = 0x18;
    } else {
        args.unk_C = owner->stats[owner->activePartner + 1].foodCapacityLeft;
    }

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_foodGauge, NULL, 0, NULL, &args);
}
