#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ BOOL             shouldRender[3];
    /* 0xCC */ MenuEquipObject* owner;
} MenuEquip_itemNum; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              index;
    /* 0xA */ u16              itemId;
    /* 0xC */ u16              count;
} MenuEquip_itemNum_Args;

static SpriteFrameInfo* MenuEquip_itemNum_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_itemNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_itemNum = {"Tsk_MenuEquip_itemNum", MenuEquip_itemNum_RunTask,
                                                 sizeof(MenuEquip_itemNum)};

static const SpriteAnimation MenuEquip_itemNum_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_itemNum_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 4,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 5,
    .unk_28            = 6,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_itemNum_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3A2000);
}

static void MenuEquip_itemNum_Load(MenuEquip_itemNum* itemNum, Sprite* sprites, MenuEquip_itemNum_Args* args) {
    SpriteAnimation anim      = MenuEquip_itemNum_Anim;
    u16             digits[2] = {1, 1};
    s32             offsetX   = 0;

    anim.dataType = args->dataType;

    if (args->itemId == 0xFFFF || args->index <= 9) {
        itemNum->shouldRender[0] = FALSE;
        itemNum->shouldRender[1] = FALSE;
        itemNum->shouldRender[2] = FALSE;
    } else {
        u16 count = args->count;

        if (count < 10) {
            itemNum->shouldRender[0] = TRUE;
            itemNum->shouldRender[1] = TRUE;
            digits[0]                = count;
            digits[1]                = 0;
            itemNum->shouldRender[2] = FALSE;
        } else {
            itemNum->shouldRender[0] = TRUE;
            itemNum->shouldRender[1] = TRUE;
            digits[0]                = count / 10;
            digits[1]                = count % 10;
            itemNum->shouldRender[2] = TRUE;
            offsetX                  = -2;
        }
    }

    anim.unk_2A = 0x38;
    anim.unk_04 = offsetX + (data_ov043_020c96f0[args->index][0] + 0xA);
    anim.unk_06 = data_ov043_020c96f0[args->index][1] + 0xB;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = digits[0] + 0x2E;
    anim.unk_04 = offsetX + (data_ov043_020c96f0[args->index][0] + 0xA);
    anim.unk_06 = data_ov043_020c96f0[args->index][1] + 0xB;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = digits[1] + 0x2E;
    anim.unk_04 = offsetX + (data_ov043_020c96f0[args->index][0] + 0xE);
    anim.unk_06 = data_ov043_020c96f0[args->index][1] + 0xB;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 MenuEquip_itemNum_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemNum*      itemNum     = task->data;
    MenuEquip_itemNum_Args* itemNumArgs = args;

    MenuEquip_itemNum_Load(itemNum, itemNum->sprites, itemNumArgs);
    itemNum->owner = itemNumArgs->owner;
    return 1;
}

static s32 MenuEquip_itemNum_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemNum* itemNum = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&itemNum->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_itemNum_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemNum* itemNum = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (itemNum->shouldRender[i] != 0) {
            Sprite_RenderFrame(&itemNum->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_itemNum_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemNum* itemNum = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&itemNum->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_itemNum_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_itemNum_Init,
        .update     = MenuEquip_itemNum_Update,
        .render     = MenuEquip_itemNum_Render,
        .cleanup    = MenuEquip_itemNum_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_itemNum_CreateTask(TaskPool* pool, s32 dataType, u16 index, MenuEquipObject* owner) {
    MenuEquip_itemNum_Args args;

    args.index    = index;
    args.owner    = owner;
    args.dataType = dataType;
    args.itemId   = owner->slots[index]->itemId;
    args.count    = owner->slots[index]->count;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_itemNum, NULL, 0, NULL, &args);
}
