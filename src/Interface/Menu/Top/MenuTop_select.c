#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ s32    unk_44;
} MenuTop_select; // Size: 0x48

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ u16 unk_8;
} MenuTop_select_Args;

static SpriteFrameInfo* MenuTop_select_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              MenuTop_select_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

extern s32           func_ov043_0208498c(void);
extern s32           func_ov043_020849cc(s16 arg0);
extern BinIdentifier data_ov043_020c7980;

static const TaskHandle Tsk_MenuTop_select = {"Tsk_MenuTop_select", MenuTop_select_RunTask, sizeof(MenuTop_select)};

static const s16 data_ov043_020c7ac4[] = {1, 2, 3, 4, 5, 6, 7, 8};

static const Point data_ov043_020c7ad4[] = {
    {0x22, 0x31},
    {0x61, 0x31},
    {0xA0, 0x31},
    {0xDF, 0x31},
    {0x22, 0x67},
    {0x61, 0x67},
    {0xA0, 0x67},
    {0xDF, 0x67},
};

static const SpriteAnimation data_ov043_020c7af4 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 6,
    .bits_10_11 = 1,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x20,
    .unk_06     = 0x3C,
    .unk_08     = MenuTop_select_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c7980,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 10,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuTop_select_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        }

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
static void MenuTop_select_Load(Sprite* sprite, MenuTop_select_Args* args) {
    s16             var_5A;
    Point           points[8];
    s16             frameIds[8];
    SpriteAnimation anim = data_ov043_020c7af4;

    for (s32 i = 0; i < 8; i++) {
        points[i] = data_ov043_020c7ad4[i];
    }

    for (s32 i = 0; i < 8; i++) {
        frameIds[i] = data_ov043_020c7ac4[i];
    }

    anim.dataType = args->dataType;
    anim.unk_04   = points[args->unk_8].x;
    anim.unk_06   = points[args->unk_8].y;

    if (func_ov043_020849cc(args->unk_8) == 0) {
        if (args->unk_8 == 7) {
            var_5A = 9;
        } else if (args->unk_8 == 6) {
            if (func_ov043_0208498c() == 1) {
                var_5A = 9;
            } else {
                var_5A = 10;
            }
        } else {
            var_5A = 10;
        }
    } else {
        anim.unk_2A = frameIds[args->unk_8];
    }

    anim.unk_2A = var_5A;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_select_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_select*      select    = task->data;
    MenuTop_select_Args* selectArg = args;

    MenuTop_select_Load(&select->sprite, selectArg);
    select->unk_44 = selectArg->unk_4;
    return 1;
}

static s32 MenuTop_select_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_select* select = task->data;

    Sprite_Update(&select->sprite);
    return 1;
}

static s32 MenuTop_select_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_select* select = task->data;

    Sprite_RenderFrame(&select->sprite);
    return 1;
}

static s32 MenuTop_select_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_select* select = task->data;

    Sprite_Release(&select->sprite);
    return 1;
}

static s32 MenuTop_select_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_select_Init,
        .update     = MenuTop_select_Update,
        .render     = MenuTop_select_Render,
        .cleanup    = MenuTop_select_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_select_CreateTask(TaskPool* pool, s32 dataType, s32 arg2, s16 arg3) {
    MenuTop_select_Args args;

    args.dataType = dataType;
    args.unk_4    = arg2;
    args.unk_8    = arg3;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_select, NULL, 0, NULL, &args);
}
