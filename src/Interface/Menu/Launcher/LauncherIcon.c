#include "Engine/Core//OamMgr.h"
#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ s32    unk_00;
    /* 0x04 */ s32    unk_04;
    /* 0x08 */ s32    unk_08;
    /* 0x0C */ s32    unk_0C;
    /* 0x10 */ Sprite sprite;
} LauncherIcon; // Size: 0x50

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ s32 unk_8;
    /* 0xC */ s32 unk_C;
} LauncherIcon_Args; // Size: 0x10

extern const BinIdentifier data_ov046_02083a4c;

SpriteFrameInfo* func_ov046_02083698(Sprite* sprite, s32 arg1, s32 arg2);
static s32       LauncherIcon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

SpriteAnimation data_ov046_02084804 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0,
    .unk_06     = 0,
    .unk_08     = func_ov046_02083698,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov046_02083a4c,
    .unk_18     = 2,
    .packIndex  = 2,
    .unk_1C     = 0,
    .unk_1E     = 0,
    .unk_20     = 10,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 0,
    .unk_28     = 0,
    .unk_2A     = 1,
};

const TaskHandle Tsk_LauncherIcon = {"LauncherIcon", LauncherIcon_RunTask, sizeof(LauncherIcon)};

// Non-matching: Significant differences
SpriteFrameInfo* func_ov046_02083698(Sprite* sprite, s32 arg1, s32 mode) {
    void* temp = sprite->unk24;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
        } break;

        case 2: {
            if (*(s32*)(temp + 0x8) == 0x1000 && *(s32*)(temp + 0xC) != 0x1000) {
                sprite->unk_0A.unk_00 = 1;
                sprite->unk_0A.unk_01 = 0;
                sprite->unk_0A.unk_05 =
                    OamMgr_AllocAffineGroup(&g_OamMgr[sprite->bits_0_1], 0, *(u32*)(temp + 0x8), *(u32*)(temp + 0xC), 0);
            } else {
                sprite->unk_0A.unk_00 = 0;
                sprite->unk_0A.unk_01 = 0;
                sprite->unk_0A.unk_05 = 0;
            }
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;
            data_0206b408.unk_0C = 0;
            data_0206b408.unk_10 = -1;
            if (sprite->animData != NULL && sprite->frameDataTable != 0 && sprite->unk16 >= 0) {
                data_0206b408.unk_04 = *(u16*)(sprite->frameDataTable + (((sprite->unk16 * 2) + 1) * 2));
                data_0206b408.unk_08 = sprite->frameDataTable + (*(u16*)(sprite->frameDataTable + (sprite->unk16 * 8)) * 2);
            }
            if (*(s32*)(temp + 0x8) != 0 && *(s32*)(temp + 0xC) == 0) {
                data_0206b408.unk_08 = 0;
            }
        } break;
    }
    return &data_0206b408;
}

s32 func_ov046_020837f8(LauncherIcon* icon, LauncherIcon_Args* args) {
    icon->unk_00                  = 0;
    icon->unk_04                  = 0;
    icon->unk_08                  = 0;
    icon->unk_0C                  = 0;
    data_ov046_02084804.dataType  = args->dataType;
    data_ov046_02084804.unk_04    = args->unk_8 + 16;
    data_ov046_02084804.unk_06    = args->unk_C + 16;
    data_ov046_02084804.unk_10    = icon;
    data_ov046_02084804.packIndex = 2;
    if (args->unk_4 < 10) {
        data_ov046_02084804.unk_2A = args->unk_4 + 1;
        data_ov046_02084804.unk_1C = 4;
        data_ov046_02084804.unk_26 = 5;
        data_ov046_02084804.unk_28 = 6;
    } else if (args->unk_4 < 37) {
        data_ov046_02084804.unk_2A = args->unk_4 - 10;
        data_ov046_02084804.unk_1C = 1;
        data_ov046_02084804.unk_26 = 2;
        data_ov046_02084804.unk_28 = 3;
    } else {
        data_ov046_02084804.unk_2A = args->unk_4 - 37;
        data_ov046_02084804.unk_1C = 7;
        data_ov046_02084804.unk_26 = 8;
        data_ov046_02084804.unk_28 = 9;
    }
    _Sprite_Load(&icon->sprite, &data_ov046_02084804);
    return 1;
}

BOOL func_ov046_020838ec(LauncherIcon* icon) {
    BOOL result = TRUE;
    switch (icon->unk_00) {
        case 0:
            if (icon->unk_04 < 8) {
                icon->unk_04 += 1;
            } else {
                if (icon->unk_08 < 0x1000) {
                    icon->unk_08 += 0x200;
                    icon->unk_0C += 0x200;
                }
            }
            break;

        case 1:
            if (icon->unk_08 <= 0) {
                result = FALSE;
            } else {
                icon->unk_08 -= 0x200;
                icon->unk_0C -= 0x200;
            }
            break;
    }

    if (result == TRUE) {
        Sprite_UpdateAndCheck(&icon->sprite);
    }
    return result;
}

void func_ov046_02083980(LauncherIcon* icon) {
    Sprite_Render(&icon->sprite);
}

void func_ov046_02083990(LauncherIcon* icon) {
    Sprite_Destroy(&icon->sprite);
}

s32 LauncherIcon_Init(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    return func_ov046_020837f8(icon, args);
}

s32 LauncherIcon_Update(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    return func_ov046_020838ec(icon);
}

s32 LauncherIcon_Render(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    func_ov046_02083980(icon);
    return 1;
}

s32 LauncherIcon_Destroy(TaskPool* pool, Task* task, void* args) {
    LauncherIcon* icon = task->data;

    func_ov046_02083990(icon);
    return 1;
}

s32 LauncherIcon_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = LauncherIcon_Init,
        .update     = LauncherIcon_Update,
        .render     = LauncherIcon_Render,
        .cleanup    = LauncherIcon_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
