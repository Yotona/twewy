#include "Interface/Menu/Depart.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ s32           unk_40;
    /* 0x44 */ DepartObject* depart;
    /* 0x48 */ s16           unk_48;
} DepartCur; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ DepartObject* depart;
} DepartCurArgs;

static SpriteFrameInfo* Depart_cur_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2);
static s32              Depart_cur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Depart_cur_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02     = 0,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Depart_cur_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020cad88,
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

static const TaskHandle Tsk_Depart_cur = {"Tsk_Depart_cur", Depart_cur_RunTask, sizeof(DepartCur)};

static SpriteFrameInfo* Depart_cur_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1: {
            info->unk_00 = 1;
            return info;
        } break;

        case 2: {
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info->unk_10 = 0x3C0000;
            return info;
        } break;
    }

    return NULL;
}

// Nonmatching
static void Depart_cur_Load(DepartCur* cur, Sprite* sprite, DepartCurArgs* args) {
    DepartObject*   depart = cur->depart;
    SpriteAnimation anim   = Depart_cur_Anim;

    cur->unk_40 = 1;
    cur->unk_48 = depart->unk_04;

    anim.dataType = args->dataType;
    anim.unk_2A   = 3;
    anim.unk_04   = depart->unk_04 * 10 + data_ov043_020ccd00[depart->unk_0C].x;
    anim.unk_06   = depart->unk_04 * 37 + data_ov043_020ccd00[depart->unk_0C].y;

    _Sprite_Load(sprite, &anim);
}

static s32 Depart_cur_Init(TaskPool* pool, Task* task, void* args) {
    DepartCur*     cur     = task->data;
    DepartCurArgs* curArgs = args;

    cur->depart = curArgs->depart;
    Depart_cur_Load(cur, &cur->sprite, curArgs);
    return 1;
}

// Nonmatching: Regalloc
static s32 Depart_cur_Update(TaskPool* pool, Task* task, void* args) {
    DepartCur*    cur    = task->data;
    DepartObject* depart = cur->depart;

    if (depart->unk_04 != cur->unk_48) {
        cur->sprite.posX = depart->unk_04 * 10 + data_ov043_020ccd00[depart->unk_0C].x;
        cur->sprite.posY = depart->unk_04 * 37 + data_ov043_020ccd00[depart->unk_0C].y;
        cur->unk_48      = depart->unk_04;
    }
    Sprite_Update(&cur->sprite);
    return 1;
}

static s32 Depart_cur_Render(TaskPool* pool, Task* task, void* args) {
    DepartCur* cur = task->data;

    if (cur->unk_40 != 0) {
        Sprite_RenderFrame(&cur->sprite);
    }
    return 1;
}

static s32 Depart_cur_Release(TaskPool* pool, Task* task, void* args) {
    DepartCur* cur = task->data;

    Sprite_Release(&cur->sprite);
    return 1;
}

static s32 Depart_cur_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Depart_cur_Init,
        .update     = Depart_cur_Update,
        .render     = Depart_cur_Render,
        .cleanup    = Depart_cur_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Depart_cur_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart) {
    DepartCurArgs args;
    args.dataType = dataType;
    args.depart   = depart;
    return EasyTask_CreateTask(pool, &Tsk_Depart_cur, NULL, 0, NULL, &args);
}
