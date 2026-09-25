#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[2];
    /* 0x80 */ BOOL             shouldRender[2];
    /* 0x88 */ MenuEquipObject* owner;
} MenuEquip_helpCurU; // Size: 0x8C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_helpCurU_Args;

static SpriteFrameInfo* MenuEquip_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_helpCurU = {"Tsk_MenuEquip_helpCurU", MenuEquip_helpCurU_RunTask,
                                                  sizeof(MenuEquip_helpCurU)};

static const SpriteAnimation MenuEquip_helpCurU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_helpCurU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[5],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_helpCurU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuEquip_helpCurU_Load(Sprite* sprites, MenuEquip_helpCurU_Args* args) {
    SpriteAnimation anim = MenuEquip_helpCurU_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x26;
    anim.unk_04 = 7;
    anim.unk_06 = 0x60;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x27;
    anim.unk_04 = 0xF9;
    anim.unk_06 = 0x60;
    _Sprite_Load(&sprites[1], &anim);
}

static s32 MenuEquip_helpCurU_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCurU*      helpCurU     = task->data;
    MenuEquip_helpCurU_Args* helpCurUArgs = args;

    helpCurU->owner = helpCurUArgs->owner;
    MenuEquip_helpCurU_Load(helpCurU->sprites, helpCurUArgs);
    return 1;
}

static s32 MenuEquip_helpCurU_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCurU* helpCurU = task->data;

    if (helpCurU->owner->helpPage == 0) {
        helpCurU->shouldRender[0] = FALSE;
        helpCurU->shouldRender[1] = TRUE;
    } else if (helpCurU->owner->helpPage == 8) {
        helpCurU->shouldRender[0] = TRUE;
        helpCurU->shouldRender[1] = FALSE;
    } else {
        helpCurU->shouldRender[0] = TRUE;
        helpCurU->shouldRender[1] = TRUE;
    }

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_helpCurU_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->shouldRender[i] != 0) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_helpCurU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCurU* helpCurU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_helpCurU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_helpCurU_Init,
        .update     = MenuEquip_helpCurU_Update,
        .render     = MenuEquip_helpCurU_Render,
        .cleanup    = MenuEquip_helpCurU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_helpCurU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_helpCurU_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_helpCurU, NULL, 0, NULL, &args);
}
