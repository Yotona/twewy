#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ char             unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ MenuEquipObject* owner;
    /* 0xC8 */ u16              lastScroll;
} MenuEquip_sbar2; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_sbar2_Args;

static SpriteFrameInfo* MenuEquip_sbar2_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_sbar2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_sbar2 = {"Tsk_MenuEquip_sbar2", MenuEquip_sbar2_RunTask, sizeof(MenuEquip_sbar2)};

static const SpriteAnimation MenuEquip_sbar2_Anim = {
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
    .unk_08     = MenuEquip_sbar2_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[2],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xA,
    .unk_22     = 6,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuEquip_sbar2_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuEquip_sbar2_Load(Sprite* sprites, MenuEquip_sbar2_Args* args) {
    SpriteAnimation anim = MenuEquip_sbar2_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x2E;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0xB0;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x11;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0x78;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x12;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0xB7;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 MenuEquip_sbar2_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar2_Args* sbarArgs = args;
    MenuEquipObject*      owner    = sbarArgs->owner;

    MenuEquip_sbar2* sbar = task->data;

    sbar->owner      = owner;
    sbar->lastScroll = owner->listScroll;
    MenuEquip_sbar2_Load(sbar->sprites, sbarArgs);
    return 1;
}

static s32 MenuEquip_sbar2_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar2* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_sbar2_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar2* sbar  = task->data;
    MenuEquipObject* owner = sbar->owner;

    if (owner->fitsOnePage[owner->currentTab] == 1) {
        for (s32 i = 0; i < 3; i++) {
            Sprite_RenderFrame(&sbar->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_sbar2_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar2* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_sbar2_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_sbar2_Init,
        .update     = MenuEquip_sbar2_Update,
        .render     = MenuEquip_sbar2_Render,
        .cleanup    = MenuEquip_sbar2_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_sbar2_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_sbar2_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_sbar2, NULL, 0, NULL, &args);
}
