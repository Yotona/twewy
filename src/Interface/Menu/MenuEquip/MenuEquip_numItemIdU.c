#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[4];
    /* 0x100 */ BOOL             shouldRender[4];
    /* 0x110 */ MenuEquipObject* owner;
} MenuEquip_numItemIdU; // Size: 0x114

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ s32              unk_8;
} MenuEquip_numItemIdU_Args;

static SpriteFrameInfo* MenuEquip_numItemIdU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_numItemIdU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_numItemIdU = {"Tsk_MenuEquip_numItemIdU", MenuEquip_numItemIdU_RunTask,
                                                    sizeof(MenuEquip_numItemIdU)};

static const SpriteAnimation MenuEquip_numItemIdU_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuEquip_numItemIdU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[5],
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

static void MenuEquip_numItemIdU_UpdateDigits(MenuEquip_numItemIdU* numItemIdU, u16 itemId) {
    u32 digits[3];
    u32 number;

    if (numItemIdU->owner->cursorItem.itemId == 0xFFFF) {
        for (u16 i = 0; i < 3; i++) {
            numItemIdU->shouldRender[i] = FALSE;
        }
        return;
    }

    if (itemId < 0x130) {
        OS_WaitForever();
    }

    number    = itemId - 0x12F;
    digits[0] = number / 100;
    digits[1] = number % 100 / 10;
    digits[2] = number % 100 % 10;

    for (u16 i = 0; i < 3; i++) {
        numItemIdU->shouldRender[i] = TRUE;
    }

    for (u16 i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrame(&numItemIdU->sprites[i], digits[i] + 0x29);
    }
}

static SpriteFrameInfo* MenuEquip_numItemIdU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

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

// Nonmatching: regswap
static void MenuEquip_numItemIdU_Load(MenuEquip_numItemIdU* numItemIdU, Sprite* sprites, MenuEquip_numItemIdU_Args* args) {
    MenuEquipObject* owner       = numItemIdU->owner;
    SpriteAnimation  anim        = MenuEquip_numItemIdU_Anim;
    s16              digitPos[2] = {236, 10};
    s16              sepPos[2]   = {225, 10};
    u32              digits[3]   = {0, 0, 0};
    u32              number;

    anim.dataType = args->dataType;

    for (u16 i = 0; i < 4; i++) {
        numItemIdU->shouldRender[i] = TRUE;
    }

    if (owner->cursorItem.itemId == 0xFFFF) {
        for (u16 i = 0; i < 3; i++) {
            numItemIdU->shouldRender[i] = FALSE;
        }
    } else {
        if (owner->cursorItem.itemId < 304) {
            OS_WaitForever();
        }
        number    = owner->cursorItem.itemId - 303;
        digits[0] = number / 100;
        digits[1] = number % 100 / 10;
        digits[2] = number % 100 % 10;
    }

    for (u16 i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 41;
        anim.unk_04 = digitPos[0] + i * 4;
        anim.unk_06 = digitPos[1];
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.unk_2A = 40;
    anim.unk_04 = sepPos[0];
    anim.unk_06 = sepPos[1];
    _Sprite_Load(&sprites[3], &anim);
}

static s32 MenuEquip_numItemIdU_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numItemIdU*      numItemIdU     = task->data;
    MenuEquip_numItemIdU_Args* numItemIdUArgs = args;

    numItemIdU->owner = numItemIdUArgs->owner;
    MenuEquip_numItemIdU_Load(numItemIdU, numItemIdU->sprites, numItemIdUArgs);
    return 1;
}

static s32 MenuEquip_numItemIdU_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numItemIdU* numItemIdU = task->data;

    MenuEquip_numItemIdU_UpdateDigits(numItemIdU, numItemIdU->owner->cursorItem.itemId);

    for (s32 i = 0; i < 4; i++) {
        Sprite_Update(&numItemIdU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_numItemIdU_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numItemIdU* numItemIdU = task->data;

    for (s32 i = 0; i < 4; i++) {
        if (numItemIdU->shouldRender[i] != 0) {
            Sprite_RenderFrame(&numItemIdU->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_numItemIdU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numItemIdU* numItemIdU = task->data;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Release(&numItemIdU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_numItemIdU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_numItemIdU_Init,
        .update     = MenuEquip_numItemIdU_Update,
        .render     = MenuEquip_numItemIdU_Render,
        .cleanup    = MenuEquip_numItemIdU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_numItemIdU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_numItemIdU_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_numItemIdU, NULL, 0, NULL, &args);
}
