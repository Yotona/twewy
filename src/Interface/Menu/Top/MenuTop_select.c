#include "Engine/EasyTask.h"
#include "Interface/Menu/Top.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ s32            unk_40;
    /* 0x44 */ MenuTopObject* topMenu;
} MenuTop_select; // Size: 0x48

typedef struct {
    /* 0x0 */ s32            dataType;
    /* 0x4 */ MenuTopObject* topMenu;
    /* 0x8 */ u16            entry;
} MenuTop_select_Args;

static SpriteFrameInfo* MenuTop_select_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuTop_select_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_select = {"Tsk_MenuTop_select", MenuTop_select_RunTask, sizeof(MenuTop_select)};

static const s16 MenuTop_select_Frames[] = {1, 2, 3, 4, 5, 6, 7, 8};

static const Point MenuTop_select_Positions[] = {
    {0x22, 0x31},
    {0x61, 0x31},
    {0xA0, 0x31},
    {0xDF, 0x31},
    {0x22, 0x67},
    {0x61, 0x67},
    {0xA0, 0x67},
    {0xDF, 0x67},
};

static const SpriteAnimation MenuTop_select_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x20,
    .posY              = 0x3C,
    .frameInfoCallback = MenuTop_select_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &MenuTop_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 10,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuTop_select_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching
static void MenuTop_select_Load(Sprite* sprite, MenuTop_select_Args* args) {
    s16             frame;
    Point           points[8];
    s16             frameIds[8];
    SpriteAnimation anim = MenuTop_select_Anim;

    for (s32 i = 0; i < 8; i++) {
        points[i] = MenuTop_select_Positions[i];
    }

    for (s32 i = 0; i < 8; i++) {
        frameIds[i] = MenuTop_select_Frames[i];
    }

    anim.dataType = args->dataType;
    anim.posX     = points[args->entry].x;
    anim.posY     = points[args->entry].y;

    if (MenuTop_IsEntryAvailable(args->entry) == 0) {
        if (args->entry == 7) {
            frame = 9;
        } else if (args->entry == 6) {
            if (MenuTop_IsInRestrictedArea() == 1) {
                frame = 9;
            } else {
                frame = 10;
            }
        } else {
            frame = 10;
        }
    } else {
        anim.animIndex = frameIds[args->entry];
    }

    anim.animIndex = frame;
    _Sprite_Load(sprite, &anim);
}

static s32 MenuTop_select_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_select*      select    = task->data;
    MenuTop_select_Args* selectArg = args;

    MenuTop_select_Load(&select->sprite, selectArg);
    select->topMenu = selectArg->topMenu;
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

s32 MenuTop_select_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu, u16 entry) {
    MenuTop_select_Args args;

    args.dataType = dataType;
    args.topMenu  = topMenu;
    args.entry    = entry;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_select, NULL, 0, NULL, &args);
}
