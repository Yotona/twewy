#include "Interface/Save.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ void*  unk_44;
} SaveSlotCoverU; // Size: 0x48

typedef struct {
    /* 0x00 */ s32   unk_00;
    /* 0x08 */ void* unk_04;
    /* 0x0C */ u16   unk_08;
} SlotCoverUArgs;

const struct Point {
    s16 x;
    s16 y;
} data_ov043_020cb3d8[] = {};

SpriteFrameInfo* Save_SlotCoverU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
s32              Save_SlotCoverU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Save_slotCoverU = {"Tsk_Save_slotCoverU", Save_SlotCoverU_RunTask, sizeof(SaveSlotCoverU)};

static const SpriteAnimation data_ov043_020cb3f0 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = Save_SlotCoverU_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020caf88,
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

SpriteFrameInfo* Save_SlotCoverU_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
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

// Nonmatching
void Save_SlotCoverU_Load(SaveSlotCoverU* slotCoverU, Sprite* sprite, SlotCoverUArgs* slotCoverArgs) {
    SpriteAnimation anim = data_ov043_020cb3f0;
    anim.unk_2A          = 35;
    anim.dataType        = slotCoverArgs->unk_00;
    anim.unk_04          = data_ov043_020cb3d8[slotCoverArgs->unk_00].x;
    anim.unk_06          = data_ov043_020cb3d8[slotCoverArgs->unk_00].y;
    _Sprite_Load(sprite, &anim);
    slotCoverU->unk_40 = *(u8*)(slotCoverU->unk_44 + 0x168) >= slotCoverArgs->unk_08;
}

s32 Save_SlotCoverU_Init(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU    = task->data;
    SlotCoverUArgs* slotCoverArgs = args;

    slotCoverU->unk_44 = slotCoverArgs->unk_04;
    Save_SlotCoverU_Load(slotCoverU, &slotCoverU->sprite, slotCoverArgs);
    return 1;
}

s32 Save_SlotCoverU_Update(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU = task->data;

    Sprite_Update(&slotCoverU->sprite);
    return 1;
}

s32 Save_SlotCoverU_Render(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU = task->data;

    if (slotCoverU->unk_40 != 0) {
        Sprite_RenderFrame(&slotCoverU->sprite);
    }
    return 1;
}

s32 Save_SlotCoverU_Destroy(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU = task->data;

    Sprite_Release(&slotCoverU->sprite);
    return 1;
}

s32 Save_SlotCoverU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Save_SlotCoverU_Init,
        .update     = Save_SlotCoverU_Update,
        .render     = Save_SlotCoverU_Render,
        .cleanup    = Save_SlotCoverU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void Save_SlotCoverU_CreateTask(TaskPool* pool, s32 arg1, u16 arg2, void* arg3) {
    SlotCoverUArgs args;
    args.unk_00 = arg1;
    args.unk_04 = arg3;
    args.unk_08 = arg2;
    EasyTask_CreateTask(pool, &Tsk_Save_slotCoverU, NULL, 0, NULL, &args);
}
