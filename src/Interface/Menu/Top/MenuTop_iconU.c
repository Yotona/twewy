#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite             sprite;
    /* 0x40 */ u32                unk_40;
    /* 0x44 */ UnkStruct_TopMenu* unk_44;
} MenuTop_iconU; // Size: 0x48

typedef struct {
    /* 0x0 */ s32                dataType;
    /* 0x4 */ UnkStruct_TopMenu* unk_4;
    /* 0x8 */ u16                unk_8;
    /* 0xA */ u16                unk_A;
} MenuTop_iconU_Args;

enum {
    UNKTHING_A = 0,
    UNKTHING_B = 1,
};

static SpriteFrameInfo* MenuTop_iconU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_iconU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation MenuTop_iconU_Anim = {
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
    .unk_08     = MenuTop_iconU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c79a8,
    .unk_18     = 0,
    .packIndex  = 1,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static const TaskHandle Tsk_MenuTop_iconU = {"Tsk_MenuTop_iconU", MenuTop_iconU_RunTask, sizeof(MenuTop_iconU)};

static SpriteFrameInfo* MenuTop_iconU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

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

static void MenuTop_iconU_Load(MenuTop_iconU* icon, Sprite* sprite, MenuTop_iconU_Args* args) {
    SpriteAnimation anim = MenuTop_iconU_Anim;

    anim.dataType = args->dataType;

    if (args->unk_8 < 21) {
        anim.unk_2A  = args->unk_A;
        anim.unk_04  = data_ov043_020c7914[args->unk_8].x;
        anim.unk_06  = data_ov043_020c7914[args->unk_8].y - 2;
        icon->unk_40 = 1;
    } else if (args->unk_8 <= 34) {
        anim.unk_2A  = args->unk_A;
        anim.unk_04  = data_ov043_020c7914[21].x;
        anim.unk_06  = data_ov043_020c7914[21].y - 2;
        icon->unk_40 = 1;
    } else {
        anim.unk_2A  = args->unk_A;
        anim.unk_04  = data_ov043_020c7914[22].x;
        anim.unk_06  = data_ov043_020c7914[22].y - 2;
        icon->unk_40 = 1;
    }

    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_iconU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU*      icon     = task->data;
    MenuTop_iconU_Args* iconArgs = args;

    icon->unk_44 = iconArgs->unk_4;
    MenuTop_iconU_Load(icon, &icon->sprite, iconArgs);
    return 1;
}

static s32 MenuTop_iconU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU* icon = task->data;

    icon->unk_40 = (icon->unk_44->unk_66 == 1) ? UNKTHING_A : UNKTHING_B;
    Sprite_Update(&icon->sprite);
    return 1;
}

static s32 MenuTop_iconU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU* icon = task->data;

    if (icon->unk_40) {
        Sprite_RenderFrame(&icon->sprite);
    }
    return 1;
}

static s32 MenuTop_iconU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_iconU* icon = task->data;

    Sprite_Release(&icon->sprite);
    return 1;
}

static s32 MenuTop_iconU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = MenuTop_iconU_Init,
        .update     = MenuTop_iconU_Update,
        .render     = MenuTop_iconU_Render,
        .cleanup    = MenuTop_iconU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_iconU_CreateTask(TaskPool* pool, s32 arg1, UnkStruct_TopMenu* arg2, u16 arg3, u16 arg4) {
    MenuTop_iconU_Args args;
    args.dataType = arg1;
    args.unk_4    = arg2;
    args.unk_8    = arg3;
    args.unk_A    = arg4;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_iconU, NULL, 0, NULL, &args);
}
