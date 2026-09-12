#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[8];
    /* 0x200 */ BOOL             visible;
    /* 0x204 */ MenuEquipObject* owner;
} MenuEquip_nameU; // Size: 0x208

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_nameU_Args;

static SpriteFrameInfo* MenuEquip_nameU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_nameU = {"Tsk_MenuEquip_nameU", MenuEquip_nameU_RunTask, sizeof(MenuEquip_nameU)};

static const SpriteAnimation MenuEquip_nameU_Anim = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuEquip_nameU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[5],
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

static SpriteFrameInfo* MenuEquip_nameU_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

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

            info = temp;
        } break;
    }

    return info;
}

static void MenuEquip_nameU_Load(Sprite* sprites, MenuEquip_nameU_Args* args) {
    SpriteAnimation anim            = MenuEquip_nameU_Anim;
    s16             positions[8][2] = {
        {  6,  11},
        {  6,  35},
        {  6,  91},
        {  6, 145},
        {  9,  62},
        {  9,  74},
        {132,  62},
        {132,  74},
    };
    s16 frames[8] = {1, 2, 3, 4, 7, 8, 9, 10};

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 8; i++) {
        anim.unk_2A = frames[i];
        anim.unk_04 = positions[i][0];
        anim.unk_06 = positions[i][1];
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuEquip_nameU_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameU*      nameU     = task->data;
    MenuEquip_nameU_Args* nameUArgs = args;

    MenuEquip_nameU_Load(nameU->sprites, nameUArgs);
    nameU->visible = TRUE;
    nameU->owner   = nameUArgs->owner;
    return 1;
}

static s32 MenuEquip_nameU_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameU* nameU    = task->data;
    u8               category = nameU->owner->cursorItem.category;

    if (category == 2) {
        MenuEquip_SetSpriteFrame(&nameU->sprites[2], 3);
        MenuEquip_SetSpriteFrame(&nameU->sprites[3], 5);
    } else if (category == 3) {
        MenuEquip_SetSpriteFrame(&nameU->sprites[2], 3);
        MenuEquip_SetSpriteFrame(&nameU->sprites[3], 6);
    } else {
        MenuEquip_SetSpriteFrame(&nameU->sprites[2], 3);
        MenuEquip_SetSpriteFrame(&nameU->sprites[3], 4);
    }

    for (s16 i = 0; i < 8; i++) {
        Sprite_Update(&nameU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_nameU_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameU* nameU = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_RenderFrame(&nameU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_nameU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_nameU* nameU = task->data;

    for (s16 i = 0; i < 8; i++) {
        Sprite_Release(&nameU->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_nameU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_nameU_Init,
        .update     = MenuEquip_nameU_Update,
        .render     = MenuEquip_nameU_Render,
        .cleanup    = MenuEquip_nameU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_nameU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_nameU_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_nameU, NULL, 0, NULL, &args);
}
