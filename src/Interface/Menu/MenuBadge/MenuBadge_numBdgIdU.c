#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[4];
    /* 0x100 */ s32              shouldRender[4];
    /* 0x110 */ MenuBadgeObject* menuBadge;
} MenuBadge_numBdgIdU; // Size: 0x114

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ u16              unk_8;
} MenuBadge_numBdgIdU_Args;

extern void func_ov043_0208fa14(Sprite* sprite, s16 frame);

static SpriteFrameInfo* MenuBadge_numBdgIdU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_numBdgIdU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_numBdgIdU = {"Tsk_MenuBadge_numBdgIdU", MenuBadge_numBdgIdU_RunTask,
                                                   sizeof(MenuBadge_numBdgIdU)};

static const SpriteAnimation MenuBadge_numBdgIdU_Anim = {
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
    .unk_08     = MenuBadge_numBdgIdU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82fc,
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

void MenuBadge_numBdgIdU_UpdateDigits(MenuBadge_numBdgIdU* numBdgId, u16 badgeId) {
    if (numBdgId->menuBadge->unk_ADD8 == 0xFFFF) {
        for (u16 i = 0; i < 3; i++) {
            numBdgId->shouldRender[i] = 0;
        }
        return;
    }

    u32 digits[3];
    u32 value = badgeId + 1;

    digits[0] = value / 100;

    u32 rem = value % 100;

    digits[1] = rem / 10;
    digits[2] = rem % 10;

    for (u16 i = 0; i < 3; i++) {
        numBdgId->shouldRender[i] = 1;
    }

    for (u16 i = 0; i < 3; i++) {
        func_ov043_0208fa14(&numBdgId->sprites[i], (s16)(digits[i] + 0x11));
    }
}

static SpriteFrameInfo* MenuBadge_numBdgIdU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

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

// Nonmatching
static void MenuBadge_numBdgIdU_Load(MenuBadge_numBdgIdU* numBdgId, Sprite* sprites, MenuBadge_numBdgIdU_Args* args) {
    MenuBadgeObject* menuBadge = numBdgId->menuBadge;
    SpriteAnimation  anim      = MenuBadge_numBdgIdU_Anim;
    u16              i;

    const MenuBadgePoint data_ov043_020c8fac[2] = {
        {225, 10},
        {236, 10}
    };

    u32 digits[3] = {0};

    anim.dataType = args->dataType;

    for (i = 0; i < 4; i++) {
        numBdgId->shouldRender[i] = 1;
    }

    if (menuBadge->unk_ADD8 == 0xFFFF) {
        for (i = 0; i < 3; i++) {
            numBdgId->shouldRender[i] = 0;
        }

    } else {
        u32 value = menuBadge->unk_ADD8 + 1;
        digits[0] = value / 100;
        u32 rem   = value % 100;
        digits[1] = rem / 10;
        digits[2] = rem % 10;
    }
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 0x11;
        anim.unk_04 = data_ov043_020c8fac[1].x + (i * 4);
        anim.unk_06 = data_ov043_020c8fac[1].y;
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.unk_2A = 0x10;
    anim.unk_04 = data_ov043_020c8fac[0].x;
    anim.unk_06 = data_ov043_020c8fac[0].y;
    _Sprite_Load(&sprites[3], &anim);
}

static s32 MenuBadge_numBdgIdU_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numBdgIdU*      numBdgId     = task->data;
    MenuBadge_numBdgIdU_Args* numBdgIdArgs = args;

    numBdgId->menuBadge = numBdgIdArgs->menuBadge;
    MenuBadge_numBdgIdU_Load(numBdgId, numBdgId->sprites, numBdgIdArgs);
    return 1;
}

static s32 MenuBadge_numBdgIdU_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numBdgIdU* numBdgId = task->data;

    MenuBadge_numBdgIdU_UpdateDigits(numBdgId, numBdgId->menuBadge->unk_ADD8);

    for (s32 i = 0; i < 4; i++) {
        Sprite_Update(&numBdgId->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_numBdgIdU_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numBdgIdU* numBdgId = task->data;

    for (s32 i = 0; i < 4; i++) {
        if (numBdgId->shouldRender[i] != 0) {
            Sprite_RenderFrame(&numBdgId->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuBadge_numBdgIdU_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_numBdgIdU* numBdgId = task->data;

    for (s32 i = 0; i < 4; i++) {
        Sprite_Release(&numBdgId->sprites[i]);
    }
    return 1;
}

static s32 MenuBadge_numBdgIdU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_numBdgIdU_Init,
        .update     = MenuBadge_numBdgIdU_Update,
        .render     = MenuBadge_numBdgIdU_Render,
        .cleanup    = MenuBadge_numBdgIdU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_numBdgIdU_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeObject* badge) {
    MenuBadge_numBdgIdU_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_numBdgIdU, NULL, 0, NULL, &args);
}
