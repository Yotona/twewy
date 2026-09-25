#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[12];
    /* 0x300 */ BOOL             shouldRender[12];
    /* 0x330 */ MenuEquipObject* owner;
} MenuEquip_nameD; // Size: 0x334

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_nameD_Args;

static SpriteFrameInfo* MenuEquip_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_nameD = {"Tsk_MenuEquip_nameD", MenuEquip_nameD_RunTask, sizeof(MenuEquip_nameD)};

static const SpriteAnimation MenuEquip_nameD_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xC00,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_nameD_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* MenuEquip_nameD_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void MenuEquip_nameD_Load(MenuEquip_nameD* nameD, Sprite* sprites, MenuEquip_nameD_Args* args) {
    MenuEquipObject* owner            = nameD->owner;
    SpriteAnimation  anim             = MenuEquip_nameD_Anim;
    s16              positions[12][2] = {
        {  3,  31},
        {131,  31},
        {  3,  76},
        {131,  76},
        { 35,  84},
        { 65,  90},
        { 65,  98},
        { 65, 106},
        {163,  84},
        {193,  90},
        {193,  98},
        {193, 106},
    };
    s16 frames[12] = {25, 26, 29, 30, 31, 32, 33, 34, 31, 32, 33, 34};

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 12; i++) {
        nameD->shouldRender[i] = TRUE;
        if (i == 1) {
            if (owner->activePartner == 0) {
                anim.unk_2A = 26;
            } else if (owner->activePartner == 1) {
                anim.unk_2A = 27;
            } else if (owner->activePartner == 2) {
                anim.unk_2A = 28;
            } else {
                anim.unk_2A            = 1;
                nameD->shouldRender[i] = FALSE;
            }
        } else {
            anim.unk_2A = frames[i];
        }
        anim.unk_04 = positions[i][0];
        anim.unk_06 = positions[i][1];
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuEquip_nameD_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameD*      nameD     = task->data;
    MenuEquip_nameD_Args* nameDArgs = args;

    nameD->owner = nameDArgs->owner;
    MenuEquip_nameD_Load(nameD, nameD->sprites, nameDArgs);
    return 1;
}

static s32 MenuEquip_nameD_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameD* nameD = task->data;
    MenuEquipObject* owner = nameD->owner;

    if (owner->dirtyFlags & 0x10) {
        MenuEquip_SetSpriteFrame(&nameD->sprites[1], owner->activePartner + 26);
    }

    for (s16 i = 0; i < 12; i++) {
        Sprite_Update(&nameD->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_nameD_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameD* nameD = task->data;

    for (s16 i = 0; i < 12; i++) {
        if (nameD->shouldRender[i] != 0) {
            Sprite_RenderFrame(&nameD->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_nameD_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameD* nameD = task->data;

    for (s16 i = 0; i < 12; i++) {
        Sprite_Release(&nameD->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_nameD_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_nameD_Init,
        .update     = MenuEquip_nameD_Update,
        .render     = MenuEquip_nameD_Render,
        .cleanup    = MenuEquip_nameD_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_nameD_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_nameD_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_nameD, NULL, 0, NULL, &args);
}
