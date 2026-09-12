#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[8];
    /* 0x200 */ BOOL             visible;
    /* 0x204 */ MenuEquipObject* owner;
} MenuEquip_tab; // Size: 0x208

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_tab_Args;

static SpriteFrameInfo* MenuEquip_tab_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_tab = {"Tsk_MenuEquip_tab", MenuEquip_tab_RunTask, sizeof(MenuEquip_tab)};

static const SpriteAnimation MenuEquip_tab_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = -0xD,
    .unk_06     = 0xC,
    .unk_08     = MenuEquip_tab_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[2],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xA,
    .unk_22     = 6,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuEquip_tab_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuEquip_tab_Load(Sprite* sprites, MenuEquip_tab_Args* args) {
    SpriteAnimation anim = MenuEquip_tab_Anim;

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 8; i++) {
        anim.unk_2A = i * 2 + 1;
        anim.unk_04 = i * 14 + 138;
        anim.unk_06 = 121;
        _Sprite_Load(&sprites[i], &anim);
    }
}

static s32 MenuEquip_tab_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_tab*      tab     = task->data;
    MenuEquip_tab_Args* tabArgs = args;

    tab->owner   = tabArgs->owner;
    tab->visible = TRUE;
    MenuEquip_tab_Load(tab->sprites, tabArgs);
    return 1;
}

// Nonmatching: loop counter register allocation
static s32 MenuEquip_tab_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_tab*   tab   = task->data;
    MenuEquipObject* owner = tab->owner;

    s32 idleFrame   = 1;
    s32 activeFrame = 2;

    for (s32 i = 0; i < 8; i++) {
        if (owner->tabActive[i] == 1) {
            MenuEquip_SetSpriteFrame(&tab->sprites[i], activeFrame);
        } else {
            MenuEquip_SetSpriteFrame(&tab->sprites[i], idleFrame);
        }
        activeFrame += 2;
        idleFrame += 2;
    }

    for (s32 i = 0; i < 8; i++) {
        Sprite_Update(&tab->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_tab_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_tab* tab = task->data;

    for (s32 i = 0; i < 8; i++) {
        Sprite_RenderFrame(&tab->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_tab_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_tab* tab = task->data;

    for (s32 i = 0; i < 8; i++) {
        Sprite_Release(&tab->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_tab_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_tab_Init,
        .update     = MenuEquip_tab_Update,
        .render     = MenuEquip_tab_Render,
        .cleanup    = MenuEquip_tab_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_tab_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_tab_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_tab, NULL, 0, NULL, &args);
}
