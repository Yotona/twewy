#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[2];
    /* 0x80 */ char             unk_80[0x84 - 0x80];
    /* 0x84 */ MenuEquipObject* owner;
} MenuEquip_icon; // Size: 0x88

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_icon_Args;

static SpriteFrameInfo* MenuEquip_icon_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_icon = {"Tsk_MenuEquip_icon", MenuEquip_icon_RunTask, sizeof(MenuEquip_icon)};

static const SpriteAnimation MenuEquip_icon_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0xB6,
    .unk_06     = 0x10,
    .unk_08     = MenuEquip_icon_GetFrameInfo,
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

static SpriteFrameInfo* MenuEquip_icon_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuEquip_icon_Load(Sprite* sprites, MenuEquip_icon_Args* args) {
    SpriteAnimation anim = MenuEquip_icon_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x26;
    anim.unk_04 = 0xDA;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x28;
    anim.unk_04 = 0xF3;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuEquip_icon_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_icon*      icon     = task->data;
    MenuEquip_icon_Args* iconArgs = args;

    MenuEquip_icon_Load(icon->sprites, iconArgs);
    icon->owner = iconArgs->owner;
    return 1;
}

// Nonmatching: loop counter register allocation
static s32 MenuEquip_icon_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_icon*  icon  = task->data;
    MenuEquipObject* owner = icon->owner;

    s32 idleFrame   = 38;
    s32 activeFrame = 39;

    for (s32 i = 0; i < 2; i++) {
        if (owner->iconPressed[i] == 1) {
            MenuEquip_SetSpriteFrameFromPack(&icon->sprites[i], activeFrame, 3, 2);
            if (owner->pressTimer != 0) {
                owner->pressTimer--;
            } else {
                owner->iconPressed[i] = 0;
            }
        } else {
            MenuEquip_SetSpriteFrameFromPack(&icon->sprites[i], idleFrame, 3, 2);
        }
        activeFrame += 2;
        idleFrame += 2;
    }

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_icon_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_icon_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_icon_Init,
        .update     = MenuEquip_icon_Update,
        .render     = MenuEquip_icon_Render,
        .cleanup    = MenuEquip_icon_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_icon_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_icon_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_icon, NULL, 0, NULL, &args);
}
