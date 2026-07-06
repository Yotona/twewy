#include "Interface/Menu/Depart.h"

extern BinIdentifier data_ov043_020cad90;

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ s32           unk_40;
    /* 0x44 */ DepartObject* depart;
} DepartPanel; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ DepartObject* depart;
    /* 0x8 */ u16           unk_8;
} DepartPanelArgs;

static SpriteFrameInfo* func_ov043_020be59c(Sprite* arg0, s32 arg1, s32 arg2);
static s32              DepartPanel_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle data_ov043_020cae34 = {"Tsk_Depart_panel", DepartPanel_RunTask, sizeof(DepartPanel)};

static const SpriteAnimation data_ov043_020cae50 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = func_ov043_020be59c,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020cad90,
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

static SpriteFrameInfo* func_ov043_020be59c(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

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

static void func_ov043_020be638(DepartPanel* panel, Sprite* sprite, DepartPanelArgs* args) {
    DepartObject* depart = panel->depart;

    SpriteAnimation anim = data_ov043_020cae50;

    anim.dataType = args->dataType;
    anim.unk_2A   = 1;
    anim.unk_04   = args->unk_8 * 10 + data_ov043_020ccd00[depart->unk_0C].x;
    anim.unk_06   = args->unk_8 * 37 + data_ov043_020ccd00[depart->unk_0C].y;

    _Sprite_Load(sprite, &anim);
}

static s32 DepartPanel_Init(TaskPool* pool, Task* task, void* args) {
    DepartPanel*     panel     = task->data;
    DepartPanelArgs* panelArgs = args;

    panel->unk_40 = 1;
    panel->depart = panelArgs->depart;
    func_ov043_020be638(panel, &panel->sprite, panelArgs);
    return 1;
}

static s32 DepartPanel_Update(TaskPool* pool, Task* task, void* args) {
    DepartPanel* panel = task->data;

    Sprite_Update(&panel->sprite);
    return 1;
}

static s32 DepartPanel_Render(TaskPool* pool, Task* task, void* args) {
    DepartPanel* panel = task->data;

    Sprite_RenderFrame(&panel->sprite);
    return 1;
}

static s32 DepartPanel_Release(TaskPool* pool, Task* task, void* args) {
    DepartPanel* panel = task->data;

    Sprite_Release(&panel->sprite);
    return 1;
}

static s32 DepartPanel_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = DepartPanel_Init,
        .update     = DepartPanel_Update,
        .render     = DepartPanel_Render,
        .cleanup    = DepartPanel_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 DepartPanel_CreateTask(TaskPool* pool, s32 dataType, u16 arg2, DepartObject* depart) {
    DepartPanelArgs args;
    args.dataType = dataType;
    args.depart   = depart;
    args.unk_8    = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020cae34, NULL, 0, NULL, &args);
}
