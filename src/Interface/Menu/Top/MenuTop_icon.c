#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ u32    unk_80;
    /* 0x84 */ u8*    unk_84;
} MenuTop_icon; // Size: 0x88

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ u8* unk_4;
} MenuTop_icon_Args;

static SpriteFrameInfo* MenuTop_icon_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern void func_ov043_02084620(Sprite* sprite, s16 arg1);

extern BinIdentifier data_ov043_020c79d0;

static const u16 data_ov043_020c7b20[] = {3, 5};

static const TaskHandle Tsk_MenuTop_icon = {"Tsk_MenuTop_icon", MenuTop_icon_RunTask, sizeof(MenuTop_icon)};

static const SpriteAnimation MenuTop_icon_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0xB6,
    .unk_06     = 0x10,
    .unk_08     = MenuTop_icon_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79d0,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 1,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_icon_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* frameInfo = NULL;

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
            frameInfo = temp;
        } break;
    }

    return frameInfo;
}

// Nonmatching
static void MenuTop_icon_Load(MenuTop_icon* icon, MenuTop_icon_Args* args) {
    SpriteAnimation anim = MenuTop_icon_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 3;
    anim.unk_04 = 0xDA;
    anim.unk_06 = 0xC;
    anim.unk_22 = 1;
    _Sprite_Load(&icon->sprites[0], &anim);

    anim.unk_2A = 5;
    anim.unk_04 = 0xF3;
    anim.unk_06 = 0xC;
    anim.unk_22 = 1;
    _Sprite_Load(&icon->sprites[1], &anim);
}

static s32 MenuTop_icon_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon*      icon     = task->data;
    MenuTop_icon_Args* iconArgs = args;

    icon->unk_84 = iconArgs->unk_4;
    MenuTop_icon_Load(icon, iconArgs);
    return 1;
}

// Nonmatching
static s32 MenuTop_icon_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        if (icon->unk_84[i + 2] == 1) {
            func_ov043_02084620(&icon->sprites[i], (s16)(data_ov043_020c7b20[i] + 1));

            if (icon->unk_84[4] != 0) {
                icon->unk_84[4]--;
            } else {
                icon->unk_84[i + 2] = 0;
            }
        } else {
            func_ov043_02084620(&icon->sprites[i], (s16)data_ov043_020c7b20[i]);
        }
    }

    for (s32 i = 0; i < 2; i++) {
        Sprite_Update(&icon->sprites[i]);
    }

    return 1;
}

static s32 MenuTop_icon_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_RenderFrame(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_icon_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_icon* icon = task->data;

    for (s32 i = 0; i < 2; i++) {
        Sprite_Release(&icon->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_icon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_icon_Init,
        .update     = MenuTop_icon_Update,
        .render     = MenuTop_icon_Render,
        .cleanup    = MenuTop_icon_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_icon_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    MenuTop_icon_Args args;

    args.dataType = arg1;
    args.unk_4    = arg2;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_icon, NULL, 0, NULL, &args);
}
