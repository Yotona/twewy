#include "Otosu/OtosuGame.h"

typedef struct {
    s32 dataType;
    s32 seconds;
} OtosuGame_timer_Args;

static s32              OtosuGame_timer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_timer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_timer = {"Tsk_OtosuGame_timer", OtosuGame_timer_RunTask, sizeof(OtosuGame_timer)};

static const SpriteAnimation OtosuGame_timer_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = OtosuGame_timer_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
    .unk_18            = 2,
    .packIndex         = 6,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 0xA,
};

static SpriteFrameInfo* OtosuGame_timer_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 3);
}

static void OtosuGame_timer_Load(OtosuGame_timer* data, Sprite* sprite, s32 index) {
    SpriteAnimation anim = OtosuGame_timer_Anim;

    anim.owner    = data;
    anim.dataType = data->dataType;
    anim.posX     = (0x1F - index * 2) * 8;
    anim.posY     = 0x12;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_timer_Init(TaskPool* pool, Task* self, void* args) {
    OtosuGame_timer_Args* taskArgs = args;
    OtosuGame_timer*      data     = (OtosuGame_timer*)self->data;
    s32                   i;

    data->visible   = 1;
    data->alarmed   = 0;
    data->dataType  = taskArgs->dataType;
    data->countdown = taskArgs->seconds * 0x3C;

    for (i = 0; i < 3; i++) {
        OtosuGame_timer_Load(data, &data->digits[i], i);
    }

    return 1;
}

static s32 OtosuGame_timer_Update(TaskPool* pool, Task* self, void* args) {
    OtosuGame_timer* data = (OtosuGame_timer*)self->data;
    s32              before;
    s32              i;
    s32              j;
    s32              secs;

    before = data->countdown;
    if (before > 0) {
        data->countdown = before - 1;

        if (before / 60 != (before - 1) / 60 && before - 1 <= 0x4B0) {
            SndMgr_StartPlayingSE(0x33E);
        }
    }

    if (before >= 0x4B0 && data->countdown < 0x4B0) {
        void* pal = Data_GetPackEntryData(data->digits[0].resourceData, 5);

        for (i = 0; i < 3; i++) {
            data->digits[i].unk3C = (UnkSmallInternal*)pal;
        }

        data->alarmed = 1;
    } else {
        data->alarmed = 0;
    }

    secs = data->countdown / 60;
    for (j = 0; j < 3; j++) {
        Sprite_ChangeAnimation(&data->digits[j], data->digits[j].animData, (s16)(secs % 10 + 1), data->digits[j].cellTable);
        Sprite_Update(&data->digits[j]);
        secs /= 10;
    }

    return 1;
}

static s32 OtosuGame_timer_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_timer* data = task->data;
    s32              i;

    if (data->visible != 0) {
        for (i = 0; i < 3; i++) {
            Sprite_RenderFrame(&data->digits[i]);
        }
    }
    return 1;
}

static s32 OtosuGame_timer_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_timer* data = task->data;
    s32              i;

    for (i = 0; i < 3; i++) {
        Sprite_Release(&data->digits[i]);
    }
    return 1;
}

static s32 OtosuGame_timer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_timer_Init,
        .update     = OtosuGame_timer_Update,
        .render     = OtosuGame_timer_Render,
        .cleanup    = OtosuGame_timer_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_timer_CreateTask(TaskPool* pool, s32 dataType, s32 seconds) {
    OtosuGame_timer_Args args;

    args.dataType = dataType;
    args.seconds  = seconds;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_timer, NULL, 0, NULL, &args);
}

s32 OtosuGame_timer_GetCountdown(OtosuGame_timer* self) {
    return self->countdown;
}

s32 OtosuGame_timer_IsAlarmed(OtosuGame_timer* self) {
    return self->alarmed;
}
