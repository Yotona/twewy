#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x000 */ Sprite        sprites[9];
    /* 0x240 */ BOOL          visible[9];
    /* 0x264 */ ResultObject* owner;
} Result_numTime; // Size: 0x268

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ u32           seconds;
    /* 0x8 */ ResultObject* owner;
} Result_numTime_Args;

static SpriteFrameInfo* Result_numTime_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_numTime_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_numTime = {"Tsk_Result_numTime", Result_numTime_RunTask, sizeof(Result_numTime)};

static const SpriteAnimation Result_numTime_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_numTime_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[12],
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

static SpriteFrameInfo* Result_numTime_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_numTime_Load(Result_numTime* numTime, Sprite* sprites, Result_numTime_Args* args) {
    u16             digits[8];
    SpriteAnimation anim    = Result_numTime_Anim;
    u16             posX[8] = {206, 213, 213, 224, 231, 231, 242, 249};
    u32             seconds;
    u32             hours;
    u32             minutes;
    u32             hundredths;
    s16             i;

    anim.dataType = args->dataType;

    seconds = args->seconds;
    if (seconds >= 360000) {
        digits[0] = 9;
        digits[1] = 9;
        digits[2] = 61;
        digits[3] = 5;
        digits[4] = 9;
        digits[5] = 62;
        digits[6] = 9;
        digits[7] = 9;

        for (i = 0; i < 9; i++) {
            numTime->visible[i] = TRUE;
        }
    } else {
        hours      = seconds / 3600;
        minutes    = seconds % 3600 / 60;
        hundredths = seconds % 3600 % 60 * 100 / 60;

        if (hours > 99) {
            hours = 99;
        }

        digits[0] = hours / 10;
        digits[1] = hours % 10;
        digits[2] = 0x3D;
        digits[3] = minutes / 10;
        digits[4] = minutes % 10;
        digits[5] = 0x3E;
        digits[6] = hundredths / 10;
        digits[7] = hundredths % 10;

        if (digits[0] == 0) {
            numTime->visible[0] = FALSE;
        } else {
            numTime->visible[0] = TRUE;
        }

        for (i = 1; i < 9; i++) {
            numTime->visible[i] = TRUE;
        }
    }

    for (i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            anim.unk_2A = digits[i];
        } else {
            anim.unk_2A = digits[i] + 0xF;
        }
        anim.unk_04 = posX[i];
        anim.unk_06 = 8;
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.unk_2A = 1;
    anim.unk_04 = 0xD8;
    anim.unk_06 = 8;
    _Sprite_Load(&sprites[8], &anim);
}

static s32 Result_numTime_Init(TaskPool* pool, Task* task, void* args) {
    Result_numTime*      numTime     = task->data;
    Result_numTime_Args* numTimeArgs = args;

    Result_numTime_Load(numTime, numTime->sprites, numTimeArgs);
    numTime->owner = numTimeArgs->owner;
    return 1;
}

static s32 Result_numTime_Update(TaskPool* pool, Task* task, void* args) {
    Result_numTime* numTime = task->data;

    for (s32 i = 0; i < 9; i++) {
        Sprite_Update(&numTime->sprites[i]);
    }
    return 1;
}

static s32 Result_numTime_Render(TaskPool* pool, Task* task, void* args) {
    Result_numTime* numTime = task->data;

    for (s32 i = 0; i < 9; i++) {
        if (numTime->visible[i] != 0) {
            Sprite_RenderFrame(&numTime->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_numTime_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_numTime* numTime = task->data;

    for (s32 i = 0; i < 9; i++) {
        Sprite_Release(&numTime->sprites[i]);
    }
    return 1;
}

static s32 Result_numTime_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_numTime_Init,
        .update     = Result_numTime_Update,
        .render     = Result_numTime_Render,
        .cleanup    = Result_numTime_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_numTime_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_numTime_Args args;

    args.dataType = dataType;
    args.seconds  = owner->clearTime;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_numTime, NULL, 0, NULL, &args);
}
