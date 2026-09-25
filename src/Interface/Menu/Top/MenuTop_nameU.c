#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    Sprite sprites[5];
    s32    unk_140;
    s32    unk_144;
} MenuTop_nameU;

typedef struct {
    s32 dataType;
    s32 unk_4;
} MenuTop_nameU_Args;

typedef struct {
    /* 0x0 */ u16 x;
    /* 0x2 */ u16 y;
    /* 0x4 */ u16 unk_4; // always equals x
    /* 0x6 */ u16 unk_6; // always equals y
} MenuTop_nameU_Pos;

static SpriteFrameInfo* MenuTop_nameU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_nameU = {"Tsk_MenuTop_nameU", MenuTop_nameU_RunTask, sizeof(MenuTop_nameU)};

static const SpriteAnimation data_ov043_020c7cf8 = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuTop_nameU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c79a8,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuTop_nameU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuTop_nameU_Load(MenuTop_nameU* nameU, MenuTop_nameU_Args* args) {
    SpriteAnimation anim = data_ov043_020c7cf8;

    MenuTop_nameU_Pos data_ov043_020c7ce4[5] = {
        {0x06, 0x1C, 0x06, 0x1C},
        {0x06, 0x5E, 0x06, 0x5E},
        {0x06, 0x77, 0x06, 0x77},
        {0x06, 0x90, 0x06, 0x90},
        {0x06, 0xA9, 0x06, 0xA9},
    };

    s16 data_ov043_020c7cbc[] = {1, 2, 2, 2, 2, 0};

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 5; i++) {
        anim.unk_2A = data_ov043_020c7cbc[i];
        anim.unk_04 = data_ov043_020c7ce4[i].x;
        anim.unk_06 = data_ov043_020c7ce4[i].y;
        _Sprite_Load(&nameU->sprites[i], &anim);
    }
}

static s32 MenuTop_nameU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameU*      nameU    = task->data;
    MenuTop_nameU_Args* taskArgs = args;

    nameU->unk_144 = taskArgs->unk_4;
    nameU->unk_140 = 1;
    MenuTop_nameU_Load(nameU, taskArgs);
    return 1;
}

static s32 MenuTop_nameU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameU* nameU = task->data;

    for (s16 i = 0; i < 5; i++) {
        Sprite_Update(&nameU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameU_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameU* nameU = task->data;

    for (s16 i = 0; i < 5; i++) {
        Sprite_RenderFrame(&nameU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_nameU* nameU = task->data;

    for (s16 i = 0; i < 5; i++) {
        Sprite_Release(&nameU->sprites[i]);
    }
    return 1;
}

static s32 MenuTop_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_nameU_Init,
        .update     = MenuTop_nameU_Update,
        .render     = MenuTop_nameU_Render,
        .cleanup    = MenuTop_nameU_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_nameU_CreateTask(TaskPool* pool, s32 arg1, s32 arg2) {
    MenuTop_nameU_Args args;

    args.dataType = arg1;
    args.unk_4    = arg2;
    return EasyTask_CreateTask(pool, &Tsk_MenuTop_nameU, 0, 0, 0, &args);
}
