#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ BOOL             visible[3];
    /* 0xCC */ MenuEquipObject* owner;
} MenuEquip_helpCur; // Size: 0xD0

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_helpCur_Args;

extern s32 Inventory_IsHelpSeen(s32 arg0);

static SpriteFrameInfo* MenuEquip_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_helpCur = {"Tsk_MenuEquip_helpCur", MenuEquip_helpCur_RunTask,
                                                 sizeof(MenuEquip_helpCur)};

static const SpriteAnimation MenuEquip_helpCur_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = MenuEquip_helpCur_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 7,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 8,
    .unk_28            = 9,
    .animIndex         = 1,
};

static SpriteFrameInfo* MenuEquip_helpCur_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuEquip_helpCur_Load(MenuEquip_helpCur* helpCur, Sprite* sprites, MenuEquip_helpCur_Args* args) {
    SpriteAnimation anim = MenuEquip_helpCur_Anim;

    anim.dataType = args->dataType;

    for (u16 i = 0; i < 3; i++) {
        helpCur->visible[i] = TRUE;
    }

    anim.animIndex = 1;
    anim.posX      = 0x30;
    anim.posY      = 0x58;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 3;
    anim.posX      = 0xD0;
    anim.posY      = 0x58;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 5;
    anim.posX      = 0x80;
    anim.posY      = 0x7D;
    _Sprite_Load(&sprites[2], &anim);

    if (Inventory_IsHelpSeen(3) == 0) {
        helpCur->visible[2] = FALSE;
    }
}

static s32 MenuEquip_helpCur_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCur*      helpCur     = task->data;
    MenuEquip_helpCur_Args* helpCurArgs = args;

    helpCur->owner = helpCurArgs->owner;
    MenuEquip_helpCur_Load(helpCur, helpCur->sprites, helpCurArgs);
    return 1;
}

// Nonmatching: loop counter register allocation
static s32 MenuEquip_helpCur_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCur* helpCur = task->data;
    MenuEquipObject*   owner   = helpCur->owner;

    s32 idleFrame   = 1;
    s32 activeFrame = 2;

    for (s32 i = 0; i < 3; i++) {
        if (owner->helpPressed[i] == 1) {
            MenuEquip_SetSpriteFrameFromPack(&helpCur->sprites[i], activeFrame, 9, 8);
            if (owner->pressTimer != 0) {
                owner->pressTimer--;
            } else {
                owner->helpPressed[i] = 0;
            }
        } else {
            MenuEquip_SetSpriteFrameFromPack(&helpCur->sprites[i], idleFrame, 9, 8);
        }
        activeFrame += 2;
        idleFrame += 2;
    }

    if (owner->helpPage == 0) {
        helpCur->visible[0] = FALSE;
        helpCur->visible[1] = TRUE;
    } else if (owner->helpPage == 8) {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = FALSE;
    } else {
        helpCur->visible[0] = TRUE;
        helpCur->visible[1] = TRUE;
    }

    if (Inventory_IsHelpSeen(3) == 1) {
        helpCur->visible[2] = TRUE;
    }

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_helpCur_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        if (helpCur->visible[i] != 0) {
            Sprite_RenderFrame(&helpCur->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_helpCur_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_helpCur* helpCur = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&helpCur->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_helpCur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_helpCur_Init,
        .update     = MenuEquip_helpCur_Update,
        .render     = MenuEquip_helpCur_Render,
        .cleanup    = MenuEquip_helpCur_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_helpCur_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_helpCur_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_helpCur, NULL, 0, NULL, &args);
}
